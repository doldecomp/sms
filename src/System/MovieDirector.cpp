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
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/DummyStrings.hpp>

class TEndingString : public JDrama::TViewObj {
public:
	enum {
		ENDING_FADE_NONE     = 0,
		ENDING_FADE_IN       = 1,
		ENDING_FADE_WAIT     = 2,
		ENDING_FADE_OUT      = 3,
	};

	TEndingString(const char* name);

	virtual ~TEndingString() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void startFadeIn();

public:
	/* 0x10 */ u8 mFadeState;
	/* 0x14 */ int mWaitTimer;
	/* 0x18 */ J2DSetScreen* mScreen;
	/* 0x1C */ J2DPane* mRootPane;
};

// TODO: retail numbers this constructor's "ending_1.blo" literal @3092, the
// highest in the TU, i.e. the body was code-generated last. MWCC instead
// generates it on demand at rsetup's `new TEndingString(...)` (the literal
// lands between "EndingString" and "card save"), which shifts every later
// .rodata string by 0x10 and holds rsetup at 87.9%. Moving the definition
// below rsetup only moves the generation to the front of the file. The
// missing lever is whatever made retail's constructor a non-candidate for
// inlining at that call site.
TEndingString::TEndingString(const char* name)
    : JDrama::TViewObj(name)
    , mFadeState(ENDING_FADE_NONE)
    , mWaitTimer(0)
    , mScreen(nullptr)
    , mRootPane(nullptr)
{
	JKRArchive* arc = (JKRArchive*)JKRFileLoader::getVolume("endsave");
	mScreen         = new J2DSetScreen("ending_1.blo", arc);
	mRootPane       = mScreen->search('ROOT');
	mRootPane->setAlpha(0);
}

void TEndingString::startFadeIn()
{
	if (gpApplication.getMovie() == 16) {
		mScreen->search('tx_1')->show();
		mScreen->search('tx_2')->hide();
	} else if (gpApplication.getMovie() == 17) {
		mScreen->search('tx_1')->hide();
		mScreen->search('tx_2')->show();
	}

	mFadeState = ENDING_FADE_IN;
}

void TEndingString::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		switch (mFadeState) {
		case ENDING_FADE_NONE:
			break;
		case ENDING_FADE_IN: {
			u16 alpha = mRootPane->getAlpha();
			alpha += 8;
			if (alpha > 255) {
				mFadeState = ENDING_FADE_WAIT;
				mWaitTimer = 0;
				alpha      = 255;
			}
			mRootPane->setAlpha(alpha);
			break;
		}
		case ENDING_FADE_WAIT:
			mWaitTimer++;
			if (mWaitTimer > 225)
				mFadeState = ENDING_FADE_OUT;
			break;
		case ENDING_FADE_OUT: {
			s16 alpha = mRootPane->getAlpha();
			alpha -= 8;
			if (alpha < 0) {
				mFadeState = ENDING_FADE_NONE;
				alpha      = 0;
			}
			mRootPane->setAlpha(alpha);
			break;
		}
		}
	}

	// TODO: 8 bytes of frame short (retail 0x128, ours 0x120) and the
	// J2DOrthoGraph lands at 0x28 instead of 0x2c: one unexplained 4-byte
	// temp below the named object. Every instruction matches.
	if (cue & CUE_DRAW) {
		switch (mFadeState) {
		case ENDING_FADE_IN:
		case ENDING_FADE_WAIT:
		case ENDING_FADE_OUT: {
			J2DOrthoGraph ortho(graphics->getViewport());
			ortho.setup2D();
			mScreen->draw(0, 0, &ortho);
			break;
		}
		}
	}
}

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
    , unk34(nullptr)
    , unk38(nullptr)
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
	unk20->mFlags = 1;

	SMSRumbleMgr->reset();

	OSCreateThread(&gSetupThread, &setupThreadFunc, this,
	               gpSetupThreadStack + 0x10000, 0x10000, 0x11, 0);
	OSResumeThread(&gSetupThread);
}

