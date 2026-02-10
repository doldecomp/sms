#ifndef J3D_JOINT_HPP
#define J3D_JOINT_HPP

#include <types.h>
#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <dolphin/mtx.h>

// TODO: this is hell on earth

class J3DAnmTransform;
class J3DTransformInfo;
class J3DMaterial;

class J3DMtxCalc {
public:
	virtual void init(const Vec&, const Mtx&) { }
	virtual void recursiveUpdate(J3DNode*) { }
	virtual void recursiveCalc(J3DNode*) { }
	virtual void recursiveEntry(J3DNode*) { }
	virtual void calcTransform(u16, const J3DTransformInfo&) { }
	virtual void calc(u16) { }
};

class J3DMtxCalcAnm : public virtual J3DMtxCalc {
public:
	J3DMtxCalcAnm(J3DAnmTransform* transform)
	{
		for (int i = 0; i < 2; i++) {
			mOne[i] = nullptr;
			mTwo[i] = 1.0f;
		}
		mOne[0] = transform;
	}

	virtual ~J3DMtxCalcAnm()
	{
		for (int i = 0; i < 2; i++) {
			mOne[i] = nullptr;
			mTwo[i] = 1.0f;
		}
	}
	virtual void calc(u16);

public:
	J3DAnmTransform* mOne[2];
	f32 mTwo[2];
};

class J3DMtxCalcBasic : public virtual J3DMtxCalc {
	Mtx mBackupMtx;
	Vec mBackupS;
	Vec mBackupParentS;

public:
	J3DMtxCalcBasic();

	virtual ~J3DMtxCalcBasic() { }
	virtual void init(const Vec& vec, const Mtx& mtx)
	{
		J3DSys::mCurrentS         = vec;
		J3DSys::mParentS          = (Vec) { 1.0f, 1.0f, 1.0f };
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
	virtual void recursiveUpdate(J3DNode*);
	virtual void recursiveCalc(J3DNode*);
	virtual void recursiveEntry(J3DNode*);
	virtual void calcTransform(u16, const J3DTransformInfo&);
	virtual void calc(u16);

	Mtx& getBackupMtx() { return mBackupMtx; }
	Vec& getBackupS() { return mBackupS; }
	Vec& getBackupParentS() { return mBackupParentS; }

	void setBackupS(const Vec& vec) { mBackupS = vec; }
	void setBackupParentS(const Vec& vec) { mBackupParentS = vec; }
};

class J3DMtxCalcBasicAnm : public J3DMtxCalcBasic, public J3DMtxCalcAnm {
public:
	J3DMtxCalcBasicAnm(J3DAnmTransform* transform)
	    : J3DMtxCalcBasic()
	    , J3DMtxCalcAnm(transform)
	{
	}

	virtual ~J3DMtxCalcBasicAnm() { }
	virtual void calc(u16 v) { J3DMtxCalcAnm::calc(v); }
};

class J3DMtxCalcMaya : public J3DMtxCalcBasic {
public:
	virtual ~J3DMtxCalcMaya() { }

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

class J3DMtxCalcMayaAnm : public J3DMtxCalcMaya, public J3DMtxCalcAnm {
public:
	J3DMtxCalcMayaAnm(J3DAnmTransform* transform)
	    : J3DMtxCalcMaya()
	    , J3DMtxCalcAnm(transform)
	{
	}

	virtual ~J3DMtxCalcMayaAnm() { }
	virtual void calc(u16 v) { J3DMtxCalcAnm::calc(v); }
};

class J3DMtxCalcSoftimage : public J3DMtxCalcBasic {
public:
	J3DMtxCalcSoftimage() { }

	virtual ~J3DMtxCalcSoftimage() { }
	virtual void init(const Vec& vec, const Mtx& mtx)
	{
		J3DSys::mCurrentS = vec;
		MTXCopy((Mtx&)mtx, J3DSys::mCurrentMtx);
	}
	virtual void calcTransform(u16, const J3DTransformInfo&);
};

class J3DMtxCalcSoftimageAnm : public J3DMtxCalcSoftimage,
                               public J3DMtxCalcAnm {
public:
	J3DMtxCalcSoftimageAnm(J3DAnmTransform* transform)
	    : J3DMtxCalcSoftimage()
	    , J3DMtxCalcAnm(transform)
	{
	}

	virtual ~J3DMtxCalcSoftimageAnm() { }
	virtual void calc(u16 v) { J3DMtxCalcAnm::calc(v); }
};

J3DMtxCalc* J3DNewMtxCalcAnm(u32, J3DAnmTransform*);

enum J3DJointMtxType {
	J3DJntMtxType_Normal,
	J3DJntMtxType_BBoard,
	J3DJntMtxType_YBBoard,
	J3DJntMtxType_Multi,
};

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
	u8 getMtxType() const { return (mKind >> 4) & 0xF; }
	void setMtxType(u8 type) { mKind = (mKind & ~0xf0) | (type << 4); }
	void setMtxCalc(J3DMtxCalc* mtx_calc) { mMtxCalc = mtx_calc; }
	J3DMtxCalc* getMtxCalc() { return mMtxCalc; }

	// fabricated
	const Vec& getMin() { return mMin; }
	const Vec& getMax() { return mMax; }
	void setTransformInfo(const J3DTransformInfo& v) { mTransformInfo = v; }

public:
	/* 0x18 */ u16 mJntNo;
	/* 0x1A */ u8 mKind;
	/* 0x1B */ u8 mScaleCompensate;
	/* 0x1C */ J3DTransformInfo mTransformInfo;
	/* 0x3C */ f32 mRadius;
	/* 0x40 */ Vec mMin;
	/* 0x4C */ Vec mMax;
	/* 0x58 */ J3DMtxCalc* mMtxCalc;
	/* 0x5C */ J3DMtxCalc* mOldMtxCalc;
	/* 0x60 */ J3DMaterial* mMesh;
};

#endif
