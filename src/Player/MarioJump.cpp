#include <Player/Mario.hpp>
#include <JSystem/JMath.hpp>
#include <Strategic/LiveActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Map/MapData.hpp>
#include <Camera/CameraShake.hpp>
#include <System/Particles.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/DrawUtil.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

BOOL TMario::startJumpWall()
{
	if (mWallPlane != NULL) {
		const JGeometry::TVec3<f32>& normal = mWallPlane->getNormal();
		s16 angle = matan(mWallPlane->mMinY, normal.x) + 0x8000;
		emitParticle(PARTICLE_MS_WALLKICK_A, angle);
		emitParticle(PARTICLE_MS_WALLKICK_B, angle);
	}

	mVel.y = 52.0f;
	mFaceAngle.y += 0x8000;
	if (mVel.y + (160.0f + mPosition.y) >= mFloorPosition.x)
		mVel.y = 1.0f;

	return changePlayerStatus(MARIO_STATUS_WALL_JUMP, 0, 0);
}

void TMario::doJumping()
{
	f32 sideVel = 0.0f;
	mForwardVel *= mJumpParams.mJumpSpeedBrake.get();

	if (mInput & 1) {
		f32 mag       = mIntendedMag;
		s16 angleDiff = mIntendedYaw - mFaceAngle.y;

		if (mStatus == MARIO_STATUS_ROCKET
		    && checkFlag(MARIO_STATUS_FLAG_UNK8000)) {
			if (mWaterGun->canSpray())
				mag = 2.5f * mIntendedMag;
		}

		if (mStatus == MARIO_STATUS_WALL_JUMP && mVel.y > 0.0f
		    && (angleDiff < -0x4000 || angleDiff > 0x4000)) {
			mag = 0.0f;
		}

		f32 accelControl;
		if (onYoshi() && (mYoshi->mFlutterState == 1 ? true : false)) {
			if (angleDiff > -0x4000 && angleDiff < 0x4000)
				accelControl = mYoshiParams.mHoldOutAccCtrlF.get();
			else
				accelControl = mYoshiParams.mHoldOutAccCtrlB.get();
		} else {
			accelControl = getJumpAccelControl();
		}

		mForwardVel += mag * JMASCos(angleDiff) * accelControl;
		sideVel = mag * JMASSin(angleDiff) * getJumpSlideControl();
	}

	if (mForwardVel > 32.0f)
		mForwardVel -= 0.2f;
	if (mForwardVel < -16.0f)
		mForwardVel += 0.4f;

	mSlideVelX = mForwardVel * JMASSin(mFaceAngle.y);
	mSlideVelZ = mForwardVel * JMASCos(mFaceAngle.y);
	mSlideVelX += sideVel * JMASSin(mFaceAngle.y + 0x4000);
	mSlideVelZ += sideVel * JMASCos(mFaceAngle.y + 0x4000);
	mVel.x = mSlideVelX;
	mVel.z = mSlideVelZ;

	if (mVel.y < 0.0f) {
		setAttackRadius(mDeParams.mTrampleRadius.get());
		setAttackHeight(mDeParams.mAttackHeight.get());
	} else {
		setAttackRadius(mDeParams.mPushupRadius.get());
		setAttackHeight(mDeParams.mPushupHeight.get());
	}
}

void TMario::checkJumpingThrowStart() { }

void TMario::askStrongGroundTouch() { }

