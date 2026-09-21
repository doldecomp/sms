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
#include <System/GCLogoDir.hpp>
#include <System/MovieDirector.hpp>
#include <System/SelectDir.hpp>
#include <System/MenuDir.hpp>
#include <MSound/MSound.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/DummyStrings.hpp>

TMarDirector* gpMarDirector;
MSound* gpMSound;

OSThread gSetupThread;
u8* gpSetupThreadStack;

TCardManager* gpCardManager;
JUTResFont* gpSystemFont;
JUTRomFont* gpRomFont;

static int sGameInit;
static void* arcBufNLogo;
static void* arcBufMario;
static void* arcBufCmn;
static void* bufStageArcBin;
static void* spGameHeapBlock;
#ifdef VERSION_GMSP01
static JKRMemArchive* sCmn2DArc;
static void* sCmn2DArcBuf;
#endif

TARAMBlock gArBkConsole;
TARAMBlock gArBkGuide;

extern "C" void ReInitializeGX();
u8 SMS_getShineIDofExStage(u8);

#ifdef VERSION_GMSP01
f32 SMSGetRealVSyncTimesPerSec()
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
	return result;
}

f32 SMSGetVSyncTimesPerSec() { return SMSGetRealVSyncTimesPerSec() / 2.0f; }
#else

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

#endif

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

void* SMSLoadArchive(const char* path, void* dst, u32 dstLength, JKRHeap* heap)
{
	if (heap == nullptr)
		heap = JKRGetCurrentHeap();

	void* result = nullptr;

	// Try to load a compressed version of the archive first
	char compressedArcPath[64];
	strcpy(compressedArcPath, path);
	char* loc = strstr(compressedArcPath, ".arc");
	if (loc != nullptr) {
		strcpy(loc, ".szs");
		s32 entryNum = DVDConvertPathToEntrynum(compressedArcPath);
		if (entryNum != -1) {
			result = JKRDvdToMainRam(compressedArcPath, (u8*)dst,
			                         EXPAND_SWITCH_DECOMPRESS, dstLength, heap,
			                         JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0,
			                         nullptr);
		}
	}

	// If that fails, then try to load the uncompressed version
	if (result == nullptr)
		result = JKRDvdToMainRam(
		    path, (u8*)dst, EXPAND_SWITCH_DEFAULT, dstLength, heap,
		    JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, nullptr);

	return result;
}

void SMSLoadArchiveARAM(TARAMBlock* out_block, const char* path)
{
	// Try to load a compressed version of the archive first
	char compressedArcPath[64];
	strcpy(compressedArcPath, path);
#ifdef VERSION_GMSP01
	out_block->mBlock = nullptr;
#endif
	char* loc = strstr(compressedArcPath, ".arc");
	if (loc != nullptr) {
		strcpy(loc, ".szs");
		s32 entryNum = DVDConvertPathToEntrynum(compressedArcPath);
		if (entryNum != -1) {
			out_block->mBlock = JKRDvdAramRipper::loadToAram(
			    compressedArcPath, 0, EXPAND_SWITCH_DEFAULT, 0, 0);
			out_block->mIsCompressed = true;
		}
	}

	// If that fails, then try to load the uncompressed version
	if (out_block->mBlock == nullptr) {
		out_block->mBlock = JKRDvdAramRipper::loadToAram(
		    (char*)path, 0, EXPAND_SWITCH_DEFAULT, 0, 0);
		out_block->mIsCompressed = false;
	}
}

void SMSMountAramArchive(JKRMemArchive* archive, TARAMBlock& block)
{
	if (block.mIsCompressed) {
		JKRAram::aramToMainRam(
		    block.mBlock, (u8*)SMSGetMarDirector()->getUnkD4(), 0, 0,
		    EXPAND_SWITCH_DECOMPRESS, 0x64000, nullptr, -1, nullptr);
	} else {
		JKRAram::aramToMainRam(block.mBlock,
		                       (u8*)SMSGetMarDirector()->getUnkD4(), 0, 0,
		                       EXPAND_SWITCH_DEFAULT, 0, nullptr, -1, nullptr);
	}
	archive->mountFixed(SMSGetMarDirector()->getUnkD4(), MBF_0);
}

