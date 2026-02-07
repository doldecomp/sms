#ifndef PLAYER_MARIO_ACCESS_HPP
#define PLAYER_MARIO_ACCESS_HPP

#include <JSystem/JGeometry.hpp>
#include <stddef.h>

class TLiveActor;
class THitActor;
class TBGCheckData;

extern size_t gpMarioAddress;

extern JGeometry::TVec3<f32>* gpMarioPos;

extern s16 *gpMarioAngleX, *gpMarioAngleY, *gpMarioAngleZ;
extern f32 *gpMarioSpeedX, *gpMarioSpeedY, *gpMarioSpeedZ;

extern u16* gpMarioLightID;
extern u32* gpMarioFlag;
extern f32* gpMarioThrowPower;
extern TBGCheckData** gpMarioGroundPlane;

inline bool SMS_CheckMarioFlag(u32 flag)
{
	return (*gpMarioFlag & flag) ? true : false;
}

bool SMS_IsMarioRoofing();
bool SMS_IsMarioFencing();
void SMS_GetMarioSpeedY(THitActor*);
void SMS_IsMarioTouchGround4cm(THitActor*);
u32 SMS_GetMarioStatus(THitActor*);
TBGCheckData* SMS_GetMarioRfPlane();
TBGCheckData* SMS_GetMarioWlPlane();
TBGCheckData* SMS_GetMarioGrPlane();
u32 SMS_GetMarioStatus();
void SMS_GetMarioWork(int);
void SMS_WindMoveMario(const JGeometry::TVec3<f32>&);
void SMS_FlowMoveMario(const JGeometry::TVec3<f32>&);
void SMS_MarioWarpRequest(const JGeometry::TVec3<f32>&, f32);
void SMS_MarioMoveRequest(const JGeometry::TVec3<f32>&);

bool SMS_IsMarioDashing();
bool SMS_IsMarioOnYoshi();
bool SMS_IsMarioOpeningDoor();
bool SMS_IsMarioOnWire();
void SMS_IsMarioSpeedZero();
bool SMS_IsMarioTouchGround4cm();

void SMS_ThrowMario(const JGeometry::TVec3<f32>&, f32);
bool SMS_SendMessageToMario(THitActor*, u32);
void SMS_GetMarioJumpIntoWaterModelData();
void* SMS_GetMarioWaterGun();

f32 SMS_GetMarioGravity();
f32 SMS_GetMarioGrLevel();
f32 SMS_GetMarioDamageRadius();
s16 SMS_GetMarioHPMax();
s16 SMS_GetMarioHP();

bool SMS_IsMarioHeadSlideAttack();
bool SMS_IsMarioWearingAloha();
bool SMS_IsMarioNoCap();
bool SMS_IsStatusHipDropOrHipDropEnd(THitActor*);
bool SMS_IsMarioStatusElecDamage();
bool SMS_IsMarioStatusThrownDown();
bool SMS_IsMarioStatusHipDropEnd();
bool SMS_IsMarioStatusHipDrop();
bool SMS_IsMarioStatusTypeSwimming();
bool SMS_IsMarioStatusTypeJumping();
bool SMS_IsMarioStatusWaiting();

TLiveActor* SMS_GetMarioLiveActor();
THitActor* SMS_GetMarioHitActor();
u32 SMS_AskJumpIntoWaterEffectExist();
void* SMS_GetYoshi();
void SMS_SetMarioAccessParams();

// Real, see bossgesso
inline JGeometry::TVec3<f32>& SMS_GetMarioPos() { return *gpMarioPos; }

JGeometry::TVec3<f32> SMS_DistanceFromMarioVec(const JGeometry::TVec3<f32>& pos)
{
	JGeometry::TVec3<f32> marioPos = SMS_GetMarioPos();
	marioPos.sub(pos);
	return marioPos;
}

f32 SMS_DistanceFromMario(const JGeometry::TVec3<f32>& pos)
{
	return SMS_DistanceFromMarioVec(pos).length();
}

// fabricated and very unlikely to be real
// TODO: removeme
inline void SMS_GetMarioPosStupid(Vec* result)
{
	result->x = 0;
	result->y = 0;
	result->z = 0;

	if (!gpMarioAddress)
		return;

	result->z = *(f32*)(gpMarioAddress + 0x18);
	result->y = *(f32*)(gpMarioAddress + 0x14);
	result->x = *(f32*)(gpMarioAddress + 0x10);
}

// fabricated
inline const TBGCheckData* SMS_GetMarioGroundPlane()
{
	return *gpMarioGroundPlane;
}

#endif
