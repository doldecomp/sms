#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>
#include <types.h>

J3DNode::J3DNode()
{
	mCallBackUserData = nullptr;
	mCallBack         = nullptr;
	mChild            = nullptr;
	mYounger          = nullptr;
}

J3DNode::~J3DNode() { }

void J3DNode::appendChild(J3DNode* child)
{
	if (mChild == nullptr) {
		mChild = child;
	} else {
		J3DNode* curChild = mChild;
		while (curChild->getYounger() != NULL) {
			curChild = curChild->getYounger();
		}
		curChild->setYounger(child);
	}
}
