#ifndef J3D_TEVS_HPP
#define J3D_TEVS_HPP

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <JSystem/JRenderer.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

extern const J3DLightInfo j3dDefaultLightInfo;
extern const J3DTexCoordInfo j3dDefaultTexCoordInfo[8];
extern const J3DTexMtxInfo j3dDefaultTexMtxInfo;
extern const J3DIndTexMtxInfo j3dDefaultIndTexMtxInfo;
extern const J3DTevStageInfo j3dDefaultTevStageInfo;
extern const J3DIndTevStageInfo j3dDefaultIndTevStageInfo;
extern const J3DFogInfo j3dDefaultFogInfo;
extern const J3DNBTScaleInfo j3dDefaultNBTScaleInfo;

extern const GXColor j3dDefaultColInfo;
extern const GXColor j3dDefaultAmbInfo;
extern const u8 j3dDefaultColorChanNum;
extern const J3DTevOrderInfo j3dDefaultTevOrderInfoNull;
extern const J3DIndTexOrderInfo j3dDefaultIndTexOrderNull;
extern const GXColorS10 j3dDefaultTevColor;
extern const J3DIndTexCoordScaleInfo j3dDefaultIndTexCoordScaleInfo;
extern const GXColor j3dDefaultTevKColor;
extern const J3DTevSwapModeInfo j3dDefaultTevSwapMode;
extern const J3DTevSwapModeTableInfo j3dDefaultTevSwapModeTable;
extern const J3DBlendInfo j3dDefaultBlendInfo;
extern const J3DColorChanInfo j3dDefaultColorChanInfo;
extern const u8 j3dDefaultTevSwapTableID;
extern const u16 j3dDefaultAlphaCmpID;
extern const u16 j3dDefaultZModeID;

extern u8 j3dTexCoordTable[7624];
extern u8 j3dTevSwapTableTable[1024];
extern u8 j3dAlphaCmpTable[768];
extern u8 j3dZModeTable[96];

void J3DLoadCPCmd(u8, u32);

void J3DLoadArrayBasePtr(GXAttr, void*);
void J3DSetVtxAttrFmtv(GXVtxFmt, GXVtxAttrFmtList*, bool);

void J3DGDSetTexLookupMode(GXTexMapID id, GXTexWrapMode wrapS,
                           GXTexWrapMode wrapT, GXTexFilter minFilter,
                           GXTexFilter magFilter, f32 minLOD, f32 maxLOD,
                           f32 lodBias, u8 biasClamp, u8 edgeLOD,
                           GXAnisotropy maxAniso);

#endif
