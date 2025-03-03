#include <JSystem/JAudio/JALibrary/JALCalc.hpp>
#include <math.h>
namespace JALCalc {

const f32 cEqualCSlope = 1.0f;
const f32 cPlusPSlope  = 1.0f;

float getRandom_0_1() { return 0.0f; }

float getRandom(float min, float max, float step) { return min; }

// Unused / inlined
int getRandom_Sign() { return 0; }

float linearTransform(float input, float inMin, float inMax, float outMin,
                      float outMax, bool clamp)
{
	return outMin;
}

float getParamByExp(float input, float inMin, float inMax, float outMin,
                    float outMax, float exp, CurveSign sign)
{
	return outMin;
}

// Unused / inlined
float getParamByExp_0_1(float input, float outMin, float outMax, float exp,
                        CurveSign sign)
{
	return outMin;
}

// Unused / inlined
float pow2(float value) { return 0.0f; }

// Unused / inlined
float getRint(float value) { return 0.0f; }

float getDist(Vec* vec1, Vec* vec2) { return 0.0f; }

float getDistPow(Vec* vec1, Vec* vec2) { return 0.0f; }

} // namespace JALCalc
