#include <M3DUtil/SampleCtrlNode.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DColorBlocks.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTevs.hpp>

SampleCtrlJoint::SampleCtrlJoint(J3DJoint* joint)
    : unk4(nullptr)
    , unk8(nullptr)
    , mChild(nullptr)
    , mYounger(nullptr)
    , unk14(nullptr)
{
	unk4    = joint;
	mRadius = joint->getRadius();
	mMin.set(joint->getMin());
	mMax.set(joint->getMax());
}

SampleCtrlMaterial::SampleCtrlMaterial(J3DMaterial* material)
{
	unk38 = j3dDefaultTevOrderInfoNull;
	unk4  = nullptr;
	unk8  = nullptr;
	unkC  = 0;
	unk10 = nullptr;
	unk4  = material;
	unk14 = material->getMatColor(0)->color;

	for (u8 i = 0; i < 4; ++i) {
		unk18[i].mEnable    = material->getColorChan(i)->getEnable();
		unk18[i].mMatSrc    = material->getColorChan(i)->getMatSrc();
		unk18[i].mAmbSrc    = material->getColorChan(i)->getAmbSrc();
		unk18[i].mLightMask = material->getColorChan(i)->getLightMask();
		unk18[i].mDiffuseFn = material->getColorChan(i)->getDiffuseFn();
		unk18[i].mAttnFn    = material->getColorChan(i)->getAttnFn();
	}

	unk38 = *material->getTevOrder(0);

	u8 stageNum = material->getTevStageNum();
	for (u8 i = 0; i < stageNum; ++i) {
		unk3C[i].field_0x1  = material->getTevStage(i)->getTevColorA();
		unk3C[i].field_0x2  = material->getTevStage(i)->getTevColorB();
		unk3C[i].field_0x3  = material->getTevStage(i)->getTevColorC();
		unk3C[i].field_0x4  = material->getTevStage(i)->getTevColorD();
		unk3C[i].field_0x5  = material->getTevStage(i)->getTevColorOp();
		unk3C[i].field_0x6  = material->getTevStage(i)->getTevColorBias();
		unk3C[i].field_0x7  = material->getTevStage(i)->getTevColorScale();
		unk3C[i].field_0x8  = material->getTevStage(i)->getTevColorClamp();
		unk3C[i].field_0x9  = material->getTevStage(i)->getTevColorOutReg();
		unk3C[i].field_0xa  = material->getTevStage(i)->getAlphaA();
		unk3C[i].field_0xb  = material->getTevStage(i)->getAlphaB();
		unk3C[i].field_0xc  = material->getTevStage(i)->getAlphaC();
		unk3C[i].field_0xd  = material->getTevStage(i)->getAlphaD();
		unk3C[i].field_0xe  = material->getTevStage(i)->getTevAlphaOp();
		unk3C[i].field_0xf  = material->getTevStage(i)->getTevAlphaBias();
		unk3C[i].field_0x10 = material->getTevStage(i)->getTevAlphaScale();
		unk3C[i].field_0x11 = material->getTevStage(i)->getTevAlphaClamp();
		unk3C[i].field_0x12 = material->getTevStage(i)->getTevAlphaOutReg();
	}
}

SampleCtrlShape::SampleCtrlShape(J3DShape* param_1)
    : unk4(nullptr)
{
	unk4 = param_1;
	if (unk4->checkFlag(J3DShpFlag_Visible))
		unk8 = false;
	else
		unk8 = true;
}
