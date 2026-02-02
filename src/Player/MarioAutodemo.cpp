#include <Player/MarioMain.hpp>
#include <Enemy/Conductor.hpp>
#include <System/MarDirector.hpp>
#include <NPC/NpcBase.hpp>
#include <MarioUtil/MathUtil.hpp>

BOOL TMario::winDemo()
{
	switch (mActionState) {
	case 0:
		if (mHeldObject != nullptr) {
			mHeldObject->receiveMessage(mHeldObject, 0xD);
			mHeldObject = nullptr;
		}
		gpConductor->killEnemiesWithin(mPosition, 2000.0f);
		if (jumpProcess(0) == TRUE) {
			gpMarDirector->fireGetStar((TShine*)unk384);
			unk384->receiveMessage(this, 0x4);
			mActionState = 1;
		}
		break;
	case 1:
		setAnimation(0xCD, 1.0f);
		stopProcess();
		break;
	}

	return FALSE;
}

BOOL TMario::readBillboard()
{
	// Missing stack space
	// volatile u32 padding[16];
	TBaseNPC* talkingNpc = gpMarDirector->unkA0;
	switch (mActionState) {
	case 0:
		const JGeometry::TVec3<f32>& targetPos = talkingNpc->getPosition();
		f32 dx                                 = mPosition.x - targetPos.x;
		f32 dz                                 = mPosition.z - targetPos.z;
		if (dx == 0.0f && dz == 0.0f) {
			dx += 1.0f;
		}
		f32 dist = std::sqrtf(dx * dx + dz * dz);
		if (dist < 100.0f) {
			JGeometry::TVec3<f32> moveDist;
			moveDist.x = dx / dist * 2.0f * 50.0f + mPosition.x;
			moveDist.z = dz / dist * 2.0f * 50.0f + mPosition.z;
			moveDist.y = mFloorPosition.y;
			moveRequest(moveDist);
		}
		setAnimation(0xD9, 1.0f);
		mActionState = 1;
	case 1:
		s16 attackAngle = getAttackAngle(talkingNpc);
		s16 diffAngle   = attackAngle - mFaceAngle.y;
		s32 convAngle
		    = attackAngle
		      - IConverge(diffAngle, 0, mAutoDemoParams.mReadRotSp.get(),
		                  mAutoDemoParams.mReadRotSp.get());
		mFaceAngle.y = convAngle;
		if (attackAngle == mFaceAngle.y) {
			gpMarDirector->unk126 = 2;
			mActionState          = 2;
		}
		break;
	case 2:
		if (gpMarDirector->unk124 == 0 || gpMarDirector->unk124 == 5) {
			changePlayerStatus(0xC400201, 0, true);
		}
		break;
	}
	mFaceAngle.x    = 0;
	mModelFaceAngle = mFaceAngle.y;
	return FALSE;
}

BOOL TMario::jumpingDemoCommon(u32 playerStatus, int animationId, f32 velocity)
{
	setPlayerVelocity(velocity);
	setAnimation(animationId, 1.0f);
	if (jumpProcess(0) == TRUE) {
		changePlayerStatus(playerStatus, 0, true);
		return TRUE;
	}
	return FALSE;
}
