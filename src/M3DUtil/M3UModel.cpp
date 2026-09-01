#include <M3DUtil/M3UModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

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
	ctrl.setEnd(unk4->unk4[param_2]->getFrameMax());
	ctrl.setFrame(0.0f);
}

void M3UModel::changeAnmTexPattern(int param_1, u8 param_2)
{
	Unk1CStruct& tmp = unk1C[param_1];
	tmp.unk0         = param_2;

	J3DFrameCtrl& ctrl = getFrameCtrl(tmp.unk1);
	ctrl.setEnd(unk4->unk8[param_2]->getFrameMax());
	ctrl.setFrame(0.0f);
}

// TODO: 99.9%. Every instruction matches; the frame is 0x28 short of 0x80.
//
// frameCtrl must be declared before anmTrans even though anmTrans is used
// first: the declaration order decides which of the two index chains gets
// r3/r0 and which gets r5/r4, and the other order swaps them all.
// getFrameCtrl() does not belong here either -- it re-breaks that allocation,
// though the rest of the file does use it.
void M3UModel::updateInMotion()
{
	for (int i = 0; i < unk10; i++) {
		M3UMtxCalcSetInfo& info   = unk14[i];
		J3DFrameCtrl& frameCtrl   = unkC[info.mFrameCalcIdx];
		J3DAnmTransform* anmTrans = unk4->unk4[info.mAnmTransformIdx];
		frameCtrl.update();

		J3DJoint* jnt = unk8->getModelData()->getJointNodePointer(info.mJntIdx);
		if (info.mMtxCalcIdx == 0xff) {
			jnt->setMtxCalc(nullptr);
			continue;
		}
		anmTrans->setFrame(frameCtrl.getFrame());

		switch (info.mAnmType) {
		case 0:
			unk4->unk10[info.mMtxCalcIdx].setAnmTransform(anmTrans);
			break;
		case 1:
			unk4->unk14[info.mMtxCalcIdx].setAnmTransform(anmTrans);
			break;
		}

		jnt->setMtxCalc(unk4->getMtxCalc(unk14[i]));
	}
}

void M3UModel::updateInTexPatternAnm()
{
	if (unk1C)
		getFrameCtrl(unk1C->unk1).update();
}

void M3UModel::updateIn()
{
	updateInMotion();
	if (unk1C != nullptr) {
		getFrameCtrl(unk1C->unk1).update();
	}
}

void M3UModel::updateOut()
{
	for (int i = 0; i < unk10; i++) {
		M3UMtxCalcSetInfo& unk = unk14[i];
		unk8->getModelData()
		    ->getJointNodePointer(unk.mJntIdx)
		    ->setMtxCalc(nullptr);
	}
}

void M3UModel::entryInTexPatternAnm()
{
	if (unk1C != nullptr) {
		Unk1CStruct& tmp        = unk1C[0];
		J3DFrameCtrl& frameCtrl = getFrameCtrl(tmp.unk1);
		if (tmp.unk0 != 0xff) {
			J3DAnmTexPattern* pattern = unk4->unk8[tmp.unk0];
			pattern->setFrame(frameCtrl.getFrame());
			unk8->getModelData()->setTexNoAnimator(pattern,
			                                       unk4->unkC[tmp.unk0]);
		}
	}
}

void M3UModel::entryOutTexPatternAnm()
{
	if (unk1C != nullptr && unk1C->unk0 != 0xff)
		unk8->getModelData()->removeTexNoAnimator(unk4->unk8[unk1C->unk0]);
}

void M3UModel::entryIn() { entryInTexPatternAnm(); }

void M3UModel::entryOut() { entryOutTexPatternAnm(); }

void M3UModel::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		updateIn();
		unk8->calc();
		updateOut();
	}

	if (cue & CUE_CALC_VIEW) {
		unk8->viewCalc();
	}

	if (cue & CUE_ENTRY) {
		entryIn();
		unk8->entry();
		entryOut();
	}
}
