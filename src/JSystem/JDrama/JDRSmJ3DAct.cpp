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

// TODO: 74.5%. Fixed here: the Euler rotations are applied **Z, then Y, then
// X** -- retail's first `sinf`/`cosf` pair takes `0x38(this)` (mRotation.z) and
// the last takes `0x30` (mRotation.x), and we had the order reversed. The
// remaining residue is the concat block itself: ~240 float operands permuted,
// our frame 8 bytes bigger with one extra callee-saved FPR (retail saves
// f21-f31, we save f20-f31), and the identity+translation matrix parked at
// 0x140(r1) in retail against 0x170 in ours while `local_110` is at 0x110 in
// both -- i.e. retail declares a fourth 48-byte matrix above it and we declare
// that one first. Declaring `tmp` before it (with or without hoisting its
// `identity()`) is worse (66.9%), so the block is a different shape, not just a
// different declaration order. The `(void)&local_148;` below is an inherited
// hack to force the matrix a stack home and should go when the block is
// re-derived.
void TSmJ3DAct::perform(u32 cue, TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		TPosition3f local_148;
		local_148.identity();
		local_148.setTrans(getPosition());

		(void)&local_148;

		TPosition3f tmp;
		tmp.identity();
		tmp.setEularZ(DEG_TO_RAD(getRotation().z));

		TMtx34f local_110;
		local_110.concat(local_148, tmp);

		tmp.setEularY(DEG_TO_RAD(getRotation().y));
		TMtx34f local_140;
		local_140.concat(local_110, tmp);

		tmp.setEularX(DEG_TO_RAD(getRotation().x));
		local_110.concat(local_140, tmp);

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
