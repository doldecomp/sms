#ifndef _THP_THPDRAW_H
#define _THP_THPDRAW_H

#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

void THPGXRestore();
void THPGXYuv2RgbSetup(GXRenderModeObj* rmode);
void THPGXYuv2RgbDraw(u8* yTexture, u8* uTexture, u8* vTexture, s16 x, s16 y,
                      s16 textureWidth, s16 textureHeight, s16 polygonWidth,
                      s16 polygonHeight);

#ifdef __cplusplus
}
#endif

#endif
