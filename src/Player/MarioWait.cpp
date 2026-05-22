#include <Player/MarioMain.hpp>
#include <Player/WaterGun.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

BOOL TMario::startTalking()
{
	if (mGroundPlane->isLegal()) {
		mPosition.y = 1.0f + mFloorPosition.y;
		changePlayerStatus(STATUS_READ_BILLBOARD, 0, true);
		return 1;
	}
	return 0;
}

bool TMario::canSleep()
{
	if (checkFlag(MARIO_FLAG_IN_SHALLOW_WATER | MARIO_FLAG_IN_WATER))
		return false;

	f32 dist   = mDeParams.mSleepingCheckDist.get();
	f32 height = mDeParams.mSleepingCheckHeight.get();
	f32 groundY;
	const TBGCheckData* gnd;

	groundY = gpMap->checkGround(mPosition.x - dist, mPosition.y + 30.0f,
	                             mPosition.z, &gnd);
	if (groundY < mFloorPosition.y - height
	    || mFloorPosition.y + height < groundY)
		return false;

	groundY = gpMap->checkGround(mPosition.x + dist, mPosition.y + 30.0f,
	                             mPosition.z, &gnd);
	if (groundY < mFloorPosition.y - height
	    || mFloorPosition.y + height < groundY)
		return false;

	groundY = gpMap->checkGround(mPosition.x, mPosition.y + 30.0f,
	                             mPosition.z - dist, &gnd);
	if (groundY < mFloorPosition.y - height
	    || mFloorPosition.y + height < groundY)
		return false;

	groundY = gpMap->checkGround(mPosition.x, mPosition.y + 30.0f,
	                             mPosition.z + dist, &gnd);
	if (groundY < mFloorPosition.y - height
	    || mFloorPosition.y + height < groundY)
		return false;

	if (gpMap->isTouchedOneWall(mPosition.x, mPosition.y + 30.0f, mPosition.z,
	                            80.0f))
		return false;

	return true;
}

BOOL TMario::canPut()
{
	if (gpMap->isTouchedOneWall(mPosition.x + 100.0f * JMASSin(mFaceAngle.y),
	                            10.0f + mPosition.y,
	                            mPosition.z + 100.0f * JMASCos(mFaceAngle.y),
	                            mHeldObject->getDamageRadius()))
		return 0;

	if (gpMap->isTouchedOneWall(mPosition.x, 10.0f + mPosition.y, mPosition.z,
	                            mHeldObject->getDamageRadius()))
		return 0;
	return 1;
}

void TMario::checkPutStart() { }

BOOL TMario::waitingCommonEvents()
{
	if (mInput & 0x2) {
		if (considerRotateJumpStart())
			return 1;
		return changePlayerJumping(STATUS_JUMP, 0);
	}

	if (mInput & 0x4)
		return changePlayerStatus(0x88c, 0, false);
	if (mInput & 0x8)
		return changePlayerStatus(0x50, 0, false);
	if (mInput & 0x10)
		return changePlayerStatus(0xc000227, 0, false);

	if (mInput & 0x1) {
		s16 diff      = mIntendedYaw - mFaceAngle.y;
		s16 rotSp     = mDeParams.mWaitingRotSp.get();
		s16 converged = IConverge(diff, 0, rotSp, rotSp);
		mFaceAngle.y  = mIntendedYaw - converged;
		if (mIntendedMag > mControllerParams.mStartToWalkLevel.get()) {
			emitSmoke(mFaceAngle.y);
			return changePlayerStatus(STATUS_RUN, 0, false);
		}
	}

	if (checkFlag(MARIO_FLAG_IS_PERFORMING))
		return changePlayerStatus(STATUS_RUN, 0, false);

	if (canSquat()) {
		mForwardVel = 0.0f;
		return changePlayerStatus(STATUS_SQUAT, 0, false);
	}

	if (mInput & 0x10000)
		return changePlayerStatus(STATUS_TAKE_POSE, 0, false);

	if (rocketCheck()) {
		unk314
		    = mFloorPosition.y + mWaterGun->mWatergunParams.mHoverHeight.get();
		return changePlayerStatus(0x88b, 0, false);
	}

	if (considerRotateStart())
		return 1;
	return 0;
}

