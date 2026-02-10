#include <M3DUtil/M3UModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>

J3DMtxCalc* M3UModelCommon::getMtxCalc(const M3UMtxCalcSetInfo& param_1)
{
	// Same type as in J3DNewMtxCalcAnm
	// TODO: Extract to enum?
	switch (param_1.mAnmType) {
	case 0:
		return &unk10[param_1.mMtxCalcIdx];
	case 1:
		return &unk14[param_1.mMtxCalcIdx];
	}
	return nullptr;
}

void M3UModel::changeMtxCalcAnmTransform(int param_1, u8 param_2)
{
	M3UMtxCalcSetInfo& ptr = unk14[param_1];
	ptr.mAnmTransformIdx   = param_2;

	J3DFrameCtrl& ctrl = unkC[ptr.mFrameCalcIdx];
	ctrl.mEndFrame     = unk4->unk4[param_2]->getFrameMax();
	ctrl.mCurrentFrame = 0.0f;
}

void M3UModel::changeAnmTexPattern(int param_1, u8 param_2)
{
	u8* ptr = &unk1C[param_1 * 2];
	ptr[0]  = param_2;

	J3DFrameCtrl& ctrl = getFrameCtrl(ptr[1]);
	ctrl.mEndFrame     = unk4->unk8[param_2]->getFrameMax();
	ctrl.mCurrentFrame = 0.0f;
}

void M3UModel::updateInMotion()
{
	// Unused stack
	// volatile u32 padding[12];
	for (int i = 0; i < unk10; i++) {
		M3UMtxCalcSetInfo& info   = unk14[i];
		J3DAnmTransform* anmTrans = unk4->unk4[info.mAnmTransformIdx];
		J3DFrameCtrl& frameCtrl   = getFrameCtrl(info.mFrameCalcIdx);
		frameCtrl.update();

		J3DJoint* jnt = unk8->mModelData->getJointNodePointer(info.mJntIdx);
		if (info.mMtxCalcIdx == 0xff) {
			jnt->setMtxCalc(nullptr);
			continue;
		}
		f32 currentFrame = frameCtrl.getCurrentFrame();
		anmTrans->setFrame(currentFrame);

		// Possibly inlined? Feels like it fits more in M3UModelCommon
		switch (info.mAnmType) {
		case 0:
			unk4->unk10[info.mMtxCalcIdx].mOne[0] = anmTrans;
			break;
		case 1:
			unk4->unk14[info.mMtxCalcIdx].mOne[0] = anmTrans;
			break;
		}

		jnt->setMtxCalc(unk4->getMtxCalc(unk14[i]));
	}
}

void M3UModel::updateInTexPatternAnm()
{
	if (unk1C)
		getFrameCtrl(unk1C[1]).update();
}

void M3UModel::updateIn()
{
	updateInMotion();
	if (unk1C != nullptr) {
		getFrameCtrl(unk1C[1]).update();
	}
}

void M3UModel::updateOut()
{
	for (int i = 0; i < unk10; i++) {
		M3UMtxCalcSetInfo& unk = unk14[i];
		unk8->mModelData->getJointNodePointer(unk.mJntIdx)->setMtxCalc(nullptr);
	}
}

void M3UModel::entryIn()
{
	if (unk1C != nullptr) {
		u8* ids                 = unk1C;
		u8 patternId            = ids[0];
		u8 frameCtrlId          = ids[1];
		J3DFrameCtrl& frameCtrl = getFrameCtrl(frameCtrlId);
		if (patternId != 0xff) {
			J3DAnmTexPattern* pattern = unk4->unk8[patternId];
			pattern->setFrame(frameCtrl.getCurrentFrame());
			unk8->mModelData->setTexNoAnimator(pattern, unk4->unkC[ids[0]]);
		}
	}
}

void M3UModel::entryOut()
{
	if (unk1C != nullptr && *unk1C != 0xff) {
		unk8->mModelData->removeTexNoAnimator(unk4->unk8[unk1C[0]]);
	}
}

void M3UModel::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 2) {
		updateIn();
		unk8->calc();
		updateOut();
	}

	if (param_1 & 4) {
		unk8->viewCalc();
	}

	if (param_1 & 0x200) {
		entryIn();
		unk8->entry();
		entryOut();
	}
}
