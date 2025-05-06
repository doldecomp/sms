#include <MarioUtil/MathUtil.hpp>

#define qr0 0

int IConverge(int param_1, int param_2, int param_3, int param_4)
{
	int result = param_1;
	if (result < param_2) {
		result += param_3;
		if (result > param_2)
			result = param_2;
	} else {
		result -= param_4;
		if (result < param_2)
			result = param_2;
	}
	return result;
}

f32 FConverge(f32 param_1, f32 param_2, f32 param_3, f32 param_4)
{
	f32 result = param_1;
	if (result < param_2) {
		result += param_3;
		if (result > param_2)
			result = param_2;
	} else {
		result -= param_4;
		if (result < param_2)
			result = param_2;
	}
	return result;
}

void GetAtanTable(f32, f32) { }

#pragma dont_inline on
s16 matan(f32, f32) { }
#pragma dont_inline off

JGeometry::TVec3<f32> MsGetRotFromZaxis(const JGeometry::TVec3<f32>&) { }
void MsMtxSetRotRPH(MtxPtr, f32, f32, f32) { }
void MsMtxSetXYZRPH(MtxPtr, f32, f32, f32, short, short, short) { }
void MsMtxSetTRS(MtxPtr, f32, f32, f32, f32, f32, f32, f32, f32, f32) { }

void MsIsInSight(const JGeometry::TVec3<f32>&, f32,
                 const JGeometry::TVec3<f32>&, f32, f32, f32)
{
}

void SMS_GoRotate(const JGeometry::TVec3<f32>& param_1,
                  const JGeometry::TVec3<f32>& param_2, f32 param_3,
                  f32* param_4)
{
	float diffx = param_2.x - param_1.x;
	float diffz = param_2.z - param_1.z;
	s16 angle   = matan(diffz, diffx);
	s16 sVar3   = param_3 * 182.0444f;
	int iVar1 = IConverge((s16)(angle - (int)(*param_4 * 182.0444f)), 0, sVar3,
	                      sVar3);
	*param_4  = (float)(s16)(angle - iVar1) * 0.005493164f;
}

void SMSCalcJumpVelocityY(const JGeometry::TVec3<f32>&,
                          const JGeometry::TVec3<f32>&, f32, f32, f32,
                          JGeometry::TVec3<f32>*)
{
}

void SMSCalcJumpVelocityXZ(const JGeometry::TVec3<f32>& param_1,
                           const JGeometry::TVec3<f32>& param_2, f32 param_3,
                           f32 param_4, JGeometry::TVec3<f32>* param_5)
{
}

asm void MsVECMag2(register Vec* v)
{
#ifdef __MWERKS__ // clang-format off
  psq_l   f3, Vec.x(v), 0, qr0
  ps_mul  f3, f3, f3

  lfs     f4, Vec.z(v)
  ps_madd f5, f4, f4, f3

  ps_sum0 f2, f5, f3, f3
  frsqrte f0, f2
  fneg    f1, f2
  fsel    f0, f1, f2, f0
  fmuls   f1, f2, f0
#endif // clang-format on
}

asm void MsVECNormalize(register Vec* v1, register Vec* v2)
{
#ifdef __MWERKS__ // clang-format off
  psq_l   f6, Vec.x(v1), 0, qr0
  ps_mul  f3, f6, f6

  lfs     f4, Vec.z(v1)
  ps_madd f5, f4, f4, f3

  ps_sum0 f2, f5, f3, f3

  frsqrte  f0, f2
  ps_muls0 f6, f6, f0

  psq_st f6, Vec.x(v2), 0, qr0
  fmuls  f4, f4, f0
  stfs   f4, Vec.z(v2)
#endif // clang-format on
}
