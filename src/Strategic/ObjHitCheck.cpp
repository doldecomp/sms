#include <Strategic/ObjHitCheck.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/Strategy.hpp>
#include <Player/ModelWaterManager.hpp>
#include <macros.h>

TObjHitCheck* gpObjHitCheck;

static bool checkDistance(const JGeometry::TVec3<f32>& param_1, f32 param_2,
                          f32 param_3, const JGeometry::TVec3<f32>& param_4,
                          f32 param_5, f32 param_6)
{
	if (param_1.y > param_4.y + param_6)
		return false;
	if (param_1.y + param_3 < param_4.y)
		return false;

	f32 fVar1 = param_2 + param_5;
	f32 dx    = param_1.x - param_4.x;
	f32 dz    = param_1.z - param_4.z;
	if (fVar1 * fVar1 <= dx * dx + dz * dz)
		return false;

	return true;
}

void TObjHitCheck::suffererIsInAttackArea(THitActor* param_1,
                                          THitActor* param_2)
{
	if (param_1->mColCount >= param_1->mColCapacity)
		return;

	u32 i;
	for (i = 0; i < param_1->mColCount; ++i)
		if (param_1->mCollisions[i] == param_2)
			return;

	param_1->mCollisions[param_1->mColCount] = param_2;
	++param_1->mColCount;
}

void TObjHitCheck::checkActorsInList(THitActor*, TObjCheckList*) { }

void TObjHitCheck::checkWaterWithActorsInList(const JGeometry::TVec3<f32>&,
                                              TObjCheckList*)
{
}

void TObjHitCheck::checkWater() { }

void TObjHitCheck::entryActor(THitActor*, TObjCheckList*) { }

void TObjHitCheck::getTableIndex(const JGeometry::TVec3<f32>&, f32, u32*) { }

void TObjHitCheck::checkAndEntryGroup(TIdxGroupObj*) { }

void TObjHitCheck::entryGroup(TIdxGroupObj* param_1) { }

void TObjHitCheck::clearGroup(TIdxGroupObj*) { }

void TObjHitCheck::checkGroupPlayer(TIdxGroupObj*) { }

void TObjHitCheck::checkGroup(TIdxGroupObj*) { }

void TObjHitCheck::checkActorsHit() { }

void TObjHitCheck::clearHitNum() { }

void TObjHitCheck::initTable() { }

TObjHitCheck::TObjHitCheck()
{
	unk800 = new TObjCheckList[7000];
	unk804 = 0;
	for (int i = 0; i < ARRAY_COUNT(unk0); ++i) {
		unk0[i].unk0 = nullptr;
	}
	unk804        = 0;
	gpObjHitCheck = this;
}

TObjCheckList::TObjCheckList()
    : unk0(nullptr)
    , unk4(nullptr)
{
}
