#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/JGeometry.hpp>

bool SMS_EmitRippleTiny(JGeometry::TVec3<float>* arg0)
{
	bool ret = 1;
	if (!gpMarioParticleManager->emit(PARTICLE_MS_M_WATRUN_B, arg0, 0U,
	                                  nullptr))
		ret = 0;

	if (!gpMarioParticleManager->emit(PARTICLE_MS_M_WATRUN_C, arg0, 0U,
	                                  nullptr))
		ret = 0;

	return ret;
}

bool SMS_EmitRipplePool(f32 (*arg0)[4], void* arg1)
{
	bool ret = 1;
	if (!gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_MZBHAMON_B,
	                                                 arg0, 3U, arg1))
		ret = 0;

	if (!gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_MZBHAMON_A,
	                                                 arg0, 1U, arg1))
		ret = 0;

	if (!gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_MZBHAMON_C,
	                                                 arg0, 1U, arg1))
		ret = 0;

	return ret;
}

bool SMS_EmitRippleSea(MtxPtr arg0, void* arg1)
{
	bool ret = 1;
	if (!gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_HAMON_B,
	                                                 arg0, 3U, arg1))
		ret = 0;

	if (!gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_HAMON_A,
	                                                 arg0, 1U, arg1))
		ret = 0;

	if (!gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_HAMON_C,
	                                                 arg0, 1U, arg1))
		ret = 0;

	return ret;
}

static inline TMarioParticleManager* EffectUtilGetParticleManager()
{
	TMarioParticleManager* manager = gpMarioParticleManager;
	return manager;
}

static inline void EffectUtilCross(JGeometry::TVec3<f32>& dst,
                                   const JGeometry::TVec3<f32>& a,
                                   const JGeometry::TVec3<f32>& b)
{
	f32 z;
	f32 y;
	f32 x;

	x = a.y * b.z - a.z * b.y;
	y = a.z * b.x - a.x * b.z;
	z = a.x * b.y - a.y * b.x;

	dst.x = x;
	dst.y = y;
	dst.z = z;
}

void SMS_EmitSinkInPollutionEffect(const JGeometry::TVec3<float>& arg0,
                                   const JGeometry::TVec3<float>& arg1,
                                   bool arg2)
{
	if ((SMSGetMarDirector()->unk58 % 20) != 0)
		return;

	using namespace JGeometry;

	const TVec3<f32> fwd(1.f, 0.0f, 0.0f);

	TVec3<f32> B;
	EffectUtilCross(B, fwd, arg1);

	TPosition3f matrix;

	TVec3<f32> C;
	EffectUtilCross(C, arg1, B);
	C.normalize();
	B.normalize();

	matrix.mMtx[0][0] = C.x;
	matrix.mMtx[1][0] = C.y;
	matrix.mMtx[2][0] = C.z;
	matrix.mMtx[0][1] = arg1.x;
	matrix.mMtx[1][1] = arg1.y;
	matrix.mMtx[2][1] = arg1.z;
	matrix.mMtx[0][2] = B.x;
	matrix.mMtx[1][2] = B.y;
	matrix.mMtx[2][2] = B.z;
	matrix.mMtx[0][3] = arg0.x;
	matrix.mMtx[1][3] = arg0.y;
	matrix.mMtx[2][3] = arg0.z;

	if (arg2) {
		EffectUtilGetParticleManager()->emitAndBindToMtx(0x1D8, matrix.mMtx,
		                                                 2U, nullptr);
	}
	EffectUtilGetParticleManager()->emitAndBindToMtx(0x1D9, matrix.mMtx, 2U,
	                                                 nullptr);
}

// Declared locally rather than by including <Player/MarioAccess.hpp>, which
// would perturb this TU's string pool; MarioAccess.hpp has the real
// declaration and the map lists that function UNUSED as well.
void SMS_GetMarioJumpIntoWaterModelData();

// UNUSED (map size 0x20): the last link of the JumpIntoWater model-data
// chain (TMarioEffect 0x10 -> TMario 0x24 -> SMS_GetMario... 0x24 -> this).
// Eight instructions is exactly a call-forwarding frame.
void SMS_GetJumpIntoWaterModelData() { SMS_GetMarioJumpIntoWaterModelData(); }
