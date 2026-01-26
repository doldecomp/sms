#ifndef MAP_MAP_DATA_HPP
#define MAP_MAP_DATA_HPP

#include <JSystem/JGeometry.hpp>

class TLiveActor;
class JSUMemoryInputStream;

// fabricated
enum BGCheckFlagBits {
	BG_CHECK_FLAG_ILLEGAL = 0x10,
};

// fabricated
enum BGTypeBits {
	BG_TYPE_WET_GROUND = 0x4,

	BG_TYPE_WATER                        = 0x100,
	BG_TYPE_DAMAGING_WATER               = 0x101,
	BG_TYPE_SEA_WATER                    = 0x102,
	BG_TYPE_DAMAGING_SEA_WATER           = 0x103,
	BG_TYPE_POOL                         = 0x104,
	BG_TYPE_INDOOR_POOL                  = 0x105,
	BG_TYPE_UNDERGROUND_PATHWAY          = 0x106,
	BG_TYPE_GROUND_POUND_TO_PASS_THROUGH = 0x107,
	BG_TYPE_UNDERGROUND_SUPER_JUMP       = 0x108,
	BG_TYPE_INDOORS                      = 0x109,
	BG_TYPE_CLIMBABLE_FENCE              = 0x10A,

	BG_TYPE_ONLY_MARIO_PHASES_THROUGH  = 0x400,
	BG_TYPE_ONLY_WATER_PHASES_THROUGH  = 0x401,
	BG_TYPE_ONLY_ENEMIES_PHASE_THROUGH = 0x402,

	BG_TYPE_HYDROPHOBIC = 0x500,

	BG_TYPE_MAP_WARP                 = 0x200,
	BG_TYPE_MAP_WARP_PHASE_THROUGH   = 0x201,
	BG_TYPE_MAP_CHANGE               = 0x202,
	BG_TYPE_MAP_CHANGE_PHASE_THROUGH = 0x203,

	BG_TYPE_SAND = 0x701,

	BG_TYPE_DEATH_PLANE                              = 0x800,
	BG_TYPE_EVERYTHING_BUT_MAP_OBJECTS_PHASE_THROUGH = 0x801,

	BG_PROPERTY_FLAG_SHADOW           = 0x4000,
	BG_PROPERTY_FLAG_CAMERA_WONT_CLIP = 0x8000,

	BG_TYPE_SHADED_POOL = BG_TYPE_POOL | BG_PROPERTY_FLAG_SHADOW, // 0x4104
	BG_TYPE_SHADED_WET_GROUND
	= BG_TYPE_WET_GROUND | BG_PROPERTY_FLAG_SHADOW, // 0x4004

	BG_TYPE_SHADED_SAND = BG_TYPE_SAND | BG_PROPERTY_FLAG_SHADOW, // 0x4701
	BG_TYPE_CAM_NOCLIP_SAND
	= BG_TYPE_SAND | BG_PROPERTY_FLAG_CAMERA_WONT_CLIP, // 0x8701
	BG_TYPE_CAM_NOCLIP_SHADED_SAND
	= BG_TYPE_SAND | BG_PROPERTY_FLAG_SHADOW
	  | BG_PROPERTY_FLAG_CAMERA_WONT_CLIP, // 0xC701

	BG_TYPE_CAM_NOCLIP_ONLY_MARIO_PHASES_THROUGH // 0x8400
	= BG_TYPE_ONLY_MARIO_PHASES_THROUGH | BG_PROPERTY_FLAG_CAMERA_WONT_CLIP,
	BG_TYPE_CAM_NOCLIP_WET_GROUND
	= BG_TYPE_WET_GROUND | BG_PROPERTY_FLAG_CAMERA_WONT_CLIP, // 0x8004

	BG_TYPE_CAM_NOCLIP_SHADED_WET_GROUND
	= BG_TYPE_WET_GROUND | BG_PROPERTY_FLAG_SHADOW
	  | BG_PROPERTY_FLAG_CAMERA_WONT_CLIP, // 0xC004
};

/**
 * @brief A triangle of the collision mesh with additional metadata.
 */
class TBGCheckData {
public:
	TBGCheckData();

