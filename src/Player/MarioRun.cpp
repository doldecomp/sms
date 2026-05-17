#include <Player/MarioMain.hpp>
#include <Player/WaterGun.hpp>
#include <Player/NozzleTrigger.hpp>
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
			return changePlayerStatus(STATUS_ROTATE_L, 0, false);
		else
			return changePlayerStatus(STATUS_ROTATE_R, 0, false);
	}

	return 0;
}

static bool unknown_inline_2(TMario* mario)
{
	if (mario->unk380 == 0 || mario->unk380 == 1)
		return true;
	else
		return false;
}

static int unknown_inline_4(TMario* mario)
{
	static TMario::JumpSlipRecord param
	    = { 0x10, 0x0C000230, TMario::STATUS_SECOND_JUMP, 0x88C, 0x50 };

	if (mario->jumpSlipEvents(&param))
		return 1;

	mario->jumpSlipCommon(0x4E, 0x88C);

	return 0;
}

static int unknown_inline_5(TMario* mario)
{
	static TMario::JumpSlipRecord param
	    = { 0x10, 0xC000232, TMario::STATUS_SECOND_JUMP, 0x88C, 0x50 };

	if (mario->jumpSlipEvents(&param))
		return 1;

	mario->jumpSlipCommon(0x57, 0x88C);

	return 0;
}

static int unknown_inline_6(TMario* mario)
{
	static TMario::JumpSlipRecord param
	    = { 0x10, 0xC000231, TMario::STATUS_ULTRA_JUMP, 0x88C, 0x50 };

	if (mario->jumpSlipEvents(&param))
		return 1;

	mario->jumpSlipCommon(0x4B, 0x88C);

	return 0;
}

static int unknown_inline_7(TMario* mario)
{
	static TMario::JumpSlipRecord param
	    = { 0x10, 0xC000233, TMario::STATUS_JUMP, 0x88C, 0x50 };

	if (mario->jumpSlipEvents(&param))
		return 1;

	if (mario->jumpSlipCommon(0xBE, 0x88C) != 2) {
		mario->mFaceAngle.x = 0;
		mario->mModelFaceAngle += 0x8000;
	}

	return 0;
}

static int unknown_inline_8(TMario* mario)
{
	static TMario::JumpSlipRecord param
	    = { 0x04, 0x800023A, TMario::STATUS_JUMP, 0x88C, 0x50 };

	if (mario->jumpSlipEvents(&param))
		return 1;

	mario->jumpSlipCommon(0xC0, 0x88C);

	return 0;
}

static int unknown_inline_9(TMario* mario)
{
	if (!(mario->mInput & 0x4000))
		mario->mInput &= ~0x2;

	static TMario::JumpSlipRecord param
	    = { 0x18, 0x800023B, TMario::STATUS_BROAD_JUMP, 0x88C, 0x50 };

	if (mario->jumpSlipEvents(&param))
		return 1;

	mario->jumpSlipCommon(0x98, 0x88C);

	return 0;
}

static int unknown_inline_10(TMario* mario)
{
	if ((mario->mInput & 0x8000)
	    && mario->mForwardVel > mario->mRunParams.mDoJumpCatchSp.get()
	    && mario->mIntendedMag > 0.75f) {
		mario->mVel.y = 20.0f;
		return mario->changePlayerStatus(TMario::STATUS_DIVE_JUMP, 1, false);
	}

	return 0;
}

