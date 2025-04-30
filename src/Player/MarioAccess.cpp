#include <Player/MarioAccess.hpp>

#include <Strategic/LiveActor.hpp>
#include <Player/MarioMain.hpp>
#include <JSystem/JGeometry.hpp>

TMario* gpMarioAddress;
JGeometry::TVec3<float>* gpMarioPos;
s16 *gpMarioAngleX, *gpMarioAngleY, *gpMarioAngleZ;
f32 *gpMarioSpeedX, *gpMarioSpeedY, *gpMarioSpeedZ;
u16* gpMarioLightID;
u32* gpMarioFlag;
f32* gpMarioThrowPower;
TBGCheckData** gpMarioGroundPlane;

bool SMS_IsMarioRoofing()
{
	bool ret;

	u32 val = gpMarioOriginal->action & 0x1FF;

	if (val >= 0x147 and val <= 0x14A) {
		ret = true;
	} else {
		ret = false;
	}

	return ret;
}

bool SMS_IsMarioFencing()
{
	bool ret;

	u32 val = gpMarioOriginal->action & 0x1FF;

	if (val >= 0x168 and val <= 0x16C) {
		ret = true;
	} else {
		ret = false;
	}

	return ret;
}

u32 SMS_GetMarioStatus(THitActor* actor) { return ((TMario*)actor)->action; }

TBGCheckData* SMS_GetMarioRfPlane() { return gpMarioOriginal->mRoofPlane; }

TBGCheckData* SMS_GetMarioWlPlane() { return gpMarioOriginal->mWallPlane; }

TBGCheckData* SMS_GetMarioGrPlane() { return gpMarioOriginal->mGroundPlane; }

u32 SMS_GetMarioStatus() { return gpMarioOriginal->action; }

void SMS_WindMoveMario(const JGeometry::TVec3<float>& vec)
{
	gpMarioOriginal->windMove(vec);
}

void SMS_FlowMoveMario(const JGeometry::TVec3<float>& vec)
{
	gpMarioOriginal->flowMove(vec);
}

void SMS_MarioWarpRequest(const JGeometry::TVec3<float>& vec, float f)
{
	gpMarioOriginal->warpRequest(vec, f);
}

void SMS_MarioMoveRequest(const JGeometry::TVec3<float>& vec) { }

u8 SMS_IsMarioDashing()
{
	u8 ret;

	if ((gpMarioOriginal->_118 & 0x4000) != 0) {
		ret = true;
	} else {
		ret = false;
	}

	return !!ret;
}

u32 SMS_IsMarioOnYoshi()
{
	u32 ret = gpMarioOriginal->onYoshi();

	return !!ret;
}

bool SMS_IsMarioOpeningDoor()
{
	if (gpMarioOriginal->action == 0x1320
	    || gpMarioOriginal->action == 0x1321) {
		return true;
	} else {
		return false;
	}
}

u8 SMS_IsMarioOnWire() { }

u8 SMS_IsMarioTouchGround4cm()
{
	u8 ret;

	if (gpMarioOriginal->mPosition.y
	    <= 4.0f + gpMarioOriginal->floorPosition.y) {
		ret = 1;
	} else {
		ret = 0;
	}

	if (ret)
		return 1;
	else
		return 0;
}

void SMS_ThrowMario(const JGeometry::TVec3<float>& vec, float f)
{
	gpMarioOriginal->throwMario(vec, f);
}

void SMS_SendMessageToMario(THitActor* m, u32 mesg) { }

void* SMS_GetMarioWaterGun()
{ // TODO: returns TWaterGun
	return gpMarioOriginal->waterGun;
}

f32 SMS_GetMarioGravity() { return gpMarioOriginal->jumpParams.mGravity.value; }

f32 SMS_GetMarioGrLevel() { return gpMarioOriginal->floorPosition.y; }

f32 SMS_GetMarioDamageRadius() { return gpMarioOriginal->mDamageRadius; }

s16 SMS_GetMarioHP() { return gpMarioOriginal->health; }

bool SMS_IsMarioHeadSlideAttack()
{
	if (gpMarioOriginal->action == 0x00800456
	    || gpMarioOriginal->action == 0x0080088A) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusElecDamage()
{
	if (gpMarioOriginal->action == 0x00020338) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusThrownDown()
{
	if (gpMarioOriginal->action == 0x000208B8) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusHipDrop()
{
	if (gpMarioOriginal->action == 0x008008A9) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusTypeSwimming()
{
	if (gpMarioOriginal->action & (1 << 13)) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusTypeJumping()
{
	if (gpMarioOriginal->action & (1 << 11)) {
		return 1;
	} else {
		return 0;
	}
}

TLiveActor* SMS_GetMarioLiveActor() { return (TLiveActor*)gpMarioAddress; }

THitActor* SMS_GetMarioHitActor() { return (THitActor*)gpMarioAddress; }

u32 SMS_AskJumpIntoWaterEffectExist() // bool?
{
	return gpMarioOriginal->askJumpIntoWaterEffectExist();
}

void* SMS_GetYoshi()
{ // TODO: returns TYoshi
	return gpMarioOriginal->yoshi;
}

void SMS_SetMarioAccessParams()
{
	s16* angle;
	f32* speed;

	gpMarioAddress = gpMarioOriginal;
	gpMarioPos     = &gpMarioOriginal->mPosition;

	angle         = &gpMarioOriginal->faceAngle.x;
	gpMarioAngleX = angle;
	gpMarioAngleY = angle + 1;
	gpMarioAngleZ = angle + 2;

	speed         = &gpMarioOriginal->vel.x;
	gpMarioSpeedX = speed;
	gpMarioSpeedY = speed + 1;
	gpMarioSpeedZ = speed + 2;

	gpMarioLightID = &gpMarioOriginal->lightID;
	gpMarioFlag    = &gpMarioOriginal->_118;

	gpMarioThrowPower  = &gpMarioOriginal->deParams.mThrowPower.value;
	gpMarioGroundPlane = &gpMarioOriginal->mGroundPlane;
}
