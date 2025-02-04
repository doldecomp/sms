#ifndef J_RENDERER_HPP
#define J_RENDERER_HPP

#include <dolphin/gx.h>
#include <dolphin/gd.h>
#include <dolphin/mtx.h>

inline void J3DGDWrite_u8(u8 param) {
    __GDWrite(param);
}

inline void J3DGDWrite_u16(u16 param) {
    __GDWrite(param >> 8);
    __GDWrite(param & 0xff);
}

inline void J3DGDWrite_u32(u32 param) {
    __GDWrite(param >> 24);
    __GDWrite((param >> 16) & 0xff);
    __GDWrite((param >> 8) & 0xff);
    __GDWrite(param & 0xff);
}

inline void J3DGDWrite_f32(f32 param) {
	union {
		float f;
		u32 u;
	} fid;
	fid.f = param;
    J3DGDWrite_u32(fid.u);
}

inline void J3DGDWriteBPCmd(u32 cmd) {
    J3DGDWrite_u8(GX_CMD_LOAD_BP_REG);
    J3DGDWrite_u32(cmd);
}

inline void J3DGDWriteCPCmd(u8 cmd, u32 param) {
    J3DGDWrite_u8(GX_CMD_LOAD_CP_REG);
    J3DGDWrite_u8(cmd);
    J3DGDWrite_u32(param);
}

inline void J3DGDWriteXFCmd(u16 cmd, u32 param) {
    J3DGDWrite_u8(GX_CMD_LOAD_XF_REG);
    J3DGDWrite_u16(0);
    J3DGDWrite_u16(cmd);
    J3DGDWrite_u32(param);
}

inline void J3DGDWriteXFCmdHdr(u16 cmd, u8 len) {
    J3DGDWrite_u8(GX_CMD_LOAD_XF_REG);
    J3DGDWrite_u16(len - 1);
    J3DGDWrite_u16(cmd);
}

void J3DGDLoadTexMtxImm(MtxPtr, u32, GXTexMtxType);
void J3DGDSetChanAmbColor(GXChannelID, GXColor);
void J3DGDSetChanMatColor(GXChannelID, GXColor);
void J3DGDSetChanCtrl(GXChannelID, u8, GXColorSrc, GXColorSrc, u32, GXDiffuseFn, GXAttnFn);
void J3DGDSetTexCoordGen(GXTexCoordID, GXTexGenType, GXTexGenSrc, u8, u32);
void J3DGDSetFog(GXFogType, f32, f32, f32, f32, GXColor);
void J3DGDSetTexCoordScale(GXTexCoordID, u16, u16);
void J3DGDSetTexCoordScale2(GXTexCoordID, u16, u8, u8, u16, u8, u8);
void J3DGDSetTexImgAttr(GXTexMapID, u16, u16, GXTexFmt);
void J3DGDSetTexImgPtr(GXTexMapID, void*);
void J3DGDSetTexImgPtrRaw(GXTexMapID, u32);
void J3DGDSetTexTlut(GXTexMapID, u32, GXTlutFmt);
void J3DGDLoadTlut(void*, u32, GXTlutSize);
void J3DGDSetTevKColor(GXTevKColorID, GXColor);
void JRNLoadCurrentMtx(u32, u32, u32, u32, u32, u32, u32, u32, u32);
void JRNSetTevIndirect(GXTevStageID, GXIndTexStageID, GXIndTexFormat, GXIndTexBiasSel, GXIndTexMtxID, GXIndTexWrap, GXIndTexWrap, u8, u8, GXIndTexAlphaSel);
void JRNSetTevDirect(GXTevStageID);
void JRNSetIndTexMtx(GXIndTexMtxID, ROMtxPtr, s8);
void JRNSetIndTexCoordScale(GXIndTexStageID, GXIndTexScale, GXIndTexScale, GXIndTexScale, GXIndTexScale);
void JRNSetIndTexOrder(u32, GXTexCoordID, GXTexMapID, GXTexCoordID, GXTexMapID, GXTexCoordID, GXTexMapID, GXTexCoordID, GXTexMapID);
void JRNFlushTextureState();
void JRNLoadTexCached(GXTexMapID, u32, GXTexCacheSize, u32, GXTexCacheSize);
void JRNISetTevOrder(GXTevStageID, GXTexCoordID, GXTexMapID, GXChannelID, GXTexCoordID, GXTexMapID, GXChannelID);
void JRNISetTevColorS10(GXTevRegID, GXColorS10);
void JRNISetFogRangeAdj(u8, u16, GXFogAdjTable*);

#endif
