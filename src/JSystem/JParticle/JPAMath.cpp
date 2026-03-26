#include <JSystem/JParticle/JPAMath.hpp>
#include <JSystem/JMath.hpp>
#include <math.h>

f32 JPASqrtf(f32 x)
{
	if (x > 0.0f) {
		return x * (f32)__frsqrte(x);
	}
	return 0.0f;
}

// TODO: all these vec math funcs are equivalent (I think), but matching them
// will be quite the ordeal.

void JPAGetXYZRotateMtx(s16 x, s16 y, s16 z, MtxPtr dst)
{
	f32* ptr = &dst[0][0];

	*ptr++ = JMASCos(z) * JMASCos(y);
	*ptr++ = -JMASSin(z) * JMASCos(x) + JMASCos(z) * JMASSin(y) * JMASSin(x);
	*ptr++ = JMASSin(z) * JMASSin(x) + JMASCos(z) * JMASSin(y) * JMASCos(x);
	*ptr++ = 0.0f;

	*ptr++ = JMASSin(z) * JMASCos(y);
	*ptr++ = JMASCos(z) * JMASCos(x) + JMASSin(z) * JMASSin(y) * JMASSin(x);
	*ptr++ = -JMASCos(z) * JMASSin(x) + JMASSin(z) * JMASSin(y) * JMASCos(x);
	*ptr++ = 0.0f;

	*ptr++ = -JMASSin(y);
	*ptr++ = JMASCos(y) * JMASSin(x);
	*ptr++ = JMASCos(y) * JMASCos(x);
	*ptr++ = 0.0f;
}

void JPAGetXYRotateMtx(s16 x, s16 y, MtxPtr dst)
{
	f32* ptr = &dst[0][0];

	*ptr++ = JMASCos(y);
	*ptr++ = JMASSin(y) * JMASSin(x);
	*ptr++ = JMASSin(y) * JMASCos(x);
	*ptr++ = 0.0f;

	*ptr++ = 0.0f;
	*ptr++ = JMASCos(x);
	*ptr++ = -JMASSin(x);
	*ptr++ = 0.0f;

	*ptr++ = -JMASSin(y);
	*ptr++ = JMASCos(y) * JMASSin(x);
	*ptr++ = JMASCos(y) * JMASCos(x);
	*ptr++ = 0.0f;
}

void JPAGetScaleXYRotateMtx(JGeometry::TVec3<f32>&, s16, s16, MtxPtr) { }

void JPAGetYZRotateMtx(s16 y, s16 z, MtxPtr dst)
{
	f32* ptr = &dst[0][0];

	*ptr++ = JMASCos(z) * JMASCos(y);
	*ptr++ = -JMASSin(z);
	*ptr++ = JMASCos(z) * JMASSin(y);
	*ptr++ = 0.0f;

	*ptr++ = JMASSin(z) * JMASCos(y);
	*ptr++ = JMASCos(z);
	*ptr++ = JMASSin(z) * JMASSin(y);
	*ptr++ = 0.0f;

	*ptr++ = -JMASSin(y);
	*ptr++ = 0.0f;
	*ptr++ = JMASCos(y);
	*ptr++ = 0.0f;
}

void JPAGetScaleYZRotateMtx(JGeometry::TVec3<f32>&, s16, s16, MtxPtr) { }

void JPAGetYRotateMtx(s16 y, MtxPtr dst)
{
	f32* ptr = &dst[0][0];

	*ptr++ = JMASCos(y);
	*ptr++ = 0.0f;
	*ptr++ = JMASSin(y);
	*ptr++ = 0.0f;

	*ptr++ = 0.0f;
	*ptr++ = 1.0f;
	*ptr++ = 0.0f;
	*ptr++ = 0.0f;

	*ptr++ = -JMASSin(y);
	*ptr++ = 0.0f;
	*ptr++ = JMASCos(y);
	*ptr++ = 0.0f;
}

void JPAGetZRotateMtx(s16 z, MtxPtr dst)
{
	f32* ptr = &dst[0][0];

	*ptr++ = JMASCos(z);
	*ptr++ = JMASSin(z);
	*ptr++ = 0.0f;
	*ptr++ = 0.0f;

	*ptr++ = -JMASSin(z);
	*ptr++ = JMASCos(z);
	*ptr++ = 0.0f;
	*ptr++ = 0.0f;

	*ptr++ = 0.0f;
	*ptr++ = 0.0f;
	*ptr++ = 1.0f;
	*ptr++ = 0.0f;
}

void JPAVecToRotaMtx(MtxPtr dst, JGeometry::TVec3<f32> a,
                     JGeometry::TVec3<f32> b)
{
	f32 p = a.y * b.z - a.z * b.y;
	f32 q = a.z * b.x - a.x * b.z;
	f32 r = a.x * b.y - a.y * b.x;

	f32 crossLen = r * r + p * p + q * q;
	f32 dot      = a.x * b.x + a.y * b.y + a.z * b.z;
	crossLen     = JPASqrtf(crossLen);

	if (crossLen > 3.814697e-06) {
		f32 inv = 1.0f / crossLen;
		p *= inv;
		q *= inv;
		r *= inv;
	} else {
		p = 0.0f;
		q = 0.0f;
		r = 0.0f;
	}

	f32 oneMinusDot = 1.0f - dot;

	dst[0][0] = dot * (1.0f - p * p) + p * p;
	dst[0][1] = p * q * oneMinusDot + r * crossLen;
	dst[0][2] = r * p * oneMinusDot - q * crossLen;
	dst[0][3] = 0.0f;

	dst[1][0] = p * q * oneMinusDot - r * crossLen;
	dst[1][1] = dot * (1.0f - q * q) + q * q;
	dst[1][2] = q * r * oneMinusDot + p * crossLen;
	dst[1][3] = 0.0f;

	dst[2][0] = r * p * oneMinusDot + q * crossLen;
	dst[2][1] = q * r * oneMinusDot - p * crossLen;
	dst[2][2] = dot * (1.0f - r * r) + r * r;
	dst[2][3] = 0.0f;
}

