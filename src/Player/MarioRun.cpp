#include <Player/Mario.hpp>
#include <Player/WaterGun.hpp>
#include <Player/NozzleTrigger.hpp>
#include <System/Particles.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/PollutionLayer.hpp>
#include <Map/Map.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
static const char cDirtyTexName[]  = "H_ma_rak_dummy";

BOOL TMario::considerRotateStart()
{
	int direction;
	if (checkStickRotate(&direction) == 1 && mWaterGun != nullptr
	    && mWaterGun->isEmitting()) {
		if (direction > 0)
			return changePlayerStatus(MARIO_STATUS_ROTATE_L, 0, false);
		else
			return changePlayerStatus(MARIO_STATUS_ROTATE_R, 0, false);
	}

	return 0;
}

static int unknown_inline_10(TMario* mario)
{
	if ((mario->mInput & 0x8000)
	    && mario->mForwardVel > mario->mRunParams.mDoJumpCatchSp.get()
	    && mario->mIntendedMag > 0.75f) {
		mario->mVel.y = 20.0f;
		return mario->changePlayerStatus(MARIO_STATUS_JUMP_CATCH, 1, false);
	}

	return 0;
}

BOOL TMario::isThrowStart()
{
	if (mHeldObject != nullptr && ((mInput & 0x2000) ? true : false)) {

		if (mHeldObject->checkActorType(0x10000000))
			return changePlayerStatus(MARIO_STATUS_PITCHING, 0, false);

		switch (mHeldObject->getActorType()) {
		case 0x80000001:
			return changePlayerStatus(MARIO_STATUS_PITCHING, 0, false);

		default:
			if (mForwardVel > 16.0f)
				return changePlayerStatus(MARIO_STATUS_PITCHING, 0, false);
			if (canPut())
				return changePlayerStatus(MARIO_STATUS_PUTTING, 0, false);
			break;
		}
	}

	return 0;
}

void TMario::postureControl() { }

void TMario::clashStandard(u32, u32) { }

void TMario::changePlayerPower(f32, u32, u32) { }

BOOL TMario::isRunningSlipStart()
{
	if ((mInput & 0x8) && (mForwardVel <= 0.1f || isFrontSlip(0)))
		return true;
	else
		return false;
}

BOOL TMario::isRunningTurnning()
{
	if (isUpperPumpingStyle())
		return false;

	if (onYoshi())
		return false;

	bool result = true;
	s16 yawDiff = mIntendedYaw - mFaceAngle.y;
	if (-0x471C <= yawDiff && yawDiff <= 0x471C)
		result = false;

	return result;
}

void TMario::changePlayerCatching() { }

bool TMario::isRunningInWater()
{
	if (checkFlag(MARIO_FLAG_IN_ANY_WATER)
	    && (mFloorPosition.z < mPosition.y + mRunParams.mSwimDepth.get()))
		return true;

	return false;
}

void TMario::getRunningInWaterBrake() { }

BOOL TMario::doRunningAnimation()
{
	BOOL loop = true;

	f32 rate;
	f32 sp;

	sp = mIntendedMag > mForwardVel ? mForwardVel : mIntendedMag;

	if (sp < 4.0f)
		sp = 4.0f;

	while (loop) {
		switch (mAnimationId) {
		default:
		case ANIM_RUN2:
			if (mForwardVel >= mDeParams.mDashMax.get() - 1.0f) {
				setAnimation(ANIM_TURBO_DASH, 1.0f);
				loop = false;
			} else if (sp < mRunParams.mWalk2Soft.get()) {
				setAnimation(ANIM_SSTEP, 1.0f);
				loop = false;
			} else {
				rate = sp * mRunParams.mRunAnmSpeedMult.get()
				       + mRunParams.mRunAnmSpeedBase.get();
				if (isRunningInWater()) {
					f32 tmp = (mFloorPosition.z - mPosition.y)
					          / mRunParams.mSwimDepth.get();
					rate *= (1.0f
					         - tmp * (1.0f - mRunParams.mInWaterBrake.get()))
					        * mRunParams.mInWaterAnmBrake.get();
				}
				setAnimation(ANIM_RUN2, rate);
				f32 lo = mRunParams.mMotBlendWalkSp.get();
				f32 hi = mRunParams.mMotBlendRunSp.get();
				f32 mix;
				if (rate < lo)
					mix = 0.0f;
				if (hi < rate)
					mix = 1.0f;
				if (lo <= rate && rate <= hi)
					mix = (rate - lo) / (hi - lo);
				unk414.z = 1.0f - mix;
				if (onYoshi()) {
					mModel->unk20->unk18->unk50 = 0.0f;
				} else {
					mModel->unk20->unk18->unk50 = unk414.z;
				}
				loop = false;
			}
			break;
		case ANIM_SSTEP:
			if (sp > mRunParams.mSoft2Walk.get()) {
				setAnimation(ANIM_RUN2, 1.0f);
				loop = false;
			} else {
				f32 rate2 = sp;
				if (rate2 < 0.1f)
					rate2 = 0.1f;
				rate2 *= mRunParams.mSoftStepAnmMult.get();
				setAnimation(ANIM_SSTEP, rate2);
				loop = false;
			}
			break;
		case ANIM_TURBO_DASH:
			if (mForwardVel < mDeParams.mDashMax.get() - 1.0f) {
				setAnimation(ANIM_RUN2, 1.0f);
				loop = false;
			} else {
				f32 sp2 = sp * mRunParams.mRunAnmSpeedMult.get()
				          + mRunParams.mRunAnmSpeedBase.get();
				setAnimation(ANIM_TURBO_DASH, sp2);
				loop = false;
			}
			break;
		}
	}
	return 1;
}

void TMario::getSlopeNormalAccele(f32* arg0, f32* arg1)
{
	if (isForceSlip()) {
		*arg0 = mSlipParamsAll.mSlopeAcceleUp.get();
		*arg1 = mSlipParamsAll.mSlopeAcceleDown.get();
		return;
	}

	if (mGroundPlane->isSlider()) {
		*arg0 = mSlipParamsAllSlider.mSlopeAcceleUp.get();
		*arg1 = mSlipParamsAllSlider.mSlopeAcceleDown.get();
		return;
	}

	if (mGroundPlane->isUnk2()) {
		*arg0 = mSlipParams45.mSlopeAcceleUp.get();
		*arg1 = mSlipParams45.mSlopeAcceleDown.get();
		return;
	}

	if (mGroundPlane->isWetGround()) {
		if (mGroundPlane->mNormal.y > 0.99f) {
			*arg0 = mSlipParamsWaterGround.mSlopeAcceleUp.get();
			*arg1 = mSlipParamsWaterGround.mSlopeAcceleDown.get();
		} else {
			*arg0 = mSlipParamsWaterSlope.mSlopeAcceleUp.get();
			*arg1 = mSlipParamsWaterSlope.mSlopeAcceleDown.get();
		}
		return;
	}

	*arg0 = mSlipParamsNormal.mSlopeAcceleUp.get();
	*arg1 = mSlipParamsNormal.mSlopeAcceleDown.get();
}

