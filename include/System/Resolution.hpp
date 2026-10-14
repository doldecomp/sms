#ifndef SYSTEM_RESOLUTION_HPP
#define SYSTEM_RESOLUTION_HPP

#include <dolphin/types.h>
#include "version.h"

#if defined(VERSION_GMSP01_NX)
u16 SMSGetIntroRenderWidth();
u16 SMSGetIntroRenderHeight();
#endif

#if defined(VERSION_GMSP01) || defined(VERSION_GMSJ01_01) || defined(VERSION_GMSP01_NX)
u16 SMSGetGCLogoVideoHeight(u32 tvFormat);
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
#if defined(VERSION_GMSP01) || defined(VERSION_GMSJ01_01) || defined(VERSION_GMSP01_NX)
u16 SMSGetTitleVideoHeight(u32 tvFormat);
#else
u16 SMSGetTitleVideoHeight();
#endif
u16 SMSGetTitleVideoWidth();
s32 SMSGetDisplayHeightMax();
s32 SMSGetDisplayWidthMax();
#if defined(VERSION_GMSP01) || defined(VERSION_GMSJ01_01) || defined(VERSION_GMSP01_NX)
u16 SMSGetGameVideoHeight(u32 tvFormat);
#else
u16 SMSGetGameVideoHeight();
#endif
u16 SMSGetGameVideoWidth();

#endif
