#include <System/Application.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <dolphin/gx.h>
#include <dolphin/card.h>
#include <THPPlayer/THPPlayer.h>
#include <JSystem/JKernel/JKRDvdRipper.hpp>
#include <JSystem/JKernel/JKRDvdAramRipper.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JKernel/JKRAram.hpp>
#include <JSystem/JKernel/JKRAramStream.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JUtility/JUTGamePad.hpp>
#include <JSystem/JUtility/JUTRomFont.hpp>
#include <JSystem/JUtility/JUTDirectPrint.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <JSystem/ResTIMG.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JDrama/JDRDisplay.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRVideo.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/J2D/J2DPrint.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <GC2D/ScrnFader.hpp>
#include <System/MarDirector.hpp>
#include <System/RenderModeObj.hpp>
#include <System/TexCache.hpp>
#include <System/DrawSyncManager.hpp>
#include <System/MarioGamePad.hpp>
#include <System/Resolution.hpp>
#include <System/FlagManager.hpp>
#include <System/TimeRec.hpp>
#include <System/ProcessMeter.hpp>
#include <System/CardManager.hpp>
#include <System/ScenarioArchiveName.hpp>
#include <System/MarNameRefGen.hpp>
#include <System/StageUtil.hpp>
#include <System/GCLogoDir.hpp>
#include <System/MovieDirector.hpp>
#include <System/SelectDir.hpp>
#include <System/MenuDir.hpp>
#include <MSound/MSound.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

TMarDirector* gpMarDirector;
MSound* gpMSound;

OSThread gSetupThread;
u8* gpSetupThreadStack;

TCardManager* gpCardManager;
JUTFont* gpSystemFont;
JUTRomFont* gpRomFont;

static int sGameInit;
static void* arcBufNLogo;
static void* arcBufMario;
static void* arcBufCmn;
static void* bufStageArcBin;
static void* spGameHeapBlock;

TARAMBlock gArBkConsole;
TARAMBlock gArBkGuide;

extern "C" void ReInitializeGX();

f32 SMSGetVSyncTimesPerSec()
{
	f32 result = 60.0f;
	switch (VIGetTvFormat()) {
	case VI_MPAL:
	case VI_NTSC:
	case VI_EURGB60:
		result = 60.0f;
		break;
	case VI_PAL:
		result = 50.0f;
		break;
	}
	return result / 2.0f;
}

f32 SMSGetAnmFrameRate() { return 60.0f / SMSGetVSyncTimesPerSec(); }

TApplication::TApplication()
    : mSelf(this)
    , mDirector(nullptr)
    , mDisplay(nullptr)
    , unk30(nullptr)
    , unk3C(0)
    , mHeap(nullptr)
    , unk44(0)
    , unk46(0)
{
}

void* SMSLoadArchive(const char* param_1, void* param_2, u32 param_3,
                     JKRHeap* param_4)
{
	if (param_4 == nullptr)
		param_4 = JKRGetCurrentHeap();

	void* result = nullptr;

	// Try to load a compressed version of the archive first
	char compressedArcPath[64];
	strcpy(compressedArcPath, param_1);
	char* loc = strstr(compressedArcPath, ".arc");
	if (loc != nullptr) {
		strcpy(loc, ".szs");
		s32 entryNum = DVDConvertPathToEntrynum(compressedArcPath);
		if (entryNum != -1) {
			result = JKRDvdRipper::loadToMainRAM(
			    compressedArcPath, (u8*)param_2, EXPAND_SWITCH_DECOMPRESS,
			    param_3, param_4, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0,
			    nullptr);
		}
	}

	// If that fails, then try to load the uncompressed version
	if (result == nullptr)
		result = JKRDvdRipper::loadToMainRAM(
		    param_1, (u8*)param_2, EXPAND_SWITCH_DEFAULT, param_3, param_4,
		    JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, nullptr);

	return result;
}

