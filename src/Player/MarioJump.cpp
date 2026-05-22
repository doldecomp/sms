#include <Player/MarioMain.hpp>
#include <JSystem/JMath.hpp>
#include <Strategic/LiveActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Map/MapData.hpp>
#include <Camera/CameraShake.hpp>
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
		emitParticle(0x18, angle);
		emitParticle(0x19, angle);
	}

	mVel.y = 52.0f;
	mFaceAngle.y += 0x8000;
	if (mVel.y + (160.0f + mPosition.y) >= mFloorPosition.x)
		mVel.y = 1.0f;

	return changePlayerStatus(STATUS_WALL_JUMP, 0, 0);
}

void TMario::doJumping()
{
	f32 sideVel = 0.0f;
	mForwardVel *= mJumpParams.mJumpSpeedBrake.get();

	if (mInput & 1) {
		f32 mag       = mIntendedMag;
		s16 angleDiff = mIntendedYaw - mFaceAngle.y;

		if (mStatus == 0x88B && checkFlag(0x8000)) {
			if (mWaterGun->canSpray())
				mag = 2.5f * mIntendedMag;
		}

		if (mStatus == STATUS_WALL_JUMP && mVel.y > 0.0f
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

int TMario::jumpingBasic(int statusOnGround, int animation, int processArg)
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
		if ((unk114 & 0x100 ? true : false) == true)
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
			if (unk118 & 0x40000 ? true : false) {
				sinkInSandEffect();
				return changePlayerStatus(STATUS_FOOT_DOWN, 0, 0);
			}
			if ((unk118 & 0x8000 ? true : false)
			    && (int)mWaterGun->mCurrentNozzle != 2) {
				mTrembleModelEffect->tremble(mJumpParams.mTremblePower.get(),
				                             mJumpParams.mTrembleAccele.get(),
				                             mJumpParams.mTrembleBrake.get(),
				                             mJumpParams.mTrembleTime.get());
				changePlayerStatus(0x479, 0, 0);
				rumbleStart(0x15, mMotorParams.mMotorHipDrop.get());
				didTrample = true;
				startVoice(0x789EU);
				SMSGetMSound()->startSoundActor(0x193E, &mPosition, 0, nullptr,
				                                0, 4);
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

		if (mPrevStatus == 0x887U || mPrevStatus == 0x895U
		    || mPrevStatus == 0x896U)
			strongTouchDownEffect();
		else
			smallTouchDownEffect();
		break;
	}

	case 2:
		if (mStatus != 0x893U && mForwardVel > mDeParams.mClashSpeed.get()) {
			emitParticle(0xC);
			changePlayerDropping(0x208B0, 0U);
			return result;
		}
		setAnimation(animation, 1.0f);
		if (onYoshi()) {
			setPlayerVelocity(0.0f);
			break;
		}
		if (mWallPlane != nullptr) {
			if (mWallPlane->isNoWallJump()) {
				changePlayerStatus(0x88D, 0, 0);
				setPlayerVelocity(0.0f);
				break;
			}
		}
		if (mWallPlane != nullptr) {
			if (mWallPlane->isFence()) {
				const JGeometry::TVec3<f32>& normal = mWallPlane->getNormal();
				mFaceAngle.y    = matan(normal.z, normal.x) + 0x8000;
				mModelFaceAngle = mFaceAngle.y;
				if (mStatus == 0x887)
					mModelFaceAngle -= 0x8000;
				rumbleStart(0x15, mMotorParams.mMotorWall.get());
				return changePlayerStatus(STATUS_FENCE_JUMP_CATCH, 0, 0);
			}
		}
		if (mForwardVel > 16.0f && mStatus != 0x88DU) {
			playerRefrection(0);
			mFaceAngle.y += 0x8000;
			if (mWallPlane != nullptr) {
				changePlayerStatus(0x8A7, 0, 0);
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
				changePlayerStatus(0x208B0, 0, 0);
				break;
			}
			if (mForwardVel > 8.0f)
				setPlayerVelocity(-8.0f);
			return changePlayerStatus(0x208B6, 0, 0);
		}
		setPlayerVelocity(0.0f);
		break;

	case 3:
		setAnimation(ANIM_HANG, 1.0f);
		changePlayerDropping(STATUS_HANGING, 0U);
		break;

	case 4:
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
		changePlayerStatus(STATUS_HANG_ROOF, 0, 0);
		break;
	}
	return result;
}

int TMario::considerJumpRotate()
{
	int spC;

	if (checkStickRotate(&spC) == 1) {
		switch (spC) {
		case 2:
			mStatus = STATUS_RIGHT_ROTATE_JUMP;
			break;
		case 3:
			mStatus = STATUS_LEFT_ROTATE_JUMP;
			break;
		}
		return 1;
	}
	return 0;
}

int TMario::checkBackTrig()
{
	if (mInput & 0x8000) {
		if (mGamePad->mEnabledFrameMeaning & 0x2000) {
			return changePlayerStatus(STATUS_HIP_DROP, 0, 0);
		}
		if (onYoshi() == 0) {
			setPlayerVelocity(mJumpParams.mJumpJumpCatchSp.get());
			return changePlayerStatus(0x80088A, 0, 0);
		}
	}
	return 0;
}

int TMario::jumpingCommonEvents()
{
	if (checkBackTrig())
		return 1;
	if (rocketCheck())
		return 1;
	if (considerJumpRotate())
		return 1;

	return 0;
}

int TMario::jumping()
{
	if (jumpingCommonEvents())
		return 1;

	switch (mStatus) {
	case 0x89C:
		jumpingBasic(STATUS_THROWN_DOWN, 0x120, 0);
		break;
	case 0x884: {
		int anim;
		if (mVel.y >= 0.0f)
			anim = 0x50;
		else
			anim = 0x4C;
		jumpingBasic(0x4000470, anim, 3);
		break;
	}
	default:
		jumpingBasic(0x4000470, 0x4D, 3);
		break;
	}
	return 0;
}

int TMario::secJumping()
{
	if (mStatusTimer == 0) {
		mStatusTimer += 1;
		rumbleStart(0x14, mMotorParams.mMotorWall.get());
	}

	int anim;
	if (mVel.y >= 0.0f)
		anim = 0x50;
	else
		anim = 0x4C;

	if (jumpingCommonEvents())
		return 1;

	jumpingBasic(0x4000472, anim, 3);
	return 0;
}

int TMario::ultraJumping()
{
	if (mStatusTimer == 0) {
		mStatusTimer += 1;
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}

	if (checkBackTrig())
		return 1;

	if (rocketCheck())
		return 1;

	jumpingBasic(0x4000478, 0x6F, 0);
	return 0;
}

int TMario::backJumping()
{
	if (checkBackTrig())
		return 1;

	if (rocketCheck())
		return 1;

	if (mStatusState == 0 && isLast1AnimeFrame())
		mStatusState = 1;

	if (mStatusState == 0)
		jumpingBasic(0x4000472, 0xF7, 0);
	else
		jumpingBasic(0x4000472, 0x56, 0);

	return 0;
}

int TMario::landing()
{
	if (mVel.y < 0.0f) {
		if (mStatusTimer++ > 240) {
			mStatusTimer = 240;
			startSoundActor(0x786B);
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
		r31 = 0x56;
		break;
	case 1:
		r31 = 0x90;
		break;
	case 3:
		r31 = 0x120;
		break;
	}

	jumpingBasic(0x4000471, r31, 3);
	return 0;
}

int TMario::uTurnJumping()
{
	if (jumpingCommonEvents())
		return 1;

	if (checkBackTrig())
		return 1;

	if (jumpingBasic(0x4000473, 0xBF, 1) != 3) {
		mFaceAngle.x = 0;
		mModelFaceAngle += 0x8000;
	}
	return 0;
}

int TMario::jumpWall()
{
	if (checkBackTrig())
		return 1;
	if (considerJumpRotate())
		return 1;
	if (rocketCheck())
		return 1;

	jumpingBasic(0x4000470, 0xCB, 3);
	return 0;
}

int TMario::jumpCatch()
{
	if ((mInput & 0x8000) != 0
	    && ((mGamePad->mEnabledFrameMeaning & 0x2000) != 0)) {
		return changePlayerStatus(STATUS_HIP_DROP, 0, false);
	}
	setAnimation(ANIM_SLDCT, 1.0);
	doJumping();

	switch (jumpProcess(0)) {
	case 0:
		break;

	case 1: {
		bool isStrong = true;
		if ((unk114 & 0x100 ? true : false) == true)
			isStrong = false;

		if (unk2A8.y - mPosition.y <= mDeParams.mDamageFallHeight.get())
			isStrong = false;

		if (onYoshi())
			isStrong = false;

		if (mGroundPlane->isThing4())
			isStrong = false;

		if (mVel.y > -70.0f)
			isStrong = false;

		if (isStrong && (unk118 & 0x40000 ? true : false)) {
			sinkInSandEffect();
			changePlayerStatus(STATUS_FOOT_DOWN, 1, false);
		} else {
			changePlayerStatus(0x800456, 0, false);
		}
		break;
	}

	case 2:
		if (mWallPlane && mWallPlane->isFence())
			return changePlayerDropping(STATUS_FENCE_JUMP_CATCH, 0);
		playerRefrection(1);
		if (mVel.y > 0.0f) {
			mVel.y = 0.0f;
		}
		emitParticle(0xC);
		changePlayerDropping(STATUS_JUMP_SHORT_BACK_DOWN, 0);
		break;
	}

	return 0;
}

int TMario::jumpingThrow()
{
	setAnimation(ANIM_THROW, 1.0f);
	checkThrowObject();
	doJumping();

	switch (jumpProcess(0)) {
	case 1:
		mStatus = STATUS_JUMP_THROW_END;
		break;

	case 2:
		setPlayerVelocity(0.0f);
		break;
	}

	return 0;
}

int TMario::jumpDownCommon(int param_1, int animation, float velocity)
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

int TMario::jumpShortBackDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer += 1;
		rumbleStart(0x15, 0x14);
	}
	jumpDownCommon(STATUS_SHORT_BACK_DOWN, 2, -16.0f);
	return 0;
}

