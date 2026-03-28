#include <JSystem/JDrama/JDRSmJ3DAct.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DAnmLoader.hpp>
#include <JSystem/JDrama/JDRCharacter.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JGeometry.hpp>
#include <math.h>

using namespace JDrama;

void TSmJ3DAct::load(JSUMemoryInputStream& stream)
{
	TActor::load(stream);

	void* modelRes = unk3C->getRes("/default.bmd");

	unk44 = J3DModelLoaderDataBase::load(modelRes, 0x240000);
	unk48 = new J3DModel(unk44, 0, 1);

	void* anmRes = unk3C->getRes("/default.bck");
	if (anmRes) {
		unk4C = J3DAnmLoaderDataBase::load(anmRes);
		unk54 = J3DNewMtxCalcAnm(unk44->getUnkC(), (J3DAnmTransform*)unk4C);
		unk50 = new J3DFrameCtrl;
		unk50->setEnd(unk4C->getFrameMax());
	}
}

void TSmJ3DAct::perform(u32 param_1, TGraphics* param_2)
{
	if (param_1 & 2) {
		TPosition3f local_148;
		local_148.identity();
		local_148.setTrans(mPosition);

		(void)&local_148;

		TPosition3f tmp;
		tmp.identity();
		tmp.setEularX(DEG_TO_RAD(mRotation.x));

		TMtx34f local_110;
		local_110.concat(local_148, tmp);

		tmp.setEularY(DEG_TO_RAD(mRotation.y));
		TMtx34f local_140;
		local_140.concat(local_110, tmp);

		tmp.setEularZ(DEG_TO_RAD(mRotation.z));
		local_110.concat(local_140, tmp);

		unk48->setBaseTRMtx(local_110);
		unk48->setBaseScale(mScaling);

		if (unk4C == nullptr) {
			unk48->calc();
		} else {
			unk50->update();
			unk4C->setFrame(unk50->getFrame());
			J3DMtxCalc* prevCalc
			    = unk48->getModelData()->getJointNodePointer(0)->getMtxCalc();
			unk48->getModelData()->getJointNodePointer(0)->setMtxCalc(unk54);
			unk48->calc();
			unk48->getModelData()->getJointNodePointer(0)->setMtxCalc(prevCalc);
		}
	}

	if (param_1 & 0x200)
		unk48->entry();

	if (param_1 & 0x4)
		unk48->viewCalc();
}
