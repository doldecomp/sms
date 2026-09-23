#include <types.h>
#include <Camera/cameralib.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>

// TODO: This macro should probably be consolidated elsewhere
#define ABS(x) ((x) >= 0 ? (x) : -(x))

const JGeometry::TVec3<f32> CLBConstUpVec(0.0f, 1.0f, 0.0f);

static const f32 SHORTANGLE_TO_DEGREES = 0.005493164f; // 360/65536
static const f32 DEGREES_TO_RADIANS    = 0.017453294f; // pi/180

// TODO: fabricated, and only the level it adds is evidence. Each normalize
// site needs TVec3::normalize() one statement below it, so that
// TVec3::setLength lands past its depth allowance and stays the ROM's `bl`;
// called directly, setLength expands and its dot/inv_sqrt/scale leaves become
// the calls instead. The real name is unknown -- nothing weak or UNUSED in the
// map covers it -- so it is parked here rather than in cameralib.hpp.
static void normalizeInner1(JGeometry::TVec3<f32>& vec)
{
	vec.setLength(1.0f);
}

// The copy of the input vector is the ROM's: mult33 writes its result back over
// its argument, so the ROM reloads the operands from a copy taken after
// setRotate returns. It has to stay spelled out here rather than moving into
// the one-argument `TRotation3::mult33(TVec3&)`: with the forwarder spelled
// `TVec3<f32> tmp(v); mult33(tmp, v);` the other one-argument caller,
// TMapWire::init, drops 98.79 -> 95.91%, so only this site wants the copy.
// Declaring `in` before `mtxT` (67.3%) or as a bare `Vec` (73.9%) is worse
// than this order (83.8%).
static inline void RotateAboutAxis(const JGeometry::TVec3<f32>& param_axis,
                                   f32 angle, JGeometry::TVec3<f32>* vec)
{
	JGeometry::TRotation3<TMtx33f> mtxT;

	mtxT.identity();
	mtxT.setRotate(param_axis, angle);

	JGeometry::TVec3<f32> in(*vec);
	vec->x = mtxT.at(0, 0) * in.x + mtxT.at(1, 0) * in.y + mtxT.at(2, 0) * in.z;
	vec->y = mtxT.at(0, 1) * in.x + mtxT.at(1, 1) * in.y + mtxT.at(2, 1) * in.z;
	vec->z = mtxT.at(0, 2) * in.x + mtxT.at(1, 2) * in.y + mtxT.at(2, 2) * in.z;
}

// TODO: 98.7%. Frame 0x40 against 0x58 -- 24 bytes, and the ROM's locals start
// at 0x38 where ours start at 0x24 -- plus the float register numbering that
// follows.
// Two levels for CLBCalc2DFPos: the reciprocal of the view depth and the
// projected depth, computed in the value parameter in place. Together they
// give retail's volatile FPR assignment and the frame (0x58).
static inline f32 CLBInvDepth(f32 z)
{
	return 1.0f / -z;
}

static inline f32 CLBProjectDepth(const f32 (*proj_mtx)[4], f32 z, f32 scale)
{
	z = proj_mtx[2][2] * z + proj_mtx[2][3];
	z *= scale;
	return z;
}

