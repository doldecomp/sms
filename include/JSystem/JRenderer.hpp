#ifndef J_RENDERER_HPP
#define J_RENDERER_HPP

#include <dolphin/gx.h>

void JRNLoadTexCached(GXTexMapID, u32, GXTexCacheSize, u32, GXTexCacheSize);
void JRNLoadCurrentMtx(u32, u32, u32, u32, u32, u32, u32, u32, u32);

#endif