int TMovieDirector::rsetup()
{
	void* subtitleArcBlob
	    = SMSLoadArchive("/data/subtitle.arc", nullptr, 0, nullptr);
	JKRMemArchive* subtitleArc = new JKRMemArchive;
	subtitleArc->mountFixed(subtitleArcBlob, MBF_0);

	switch (gpApplication.getMovie()) {
	case 16:
	case 17:
	case 18:
	case 19: {
		void* arcBlob
		    = SMSLoadArchive("/data/endsave.arc", nullptr, 0, nullptr);
		JKRMemArchive* arc = new JKRMemArchive;
		arc->mountFixed(arcBlob, MBF_0);
		break;
	}
	}

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* rootViewObjs
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("root View Objs");
	unk10 = rootViewObjs;

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group2d
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("Group 2D");
	rootViewObjs->getChildren().push_back(group2d);

	DVDChangeDir("/data");

	const char* movie = "EX128x144_q0.thp";

	{
		const char* movieName = getStreamMovieName(gpApplication.getMovie());
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

	switch (gpApplication.getMovie()) {
	case 16:
	case 17:
		unk34 = new TEndingString("EndingString");
		group2d->getChildren().push_back(unk34);
		// fallthrough
	case 18:
	case 19:
		unk24 = new TCardSave("card save", true);
		unk24->initData(unk20);
		group2d->getChildren().push_back(unk24);
		break;
	}

	JDrama::TDStageDisp* stageDisp = new JDrama::TDStageDisp;
	unk14->getChildren().push_back(stageDisp);

	JDrama::TRect rect(0, 0, SMSGetTitleRenderWidth(),
	                   SMSGetTitleRenderHeight());
	stageDisp->getEfbCtrlDisp()->TEfbCtrl::setSrcRect(rect);

	JDrama::TOrthoProj* camera = new JDrama::TOrthoProj(
	    -1.0f, 1.0f, 0.0f, 0.0f, rect.getWidth(), rect.getHeight());
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

	// TODO: Huh? TBox2 or something?
	thpRender->setParams(SMSGetGameRenderWidth() - videoInfo.xSize / 2,
	                     SMSGetGameRenderHeight() - videoInfo.ySize / 2,
	                     videoInfo.xSize, videoInfo.ySize);

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
	unk20->offFlag(0x1);
}

u32 TMovieDirector::decideNextMode(s32* param_1)
{

	if (gpApplication.getMovie() != 14) {
		if (!(gpApplication.getMovie() == 15 || gpApplication.getMovie() == 16
		      || gpApplication.getMovie() == 17)) {
			int flag = gpApplication.getMovie() + 0x10391;
			TFlagManager::getInstance()->setBool(true, flag);
		}
	}

	u32 nextMode = 1;
	if (unk20->isSomethingPushed()) {
		nextMode = 4;
	} else if (gpApplication.getMovie() == 2) {
		gpApplication.setMovie(18);
		nextMode = 6;
	} else if (gpApplication.getMovie() == 0x12) {
		*param_1 = 4;
	} else if (gpApplication.getMovie() == 3) {
		gpApplication.setMovie(19);
		nextMode = 6;
	} else if (gpApplication.getMovie() == 0x13) {
		*param_1 = 4;
	} else if (gpApplication.getMovie() == 0xc) {
		TGameSequence& nextArea = gpApplication.mNextArea;
		nextArea.set(15, 0, 0);
		nextMode = 5;
	} else if (gpApplication.getMovie() == 0xe) {
		if (!TFlagManager::getInstance()->getShineFlag(0x77))
			TFlagManager::getInstance()->setShineFlag(0x77);

		gpApplication.setMovie(15);
		nextMode = 6;
	} else if (gpApplication.getMovie() == 15) {
		u8 movie
		    = TFlagManager::getInstance()->getFlag(0x40000) < 120 ? 16 : 17;
		gpApplication.setMovie(movie);

		nextMode = 6;
	} else if (gpApplication.getMovie() == 16
	           || gpApplication.getMovie() == 17) {
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

		u32 errc;
		OSJoinThread(&gSetupThread, &errc);
		if (errc)
			return 5;

		unk30.on(0x1);

		gpMSound->initSound();
		if (gpApplication.getMovie() == 9) {
			gpApplication.mFader->startWipe(12, 0.0f, 0.0f);
			unk18 = false;
		} else {
			gpApplication.mFader->startWipe(14, 1.0f, 0.0f);
			gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 255));
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
		graphics.unk2 = 0;
		unk10->testPerform(CUE_MOVE, &graphics);
		graphics.unk2 = 0;
		unk10->testPerform(CUE_MOVE, &graphics);
		graphics.unk2 = 0;
		unk10->testPerform(CUE_MOVE | CUE_CALC_ANIM, &graphics);
		unk14->testPerform(CUE_DRAW, &graphics);
	} else {
		JDrama::TDirector::direct();
	}

	s32 nextState = unk1C;
	switch (unk1C) {
	case STATE_FADE_IN:
		if (TFlagManager::getInstance()->getBool(gpApplication.getMovie()
		                                         + 0x10391)
		    && unk20->checkFrameMeaning(0x61)) {
			nextState = STATE_FADE_OUT;
		} else if (gpApplication.mFader->isFullyFadedIn()) {
			nextState = STATE_PLAYING;
		}
		break;

	case STATE_PLAYING:
		if (TFlagManager::getInstance()->getBool(gpApplication.getMovie()
		                                         + 0x10391)
		    && unk20->checkFrameMeaning(0x61)) {
			nextState = STATE_FADE_OUT;
		} else if (THPPlayerGetState() == 5) {
			nextState = STATE_FADE_OUT;
		} else if (THPPlayerGetState() == 3) {
			desiredAppState = decideNextMode(&nextState);
		}
		break;

	case STATE_FADE_OUT:
		if (gpApplication.mFader->isFullyFadedOut())
			desiredAppState = decideNextMode(&nextState);
		break;

	case STATE_SAVE_TO_TITLE:
		switch (unk24->getNextState()) {
		case 0:
		case 1:
			gpApplication.mFader->setFadeStatus(
			    TSMSFader::FADE_STATUS_FULLY_FADED_OUT);
			desiredAppState = TApplication::APP_STATE_DONE;
			break;
		}
		break;

	case STATE_SAVE_AND_CONTINUE:
		switch (unk24->getNextState()) {
		case 1:
			gpApplication.mFader->setFadeStatus(
			    TSMSFader::FADE_STATUS_FULLY_FADED_OUT);
			desiredAppState = TApplication::APP_STATE_MOVIE;
			break;
		case 0:
			gpApplication.mFader->setFadeStatus(
			    TSMSFader::FADE_STATUS_FULLY_FADED_OUT);
			if (gpApplication.getMovie() == 19) {
				gpApplication.setMovie(13);
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
			gpApplication.mFader->startWipe(15, 0.3f, 0.0f);
			gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 255));
			unk24->init(gpApplication.getMovie() == 17 ? 8 : 0);
			break;

		case STATE_SAVE_AND_CONTINUE:
			THPPlayerStop();
			unk28->unkC.on(CUE_DRAW | CUE_MOVE);
			unk2C->unkC.on(CUE_DRAW | CUE_MOVE);
			gpApplication.mFader->startWipe(14, 0.3f, 0.0f);
			gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 255));
			unk24->init(9);
			break;

		case STATE_FADE_OUT:
			if (unk20->isSomethingPushed()) {
				gpApplication.mFader->startWipe(4, 1.0f, 0.0f);
			} else {
				gpApplication.mFader->startWipe(15, 1.0f, 0.0f);
				gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 255));
			}

			if (gpApplication.getMovie() == 9) {
				MSound* sound = gpMSound;
				sound->fadeOutAllSound(SMSGetVSyncTimesPerSec());
			}
			THPPlayerSetVolume(0, 1000);
			break;
		}
		unk1C = nextState;
	}

	return desiredAppState;
}
