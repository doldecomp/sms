#include <Player/MarioMain.hpp>
#include <Enemy/Conductor.hpp>
#include <System/MarDirector.hpp>
#include <NPC/NpcBase.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MoveBG/ModelGate.hpp>

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

BOOL TMario::warpIn()
{
	// Missing stack space
	// volatile u32 padding[15];
	mActionTimer += 1;
	const JGeometry::TVec3<f32>& gatePosOffset = ((TModelGate*)mHolder)->unkAC;
	JGeometry::TVec3<f32> holderPosOffset(((TModelGate*)mHolder)->unkAC);
	holderPosOffset.y -= 80.0f;
	switch (mActionState) {
	case 0:
		if (mActionTimer <= 1) {
			if (onYoshi() != FALSE) {
				getOffYoshi(true);
			}
			TModelGate* gate  = (TModelGate*)mHolder;
			u16 gateUnk72     = gate->unk72;
			Mtx* nodeMatrices = gate->unk78->getModel()->mNodeMatrices;
			unk45C.x          = nodeMatrices[gateUnk72][0][3] - gatePosOffset.x;
			unk45C.y          = nodeMatrices[gateUnk72][1][3] - gatePosOffset.y;
			unk45C.z          = nodeMatrices[gateUnk72][2][3] - gatePosOffset.z;
			unk45C.normalize();
			warpInLight();

			u8 nextStage   = 2;
			u8 destination = ((TModelGate*)mHolder)->unk71;
			switch (destination) {
			case 0:
				nextStage = 2;
				break;
			case 1:
				nextStage = 3;
				break;
			case 2:
				nextStage = 4;
				break;
			}
			gpMarDirector->setNextStage(nextStage, mHolder);
		}

		unk114 |= 2;
		J3DFrameCtrl* frameCtrl = getMotionFrameCtrl();
		frameCtrl->setSpeed(0.0f);

		// Possibly TVec3 inaccuracies?
		JGeometry::TVec3<f32> marioDist = holderPosOffset - mPosition;
		mPosition                       = marioDist * 0.02f + mPosition;

		f32 dist
		    = mAutoDemoParams.mWarpInTremble.get() - marioDist.length() * 0.1f;
		if (dist > 0.0f) {
			unk53C->clash(dist);
		}

		if (0x78 < mActionTimer) {
			mActionTimer = 0;
			warpInEffect();
			unk468       = 0.0f;
			mActionState = 1;
			// Probably some reserved sound?
			startVoice(-0x2);
		}
		break;
	case 1:
		if ((f32)mActionTimer > mAutoDemoParams.mWarpInBallsDispTime.get()) {
			unk114 &= ~(1 << 1);
			rumbleStart(0x15, 0x14);
		}
		if (mAutoDemoParams.mWarpInBallsTime.get() > (f32)mActionTimer) {
			mActionTimer = 0;
			unk468       = mAutoDemoParams.mWarpInVecBase.get();
			mActionState = 2;
		}

		break;
	case 2:
		unk114 &= ~(1 << 1);
		rumbleStart(0x14, mMotorParams.mMotorWall.get() / 2);

		if ((f32)mActionTimer > mAutoDemoParams.mWarpInCapturedTime.get()) {
			unk114 &= ~(1 << 1);
			mActionTimer = 0;
			mHolder->receiveMessage(this, 0xE);
			mActionState = 3;
		}

		break;
	case 3:
		unk118 |= MARIO_FLAG_IS_PERFORMING;
		break;
	}

	return FALSE;
}
