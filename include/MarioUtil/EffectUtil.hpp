#ifndef MARIO_UTIL_EFFECT_UTIL_HPP
#define MARIO_UTIL_EFFECT_UTIL_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

void SMS_GetJumpIntoWaterModelData();
void SMS_EmitSinkInPollutionEffect(const JGeometry::TVec3<f32>&,
                                   const JGeometry::TVec3<f32>&, bool);
bool SMS_EmitRippleSea(MtxPtr, void*);
bool SMS_EmitRipplePool(MtxPtr, void*);
bool SMS_EmitRippleTiny(JGeometry::TVec3<f32>*);

#endif