BOOL TMario::jumpingBasic(int statusOnGround, int animation, int processArg)
{
	doJumping();
	int result = jumpProcess(processArg);
	switch (result) {
	case 0:
		setAnimation(animation, 1.0f);
		break;

	case 1: {
		if (mGroundPlane->mActor != nullptr)
			((THitActor*)mGroundPlane->mActor)->receiveMessage(this, 0);

		bool didTrample = false;

		bool isStrong = true;
		if (checkUnk114(UNK114_FLAG_UNK100) == true)
			isStrong = false;

		if (unk2A8.y - mPosition.y <= mDeParams.mDamageFallHeight.get())
			isStrong = false;

		if (onYoshi())
			isStrong = false;

		if (mGroundPlane->isThing4())
			isStrong = false;

		if (mVel.y > -70.0f)
			isStrong = false;

		if (isStrong) {
			if (checkFlag(MARIO_FLAG_ON_SAND)) {
				sinkInSandEffect();
				return changePlayerStatus(MARIO_STATUS_FOOT_DOWN, 0, 0);
			}
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)
			    && (int)mWaterGun->mCurrentNozzle != 2) {
				mTrembleModelEffect->tremble(mJumpParams.mTremblePower.get(),
				                             mJumpParams.mTrembleAccele.get(),
				                             mJumpParams.mTrembleBrake.get(),
				                             mJumpParams.mTrembleTime.get());
				changePlayerStatus(MARIO_STATUS_BROAD_JUMP_SLIP, 0, 0);
				rumbleStart(0x15, mMotorParams.mMotorHipDrop.get());
				didTrample = true;
				startVoice(0x789EU);
				SMSGetMSound()->startSoundActor(MSD_SE_MA_FALL_AFTER,
				                                &mPosition, 0, nullptr, 0, 4);
				strongTouchDownEffect();
				floorDamageExec(1, 3, 0, mMotorParams.mMotorTrample.get());
			}
		}

		unk2A8.y = mPosition.y;
		changePlayerStatus(statusOnGround, 0, 0);
		if (!didTrample) {
			rumbleStart(0x14, (s16)mMotorParams.mMotorWall.get() / 2);
			stopVoice();
		}

		if (mPrevStatus == MARIO_STATUS_U_TURN_JUMP
		    || mPrevStatus == MARIO_STATUS_LEFT_ROTATE_JUMP
		    || mPrevStatus == MARIO_STATUS_RIGHT_ROTATE_JUMP)
			strongTouchDownEffect();
		else
			smallTouchDownEffect();
		break;
	}

	case 2:
		if (mStatus != MARIO_STATUS_WIRE_ROLL_JUMP
		    && mForwardVel > mDeParams.mClashSpeed.get()) {
			emitParticle(PARTICLE_MS_DMG_C);
			changePlayerDropping(MARIO_STATUS_JUMP_SHORT_BACK_DOWN, 0U);
			return result;
		}
		setAnimation(animation, 1.0f);
		if (onYoshi()) {
			setPlayerVelocity(0.0f);
			break;
		}
		if (mWallPlane != nullptr) {
			if (mWallPlane->isNoWallJump()) {
				changePlayerStatus(MARIO_STATUS_ROCKET_LANDING, 0, 0);
				setPlayerVelocity(0.0f);
				break;
			}
		}
		if (mWallPlane != nullptr) {
			if (mWallPlane->isFence()) {
				const JGeometry::TVec3<f32>& normal = mWallPlane->getNormal();
				mFaceAngle.y    = matan(normal.z, normal.x) + 0x8000;
				mModelFaceAngle = mFaceAngle.y;
				if (mStatus == MARIO_STATUS_U_TURN_JUMP)
					mModelFaceAngle -= 0x8000;
				rumbleStart(0x15, mMotorParams.mMotorWall.get());
				return changePlayerStatus(MARIO_STATUS_FENCE_JUMP_CATCH, 0, 0);
			}
		}
		if (mForwardVel > 16.0f && mStatus != MARIO_STATUS_ROCKET_LANDING) {
			playerRefrection(0);
			mFaceAngle.y += 0x8000;
			if (mWallPlane != nullptr) {
				changePlayerStatus(MARIO_STATUS_WALL_SLIDE, 0, 0);
				if (isMario()) {
					rumbleStart(0x15, mMotorParams.mMotorWall.get());
					gpCameraShake->startShake((EnumCamShakeMode)1, 1.0f);
					u32 sfx
					    = gpMSound->getWallSound(mWallPlane->unk6, mForwardVel);
					SMSGetMSound()->startSoundActor(sfx, &mPosition, 0, nullptr,
					                                0, 4);
				}
				break;
			}
			if (mVel.y > 0.0f)
				mVel.y = 0.0f;
			if (mForwardVel >= 38.0f) {
				changePlayerStatus(MARIO_STATUS_JUMP_SHORT_BACK_DOWN, 0, 0);
				break;
			}
			if (mForwardVel > 8.0f)
				setPlayerVelocity(-8.0f);
			return changePlayerStatus(MARIO_STATUS_LAND_SAFE_DOWN, 0, 0);
		}
		setPlayerVelocity(0.0f);
		break;

	case 3:
		setAnimation(ANIM_HANG, 1.0f);
		changePlayerDropping(MARIO_STATUS_HANGING, 0U);
		break;

	case 4:
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
		changePlayerStatus(MARIO_STATUS_HANG_ROOF, 0, 0);
		break;
	}
	return result;
}

BOOL TMario::considerJumpRotate()
{
	int spC;

	if (checkStickRotate(&spC) == 1) {
		switch (spC) {
		case 2:
			mStatus = MARIO_STATUS_RIGHT_ROTATE_JUMP;
			break;
		case 3:
			mStatus = MARIO_STATUS_LEFT_ROTATE_JUMP;
			break;
		}
		return 1;
	}
	return 0;
}

BOOL TMario::checkBackTrig()
{
	if (mInput & 0x8000) {
		if (mGamePad->mEnabledFrameMeaning & 0x2000) {
			return changePlayerStatus(MARIO_STATUS_HIP_DROP, 0, 0);
		}
		if (onYoshi() == 0) {
			setPlayerVelocity(mJumpParams.mJumpJumpCatchSp.get());
			return changePlayerStatus(MARIO_STATUS_JUMP_CATCH, 0, 0);
		}
	}
	return 0;
}

BOOL TMario::jumpingCommonEvents()
{
	if (checkBackTrig())
		return 1;
	if (rocketCheck())
		return 1;
	if (considerJumpRotate())
		return 1;

	return 0;
}

BOOL TMario::jumping()
{
	if (jumpingCommonEvents())
		return 1;

	switch (mStatus) {
	case 0x89C:
		jumpingBasic(MARIO_STATUS_THROWN_DOWN, ANIM_THROWN, 0);
		break;
	case MARIO_STATUS_FORCE_JUMP: {
		int anim;
		if (mVel.y >= 0.0f)
			anim = ANIM_2JMP1;
		else
			anim = ANIM_2JMP2;
		jumpingBasic(MARIO_STATUS_JUMP_SLIP, anim, 3);
		break;
	}
	default:
		jumpingBasic(MARIO_STATUS_JUMP_SLIP, ANIM_JUMP, 3);
		break;
	}
	return 0;
}

BOOL TMario::secJumping()
{
	if (mStatusTimer == 0) {
		mStatusTimer += 1;
		rumbleStart(0x14, mMotorParams.mMotorWall.get());
	}

	int anim;
	if (mVel.y >= 0.0f)
		anim = ANIM_2JMP1;
	else
		anim = ANIM_2JMP2;

	if (jumpingCommonEvents())
		return 1;

	jumpingBasic(MARIO_STATUS_SEC_JUMP_SLIP, anim, 3);
	return 0;
}

BOOL TMario::ultraJumping()
{
	if (mStatusTimer == 0) {
		mStatusTimer += 1;
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}

	if (checkBackTrig())
		return 1;

	if (rocketCheck())
		return 1;

	jumpingBasic(MARIO_STATUS_ULTRA_JUMP_SLIP, ANIM_ROLL, 0);
	return 0;
}

