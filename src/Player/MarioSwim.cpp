#include <Player/MarioMain.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <System/Particles.hpp>
#include <Map/MapData.hpp>
#include <MSound/MSound.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMario::isSwimWaiting() { }

void TMario::doSwimming()
{
	if (mInput & 0x8000) {
		changePlayerStatus(STATUS_SWIM_DIVE, 0, false);
		return;
	}

	if (mFloorPosition.y + mSwimParams.mEndDepth.get() > mFloorPosition.z) {
		changePlayerStatus(STATUS_WAIT, 0, false);
		return;
	}

	mForwardVel += mIntendedMag * mSwimParams.mMoveSp.get() * 0.03125f;
	mForwardVel *= mSwimParams.mMoveBrake.get();

	f32 rotMinF, rotMaxF;
	if (fabricatedUnk380Inline()) {
		rotMinF = (f32)mSwimParams.mPumpingRotSpMin.get();
		rotMaxF = (f32)mSwimParams.mPumpingRotSpMax.get();
	} else {
		rotMinF = (f32)mSwimParams.mSwimmingRotSpMin.get();
		rotMaxF = (f32)mSwimParams.mSwimmingRotSpMax.get();
	}

	s16 rotSp    = mForwardVel / 32.0f * (rotMaxF - rotMinF) + rotMinF;
	s16 diff     = mIntendedYaw - mFaceAngle.y;
	mFaceAngle.y = mIntendedYaw - IConverge(diff, 0, rotSp, rotSp);

	setPlayerVelocity(mForwardVel);

	mVel.y -= mSwimParams.mGravity.get();

	f32 depthRatio
	    = (mFloorPosition.z - mPosition.y) / mSwimParams.mFloatHeight.get();
	if (depthRatio < 0.0f)
		depthRatio = 0.0f;
	if (depthRatio > 1.0f)
		depthRatio = 1.0f;

	if (mAnimationId == ANIM_SWIM_WAIT || mAnimationId == ANIM_SWIM_START
	    || mStatus == STATUS_SWIM_WAIT)
		depthRatio *= mSwimParams.mWaitBouyancy.get();
	else
		depthRatio *= mSwimParams.mMoveBouyancy.get();

	mVel.y += depthRatio;
	mVel.y *= mSwimParams.mUpDownBrake.get();

	switch (jumpProcess(1)) {
	case 1:
		break;
	case 2:
		if (checkFlag(MARIO_FLAG_FLUDD_EMITTING)) {
			if (isUnderWater()) {
				mForwardVel = 0.8f * -mForwardVel;
				changePlayerStatus(STATUS_SWIM_P_DAMAGE, 0, true);
			} else {
				changePlayerStatus(0x208b3, 0, true);
				mForwardVel = 0.8f * -mForwardVel;
				mVel.y      = 50.0f;
			}
		} else {
			setPlayerVelocity(0.0f);
			mVel.y = 0.0f;
		}
		break;
	}

	if (mFloorPosition.z > mFloorPosition.y + 400.0f
	    && mPosition.y < mFloorPosition.y + 100.0f
	    && mStatus != STATUS_SWIM_WAIT) {
		unk1B4   = mPosition;
		unk1B4.y = mFloorPosition.y;
		gpMarioParticleManager->emitAndBindToPosPtr(0x11e, &unk1B4, 1, this);
	}

	if (mPosition.y < mFloorPosition.y + 35.0f)
		mPosition.y = mFloorPosition.y + 35.0f;
}

BOOL TMario::checkSwimJump()
{
	if (mInput & 0x2) {
		if (checkFlag(MARIO_FLAG_FLUDD_EMITTING) && !isUnderWater()) {
			mPosition.y = 1.0f + mFloorPosition.z;
			startSoundActor(0x828);
			return changePlayerJumping(0x888, 0);
		}

		if (mFloorPosition.z - mSwimParams.mCanJumpDepth.get() < mPosition.y) {
			bool doJump = false;
			if (mIntendedMag == 0.0f)
				doJump = true;
			if (mWallPlane != nullptr)
				doJump = true;
			s16 diff = mModelFaceAngle - mIntendedYaw;
			if (diff < -0x5555 || diff > 0x5555)
				doJump = true;
			if (doJump == true) {
				inOutWaterEffect(mFloorPosition.z);
				changePlayerStatus(STATUS_JUMP, 0, false);
				return 1;
			}
		}

		if (mIntendedMag == 0.0f)
			return changePlayerStatus(STATUS_SWIM_UP, 0, false);
		return changePlayerStatus(STATUS_SWIM_PADDLE_START, 0, false);
	}
	return 0;
}

