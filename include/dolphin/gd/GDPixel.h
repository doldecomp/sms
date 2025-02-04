#ifndef _DOLPHIN_GD_PIXEL_H
#define _DOLPHIN_GD_PIXEL_H

#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off
#define BP_FOG_UNK0(a, id) \
    ( \
        (u32)(a) << 0 | \
        (u32)(id) << 24 \
    )

#define BP_FOG_UNK1(b_m, id) \
    ( \
        (u32)(b_m) << 0 | \
        (u32)(id) << 24 \
    )

#define BP_FOG_UNK2(b_expn, id) \
    ( \
        (u32)(b_expn) << 0 | \
        (u32)(id) << 24 \
    )

#define BP_FOG_UNK3(c, proj, fsel, id) \
    ( \
        (u32)(c) << 0 | \
        (u32)(proj) << 20 | \
        (u32)(fsel) << 21 | \
        (u32)(id) << 24 \
    )

#define BP_FOG_COLOR(r, g, b, id) \
    ( \
        (u32)(b) << 0 | \
        (u32)(g) << 8 | \
        (u32)(r) << 16 | \
        (u32)(id) << 24 \
    )

#define BP_BLEND_MODE(enable, enable_logic, enable_dither, enable_color_update, enable_alpha_update, dst_factor, src_factor, blend_sub, logic_op, id) \
    ( \
        (u32)(enable) << 0 | \
        (u32)(enable_logic) << 1 | \
        (u32)(enable_dither) << 2 | \
        (u32)(enable_color_update) << 3 | \
        (u32)(enable_alpha_update) << 4 | \
        (u32)(dst_factor) << 5 | \
        (u32)(src_factor) << 8 | \
        (u32)(blend_sub) << 11 | \
        (u32)(logic_op) << 12 | \
        (u32)(id) << 24 \
    )

#define BP_Z_MODE(enable_compare, compare_fn, enable_update, id) \
    ( \
        (u32)(enable_compare) << 0 | \
        (u32)(compare_fn) << 1 | \
        (u32)(enable_update) << 4 | \
        (u32)(id) << 24 \
    )

#define BP_DST_ALPHA(alpha, enable, id) \
    ( \
        (u32)(alpha) << 0 | \
        (u32)(enable) << 8 | \
        (u32)(id) << 24 \
    )

#define BP_TOKEN(token, id) \
    ( \
        (u32)(token) << 0 | \
        (u32)(id) << 24 \
    )
// clang-format on

void GDSetFog(GXFogType type, float startz, float endz, float nearz, float farz,
              GXColor color);
void GDSetBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                    GXBlendFactor dst_factor, GXLogicOp logic_op);
void GDSetBlendModeEtc(GXBlendMode type, GXBlendFactor src_factor,
                       GXBlendFactor dst_factor, GXLogicOp logic_op,
                       unsigned char color_update_enable,
                       unsigned char alpha_update_enable,
                       unsigned char dither_enable);
void GDSetZMode(unsigned char compare_enable, GXCompare func,
                unsigned char update_enable);
void GDSetDstAlpha(unsigned char enable, unsigned char alpha);
void GDSetDrawSync(u16 token);

#ifdef __cplusplus
}
#endif

#endif