void TMario::stopCommon(int anim_id, int status_on_end)
{
	waitProcess();
	setAnimation(anim_id, 1.0f);
	if (onYoshi() && mYoshi->mActor->curAnmEndsNext(0, nullptr)) {
		changePlayerStatus(status_on_end, 0, false);
	} else if (isLast1AnimeFrame()) {
		changePlayerStatus(status_on_end, 0, false);
	}
}

void TMario::changeMontemanWaitingAnim()
{
	if (mStatus != STATUS_WAIT)
		return;
	mStatusState |= 0x2;
}

BOOL TMario::waiting()
{
	if (waitingCommonEvents())
		return 1;

	if (isMario() && (unk380 == 5 ? true : false) && canSleep()
	    && mAnimationId == ANIM_WAIT && isAnimeLoopOrStop()
	    && mGroundPlane != nullptr && mGroundPlane->getNormal().y > 0.99f) {
		mStatusTimer += 1;
		if (mStatusTimer >= 10)
			return changePlayerStatus(STATUS_SLEEPY, 0, false);
	}

	if (mStatusState & 0x2) {
		setAnimation(ANIM_MONTEMAN_WAIT, 1.0f);
	} else if (gpMarDirector->unk124 == 3) {
		setAnimation(ANIM_T_WAIT, 1.0f);
	} else if (unk368 > 0.0f ? TRUE : FALSE) {
		setAnimation(ANIM_SINKING, 1.0f);
	} else if (unk380 == 5
	           && (mPrevStatus == STATUS_BRAKE_END || checkFlag(0x20))
	           && !(mStatusState & 0x1)) {
		setAnimation(ANIM_HOT_WAIT, 1.0f);
		if (mModel->getFrameCtrl(0).checkPass(138.0f))
			emitSweat(mFaceAngle.y - 0x4000);
		if (isLast1AnimeFrame())
			mStatusState |= 0x1;
	} else if (mHealth <= 3) {
		if (mAnimationId != ANIM_DAMAGE_WAIT
		    && mAnimationId != ANIM_DAMAGE_WAIT_START) {
			setAnimation(ANIM_DAMAGE_WAIT_START, 1.0f);
		} else {
			if (mAnimationId == ANIM_DAMAGE_WAIT_START && isLast1AnimeFrame())
				setAnimation(ANIM_DAMAGE_WAIT, 1.0f);
		}
	} else if (mIntendedMag == 0.0f) {
		setAnimation(ANIM_WAIT, 1.0f);
	} else {
		setAnimation(ANIM_PIVOT, 1.0f);
	}

	waitProcess();
	return 0;
}

BOOL TMario::sleepily()
{
	if (waitingCommonEvents())
		return 1;

	if (unk108->mAnalogR > 0.0f || unk108->mAnalogL > 0.0f)
		changePlayerStatus(STATUS_WAIT, 0, false);

	if (mStatusState == 3)
		return changePlayerStatus(STATUS_SLEEP, 0, false);

	switch (mStatusState) {
	case 0:
		setAnimation(ANIM_BELT_UP, 1.0f);
		break;
	case 1:
		setAnimation(ANIM_YAWN, 1.0f);
		break;
	case 2:
		setAnimation(ANIM_SIT, 1.0f);
		break;
	}

	if (isLast1AnimeFrame())
		mStatusState += 1;

	waitProcess();
	return 0;
}

BOOL TMario::sleeping()
{
	if ((mInput & 0xa41f) || unk108->mAnalogR > 0.0f
	    || unk108->mAnalogL > 0.0f) {
		if (mStatusState == 0)
			startSoundActor(0x7883);
		else
			startSoundActor(0x789a);
		return changePlayerStatus(STATUS_WAKEUP, mStatusState, false);
	}

	waitProcess();
	if ((unk114 & 0x2) ? true : false) {
		unk1B4 = mPosition;
		sleepingEffect();
	}

	switch (mStatusState) {
	case 0:
		setAnimation(ANIM_SIT_WAIT, 1.0f);
		if (isLast1AnimeFrame()) {
			if (++mStatusTimer > 0x28)
				mStatusState += 1;
		}
		break;
	case 1:
		setAnimation(ANIM_SLEEP, 1.0f);
		if (isLast1AnimeFrame())
			mStatusState += 1;
		break;
	case 2:
		setAnimation(ANIM_SLEEP_WAIT, 1.0f);
		break;
	}
	return 0;
}

