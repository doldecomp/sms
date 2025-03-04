#ifndef JALCALC_HPP
#define JALCALC_HPP

#include <dolphin/mtx.h>
#include <types.h>

namespace JALCalc {

enum CurveSign {
	CS_NEGATIVE_CURVE = 0,
	CS_POSITIVE_CURVE = 1,
};

extern const f32 cEqualCSlope;
extern const f32 cPlusPSlope;

f32 getRandom_0_1();
f32 getRandom(f32 min, f32 max, f32 step);
// Unused / inlined
s32 getRandom_Sign();

f32 linearTransform(f32 input, f32 inMin, f32 inMax, f32 outMin, f32 outMax,
                    bool clamp);
f32 getParamByExp(f32 input, f32 inMin, f32 inMax, f32 outMin, f32 outMax,
                  f32 exp, CurveSign sign);
// Unused / inlined
f32 getParamByExp_0_1(f32 input, f32 outMin, f32 outMax, f32 exp,
                      CurveSign sign);

f32 getDist(Vec* vec1, Vec* vec2);
f32 getDistPow(Vec* vec1, Vec* vec2);

// Unused / inlined
f32 pow2(f32 value);
// Unused / inlined
f32 getRint(f32 value);

} // namespace JALCalc

#endif // JALCALC_HPP
