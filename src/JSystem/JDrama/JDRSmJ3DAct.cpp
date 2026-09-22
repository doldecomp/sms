#include <JSystem/JDrama/JDRSmJ3DAct.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DAnmLoader.hpp>
#include <JSystem/JDrama/JDRCharacter.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JGeometry.hpp>
#include <math.h>

using namespace JDrama;

void TSmJ3DAct::initModDat()
{
	void* modelRes = unk3C->getRes("/default.bmd");

	unk44 = J3DModelLoaderDataBase::load(
	    modelRes, J3DMLF_UseUniqueMaterials | (4 << J3DMLF_TevStageNumShift));
	unk48 = new J3DModel(unk44, 0, 1);

	void* anmRes = unk3C->getRes("/default.bck");
	if (anmRes) {
		unk4C = J3DAnmLoaderDataBase::load(anmRes);
		unk54 = J3DNewMtxCalcAnm(unk44->getFlag() & J3DMLF_MtxCalcMask,
		                         (J3DAnmTransform*)unk4C);
		unk50 = new J3DFrameCtrl;
		unk50->setEnd(unk4C->getFrameMax());
	}
}

void TSmJ3DAct::load(JSUMemoryInputStream& stream)
{
	TActor::load(stream);
	initModDat();
}

// Z, Y, X Euler chain applied to the actor's translation, the JDRCamera
// shape: one rotation matrix (never in memory, 0x170) reset per axis with its
// translation zeroed, and the two result matrices at 0x140/0x110 ping-ponging
// through `concat(a, b)`. The two `TVec3` zero translations are code-free
// +0x18 carriers that make up retail's low region; a third one overshoots by
// 0x18, so the last is the float form. Declaration order rotation, translation,
// result puts the three at 0x170/0x140/0x110.

void TSmJ3DAct::perform(u32 cue, TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		TPosition3f tmp;
		TPosition3f local_148;
		TPosition3f local_110;
		local_148.identity();
		local_148.setTrans(getPosition());
		tmp.setEularZ(DEG_TO_RAD(getRotation().z));
		tmp.setTrans(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
		local_110.concat(local_148, tmp);
		tmp.setEularY(DEG_TO_RAD(getRotation().y));
		tmp.setTrans(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
		local_148.concat(local_110, tmp);
		tmp.setEularX(DEG_TO_RAD(getRotation().x));
		tmp.setTrans(0.0f, 0.0f, 0.0f);
		local_110.concat(local_148, tmp);

		unk48->setBaseTRMtx(local_110);
		unk48->setBaseScale(getScaling());

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

	if (cue & CUE_ENTRY)
		unk48->entry();

	if (cue & CUE_CALC_VIEW)
		unk48->viewCalc();
}
