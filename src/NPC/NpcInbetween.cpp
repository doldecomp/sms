#include <NPC/NpcInbetween.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <M3DUtil/MActor.hpp>

void TNpcInbetween::execPosInbetween(JGeometry::TVec3<f32>* param_1)
{
	unk18.set(*param_1);
	if (unk8 >= 2) {
		unk8 -= 1;

		f32 fVar2  = unk8 * (1.0f / unk0);
		param_1->x = fVar2 * (unkC.x - unk18.x) + unk18.x;
		param_1->y = fVar2 * (unkC.y - unk18.y) + unk18.y;
		param_1->z = fVar2 * (unkC.z - unk18.z) + unk18.z;
	} else {
		unkC.set(*param_1);
		unk8 = 0;
	}
}

void TNpcInbetween::execMotionBlend(MActor* param_1)
{
	f32 fVar1 = 0.0f;
	if (isOtherThing()) {
		unk24 = 0;

		J3DAnmTransform* pJVar4 = param_1->getBckOldMotionBlendAnmPtr();

		if (pJVar4 != nullptr) {
			J3DFrameCtrl ctrl = *param_1->getFrameCtrl(0);
			ctrl.update();
			pJVar4->setFrame(ctrl.getFrame());
			fVar1 = unk28;
		}
	} else if (isThing()) {
		if (unk24 > 0)
			unk24 -= 1;
		if (unk24 > 0) {
			J3DAnmTransform* pJVar4 = param_1->getBckOldMotionBlendAnmPtr();

			if (pJVar4 != nullptr)
				pJVar4->setFrame(param_1->getBckOldMotionBlendFrame());

			fVar1 = unk24 * (1.0f / unk4);
		}
	}

	param_1->setMotionBlendRatioForBck(fVar1);
}