BOOL TMario::isThrowStart()
{
	if (mHeldObject != nullptr && ((mInput & 0x2000) ? true : false)) {

		if (mHeldObject->checkActorType(0x10000000))
			return changePlayerStatus(0x80000588, 0, false);

		switch (mHeldObject->getActorType()) {
		case 0x80000001:
			return changePlayerStatus(0x80000588, 0, false);

		default:
			if (mForwardVel > 16.0f)
				return changePlayerStatus(0x80000588, 0, false);
			if (canPut())
				return changePlayerStatus(0x80000387, 0, false);
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
	if (unknown_inline_2(this))
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
	if (checkFlag(0x30000)
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
		case 0x72:
			if (mForwardVel >= mDeParams.mDashMax.get() - 1.0f) {
				setAnimation(0xF5, 1.0f);
				loop = false;
			} else if (sp < mRunParams.mWalk2Soft.get()) {
				setAnimation(0x92, 1.0f);
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
				setAnimation(0x72, rate);
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
		case 0x92:
			if (sp > mRunParams.mSoft2Walk.get()) {
				setAnimation(0x72, 1.0f);
				loop = false;
			} else {
				f32 rate2 = sp;
				if (rate2 < 0.1f)
					rate2 = 0.1f;
				rate2 *= mRunParams.mSoftStepAnmMult.get();
				setAnimation(0x92, rate2);
				loop = false;
			}
			break;
		case 0xF5:
			if (mForwardVel < mDeParams.mDashMax.get() - 1.0f) {
				setAnimation(0x72, 1.0f);
				loop = false;
			} else {
				f32 sp2 = sp * mRunParams.mRunAnmSpeedMult.get()
				          + mRunParams.mRunAnmSpeedBase.get();
				setAnimation(0xF5, sp2);
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

	if (mGroundPlane->isUnkC()) {
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

	if (mGroundPlane->isUnkC()) {
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
		if (mGroundPlane->isUnkC()) {
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

	if (mGroundPlane->isUnkC())
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
	if (mStatus == 0x84045D) {
		slipFr = mSlipParamsOil.mSlipFriction.get();
	} else if (isForceSlip()) {
		slipFr = mSlipParamsAll.mSlipFriction.get();
	} else if (mGroundPlane->isUnkC()) {
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
		if (mStatus == 0x800456) {
			if (mStatusState == 1)
				slipFr = mDeParams.mWasOnWaterSlip.get();
			if (checkFlag(0x30000))
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

	if (!mGroundPlane->isUnk1() && !mGroundPlane->isUnkC()) {
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
	if (unk380 == 0 || unk380 == 1) {
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

	if (checkFlag(0x4000))
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
	switch (unk389) {
	case 1:
		return &mSurfingParamsWaterYellow;
	case 2:
		return &mSurfingParamsWaterGreen;
	default:
		return &mSurfingParamsWaterRed;
	}
}

TMario::TSurfingParams* TMario::getSurfingParamsGround()
{
	switch (unk389) {
	case 1:
		return &mSurfingParamsGroundYellow;
	case 2:
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
		setAnimation(0x6C, 1.0f);
		startVoice(0x7094);
		return;
	}

	f32 rate = 2.0f * MsSqrtf(dx * dx + dz * dz);
	if (angDiff < 0)
		setAnimation(0x80, rate);
	else
		setAnimation(0x7F, rate);

	mFaceAngle.x    = 0;
	mModelFaceAngle = wallAngle + 0x8000;
}

BOOL TMario::running()
{
	mStatusTimer++;

	if (isThrowStart())
		return 1;

	if (isRunningSlipStart())
		return changePlayerStatus(0x50, 0, false);

	if (mInput & 0x10) {
		if (mStatusState == 1) {
			mFaceAngle.y = (s16)mStatusArg;
			return changePlayerStatus(0xC400209, 0, false);
		}
		if (mStatusTimer > 0xF0 && mForwardVel >= 16.0f
		    && mGroundPlane->mNormal.y <= 0.17364818f) {
			return changePlayerStatus(STATUS_BRAKE, 0, false);
		}
		return changePlayerStatus(STATUS_WALK_END, 0, false);
	}

	if (checkFlag(0x4000) && (mInput & 0x2)
	    && mForwardVel > mDeParams.mDashMax.get() - 1.0f)
		return changePlayerJumping(STATUS_BROAD_JUMP, 0);

	if (mInput & 0x2)
		return changePlayerTriJump();

	if (!onYoshi() && (mInput & 0x8000)) {
		if (unknown_inline_10(this))
			return 1;
		changePlayerStatus(0x384, 0, false);
	}

	if (mInput & 0x20) {
		if (mStatusState == 1) {
			mFaceAngle.y = (s16)mStatusArg;
			return changePlayerStatus(0xC400209, 0, false);
		}
		if (mStatusTimer > 0xF0 && mForwardVel >= 16.0f
		    && mGroundPlane->mNormal.y <= 0.17364818f) {
			return changePlayerStatus(STATUS_BRAKE, 0, false);
		}
		return changePlayerStatus(STATUS_WALK_END, 0, false);
	}

	if (isRunningTurnning() && mForwardVel >= mRunParams.mTurnNeedSp.get()) {
		emitParticle(0x15, mFaceAngle.y + 0x8000);
		emitParticle(0x17, mFaceAngle.y + 0x8000);
		emitParticle(0x16, mFaceAngle.y + 0x8000);
		return changePlayerStatus(STATUS_TURN, 0, false);
	}

	if (canSquat()) {
		setPlayerVelocity(0.0f);
		return changePlayerStatus(0xC008220, 0, false);
	}

	if (rocketCheck()) {
		unk314
		    = mFloorPosition.y + mWaterGun->mWatergunParams.mHoverHeight.get();
		return changePlayerStatus(0x88B, 0, false);
	}

	bool pushed                   = false;
	mStatusState                  = 0;
	JGeometry::TVec3<f32> prevPos = mPosition;
	doRunning();
	setNormalAttackArea();

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(0x88C, 0, false);
		setAnimation(0x56, 1.0f);
		break;
	case 1:
		doRunningAnimation();
		break;
	case 2:
		if (onYoshi()) {
			setPlayerVelocity(0.0f);
			break;
		}

		if (unk118 & 2)
			pushed = true;

		if (!pushed) {
			if (mForwardVel > mDeParams.mClashSpeed.get()) {
				emitParticle(0xC);
				return changePlayerDropping(STATUS_JUMP_SHORT_BACK_DOWN, 0);
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
				return changePlayerStatus(0x02000886, 0, false);
			}

			if (mWallPlane != nullptr && mWallPlane->isFence()) {
				mFaceAngle.y = matan(mWallPlane->getNormal().z,
				                     mWallPlane->getNormal().x)
				               + 0x8000;
				mModelFaceAngle = mFaceAngle.y;
				return changePlayerStatus(0x3000036B, 0, false);
			}

			doPushingAnimation(prevPos);
			unkC4 = 0;
			unk118 &= ~0x4000;
		}
		break;
	}

	checkDescent();
	if (checkFlag(0x4000)) {
		setPlayerVelocity(mDeParams.mDashMax.get());
		startSoundActor(0x19);
	}
	return 0;
}

BOOL TMario::rotating()
{
	if (mInput & 0x2) {
		if (mStatus == STATUS_ROTATE_L)
			return changePlayerStatus(0x896, 0, false);
		else
			return changePlayerStatus(0x895, 0, false);
	}

	setAnimation(0xF4, 1.0f);
	emitRotateShootEffect();
	emitBlurSpinJump();

	mStatusTimer++;
	if (mStatusTimer > 0x78)
		return changePlayerStatus(0x0C400201, 0, false);

	doRunning();

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(0x88C, 0, false);
		break;
	}

	if (mStatus == STATUS_ROTATE_L)
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
		return changePlayerStatus(0x50, 0, false);

	if (mInput & 0x2)
		return changePlayerJumping(STATUS_U_TURN_JUMP, 0);

	if (considerRotateStart())
		return true;

	if (mInput & 0x20)
		return changePlayerStatus(STATUS_BRAKE, 0, false);

	if (!isRunningTurnning())
		return changePlayerStatus(STATUS_RUN, 0, false);

	BOOL zeroed = false;
	f32 v       = FConverge(mForwardVel, 0.0f, 4.0f, 4.0f);
	mForwardVel = v;
	if (v == 0.0f)
		zeroed = true;

	slopeProcess();
	if (zeroed) {
		mFaceAngle.y = mIntendedYaw;
		setPlayerVelocity(8.0f);
		return changePlayerStatus(STATUS_TURN_END, 0, false);
	}

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(0x88C, 0, false);
		break;
	}

	if (mForwardVel >= 18.0f) {
		setAnimation(0xBC, 1.0f);
	} else {
		setAnimation(0xBD, 1.0f);
		if (isLast1AnimeFrame()) {
			f32 vel = mForwardVel;
			if (vel > 0.0f) {
				mFaceAngle.y = mIntendedYaw;
				setPlayerVelocity(-vel);
				changePlayerStatus(STATUS_RUN, 0, false);
			} else {
				mFaceAngle.y = mIntendedYaw;
				setPlayerVelocity(8.0f);
				changePlayerStatus(STATUS_RUN, 0, false);
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
		return changePlayerStatus(0x50, 0, false);

	if (mInput & 0x2)
		return changePlayerJumping(STATUS_U_TURN_JUMP, 0);

	if (considerRotateStart())
		return true;

	doRunning();
	setAnimation(0xBD, 1.0f);

	if (!walkProcess())
		changePlayerStatus(0x88C, 0, false);

	if (isLast1AnimeFrame())
		changePlayerStatus(STATUS_RUN, 0, false);

	mFaceAngle.x = 0;
	mModelFaceAngle += 0x8000;
	return 0;
}

// TODO: remove the inline mark here!!!
inline int TMario::braking()
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
		return changePlayerStatus(0x0C00023D, 0, false);

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(0x88C, 0, false);
		break;

	case 1:
		break;

	case 2:
		if (mForwardVel > 16.0f) {
			playerRefrection(1);
			changePlayerDropping(STATUS_SHORT_BACK_DOWN, 0);
		} else {
			setPlayerVelocity(0.0f);
			changePlayerStatus(0x0C00023D, 0, false);
		}
		break;
	}
	setAnimation(0xF, 1.0f);
	return 0;
}

BOOL TMario::surfing()
{
	setAnimation(0x6D, 1.0f);
	if (mStatusTimer != 0) {
		mStatusTimer--;
		return 0;
	}

	if (mInput & 0x2) {
		mPosition.y += 1.0f;
		return changePlayerStatus(0x0281089A, 0, false);
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
			BOOL ret    = changePlayerStatus(STATUS_JUMP_BACK_DOWN, 0, true);
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

int TMario::toroccoing()
{
	soundTorocco();
	toroccoEffect();
	return 0;
}

BOOL TMario::walkEnd()
{
	if (!(mInput & 0x10)) {
		if (isRunningSlipStart())
			return changePlayerStatus(0x50, 0, false);

		if (mInput & 0x2)
			return changePlayerTriJump();

		if (mInput & 0x1)
			return changePlayerStatus(STATUS_RUN, 0, false);

		if (mInput & 0x8000)
			return changePlayerStatus(0x384, 0, false);
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
		return changePlayerStatus(0xC400201, 0, false);

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(0x88C, 0, false);
		break;
	case 2:
		setPlayerVelocity(0.0f);
		break;
	}

	f32 rate = 0.25f * mForwardVel;
	if (rate < 0.1f)
		rate = 0.1f;
	setAnimation(0x48, rate);
	return 0;
}

int TMario::speedSliding()
{
	s16 savedFace = mFaceAngle.y;

	if (doSliding(getSlideStopNormal()))
		return changePlayerStatus(0x0C00023E, 0, false);

	slippingBasic(0x0C00023E, 0x0200088E, 0xF);
	mFaceAngle.y = savedFace;
	return 0;
}

BOOL TMario::fireDashing()
{
	if (mInput & 0x2)
		return changePlayerStatus(0x208B4, 0, false);

	if (mStatusTimer++ > 0xA0)
		return changePlayerStatus(STATUS_RUN, 0, false);

	if (checkFlag(0x30000))
		return changePlayerStatus(STATUS_RUN, 0, false);

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
		changePlayerStatus(0x208B5, 0, false);
	}
	setAnimation(0x29, 0.1f * (0.5f * mForwardVel));
	return 0;
}

void TMario::slippingBasic(int statusOnStop, int statusOnFall, int slipAnim)
{
	isForceSlip();
	if ((mInput & 0x2) && canSlipJump() == 1) {
		changePlayerStatus(STATUS_JUMP, 0, false);
		return;
	}
	if (mInput & 0x8000) {
		mVel.y = 20.0f;
		changePlayerStatus(STATUS_DIVE_JUMP, 1, false);
		return;
	}

	switch (walkProcess()) {
	case 0:
		changePlayerStatus(statusOnFall, 0, false);
		return;
	case 1:
		setAnimation(slipAnim, 1.0f);
		unk114 |= 8;
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
				emitParticle(0xC);
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
			changePlayerDropping(STATUS_CATCH_DOWN, 0);
		} else {
			setPlayerVelocity(0.0f);
			changePlayerStatus(statusOnStop, 0, false);
		}
		unk114 |= 8;
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

int TMario::slipFore()
{
	return slipForeCommon(0xC00023E, STATUS_JUMP, 0x200088E, 0x91);
}

BOOL TMario::slipBackCommon(int arg0, int arg1, int arg2)
{
	if (mStatusTimer > 20) {
		if (!(mInput & 0x8) && (mInput & 0x2) && canSlipJump())
			return changePlayerDropping(0x8A6, 0);
	} else {
		mStatusTimer++;
	}

	if (doSliding(getSlideStopNormal()))
		return changePlayerStatus(arg0, 0, false);

	slippingBasic(arg0, arg1, arg2);
	return 0;
}

int TMario::slipBack() { return slipBackCommon(0x386, 0x88C, 0x89); }

BOOL TMario::catching()
{
	if (!(mInput & 0x8) && (mInput & 0x2)) {
		if (mForwardVel > mDeParams.mClashSpeed.get()) {
			return changePlayerStatus(0x2000889, 0, false);
		}
		return changePlayerStatus(0x8A6, 0, false);
	}
	if (checkFlag(0x10)) {
		mStatusState = 1;
	}
	if (doSliding(getSlideStopCatch())) {
		setPlayerVelocity(0.0f);
		return changePlayerStatus(0x386, 0, false);
	}
	slippingBasic(0x386, 0x88C, 0x88);
	if (gpMSound->gateCheck(0x1009)) {
		MSoundSESystem::MSoundSE::startSoundActor(0x1009, &mPosition, 0,
		                                          nullptr, 0, 4);
	}
	if (getMotionFrameCtrl().getFrame() > 50.0f) {
		getMotionFrameCtrl().setFrame(50.0f);
	}
	return 0;
}

int TMario::squatSlipping()
{
	setNormalAttackArea();

	if (mInput & 0x8)
		return changePlayerStatus(STATUS_SLIP_FORE, 0, false);

	if (mInput & 0x2)
		return changePlayerJumping(STATUS_JUMP, 0);

	if (mInput & 0x10)
		return changePlayerStatus(STATUS_BRAKE, 0, false);

	return slipForeCommon(0xC008220, STATUS_JUMP, 0x88C, 0x97);
}

int TMario::oilRun()
{
	if (mInput & 0x2) {
		setPlayerVelocity(0.0f);
		return changePlayerJumping(STATUS_JUMP, 0);
	}

	if (mInput & 0x8000) {
		mVel.y = 20.0f;
		return changePlayerStatus(0x80088A, 1, false);
	}

	if (-1.0f < mVel.x && mVel.x < 1.0f) {
		if (-1.0f < mVel.z && mVel.z < 1.0f) {
			setPlayerVelocity(0.0f);
			return changePlayerStatus(0xC400201, 0, false);
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
	unk13C--;
	if (unk13C <= 0) {
		unk13C = 0;
		unk138 = 0.0f;
	}
	mVel.x *= unk138;
	mVel.z *= unk138;
	mForwardVel = 0.0f;
	mSlideVelX  = 0.0f;
	mSlideVelZ  = 0.0f;
	if (mIntendedMag == 0.0f) {
		if (mInput & 0x4000) {
			setAnimation(0x98, 0.5f);
		} else {
			setAnimation(0xC3, 0.5f);
		}
	} else {
		setAnimation(0x72,
		             0.5f * mIntendedMag * mDirtyParams.mSlipAnmSpeed.get());
		startVoiceIfNoVoice(0x78D3);
		if (gpMSound->gateCheck(0x1001)) {
			MSoundSESystem::MSoundSE::startSoundActor(0x1001, &mPosition, 0,
			                                          nullptr, 0, 4);
		}
	}

	switch (walkProcess()) {
	case 0:
		return changePlayerStatus(0x88C, 0, false);
	case 1:
	case 2:
		break;
	}

	return 0;
}

int TMario::oilSlip()
{
	if (mInput & 0x2) {
		setPlayerVelocity(0.0f);
		return changePlayerJumping(STATUS_JUMP, 0);
	}

	{
		f32 rotSp = mDirtyParams.mSlipCatchRotate.get();
		s16 diff  = mIntendedYaw - mFaceAngle.y;
		mFaceAngle.y
		    = mIntendedYaw - IConverge(diff, 0, (s16)rotSp, (s16)rotSp);
	}

	unk13C--;
	if (unk13C <= 0) {
		unk13C = 0;
		unk138 = 0.0f;
		changePlayerStatus(0x800456, 0, false);
	}

	f32 tmp = mDirtyParams.mPolSizeSlip.get();
	gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z, tmp);
	SMSGetMSound()->startSoundActor(0x1141, &mPosition, 0, nullptr, 0, 4);

	mForwardVel += mIntendedMag * JMASCos((s16)(mFaceAngle.y - mIntendedYaw))
	               * mDirtyParams.mSlipCatchSp.get();
	mForwardVel *= unk138;
	setPlayerVelocity(mForwardVel);

	if (-1.0f < mForwardVel && mForwardVel < 1.0f) {
		setPlayerVelocity(0.0f);
		return changePlayerStatus(0x386, 0, false);
	}

	switch (walkProcess()) {
	case 0:
		return changePlayerStatus(0x88C, 0, false);

	case 1:
	case 2:
	default:
		if (getMotionFrameCtrl().getFrame() > 50.0f)
			getMotionFrameCtrl().setFrame(50.0f);
		return 0;
	}
}

int TMario::oilSlope()
{
	unk13C--;
	if (unk13C <= 0) {
		unk13C = 0;
		unk138 = 0.0f;
		changePlayerStatus(0x800456, 0, false);
	}
	gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z,
	                   mDirtyParams.mPolSizeSlip.get());
	return slipBackCommon(0x386, 0x88C, 0x89);
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
			changePlayerStatus(STATUS_JUMP_SHORT_FORE_DOWN, arg2, false);
		else
			changePlayerStatus(STATUS_JUMP_SHORT_BACK_DOWN, arg2, false);

	} else if (isLast1AnimeFrame()) {
		changePlayerStatus(0xC400201, 0, false);
	}

	return animRate;
}

int TMario::backDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(0xC);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(1, 86.0f, mStatusArg);
	return 0;
}

int TMario::foreDown()
{

	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(0xC);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(0x2C, 42.0f, mStatusArg);
	return 0;
}

int TMario::shortBackDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(0xC);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(0x7B, 88.0f, mStatusArg);
	return 0;
}

int TMario::shortForeDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(0xC);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(0x7C, 80.0f, mStatusArg);
	return 0;
}

int TMario::safeBackDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(0xC);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(0x74, 200.0f, mStatusArg);
	return 0;
}

int TMario::safeForeDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(0xC);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(0x75, 100.0f, mStatusArg);
	return 0;
}

int TMario::catchDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer++;
		emitParticle(0xC);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
	downingCommon(0x8A, 128.0f, mStatusArg);
	return 0;
}

BOOL TMario::loserDown()
{
	slopeProcess();
	mForwardVel *= 0.9f;
	if (mForwardVel * mForwardVel < 1.0f)
		setPlayerVelocity(0.0f);

	setAnimation(0x113, 1.0f);
	switch (mStatusState) {
	case 0:
		startVoice(0x785D);
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
		startVoice(0x7861);
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
		setAnimation(0x6C, 1.0f);
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
		if (rec->mJumpStatus == STATUS_SECOND_JUMP)
			if (mForwardVel >= mJumpParams.mSecJumpEnableSp.get())
				return changePlayerJumping(STATUS_SECOND_JUMP, 0);

		if (rec->mJumpStatus == STATUS_ULTRA_JUMP)
			if (mForwardVel >= mJumpParams.mTriJumpEnableSp.get())
				return changePlayerJumping(STATUS_ULTRA_JUMP, 0);

		return changePlayerJumping(STATUS_JUMP, 0);
	}

	if (mInput & 0x8000) {
		mVel.y = 20.0f;
		return changePlayerStatus(0x80088A, 1, false);
	}

	if (mInput & 0x4)
		return changePlayerStatus(rec->unkC, 0, false);

	return 0;
}

void TMario::jumpSlip() { }

void TMario::landSlip() { }

void TMario::secJumpSlip() { }

void TMario::uTurnJumpSlip() { }

void TMario::ultraJumpSlip() { }

void TMario::broadJumpSlip() { }

BOOL TMario::moveMain()
{
	BOOL ret = 0;

	checkEnforceJump();
	checkReturn();

	if ((mStatus & STATUS_FLAG_UNK40000) ? true : false) {
		if (!((mStatus & 0x4045C) ? true : false)) {
			if (!((mStatus & 0x84045D) ? true : false)) {
				SMSGetMSound()->startSoundActor(0x1009, &mPosition, 0, nullptr,
				                                0, 4);
			}
		}
	}

	switch (mStatus) {
	case STATUS_RUN:
		ret = running();
		break;

	case STATUS_ROTATE_L:
	case STATUS_ROTATE_R:
		ret = rotating();
		break;

	case STATUS_TURN:
		ret = turnning();
		break;

	case STATUS_TURN_END:
		ret = turnEnd();
		break;

	case STATUS_BRAKE:
		ret = braking();
		break;

	case STATUS_SURF:
		ret = surfing();
		break;

	case STATUS_TOROCCO:
		ret = toroccoing();
		break;

	case STATUS_WALK_END:
		ret = walkEnd();
		break;

	case STATUS_SPEED_SLIDING:
		ret = speedSliding();
		break;

	case STATUS_FIRE_DASH:
		ret = fireDashing();
		break;

	case STATUS_SLIP_FORE:
		ret = slipFore();
		break;

	case STATUS_SLIP_BACK:
		ret = slipBack();
		break;

	case STATUS_CATCH:
		ret = catching();
		break;

	case STATUS_SQUAT_SLIP:
		ret = squatSlipping();
		break;

	case STATUS_OIL_RUN:
		ret = oilRun();
		break;

	case STATUS_OIL_SLIP:
		ret = oilSlip();
		break;

	case STATUS_OIL_SLOPE:
		ret = oilSlope();
		break;

	case STATUS_BACK_DOWN:
		ret = backDown();
		break;

	case STATUS_FORE_DOWN:
		ret = foreDown();
		break;

	case STATUS_SHORT_BACK_DOWN:
		ret = shortBackDown();
		break;

	case STATUS_SHORT_FORE_DOWN:
		ret = shortForeDown();
		break;

	case STATUS_SAFE_BACK_DOWN:
		ret = safeBackDown();
		break;

	case STATUS_SAFE_FORE_DOWN:
		ret = safeForeDown();
		break;

	case STATUS_CATCH_DOWN:
		ret = catchDown();
		break;

	case STATUS_LOSER_DOWN:
		ret = loserDown();
		break;

	case 0x4000470:
		ret = unknown_inline_4(this);
		break;

	case 0x4000471:
		ret = unknown_inline_5(this);
		break;

	case 0x4000472:
		ret = unknown_inline_6(this);
		break;

	case 0x4000473:
		ret = unknown_inline_7(this);
		break;

	case 0x4000478:
		ret = unknown_inline_8(this);
		break;

	case 0x479:
		ret = unknown_inline_9(this);
		break;
	}

	return ret;
}
