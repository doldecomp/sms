#include <dolphin.h>

#include "types.h"

extern "C" {
	s32 VIGetTvFormat();
}

void SMSGetGCLogoVideoHeight() {
	return SMSGetGameVideoHeight();
}

s32 SMSGetGCLogoVideoWidth() {
	return 640;
}

s32 SMSGetGCLogoRenderHeight() {
	return 448;
}

s32 SMSGetGCLogoRenderWidth() {
	return 640;
}

s32 SMSGetTitleRenderHeight() {
	return 448;
}

s32 SMSGetTitleRenderWidth() {
	return 640;
}

s32 SMSGetGameRenderHeight() {
	return 448;
}

s32 SMSGetGameRenderWidth() {
	return 640;
}