void SMSLoadArchiveARAM(TARAMBlock* param_1, const char* param_2)
{
	// Try to load a compressed version of the archive first
	char compressedArcPath[64];
	strcpy(compressedArcPath, param_2);
	char* loc = strstr(compressedArcPath, ".arc");
	if (loc != nullptr) {
		strcpy(loc, ".szs");
		s32 entryNum = DVDConvertPathToEntrynum(compressedArcPath);
		if (entryNum != -1) {
			param_1->unk0 = JKRDvdAramRipper::loadToAram(
			    compressedArcPath, 0, EXPAND_SWITCH_DEFAULT, 0, 0);
			param_1->unk4 = true;
		}
	}

	// If that fails, then try to load the uncompressed version
	if (param_1->unk0 == nullptr) {
		param_1->unk0 = JKRDvdAramRipper::loadToAram(
		    (char*)param_2, 0, EXPAND_SWITCH_DEFAULT, 0, 0);
		param_1->unk4 = false;
	}
}

void SMSMountAramArchive(JKRMemArchive* param_1, TARAMBlock& param_2)
{
	if (param_2.unk4) {
		JKRAram::aramToMainRam(param_2.unk0, (u8*)gpMarDirector->getUnkD4(), 0,
		                       0, EXPAND_SWITCH_DECOMPRESS, 0x64000, nullptr,
		                       -1, nullptr);
	} else {
		JKRAram::aramToMainRam(param_2.unk0, (u8*)gpMarDirector->getUnkD4(), 0,
		                       0, EXPAND_SWITCH_DEFAULT, 0, nullptr, -1,
		                       nullptr);
	}
	param_1->mountFixed(gpMarDirector->getUnkD4(), MBF_0);
}

JKRArchive* SMSSwitch2DArchive(const char* param_1, TARAMBlock& param_2)
{
	JKRMemArchive* arch = (JKRMemArchive*)JKRFileLoader::getVolume(param_1);
	arch->unmountFixed();
	SMSMountAramArchive(arch, param_2);
	return arch;
}

void* TApplication::setupThreadFuncBoot()
{
	arcBufNLogo = SMSLoadArchive("/data/nintendo.arc", nullptr, 0, nullptr);
	return nullptr;
}

static void* SetupThreadFuncBoot(void* param)
{
	return ((TApplication*)param)->setupThreadFuncBoot();
}

extern void MarErrInit();

void TApplication::initialize()
{
	OSProtectRange(0, nullptr, 0x80000000, 0);
	OSProtectRange(1, (void*)0x83000000, 0x7d000000, 0);
	OSInitFastCast();
	JKRDvdRipper::setSzpBufferSize(0x1000);
	JKRDvdAramRipper::setSzpBufferSize(0x1000);
	JKRExpHeap::createRoot(1, false);
	JKRExpHeap::create(0x20000, JKRGetRootHeap(), false)->becomeSystemHeap();
	JKRGetRootHeap()->becomeCurrentHeap();
	DVDInit();
	MarErrInit();
	VIInit();
	JUTGamePad::init();
	GXInit(JKRAllocFromHeap(nullptr, 0x80000, 0x20), 0x80000);
	SMS_ResetTexCacheRegion();
	GXPokeAlphaRead(GX_READ_NONE);
	void* pvVar3 = new (0x20) u8[0xa5000];
	GXRenderModeObj rmode;
	SMSSetupTitleRenderMode(&rmode);
	mDisplay = new JDrama::TDisplay(2, pvVar3, pvVar3, rmode);

	JKRHeap* prevHeap = JKRGetCurrentHeap();

	spGameHeapBlock = new (-0x20) u8[0x500000];
	mHeap = JKRExpHeap::create(spGameHeapBlock, 0x500000, JKRGetCurrentHeap(),
	                           true);

	gpRomFont = new JUTRomFont(JKRGetCurrentHeap());
	prevHeap->becomeCurrentHeap();

	JKRAram::create(0xf40000, -1, 8, 8, 8);
	JKRAramStream::setTransBuffer(new (0x20) u8[0x8000], 0x8000, nullptr);
	TDrawSyncManager::start(5, 0x28, 0xf);
	GXSetDrawSyncCallback(&TDrawSyncManager::drawSyncCallback);

	mGamePads[0] = new TMarioGamePad(JUTGamePad::EPort1);
	mGamePads[1] = new TMarioGamePad(JUTGamePad::EPort2);
	mGamePads[2] = new TMarioGamePad(JUTGamePad::EPort3);
	mGamePads[3] = new TMarioGamePad(JUTGamePad::EPort4);

	SMSRumbleMgr = new RumbleMgr(true, true, true, true);
	SMSRumbleMgr->init();
	mFader = new TSmplFader(SMSGetVSyncTimesPerSec(), "ルートフェーダー");
	mFader->setDisplaySize(SMSGetGCLogoRenderWidth(),
	                       SMSGetGCLogoRenderHeight());
	TFlagManager::start(JKRGetCurrentHeap());
	TTimeRec::start(0xDFC0);
	TTimeRec::instance()->unk81C |= 1;
	TDrawSyncManager::smInstance->setCallback(0, 0xDFC0, 0xDFFF,
	                                          TTimeRec::instance());
	mMeter = new TProcessMeter(2);
	srand(OSGetTick());
	gpSetupThreadStack = new (0x20) u8[0x10000];

	mAppState = APP_STATE_BOOT;

	mHeap->becomeCurrentHeap();
	OSCreateThread(&gSetupThread, &SetupThreadFuncBoot, this,
	               gpSetupThreadStack + 0x10000, 0x10000, 0x11, 0);
	OSResumeThread(&gSetupThread);
}

