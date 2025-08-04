#ifndef MAP_MAP_DATA_HPP
#define MAP_MAP_DATA_HPP

#include <JSystem/JGeometry.hpp>

class TLiveActor;
class JSUMemoryInputStream;

// fabricated
enum PlaneFlagBits {
	PLANE_TYPE_KILL = 0x10,
};

class TBGCheckData {
public:
	TBGCheckData();

	const JGeometry::TVec3<f32>& getNormal() const { return mNormal; };
	bool isIllegalData() const { return unk4 & 0x10 ? true : false; }
	f32 getActiveJumpPower() const;
	u32 getPlaneType();
	void setVertex(const JGeometry::TVec3<f32>& point1,
	               const JGeometry::TVec3<f32>& point2,
	               const JGeometry::TVec3<f32>& point3);
	void updateTrans(const JGeometry::TVec3<f32>& translate_by);
	bool isWaterSlip() const
	{
		if (unk0 == 0x500 || unk0 == 0x4004 || unk0 == 0x8004 || unk0 == 0xC004)
			return true;
		return false;
	}
	bool isMarioThrough() const // matching
	{
		if (unk0 == 0x400 || unk0 == 0x8400 || unk0 == 0x100 || unk0 == 0x101
		    || unk0 == 0x102 || unk0 == 0x103 || unk0 == 0x800 || unk0 == 0x801
		    || unk0 == 0x201 || unk0 == 0x203)
			return true;
		return false;
	}
	bool isWaterSurface() const // matching
	{
		if (unk0 == 0x100 || unk0 == 0x101 || unk0 == 0x102 || unk0 == 0x103
		    || unk0 == 0x104 || unk0 == 0x105 || unk0 == 0x4104)
			return true;
		return false;
	}

	// fabricated
	bool checkFlag(u32 flag) const { return unk0 & flag ? true : false; }
	bool checkFlag2(u32 flag) const { return unk4 & flag ? true : false; }
	const TLiveActor* getUnk44() const { return unk44; }

	bool checkSomething2() const
	{
		if (unk0 == 0x104 || unk0 == 0x105 || unk0 == 0x4104)
			return true;
		else
			return false;
	}

	bool checkSomething3() const
	{
		if (unk0 == 0x200 || unk0 == 0x201)
			return true;
		else
			return false;
	}

	bool checkSomething4() const
	{
		if (unk0 == 0x202 || unk0 == 0x203)
			return true;
		else
			return false;
	}

	bool checkSomething6() const
	{
		if (unk0 == 0x800)
			return true;
		else
			return false;
	}

	bool checkSomething5() const
	{
		return !(checkFlag2(0x10) == 1 ? false : true);
	}

	bool checkSomething7() const
	{
		if (unk0 == 0x801 || unk0 == 0x402)
			return true;
		return false;
	}

	const JGeometry::TVec3<f32>& getPoint1() const { return mPoint1; }
	const JGeometry::TVec3<f32>& getPoint2() const { return mPoint2; }
	const JGeometry::TVec3<f32>& getPoint3() const { return mPoint3; }

public:
	/* 0x0 */ u16 unk0;
	/* 0x2 */ s16 unk2;
	/* 0x4 */ u16 unk4; // see PlaneFlagBits
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
