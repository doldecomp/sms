#include <types.h>
#include <Camera/cameralib.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>

// TODO: This macro should probably be consolidated elsewhere
#define ABS(x) ((x) >= 0 ? (x) : -(x))

static const f32 SHORTANGLE_TO_DEGREES = 0.005493164f; // 360/65536
static const f32 DEGREES_TO_RADIANS    = 0.017453294f; // pi/180

// TODO: Almost definitely fake, this is probably inlined somewhere else
static inline f32 fastSqrt(f32 x)
{
	if (x > 0.0f) {
		double guess = __frsqrte((double)x);
		return 0.5 * guess * (3.0 - x * (guess * guess)) * x;
	} else {
		return x;
	}
}

// TODO: Possibly fake but still useful for now?
static inline void MatrixT33Multiply(const TMtx33f& mtxT,
                                     JGeometry::TVec3<f32>* inOutVec)
{
	JGeometry::TVec3<f32> oldVec = *inOutVec;

	inOutVec->x = oldVec.x * mtxT.at(0, 0) + oldVec.y * mtxT.at(1, 0)
	              + oldVec.z * mtxT.at(2, 0);
	inOutVec->y = oldVec.x * mtxT.at(0, 1) + oldVec.y * mtxT.at(1, 1)
	              + oldVec.z * mtxT.at(2, 1);
	inOutVec->z = oldVec.x * mtxT.at(0, 2) + oldVec.y * mtxT.at(1, 2)
	              + oldVec.z * mtxT.at(2, 2);
}

// TODO: These are very fake
static void normalizeInner1(JGeometry::TVec3<f32>& vec) { vec.normalize(); }
static void normalizeInner2(JGeometry::TVec3<f32>& vec)
{
	normalizeInner1(vec);
}

