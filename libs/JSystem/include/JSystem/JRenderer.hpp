#ifndef J_RENDERER_HPP
#define J_RENDERER_HPP

#include <dolphin/gx.h>
#include <dolphin/gd.h>
#include <dolphin/mtx.h>

inline void J3DGDWrite_u8(u8 param) { __GDWrite(param); }

inline void J3DGDWrite_u16(u16 param)
{
	__GDWrite((u8)(param >> 8));
	__GDWrite((u8)(param & 0xff));
}

inline void J3DGDWrite_u32(u32 param)
{
	__GDWrite((u8)((param >> 24) & 0xff));
	__GDWrite((u8)((param >> 16) & 0xff));
	__GDWrite((u8)((param >> 8) & 0xff));
	__GDWrite((u8)((param >> 0) & 0xff));
}

inline void J3DGDWrite_f32(f32 param)
{
	union {
		float f;
		u32 u;
	} fid;
	fid.f = param;
	J3DGDWrite_u32(fid.u);
}

inline void J3DGDWriteBPCmd(u32 cmd)
{
	J3DGDWrite_u8(GX_CMD_LOAD_BP_REG);
	J3DGDWrite_u32(cmd);
}

// Made up
inline void J3DGDWriteBPCmdCheck(u32 cmd)
{
	GDOverflowCheck(5);
	J3DGDWrite_u8(GX_CMD_LOAD_BP_REG);
	J3DGDWrite_u32(cmd);
}

inline void J3DGDWriteCPCmd(u8 cmd, u32 param)
{
	J3DGDWrite_u8(GX_CMD_LOAD_CP_REG);
	J3DGDWrite_u8(cmd);
	J3DGDWrite_u32(param);
}

inline void J3DGDWriteXFCmd(u16 cmd, u32 param)
{
	J3DGDWrite_u8(GX_CMD_LOAD_XF_REG);
	J3DGDWrite_u16(0);
	J3DGDWrite_u16(cmd);
	J3DGDWrite_u32(param);
}

inline void J3DGDWriteXFCmdHdr(u16 cmd, u8 len)
{
	J3DGDWrite_u8(GX_CMD_LOAD_XF_REG);
	J3DGDWrite_u16(len - 1);
	J3DGDWrite_u16(cmd);
}

/**
 * The functions below set the same graphics state as the GX library.
 * See SDK documentation for the relevant GX functions.
 */

/**
 * @brief Loads a texture matrix.
 * @see GXLoadTexMtxImm
 */
void J3DGDLoadTexMtxImm(MtxPtr mtx, u32 id, GXTexMtxType type);

/**
 * @brief Sets the ambient colour of a lighting channel.
 * @see GXSetChanAmbColor
 */
void J3DGDSetChanAmbColor(GXChannelID chan, GXColor color);

/**
 * @brief Sets the material colour of a lighting channel.
 * @see GXSetChanMatColor
 */
void J3DGDSetChanMatColor(GXChannelID chan, GXColor color);

/**
 * @brief Sets how a lighting channel makes its colour.
 * @see GXSetChanCtrl
 */
void J3DGDSetChanCtrl(GXChannelID chan, u8 enable, GXColorSrc amb_src,
                      GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn,
                      GXAttnFn attn_fn);

/**
 * @brief Sets how the hardware makes a texture coordinate.
 * @see GXSetTexCoordGen2
 */
void J3DGDSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func,
                         GXTexGenSrc src_param, u8 normalize, u32 postmtx);

/**
 * @brief Sets the fog.
 * @see GXSetFog
 */
void J3DGDSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz, f32 farz,
                 GXColor color);

/**
 * @brief Sets the size of a texture coordinate.
 * @see GXSetTexCoordScaleManually
 */
void J3DGDSetTexCoordScale(GXTexCoordID coord, u16 s_size, u16 t_size);

/**
 * @brief Sets the size, the bias and the cylindrical wrap of a texture
 * coordinate.
 * @see GXSetTexCoordScaleManually, GXSetTexCoordBias, GXSetTexCoordCylWrap
 */
void J3DGDSetTexCoordScale2(GXTexCoordID coord, u16 s_size, u8 s_bias,
                            u8 s_cyl_wrap, u16 t_size, u8 t_bias,
                            u8 t_cyl_wrap);

/**
 * @brief Sets the size and the format of a texture map.
 * @see GXInitTexObj, GXLoadTexObj
 */
void J3DGDSetTexImgAttr(GXTexMapID map, u16 width, u16 height, GXTexFmt format);

/**
 * @brief Gives a texture map its image data.
 * @see GXLoadTexObj
 */
void J3DGDSetTexImgPtr(GXTexMapID map, void* image);

/**
 * @brief Gives a texture map its image data at a physical address.
 * @see GXLoadTexObj
 */
void J3DGDSetTexImgPtrRaw(GXTexMapID map, u32 image);

/**
 * @brief Gives a texture map its colour lookup table.
 * @see GXInitTexObjTlut, GXLoadTexObj
 */
void J3DGDSetTexTlut(GXTexMapID map, u32 tlut_addr, GXTlutFmt format);

/**
 * @brief Loads a colour lookup table into texture memory.
 * @see GXLoadTlut
 */
