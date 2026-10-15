#include <Strategic/ObjHitCheck.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/Strategy.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/MarioAccess.hpp>
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

void TObjHitCheck::suffererIsInAttackArea(THitActor* attacker,
                                          THitActor* sufferer)
{
	if (attacker->mColCount >= attacker->mColCapacity)
		return;

	u32 i;
	for (i = 0; i < attacker->mColCount; ++i)
		if (attacker->mCollisions[i] == sufferer)
			return;

	attacker->mCollisions[attacker->mColCount] = sufferer;
	++attacker->mColCount;
}

void TObjHitCheck::checkActorsInList(THitActor* actor, TObjCheckList* list)
{
	while (list != nullptr) {
		THitActor* candidate = list->unk4;
		list                 = list->unk0;

		if (!actor->checkHitFilter(HIT_FILTER_NO_ATTACK)
		    && !candidate->checkHitFilter(HIT_FILTER_NO_DAMAGE)
		    && actor->canAttack(candidate)
		    && checkDistance(actor->mPosition, actor->getAttackRadius(),
		                     actor->getAttackHeight(), candidate->mPosition,
		                     candidate->getDamageRadius(),
		                     candidate->getDamageHeight())) {
			suffererIsInAttackArea(actor, candidate);
		}

		if (!candidate->checkHitFilter(HIT_FILTER_NO_ATTACK)
		    && !actor->checkHitFilter(HIT_FILTER_NO_DAMAGE)
		    && candidate->canAttack(actor)
		    && checkDistance(candidate->mPosition, candidate->getAttackRadius(),
		                     candidate->getAttackHeight(), actor->mPosition,
		                     actor->getDamageRadius(),
		                     actor->getDamageHeight())) {
			suffererIsInAttackArea(candidate, actor);
		}
	}
}

THitActor*
TObjHitCheck::checkWaterWithActorsInList(const JGeometry::TVec3<f32>& pos,
                                         TObjCheckList* list)
{
	while (list != nullptr) {
		THitActor* candidate = list->unk4;
		list                 = list->unk0;

		if (candidate->isHitCategory(HIT_CATEGORY_PLAYER)
		    || candidate->checkHitFilter(HIT_FILTER_NO_DAMAGE))
			continue;

		if (!checkDistance(
		        pos, TModelWaterManager::mStaticHitActor.getAttackRadius(),
		        TModelWaterManager::mStaticHitActor.getAttackHeight(),
		        candidate->mPosition, candidate->getDamageRadius(),
		        candidate->getDamageHeight()))
			continue;

		return candidate;
	}

	return nullptr;
}

#pragma dont_inline on
void TObjHitCheck::checkWater()
{
	f32 fVar2 = TModelWaterManager::mStaticHitActor.getEntryRadius();

	const JGeometry::TVec3<f32>* particlePositions
	    = gpModelWaterManager->getParticlePositions();
	THitActor** particleHitActors = gpModelWaterManager->getParticleUnk2514();

	for (int i = 0; i < gpModelWaterManager->getParticleCount(); ++i) {
		if (!gpModelWaterManager->checkFlagBottom4Bits(i, 0x1))
			continue;

		const JGeometry::TVec3<f32>& pos = particlePositions[i];

		u32 e;
		u32 j = getTableIndex(pos, fVar2, &e);

		TObjCheckList& list = unk0[j];

		if (j != e)
			particleHitActors[i] = checkWaterWithActorsInList(pos, list.unk0);
	}
}
#pragma dont_inline off

void TObjHitCheck::entryActor(THitActor* actor, TObjCheckList* head)
{
	if (!actor->checkHitFilter(HIT_FILTER_NO_ATTACK)
	    || !actor->checkHitFilter(HIT_FILTER_NO_DAMAGE)) {
		TObjCheckList* newList = &unk800[unk804];

		newList->unk4 = actor;
		newList->unk0 = head->unk0;
		head->unk0    = newList;

		unk804 += 1;
	}
}

u32 TObjHitCheck::getTableIndex(const JGeometry::TVec3<f32>& pos,
                                f32 entry_radius, u32* out)
{
	f32 fVar1 = abs(pos.x) + abs(pos.y) + abs(pos.z);

	u32 i = (fVar1 - entry_radius) * (10.f / 700.f);
	*out  = (fVar1 + entry_radius) * (10.f / 700.f);
	if (i == *out)
		*out += 1;
	*out &= 0xff;
	return i & 0xff;
}

