#include <dolphin/gd/GDPixel.h>
#include "__gd.h"

void GDSetBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                    GXBlendFactor dst_factor, GXLogicOp logic_op)
{
	GDWriteBPCmd(0xfe001fe3);
	// clang-format off
	GDWriteBPCmd(
		((type == GX_BM_BLEND) || (type == GX_BM_SUBTRACT))
		| (type == GX_BM_LOGIC) << 1
		| dst_factor << 5
		| src_factor << 8
		| (type == GX_BM_SUBTRACT) << 11
		| logic_op << 12
		| 1u << 24
		| 1u << 30
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
		((type == GX_BM_BLEND) || (type == GX_BM_SUBTRACT))
		| (type == GX_BM_LOGIC) << 1
		| dither_enable << 2
		| color_update_enable << 3
		| alpha_update_enable << 4
		| dst_factor << 5
		| src_factor << 8
		| (type == GX_BM_SUBTRACT) << 11
		| logic_op << 12
		| 1u << 24
		| 1u << 30
	);
	// clang-format on
}

void GDSetZMode(u8 compare_enable, GXCompare func, u8 update_enable)
{
	// clang-format off
	GDWriteBPCmd(
		compare_enable
		| func << 1
		| update_enable << 4
		| 1u << 30
	);
	// clang-format on
}

void GDSetDstAlpha(u8 enable, u8 alpha)
{
	// clang-format off
	GDWriteBPCmd(
		alpha
		| enable << 8
		| 1 << 25
		| 1 << 30
	);
	// clang-format on
}
