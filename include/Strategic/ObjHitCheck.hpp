#ifndef STRATEGIC_OBJ_HIT_CHECK_HPP
#define STRATEGIC_OBJ_HIT_CHECK_HPP

#include <JSystem/JGeometry.hpp>

class THitActor;
class TIdxGroupObj;

class TObjCheckList {
public:
	TObjCheckList();

	TObjCheckList* getNext() { return unk0; }
	THitActor* getActor() { return unk4; }

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
	u32 getTableIndex(const JGeometry::TVec3<f32>&, f32, u32*);
	void entryActor(THitActor*, TObjCheckList*);
	void checkWater();
	THitActor* checkWaterWithActorsInList(const JGeometry::TVec3<f32>&,
	                                      TObjCheckList*);
	void checkActorsInList(THitActor*, TObjCheckList*);
	void suffererIsInAttackArea(THitActor*, THitActor*);

	// fabricated
	TObjCheckList* getCheckList(u32 index) { return &unk0[index]; }

	TObjCheckList* getCheckList2(u32 index) { return unk0[index].unk0; }

public:
	/* 0x0 */ TObjCheckList unk0[256];
	/* 0x800 */ TObjCheckList* unk800;
	/* 0x804 */ u32 unk804;
};

#endif