JKRArchive* SMSSwitch2DArchive(const char* arc_path, TARAMBlock& block)
{
	JKRMemArchive* arch = (JKRMemArchive*)JKRFileLoader::getVolume(arc_path);
	arch->unmountFixed();
	SMSMountAramArchive(arch, block);
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
	mFader = new TSmplFader(JUtility::TColor(0, 0, 0, 0xff),
	                        SMSGetVSyncTimesPerSec(), "ルートフェーダー");
	mFader->setDisplaySize(SMSGetGCLogoRenderWidth(),
	                       SMSGetGCLogoRenderHeight());
	TFlagManager::start(JKRGetCurrentHeap());
	TTimeRec::start(0xDFC0);
	TTimeRec::instance()->mFlags.on(1);
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

#ifdef VERSION_GMSP01
#pragma dont_inline on
void load2DResource2Aram()
{
	static u8 sLoadResourceLang     = -1;
	static const char* cmn2dNames[] = {
		"/data/cmn2d_en.arc", "/data/cmn2d_ge.arc", "/data/cmn2d_fr.arc",
		"/data/cmn2d_sp.arc", "/data/cmn2d_it.arc",
	};
	static const char* game_6Names[] = {
		"/data/game_6_en.arc", "/data/game_6_ge.arc", "/data/game_6_fr.arc",
		"/data/game_6_sp.arc", "/data/game_6_it.arc",
	};
	static const char* guideNames[] = {
		"/data/guide_en.arc", "/data/guide_ge.arc", "/data/guide_fr.arc",
		"/data/guide_sp.arc", "/data/guide_it.arc",
	};

	if (sLoadResourceLang
	    != (u8)TFlagManager::getInstance()->getFlag(0xA0001)) {
		sLoadResourceLang = (u8)TFlagManager::getInstance()->getFlag(0xA0001);
		if (JKRFileLoader::getVolume("cmn2d") != nullptr)
			sCmn2DArc->unmountFixed();

		SMSLoadArchive(cmn2dNames[sLoadResourceLang], sCmn2DArcBuf, 0xA000,
		               nullptr);
		sCmn2DArc->mountFixed(sCmn2DArcBuf, MBF_0);
		JKRAram::getAramHeap()->freeAll();

		SMSLoadArchiveARAM(&gArBkConsole, game_6Names[sLoadResourceLang]);
		SMSLoadArchiveARAM(&gArBkGuide, guideNames[sLoadResourceLang]);
	}
}
#pragma dont_inline off
#endif

#pragma dont_inline on
void* TApplication::setupThreadFuncLogo()
{
	while (!SMSGetMSound()->checkWaveOnAram(MS_WAVE_UNK0))
		OSYieldThread();
	while (!SMSGetMSound()->checkWaveOnAram(MS_WAVE_UNK210))
		OSYieldThread();

	arcBufMario
	    = SMSLoadArchive("/data/mario.arc", nullptr, 0, JKRGetRootHeap());

	arcBufCmn
	    = SMSLoadArchive("/data/common.arc", nullptr, 0, JKRGetRootHeap());

	bufStageArcBin = JKRDvdToMainRam(
	    "/data/stageArc.bin", nullptr, EXPAND_SWITCH_DEFAULT, 0, mHeap,
	    JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, nullptr);

#ifdef VERSION_GMSP01
	load2DResource2Aram();
#else
	SMSLoadArchiveARAM(&gArBkConsole, "/data/game_6.arc");
	SMSLoadArchiveARAM(&gArBkGuide, "/data/guide.arc");
#endif

	return nullptr;
}
#pragma dont_inline off

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
	void* fontRes = this_01->getResource("standard_fontEx.bfn");
	u32 uVar1     = this_01->getResSize(fontRes);
	ResFONT* font = (ResFONT*)new (0x20) u8[uVar1];
	this_01->readResource(font, uVar1, "standard_fontEx.bfn");
	gpSystemFont = new JUTResFont(font, nullptr);

	this_01->becomeCurrent("/audi");
	void* aafRes = this_01->getResource("mSound.aaf");
	u32 uVar3    = this_01->getResSize(aafRes);
	u8* buf      = new u8[uVar3];
	this_01->readResource(buf, uVar3, "mSound.aaf");
	JKRHeap* prevHeap = JKRGetCurrentHeap();
	gpMSound = new MSound(prevHeap, nullptr, 0xF40000, buf, nullptr, 0xb00000);

	JAIGlobalParameter::setParamSoundOutputMode(!OSGetSoundMode() ? 0 : 1);
	gpMSound->loadWave(MS_WAVE_UNK0);
	gpMSound->loadWave(MS_WAVE_UNK210);
	prevHeap->becomeCurrentHeap();

	CARDInit();

	void* sectorWorkArea    = new (0x20) u8[0x2000];
	void* cardWorkArea      = new (0x20) u8[0xA000];
	void* workerThreadStack = new (0x20) u8[0x1000];
	gpCardManager = new TCardManager(sectorWorkArea, cardWorkArea, 0, 14,
	                                 workerThreadStack, 0x1000);
	gpCardManager->readOptionBlock();

#ifdef VERSION_GMSP01
	sCmn2DArcBuf = new (0x20) u8[0xA000];
	sCmn2DArc    = new JKRMemArchive;
#endif

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
		unk30 = static_cast<
		    TNameRefPtrAryT<TNameRefAryT<TScenarioArchiveName> >*>(
		    JDrama::TNameRefGen::search("ステージ毎シナリオアーカイブ名群"));

		delete JDrama::TNameRefGen::instance;
		JDrama::TNameRefGen::instance = nullptr;
	}

	gpRomFont = nullptr;
	((JKRExpHeap*)mHeap)->destroy();
	JKRGetRootHeap()->free(spGameHeapBlock);

	JKRMemArchive* this_00 = new JKRMemArchive(arcBufMario, 0, MBF_0);
	gpCardManager->mIcons
	    = (ResTIMG*)piVar2->getResource("/card/mario_icon.bti") + 1;
	gpCardManager->mBanner