void TMario::getSlopeSlideAccele(f32* arg0, f32* arg1)
{
	if (isForceSlip()) {
		*arg0 = mSlipParamsAll.mSlideAcceleUp.get();
		*arg1 = mSlipParamsAll.mSlideAcceleDown.get();
		return;
	}

	if (mGroundPlane->isSlider()) {
		*arg0 = mSlipParamsAllSlider.mSlideAcceleUp.get();
		*arg1 = mSlipParamsAllSlider.mSlideAcceleDown.get();
		return;
	}

	if (mGroundPlane->isUnk2()) {
		*arg0 = mSlipParams45.mSlideAcceleUp.get();
		*arg1 = mSlipParams45.mSlideAcceleDown.get();
		return;
	}

	if (mGroundPlane->isWetGround()) {
		if (mGroundPlane->mNormal.y > 0.99f) {
			*arg0 = mSlipParamsWaterGround.mSlideAcceleUp.get();
			*arg1 = mSlipParamsWaterGround.mSlideAcceleDown.get();
		} else {
			*arg0 = mSlipParamsWaterSlope.mSlideAcceleUp.get();
			*arg1 = mSlipParamsWaterSlope.mSlideAcceleDown.get();
		}
		return;
	}
	*arg0 = mSlipParamsNormal.mSlideAcceleUp.get();
	*arg1 = mSlipParamsNormal.mSlideAcceleDown.get();
}

f32 TMario::getChangeAngleSpeed()
{
	f32 angSp;
	if (isForceSlip()) {
		angSp = (f32)mSlipParamsAll.mSlideAngleYSp.get();
	} else {
		if (mGroundPlane->isSlider()) {
			angSp = (f32)mSlipParamsAllSlider.mSlideAngleYSp.get();
		} else if (mGroundPlane->isUnk2()) {
			angSp = (f32)mSlipParams45.mSlideAngleYSp.get();
		} else if (mGroundPlane->isWetGround()) {
			if (mGroundPlane->mNormal.y > 0.99f) {
				angSp = (f32)mSlipParamsWaterGround.mSlideAngleYSp.get();
			} else {
				angSp = (f32)mSlipParamsWaterSlope.mSlideAngleYSp.get();
			}
		} else {
			angSp = (f32)mSlipParamsNormal.mSlideAngleYSp.get();
		}
	}
	return 0.03125f * (angSp * mForwardVel);
}

f32 TMario::getSlideStickMult()
{
	(void)0;
	(void)0;
	(void)0;
	(void)0;
	(void)0;
	(void)0;
	(void)0;
	(void)0;
	(void)0;
	(void)0;
	(void)0;

	if (isForceSlip())
		return mSlipParamsAll.mStickSlideMult.get();

	if (mGroundPlane->isSlider())
		return mSlipParamsAllSlider.mStickSlideMult.get();

	if (mGroundPlane->isUnk2())
		return mSlipParams45.mStickSlideMult.get();

	return mSlipParamsNormal.mStickSlideMult.get();
}

void TMario::slideProcess(f32 baseAcc, f32 friction)
{
	const TBGCheckData* ground = mGroundPlane;

	s16 dirAng = matan(ground->getNormal().z, ground->getNormal().x);

	f32 mag = MsSqrtf(ground->getNormal().x * ground->getNormal().x
	                  + ground->getNormal().z * ground->getNormal().z);

	s16 angDiff = mSlopeAngle - mFaceAngle.y;
	f32 slopeUp;
	f32 slopeDown;
	getSlopeSlideAccele(&slopeUp, &slopeDown);
	f32 acc;
	if (angDiff > -0x4000 && angDiff < 0x4000)
		acc = slopeUp * mag + baseAcc;
	else
		acc = slopeDown * mag + baseAcc;

	mSlideVelX += acc * JMASSin(dirAng);
	mSlideVelZ += acc * JMASCos(dirAng);
	mSlideVelX *= friction;
	mSlideVelZ *= friction;
	unk9E = matan(mSlideVelZ, mSlideVelX);

	int turn;
	if (!(-0.1f < mSlideVelX && mSlideVelX < 0.1f && -0.1f < mSlideVelZ
	      && mSlideVelZ < 0.1f)) {
		turn = (s16)(mFaceAngle.y - unk9E);
		if (turn > 0 && turn <= 0x4000) {
			turn = ((f32)turn - getChangeAngleSpeed());
			if (turn < 0)
				turn = 0;
		} else if (turn > -0x4000 && turn < 0) {
			turn = ((f32)turn + getChangeAngleSpeed());
			if (turn > 0)
				turn = 0;
		} else if (turn > 0x4000 && turn < 0x8000) {
			turn = ((f32)turn + getChangeAngleSpeed());
			if (turn > 0x8000)
				turn = 0x8000;
		} else if (turn > -0x8000 && turn < -0x4000) {
			turn = ((f32)turn - getChangeAngleSpeed());
			if (turn < -0x8000)
				turn = -0x8000;
		}
		mFaceAngle.y = unk9E + turn;
	}

	mVel.x      = mSlideVelX;
	mVel.y      = 0.0f;
	mVel.z      = mSlideVelZ;
	mForwardVel = MsSqrtf(mSlideVelX * mSlideVelX + mSlideVelZ * mSlideVelZ);

	if (mForwardVel > 100.0f) {
		mSlideVelX = (100.0f * mSlideVelX) / mForwardVel;
		mSlideVelZ = (100.0f * mSlideVelZ) / mForwardVel;
	}

	if (turn < -0x4000 || turn > 0x4000)
		mForwardVel *= -1.0f;
}

BOOL TMario::doSliding(f32 stopThreshold)
{
	BOOL stopped = false;
	f32 cs       = JMASCos((s16)(mIntendedYaw - unk9E));
	f32 sn       = JMASSin((s16)(mIntendedYaw - unk9E));
	if (cs < 0.0f && mForwardVel >= 0.0f)
		cs *= 0.01f * (0.5f * mForwardVel) + 0.5f;

	f32 slipFr;
	if (mStatus == MARIO_STATUS_OIL_SLIP) {
		slipFr = mSlipParamsOil.mSlipFriction.get();
	} else if (isForceSlip()) {
		slipFr = mSlipParamsAll.mSlipFriction.get();
	} else if (mGroundPlane->isSlider()) {
		slipFr = mSlipParamsAllSlider.mSlipFriction.get();
	} else if (mGroundPlane->isUnk2()) {
		slipFr = mSlipParams45.mSlipFriction.get();
	} else if (mGroundPlane->isWetGround()) {
		if (mGroundPlane->mNormal.y > 0.99f)
			slipFr = mSlipParamsWaterGround.mSlipFriction.get();
		else
			slipFr = mSlipParamsWaterSlope.mSlipFriction.get();
	} else if (onYoshi()) {
		slipFr = mSlipParamsYoshi.mSlipFriction.get();
	} else {
		slipFr = mSlipParamsNormal.mSlipFriction.get();
		if (mStatus == MARIO_STATUS_CATCH) {
			if (mStatusState == 1)
				slipFr = mDeParams.mWasOnWaterSlip.get();
			if (checkFlag(MARIO_FLAG_IN_ANY_WATER))
				slipFr = mDeParams.mInWaterSlip.get();
		}
	}

	f32 mult   = (0.02f * (mIntendedMag * 0.03125f * cs)) + slipFr;
	f32 oldMag = MsSqrtf(mSlideVelX * mSlideVelX + mSlideVelZ * mSlideVelZ);

	mSlideVelX
	    += sn * (mSlideVelZ * (mIntendedMag * 0.03125f)) * getSlideStickMult();
	mSlideVelZ = -(
	    (sn * (mSlideVelX * (mIntendedMag * 0.03125f)) * getSlideStickMult())
	    - mSlideVelZ);

	f32 newMag = MsSqrtf(mSlideVelX * mSlideVelX + mSlideVelZ * mSlideVelZ);
	if (oldMag > 0.0f && newMag > 0.0f) {
		mSlideVelX = (mSlideVelX * oldMag) / newMag;
		mSlideVelZ = (mSlideVelZ * oldMag) / newMag;
	}

	slideProcess(0.0f, mult);

	if (!mGroundPlane->isUnk1() && !mGroundPlane->isSlider()) {
		if (mForwardVel * mForwardVel < stopThreshold * stopThreshold) {
			setPlayerVelocity(0.0f);
			stopped = true;
			mInput &= ~0x8;
		}
	}

	return stopped;
}

