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
// TODO: 66.3%. The prologue, the C_MTXPerspective call and the near/far stores
// are exact; the whole residue is the view-matrix block. Two tells:
//   * retail parks three literals in callee-saved FPRs *before* the first
//     `sinf` -- `lfs f31, @877` (0.0f), `fmr f30, f31`, `lfs f29, @878` (1.0f)
//     -- and reuses them for every `setTrans`/identity store after the calls,
//     where we reload `@468`/`@469` after each call. The `fmr f30, f31` means
//     two *distinct* live values that both hold 0.0f, i.e. two separate
//     argument temporaries, so retail's block has more `setTrans`-like
//     expansions than ours, not fewer.
//   * our frame is 0x168 against retail's 0x1a0 (56 bytes short) and we save
//     one extra callee-saved FPR (f20-f31 against f21-f31).
// The read order (`0x40` then `0x44`, i.e. -unk40 then -unk44) already matches,
// so the Euler order is right (cf. TSmJ3DAct::perform, where it was reversed).
// Ruled out: giving each concat its own destination matrix (four or five
// `TPosition3f` locals instead of reusing two) overshoots to 0x1f0 and scores
// 60.0%, so the missing 56 bytes are not another matrix local.
// TODO: 66.3% (304 instructions, 11 opcode / 36 deleted / 32 inserted
// operands, frame 0x168 against retail's 0x1a0). The pool of this TU is
// `@877` = 0.0f, `@878` = 1.0f, `@879` = DEG_TO_RAD, and retail keeps three of
// them live in callee-saved FPRs for the whole body -- f31 = 0.0f,
// f30 = `fmr f30, f31` (a *second* 0.0f), f29 = 1.0f, f28 = -unk44 -- while we
// reload each literal at every use, which is where all 32 inserts come from.
// By batch 83's rule a callee-saved FPR only ever holds a named scalar local of
// the function's own body, so retail named at least two zeros, a one and the
// negated distance here and passed them into the `TPosition3f` calls instead of
// writing literals. That is a rewrite of the matrix setup, not a frame lever,
// so none of the batch 142-146 rules touches it; it wants its own batch.
void TPolarCamera::perform(u32 cue, TGraphics* graphics)
{
	if (!(cue & (CUE_CALC_VIEW | CUE_SET_PROJECTION)))
		return;

	MtxPtr projMtx = graphics->mProjMtx.mMtx;
	C_MTXPerspective(projMtx, mFovy, mAspect, mNear, mFar);
	graphics->mNearPlane = mNear;
	graphics->mFarPlane  = mFar;

	TPosition3f tmp;
	tmp.identity33();
	tmp.setTrans(0.0f, 0.0f, -unk44);

	TPosition3f local_D4;
	local_D4.setEularZ(DEG_TO_RAD(-unk40));
	local_D4.setTrans(0.0f, 0.0f, 0.0f);

	TPosition3f local_A4;
	local_A4.concat(local_D4, tmp);

	tmp.setEularY(DEG_TO_RAD(-unk3C));
	tmp.setTrans(0.0f, 0.0f, 0.0f);
	local_D4.concat(tmp, local_A4);

	tmp.setEularX(DEG_TO_RAD(unk38));
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
