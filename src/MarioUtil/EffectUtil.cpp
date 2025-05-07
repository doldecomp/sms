#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <JSystem/JGeometry.hpp>

bool SMS_EmitRippleTiny(JGeometry::TVec3<float>* arg0)
{
	bool ret = 1;
	if (!gpMarioParticleManager->emit(0x35, arg0, 0U, nullptr))
		ret = 0;

	if (!gpMarioParticleManager->emit(0x36, arg0, 0U, nullptr))
		ret = 0;

	return ret;
}

bool SMS_EmitRipplePool(f32 (*arg0)[4], void* arg1)
{
	bool ret = 1;
	if (!gpMarioParticleManager->emitAndBindToMtxPtr(0x1E9, arg0, 3U, arg1))
		ret = 0;

	if (!gpMarioParticleManager->emitAndBindToMtxPtr(0x10A, arg0, 1U, arg1))
		ret = 0;

	if (!gpMarioParticleManager->emitAndBindToMtxPtr(0x10B, arg0, 1U, arg1))
		ret = 0;

	return ret;
}

bool SMS_EmitRippleSea(MtxPtr arg0, void* arg1)
{
	bool ret = 1;
	if (!gpMarioParticleManager->emitAndBindToMtxPtr(0x1E8, arg0, 3U, arg1))
		ret = 0;

	if (!gpMarioParticleManager->emitAndBindToMtxPtr(0x107, arg0, 1U, arg1))
		ret = 0;

	if (!gpMarioParticleManager->emitAndBindToMtxPtr(0x108, arg0, 1U, arg1))
		ret = 0;

	return ret;
}

void SMS_EmitSinkInPollutionEffect(const JGeometry::TVec3<float>& arg0,
                                   const JGeometry::TVec3<float>& arg1,
                                   bool arg2)
{
	if ((gpMarDirector->unk58 % 20) != 0)
		return;

	using namespace JGeometry;

	const TVec3<f32> fwd(1.f, 0.0f, 0.0f);

	TVec3<f32> B;
	B.cross(fwd, arg1);

	TMtx34f matrix;

	TVec3<f32> C;
	C.cross(arg1, B);
	TVec3<f32> Cnorm;
	Cnorm.setLength(C, 1);
	TVec3<f32> Bnorm;
	Bnorm.setLength(B, 1);

	matrix.mMtx[0][0] = Cnorm.x;
	matrix.mMtx[1][0] = Cnorm.y;
	matrix.mMtx[2][0] = Cnorm.z;
	matrix.mMtx[0][1] = arg1.x;
	matrix.mMtx[1][1] = arg1.y;
	matrix.mMtx[2][1] = arg1.z;
	matrix.mMtx[0][2] = Bnorm.x;
	matrix.mMtx[1][2] = Bnorm.y;
	matrix.mMtx[2][2] = Bnorm.z;
	matrix.mMtx[0][3] = arg0.x;
	matrix.mMtx[1][3] = arg0.y;
	matrix.mMtx[2][3] = arg0.z;

	if (arg2) {
		gpMarioParticleManager->emitAndBindToMtx(0x1D8, matrix.mMtx, 2U,
		                                         nullptr);
	}
	gpMarioParticleManager->emitAndBindToMtx(0x1D9, matrix.mMtx, 2U, nullptr);
}
