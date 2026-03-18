#include <Player/MarioMain.hpp>

#include <Map/Map.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Player/Watergun.hpp>
#include <Player/NozzleBase.hpp>
#include <Player/NozzleTrigger.hpp>
#include <Strategic/LiveActor.hpp>
#include <Camera/CameraShake.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <System/MarioGamePad.hpp>
#include <fake_tgmath.h>

// NOTE: -inline deferred means functions must be in REVERSE address order.

void TMario::startJumpWall()
{
	if (mWallPlane != nullptr) {
		s16 wallAngle = matan(mWallPlane->getNormal().z, mWallPlane->getNormal().x);
		s16 newAngle = wallAngle + 0x8000;
		emitParticle(24, newAngle);
		emitParticle(25, newAngle);
	}
	mVel.y = 0.0f;
	mFaceAngle.y = mFaceAngle.y + 0x8000;
	f32 ceiling = mJumpParams.mJumpJumpCatchSp.value;
	if (mVel.y + ceiling + mPosition.y >= mFloorPosition.y)
		mVel.y = 0.0f;
	changePlayerStatus(0x02000886, 0, false);
}

void TMario::doJumping()
{
	mForwardVel = mForwardVel * mJumpParams.mJumpSpeedBrake.value;
	f32 sideVel = 0.0f;
	if (mInput & 1) {
		s16 intendedYaw = mIntendedYaw;
		s16 faceY = mFaceAngle.y;
		f32 intendedMag = mIntendedMag;
		s16 angleDiff = intendedYaw - faceY;
		if (mAction == 0x088B) {
			u8 hasFludd;
			if (mState & 0x10000) hasFludd = 1; else hasFludd = 0;
			if (hasFludd) {
				TWaterGun* gun = mWaterGun;
				if (gun->mCurrentWater != 0) {
					s32 kind = gun->getCurrentNozzle()->getNozzleKind();
					if (kind == 1) {
						TNozzleTrigger* t = (TNozzleTrigger*)gun->getCurrentNozzle();
						if (t->unk385 == 1) {}
					} else {
						if (gun->getCurrentNozzle()->unk378 > 0.0f) {}
					}
				}
				intendedMag = mDivingParams.mAccelControl.value * intendedMag;
			}
		}
		u32 actionBase = mAction - 0xFE000000;
		if (actionBase <= 2182) {
			if (mVel.y > 0.0f) {
				s16 d = (s16)angleDiff;
				if (d < -16384 || d > 16384) intendedMag = 0.0f;
			}
		}
		f32 accel;
		if (onYoshi()) {
			TYoshi* y = mYoshi;
			u8 sw; if (y->mFlutterState == 1) sw = 1; else sw = 0;
			if (sw) {
				s16 d = (s16)angleDiff;
				if (d > -16384 && d < 16384) accel = mYoshiParams.mHoldOutAccCtrlF.value;
				else accel = mYoshiParams.mHoldOutAccCtrlB.value;
			} else accel = getJumpAccelControl();
		} else accel = getJumpAccelControl();
		u16 au = (u16)angleDiff;
		mForwardVel = accel * intendedMag * JMASSin(au) + mForwardVel;
		sideVel = intendedMag * intendedMag * JMASCos(au);
	}
	if (mForwardVel > 0.0f) mForwardVel -= mJumpParams.mJumpAccelControl.value;
	if (mForwardVel < 0.0f) mForwardVel += mJumpParams.mJumpAccelControl.value;
	u16 fa = mFaceAngle.y;
	mSlideVelX = mForwardVel * JMASSin(fa);
	mSlideVelZ = mForwardVel * JMASCos(fa);
	u16 pa = (u16)(mFaceAngle.y + 16384);
	mSlideVelX += sideVel * JMASSin(pa);
	mSlideVelZ += sideVel * JMASCos(pa);
	mVel.x = mSlideVelX;
	mVel.z = mSlideVelZ;
	if (mVel.y < 0.0f) {
		*(f32*)((u8*)this + 0x50) = mDeParams.mTrampleRadius.value;
		calcEntryRadius();
		*(f32*)((u8*)this + 0x54) = mDeParams.mAttackHeight.value;
		calcEntryRadius();
	} else {
		*(f32*)((u8*)this + 0x50) = mDeParams.mPushupRadius.value;
		calcEntryRadius();
		*(f32*)((u8*)this + 0x54) = mDeParams.mPushupHeight.value;
		calcEntryRadius();
	}
}

