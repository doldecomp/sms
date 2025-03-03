#ifndef JALCALC_HPP
#define JALCALC_HPP

#include <JSystem/JGeometry.hpp>
namespace JALCalc {

enum CurveSign {
	CS_NEGATIVE_CURVE = 0,
	CS_POSITIVE_CURVE = 1,
};

extern const f32 cEqualCSlope;
extern const f32 cPlusPSlope;

// Random number generation
float getRandom_0_1();
float getRandom(float min, float max, float step);
// Unused
int getRandom_Sign();

float linearTransform(float input, float inMin, float inMax, float outMin,
                      float outMax, bool clamp);
float getParamByExp(float input, float inMin, float inMax, float outMin,
                    float outMax, float exp, CurveSign sign);
// Unused
float getParamByExp_0_1(float input, float outMin, float outMax, float exp,
                        CurveSign sign);

// Unused
float pow2(float value);
// Unused
float getRint(float value);

float getDist(Vec* vec1, Vec* vec2);
float getDistPow(Vec* vec1, Vec* vec2);

} // namespace JALCalc

#endif // JALCALC_HPP
