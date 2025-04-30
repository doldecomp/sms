#ifndef MAP_MAP_DATA_HPP
#define MAP_MAP_DATA_HPP

#include <JSystem/JGeometry.hpp>

class TLiveActor;
class JSUMemoryInputStream;

class TBGCheckData {
public:
	TBGCheckData();

	void getNormal() const;
	void isIllegalData() const;
	void getActiveJumpPower() const;
	void getPlaneType();
	void setVertex(const JGeometry::TVec3<float>&,
	               const JGeometry::TVec3<float>&,
	               const JGeometry::TVec3<float>&);
	void updateTrans(const JGeometry::TVec3<float>&);
	void isWaterSlip() const;
	void isMarioThrough() const;
	void isWaterSurface() const;

	// fabricated
	bool checkFlag(u32 flag) const { return unk0 & flag ? true : false; }
	bool checkFlag2(u32 flag) const { return unk4 & flag ? true : false; }
	const TLiveActor* getUnk44() { return unk44; }

public:
	/* 0x0 */ u16 unk0;
	/* 0x2 */ s16 unk2;
	/* 0x4 */ u16 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ JGeometry::TVec3<f32> unk34;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ const TLiveActor* unk44;
};

#endif
