#include <JSystem/JDrama/JDRCamera.hpp>
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

using namespace JDrama;

TCamera::~TCamera() { }
u32 TCamera::JSGGetFlag() const { return mFlag.get(); }
void TCamera::JSGSetFlag(u32 flag) { mFlag.set(flag); }
float TCamera::JSGGetProjectionNear() const { return mNear; }
void TCamera::JSGSetProjectionNear(float near) { mNear = near; }
float TCamera::JSGGetProjectionFar() const { return mFar; }
void TCamera::JSGSetProjectionFar(float far) { mFar = far; }

void TPolarCamera::load(JSUMemoryInputStream& stream)
{
	TPlacement::load(stream);
	unk3C = stream.readF32();
	unk38 = stream.readF32();
}
// The view matrix is a ZXZ Euler chain applied to a pure -Z translation:
//   view = T(0, 0, -unk44) * Rz(-unk40) * Rx(-unk3C) * Rz(unk38)
// which is what the three `concat` operand orders below spell out. Recovered
// from the products in the retail block: the constant (scalar-replaced) matrix
// is concat's `a` operand in the first block and its `b` operand in the other
// two, and each rotation's axis is fixed by which column of the constant
// matrix carries the 1.0 (`setEularZ`, `setEularX`, `setEularZ` -- not
// Z/Y/X as previously guessed).
//
// Three tells drove the reconstruction, all now settled:
//   * retail parks four values in callee-saved FPRs before the first `sinf`
//     -- f31 = 0.0f, f30 = `fmr f30, f31` (a *second* 0.0f), f29 = 1.0f,
//     f28 = -unk44. The two distinct zeros are `identity()`'s chained zero and
//     `setTrans`'s x/y argument, which the register allocator copies rather
//     than CSEs; `identity33()` merges them into one and loses 26 points. The
//     `-unk44` only ranks below the three constants (f28, not f31) when it is
//     a *named* local of this function's body, hence `dist`.
//   * `setEularZ`/`setEularX` store their zeros as one right-to-left chained
//     assignment after the `1.0f` on the lone axis, not per element: retail's
//     store order is (0,0) (0,1) (1,0) (1,1) (2,2) then (2,1) (1,2) (2,0)
//     (0,2), which is exactly `ref(0,2) = ref(2,0) = ref(1,2) = ref(2,1) =
//     0.0f`. That one header change is worth 73.4% -> 86.2%.
//   * `setEularX`'s sign convention was inverted relative to `setEularY`/
//     `setEularZ` in the header ((1,2) must be -s and (2,1) +s); block two's
//     products prove it.
//
// TODO: 99.69%. Every instruction, register and float-register now matches
// byte for byte (0 opcode / 0 inserted / 0 deleted markers); the whole residue
// is 104 bytes of *dead* frame -- retail's frame is 0x1a0 with 140 bytes of
// untouched low region between the outgoing-argument area (0x8..0x18) and the
// first matrix at 0xa4, ours is 0x138 with 36. The three named `TPosition3f`
// slots (148 bytes, the first one never materialised) and the eleven saved
// FPRs f21-f31 already match exactly, and `u8 trash[104]` declared after
// `local_A4` gives a byte-identical 100% -- so this is purely a missing
// declaration, not a codegen difference. Measured levers (each moves the
// matrix base 1:1, so the two are indistinguishable in the layout):
//   dead named local in this body        cost
//     TPosition3f / Mtx                  48 each (2 of them = 96, 8 short)
//     TVec3 12, TQuat4 16, f32 4 (the first 4 bytes are absorbed by the
//     named block's existing alignment pad)
//   dead local inside an inlined callee  sizeof, rounded to 8, per expansion
//     concat x3, setTrans x4, setEularZ x2, setEularX x1, identity x1
// 104 = 96 + 8 in every decomposition and nothing natural supplies the last
// 8 bytes, so no non-fabricated candidate has been found yet; two extra
// scalar-replaced `TPosition3f` (one rotation matrix per axis instead of
// reusing `tmp`) would give the 96. Rejected outright: `TPosition3f
// tmp(0, 0, -unk44)` and `tmp.translation(...)` (both 42.5% -- the matrix is
// forced into memory), `tmp.identity()` before each rotation instead of
// `setTrans(0, 0, 0)` (77.4% / 86.3%), `at()` returning `const f32&`
// (68.8%), a `translation()` level and a TU-local `static inline` level
// (both +0 frame), and giving each concat its own destination matrix.
void TPolarCamera::perform(u32 cue, TGraphics* graphics)
{
	if (!(cue & (CUE_CALC_VIEW | CUE_SET_PROJECTION)))
		return;

	MtxPtr projMtx = graphics->mProjMtx.mMtx;
	C_MTXPerspective(projMtx, mFovy, mAspect, mNear, mFar);
	graphics->mNearPlane = mNear;
	graphics->mFarPlane  = mFar;

	// TODO: 99.69%; all 272 instructions match and the only residue is frame
	// 0x138 against retail's 0x1a0. Library re-pass 2026-09-18 measured the
	// gap exactly: it is 104 bytes of dead space that has to sit *below*
	// `local_A4`, i.e. in the last-declared named slots, and its granularity
	// is 8. `volatile char trash[104]` declared right here is 100.0% with zero
	// markers; declared *before* `tmp` it also lands 0x1a0 but scrambles the
	// three matrices (52 operands), so the position is pinned. trash[96..100]
	// stops at 0x198 (the four pad bytes below the saved registers absorb the
	// remainder), trash[101] already reaches 0x1a0.
	// The natural half is two more `TPosition3f` (2 x 48 = 96, measured 0x198
	// with no instruction change); the last 8 bytes need one *non-trivial*
	// 8-byte class local, and `TPosition3f d1; TPosition3f d2;
	// JGeometry::TVec2<f32> d3;` is a literal 100.0% / 0 markers here.
	// It is not committed because nothing in a polar camera wants three dead
	// objects: the 96 is credible as scratch matrices the author declared and
	// never used, the `TVec2` is not.
	// Also measured: a dead `TVec3<f32>` is worth **0** in this caller (not the
	// 12 the rules card gives), and `JDrama::TRect` is not a zero-instruction
	// carrier at all -- its `JUTRect()` initialiser list emits 13 extra
	// instructions (90.4%). So the missing carrier must be an 8-byte class
	// whose constructor is empty, like `TVec2<f32>` or `TPosition3<T>` itself.
	// The pool is not the place to look: all three `concat` expansions spill
	// their three float temps to 0xc/0x10/0x14 in *both* builds, so retail does
	// not give each expansion its own 48-byte block.
	//
	// `dist` has to be named: as a bare `-unk44` argument the value outranks
	// the three literals above (f31 instead of f28) and every FPR in the
	// first block is renumbered.
	TPosition3f tmp;
	f32 dist = -unk44;
	tmp.identity();
	tmp.setTrans(0.0f, 0.0f, dist);

	TPosition3f local_D4;
	local_D4.setEularZ(DEG_TO_RAD(-unk40));
	local_D4.setTrans(0.0f, 0.0f, 0.0f);

	TPosition3f local_A4;
	local_A4.concat(tmp, local_D4);

	tmp.setEularX(DEG_TO_RAD(-unk3C));
	tmp.setTrans(0.0f, 0.0f, 0.0f);
	local_D4.concat(local_A4, tmp);

	tmp.setEularZ(DEG_TO_RAD(unk38));
	tmp.setTrans(0.0f, 0.0f, 0.0f);
	local_A4.concat(local_D4, tmp);

	MTXCopy(local_A4, graphics->mViewMtx);
	if (cue & CUE_SET_PROJECTION)
		GXSetProjection(projMtx, GX_PERSPECTIVE);
}
JStage::TECameraProjection TPolarCamera::JSGGetProjectionType() const
{
	return JStage::TECAMERAPROJECTION_Unk1;
}
void TPolarCamera::JSGSetProjectionType(JStage::TECameraProjection) { }
float TPolarCamera::JSGGetProjectionFovy() const { return mFovy; }
void TPolarCamera::JSGSetProjectionFovy(float fovy) { mFovy = fovy; }
float TPolarCamera::JSGGetProjectionAspect() const { return mAspect; }
void TPolarCamera::JSGSetProjectionAspect(float aspect) { mAspect = aspect; }

