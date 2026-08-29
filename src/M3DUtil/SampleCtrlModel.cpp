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
SampleCtrlJoint* SampleCtrlModelData::makeHierarchy(J3DJoint* joint)
{
	for (J3DMaterial* it = joint->getMesh(); it != nullptr;
	     it              = it->getNext()) {
		J3DShape* shape                = it->getShape();
		unk8[joint->getJntNo()]->unk14 = unkC[it->getIndex()];
		unkC[it->getIndex()]->unk10    = unk10[shape->getIndex()];
	}

	if (J3DJoint* child = (J3DJoint*)joint->getChild()) {
		unk8[joint->getJntNo()]->unkC = makeHierarchy(child);
	}

	if (J3DJoint* next = (J3DJoint*)joint->getYounger()) {
		unk8[joint->getJntNo()]->unk10 = makeHierarchy(next);
	}

	return unk8[joint->getJntNo()];
}
// #pragma dont_inline off

SampleCtrlModelData::SampleCtrlModelData(J3DModelData* model_data)
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
	unk4  = model_data;
	unk18 = model_data->getJointNum();
	unk1A = model_data->getMaterialNum();
	unk1C = model_data->getShapeNum();
	unk1E = model_data->getTexture()->getNum();
	unk8  = new SampleCtrlJoint*[model_data->getJointNum()];
	unkC  = new SampleCtrlMaterial*[model_data->getMaterialNum()];
	unk10 = new SampleCtrlShape*[model_data->getShapeNum()];
	for (u16 i = 0; i < model_data->getJointNum(); ++i) {
		unk8[i]       = new SampleCtrlJoint(model_data->getJointNodePointer(i));
		unk8[i]->unk8 = model_data->getJointName()->getName(i);
	}

	for (u16 i = 0; i < model_data->getMaterialNum(); ++i) {
		unkC[i] = new SampleCtrlMaterial(model_data->getMaterialNodePointer(i));
		unkC[i]->unk8 = model_data->getMaterialName()->getName(i);
	}

	for (u16 i = 0; i < model_data->getShapeNum(); ++i) {
		unk10[i] = new SampleCtrlShape(model_data->getShapeNodePointer(i));
	}

	unk14 = makeHierarchy(model_data->getRootNode());
}

// UNUSED (Size: 0x50 in MAP)
SampleCtrlModel::SampleCtrlModel(J3DModel*) { }