void TMario::slopeProcess()
{
	f32 mag = std::sqrtf(
	    mGroundPlane->getNormal().x * mGroundPlane->getNormal().x
	    + mGroundPlane->getNormal().z * mGroundPlane->getNormal().z);

	s16 angDiff = mSlopeAngle - mFaceAngle.y;
	f32 slopeUp;
	f32 slopeDown;
	getSlopeNormalAccele(&slopeUp, &slopeDown);
	if (angDiff > -0x4000 && angDiff < 0x4000) {
		mForwardVel += slopeUp * mag;
	} else {
		mForwardVel = -(slopeDown * mag - mForwardVel);
	}

	if (mForwardVel > mDeParams.mRunningMax.get())
		mForwardVel = mDeParams.mRunningMax.get();

	unk9E      = mFaceAngle.y;
	mSlideVelX = mForwardVel * JMASSin(mFaceAngle.y);
	mSlideVelZ = mForwardVel * JMASCos(mFaceAngle.y);
	mVel.x     = mSlideVelX;
	mVel.y     = 0.0f;
	mVel.z     = mSlideVelZ;
}

void TMario::doSlipping(f32) { }

void TMario::doStopping() { }

void TMario::doRunning()
{
	f32 sp = mIntendedMag < mRunParams.mMaxSpeed.get()
	             ? mRunParams.mMaxSpeed.get()
	             : mIntendedMag;

	if (onYoshi())
		sp *= mYoshiParams.mRunYoshiMult.get();

	if (mForwardVel <= 0.0f) {
		mForwardVel += mRunParams.mVelMinusBrake.get();
	} else if (mForwardVel <= sp) {
		mForwardVel += mRunParams.mAddBase.get()
		               - mForwardVel * mRunParams.mAddVelDiv.get();
	} else if (mGroundPlane->mNormal.y >= mRunParams.mDecStartNrmY.get()) {
		mForwardVel -= mRunParams.mDecBrake.get();
		mForwardVel -= mYoshiParams.mDecBrake.get();
	}

	if (mForwardVel < 0.0f)
		mForwardVel = 0.0f;

	s16 rotSp;
	if (isUpperPumpingStyle()) {
		s16 base = mDeParams.mPumpingRotSpMin.get();
		rotSp    = (s16)(0.03125f
                          * (mForwardVel
                             * (mDeParams.mPumpingRotSpMax.get() - base))
                      + base);
	} else {
		s16 base = mDeParams.mRunningRotSpMin.get();
		rotSp    = (s16)(0.03125f
                          * (mForwardVel
                             * (mDeParams.mRunningRotSpMax.get() - base))
                      + base);
	}

	if (onYoshi())
		rotSp = (s16)((f32)rotSp * mYoshiParams.mRotYoshiMult.get());

	if (checkFlag(MARIO_FLAG_FLUDD_EMITTING))
		rotSp = mDeParams.mRunningRotSpMin.get();

	if (isRunningInWater()) {
		mForwardVel *= -(
		    (((mFloorPosition.z - mPosition.y) / mRunParams.mSwimDepth.get())
		     * (1.0f - mRunParams.mInWaterBrake.get()))
		    - 1.0f);
	}

	mFaceAngle.y
	    = mIntendedYaw
	      - IConverge((s16)(mIntendedYaw - mFaceAngle.y), 0, rotSp, rotSp);

	slopeProcess();
}

TMario::TSurfingParams* TMario::getSurfingParamsWater()
{
	switch (mSurfGessoType) {
	case SURF_GESSO_TYPE_YELLOW:
		return &mSurfingParamsWaterYellow;
	case SURF_GESSO_TYPE_GREEN:
		return &mSurfingParamsWaterGreen;
	default:
		return &mSurfingParamsWaterRed;
	}
}

TMario::TSurfingParams* TMario::getSurfingParamsGround()
{
	switch (mSurfGessoType) {
	case SURF_GESSO_TYPE_YELLOW:
		return &mSurfingParamsGroundYellow;
	case SURF_GESSO_TYPE_GREEN:
		return &mSurfingParamsGroundGreen;
	default:
		return &mSurfingParamsGroundRed;
	}
}

void TMario::doSurfing()
{
	const TBGCheckData* below;
	gpMap->checkGround(mPosition.x, mPosition.y - mVel.y, mPosition.z, &below);

	f32 rotMin;
	f32 rotMax;
	f32 powMin;
	f32 powMax;

	if (below->isWaterSurface()) {
		rotMin = getSurfingParamsWater()->mRotMin.get();
		rotMax = getSurfingParamsWater()->mRotMax.get();
		powMin = getSurfingParamsWater()->mPowMin.get();
		powMax = getSurfingParamsWater()->mPowMax.get();
	} else {
		rotMin = getSurfingParamsGround()->mRotMin.get();
		rotMax = getSurfingParamsGround()->mRotMax.get();
		powMin = getSurfingParamsGround()->mPowMin.get();
		powMax = getSurfingParamsGround()->mPowMax.get();
	}

	f32 want = 2.0f * mIntendedMag;
	if (want > powMax)
		want = powMax;
	if (want < powMin)
		want = powMin;

	if (mForwardVel <= 0.0f) {
		mForwardVel += 1.1f;
	} else if (mForwardVel <= want) {
		f32 accel;
		if (mGroundPlane->isWaterSurface())
			accel = getSurfingParamsWater()->mAccel.get();
		else
			accel = getSurfingParamsGround()->mAccel.get();
		mForwardVel += 1.1f - (mForwardVel / accel);
	} else if (mGroundPlane->mNormal.y >= 0.95f) {
		mForwardVel -= 1.0f;
	}

	if (mForwardVel > powMax)
		mForwardVel = powMax;

	s16 rotSp
	    = (((want - powMin) / (powMax - powMin)) * (rotMax - rotMin)) + rotMin;
	s16 diff     = mIntendedYaw - mFaceAngle.y;
	mFaceAngle.y = mIntendedYaw - IConverge(diff, 0, rotSp, rotSp);
	slopeProcess();

	if (below->isWaterSurface())
		surfingEffect();
}

void TMario::doBraking(f32) { }

