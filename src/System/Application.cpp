#include <System/Application.hpp>
#include <dolphin/vi.h>

JKRMemArchive* TApplication::mountStageArchive() { }
void TApplication::drawDVDErr() { }
void TApplication::gameLoop() { }
void TApplication::proc() { }
void TApplication::checkAdditionalMovie() { }
void TApplication::finalize() { }
void TApplication::initialize_processMeter() { }
void TApplication::initialize_nlogoAfter() { }
void TApplication::initialize_bootAfter() { }
void SetupThreadFuncLogo(void*) { }
void TApplication::setupThreadFuncLogo() { }
void TApplication::initialize() { }
void SetupThreadFuncBoot(void*) { }
void TApplication::setupThreadFuncBoot() { }
JKRArchive* SMSSwitch2DArchive(const char*, TARAMBlock&) { }
void SMSMountAramArchive(JKRMemArchive*, TARAMBlock&) { }
void SMSLoadArchiveARAM(TARAMBlock*, const char*) { }
void* SMSLoadArchive(const char*, void*, u32, JKRHeap*) { }
TApplication::TApplication() { }
f32 SMSGetAnmFrameRate() { }

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
