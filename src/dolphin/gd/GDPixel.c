#include <dolphin/gd/GDPixel.h>
#include "__gd.h"

void GDSetBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                    GXBlendFactor dst_factor, GXLogicOp logic_op)
{
	GDWriteBPCmd(0xfe001fe3);
	// clang-format off
	GDWriteBPCmd(
		((u32)(type == GX_BM_BLEND || type == GX_BM_SUBTRACT)) |
		((u32)(type == GX_BM_LOGIC)) << 1 |
		((u32)dst_factor) << 5 |
		((u32)src_factor) << 8 |
		((u32)(type == GX_BM_SUBTRACT)) << 11 |
		((u32)logic_op) << 12 |
		((u32)0x41) << 24
	);
	// clang-format on
}

void GDSetBlendModeEtc(GXBlendMode type, GXBlendFactor src_factor,
                       GXBlendFactor dst_factor, GXLogicOp logic_op,
                       u8 color_update_enable, u8 alpha_update_enable,
                       u8 dither_enable)
{
	// clang-format off
	GDWriteBPCmd(
		((u32)(type == GX_BM_BLEND || type == GX_BM_SUBTRACT)) |
		((u32)(type == GX_BM_LOGIC)) << 1 |
		((u32)dither_enable) << 2 |
		((u32)color_update_enable) << 3 |
		((u32)alpha_update_enable) << 4 |
		((u32)dst_factor) << 5 |
		((u32)src_factor) << 8 |
		((u32)(type == GX_BM_SUBTRACT)) << 11 |
		((u32)logic_op) << 12 |
		((u32)0x41) << 24
	);
	// clang-format on
}

void GDSetZMode(u8 compare_enable, GXCompare func, u8 update_enable)
{
	// clang-format off
	GDWriteBPCmd(
		((u32)compare_enable) |
		((u32)func) << 1 |
		((u32)update_enable) << 4 |
		((u32)1) << 30
	);
	// clang-format on
}

void GDSetDstAlpha(u8 enable, u8 alpha)
{
	// clang-format off
	GDWriteBPCmd(
		((u32)alpha) |
		((u32)enable) << 8 |
		((u32)1) << 25 |
		((u32)1) << 30
	);
	// clang-format on
}
