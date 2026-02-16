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

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

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
    , unk1C(0)
    , unk20(nullptr)
    , unk24(nullptr)
    , unk30(0)
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

	if (gpApplication.getMovie() < 20) {
		if (gpApplication.getMovie() < 16) {
			(void)gpApplication.getMovie();
		} else {
			void* arcBlob
			    = SMSLoadArchive("/data/endsave.arc", nullptr, 0, nullptr);
			JKRMemArchive* arc = new JKRMemArchive;
			arc->mountFixed(arcBlob, MBF_0);
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

	if (gpApplication.getMovie() < 20) {
		if (gpApplication.getMovie() < 16) {
			(void)gpApplication.getMovie();
		} else {
			unk24 = new TCardSave;
			unk24->initData(unk20);
			group2d->getChildren().push_back(unk24);
		}
	}

	JDrama::TDStageDisp* stageDisp = new JDrama::TDStageDisp;
	unk14->getChildren().push_back(stageDisp);

	JDrama::TRect rect(0, 0, SMSGetTitleRenderWidth(),
	                   SMSGetTitleRenderHeight());
	stageDisp->getEfbCtrlDisp()->TEfbCtrl::setSrcRect(rect);

	JDrama::TOrthoProj* camera
	    = new JDrama::TOrthoProj(0.0f, 0.0f, rect.getWidth(), rect.getHeight());
	group2d->getChildren().push_back(camera);

	JDrama::TScreen* screen = new JDrama::TScreen(rect, "Screen 2D");
	stageDisp->getUnk14()->getChildren().push_back(screen);
	screen->assignCamera(camera);
	screen->assignViewObj(group2d);

	THPPlayerInit();
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

	gpMSound->stopAllSound();
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
			if (gpMSound->gateCheck(0x2972))
				MSoundSESystem::MSoundSE::startSoundSystemSE(0x2972, 0, nullptr,
				                                             0);
			break;
		case 2:
			unk18 = true;
			THPPlayerPlay();
			break;
		}
	}

	SMSRumbleMgr->update();

	if (unk1C == 3 || unk1C == 4) {
		JDrama::TGraphics graphics;
		graphics.unk2 = 1;
		unk10->testPerform(1, &graphics);
		graphics.unk2 = 0;
		unk10->testPerform(1, &graphics);
		graphics.unk2 = 0;
		unk10->testPerform(1, &graphics);
		graphics.unk2 = 0;
		unk10->testPerform(3, &graphics);
		unk14->testPerform(8, &graphics);
	} else {
		JDrama::TDirector::direct();
	}

	s32 nextState = unk1C;
	switch (unk1C) {
	case 0:
		if (TFlagManager::getInstance()->getBool(gpApplication.getMovie()
		                                         + 0x10391)
		    && unk20->checkFrameMeaning(0x61)) {
			nextState = 2;
		} else if (gpApplication.mFader->isFullyFadedIn()) {
			nextState = 1;
		}
		break;

	case 1:
		if (TFlagManager::getInstance()->getBool(gpApplication.getMovie()
		                                         + 0x10391)
		    && unk20->checkFrameMeaning(0x61)) {
			nextState = 2;
		} else if (THPPlayerGetState() == 5) {
			nextState = 2;
		} else if (THPPlayerGetState() == 3) {
			desiredAppState = decideNextMode(&nextState);
		}
		break;

	case 2:
		if (gpApplication.mFader->isFullyFadedIn())
			desiredAppState = decideNextMode(&nextState);
		break;

	case 3:
		switch (unk24->getNextState()) {
		case 0:
		case 1:
			gpApplication.mFader->setFadeStatus(
			    TSMSFader::FADE_STATUS_FULLY_FADED_OUT);
			desiredAppState = TApplication::APP_STATE_DONE;
			break;
		}
		break;

	case 4:
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

	if (unk20->isSomethingPushed() && gpCardManager->getLastStatus() != -1
	    && !unk30.check(0x2)) {
		unk30.on(0x2);
		nextState = 2;
		unk28->unkC.on(0x9);
		unk2C->unkC.on(0x9);
		THPPlayerPause();
	}

	if (nextState != unk1C) {
		switch (nextState) {
		case 3:
			THPPlayerStop();
			unk28->unkC.on(0x9);
			unk2C->unkC.on(0x9);
			gpApplication.mFader->startWipe(15, 0.3f, 0.0f);
			gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 255));
			{
				unk24->init(gpApplication.getMovie() == 17 ? 8 : 0);
			}
			break;

		case 4:
			THPPlayerStop();
			unk28->unkC.on(0x9);
			unk2C->unkC.on(0x9);
			gpApplication.mFader->startWipe(14, 0.3f, 0.0f);
			gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 255));
			unk24->init(9);
			break;

		case 2:
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