BOOL TMario::backJumping()
{
	if (checkBackTrig())
		return 1;

	if (rocketCheck())
		return 1;

	if (mStatusState == 0 && isLast1AnimeFrame())
		mStatusState = 1;

	if (mStatusState == 0)
		jumpingBasic(MARIO_STATUS_SEC_JUMP_SLIP, 0xF7, 0);
	else
		jumpingBasic(MARIO_STATUS_SEC_JUMP_SLIP, ANIM_LAND, 0);

	return 0;
}

BOOL TMario::landing()
{
	if (mVel.y < 0.0f) {
		if (mStatusTimer++ > 240) {
			mStatusTimer = 240;
			startSoundActor(MSD_SE_MV10B_CRY_JUMP_01);
			mStatusArg = 3;
		}
	}

	if (checkBackTrig())
		return true;

	if (rocketCheck())
		return true;

	int r31;
	switch (mStatusArg) {
	case 0:
		r31 = ANIM_LAND;
		break;
	case 1:
		r31 = ANIM_SLPLA;
		break;
	case 3:
		r31 = ANIM_THROWN;
		break;
	}

	jumpingBasic(MARIO_STATUS_LAND_SLIP, r31, 3);
	return 0;
}

BOOL TMario::uTurnJumping()
{
	if (jumpingCommonEvents())
		return 1;

	if (checkBackTrig())
		return 1;

	if (jumpingBasic(MARIO_STATUS_U_TURN_JUMP_SLIP, ANIM_TJMP1, 1) != 3) {
		mFaceAngle.x = 0;
		mModelFaceAngle += 0x8000;
	}
	return 0;
}

BOOL TMario::jumpWall()
{
	if (checkBackTrig())
		return 1;
	if (considerJumpRotate())
		return 1;
	if (rocketCheck())
		return 1;

	jumpingBasic(MARIO_STATUS_JUMP_SLIP, ANIM_WJMP, 3);
	return 0;
}

BOOL TMario::jumpCatch()
{
	if ((mInput & 0x8000) != 0
	    && ((mGamePad->mEnabledFrameMeaning & 0x2000) != 0)) {
		return changePlayerStatus(MARIO_STATUS_HIP_DROP, 0, false);
	}
	setAnimation(ANIM_SLDCT, 1.0);
	doJumping();

	switch (jumpProcess(0)) {
	case 0:
		break;

	case 1: {
		bool isStrong = true;
		if (checkUnk114(UNK114_FLAG_UNK100) == true)
			isStrong = false;

		if (unk2A8.y - mPosition.y <= mDeParams.mDamageFallHeight.get())
			isStrong = false;

		if (onYoshi())
			isStrong = false;

		if (mGroundPlane->isThing4())
			isStrong = false;

		if (mVel.y > -70.0f)
			isStrong = false;

		if (isStrong && checkFlag(MARIO_FLAG_ON_SAND)) {
			sinkInSandEffect();
			changePlayerStatus(MARIO_STATUS_FOOT_DOWN, 1, false);
		} else {
			changePlayerStatus(MARIO_STATUS_CATCH, 0, false);
		}
		break;
	}

	case 2:
		if (mWallPlane && mWallPlane->isFence())
			return changePlayerDropping(MARIO_STATUS_FENCE_JUMP_CATCH, 0);
		playerRefrection(1);
		if (mVel.y > 0.0f) {
			mVel.y = 0.0f;
		}
		emitParticle(PARTICLE_MS_DMG_C);
		changePlayerDropping(MARIO_STATUS_JUMP_SHORT_BACK_DOWN, 0);
		break;
	}

	return 0;
}

BOOL TMario::jumpingThrow()
{
	setAnimation(ANIM_THROW, 1.0f);
	checkThrowObject();
	doJumping();

	switch (jumpProcess(0)) {
	case 1:
		mStatus = MARIO_STATUS_JUMP_THROW_END;
		break;

	case 2:
		setPlayerVelocity(0.0f);
		break;
	}

	return 0;
}

BOOL TMario::jumpDownCommon(int param_1, int animation, float velocity)
{
	setPlayerVelocity(velocity);
	int result = jumpProcess(0);
	switch (result) {
	case 0:
		setAnimation(animation, 1.0f);
		break;
	case 1:
		changePlayerStatus(param_1, mStatusArg, 0);
		break;
	case 2:
		setAnimation(ANIM_BKDWN, 1.0f);
		playerRefrection(0);
		if (mVel.y > 0.0f)
			mVel.y = 0.0f;

		setPlayerVelocity(-velocity);
		break;
	}
	return result;
}

void TMario::checkWallJumping() { }

BOOL TMario::jumpShortBackDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer += 1;
		rumbleStart(0x15, 0x14);
	}
	jumpDownCommon(MARIO_STATUS_SHORT_BACK_DOWN, ANIM_BKDWN, -16.0f);
	return 0;
}

BOOL TMario::jumpShortForeDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer += 1;
		rumbleStart(0x15, 0x14);
	}
	jumpDownCommon(MARIO_STATUS_SHORT_FORE_DOWN, ANIM_JFDWN, 16.0f);
	return 0;
}

BOOL TMario::jumpBackDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer += 1;
		rumbleStart(0x15, 0x14);
	}
	jumpDownCommon(MARIO_STATUS_BACK_DOWN, ANIM_BKDWN, -16.0f);
	return 0;
}

BOOL TMario::jumpForeDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer += 1;
		rumbleStart(0x15, 0x14);
	}
	jumpDownCommon(MARIO_STATUS_FORE_DOWN, ANIM_JFDWN, 16.0f);
	return 0;
}

BOOL TMario::landSafeDown()
{
	jumpDownCommon(MARIO_STATUS_LAND_SLIP, ANIM_LAND, mForwardVel);
	return 0;
}

