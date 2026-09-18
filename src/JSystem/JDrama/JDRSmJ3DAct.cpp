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

// TODO: 74.5%.  Batch 152 pinned the shape down but did not commit it: the
// three-matrix body below scores 74.06 against this one's 74.53, and a
// per-function regression is not committable, so the finding lives here.  Retail's named
// region is 0x110..0x1a4 -- exactly three 48-byte matrices, at 0x110 (the
// concat destination), 0x140 (identity + translation) and 0x170 (the rotation
// matrix, which never reaches memory) -- so the three concats reuse three
// matrices and the last two are the one-argument in-place `concat(b)`.  The
// four-matrix shape this function used to have put the translation matrix at
// 0x170 and could not be right.  With three, our slots sit at exactly the same
// relative positions, 0x30 lower, because the one difference left is 48 bytes
// of *low* region: retail spills the in-place concat's twelve results to a
// 48-byte temporary block (0xc..0x110 = 260 bytes against our 212) and
// therefore needs only f21-f31, while we keep the twelve in registers and pay
// f20 plus an extra 8 bytes of frame.  That also explains the one visible
// register difference: retail reloads 0.0f after the sinf/cosf calls (f0/f6)
// where we keep it in f31 across them.  The next step is a concat whose twelve
// values are all live before the first store -- but `concat` lives in the
// shared JGMatrix34.hpp, so it must be measured tree-wide, not changed here.
// The Euler order Z, Y, X is confirmed by the first sinf argument (0x38 =
// mRotation.z) and the last (0x30 = .x).  The `(void)&trans;` below is an
// inherited hack: without it MWCC forwards the identity/setTrans stores and
// the matrix never gets a stack home, which retail's 0x140 block contradicts.
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
