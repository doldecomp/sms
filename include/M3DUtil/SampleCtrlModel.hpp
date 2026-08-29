#ifndef M3DUTIL_SAMPLE_CTRL_MODEL_HPP
#define M3DUTIL_SAMPLE_CTRL_MODEL_HPP

#include <dolphin/types.h>

class J3DModelData;
class J3DModel;
class J3DJoint;
class SampleCtrlJoint;
class SampleCtrlMaterial;
class SampleCtrlShape;

// TODO: reconstructed from the symbol map only. The members and the
// constructor body are not known yet (the map gives the constructor a size of
// 0x50 and the vtable a size of 0xc, so the destructor is the only virtual).
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

public:
	/* 0x4 */ J3DModelData* unk4;
	/* 0x8 */ SampleCtrlJoint** unk8;
	/* 0xC */ SampleCtrlMaterial** unkC;
	/* 0x10 */ SampleCtrlShape** unk10;
	/* 0x14 */ SampleCtrlJoint* unk14;
	/* 0x18 */ u16 unk18;
	/* 0x1A */ u16 unk1A;
	/* 0x1C */ u16 unk1C;
	/* 0x1E */ u16 unk1E;
};

#endif
