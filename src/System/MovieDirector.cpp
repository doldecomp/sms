#include <System/MovieDirector.hpp>
#include <THPPlayer/THPPlayer.h>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JDrama/JDRDStage.hpp>
#include <JSystem/JDrama/JDREfbCtrl.hpp>
#include <JSystem/JDrama/JDRScreen.hpp>
#include <System/Resolution.hpp>
#include <System/Application.hpp>
#include <System/MarioGamePad.hpp>
#include <System/THPRender.hpp>
#include <System/CardManager.hpp>
#include <System/FlagManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <GC2D/MovieSubtitle.hpp>
#include <GC2D/MovieRumble.hpp>
#include <GC2D/CardSave.hpp>
#include <GC2D/ScrnFader.hpp>
#include <GC2D/hx_wiper.h>
#ifdef VERSION_GMSP01
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <dolphin/vi.h>
#endif
#include <version.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/DummyStrings.hpp>

#ifdef VERSION_GMSP01
#pragma dont_inline on
TEndingString::TEndingString(const char* name)
    : JDrama::TViewObj(name)
    , mState(STATE_HIDDEN)
    , mTimer(0)
    , mScreen(nullptr)
    , mRootPane(nullptr)
{
	JKRArchive* archive  = (JKRArchive*)JKRFileLoader::getVolume("endsave");
	J2DSetScreen* screen = new J2DSetScreen("ending_1.blo", archive);
	mScreen              = screen;
	J2DPane* root        = mScreen->search('ROOT');
	mRootPane            = root;
	mRootPane->setAlpha(0);
}
#pragma dont_inline off

void TEndingString::startFadeIn()
{
	if (SMSGetApplication()->getMovie() == 16) {
		mScreen->search('tx_1')->show();
		mScreen->search('tx_2')->hide();
	} else if (SMSGetApplication()->getMovie() == 17) {
		mScreen->search('tx_1')->hide();
		mScreen->search('tx_2')->show();
	}
	mState = STATE_FADE_IN;
}

void TEndingString::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		switch (mState) {
		case STATE_HIDDEN:
			break;
		case STATE_FADE_IN: {
			u16 alpha = mRootPane->getAlpha();
			alpha += 8;
			if (alpha > 0xFF) {
				mState = STATE_SHOWN;
				mTimer = 0;
				alpha  = 0xFF;
			}
			mRootPane->setAlpha(alpha);
		} break;
		case STATE_SHOWN:
			++mTimer;
			if (mTimer > 225)
				mState = STATE_FADE_OUT;
			break;
		case STATE_FADE_OUT: {
			s16 alpha = mRootPane->getAlpha();
			alpha -= 8;
			if (alpha < 0) {
				mState = STATE_HIDDEN;
				alpha  = 0;
			}
			mRootPane->setAlpha(alpha);
		} break;
		}
	}

	if (cue & CUE_DRAW) {
		switch (mState) {
		case STATE_FADE_IN:
		case STATE_SHOWN:
		case STATE_FADE_OUT: {
			J2DOrthoGraph graph(graphics->getViewport());
			graph.setup2D();
			mScreen->draw(0, 0, &graph);
		} break;
		}
	}
}
#endif

const char* TMovieDirector::getStreamMovieName(u32 idx)
{
	if (idx >= 20)
		return nullptr;

	static const char* movies[] = {
		"stolenpeach.thp", "openingA.thp",     "NozuruA.thp",  "openingBA.thp",
		"kagemario.thp",   "stolennozuru.thp", "pakkun.thp",   "MechaKuppa.thp",
		"KuppaJr.thp",     "Entrance.thp",     "demogeso.thp", "bath.thp",
		"autodemoA.thp",   "openingBC.thp",    "epilogue.thp", "staffroll.thp",
		"omakeA.thp",      "omakeB.thp",       "NozuruB.thp",  "openingBB.thp",
	};

	return movies[idx];
}

TMovieDirector::TMovieDirector()
    : unk18(1)
    , unk1C(STATE_FADE_IN)
    , unk20(nullptr)
    , unk24(nullptr)
    , unk30(0)
