#include <JSystem/JRenderer.hpp>
#include <dolphin/os.h>

void J3DGDLoadTexMtxImm(MtxPtr mtx, u32 i, GXTexMtxType type)
{
	u16 cmd;
	u8 len;
	if (i >= 0x40) {
		cmd = (i - 0x40) * 4 + 0x500;
		len = 12;
	} else {
		cmd = i * 4;
		len = type == GX_MTX2x4 ? 8 : 12;
	}

	GDOverflowCheck(0x25);
	J3DGDWriteXFCmdHdr(cmd, len);
	J3DGDWrite_f32(mtx[0][0]);
	J3DGDWrite_f32(mtx[0][1]);
	J3DGDWrite_f32(mtx[0][2]);
	J3DGDWrite_f32(mtx[0][3]);
	J3DGDWrite_f32(mtx[1][0]);
	J3DGDWrite_f32(mtx[1][1]);
	J3DGDWrite_f32(mtx[1][2]);
	J3DGDWrite_f32(mtx[1][3]);

	if (type == GX_MTX3x4) {
		GDOverflowCheck(0x10);
		J3DGDWrite_f32(mtx[2][0]);
		J3DGDWrite_f32(mtx[2][1]);
		J3DGDWrite_f32(mtx[2][2]);
		J3DGDWrite_f32(mtx[2][3]);
	}
}

void J3DGDSetChanAmbColor(GXChannelID chan, GXColor color)
{
	u32 c = color.r << 24 | color.g << 16 | color.b << 8 | color.a;
	GDOverflowCheck(0x9);
	J3DGDWriteXFCmd((chan & 1) + XF_REG_AMBIENT0_ID, c);
}

void J3DGDSetChanMatColor(GXChannelID chan, GXColor color)
{
	u32 c = color.r << 24 | color.g << 16 | color.b << 8 | color.a;
	GDOverflowCheck(0x9);
	J3DGDWriteXFCmd((chan & 1) + XF_REG_MATERIAL0_ID, c);
}

void J3DGDSetChanCtrl(GXChannelID chan, u8 enable, GXColorSrc amb_src,
                      GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn,
                      GXAttnFn attn_fn)
{
	u32 reg;

	reg = XF_REG_CHAN_CTRL(mat_src, enable, light_mask & 0xF, amb_src,
	                       attn_fn == GX_AF_SPEC ? GX_DF_NONE : diff_fn,
	                       attn_fn != GX_AF_NONE, attn_fn != GX_AF_SPEC,
	                       (light_mask >> 4) & 0xF);
	GDOverflowCheck(0x9);
	J3DGDWriteXFCmd((chan & 3) + XF_REG_COLOR0CNTRL_ID, reg);
	if ((chan == 4) || (chan == 5)) {
		GDOverflowCheck(0x9);
		J3DGDWriteXFCmd(chan + XF_REG_MATERIAL0_ID, reg);
	}
}