void TMario::changePlayerWaiting() { }

void TMario::doPushingAnimation(const Vec& vec)
{
	f32 dx = mPosition.x - vec.x;
	f32 dz = mPosition.z - vec.z;
	if (mForwardVel > 6.0f)
		setPlayerVelocity(6.0f);

	s16 wallAngle;
	s16 angDiff;
	if (mWallPlane != nullptr) {
		wallAngle = getWallAngle();
		angDiff   = wallAngle - mFaceAngle.y;
	}

	if (mWallPlane == nullptr || angDiff < -0x71C7 || angDiff > 0x71C7) {
		setAnimation(ANIM_PUSH, 1.0f);
		startVoice(MSD_SE_MV16_EXERT_CONT_01);
		return;
	}

	f32 rate = 2.0f * MsSqrtf(dx * dx + dz * dz);
	if (angDiff < 0)
		setAnimation(ANIM_SWLKR, rate);
	else
		setAnimation(ANIM_SWLKL, rate);

	mFaceAngle.x    = 0;
	mModelFaceAngle = wallAngle + 0x8000;
}

BOOL TMario::running()
{
	mStatusTimer++;

	if (isThrowStart())
		return 1;

	if (isRunningSlipStart())
		return changePlayerStatus(MARIO_STATUS_SLIP, 0, false);

	if (mInput & 0x10) {
		if (mStatusState == 1) {
			mFaceAngle.y = (s16)mStatusArg;
			return changePlayerStatus(0xC400209, 0, false);
		}
		if (mStatusTimer > 0xF0 && mForwardVel >= 16.0f
		    && mGroundPlane->mNormal.y <= 0.17364818f) {
			return changePlayerStatus(MARIO_STATUS_BRAKE, 0, false);
		}
		return changePlayerStatus(MARIO_STATUS_WALK_END, 0, false);
	}

	if (checkFlag(MARIO_FLAG_FLUDD_EMITTING) && (mInput & 0x2)
	    && mForwardVel > mDeParams.mDashMax.get() - 1.0f)
		return changePlayerJumping(MARIO_STATUS_BROAD_JUMP, 0);

	if (mInput & 0x2)
		return changePlayerTriJump();

	if (!onYoshi() && (mInput & 0x8000)) {
		if (unknown_inline_10(this))
			return 1;
		changePlayerStatus(MARIO_STATUS_TAKE_POSE, 0, false);
	}

	if (mInput & 0x20) {
		if (mStatusState == 1) {
			mFaceAngle.y = (s16)mStatusArg;
			return changePlayerStatus(0xC400209, 0, false);
		}
		if (mStatusTimer > 0xF0 && mForwardVel >= 16.0f
		    && mGroundPlane->mNormal.y <= 0.17364818f) {
			return changePlayerStatus(MARIO_STATUS_BRAKE, 0, false);
		}
		return changePlayerStatus(MARIO_STATUS_WALK_END, 0, false);
	}

	if (isRunningTurnning() && mForwardVel >= mRunParams.mTurnNeedSp.get()) {
		emitParticle(PARTICLE_MS_MARIWALK1_A, mFaceAngle.y + 0x8000);
		emitParticle(PARTICLE_MS_MARIWALK1_C, mFaceAngle.y + 0x8000);
		emitParticle(PARTICLE_MS_MARIWALK1_B, mFaceAngle.y + 0x8000);
		return changePlayerStatus(MARIO_STATUS_TURN, 0, false);
	}

	if (canSquat()) {
		setPlayerVelocity(0.0f);
		return changePlayerStatus(MARIO_STATUS_SQUAT, 0, false);
	}

	if (rocketCheck()) {
		unk314
		    = mFloorPosition.y + mWaterGun->mWatergunParams.mHoverHeight.get();
		return changePlayerStatus(MARIO_STATUS_ROCKET, 0, false);
	}

	bool pushed                   = false;
	mStatusState                  = 0;
	JGeometry::TVec3<f32> prevPos = mPosition;
	doRunning();
	setNormalAttackArea();

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
		setAnimation(ANIM_LAND, 1.0f);
		break;
	case 1:
		doRunningAnimation();
		break;
	case 2:
		if (onYoshi()) {
			setPlayerVelocity(0.0f);
			break;
		}

		if (mFlag & MARIO_FLAG_VISIBLE)
			pushed = true;

		if (!pushed) {
			if (mForwardVel > mDeParams.mClashSpeed.get()) {
				emitParticle(PARTICLE_MS_DMG_C);
				return changePlayerDropping(MARIO_STATUS_JUMP_SHORT_BACK_DOWN,
				                            0);
			}

			if ((mInput & 0x2)
			    && gpMap->isTouchedOneWall(
			           mPosition.x,
			           mPosition.y + mDeParams.mJumpWallHeight.get(),
			           mPosition.z, 50.0f)
			           == true) {
				mVel.y = 52.0f;
				mFaceAngle.y += 0x8000;
				setPlayerVelocity(50.0f);
				return changePlayerStatus(MARIO_STATUS_WALL_JUMP, 0, false);
			}

			if (mWallPlane != nullptr && mWallPlane->isFence()) {
				mFaceAngle.y = matan(mWallPlane->getNormal().z,
				                     mWallPlane->getNormal().x)
				               + 0x8000;
				mModelFaceAngle = mFaceAngle.y;
				return changePlayerStatus(MARIO_STATUS_FENCE_CATCH, 0, false);
			}

			doPushingAnimation(prevPos);
			mDashTimer = 0;
			offFlag(MARIO_FLAG_FLUDD_EMITTING);
		}
		break;
	}

	checkDescent();
	if (checkFlag(MARIO_FLAG_FLUDD_EMITTING)) {
		setPlayerVelocity(mDeParams.mDashMax.get());
		startSoundActor(MSD_SE_PO_SPREAD);
	}
	return 0;
}

BOOL TMario::rotating()
{
	if (mInput & 0x2) {
		if (mStatus == MARIO_STATUS_ROTATE_L)
			return changePlayerStatus(MARIO_STATUS_RIGHT_ROTATE_JUMP, 0, false);
		else
			return changePlayerStatus(MARIO_STATUS_LEFT_ROTATE_JUMP, 0, false);
	}

	setAnimation(ANIM_SPIN_P, 1.0f);
	emitRotateShootEffect();
	emitBlurSpinJump();

	mStatusTimer++;
	if (mStatusTimer > 0x78)
		return changePlayerStatus(MARIO_STATUS_WAIT, 0, false);

	doRunning();

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
		break;
	}

	if (mStatus == MARIO_STATUS_ROTATE_L)
		mModelFaceAngle = mStatusTimer * 4096;
	else
		mModelFaceAngle = -(mStatusTimer * 4096);

	return 0;
}

