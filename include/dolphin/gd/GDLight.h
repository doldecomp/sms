#ifndef _DOLPHIN_GDLIGHT_H_
#define _DOLPHIN_GDLIGHT_H_

#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>

#define XF_AMBIENT0_ID  0x100a
#define XF_MATERIAL0_ID 0x100c

#ifdef __cplusplus
extern "C" {
#endif

void GDSetLightAttn(GXLightID light, float a0, float a1, float a2, float k0,
                    float k1, float k2);

void GDSetLightColor(GXLightID light, GXColor* color);

void GDSetLightPos(GXLightID light, float x, float y, float z);

void GDSetLightDir(GXLightID light, float nx, float ny, float nz);

void GDSetChanMatColor(GXChannelID chan, GXColor* color);

void GDSetChanCtrl(GXChannelID chan, u8 enable, GXColorSrc amb_src,
                   GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn,
                   GXAttnFn attn_fn);

#ifdef __cplusplus
}
#endif

#endif