void* TApplication::setupThreadFuncLogo()
{
	while (!gpMSound->checkWaveOnAram(MS_WAVE_UNK0))
		OSYieldThread();
	while (!gpMSound->checkWaveOnAram(MS_WAVE_UNK528))
		OSYieldThread();

	arcBufMario
	    = SMSLoadArchive("/data/mario.arc", nullptr, 0, JKRGetRootHeap());

	arcBufCmn
	    = SMSLoadArchive("/data/common.arc", nullptr, 0, JKRGetRootHeap());

	bufStageArcBin = JKRDvdRipper::loadToMainRAM(
	    "/data/stageArc.bin", nullptr, EXPAND_SWITCH_DEFAULT, 0, mHeap,
	    JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, nullptr);

	SMSLoadArchiveARAM(&gArBkConsole, "/data/game_6.arc");

	SMSLoadArchiveARAM(&gArBkGuide, "/data/guide.arc");

	return nullptr;
}

static void* SetupThreadFuncLogo(void* param)
{
	return ((TApplication*)param)->setupThreadFuncLogo();
}

void TApplication::initialize_bootAfter()
{
	JKRGetRootHeap()->becomeCurrentHeap();
	JKRMemArchive* this_01 = new (JKRGetSystemHeap(), 0) JKRMemArchive;
	this_01->mountFixed(arcBufNLogo, MBF_0);

	this_01->becomeCurrent("/font");
	u32 uVar1
	    = this_01->getResSize(this_01->getResource("standard_fontEx.bfn"));
	ResFONT* font = (ResFONT*)new (0x20) u8[uVar1];
	this_01->readResource(font, uVar1, "standard_fontEx.bfn");
	gpSystemFont = new JUTResFont(font, nullptr);

	this_01->becomeCurrent("/audi");
	u32 uVar3 = this_01->getResSize(this_01->getResource("mSound.aaf"));
	u8* buf   = new u8[uVar3];
	this_01->readResource(buf, uVar3, "mSound.aaf");
	JKRHeap* prevHeap = JKRGetCurrentHeap();
	gpMSound = new MSound(prevHeap, nullptr, 0xF40000, buf, nullptr, 0xb00000);

	JAIGlobalParameter::setParamSoundOutputMode(!OSGetSoundMode() ? 0 : 1);
	gpMSound->loadWave(MS_WAVE_UNK0);
	gpMSound->loadWave(MS_WAVE_UNK528);
	prevHeap->becomeCurrentHeap();

	CARDInit();
	void* buf1 = new (0x20) u8[0x2000];
	void* buf2 = new (0x20) u8[0xA000];
	void* buf3 = new (0x20) u8[0x1000];

	gpCardManager = new TCardManager(buf1, buf2, 0, 14, buf3, 0x1000);
	gpCardManager->readOptionBlock();

	mHeap->becomeCurrentHeap();

	OSCreateThread(&gSetupThread, SetupThreadFuncLogo, this,
	               gpSetupThreadStack + 0x10000, 0x10000, 0x11, 0);
	OSResumeThread(&gSetupThread);
}