void TMario::jumpingBasic(int statusId, int anmId, int groundCheck)
{
	doJumping();
	int result = jumpProcess(groundCheck);
	switch (result) {
	case 0:
		setAnimation(anmId, 1.0f);
		break;
	case 1: {
		if (mGroundPlane->getActor())
			((TLiveActor*)mGroundPlane->getActor())->receiveMessage(this, 0);
		u8 canCatch = 0, shouldCatch = 1;
		u8 hy; if (mSubState & 0x100) hy = 1; else hy = 0;
		if (hy) shouldCatch = 0;
		if (*(f32*)((u8*)this + 0x02AC) - mPosition.y <= mDeParams.mDamageFallHeight.value)
			shouldCatch = 0;
		if (onYoshi()) shouldCatch = 0;
		u16 bg = *(u16*)((u8*)mGroundPlane);
		if (bg == 0x0A || bg == 0x800A || bg == 0x0108) shouldCatch = 0;
		else {
			u8 r; if (bg == 0x07 || bg == 0x8007) r = 1; else r = 0;
			if (r) shouldCatch = 0;
			else {
				if (bg == 0x08 || bg == 0x8008) shouldCatch = 0;
				else { u8 w; if (bg == 0x09 || bg == 0x8009) w = 1; else w = 0; if (w) shouldCatch = 0; }
			}
		}
		if (mVel.y > 0.0f) shouldCatch = 0;
		if (shouldCatch) {
			u8 cf; if (mState & 0x80000) cf = 1; else cf = 0;
			if (cf) {
				u8 fe; if (mState & 0x10000) fe = 1; else fe = 0;
				if (fe) {
					if (*(u8*)((u8*)mWaterGun + 0x1C84) == 2) break;
					jumpProcess(0);
					changePlayerStatus(0x0479, 0, false);
					rumbleStart(21, mMotorParams.mMotorHipDrop.value);
					startVoice(0x789E);
					canCatch = 1;
					if (gpMSound->gateCheck(0x193E))
						MSoundSESystem::MSoundSE::startSoundActor(0x193E, (const Vec*)&mPosition, 0, nullptr, 0, 4);
					strongTouchDownEffect();
					floorDamageExec(1, 3, 0, mMotorParams.mMotorReturn.value);
					break;
				}
			}
		}
		*(f32*)((u8*)this + 0x02AC) = mPosition.y;
		changePlayerStatus(statusId, 0, false);
		if (!canCatch) { rumbleStart(20, mMotorParams.mMotorWall.value / 2); stopVoice(); }
		u32 pa = mPrevAction;
		if (pa == 0x0887 || (pa - 0x0895 <= 1)) strongTouchDownEffect();
		else smallTouchDownEffect();
		break;
	}
	case 2: {
		if (mAction == 0x0893) break;
		if (mForwardVel > mDeParams.mClashSpeed.value) {
			emitParticle(12);
			changePlayerDropping(0x000208B0, 0);
			break;
		}
		setAnimation(anmId, 1.0f);
		if (onYoshi()) { setPlayerVelocity(0.0f); break; }
		if (mWallPlane) {
			u16 wt = *(u16*)((u8*)mWallPlane);
			u8 rp; if (wt == 5 || wt == 0x8005) rp = 1; else rp = 0;
			if (rp) { changePlayerStatus(0x088D, 0, false); setPlayerVelocity(0.0f); break; }
		}
		if (mWallPlane) {
			u16 wt = *(u16*)((u8*)mWallPlane);
			u8 fc; if (wt == 0x010A) fc = 1; else fc = 0;
			if (fc) {
				s16 wa = matan(mWallPlane->getNormal().z, mWallPlane->getNormal().x);
				mFaceAngle.y = wa + 0x8000;
				mModelFaceAngle = mFaceAngle.y;
				if (mAction == 0x0887) mModelFaceAngle = mFaceAngle.y - 0x8000;
				rumbleStart(21, mMotorParams.mMotorWall.value);
				changePlayerStatus(0x3000036C, 0, false);
				break;
			}
		}
		playerRefrection(0);
		if (mWallPlane) {
			changePlayerStatus(0x08A7, 0, false);
			if (isMario()) {
				rumbleStart(21, mMotorParams.mMotorWall.value);
				gpCameraShake->startShake((EnumCamShakeMode)1, 0.0f);
				u32 sid = gpMSound->getWallSound(0, mForwardVel);
				if (gpMSound->gateCheck(sid))
					MSoundSESystem::MSoundSE::startSoundActor(sid, (const Vec*)&mPosition, 0, nullptr, 0, 4);
			}
			break;
		}
		if (mVel.y > 0.0f) mVel.y = 0.0f;
		changePlayerStatus(0x000208B0, 0, false);
		break;
	}
	case 3:
		setAnimation(0x33, 1.0f);
		changePlayerDropping(0x3800034B, 0);
		break;
	case 4:
		rumbleStart(21, mMotorParams.mMotorWall.value);
		changePlayerStatus(0x08200348, 0, false);
		break;
	}
}

