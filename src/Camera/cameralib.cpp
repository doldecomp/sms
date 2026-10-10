#include <dolphin/types.h>
#include <Camera/cameralib.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>

// TODO: This macro should probably be consolidated elsewhere
#define ABS(x) ((x) >= 0 ? (x) : -(x))

const JGeometry::TVec3<f32> CLBConstUpVec(0.0f, 1.0f, 0.0f);

static const f32 SHORTANGLE_TO_DEGREES = 0.005493164f; // 360/65536
static const f32 DEGREES_TO_RADIANS    = 0.017453294f; // pi/180

// TODO: fabricated; shared direction inline also appears in the camera
// siblings.
static inline void unitVecTo(const JGeometry::TVec3<f32>& from,
                             const JGeometry::TVec3<f32>& to,
                             JGeometry::TVec3<f32>* out)
{
	out->set(to.x - from.x, to.y - from.y, to.z - from.z);
	out->normalize();
}
// TODO: fabricated helper; target keeps the matrix and source copy together.
static inline void RotateAboutAxis(const JGeometry::TVec3<f32>& param_axis,
                                   f32 angle, JGeometry::TVec3<f32>& vec)
{
	JGeometry::TRotation3<TMtx33f> mtxT;

	mtxT.identity();
	mtxT.setRotate(param_axis, angle);
	const JGeometry::TVec3<f32> src = vec;
	CLBMultTranspose33(mtxT, src, vec);
}

void CLBCalc2DFPos(JGeometry::TVec2<f32>* out_ndc_pos, const f32 (*proj_mtx)[4],
                   const f32 (*view_mtx)[4], const Vec& world_pos,
                   u32* out_depth, bool disable_z_clip)
{
	JGeometry::TVec3<f32> projPos;
	Vec camSpacePos;

	MTXMultVec((MtxPtr)view_mtx, (Vec*)&world_pos, &camSpacePos);

	if (camSpacePos.z == 0.0f) {
		out_ndc_pos->x = out_ndc_pos->y = 10000.0f;
		return;
	}

	f32 perspectiveFactor = 1.0f / -camSpacePos.z;

	projPos.z = proj_mtx[2][2] * camSpacePos.z + proj_mtx[2][3];
	projPos.z *= perspectiveFactor;
	if (!disable_z_clip && (projPos.z > 0.0f || projPos.z < -1.0f)) {
		out_ndc_pos->x = out_ndc_pos->y = 10000.0f;
		return;
	}

	projPos.set(proj_mtx[0][0] * camSpacePos.x + proj_mtx[0][2] * camSpacePos.z,
	            proj_mtx[1][1] * camSpacePos.y + proj_mtx[1][2] * camSpacePos.z,
	            projPos.z);
	out_ndc_pos->set(projPos.x * perspectiveFactor,
	                 projPos.y * perspectiveFactor);

	if (out_depth != nullptr)
		*out_depth = CLBLinearInbetween<u32>(0, 0xffffff, projPos.z + 1.0f);
}

BOOL CLBChaseAngleDecrease(s16* value, s16 desired, s16 ratio)
{
	if (ratio == 0) {
		*value = desired;
	} else {
		s16 newValue = *value - desired;
		newValue -= newValue / ratio;
		newValue += desired;
		if (newValue == *value)
			return false;

		*value = newValue;
	}

	if (*value == desired)
		return false;

	return true;
}

BOOL CLBChaseDecrease(f32* value, f32 desired, f32 ratio, f32 threshold)
{
	if (ratio > 1.0f)
		ratio = 1.0f;

	*value += ratio * (desired - *value);

	// You'd think this would just be
	// "return ABS(*dstValue - targetValue) > ABS(threshold);"
	// but this gives an exact match
	if (ABS(*value - desired) <= ABS(threshold))
		return false;

	return true;
}