static inline void RotateAboutAxis(const JGeometry::TVec3<f32>& param_axis,
                                   f32 angle, JGeometry::TVec3<f32>* vec)
{
	JGeometry::TRotation3<TMtx33f> mtxT;

	mtxT.ref(0, 2) = mtxT.ref(1, 2) = 0.0f;
	mtxT.ref(0, 1) = mtxT.ref(2, 1) = 0.0f;
	mtxT.ref(1, 0) = mtxT.ref(2, 0) = 0.0f;
	mtxT.ref(0, 0) = mtxT.ref(1, 1) = mtxT.ref(2, 2) = 1.0f;

	mtxT.setRotate(param_axis, angle);

	MatrixT33Multiply(mtxT, vec);
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

// TODO: This needs matching work, but it's mathematically correct
void CLBCalcNearNinePos(JGeometry::TVec3<f32>* param_2, S16Vec* param_3,
                        const JGeometry::TVec3<f32>& param_4,
                        const JGeometry::TVec3<f32>& param_5, s16 param_6,
                        f32 distance, const JGeometry::TVec2<f32>& param_7)
{
	JGeometry::TVec3<f32> fVar16;
	JGeometry::TVec3<f32> fVar19;

	JGeometry::TRotation3<TMtx33f> local_118;

	JGeometry::TRotation3<TMtx33f> local_e4;

	JGeometry::TVec3<f32> local_a8;
	JGeometry::TVec3<f32> local_90;
	JGeometry::TVec3<f32> local_80;
	JGeometry::TVec3<f32> local_74;
	JGeometry::TVec3<f32> local_68;

	local_a8.sub(param_5, param_4);
	normalizeInner2(local_a8);

	param_2[4].scaleAdd(distance, param_4, local_a8);

	f32 xzDistance
	    = MsSqrtf(((param_4.x - param_5.x) * (param_4.x - param_5.x)
	               + (param_4.z - param_5.z) * (param_4.z - param_5.z)));
	param_3->x = -matan(xzDistance, param_4.y - param_5.y);
	param_3->y = matan(param_4.z - param_5.z, param_4.x - param_5.x);
	param_3->z = param_6;

	local_68.set(0.0f, 1.0f, 0.0f);
	local_74.set(1.0f, 0.0f, 0.0f);

	// We already did this, so maybe we're calling another function here?
	local_80.sub(param_5, param_4);
	normalizeInner1(local_80);

	fVar16.z = param_3->z * SHORTANGLE_TO_DEGREES * DEGREES_TO_RADIANS;

	// Putting these in separate scopes for now because this feels like an
	// inlined function
	{
		f32 sinX = JMASSin(param_3->x);
		f32 cosX = JMASCos(param_3->x);
		f32 sinY = JMASSin(param_3->y);
		f32 cosY = JMASCos(param_3->y);

		// This transformation appears to be the following:
		// [ cosY, 0, sinY]   [1,   0,     0 ]
		// [   0,  1,   0 ] * [0, cosX, -sinX]
		// [-sinY, 0, cosY]   [0, sinX,  cosX]
		local_68.set(local_68.x * cosY
		                 + (local_68.y * sinX + local_68.z * cosX) * sinY,
		             local_68.y * cosX - local_68.z * sinX,
		             -local_68.x * sinY
		                 + (local_68.y * sinX + local_68.z * cosX) * cosY);

		local_e4.identity33();
		local_e4.setRotate(local_80, fVar16.z);

		MatrixT33Multiply(local_e4, &local_68);
	}

	{
		f32 sinX = JMASSin(param_3->x);
		f32 cosX = JMASCos(param_3->x);
		f32 sinY = JMASSin(param_3->y);
		f32 cosY = JMASCos(param_3->y);

		local_74.set(local_74.x * cosY
		                 + (local_74.y * sinX + local_74.z * cosX) * sinY,
		             local_74.y * cosX - local_74.z * sinX,
		             -local_74.x * sinY
		                 + (local_74.y * sinX + local_74.z * cosX) * cosY);

		local_118.identity33();
		local_118.setRotate(local_80, fVar16.z);

		MatrixT33Multiply(local_118, &local_74);
	}

	f32 fVar3 = param_7.y * 0.5f;
	f32 fVar5 = param_7.x * 0.5f;
	f32 fVar6 = -fVar3;
	f32 fVar7 = -fVar5;

	param_2[1].scaleAdd(fVar3, param_2[4], local_68);
	param_2[7].scaleAdd(fVar6, param_2[4], local_68);
	param_2[3].scaleAdd(fVar7, param_2[4], local_74);
	param_2[5].scaleAdd(fVar5, param_2[4], local_74);

	// Anything below here could be part of CLBCalcNearFourPos?

	fVar16.scale(fVar5, local_74);
	fVar19.scale(fVar3, local_68);

	f32 halfPlaneDiagonal = fastSqrt(fVar3 * fVar3 + fVar5 * fVar5);

	local_90.scaleAdd(fVar7, fVar19, local_74);
	MsVECNormalize(&local_90, &local_90);

	param_2[0].scaleAdd(halfPlaneDiagonal, param_2[4], local_90);
	local_90.negate();
	param_2[8].scaleAdd(halfPlaneDiagonal, param_2[4], local_90);

	local_90.x = (f32)(fVar16.x + fVar19.x);
	local_90.y = (f32)(fVar16.y + fVar19.y);
	local_90.z = (f32)(fVar16.z + fVar19.z);
	MsVECNormalize(&local_90, &local_90);

	param_2[2].scaleAdd(halfPlaneDiagonal, param_2[4], local_90);
	local_90.negate();
	param_2[6].scaleAdd(halfPlaneDiagonal, param_2[4], local_90);
}

void CLBCalcPointInCubeRatio(const Vec& param_1, const Vec& param_2,
                             const Vec& param_3, const Vec& param_4,
                             f32* param_5, f32* param_6, f32* param_7)
{
	f32 dx = param_1.x - param_2.x;
	f32 dy = param_1.y - param_2.y;
	f32 dz = param_1.z - param_2.z;

	if (param_3.z != 0) {
		s16 zAngle = CLBRoundf<s16>(DEG2SHORTANGLE(-param_3.z));
		f32 cosZ   = JMASCos(zAngle);
		f32 sinZ   = JMASSin(zAngle);

		f32 dySinZ = dy * sinZ;

		dy = dx * sinZ + dy * cosZ;
		dx = dx * cosZ - dySinZ;
	}

	if (param_3.y != 0) {
		s16 yAngle = CLBRoundf<s16>(DEG2SHORTANGLE(-param_3.y));
		f32 cosY   = JMASCos(yAngle);
		f32 sinY   = JMASSin(yAngle);

		f32 dzSinY = dz * sinY;

		dz = -dx * sinY + dz * cosY;
		dx = dx * cosY + dzSinY;
	}

	if (param_3.x != 0) {
		s16 xAngle = CLBRoundf<s16>(DEG2SHORTANGLE(-param_3.x));
		f32 cosX   = JMASCos(xAngle);
		f32 sinX   = JMASSin(xAngle);

		f32 dzSinX = dz * sinX;

		dz = dy * sinX + dz * cosX;
		dy = dy * cosX - dzSinX;
	}

	if (param_5 != nullptr) {
		*param_5 = CLBCalcRatio(-param_4.x * 0.5f, param_4.x * 0.5f, dx);
	}

	if (param_6 != nullptr) {
		*param_6 = CLBCalcRatio(0.0f, param_4.y, dy);
	}

	if (param_7 != nullptr) {
		*param_7 = CLBCalcRatio(-param_4.z * 0.5f, param_4.z * 0.5f, dz);
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
 * @brief Moves a SHORTANGLE towards another SHORTANGLE by a specified ratio.
 *
 * @param out the value to be modified
 * @param target the target value to approach
 * @param invSpeed a constant inversely proportional to the rate of change
 * @return whether another iteration may refine the angle even further
 */
bool CLBChaseAngleDecrease(s16* out, s16 target, s16 invSpeed)
{
	if (invSpeed == 0) {
		*out = target;
	} else {
		s16 difference = *out - target;
		s16 newValue   = difference - difference / invSpeed + target;
		if (newValue == *out) {
			// No further calls will update the angle
			return false;
		}
		*out = newValue;
	}
	if (*out == target) {
		// Destination angle reached!
		return false;
	}
	return true;
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

bool CLBChaseSpecialDecrease(f32* param_1, f32 param_2, f32 param_3,
                             f32 param_4)
{
	if (param_3 > 1.0f) {
		param_3 = 1.0f;
	}

	f32 fVar2 = param_3 * (param_2 - *param_1);
	f32 fVar3 = ABS(param_4);
	f32 fVar1 = ABS(fVar2);

	if (fVar1 < fVar3) {
		fVar2 = param_4;
	}

	return CLBChaseGeneralConstantSpecifySpeed(param_1, param_2, fVar2);
}

/**
 * @brief Converts Cartesian coordinates to spherical coordinates.
 *
 * @param origin the point to use as the origin
 * @param in the input vector
 * @param outRadius the output radius
 * @param outVAngle the output vertical angle
 * @param outHAngle the output horizontal angle
 */
void CLBCrossToPolar(const Vec& origin, const Vec& in, f32* outRadius,
                     s16* outVAngle, s16* outHAngle)
{
	f32 dx = in.x - origin.x;
	f32 dy = in.y - origin.y;
	f32 dz = in.z - origin.z;

	*outRadius = fastSqrt(dx * dx + dy * dy + dz * dz);

	*outVAngle = matan(fastSqrt(dx * dx + dz * dz), dy);
	*outHAngle = matan(dz, dx);
}

bool CLBIsPointInCube(const Vec& param_1, const Vec& param_2,
                      const Vec& param_3, const Vec& param_4)
{
	// TODO: Why is so much of this copy-pasted from CLBCalcPointInCubeRatio?

	f32 dx = param_1.x - param_2.x;
	f32 dy = param_1.y - param_2.y;
	f32 dz = param_1.z - param_2.z;

	bool result = false;

	if (param_3.y != 0.0f || param_3.x != 0.0f || param_3.z != 0.0f) {
		if (param_3.z != 0.0f) {
			s16 zAngle = CLBRoundf<s16>(DEG2SHORTANGLE(-param_3.z));
			f32 cosZ   = JMASCos(zAngle);
			f32 sinZ   = JMASSin(zAngle);

			f32 dySinZ = dy * sinZ;

			dy = dx * sinZ + dy * cosZ;
			dx = dx * cosZ - dySinZ;
		}

		if (param_3.y != 0.0f) {
			s16 yAngle = CLBRoundf<s16>(DEG2SHORTANGLE(-param_3.y));
			f32 cosY   = JMASCos(yAngle);
			f32 sinY   = JMASSin(yAngle);

			f32 dzSinY = dz * sinY;

			dz = -dx * sinY + dz * cosY;
			dx = dx * cosY + dzSinY;
		}

		if (param_3.x != 0.0f) {
			s16 xAngle = CLBRoundf<s16>(DEG2SHORTANGLE(-param_3.x));
			f32 cosX   = JMASCos(xAngle);
			f32 sinX   = JMASSin(xAngle);

			f32 dzSinX = dz * sinX;

			dz = dy * sinX + dz * cosX;
			dy = dy * cosX - dzSinX;
		}
	}

	if ((-param_4.x * 0.5f < dx && dx < param_4.x * 0.5f)
	    && (0.0f < dy && dy < param_4.y)
	    && (-param_4.z * 0.5f < dz && dz < param_4.z * 0.5f)) {
		result = true;
	}

	return result;
}

/**
 * @brief Converts spherical coordinates to Cartesian coordinates.
 *
 * @param origin the point to use as the origin
 * @param out the output vector
 * @param radius the radius of the point
 * @param vAngle the vertical angle from the xz-plane to the y-axis
 * @param hAngle the horizontal angle in the xz-plane, clockwise from the z-axis
 */
void CLBPolarToCross(const Vec& origin, Vec* out, f32 radius, s16 vAngle,
                     s16 hAngle)
{
	out->x = origin.x + radius * JMASCos(vAngle) * JMASSin(hAngle);
	out->y = origin.y + radius * JMASSin(vAngle);
	out->z = origin.z + radius * JMASCos(vAngle) * JMASCos(hAngle);
}

/**
 * @brief Clamps a vector within a specified vertical angle range.
 *
 * @param vAngleMin the minimum vertical angle
 * @param vAngleMax the maximum vertical angle
 * @param origin the point to use as the origin
 * @param inOut the input and output vector
 */
void CLBRevisionLookatByAngleX(s16 vAngleMin, s16 vAngleMax, const Vec& origin,
                               Vec* inOut)
{
	f32 radius;
	s16 vAngle;
	s16 hAngle;

	CLBCrossToPolar(origin, *inOut, &radius, &vAngle, &hAngle);
	vAngle = MsClamp(vAngle, vAngleMin, vAngleMax);
	CLBPolarToCross(origin, inOut, radius, vAngle, hAngle);
}

void CLBRotatePosAndUp(s16 sAngle1, s16 sAngle2,
                       const JGeometry::TVec3<f32>& axis1,
                       const JGeometry::TVec3<f32>& axis2,
                       const JGeometry::TVec3<f32>& offset,
                       JGeometry::TVec3<f32>* param_6,
                       JGeometry::TVec3<f32>* param_7)

{
	f32 angle1 = sAngle1 * SHORTANGLE_TO_DEGREES * DEGREES_TO_RADIANS;
	f32 angle2 = sAngle2 * SHORTANGLE_TO_DEGREES * DEGREES_TO_RADIANS;

	JGeometry::TVec3<f32> v1 = *param_6 - offset;
	RotateAboutAxis(axis1, -angle1, &v1);
	*param_6 = offset + v1;
	RotateAboutAxis(axis1, -angle1, param_7);

	JGeometry::TVec3<f32> v2 = *param_6 - offset;
	RotateAboutAxis(axis2, -angle2, &v2);
	*param_6 = offset + v2;
	RotateAboutAxis(axis2, -angle2, param_7);
}
