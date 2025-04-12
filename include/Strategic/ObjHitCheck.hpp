#ifndef STRATEGIC_OBJ_HIT_CHECK_HPP
#define STRATEGIC_OBJ_HIT_CHECK_HPP

#include <JSystem/JGeometry.hpp>

class THitActor;
class TIdxGroupObj;

class TObjCheckList {
public:
	TObjCheckList();

public:
	/* 0x0 */ TObjCheckList* unk0;
	/* 0x4 */ THitActor* unk4;
};

class TObjHitCheck;

extern TObjHitCheck* gpObjHitCheck;

class TObjHitCheck {
public:
	TObjHitCheck();
	void initTable();
	void clearHitNum();
	void checkActorsHit();
	void checkGroup(TIdxGroupObj*);
	void checkGroupPlayer(TIdxGroupObj*);
	void clearGroup(TIdxGroupObj*);
	void entryGroup(TIdxGroupObj*);
	void checkAndEntryGroup(TIdxGroupObj*);
	void getTableIndex(const JGeometry::TVec3<f32>&, f32, u32*);
	void entryActor(THitActor*, TObjCheckList*);
	void checkWater();
	void checkWaterWithActorsInList(const JGeometry::TVec3<f32>&,
	                                TObjCheckList*);
	void checkActorsInList(THitActor*, TObjCheckList*);
	void suffererIsInAttackArea(THitActor*, THitActor*);

public:
	/* 0x0 */ TObjCheckList unk0[256];
	/* 0x800 */ TObjCheckList* unk800;
	/* 0x804 */ u32 unk804;
};

#endif