void TMario::considerJumpRotate()
{
	int dir;
	if (checkStickRotate(&dir) != 1) return;
	switch (dir) {
	case 2: mAction = 0x0896; break;
	case 3: mAction = 0x0895; break;
	}
}

void TMario::checkBackTrig()
{
	if (!(mInput & 0x10000)) return;
	TMarioGamePad* pad = mGamePad;
	if (pad->mEnabledFrameMeaning & 0x4000) { changePlayerStatus(0x008008A9, 0, false); return; }
	if (!onYoshi()) {
		setPlayerVelocity(mJumpParams.mJumpJumpCatchSp.value);
		changePlayerStatus(0x0080088A, 0, false);
	}
}

void TMario::landing()
{
	if (mVel.y < 0.0f) {
		u16 t = mActionTimer; mActionTimer = t + 1;
		if (t > 240) { mActionTimer = 240; startSoundActor(0x786B); mActionArg = 3; }
	}
	if (mInput & 0x10000) {
		TMarioGamePad* pad = mGamePad;
		if (pad->mEnabledFrameMeaning & 0x4000) changePlayerStatus(0x008008A9, 0, false);
		else if (!onYoshi()) { setPlayerVelocity(mJumpParams.mJumpJumpCatchSp.value); changePlayerStatus(0x0080088A, 0, false); }
	}
	if (rocketCheck()) return;
	s32 anm = 86;
	switch (mActionArg) {
	case 0: anm = 86; break;
	case 1: anm = 144; break;
	case 3: anm = 288; break;
	}
	jumpingBasic(0x04000471, anm, 3);
}

void TMario::jumpCatch()
{
	if (mInput & 0x10000) {
		TMarioGamePad* pad = mGamePad;
		if (pad->mEnabledFrameMeaning & 0x4000) { changePlayerStatus(0x008008A9, 0, false); return; }
	}
	setAnimation(136, 1.0f);
	doJumping();
	int r = jumpProcess(0);
	switch (r) {
	case 1: {
		u8 cc = 1;
		u8 hy; if (mSubState & 0x100) hy = 1; else hy = 0;
		if (hy) cc = 0;
		if (*(f32*)((u8*)this + 0x02AC) - mPosition.y <= mDeParams.mDamageFallHeight.value) cc = 0;
		if (onYoshi()) cc = 0;
		u16 bg = *(u16*)((u8*)mGroundPlane);
		if (bg == 0x0A || bg == 0x800A || bg == 0x0108) cc = 0;
		else {
			u8 t; if (bg == 0x07 || bg == 0x8007) t = 1; else t = 0;
			if (t) cc = 0;
			else { if (bg == 0x08 || bg == 0x8008) cc = 0;
			else { u8 w; if (bg == 0x09 || bg == 0x8009) w = 1; else w = 0; if (w) cc = 0; } }
		}
		if (mVel.y > 0.0f) cc = 0;
		if (cc) {
			u8 cf; if (mState & 0x80000) cf = 1; else cf = 0;
			if (cf) { sinkInSandEffect(); changePlayerStatus(0x0002033C, 1, false); break; }
		}
		changePlayerStatus(0x00800456, 0, false);
		break;
	}
	case 2: case 3: {
		if (mWallPlane) {
			u8 fc; if (*(u16*)((u8*)mWallPlane) == 0x010A) fc = 1; else fc = 0;
			if (fc) { changePlayerDropping(0x3000036C, 0); break; }
		}
		playerRefrection(1);
		if (mVel.y > 0.0f) mVel.y = 0.0f;
		emitParticle(12);
		changePlayerDropping(0x000208B0, 0);
		break;
	}
	}
}

void TMario::jumpDownCommon(int statusId, int anmId, f32 velY)
{
	setPlayerVelocity(velY);
	int r = jumpProcess(0);
	switch (r) {
	case 0: setAnimation(anmId, 1.0f); break;
	case 1: changePlayerStatus(statusId, mActionArg, false); break;
	case 2: case 3:
		setAnimation(0, 1.0f);
		playerRefrection(0);
		if (mVel.y > 0.0f) mVel.y = 0.0f;
		setPlayerVelocity(-velY);
		break;
	}
}

