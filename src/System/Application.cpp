#include <System/Application.hpp>
#include <stdlib.h>
#include <types.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <dolphin/gx.h>
#include <JSystem/JKernel/JKRDvdRipper.hpp>
#include <JSystem/JKernel/JKRDvdAramRipper.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JKernel/JKRAram.hpp>
#include <JSystem/JKernel/JKRAramStream.hpp>
#include <JSystem/JUtility/JUTGamePad.hpp>
#include <JSystem/JUtility/JUTRomFont.hpp>
#include <JSystem/JDrama/JDRDisplay.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <GC2D/ScrnFader.hpp>
#include <System/RenderModeObj.hpp>
#include <System/TexCache.hpp>
#include <System/DrawSyncManager.hpp>
#include <System/MarioGamePad.hpp>
#include <System/Resolution.hpp>
#include <System/FlagManager.hpp>
#include <System/TimeRec.hpp>
#include <System/ProcessMeter.hpp>

static void* spGameHeapBlock;
JUTRomFont* gpRomFont;
OSThread gSetupThread;
u8* gpSetupThreadStack;

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
    , unk30(0)
    , unk3C(0)
    , mHeap(nullptr)
    , unk44(0)
    , unk46(0)
{
}

void* SMSLoadArchive(const char*, void*, u32, JKRHeap*) { }

void SMSLoadArchiveARAM(TARAMBlock*, const char*) { }

void SMSMountAramArchive(JKRMemArchive*, TARAMBlock&) { }

JKRArchive* SMSSwitch2DArchive(const char*, TARAMBlock&) { }

void TApplication::setupThreadFuncBoot() { }

void* SetupThreadFuncBoot(void*) { }

extern void MarErrInit();

void TApplication::initialize()
{
	OSProtectRange(0, nullptr, 0x80000000, 0);
	OSProtectRange(1, (void*)0x83000000, 0x7d000000, 0);
	OSInitFastCast();
	JKRDvdRipper::setSzpBufferSize(0x1000);
	JKRDvdAramRipper::setSzpBufferSize(0x1000);
	JKRExpHeap::createRoot(1, false);
	JKRExpHeap::create(0x20000, JKRHeap::getRootHeap(), false)
	    ->becomeSystemHeap();
	JKRHeap::getRootHeap()->becomeCurrentHeap();
	DVDInit();
	MarErrInit();
	VIInit();
	JUTGamePad::init();
	GXInit(JKRHeap::alloc(0x80000, 0x20, nullptr), 0x80000);
	SMS_ResetTexCacheRegion();
	GXPokeAlphaRead(GX_READ_NONE);
	void* pvVar3 = new (0x20) u8[0xa5000];
	GXRenderModeObj rmode;
	SMSSetupTitleRenderMode(&rmode);
	mDisplay = new JDrama::TDisplay(2, pvVar3, pvVar3, rmode);

	JKRHeap* prevHeap = JKRHeap::getCurrentHeap();

	spGameHeapBlock = new (-0x20) u8[0x500000];
	mHeap           = JKRExpHeap::create(spGameHeapBlock, 0x500000,
	                                     JKRHeap::getCurrentHeap(), true);

	gpRomFont = new JUTRomFont(JKRHeap::getCurrentHeap());
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
	TFlagManager::start(JKRHeap::getCurrentHeap());
	TTimeRec::start(0xDFC0);
	TTimeRec::instance()->unk81C |= 1;
	TDrawSyncManager::smInstance->setCallback(0, 0xDFC0, 0xDFFF,
	                                          TTimeRec::instance());
	mMeter = new TProcessMeter(2);
	srand(OSGetTick());
	gpSetupThreadStack = new (0x20) u8[0x10000];

	unk8 = 2;

	mHeap->becomeCurrentHeap();
	OSCreateThread(&gSetupThread, &SetupThreadFuncBoot, this,
	               gpSetupThreadStack + 0x10000, 0x10000, 0x11, 0);
	OSResumeThread(&gSetupThread);
}

void TApplication::setupThreadFuncLogo() { }

void SetupThreadFuncLogo(void*) { }

void TApplication::initialize_bootAfter() { }

void TApplication::initialize_nlogoAfter() { }

void TApplication::initialize_processMeter() { }

void TApplication::finalize() { }

void TApplication::checkAdditionalMovie() { }

void TApplication::proc() { }

void TApplication::gameLoop() { }

void TApplication::drawDVDErr() { }

JKRMemArchive* TApplication::mountStageArchive() { }
