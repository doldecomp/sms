#include <System/Resolution.hpp>
#include <dolphin/vi.h>

u16 SMSGetGameVideoWidth() {
	return VERSION_SELECT_DEFAULT(660, GMSP01_NX(640));
}

#if defined(VERSION_GMSP01) || defined(VERSION_GMSJ01_01) || defined(VERSION_GMSP01_NX)
u16 SMSGetGameVideoHeight(u32 tvFormat)
{
	u16 ret = VERSION_SELECT_DEFAULT(448, GMSP01_NX(360));
	switch (tvFormat) {
	case VI_MPAL:
	case VI_NTSC:
	case VI_EURGB60:
		ret = VERSION_SELECT_DEFAULT(448, GMSP01_NX(360));
		break;
	case VI_PAL:
		ret = VERSION_SELECT_DEFAULT(448, GMSP01_NX(360));
		break;
	default:
		break;
	}
	return ret;
}
#else
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
#endif

// UNUSED
s32 SMSGetDisplayWidthMax() { return 720; }

// UNUSED
s32 SMSGetDisplayHeightMax() { return 574; }

u16 SMSGetTitleVideoWidth() { return VERSION_SELECT_DEFAULT(660, GMSP01_NX(640)); }

#if defined(VERSION_GMSP01) || defined(VERSION_GMSJ01_01) || defined(VERSION_GMSP01_NX)
u16 SMSGetTitleVideoHeight(u32 tvFormat)
{
	return SMSGetGameVideoHeight(tvFormat);
}
#else
u16 SMSGetTitleVideoHeight() { return SMSGetGameVideoHeight(); }
#endif

u16 SMSGetGameRenderWidth() { return 640; }

u16 SMSGetGameRenderHeight() { return VERSION_SELECT_DEFAULT(448, GMSP01_NX(360)); }

u16 SMSGetTitleRenderWidth() { return 640; }

u16 SMSGetTitleRenderHeight() { return VERSION_SELECT_DEFAULT(448, GMSP01_NX(360)); }

u16 SMSGetGCLogoRenderWidth() { return 640; }

u16 SMSGetGCLogoRenderHeight() { return VERSION_SELECT_DEFAULT(448, GMSP01_NX(360)); }

u16 SMSGetGCLogoVideoWidth() { return 640; }

#if defined(VERSION_GMSP01) || defined(VERSION_GMSJ01_01) || defined(VERSION_GMSP01_NX)
u16 SMSGetGCLogoVideoHeight(u32 tvFormat)
{
	return SMSGetTitleVideoHeight(tvFormat);
}
#else
u16 SMSGetGCLogoVideoHeight() { return SMSGetTitleVideoHeight(); }
#endif

#if defined(VERSION_GMSP01_NX)
u16 SMSGetIntroRenderWidth() { return 640; }
u16 SMSGetIntroRenderHeight() { return 448; }
#endif