void TMario::stayWall()
{
	u16 t = mActionTimer; mActionTimer = t + 1;
	if (mActionTimer > 60) mActionTimer = 60;
	if (mInput & 2) {
		if (mWallPlane) {
			s16 wa = matan(mWallPlane->getNormal().z, mWallPlane->getNormal().x);
			s16 na = wa + 0x8000;
			emitParticle(24, na); emitParticle(25, na);
		}
		mVel.y = 0.0f;
		mFaceAngle.y = mFaceAngle.y + 0x8000;
		f32 c = mJumpParams.mJumpJumpCatchSp.value;
		if (mVel.y + c + mPosition.y >= mFloorPosition.y) mVel.y = 0.0f;
		changePlayerStatus(0x02000886, 0, false);
		return;
	}
	if (mInput & 0x10000) {
		TMarioGamePad* pad = mGamePad;
		if (pad->mEnabledFrameMeaning & 0x4000) { changePlayerStatus(0x008008A9, 0, false); return; }
		if (!onYoshi()) { setPlayerVelocity(mJumpParams.mJumpJumpCatchSp.value); changePlayerStatus(0x0080088A, 0, false); return; }
	}
	if (mActionTimer < 20) {
		mActionTimer = mActionTimer + 1;
		mVel.x = 0.0f; mVel.y = 0.0f; mVel.z = 0.0f;
	} else {
		mVel.y = -(f32)mActionTimer * mJumpParams.mJumpAccelControl.value;
	}
	if (mWallPlane) {
		mPosition.x -= mWallPlane->mNormal.x;
		mPosition.z -= mWallPlane->mNormal.z;
	}
	int jr = jumpProcess(0);
	if (jr == 1) { mFaceAngle.y += 0x8000; changePlayerStatus(0x088C, 0, false); return; }
	if (!mWallPlane) {
		mFaceAngle.y += 0x8000;
		setPlayerVelocity(mJumpParams.mJumpJumpCatchSp.value);
		mVel.y = 0.0f;
		changePlayerStatus(0x088C, 0, false);
		return;
	}
	setAnimation(204, 1.0f);
	if (mVel.y < 0.0f) {
		wallSlipEffect();
		if (gpMSound->gateCheck(0x113F))
			MSoundSESystem::MSoundSE::startSoundActor(0x113F, (const Vec*)&mPosition, 0, nullptr, 0, 4);
	}
}

void TMario::catchStop()
{
	if (mActionState == 0) { mVel.y = 0.0f; mActionState = 1; }
	doJumping();
	int r = jumpProcess(0);
	switch (r) {
	case 0:
		if (mActionState == 1) setAnimation(111, 1.0f);
		else setAnimation(86, 1.0f);
		break;
	case 1: changePlayerStatus(0x0C000232, 0, false); break;
	case 2: case 3: setPlayerVelocity(0.0f); break;
	}
	if (mActionState == 1 && isLast1AnimeFrame()) mActionState = 2;
}

void TMario::slipFalling()
{
	u16 t = mActionTimer; mActionTimer = t + 1;
	if (mActionTimer > 120 && mPosition.y - mFloorPosition.y > 0.0f) {
		changePlayerStatus(0x088C, 1, false); return;
	}
	mForwardVel *= mJumpParams.mJumpSpeedBrake.value;
	if (mInput & 1) {
		s16 ad = mIntendedYaw - mFaceAngle.y;
		u16 au = (u16)ad;
		f32 im = mIntendedMag;
		f32 cr = *(f32*)((u8*)this + 0x0B8C);
		f32 ac = im * cr;
		mForwardVel += ac * JMASSin(au);
		mFaceAngle.y = (s16)(ac * JMASCos(au) + (f32)mFaceAngle.y);
	}
	if (mForwardVel > 0.0f) mForwardVel -= mJumpParams.mJumpAccelControl.value;
	if (mForwardVel < 0.0f) mForwardVel += mJumpParams.mJumpAccelControl.value;
	u16 fa = mFaceAngle.y;
	f32 vx = mForwardVel * JMASSin(fa); mSlideVelX = vx; mVel.x = vx;
	f32 vz = mForwardVel * JMASCos(fa); mSlideVelZ = vz; mVel.z = vz;
	int jr = jumpProcess(0);
	switch (jr) {
	case 1:
		if (mActionState == 0 && mVel.y < 0.0f && mGroundPlane->getNormal().y >= 0.0f) {
			mVel.y = -mVel.y * *(f32*)((u8*)this + 0x0BA0); mActionState = 1;
		} else changePlayerStatus(0x00840452, 0, false);
		break;
	case 2:
		if (mVel.y > 0.0f) mVel.y = 0.0f;
		rumbleStart(21, mMotorParams.mMotorWall.value);
		changePlayerStatus(0x000208B0, 0, false);
		break;
	}
	setAnimation(145, 1.0f);
}

