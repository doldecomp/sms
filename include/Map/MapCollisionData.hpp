#ifndef MAP_MAP_COLLISION_DATA_HPP
#define MAP_MAP_COLLISION_DATA_HPP

#include <JSystem/JGeometry.hpp>

class JSUMemoryInputStream;
class TBGCheckData;
class TBGCheckListRoot;
class TBGCheckList;
class TBGWallCheckRecord;

class TMapCollisionData {
public:
	TMapCollisionData();

	void polygonIsInGrid(f32, f32, f32, f32, TBGCheckData*);
	void intersectLine(const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec3<f32>&, bool,
	                   JGeometry::TVec3<f32>*) const;
	void checkGround(f32, f32, f32, u8, const TBGCheckData**) const;
	void checkGroundList(f32, f32, f32, u8, const TBGCheckList*,
	                     const TBGCheckData**);
	void checkRoof(f32, f32, f32, u8, const TBGCheckData**) const;
	void checkRoofList(f32, f32, f32, u8, const TBGCheckList*,
	                   const TBGCheckData**);
	void checkWalls(TBGWallCheckRecord*) const;
	void checkWallList(const TBGCheckList*, TBGWallCheckRecord*);
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
};

#endif
