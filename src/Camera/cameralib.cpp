#include <types.h>
#include <Camera/cameralib.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>

// TODO: This macro should probably be consolidated elsewhere
#define ABS(x) ((x) >= 0 ? (x) : -(x))

const JGeometry::TVec3<f32> CLBConstUpVec(0.0f, 1.0f, 0.0f);

static const f32 SHORTANGLE_TO_DEGREES = 0.005493164f; // 360/65536
static const f32 DEGREES_TO_RADIANS    = 0.017453294f; // pi/180

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

	mtxT.identity();
	mtxT.setRotate(param_axis, angle);
	mtxT.mult33(*vec);
}

// TODO: Explore how this is used, and add documentation
void CLBCalc2DFPos(JGeometry::TVec2<f32>* param_1, const f32 (*param_2)[4],
                   const f32 (*param_3)[4], const Vec& param_4, u32* param_5,
                   bool param_6)
{
	Vec prod;

	MTXMultVec((MtxPtr)param_3, (Vec*)&param_4, &prod);

	if (prod.z == 0.0f) {
		param_1->x = param_1->y = 10000.0f;
		return;
	}

	f32 fVar3 = 1.0f / -prod.z;
	f32 fVar4 = param_2[2][2] * prod.z + param_2[2][3];
	fVar4 *= fVar3;
	if (!param_6 && (fVar4 > 0.0f || fVar4 < -1.0f)) {
		param_1->x = param_1->y = 10000.0f;
		return;
	}

	f32 x = (param_2[0][0] * prod.x + param_2[0][2] * prod.z);
	f32 y = (param_2[1][1] * prod.y + param_2[1][2] * prod.z);
	param_1->set(x * fVar3, y * fVar3);

	if (param_5 != nullptr)
		*param_5 = CLBLinearInbetween<u32>(0, 0xffffff, fVar4 + 1.0f);
}

BOOL CLBChaseAngleDecrease(s16* value, s16 target, s16 ratio)
{
	if (ratio == 0) {
		*value = target;
	} else {
		s16 newValue = *value - target;
		newValue -= newValue / ratio;
		newValue += target;
		if (newValue == *value)
			return false;

		*value = newValue;
	}

	if (*value == target)
		return false;

	return true;
}

BOOL CLBChaseDecrease(f32* dstValue, f32 targetValue, f32 ratio, f32 threshold)
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

