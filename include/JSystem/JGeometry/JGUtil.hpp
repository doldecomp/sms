#ifndef JG_UTIL_HPP
#define JG_UTIL_HPP

#include <dolphin/types.h>
#include <math.h>

namespace JGeometry {

template <typename T> struct TUtil {
	static T clamp(T value, T min, T max)
	{
		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}

	static T mod(T value, T modulus);
};

template <class T> T TUtil<T>::mod(T value, T modulus)
{
	return value % modulus;
}

template <> struct TUtil<f32> {
	static f32 one() { return 1.0f; }
	static f32 epsilon() { return 3.81469727e-06f; }
	static f32 PI() { return 3.14159265358979323846f; }
	static f32 halfPI() { return 1.5707963267948966f; }

	static bool epsilonEquals(f32 param_1, f32 param_2, f32 epsilon)
	{
		if (param_1 - param_2 < -epsilon)
			return 0;

		if (epsilon < param_1 - param_2)
			return 0;

		return 1;
	}

	static f32 clamp(f32 value, f32 min, f32 max)
	{
		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}

	static f32 sqrt(f32 mag)
	{
		if (mag <= 0.0f)
			return mag;

		f32 root = __frsqrte(mag);
		return 0.5f * root * (3.0f - mag * (root * root)) * mag;
	}

	static f32 inv_sqrt(f32 mag);
};

#pragma dont_inline on
inline f32 TUtil<f32>::inv_sqrt(f32 mag)
{
	if (mag <= 0.0f)
		return mag;

	f32 root = __frsqrte(mag);
	return 0.5f * root * (3.0f - mag * (root * root));
}
#pragma dont_inline off

} // namespace JGeometry

#endif
