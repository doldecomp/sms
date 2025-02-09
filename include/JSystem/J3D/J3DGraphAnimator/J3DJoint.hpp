#ifndef J3D_JOINT_HPP
#define J3D_JOINT_HPP

#include <types.h>
#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>
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
	virtual void init(const Vec&, const Mtx&) { }
	virtual void recursiveUpdate(J3DNode*);
	virtual void recursiveCalc(J3DNode*);
	virtual void recursiveEntry(J3DNode*);
	virtual void calcTransform(u16, const J3DTransformInfo&);
	virtual void calc(u16);
};

class J3DMtxCalcBasicAnm : public J3DMtxCalcBasic, public J3DMtxCalcAnm {
public:
	J3DMtxCalcBasicAnm(J3DAnmTransform* transform)
	    : J3DMtxCalcBasic()
	    , J3DMtxCalcAnm(transform)
	{
	}

	virtual ~J3DMtxCalcBasicAnm() { }
	virtual void calc(u16);
};

class J3DMtxCalcMaya : public J3DMtxCalcBasic {
public:
	virtual ~J3DMtxCalcMaya() { }

	virtual void init(const Vec&, const Mtx&) { }
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
	virtual void calc(u16) { }
};

class J3DMtxCalcSoftimage : public J3DMtxCalcBasic {
public:
	J3DMtxCalcSoftimage() { }

	virtual ~J3DMtxCalcSoftimage() { }
	virtual void init(const Vec&, const Mtx&) { }
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
	virtual void calc(u16) { }
};

J3DMtxCalc* J3DNewMtxCalcAnm(u32, J3DAnmTransform*);

class J3DJoint : public J3DNode {
public:
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
};

#endif