BOOL TMario::stayWall()
{
	mStatusTimer += 1;
	if (mStatusTimer > 60)
		mStatusTimer = 60;

	if (mInput & 2) {
		if (mWallPlane) {
			s16 sVar5
			    = matan(mWallPlane->getNormal().z, mWallPlane->getNormal().x)
			      + 0x8000;
			emitParticle(PARTICLE_MS_WALLKICK_A, sVar5);
			emitParticle(PARTICLE_MS_WALLKICK_B, sVar5);
		}

		mVel.y = 52.0f;
		mFaceAngle.y += 0x8000;
		if (mVel.y + (mPosition.y + 160.0f) >= mFloorPosition.x)
			mVel.y = 1.0f;
		return changePlayerStatus(MARIO_STATUS_WALL_JUMP, 0, false);
	}

	if (checkBackTrig())
		return true;

	if (mStatusTimer < 20) {
		mStatusTimer += 1;
		mVel.set(0.0f, 0.0f, 0.0f);
	} else {
		mVel.y = -mStatusTimer * 0.5f;
	}

	if (mWallPlane) {
		mPosition.x -= mWallPlane->getNormal().x;
		mPosition.z -= mWallPlane->getNormal().z;
	}

	switch (jumpProcess(0)) {
	case 1:
		mFaceAngle.y += 0x8000;
		return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
	}

	if (mWallPlane == nullptr) {
		mFaceAngle.y += 0x8000;
		setPlayerVelocity(0.0f);
		mVel.y = 0.0f;
		return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
	}

	setAnimation(ANIM_WSLD, 1.0f);
	if (mVel.y < -10.0f) {
		wallSlipEffect();
		SMSGetMSound()->startSoundActor(MSD_SE_MA_SLIP_WALL, &mPosition, 0,
		                                nullptr, 0, 4);
	}

	return false;
}

BOOL TMario::catchStop()
{
	if (mStatusState == 0) {
		mVel.y       = 30.0f;
		mStatusState = 1;
	}
	doJumping();
	switch (jumpProcess(0)) {
	case 0:
		if (mStatusState == 1)
			setAnimation(ANIM_ROLL, 1.0f);
		else
			setAnimation(ANIM_LAND, 1.0f);
		break;
	case 1:
		changePlayerStatus(MARIO_STATUS_LAND_END, 0, 0);
		break;
	case 2:
		setPlayerVelocity(0.0f);
		break;
	}
	if (mStatusState == 1 && isLast1AnimeFrame()) {
		mStatusState = 2;
	}
	return 0;
}

BOOL TMario::slipFalling()
{
	mStatusTimer += 1;
	if (mStatusTimer > 120 && mPosition.y - mFloorPosition.y > 500.0f)
		return changePlayerStatus(MARIO_STATUS_LANDING, 1, false);

	mForwardVel *= mJumpParams.mJumpSpeedBrake.get();
	if (mInput & 1) {
		u16 angleDiff    = mIntendedYaw - mFaceAngle.y;
		f32 velIncrement = 0.03125f * mIntendedMag;

		mForwardVel
		    += velIncrement * JMASCos(angleDiff) * getJumpAccelControl();

		mFaceAngle.y
		    += velIncrement * JMASSin(angleDiff) * getJumpSlideControl();
	}

	if (mForwardVel > 32.0f)
		mForwardVel -= 0.2f;

	if (mForwardVel < -16.0f)
		mForwardVel += 0.4f;

	mVel.x = mSlideVelX = mForwardVel * JMASSin(mFaceAngle.y);
	mVel.z = mSlideVelZ = mForwardVel * JMASCos(mFaceAngle.y);

	switch (jumpProcess(0)) {
	case 1:
		if (mStatusState == 0 && mVel.y < 0.0f
		    && mGroundPlane->getNormal().y >= 0.9848077f) {
			mVel.y       = -mVel.y / 2.0f;
			mStatusState = 1;
		} else {
			changePlayerStatus(MARIO_STATUS_SLIP_FORE, 0, false);
		}
		break;

	case 2:
		if (mVel.y > 0.0f)
			mVel.y = 0.0f;
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
		changePlayerStatus(MARIO_STATUS_JUMP_SHORT_BACK_DOWN, 0, false);
		break;
	}
	setAnimation(ANIM_SLIP, 1.0f);
	return false;
}

BOOL TMario::fireDowning()
{
	if (mStatusTimer == 1)
		startVoice(MSD_SE_MV05_DAMAGE_FIRE_01);
	mStatusTimer += 1;

	if (!(mInput & 1))
		mForwardVel = FConverge(mForwardVel, 0.0f, 0.35f, 0.35f);

	if (mInput & 1) {
		u16 angleDiff = mIntendedYaw - mFaceAngle.y;
		f32 velIncrement
		    = 0.03125f * mIntendedMag * mJumpParams.mFireDownControl.get();

		mForwardVel += velIncrement * JMASCos(angleDiff);
		mFaceAngle.y += 1024.0f * (velIncrement * JMASSin(angleDiff));

		if (mForwardVel < 0.0f) {
			mFaceAngle.y += 0x8000;
			mForwardVel *= -1.0f;
		}
		if (mForwardVel > 32.0f)
			mForwardVel -= 2.0f;
	}

	mVel.x = mSlideVelX = mForwardVel * JMASSin(mFaceAngle.y);
	mVel.z = mSlideVelZ = mForwardVel * JMASCos(mFaceAngle.y);

	switch (jumpProcess(0)) {
	case 1:
		if (mStatusState < 2 && mVel.y < 0.0f) {
			mVel.y = 0.4f * -mVel.y;
			setPlayerVelocity(0.5f * mForwardVel);
			mStatusState += 1;
		} else {
			startVoice(MSD_SE_MV07_DAMAGE_REACT_01);
			changePlayerStatus(MARIO_STATUS_FIRE_JUMP_END, 0, 0);
		}
		break;

	case 2:
		playerRefrection(0);
		break;
	}
	setAnimation(ANIM_FIREJMP, 1.0f);
	return 0;
}

