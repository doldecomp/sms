#include <Player/MarioMain.hpp>
#include <Enemy/Conductor.hpp>
#include <System/MarDirector.hpp>
#include <NPC/NpcBase.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MoveBG/ModelGate.hpp>
#include <System/StageUtil.hpp>
#include <Player/Watergun.hpp>
#include <Player/MarioCap.hpp>
#include <JSystem/JMath.hpp>

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

BOOL TMario::isUnUsualStageStart()
{
	// Missing stack space
	// volatile u32 padding[14];

	// Pinna rollercoaster
	if ((gpMarDirector->mMap == 0x3A)
	    && (gpMarDirector->unk7D == 0 || gpMarDirector->unk7D == 1)) {
		changePlayerStatus(0x800447, 0, true);
		unk114 |= 2;
		if (unk3FC != nullptr) {
			unk3FC->setBckFromIndex(0);
			unk3FC->getFrameCtrl(0)->setSpeed(0.5f);
			unk3FC->getFrameCtrl(0)->setFrame(0.0f);
		}
		if (unk400 != nullptr) {
			unk400->setBckFromIndex(0);
			unk400->getFrameCtrl(0)->setSpeed(0.5f);
			unk400->getFrameCtrl(0)->setFrame(0.0f);
		}
		return TRUE;
	}

	if (SMS_isDivingMap()) {
		unk114 |= 2;

		// I suspect some inline stuff here, weird to check right after you set
		// it
		unk118 |= MARIO_FLAG_HELMET_FLW_CAMERA;
		unk118 |= MARIO_FLAG_HELMET;
		unk118 |= MARIO_FLAG_HAS_FLUDD;

		if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
			mWaterGun->changeNozzle(2, true);
		}

		if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
			mWaterGun->changeNozzle(mWaterGun->mSecondNozzle, true);
		}

		if (mCap != nullptr) {
			mCap->unk4 |= 2;
		}
		changePlayerStatus(0x891, 0, true);
		return TRUE;
	}
	return FALSE;
}

BOOL TMario::rollingStart(const JGeometry::TVec3<f32>* warpPos, f32 rotation)
{
	u8 result = isUnUsualStageStart();
	if (result != 0) {
		return TRUE;
	} else {
		if (mAction == 0x133f) {
			unk114 &= ~2;
			if (warpPos != nullptr) {
				warpRequest(*warpPos, rotation);
				mFaceAngle.set(0, DEG2SHORTANGLE(rotation), 0);
			}

			checkGroundPlane(mPosition.x, mPosition.y + 25.0f, mPosition.z,
			                 &mFloorPosition.y, &mGroundPlane);
			unk2BC = mFloorPosition.y;
			setAnimation(0xC3, 1.0f);
			changePlayerStatus(0x1337, 0x200, true);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL TMario::returnStart(const JGeometry::TVec3<f32>* warpPos, f32 rotation,
                         bool flag, int playerStatus)
{
	if (mAction == 0x133f) {
		int offsetPlayerStatus = playerStatus << 8;
		if (flag == TRUE) {
			unk114 &= ~2;
			if (warpPos != nullptr) {
				warpRequest(*warpPos, rotation);
				mFaceAngle.set(0, DEG2SHORTANGLE(rotation), 0);
			}
			checkGroundPlane(mPosition.x, mPosition.y + 25.0f, mPosition.z,
			                 &mFloorPosition.y, &mGroundPlane);
			unk2BC = mFloorPosition.y;
			setAnimation(0xC3, 1.0f);
			changePlayerStatus(0x1337, offsetPlayerStatus | 2, true);
		} else {
			unk114 &= ~2;
			if (warpPos != nullptr) {
				f32 flippedAngle = rotation + 180.0f;
				warpRequest(*warpPos, flippedAngle);
				mFaceAngle.set(0, DEG2SHORTANGLE(flippedAngle), 0);
			}
			checkGroundPlane(mPosition.x, mPosition.y + 25.0f, mPosition.z,
			                 &mFloorPosition.y, &mGroundPlane);
			unk2BC = mFloorPosition.y;
			setAnimation(0xC3, 1.0f);
			changePlayerStatus(0x1337, offsetPlayerStatus | 1, true);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL TMario::waitingStart(const JGeometry::TVec3<f32>* warpPos, f32 rotation)
{
	u8 result = isUnUsualStageStart();
	if (result != 0) {
		return TRUE;
	} else {
		unk114 &= ~2;
		if (warpPos != nullptr) {
			warpRequest(*warpPos, rotation);
			mFaceAngle.set(0, DEG2SHORTANGLE(rotation), 0);
		}

		checkGroundPlane(mPosition.x, mPosition.y + 25.0f, mPosition.z,
		                 &mFloorPosition.y, &mGroundPlane);
		unk2BC = mFloorPosition.y;
		setAnimation(0xC3, 1.0f);
		unk114 |= 2;
		changePlayerStatus(0xC400201, 0, true);
		return TRUE;
	}
	return FALSE;
}
