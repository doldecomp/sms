#ifndef J3D_JOINT_HPP
#define J3D_JOINT_HPP

#include <types.h>
#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <dolphin/mtx.h>

class J3DAnmTransform;
class J3DTransformInfo;
class J3DMaterial;

/**
 * @brief Skeleton traversal strategy for calculating global J3DNode transforms
 * from local ones. Derived classes correspond to different exporters used for
 * 3D model authoring.
 */
class J3DMtxCalc {
public:
	/**
	 * @brief Prepares for traversal of the joint hierarchy.
	 *
	 * @param baseScale Base scale to apply to the root joint.
	 * @param baseTransform Base transform to apply to the root joint.
	 */
	virtual void init(const Vec& baseScale, const Mtx& baseTransform) { }

	/**
	 * @brief Traverses the hierarchy, and does the transform work and the draw
	 * work.
	 *
	 * @details Depth first. At each node it calls J3DNode::updateIn(), goes
	 * into the child, then calls J3DNode::updateOut(). A joint calculates its
	 * matrix and entries its materials.
	 *
	 * @param node Node to start from, usually the root joint.
	 */
	virtual void recursiveUpdate(J3DNode* node) { }

	/**
	 * @brief Traverses the hierarchy, and does the transform work only.
	 *
	 * @details As recursiveUpdate(), but through J3DNode::calcIn() and
	 * J3DNode::calcOut(). A joint calculates its matrix and entries nothing.
	 * Use it to move a model that you draw later.
	 *
	 * @param node Node to start from, usually the root joint.
	 */
	virtual void recursiveCalc(J3DNode* node) { }

	/**
	 * @brief Traverses the hierarchy, and does the draw work only.
	 *
	 * @details Calls J3DNode::entryIn(), then goes into the child. It entries
	 * the materials against the matrices of an earlier pass, so one set of
	 * matrices can serve several draws.
	 *
	 * @param node Node to start from, usually the root joint.
	 */
	virtual void recursiveEntry(J3DNode* node) { }

	/**
	 * @brief Combines the local transform of a joint with the parent transform,
	 * and stores the result.
	 *
	 * @param jntIdx Joint number. It indexes the matrix array of the model.
	 * @param transform Local transform of the joint for this frame.
	 */
	virtual void calcTransform(u16 jntIdx, const J3DTransformInfo& transform) {
	}

	/**
	 * @brief Finds the local transform of one joint, and applies it.
	 *
	 * @param jntIdx Joint number to calculate.
	 */
	virtual void calc(u16 jntIdx) { }
};

/**
 * @brief MtxCalc that fetches local joint transforms from a J3DAnmTransform and
 * so supports skeletal animations.
 */
class J3DMtxCalcAnm : public virtual J3DMtxCalc {
public:
	J3DMtxCalcAnm(J3DAnmTransform* transform)
	{
		initAnm();
		setAnmTransform(transform);
	}

	virtual ~J3DMtxCalcAnm() { initAnm(); }
	virtual void calc(u16 jntIdx);

	void initAnm()
	{
		for (int i = 0; i < 2; i++) {
			mOne[i] = nullptr;
			mTwo[i] = 1.0f;
		}
	}
	void setAnmTransform(J3DAnmTransform* mAnmTransform)
	{
		mOne[0] = mAnmTransform;
	}

public:
	J3DAnmTransform* mOne[2];
	f32 mTwo[2];
};

/**
 * @brief MtxCalc for a basic in-house skeletal hierarchy format.
 * @details See J3DMtxCalcBasic::calcTransform for how scaling is propagated
 * through the hierarchy.
 */
class J3DMtxCalcBasic : public virtual J3DMtxCalc {
	Mtx mBackupMtx;
	Vec mBackupS;
	Vec mBackupParentS;

public:
	J3DMtxCalcBasic();