void TObjHitCheck::checkAndEntryGroup(TIdxGroupObj* group)
{
	TIdxGroupObj::iterator end = group->getChildren().end();
	for (TIdxGroupObj::iterator it = group->getChildren().begin(); it != end;
	     ++it) {
		(*it)->mColCount = 0;

		if ((*it)->checkHitFilter(HIT_FILTER_NO_COLLISION))
			continue;

		u32 e;
		u32 i = getTableIndex((*it)->mPosition, (*it)->mEntryRadius, &e);

		while (i != e) {
			checkActorsInList(*it, unk0[i].unk0);

			entryActor(*it, &unk0[i]);

			if (i == 0xff)
				i = 0;
			else
				i += 1;
		}
	}
}

void TObjHitCheck::entryGroup(TIdxGroupObj* group)
{
	TIdxGroupObj::iterator end = group->getChildren().end();
	for (TIdxGroupObj::iterator it = group->getChildren().begin(); it != end;
	     ++it) {
		(*it)->mColCount = 0;

		if ((*it)->checkHitFilter(HIT_FILTER_NO_COLLISION))
			continue;

		u32 e;
		u32 i = getTableIndex((*it)->mPosition, (*it)->getEntryRadius(), &e);

		while (i != e) {
			entryActor(*it, &unk0[i]);

			if (i == 0xff)
				i = 0;
			else
				i += 1;
		}
	}
}

void TObjHitCheck::clearGroup(TIdxGroupObj* group)
{
	TIdxGroupObj::iterator end = group->getChildren().end();

	for (TIdxGroupObj::iterator it = group->getChildren().begin(); it != end;
	     ++it)
		(*it)->mColCount = 0;
}

void TObjHitCheck::checkGroupPlayer(TIdxGroupObj* group)
{
	TIdxGroupObj::iterator end = group->getChildren().end();
	THitActor* mario           = (THitActor*)gpMarioAddress;

	for (TIdxGroupObj::iterator it = group->getChildren().begin(); it != end;
	     ++it) {
		(*it)->mColCount = 0;
		if ((*it)->checkHitFilter(HIT_FILTER_NO_COLLISION))
			continue;

		if (checkDistance((*it)->mPosition, (*it)->getAttackRadius(),
		                  (*it)->getAttackHeight(), mario->mPosition,
		                  mario->getDamageRadius(), mario->getDamageHeight())) {
			suffererIsInAttackArea(*it, mario);
		}
	}
}

void TObjHitCheck::checkGroup(TIdxGroupObj* group) { }

void TObjHitCheck::checkActorsHit()
{
	initTable();

	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_OBJECT))
		entryGroup(gpStrategy->mGroups[IDX_GROUP_OBJECT]);
	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_ENEMY))
		checkAndEntryGroup(gpStrategy->mGroups[IDX_GROUP_ENEMY]);
	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_BOSS))
		checkAndEntryGroup(gpStrategy->mGroups[IDX_GROUP_BOSS]);
	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_NPC))
		checkAndEntryGroup(gpStrategy->mGroups[IDX_GROUP_NPC]);
	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_PLAYER))
		checkAndEntryGroup(gpStrategy->mGroups[IDX_GROUP_PLAYER]);

	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_WATER)
	    && gpModelWaterManager->askDoWaterHitCheck())
		checkWater();

	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_OBJECT))
		checkGroupPlayer(gpStrategy->mGroups[IDX_GROUP_ITEM]);
}

void TObjHitCheck::clearHitNum()
{
	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_ENEMY))
		clearGroup(gpStrategy->mGroups[IDX_GROUP_ENEMY]);
	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_BOSS))
		clearGroup(gpStrategy->mGroups[IDX_GROUP_BOSS]);
	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_NPC))
		clearGroup(gpStrategy->mGroups[IDX_GROUP_NPC]);
	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_PLAYER))
		clearGroup(gpStrategy->mGroups[IDX_GROUP_PLAYER]);
	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_WATER))
		clearGroup(gpStrategy->mGroups[IDX_GROUP_WATER_PARTICLE]);
	if (!gpStrategy->isHitCheckOff(HIT_CHECK_OFF_OBJECT))
		clearGroup(gpStrategy->mGroups[IDX_GROUP_ITEM]);
}

void TObjHitCheck::initTable()
{
	for (int i = 0; i < ARRAY_COUNT(unk0); ++i)
		unk0[i].unk0 = nullptr;

	unk804 = 0;
}

TObjHitCheck::TObjHitCheck()
{
	unk800 = new TObjCheckList[7000];
	unk804 = 0;
	initTable();
	gpObjHitCheck = this;
}

TObjCheckList::TObjCheckList()
    : unk0(nullptr)
    , unk4(nullptr)
{
}