BOOL TMario::turnning()
{
	if (isThrowStart())
		return 1;

	if (mInput & 0x8)
		return changePlayerStatus(MARIO_STATUS_SLIP, 0, false);

	if (mInput & 0x2)
		return changePlayerJumping(MARIO_STATUS_U_TURN_JUMP, 0);

	if (considerRotateStart())
		return true;

	if (mInput & 0x20)
		return changePlayerStatus(MARIO_STATUS_BRAKE, 0, false);

	if (!isRunningTurnning())
		return changePlayerStatus(MARIO_STATUS_RUN, 0, false);

	BOOL zeroed = false;
	f32 v       = FConverge(mForwardVel, 0.0f, 4.0f, 4.0f);
	mForwardVel = v;
	if (v == 0.0f)
		zeroed = true;

	slopeProcess();
	if (zeroed) {
		mFaceAngle.y = mIntendedYaw;
		setPlayerVelocity(8.0f);
		return changePlayerStatus(MARIO_STATUS_TURN_END, 0, false);
	}

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
		break;
	}

	if (mForwardVel >= 18.0f) {
		setAnimation(ANIM_TURN, 1.0f);
	} else {
		setAnimation(ANIM_TRNED, 1.0f);
		if (isLast1AnimeFrame()) {
			f32 vel = mForwardVel;
			if (vel > 0.0f) {
				mFaceAngle.y = mIntendedYaw;
				setPlayerVelocity(-vel);
				changePlayerStatus(MARIO_STATUS_RUN, 0, false);
			} else {
				mFaceAngle.y = mIntendedYaw;
				setPlayerVelocity(8.0f);
				changePlayerStatus(MARIO_STATUS_RUN, 0, false);
			}
		}
	}
	return 0;
}

BOOL TMario::turnEnd()
{
	if (isThrowStart())
		return 1;

	if (mInput & 0x8)
		return changePlayerStatus(MARIO_STATUS_SLIP, 0, false);

	if (mInput & 0x2)
		return changePlayerJumping(MARIO_STATUS_U_TURN_JUMP, 0);

	if (considerRotateStart())
		return true;

	doRunning();
	setAnimation(ANIM_TRNED, 1.0f);

	if (!walkProcess())
		changePlayerStatus(MARIO_STATUS_LANDING, 0, false);

	if (isLast1AnimeFrame())
		changePlayerStatus(MARIO_STATUS_RUN, 0, false);

	mFaceAngle.x = 0;
	mModelFaceAngle += 0x8000;
	return 0;
}

// TODO: remove the inline mark here!!!
inline BOOL TMario::braking()
{
	if (!(mInput & 0x10) && (mInput & 0xF))
		return checkAllMotions();

	// TODO: inline
	BOOL zeroed = 0;
	f32 tmp     = FConverge(mForwardVel, 0.0f, 4.0f, 4.0f);
	mForwardVel = tmp;
	if (tmp == 0.0f)
		zeroed = 1;

	slopeProcess();
	if (zeroed)
		return changePlayerStatus(MARIO_STATUS_BRAKE_END, 0, false);

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
		break;

	case 1:
		break;

	case 2:
		if (mForwardVel > 16.0f) {
			playerRefrection(1);
			changePlayerDropping(MARIO_STATUS_SHORT_BACK_DOWN, 0);
		} else {
			setPlayerVelocity(0.0f);
			changePlayerStatus(MARIO_STATUS_BRAKE_END, 0, false);
		}
		break;
	}
	setAnimation(ANIM_BRAKE, 1.0f);
	return 0;
}

BOOL TMario::surfing()
{
	setAnimation(ANIM_RIDE_SHELL, 1.0f);
	if (mStatusTimer != 0) {
		mStatusTimer--;
		return 0;
	}

	if (mInput & 0x2) {
		mPosition.y += 1.0f;
		return changePlayerStatus(0x281089A, 0, false);
	}

	doSurfing();
	walkProcess();

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(0x81089B, 0, false);
		break;
	case 2: {
		if (mWallPlane == nullptr) {
			setPlayerVelocity(0.0f);
			loserExec();
			break;
		}

		s16 wallToFace
		    = matan(mWallPlane->getNormal().z, mWallPlane->getNormal().x)
		      - mFaceAngle.y;

		s16 maxAngle;
		f32 minSpeed;

		if (mWallPlane->isWaterSurface()) {
			maxAngle = getSurfingParamsWater()->mClashAngle.get();
			minSpeed = getSurfingParamsWater()->mClashSpeed.get();
		} else {
			maxAngle = getSurfingParamsGround()->mClashAngle.get();
			minSpeed = getSurfingParamsGround()->mClashSpeed.get();
		}

		if ((wallToFace < -maxAngle || maxAngle < wallToFace)
		    && mForwardVel > minSpeed) {
			decHP(mDeParams.mHpMax.get());
			BOOL ret = changePlayerStatus(MARIO_STATUS_JUMP_BACK_DOWN, 0, true);
			mForwardVel = 0.8f * -mForwardVel;
			mVel.y      = 50.0f;
			return ret;
		}
		setPlayerVelocity(0.0f);
		break;
	}
	}

	return 0;
}

BOOL TMario::toroccoing()
{
	soundTorocco();
	toroccoEffect();
	return 0;
}

BOOL TMario::walkEnd()
{
	if (!(mInput & 0x10)) {
		if (isRunningSlipStart())
			return changePlayerStatus(MARIO_STATUS_SLIP, 0, false);

		if (mInput & 0x2)
			return changePlayerTriJump();

		if (mInput & 0x1)
			return changePlayerStatus(MARIO_STATUS_RUN, 0, false);

		if (mInput & 0x8000)
			return changePlayerStatus(MARIO_STATUS_TAKE_POSE, 0, false);
	}

	if (considerRotateStart())
		return true;

	// TODO: inline
	BOOL zeroed = false;
	f32 v       = FConverge(mForwardVel, 0.0f, 1.0f, 1.0f);
	mForwardVel = v;
	if (v == 0.0f)
		zeroed = true;

	setPlayerVelocity(mForwardVel);

	if (zeroed)
		return changePlayerStatus(MARIO_STATUS_WAIT, 0, false);

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
		break;
	case 2:
		setPlayerVelocity(0.0f);
		break;
	}

	f32 rate = 0.25f * mForwardVel;
	if (rate < 0.1f)
		rate = 0.1f;
	setAnimation(ANIM_RUN1, rate);
	return 0;
}

BOOL TMario::speedSliding()
{
	s16 savedFace = mFaceAngle.y;

	if (doSliding(getSlideStopNormal()))
		return changePlayerStatus(MARIO_STATUS_SLIP_END, 0, false);

	slippingBasic(MARIO_STATUS_SLIP_END, MARIO_STATUS_SLIP_FALL, 0xF);
	mFaceAngle.y = savedFace;
	return 0;
}

BOOL TMario::fireDashing()
{
	if (mInput & 0x2)
		return changePlayerStatus(MARIO_STATUS_FIRE_JUMP, 0, false);

	if (mStatusTimer++ > 0xA0)
		return changePlayerStatus(MARIO_STATUS_RUN, 0, false);

	if (checkFlag(MARIO_FLAG_IN_ANY_WATER))
		return changePlayerStatus(MARIO_STATUS_RUN, 0, false);

	if (mForwardVel < 8.0f)
		mForwardVel = 8.0f;

	if (mForwardVel > 48.0f)
		mForwardVel = 48.0f;

	mForwardVel = FConverge(mForwardVel, 32.0f, 4.0f, 1.0f);
	if (mInput & 0x1) {
		mFaceAngle.y
		    = mIntendedYaw
		      - IConverge((s16)(mIntendedYaw - mFaceAngle.y), 0, 0x600, 0x600);
	}
	slopeProcess();
	if (!walkProcess()) {
		changePlayerStatus(MARIO_STATUS_FIRE_LAND, 0, false);
	}
	setAnimation(ANIM_FIREJMP, 0.1f * (0.5f * mForwardVel));
	return 0;
}

