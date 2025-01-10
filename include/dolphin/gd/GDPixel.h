#ifndef _DOLPHIN_GD_PIXEL_H
#define _DOLPHIN_GD_PIXEL_H

#include <dolphin/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif
