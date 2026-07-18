#include <M3DUtil/SampleCtrlModel.hpp>
#include <M3DUtil/SampleCtrlNode.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>

// Matches 100% with dont_inline...
// #pragma dont_inline on
SampleCtrlJoint* SampleCtrlModelData::makeHierarchy(J3DJoint* param_1)
{
	for (J3DMaterial* it = param_1->getMesh(); it != nullptr;
	     it              = it->getNext()) {
		J3DShape* shape                  = it->getShape();
		unk8[param_1->getJntNo()]->unk14 = unkC[it->getIndex()];
		unkC[it->getIndex()]->unk10      = unk10[shape->getIndex()];
	}

	if (J3DJoint* child = (J3DJoint*)param_1->getChild()) {
		unk8[param_1->getJntNo()]->unkC = makeHierarchy(child);
	}

	if (J3DJoint* next = (J3DJoint*)param_1->getYounger()) {
		unk8[param_1->getJntNo()]->unk10 = makeHierarchy(next);
	}

	return unk8[param_1->getJntNo()];
}
// #pragma dont_inline off

SampleCtrlModelData::SampleCtrlModelData(J3DModelData* param_1)
    : unk4(nullptr)
    , unk14(nullptr)
    , unk18(0)
    , unk1A(0)
    , unk1C(0)
    , unk1E(0)
{
	unk8  = nullptr;
	unkC  = nullptr;
	unk10 = nullptr;
	unk4  = param_1;
	unk18 = param_1->getJointNum();
	unk1A = param_1->getMaterialNum();
	unk1C = param_1->getShapeNum();
	unk1E = param_1->getTexture()->getNum();
	unk8  = new SampleCtrlJoint*[param_1->getJointNum()];
	unkC  = new SampleCtrlMaterial*[param_1->getMaterialNum()];
	unk10 = new SampleCtrlShape*[param_1->getShapeNum()];
	for (u16 i = 0; i < param_1->getJointNum(); ++i) {
		unk8[i]       = new SampleCtrlJoint(param_1->getJointNodePointer(i));
		unk8[i]->unk8 = param_1->getJointName()->getName(i);
	}

	for (u16 i = 0; i < param_1->getMaterialNum(); ++i) {
		unkC[i] = new SampleCtrlMaterial(param_1->getMaterialNodePointer(i));
		unkC[i]->unk8 = param_1->getMaterialName()->getName(i);
	}

	for (u16 i = 0; i < param_1->getShapeNum(); ++i) {
		unk10[i] = new SampleCtrlShape(param_1->getShapeNodePointer(i));
	}

	unk14 = makeHierarchy(param_1->getRootNode());
}