void J3DGDSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func,
                         GXTexGenSrc src_param, u8 normalize, u32 postmtx)
{
	u32 form = 0;
	u32 tgType;
	u32 proj      = 0;
	u32 row       = 5;
	u32 embossRow = 5;
	u32 embossLit = 0;

	switch (src_param) {
	case GX_TG_POS:
		row  = 0;
		form = 1;
		break;
	case GX_TG_NRM:
		row  = 1;
		form = 1;
		break;
	case GX_TG_BINRM:
		row  = 3;
		form = 1;
		break;
	case GX_TG_TANGENT:
		row  = 4;
		form = 1;
		break;
	case GX_TG_COLOR0:
		row = 2;
		break;
	case GX_TG_COLOR1:
		row = 2;
		break;
	case GX_TG_TEX0:
		row = 5;
		break;
	case GX_TG_TEX1:
		row = 6;
		break;
	case GX_TG_TEX2:
		row = 7;
		break;
	case GX_TG_TEX3:
		row = 8;
		break;
	case GX_TG_TEX4:
		row = 9;
		break;
	case GX_TG_TEX5:
		row = 10;
		break;
	case GX_TG_TEX6:
		row = 0xb;
		break;
	case GX_TG_TEX7:
		row = 0xc;
		break;
	case GX_TG_TEXCOORD0:
		embossRow = 0;
		break;
	case GX_TG_TEXCOORD1:
		embossRow = 1;
		break;
	case GX_TG_TEXCOORD2:
		embossRow = 2;
		break;
	case GX_TG_TEXCOORD3:
		embossRow = 3;
		break;
	case GX_TG_TEXCOORD4:
		embossRow = 4;
		break;
	case GX_TG_TEXCOORD5:
		embossRow = 5;
		break;
	case GX_TG_TEXCOORD6:
		embossRow = 6;
		break;
	default:
		break;
	}

	switch (func) {
	case GX_TG_MTX2x4:
		tgType = 0;
		break;
	case GX_TG_MTX3x4:
		tgType = 0;
		proj   = 1;
		break;

	case GX_TG_BUMP0:
	case GX_TG_BUMP1:
	case GX_TG_BUMP2:
	case GX_TG_BUMP3:
	case GX_TG_BUMP4:
	case GX_TG_BUMP5:
	case GX_TG_BUMP6:
	case GX_TG_BUMP7:
		tgType    = 1;
		embossLit = func - GX_TG_BUMP0;
		break;

	case GX_TG_SRTG:
		if (src_param == GX_TG_COLOR0) {
			tgType = 2;
		} else {
			tgType = 3;
		}
		break;

	default:
		break;
	}

	GDOverflowCheck(0x12);
	J3DGDWriteXFCmd(dst_coord + XF_REG_TEX0_ID,
	                XF_REG_TEX(proj, form, tgType, row, embossRow, embossLit));
	J3DGDWriteXFCmd(dst_coord + XF_REG_DUALTEX0_ID,
	                XF_REG_DUALTEX(postmtx - 0x40, normalize));
}

void J3DGDSetFog(GXFogType type, float startz, float endz, float nearz,
                 float farz, GXColor color)
{
	float A;
	float B;
	float B_mant;
	float C;
	float A_f;
	u32 b_expn;
	u32 b_m;
	u32 a_hex;
	u32 c_hex;

	if ((farz == nearz) || (endz == startz)) {
		A = 0.0f;
		B = 0.5f;
		C = 0.0f;
	} else {
		A = (farz * nearz) / ((farz - nearz) * (endz - startz));
		B = (farz / (farz - nearz));
		C = (startz / (endz - startz));
	}

	B_mant = B;
	b_expn = 1;
	while (B_mant > 1.0) {
		B_mant *= 0.5f;
		b_expn++;
	}
	while (B_mant > 0.0f && B_mant < 0.5) {
		B_mant *= 2.0f;
		b_expn--;
	}

	A_f = A / (1 << b_expn);
	b_m = (u32)(8388638.0f * B_mant);

	a_hex = *(u32*)&A_f;
	c_hex = *(u32*)&C;

	GDOverflowCheck(0x19);
	J3DGDWriteBPCmd(BP_FOG_UNK0(a_hex >> 12, 0xee));
	J3DGDWriteBPCmd(BP_FOG_UNK1(b_m, 0xef));
	J3DGDWriteBPCmd(BP_FOG_UNK2(b_expn, 0xf0));
	J3DGDWriteBPCmd(BP_FOG_UNK3(c_hex >> 12, 0, type, 0xf1));
	J3DGDWriteBPCmd(BP_FOG_COLOR(color.r, color.g, color.b, 0xf2));
}

void J3DGDSetTexCoordScale2(GXTexCoordID param_1, u16 param_2, u8 param_3,
                            u8 param_4, u16 param_5, u8 param_6, u8 param_7)
{
	GDOverflowCheck(0xf);
	J3DGDWriteBPCmd(0xfe03ffff);
	// clang-format off
	J3DGDWriteBPCmd(
		(u32)(param_2 - 1) |
		(u32)(param_3) << 16 |
		(u32)(param_4) << 17 |
		(u32)(param_1 * 2 + 0x30) << 24
	);
	J3DGDWriteBPCmd(
		(u32)(param_5 - 1) |
		(u32)(param_6) << 16 |
		(u32)(param_7) << 17 |
		(u32)(param_1 * 2 + 0x31) << 24
	);
	// clang-format on
}

