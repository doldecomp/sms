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
			mShapes[i]->offFlag(1);

		offFlag(FLAG_SITTING);
	}
}

void TJointObj::sit()
{
	for (int i = 0; i < getChildrenNum(); ++i)
		mChildren[i]->sit();

	if (!checkFlag(FLAG_SITTING | FLAG_ASLEEP | FLAG_DEAD)) {
		for (int i = 0; i < getShapeNum(); ++i)
			mShapes[i]->onFlag(1);

		onFlag(FLAG_SITTING);
	}
}

void TJointObj::awake()
{
	for (int i = 0; i < getChildrenNum(); ++i)
		mChildren[i]->awake();

	if (checkFlag(FLAG_SITTING | FLAG_ASLEEP)) {
		for (int i = 0; i < getShapeNum(); ++i)
			mShapes[i]->offFlag(1);

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
			mShapes[i]->onFlag(1);

		offFlag(FLAG_SITTING);
		onFlag(FLAG_ASLEEP);
	}
}

void TJointObj::alive()
{
	for (int i = 0; i < getChildrenNum(); ++i)
		mChildren[i]->alive();

	for (int i = 0; i < getShapeNum(); ++i)
		mShapes[i]->offFlag(1);

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
			mShapes[i]->onFlag(1);

		onFlag(FLAG_DEAD);
		offFlag(FLAG_ASLEEP);
		offFlag(FLAG_SITTING);
	}
}

// TODO: size is wrong, name sort of makes sense?
static void getShapeInOneJoint(J3DJoint* joint, J3DShape** shapes)
{
	u32 i          = 0;
	J3DMaterial* m = joint->getMesh();
	if (shapes) {
		for (; m != nullptr; ++i, m = m->getNext())
			shapes[i] = m->getShape();
	} else {
		for (; m != nullptr; m = m->getNext())
			; // assert? debug print?
	}
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

	u32 count = 0;
	for (J3DMaterial* m = getJoint()->getMesh(); m != nullptr; m = m->getNext())
		++count;
	mShapeNum = count;

	if (getShapeNum() <= 0)
		return;

	mShapes = new J3DShape*[getShapeNum()];

	getShapeInOneJoint(getJoint(), mShapes);
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
