#ifndef MARIO_UTIL_MATH_UTIL_HPP
#define MARIO_UTIL_MATH_UTIL_HPP

#include <JSystem/JGeometry.hpp>

int IConverge(int, int, int, int);
f32 FConverge(f32, f32, f32, f32);
s16 matan(f32, f32);

// fabricated
inline f32 MsAtan2(f32 y, f32 x)
{
	return abs(matan(y, x) * (360.0f / 65536.0f));
}

inline f32 MsGetRotFromZaxisY(const JGeometry::TVec3<f32>& axis)
{
	if (axis.z == 0.0f) {
		if (axis.x >= 0.0f)
			return 90.0f;
		else
			return -90.0f;
	}

	if (axis.z >= 0.0f) {
		return (360.0f / 65536.0f) * matan(axis.z, axis.x);
	} else {
		f32 theta = matan(-axis.z, axis.x) * (360.0f / 65536.0f);
		return 180.0f - theta;
	}
}

JGeometry::TVec3<f32> MsGetRotFromZaxis(const JGeometry::TVec3<f32>&);
void MsMtxSetRotRPH(MtxPtr mtx, f32 x, f32 y, f32 z);
void MsMtxSetXYZRPH(MtxPtr mtx, f32 x, f32 y, f32 z, s16 r, s16 p, s16 h);
inline void MsMtxSetXYZRPH(MtxPtr mtx, f32 x, f32 y, f32 z, f32 r, f32 p, f32 h)
{
	MsMtxSetXYZRPH(mtx, x, y, z, static_cast<s16>(r * (65536.0f / 360.0f)),
	               static_cast<s16>(p * (65536.0f / 360.0f)),
	               static_cast<s16>(h * (65536.0f / 360.0f)));
}
void MsMtxSetTRS(MtxPtr, f32, f32, f32, f32, f32, f32, f32, f32, f32);

template <class T> inline T MsWrap(T t, T l, T r)
{
	if (l >= r)
		return l;

	while (t >= r)
		t -= r - l;
	while (t < l)
		t += r - l;

	return t;
}

template <class T> inline T MsClamp(T t, T l, T r)
{
	if (t > r)
		t = r;
	else if (t < l)
		t = l;
	return t;
}

// fabricated
inline f32 MsAngleDiff(f32 alpha, f32 beta)
{
	return alpha - MsWrap(beta, alpha - 180.0f, alpha + 180.0f);
}

/**
 * @brief Checks whether the point \p target is within the line of sight of
 * an \p eye looking in direction \p sight, given a sight cone \p angle and
 * \p length, as well as a distance \p aware, which is the distance at which
 * the target is considered to be in sight regardless of the angle.
 *
 * @param eye the point from which the sight is checked
 * @param sight the direction of sight
 * @param target the point to check
 * @param length the distance of the sight
 * @param angle the sweep angle of the sight cone
 * @param aware distance closer than which angle is not considered
 * @return BOOL true if the target is in sight, false otherwise
 */
BOOL MsIsInSight(const JGeometry::TVec3<f32>& eye, f32 sight,
                 const JGeometry::TVec3<f32>& target, f32 length, f32 angle,
                 f32 aware);

void SMS_GoRotate(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
                  f32, f32*);
void SMSCalcJumpVelocityY(const JGeometry::TVec3<f32>&,
                          const JGeometry::TVec3<f32>&, f32, f32, f32,
                          JGeometry::TVec3<f32>*);
void SMSCalcJumpVelocityXZ(const JGeometry::TVec3<f32>&,
                           const JGeometry::TVec3<f32>&, f32, f32,
                           JGeometry::TVec3<f32>*);

void MsVECNormalize(Vec*, Vec*);
f32 MsVECMag2(Vec*);

inline JGeometry::TVec3<f32>
MsPerpendicFootToLineR(const JGeometry::TVec3<f32>& param_1,
                       const JGeometry::TVec3<f32>& param_2,
                       const JGeometry::TVec3<f32>& param_3)
{
	// TODO: floats are the worst, doesn't match at all...
	JGeometry::TVec3<f32> diff;
	diff.sub(param_2, param_1);
	f32 fVar1 = (param_3.dot(diff) - param_1.dot(diff)) / diff.squared();
	if (fVar1 < 0.0f) {
		fVar1 = 0.0f;
	} else if (fVar1 > 1.0f)
		fVar1 = 1.0f;
	JGeometry::TVec3<f32> thing;
	thing.scale(fVar1, diff);
	thing.add(param_1);
	return thing;
}

f32 MsSqrtf(f32);

#endif