BOOL TMario::checkSwimToHangFence()
{
	if (mIntendedMag > 0.0f && mWallPlane != nullptr && mWallPlane->isFence()) {
		s16 wallAng
		    = matan(mWallPlane->getNormal().z, mWallPlane->getNormal().x);
		s16 diff = mFaceAngle.y - wallAng;
		if (diff < -0x5555 || diff > 0x5555) {
			mFaceAngle.y    = wallAng + 0x8000;
			mModelFaceAngle = mFaceAngle.y;
			mPosition.y += 100.0f;
			return changePlayerStatus(STATUS_FENCE_CATCH, 0, false);
		}
	}
	return 0;
}

BOOL TMario::swimStart()
{
	doSwimming();

	if (!checkStatusFlag(STATUS_FLAG_SWIMMING))
		return 1;

	setAnimation(ANIM_PADDLE_SWIM_START, 1.0f);

	if (isLast1AnimeFrame())
		changePlayerStatus(STATUS_SWIM_WAIT, 0, false);

	return 0;
}

BOOL TMario::swimWait()
{
	setAnimation(ANIM_PADDLE_SWIM_WAIT, 1.0f);

	if (mInput & 0x1)
		return changePlayerStatus(STATUS_SWIM_WAIT_TO_PADDLE, 0, false);

	doSwimming();

	if (!checkStatusFlag(STATUS_FLAG_SWIMMING))
		return 1;

	setAnimation(ANIM_PADDLE_SWIM_WAIT, 1.0f);

	if (gpMSound->gateCheck(0x1950))
		MSoundSESystem::MSRandPlay::startSeRandPlay(0x1950, 0);
	return 0;
}

BOOL TMario::swimWaitToPaddle()
{
	setAnimation(ANIM_WAIT_TO_PADDLE, 1.0f);

	if (isLast1AnimeFrame())
		changePlayerStatus(STATUS_SWIM_PADDLE_START, 0, false);

	doSwimming();

	if (!checkStatusFlag(STATUS_FLAG_SWIMMING))
		return 1;

	return 0;
}

BOOL TMario::swimPaddleStart()
{
	setAnimation(ANIM_PADDLE_START, 1.0f);
	mForwardVel += mSwimParams.mPaddleSpeedUp.get();
	mVel.y += mSwimParams.mPaddleJumpUp.get();

	if (isLast1AnimeFrame())
		changePlayerStatus(STATUS_SWIM_PADDLE, 0, false);

	doSwimming();

	if (!checkStatusFlag(STATUS_FLAG_SWIMMING))
		return 1;

	return 0;
}

BOOL TMario::swimPaddle()
{
	f32 anmRate = 0.5f;
	if (checkFlag(MARIO_FLAG_FLUDD_EMITTING))
		anmRate = 5.0f;
	setAnimation(ANIM_PADDLE_SWIM, anmRate);

	if (checkFlag(MARIO_FLAG_FLUDD_EMITTING)) {
		setPlayerVelocity(mDeParams.mDashMax.get());
		startSoundActor(0x19);
		startSoundActor(0x117d);
	}

	if (mIntendedMag == 0.0f)
		changePlayerStatus(STATUS_SWIM_PADDLE_END, 0, false);

	doSwimming();

	if (!checkStatusFlag(STATUS_FLAG_SWIMMING))
		return 1;

	if (checkFlag(MARIO_FLAG_FLUDD_EMITTING) && !isUnderWater())
		surfingEffect();

	return 0;
}

BOOL TMario::swimPaddleEnd()
{
	setAnimation(ANIM_PADDLE_END, 1.0f);

	if (isLast1AnimeFrame())
		return changePlayerStatus(STATUS_SWIM_PADDLE_END_TO_WAIT, 0, false);

	doSwimming();

	if (!checkStatusFlag(STATUS_FLAG_SWIMMING))
		return 1;

	return 0;
}