#ifdef VERSION_GMSP01
    , mEndingString(nullptr)
    , mEndingTimer(0)
#endif
{
}

void* TMovieDirector::setupThreadFunc(void* self)
{
	((TMovieDirector*)self)->rsetup();
}

extern OSThread gSetupThread;
extern u8* gpSetupThreadStack;

void TMovieDirector::setup(JDrama::TDisplay* param_1, TMarioGamePad* param_2)
{
	unk14         = new JDrama::TDStageGroup(param_1);
	unk20         = param_2;
	unk20->mFlags = TMarioGamePad::PAD_FLAG_MENU_INPUT;

	SMSRumbleMgr->reset();

	OSCreateThread(&gSetupThread, &setupThreadFunc, this,
	               gpSetupThreadStack + 0x10000, 0x10000, 0x11, 0);
	OSResumeThread(&gSetupThread);
}

int TMovieDirector::rsetup()
{
#ifdef VERSION_GMSP01
	static const char* subtitleNames[] = {
		"/data/subtitle_en.arc", "/data/subtitle_ge.arc",
		"/data/subtitle_fr.arc", "/data/subtitle_sp.arc",
		"/data/subtitle_it.arc",
	};
	static const char* endsaveNames[] = {
		"/data/endsave_en.arc", "/data/endsave_ge.arc", "/data/endsave_fr.arc",
		"/data/endsave_sp.arc", "/data/endsave_it.arc",
	};

	void* subtitleArcBlob = SMSLoadArchive(
	    subtitleNames[TFlagManager::getInstance()->getFlag(0xA0001)], nullptr,
	    0, nullptr);
#else
	void* subtitleArcBlob
	    = SMSLoadArchive("/data/subtitle.arc", nullptr, 0, nullptr);
#endif
	JKRMemArchive* subtitleArc = new JKRMemArchive;
	subtitleArc->mountFixed(subtitleArcBlob, MBF_0);

	switch (SMSGetApplication()->getMovie()) {
	case 16:
	case 17:
	case 18:
	case 19: {
#ifdef VERSION_GMSP01
		void* arcBlob = SMSLoadArchive(
		    endsaveNames[TFlagManager::getInstance()->getFlag(0xA0001)],
		    nullptr, 0, nullptr);
#else
		void* arcBlob
		    = SMSLoadArchive("/data/endsave.arc", nullptr, 0, nullptr);
#endif
		JKRMemArchive* arc = new JKRMemArchive;
		arc->mountFixed(arcBlob, MBF_0);
	} break;
	}
#ifdef VERSION_GMSP01
	load2DResource2Aram();
#endif

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* rootViewObjs
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("root View Objs");
	unk10 = rootViewObjs;

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group2d
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("Group 2D");
	rootViewObjs->getChildren().push_back(group2d);

	DVDChangeDir("/data");

	const char* movie = "EX128x144_q0.thp";

	{
		const char* movieName
		    = getStreamMovieName(SMSGetApplication()->getMovie());
		if (movieName != nullptr)
			if (DVDConvertPathToEntrynum((char*)movieName) != -1)
				movie = movieName;
	}

	TTHPRender* thpRender = new TTHPRender;
	group2d->getChildren().push_back(thpRender);
	unk28 = new TMovieSubTitle(thpRender);
	unk28->setupResource(movie, subtitleArc);
	group2d->getChildren().push_back(unk28);
	unk2C = new TMovieRumble(thpRender);
	unk2C->init(movie);
	group2d->getChildren().push_back(unk2C);

	switch (SMSGetApplication()->getMovie()) {
	case 16:
	case 17:
#ifdef VERSION_GMSP01
		mEndingString = new TEndingString("EndingString");
		group2d->getChildren().push_back(mEndingString);
#endif
		// FALLTHROUGH
	case 18:
	case 19:
		unk24 = new TCardSave("card save", true);
		unk24->initData(unk20);
		group2d->getChildren().push_back(unk24);
		break;
	}

	JDrama::TDStageDisp* stageDisp = new JDrama::TDStageDisp("<DStageDisp>");
	unk14->getChildren().push_back(stageDisp);

	JDrama::TRect rect(0, 0, SMSGetTitleRenderWidth(),
	                   SMSGetTitleRenderHeight());
	stageDisp->getEfbCtrlDisp()->TEfbCtrl::setSrcRect(rect);

	JDrama::TOrthoProj* camera = new JDrama::TOrthoProj(
	    -1.0f, 1.0f, 0.0f, rect.getHeight(), 0.0f, rect.getWidth());
	group2d->getChildren().push_back(camera);

	JDrama::TScreen* screen = new JDrama::TScreen(rect, "Screen 2D");
	stageDisp->getUnk14()->getChildren().push_back(screen);
	screen->assignCamera(camera);
	screen->assignViewObj(group2d);

	THPPlayerInit(0);
	THPPlayerOpen(movie, 0);
	u32 mem = THPPlayerCalcNeedMemory();
	THPPlayerSetBuffer(new (0x20) u8[mem]);

	int audioTrack = 0;

	THPAudioInfo audioInfo;
	if (THPPlayerGetAudioInfo(&audioInfo) && audioInfo.sndNumTracks >= 2
	    && TFlagManager::getInstance()->getFlag(0xA0000) == 2) {
		audioTrack = 1;
	}

	if (!THPPlayerPrepare(0, 0, audioTrack))
		return 1;

	THPVideoInfo videoInfo;
	THPPlayerGetVideoInfo(&videoInfo);

	thpRender->setPos(JGeometry::TVec2<u32>(
	    (SMSGetGameRenderWidth() - videoInfo.xSize) / 2,
	    (SMSGetGameRenderHeight() - videoInfo.ySize) / 2));
	thpRender->setSize(JDrama::TSize(videoInfo.xSize, videoInfo.ySize));

	DVDChangeDir("/");

	return 0;
}

