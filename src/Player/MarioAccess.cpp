#include <Player/MarioAccess.hpp>

#include <Strategic/LiveActor.hpp>
#include <Player/Mario.hpp>
#include <JSystem/JGeometry.hpp>

void* gpMarioAddress;
JGeometry::TVec3<f32>* gpMarioPos;
s16 *gpMarioAngleX, *gpMarioAngleY, *gpMarioAngleZ;
f32 *gpMarioSpeedX, *gpMarioSpeedY, *gpMarioSpeedZ;
s16* gpMarioLightID;
u32* gpMarioFlag;
f32* gpMarioThrowPower;
const TBGCheckData** gpMarioGroundPlane;

void SMS_SetMarioAccessParams()
{
	s16* angle;
	f32* speed;

	gpMarioAddress = gpMarioOriginal;
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
	gpMarioFlag    = &gpMarioOriginal->mFlag;

	gpMarioThrowPower  = &gpMarioOriginal->mDeParams.mThrowPower.value;
	gpMarioGroundPlane = &gpMarioOriginal->mGroundPlane;
}

TYoshi* SMS_GetYoshi() { return gpMarioOriginal->mYoshi; }

bool SMS_AskJumpIntoWaterEffectExist()
{
	return gpMarioOriginal->askJumpIntoWaterEffectExist();
}

THitActor* SMS_GetMarioHitActor() { return (THitActor*)gpMarioAddress; }

TLiveActor* SMS_GetMarioLiveActor() { return (TLiveActor*)gpMarioAddress; }

bool SMS_IsMarioStatusTypeJumping()
{
	return gpMarioOriginal->checkStatusFlag(MARIO_STATUS_FLAG_JUMPING);
}

bool SMS_IsMarioStatusTypeSwimming()
{
	return gpMarioOriginal->checkStatusFlag(MARIO_STATUS_FLAG_SWIMMING);
}

bool SMS_IsMarioStatusHipDrop()
{
	if (gpMarioOriginal->mStatus == MARIO_STATUS_HIP_DROP) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusThrownDown()
{
	if (gpMarioOriginal->mStatus == MARIO_STATUS_THROWN_DOWN) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusElecDamage()
{
	if (gpMarioOriginal->mStatus == MARIO_STATUS_ELECTRIC_DAMAGE) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioHeadSlideAttack()
{
	if (gpMarioOriginal->mStatus == MARIO_STATUS_CATCH
	    || gpMarioOriginal->mStatus == MARIO_STATUS_JUMP_CATCH) {
		return 1;
	} else {
		return 0;
	}
}

s16 SMS_GetMarioHP() { return gpMarioOriginal->mHealth; }

f32 SMS_GetMarioDamageRadius() { return gpMarioOriginal->mDamageRadius; }

f32 SMS_GetMarioGrLevel() { return gpMarioOriginal->mFloorPosition.y; }

f32 SMS_GetMarioGravity()
{
	return gpMarioOriginal->mJumpParams.mGravity.value;
}

TWaterGun* SMS_GetMarioWaterGun() { return gpMarioOriginal->mWaterGun; }

bool SMS_SendMessageToMario(THitActor* m, u32 mesg)
{
	if (gpMarioOriginal->receiveMessage(m, mesg))
		return true;

	return false;
}

void SMS_ThrowMario(const JGeometry::TVec3<float>& vec, float f)
{
	gpMarioOriginal->throwMario(vec, f);
}

bool SMS_IsMarioTouchGround4cm()
{
	if (gpMarioOriginal->isTouchGround4cm())
		return true;
	else
		return false;
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

bool SMS_IsMarioOpeningDoor()
{
	if (gpMarioOriginal->mStatus == MARIO_STATUS_DOOR_OPEN_R
	    || gpMarioOriginal->mStatus == MARIO_STATUS_DOOR_OPEN_L) {
		return true;
	} else {
		return false;
	}
}

bool SMS_IsMarioOnYoshi() { return gpMarioOriginal->onYoshi(); }

bool SMS_IsMarioDashing()
{
	return !!gpMarioOriginal->checkFlag(MARIO_FLAG_FLUDD_EMITTING);
}

void SMS_MarioMoveRequest(const JGeometry::TVec3<float>& vec)
{
	gpMarioOriginal->moveRequest(vec);
}

void SMS_MarioWarpRequest(const JGeometry::TVec3<float>& vec, float f)
{
	gpMarioOriginal->warpRequest(vec, f);
}

void SMS_FlowMoveMario(const JGeometry::TVec3<float>& vec)
{
	gpMarioOriginal->flowMove(vec);
}

void SMS_WindMoveMario(const JGeometry::TVec3<float>& vec)
{
	gpMarioOriginal->windMove(vec);
}

u32 SMS_GetMarioStatus() { return gpMarioOriginal->mStatus; }

const TBGCheckData* SMS_GetMarioGrPlane()
{
	return gpMarioOriginal->mGroundPlane;
}

const TBGCheckData* SMS_GetMarioWlPlane()
{
	return gpMarioOriginal->mWallPlane;
}

const TBGCheckData* SMS_GetMarioRfPlane()
{
	return gpMarioOriginal->mRoofPlane;
}

u32 SMS_GetMarioStatus(THitActor* actor) { return ((TMario*)actor)->mStatus; }

bool SMS_IsMarioFencing() { return gpMarioOriginal->isFencing(); }

bool SMS_IsMarioRoofing() { return gpMarioOriginal->isRoofing(); }
