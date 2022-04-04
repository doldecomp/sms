#include "dolphin.h"

#include "types.h"

s32 SMSGetGameVideoHeight();

s32 SMSGetGCLogoVideoHeight() { return SMSGetGameVideoHeight(); }

s32 SMSGetGCLogoVideoWidth() { return 640; }

s32 SMSGetGCLogoRenderHeight() { return 448; }

s32 SMSGetGCLogoRenderWidth() { return 640; }

s32 SMSGetTitleRenderHeight() { return 448; }

s32 SMSGetTitleRenderWidth() { return 640; }

s32 SMSGetGameRenderHeight() { return 448; }

s32 SMSGetGameRenderWidth() { return 640; }

s32 SMSGetTitleVideoHeight()
{
	s32 ret = 0x1C0;

	switch (VIGetTvFormat()) {
	case 2:
	case 0:
	case 5:
		ret = 0x1C0;
		break;

	case 1:
		ret = 0x212;
		break;

	default:
		break;
	}
	return ret;
}

s32 SMSGetTitleVideoWidth() { return 0x294; }

s32 SMSGetGameVideoHeight()
{
	s32 ret = 0x1C0;

	switch (VIGetTvFormat()) {
	case 2:
	case 0:
	case 5:
		ret = 0x1C0;
		break;

	case 1:
		ret = 0x212;
		break;

	default:
		break;
	}
	return ret;
}

s32 SMSGetGameVideoWidth() { return 0x294; }
