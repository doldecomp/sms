#include <types.h>
#include <Camera/cameralib.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>

// TODO: This macro should probably be consolidated elsewhere
#define ABS(x) ((x) >= 0 ? (x) : -(x))

// TODO: This feels too useful to be limited to this file
/**
 * @brief A fast square root implementation.
 * 
 * @param x input value
 * @return f32 sqrt(x) if x >= 0, x otherwise
 */
static inline f32 fastSqrt(f32 x)
{
	if (x > 0.0f) {
		double guess = __frsqrte((double)x);
		return 0.5 * guess * (3.0 - x * (guess * guess)) * x;
	} else {
		return x;
	}
}

// TODO: Explore how this is used, and add documentation
void CLBCalc2DFPos(JGeometry::TVec2<f32>* param_1, MtxPtr param_2,
                   const MtxPtr param_3, const Vec& param_4, u32* param_5,
                   bool param_6)
{
	Vec prod;

	PSMTXMultVec(param_3, (Vec*)&param_4, &prod);

	if (prod.z == 0.0f) {
		param_1->x = param_1->y = 10000.0f;
	} else {
		f32 fVar3 = 1.0f / -prod.z;
		f32 fVar4 = (param_2[2][2] * prod.z + param_2[2][3]) * fVar3;
		if (!param_6 && (fVar4 > 0.0f || fVar4 < -1.0f)) {
			param_1->x = param_1->y = 10000.0f;
		} else {
			param_1->set(
			    fVar3 * (param_2[0][0] * prod.x + param_2[0][2] * prod.z),
			    fVar3 * (param_2[1][1] * prod.y + param_2[1][2] * prod.z));
			if (param_5 != nullptr) {
				*param_5
				    = CLBLinearInbetween((u32)0, (u32)0xffffff, fVar4 + 1.0f);
			}
		}
	}
}

/**
 * @brief Creates a rotate-and-translate matrix.
 *
 * @details
 * Rotations are applied in ZXY order.
 *
 * @param mtx the destination matrix
 * @param rotate rotation angles in degrees
 * @param translate translation values
 */
void CLBCalcRotateZXYTranslateMatrix(MtxPtr mtx, const Vec& rotate,
                                     const Vec& translate)
{
	s16 sAngleX = CLBRoundf<s16>(DEG2SHORTANGLE(rotate.x));
	s16 sAngleY = CLBRoundf<s16>(DEG2SHORTANGLE(rotate.y));
	s16 sAngleZ = CLBRoundf<s16>(DEG2SHORTANGLE(rotate.z));

	f32 sinX = JMASSin(sAngleX);
	f32 cosX = JMASCos(sAngleX);
	f32 sinY = JMASSin(sAngleY);
	f32 cosY = JMASCos(sAngleY);
	f32 sinZ = JMASSin(sAngleZ);
	f32 cosZ = JMASCos(sAngleZ);

	mtx[0][0] = cosY * cosZ + sinY * sinX * sinZ;
	mtx[0][1] = -sinZ * cosY + cosZ * sinY * sinX;
	mtx[0][2] = sinY * cosX;
	mtx[0][3] = translate.x;

	mtx[1][0] = cosX * sinZ;
	mtx[1][1] = cosX * cosZ;
	mtx[1][2] = -sinX;
	mtx[1][3] = translate.y;

	mtx[2][0] = -sinY * cosZ + sinZ * cosY * sinX;
	mtx[2][1] = sinY * sinZ + cosZ * cosY * sinX;
	mtx[2][2] = cosY * cosX;
	mtx[2][3] = translate.z;
}

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

/**
 * @brief Converts Cartesian coordinates to spherical coordinates.
 *
 * @param offset an offset to apply to the input coordinates
 * @param in the input vector
 * @param outRadius the output radius
 * @param outVAngle the output vertical angle from the xz-plane to the y-axis
 * @param outHAngle the output horizontal angle in the xz-plane, clockwise from
 * the z-axis
 */
void CLBCrossToPolar(const Vec& offset, const Vec& in, f32* outRadius,
                     s16* outVAngle, s16* outHAngle)
{
	f32 dx = in.x - offset.x;
	f32 dy = in.y - offset.y;
	f32 dz = in.z - offset.z;

	*outRadius = fastSqrt(dx * dx + dy * dy + dz * dz);

	f32 xzDist = fastSqrt(dx * dx + dz * dz);
	*outVAngle = matan(xzDist, dy);
	*outHAngle = matan(dz, dx);
}

/**
 * @brief Converts spherical coordinates to Cartesian coordinates.
 *
 * @param offset an offset to apply to the resulting coordinates
 * @param out the output vector
 * @param radius the radius of the point
 * @param vAngle the vertical angle from the xz-plane to the y-axis
 * @param hAngle the horizontal angle in the xz-plane, clockwise from the z-axis
 */
void CLBPolarToCross(const Vec& offset, Vec* out, f32 radius, s16 vAngle,
                     s16 hAngle)
{
	out->x = offset.x + radius * JMASCos(vAngle) * JMASSin(hAngle);
	out->y = offset.y + radius * JMASSin(vAngle);
	out->z = offset.z + radius * JMASCos(vAngle) * JMASCos(hAngle);
}
