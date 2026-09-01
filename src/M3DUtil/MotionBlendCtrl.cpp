#include <M3DUtil/MotionBlendCtrl.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <Camera/cameralib.hpp>

TMotionBlendCtrl::TMotionBlendCtrl(bool param_1, int param_2)
{
	unk0 = 1;
	if (param_2 <= 0)
		unk4 = 1.0f;
	else
		unk4 = 1.0f / CLBPalFrame(param_2);

	unk8 = new M3UMtxCalcSIAnmBlendQuat(param_1);
}

TMotionBlendCtrl::TMotionBlendCtrl(bool param_1)
{
	unk0 = 0;
	unk4 = 0.0f;
	unk8 = new M3UMtxCalcSIAnmBlendQuat(param_1);
}

void TMotionBlendCtrl::execSimpleMotionBlend()
{
	if (!(unk0 & 1))
		return;

	f32 fVar1;
	if (unk8->mMotionBlendRatio - unk4 <= 0.0f) {
		fVar1 = 0.0f;
	} else {
		fVar1 = unk8->mMotionBlendRatio - unk4;
		if (unk8->mOldAnm)
			unk8->mOldAnm->setFrame(unk8->unk60);
	}
	unk8->mMotionBlendRatio = fVar1;
}

f32 TMotionBlendCtrl::getMotionBlendRatio() const
{
	return unk8->mMotionBlendRatio;
}

void TMotionBlendCtrl::setMotionBlendRatio(f32 value)
{
	unk8->mMotionBlendRatio = value;
}

void TMotionBlendCtrl::keepCurAnm(J3DAnmTransform* param_1, f32 param_2)
{
	unk8->unk60   = param_2;
	unk8->mOldAnm = param_1;
}

void TMotionBlendCtrl::setNewAnm(J3DAnmTransform* param_1)
{
	unk8->mNewAnm = param_1;
	if (!(unk0 & 1))
		return;
	unk8->mMotionBlendRatio = 1.0f;
}

J3DAnmTransform* TMotionBlendCtrl::getOldMotionBlendAnmPtr() const
{
	return unk8->mOldAnm;
}

void TMotionBlendCtrl::setOldMotionBlendAnmPtr(J3DAnmTransform* param_1)
{
	unk8->mOldAnm = param_1;
}

f32 TMotionBlendCtrl::getOldMotionBlendFrame() const { return unk8->unk60; }