void TApplication::initialize_nlogoAfter()
{
	JKRMemArchive* arch = (JKRMemArchive*)JKRFileLoader::getVolume("nintendo");
	arch->unmountFixed();
	delete arch;

	JKRGetRootHeap()->becomeCurrentHeap();

	JKRMemArchive* piVar2 = new JKRMemArchive(arcBufCmn, 0, MBF_0);

	{
		JDrama::TNameRefGen::instance
		    = new (JKRGetSystemHeap(), 0) TMarNameRefGen;

		u32 lVar3 = JKRGetRootHeap()->getSize(bufStageArcBin);
		JSUMemoryInputStream stream(bufStageArcBin, lVar3);
		JDrama::TNameRefGen::getInstance()->load(stream);
		unk30 = JDrama::TNameRefGen::search<
		    TNameRefPtrAryT<TNameRefAryT<TScenarioArchiveName> > >(
		    "ステージ毎シナリオアーカイブ名群");

		delete JDrama::TNameRefGen::instance;
		JDrama::TNameRefGen::instance = nullptr;
	}

	gpRomFont = nullptr;
	((JKRExpHeap*)mHeap)->destroy();
	JKRGetRootHeap()->getSize(spGameHeapBlock);

	JKRMemArchive* this_00 = new JKRMemArchive(arcBufMario, 0, MBF_0);
	gpCardManager->unk4
	    = (ResTIMG*)piVar2->getResource("/card/mario_icon.bti") + 1;
	gpCardManager->unk8
	    = (ResTIMG*)piVar2->getResource("/card/mariobnr_jpn.bti") + 1;

	int status;
	while ((status = gpCardManager->getLastStatus()) == -1)
		OSYieldThread();

	if (status == 0) {
		JSUMemoryInputStream stream;
		gpCardManager->getOptionReadStream(&stream);
		TFlagManager::getInstance()->loadOption(stream);
	}

	gpCardManager->unmount();

	int outputMode = 1;
	switch (TFlagManager::getInstance()->getFlag(0xA0000)) {
	case 0:
		outputMode = 0;
		break;
	case 2:
		outputMode = 2;
		break;
	}

	JAIGlobalParameter::setParamSoundOutputMode(outputMode);

	JMANewSinTable(0xC);

	JKRHeap* heap = JKRGetCurrentHeap();
	mHeap         = JKRSolidHeap::create(heap->getFreeSize(), heap, true);
	mHeap->becomeCurrentHeap();
}

void TApplication::initialize_processMeter() { }

void TApplication::finalize()
{
	SMSRumbleMgr->reset();
	if (gpMSound != nullptr)
		gpMSound->resetAudioAll(0x3C);
	JUTGamePad::clearForReset();
	if (gpCardManager != nullptr)
		gpCardManager->~TCardManager();
	VISetBlack(TRUE);
	VIFlush();
	VIWaitForRetrace();
	if (gpMSound != nullptr)
		while (!gpMSound->resetAudioAll(0x3C))
			VIWaitForRetrace();
	OSResetSystem(unk44 & 2 ? 1 : 0, 0, FALSE);
}

bool TApplication::checkAdditionalMovie()
{
	bool result = false;

	const TGameSequence& currArea = gpApplication.mCurrArea;

	u8 uVar1 = SMS_getShineIDofExStage(currArea.unk0);
	if (uVar1 != 0xFF) {
		if (!TFlagManager::getInstance()->getShineFlag(uVar1)) {
			if (!TFlagManager::getInstance()->getBool(0x3000D)) {
				mMovie = 5;
				TFlagManager::getInstance()->setBool(true, 0x3000D);
				result = true;
			}
		}
	} else {
		switch (currArea.unk0) {
		case 0:
			if (currArea.unk1 == 0) {
				if (!TFlagManager::getInstance()->getBool(0x30009)) {
					mMovie = 1;
					TFlagManager::getInstance()->setBool(true, 0x30009);
					result = true;
				}
			}
			break;

		case 1:
			if (currArea.unk1 == 0) {
				if (!TFlagManager::getInstance()->getBool(0x3000B)) {
					mMovie = 3;
					TFlagManager::getInstance()->setBool(true, 0x3000B);
					result = true;
				}
			} else if (currArea.unk1 == 1) {
				if (!TFlagManager::getInstance()->getBool(0x3000C)) {
					mMovie = 4;
					TFlagManager::getInstance()->setBool(true, 0x3000C);
					result = true;
				}
			}
			break;

		case 8:
			if (currArea.unk1 == 2) {
				if (!TFlagManager::getInstance()->getBool(0x3000D)) {
					mMovie = 5;
					TFlagManager::getInstance()->setBool(true, 0x3000D);
					result = true;
				}
			}
			break;
		}
	}

	return result;
}

