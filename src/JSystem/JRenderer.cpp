#include <JSystem/JRenderer.hpp>
#include <dolphin/os.h>

#pragma opt_strength_reduction off

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

void JRNLoadCurrentMtx(u32 param_1, u32 param_2, u32 param_3, u32 param_4,
                       u32 param_5, u32 param_6, u32 param_7, u32 param_8,
                       u32 param_9)
{
	// clang-format off
	u32 a =
		param_1 |
		param_2 << 6 |
		param_3 << 12 |
		param_4 << 18 |
		param_5 << 24;
	// clang-format on

	// clang-format off
	u32 b =
		param_6 |
		param_7 << 6 |
		param_8 << 12 |
		param_9 << 18;
	// clang-format on

	// TODO: this is all wrong, it should be using the CP register macros
	GXCmd1u8(0x8);
	GXCmd1u8(0x30);
	GXCmd1u32(a);

	GXCmd1u8(0x8);
	GXCmd1u8(0x40);
	GXCmd1u32(b);

	GXCmd1u8(0x10);
	GXCmd1u16(1);
	GXCmd1u16(0x1018);

	GXCmd1u32(a);
	GXCmd1u32(b);
}

void JRNSetTevIndirect(GXTevStageID param_1, GXIndTexStageID param_2,
                       GXIndTexFormat param_3, GXIndTexBiasSel param_4,
                       GXIndTexMtxID param_5, GXIndTexWrap param_6,
                       GXIndTexWrap param_7, u8 param_8, u8 param_9,
                       GXIndTexAlphaSel param_10)
{
	GDOverflowCheck(5);
	// clang-format off
	J3DGDWriteBPCmd(
		param_2 |
		param_3 << 2 |
		param_4 << 4 |
		param_10 << 7 |
		param_5 << 9 |
		param_6 << 13 |
		param_7 << 16 |
		param_9 << 19 |
		param_8 << 20 |
		(param_1 + 0x10) << 24
	);
	// clang-format on
}

void JRNSetIndTexMtx(GXIndTexMtxID mtx_id, ROMtxPtr offset, s8 scale_exp)
{
	s32 mtx[6];
	u32 reg;
	u32 id;
	switch (mtx_id) {
	case GX_ITM_0:
	case GX_ITM_1:
	case GX_ITM_2:
		id = mtx_id - GX_ITM_0;
		break;

	case GX_ITM_S0:
	case GX_ITM_S1:
	case GX_ITM_S2:
		id = mtx_id - GX_ITM_S0;
		break;

	case GX_ITM_T0:
	case GX_ITM_T1:
	case GX_ITM_T2:
		id = mtx_id - GX_ITM_T0;
		break;

	default:
		id = 0;
		break;
	}

	mtx[0] = (int)(1024.0f * offset[0][0]) & 0x7FF;
	mtx[1] = (int)(1024.0f * offset[1][0]) & 0x7FF;
	mtx[2] = (int)(1024.0f * offset[0][1]) & 0x7FF;
	mtx[3] = (int)(1024.0f * offset[1][1]) & 0x7FF;
	mtx[4] = (int)(1024.0f * offset[0][2]) & 0x7FF;
	mtx[5] = (int)(1024.0f * offset[1][2]) & 0x7FF;

	scale_exp += 0x11;

	GDOverflowCheck(15);
	J3DGDWriteBPCmd(mtx[0] | mtx[1] << 11 | ((scale_exp >> 0) & 3) << 22
	                | (3 * id + 6) << 24);
	J3DGDWriteBPCmd(mtx[2] | mtx[3] << 11 | ((scale_exp >> 2) & 3) << 22
	                | (3 * id + 7) << 24);
	J3DGDWriteBPCmd(mtx[4] | mtx[5] << 11 | ((scale_exp >> 4) & 3) << 22
	                | (3 * id + 8) << 24);
}

void JRNSetIndTexCoordScale(GXIndTexStageID param_1, GXIndTexScale param_2,
                            GXIndTexScale param_3, GXIndTexScale param_4,
                            GXIndTexScale param_5)
{
	GDOverflowCheck(5);
	// clang-format off
	J3DGDWriteBPCmd(
		param_2 |
		param_3 << 4 |
		param_4 << 8 |
		param_5 << 12 |
		((param_1 >> 1) + 0x25) << 24
	);
	// clang-format on
}