void TMario::slippingBasic(int statusOnStop, int statusOnFall, int slipAnim)
{
	isForceSlip();
	if ((mInput & 0x2) && canSlipJump() == 1) {
		changePlayerStatus(MARIO_STATUS_JUMP, 0, false);
		return;
	}
	if (mInput & 0x8000) {
		mVel.y = 20.0f;
		changePlayerStatus(MARIO_STATUS_JUMP_CATCH, 1, false);
		return;
	}

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(statusOnFall, 0, false);
		return;
	case 1:
		setAnimation(slipAnim, 1.0f);
		onUnk114(UNK114_FLAG_UNK8);
		frontSlipEffect();
		return;
	case 2:
		if (onYoshi()) {
			setPlayerVelocity(0.0f);
			return;
		}
		{
			f32 absVel = mForwardVel;
			if (absVel < 0.0f)
				absVel = -absVel;
			if (absVel > mDeParams.mClashSpeed.get())
				emitParticle(PARTICLE_MS_DMG_C);
		}
		if (isSlipStart()) {
			if (mWallPlane != nullptr) {
				s16 wallAng = matan(mWallPlane->getNormal().z,
				                    mWallPlane->getNormal().x);

				f32 newMag
				    = MsSqrtf(mSlideVelX * mSlideVelX + mSlideVelZ * mSlideVelZ)
				      * 0.9;
				if (newMag < 4.0f)
					newMag = 4.0f;

				unk9E  = (wallAng - (s16)(unk9E - wallAng)) + 0x8000;
				mVel.x = mSlideVelX = newMag * JMASSin(unk9E);
				mVel.z = mSlideVelZ = newMag * JMASCos(unk9E);
				u32 sndId
				    = gpMSound->getWallSound(mWallPlane->unk6, mForwardVel);
				SMSGetMSound()->startSoundActor(sndId, &mPosition, 0, nullptr,
				                                0, 4);
			}
		} else if (mForwardVel > 16.0f) {
			playerRefrection(1);
			changePlayerDropping(MARIO_STATUS_CATCH_DOWN, 0);
		} else {
			setPlayerVelocity(0.0f);
			changePlayerStatus(statusOnStop, 0, false);
		}
		onUnk114(UNK114_FLAG_UNK8);
		return;
	}
}

BOOL TMario::slipForeCommon(int arg0, int arg1, int arg2, int arg3)
{
	if (mStatusTimer > 20 && canSlipJump()) {
		if (mInput & 0x2)
			return changePlayerJumping(arg1, 0);
	} else {
		mStatusTimer++;
	}

	if (doSliding(getSlideStopNormal()))
		return changePlayerStatus(arg0, 0, false);

	slippingBasic(arg0, arg2, arg3);
	return 0;
}

BOOL TMario::slipFore()
{
	return slipForeCommon(MARIO_STATUS_SLIP_END, MARIO_STATUS_JUMP,
	                      MARIO_STATUS_SLIP_FALL, 0x91);
}

BOOL TMario::slipBackCommon(int arg0, int arg1, int arg2)
{
	if (mStatusTimer > 20) {
		if (!(mInput & 0x8) && (mInput & 0x2) && canSlipJump())
			return changePlayerDropping(MARIO_STATUS_CATCH_STOP, 0);
	} else {
		mStatusTimer++;
	}

	if (doSliding(getSlideStopNormal()))
		return changePlayerStatus(arg0, 0, false);

	slippingBasic(arg0, arg1, arg2);
	return 0;
}

BOOL TMario::slipBack()
{
	return slipBackCommon(MARIO_STATUS_CATCH_LOST, MARIO_STATUS_LANDING, 0x89);
}

BOOL TMario::catching()
{
	// TODO: removeme
	(void)0;

	if (!(mInput & 0x8) && (mInput & 0x2)) {
		if (mForwardVel > mDeParams.mClashSpeed.get())
			return changePlayerStatus(MARIO_STATUS_ROTATE_BROAD_JUMP, 0, false);

		return changePlayerStatus(MARIO_STATUS_CATCH_STOP, 0, false);
	}

	if (checkFlag(MARIO_FLAG_RECENTLY_LEFT_WATER))
		mStatusState = 1;

	if (doSliding(getSlideStopCatch())) {
		setPlayerVelocity(0.0f);
		return changePlayerStatus(MARIO_STATUS_CATCH_LOST, 0, false);
	}

	slippingBasic(MARIO_STATUS_CATCH_LOST, MARIO_STATUS_LANDING, 0x88);

	SMSGetMSound()->startSoundActor(MSD_SE_MA_SLIP, &mPosition, 0, nullptr, 0,
	                                4);

	if (getMotionFrameCtrl().getFrame() > 50.0f)
		getMotionFrameCtrl().setFrame(50.0f);

	return 0;
}

BOOL TMario::squatSlipping()
{
	setNormalAttackArea();

	if (mInput & 0x8)
		return changePlayerStatus(MARIO_STATUS_SLIP_FORE, 0, false);

	if (mInput & 0x2)
		return changePlayerJumping(MARIO_STATUS_JUMP, 0);

	if (mInput & 0x10)
		return changePlayerStatus(MARIO_STATUS_BRAKE, 0, false);

	return slipForeCommon(MARIO_STATUS_SQUAT, MARIO_STATUS_JUMP,
	                      MARIO_STATUS_LANDING, 0x97);
}

BOOL TMario::oilRun()
{
	if (mInput & 0x2) {
		setPlayerVelocity(0.0f);
		return changePlayerJumping(MARIO_STATUS_JUMP, 0);
	}

	if (mInput & 0x8000) {
		mVel.y = 20.0f;
		return changePlayerStatus(MARIO_STATUS_JUMP_CATCH, 1, false);
	}

	if (-1.0f < mVel.x && mVel.x < 1.0f) {
		if (-1.0f < mVel.z && mVel.z < 1.0f) {
			setPlayerVelocity(0.0f);
			return changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
		}
	}

	f32 tmp = mDirtyParams.mPolSizeRun.get();
	gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z, tmp);

	{
		f32 rotSp = mDirtyParams.mSlipRotate.get();
		s16 diff  = mIntendedYaw - mFaceAngle.y;
		mFaceAngle.y
		    = mIntendedYaw - IConverge(diff, 0, (s16)rotSp, (s16)rotSp);
	}

	mVel.x
	    += mIntendedMag * JMASSin(mFaceAngle.y) * mDirtyParams.mSlipRunSp.get();
	mVel.z
	    += mIntendedMag * JMASCos(mFaceAngle.y) * mDirtyParams.mSlipRunSp.get();
	mDirtyTimer--;
	if (mDirtyTimer <= 0) {
		mDirtyTimer = 0;
		mOilBrake   = 0.0f;
	}
	mVel.x *= mOilBrake;
	mVel.z *= mOilBrake;
	mForwardVel = 0.0f;
	mSlideVelX  = 0.0f;
	mSlideVelZ  = 0.0f;
	if (mIntendedMag == 0.0f) {
		if (mInput & 0x4000) {
			setAnimation(ANIM_SQWAT, 0.5f);
		} else {
			setAnimation(ANIM_WAIT, 0.5f);
		}
	} else {
		setAnimation(ANIM_RUN2,
		             0.5f * mIntendedMag * mDirtyParams.mSlipAnmSpeed.get());
		startVoiceIfNoVoice(MSD_SE_MV28_SPRISE_SMALL_01);
		SMSGetMSound()->startSoundActor(MSD_SE_MA_SLIP_POLLUT, &mPosition, 0,
		                                nullptr, 0, 4);
	}

	switch (walkProcess()) {
	case 0:
		return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
	case 1:
	case 2:
		break;
	}

	return 0;
}

