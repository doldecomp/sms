#include <dolphin/types.h>

f32 acosf(f32);
f32 sinf(f32);

#pragma cplusplus on

extern inline float sqrtf(float x)
{
	volatile float y;
	if (x > 0.0f) {
		double guess = __frsqrte((double)x); // returns an approximation to
		guess = .5 * guess * (3.0 - guess * guess * x); // now have 12 sig bits
		guess = .5 * guess * (3.0 - guess * guess * x); // now have 24 sig bits
		guess = .5 * guess * (3.0 - guess * guess * x); // now have 32 sig bits
		y     = (float)(x * guess);
		return y;
	}
	return x;
}

extern inline float sqrt(float x)
{
	volatile float y;
	if (x > 0.0f) {
		double guess = __frsqrte((double)x); // returns an approximation to
		guess = .5 * guess * (3.0 - guess * guess * x); // now have 12 sig bits
		guess = .5 * guess * (3.0 - guess * guess * x); // now have 24 sig bits
		guess = .5 * guess * (3.0 - guess * guess * x); // now have 32 sig bits
		guess = .5 * guess * (3.0 - guess * guess * x); // now have 32 sig bits

		y = (float)(x * guess);
		return y;
	}
	return x;
}

extern inline float fabs(float x)
{
#if __MIPS__
	return fabsf(x);
#else
	(*(int*)&x) &= 0x7fffffff;
	return x;
#endif
}

inline float floor(float x)
{
	int i   = (int)x;
	float y = x - (float)i;

	if (!y || x > 8388608.0f)
		return x; // x is already an int

	if (x < 0)
		return (float)--i;
	// x < 0 -> int conversion of x above rounded toward zero(so decrement)
	return (float)i;
}

#pragma cplusplus reset