int TMario::jumpShortForeDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer += 1;
		rumbleStart(0x15, 0x14);
	}
	jumpDownCommon(STATUS_SHORT_FORE_DOWN, 0x2D, 16.0f);
	return 0;
}

int TMario::jumpBackDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer += 1;
		rumbleStart(0x15, 0x14);
	}
	jumpDownCommon(STATUS_BACK_DOWN, 2, -16.0f);
	return 0;
}

int TMario::jumpForeDown()
{
	if (mStatusTimer == 0) {
		mStatusTimer += 1;
		rumbleStart(0x15, 0x14);
	}
	jumpDownCommon(STATUS_FORE_DOWN, 0x2D, 16.0f);
	return 0;
}

void TMario::landSafeDown() { }

int TMario::stayWall()
{
	mStatusTimer += 1;
	if (mStatusTimer > 60)
		mStatusTimer = 60;

	if (mInput & 2) {
		if (mWallPlane) {
			s16 sVar5
			    = matan(mWallPlane->getNormal().z, mWallPlane->getNormal().x)
			      + 0x8000;
			emitParticle(0x18, sVar5);
			emitParticle(0x19, sVar5);
		}

		mVel.y = 52.0f;
		mFaceAngle.y += 0x8000;
		if (mVel.y + (mPosition.y + 160.0f) >= mFloorPosition.x)
			mVel.y = 1.0f;
		return changePlayerStatus(STATUS_WALL_JUMP, 0, false);
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
		return changePlayerStatus(0x88C, 0, false);
	}

	if (mWallPlane == nullptr) {
		mFaceAngle.y += 0x8000;
		setPlayerVelocity(0.0f);
		mVel.y = 0.0f;
		return changePlayerStatus(0x88C, 0, false);
	}

	setAnimation(ANIM_WSLD, 1.0f);
	if (mVel.y < -10.0f) {
		wallSlipEffect();
		SMSGetMSound()->startSoundActor(0x113F, &mPosition, 0, nullptr, 0, 4);
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
		if (mStatusState == 1) {
			setAnimation(ANIM_ROLL, 1.0f);
		} else {
			setAnimation(ANIM_LAND, 1.0f);
		}
		break;
	case 1:
		changePlayerStatus(0xC000232, 0, 0);
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

int TMario::slipFalling()
{
	mStatusTimer += 1;
	if (mStatusTimer > 120 && mPosition.y - mFloorPosition.y > 500.0f)
		return changePlayerStatus(0x88C, 1, false);

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
			changePlayerStatus(0x840452, 0, false);
		}
		break;

	case 2:
		if (mVel.y > 0.0f)
			mVel.y = 0.0f;
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
		changePlayerStatus(0x208B0, 0, false);
		break;
	}
	setAnimation(ANIM_SLIP, 1.0f);
	return false;
}

