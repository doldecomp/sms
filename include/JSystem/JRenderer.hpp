#ifndef J_RENDERER_HPP
#define J_RENDERER_HPP

#include <dolphin/gx.h>
#include <dolphin/mtx.h>

void JRNLoadTexCached(GXTexMapID, u32, GXTexCacheSize, u32, GXTexCacheSize);
void JRNLoadCurrentMtx(u32, u32, u32, u32, u32, u32, u32, u32, u32);
void J3DGDLoadTexMtxImm(MtxPtr, u32, GXTexMtxType);
void J3DGDSetTexTlut(GXTexMapID, u32, GXTlutFmt);

#endif
