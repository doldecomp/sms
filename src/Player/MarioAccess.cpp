#include <Player/MarioAccess.hpp>

#include <Strategic/LiveActor.hpp>
#include <Player/MarioMain.hpp>
#include <JSystem/JGeometry.hpp>

size_t gpMarioAddress;
JGeometry::TVec3<f32>* gpMarioPos;
s16 *gpMarioAngleX, *gpMarioAngleY, *gpMarioAngleZ;
f32 *gpMarioSpeedX, *gpMarioSpeedY, *gpMarioSpeedZ;
u16* gpMarioLightID;
u32* gpMarioFlag;
f32* gpMarioThrowPower;
TBGCheckData** gpMarioGroundPlane;

bool SMS_IsMarioRoofing()
{
	bool ret;

	u32 val = gpMarioOriginal->mAction & 0x1FF;

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

	u32 val = gpMarioOriginal->mAction & 0x1FF;

	if (val >= 0x168 and val <= 0x16C) {
		ret = true;
	} else {
		ret = false;
	}

	return ret;
}

u32 SMS_GetMarioStatus(THitActor* actor) { return ((TMario*)actor)->mAction; }

TBGCheckData* SMS_GetMarioRfPlane() { return gpMarioOriginal->mRoofPlane; }

TBGCheckData* SMS_GetMarioWlPlane() { return gpMarioOriginal->mWallPlane; }

TBGCheckData* SMS_GetMarioGrPlane() { return gpMarioOriginal->mGroundPlane; }

u32 SMS_GetMarioStatus() { return gpMarioOriginal->mAction; }

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

void SMS_MarioMoveRequest(const JGeometry::TVec3<float>& vec)
{
	gpMarioOriginal->moveRequest(vec);
}

bool SMS_IsMarioDashing()
{
	bool ret;

	if ((gpMarioOriginal->unk118 & 0x4000) != 0) {
		ret = true;
	} else {
		ret = false;
	}

	return !!ret;
}

bool SMS_IsMarioOnYoshi() { return gpMarioOriginal->onYoshi(); }

bool SMS_IsMarioOpeningDoor()
{
	if (gpMarioOriginal->mAction == 0x1320
	    || gpMarioOriginal->mAction == 0x1321) {
		return true;
	} else {
		return false;
	}
}

bool SMS_IsMarioOnWire()
{
	bool ret;
	if (gpMarioOriginal->mHolder
	    && gpMarioOriginal->mHolder->mActorType == 0x40000098)
		ret = true;
	else
		ret = false;

	return !!ret;
}

bool SMS_IsMarioTouchGround4cm()
{
	bool ret;

	if (gpMarioOriginal->mPosition.y
	    <= 4.0f + gpMarioOriginal->mFloorPosition.y) {
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

bool SMS_SendMessageToMario(THitActor* m, u32 mesg)
{
	if (gpMarioOriginal->receiveMessage(m, mesg))
		return true;

	return false;
}

void* SMS_GetMarioWaterGun()
{ // TODO: returns TWaterGun
	return gpMarioOriginal->mWaterGun;
}

f32 SMS_GetMarioGravity()
{
	return gpMarioOriginal->mJumpParams.mGravity.value;
}

f32 SMS_GetMarioGrLevel() { return gpMarioOriginal->mFloorPosition.y; }

f32 SMS_GetMarioDamageRadius() { return gpMarioOriginal->mDamageRadius; }

s16 SMS_GetMarioHP() { return gpMarioOriginal->mHealth; }

bool SMS_IsMarioHeadSlideAttack()
{
	if (gpMarioOriginal->mAction == 0x00800456
	    || gpMarioOriginal->mAction == 0x0080088A) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusElecDamage()
{
	if (gpMarioOriginal->mAction == 0x00020338) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusThrownDown()
{
	if (gpMarioOriginal->mAction == 0x000208B8) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusHipDrop()
{
	if (gpMarioOriginal->mAction == 0x008008A9) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusTypeSwimming()
{
	if (gpMarioOriginal->mAction & (1 << 13)) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusTypeJumping()
{
	if (gpMarioOriginal->mAction & (1 << 11)) {
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
	return gpMarioOriginal->mYoshi;
}

void SMS_SetMarioAccessParams()
{
	s16* angle;
	f32* speed;

	gpMarioAddress = (size_t)gpMarioOriginal;
	gpMarioPos     = &gpMarioOriginal->mPosition;

	angle         = &gpMarioOriginal->mFaceAngle.x;
	gpMarioAngleX = angle;
	gpMarioAngleY = angle + 1;
	gpMarioAngleZ = angle + 2;

	speed         = &gpMarioOriginal->mVel.x;
	gpMarioSpeedX = speed;
	gpMarioSpeedY = speed + 1;
	gpMarioSpeedZ = speed + 2;

	gpMarioLightID = &gpMarioOriginal->mLightID;
	gpMarioFlag    = &gpMarioOriginal->unk118;

	gpMarioThrowPower  = &gpMarioOriginal->mDeParams.mThrowPower.value;
	gpMarioGroundPlane = &gpMarioOriginal->mGroundPlane;
}