	const JGeometry::TVec3<f32>& getNormal() const { return mNormal; };
	bool isIllegalData() const
	{
		return mFlags & BG_CHECK_FLAG_ILLEGAL ? true : false;
	}
	f32 getActiveJumpPower() const;
	u32 getPlaneType();
	void setVertex(const JGeometry::TVec3<f32>& point1,
	               const JGeometry::TVec3<f32>& point2,
	               const JGeometry::TVec3<f32>& point3);
	void updateTrans(const JGeometry::TVec3<f32>& translate_by);
	bool isWaterSlip() const
	{
		if (mBGType == BG_TYPE_HYDROPHOBIC
		    || mBGType == BG_TYPE_SHADED_WET_GROUND
		    || mBGType == BG_TYPE_CAM_NOCLIP_WET_GROUND
		    || mBGType == BG_TYPE_CAM_NOCLIP_SHADED_WET_GROUND)
			return true;
		else
			return false;
	}
	bool isMarioThrough() const // matching
	{
		if (mBGType == BG_TYPE_ONLY_MARIO_PHASES_THROUGH || mBGType == 0x8400
		    || mBGType == BG_TYPE_WATER || mBGType == BG_TYPE_DAMAGING_WATER
		    || mBGType == BG_TYPE_SEA_WATER
		    || mBGType == BG_TYPE_DAMAGING_SEA_WATER
		    || mBGType == BG_TYPE_DEATH_PLANE
		    || mBGType == BG_TYPE_EVERYTHING_BUT_MAP_OBJECTS_PHASE_THROUGH
		    || mBGType == BG_TYPE_MAP_WARP_PHASE_THROUGH
		    || mBGType == BG_TYPE_MAP_CHANGE_PHASE_THROUGH)
			return true;
		return false;
	}
	bool isWaterSurface() const // matching
	{
		if (mBGType == BG_TYPE_WATER || mBGType == BG_TYPE_DAMAGING_WATER
		    || mBGType == BG_TYPE_SEA_WATER
		    || mBGType == BG_TYPE_DAMAGING_SEA_WATER || mBGType == BG_TYPE_POOL
		    || mBGType == BG_TYPE_INDOOR_POOL || mBGType == BG_TYPE_SHADED_POOL)
			return true;
		return false;
	}

	// fabricated
	bool checkFlag(u32 flag) const { return mFlags & flag ? true : false; }

	bool isLegal() const
	{
		return checkFlag(BG_CHECK_FLAG_ILLEGAL) == 1 ? false : true;
	}

	bool isSand() const
	{
		if (mBGType == BG_TYPE_SAND || mBGType == BG_TYPE_SHADED_SAND
		    || mBGType == BG_TYPE_CAM_NOCLIP_SAND
		    || mBGType == BG_TYPE_CAM_NOCLIP_SHADED_SAND)
			return true;
		else
			return false;
	}

	bool isPool() const
	{
		if (mBGType == BG_TYPE_POOL || mBGType == BG_TYPE_INDOOR_POOL
		    || mBGType == BG_TYPE_SHADED_POOL)
			return true;
		else
			return false;
	}

	bool isIndoors() const
	{
		if (mBGType == BG_TYPE_UNDERGROUND_PATHWAY
		    || mBGType == BG_TYPE_INDOOR_POOL
		    || mBGType == BG_TYPE_UNDERGROUND_SUPER_JUMP
		    || mBGType == BG_TYPE_INDOORS)
			return true;
		else
			return false;
	}

	bool isWarp() const
	{
		if (mBGType == BG_TYPE_MAP_WARP
		    || mBGType == BG_TYPE_MAP_WARP_PHASE_THROUGH)
			return true;
		else
			return false;
	}

	bool isMapChange() const
	{
		if (mBGType == BG_TYPE_MAP_CHANGE
		    || mBGType == BG_TYPE_MAP_CHANGE_PHASE_THROUGH)
			return true;
		else
			return false;
	}

	bool isDeathPlane() const
	{
		if (mBGType == BG_TYPE_DEATH_PLANE)
			return true;
		else
			return false;
	}
	bool checkSomething7() const
	{
		if (mBGType == BG_TYPE_EVERYTHING_BUT_MAP_OBJECTS_PHASE_THROUGH
		    || mBGType == BG_TYPE_ONLY_ENEMIES_PHASE_THROUGH)
			return true;
		else
			return false;
	}

	bool isWaterThrough() const
	{
		if (mBGType == BG_TYPE_ONLY_WATER_PHASES_THROUGH
		    || mBGType == BG_TYPE_EVERYTHING_BUT_MAP_OBJECTS_PHASE_THROUGH
		    || mBGType == BG_TYPE_CLIMBABLE_FENCE
		    || mBGType == BG_TYPE_CAM_NOCLIP_ONLY_MARIO_PHASES_THROUGH)
			return true;
		else
			return false;
	}

	bool isShadow() const
	{
		if (mBGType & BG_PROPERTY_FLAG_SHADOW)
			return true;
		else
			return false;
	}

	const JGeometry::TVec3<f32>& getPoint1() const { return mPoint1; }
	const JGeometry::TVec3<f32>& getPoint2() const { return mPoint2; }
	const JGeometry::TVec3<f32>& getPoint3() const { return mPoint3; }

	f32 getPlaneDistance() const { return mPlaneDistance; }

	// polymorphic parameter, means different things for each plane type
	s16 getData() const { return mData; }

	// the actor this collision is attached to, can be null
	const TLiveActor* getActor() const { return mActor; }

public:
	/* 0x0 */ u16 mBGType;
	/* 0x2 */ s16 mData;
	/* 0x4 */ u16 mFlags; // see PlaneFlagBits
	/* 0x6 */ u8 unk6;
	/* 0x7 */ u8 unk7;
	/* 0x8 */ f32 mMinY;
	/* 0xC */ f32 mMaxY;
	/* 0x10 */ JGeometry::TVec3<f32> mPoint1;
	/* 0x1C */ JGeometry::TVec3<f32> mPoint2;
	/* 0x28 */ JGeometry::TVec3<f32> mPoint3;
	/* 0x34 */ JGeometry::TVec3<f32> mNormal;
	/* 0x40 */ f32 mPlaneDistance;       // distance from plane to origin
	/* 0x44 */ const TLiveActor* mActor; // who we are attached to
};

#endif