BOOL TMario::oilSlip()
{
	if (mInput & 0x2) {
		setPlayerVelocity(0.0f);
		return changePlayerJumping(MARIO_STATUS_JUMP, 0);
	}

	{
		f32 rotSp = mDirtyParams.mSlipCatchRotate.get();
		s16 diff  = mIntendedYaw - mFaceAngle.y;
		mFaceAngle.y
		    = mIntendedYaw - IConverge(diff, 0, (s16)rotSp, (s16)rotSp);
	}

	mDirtyTimer--;
	if (mDirtyTimer <= 0) {
		mDirtyTimer = 0;
		mOilBrake   = 0.0f;
		changePlayerStatus(MARIO_STATUS_CATCH, 0, false);
	}

	f32 tmp = mDirtyParams.mPolSizeSlip.get();
	gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z, tmp);
	SMSGetMSound()->startSoundActor(MSD_SE_MA_SLIP_POLLUT_CP, &mPosition, 0,
	                                nullptr, 0, 4);

	mForwardVel += mIntendedMag * JMASCos((s16)(mFaceAngle.y - mIntendedYaw))
	               * mDirtyParams.mSlipCatchSp.get();
	mForwardVel *= mOilBrake;
	setPlayerVelocity(mForwardVel);

	if (-1.0f < mForwardVel && mForwardVel < 1.0f) {
		setPlayerVelocity(0.0f);
		return changePlayerStatus(MARIO_STATUS_CATCH_LOST, 0, false);
	}

	switch (walkProcess()) {
	case 0:
		return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);

	case 1:
	case 2:
	default:
		if (getMotionFrameCtrl().getFrame() > 50.0f)
			getMotionFrameCtrl().setFrame(50.0f);
		return 0;
	}
}

BOOL TMario::oilSlope()
{
	mDirtyTimer--;
	if (mDirtyTimer <= 0) {
		mDirtyTimer = 0;
		mOilBrake   = 0.0f;
		changePlayerStatus(MARIO_STATUS_CATCH, 0, false);
	}
	gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z,
	                   mDirtyParams.mPolSizeSlip.get());
	return slipBackCommon(MARIO_STATUS_CATCH_LOST, MARIO_STATUS_LANDING, 0x89);
}

f32 TMario::downingCommon(int anim, f32 limit, int arg2)
{
	f32 animRate = setAnimation(anim, 1.0f);
	if (animRate < limit) {
		slopeProcess();
		mForwardVel *= 0.96f;
		if (mForwardVel * mForwardVel < 1.0f)
			setPlayerVelocity(0.0f);
	} else {
		if (mForwardVel >= 0.0f)
			setPlayerVelocity(0.1f);
		else
			setPlayerVelocity(-0.1f);
	}

	if (!walkProcess()) {
		if (mForwardVel >= 0.0f)
			changePlayerStatus(MARIO_STATUS_JUMP_SHORT_FORE_DOWN, arg2, false);
		else
			changePlayerStatus(MARIO_STATUS_JUMP_SHORT_BACK_DOWN, arg2, false);

	} else if (isLast1AnimeFrame()) {
		changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
	}

	return animRate;
}

BOOL TMario::backDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(PARTICLE_MS_DMG_C);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(1, 86.0f, mStatusArg);
	return 0;
}

BOOL TMario::foreDown()
{

	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(PARTICLE_MS_DMG_C);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(ANIM_SDWNF, 42.0f, mStatusArg);
	return 0;
}

BOOL TMario::shortBackDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(PARTICLE_MS_DMG_C);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(ANIM_SDOWN, 88.0f, mStatusArg);
	return 0;
}

BOOL TMario::shortForeDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(PARTICLE_MS_DMG_C);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(ANIM_SHFDN, 80.0f, mStatusArg);
	return 0;
}

BOOL TMario::safeBackDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(PARTICLE_MS_DMG_C);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(ANIM_SFBDN, 200.0f, mStatusArg);
	return 0;
}

BOOL TMario::safeForeDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(PARTICLE_MS_DMG_C);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(ANIM_SFFDN, 100.0f, mStatusArg);
	return 0;
}

BOOL TMario::catchDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(PARTICLE_MS_DMG_C);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(ANIM_SLDWN, 128.0f, mStatusArg);
	return 0;
}

BOOL TMario::loserDown()
{
	slopeProcess();
	mForwardVel *= 0.9f;
	if (mForwardVel * mForwardVel < 1.0f)
		setPlayerVelocity(0.0f);

	setAnimation(ANIM_DIE, 1.0f);
	switch (mStatusState) {
	case 0:
		startVoice(MSD_SE_MV08A_DOWN_01);
		mStatusState++;
		break;
	case 1:
		if (!gpMSound->checkMarioVoicePlaying(0)) {
			mStatusTimer = 0;
			mStatusState++;
		}
		break;
	case 2:
		if (mStatusTimer++ > 60)
			mStatusState++;
		break;
	case 3:
		startVoice(MSD_SE_MV08B_DOWN_REACT_01);
		mStatusState++;
		break;
	case 4:
		break;
	}

	return 0;
}

BOOL TMario::jumpSlipCommon(s16 anim, u32 status)
{
	if (mInput & 0x1) {
		slopeProcess();
		mForwardVel *= 0.98f;
		if (mForwardVel * mForwardVel < 1.0f)
			setPlayerVelocity(0.0f);
	} else {
		if (mForwardVel >= 16.0f) {
			mForwardVel = FConverge(mForwardVel, 0.0f, 4.0f, 4.0f);
			slopeProcess();
		} else {
			mVel.y = 0.0f;
		}
	}
	BOOL wp = walkProcess();
	switch (wp) {
	case 0:
		changePlayerStatus(status, 0, false);
		break;
	case 2:
		setAnimation(ANIM_PUSH, 1.0f);
		break;
	case 1:
		break;
	}
	setAnimation(anim, 1.0f);
	return wp;
}

