
#include <M3DUtil/M3UModelMario.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

void M3UModelMario::changeMtxCalcSIAnmBQAnmTransform(int param_1, int param_2,
                                                     u16 param_3)
{
	M3UMarioMtxCalcSetInfo& unk = unk24[param_1];
	u16 jntId                   = unk.mAnmTransformIdx[param_2];
	if (jntId != param_3) {
		unk.mAnmTransformIdx[param_2] = param_3;
		J3DFrameCtrl& frameCtrl       = getFrameCtrl(unk.mFrameCtrlIdx);
		frameCtrl.init(unk4->unk4[param_3]->getFrameMax());
	}
}

void M3UModelMario::updateInMotion()
{
	// volatile u32 unused[12];
	for (int i = 0; i < unk10; ++i) {
		M3UMarioMtxCalcSetInfo& info = unk24[i];
		getFrameCtrl(info.mFrameCtrlIdx).update();
		J3DJoint* jnt = unk8->mModelData->getJointNodePointer(info.mJointIdx);
		if (info.mMtxCalcIdx == 0xff) {
			jnt->setMtxCalc(nullptr);
			continue;
		}
		J3DAnmTransform* prev = unk4->unk4[info.mAnmTransformIdx[0]];
		J3DAnmTransform* next = unk4->unk4[info.mAnmTransformIdx[1]];
		if (prev != nullptr) {
			prev->setFrame(getFrameCtrl(info.mFrameCtrlIdx).getCurrentFrame());
		}
		if (next != nullptr) {
			next->setFrame(getFrameCtrl(info.mFrameCtrlIdx).getCurrentFrame());
		}

		// If we make a fabricated getter for unk18 it uses all padding and
		// matches 100% I don't feel it fits as i would expect the same from
		// fields in unk4, which would make the stack larger
		unk20->unk18[info.mMtxCalcIdx].unk54 = prev;
		unk20->unk18[info.mMtxCalcIdx].unk58 = next;
		jnt->setMtxCalc(&unk20->unk18[info.mMtxCalcIdx]);
	}
}

void M3UModelMario::updateIn()
{
	updateInMotion();
	updateInTexPatternAnm();
}

void M3UModelMario::updateOut()
{
	for (int i = 0; i < unk10; i++) {
		M3UMarioMtxCalcSetInfo& jntId = unk24[i];
		unk8->mModelData->getJointNodePointer(jntId.mJointIdx)
		    ->setMtxCalc(nullptr);
	}
}