void J3DGDSetTexImgAttr(GXTexMapID param_1, u16 param_2, u16 param_3,
                        GXTexFmt param_4)
{
	// clang-format off
	u32 reg =
		(u32) (param_2 - 1) |
		(u32) (param_3 - 1) << 10 |
		(u32) param_4 << 20 |
		(u32) GXTexImage0Ids[param_1] << 24;
	// clang-format on

	GDOverflowCheck(5);
	J3DGDWriteBPCmd(reg);
}

void J3DGDSetTexImgPtr(GXTexMapID param_1, void* param_2)
{
	// clang-format off
	u32 reg =
		(u32) ((u32)OSPhysicalToCached(param_2) >> 5) |
		(u32) GXTexImage3Ids[param_1] << 24;
	// clang-format on

	GDOverflowCheck(5);
	J3DGDWriteBPCmd(reg);
}

void J3DGDSetTexTlut(GXTexMapID param_1, u32 param_2, GXTlutFmt param_3)
{
	// clang-format off
	u32 reg =
		(param_2 - 0x80000) >> 9 |
		param_3 << 10 |
		GXTexTlutIds[param_1] << 24
		;
	// clang-format on

	GDOverflowCheck(5);
	J3DGDWriteBPCmd(reg);
}

void J3DGDLoadTlut(void* param_1, u32 param_2, GXTlutSize param_3)
{
	GDOverflowCheck(5);
	J3DGDWriteBPCmd(0xfeffff00);
	GDOverflowCheck(5);
	J3DGDWriteBPCmd(0x0f000000);
	GDOverflowCheck(5);
	// clang-format off
	J3DGDWriteBPCmd(
		(u32) ((u32)OSPhysicalToCached(param_1) >> 5) |
		(u32) 0x64 << 24
	);
	// clang-format on
	GDOverflowCheck(5);
	// clang-format off
	J3DGDWriteBPCmd(
		(param_2 - 0x80000) >> 9 |
		param_3 << 10 |
		0x65 << 24
	);
	// clang-format on
	GDOverflowCheck(5);
	J3DGDWriteBPCmd(0xfeffff00);
	GDOverflowCheck(5);
	J3DGDWriteBPCmd(0x0f000000);
}

void J3DGDSetTevKColor(GXTevKColorID reg, GXColor color)
{
	u32 regRA;
	u32 regBG;

	regRA = BP_TEV_COLOR_REG_RA(color.r, color.a, 1, 0xE0 + reg * 2);
	regBG = BP_TEV_COLOR_REG_BG(color.b, color.g, 1, 0xE1 + reg * 2);

	GDOverflowCheck(10);
	J3DGDWriteBPCmd(regRA);
	J3DGDWriteBPCmd(regBG);
}

void JRNLoadCurrentMtx(u32, u32, u32, u32, u32, u32, u32, u32, u32) { }

void JRNSetTevIndirect(GXTevStageID, GXIndTexStageID, GXIndTexFormat,
                       GXIndTexBiasSel, GXIndTexMtxID, GXIndTexWrap,
                       GXIndTexWrap, u8, u8, GXIndTexAlphaSel)
{
}

void JRNSetIndTexMtx(GXIndTexMtxID, ROMtxPtr, s8) { }

void JRNSetIndTexCoordScale(GXIndTexStageID, GXIndTexScale, GXIndTexScale,
                            GXIndTexScale, GXIndTexScale)
{
}

void JRNSetIndTexOrder(u32, GXTexCoordID, GXTexMapID, GXTexCoordID, GXTexMapID,
                       GXTexCoordID, GXTexMapID, GXTexCoordID, GXTexMapID)
{
}

void JRNLoadTexCached(GXTexMapID, u32, GXTexCacheSize, u32, GXTexCacheSize) { }

void JRNISetTevOrder(GXTevStageID, GXTexCoordID, GXTexMapID, GXChannelID,
                     GXTexCoordID, GXTexMapID, GXChannelID)
{
}

void JRNISetTevColorS10(GXTevRegID, GXColorS10) { }

void JRNISetFogRangeAdj(u8, u16, GXFogAdjTable*) { }