void TApplication::proc()
{
	while (mAppState != APP_STATE_QUIT) {
		u8 nextState = APP_STATE_DEFAULT;
		int iVar9    = 0;

		switch (mAppState) {
		case APP_STATE_BOOT:
			SMSSetupGCLogoRenderingInfo(mDisplay);
			break;

		case APP_STATE_NLOGO: {
			SMSSetupGCLogoRenderingInfo(mDisplay);
			TGCLogoDir* dir = new TGCLogoDir;
			mDirector       = dir;
			dir->setup(mDisplay, mGamePads[0]);
		} break;

		case APP_STATE_MENU: {
			SMSSetupTitleRenderingInfo(mDisplay);
			mFader->setDisplaySize(SMSGetTitleRenderWidth(),
			                       SMSGetTitleRenderHeight());
			TMenuDirector* dir = new TMenuDirector;
			mDirector          = dir;
			dir->setup(mDisplay, mGamePads[0]);
			TFlagManager::getInstance()->setFlag(3, 0x20001);
			mCurrArea.set(1, 0, 0);
		} break;

		case APP_STATE_GAMEPLAY:
			if (checkAdditionalMovie()) {
				// Show a movie before entering a stage, e.g. the secret levels
				SMSSetupMovieRenderingInfo(mDisplay);
				mFader->setDisplaySize((u16)SMSGetGameRenderWidth(),
				                       (u16)SMSGetGameRenderHeight());
				TMovieDirector* dir = new TMovieDirector;
				mDirector           = dir;
				dir->setup(mDisplay, mGamePads[0]);
			} else {
				SMSSetupGameRenderingInfo(mDisplay, (unk44 & 1) != 0);
				mFader->setDisplaySize((u16)SMSGetGameRenderWidth(),
				                       (u16)SMSGetGameRenderHeight());
				TMarDirector* dir = new TMarDirector;
				mDirector         = dir;
				iVar9 = dir->setup(mDisplay, mGamePads, mCurrArea.getStage(),
				                   mCurrArea.getScenario());
				if (iVar9)
					nextState = APP_STATE_DONE;
			}
			break;

		case APP_STATE_TITLE: {
			SMSSetupTitleRenderingInfo(mDisplay);
			mFader->setDisplaySize(SMSGetTitleRenderWidth(),
			                       SMSGetTitleRenderHeight());
			TSelectDir* selectDir = new TSelectDir;
			mDirector             = selectDir;
			selectDir->setup(mDisplay, mGamePads[0], mCurrArea.getStage());
		} break;

		case APP_STATE_DONE:
			gpApplication.mMovie = 9;
			mNextArea.set(15, 0, 0);
			// FALLTHROUGH

		case APP_STATE_MOVIE: {
			SMSSetupMovieRenderingInfo(mDisplay);
			mFader->setDisplaySize((u16)SMSGetGameRenderWidth(),
			                       (u16)SMSGetGameRenderHeight());
			TMovieDirector* dir = new TMovieDirector;
			mDirector           = dir;
			dir->setup(mDisplay, mGamePads[0]);
		} break;
		}

		if (!iVar9)
			nextState = gameLoop();

		delete mDirector;
		mDirector = nullptr;

		switch (mAppState) {
		case APP_STATE_BOOT:
			if (!mGamePads[0]->isSomethingPushed())
				initialize_bootAfter();
			break;

		case APP_STATE_NLOGO:
			if (!mGamePads[0]->isSomethingPushed())
				initialize_nlogoAfter();
			break;

		default:
			mHeap->freeAll();
			break;
		}

		SMSRumbleMgr->reset();
		if (mGamePads[0]->isSomethingPushed()) {
			TMarioGamePad::mResetFlag = 0;
			JUTGamePad::recalibrate(0xf0000000);
			if (!DVDCheckDisk()) {
				nextState = APP_STATE_QUIT;
				unk44 |= 2;
			} else if (mAppState == APP_STATE_BOOT
			           || mAppState == APP_STATE_NLOGO) {
				nextState = APP_STATE_QUIT;
			} else if (nextState != APP_STATE_QUIT) {
				nextState = APP_STATE_DONE;
				gpCardManager->unmount();
			}
		}

		mAppState = nextState;
		mPrevArea = mCurrArea;
		mCurrArea = mNextArea;
	}
}

