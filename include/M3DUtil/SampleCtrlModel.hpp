#ifndef M3DUTIL_SAMPLE_CTRL_MODEL_HPP
#define M3DUTIL_SAMPLE_CTRL_MODEL_HPP

#include <dolphin/types.h>

class J3DModelData;
class J3DJoint;
class SampleCtrlJoint;
class SampleCtrlMaterial;
class SampleCtrlShape;

class SampleCtrlModelData {
public:
	SampleCtrlModelData(J3DModelData*);

	SampleCtrlJoint* makeHierarchy(J3DJoint*);

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