BOOL TMario::wakeup()
{
	if (mInput & 0x4) {
		sleepingEffectKill();
		return changePlayerStatus(0x88c, 0, false);
	}

	if (mInput & 0x8) {
		sleepingEffectKill();
		return changePlayerStatus(0x50, 0, false);
	}

	if (waitingCommonEvents()) {
		sleepingEffectKill();
		return 1;
	}

	waitProcess();
	setAnimation(mStatusArg == 0 ? ANIM_SIT_END : ANIM_SLEEP_END, 1.0f);

	if (isLast1AnimeFrame()) {
		sleepingEffectKill();
		return changePlayerStatus(STATUS_WAIT, 0, false);
	}
	return 0;
}

void TMario::getSideWalkValues(E_SIDEWALK_TYPE* type, f32* val1, f32* val2)
{
	s16 diff = mIntendedYaw - mFaceAngle.y;
	f32 side = mIntendedMag * JMASSin(diff) * mRunParams.mPumpingSlideSp.get();

	if (side == 0.0f) {
		*type = SIDEWALK_TYPE_UNK0;
		*val1 = getMotionFrameCtrl().getRate();
	} else {
		f32 anmRate = side * mRunParams.mPumpingSlideAnmSp.get();
		if (anmRate < 0.0f)
			anmRate = -anmRate;
		*val1 = anmRate;
		if (diff > 0)
			*type = SIDEWALK_TYPE_UNK1;
		else
			*type = SIDEWALK_TYPE_UNK2;
	}
	*val2 = side;
}

BOOL TMario::squating()
{
	if (mInput & 0x4)
		return changePlayerStatus(0x88c, 0, false);

	if (mInput & 0x8)
		return changePlayerStatus(0x50, 0, false);

	if (mInput & 0x10)
		return changePlayerStatus(0xc008222, 0, false);

	if (!(mInput & 0x4000) && !(mInput & 0x200))
		return changePlayerStatus(0xc008222, 0, false);

	if (mWaterGun == nullptr || !checkFlag(0x8000))
		return changePlayerStatus(0xc008222, 0, false);

	if (mInput & 0x2) {
		if ((mGamePad->mMeaning & TMarioGamePad::MEANING_0x400)
		    && mWaterGun != nullptr && (int)mWaterGun->mCurrentNozzle == 0) {
			rumbleStart(0x15, mMotorParams.mMotorHipDrop.get());
			return changePlayerStatus(STATUS_BACK_JUMP, 0, false);
		}
	}

	if (((const TWaterGun*)mWaterGun)
	            ->getCurrentNozzle()
	            ->mEmitParams.mRocketType.get()
	        == 1
	    && mWaterGun->isEmitting()) {
		unk314
		    = mFloorPosition.y + mWaterGun->mWatergunParams.mHoverHeight.get();
		return changePlayerStatus(0x88b, 0, false);
	}

	if (mGamePad->mMeaning & TMarioGamePad::MEANING_0x2000) {
		E_SIDEWALK_TYPE type;
		f32 v1, v2;
		getSideWalkValues(&type, &v1, &v2);
		switch ((int)type) {
		case 0:
			setAnimation(ANIM_SQWAT, 1.0f);
			break;
		case 1:
			setAnimation(ANIM_SWLKL, v1);
			break;
		case 2:
			setAnimation(ANIM_SWLKR, v1);
			break;
		}

		mPosition.x += v2 * JMASCos(mFaceAngle.y);
		mPosition.z -= v2 * JMASSin(mFaceAngle.y);
	} else if (mGamePad->mMeaning & TMarioGamePad::MEANING_0x400) {
		f32 absH      = fabsf(mGamePad->mCompSPos[0]);
		bool positive = true;
		if (mGamePad->mCompSPos[0] < 0.0f)
			positive = false;

		f32 mid    = mControllerParams.mSquatRotMidAnalog.get();
		f32 midVal = mControllerParams.mSquatRotMidValue.get();
		f32 rotAmt;
		if (absH < mid) {
			rotAmt = midVal * (absH / mid);
		} else {
			rotAmt = midVal + (1.0f - midVal) * ((absH - mid) / (1.0f - mid));
		}
		if (!positive)
			rotAmt = -rotAmt;
		mFaceAngle.y += (s16)(rotAmt * (f32)(-mDeParams.mWaitingRotSp.get()));
		setAnimation(ANIM_SQWAT, 1.0f);
	}

	waitProcess();
	return 0;
}