BOOL CLBChaseSpecialDecrease(f32* param_1, f32 param_2, f32 param_3,
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

void CLBCrossToPolar(const Vec& origin, const Vec& in, f32* out_radius,
                     s16* out_pitch, s16* out_yaw)
{
	f32 dx = in.x - origin.x;
	f32 dy = in.y - origin.y;
	f32 dz = in.z - origin.z;

	*out_radius = MsSqrtf(dx * dx + dy * dy + dz * dz);

	*out_pitch = matan(MsSqrtf(dx * dx + dz * dz), dy);
	*out_yaw   = matan(dz, dx);
}

void CLBPolarToCross(const Vec& origin, Vec* out, f32 radius, s16 vAngle,
                     s16 hAngle)
{
	out->x = origin.x + radius * JMASCos(vAngle) * JMASSin(hAngle);
	out->y = origin.y + radius * JMASSin(vAngle);
	out->z = origin.z + radius * JMASCos(vAngle) * JMASCos(hAngle);
}

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

void CLBCalcNearNinePos(JGeometry::TVec3<f32>* out_grid, S16Vec* out_euler,
                        const JGeometry::TVec3<f32>& origin,
                        const JGeometry::TVec3<f32>& lookat, s16 roll,
                        f32 near_dist, const JGeometry::TVec2<f32>& near_dims)
{
	// TODO: This needs matching work, but it's mathematically correct

	JGeometry::TVec3<f32> fVar16;
	JGeometry::TVec3<f32> fVar19;

	JGeometry::TRotation3<TMtx33f> local_118;

	JGeometry::TRotation3<TMtx33f> local_e4;

	JGeometry::TVec3<f32> local_a8;
	JGeometry::TVec3<f32> local_90;
	JGeometry::TVec3<f32> local_80;
	JGeometry::TVec3<f32> local_74;
	JGeometry::TVec3<f32> local_68;

	local_a8.sub(lookat, origin);
	normalizeInner2(local_a8);

	// Center point
	out_grid[4].scaleAdd(near_dist, origin, local_a8);

	f32 xzDistance = MsSqrtf(((origin.x - lookat.x) * (origin.x - lookat.x)
	                          + (origin.z - lookat.z) * (origin.z - lookat.z)));
	out_euler->x   = -matan(xzDistance, origin.y - lookat.y);
	out_euler->y   = matan(origin.z - lookat.z, origin.x - lookat.x);
	out_euler->z   = roll;

	local_68.set(0.0f, 1.0f, 0.0f);
	local_74.set(1.0f, 0.0f, 0.0f);

	// We already did this, so maybe we're calling another function here?
	local_80.sub(lookat, origin);
	normalizeInner1(local_80);

	fVar16.z = out_euler->z * SHORTANGLE_TO_DEGREES * DEGREES_TO_RADIANS;

	// Basically transform the up/right vectors from cam space into world space.
	// TODO: Definitely inlines...

	{
		f32 sinX = JMASSin(out_euler->x);
		f32 cosX = JMASCos(out_euler->x);
		f32 sinY = JMASSin(out_euler->y);
		f32 cosY = JMASCos(out_euler->y);

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

		local_e4.mult33(local_68);
	}

	{
		f32 sinX = JMASSin(out_euler->x);
		f32 cosX = JMASCos(out_euler->x);
		f32 sinY = JMASSin(out_euler->y);
		f32 cosY = JMASCos(out_euler->y);

		local_74.set(local_74.x * cosY
		                 + (local_74.y * sinX + local_74.z * cosX) * sinY,
		             local_74.y * cosX - local_74.z * sinX,
		             -local_74.x * sinY
		                 + (local_74.y * sinX + local_74.z * cosX) * cosY);

		local_118.identity33();
		local_118.setRotate(local_80, fVar16.z);

		local_118.mult33(local_74);
	}

	f32 fVar3 = near_dims.y * 0.5f;
	f32 fVar5 = near_dims.x * 0.5f;
	f32 fVar6 = -fVar3;
	f32 fVar7 = -fVar5;

	out_grid[1].scaleAdd(fVar3, out_grid[4], local_68);
	out_grid[7].scaleAdd(fVar6, out_grid[4], local_68);
	out_grid[3].scaleAdd(fVar7, out_grid[4], local_74);
	out_grid[5].scaleAdd(fVar5, out_grid[4], local_74);

	// Anything below here could be part of CLBCalcNearFourPos?

	fVar16.scale(fVar5, local_74);
	fVar19.scale(fVar3, local_68);

	f32 halfPlaneDiagonal = MsSqrtf(fVar3 * fVar3 + fVar5 * fVar5);

	local_90.scaleAdd(fVar7, fVar19, local_74);
	MsVECNormalize(&local_90, &local_90);

	out_grid[0].scaleAdd(halfPlaneDiagonal, out_grid[4], local_90);
	local_90.negate();
	out_grid[8].scaleAdd(halfPlaneDiagonal, out_grid[4], local_90);

	local_90.x = (f32)(fVar16.x + fVar19.x);
	local_90.y = (f32)(fVar16.y + fVar19.y);
	local_90.z = (f32)(fVar16.z + fVar19.z);
	MsVECNormalize(&local_90, &local_90);

	out_grid[2].scaleAdd(halfPlaneDiagonal, out_grid[4], local_90);
	local_90.negate();
	out_grid[6].scaleAdd(halfPlaneDiagonal, out_grid[4], local_90);
}
