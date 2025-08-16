#ifndef MAP_MAP_COLLISION_DATA_HPP
#define MAP_MAP_COLLISION_DATA_HPP

#include <JSystem/JGeometry.hpp>

class JSUMemoryInputStream;
class TBGCheckData;
class TMapCheckGroundPlane;

class TBGCheckList {
public:
	TBGCheckList();

	virtual void setPreNode(TBGCheckList*) { }

	// fabricated
	TBGCheckList* getNext() { return mNext; }
	const TBGCheckList* getNext() const { return mNext; }
	void setNext(TBGCheckList* v) { mNext = v; }

public:
	/* 0x4 */ TBGCheckList* mNext;
	/* 0x8 */ TBGCheckData* unk8;
};

class TBGCheckListWarp : public TBGCheckList {
public:
	TBGCheckListWarp();

	virtual void setPreNode(TBGCheckList* node)
	{
		// TODO: maybe unkC is TBGCheckList*?
		unkC = (TBGCheckListWarp*)node;
	}

	// fabricated
	TBGCheckListWarp* getPreNode() { return unkC; }

public:
	/* 0xC */ TBGCheckListWarp* unkC;
	/* 0x10 */ s16 unk10;
	/* 0x12 */ s16 unk12;
};

class TBGCheckListRoot {
public:
	TBGCheckListRoot() { }

	// fabricated
	const TBGCheckList* getWallList() const { return unk0[2].getNext(); }
	const TBGCheckList* getRoofList() const { return unk0[1].getNext(); }

public:
	/* 0x0 */ TBGCheckList unk0[3];
};

/**
 * @brief Temporary struct used for querying for collisions with walls.
 * Given a sphere of radius mRadius located at mCenter, finds up to 4
 * walls (triangles) that intersect with it while respecting ignore flags
 * and returns the intersecting walls via mResultWalls and mResultWallsNum.
 * By default, also modifies X and Z of  mCenter to "move" the sphere outside of
 * each returned wall. This is done in-order and only guarantees that the LAST
 * of the walls returned will not intersect with the sphere if placed at the
 * updated position. Walls other than the last one MIGHT intersect it.
 */
struct TBGWallCheckRecord {
	enum {
		IGNORE_WATER_SURFACE = 0x1,
		DONT_MOVE_XZ         = 0x2,
		IGNORE_WATER_THROUGH = 0x4,
	};

	// TODO: the constructors are 99% wrong, need to analyze a bunch of
	// use-cases. Maybe this was a plain old struct with no methods?

	// fabricated
	TBGWallCheckRecord() { }

	// fabricated
	TBGWallCheckRecord(f32 x, f32 y, f32 z, f32 radius, u32 max_results,
	                   u32 flags)
	    : mCenter(x, y, z)
	    , mRadius(radius)
	    , mMaxResults(max_results)
	    , mFlags(flags)
	{
	}

	// fabricated
	TBGWallCheckRecord(const JGeometry::TVec3<f32>& center, f32 radius,
	                   u32 max_results, u32 flags)
	    : mCenter(center)
	    , mRadius(radius)
	    , mMaxResults(max_results)
	    , mFlags(flags)
	{
	}

	// fabricated
	void set(f32 x, f32 y, f32 z, f32 radius, u32 max_results, u32 flags)
	{
		mCenter.set(x, y, z);
		mRadius     = radius;
		mMaxResults = max_results;
		mFlags      = flags;
	}

	/* 0x0 */ JGeometry::TVec3<f32> mCenter;
	/* 0xC */ f32 mRadius;
	/* 0x10 */ int mMaxResults;
	/* 0x14 */ int mResultWallsNum;
	/* 0x18 */ u32 mFlags;
	/* 0x1C */ TBGCheckData* mResultWalls[4];
};

class TMapCollisionData;

extern TMapCollisionData* gpMapCollisionData;

class TMapCollisionData {
public:
	enum {
		IGNORE_WATER_SURFACE = 0x1,
		IGNORE_WATER_THROUGH = 0x4,
	};

	// fabricated
	static TMapCollisionData* getInstance() { return gpMapCollisionData; }

	TMapCollisionData();

	bool polygonIsInGrid(f32, f32, f32, f32, TBGCheckData*);
	const TBGCheckData* intersectLine(const JGeometry::TVec3<f32>&,
	                                  const JGeometry::TVec3<f32>&, bool,
	                                  JGeometry::TVec3<f32>*) const;

	f32 checkGround(f32 x, f32 y, f32 z, u8 flags,
	                const TBGCheckData** result) const;
	static f32 checkGroundList(f32 x, f32 y, f32 z, u8 flags,
	                           const TBGCheckList*,
	                           const TBGCheckData** result);

	f32 checkRoof(f32 x, f32 y, f32 z, u8 flags,
	              const TBGCheckData** result) const;
	static f32 checkRoofList(f32 x, f32 y, f32 z, u8 flags, const TBGCheckList*,
	                         const TBGCheckData** result);

	int checkWalls(TBGWallCheckRecord*) const;
	static int checkWallList(const TBGCheckList*, TBGWallCheckRecord*);

	void init(JSUMemoryInputStream&);
	void initAllCheckDataAndList();
	void initMoveCollision();
	void initGrid(TBGCheckListRoot*);

	void removeCheckListData(u16, s32);
	void updateCheckListNode(s32, s32, s32);
	void removeCheckListNode(s32, s32);

	void addCheckDataToGrid(TBGCheckData*, int);
	bool getGridArea(const TBGCheckData*, int, int*, int*, int*, int*);
	void addCheckDataToList(int, int, int, int, TBGCheckData*);
	TBGCheckList* getListRoot(int, int, int, int) const;

	TBGCheckList* allocCheckList(int type, int count);
	u32 getEntryID();
	TBGCheckData* allocCheckData(u32 count);

	static TBGCheckData mIllegalCheckData;

	// fabricated
	u32 getEntryRelatedThing(u16 n) const { return unk40 - unk42[n]; }
	const TBGCheckListRoot& getGridRoot14(int x, int z) const
	{
		return unk14[x + z * unk8];
	}
	const TBGCheckListRoot& getGridRoot18(int x, int z) const
	{
		return unk18[x + z * unk8];
	}
	void setGroundPlane(TMapCheckGroundPlane* plane) { mGroundPlane = plane; }

public:
	/* 0x0 */ f32 mGridExtentX;
	/* 0x4 */ f32 mGridExtentY;
	/* 0x8 */ int unk8;
	/* 0xC */ int unkC;
	/* 0x10 */ int unk10;
	/* 0x14 */ TBGCheckListRoot* unk14;
	/* 0x18 */ TBGCheckListRoot* unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ TBGCheckData* unk28;
	/* 0x2C */ TBGCheckList* unk2C;
	/* 0x30 */ TBGCheckListWarp* unk30;
	/* 0x34 */ u32 unk34;
	/* 0x38 */ u32 unk38;
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ u16 unk40;
	/* 0x42 */ u16 unk42[256];
	/* 0x242 */ u16 unk242;
	/* 0x244 */ TMapCheckGroundPlane* mGroundPlane;
};

#endif
