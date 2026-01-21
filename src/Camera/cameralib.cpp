#include <Camera/cameralib.hpp>

// TODO: This macro should probably be consolidated elsewhere
#define ABS(x) ((x) >= 0 ? (x) : -(x))

/**
 * @brief Creates a scale-and-translate matrix.
 *
 * @param mtx the destination matrix
 * @param scale scale factors
 * @param translate translation values
 */
void CLBCalcScaleTranslateMatrix(MtxPtr mtx, const Vec& scale,
                                 const Vec& translate)
{
	mtx[0][0] = scale.x;
	mtx[0][1] = 0.0f;
	mtx[0][2] = 0.0f;
	mtx[0][3] = translate.x;

	mtx[1][0] = 0.0f;
	mtx[1][1] = scale.y;
	mtx[1][2] = 0.0f;
	mtx[1][3] = translate.y;

	mtx[2][0] = 0.0f;
	mtx[2][1] = 0.0f;
	mtx[2][2] = scale.z;
	mtx[2][3] = translate.z;
}

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
