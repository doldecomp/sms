#ifndef JG_UTIL_HPP
#define JG_UTIL_HPP

#include <dolphin/types.h>
#include <math.h>

namespace JGeometry {

template <typename T> struct TUtil;

template <> struct TUtil<f32> {
	static f32 epsilon() { return 3.81469727e-06f; }

	static f32 sqrt(f32 mag)
	{
		if (mag <= 0.0f) {
			return mag;
		} else {
			f32 root = __frsqrte(mag);
			return 0.5f * root * (3.0f - mag * (root * root)) * mag;
		}
	}

	static f32 inv_sqrt(f32 mag);
};

#pragma dont_inline on
inline f32 TUtil<f32>::inv_sqrt(f32 mag)
{
	if (mag <= 0.0f) {
		return mag;
	} else {
		f32 root = __frsqrte(mag);
		return 0.5f * root * (3.0f - mag * (root * root));
	}
}
#pragma dont_inline off

} // namespace JGeometry

#endif
