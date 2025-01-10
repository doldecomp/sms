#ifndef _DOLPHIN_GDGEOMETRY_H_
#define _DOLPHIN_GDGEOMETRY_H_

#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDSetVtxDescv(const GXVtxDescList* attrPtr);
void GDSetVtxAttrFmtv(GXVtxFmt vtxfmt, const GXVtxAttrFmtList* list);
void GDSetArray(GXAttr attr, void* base_ptr, u8 stride);
void GDSetArrayRaw(GXAttr attr, u32 base_ptr_raw, u8 stride);
void GDSetCullMode(GXCullMode mode);
void GDSetGenMode2(u8 nTexGens, u8 nChans, u8 nTevs, u8 nInds, GXCullMode cm);

#ifdef __cplusplus
}
#endif

#endif
