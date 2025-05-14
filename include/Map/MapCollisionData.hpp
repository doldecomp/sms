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

public:
	/* 0x0 */ TBGCheckList unk0[3];
};

struct TBGWallCheckRecord {
	// fabricated
	TBGWallCheckRecord(f32 x, f32 y, f32 z, f32 param_4, u32 param_5,
	                   u32 param_6)
	    : unk0(x, y, z)
	    , unkC(param_4)
	    , unk10(param_5)
	    , unk18(param_6)
	{
	}
	// fabricated
	TBGWallCheckRecord(const JGeometry::TVec3<f32>& param_1, f32 param_2,
	                   u32 param_3, u32 param_4)
	    : unk0(param_1)
	    , unkC(param_2)
	    , unk10(param_3)
	    , unk18(param_4)
	{
	}

	/* 0x0 */ JGeometry::TVec3<f32> unk0;
	/* 0xC */ f32 unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ TBGCheckData* unk1C;
};

class TMapCollisionData;

extern TMapCollisionData* gpMapCollisionData;

class TMapCollisionData {
public:
	// fabricated
	static TMapCollisionData* getInstance() { return gpMapCollisionData; }

	TMapCollisionData();

	bool polygonIsInGrid(f32, f32, f32, f32, TBGCheckData*);
	void intersectLine(const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec3<f32>&, bool,
	                   JGeometry::TVec3<f32>*) const;

	f32 checkGround(f32, f32, f32, u8, const TBGCheckData**) const;
	void checkGroundList(f32, f32, f32, u8, const TBGCheckList*,
	                     const TBGCheckData**);
	f32 checkRoof(f32, f32, f32, u8, const TBGCheckData**) const;
	void checkRoofList(f32, f32, f32, u8, const TBGCheckList*,
	                   const TBGCheckData**);
	int checkWalls(TBGWallCheckRecord*) const;
	int checkWallList(const TBGCheckList*, TBGWallCheckRecord*);

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

public:
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
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
	/* 0x244 */ TMapCheckGroundPlane* unk244;
};

#endif