BOOL TMario::thrownDowning()
{
	u16 angleDiff    = mIntendedYaw - mFaceAngle.y;
	f32 velIncrement = 0.03125f * mIntendedMag;

	mForwardVel
	    += velIncrement * JMASCos(angleDiff) * mJumpParams.mThrownAccel.get();

	mFaceAngle.y
	    += velIncrement * JMASSin(angleDiff) * mJumpParams.mThrownSlide.get();

	mForwardVel *= mJumpParams.mThrownBrake.get();

	mVel.x = mSlideVelX = mForwardVel * JMASSin(mFaceAngle.y);
	mVel.z = mSlideVelZ = mForwardVel * JMASCos(mFaceAngle.y);

	switch (jumpProcess(0)) {
	case 1:
		if (mStatusState < 2 && mVel.y < 0.0f) {
			mVel.y = 0.4f * -mVel.y;
			setPlayerVelocity(0.5f * mForwardVel);
			mStatusState += 1;
		} else {
			return changePlayerStatus(MARIO_STATUS_THROWN_END, 0, 0);
		}
		break;

	case 2:
		playerRefrection(0);
		break;
	}
	setAnimation(ANIM_THROWN, 1.0f);
	return 0;
}

BOOL TMario::trample()
{
	if (jumpingCommonEvents())
		return 1;

	jumpingBasic(MARIO_STATUS_SEC_JUMP_SLIP, mAnimationId, 3);
	return 0;
}

BOOL TMario::missJumping()
{
	if (mInput & 0x8000)
		return changePlayerStatus(MARIO_STATUS_JUMP_CATCH, 0, 0);

	setPlayerVelocity(0.98f * mForwardVel);
	switch (jumpProcess(0)) {
	case 1: {
		u32 nextStatus;
		if (mForwardVel < 0.0f)
			nextStatus = MARIO_STATUS_SLIP;
		else
			nextStatus = MARIO_STATUS_JUMP_SLIP;
		changePlayerStatus(nextStatus, 0, 0);
		break;
	}
	case 2:
		setPlayerVelocity(0.0f);
		break;
	}

	setAnimation(ANIM_JUMP, 1.0f);
	return 0;
}

BOOL TMario::fireJumping()
{
	if (mStatusTimer == 1)
		startVoice(MSD_SE_MV05_DAMAGE_FIRE_01);

	mStatusTimer += 1;
	setPlayerVelocity(mForwardVel);

	if (jumpProcess(0) == 1)
		changePlayerStatus(MARIO_STATUS_FIRE_DASH, 0, 0);

	int anim;
	if (mStatusArg == 0)
		anim = ANIM_JUMP;
	else
		anim = ANIM_FIREJMP;

	setAnimation(anim, 1.0f);
	return 0;
}

BOOL TMario::fireLanding()
{
	setPlayerVelocity(mForwardVel);
	if (jumpProcess(0) == 1)
		changePlayerStatus(MARIO_STATUS_FIRE_DASH, 0, 0);
	setAnimation(ANIM_LAND, 1.0f);
	return 0;
}

BOOL TMario::broadJumping()
{
	jumpingBasic(MARIO_STATUS_RUN, ANIM_BROAD_JUMP, 1);
	onFlag(MARIO_FLAG_FLUDD_EMITTING);
	return 0;
}

BOOL TMario::rotateBroadJumping()
{
	jumpingBasic(MARIO_STATUS_RUN, ANIM_ROLL_JUMP, 1);
	return 0;
}

BOOL TMario::boardJumping()
{
	setAnimation(ANIM_RIDE_SHELL, 1.0f);
	if (mVel.y < 0.0f) {
		setAttackRadius(mDeParams.mTrampleRadius.get());
		setAttackHeight(mDeParams.mAttackHeight.get());
	} else {
		setAttackRadius(mDeParams.mPushupRadius.get());
		setAttackHeight(mDeParams.mPushupHeight.get());
	}
	switch (jumpProcess(0)) {
	case 1:
		if (mVel.y < 0.0f)
			changePlayerStatus(MARIO_STATUS_SURF, 0, 0);
		break;
	case 2:
		if (mWallPlane == nullptr) {
			setPlayerVelocity(0.0f);
			loserExec();
		} else {
			s16 diff
			    = matan(mWallPlane->getNormal().z, mWallPlane->getNormal().x)
			      - mFaceAngle.y;
			s16 max = mSurfingParamsWaterRed.mClashAngle.get();
			if ((diff < -max || max < diff)
			    && mForwardVel > mSurfingParamsWaterRed.mClashSpeed.get()) {
				loserExec();
			} else {
				setPlayerVelocity(0.0f);
			}
		}
		break;
	}
	return 0;
}

BOOL TMario::rocketCheck()
{
	bool bVar2 = true;
	if (mStatus == MARIO_STATUS_ROCKET)
		bVar2 = false;
	if (mStatus == MARIO_STATUS_ROCKET_LANDING)
		bVar2 = false;

	if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
		if (mWaterGun->getEmitParams().mRocketType.get() != 1)
			bVar2 = false;

		if (!isUpperState(UPPER_STATE_PUMPING))
			bVar2 = false;

		if (!mWaterGun->canSpray())
			bVar2 = false;
	} else {
		bVar2 = false;
	}

	if (bVar2 == true) {
		unk314 = mPosition.y + mWaterGun->mWatergunParams.mHoverHeight.get();
		return changePlayerStatus(MARIO_STATUS_ROCKET, 0, false);
	}

	return 0;
}

