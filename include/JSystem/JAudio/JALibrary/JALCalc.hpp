#ifndef JALCALC_HPP
#define JALCALC_HPP

#include <JSystem/JGeometry.hpp>

class JALCalc {
public:
	enum CurveSign {
		CS_NEGATIVE_CURVE = 0,
		CS_POSITIVE_CURVE = 1,
	};

	// Random number generation
	static float getRandom_0_1();
	static float getRandom(float min, float max, float step);
	static int getRandom_Sign();

	// Parameter transformation
	static float linearTransform(float input, float inMin, float inMax,
	                             float outMin, float outMax, bool clamp);
	static float getParamByExp(float input, float inMin, float inMax,
	                           float outMin, float outMax, float exp,
	                           CurveSign sign);
	static float getParamByExp_0_1(float input, float outMin, float outMax,
	                               float exp, CurveSign sign);

	// Math utilities
	static float pow2(float value);
	static float getRint(float value);

	// Distance calculations
	static float getDist(Vec* vec1, Vec* vec2);
	static float getDistPow(Vec* vec1, Vec* vec2);
};

#endif // JALCALC_HPP
