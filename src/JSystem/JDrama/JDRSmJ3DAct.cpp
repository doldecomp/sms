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
		unk50->mEndFrame = unk4C->getFrameMax();
	}
}

void TSmJ3DAct::perform(u32 param_1, TGraphics* param_2) { }
