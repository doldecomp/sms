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

void GDSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1,
                       u8 ref1);

void GDSetTevOrder(GXTevStageID evenStage, GXTexCoordID coord0, GXTexMapID map0,
                   GXChannelID color0, GXTexCoordID coord1, GXTexMapID map1,
                   GXChannelID color1);

#ifdef __cplusplus
}
#endif

#endif