#ifdef VERSION_GMSP01
	    = (ResTIMG*)piVar2->getResource("/card/mariobnr.bti") + 1;
#else
	    = (ResTIMG*)piVar2->getResource("/card/mariobnr_jpn.bti") + 1;
#endif

#ifndef VERSION_GMSP01
	int status;
	while ((status = gpCardManager->getLastStatus()) == -1)
		OSYieldThread();

	if (status == 0) {
		JSUMemoryInputStream stream;
		gpCardManager->getOptionReadStream(&stream);
		TFlagManager::getInstance()->loadOption(stream);
	}

#endif

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

	mHeap = JKRSolidHeap::create(JKRGetCurrentHeap()->getFreeSize(),
	                             JKRGetCurrentHeap(), true);
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

	const TGameSequence& currArea = SMSGetApplication()->mCurrArea;

	u8 uVar1 = SMS_getShineIDofExStage(currArea.getStage());
	if (uVar1 != 0xFF) {
		if (!TFlagManager::getInstance()->getShineFlag(uVar1)) {
			if (!TFlagManager::getInstance()->getBool(0x3000D)) {
				mMovie = 5;
				TFlagManager::getInstance()->setBool(true, 0x3000D);
				result = true;
			}
		}
	} else {
		switch (currArea.getStage()) {
		case 0:
			if (currArea.getScenario() == 0) {
				if (!TFlagManager::getInstance()->getBool(0x30009)) {
					mMovie = 1;
					TFlagManager::getInstance()->setBool(true, 0x30009);
					result = true;
				}
			}
			break;

		case 1:
			if (currArea.getScenario() == 0) {
				if (!TFlagManager::getInstance()->getBool(0x3000B)) {
					mMovie = 3;
					TFlagManager::getInstance()->setBool(true, 0x3000B);
					result = true;
				}
			} else if (currArea.getScenario() == 1) {
				if (!TFlagManager::getInstance()->getBool(0x3000C)) {
					mMovie = 4;
					TFlagManager::getInstance()->setBool(true, 0x3000C);
					result = true;
				}
			}
			break;

		case 8:
			if (currArea.getScenario() == 2) {
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
#ifdef VERSION_GMSP01
		mDisplay->unk4C = 2;
#endif
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
			TFlagManager::getInstance()->setFlag(0x20001, 3);
			mCurrArea.set(1, 0, 0);
		} break;

		case APP_STATE_GAMEPLAY:
			if (checkAdditionalMovie()) {
				// Show a movie before entering a stage, e.g. the secret levels
				SMSSetupMovieRenderingInfo(mDisplay);
#ifdef VERSION_GMSP01
				if ((mDisplay->getRenderMode().viTVmode >> 2) == VI_PAL)
					mDisplay->unk4C = 1;
#endif
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
			SMSGetApplication()->setMovie(9);
			mNextArea.set(15, 0, 0);
			// FALLTHROUGH

		case APP_STATE_MOVIE: {
			SMSSetupMovieRenderingInfo(mDisplay);
#ifdef VERSION_GMSP01
			if ((mDisplay->getRenderMode().viTVmode >> 2) == VI_PAL)
				mDisplay->unk4C = 1;
#endif
			mFader->setDisplaySize((u16)SMSGetGameRenderWidth(),
			                       (u16)SMSGetGameRenderHeight());
			TMovieDirector* dir = new TMovieDirector;
			mDirector           = dir;
			dir->setup(mDisplay, mGamePads[0]);
		} break;
		}

		if (!iVar9)
			nextState = gameLoop();

		if (mDirector != nullptr)
			mDirector->~TDirector();
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

		TTimeRec::startFrameSt(mDisplay->unk60->mLastRetraceTime);
		TTimeRec::snapGXTimeSt(0);

		TMarioGamePad::read();
		for (int i = 0; i < 4; i++) {
			mGamePads[i]->updateMeaning();
			mGamePads[i]->onFlag(TMarioGamePad::PAD_FLAG_0x40);
		}

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
			graphics.unkFE = 0;

			const GXRenderModeObj& rmode
			    = mDisplay->getVideo()->mNextRenderMode;
			GXSetViewport(0.0f, 0.0f, rmode.fbWidth, rmode.efbHeight, 0.0f,
			              1.0f);
			GXSetScissor(0, 0, rmode.fbWidth, rmode.efbHeight);
			Mtx afStack_1ac;
			C_MTXOrtho(afStack_1ac, 0.0f, (f32)rmode.efbHeight, 0.0f,
			           (f32)rmode.fbWidth, -1.0f, 1.0f);
			GXSetProjection(afStack_1ac, GX_ORTHOGRAPHIC);
			mFader->update();
			mFader->draw(JDrama::TRect(0, 0, rmode.fbWidth, rmode.efbHeight));
			if (gpMSound != nullptr)
				gpMSound->mainLoop();
		}

		TTimeRec::snapCPUTime(0);

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

#ifdef VERSION_GMSP01
static const char* sDvdErrMsgs[][6] = {
	{
	    "An error has occurred. Turn the\npower OFF and check the \n"
	    "NINTENDO GAMECUBE\x99"
	    "\nInstruction Booklet for further instructions.",
	    "The Disc could not be read.\nPlease read the NINTENDO GAMECUBE\x99"
	    "\nInstruction Booklet for\nmore information.",
	    "Reading Disc... ",
	    "The Disc Cover is open.\nTo continue playing,\nplease close the\nDisc "
	    "Cover.",
	    "Please insert a Super Mario Sunshine\nGame Disc.",
	    "This Disc is not\nSuper Mario Sunshine.\nPlease insert a Super\n"
	    "Mario Sunshine Game Disc.",
	},
	{
	    "Ein Fehler ist aufgetreten. Bitte schalte\nden NINTENDO GAMECUBE\x99"
	    " aus und\nlies die Bedienungsanleitung,\num weitere Informationen zu "
	    "erhalten.",
	    "Diese Disc kann nicht gelesen werden.\nBitte lies die "
	    "Bedienungsanleitung\n"
	    "des NINTENDO GAMECUBE\x99"
	    ",\num weitere Informationen zu erhalten.",
	    "Disc wird gelesen...",
	    "Disc-Deckel ist ge\xF6"
	    "ffnet. Bitte\nschlie\xDF"
	    "e den Disc-Deckel,\n"
	    "um weiterzuspielen.",
	    "Bitte lege eine Super Mario Sunshine\nGame Disc ein.",
	    "Diese Disc beinhaltet nicht Super Mario Sunshine.\n"
	    "Bitte lege eine Super Mario Sunshine\nGame Disc ein.",
	},
	{
	    "Une erreur est survenue. Eteignez\nla console et r\xE9"
	    "f\xE9"
	    "rez-vous au manuel\n"
	    "d'instructions NINTENDO GAMECUBE\x99"
	    "\npour de plus amples informations.",
	    "La lecture du disque a \xE9"
	    "chou\xE9"
	    ".\nVeuillez-vous r\xE9"
	    "f\xE9"
	    "rer au manuel\nd'instructions NINTENDO GAMECUBE\x99"
	    "\npour de plus amples informations.",
	    "Lecture du disque...",
	    "Le couvercle est ouvert.\nPour continuer \xE0"
	    " jouer,\n"
	    "veuillez fermer le couvercle.",
	    "Veuillez ins\xE9"
	    "rer\nle disque\nSuper Mario Sunshine.",
	    "Ce disque n'est pas\nle bon. Veuillez\nins\xE9"
	    "rer le disque\n"
	    "Super Mario Sunshine.",
	},
	{
	    "Se ha producido un error. Apaga la consola\ny consulta el manual de "
	    "instrucciones\n"
	    "de NINTENDO GAMECUBE\x99"
	    " para\nobtener m\xE1"
	    "s informaci\xF3"
	    "n.",
	    "No se puede leer el disco. Consulta\nel manual de instrucciones de \n"
	    "NINTENDO GAMECUBE\x99"
	    "\npara obtener m\xE1"
	    "s informaci\xF3"
	    "n.",
	    "Leyendo el disco...",
	    "La tapa est\xE1"
	    " abierta.\nCi\xE9"
	    "rrala para seguir jugando.",
	    "Coloca el disco de Super Mario Sunshine.",
	    "\xC9"
	    "ste no es el disco de\nSuper Mario Sunshine.\n"
	    "Coloca el disco apropiado.",
	},
	{
	    "Si \xE8"
	    " verificato un errore. Spegni (OFF) e\n"
	    "controlla il manuale d'istruzioni del\nNINTENDO GAMECUBE\x99"
	    "\nper ulteriori indicazioni.",
	    "Impossibile leggere il disco. Consulta il manuale \n"
	    "d'istruzioni del NINTENDO GAMECUBE\x99"
	    "\nper ulteriori indicazioni.",
	    "Lettura del disco in corso...",
	    "Il coperchio del disco \xE8"
	    "\naperto. Se vuoi proseguire\n"
	    "il gioco, chiudi il\ncoperchio del disco.",
	    "Inserisci il disco\nSuper Mario Sunshine.",
	    "Questo disco non \xE8"
	    " \nSuper Mario Sunshine.\n"
	    "Inserisci il disco \nSuper Mario Sunshine.",
	},
};
#endif

int TApplication::drawDVDErr()
{
	char message[512];
	u32 error = 0;
#ifdef VERSION_GMSP01
	u32 language;
	language = TFlagManager::getInstance()->getFlag(0xA0001);
#endif

	switch (DVDGetDriveStatus()) {
	case -1:
#ifdef VERSION_GMSP01
		snprintf(message, 512, sDvdErrMsgs[language][0]);
#else
		snprintf(message, 512,
		         "エラーが発生しました。\n"
		         "本体のパワーボタンを押して電源をOFFにし\n"
		         "本体の取扱説明書の指示に従ってください。");
#endif
		error = 'em_1';
		break;

	case 11:
#ifdef VERSION_GMSP01
		snprintf(message, 512, sDvdErrMsgs[language][1]);
#else
		snprintf(message, 512,
		         "ディスクを読めませんでした。\n"
		         "くわしくは、本体の取扱説明書を\n"
		         "お読みください。");
#endif
		error = 'em_2';
		break;

	case 1:
		if (DVDCheckDisk() == 0) {
#ifdef VERSION_GMSP01
			snprintf(message, 512, sDvdErrMsgs[language][2]);
#else
			snprintf(message, 512, "ディスクを読み込んでいます。");
#endif
			error = 'em_3';
		}
		break;

	case 5:
#ifdef VERSION_GMSP01
		snprintf(message, 512, sDvdErrMsgs[language][3]);
#else
		snprintf(message, 512,
		         "ディスクカバーが開いています。\n"
		         "ゲームを続ける場合は\n"
		         "ディスクカバーを閉めてください。");
#endif
		error = 'em_4';
		break;

	case 4:
#ifdef VERSION_GMSP01
		snprintf(message, 512, sDvdErrMsgs[language][4]);
#else
		snprintf(message, 512,
		         "「スーパーマリオサンシャイン」の\n"
		         "ディスクをセットしてください。");
#endif
		error = 'em_5';
		break;

	case 6:
#ifdef VERSION_GMSP01
		snprintf(message, 512, sDvdErrMsgs[language][5]);
#else
		snprintf(message, 512,
		         "このディスクは、「スーパーマリオサンシャイン」の\n"
		         "ディスクではありません。\n"
		         "「スーパーマリオサンシャイン」の\n"
		         "ディスクをセットしてください。 ");
#endif
		error = 'em_6';
		break;
	}

	if (error != 0) {
		ReInitializeGX();
		SMS_DrawInit();
		const GXRenderModeObj& rmode = mDisplay->getVideo()->mNextRenderMode;

		GXSetViewport(0.0f, 0.0f, rmode.fbWidth, rmode.efbHeight, 0.0f, 1.0f);
		Mtx afStack_260;
		C_MTXOrtho(afStack_260, 16.0f, 464.0f, 0.0f, 600.0f, -1.0f, 1.0f);
		GXSetProjection(afStack_260, GX_ORTHOGRAPHIC);
		MTXIdentity(afStack_260);
		GXLoadPosMtxImm(afStack_260, GX_PNMTX0);
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
		print.print(0.5f * (600.0f - msgWidth), 230, message);
	}

	return error;
}

JKRMemArchive* TApplication::mountStageArchive()
{
	JKRMemArchive* result = nullptr;

	TNameRefPtrAryT<TNameRefAryT<TScenarioArchiveName> >& tmp = *unk30;
	if (mCurrArea.getStage() < tmp.getChildren().size()) {
		TNameRefAryT<TScenarioArchiveName>& scenarios
		    = tmp[mCurrArea.getStage()];
		if (mCurrArea.getScenario() < scenarios.size()) {
			const char* scenarioArcName
			    = scenarios.getChildren()[mCurrArea.getScenario()].mArcName;

			DVDChangeDir("/data/scene");
			if (void* archBlob
			    = SMSLoadArchive(scenarioArcName, nullptr, 0, nullptr)) {
				JKRMemArchive* arch = new JKRMemArchive;
				arch->mountFixed(archBlob, MBF_0);
				result = arch;
			}

			DVDChangeDir("/");
		}
	}

	return result;
}
