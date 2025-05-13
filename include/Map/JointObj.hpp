#ifndef JOINT_OBJ_HPP
#define JOINT_OBJ_HPP

#include <dolphin/types.h>

class J3DJoint;
class J3DShape;

class TJointObj {
public:
	TJointObj();

	void initChildren();

	virtual void initJointObj(J3DJoint*);
	virtual void alive();
	virtual void awake();
	virtual void stand();
	virtual void kill();
	virtual void sleep();
	virtual void sit();
	virtual TJointObj* newJointObj() const { return new TJointObj; }

	// fabricated
	bool checkFlag(u32 flag) { return mFlags & flag ? true : false; }
	void onFlag(u32 flag) { mFlags |= flag; }
	void offFlag(u32 flag) { mFlags &= ~flag; }
	void setIndexInParent(u16 i) { mIndexInParent = i; }
	int getShapeNum() const { return mShapeNum; }
	J3DShape* getShape(int i) { return mShapes[i]; }
	int getChildrenNum() const { return mChildrenNum; }
	TJointObj* getChild(int i) { return mChildren[i]; }
	J3DJoint* getJoint() { return mJoint; }

public:
	/* 0x4 */ u32 mIndexInParent;
	/* 0x8 */ u32 mFlags;
	/* 0xC */ J3DJoint* mJoint;
	/* 0x10 */ int mChildrenNum;
	/* 0x14 */ TJointObj** mChildren;
	/* 0x18 */ int mShapeNum;
	/* 0x1C */ J3DShape** mShapes;
};

#endif
