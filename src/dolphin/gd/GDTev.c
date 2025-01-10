#include <dolphin/gd/GDTev.h>
#include "__gd.h"

void GDSetTevOp(GXTevStageID stage, GXTevMode mode)
{
	GXTevColorArg carg = GX_CC_RASC;
	GXTevAlphaArg aarg = GX_CA_RASA;

	if (stage != 0) {
		carg = GX_CC_CPREV;
		aarg = GX_CA_APREV;
	}

	switch (mode) {
	case GX_MODULATE:
		GDSetTevColorCalc(stage, 0xf, 8, carg, 0xf, 0, 0, 0, 1, 0);
		GDSetTevAlphaCalcAndSwap(stage, 7, 4, aarg, 7, 0, 0, 0, 1, 0, 0, 0);
		break;
	case GX_DECAL:
		GDSetTevColorCalc(stage, carg, 8, 9, 0xf, 0, 0, 0, 1, 0);
		GDSetTevAlphaCalcAndSwap(stage, 7, 7, 7, aarg, 0, 0, 0, 1, 0, 0, 0);
		break;
	case GX_BLEND:
		GDSetTevColorCalc(stage, carg, 0xc, 8, 0xf, 0, 0, 0, 1, 0);
		GDSetTevAlphaCalcAndSwap(stage, 7, 4, aarg, 7, 0, 0, 0, 1, 0, 0, 0);
		break;
	case GX_REPLACE:
		GDSetTevColorCalc(stage, 0xf, 0xf, 0xf, 8, 0, 0, 0, 1, 0);
		GDSetTevAlphaCalcAndSwap(stage, 7, 7, 7, 4, 0, 0, 0, 1, 0, 0, 0);
		break;
	case GX_PASSCLR:
		GDSetTevColorCalc(stage, 0xf, 0xf, 0xf, carg, 0, 0, 0, 1, 0);
		GDSetTevAlphaCalcAndSwap(stage, 7, 7, 7, aarg, 0, 0, 0, 1, 0, 0, 0);
		break;
	}
}

void GDSetTevColorCalc(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b,
                       GXTevColorArg c, GXTevColorArg d, GXTevOp op,
                       GXTevBias bias, GXTevScale scale, u8 clamp,
                       GXTevRegID out_reg)
{
	if (op <= 1) {
		GDWriteBPCmd(out_reg << 0x16 | scale << 0x14 | (u32)clamp << 0x13
		             | (op & 1) << 0x12 | bias << 0x10 | a << 0xc | b << 8 | d
		             | c << 4
		             | (u32)((((u32)stage & 0x7fffffff) * 2 + 0xc0 & 0xffffffff)
		                     << 0x18));
	} else {
		GDWriteBPCmd(out_reg << 0x16 | (op & 6) << 0x13 | (u32)clamp << 0x13
		             | (op & 1) << 0x12 | a << 0xc | b << 8 | d | c << 4
		             | 0x30000
		             | (u32)((((u32)stage & 0x7fffffff) * 2 + 0xc0 & 0xffffffff)
		                     << 0x18));
	}
}

void GDSetTevAlphaCalcAndSwap(GXTevStageID stage, GXTevAlphaArg a,
                              GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d,
                              GXTevOp op, GXTevBias bias, GXTevScale scale,
                              u8 clamp, GXTevRegID out_reg,
                              GXTevSwapSel ras_sel, GXTevSwapSel tex_sel)
{
	if (op <= 1) {
		GDWriteBPCmd(
		    out_reg << 0x16 | scale << 0x14 | clamp << 0x13 | (op & 1) << 0x12
		    | bias << 0x10 | a << 0xd | b << 10 | c << 7 | d << 4 | ras_sel
		    | tex_sel << 2
		    | (((stage & 0x7fffffff) * 2 + 0xc1 & 0xffffffff) << 0x18));
	} else {
		GDWriteBPCmd(
		    out_reg << 0x16 | (op & 6) << 0x13 | clamp << 0x13
		    | (op & 1) << 0x12 | a << 0xd | b << 10 | c << 7 | d << 4 | ras_sel
		    | tex_sel << 2 | 0x30000
		    | (((stage & 0x7fffffff) * 2 + 0xc1 & 0xffffffff) << 0x18));
	}
}

void GDSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1,
                       u8 ref1)
{
	GDWriteBPCmd(ref0 | ref1 << 0x8 | comp0 << 0x10 | comp1 << 0x13 | op << 0x16
	             | 0xf3000000);
}

void GDSetTevOrder(GXTevStageID evenStage, GXTexCoordID coord0, GXTexMapID map0,
                   GXChannelID color0, GXTexCoordID coord1, GXTexMapID map1,
                   GXChannelID color1)
{
	static u8 c2r[] = { 0, 1, 0, 1, 0, 1, 7, 5, 6, 0, 0, 0, 0, 0, 0, 7 };

	u32 reg = 0;

	reg |= map0 & 7;
	reg |= (coord0 & 7) << 3;
	reg |= ((map0 != 0xff) && !(map0 & 0x100)) << 6;
	reg |= c2r[color0 & 0xf] << 7;
	reg |= (map1 & 7) << 0xc;
	reg |= (coord1 & 7) << 0xf;
	reg |= ((map1 != 0xff) && !(map1 & 0x100)) << 0x12;
	reg |= c2r[color1 & 0xf] << 0x13;
	reg |= ((evenStage >> 1) + (u32)(evenStage < 0 && (evenStage & 1)) + 0x28)
	       << 24;

	GDWriteBPCmd(reg);
}
