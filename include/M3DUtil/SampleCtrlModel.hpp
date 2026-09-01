#ifndef M3DUTIL_SAMPLE_CTRL_MODEL_HPP
#define M3DUTIL_SAMPLE_CTRL_MODEL_HPP

#include <dolphin/types.h>

class J3DModelData;
class J3DModel;
class J3DJoint;
class SampleCtrlJoint;
class SampleCtrlMaterial;
class SampleCtrlShape;

// TODO: unused and probably not reconstructable
class SampleCtrlModel {
public:
	SampleCtrlModel(J3DModel*);

	virtual ~SampleCtrlModel() { }
};

class SampleCtrlModelData {
public:
	SampleCtrlModelData(J3DModelData*);

	SampleCtrlJoint* makeHierarchy(J3DJoint* joint);

	virtual ~SampleCtrlModelData() { }

	SampleCtrlJoint* getJoint(int idx) { return mJoints[idx]; }

public:
	/* 0x4 */ J3DModelData* unk4;
	/* 0x8 */ SampleCtrlJoint** mJoints;
	/* 0xC */ SampleCtrlMaterial** mMaterials;
	/* 0x10 */ SampleCtrlShape** mShapes;
	/* 0x14 */ SampleCtrlJoint* mRootJoint;
	/* 0x18 */ u16 unk18;
	/* 0x1A */ u16 unk1A;
	/* 0x1C */ u16 unk1C;
	/* 0x1E */ u16 unk1E;
};

#endif