TMovieDirector::~TMovieDirector()
{
	if (JKRMemArchive* arc
	    = (JKRMemArchive*)JKRFileLoader::getVolume("endsave"))
		arc->unmountFixed();

	if (JKRMemArchive* arc
	    = (JKRMemArchive*)JKRFileLoader::getVolume("subtitle"))
		arc->unmountFixed();

	SMSGetMSound()->stopAllSound();
	THPPlayerStop();
	THPPlayerClose();
	THPPlayerQuit();
	SMSRumbleMgr->reset();
	unk20->offFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
}

u32 TMovieDirector::decideNextMode(s32* param_1)
{

	if (SMSGetApplication()->getMovie() != 14) {
		if (!(SMSGetApplication()->getMovie() == 15
		      || SMSGetApplication()->getMovie() == 16
		      || SMSGetApplication()->getMovie() == 17)) {
			int flag = SMSGetApplication()->getMovie() + 0x10391;
			TFlagManager::getInstance()->setBool(true, flag);
		}
	}

	u32 nextMode = 1;
	if (unk20->isSomethingPushed()) {
		nextMode = 4;
	} else if (SMSGetApplication()->getMovie() == 2) {
		SMSGetApplication()->setMovie(18);
		nextMode = 6;
	} else if (SMSGetApplication()->getMovie() == 0x12) {
		*param_1 = 4;
	} else if (SMSGetApplication()->getMovie() == 3) {
		SMSGetApplication()->setMovie(19);
		nextMode = 6;
	} else if (SMSGetApplication()->getMovie() == 0x13) {
		*param_1 = 4;
	} else if (SMSGetApplication()->getMovie() == 0xc) {
		TGameSequence& nextArea = SMSGetApplication()->mNextArea;
		nextArea.set(15, 0, 0);
		nextMode = 5;
	} else if (SMSGetApplication()->getMovie() == 0xe) {
		if (!TFlagManager::getInstance()->getShineFlag(0x77))
			TFlagManager::getInstance()->setShineFlag(0x77);

		SMSGetApplication()->setMovie(15);
		nextMode = 6;
	} else if (SMSGetApplication()->getMovie() == 15) {
		u8 movie
		    = TFlagManager::getInstance()->getFlag(0x40000) < 120 ? 16 : 17;
		SMSGetApplication()->setMovie(movie);

		nextMode = 6;
	} else if (SMSGetApplication()->getMovie() == 16
	           || SMSGetApplication()->getMovie() == 17) {
		*param_1 = 3;
	} else {
		nextMode = 5;
	}

	return nextMode;
}