void CLBCalc2DFPos(JGeometry::TVec2<f32>* out_ndc_pos, const f32 (*proj_mtx)[4],
                   const f32 (*view_mtx)[4], const Vec& world_pos,
                   u32* out_depth, bool disable_z_clip)
{
	Vec camSpacePos;

	MTXMultVec((MtxPtr)view_mtx, (Vec*)&world_pos, &camSpacePos);

	if (camSpacePos.z == 0.0f) {
		out_ndc_pos->x = out_ndc_pos->y = 10000.0f;
		return;
	}

	f32 perspectiveFactor = CLBInvDepth(camSpacePos.z);

	f32 z = CLBProjectDepth(proj_mtx, camSpacePos.z, perspectiveFactor);
	if (!disable_z_clip && (z > 0.0f || z < -1.0f)) {
		out_ndc_pos->x = out_ndc_pos->y = 10000.0f;
		return;
	}

	f32 x = proj_mtx[0][0] * camSpacePos.x + proj_mtx[0][2] * camSpacePos.z;
	f32 y = proj_mtx[1][1] * camSpacePos.y + proj_mtx[1][2] * camSpacePos.z;
	out_ndc_pos->set(x * perspectiveFactor, y * perspectiveFactor);

	if (out_depth != nullptr)
		*out_depth = CLBLinearInbetween<u32>(0, 0xffffff, z + 1.0f);
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

// TODO: 97.8%. Frame 0x18 short, and the third row of RotateAboutAxis's
// product reloads in.x/m(0,2) in retail where ours reuses a register.
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

// TODO: 98.5%. Instruction-identical apart from the frame (0x58 against the
// ROM's 0x98) and the float register pair that the three
// `fneg`/literal-load/multiply triples pick. 64 bytes of uninitialised locals
// close the frame (validated with padding, which is not committed) but the
// byte count is the only evidence for what they were, so per
// docs/catalog/frame-gaps.md the gap is left alone.
bool CLBIsPointInCube(const Vec& param_1, const Vec& param_2,
                      const Vec& param_3, const Vec& param_4)
{
	// TODO: Why is so much of this copy-pasted from CLBCalcPointInCubeRatio?
	// TODO: frame 0x70 against 0x98. The header CLBDegToShortAngle fixed the
	// angle-product registers (+0x18); tried (cc41) for the rest: JMASCos/
	// JMASSin forks and binders (frame lands, lfsx pair swaps), a product
	// helper for the cross terms, and direct/named half-extent helpers on the
	// final test at every site subset (+0x20 clean at the two upper bounds,
	// every frame-exact subset changes code).

	f32 dx = param_1.x - param_2.x;
	f32 dy = param_1.y - param_2.y;
	f32 dz = param_1.z - param_2.z;

	bool result = false;

	if (param_3.y != 0.0f || param_3.x != 0.0f || param_3.z != 0.0f) {
		if (param_3.z != 0.0f) {
			s16 zAngle = CLBDegToShortAngle(-param_3.z);
			f32 cosZ   = JMASCos(zAngle);
			f32 sinZ   = JMASSin(zAngle);

			f32 dySinZ = dy * sinZ;

			dy = dx * sinZ + dy * cosZ;
			dx = dx * cosZ - dySinZ;
		}

		if (param_3.y != 0.0f) {
			s16 yAngle = CLBDegToShortAngle(-param_3.y);
			f32 cosY   = JMASCos(yAngle);
			f32 sinY   = JMASSin(yAngle);

			f32 dzSinY = dz * sinY;

			dz = -dx * sinY + dz * cosY;
			dx = dx * cosY + dzSinY;
		}

		if (param_3.x != 0.0f) {
			s16 xAngle = CLBDegToShortAngle(-param_3.x);
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

// TODO: 98.4%, the same shape as CLBIsPointInCube below: frame 0x70 against
// 0xb0 (64 bytes) and the float registers of the three rotation blocks.
void CLBCalcPointInCubeRatio(const Vec& param_1, const Vec& param_2,
                             const Vec& param_3, const Vec& param_4,
                             f32* param_5, f32* param_6, f32* param_7)
{
	f32 dx = param_1.x - param_2.x;
	f32 dy = param_1.y - param_2.y;
	f32 dz = param_1.z - param_2.z;

	if (param_3.z != 0) {
		s16 zAngle = CLBDegToShortAngle(-param_3.z);
		f32 cosZ   = JMASCos(zAngle);
		f32 sinZ   = JMASSin(zAngle);

		f32 dySinZ = dy * sinZ;

		dy = dx * sinZ + dy * cosZ;
		dx = dx * cosZ - dySinZ;
	}

	if (param_3.y != 0) {
		s16 yAngle = CLBDegToShortAngle(-param_3.y);
		f32 cosY   = JMASCos(yAngle);
		f32 sinY   = JMASSin(yAngle);

		f32 dzSinY = dz * sinY;

		dz = -dx * sinY + dz * cosY;
		dx = dx * cosY + dzSinY;
	}

	if (param_3.x != 0) {
		s16 xAngle = CLBDegToShortAngle(-param_3.x);
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

// TODO: fabricated. CLBCalcNearNinePos transforms the camera-space up and
// right vectors identically, and the ROM *calls* TRotation3::setRotate at both
// sites (the map's weak 0x154 copy) while our spelled-out blocks expand it,
// which needs the statement to sit one inline level down. The ROM rotates the
// vector in place, about X and then about Y, reading the Y angle only after
// the X rotation is stored.
static inline void
CLBRotateVecByEulerAndRoll(JGeometry::TVec3<f32>* vec, const S16Vec& euler,
                           const JGeometry::TVec3<f32>& axis, f32 roll)
{
	f32 cosX = JMASCos(euler.x);
	f32 sinX = JMASSin(euler.x);
	f32 y  = vec->y;
	vec->y = y * cosX - vec->z * sinX;
	vec->z = y * sinX + vec->z * cosX;

	f32 cosY = JMASCos(euler.y);
	f32 sinY = JMASSin(euler.y);
	f32 x  = vec->x;
	vec->x = x * cosY + vec->z * sinY;
	vec->z = -x * sinY + vec->z * cosY;

	JGeometry::TRotation3<TMtx33f> mtxT;

	mtxT.identity();
	mtxT.setRotate(axis, roll);

	JGeometry::TVec3<f32> in(*vec);
	mtxT.mult33(in, *vec);
}

// The ROM calls MsSqrtf out of line inside the inlined CLBCalcNearClipAngle,
// so the square root sits two TU-local levels below ClipAngle (the inline
// allowance runs out there, one level deeper than in the standalone body).
// Neither level leaves a map name when fully inlined; the split into a
// point-pair distance over a component length is a reconstruction.
static inline f32 CLBCalcLengthXZ(f32 dx, f32 dz)
{
	return MsSqrtf(dx * dx + dz * dz);
}

static inline f32 CLBCalcDistanceXZ(const JGeometry::TVec3<f32>& a,
                                    const JGeometry::TVec3<f32>& b)
{
	return CLBCalcLengthXZ(a.x - b.x, a.z - b.z);
}

// UNUSED in the map (0x154); ours is 0x14c. The body is CLBCalcNearNinePos'
// own head: the name, the argument list (NinePos' minus the near-plane
// dimensions), the emission position (between setRotate and
// CLBCalcScaleTranslateMatrix, so immediately before NinePos in source order)
// and the size all agree.
void CLBCalcNearClipAngle(JGeometry::TVec3<f32>* out_center, S16Vec* out_euler,
                          const JGeometry::TVec3<f32>& origin,
                          const JGeometry::TVec3<f32>& lookat, s16 roll,
                          f32 near_dist)
{
	JGeometry::TVec3<f32> dir;

	dir.set(lookat.x - origin.x, lookat.y - origin.y, lookat.z - origin.z);
	normalizeInner1(dir);

	out_center->scaleAdd(near_dist, dir, origin);

	f32 xzDistance = CLBCalcDistanceXZ(origin, lookat);
	out_euler->x   = -matan(xzDistance, origin.y - lookat.y);
	out_euler->y   = matan(origin.z - lookat.z, origin.x - lookat.x);
	out_euler->z   = roll;
}

// TODO: the call structure and the scalarised offset vectors now match the
// ROM. The rest is the frame (0x208 against 0x1f0: a 4-byte hole between dir
// and corner in the ROM, and a larger inline-temporary region here) and the
// float register numbering (the ROM keeps the squared half-diagonal in f31,
// above the scalarised upOfs; a named squared local declared early is worse).
void CLBCalcNearNinePos(JGeometry::TVec3<f32>* out_grid, S16Vec* out_euler,
                        const JGeometry::TVec3<f32>& origin,
                        const JGeometry::TVec3<f32>& lookat, s16 roll,
                        f32 near_dist, const JGeometry::TVec2<f32>& near_dims)
{
	JGeometry::TVec3<f32> up;
	JGeometry::TVec3<f32> side;
	JGeometry::TVec3<f32> dir;
	JGeometry::TVec3<f32> corner;
	JGeometry::TVec3<f32> upOfs;
	JGeometry::TVec3<f32> leftOfs;
	JGeometry::TVec3<f32> rightOfs;

	CLBCalcNearClipAngle(&out_grid[4], out_euler, origin, lookat, roll,
	                     near_dist);

	up.set(0.0f, 1.0f, 0.0f);
	side.set(1.0f, 0.0f, 0.0f);

	dir.set(lookat.x - origin.x, lookat.y - origin.y, lookat.z - origin.z);
	normalizeInner1(dir);

	f32 rollRad = out_euler->z * SHORTANGLE_TO_DEGREES * DEGREES_TO_RADIANS;

	// Transform the up/right vectors from cam space into world space.
	CLBRotateVecByEulerAndRoll(&up, *out_euler, dir, rollRad);
	CLBRotateVecByEulerAndRoll(&side, *out_euler, dir, rollRad);

	f32 halfHeight = near_dims.y * 0.5f;
	f32 halfWidth  = near_dims.x * 0.5f;

	upOfs.scale(halfHeight, up);
	out_grid[1].add(out_grid[4], upOfs);
	out_grid[7].scaleAdd(-halfHeight, up, out_grid[4]);
	leftOfs.scale(-halfWidth, side);
	out_grid[3].add(out_grid[4], leftOfs);
	rightOfs.scale(halfWidth, side);
	out_grid[5].add(out_grid[4], rightOfs);

	f32 halfDiagonal = MsSqrtf(halfHeight * halfHeight + halfWidth * halfWidth);

	corner.set(leftOfs);
	corner.add(upOfs);

	MsVECNormalize(&corner, &corner);
	out_grid[0].scaleAdd(halfDiagonal, corner, out_grid[4]);
	corner.negate();
	out_grid[8].scaleAdd(halfDiagonal, corner, out_grid[4]);

	corner.set(rightOfs);
	corner.add(upOfs);

	MsVECNormalize(&corner, &corner);
	out_grid[2].scaleAdd(halfDiagonal, corner, out_grid[4]);
	corner.negate();
	out_grid[6].scaleAdd(halfDiagonal, corner, out_grid[4]);
}

// UNUSED in the map (0x104), and emitted before CLBCalcNearNinePos, so in
// source order it is the last function in the file and may call NinePos.
// The body is reconstructed, but it compiles to the map's 0x104 exactly: a
// nine-point grid local, one call to NinePos, and five vector copies out. The
// four points are the near plane's corners -- the grid entries NinePos writes
// with the half-diagonal -- and the second output is the plane centre. The
// parameter names are guesses.
void CLBCalcNearFourPos(JGeometry::TVec3<f32>* out_quad,
                        JGeometry::TVec3<f32>* out_center, S16Vec* out_euler,
                        const JGeometry::TVec3<f32>& origin,
                        const JGeometry::TVec3<f32>& lookat, s16 roll,
                        f32 near_dist,
                        const JGeometry::TVec2<f32>& near_dims)
{
	JGeometry::TVec3<f32> grid[9];

	CLBCalcNearNinePos(grid, out_euler, origin, lookat, roll, near_dist,
	                   near_dims);

	out_quad[0].set(grid[0]);
	out_quad[1].set(grid[2]);
	out_quad[2].set(grid[6]);
	out_quad[3].set(grid[8]);
	out_center->set(grid[4]);
}
