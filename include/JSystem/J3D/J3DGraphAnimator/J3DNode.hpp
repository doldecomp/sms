#ifndef J3D_NODE_HPP
#define J3D_NODE_HPP

#include <dolphin/types.h>

class J3DNode;
class J3DModelData;
typedef int (*J3DNodeCallBack)(J3DNode*, int);

class J3DNode {
public:
	J3DNode();

	virtual void init(J3DModelData*) { }
	virtual void updateIn() { }
	virtual void updateOut() { }
	virtual void entryIn() { }
	virtual void calcIn() { }
	virtual void calcOut() { }
	virtual u32 getType() const { return 'NNON'; }
	virtual ~J3DNode();

	void appendChild(J3DNode*);

	J3DNode* getYounger() { return mYounger; }
	void setYounger(J3DNode* pYounger) { mYounger = pYounger; }
	void setCallBack(J3DNodeCallBack callback) { mCallBack = callback; }
	void setCallBackUserData(void* data) { mCallBackUserData = data; }
	J3DNodeCallBack getCallBack() { return mCallBack; }
	J3DNode* getChild() { return mChild; }

public:
	/* 0x04 */ void* mCallBackUserData;
	/* 0x08 */ J3DNodeCallBack mCallBack;
	/* 0x0C */ void* field_0x8;
	/* 0x10 */ J3DNode* mChild;
	/* 0x14 */ J3DNode* mYounger;
};

#endif
