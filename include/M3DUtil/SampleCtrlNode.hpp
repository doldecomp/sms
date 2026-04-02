#ifndef M3DUTIL_SAMPLE_CTRL_NODE_HPP
#define M3DUTIL_SAMPLE_CTRL_NODE_HPP

#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

class J3DShape;
class J3DMaterial;
class J3DJoint;

class SampleCtrlShape {
public:
	SampleCtrlShape(J3DShape*);
	virtual ~SampleCtrlShape() { }

public:
	/* 0x4 */ J3DShape* unk4;
	/* 0x8 */ u8 unk8;
};

class SampleCtrlMaterial {
public:
	SampleCtrlMaterial(J3DMaterial*);
	virtual ~SampleCtrlMaterial() { }

public:
	/* 0x4 */ J3DMaterial* unk4;
	/* 0x8 */ const char* unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ SampleCtrlShape* unk10;
	/* 0x14 */ GXColor unk14;
	/* 0x18 */ J3DColorChanInfo unk18[4];
	/* 0x38 */ J3DTevOrderInfo unk38;
	/* 0x3C */ J3DTevStageInfo unk3C[16];
};

class SampleCtrlJoint {
public:
	SampleCtrlJoint(J3DJoint*);
	virtual ~SampleCtrlJoint() { }

public:
	/* 0x4 */ J3DJoint* unk4;
	/* 0x8 */ const char* unk8;
	/* 0xC */ SampleCtrlJoint* unkC;
	/* 0x10 */ SampleCtrlJoint* unk10;
	/* 0x14 */ SampleCtrlMaterial* unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ JGeometry::TVec3<f32> unk1C;
	/* 0x28 */ JGeometry::TVec3<f32> unk28;
};

#endif