BOOL CLBChaseSpecialDecrease(f32* value, f32 desired, f32 ratio, f32 speed)
{
	if (ratio > 1.0f)
		ratio = 1.0f;

	f32 actualSpeed = ratio * (desired - *value);

	if (CLBAbs(actualSpeed) < CLBAbs(speed))
		actualSpeed = speed;

	return CLBChaseGeneralConstantSpecifySpeed(value, desired, actualSpeed);
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

// TODO: fabricated; shared rotation boundary inferred from the cube functions.
static inline void RotatePair(f32 angle, f32& first, f32& second)
{
	s16 shortAngle = CLBDegToShortAngle(angle);
	f32 cosAngle   = JMASCos(shortAngle);
	f32 sinAngle   = JMASSin(shortAngle);
	f32 secondSin  = second * sinAngle;

	second = first * sinAngle + second * cosAngle;
	first  = first * cosAngle - secondSin;
}

// TODO: fabricated; the Y rotation has the opposite orientation to X and Z.
static inline void RotatePairY(f32 angle, f32& first, f32& second)
{
	s16 shortAngle = CLBDegToShortAngle(angle);
	f32 cosAngle   = JMASCos(shortAngle);
	f32 sinAngle   = JMASSin(shortAngle);
	f32 secondSin  = second * sinAngle;

	second = -first * sinAngle + second * cosAngle;
	first  = first * cosAngle + secondSin;
}

// TODO: fabricated; common inverse rotation recovered from both cube functions.
static inline void RotateCubePoint(const Vec& rotate, f32& dx, f32& dy, f32& dz)
{
	if (rotate.z != 0.0f)
		RotatePair(-rotate.z, dx, dy);
	if (rotate.y != 0.0f)
		RotatePairY(-rotate.y, dx, dz);
	if (rotate.x != 0.0f)
		RotatePair(-rotate.x, dy, dz);
}

bool CLBIsPointInCube(const Vec& param_1, const Vec& param_2,
                      const Vec& param_3, const Vec& param_4)
{
	// TODO: Why is so much of this copy-pasted from CLBCalcPointInCubeRatio?

	Vec localPos;
	localPos.x = param_1.x - param_2.x;
	localPos.y = param_1.y - param_2.y;
	localPos.z = param_1.z - param_2.z;

	bool result = false;

	if (param_3.y != 0.0f || param_3.x != 0.0f || param_3.z != 0.0f) {
		RotateCubePoint(param_3, localPos.x, localPos.y, localPos.z);
	}

	if ((-param_4.x * 0.5f < localPos.x && localPos.x < param_4.x * 0.5f)
	    && (0.0f < localPos.y && localPos.y < param_4.y)
	    && (-param_4.z * 0.5f < localPos.z && localPos.z < param_4.z * 0.5f)) {
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

	RotateCubePoint(param_3, dx, dy, dz);

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
	s16 sAngleX = CLBDegToShortAngle(rotate.x);
	s16 sAngleY = CLBDegToShortAngle(rotate.y);
	s16 sAngleZ = CLBDegToShortAngle(rotate.z);

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

// TODO: fabricated pitch boundary; the target retains the XZ square root call.
static inline s16 CalcPitch(const Vec& origin, const Vec& in)
{
	f32 dx = in.x - origin.x;
	f32 dz = in.z - origin.z;
	return matan(MsSqrtf(dx * dx + dz * dz), in.y - origin.y);
}

// Recovered from the opening inline in CLBCalcNearNinePos.
// UNUSED body also has the JP map's 0x154 size.
void CLBCalcNearClipAngle(JGeometry::TVec3<f32>* out_pos, S16Vec* out_euler,
                          const JGeometry::TVec3<f32>& origin,
                          const JGeometry::TVec3<f32>& lookat, s16 roll,
                          f32 near_dist)
{
	JGeometry::TVec3<f32> direction;
	unitVecTo(origin, lookat, &direction);
	out_pos->scaleAdd(near_dist, origin, direction);

	out_euler->x = -CalcPitch(lookat, origin);
	out_euler->y = matan(origin.z - lookat.z, origin.x - lookat.x);
	out_euler->z = roll;
}

// TODO: fabricated shared boundary; both plane axes use this X/Y rotation.
static inline void RotateByAngles(const S16Vec& angles,
                                  JGeometry::TVec3<f32>& vec)
{
	f32 cosX = JMASCos(angles.x);
	f32 sinX = JMASSin(angles.x);
	f32 y    = vec.y;
	vec.y    = y * cosX - vec.z * sinX;
	vec.z    = y * sinX + vec.z * cosX;
	f32 cosY = JMASCos(angles.y);
	f32 sinY = JMASSin(angles.y);
	f32 x    = vec.x;
	vec.x    = x * cosY + vec.z * sinY;
	vec.z    = -x * sinY + vec.z * cosY;
}

void CLBCalcNearNinePos(JGeometry::TVec3<f32>* out_grid, S16Vec* out_euler,
                        const JGeometry::TVec3<f32>& origin,
                        const JGeometry::TVec3<f32>& lookat, s16 roll,
                        f32 near_dist, const JGeometry::TVec2<f32>& near_dims)
{
	// TODO: center/rotation inline ownership and float lifetimes still differ.

	JGeometry::TVec3<f32> local_68;
	JGeometry::TVec3<f32> local_74;
	JGeometry::TVec3<f32> local_80;
	JGeometry::TVec3<f32> local_90;

	JGeometry::TVec3<f32> fVar16;
	JGeometry::TVec3<f32> fVar19;

	CLBCalcNearClipAngle(&out_grid[4], out_euler, origin, lookat, roll,
	                     near_dist);

	local_68.set(0.0f, 1.0f, 0.0f);
	local_74.set(1.0f, 0.0f, 0.0f);

	unitVecTo(origin, lookat, &local_80);

	f32 shortRoll = out_euler->z;
	f32 rollAngle = shortRoll * SHORTANGLE_TO_DEGREES * DEGREES_TO_RADIANS;

	// Transform the up/right vectors from camera space into world space.
	RotateByAngles(*out_euler, local_68);
	RotateAboutAxis(local_80, rollAngle, local_68);
	RotateByAngles(*out_euler, local_74);
	RotateAboutAxis(local_80, rollAngle, local_74);

	f32 fVar3 = near_dims.y * 0.5f;
	f32 fVar5 = near_dims.x * 0.5f;
	f32 fVar6 = -fVar3;
	f32 fVar7 = -fVar5;

	fVar19.scale(fVar3, local_68);
	out_grid[1].add(out_grid[4], fVar19);
	out_grid[7].scaleAdd(fVar6, out_grid[4], local_68);
	JGeometry::TVec3<f32> negativeRight;
	negativeRight.scale(fVar7, local_74);
	out_grid[3].add(out_grid[4], negativeRight);
	fVar16.scale(fVar5, local_74);
	out_grid[5].add(out_grid[4], fVar16);

	f32 halfWidthSquared  = fVar5 * fVar5;
	f32 halfPlaneDiagonal = MsSqrtf(fVar3 * fVar3 + halfWidthSquared);

	local_90.set(negativeRight);
	local_90.add(fVar19);
	MsVECNormalize(&local_90, &local_90);

	out_grid[0].scaleAdd(halfPlaneDiagonal, out_grid[4], local_90);
	local_90.negate();
	out_grid[8].scaleAdd(halfPlaneDiagonal, out_grid[4], local_90);

	local_90.set(fVar16);
	local_90.add(fVar19);
	MsVECNormalize(&local_90, &local_90);

	out_grid[2].scaleAdd(halfPlaneDiagonal, out_grid[4], local_90);
	local_90.negate();
	out_grid[6].scaleAdd(halfPlaneDiagonal, out_grid[4], local_90);
}

void CLBCalcNearFourPos(JGeometry::TVec3<f32>* out_corners,
                        JGeometry::TVec3<f32>* out_center, S16Vec* out_euler,
                        const JGeometry::TVec3<f32>& origin,
                        const JGeometry::TVec3<f32>& lookat, s16 roll,
                        f32 near_dist, const JGeometry::TVec2<f32>& near_dims)
{
	// TODO: UNUSED wrapper; corner ordering has no surviving caller.
	// NinePos's row-major grid supplies the four corners and center.
	// Compiled size is 0x168; the JP map records 0x104.
	JGeometry::TVec3<f32> grid[9];
	CLBCalcNearNinePos(grid, out_euler, origin, lookat, roll, near_dist,
	                   near_dims);
	for (int row = 0; row < 2; ++row) {
		for (int col = 0; col < 2; ++col)
			out_corners[row * 2 + col] = grid[row * 6 + col * 2];
	}
	*out_center = grid[4];
}

// @todo: .sdata2 order: cameralib.cpp's @1906 through @2404 in the JP object.
// Extra weak-inline constants otherwise displace the cube predicates' literals.
void order_sdata2(f32* constants, f64* conversion)
{
	constants[0]  = 1.0f;
	constants[1]  = 0.0f;
	constants[2]  = 3.81469727e-06f;
	constants[3]  = 0.0174532942f;
	constants[4]  = 0.00549316406f;
	constants[5]  = 0.5f;
	conversion[0] = 0.5;
	conversion[1] = 3.0;
	conversion[2] = 4503601774854144.0;
	constants[6]  = 182.044449f;
	constants[7]  = 10000.0f;
	constants[8]  = -1.0f;
}