BOOL TMario::swimPaddleEndToWait()
{
	setAnimation(ANIM_PADDLE_TO_WAIT, 1.0f);

	if (isLast1AnimeFrame())
		return changePlayerStatus(STATUS_SWIM_WAIT, 0, false);

	doSwimming();

	if (!checkStatusFlag(STATUS_FLAG_SWIMMING))
		return 1;

	return 0;
}

BOOL TMario::swimUp()
{
	setAnimation(ANIM_FLOAT, 1.0f);
	mVel.y += mSwimParams.mFloatUp.get();

	if (isLast1AnimeFrame()) {
		setAnimation(ANIM_PADDLE_SWIM_WAIT, 1.0f);
		return changePlayerStatus(STATUS_SWIM_WAIT, 0, false);
	}

	doSwimming();

	if (!checkStatusFlag(STATUS_FLAG_SWIMMING))
		return 1;

	return 0;
}

BOOL TMario::swimDive()
{
	setAnimation(ANIM_SWIM_DIVE, 1.0f);

	if (getMotionFrameCtrl().checkPass(16.0f))
		unk366 = mSwimParams.mWaitSinkTime.get();

	if (unk366 > 0) {
		unk366 -= 1;
		mVel.y -= mSwimParams.mWaitSinkSpeed.get();
	}

	if (isLast1AnimeFrame()) {
		setAnimation(ANIM_PADDLE_SWIM_WAIT, 1.0f);
		return changePlayerStatus(STATUS_SWIM_PADDLE_END, 0, false);
	}

	doSwimming();

	if (!checkStatusFlag(STATUS_FLAG_SWIMMING))
		return 1;

	return 0;
}

BOOL TMario::swimPDamage()
{
	doSwimming();
	jumpingDemoCommon(STATUS_SWIM_P_DAMAGE, ANIM_SWIM_P_DAMAGE, 0.0f);

	if (isLast1AnimeFrame())
		changePlayerStatus(STATUS_SWIM_WAIT, 0, false);

	return 0;
}

BOOL TMario::swimDown() { }

BOOL TMario::swimPDown() { }

static int unknown_inline_10(TMario* mario)
{
	mario->doSwimming();
	mario->jumpingDemoCommon(0x224e0, TMario::ANIM_SWIM_DOWN, 0.0f);
	return 0;
}

static int unknown_inline_11(TMario* mario)
{
	mario->doSwimming();
	mario->jumpingDemoCommon(0x224e1, TMario::ANIM_SWIM_P_DOWN, 0.0f);
	return 0;
}

int TMario::swimMain()
{
	if (checkFlag(MARIO_FLAG_GAME_OVER))
		changePlayerStatus(0x224e1, 0, false);

	if (checkSwimJump() == 1)
		return 0;

	int result = checkSwimToHangFence();

	f32 limit = mFloorPosition.z - mSwimParams.mFloatHeight.get();
	if (mPosition.y >= limit)
		mPosition.y = limit;

	unk2A8.y = mFloorPosition.z;

	if (checkFlag(MARIO_FLAG_FLUDD_EMITTING))
		if (mStatus != STATUS_SWIM_PADDLE_START
		    && mStatus != STATUS_SWIM_PADDLE)
			return changePlayerStatus(STATUS_SWIM_PADDLE_START, 0, false);

	switch (mStatus) {
	case STATUS_SWIM_START:
		return swimStart();

	case STATUS_SWIM_WAIT:
		return swimWait();

	case STATUS_SWIM_WAIT_TO_PADDLE:
		return swimWaitToPaddle();

	case STATUS_SWIM_PADDLE_START:
		return swimPaddleStart();

	case STATUS_SWIM_PADDLE:
		return swimPaddle();

	case STATUS_SWIM_PADDLE_END:
		return swimPaddleEnd();

	case STATUS_SWIM_PADDLE_END_TO_WAIT:
		return swimPaddleEndToWait();

	case STATUS_SWIM_UP:
		return swimUp();

	case STATUS_SWIM_DIVE:
		return swimDive();

	case STATUS_SWIM_P_DAMAGE:
		return swimPDamage();

	case 0x224e0:
		return unknown_inline_10(this);

	case 0x224e1:
		return unknown_inline_11(this);
	}

	return result;
}