f32 JPAConvertFixToFloat(s16 param_1)
{
	if (param_1 == 0x7fff)
		return 1.0f;

	f32 f0 = (float)param_1 * (1.0f / 32768.0f) * 100000.0f;
	int r5 = (int)f0;
	int r0 = (int)f0 % 10;
	if (r0 >= 5) {
		r5 += (10 - r0);
	} else {
		r5 -= r0;
	}
	return (float)r5 * 1e-05f;
}

s16 JPAConvertFloatToFix(f32) { }

void JPAConvertFixVecToFloatVec(JGeometry::TVec3<f32>& param_1,
                                const JGeometry::TVec3<s16>& param_2)
{
	param_1.x = JPAConvertFixToFloat(param_2.x);
	param_1.y = JPAConvertFixToFloat(param_2.y);
	param_1.z = JPAConvertFixToFloat(param_2.z);
}

void JPAConvertFloatVecToFixVec(JGeometry::TVec3<s16>&,
                                const JGeometry::TVec3<f32>&)
{
}

void JPABound(JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
              const JGeometry::TVec3<f32>&, f32, f32)
{
}

void JPAGetSVecElement(MtxPtr, JGeometry::TVec3<f32>&) { }

void JPAGetRMtxSTVecElement(MtxPtr param_1, MtxPtr param_2,
                            JGeometry::TVec3<f32>& param_3,
                            JGeometry::TVec3<f32>& param_4)
{
	param_3.x = std::sqrtf(param_1[0][0] * param_1[0][0]
	                       + param_1[1][0] * param_1[1][0]
	                       + param_1[2][0] * param_1[2][0]);
	param_3.y = std::sqrtf(param_1[0][1] * param_1[0][1]
	                       + param_1[1][1] * param_1[1][1]
	                       + param_1[2][1] * param_1[2][1]);
	param_3.z = std::sqrtf(param_1[0][2] * param_1[0][2]
	                       + param_1[1][2] * param_1[1][2]
	                       + param_1[2][2] * param_1[2][2]);

	MTXIdentity(param_2);
	if (param_3.x != 0.0f) {
		param_2[0][0] = param_1[0][0] / param_3.x;
		param_2[1][0] = param_1[1][0] / param_3.x;
		param_2[2][0] = param_1[2][0] / param_3.x;
	}
	if (param_3.y != 0.0f) {
		param_2[0][1] = param_1[0][1] / param_3.y;
		param_2[1][1] = param_1[1][1] / param_3.y;
		param_2[2][1] = param_1[2][1] / param_3.y;
	}
	if (param_3.z != 0.0f) {
		param_2[0][2] = param_1[0][2] / param_3.z;
		param_2[1][2] = param_1[1][2] / param_3.z;
		param_2[2][2] = param_1[2][2] / param_3.z;
	}

	param_4.set(param_1[0][3], param_1[1][3], param_1[2][3]);
}

void JPAGetRMtxTVecElement(MtxPtr param_1, MtxPtr param_2,
                           JGeometry::TVec3<f32>& param_3)
{
	f32 l1 = std::sqrtf(param_1[0][0] * param_1[0][0]
	                    + param_1[1][0] * param_1[1][0]
	                    + param_1[2][0] * param_1[2][0]);
	f32 l2 = std::sqrtf(param_1[0][1] * param_1[0][1]
	                    + param_1[1][1] * param_1[1][1]
	                    + param_1[2][1] * param_1[2][1]);
	f32 l3 = std::sqrtf(param_1[0][2] * param_1[0][2]
	                    + param_1[1][2] * param_1[1][2]
	                    + param_1[2][2] * param_1[2][2]);

	MTXIdentity(param_2);
	if (l1 != 0.0f) {
		param_2[0][0] = param_1[0][0] / l1;
		param_2[1][0] = param_1[1][0] / l1;
		param_2[2][0] = param_1[2][0] / l1;
	}
	if (l2 != 0.0f) {
		param_2[0][1] = param_1[0][1] / l2;
		param_2[1][1] = param_1[1][1] / l2;
		param_2[2][1] = param_1[2][1] / l2;
	}
	if (l3 != 0.0f) {
		param_2[0][2] = param_1[0][2] / l3;
		param_2[1][2] = param_1[1][2] / l3;
		param_2[2][2] = param_1[2][2] / l3;
	}

	param_3.set(param_1[0][3], param_1[1][3], param_1[2][3]);
}

void JPAGetRMtxElement(MtxPtr, MtxPtr) { }

void JPAGetRTMtxElement(MtxPtr, MtxPtr) { }

void JPARadVecToSVec(JGeometry::TVec3<f32>&, JGeometry::TVec3<s16>&) { }

f32 JPAGetKeyFrameValue(f32 time, u16 frame_num, f32* frames)
{
	return JPAGetKeyFrameInterpolation<f32>(time, frame_num, frames);
}
