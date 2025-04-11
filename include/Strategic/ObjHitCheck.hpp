#ifndef STRATEGIC_OBJ_HIT_CHECK_HPP
#define STRATEGIC_OBJ_HIT_CHECK_HPP

#include <JSystem/JGeometry.hpp>

class THitActor;
class TIdxGroupObj;
class TObjCheckList;

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
};

#endif
