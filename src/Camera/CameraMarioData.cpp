#include <Camera/CameraMarioData.hpp>
#include <Strategic/LiveActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/MapData.hpp>
#include <Camera/cameralib.hpp>

TCameraMarioData* gpCameraMario;

TCameraMarioData::TCameraMarioData()
{
	unk0.x                        = 0.0f;
	unk0.y                        = 0.0f;
	unk0.z                        = 0.0f;
	mFrameMoveDistHorizontal      = 0.0f;
	mFrameMoveDistVertical        = 0.0f;
	mLastMarioStatus              = 0;
	mFramesSinceMarioStatusChange = 0;
	unk1C                         = 0.0f;
}

void TCameraMarioData::calcAndSetMarioData()
{
	int status = SMS_GetMarioStatus();
	switch (status) {
	case MARIO_STATUS_HANGING:
	case MARIO_STATUS_ASCEND:
		mFrameMoveDistHorizontal = 0.0f;
		mFrameMoveDistVertical   = 0.0f;
		break;

	default:
		JGeometry::TVec3<f32> offset;
		offset.sub(SMS_GetMarioPos(), gpMarioOriginal->getPrevPosition());
		mFrameMoveDistHorizontal = offset.x * offset.x + offset.z * offset.z;
		mFrameMoveDistVertical   = offset.y * offset.y;
		if (mFrameMoveDistHorizontal > 100.0f)
			mFrameMoveDistHorizontal = 100.0f;
		if (mFrameMoveDistVertical > 100.0f)
			mFrameMoveDistVertical = 100.0f;
		break;
	}

	if (mLastMarioStatus != status) {
		mLastMarioStatus              = status;
		mFramesSinceMarioStatusChange = 0;
	} else {
		mFramesSinceMarioStatusChange += 1;
	}

	s16 angleMin = SMS_GetMarioWaterGun()->getEmitParams().mLAngleMin.get();
	s16 angle    = ((const TWaterGun*)SMS_GetMarioWaterGun())
	                ->getCurrentNozzle()
	                ->getGunAngle();
	unk1C = CLBCalcRatio<s16>(0, angleMin, angle);
	unk1C = MsClamp(unk1C, 0.0f, 1.0f);
}

bool TCameraMarioData::isMarioGoDown() const
{
	bool result = false;
	if (mFrameMoveDistVertical != 0.0f
	    && gpMarioPos->y - gpMarioOriginal->unk29C.y < 0.0f)
		result = true;
	return result;
}

bool TCameraMarioData::isMarioRocketing() const
{
	bool result = false;
	switch (SMS_GetMarioStatus()) {
	case MARIO_STATUS_ROCKET_LANDING:
	case MARIO_STATUS_ROCKET:
		result = true;
		break;
	}
	return result;
}

bool TCameraMarioData::isMarioIndoor() const
{
	bool result = false;
	if (SMS_GetMarioGrPlane())
		result = SMS_GetMarioGrPlane()->isIndoors();
	return result;
}

bool TCameraMarioData::isMarioSlider() const
{
	bool result = false;
	if (SMS_GetMarioGrPlane())
		result = SMS_GetMarioGrPlane()->isSlider();
	return result;
}

bool TCameraMarioData::isMarioLeanMirror() const
{
	bool result = false;
	if (SMS_GetMarioGrPlane()) {
		const TLiveActor* actor = SMS_GetMarioGrPlane()->getActor();
		if (actor != nullptr && actor->getActorType() == 0x400000CF)
			result = true;
	}
	return result;
}

bool TCameraMarioData::isMarioBathtub() const { }

bool TCameraMarioData::isMarioDoorDemoStart() const { }

bool TCameraMarioData::isMarioDoorDemoEnd() const { }

bool TCameraMarioData::isMarioClimb(u32 status) const
{
	bool result = false;
	switch (status) {
	case MARIO_STATUS_BAR_WAIT:
	case MARIO_STATUS_BAR_HANG:
	case 0x10100342:
	case MARIO_STATUS_BAR_CLIMB:
		result = true;
		break;
	}
	return result;
}
