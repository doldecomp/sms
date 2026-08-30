#ifndef J3D_NODE_HPP
#define J3D_NODE_HPP

#include <dolphin/types.h>

class J3DNode;
class J3DModelData;
typedef BOOL (*J3DNodeCallBack)(J3DNode*, BOOL);

/**
 * @brief A node of a hierarchical modal.
 *
 * @details Two main actions here are *calc* and *entry*. Calc calculates
 * the global transform of this node from local transforms and entry writes this
 * node's packet into a draw buffer. *Update* is a combination of both: calc
 * then entry. See the J3DMtxCalc class for the visitors of the hierarchy.
 */
class J3DNode {
public:
	J3DNode();

	virtual void init(J3DModelData*) { }

	/// Combination of calc and entry before children
	virtual void updateIn() { }
	/// Combination of calc and entry after children
	virtual void updateOut() { }

	/// Callback for entrying this node's packets into current draw buffers.
	virtual void entryIn() { }

	/// Visit this node for calculating global transform before children
	virtual void calcIn() { }
	/// Visit this node for calculating global transform after children
	virtual void calcOut() { }

	virtual u32 getType() const { return 'NNON'; }
	virtual ~J3DNode();

	void appendChild(J3DNode*);

	J3DNode* getChild() { return mChild; }
	J3DNode* getYounger() { return mYounger; }
	void setYounger(J3DNode* pYounger) { mYounger = pYounger; }

	void setCallBack(J3DNodeCallBack callback) { mCallBack = callback; }
	void setCallBackUserData(void* data) { mCallBackUserData = data; }
	J3DNodeCallBack getCallBack() { return mCallBack; }

public:
	/* 0x04 */ void* mCallBackUserData;
	/* 0x08 */ J3DNodeCallBack mCallBack;
	/* 0x0C */ char unkC[0x4];
	/* 0x10 */ J3DNode* mChild;   ///< first child of this node
	/* 0x14 */ J3DNode* mYounger; ///< next sibling of this node
};

#endif