// Binding level over a raw member read, worth +8 of low region in
// JDrama::TLookAtCamera::perform (batch 127).
static inline f32 JDRCameraFar(const TLookAtCamera* p)
{
	f32 far = p->mFar;
	return far;
}

void TLookAtCamera::perform(u32 cue, TGraphics* graphics)
{
	if (!(cue & (CUE_CALC_VIEW | CUE_SET_PROJECTION)))
		return;

	MtxPtr projMtx = graphics->mProjMtx.mMtx;
	C_MTXPerspective(projMtx, mFovy, mAspect, mNear, mFar);
	graphics->mNearPlane = mNear;
	graphics->mFarPlane  = JDRCameraFar(this);
	C_MTXLookAt(graphics->mViewMtx, &mPosition, &mUp, &mTarget);

	if (cue & CUE_SET_PROJECTION)
		GXSetProjection(projMtx, GX_PERSPECTIVE);
}
JStage::TECameraProjection TLookAtCamera::JSGGetProjectionType() const
{
	return JStage::TECAMERAPROJECTION_Unk1;
}
void TLookAtCamera::JSGSetProjectionType(JStage::TECameraProjection) { }
float TLookAtCamera::JSGGetProjectionFovy() const { return mFovy; }
void TLookAtCamera::JSGSetProjectionFovy(float fovy) { mFovy = fovy; }
float TLookAtCamera::JSGGetProjectionAspect() const { return mAspect; }
void TLookAtCamera::JSGSetProjectionAspect(float aspect) { mAspect = aspect; }
void TLookAtCamera::JSGGetViewPosition(Vec* pos) const { *pos = mPosition; }
void TLookAtCamera::JSGSetViewPosition(const Vec& pos) { mPosition.set(pos); }
void TLookAtCamera::JSGGetViewUpVector(Vec* up) const { *up = mUp; }
void TLookAtCamera::JSGSetViewUpVector(const Vec& up) { mUp.set(up); }
void TLookAtCamera::JSGGetViewTargetPosition(Vec* tgt) const { *tgt = mTarget; }
void TLookAtCamera::JSGSetViewTargetPosition(const Vec& tgt)
{
	mTarget.set(tgt);
}

