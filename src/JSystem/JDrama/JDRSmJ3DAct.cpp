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
		unk50->setEndFrame(unk4C->getFrameMax());
	}
}

void TSmJ3DAct::perform(u32 param_1, TGraphics* param_2)
{
	if (param_1 & 2) {
		TPosition3f mtx;
		mtx.translation(mPosition.x, mPosition.y, mPosition.z);

		TRotation3f rotX;
		rotX.setEularX(DEG_TO_RAD(mRotation.x));
		TMtx34f mtx1;
		mtx1.concat(rotX, mtx);

		TRotation3f rotY;
		rotY.setEularY(DEG_TO_RAD(mRotation.y));
		TMtx34f mtx2;
		mtx2.concat(rotY, mtx1);

		TRotation3f rotZ;
		rotZ.setEularZ(DEG_TO_RAD(mRotation.z));
		TMtx34f mtx3;
		mtx3.concat(rotZ, mtx2);

		unk48->setBaseTRMtx(mtx3);
		unk48->setBaseScale(mScaling);

		if (unk4C == nullptr) {
			unk48->calc();
		} else {
			unk50->update();
			unk4C->setFrame(unk50->getCurrentFrame());
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