void TMario::fireDowning()
{
	if (mActionTimer == 1) startVoice(0x7849);
	u16 t2 = mActionTimer; mActionTimer = t2 + 1;
	if (!(mInput & 1)) mForwardVel = FConverge(mForwardVel, 0.0f, 0.0f, mJumpParams.mFireDownControl.value);
	if (mInput & 1) {
		s16 ad = mIntendedYaw - mFaceAngle.y; u16 au = (u16)ad;
		f32 ac = mIntendedMag * *(f32*)((u8*)this + 0x0B8C) * mJumpParams.mFireDownControl.value;
		mForwardVel += ac * JMASSin(au);
		mFaceAngle.y = (s16)(ac * JMASCos(au) + (f32)mFaceAngle.y);
		if (mForwardVel < 0.0f) { mFaceAngle.y += 0x8000; mForwardVel *= mJumpParams.mFireBackVelocity.value; }
		if (mForwardVel > 0.0f) mForwardVel -= mJumpParams.mJumpAccelControl.value;
	}
	u16 fa = mFaceAngle.y;
	f32 vx = mForwardVel * JMASSin(fa); mSlideVelX = vx; mVel.x = vx;
	f32 vz = mForwardVel * JMASCos(fa); mSlideVelZ = vz; mVel.z = vz;
	int jr = jumpProcess(0);
	if (jr == 1) {
		if (mActionState < 2 && mVel.y < 0.0f) {
			mVel.y = -mVel.y * mJumpParams.mBroadJumpForce.value;
			setPlayerVelocity(mJumpParams.mBroadJumpForce.value * mForwardVel);
			mActionState = mActionState + 1;
		} else { startVoice(0x7852); changePlayerStatus(0x08000239, 0, false); }
	} else if (jr == 2) playerRefrection(0);
	setAnimation(41, 1.0f);
}

void TMario::thrownDowning()
{
	s16 ad = mIntendedYaw - mFaceAngle.y; u16 au = (u16)ad;
	f32 ac = mIntendedMag * *(f32*)((u8*)this + 0x0B8C);
	f32 ta = mJumpParams.mThrownAccel.value;
	mForwardVel += ac * JMASSin(au) * ta;
	f32 ts = mJumpParams.mThrownSlide.value;
	mFaceAngle.y = (s16)(ac * JMASCos(au) * ts + (f32)mFaceAngle.y);
	mForwardVel *= mJumpParams.mThrownBrake.value;
	u16 fa = mFaceAngle.y;
	f32 vx = mForwardVel * JMASSin(fa); mSlideVelX = vx; mVel.x = vx;
	f32 vz = mForwardVel * JMASCos(fa); mSlideVelZ = vz; mVel.z = vz;
	int jr = jumpProcess(0);
	if (jr == 1) {
		if (mActionState < 2 && mVel.y < 0.0f) {
			mVel.y = -mVel.y * mJumpParams.mBroadJumpForce.value;
			setPlayerVelocity(mJumpParams.mBroadJumpForce.value * mForwardVel);
			mActionState = mActionState + 1;
		} else changePlayerStatus(0x0C000223, 0, false);
	} else if (jr == 2) playerRefrection(0);
	setAnimation(288, 1.0f);
}

void TMario::boardJumping()
{
	setAnimation(109, 1.0f);
	if (mVel.y < 0.0f) {
		*(f32*)((u8*)this + 0x50) = mDeParams.mTrampleRadius.value; calcEntryRadius();
		*(f32*)((u8*)this + 0x54) = mDeParams.mAttackHeight.value; calcEntryRadius();
	} else {
		*(f32*)((u8*)this + 0x50) = mDeParams.mPushupRadius.value; calcEntryRadius();
		*(f32*)((u8*)this + 0x54) = mDeParams.mPushupHeight.value; calcEntryRadius();
	}
	int r = jumpProcess(0);
	if (r == 1 && mVel.y < 0.0f) changePlayerStatus(0x00810446, 0, false);
	else if (r == 2) {
		if (!mWallPlane) { setPlayerVelocity(0.0f); loserExec(); }
		else {
			s16 wa = matan(mWallPlane->getNormal().z, mWallPlane->getNormal().x);
			s16 d = wa - mFaceAngle.y;
			s16 mx = mSurfingParamsWaterRed.mClashAngle.value;
			if ((s16)d < -mx || (s16)d > mx) {
				if (mForwardVel > mSurfingParamsWaterRed.mClashSpeed.value) startJumpWall();
				else setPlayerVelocity(0.0f);
			} else setPlayerVelocity(0.0f);
		}
	}
}

