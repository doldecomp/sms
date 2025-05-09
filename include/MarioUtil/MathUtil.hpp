#ifndef MARIO_UTIL_MATH_UTIL_HPP
#define MARIO_UTIL_MATH_UTIL_HPP

#include <JSystem/JGeometry.hpp>

int IConverge(int, int, int, int);
f32 FConverge(f32, f32, f32, f32);
void GetAtanTable(f32, f32);
s16 matan(f32, f32);

JGeometry::TVec3<f32> MsGetRotFromZaxis(const JGeometry::TVec3<f32>&);
void MsMtxSetRotRPH(MtxPtr, f32, f32, f32);
void MsMtxSetXYZRPH(MtxPtr, f32, f32, f32, s16, s16, s16);
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

void MsIsInSight(const JGeometry::TVec3<f32>&, f32,
                 const JGeometry::TVec3<f32>&, f32, f32, f32);

void SMS_GoRotate(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
                  f32, f32*);
void SMSCalcJumpVelocityY(const JGeometry::TVec3<f32>&,
                          const JGeometry::TVec3<f32>&, f32, f32, f32,
                          JGeometry::TVec3<f32>*);
void SMSCalcJumpVelocityXZ(const JGeometry::TVec3<f32>&,
                           const JGeometry::TVec3<f32>&, f32, f32,
                           JGeometry::TVec3<f32>*);

void MsVECNormalize(Vec*, Vec*);
void MsVECMag2(Vec*);

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

#endif
