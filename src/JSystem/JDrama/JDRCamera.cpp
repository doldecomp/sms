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
void TPolarCamera::perform(u32 param_1, TGraphics* param_2)
{
	if (!(param_1 & 0x14))
		return;

	MtxPtr projMtx = param_2->mProjMtx.mMtx;
	C_MTXPerspective(projMtx, mFovy, mAspect, mNear, mFar);
	param_2->mNearPlane = mNear;
	param_2->mFarPlane  = mFar;

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

	MTXCopy(local_A4, param_2->mViewMtx);
	if (param_1 & 0x10)
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

void TLookAtCamera::perform(u32 param_1, TGraphics* param_2)
{
	if (!(param_1 & 0x14))
		return;

	MtxPtr projMtx = param_2->mProjMtx.mMtx;
	C_MTXPerspective(projMtx, mFovy, mAspect, mNear, mFar);
	param_2->mNearPlane = mNear;
	param_2->mFarPlane  = mFar;
	C_MTXLookAt(param_2->mViewMtx, &mPosition, &mUp, &mTarget);

	if (param_1 & 0x10)
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
	stream.read(&mField[0], sizeof(float));
	stream.read(&mField[1], sizeof(float));
	stream.read(&mField[2], sizeof(float));
	stream.read(&mField[3], sizeof(float));
}
void TOrthoProj::perform(u32 param_1, TGraphics* param_2)
{
	if (!(param_1 & 0x14))
		return;

	MtxPtr projMtx = param_2->mProjMtx.mMtx;
	C_MTXOrtho(projMtx, mField[1], mField[3], mField[0], mField[2], mNear,
	           mFar);
	param_2->mNearPlane = mNear;
	param_2->mFarPlane  = mFar;
	MTXTrans(param_2->mViewMtx, mPosition.x, mPosition.y, mPosition.z);

	if (param_1 & 0x10)
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
