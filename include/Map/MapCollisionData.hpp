#ifndef MAP_MAP_COLLISION_DATA_HPP
#define MAP_MAP_COLLISION_DATA_HPP

#include <JSystem/JGeometry.hpp>

class JSUMemoryInputStream;
class TBGCheckData;
class TBGCheckListRoot;
class TBGCheckList;

struct TBGWallCheckRecord {
	TBGWallCheckRecord(f32 x, f32 y, f32 z, f32 param_4, u32 param_5,
	                   u32 param_6)
	    : unk0(x, y, z)
	    , unkC(param_4)
	    , unk10(param_5)
	    , unk18(param_6)
	{
	}

	/* 0x0 */ JGeometry::TVec3<f32> unk0;
	/* 0xC */ f32 unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u32 unk18;
};

class TMapCollisionData {
public:
	TMapCollisionData();

	void polygonIsInGrid(f32, f32, f32, f32, TBGCheckData*);
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
	void getGridArea(const TBGCheckData*, int, int*, int*, int*, int*);
	void addCheckDataToList(int, int, int, int, TBGCheckData*);
	void getListRoot(int, int, int, int) const;
	void allocCheckList(int, int);
	void getEntryID();
	void allocCheckData(u32);

	static TBGCheckData mIllegalCheckData;

public:
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
};

#endif
