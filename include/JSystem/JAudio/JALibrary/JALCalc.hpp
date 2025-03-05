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

f32 linearTransform(f32 x, f32 xStart, f32 xEnd, f32 yStart, f32 yEnd,
                    bool unbounded);

f32 getParamByExp(f32 x, f32 xStart, f32 xEnd, f32 y, f32 yStart, f32 yEnd,
                  CurveSign curve);
// Unused / inlined
f32 getParamByExp_0_1(f32, f32, f32, f32, CurveSign);

f32 getRandom(f32 min, f32 max, f32 step);
f32 getRandom_0_1();
// Unused / inlined
s32 getRandom_Sign();

// Unused / inlined
f32 pow2(f32 value);
// Unused / inlined
f32 getRint(f32 value);

f32 getDist(Vec* vec1, Vec* vec2);
f32 getDistPow(Vec* vec1, Vec* vec2);

} // namespace JALCalc

#endif // JALCALC_HPP