BOOL TMario::rocketCheck()
{
	u8 cr = 1;
	if (mAction == 0x088B) cr = 0;
	if (mAction == 0x088D) cr = 0;
	u8 hf; if (mState & 0x10000) hf = 1; else hf = 0;
	if (hf) {
		if (*(u8*)((u8*)mWaterGun->getCurrentNozzle() + 0x18) != 1) cr = 0;
		u8 nw; if (mPumpState == 0) nw = 1; else nw = 0;
		if (nw) cr = 0;
		TWaterGun* g = mWaterGun;
		if (g->mCurrentWater == 0) cr = 0;
		else {
			s32 k = g->getCurrentNozzle()->getNozzleKind();
			if (k == 1) { TNozzleTrigger* t = (TNozzleTrigger*)g->getCurrentNozzle(); if (t->unk385 != 1) cr = 0; }
			else { if (g->getCurrentNozzle()->unk378 <= 0.0f) cr = 0; }
		}
	} else cr = 0;
	if ((u8)cr == 1) {
		*(f32*)((u8*)this + 0x0314) = mPosition.y + *(f32*)((u8*)mWaterGun + 0x1D40);
		changePlayerStatus(0x088B, 0, false);
	}
	return 0;
}

void TMario::rocketing()
{
	u8 hf; if (mState & 0x10000) hf = 1; else hf = 0;
	if (!hf) { changePlayerStatus(0x088D, 0, false); return; }
	if (*(u8*)((u8*)mWaterGun->getCurrentNozzle() + 0x18) != 1) { changePlayerStatus(0x088D, 0, false); return; }
	u8 nw; if (mPumpState == 0) nw = 1; else nw = 0;
	if (nw) { changePlayerStatus(0x088D, 0, false); return; }
	if (mInput & 1) {
		u8 rp = *(u8*)((u8*)mWaterGun + 0x1C84);
		if (rp == 4) {
			s16 ad = mIntendedYaw - mFaceAngle.y;
			f32 im = mIntendedMag;
			s16 ade = (s16)ad;
			if (ade > -5461 && ade < 5461) {
			} else if (ade >= -27306 && ade <= 27306) {
				s16 fa2 = *(s16*)((u8*)mWaterGun->getCurrentNozzle() + 0x0310);
				f32 ac = *(f32*)((u8*)this + 0x0B8C) * (-im);
				u16 au = (u16)ad;
				s16 ra = (s16)(ac * (f32)fa2 * JMASSin(au));
				u8 fo; if (mState & 0x10000) fo = 1; else fo = 0;
				if (fo) {
					mWaterGun->unk1CC2 = -ra;
					mWaterGun->unk1CC4 = ra;
					IConverge((int)mFaceAngle.y, (int)mIntendedYaw, (int)mHoverParams.mRotSp.value, (int)mHoverParams.mRotSp.value);
					mFaceAngle.y = mIntendedYaw - ra;
				}
			} else {
				s16 ta;
				if (ade >= -16384 && ade <= 16384) ta = *(s16*)((u8*)mWaterGun->getCurrentNozzle() + 0x0324);
				else ta = *(s16*)((u8*)mWaterGun->getCurrentNozzle() + 0x0338);
				f32 ac = *(f32*)((u8*)this + 0x0B8C) * (-im);
				u16 au = (u16)ad;
				s16 ra = (s16)(ac * (f32)ta * JMASSin(au));
				mWaterGun->unk1CC2 = ra;
				mWaterGun->unk1CC4 = ra;
				mForwardVel += im * JMASCos(au) * mDivingParams.mAccelControl.value;
			}
		}
	} else { mWaterGun->unk1CC2 = 0; mWaterGun->unk1CC4 = 0; }
	u16 fa = mFaceAngle.y;
	mSlideVelX = mForwardVel * JMASSin(fa); mSlideVelZ = mForwardVel * JMASCos(fa);
	mVel.x = mSlideVelX; mVel.z = mSlideVelZ;
	u8 rp2 = *(u8*)((u8*)mWaterGun + 0x1C84);
	if (rp2 == 4) {
		mVel.y = (*(f32*)((u8*)this + 0x0314) - mPosition.y) * mHoverParams.mAccelRate.value;
		mForwardVel *= mHoverParams.mBrake.value;
	}
	int res = jumpProcess(2);
	if (res >= 3 && res < 5) { rumbleStart(21, mMotorParams.mMotorWall.value); changePlayerStatus(0x08200348, 0, false); }
	if (mRoofPlane) {
		f32 c = mJumpParams.mJumpJumpCatchSp.value;
		if (c + mPosition.y > mFloorPosition.y) mPosition.y = mFloorPosition.y - c;
	}
	setAnimation(86, 1.0f);
}