void TOrthoProj::load(JSUMemoryInputStream& stream)
{
	TPlacement::load(stream);
	stream >> mField[0] >> mField[1] >> mField[2] >> mField[3];
}
// Binding level over a raw member read, worth +8 of low region in
// JDrama::TOrthoProj::perform (batch 127).
static inline f32 JDRCameraFar(const TOrthoProj* p)
{
	f32 far = p->mFar;
	return far;
}

void TOrthoProj::perform(u32 cue, TGraphics* graphics)
{
	if (!(cue & (CUE_CALC_VIEW | CUE_SET_PROJECTION)))
		return;

	MtxPtr projMtx = graphics->mProjMtx.mMtx;
	C_MTXOrtho(projMtx, mField[1], mField[3], mField[0], mField[2], mNear,
	           mFar);
	graphics->mNearPlane = mNear;
	graphics->mFarPlane  = JDRCameraFar(this);
	MTXTrans(graphics->mViewMtx, mPosition.x, mPosition.y, mPosition.z);

	if (cue & CUE_SET_PROJECTION)
		GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
}
JStage::TECameraProjection TOrthoProj::JSGGetProjectionType() const
{
	return JStage::TECAMERAPROJECTION_Unk1;
}
void TOrthoProj::JSGSetProjectionType(JStage::TECameraProjection) { }
void TOrthoProj::JSGGetProjectionField(float* field) const
{
	for (int i = 0; i < 4; ++i)
		field[i] = mField[i];
}
void TOrthoProj::JSGSetProjectionField(const float* field)
{
	for (int i = 0; i < 4; ++i)
		mField[i] = field[i];
}
