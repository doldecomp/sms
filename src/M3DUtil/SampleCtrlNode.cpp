#include <M3DUtil/SampleCtrlNode.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DColorBlocks.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTevs.hpp>

SampleCtrlJoint::SampleCtrlJoint(J3DJoint* param_1)
    : unk4(nullptr)
    , unk8(nullptr)
    , unkC(nullptr)
    , unk10(nullptr)
    , unk14(nullptr)
{
	unk4  = param_1;
	unk18 = param_1->getRadius();
	unk1C.set(param_1->getMin());
	unk28.set(param_1->getMax());
}

SampleCtrlMaterial::SampleCtrlMaterial(J3DMaterial* param_1)
{
	unk38 = j3dDefaultTevOrderInfoNull;
	unk4  = nullptr;
	unk8  = nullptr;
	unkC  = 0;
	unk10 = nullptr;
	unk4  = param_1;
	unk14 = param_1->getColorBlock()->getMatColor(0)->color;

	for (u8 i = 0; i < 4; ++i) {
		unk18[i].mEnable
		    = param_1->getColorBlock()->getColorChan(i)->getEnable();
		unk18[i].mMatSrc
		    = param_1->getColorBlock()->getColorChan(i)->getMatSrc();
		unk18[i].mAmbSrc
		    = param_1->getColorBlock()->getColorChan(i)->getAmbSrc();
		unk18[i].mLightMask
		    = param_1->getColorBlock()->getColorChan(i)->getLightMask();
		unk18[i].mDiffuseFn
		    = param_1->getColorBlock()->getColorChan(i)->getDiffuseFn();
		unk18[i].mAttnFn
		    = param_1->getColorBlock()->getColorChan(i)->getAttnFn();
	}

	unk38 = *param_1->getTevBlock()->getTevOrder(0);

	u8 stageNum = param_1->getTevBlock()->getTevStageNum();
	for (u8 i = 0; i < stageNum; ++i) {
		unk3C[i].field_0x1
		    = param_1->getTevBlock()->getTevStage(i)->getTevColorA();
		unk3C[i].field_0x2
		    = param_1->getTevBlock()->getTevStage(i)->getTevColorB();
		unk3C[i].field_0x3
		    = param_1->getTevBlock()->getTevStage(i)->getTevColorC();
		unk3C[i].field_0x4
		    = param_1->getTevBlock()->getTevStage(i)->getTevColorD();
		unk3C[i].field_0x5
		    = param_1->getTevBlock()->getTevStage(i)->getColorOpSomething1();
		unk3C[i].field_0x6
		    = param_1->getTevBlock()->getTevStage(i)->getColorOpSomething2();
		unk3C[i].field_0x7
		    = param_1->getTevBlock()->getTevStage(i)->getColorOpSomething3();
		unk3C[i].field_0x8
		    = param_1->getTevBlock()->getTevStage(i)->getColorOpSomething4();
		unk3C[i].field_0x9
		    = param_1->getTevBlock()->getTevStage(i)->getColorOpSomething5();
		unk3C[i].field_0xa
		    = param_1->getTevBlock()->getTevStage(i)->getAlphaA();
		unk3C[i].field_0xb
		    = param_1->getTevBlock()->getTevStage(i)->getAlphaB();
		unk3C[i].field_0xc
		    = param_1->getTevBlock()->getTevStage(i)->getAlphaC();
		unk3C[i].field_0xd
		    = param_1->getTevBlock()->getTevStage(i)->getAlphaD();
		unk3C[i].field_0xe
		    = param_1->getTevBlock()->getTevStage(i)->getAlphaOpSomething1();
		unk3C[i].field_0xf
		    = param_1->getTevBlock()->getTevStage(i)->getAlphaOpSomething2();
		unk3C[i].field_0x10
		    = param_1->getTevBlock()->getTevStage(i)->getAlphaOpSomething3();
		unk3C[i].field_0x11
		    = param_1->getTevBlock()->getTevStage(i)->getAlphaOpSomething4();
		unk3C[i].field_0x12
		    = param_1->getTevBlock()->getTevStage(i)->getAlphaOpSomething5();
	}
}

SampleCtrlShape::SampleCtrlShape(J3DShape* param_1)
    : unk4(nullptr)
{
	unk4 = param_1;
	if (unk4->checkFlag(0x1))
		unk8 = false;
	else
		unk8 = true;
}
