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
// Parked binding level (batch 110's shape), TU-prefixed so a header batch can
// give it its real name later. Measured in SMS_EmitSinkInPollutionEffect: it is
// +8 of low region per expansion with no instruction change, and its two
// expansions there are 16 of the 20 low bytes that function was short.
static inline TMarioParticleManager* EffectUtilGetParticleManager()
{
	TMarioParticleManager* manager = gpMarioParticleManager;
	return manager;
}

// TODO: 99.1%. Frame and every stack displacement are exact as of batch 115;
// the whole residue is a three-way float-register rotation inside the two
// cross products. Retail ranks each cross result descending by component
// (B.x f31, B.y f30, B.z f29; C.x f27, C.y f26, C.z f25) and we rank them
// ascending (B.x f29 ... B.z f31, C.x f25 ... C.z f27), with the 0.0f constant
// correctly in f28 between the two groups; the instruction stream is otherwise
// identical.
// How the frame was landed (it was 20 low bytes short and 4 named bytes long):
// `SMSGetMarDirector()` over the raw `gpMarDirector` read is +4 of low region
// (it is +0 on its own -- levers interact, so measure it last), and the parked
// EffectUtilGetParticleManager() binding level above is +8 per emit site.
// Batch 131 located the rule behind the rotation: the callee-saved FPR a
// scalar-replaced vector component gets is ranked by the *declaration order of
// cross()'s three temporaries*, with f31 going to the last-declared one. Our
// `_x, _y, _z` therefore gives f31 to z (ascending) and retail's descending
// ranking means retail declares `_z, _y, _x`. Measured with a TU-local clone of
// cross() (frame-neutral at both sites): `_z, _y, _x` with the stores left in
// x/y/z order reproduces retail's ranking in *both* groups exactly (B.x f31,
// B.y f30, B.z f29; C.x f27, C.y f26, C.z f25) and drops the marker count
// 17 -> 13, leaving only the first cross's schedule (our `_x` fmsubs sinks past
// the `_y` fnmsubs, +1 instruction) and the volatile f1/f2/f4/f5 permutation
// that follows from it. All 12 (declaration order x {6 store orders}) and the
// other four declaration orders were measured: only `_z, _y, _x` ranks both
// groups right, reversing the *stores* alone fixes C.x only, and assigning the
// three expressions straight into `d->` costs 2-3 instructions and 8 of frame.
// This is a JGVec3.hpp item for a header round -- the TODO at cross() records
// store-order and temporary-count variants but never the declaration order
// with the stores held fixed, so it has not been measured tree-wide.
// Rejected: spelling the first cross out with its three temporaries and
// storing z, y, x (98.2, and it moves the 0.0f constant into f31); declaring
// `C` before the matrix (moves the matrix to 0x3c); normalizing B before C
// (89.5, +5 instructions); an unnamed `TVec3<f32>(1, 0, 0)` argument in place
// of the named `fwd` (frame-neutral, -12 on the matrix, registers unchanged).
void SMS_EmitSinkInPollutionEffect(const JGeometry::TVec3<float>& arg0,
                                   const JGeometry::TVec3<float>& arg1,
                                   bool arg2)
{
	if ((SMSGetMarDirector()->unk58 % 20) != 0)
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