void JRNSetIndTexOrder(u32 param_1, GXTexCoordID param_2, GXTexMapID param_3,
                       GXTexCoordID param_4, GXTexMapID param_5,
                       GXTexCoordID param_6, GXTexMapID param_7,
                       GXTexCoordID param_8, GXTexMapID param_9)
{
	GDOverflowCheck(5);
	// clang-format off
	J3DGDWriteBPCmd(
		((u32)(param_3 & 0x7)) |
		((u32)(param_2 & 0x7)) << 3 |
		((u32)(param_5 & 0x7)) << 6 |
		((u32)(param_4 & 0x7)) << 9 |
		((u32)(param_7 & 0x7)) << 12 |
		((u32)(param_6 & 0x7)) << 15 |
		((u32)(param_9 & 0x7)) << 18 |
		((u32)(param_8 & 0x7)) << 21 |
		((u32)0x27) << 24
	);
	// clang-format on
	u32 thing = 0;
	for (u32 i = 0; i < param_1; ++i) {
		switch (i) {
		case 0:
			thing |= 1 << (param_3 & 0x7);
			break;

		case 1:
			thing |= 1 << (param_5 & 0x7);
			break;

		case 2:
			thing |= 1 << (param_7 & 0x7);
			break;

		case 3:
			thing |= 1 << (param_9 & 0x7);
			break;
		}
	}
	GDOverflowCheck(5);
	J3DGDWriteBPCmd(thing | 0xf << 24);
}

u8 JRNTexImage1Ids[8] = { 0x8c, 0x8d, 0x8e, 0x8f, 0xac, 0xad, 0xae, 0xaf };
u8 JRNTexImage2Ids[8] = { 0x90, 0x91, 0x92, 0x93, 0xb0, 0xb1, 0xb2, 0xb3 };

void JRNLoadTexCached(GXTexMapID param_1, u32 param_2, GXTexCacheSize param_3,
                      u32 param_4, GXTexCacheSize param_5)
{
	// TODO: this should use different GX stuff
	// clang-format off
	u32 reg1 =
		(param_2 >> 5) |
		(param_3 + 3) << 15 |
		(param_3 + 3) << 18 |
		JRNTexImage1Ids[param_1] << 24;
	// clang-format on
	GXCmd1u8(0x61);
	GXCmd1u32(reg1);
	if (param_5 == GX_TEXCACHE_NONE)
		return;
	if (param_4 >= 0x100000)
		return;
	// clang-format off
	u32 reg2 =
		(param_4 >> 5) |
		(param_5 + 3) << 15 |
		(param_5 + 3) << 18 |
		JRNTexImage2Ids[param_1] << 24;
	// clang-format on
	GXCmd1u8(0x61);
	GXCmd1u32(reg2);
}

void JRNISetTevOrder(GXTevStageID param_1, GXTexCoordID param_2,
                     GXTexMapID param_3, GXChannelID param_4,
                     GXTexCoordID param_5, GXTexMapID param_6,
                     GXChannelID param_7)
{
	static u8 c2r[16] = { 0, 1, 0, 1, 0, 1, 7, 5, 6, 0, 0, 0, 0, 0, 0, 7 };

	if (param_2 >= GX_MAX_TEXCOORD) {
		param_2 = GX_TEXCOORD0;
	}
	if (param_5 >= GX_MAX_TEXCOORD) {
		param_5 = GX_TEXCOORD0;
	}

	// clang-format off
	u32 reg =
		(u32)(param_3 & 0x7) |
		(u32)param_2 << 3 |
		(u32)(param_3 != 0xff && !(param_3 & 0x100)) << 6 |
		(u32)c2r[param_4 & 0xf] << 7 |
		(u32)(param_6 & 0x7) << 12 |
		(u32)param_5 << 15 |
		(u32)(param_6 != 0xff && !(param_6 & 0x100)) << 18 |
		(u32)c2r[param_7 & 0xf] << 19 |
		(u32)(param_1 / 2 + 0x28) << 24;
	// clang-format on

	GDOverflowCheck(5);
	J3DGDWriteBPCmd(reg);
}

void JRNISetTevColorS10(GXTevRegID reg, GXColorS10 color)
{
	u32 regRA;
	u32 regBG;

	regRA = BP_TEV_COLOR_REG_RA(color.r & 0x7FF, color.a & 0x7FF, 0,
	                            0xE0 + reg * 2);
	regBG = BP_TEV_COLOR_REG_BG(color.b & 0x7FF, color.g & 0x7FF, 0,
	                            0xE1 + reg * 2);

	GDOverflowCheck(20);
	J3DGDWriteBPCmd(regRA);
	J3DGDWriteBPCmd(regBG);
	J3DGDWriteBPCmd(regBG);
	J3DGDWriteBPCmd(regBG);
}

void JRNISetFogRangeAdj(GXBool enable, u16 center, GXFogAdjTable* table)
{
	if (enable) {
		for (int i = 0; i < 10; i += 2) {
			// clang-format off
			u32 reg =
				(0xE9 + (i / 2)) << 24 |
				table->r[i + 1] << 12 |
				table->r[i];
			// clang-format on
			GDOverflowCheck(5);
			J3DGDWriteBPCmd(reg);
		}
	}
	// clang-format off
	u32 reg =
		0xE8 << 24 |
		(center + 342) |
		enable << 10;
	// clang-format on
	GDOverflowCheck(5);
	J3DGDWriteBPCmd(reg);
}
