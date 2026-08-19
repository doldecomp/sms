
#include <M3DUtil/M3UModelMario.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

void M3UModelMario::changeMtxCalcSIAnmBQAnmTransform(int param_1, int param_2,
                                                     u16 param_3)
{
	SomeModelMarioStruct& tmp = unk24[param_1];
	if (tmp.unk4[param_2] != param_3) {
		tmp.unk4[param_2]  = param_3;
		J3DFrameCtrl& ctrl = unkC[tmp.unk8];
		ctrl.init(unk4->unk4[param_3]->getFrameMax());
	}
}

void M3UModelMario::updateInMotion()
{
	for (int i = 0; i < unk10; ++i) {
		SomeModelMarioStruct& info = unk24[i];
		getFrameCtrl(info.unk8).update();
		J3DJoint* jnt
		    = getModel()->getModelData()->getJointNodePointer(info.unk0);

		if (info.unk3 == 0xff) {
			jnt->setMtxCalc(nullptr);
			continue;
		}

		J3DAnmTransform* prev = unk4->getAnmTransform(info.unk4[0]);
		J3DAnmTransform* next = unk4->getAnmTransform(info.unk4[1]);

		if (prev != nullptr)
			prev->setFrame(getFrameCtrl(info.unk8).getFrame());
		if (next != nullptr)
			next->setFrame(getFrameCtrl(info.unk8).getFrame());

		unk20->unk18[info.unk3].unk54 = prev;
		unk20->unk18[info.unk3].unk58 = next;
		jnt->setMtxCalc(&unk20->unk18[info.unk3]);
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
		unk8->getModelData()
		    ->getJointNodePointer(unk24[i].unk0)
		    ->setMtxCalc(nullptr);
	}
}