int TApplication::gameLoop()
{
	u32 nextState = APP_STATE_DEFAULT;
	while (nextState <= APP_STATE_DEFAULT) {
		mDisplay->startRendering();

		// TODO: TimeRec BS
		TTimeRec::startTimer();
		TTimeRec::snapGxTimeStatic(0);

		TMarioGamePad::read();
		for (int i = 0; i < 4; i++)
			mGamePads[i]->updateMeaning();

		if (int dvderr = drawDVDErr()) {
			SMSRumbleMgr->reset();
			if (dvderr != 'em_1') {
				if (mGamePads[0]->isSomethingPushed()) {
					nextState = APP_STATE_QUIT;
					unk44 |= 2;
				}
			}
		} else {
			if (mAppState == APP_STATE_BOOT) {
				nextState = APP_STATE_WAIT;
				if (OSIsThreadTerminated(&gSetupThread)) {
					void* val;
					OSJoinThread(&gSetupThread, &val);
					nextState = APP_STATE_NLOGO;
				}
			} else if (mAppState == APP_STATE_NLOGO) {
				nextState = APP_STATE_WAIT;
				if (!(sGameInit & 1)) {
					if (mDirector->direct() == APP_STATE_DONE)
						sGameInit |= 1;
				}
				if (!(sGameInit & 2) && OSIsThreadTerminated(&gSetupThread)) {
					void* val;
					OSJoinThread(&gSetupThread, &val);
					sGameInit |= 2;
				}

				if (sGameInit == 3)
					nextState = APP_STATE_DONE;
			} else {
				nextState = mDirector->direct();
			}

			JDrama::TGraphics graphics;

			JDrama::TVideo* video = mDisplay->unk60;
			GXSetViewport(0.0f, 0.0f, video->mNextRenderMode.fbWidth,
			              video->mNextRenderMode.efbHeight, 0.0f, 1.0f);
			GXSetScissor(0, 0, video->mNextRenderMode.fbWidth,
			             video->mNextRenderMode.efbHeight);
			Mtx afStack_1ac;
			C_MTXOrtho(afStack_1ac, 0.0f, (f32)video->mNextRenderMode.fbWidth,
			           0.0f, (f32)video->mNextRenderMode.efbHeight, -1.0f,
			           1.0f);
			GXSetProjection(afStack_1ac, GX_ORTHOGRAPHIC);
			mFader->update();
			mFader->draw(JDrama::TRect(0, 0, video->mNextRenderMode.fbWidth,
			                           video->mNextRenderMode.efbHeight));
			if (gpMSound != nullptr)
				gpMSound->mainLoop();
		}

		TTimeRec::endTimer();

		THPPlayerDrawDone();
		mDisplay->endRendering();

		if (TTimeRec::_instance)
			TTimeRec::_instance->flip();

		JDrama::TVideo* video = mDisplay->unk60;
		if (video->mCurFrameBuffer) {
			JUTDirectPrint::getManager()->changeFrameBuffer(
			    (void*)video->mCurFrameBuffer, video->mCurRenderMode.fbWidth,
			    video->mCurRenderMode.xfbHeight);
			JUTAssertion::flushMessage();
		}
	}

	return nextState;
}

