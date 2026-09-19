#ifndef SYSTEM_RESOLUTION_HPP
#define SYSTEM_RESOLUTION_HPP

#include <dolphin/types.h>
#include <dolphin/vi.h>

u16 SMSGetGCLogoVideoWidth();
u16 SMSGetGCLogoRenderHeight();
u16 SMSGetGCLogoRenderWidth();
u16 SMSGetTitleRenderHeight();
u16 SMSGetTitleRenderWidth();
u16 SMSGetGameRenderHeight();
u16 SMSGetGameRenderWidth();
u16 SMSGetTitleVideoWidth();
s32 SMSGetDisplayHeightMax();
s32 SMSGetDisplayWidthMax();
u16 SMSGetGameVideoWidth();

// PAL hoists the VIGetTvFormat() query out of these functions and passes the
// TV format in as an argument instead. The default argument keeps the existing
// no-argument call sites valid without affecting the mangled name (__FUl).
#ifdef VERSION_GMSP01
u16 SMSGetGCLogoVideoHeight(u32 format = VIGetTvFormat());
u16 SMSGetTitleVideoHeight(u32 format = VIGetTvFormat());
u16 SMSGetGameVideoHeight(u32 format = VIGetTvFormat());
#else
u16 SMSGetGCLogoVideoHeight();
u16 SMSGetTitleVideoHeight();
u16 SMSGetGameVideoHeight();
#endif

#endif
