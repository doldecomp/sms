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

#pragma dont_inline on
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
#pragma dont_inline off

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
	// volatile u32 padding[10];
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
			MtxPtr nodeMatrix = gate->unk78->getModel()->getAnmMtx(gate->unk72);
			mWarpInDir.x      = nodeMatrix[0][3] - gatePosOffset.x;
			mWarpInDir.y      = nodeMatrix[1][3] - gatePosOffset.y;
			mWarpInDir.z      = nodeMatrix[2][3] - gatePosOffset.z;
			mWarpInDir.normalize();
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
			mTrembleModelEffect->clash(dist);
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
		if (mPinaRail != nullptr) {
			mPinaRail->setBckFromIndex(0);
			mPinaRail->getFrameCtrl(0)->setSpeed(0.5f);
			mPinaRail->getFrameCtrl(0)->setFrame(0.0f);
		}
		if (mKoopaRail != nullptr) {
			mKoopaRail->setBckFromIndex(0);
			mKoopaRail->getFrameCtrl(0)->setSpeed(0.5f);
			mKoopaRail->getFrameCtrl(0)->setFrame(0.0f);
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

BOOL TMario::toroccoStart()
{
	changePlayerStatus(0x800447, 0, true);
	unk114 |= 2;
	if (mPinaRail != nullptr) {
		mPinaRail->setBckFromIndex(0);
		mPinaRail->getFrameCtrl(0)->setSpeed(0.5f);
		mPinaRail->getFrameCtrl(0)->setFrame(0.0f);
	}
	if (mKoopaRail != nullptr) {
		mKoopaRail->setBckFromIndex(0);
		mKoopaRail->getFrameCtrl(0)->setSpeed(0.5f);
		mKoopaRail->getFrameCtrl(0)->setFrame(0.0f);
	}
	return TRUE;
}

BOOL TMario::warpOut()
{
	// Missing stack space
	// volatile u32 padding[4];

	mActionTimer += 1;
	unk114 |= 2;
	switch (mActionState) {
	case 0:
		unk114 |= 2;
		if ((mActionArg & 0xff) == 2) {
			setAnimation(0x13B, 1.0f);
		} else {
			setAnimation(0x12E, 1.0f);
		}
		warpOutEffect((mActionArg >> 8) & 0xff,
		              (mActionArg & 0xff) * 180.0f
		                  + SHORTANGLE2DEG(mFaceAngle.y));
		mActionState = 1;
		mActionTimer = 0;
		break;
	case 1:
		s32 unkDelay;
		if ((mActionArg & 0x200) != 0) {
			unkDelay = 0x70;
		} else {
			unkDelay = 0xb4;
		}
		if (mActionTimer >= unkDelay) {
			if (checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)) {
				unk114 |= 2;
				return changePlayerStatus(0x891, 0, true);
			}
			mActionState = 2;
		}
		break;
	case 2:
		unk114 |= 2;
		if ((mActionArg & 0xff) == 2) {
			setAnimation(0x13C, 1.0f);
		} else {
			setAnimation(0x13D, 1.0f);
		}
		if (jumpProcess(0) == TRUE) {
			mActionState = 3;
		}
		break;
	case 3:
		unk114 |= 2;
		switch (mActionArg & 0xff) {
		case 0:
			return changePlayerStatus(0xC000230, 0, true);
		case 1:
			setAnimation(0x10E, 1.0f);
			if (isLast1AnimeFrame()) {
				return changePlayerStatus(0xC400201, 0, true);
			}
			break;
		case 2:
			setAnimation(0x12D, 1.0f);
			if (isLast1AnimeFrame()) {
				return changePlayerStatus(0xC400201, 0, true);
			}
			break;
		}
		break;
	}
	return false;
}

