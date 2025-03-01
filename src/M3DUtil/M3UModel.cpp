#include <M3DUtil/M3UModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>

void M3UModelCommon::getMtxCalc(const M3UMtxCalcSetInfo&) { }

void M3UModel::changeMtxCalcAnmTransform(int param_1, u8 param_2) { }

void M3UModel::changeAnmTexPattern(int param_1, u8 param_2)
{
	u8* ptr = &unk1C[param_1 * 2];
	ptr[0]  = param_2;

	J3DFrameCtrl& ctrl = unkC[ptr[1]];
	ctrl.mEndFrame     = unk4->unk8[param_2]->getFrameMax();
	ctrl.mCurrentFrame = 0.0f;
}

void M3UModel::updateInMotion() { }

void M3UModel::updateInTexPatternAnm()
{
	if (unk1C)
		unkC[unk1C[1]].update();
}

void M3UModel::updateIn()
{
	updateInMotion();
	updateInTexPatternAnm();
}

void M3UModel::updateOut() { }

void M3UModel::entryIn() { }

void M3UModel::entryOut() { }

void M3UModel::perform(u32, JDrama::TGraphics*) { }
