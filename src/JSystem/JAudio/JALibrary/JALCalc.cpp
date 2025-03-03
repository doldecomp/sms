#include <JSystem/JAudio/JALibrary/JALCalc.hpp>
namespace JALCalc {

const f32 cEqualCSlope = 1.0f;
const f32 cPlusPSlope  = 1.0f;

f32 getRandom_0_1() { return 0.0f; }

f32 getRandom(f32 min, f32 max, f32 step) { return min; }

// Unused / inlined
s32 getRandom_Sign() { return 0; }

f32 linearTransform(f32 input, f32 inMin, f32 inMax, f32 outMin, f32 outMax,
                    bool clamp)
{
	return outMin;
}

f32 getParamByExp(f32 input, f32 inMin, f32 inMax, f32 outMin, f32 outMax,
                  f32 exp, CurveSign sign)
{
	return outMin;
}

// Unused / inlined
f32 getParamByExp_0_1(f32 input, f32 outMin, f32 outMax, f32 exp,
                      CurveSign sign)
{
	return outMin;
}

// Unused / inlined
f32 pow2(f32 value) { return 0.0f; }

// Unused / inlined
f32 getRint(f32 value) { return 0.0f; }

f32 getDist(Vec* vec1, Vec* vec2) { return 0.0f; }

f32 getDistPow(Vec* vec1, Vec* vec2) { return 0.0f; }

} // namespace JALCalc
