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

	// Emitted weak (0x5c) from koopajr.cpp and Koopa.cpp, with the same
	// body as std::fmodf.
	static f32 mod(f32 value, f32 modulus)
	{
		if (fabsf(modulus) > fabsf(value))
			return value;
		u64 quotient = (u64)(value / modulus);
		return value - modulus * (f32)(s64)quotient;
	}

	// from SMG
	static bool epsilonEquals(f32 param_1, f32 param_2, f32 eps)
	{
		return -eps <= param_2 - param_1 && param_2 - param_1 <= eps;
	}

	// fabricated
	static bool epsilonEquals(f32 param_1, f32 param_2)
	{
		return -epsilon() <= param_2 - param_1
		       && param_2 - param_1 <= epsilon();
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

	static f32 inv_sqrt(f32 mag)
	{
		if (mag <= 0.0f)
			return mag;

		f32 root = __frsqrte(mag);
		return 0.5f * root * (3.0f - mag * (root * root));
	}
};

} // namespace JGeometry

#endif
