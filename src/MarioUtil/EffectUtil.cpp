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

void SMS_EmitSinkInPollutionEffect(const JGeometry::TVec3<float>& arg0,
                                   const JGeometry::TVec3<float>& arg1,
                                   bool arg2)
{
	if ((gpMarDirector->mMoveTickCount % 20) != 0)
		return;

	using namespace JGeometry;

	const TVec3<f32> fwd(1.0f, 0.0f, 0.0f);
	TVec3<f32> B;
	B.cross(fwd, arg1);

	TPosition3f matrix;

	TVec3<f32> C;
	C.cross(arg1, B);
	C.normalize();
	B.normalize();

	matrix.setXYZDir(C, arg1, B);
	matrix.setTrans(arg0);

	if (arg2)
		gpMarioParticleManager->emitAndBindToMtx(PARTICLE_MS_MARI_RAKUBALL,
		                                         matrix.mMtx, 2U, nullptr);

	gpMarioParticleManager->emitAndBindToMtx(PARTICLE_MS_MARI_RAKUHAMON,
	                                         matrix.mMtx, 2U, nullptr);
}