BOOL TMario::electricDamage()
{
	if (mActionState == 0) {
		startVoice(0x7844);
		setAnimation(0x7A, 1.0f);
		decHP(mDmgParamsGraffitoElec.mDamage.get());
		rumbleStart(0x16, 1);
		mActionState = 1;
	}

	elecEffect();
	mActionTimer += 1;
	if (mActionTimer > 0x78) {

		J3DFrameCtrl* frameCtrl = getMotionFrameCtrl();
		frameCtrl->setFrame(0.0f);
		mActionTimer += 1;
		startVoice(0x7852);

		if (mTrembleModelEffect != nullptr) {
			mTrembleModelEffect->tremble(5.0f, 2.0f, 0.99f, 600);
		}

		elecEndEffect();

		mFloorHitActor.mPosition.x = mPosition.x + JMASSin(mFaceAngle.y);
		mFloorHitActor.mPosition.z = mPosition.z + JMASCos(mFaceAngle.y);

		damageExec(&mFloorHitActor, 0, 3,
		           mDmgParamsGraffitoElec.mWaterEmit.get(),
		           mDmgParamsGraffitoElec.mMinSpeed.get(),
		           mDmgParamsGraffitoElec.mMotor.get(), 0.0f, 0x3C);

		return changePlayerStatus(0xC400201, 0, true);
	}
	return false;
}

BOOL TMario::footDowning()
{
	switch (mActionArg) {
	case 0:
		setAnimation(0x125, 1.0f);
		if ((mInput & 2) != 0) {
			mActionArg = 2;
		}
		break;
	case 1:
		setAnimation(0x123, 1.0f);
		if ((mInput & 2) != 0) {
			mActionArg = 3;
		}
		break;
	case 2:
		setAnimation(0x126, 1.0f);
		if (isLast1AnimeFrame()) {
			return changePlayerStatus(0xC400201, 0, false);
		}
		break;
	case 3:
		setAnimation(0x124, 1.0f);
		J3DFrameCtrl* frameCtrl = getMotionFrameCtrl();
		if (frameCtrl->checkPass(24.0f) != FALSE) {
			sinkInSandEffect();
		}
		if (isLast1AnimeFrame()) {
			return changePlayerStatus(0xC400201, 0, false);
		}
		break;
	}
	stopProcess();
	return false;
}

BOOL TMario::demoMain()
{
	// Missing stack space
	// volatile u32 padding[10];

	BOOL result = FALSE;
	switch (mAction) {
	case 0x1302:
		result = winDemo();
		break;
	case 0x10001308:
		result = readBillboard();
		break;
	case 0x1310:
		setAnimation(0x122, 1.0f);
		result = FALSE;
		break;
	case 0x21313:
		setAnimation(0x79, 1.0f);
		result = FALSE;
		break;
	case 0x1320:
	case 0x1321:
		// Probably inlined?
		if (mActionState == 0) {
			startVoice(0x7884);
			mActionState = 1;
		}
		stopProcess();
		if (isLast1AnimeFrame()) {
			if ((unk0FA == 0x5f) || (unk0FA == 0x60) || (unk0FA == 0xe9)) {
				mPosition.x += JMASSin(mFaceAngle.y) * 150.0f;
				mPosition.z += JMASCos(mFaceAngle.y) * 150.0f;
			} else {
				mPosition.x -= JMASSin(mFaceAngle.y) * 150.0f;
				mPosition.z -= JMASCos(mFaceAngle.y) * 150.0f;
			}
			return changePlayerStatus(0xC400201, 0, true);
		}
		result = FALSE;
		break;
	case 0x10001123:
		setPlayerVelocity(0.0f);
		setAnimation(0xE8, 1.0f);
		if (jumpProcess(0) == TRUE) {
			changePlayerStatus(0x20000 + 0x467, 0, true);
		}
		result = FALSE;
		break;
	case 0x1000192a:
		setPlayerVelocity(0.0f);
		setAnimation(0x56, 1.0f);
		if (jumpProcess(0) == TRUE) {
			changePlayerStatus(0x20000 + 0x467, 0, true);
		}
		result = FALSE;
		break;
	case 0x1337:
		result = warpOut();
		break;
	case 0x1336:
		result = warpIn();
		break;
	case 0x20338:
		result = electricDamage();
		break;
	case 0x2033c:
		result = footDowning();
		break;
	case 0x133E:
		stopProcess();
		result = FALSE;
		break;
	case 0x133F:
		unk114 &= ~2;
		result = FALSE;
		break;
	}
	return result;
}