	virtual ~J3DMtxCalcBasic() { }
	virtual void init(const Vec& baseScale, const Mtx& baseTransform)
	{
		J3DSys::mCurrentS         = baseScale;
		J3DSys::mParentS          = (Vec) { 1.0f, 1.0f, 1.0f };
		J3DSys::mCurrentMtx[0][0] = baseTransform[0][0] * baseScale.x;
		J3DSys::mCurrentMtx[0][1] = baseTransform[0][1] * baseScale.y;
		J3DSys::mCurrentMtx[0][2] = baseTransform[0][2] * baseScale.z;
		J3DSys::mCurrentMtx[0][3] = baseTransform[0][3];
		J3DSys::mCurrentMtx[1][0] = baseTransform[1][0] * baseScale.x;
		J3DSys::mCurrentMtx[1][1] = baseTransform[1][1] * baseScale.y;
		J3DSys::mCurrentMtx[1][2] = baseTransform[1][2] * baseScale.z;
		J3DSys::mCurrentMtx[1][3] = baseTransform[1][3];
		J3DSys::mCurrentMtx[2][0] = baseTransform[2][0] * baseScale.x;
		J3DSys::mCurrentMtx[2][1] = baseTransform[2][1] * baseScale.y;
		J3DSys::mCurrentMtx[2][2] = baseTransform[2][2] * baseScale.z;
		J3DSys::mCurrentMtx[2][3] = baseTransform[2][3];
	}
	virtual void recursiveUpdate(J3DNode* node);
	virtual void recursiveCalc(J3DNode* node);
	virtual void recursiveEntry(J3DNode* node);
	virtual void calcTransform(u16, const J3DTransformInfo&);
	virtual void calc(u16);

	Mtx& getBackupMtx() { return mBackupMtx; }
	Vec& getBackupS() { return mBackupS; }
	Vec& getBackupParentS() { return mBackupParentS; }

	void setBackupS(const Vec& vec) { mBackupS = vec; }
	void setBackupParentS(const Vec& vec) { mBackupParentS = vec; }
};

/**
 * @brief MtxCalc for a basic in-house skeletal hierarchy format with
 * added skeletal animation support.
 */
class J3DMtxCalcBasicAnm : public J3DMtxCalcBasic, public J3DMtxCalcAnm {
public:
	J3DMtxCalcBasicAnm(J3DAnmTransform* transform)
	    : J3DMtxCalcBasic()
	    , J3DMtxCalcAnm(transform)
	{
	}

	virtual void calc(u16 v) { J3DMtxCalcAnm::calc(v); }
};

/**
 * @brief MtxCalc for Maya (the software) skeletal hierarchy format.
 * @details See J3DMtxCalcMaya::calcTransform for how scaling is propagated
 * through the hierarchy.
 */
class J3DMtxCalcMaya : public J3DMtxCalcBasic {
public:
	virtual void init(const Vec& vec, const Mtx& mtx)
	{
		J3DSys::mParentS          = (Vec) { 1.0f, 1.0f, 1.0f };
		J3DSys::mCurrentS         = vec;
		J3DSys::mCurrentMtx[0][0] = mtx[0][0] * vec.x;
		J3DSys::mCurrentMtx[0][1] = mtx[0][1] * vec.y;
		J3DSys::mCurrentMtx[0][2] = mtx[0][2] * vec.z;
		J3DSys::mCurrentMtx[0][3] = mtx[0][3];
		J3DSys::mCurrentMtx[1][0] = mtx[1][0] * vec.x;
		J3DSys::mCurrentMtx[1][1] = mtx[1][1] * vec.y;
		J3DSys::mCurrentMtx[1][2] = mtx[1][2] * vec.z;
		J3DSys::mCurrentMtx[1][3] = mtx[1][3];
		J3DSys::mCurrentMtx[2][0] = mtx[2][0] * vec.x;
		J3DSys::mCurrentMtx[2][1] = mtx[2][1] * vec.y;
		J3DSys::mCurrentMtx[2][2] = mtx[2][2] * vec.z;
		J3DSys::mCurrentMtx[2][3] = mtx[2][3];
	}
	virtual void calcTransform(u16, const J3DTransformInfo&);
};

/**
 * @brief MtxCalc for Maya (the software) skeletal hierarchy format with
 * added skeletal animation support.
 */
class J3DMtxCalcMayaAnm : public J3DMtxCalcMaya, public J3DMtxCalcAnm {
public:
	J3DMtxCalcMayaAnm(J3DAnmTransform* transform)
	    : J3DMtxCalcMaya()
	    , J3DMtxCalcAnm(transform)
	{
	}