void TMario::hipAttacking()
{
	s32 i = 0; f32 md = 0.0f;
	while (i < *(u16*)((u8*)this + 0x48)) {
		THitActor* a = ((THitActor**)*(u32*)((u8*)this + 0x44))[i];
		u32 at = *(u32*)((u8*)a + 0x4C);
		u8 it; if (at - 0xC0000000 <= 11) it = 1; else it = 0;
		if (it) {
			f32 dx = *(f32*)((u8*)a + 0x10) - mPosition.x;
			f32 dy = *(f32*)((u8*)a + 0x14) - mPosition.y;
			f32 dz = *(f32*)((u8*)a + 0x18) - mPosition.z;
			f32 d = JGeometry::TUtil<f32>::sqrt(dx*dx + dy*dy + dz*dz);
			if (d > md) { mPosition.x = *(f32*)((u8*)a + 0x10); mPosition.z = *(f32*)((u8*)a + 0x18); }
		}
		i++;
	}
	switch (mActionState) {
	case 0: startVoice(0x788F); mActionState = 1;
	case 1: {
		if (mFloorPosition.y > mPosition.y) { mPosition.y = 10.0f + mFloorPosition.y; changePlayerStatus(0x0080023C, 0, false); break; }
		if (mActionTimer < 40) {
			f32 f = (f32)(40 - mActionTimer) * mJumpParams.mHipAttackSpeedY.value;
			if (mJumpParams.mJumpJumpCatchSp.value + mPosition.y + f < mFloorPosition.y) {
				mPosition.y += f * mJumpParams.mHipAttackSpeedY.value;
				*(f32*)((u8*)this + 0x104) = mPosition.y;
			}
		}
		setPlayerVelocity(0.0f);
		*(f32*)((u8*)this + 0x50) = 0.0f; calcEntryRadius();
		setAnimation(60, 1.0f);
		u16 tt = mActionTimer; mActionTimer = tt + 1;
		if (mActionTimer >= 60) { mActionTimer = 0; mActionState = 2; }
		mVel.y = 0.0f;
		int r = jumpProcess(0);
		if (r == 1) { changePlayerStatus(0x0C000230, 0, false); break; }
		if (r == 2) { setPlayerVelocity(0.0f); if (mVel.y > 0.0f) mVel.y = 0.0f; changePlayerStatus(0x000208B0, 0, false); break; }
		break;
	}
	case 2: case 3: {
		setAnimation(61, 1.0f);
		u16 tt = mActionTimer; mActionTimer = tt + 1;
		if ((s16)mActionTimer > mJumpParams.mSuperHipAttackCt.value) mActionState = 3;
		if (mActionState == 2) { mVel.y = mJumpParams.mHipAttackSpeedY.value; emitBlurHipDrop(); }
		else { mVel.y = mJumpParams.mSuperHipAttackSpeedY.value; emitBlurHipDropSuper(); }
		*(f32*)((u8*)this + 0x50) = mDeParams.mHipdropRadius.value; calcEntryRadius();
		*(f32*)((u8*)this + 0x54) = mDeParams.mAttackHeight.value; calcEntryRadius();
		int r = jumpProcess(0);
		if (r == 1) {
			if (isMario()) {
				if (mActionState == 2) { SMSRumbleMgr->start(0, (f32*)nullptr); gpCameraShake->startShake((EnumCamShakeMode)0, 0.0f); }
				else { rumbleStart(21, 30); gpCameraShake->startShake((EnumCamShakeMode)39, 0.0f); }
			}
			if (mGroundPlane->getActor()) {
				if (!onYoshi() && *(u32*)((u8*)mGroundPlane->getActor() + 0x4C) - 0xC0000000 <= 106) {
					emitParticle(57, (const JGeometry::TVec3<f32>*)&mPosition);
					mPosition.y -= 5.0f;
					((TLiveActor*)mGroundPlane->getActor())->receiveMessage(this, 3);
					startVoice(0x78D3); changePlayerStatus(0x00200346, 0, false); break;
				}
				if (mActionState == 2) ((TLiveActor*)mGroundPlane->getActor())->receiveMessage(this, 1);
				else { ((TLiveActor*)mGroundPlane->getActor())->receiveMessage(this, 3); ((TLiveActor*)mGroundPlane->getActor())->receiveMessage(this, 1); }
			}
			if (mActionState == 2) { emitParticle(20); emitParticle(19); emitParticle(18); }
			else { emitParticle(67); emitParticle(68); emitParticle(69); emitParticle(70); }
			changePlayerStatus(0x0080023C, 0, false);
		} else if (r == 2) {
			setPlayerVelocity(0.0f); if (mVel.y > 0.0f) mVel.y = 0.0f;
			changePlayerStatus(0x000208B0, 0, false);
			rumbleStart(21, mMotorParams.mMotorWall.value);
			if (gpMSound->gateCheck(0x180E)) MSoundSESystem::MSoundSE::startSoundActor(0x180E, (const Vec*)&mPosition, 0, nullptr, 0, 4);
		}
		break;
	}
	}
	mModelFaceAngle = mFaceAngle.y;
}

