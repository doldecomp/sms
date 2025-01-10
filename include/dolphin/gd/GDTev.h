#ifndef _DOLPHIN_GD_TEV_H
#define _DOLPHIN_GD_TEV_H

#include <dolphin/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDSetTevOp(GXTevStageID stage, GXTevMode mode);

void GDSetTevColorCalc(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b,
                       GXTevColorArg c, GXTevColorArg d, GXTevOp op,
                       GXTevBias bias, GXTevScale scale, u8 clamp,
                       GXTevRegID out_reg);

void GDSetTevAlphaCalcAndSwap(GXTevStageID stage, GXTevAlphaArg a,
                              GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d,
                              GXTevOp op, GXTevBias bias, GXTevScale scale,
                              u8 clamp, GXTevRegID out_reg,
                              GXTevSwapSel ras_sel, GXTevSwapSel tex_sel);

#ifdef __cplusplus
}
#endif

#endif
