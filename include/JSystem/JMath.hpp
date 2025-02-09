#ifndef JMATH_HPP
#define JMATH_HPP

#include <dolphin/types.h>

f32 JMAHermiteInterpolation(f32 frame, f32 time0, f32 value0, f32 tangent0,
                            f32 time1, f32 value1, f32 tangent1);

#endif