void TMario::diving()
{
	jumpProcess(0);
}

BOOL TMario::jumpMain()
{
	if (*(u32*)((u8*)this + 0x6C) != 0) {
		u8 s; if (mInput & 0x4000) s = 1; else s = 0;
		if (s) changePlayerStatus(0x820008AB, 0, false);
	}
	BOOL result = 0;
	switch (mAction) {
	case 0x000208B4: checkBackTrig(); rocketCheck(); considerJumpRotate(); jumpingBasic(0x000208B4, 86, 0); break;
	case 0x02000882: checkBackTrig(); doJumping(); jumpProcess(0); break;
	case 0x08000883: checkThrowObject(); doJumping(); jumpProcess(0); break;
	case 0x0884: checkBackTrig(); rocketCheck(); considerJumpRotate(); jumpingBasic(0x0884, 86, 0); break;
	case 0x02000885: setPlayerVelocity(0.0f); jumpProcess(0); break;
	case 0x02000886: stayWall(); break;
	case 0x08000887: checkBackTrig(); rocketCheck(); considerJumpRotate(); jumpingBasic(0x08000887, 191, 1); break;
	case 0x0888: jumpingBasic(0x0888, 86, 0); break;
	case 0x02000889: setPlayerVelocity(0.0f); jumpProcess(0); break;
	case 0x088B: rocketing(); break;
	case 0x088C: slipFalling(); break;
	case 0x088D: checkBackTrig(); rocketCheck(); considerJumpRotate(); jumpingBasic(0x088D, 86, 0); break;
	case 0x088E: jumpingBasic(0x088E, 86, 0); break;
	case 0x0891: jumpingBasic(0x0891, 86, 0); break;
	case 0x0892: jumpingBasic(0x0892, 86, 0); break;
	case 0x0893: jumpingBasic(0x0893, 51, 0); break;
	case 0x0894: checkBackTrig(); rocketCheck(); considerJumpRotate(); jumpingBasic(0x0894, 86, 0); break;
	case 0x0895: case 0x0896: jumpingBasic(mAction, 86, 0); break;
	case 0x0897: emitBlurSpinJump(); jumpingBasic(0x0897, 86, 0); break;
	case 0x089C: jumpingBasic(0x089C, 86, 0); break;
	case 0x000208B0: fireDowning(); break;
	case 0x000208B1: thrownDowning(); break;
	case 0x000208B3: boardJumping(); break;
	case 0x000208B6: jumpDownCommon(0, 0, 0.0f); break;
	case 0x000208B8: catchStop(); break;
	case 0x000208BA: jumpDownCommon(0, 0, 0.0f); break;
	case 0x02000880: case 0x02000890: diving(); break;
	case 0x008008A6: jumpCatch(); break;
	case 0x008008A9: hipAttacking(); break;
	case 0x0080088A: checkBackTrig(); rocketCheck(); considerJumpRotate(); jumpingBasic(0x0080088A, 86, 0); break;
	case 0x00810446: boardJumping(); break;
	case 0x0281089A: jumpingBasic(0x0281089A, 86, 0); break;
	case 0x0081089B: jumpingBasic(0x0081089B, 86, 0); break;
	}
	return result;
}
