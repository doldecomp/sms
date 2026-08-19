#include <Map/JointObj.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <types.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

class J3DShape;

void TJointObj::stand()
{
	for (int i = 0; i < getChildrenNum(); ++i)
		mChildren[i]->stand();

	if (checkFlag(FLAG_SITTING)) {
		for (int i = 0; i < getShapeNum(); ++i)
			mShapes[i]->offFlag(J3DShpFlag_Visible);

		offFlag(FLAG_SITTING);
	}
}

void TJointObj::sit()
{
	for (int i = 0; i < getChildrenNum(); ++i)
		mChildren[i]->sit();

	if (!checkFlag(FLAG_SITTING | FLAG_ASLEEP | FLAG_DEAD)) {
		for (int i = 0; i < getShapeNum(); ++i)
			mShapes[i]->onFlag(J3DShpFlag_Visible);

		onFlag(FLAG_SITTING);
	}
}

void TJointObj::awake()
{
	for (int i = 0; i < getChildrenNum(); ++i)
		mChildren[i]->awake();

	if (checkFlag(FLAG_SITTING | FLAG_ASLEEP)) {
		for (int i = 0; i < getShapeNum(); ++i)
			mShapes[i]->offFlag(J3DShpFlag_Visible);

		offFlag(FLAG_SITTING);
		offFlag(FLAG_ASLEEP);
	}
}

void TJointObj::sleep()
{
	for (int i = 0; i < getChildrenNum(); ++i)
		mChildren[i]->sleep();

	if (!checkFlag(FLAG_DEAD) && !checkFlag(FLAG_ASLEEP)) {
		for (int i = 0; i < getShapeNum(); ++i)
			mShapes[i]->onFlag(J3DShpFlag_Visible);

		offFlag(FLAG_SITTING);
		onFlag(FLAG_ASLEEP);
	}
}

void TJointObj::alive()
{
	for (int i = 0; i < getChildrenNum(); ++i)
		mChildren[i]->alive();

	for (int i = 0; i < getShapeNum(); ++i)
		mShapes[i]->offFlag(J3DShpFlag_Visible);

	offFlag(FLAG_DEAD);
	offFlag(FLAG_ASLEEP);
	offFlag(FLAG_SITTING);
}

void TJointObj::kill()
{
	for (int i = 0; i < getChildrenNum(); ++i)
		mChildren[i]->kill();

	if (!checkFlag(FLAG_DEAD)) {
		for (int i = 0; i < getShapeNum(); ++i)
			mShapes[i]->onFlag(J3DShpFlag_Visible);

		onFlag(FLAG_DEAD);
		offFlag(FLAG_ASLEEP);
		offFlag(FLAG_SITTING);
	}
}

static u32 getShapeInOneJoint(J3DJoint* joint, J3DShape** shapes)
{
	u32 count             = 0;
	J3DMaterial* material = joint->getMesh();
	for (; material != nullptr; ++count, material = material->getNext()) {
		if (shapes != nullptr)
			shapes[count] = material->getShape();
	}
	return count;
}

void TJointObj::initChildren()
{
	J3DJoint* child = (J3DJoint*)mJoint->getChild();
	if (!child)
		return;

	J3DNode* it = child;
	while (it != nullptr) {
		it = it->getYounger();
		++mChildrenNum;
	}

	mChildren = new TJointObj*[mChildrenNum];

	for (int i = mChildrenNum; i > 0; --i) {
		int idx = i - 1;

		mChildren[idx] = newJointObj();

		mChildren[idx]->mIndexInParent = idx;
		mChildren[idx]->initJointObj(child);

		child = (J3DJoint*)child->getYounger();
	}
}

void TJointObj::initJointObj(J3DJoint* joint)
{
	mJoint = joint;
	initChildren();

	mShapeNum = getShapeInOneJoint(mJoint, nullptr);

	if (getShapeNum() <= 0)
		return;

	mShapes = new J3DShape*[getShapeNum()];

	getShapeInOneJoint(mJoint, mShapes);
}

TJointObj::TJointObj()
{
	mIndexInParent = 0;
	mFlags         = 0;
	mJoint         = nullptr;
	mChildrenNum   = 0;
	mChildren      = nullptr;
	mShapeNum      = 0;
	mShapes        = nullptr;
}