BOOL TMario::squatStart() { return 0; }

BOOL TMario::squatStandup()
{
	if (mInput & 0x4)
		return changePlayerStatus(0x88c, 0, false);
	if (mInput & 0x8)
		return changePlayerStatus(0x50, 0, false);
	if (mInput & 0x2)
		return changePlayerStatus(STATUS_JUMP, 0, false);
	if (mInput & 0x1)
		return changePlayerStatus(STATUS_RUN, 0, false);

	waitProcess();

	if (mStatus == 0xc000223)
		setAnimation(ANIM_THROWN_END, 1.0f);
	else
		setAnimation(ANIM_SQEND, 1.0f);

	if (isLast1AnimeFrame())
		changePlayerStatus(STATUS_WAIT, 0, false);
	return 0;
}

BOOL TMario::pullEnd()
{
	if (mInput & 0x4)
		return changePlayerStatus(0x88c, 0, false);

	if (mInput & 0x8)
		return changePlayerStatus(0x50, 0, false);

	waitProcess();
	setAnimation(ANIM_HOLD_RETURN, 1.0f);
	if (isLast1AnimeFrame())
		changePlayerStatus(STATUS_WAIT, 0, false);
	return 0;
}

BOOL TMario::jumpEndCommon(int param_1, int param_2)
{
	waitProcess();
	setAnimation(param_1, 1.0f);
	if (isLast1AnimeFrame())
		return changePlayerStatus(param_2, 0, false);
	return 0;
}

BOOL TMario::jumpEndEvents(u32 param_1)
{
	if (mInput & 0x10)
		return changePlayerStatus(STATUS_WAIT, 0, false);

	if (mInput & 0x2) {
		if (param_1 == 0)
			return changePlayerTriJump();
		return changePlayerJumping(param_1, 0);
	}
	if (mInput & 0xF)
		return checkAllMotions();
	return 0;
}

BOOL TMario::jumpEnd() { }

BOOL TMario::secJumpEnd() { }

BOOL TMario::landEnd() { }

BOOL TMario::ultraJumpEnd()
{
	if (jumpEndEvents(STATUS_JUMP))
		return 1;

	waitProcess();
	setAnimation(ANIM_LAEND, 1.0f);
	if (isLast1AnimeFrame())
		changePlayerStatus(STATUS_WAIT, 0, false);
	return 0;
}

BOOL TMario::uTurnJumpEnd()
{
	if (jumpEndEvents(0))
		return 1;

	waitProcess();
	setAnimation(ANIM_TJMP2, 1.0f);
	if (isLast1AnimeFrame())
		changePlayerStatus(STATUS_WAIT, 0, false);

	mFaceAngle.x = 0;
	mModelFaceAngle += 0x8000;
	return 0;
}

BOOL TMario::jumpThrowEnd()
{
	checkThrowObject();
	jumpEndCommon(0x65, STATUS_WAIT);
	return 0;
}

BOOL TMario::fireJumpEnd()
{
	mInput &= ~0x2010;
	if (jumpEndEvents(0))
		return 1;

	waitProcess();
	setAnimation(ANIM_FJPEND, 1.0f);
	if (isLast1AnimeFrame())
		changePlayerStatus(STATUS_WAIT, 0, false);
	return 0;
}

BOOL TMario::broadJumpEnd()
{
	mInput &= ~0x2000;
	if (jumpEndEvents(STATUS_JUMP)) {
		if (mStatus - 0x4000000U == 0x440U)
			changePlayerStatus(0xC008222, 0, false);
		else
			return 1;
	} else {
		waitProcess();
		setAnimation(ANIM_SQWAT, 1.0f);
		if (isLast1AnimeFrame())
			changePlayerStatus(0xC008222, 0, false);
	}
	return 0;
}