BOOL TMario::rocketing()
{
	if (!checkFlag(MARIO_FLAG_HAS_FLUDD))
		return changePlayerStatus(MARIO_STATUS_ROCKET_LANDING, 0, 0);

	{
		if (mWaterGun->getEmitParams().mRocketType.get() != 1)
			return changePlayerStatus(MARIO_STATUS_ROCKET_LANDING, 0, 0);
	}

	if (!isUpperState(UPPER_STATE_PUMPING) || !mWaterGun->canSpray())
		return changePlayerStatus(MARIO_STATUS_ROCKET_LANDING, 0, 0);

	if (mInput & 1) {
		switch (mWaterGun->mCurrentNozzle) {
		case TWaterGun::Hover:
			f32 mag       = mIntendedMag;
			s16 angleDiff = mIntendedYaw - mFaceAngle.y;

			if ((angleDiff > -0x1555 && angleDiff < 0x1555)
			    || angleDiff < -0x6AAA || angleDiff > 0x6AAA) {
				s16 nozzleAngle;
				if (angleDiff >= -0x4000 && angleDiff <= 0x4000) {
					nozzleAngle = (s16)(0.03125f * -mag
					                    * mWaterGun->getEmitParams()
					                          .mSideAngleMaxFront.get()
					                    * JMASCos(angleDiff));
				} else {
					nozzleAngle = (s16)(0.03125f * -mag
					                    * mWaterGun->getEmitParams()
					                          .mSideAngleMaxBack.get()
					                    * JMASCos(angleDiff));
				}
				mWaterGun->unk1CC2 = nozzleAngle;
				mWaterGun->unk1CC4 = nozzleAngle;
				mForwardVel += mag * JMASCos(angleDiff)
				               * mDivingParams.mAccelControl.get();
			} else {
				s16 nozzleAngle
				    = (s16)(0.03125f * -mag
				            * mWaterGun->getEmitParams().mSideAngleMaxSide.get()
				            * JMASSin(angleDiff));
				if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
					mWaterGun->unk1CC2 = -nozzleAngle;
					mWaterGun->unk1CC4 = nozzleAngle;
					s16 rotSp          = mHoverParams.mRotSp.get();
					mFaceAngle.y
					    = mIntendedYaw
					      - IConverge((s16)(mIntendedYaw - mFaceAngle.y), 0,
					                  rotSp, rotSp);
				}
			}
			break;
		}
	} else {
		mWaterGun->unk1CC2 = 0;
		mWaterGun->unk1CC4 = 0;
	}

	mSlideVelX = mForwardVel * JMASSin(mFaceAngle.y);
	mSlideVelZ = mForwardVel * JMASCos(mFaceAngle.y);
	mVel.x     = mSlideVelX;
	mVel.z     = mSlideVelZ;

	switch (mWaterGun->mCurrentNozzle) {
	case TWaterGun::Hover:
		mVel.y = (unk314 - mPosition.y) * mHoverParams.mAccelRate.get();
		mForwardVel *= mHoverParams.mBrake.get();
		break;
	}

	switch (jumpProcess(2)) {
	case 1:
	case 2:
		break;

	case 4:
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
		changePlayerStatus(MARIO_STATUS_HANG_ROOF, 0, 0);
		break;
	}

	if (mRoofPlane != nullptr)
		if (160.0f + mPosition.y > mFloorPosition.x)
			mPosition.y = mFloorPosition.x - 160.0f;

	setAnimation(ANIM_LAND, 1.0f);
	return 0;
}

BOOL TMario::rotateJumping()
{
	if (checkBackTrig())
		return 1;

	if (rocketCheck())
		return 1;

	setAnimation(ANIM_SPIN_P, 1.0f);
	emitBlurSpinJump();
	jumpingBasic(MARIO_STATUS_SEC_JUMP_SLIP, mAnimationId, 0);
	mStatusTimer += 1;

	if (mStatus == MARIO_STATUS_RIGHT_ROTATE_JUMP)
		mModelFaceAngle = mStatusTimer * 4096;
	else
		mModelFaceAngle = -(mStatusTimer * 4096);

	if (!(gpMarDirector->unk58 & 0x3F))
		rumbleStart(0x14, mMotorParams.mMotorWall.get() / 2);

	return 0;
}

BOOL TMario::wireJumping()
{
	if (mPrevStatus == MARIO_STATUS_WIRE_ROLLING) {
		if (mStatusArg == 0U)
			setAnimation(ANIM_GIANT_ROLLING, 1.0f);
		else
			setAnimation(ANIM_ROLL, 1.0f);
	} else {
		setAnimation(ANIM_JUMP, 1.0f);
	}

	if (jumpingCommonEvents())
		return 1;

	jumpingBasic(MARIO_STATUS_SEC_JUMP_SLIP, mAnimationId, 3);
	return 0;
}

BOOL TMario::pullJumping()
{
	if (mAnimationId == ANIM_HOLD_TO_HANG && isLast1AnimeFrame())
		setAnimation(ANIM_HOLD_HANG, 1.0f);

	mForwardVel  = 0.0f;
	mIntendedMag = 0.0f;
	jumpingBasic(MARIO_STATUS_PULLING, mAnimationId, 3);

	if (mStatus == MARIO_STATUS_PULLING)
		setAnimation(ANIM_HANG_TO_HOLD, 1.0f);

	JGeometry::TVec3<f32> pos = mPosition;
	if (mHeldObject->moveRequest(pos) == 1)
		mPosition = pos;
	return 0;
}