int TMovieDirector::direct()
{
	if (!unk30.check(0x1)) {
		if (!OSIsThreadTerminated(&gSetupThread))
			return 0;

		void* errc;
		OSJoinThread(&gSetupThread, &errc);
		if (errc)
			return 5;

		unk30.on(0x1);

		gpMSound->initSound();
#ifdef VERSION_GMSP01
		if (SMSGetApplication()->getMovie() == 16)
			mEndingString->startFadeIn();
#endif
		if (SMSGetApplication()->getMovie() == 9) {
			SMSGetApplication()->getFader()->startWipe(12, 0.0f, 0.0f);
			unk18 = false;
		} else {
			SMSGetApplication()->getFader()->startWipe(14, 1.0f, 0.0f);
			SMSGetApplication()->getFader()->setColor(
			    JUtility::TColor(0, 0, 0, 255));
			THPPlayerPlay();
		}
	}

	int desiredAppState = TApplication::APP_STATE_DEFAULT;

	if (!unk18) {
		switch (Hx_MovieStartSyncEx()) {
		case 1:
			SMSGetMSound()->startSoundSystemSE(MSD_SE_TITLE_M, 0, nullptr, 0);
			break;
		case 2:
			unk18 = true;
			THPPlayerPlay();
			break;
		}
	}

	SMSRumbleMgr->update();

	if (unk1C == STATE_SAVE_TO_TITLE || unk1C == STATE_SAVE_AND_CONTINUE) {
		JDrama::TGraphics graphics;
		graphics.unk2 = 1;
		unk10->testPerform(CUE_MOVE, &graphics);
#ifdef VERSION_GMSP01
		if (VIGetTvFormat() != VI_PAL) {
			graphics.unk2 = 0;
			unk10->testPerform(CUE_MOVE, &graphics);
			graphics.unk2 = 0;
			unk10->testPerform(CUE_MOVE, &graphics);
		}
#else
		graphics.unk2 = 0;
		unk10->testPerform(CUE_MOVE, &graphics);
		graphics.unk2 = 0;
		unk10->testPerform(CUE_MOVE, &graphics);
#endif
		graphics.unk2 = 0;
		unk10->testPerform(CUE_MOVE | CUE_CALC_ANIM, &graphics);
		unk14->testPerform(CUE_DRAW, &graphics);
	} else {
		JDrama::TDirector::direct();
	}

	s32 nextState = unk1C;
	switch (unk1C) {
	case STATE_FADE_IN:
		if (TFlagManager::getInstance()->getBool(SMSGetApplication()->getMovie()
		                                         + 0x10391)
		    && unk20->checkFrameMeaning(TMarioGamePad::MEANING_START
		                                | TMarioGamePad::MEANING_MENU_A
		                                | TMarioGamePad::MEANING_MENU_B)) {
			nextState = STATE_FADE_OUT;
		} else if (SMSGetApplication()->getFader()->isFullyFadedIn()) {
			nextState = STATE_PLAYING;
		}
		break;

	case STATE_PLAYING:
		if (TFlagManager::getInstance()->getBool(SMSGetApplication()->getMovie()
		                                         + 0x10391)
		    && unk20->checkFrameMeaning(TMarioGamePad::MEANING_START
		                                | TMarioGamePad::MEANING_MENU_A
		                                | TMarioGamePad::MEANING_MENU_B)) {
			nextState = STATE_FADE_OUT;
		} else if (THPPlayerGetState() == 5) {
			nextState = STATE_FADE_OUT;
		} else if (THPPlayerGetState() == 3) {
			desiredAppState = decideNextMode(&nextState);
		}
#ifdef VERSION_GMSP01
		if (mEndingTimer < 300)
			++mEndingTimer;
		if (SMSGetApplication()->getMovie() == 17 && mEndingTimer == 30)
			mEndingString->startFadeIn();
		if (SMSGetApplication()->getMovie() == 17 && mEndingTimer == 220)
			mEndingString->startFadeOut();
#endif
		break;

	case STATE_FADE_OUT:
		if (SMSGetApplication()->getFader()->isFullyFadedOut())
			desiredAppState = decideNextMode(&nextState);
		break;

	case STATE_SAVE_TO_TITLE:
		switch (unk24->getNextState()) {
		case 0:
		case 1:
			SMSGetApplication()->getFader()->setFadeStatus(
			    TSMSFader::FADE_STATUS_FULLY_FADED_OUT);
			desiredAppState = TApplication::APP_STATE_DONE;
			break;
		}
		break;

	case STATE_SAVE_AND_CONTINUE:
		switch (unk24->getNextState()) {
		case 1:
			SMSGetApplication()->getFader()->setFadeStatus(
			    TSMSFader::FADE_STATUS_FULLY_FADED_OUT);
			desiredAppState = TApplication::APP_STATE_MOVIE;
			break;
		case 0:
			SMSGetApplication()->getFader()->setFadeStatus(
			    TSMSFader::FADE_STATUS_FULLY_FADED_OUT);
			if (SMSGetApplication()->getMovie() == 19) {
				SMSGetApplication()->setMovie(13);
				desiredAppState = TApplication::APP_STATE_MOVIE;
			} else {
				desiredAppState = TApplication::APP_STATE_GAMEPLAY;
			}
			break;
		}
		break;
	}

	if (unk20->isSomethingPushed()
	    && gpCardManager->getLastStatus() != CARD_RESULT_BUSY
	    && !unk30.check(0x2)) {
		unk30.on(0x2);
		nextState = STATE_FADE_OUT;
		unk28->unkC.on(CUE_DRAW | CUE_MOVE);
		unk2C->unkC.on(CUE_DRAW | CUE_MOVE);
		THPPlayerPause();
	}

	if (nextState != unk1C) {
		switch (nextState) {
		case STATE_SAVE_TO_TITLE:
			THPPlayerStop();
			unk28->unkC.on(CUE_DRAW | CUE_MOVE);
			unk2C->unkC.on(CUE_DRAW | CUE_MOVE);
			SMSGetApplication()->getFader()->startWipe(14, 0.3f, 0.0f);
			SMSGetApplication()->getFader()->setColor(
			    JUtility::TColor(0, 0, 0, 255));
			unk24->init(SMSGetApplication()->getMovie() == 17 ? 8 : 0);
			break;

		case STATE_SAVE_AND_CONTINUE:
			THPPlayerStop();
			unk28->unkC.on(CUE_DRAW | CUE_MOVE);
			unk2C->unkC.on(CUE_DRAW | CUE_MOVE);
			SMSGetApplication()->getFader()->startWipe(14, 0.3f, 0.0f);
			SMSGetApplication()->getFader()->setColor(
			    JUtility::TColor(0, 0, 0, 255));
			unk24->init(9);
			break;

		case STATE_FADE_OUT:
			if (unk20->isSomethingPushed()) {
				SMSGetApplication()->getFader()->startWipe(4, 1.0f, 0.0f);
			} else {
				SMSGetApplication()->getFader()->startWipe(15, 1.0f, 0.0f);
				SMSGetApplication()->getFader()->setColor(
				    JUtility::TColor(0, 0, 0, 255));
			}

			if (SMSGetApplication()->getMovie() == 9) {
				SMSGetMSound()->fadeOutAllSound(SMSGetVSyncTimesPerSec());
			}
			THPPlayerSetVolume(0, 1000);
			break;
		}
		unk1C = nextState;
	}

	return desiredAppState;
}
