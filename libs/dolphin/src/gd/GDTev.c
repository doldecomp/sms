#include <dolphin/gd/GDTev.h>
#include <dolphin/gd/GDBase.h>

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
		GDSetTevColorCalc(stage, GX_CC_ZERO, GX_CC_TEXC, carg, GX_CC_ZERO,
		                  GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		GDSetTevAlphaCalcAndSwap(stage, GX_CA_ZERO, GX_CA_TEXA, aarg,
		                         GX_CA_ZERO, GX_TEV_ADD, GX_TB_ZERO,
		                         GX_CS_SCALE_1, 1, GX_TEVPREV, GX_TEV_SWAP0,
		                         GX_TEV_SWAP0);
		break;
	case GX_DECAL:
		GDSetTevColorCalc(stage, carg, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO,
		                  GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		GDSetTevAlphaCalcAndSwap(stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
		                         aarg, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                         GX_TEVPREV, GX_TEV_SWAP0, GX_TEV_SWAP0);
		break;
	case GX_BLEND:
		GDSetTevColorCalc(stage, carg, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO,
		                  GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		GDSetTevAlphaCalcAndSwap(stage, GX_CA_ZERO, GX_CA_TEXA, aarg,
		                         GX_CA_ZERO, GX_TEV_ADD, GX_TB_ZERO,
		                         GX_CS_SCALE_1, 1, GX_TEVPREV, GX_TEV_SWAP0,
		                         GX_TEV_SWAP0);
		break;
	case GX_REPLACE:
		GDSetTevColorCalc(stage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC,
		                  GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		GDSetTevAlphaCalcAndSwap(stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
		                         GX_CA_TEXA, GX_TEV_ADD, GX_TB_ZERO,
		                         GX_CS_SCALE_1, 1, GX_TEVPREV, GX_TEV_SWAP0,
		                         GX_TEV_SWAP0);
		break;
	case GX_PASSCLR:
		GDSetTevColorCalc(stage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, carg,
		                  GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
		GDSetTevAlphaCalcAndSwap(stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
		                         aarg, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                         GX_TEVPREV, GX_TEV_SWAP0, GX_TEV_SWAP0);
		break;
	default:
		break;
	}
}

void GDSetTevColorCalc(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b,
                       GXTevColorArg c, GXTevColorArg d, GXTevOp op,
                       GXTevBias bias, GXTevScale scale, u8 clamp,
                       GXTevRegID out_reg)
{
	if (op <= GX_TEV_SUB) {
		GDWriteBPCmd(BP_TEV_COLOR(d, c, b, a, bias, op & 1, clamp, scale,
		                          out_reg, stage * 2 + 0xC0));
	} else {
		GDWriteBPCmd(BP_TEV_COLOR(d, c, b, a, 3, op & 1, clamp, (op >> 1) & 3,
		                          out_reg, stage * 2 + 0xC0));
	}
}

void GDSetTevAlphaCalcAndSwap(GXTevStageID stage, GXTevAlphaArg a,
                              GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d,
                              GXTevOp op, GXTevBias bias, GXTevScale scale,
                              u8 clamp, GXTevRegID out_reg,
                              GXTevSwapSel ras_sel, GXTevSwapSel tex_sel)
{
	if (op <= GX_TEV_SUB) {
		GDWriteBPCmd(BP_TEV_ALPHA(ras_sel, tex_sel, d, c, b, a, bias, op & 1,
		                          clamp, scale, out_reg, stage * 2 + 0xC1));
	} else {
		GDWriteBPCmd(BP_TEV_ALPHA(ras_sel, tex_sel, d, c, b, a, 3, op & 1,
		                          clamp, (op >> 1) & 3, out_reg,
		                          stage * 2 + 0xC1));
	}
}

void GDSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1,
                       u8 ref1)
{
	GDWriteBPCmd(BP_ALPHA_COMPARE(ref0, ref1, comp0, comp1, op, 0xF3));
}

void GDSetTevOrder(GXTevStageID evenStage, GXTexCoordID coord0, GXTexMapID map0,
                   GXChannelID color0, GXTexCoordID coord1, GXTexMapID map1,
                   GXChannelID color1)
{
	static u8 c2r[] = { 0, 1, 0, 1, 0, 1, 7, 5, 6, 0, 0, 0, 0, 0, 0, 7 };

	GDWriteBPCmd(
	    BP_TEV_ORDER(map0 & 7, coord0 & 7,
	                 map0 != GX_TEXMAP_NULL && !(map0 & GX_TEX_DISABLE),
	                 c2r[color0 & 0xF], map1 & 7, coord1 & 7,
	                 map1 != GX_TEXMAP_NULL && !(map1 & GX_TEX_DISABLE),
	                 c2r[color1 & 0xF], evenStage / 2 + 0x28));
}