BOOL TMario::hipAttackEnd()
{
	mStatusState = 1;
	if (mInput & 0x4) {
		return changePlayerStatus(0x88c, 0, false);
	} else if (mInput & 0x8) {
		return changePlayerStatus(0x840452, 0, false);
	} else {
		waitProcess();
		setAnimation(ANIM_HIPED, 1.0f);
		if (isLast1AnimeFrame())
			changePlayerStatus(STATUS_SLIP_END, 0, false);
	}
	return 0;
}

BOOL TMario::brakeEnd()
{
	if ((mInput & 0x10) == 0 && (mInput & 0xF))
		return checkAllMotions();

	stopCommon(0x10, STATUS_WAIT);
	return 0;
}

BOOL TMario::slipEnd()
{
	if (mInput & 0xF)
		return checkAllMotions();

	stopCommon(0x8F, STATUS_WAIT);
	return 0;
}

static int unknown_inline_1(TMario* mario)
{
	if (mario->jumpEndEvents(0))
		return 1;

	mario->waitProcess();
	mario->setAnimation(TMario::ANIM_JMPED, 1.0f);
	if (mario->isLast1AnimeFrame())
		mario->changePlayerStatus(TMario::STATUS_WAIT, 0, false);
	return 0;
}

static int unknown_inline_2(TMario* mario)
{
	if (mario->jumpEndEvents(0))
		return 1;

	mario->waitProcess();
	mario->setAnimation(TMario::ANIM_2JMED, 1.0f);
	if (mario->isLast1AnimeFrame())
		mario->changePlayerStatus(TMario::STATUS_WAIT, 0, false);
	return 0;
}

static int unknown_inline_3(TMario* mario)
{
	if (mario->jumpEndEvents(0))
		return 1;

	mario->waitProcess();
	mario->setAnimation(TMario::ANIM_LAEND, 1.0f);
	if (mario->isLast1AnimeFrame())
		mario->changePlayerStatus(TMario::STATUS_WAIT, 0, false);
	return 0;
}

int TMario::waitMain()
{
	int result = 0;

	checkEnforceJump();
	checkCollision();
	setNormalAttackArea();

	if (mHeldObject != nullptr && (mInput & 0x2000 ? true : false)) {
		switch (mHeldObject->getActorType()) {
		case 0x80000001:
			changePlayerStatus(STATUS_PITCHING, 0, false);
			break;

		default:
			if (canPut())
				changePlayerStatus(STATUS_PUTTING, 0, false);
			break;
		}
	}

	switch (mStatus) {
	case STATUS_WAIT:
		result = waiting();
		break;

	case STATUS_SLEEPY:
		result = sleepily();
		break;

	case STATUS_SLEEP:
		result = sleeping();
		break;

	case STATUS_WAKEUP:
		result = wakeup();
		break;

	case STATUS_SQUAT:
		result = squating();
		break;

	case STATUS_SQUAT_START:
		result = squatStart();
		break;

	case 0xC000223:
	case 0xC008222:
		result = squatStandup();
		break;

	case STATUS_PULL_END:
		result = pullEnd();
		break;

	case 0xC000230:
		result = unknown_inline_1(this);
		break;

	case 0xC000231:
		result = unknown_inline_2(this);
		break;

	case 0xC000232:
		result = unknown_inline_3(this);
		break;

	case STATUS_U_TURN_JUMP_END:
		result = uTurnJumpEnd();
		break;

	case STATUS_JUMP_THROW_END:
		result = jumpThrowEnd();
		break;

	case STATUS_FIRE_JUMP_END:
		result = fireJumpEnd();
		break;

	case STATUS_ULTRA_JUMP_END:
		result = ultraJumpEnd();
		break;

	case STATUS_BROAD_JUMP_END:
		result = broadJumpEnd();
		break;

	case STATUS_HIP_ATTACK_END:
		result = hipAttackEnd();
		break;

	case STATUS_BRAKE_END:
		result = brakeEnd();
		break;

	case STATUS_SLIP_END:
		result = slipEnd();
		break;
	}

	return result;
}
