#include <System/Resolution.hpp>
#include <dolphin/vi.h>

u16 SMSGetGameVideoWidth() { return 660; }

u16 SMSGetGameVideoHeight()
{
	u16 ret = 448;
	switch (VIGetTvFormat()) {
	case VI_MPAL:
	case VI_NTSC:
	case VI_EURGB60:
		ret = 448;
		break;
	case VI_PAL:
		ret = 530;
		break;
	default:
		break;
	}
	return ret;
}

u16 SMSGetTitleVideoWidth() { return 660; }

u16 SMSGetTitleVideoHeight() { return SMSGetGameVideoHeight(); }

s32 SMSGetGameRenderWidth() { return 640; }

s32 SMSGetGameRenderHeight() { return 448; }

u16 SMSGetTitleRenderWidth() { return 640; }

u16 SMSGetTitleRenderHeight() { return 448; }

u16 SMSGetGCLogoRenderWidth() { return 640; }

u16 SMSGetGCLogoRenderHeight() { return 448; }

u16 SMSGetGCLogoVideoWidth() { return 640; }

u16 SMSGetGCLogoVideoHeight() { return SMSGetTitleVideoHeight(); }