BOOL TMario::hipAttacking()
{
	for (int i = 0; i < mColCount; i++) {
		if (mCollisions[i]->isActorType(0x4000000B)) {
			if (mCollisions[i]->mPosition.distance(mPosition) > 70.0f) {
				mPosition.x = mCollisions[i]->mPosition.x;
				mPosition.z = mCollisions[i]->mPosition.z;
			}
		}
	}

	switch (mStatusState) {
	case 0:
		startVoice(MSD_SE_MV15_EXERT_INST_01);
		mStatusState = 1;
		// fallthrough

	case 1:
		if (mFloorPosition.y > mPosition.y) {
			mPosition.y = 1.0f + mFloorPosition.y;
			changePlayerStatus(MARIO_STATUS_HIP_ATTACK_END, 0, 0);
		}
		if (mStatusTimer < 0x28) {
			f32 lift = (f32)(0x28 - mStatusTimer) * 0.5f;
			if (160.0f + (mPosition.y + lift) < mFloorPosition.x) {
				mPosition.y = lift * 0.25f + mPosition.y;
				unk104      = mPosition.y;
			}
		}
		setPlayerVelocity(0.0f);
		setAttackRadius(0.0f);
		setAnimation(ANIM_HIPSR, 1.0f);
		mStatusTimer += 1;
		if (mStatusTimer >= 60) {
			mStatusTimer = 0;
			mStatusState = 2;
		}
		mVel.y = 0.0f;
		{
			int r = jumpProcess(0);

			if (r == 1)
				return changePlayerStatus(MARIO_STATUS_JUMP_END, 0, 0);

			if (r == 2) {
				setPlayerVelocity(-16.0f);
				if (mVel.y > 0.0f)
					mVel.y = 0.0f;
				return changePlayerStatus(MARIO_STATUS_JUMP_SHORT_BACK_DOWN, 0,
				                          0);
			}
		}
		break;

	case 2:
	case 3:
		setAnimation(ANIM_HIPAT, 1.0f);
		mStatusTimer += 1;
		if (mStatusTimer > mJumpParams.mSuperHipAttackCt.get())
			mStatusState = 3;

		if (mStatusState == 2)
			mVel.y = mJumpParams.mHipAttackSpeedY.get();
		else
			mVel.y = mJumpParams.mSuperHipAttackSpeedY.get();

		if (mStatusState == 2)
			emitBlurHipDrop();
		else
			emitBlurHipDropSuper();

		setAttackRadius(mDeParams.mHipdropRadius.get());
		setAttackHeight(mDeParams.mAttackHeight.get());

		int r = jumpProcess(0);

		if (r == 1) {
			if (isMario()) {
				if (mStatusState == 2) {
					SMSRumbleMgr->start(0, (f32*)nullptr);
					gpCameraShake->startShake((EnumCamShakeMode)0, 1.0f);
				} else {
					rumbleStart(0x15, 0x1E);
					gpCameraShake->startShake((EnumCamShakeMode)0x27, 1.0f);
				}
			}

			if (mGroundPlane->mActor != nullptr) {
				if (!onYoshi()
				    && mGroundPlane->mActor->mActorType == 0x4000006A) {
					emitParticle(PARTICLE_MS_M_AMIATTACK, &mPosition);
					f32 oldY    = mPosition.y;
					mPosition.y = oldY - 160.0f;
					((THitActor*)mGroundPlane->mActor)
					    ->receiveMessage(this, HIT_MESSAGE_SUPER_HIP_DROP);
					startVoice(MSD_SE_MV28_SPRISE_SMALL_01);
					return changePlayerStatus(MARIO_STATUS_KICK_ROOF_ROLL_DOWN,
					                          0, 0);
				}
				if (mStatusState == 2) {
					((THitActor*)mGroundPlane->mActor)
					    ->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
				} else {
					((THitActor*)mGroundPlane->mActor)
					    ->receiveMessage(this, HIT_MESSAGE_SUPER_HIP_DROP);
					((THitActor*)mGroundPlane->mActor)
					    ->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
				}
			}

			if (mStatusState == 2) {
				emitParticle(PARTICLE_MS_HIPDROP_C);
				emitParticle(PARTICLE_MS_HIPDROP_B);
				emitParticle(PARTICLE_MS_HIPDROP_A);
			} else {
				emitParticle(PARTICLE_MS_M_SPHIPD_HIT_A);
				emitParticle(PARTICLE_MS_M_SPHIPD_HIT_B);
				emitParticle(PARTICLE_MS_M_SPHIPD_HIT_C);
				emitParticle(PARTICLE_MS_M_SPHIPD_HIT_D);
			}
			changePlayerStatus(MARIO_STATUS_HIP_ATTACK_END, 0, 0);
		} else if (r == 2) {
			setPlayerVelocity(-16.0f);
			if (mVel.y > 0.0f)
				mVel.y = 0.0f;
			changePlayerStatus(MARIO_STATUS_JUMP_SHORT_BACK_DOWN, 0, 0);
			rumbleStart(0x15, mMotorParams.mMotorWall.get());
			SMSGetMSound()->startSoundActor(MSD_SE_MA_HIP_ATTACK, &mPosition, 0,
			                                nullptr, 0, 4);
		}
		break;
	}

	mModelFaceAngle = mFaceAngle.y;
	return 0;
}

