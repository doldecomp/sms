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
	f32 getActiveJumpPower() const;
	u32 getPlaneType();
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
	const TLiveActor* getUnk44() const { return unk44; }

	const JGeometry::TVec3<f32>& getPoint1() const { return mPoint1; }
	const JGeometry::TVec3<f32>& getPoint2() const { return mPoint2; }
	const JGeometry::TVec3<f32>& getPoint3() const { return mPoint3; }

public:
	/* 0x0 */ u16 unk0;
	/* 0x2 */ s16 unk2;
	/* 0x4 */ u16 unk4;
	/* 0x6 */ u8 unk6;
	/* 0x7 */ u8 unk7;
	/* 0x8 */ f32 mMinY;
	/* 0xC */ f32 mMaxY;
	/* 0x10 */ JGeometry::TVec3<f32> mPoint1;
	/* 0x10 */ JGeometry::TVec3<f32> mPoint2;
	/* 0x10 */ JGeometry::TVec3<f32> mPoint3;
	/* 0x34 */ JGeometry::TVec3<f32> mNormal;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ const TLiveActor* unk44;
};

#endif
