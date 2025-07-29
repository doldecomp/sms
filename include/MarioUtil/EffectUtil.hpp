#ifndef MARIO_UTIL_EFFECT_UTIL_HPP
#define MARIO_UTIL_EFFECT_UTIL_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

void SMS_GetJumpIntoWaterModelData();
void SMS_EmitSinkInPollutionEffect(const JGeometry::TVec3<f32>&,
                                   const JGeometry::TVec3<f32>&, bool);
void SMS_EmitRippleSea(MtxPtr, void*);
void SMS_EmitRipplePool(MtxPtr, void*);
void SMS_EmitRippleTiny(JGeometry::TVec3<f32>*);

#endif