BOOL TMario::diving()
{
	if (mHolder != nullptr) {
		MtxPtr mtx      = mHolder->getTakingMtx();
		mPosition.x     = mtx[0][3];
		mPosition.y     = mtx[1][3];
		mPosition.z     = mtx[2][3];
		mFaceAngle.x    = 0;
		mModelFaceAngle = mFaceAngle.y;
		mFaceAngle.z    = 0;
		setAnimation(ANIM_THROWN, 1.0f);
		return 0;
	}

	if (mInput & 1) {
		f32 mag       = mIntendedMag;
		s16 angleDiff = mIntendedYaw - mFaceAngle.y;

		if ((angleDiff > -0x1555 && angleDiff < 0x1555) || angleDiff < -0x6AAA
		    || angleDiff > 0x6AAA) {
			s16 nozzleAngle
			    = (s16)(0.03125f * -mag
			            * mWaterGun->getEmitParams().mSideAngleMaxSide.get()
			            * JMASCos(angleDiff));
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				mWaterGun->unk1CC2 = nozzleAngle;
				mWaterGun->unk1CC4 = nozzleAngle;
				mForwardVel += mag * JMASCos(angleDiff)
				               * mDivingParams.mAccelControl.get();
			}
		} else {
			s16 nozzleAngle
			    = 0.03125f * -mag
			      * mWaterGun->getEmitParams().mSideAngleMaxSide.get()
			      * JMASSin(angleDiff);
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				mWaterGun->unk1CC2 = -nozzleAngle;
				mWaterGun->unk1CC4 = nozzleAngle;
				mFaceAngle.y       = mIntendedYaw
				               - IConverge((s16)(mIntendedYaw - mFaceAngle.y),
				                           0, mDivingParams.mRotSp.get(),
				                           mDivingParams.mRotSp.get());
			}
		}
		setAnimation(ANIM_DIVE_WAIT, 1.0f);
	} else {
		if (mPosition.y <= mFloorPosition.y + 4.0f ? true : false) {
			setPlayerVelocity(0.0f);
			switch (mAnimationId) {
			case ANIM_WAIT:
				break;
			case ANIM_DIVE_LAND:
				if (isLast1AnimeFrame())
					setAnimation(ANIM_WAIT, 1.0f);
				break;
			default:
				setAnimation(ANIM_DIVE_LAND, 1.0f);
				break;
			}
		} else {
			setAnimation(ANIM_DIVE_WAIT, 1.0f);
		}
		if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
			mWaterGun->unk1CC2 = 0;
			mWaterGun->unk1CC4 = 0;
		}
	}

	if (gpMSound->gateCheck(MSD_SE_MA_DIVE_IN_MAHRE))
		MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_MA_DIVE_IN_MAHRE,
		                                          nullptr, 0, nullptr, 0, 4);

	bubbleFromBody();
	mForwardVel *= mDivingParams.mSeaBrake.get();
	mSlideVelX = mForwardVel * JMASSin(mFaceAngle.y);
	mSlideVelZ = mForwardVel * JMASCos(mFaceAngle.y);
	mVel.x     = mSlideVelX;
	mVel.y *= mDivingParams.mSeaBrakeY.get();
	mVel.z = mSlideVelZ;
	jumpProcess(0);
	return 0;
}

BOOL TMario::fallDead()
{
	jumpProcess(0);
	return FALSE;
}

BOOL TMario::jumpMain()
{
	int result;

	if (mHeldObject != nullptr)
		if (mInput & 0x2000 ? true : false)
			changePlayerStatus(MARIO_STATUS_JUMP_THROW, 0, 0);

	switch (mStatus) {
	case MARIO_STATUS_FORCE_JUMP:
	case 0x89C:
	case MARIO_STATUS_JUMP:
		result = jumping();
		break;

	case MARIO_STATUS_SECOND_JUMP:
		result = secJumping();
		break;

	case MARIO_STATUS_LANDING:
	case MARIO_STATUS_ROCKET_LANDING:
		result = landing();
		break;

	case MARIO_STATUS_ULTRA_JUMP:
		result = ultraJumping();
		break;

	case MARIO_STATUS_BACK_JUMP:
		result = backJumping();
		break;

	case MARIO_STATUS_U_TURN_JUMP:
		result = uTurnJumping();
		break;

	case MARIO_STATUS_WALL_JUMP:
		result = jumpWall();
		break;

	case MARIO_STATUS_MISS_JUMP:
		result = missJumping();
		break;

	case MARIO_STATUS_FIRE_JUMP:
		result = fireJumping();
		break;

	case MARIO_STATUS_FIRE_LAND:
		result = fireLanding();
		break;

	case MARIO_STATUS_BROAD_JUMP:
		result = broadJumping();
		break;

	case MARIO_STATUS_ROTATE_BROAD_JUMP:
		result = rotateBroadJumping();
		break;

	case 0x281089A:
	case 0x81089B:
		result = boardJumping();
		break;

	case MARIO_STATUS_ROCKET:
		result = rocketing();
		break;

	case MARIO_STATUS_LEFT_ROTATE_JUMP:
	case MARIO_STATUS_RIGHT_ROTATE_JUMP:
		result = rotateJumping();
		break;

	case MARIO_STATUS_WIRE_JUMP:
		result = wireJumping();
		break;

	case MARIO_STATUS_WIRE_ROLL_JUMP:
		result = wireJumping();
		break;

	case MARIO_STATUS_PULL_JUMP:
		result = pullJumping();
		break;

	case MARIO_STATUS_JUMP_CATCH:
		result = jumpCatch();
		break;

	case MARIO_STATUS_JUMP_THROW:
		result = jumpingThrow();
		break;

	case MARIO_STATUS_JUMP_SHORT_BACK_DOWN:
		result = jumpShortBackDown();
		break;

	case MARIO_STATUS_JUMP_SHORT_FORE_DOWN:
		result = jumpShortForeDown();
		break;

	case MARIO_STATUS_JUMP_FORE_DOWN:
		result = jumpForeDown();
		break;

	case MARIO_STATUS_JUMP_BACK_DOWN:
		result = jumpBackDown();
		break;

	case MARIO_STATUS_LAND_SAFE_DOWN:
		result = landSafeDown();
		break;

	case MARIO_STATUS_WIRE_HANG_LAND_SAFE_DOWN:
		result = landSafeDown();
		break;

	case MARIO_STATUS_WALL_SLIDE:
		result = stayWall();
		break;

	case MARIO_STATUS_CATCH_STOP:
		result = catchStop();
		break;

	case MARIO_STATUS_SLIP_FALL:
		result = slipFalling();
		break;

	case MARIO_STATUS_FIRE_DOWN:
		result = fireDowning();
		break;

	case MARIO_STATUS_THROWN_DOWN:
		result = thrownDowning();
		break;

	case MARIO_STATUS_TRAMPLE:
		result = trample();
		break;

	case MARIO_STATUS_HIP_DROP:
		result = hipAttacking();
		break;

	case MARIO_STATUS_DIVE:
		result = diving();
		break;

	case MARIO_STATUS_FALL_DEAD:
		result = fallDead();
		break;
	}

	return result;
}
