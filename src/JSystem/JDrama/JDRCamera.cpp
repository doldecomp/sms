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
// Direct-return level over the far plane, the polar sibling of the binders
// the look-at and ortho cameras use; with the projection matrix read through
// its conversion and the four translations passed as `TVec3` temporaries it
// is exactly retail's 104 bytes of inline pool.
static inline f32 JDRCameraFar(const TPolarCamera* p) { return p->mFar; }

void TPolarCamera::perform(u32 cue, TGraphics* graphics)
{
	if (!(cue & (CUE_CALC_VIEW | CUE_SET_PROJECTION)))
		return;

	MtxPtr projMtx = graphics->mProjMtx;
	C_MTXPerspective(projMtx, mFovy, mAspect, mNear, mFar);
	graphics->mNearPlane = mNear;
	graphics->mFarPlane  = JDRCameraFar(this);

	// `dist` has to be named: as a bare `-unk44` argument the value outranks
	// the three literals above (f31 instead of f28) and every FPR in the
	// first block is renumbered.
	TPosition3f tmp;
	f32 dist = -unk44;
	tmp.identity();
	tmp.setTrans(JGeometry::TVec3<f32>(0.0f, 0.0f, dist));

	TPosition3f local_D4;
	local_D4.setEularZ(DEG_TO_RAD(-unk40));
	local_D4.setTrans(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));

	TPosition3f local_A4;
	local_A4.concat(tmp, local_D4);

	tmp.setEularX(DEG_TO_RAD(-unk3C));
	tmp.setTrans(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
	local_D4.concat(local_A4, tmp);

	tmp.setEularZ(DEG_TO_RAD(unk38));
	tmp.setTrans(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
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