BOOL TMario::jumpSlipEvents(JumpSlipRecord* rec)
{
	if (mInput & 0x10)
		return changePlayerStatus(rec->mEndStatus, 0, false);

	mStatusTimer++;

	if (mStatusTimer >= rec->mMaxTimer)
		return changePlayerStatus(rec->mEndStatus, 0, false);

	if (mInput & 0x2) {
		if (rec->mJumpStatus == MARIO_STATUS_SECOND_JUMP)
			if (mForwardVel >= mJumpParams.mSecJumpEnableSp.get())
				return changePlayerJumping(MARIO_STATUS_SECOND_JUMP, 0);

		if (rec->mJumpStatus == MARIO_STATUS_ULTRA_JUMP)
			if (mForwardVel >= mJumpParams.mTriJumpEnableSp.get())
				return changePlayerJumping(MARIO_STATUS_ULTRA_JUMP, 0);

		return changePlayerJumping(MARIO_STATUS_JUMP, 0);
	}

	if (mInput & 0x8000) {
		mVel.y = 20.0f;
		return changePlayerStatus(MARIO_STATUS_JUMP_CATCH, 1, false);
	}

	if (mInput & 0x4)
		return changePlayerStatus(rec->unkC, 0, false);

	return 0;
}

BOOL TMario::jumpSlip()
{
	static TMario::JumpSlipRecord param
	    = { 0x10, MARIO_STATUS_JUMP_END, MARIO_STATUS_SECOND_JUMP,
		    MARIO_STATUS_LANDING, MARIO_STATUS_SLIP };

	if (jumpSlipEvents(&param))
		return 1;

	jumpSlipCommon(ANIM_JMPED, MARIO_STATUS_LANDING);

	return 0;
}

BOOL TMario::landSlip()
{
	static TMario::JumpSlipRecord param
	    = { 0x10, MARIO_STATUS_LAND_END, MARIO_STATUS_SECOND_JUMP,
		    MARIO_STATUS_LANDING, MARIO_STATUS_SLIP };

	if (jumpSlipEvents(&param))
		return 1;

	jumpSlipCommon(ANIM_LAEND, MARIO_STATUS_LANDING);

	return 0;
}

BOOL TMario::secJumpSlip()
{
	static TMario::JumpSlipRecord param
	    = { 0x10, MARIO_STATUS_SEC_JUMP_END, MARIO_STATUS_ULTRA_JUMP,
		    MARIO_STATUS_LANDING, MARIO_STATUS_SLIP };

	if (jumpSlipEvents(&param))
		return 1;

	jumpSlipCommon(ANIM_2JMED, MARIO_STATUS_LANDING);

	return 0;
}

BOOL TMario::uTurnJumpSlip()
{
	static TMario::JumpSlipRecord param
	    = { 0x10, MARIO_STATUS_U_TURN_JUMP_END, MARIO_STATUS_JUMP,
		    MARIO_STATUS_LANDING, MARIO_STATUS_SLIP };

	if (jumpSlipEvents(&param))
		return 1;

	if (jumpSlipCommon(ANIM_TJMP2, MARIO_STATUS_LANDING) != 2) {
		mFaceAngle.x = 0;
		mModelFaceAngle += 0x8000;
	}

	return 0;
}

BOOL TMario::ultraJumpSlip()
{
	static TMario::JumpSlipRecord param
	    = { 0x04, MARIO_STATUS_ULTRA_JUMP_END, MARIO_STATUS_JUMP,
		    MARIO_STATUS_LANDING, MARIO_STATUS_SLIP };

	if (jumpSlipEvents(&param))
		return 1;

	jumpSlipCommon(0xC0, MARIO_STATUS_LANDING);

	return 0;
}

BOOL TMario::broadJumpSlip()
{
	if (!(mInput & 0x4000))
		mInput &= ~0x2;

	static TMario::JumpSlipRecord param
	    = { 0x18, MARIO_STATUS_BROAD_JUMP_END, MARIO_STATUS_BROAD_JUMP,
		    MARIO_STATUS_LANDING, MARIO_STATUS_SLIP };

	if (jumpSlipEvents(&param))
		return 1;

	jumpSlipCommon(ANIM_SQWAT, MARIO_STATUS_LANDING);

	return 0;
}

BOOL TMario::moveMain()
{
	BOOL ret = 0;

	checkEnforceJump();
	checkReturn();

	if (checkStatusType(MARIO_STATUS_FLAG_UNK40000)
	    && !checkStatusType(MARIO_STATUS_OIL_RUN)
	    && !checkStatusType(MARIO_STATUS_OIL_SLIP)) {
		SMSGetMSound()->startSoundActor(MSD_SE_MA_SLIP, &mPosition, 0, nullptr,
		                                0, 4);
	}

	switch (mStatus) {
	case MARIO_STATUS_RUN:
		ret = running();
		break;

	case MARIO_STATUS_ROTATE_L:
	case MARIO_STATUS_ROTATE_R:
		ret = rotating();
		break;

	case MARIO_STATUS_TURN:
		ret = turnning();
		break;

	case MARIO_STATUS_TURN_END:
		ret = turnEnd();
		break;

	case MARIO_STATUS_BRAKE:
		ret = braking();
		break;

	case MARIO_STATUS_SURF:
		ret = surfing();
		break;

	case MARIO_STATUS_TOROCCO:
		ret = toroccoing();
		break;

	case MARIO_STATUS_WALK_END:
		ret = walkEnd();
		break;

	case MARIO_STATUS_SPEED_SLIDING:
		ret = speedSliding();
		break;

	case MARIO_STATUS_FIRE_DASH:
		ret = fireDashing();
		break;

	case MARIO_STATUS_SLIP_FORE:
		ret = slipFore();
		break;

	case MARIO_STATUS_SLIP_BACK:
		ret = slipBack();
		break;

	case MARIO_STATUS_CATCH:
		ret = catching();
		break;

	case MARIO_STATUS_SQUAT_SLIP:
		ret = squatSlipping();
		break;

	case MARIO_STATUS_OIL_RUN:
		ret = oilRun();
		break;

	case MARIO_STATUS_OIL_SLIP:
		ret = oilSlip();
		break;

	case MARIO_STATUS_OIL_SLOPE:
		ret = oilSlope();
		break;

	case MARIO_STATUS_BACK_DOWN:
		ret = backDown();
		break;

	case MARIO_STATUS_FORE_DOWN:
		ret = foreDown();
		break;

	case MARIO_STATUS_SHORT_BACK_DOWN:
		ret = shortBackDown();
		break;

	case MARIO_STATUS_SHORT_FORE_DOWN:
		ret = shortForeDown();
		break;

	case MARIO_STATUS_SAFE_BACK_DOWN:
		ret = safeBackDown();
		break;

	case MARIO_STATUS_SAFE_FORE_DOWN:
		ret = safeForeDown();
		break;

	case MARIO_STATUS_CATCH_DOWN:
		ret = catchDown();
		break;

	case MARIO_STATUS_LOSER_DOWN:
		ret = loserDown();
		break;

	case MARIO_STATUS_JUMP_SLIP:
		ret = jumpSlip();
		break;

	case MARIO_STATUS_LAND_SLIP:
		ret = landSlip();
		break;

	case MARIO_STATUS_SEC_JUMP_SLIP:
		ret = secJumpSlip();
		break;

	case MARIO_STATUS_U_TURN_JUMP_SLIP:
		ret = uTurnJumpSlip();
		break;

	case MARIO_STATUS_ULTRA_JUMP_SLIP:
		ret = ultraJumpSlip();
		break;

	case MARIO_STATUS_BROAD_JUMP_SLIP:
		ret = broadJumpSlip();
		break;
	}

	return ret;
}