int TApplication::drawDVDErr()
{
	char message[512];
	u32 error = 0;

	switch (DVDGetDriveStatus()) {
	case -1:
		snprintf(message, 512,
		         "エラーが発生しました。\n"
		         "本体のパワーボタンを押して電源をOFFにし\n"
		         "本体の取扱説明書の指示に従ってください。");
		error = 'em_1';
		break;

	case 11:
		snprintf(message, 512,
		         "ディスクを読めませんでした。\n"
		         "くわしくは、本体の取扱説明書を\n"
		         "お読みください。");
		error = 'em_2';
		break;

	case 1:
		if (DVDCheckDisk() == 0) {
			snprintf(message, 512, "ディスクを読み込んでいます。");
			error = 'em_3';
		}
		break;

	case 5:
		snprintf(message, 512,
		         "ディスクカバーが開いています。\n"
		         "ゲームを続ける場合は\n"
		         "ディスクカバーを閉めてください。");
		error = 'em_4';
		break;

	case 4:
		snprintf(message, 512,
		         "「スーパーマリオサンシャイン」の\n"
		         "ディスクをセットしてください。");
		error = 'em_5';
		break;

	case 6:
		snprintf(message, 512,
		         "このディスクは、「スーパーマリオサンシャイン」の\n"
		         "ディスクではありません。\n"
		         "「スーパーマリオサンシャイン」の\n"
		         "ディスクをセットしてください。 ");
		error = 'em_6';
		break;
	}

	if (error != 0) {
		ReInitializeGX();
		SMS_DrawInit();
		JDrama::TVideo* video = mDisplay->unk60;

		GXSetViewport(0.0f, 0.0f, video->mNextRenderMode.fbWidth,
		              video->mNextRenderMode.efbHeight, 0.0f, 1.0f);
		Mtx afStack_260;
		C_MTXOrtho(afStack_260, 16.0f, 464.0f, 0.0f, 600.0f, -1.0f, 1.0f);
		GXSetProjection(afStack_260, GX_ORTHOGRAPHIC);
		MTXIdentity(afStack_260);
		GXLoadPosMtxImm(afStack_260, 0);
		GXSetCullMode(GX_CULL_BACK);
		GXSetNumTexGens(1);
		GXSetNumTevStages(1);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
		               GX_LO_NOOP);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_VTX, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanAmbColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
		GXPosition2u16(0, 166);
		GXColor4u8(255, 0, 0, 200);
		GXPosition2u16(600, 166);
		GXColor4u8(255, 0, 0, 200);
		GXPosition2u16(600, 316);
		GXColor4u8(255, 0, 0, 200);
		GXPosition2u16(0, 166);
		GXColor4u8(255, 0, 0, 200);
		GXPosition2u16(600, 316);
		GXColor4u8(255, 0, 0, 200);
		GXPosition2u16(0, 316);
		GXColor4u8(255, 0, 0, 200);
		GXEnd();

		JUTFont* font = gpRomFont;
		if (gpSystemFont != nullptr)
			font = gpSystemFont;
		J2DPrint print(font, 0);
		print.unk44  = (GXColor) { 0xff, 0xff, 0, 0xff };
		print.unk48  = (GXColor) { 0xff, 0xff, 0, 0xff };
		f32 msgWidth = print.getWidth(message);
		print.print((600.0f - msgWidth) / 2.0f, 230, message);
	}

	return error;
}

JKRMemArchive* TApplication::mountStageArchive()
{
	JKRMemArchive* result = nullptr;

	TNameRefPtrAryT<TNameRefAryT<TScenarioArchiveName> >& tmp = *unk30;
	if (mCurrArea.getStage() < tmp.size()) {
		if (mCurrArea.getScenario() < tmp[mCurrArea.getStage()].size()) {
			const char* scenarioArcName
			    = tmp[mCurrArea.getStage()][mCurrArea.getScenario()].getName();

			DVDChangeDir("/data/scene");
			void* archBlob
			    = SMSLoadArchive(scenarioArcName, nullptr, 0, nullptr);

			if (archBlob) {
				JKRMemArchive* arch = new JKRMemArchive;
				arch->mountFixed(archBlob, MBF_0);
				result = arch;
			}

			DVDChangeDir("/");
		}
	}

	return result;
}
