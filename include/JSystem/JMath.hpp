#ifndef JMATH_HPP
#define JMATH_HPP

#include <dolphin/mtx.h>

void JMANewSinTable(u8);
void JMADeleteSinTable();
void JMAEulerToQuat(s16, s16, s16, Quaternion*);
void JMAQuatLerp(Quaternion*, Quaternion*, f32, Quaternion*);
f32 JMAHermiteInterpolation(f32 frame, f32 time0, f32 value0, f32 tangent0,
                            f32 time1, f32 value1, f32 tangent1);
void JMALagrangeInterpolation(int, f32*, f32*, f32);

#endif
