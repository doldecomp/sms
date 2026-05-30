#include <Player/Mario.hpp>
#include <Enemy/Conductor.hpp>
#include <System/MarDirector.hpp>
#include <NPC/NpcBase.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MoveBG/ModelGate.hpp>
#include <System/StageUtil.hpp>
#include <Player/WaterGun.hpp>
#include <Player/MarioCap.hpp>
#include <JSystem/JMath.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

#pragma dont_inline on
BOOL TMario::winDemo()
{
	switch (mStatusState) {
	case 0:
		if (mHeldObject != nullptr) {
			mHeldObject->receiveMessage(mHeldObject, HIT_MESSAGE_UNKD);
			mHeldObject = nullptr;
		}
		gpConductor->killEnemiesWithin(mPosition, 2000.0f);
		if (jumpProcess(0) == TRUE) {
			gpMarDirector->fireGetStar((TShine*)unk384);
			unk384->receiveMessage(this, HIT_MESSAGE_TAKE);
			mStatusState = 1;
		}
		break;
	case 1:
		setAnimation(ANIM_DEMO_SHINE_GET, 1.0f);
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
	switch (mStatusState) {
	case 0: {
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
		setAnimation(ANIM_T_WAIT, 1.0f);
		mStatusState = 1;
	}
	case 1: {
		s16 attackAngle = getAttackAngle(talkingNpc);
		s16 diffAngle   = attackAngle - mFaceAngle.y;
		s32 convAngle
		    = attackAngle
		      - IConverge(diffAngle, 0, mAutoDemoParams.mReadRotSp.get(),
		                  mAutoDemoParams.mReadRotSp.get());
		mFaceAngle.y = convAngle;
		if (attackAngle == mFaceAngle.y) {
			gpMarDirector->unk126 = 2;
			mStatusState          = 2;
		}
		break;
	}
	case 2:
		if (gpMarDirector->unk124 == 0 || gpMarDirector->unk124 == 5) {
			changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
		}
		break;
	}
	mFaceAngle.x    = 0;
	mModelFaceAngle = mFaceAngle.y;
	return FALSE;
}

BOOL TMario::bottleIn()
{
	setAnimation(ANIM_BOTTLE_IN, 1.0f);
	return FALSE;
}

BOOL TMario::elecDowning()
{
	setAnimation(ANIM_SHOCK_DOWN, 1.0f);
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

BOOL TMario::openDoor()
{
	if (mStatusState == 0) {
		startVoice(MSD_SE_MV13_ACTION_SMALL_01);
		mStatusState = 1;
	}
	stopProcess();
	if (isLast1AnimeFrame()) {
		if ((mAnimationId == ANIM_DOOR_OPENR)
		    || (mAnimationId == ANIM_DOOR_OPENL)
		    || (mAnimationId == ANIM_DOOR_KICK)) {
			mPosition.x += JMASSin(mFaceAngle.y) * 150.0f;
			mPosition.z += JMASCos(mFaceAngle.y) * 150.0f;
		} else {
			mPosition.x -= JMASSin(mFaceAngle.y) * 150.0f;
			mPosition.z -= JMASCos(mFaceAngle.y) * 150.0f;
		}
		return changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
	}
	return FALSE;
}

BOOL TMario::sinkLoser()
{
	setPlayerVelocity(0.0f);
	setAnimation(ANIM_SINK_DOWN, 1.0f);
	if (jumpProcess(0) == TRUE) {
		changePlayerStatus(MARIO_STATUS_LOSER_DOWN, 0, true);
	}
	return FALSE;
}

BOOL TMario::downLoser()
{
	setPlayerVelocity(0.0f);
	setAnimation(ANIM_LAND, 1.0f);
	if (jumpProcess(0) == TRUE)
		changePlayerStatus(MARIO_STATUS_LOSER_DOWN, 0, true);
	return FALSE;
}

BOOL TMario::warpIn()
{
	// Missing stack space
	// volatile u32 padding[10];
	mStatusTimer += 1;
	const JGeometry::TVec3<f32>& gatePosOffset = ((TModelGate*)mHolder)->unkAC;
	JGeometry::TVec3<f32> holderPosOffset(((TModelGate*)mHolder)->unkAC);
	holderPosOffset.y -= 80.0f;
	switch (mStatusState) {
	case 0: {
		if (mStatusTimer <= 1) {
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
		J3DFrameCtrl& frameCtrl = getMotionFrameCtrl();
		frameCtrl.setRate(0.0f);

		// Possibly TVec3 inaccuracies?
		JGeometry::TVec3<f32> marioDist = holderPosOffset - mPosition;
		mPosition                       = marioDist * 0.02f + mPosition;

		f32 dist
		    = mAutoDemoParams.mWarpInTremble.get() - marioDist.length() * 0.1f;
		if (dist > 0.0f) {
			mTrembleModelEffect->clash(dist);
		}

		if (0x78 < mStatusTimer) {
			mStatusTimer = 0;
			warpInEffect();
			unk468       = 0.0f;
			mStatusState = 1;
			// Probably some reserved sound?
			startVoice(-0x2);
		}
		break;
	}
	case 1: {
		if ((f32)mStatusTimer > mAutoDemoParams.mWarpInBallsDispTime.get()) {
			unk114 &= ~(1 << 1);
			rumbleStart(0x15, 0x14);
		}
		if (mAutoDemoParams.mWarpInBallsTime.get() > (f32)mStatusTimer) {
			mStatusTimer = 0;
			unk468       = mAutoDemoParams.mWarpInVecBase.get();
			mStatusState = 2;
		}

		break;
	}
	case 2:
		unk114 &= ~(1 << 1);
		rumbleStart(0x14, mMotorParams.mMotorWall.get() / 2);

		if ((f32)mStatusTimer > mAutoDemoParams.mWarpInCapturedTime.get()) {
			unk114 &= ~(1 << 1);
			mStatusTimer = 0;
			mHolder->receiveMessage(this, HIT_MESSAGE_ATTACK);
			mStatusState = 3;
		}

		break;
	case 3:
		onFlag(MARIO_FLAG_IS_PERFORMING);
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
		changePlayerStatus(MARIO_STATUS_TOROCCO, 0, true);
		unk114 |= 2;
		if (mPinaRail != nullptr) {
			mPinaRail->setBckFromIndex(0);
			mPinaRail->getFrameCtrl(0)->setRate(0.5f);
			mPinaRail->getFrameCtrl(0)->setFrame(0.0f);
		}
		if (mKoopaRail != nullptr) {
			mKoopaRail->setBckFromIndex(0);
			mKoopaRail->getFrameCtrl(0)->setRate(0.5f);
			mKoopaRail->getFrameCtrl(0)->setFrame(0.0f);
		}
		return TRUE;
	}

	if (SMS_isDivingMap()) {
		unk114 |= 2;

		// I suspect some inline stuff here, weird to check right after you set
		// it
		onFlag(MARIO_FLAG_HELMET_FLW_CAMERA);
		onFlag(MARIO_FLAG_HELMET);
		onFlag(MARIO_FLAG_HAS_FLUDD);

		if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
			mWaterGun->changeNozzle(TWaterGun::Underwater, true);
		}

		if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
			mWaterGun->changeNozzle(
			    (TWaterGun::TNozzleType)mWaterGun->mSecondNozzle, true);
		}

		if (mCap != nullptr) {
			mCap->unk4 |= 2;
		}
		changePlayerStatus(MARIO_STATUS_DIVE, 0, true);
		return TRUE;
	}
	return FALSE;
}

void startCommon(const JGeometry::TVec3<f32>*, f32) { }

BOOL TMario::rollingStart(const JGeometry::TVec3<f32>* warpPos, f32 rotation)
{
	u8 result = isUnUsualStageStart();
	if (result != 0) {
		return TRUE;
	} else {
		if (mStatus == MARIO_STATUS_DISAPPEAR) {
			unk114 &= ~2;
			if (warpPos != nullptr) {
				warpRequest(*warpPos, rotation);
				mFaceAngle.set(0, DEG2SHORTANGLE(rotation), 0);
			}

			checkGroundPlane(mPosition.x, mPosition.y + 25.0f, mPosition.z,
			                 &mFloorPosition.y, &mGroundPlane);
			unk2BC = mFloorPosition.y;
			setAnimation(ANIM_WAIT, 1.0f);
			changePlayerStatus(MARIO_STATUS_WARP_OUT, 0x200, true);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL TMario::returnStart(const JGeometry::TVec3<f32>* warpPos, f32 rotation,
                         bool flag, int playerStatus)
{
	if (mStatus == MARIO_STATUS_DISAPPEAR) {
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
			setAnimation(ANIM_WAIT, 1.0f);
			changePlayerStatus(MARIO_STATUS_WARP_OUT, offsetPlayerStatus | 2,
			                   true);
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
			setAnimation(ANIM_WAIT, 1.0f);
			changePlayerStatus(MARIO_STATUS_WARP_OUT, offsetPlayerStatus | 1,
			                   true);
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
		setAnimation(ANIM_WAIT, 1.0f);
		unk114 |= 2;
		changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
		return TRUE;
	}
	return FALSE;
}

BOOL TMario::toroccoStart()
{
	changePlayerStatus(MARIO_STATUS_TOROCCO, 0, true);
	unk114 |= 2;
	if (mPinaRail != nullptr) {
		mPinaRail->setBckFromIndex(0);
		mPinaRail->getFrameCtrl(0)->setRate(0.5f);
		mPinaRail->getFrameCtrl(0)->setFrame(0.0f);
	}
	if (mKoopaRail != nullptr) {
		mKoopaRail->setBckFromIndex(0);
		mKoopaRail->getFrameCtrl(0)->setRate(0.5f);
		mKoopaRail->getFrameCtrl(0)->setFrame(0.0f);
	}
	return TRUE;
}

BOOL TMario::warpOut()
{
	// Missing stack space
	// volatile u32 padding[4];

	mStatusTimer += 1;
	unk114 |= 2;
	switch (mStatusState) {
	case 0:
		unk114 |= 2;
		if ((mStatusArg & 0xff) == 2) {
			setAnimation(ANIM_DEMO_GATE_OUT_APPEAR_GET, 1.0f);
		} else {
			setAnimation(ANIM_DEMO_GATE_OUT_APPEAR, 1.0f);
		}
		warpOutEffect((mStatusArg >> 8) & 0xff,
		              (mStatusArg & 0xff) * 180.0f
		                  + SHORTANGLE2DEG(mFaceAngle.y));
		mStatusState = 1;
		mStatusTimer = 0;
		break;
	case 1:
		s32 unkDelay;
		if ((mStatusArg & 0x200) != 0) {
			unkDelay = 0x70;
		} else {
			unkDelay = 0xb4;
		}
		if (mStatusTimer >= unkDelay) {
			if (checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)) {
				unk114 |= 2;
				return changePlayerStatus(MARIO_STATUS_DIVE, 0, true);
			}
			mStatusState = 2;
		}
		break;
	case 2:
		unk114 |= 2;
		if ((mStatusArg & 0xff) == 2) {
			setAnimation(ANIM_DEMO_GATE_OUT_ROLLING_GET, 1.0f);
		} else {
			setAnimation(ANIM_DEMO_GATE_OUT_ROLLING, 1.0f);
		}
		if (jumpProcess(0) == TRUE) {
			mStatusState = 3;
		}
		break;
	case 3:
		unk114 |= 2;
		switch (mStatusArg & 0xff) {
		case 0:
			return changePlayerStatus(MARIO_STATUS_JUMP_END, 0, true);
		case 1:
			setAnimation(ANIM_DEMO_GATE_OUT, 1.0f);
			if (isLast1AnimeFrame()) {
				return changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
			}
			break;
		case 2:
			setAnimation(ANIM_DEMO_GATE_OUT_GET2, 1.0f);
			if (isLast1AnimeFrame()) {
				return changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
			}
			break;
		}
		break;
	}
	return false;
}

BOOL TMario::electricDamage()
{
	if (mStatusState == 0) {
		startVoice(MSD_SE_MV04_DAMAGE_ELEC_01);
		setAnimation(ANIM_SHOCK, 1.0f);
		decHP(mDmgParamsGraffitoElec.mDamage.get());
		rumbleStart(0x16, 1);
		mStatusState = 1;
	}

	elecEffect();
	mStatusTimer += 1;
	if (mStatusTimer > 0x78) {

		J3DFrameCtrl& frameCtrl = getMotionFrameCtrl();
		frameCtrl.setFrame(0.0f);
		mStatusTimer += 1;
		startVoice(MSD_SE_MV07_DAMAGE_REACT_01);

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

		return changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
	}
	return false;
}

BOOL TMario::footDowning()
{
	switch (mStatusArg) {
	case 0:
		setAnimation(ANIM_SANDFILL_LEG, 1.0f);
		if ((mInput & 2) != 0) {
			mStatusArg = 2;
		}
		break;
	case 1:
		setAnimation(ANIM_SAND_FILL_HEAD, 1.0f);
		if ((mInput & 2) != 0) {
			mStatusArg = 3;
		}
		break;
	case 2:
		setAnimation(ANIM_SANDFILL_LEG_END, 1.0f);
		if (isLast1AnimeFrame()) {
			return changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
		}
		break;
	case 3:
		setAnimation(ANIM_SAND_FILL_HEAD_END, 1.0f);
		J3DFrameCtrl& frameCtrl = getMotionFrameCtrl();
		if (frameCtrl.checkPass(24.0f) != FALSE) {
			sinkInSandEffect();
		}
		if (isLast1AnimeFrame()) {
			return changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
		}
		break;
	}
	stopProcess();
	return false;
}

BOOL TMario::nomotion()
{
	stopProcess();
	return FALSE;
}

BOOL TMario::disappear()
{
	unk114 &= ~2;
	return FALSE;
}

BOOL TMario::demoMain()
{
	// Missing stack space
	// volatile u32 padding[10];

	BOOL result = FALSE;
	switch (mStatus) {
	case MARIO_STATUS_WIN_DEMO:
		result = winDemo();
		break;

	case MARIO_STATUS_READ_BILLBOARD:
		result = readBillboard();
		break;

	case MARIO_STATUS_BOTTLE_IN:
		result = bottleIn();
		break;

	case MARIO_STATUS_ELEC_DOWN:
		result = elecDowning();
		break;

	case MARIO_STATUS_DOOR_OPEN_R:
	case MARIO_STATUS_DOOR_OPEN_L:
		result = openDoor();
		break;

	case MARIO_STATUS_SINK_LOSER:
		result = sinkLoser();
		break;

	case MARIO_STATUS_DOWN_LOSER:
		result = downLoser();
		break;

	case MARIO_STATUS_WARP_OUT:
		result = warpOut();
		break;

	case MARIO_STATUS_WARP_IN:
		result = warpIn();
		break;

	case MARIO_STATUS_ELECTRIC_DAMAGE:
		result = electricDamage();
		break;

	case MARIO_STATUS_FOOT_DOWN:
		result = footDowning();
		break;

	case MARIO_STATUS_NOMOTION:
		result = nomotion();
		break;

	case MARIO_STATUS_DISAPPEAR:
		result = disappear();
		break;
	}
	return result;
}