	virtual void calc(u16 v) { J3DMtxCalcAnm::calc(v); }
};

/**
 * @brief MtxCalc for Softimage|3D (the software) skeletal hierarchy format.
 * @details See J3DMtxCalcSoftimage::calcTransform for how scaling is propagated
 * through the hierarchy.
 */
class J3DMtxCalcSoftimage : public J3DMtxCalcBasic {
public:
	J3DMtxCalcSoftimage() { }

	virtual void init(const Vec& vec, const Mtx& mtx)
	{
		J3DSys::mCurrentS = vec;
		MTXCopy((Mtx&)mtx, J3DSys::mCurrentMtx);
	}
	virtual void calcTransform(u16, const J3DTransformInfo&);
};

/**
 * @brief MtxCalc for Softimage|3D (the software) skeletal hierarchy format with
 * added skeletal animation support.
 */
class J3DMtxCalcSoftimageAnm : public J3DMtxCalcSoftimage,
                               public J3DMtxCalcAnm {
public:
	J3DMtxCalcSoftimageAnm(J3DAnmTransform* transform)
	    : J3DMtxCalcSoftimage()
	    , J3DMtxCalcAnm(transform)
	{
	}

	virtual void calc(u16 v) { J3DMtxCalcAnm::calc(v); }
};

J3DMtxCalc* J3DNewMtxCalcAnm(u32, J3DAnmTransform*);

enum J3DJointMtxType {
	J3DJntMtxType_Normal,
	J3DJntMtxType_BBoard,
	J3DJntMtxType_YBBoard,
	J3DJntMtxType_Multi,
};

/**
 * @brief A joint node of a skeletal model, the only type of node out there.
 * J3DNode is merged into this class in later J3D revisions.
 */
class J3DJoint : public J3DNode {
	friend class J3DJointFactory;

public:
	J3DJoint() { initialize(); }
	J3DJoint(u16, const J3DTransformInfo&);

	void initialize();
	void addMesh(J3DMaterial*);

	virtual void updateIn();
	virtual void updateOut();
	virtual void entryIn();
	virtual void calcIn();
	virtual void calcOut();

	virtual u32 getType() const { return 'NJNT'; }
	virtual ~J3DJoint() { }

	u16 getJntNo() const { return mJntNo; }
	J3DTransformInfo& getTransformInfo() { return mTransformInfo; }
	u8 getScaleCompensate() const { return mScaleCompensate; }
	J3DMaterial* getMesh() { return mMesh; }
	u8 getMtxType() const { return (mKind & 0xF0) >> 4; }
	void setMtxType(u8 type) { mKind = (mKind & ~0xF0) | (type << 4); }

	/// Sets an MtxCalc override for this joint's subtree
	void setMtxCalc(J3DMtxCalc* mtxCalc) { mMtxCalcOverride = mtxCalc; }
	/// Gets the MtxCalc override of this joint
	J3DMtxCalc* getMtxCalc() { return mMtxCalcOverride; }

	f32 getRadius() const { return mRadius; }
	const Vec& getMin() { return mMin; }
	const Vec& getMax() { return mMax; }
	void setTransformInfo(const J3DTransformInfo& v) { mTransformInfo = v; }

private:
	/* 0x18 */ u16 mJntNo;
	/* 0x1A */ u8 mKind;
	/* 0x1B */ u8 mScaleCompensate;
	/* 0x1C */ J3DTransformInfo mTransformInfo;
	/* 0x3C */ f32 mRadius;
	/* 0x40 */ Vec mMin;
	/* 0x4C */ Vec mMax;
	/* 0x58 */ J3DMtxCalc* mMtxCalcOverride;
	/* 0x5C */ J3DMtxCalc* mOldMtxCalc;
	/* 0x60 */ J3DMaterial* mMesh;
};

inline BOOL checkScaleOne(Vec v)
{
	if (v.x == 1.0f && v.y == 1.0f && v.z == 1.0f) {
		return TRUE;
	} else {
		return FALSE;
	}
}

#endif
