#include <M3DUtil/SampleCtrlModel.hpp>
#include <M3DUtil/SampleCtrlNode.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>

SampleCtrlJoint* SampleCtrlModelData::makeHierarchy(J3DJoint* joint)
{
	// NOTE: I am almost certain that these were asserts, but it's impossible to
	// tell which ones and where. More inlines is not the answer -- we're out of
	// stack frame padding.
	(void)0;
	(void)0;
	(void)0;
	(void)0;
	(void)0;

	for (J3DMaterial* it = joint->getMesh(); it != nullptr;
	     it              = it->getNext()) {
		J3DShape* shape                   = it->getShape();
		mJoints[joint->getJntNo()]->unk14 = mMaterials[it->getIndex()];
		mMaterials[it->getIndex()]->unk10 = mShapes[shape->getIndex()];
	}

	if (J3DJoint* child = (J3DJoint*)joint->getChild()) {
		mJoints[joint->getJntNo()]->mChild = makeHierarchy(child);
	}

	if (J3DJoint* next = (J3DJoint*)joint->getYounger()) {
		mJoints[joint->getJntNo()]->mYounger = makeHierarchy(next);
	}

	return mJoints[joint->getJntNo()];
}

SampleCtrlModelData::SampleCtrlModelData(J3DModelData* model_data)
    : unk4(nullptr)
    , mRootJoint(nullptr)
    , unk18(0)
    , unk1A(0)
    , unk1C(0)
    , unk1E(0)
{
	mJoints    = nullptr;
	mMaterials = nullptr;
	mShapes    = nullptr;
	unk4       = model_data;
	unk18      = model_data->getJointNum();
	unk1A      = model_data->getMaterialNum();
	unk1C      = model_data->getShapeNum();
	unk1E      = model_data->getTexture()->getNum();
	mJoints    = new SampleCtrlJoint*[model_data->getJointNum()];
	mMaterials = new SampleCtrlMaterial*[model_data->getMaterialNum()];
	mShapes    = new SampleCtrlShape*[model_data->getShapeNum()];
	for (u16 i = 0; i < model_data->getJointNum(); ++i) {
		mJoints[i] = new SampleCtrlJoint(model_data->getJointNodePointer(i));
		mJoints[i]->unk8 = model_data->getJointName()->getName(i);
	}

	for (u16 i = 0; i < model_data->getMaterialNum(); ++i) {
		mMaterials[i]
		    = new SampleCtrlMaterial(model_data->getMaterialNodePointer(i));
		mMaterials[i]->unk8 = model_data->getMaterialName()->getName(i);
	}

	for (u16 i = 0; i < model_data->getShapeNum(); ++i) {
		mShapes[i] = new SampleCtrlShape(model_data->getShapeNodePointer(i));
	}

	mRootJoint = makeHierarchy(model_data->getRootNode());
}

SampleCtrlModel::SampleCtrlModel(J3DModel*) { }