int TMario::fireDowning()
{
	if (mStatusTimer == 1)
		startVoice(0x7849);
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
			startVoice(0x7852);
			changePlayerStatus(STATUS_FIRE_JUMP_END, 0, 0);
		}
		break;

	case 2:
		playerRefrection(0);
		break;
	}
	setAnimation(ANIM_FIREJMP, 1.0f);
	return 0;
}

int TMario::thrownDowning()
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
			return changePlayerStatus(0xC000223, 0, 0);
		}
		break;

	case 2:
		playerRefrection(0);
		break;
	}
	setAnimation(ANIM_THROWN, 1.0f);
	return 0;
}

void TMario::trample() { }

void TMario::missJumping() { }

void TMario::fireJumping() { }

void TMario::fireLanding() { }

int TMario::broadJumping()
{
	jumpingBasic(STATUS_RUN, 0xF6, 1);
	unk118 |= 0x4000;
	return 0;
}

int TMario::rotateBroadJumping()
{
	jumpDownCommon(0x4000471, 0x56, mForwardVel);
	return 0;
}

int TMario::boardJumping()
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
			changePlayerStatus(STATUS_SURF, 0, 0);
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

int TMario::rocketCheck()
{
	bool bVar2 = true;
	if (mStatus == 0x88B)
		bVar2 = false;
	if (mStatus == 0x88D)
		bVar2 = false;

	if (checkFlag(0x8000)) {
		const TWaterGun* gun = mWaterGun;
		if (gun->getCurrentNozzle()->mEmitParams.mRocketType.get() != 1)
			bVar2 = false;

		if (!(unk380 == 0 ? true : false))
			bVar2 = false;

		if (!mWaterGun->canSpray())
			bVar2 = false;
	} else {
		bVar2 = false;
	}

	if (bVar2 == true) {
		unk314 = mPosition.y + mWaterGun->mWatergunParams.mHoverHeight.get();
		return changePlayerStatus(0x88B, 0, false);
	}

	return 0;
}

