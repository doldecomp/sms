#include <JSystem/J3D/J3DGraphLoader/J3DJointFactory.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/JSupport.hpp>

J3DJointFactory::J3DJointFactory(const J3DJointBlock& jointBlock)
{
	mJointInitData = JSUConvertOffsetToPtr<J3DJointInitData>(
	    &jointBlock, (u32)jointBlock.mpJointInitData);
	mIndexTable
	    = JSUConvertOffsetToPtr<u16>(&jointBlock, (u32)jointBlock.mpIndexTable);
}

J3DJoint* J3DJointFactory::create(int jntNo)
{
	J3DJoint* joint = new J3DJoint();
	joint->mJntNo   = jntNo;

	joint->mKind            = getKind(jntNo);
	joint->mScaleCompensate = getScaleCompensate(jntNo);
	joint->mTransformInfo   = getTransformInfo(jntNo);

	joint->mRadius = getRadius(jntNo);

	joint->mMin = getMin(jntNo);
	joint->mMax = getMax(jntNo);

	joint->mMtxCalc    = nullptr;
	joint->mOldMtxCalc = nullptr;

	if (joint->mScaleCompensate == 0xFF) {
		joint->mScaleCompensate = false;
	}

	return joint;
}
