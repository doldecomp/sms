#include <Camera/cameralib.hpp>

// TODO: This macro should probably be consolidated elsewhere
#define ABS(x) ((x) >= 0 ? (x) : -(x))

/**
 * @brief Moves dstValue toward targetValue by a fraction defined by ratio.
 *
 * @param dstValue the value to be modified
 * @param targetValue the target value to approach
 * @param ratio the ratio by which dstValue approaches targetValue
 * @param threshold a threshold for if we're close enough
 * @return whether dstValue is still outside the threshold of targetValue
 */
bool CLBChaseDecrease(f32* dstValue, f32 targetValue, f32 ratio, f32 threshold)
{
	if (ratio > 1.0f) {
		ratio = 1.0f;
	}

	*dstValue += ratio * (targetValue - *dstValue);

	// You'd think this would just be
	// "return ABS(*dstValue - targetValue) > ABS(threshold);"
	// but this gives an exact match
	if (ABS(*dstValue - targetValue) <= ABS(threshold)) {
		return false;
	} else {
		return true;
	}
}