int TMario::rocketing()
{
	if (!checkFlag(0x8000))
		return changePlayerStatus(0x88D, 0, 0);

	{
		const TWaterGun* gun = mWaterGun;
		if (gun->getCurrentNozzle()->mEmitParams.mRocketType.get() != 1)
			return changePlayerStatus(0x88D, 0, 0);
	}

	if (!(unk380 == 0 ? true : false) || !mWaterGun->canSpray())
		return changePlayerStatus(0x88D, 0, 0);

	if (mInput & 1) {
		switch (mWaterGun->mCurrentNozzle) {
		case TWaterGun::Hover:
			f32 mag       = mIntendedMag;
			s16 angleDiff = mIntendedYaw - mFaceAngle.y;

			if ((angleDiff > -0x1555 && angleDiff < 0x1555)
			    || angleDiff < -0x6AAA || angleDiff > 0x6AAA) {
				s16 nozzleAngle;
				const TWaterGun* gun = mWaterGun;
				if (angleDiff >= -0x4000 && angleDiff <= 0x4000) {
					nozzleAngle
					    = (s16)(0.03125f * -mag
					            * (f32)gun->getCurrentNozzle()
					                  ->mEmitParams.mSideAngleMaxFront.get()
					            * JMASCos(angleDiff));
				} else {
					nozzleAngle
					    = (s16)(0.03125f * -mag
					            * (f32)gun->getCurrentNozzle()
					                  ->mEmitParams.mSideAngleMaxBack.get()
					            * JMASCos(angleDiff));
				}
				mWaterGun->unk1CC2 = nozzleAngle;
				mWaterGun->unk1CC4 = nozzleAngle;
				mForwardVel += mag * JMASCos(angleDiff)
				               * mDivingParams.mAccelControl.get();
			} else {
				const TWaterGun* gun = mWaterGun;
				s16 nozzleAngle
				    = (s16)(0.03125f * -mag
				            * (f32)gun->getCurrentNozzle()
				                  ->mEmitParams.mSideAngleMaxSide.get()
				            * JMASSin(angleDiff));
				if (checkFlag(0x8000)) {
					mWaterGun->unk1CC2 = -nozzleAngle;
					mWaterGun->unk1CC4 = nozzleAngle;
					mFaceAngle.y
					    = mIntendedYaw
					      - IConverge((s16)(mIntendedYaw - mFaceAngle.y), 0,
					                  mHoverParams.mRotSp.get(),
					                  mHoverParams.mRotSp.get());
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
	case 4:
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
		changePlayerStatus(STATUS_HANG_ROOF, 0, 0);
		break;
	}

	if (mRoofPlane != nullptr)
		if (160.0f + mPosition.y > mFloorPosition.x)
			mPosition.y = mFloorPosition.x - 160.0f;

	setAnimation(ANIM_LAND, 1.0f);
	return 0;
}

int TMario::rotateJumping()
{
	if (checkBackTrig())
		return 1;

	if (rocketCheck())
		return 1;

	setAnimation(ANIM_SPIN_P, 1.0f);
	emitBlurSpinJump();
	jumpingBasic(0x4000472, mAnimationId, 0);
	mStatusTimer += 1;

	if (mStatus == STATUS_RIGHT_ROTATE_JUMP)
		mModelFaceAngle = mStatusTimer * 4096;
	else
		mModelFaceAngle = -(mStatusTimer * 4096);

	if (!(gpMarDirector->unk58 & 0x3F))
		rumbleStart(0x14, mMotorParams.mMotorWall.get() / 2);

	return 0;
}

void TMario::wireJumping() { }

void TMario::pullJumping() { }

int TMario::hipAttacking()
{
	for (int i = 0; i < mColCount; i++) {
		THitActor* col = mCollisions[i];
		if (col->mActorType == 0x4000000B ? true : false) {
			if (col->mPosition.distance(mPosition) > 70.0f) {
				mPosition.x = mCollisions[i]->mPosition.x;
				mPosition.z = mCollisions[i]->mPosition.z;
			}
		}
	}

	switch (mStatusState) {
	case 0:
		startVoice(0x788F);
		mStatusState = 1;
		// fallthrough

	case 1:
		if (mFloorPosition.y > mPosition.y) {
			mPosition.y = 1.0f + mFloorPosition.y;
			changePlayerStatus(STATUS_HIP_ATTACK_END, 0, 0);
		}
		if (mStatusTimer < 0x28) {
			f32 lift = (f32)(0x28 - mStatusTimer) * 0.5f;
			if (160.0f + (mPosition.y + lift) < mFloorPosition.x) {
				mPosition.y                = lift * 0.25f + mPosition.y;
				*(f32*)((u8*)this + 0x104) = mPosition.y;
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
				return changePlayerStatus(0xC000230, 0, 0);

			if (r == 2) {
				setPlayerVelocity(-16.0f);
				if (mVel.y > 0.0f)
					mVel.y = 0.0f;
				return changePlayerStatus(0x208B0, 0, 0);
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
					emitParticle(0x39, &mPosition);
					f32 oldY    = mPosition.y;
					mPosition.y = oldY - 160.0f;
					((THitActor*)mGroundPlane->mActor)->receiveMessage(this, 3);
					startVoice(0x78D3);
					return changePlayerStatus(STATUS_KICK_ROOF_ROLL_DOWN, 0, 0);
				}
				if (mStatusState == 2) {
					((THitActor*)mGroundPlane->mActor)->receiveMessage(this, 1);
				} else {
					((THitActor*)mGroundPlane->mActor)->receiveMessage(this, 3);
					((THitActor*)mGroundPlane->mActor)->receiveMessage(this, 1);
				}
			}

			if (mStatusState == 2) {
				emitParticle(0x14);
				emitParticle(0x13);
				emitParticle(0x12);
			} else {
				emitParticle(0x43);
				emitParticle(0x44);
				emitParticle(0x45);
				emitParticle(0x46);
			}
			changePlayerStatus(STATUS_HIP_ATTACK_END, 0, 0);
		} else if (r == 2) {
			setPlayerVelocity(-16.0f);
			if (mVel.y > 0.0f)
				mVel.y = 0.0f;
			changePlayerStatus(0x208B0, 0, 0);
			rumbleStart(0x15, mMotorParams.mMotorWall.get());
			if (gpMSound->gateCheck(0x180E))
				MSoundSESystem::MSoundSE::startSoundActor(0x180E, &mPosition, 0,
				                                          nullptr, 0, 4);
		}
		break;
	}

	mModelFaceAngle = mFaceAngle.y;
	return 0;
}

int TMario::diving()
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
			const TWaterGun* gun = mWaterGun;
			s16 nozzleAngle      = (s16)(0.03125f * -mag
                                    * (f32)gun->getCurrentNozzle()
                                          ->mEmitParams.mSideAngleMaxSide.get()
                                    * JMASCos(angleDiff));
			if (checkFlag(0x8000)) {
				mWaterGun->unk1CC2 = nozzleAngle;
				mWaterGun->unk1CC4 = nozzleAngle;
				mForwardVel += mag * JMASCos(angleDiff)
				               * mDivingParams.mAccelControl.get();
			}
		} else {
			const TWaterGun* gun = mWaterGun;
			s16 nozzleAngle      = (s16)(0.03125f * -mag
                                    * (f32)gun->getCurrentNozzle()
                                          ->mEmitParams.mSideAngleMaxSide.get()
                                    * JMASSin(angleDiff));
			if (checkFlag(0x8000)) {
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
		if (checkFlag(0x8000)) {
			mWaterGun->unk1CC2 = 0;
			mWaterGun->unk1CC4 = 0;
		}
	}

	if (gpMSound->gateCheck(0x101F))
		MSoundSESystem::MSoundSE::startSoundActor(0x101F, nullptr, 0, nullptr,
		                                          0, 4);

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

void TMario::fallDead() { }

// TODO: figure out which of the real inlines these fake ones correspond to!

static int unknown_inline_1(TMario* mario)
{
	if (mario->jumpingCommonEvents())
		return 1;

	mario->jumpingBasic(0x4000472, mario->mAnimationId, 3);
	return 0;
}

static int unknown_inline_9(TMario* mario)
{
	if (mario->mInput & 0x8000)
		return mario->changePlayerStatus(0x80088A, 0, 0);

	mario->setPlayerVelocity(0.98f * mario->mForwardVel);
	switch (mario->jumpProcess(0)) {
	case 1: {
		u32 nextStatus;
		if (mario->mForwardVel < 0.0f)
			nextStatus = 0x50;
		else
			nextStatus = 0x4000470;
		mario->changePlayerStatus(nextStatus, 0, 0);
		break;
	}
	case 2:
		mario->setPlayerVelocity(0.0f);
		break;
	}

	mario->setAnimation(TMario::ANIM_JUMP, 1.0f);
	return 0;
}

static int unknown_inline_10(TMario* mario)
{
	if (mario->mStatusTimer == 1)
		mario->startVoice(0x7849);

	mario->mStatusTimer += 1;
	mario->setPlayerVelocity(mario->mForwardVel);

	if (mario->jumpProcess(0) == 1)
		mario->changePlayerStatus(TMario::STATUS_FIRE_DASH, 0, 0);

	int anim;
	if (mario->mStatusArg == 0)
		anim = 0x4D;
	else
		anim = 0x29;

	mario->setAnimation(anim, 1.0f);
	return 0;
}

static int unknown_inline_12(TMario* mario)
{
	if (mario->mPrevStatus == TMario::STATUS_WIRE_ROLLING) {
		if (mario->mStatusArg == 0U)
			mario->setAnimation(TMario::ANIM_GIANT_ROLLING, 1.0f);
		else
			mario->setAnimation(TMario::ANIM_ROLL, 1.0f);
	} else {
		mario->setAnimation(TMario::ANIM_JUMP, 1.0f);
	}

	if (mario->jumpingCommonEvents())
		return 1;

	mario->jumpingBasic(0x4000472, mario->mAnimationId, 3);
	return 0;
}

int TMario::jumpMain()
{
	int result;

	if (mHeldObject != nullptr) {
		if (mInput & 0x2000 ? true : false)
			changePlayerStatus(0x820008AB, 0, 0);
	}

	switch (mStatus) {
	case 0x884:
	case 0x89C:
	case STATUS_JUMP:
		result = jumping();
		break;
	case STATUS_SECOND_JUMP:
		result = secJumping();
		break;
	case 0x88C:
	case 0x88D:
		result = landing();
		break;
	case STATUS_ULTRA_JUMP:
		result = ultraJumping();
		break;
	case STATUS_BACK_JUMP:
		result = backJumping();
		break;
	case STATUS_U_TURN_JUMP:
		result = uTurnJumping();
		break;
	case STATUS_WALL_JUMP:
		result = jumpWall();
		break;
	case 0x2000885:
		result = unknown_inline_9(this);
		break;
	case 0x208B4:
		result = unknown_inline_10(this);
		break;

	case 0x208B5:
		setPlayerVelocity(mForwardVel);
		if (jumpProcess(0) == 1)
			changePlayerStatus(STATUS_FIRE_DASH, 0, 0);
		setAnimation(ANIM_LAND, 1.0f);
		result = 0;
		break;
	case STATUS_BROAD_JUMP:
		result = broadJumping();
		break;
	case 0x2000889:
		jumpingBasic(STATUS_RUN, 0x10F, 1);
		result = 0;
		break;
	case 0x281089A:
	case 0x81089B:
		result = boardJumping();
		break;
	case 0x88B:
		result = rocketing();
		break;
	case STATUS_LEFT_ROTATE_JUMP:
	case STATUS_RIGHT_ROTATE_JUMP:
		result = rotateJumping();
		break;
	case 0x892:
		result = unknown_inline_12(this);
		break;
	case 0x893:
		result = unknown_inline_12(this);
		break;

	case 0x894: {
		if (mAnimationId == ANIM_HOLD_TO_HANG && isLast1AnimeFrame())
			setAnimation(ANIM_HOLD_HANG, 1.0f);
		mForwardVel  = 0.0f;
		mIntendedMag = 0.0f;
		jumpingBasic(0x560, mAnimationId, 3);
		if (mStatus == 0x560U)
			setAnimation(ANIM_HANG_TO_HOLD, 1.0f);
		JGeometry::TVec3<f32> pos = mPosition;
		if (mHeldObject->moveRequest(pos) == 1)
			mPosition = pos;
		result = 0;
		break;
	}

	case STATUS_DIVE_JUMP:
		result = jumpCatch();
		break;

	case STATUS_JUMP_THROW:
		result = jumpingThrow();
		break;

	case STATUS_JUMP_SHORT_BACK_DOWN:
		result = jumpShortBackDown();
		break;

	case STATUS_JUMP_SHORT_FORE_DOWN:
		result = jumpShortForeDown();
		break;

	case STATUS_JUMP_FORE_DOWN:
		result = jumpForeDown();
		break;

	case STATUS_JUMP_BACK_DOWN:
		result = jumpBackDown();
		break;

	case 0x208B6:
		result = rotateBroadJumping();
		break;

	case 0x208BA:
		result = rotateBroadJumping();
		break;

	case STATUS_WALL_SLIDE:
		result = stayWall();
		break;

	case 0x8A6:
		result = catchStop();
		break;

	case STATUS_SLIP_FALL:
		result = slipFalling();
		break;

	case STATUS_FIRE_DOWN:
		result = fireDowning();
		break;

	case STATUS_THROWN_DOWN:
		result = thrownDowning();
		break;

	case 0x2000890:
		result = unknown_inline_1(this);
		break;

	case STATUS_HIP_DROP:
		result = hipAttacking();
		break;

	case STATUS_DIVE:
		result = diving();
		break;

	case 0x208B9:
		jumpProcess(0);
		result = 0;
		break;
	}

	return result;
}
