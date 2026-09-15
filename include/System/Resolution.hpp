#ifndef SYSTEM_RESOLUTION_HPP
#define SYSTEM_RESOLUTION_HPP

#include <dolphin/types.h>

#ifdef VERSION_GMSP01
u16 SMSGetGCLogoVideoHeight(u32 format);
#else
u16 SMSGetGCLogoVideoHeight();
#endif
u16 SMSGetGCLogoVideoWidth();
u16 SMSGetGCLogoRenderHeight();
u16 SMSGetGCLogoRenderWidth();
u16 SMSGetTitleRenderHeight();
u16 SMSGetTitleRenderWidth();
u16 SMSGetGameRenderHeight();
u16 SMSGetGameRenderWidth();
#ifdef VERSION_GMSP01
u16 SMSGetTitleVideoHeight(u32 format);
#else
u16 SMSGetTitleVideoHeight();
#endif
u16 SMSGetTitleVideoWidth();
s32 SMSGetDisplayHeightMax();
s32 SMSGetDisplayWidthMax();
#ifdef VERSION_GMSP01
u16 SMSGetGameVideoHeight(u32 format);
#else
u16 SMSGetGameVideoHeight();
#endif
u16 SMSGetGameVideoWidth();

#endif
