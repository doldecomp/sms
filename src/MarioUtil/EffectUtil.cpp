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

// TODO: 98.8%, two independent residues, no instruction-order difference.
//  1. Frame 0xd8 vs 0xc8. The split is not uniform: retail's matrix sits at
//     0x48 with 28 bytes of named locals above it (fwd + B) and 44 below
//     (C, the inline temporaries and the 20-byte argument area); ours has 32
//     above and 24 below, so the low region is 20 bytes (five temporaries)
//     short and the padding above the matrix is 4 too many. `volatile char
//     trash[16]` reaches 0xd8 with every instruction intact but leaves the
//     matrix at 0x44, so the missing space is genuinely low, not named.
//     Measured no-ops: every declaration order of fwd/B/C/matrix (matrix
//     first, all four up front, C before matrix, C first), an extra inline
//     level wrapping both cross() calls, and setLength(TUtil<f32>::one())
//     in place of normalize().
//  2. Float registers: each cross result has its x and z components in
//     swapped callee-saved registers (B.x/B.z are f31/f29 in retail, f29/f31
//     here; C.x/C.z f27/f25 vs f25/f27), with B.y/C.y and the whole
//     instruction sequence identical. Declaration order does not touch it,
//     and the three alternative cross() bodies are ruled out tree-wide in
//     JGVec3.hpp (this function is one of the sites they regress).
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

	TPosition3f matrix;

	TVec3<f32> C;
	C.cross(arg1, B);
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
		gpMarioParticleManager->emitAndBindToMtx(0x1D8, matrix.mMtx, 2U,
		                                         nullptr);
	}
	gpMarioParticleManager->emitAndBindToMtx(0x1D9, matrix.mMtx, 2U, nullptr);
}

// Declared locally rather than by including <Player/MarioAccess.hpp>, which
// would perturb this TU's string pool; MarioAccess.hpp has the real
// declaration and the map lists that function UNUSED as well.
void SMS_GetMarioJumpIntoWaterModelData();

// UNUSED (map size 0x20): the last link of the JumpIntoWater model-data
// chain (TMarioEffect 0x10 -> TMario 0x24 -> SMS_GetMario... 0x24 -> this).
// Eight instructions is exactly a call-forwarding frame.
void SMS_GetJumpIntoWaterModelData() { SMS_GetMarioJumpIntoWaterModelData(); }
