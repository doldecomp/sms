#include <dolphin/gd/GDPixel.h>
#include "__gd.h"

void GDSetBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                    GXBlendFactor dst_factor, GXLogicOp logic_op)
{
	GDWriteBPCmd(0xfe00ffe3);
	// clang-format off
	GDWriteBPCmd(
    ((type == 1) || (type == 3))
    | (type == 1) << 2 // offset wrong
    | (type == 3) << 1 // offset wrong
    | dst_factor << 5
    | src_factor << 8
    | logic_op << 0xc
    | 0x41000000);
	// clang-format on
}

void GDSetBlendModeEtc(GXBlendMode type, GXBlendFactor src_factor,
                       GXBlendFactor dst_factor, GXLogicOp logic_op,
                       u8 color_update_enable, u8 alpha_update_enable,
                       u8 dither_enable)
{
	// clang-format off
	GDWriteBPCmd(
    ((type == 1) || (type == 3))
    | dither_enable << 2
    | color_update_enable << 3
    | alpha_update_enable << 4
    | dst_factor << 5
    | src_factor << 8
    | logic_op << 0xc
    | (type == 3) << 1 // offset wrong
    | (type == 2) << 2 // offset wrong
    | 0x41000000);
	// clang-format on
}

void GDSetZMode(u8 compare_enable, GXCompare func, u8 update_enable)
{
	// clang-format off
	GDWriteBPCmd(
    compare_enable
    | func << 1
    | update_enable << 4
    | 0x40000000);
	// clang-format on
}

void GDSetDstAlpha(u8 enable, u8 alpha)
{
	GDWriteBPCmd(alpha | enable << 8 | 0x42000000);
}