void J3DGDLoadTlut(void* tlut, u32 tlut_addr, GXTlutSize entry_count);

/**
 * @brief Sets a TEV constant colour register.
 * @see GXSetTevKColor
 */
void J3DGDSetTevKColor(GXTevKColorID reg, GXColor color);

/**
 * @brief Sets the default matrices. A vertex that has no matrix index of its
 * own uses them.
 * @see GXSetCurrentMtx
 */
void JRNLoadCurrentMtx(u32 pn, u32 t0, u32 t1, u32 t2, u32 t3, u32 t4, u32 t5,
                       u32 t6, u32 t7);

/**
 * @brief Connects a TEV stage to an indirect stage.
 * @see GXSetTevIndirect
 */
void JRNSetTevIndirect(GXTevStageID tev_stage, GXIndTexStageID ind_stage,
                       GXIndTexFormat format, GXIndTexBiasSel bias_sel,
                       GXIndTexMtxID matrix_sel, GXIndTexWrap wrap_s,
                       GXIndTexWrap wrap_t, GXBool add_prev, GXBool utc_lod,
                       GXIndTexAlphaSel alpha_sel);

/**
 * @brief Disconnects a TEV stage from its indirect stage.
 * @see GXSetTevDirect
 */
void JRNSetTevDirect(GXTevStageID tev_stage);

/**
 * @brief Sets an indirect texture matrix.
 * @see GXSetIndTexMtx
 */
void JRNSetIndTexMtx(GXIndTexMtxID mtx_id, ROMtxPtr offset, s8 scale_exp);

/**
 * @brief Sets the coordinate scale of two indirect stages.
 * @see GXSetIndTexCoordScale
 */
void JRNSetIndTexCoordScale(GXIndTexStageID even_stage, GXIndTexScale scale_s0,
                            GXIndTexScale scale_t0, GXIndTexScale scale_s1,
                            GXIndTexScale scale_t1);

/**
 * @brief Gives each indirect stage its texture map and its texture coordinate.
 * @see GXSetIndTexOrder, GXSetNumIndStages
 */
void JRNSetIndTexOrder(u32 stage_count, GXTexCoordID coord0, GXTexMapID map0,
                       GXTexCoordID coord1, GXTexMapID map1,
                       GXTexCoordID coord2, GXTexMapID map2,
                       GXTexCoordID coord3, GXTexMapID map3);

/**
 * @brief Waits until the texture unit is idle.
 * @see __GXFlushTextureState
 */
void JRNFlushTextureState();

/**
 * @brief Points a texture map at a cached region of texture memory.
 * @see GXLoadTexObjPreLoaded, GXInitTexCacheRegion
 */
void JRNLoadTexCached(GXTexMapID map, u32 tmem_even, GXTexCacheSize size_even,
                      u32 tmem_odd, GXTexCacheSize size_odd);

/**
 * @brief Gives two TEV stages their texture map, their texture coordinate and
 * their colour channel.
 * @see GXSetTevOrder
 */
void JRNISetTevOrder(GXTevStageID even_stage, GXTexCoordID coord0,
                     GXTexMapID map0, GXChannelID color0, GXTexCoordID coord1,
                     GXTexMapID map1, GXChannelID color1);

/**
 * @brief Sets a TEV colour register from signed 10-bit components.
 * @see GXSetTevColorS10
 */
void JRNISetTevColorS10(GXTevRegID reg, GXColorS10 color);

/**
 * @brief Sets the fog range adjustment. It corrects the fog at the left and
 * the right of the screen.
 * @see GXSetFogRangeAdj
 */
void JRNISetFogRangeAdj(GXBool enable, u16 center, GXFogAdjTable* table);

/**
 * @brief Sets whether the depth test happens before the texture lookup.
 * @see GXSetZCompLoc
 */
inline void J3DGDSetZCompLoc(u32 compLocEnable)
{
	J3DGDWriteBPCmdCheck(0xFE000040);
	J3DGDWriteBPCmdCheck(compLocEnable << 6 | 0x43 << 24);
}

/**
 * @brief Sets the constant colour selection and the swap table of two TEV
 * stages.
 * @see GXSetTevKColorSel, GXSetTevKAlphaSel, GXSetTevSwapMode
 */
inline void J3DGDSetTevKonstantSel_SwapModeTable(
    GXTevStageID stage, GXTevKColorSel colorSel1, GXTevKAlphaSel alphaSel1,
    GXTevKColorSel colorSel2, GXTevKAlphaSel alphaSel2, GXTevColorChan chan1,
    GXTevColorChan chan2)
{
	GDOverflowCheck(5);
	// Probably a fake match but it really doesn't matter
	// real version of this is BP_TEV_KSEL
	// clang-format off
	J3DGDWriteBPCmd(
		((u32)chan1 |
		(u32)chan2 << 2 |
		(u32)colorSel1 << 4 |
		(u32)alphaSel1 << 9 |
		(u32)colorSel2 << 14 |
		(u32)alphaSel2 << 19) & 0x00FFFFFF
		| (u32)(stage / 2 + 0xF6) << 24
	);
	// clang-format on
}

#endif
