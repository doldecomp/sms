#include <Map/JointObj.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <types.h>

class J3DShape;

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

void TJointObj::initJointObj(J3DJoint* joint)
{
	mJoint = joint;
	initChildren();
	u32 count = 0;
	for (J3DMaterial* m = mJoint->getMesh(); m != nullptr; m = m->getNext())
		++count;
	mShapeNum = count;

	if (mShapeNum <= 0)
		return;

	mShapes = new J3DShape*[mShapeNum];

	u32 i          = 0;
	J3DMaterial* m = mJoint->getMesh();

	J3DShape** shapes = mShapes;
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

void getShapeInOneJoint(J3DJoint*, J3DShape**) { }

void TJointObj::kill()
{
	for (int i = 0; i < mChildrenNum; ++i)
		mChildren[i]->kill();

	if (!checkFlag(1)) {
		for (int i = 0; i < mShapeNum; ++i)
			mShapes[i]->onFlag(1);

		onFlag(1);
		offFlag(2);
		offFlag(4);
	}
}

void TJointObj::alive()
{
	for (int i = 0; i < mChildrenNum; ++i)
		mChildren[i]->alive();

	for (int i = 0; i < mShapeNum; ++i)
		mShapes[i]->offFlag(1);

	offFlag(1);
	offFlag(2);
	offFlag(4);
}

void TJointObj::sleep()
{
	for (int i = 0; i < mChildrenNum; ++i)
		mChildren[i]->sleep();

	if (!checkFlag(1) && !checkFlag(2)) {
		for (int i = 0; i < mShapeNum; ++i)
			mShapes[i]->onFlag(1);

		offFlag(4);
		onFlag(2);
	}
}

void TJointObj::awake()
{
	for (int i = 0; i < mChildrenNum; ++i)
		mChildren[i]->awake();

	if (checkFlag(4 | 2)) {
		for (int i = 0; i < mShapeNum; ++i)
			mShapes[i]->offFlag(1);

		offFlag(4);
		offFlag(2);
	}
}

void TJointObj::sit()
{
	for (int i = 0; i < mChildrenNum; ++i)
		mChildren[i]->sit();

	if (!checkFlag(4 | 2 | 1)) {
		for (int i = 0; i < mShapeNum; ++i)
			mShapes[i]->onFlag(1);

		onFlag(4);
	}
}

void TJointObj::stand()
{
	for (int i = 0; i < mChildrenNum; ++i)
		mChildren[i]->stand();

	if (checkFlag(4)) {
		for (int i = 0; i < mShapeNum; ++i)
			mShapes[i]->offFlag(1);

		offFlag(4);
	}
}
