#include <System/Resolution.hpp>
#include <dolphin/vi.h>

s32 SMSGetGameVideoHeight();

s32 SMSGetGCLogoVideoHeight() { return SMSGetGameVideoHeight(); }

s32 SMSGetGCLogoVideoWidth() { return 640; }

u16 SMSGetGCLogoRenderHeight() { return 448; }

u16 SMSGetGCLogoRenderWidth() { return 640; }

u16 SMSGetTitleRenderHeight() { return 448; }

u16 SMSGetTitleRenderWidth() { return 640; }

s32 SMSGetGameRenderHeight() { return 448; }

s32 SMSGetGameRenderWidth() { return 640; }

s32 SMSGetTitleVideoHeight()
{
	s32 ret = 448;

	switch (VIGetTvFormat()) {
	case 2:
	case 0:
	case 5:
		ret = 448;
		break;

	case 1:
		ret = 530;
		break;

	default:
		break;
	}
	return ret;
}

s32 SMSGetTitleVideoWidth() { return 660; }

s32 SMSGetGameVideoHeight()
{
	s32 ret = 448;

	switch (VIGetTvFormat()) {
	case 2:
	case 0:
	case 5:
		ret = 448;
		break;

	case 1:
		ret = 530;
		break;

	default:
		break;
	}
	return ret;
}

s32 SMSGetGameVideoWidth() { return 660; }
