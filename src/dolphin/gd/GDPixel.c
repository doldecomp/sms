#include <dolphin/gd/GDPixel.h>
#include <dolphin/gd/GDBase.h>

void GDSetBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                    GXBlendFactor dst_factor, GXLogicOp logic_op)
{
	GDWriteBPCmd(0xfe001fe3);
	GDWriteBPCmd(BP_BLEND_MODE(
	    type == GX_BM_BLEND || type == GX_BM_SUBTRACT, type == GX_BM_LOGIC, 0,
	    0, 0, dst_factor, src_factor, type == GX_BM_SUBTRACT, logic_op, 0x41));
}

void GDSetBlendModeEtc(GXBlendMode type, GXBlendFactor src_factor,
                       GXBlendFactor dst_factor, GXLogicOp logic_op,
                       u8 color_update_enable, u8 alpha_update_enable,
                       u8 dither_enable)
{
	GDWriteBPCmd(BP_BLEND_MODE(
	    type == GX_BM_BLEND || type == GX_BM_SUBTRACT, type == GX_BM_LOGIC,
	    dither_enable, color_update_enable, alpha_update_enable, dst_factor,
	    src_factor, type == GX_BM_SUBTRACT, logic_op, 0x41));
}

void GDSetZMode(u8 compare_enable, GXCompare func, u8 update_enable)
{
	GDWriteBPCmd(BP_Z_MODE(compare_enable, func, update_enable, 0x40));
}

void GDSetDstAlpha(u8 enable, u8 alpha)
{
	GDWriteBPCmd(BP_DST_ALPHA(alpha, enable, 0x42));
}
