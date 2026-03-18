#include <Player/MarioMain.hpp>

#include <M3DUtil/M3UJoint.hpp>
#include <Map/Map.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <MSound/MSound.hpp>
#include <Map/PollutionManager.hpp>
#include <Player/Watergun.hpp>
#include <Player/NozzleBase.hpp>
#include <Player/NozzleTrigger.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <MSound/MSoundSE.hpp>
#include <fake_tgmath.h>

// NOTE: -inline deferred means functions must be in REVERSE address order.

// considerRotateStart - 0x8013C118
BOOL TMario::considerRotateStart()
{
	int stickDir;
	if (checkStickRotate(&stickDir) != 1)
		return 0;

	TWaterGun* gun = mWaterGun;
	if (gun == nullptr)
		return 0;

	u8 canSpray;
	if (gun->mCurrentWater == 0) {
		canSpray = 0;
	} else {
		s32 kind = gun->getCurrentNozzle()->getNozzleKind();
		if (kind == 1) {
			TNozzleTrigger* trigger = (TNozzleTrigger*)gun->getCurrentNozzle();
			if (trigger->unk385 == 1) {
				canSpray = 1;
			} else {
				canSpray = 0;
			}
		} else {
			if (gun->getCurrentNozzle()->unk378 > 0.0f) {
				canSpray = 1;
			} else {
				canSpray = 0;
			}
		}
	}

	if (canSpray) {
		if (stickDir > 0) {
			changePlayerStatus(0x0441, 0, false);
		} else {
			changePlayerStatus(0x0442, 0, false);
		}
	} else {
		return 0;
	}
}

// doRunningAnimation - 0x8013BFA4
void TMario::doRunningAnimation()
{
	f32 speed = mIntendedMag;
	f32 fwdVel = mForwardVel;
	if (speed <= fwdVel)
		speed = fwdVel;

	f32 clampedSpeed = speed;
	if (speed < 0.0f)
		clampedSpeed = 0.0f;

	f32 anmSpd1 = 1.0f;
	f32 softWalk = 0.5f;
	f32 walk2Soft = 0.0f;

	s32 redo = 1;
	while (redo) {
		u16 anmId = mAnimationId;
		if (anmId == 0x92) {
			// softStep animation
			if (clampedSpeed > mRunParams.mWalk2Soft.get()) {
				setAnimation(0x72, anmSpd1);
				redo = 1;
				continue;
			}
			f32 mult = clampedSpeed;
			if (mult < softWalk)
				mult = softWalk;
			setAnimation(0x92, mult * mRunParams.mSoftStepAnmMult.get());
			redo = 0;
			continue;
		} else if (anmId == 0xF5) {
			// pumping slide animation
			if (mForwardVel < mRunParams.mPumpingSlideSp.get() - anmSpd1) {
				setAnimation(0x72, anmSpd1);
				redo = 1;
				continue;
			}
			f32 anmSpeed = mRunParams.mRunAnmSpeedMult.get() * clampedSpeed + mRunParams.mRunAnmSpeedBase.get();
			setAnimation(0xF5, anmSpeed);
			redo = 0;
			continue;
		} else {
			// default (0x72 run animation)
			if (mForwardVel >= mRunParams.mPumpingSlideSp.get() - anmSpd1) {
				setAnimation(0xF5, anmSpd1);
				redo = 1;
				continue;
			}

			if (clampedSpeed < mRunParams.mSoft2Walk.get()) {
				setAnimation(0x92, anmSpd1);
				redo = 1;
				continue;
			}

			u8 isShallow;
			if (unk118 & 0x30000)
				isShallow = 1;
			else
				isShallow = 0;

			f32 anmSpeed = clampedSpeed * mRunParams.mRunAnmSpeedMult.get() + mRunParams.mRunAnmSpeedBase.get();

			if (isShallow) {
				u8 isSwimDepth;
				if (mFloorPosition.y < mPosition.y + mRunParams.mSwimDepth.get())
					isSwimDepth = 1;
				else
					isSwimDepth = 0;
				if (isSwimDepth) {
					f32 ratio = (mFloorPosition.y - mPosition.y) / mRunParams.mSwimDepth.get();
					f32 factor = anmSpd1 - mRunParams.mInWaterAnmBrake.get();
					anmSpeed = anmSpeed * (anmSpd1 - ratio * factor) * mRunParams.mInWaterBrake.get();
				}
			}

			setAnimation(0x72, anmSpeed);

			f32 walkSp = mRunParams.mMotBlendWalkSp.get();
			f32 runSp = mRunParams.mMotBlendRunSp.get();
			f32 blend = 0.0f;
			if (anmSpeed < walkSp)
				blend = 1.0f;
			if (runSp < anmSpeed)
				blend = 1.0f;
			if (walkSp <= anmSpeed && anmSpeed <= runSp) {
				blend = (anmSpeed - walkSp) / (runSp - walkSp);
			}

			*(f32*)((u8*)this + 0x41C) = anmSpd1 - blend;

			if (isRunningInWater()) {
				mModel->unk20->unk18->unk50 = walk2Soft;
			} else {
				mModel->unk20->unk18->unk50 = *(f32*)((u8*)this + 0x41C);
			}

			redo = 0;
			continue;
		}
	}
}

// isRunningInWater - 0x8013C0D0
BOOL TMario::isRunningInWater()
{
	u8 flag;
	if (unk118 & 0x30000) {
		flag = 1;
	} else {
		flag = 0;
	}
	if (flag) {
		if (mFloorPosition.z < mPosition.y + mRunParams.mSwimDepth.get()) {
			return 1;
		}
	}
	return 0;
}

void TMario::getSlopeNormalAccele(f32* accelUp, f32* accelDown)
{
	if (isForceSlip()) {
		*accelUp = mSlipParamsAll.mSlopeAcceleUp.value;
		*accelDown = mSlipParamsAll.mSlopeAcceleDown.value;
		return;
	}

	const TBGCheckData* ground = mGroundPlane;
	u16 type = ground->mBGType;

	u8 isOil;
	if (type == 0x0c || type == 0x800c || type == 0xa00c)
		isOil = 1;
	else
		isOil = 0;
	if (isOil) {
		*accelUp = mSlipParamsAllSlider.mSlopeAcceleUp.value;
		*accelDown = mSlipParamsAllSlider.mSlopeAcceleDown.value;
		return;
	}

	u8 isAll;
	if (type == 0x02 || type == 0x8002)
		isAll = 1;
	else
		isAll = 0;
	if (isAll) {
		*accelUp = mSlipParams45.mSlopeAcceleUp.value;
		*accelDown = mSlipParams45.mSlopeAcceleDown.value;
		return;
	}

	u8 isWater;
	if (type == 0x04 || type == 0x4004 || type == 0x8004 || type == 0xc004)
		isWater = 1;
	else
		isWater = 0;
	if (isWater) {
		if (ground->mNormal.y > 0.99f) {
			*accelUp = mSlipParamsWaterGround.mSlopeAcceleUp.value;
			*accelDown = mSlipParamsWaterGround.mSlopeAcceleDown.value;
		} else {
			*accelUp = mSlipParamsWaterSlope.mSlopeAcceleUp.value;
			*accelDown = mSlipParamsWaterSlope.mSlopeAcceleDown.value;
		}
		return;
	}

	*accelUp = mSlipParamsNormal.mSlopeAcceleUp.value;
	*accelDown = mSlipParamsNormal.mSlopeAcceleDown.value;
}

void TMario::getSlopeSlideAccele(f32* accelUp, f32* accelDown)
{
	if (isForceSlip()) {
		*accelUp = mSlipParamsAll.mSlideAcceleUp.value;
		*accelDown = mSlipParamsAll.mSlideAcceleDown.value;
		return;
	}

	const TBGCheckData* ground = mGroundPlane;
	u16 type = ground->mBGType;

	u8 isOil;
	if (type == 0x0c || type == 0x800c || type == 0xa00c)
		isOil = 1;
	else
		isOil = 0;
	if (isOil) {
		*accelUp = mSlipParamsAllSlider.mSlideAcceleUp.value;
		*accelDown = mSlipParamsAllSlider.mSlideAcceleDown.value;
		return;
	}

	u8 isAll;
	if (type == 0x02 || type == 0x8002)
		isAll = 1;
	else
		isAll = 0;
	if (isAll) {
		*accelUp = mSlipParams45.mSlideAcceleUp.value;
		*accelDown = mSlipParams45.mSlideAcceleDown.value;
		return;
	}

	u8 isWater;
	if (type == 0x04 || type == 0x4004 || type == 0x8004 || type == 0xc004)
		isWater = 1;
	else
		isWater = 0;
	if (isWater) {
		if (ground->mNormal.y > 0.99f) {
			*accelUp = mSlipParamsWaterGround.mSlideAcceleUp.value;
			*accelDown = mSlipParamsWaterGround.mSlideAcceleDown.value;
		} else {
			*accelUp = mSlipParamsWaterSlope.mSlideAcceleUp.value;
			*accelDown = mSlipParamsWaterSlope.mSlideAcceleDown.value;
		}
		return;
	}

	*accelUp = mSlipParamsNormal.mSlideAcceleUp.value;
	*accelDown = mSlipParamsNormal.mSlideAcceleDown.value;
}

// getChangeAngleSpeed - returns angle change speed (s16 param -> f32 -> * forwardVel * 1/32)
// Despite being declared void, the asm returns a value in f1
f32 TMario::getChangeAngleSpeed()
{
	f32 angleSpeed;

	if (isForceSlip()) {
		angleSpeed = (f32) mSlipParamsAll.mSlideAngleYSp.value;
	} else {
		const TBGCheckData* ground = mGroundPlane;
		u16 type = ground->mBGType;

		u8 isOil;
		if (type == 0x0c || type == 0x800c || type == 0xa00c)
			isOil = 1;
		else
			isOil = 0;
		if (isOil) {
			angleSpeed = (f32) mSlipParamsAllSlider.mSlideAngleYSp.value;
		} else {
			u8 isAll;
			if (type == 0x02 || type == 0x8002)
				isAll = 1;
			else
				isAll = 0;
			if (isAll) {
				angleSpeed = (f32) mSlipParams45.mSlideAngleYSp.value;
			} else {
				u8 isWater;
				if (type == 0x04 || type == 0x4004
				    || type == 0x8004 || type == 0xc004)
					isWater = 1;
				else
					isWater = 0;
				if (isWater) {
					if (ground->mNormal.y > 0.99f) {
						angleSpeed = (f32) *(s16*)((u8*)this
						                           + 0x2F70);
					} else {
						angleSpeed = (f32) *(s16*)((u8*)this
						                           + 0x2E8C);
					}
				} else {
					angleSpeed
					    = (f32) mSlipParamsNormal.mSlideAngleYSp.value;
				}
			}
		}
	}

	return 0.03125f * angleSpeed * mForwardVel;
}

// getSlideStickMult - 0x8013B8E8
f32 TMario::getSlideStickMult()
{
	if (isForceSlip()) {
		return mSlipParamsAll.mStickSlideMult.get();
	}

	u16 groundType = mGroundPlane->mBGType;

	u8 isIce;
	if (groundType == 0x0C || groundType == 0x800C || groundType == 0xA00C) {
		isIce = 1;
	} else {
		isIce = 0;
	}
	if (isIce) {
		return mSlipParamsAllSlider.mStickSlideMult.get();
	}

	u8 isSand;
	if (groundType == 0x02 || groundType == 0x8002) {
		isSand = 1;
	} else {
		isSand = 0;
	}
	if (isSand) {
		return mSlipParams45.mStickSlideMult.get();
	}

	return mSlipParamsNormal.mStickSlideMult.get();
}

// slideProcess - 0x80139E28
void TMario::slideProcess(f32 accelUp, f32 accelDown)
{
	const TBGCheckData* ground = mGroundPlane;
	s16 slopeAngle = matan(ground->mNormal.z, ground->mNormal.x);

	f32 nx = ground->mNormal.x;
	f32 nz = ground->mNormal.z;
	f32 slopeMag = nx * nx + nz * nz;
	f32 sqrtMag;
	if (slopeMag > 0.0f) {
		f32 root = __frsqrte(slopeMag);
		f32 refined = 0.5f * root * (3.0f - slopeMag * (root * root));
		sqrtMag = slopeMag * refined;
		sqrtMag = (f32)sqrtMag;
	} else {
		sqrtMag = slopeMag;
	}

	s16 faceY = mFaceAngle.y;
	f32 accelUpLocal;
	f32 accelDownLocal;
	s32 angleDiff = (s16)(mSlopeAngle - faceY);
	getSlopeSlideAccele(&accelUpLocal, &accelDownLocal);

	s16 diffExt = (s16)angleDiff;
	if (diffExt > -16384 && diffExt < 16384) {
		accelUp = accelUp + accelUpLocal * sqrtMag;
	} else {
		accelUp = accelUp + accelDownLocal * sqrtMag;
	}

	u16 uAngle = (u16)slopeAngle;
	mSlideVelX = mSlideVelX + accelUp * JMASSin(uAngle);
	mSlideVelZ = mSlideVelZ + accelUp * JMASCos(uAngle);

	mSlideVelX = mSlideVelX * accelDown;
	mSlideVelZ = mSlideVelZ * accelDown;

	unk9E = matan(mSlideVelZ, mSlideVelX);

	f32 negThresh = -1.0f;
	s32 angleChange = 0;
	if (negThresh < mSlideVelX && mSlideVelX < 0.0f
	    && negThresh < mSlideVelZ && mSlideVelZ < 0.0f) {
		// velocity very small, skip angle processing
	} else {
		s16 slideAngle = unk9E;
		s16 faceDiff = (s16)(mFaceAngle.y - slideAngle);
		s32 faceDiffExt = (s16)faceDiff;
		angleChange = faceDiffExt;

		if (faceDiffExt > 0 && faceDiffExt <= 16384) {
			getChangeAngleSpeed();
			f32 changeAngleSpd = 0.03125f * (f32)mSlipParamsNormal.mSlideAngleYSp.value * mForwardVel;
			angleChange = (s32)((f32)faceDiffExt - changeAngleSpd);
			if (angleChange < 0)
				angleChange = 0;
		} else if (faceDiffExt > -16384 && faceDiffExt < 0) {
			getChangeAngleSpeed();
			f32 changeAngleSpd = 0.03125f * (f32)mSlipParamsNormal.mSlideAngleYSp.value * mForwardVel;
			angleChange = (s32)((f32)faceDiffExt + changeAngleSpd);
			if (angleChange > 0)
				angleChange = 0;
		} else if (faceDiffExt > 16384 && faceDiffExt < 0x18000) {
			getChangeAngleSpeed();
			f32 changeAngleSpd = 0.03125f * (f32)mSlipParamsNormal.mSlideAngleYSp.value * mForwardVel;
			angleChange = (s32)((f32)faceDiffExt + changeAngleSpd);
			if (angleChange > 0x18000)
				angleChange = 0x18000;
		} else if (faceDiffExt > -32768 && faceDiffExt < -16384) {
			getChangeAngleSpeed();
			f32 changeAngleSpd = 0.03125f * (f32)mSlipParamsNormal.mSlideAngleYSp.value * mForwardVel;
			angleChange = (s32)((f32)faceDiffExt - changeAngleSpd);
			if (angleChange < -32768)
				angleChange = -32768;
		}

		mFaceAngle.y = (s16)(unk9E + angleChange);
	}

	mVel.x = mSlideVelX;
	f32 zero = 0.0f;
	mVel.y = zero;
	mVel.z = mSlideVelZ;

	f32 sx = mSlideVelX;
	f32 sz = mSlideVelZ;
	f32 velSq = sx * sx + sz * sz;
	f32 velMag;
	if (velSq > zero) {
		f32 root = __frsqrte(velSq);
		f32 refined = 0.5f * root * (3.0f - velSq * (root * root));
		velMag = velSq * refined;
		velMag = (f32)velMag;
	} else {
		velMag = velSq;
	}
	mForwardVel = velMag;

	if (mForwardVel > zero) {
		mSlideVelX = zero * mSlideVelX / mForwardVel;
		mSlideVelZ = zero * mSlideVelZ / mForwardVel;
	}

	if (angleChange < -16384 || angleChange > 16384) {
		mForwardVel = mForwardVel * 0.5f;
	}
}

// doSliding - 0x80139A3C
int TMario::doSliding(f32 stopThreshold)
{
	s32 result = 0;

	// Compute sine/cosine of face-slide direction difference
	s16 slideDir = unk9E;
	s16 faceDir = mIntendedYaw;
	u16 angleDiff = (u16)(faceDir - slideDir);
	f32 sinVal = JMASSin(angleDiff);
	f32 cosVal = JMASCos(angleDiff);

	// Adjust acceleration based on forward velocity sign
	if (sinVal < 0.0f) {
		f32 fwdVel = mForwardVel;
		if (fwdVel >= 0.0f) {
			f32 mult1 = 2.0f;
			f32 mult2 = 1.5f;
			f32 factor = mult1 * fwdVel * (mult2 * sinVal + mult1);
			sinVal = sinVal * factor;
		}
	}

	// Check for specific sliding action
	u32 action = mAction;
	if ((action - 0x007C0000) == 0x045D) {
		f32 slideStop = mSlipParamsOil.mSlipFriction.value;
	} else {
		u8 forceSlip;
		forceSlip = isForceSlip();
		if (forceSlip) {
			f32 slideStop = mSlipParamsAll.mSlipFriction.value;
		} else {
			const TBGCheckData* ground = mGroundPlane;
			u16 type = ground->mBGType;

			u8 isOil;
			if (type == 0x0c || type == 0x800c || type == 0xa00c)
				isOil = 1;
			else
				isOil = 0;
			if (isOil) {
				f32 slideStop = mSlipParamsAllSlider.mSlipFriction.value;
			} else {
				u8 isAll;
				if (type == 0x02 || type == 0x8002)
					isAll = 1;
				else
					isAll = 0;
				if (isAll) {
					f32 slideStop = mSlipParams45.mSlipFriction.value;
				} else {
					u8 isWater;
					if (type == 0x04 || type == 0x4004 || type == 0x8004 || type == 0xc004)
						isWater = 1;
					else
						isWater = 0;
					if (isWater) {
						if (ground->mNormal.y > 0.99f) {
							f32 slideStop = mSlipParamsWaterGround.mSlipFriction.value;
						} else {
							f32 slideStop = mSlipParamsWaterSlope.mSlipFriction.value;
						}
					} else {
						u8 isYoshi;
						isYoshi = onYoshi();
						if (isYoshi) {
							f32 slideStop = mSlipParamsYoshi.mSlipFriction.value;
						} else {
							f32 slideStop2 = mSlipParamsNormal.mSlipFriction.value;
							u32 action2 = mAction;
							if ((action2 - 0x00800000) == 0x0456) {
								if (mActionState == 1)
									slideStop2 = mDeParams.mWasOnWaterSlip.value;
								u8 isInShallow;
								if (unk118 & 0x30000)
									isInShallow = 1;
								else
									isInShallow = 0;
								if (isInShallow) {
									slideStop2 = mDeParams.mInWaterSlip.value;
								}
							}
						}
					}
				}
			}
		}
	}

	// This is getting too complex - the real logic involves computing slideStop
	// and using it in slideProcess. Let me use a simpler approach.

	f32 slideStop;
	if ((mAction - 0x007C0000) == 0x045D) {
		slideStop = mSlipParamsOil.mSlipFriction.value;
	} else if (isForceSlip()) {
		slideStop = mSlipParamsAll.mSlipFriction.value;
	} else {
		const TBGCheckData* ground2 = mGroundPlane;
		u16 type2 = ground2->mBGType;
		u8 isOil2;
		if (type2 == 0x0c || type2 == 0x800c || type2 == 0xa00c)
			isOil2 = 1;
		else
			isOil2 = 0;
		if (isOil2) {
			slideStop = mSlipParamsAllSlider.mSlipFriction.value;
		} else {
			u8 isAll2;
			if (type2 == 0x02 || type2 == 0x8002)
				isAll2 = 1;
			else
				isAll2 = 0;
			if (isAll2) {
				slideStop = mSlipParams45.mSlipFriction.value;
			} else {
				u8 isWater2;
				if (type2 == 0x04 || type2 == 0x4004 || type2 == 0x8004 || type2 == 0xc004)
					isWater2 = 1;
				else
					isWater2 = 0;
				if (isWater2) {
					if (ground2->mNormal.y > 0.99f)
						slideStop = mSlipParamsWaterGround.mSlipFriction.value;
					else
						slideStop = mSlipParamsWaterSlope.mSlipFriction.value;
				} else if (onYoshi()) {
					slideStop = mSlipParamsYoshi.mSlipFriction.value;
				} else {
					slideStop = mSlipParamsNormal.mSlipFriction.value;
					if ((mAction - 0x00800000) == 0x0456) {
						if (mActionState == 1)
							slideStop = mDeParams.mWasOnWaterSlip.value;
						u8 isInShallow;
						if (unk118 & 0x30000)
							isInShallow = 1;
						else
							isInShallow = 0;
						if (isInShallow)
							slideStop = mDeParams.mInWaterSlip.value;
					}
				}
			}
		}
	}

	// Compute slide deceleration
	f32 mag = mSlideVelX * mSlideVelX + mSlideVelZ * mSlideVelZ;
	f32 oldSpeed;
	if (mag > 0.0f) {
		f32 root = __frsqrte(mag);
		f32 refined = 0.5f * root * (3.0f - mag * (root * root));
		oldSpeed = mag * refined;
		oldSpeed = (f32)oldSpeed;
	} else {
		oldSpeed = mag;
	}

	f32 stickMult = getSlideStickMult();
	f32 stickEffect = mIntendedMag * 0.03125f;
	f32 accelX = mSlideVelZ * stickEffect * cosVal;
	mSlideVelX = mSlideVelX + accelX * sinVal;

	stickMult = getSlideStickMult();
	stickEffect = mIntendedMag * 0.03125f;
	f32 accelZ = mSlideVelX * stickEffect * cosVal;
	mSlideVelZ = mSlideVelZ - accelZ * sinVal;

	// Compute new speed
	f32 newSx = mSlideVelX;
	f32 newSz = mSlideVelZ;
	f32 newMag = newSx * newSx + newSz * newSz;
	f32 newSpeed;
	if (newMag > 0.0f) {
		f32 root = __frsqrte(newMag);
		f32 refined = 0.5f * root * (3.0f - newMag * (root * root));
		newSpeed = newMag * refined;
		newSpeed = (f32)newSpeed;
	} else {
		newSpeed = newMag;
	}

	// Keep speed from increasing
	if (oldSpeed > 0.0f && newSpeed > 0.0f) {
		mSlideVelX = mSlideVelX * oldSpeed / newSpeed;
		mSlideVelZ = mSlideVelZ * oldSpeed / newSpeed;
	}

	slideProcess(sinVal, slideStop);

	// Check ground type for stop conditions
	const TBGCheckData* ground3 = mGroundPlane;
	u16 type3 = ground3->mBGType;
	u8 isSlippery;
	if (type3 == 0x01 || type3 == 0x4001 || type3 == 0x8001 || type3 == 0xC001)
		isSlippery = 1;
	else
		isSlippery = 0;
	if (!isSlippery) {
		u8 isOil3;
		if (type3 == 0x0c || type3 == 0x800c || type3 == 0xa00c)
			isOil3 = 1;
		else
			isOil3 = 0;
		if (!isOil3) {
			if (mForwardVel * mForwardVel < stopThreshold * stopThreshold) {
				setPlayerVelocity(0.0f);
				mInput = mInput & ~0x4;
				result = 1;
			}
		}
	}

	return result;
}

void TMario::slopeProcess()
{
	const TBGCheckData* ground = mGroundPlane;
	f32 nx = ground->mNormal.x;
	f32 nz = ground->mNormal.z;

	f32 slopeMag = nx * nx + nz * nz;
	f32 sqrtMag = sqrtf(slopeMag);

	s16 faceY = mFaceAngle.y;
	s16 slopeAngle = mSlopeAngle;
	s16 angleDiff = slopeAngle - faceY;

	f32 accelUp;
	f32 accelDown;
	getSlopeNormalAccele(&accelUp, &accelDown);

	s16 diffExt = (s16)angleDiff;
	if (diffExt > -16384 && diffExt < 16384) {
		mForwardVel = mForwardVel + accelUp * sqrtMag;
	} else {
		mForwardVel = mForwardVel - accelDown * sqrtMag;
	}

	if (mForwardVel > mDeParams.mRunningMax.value) {
		mForwardVel = mDeParams.mRunningMax.value;
	}

	s16 modelAngle = mFaceAngle.y;
	mModelFaceAngle = modelAngle;

	u16 angle = mFaceAngle.y;
	f32 sinVal = JMASSin(angle);
	mSlideVelX = mForwardVel * sinVal;

	angle = mFaceAngle.y;
	f32 cosVal = JMASCos(angle);
	mSlideVelZ = mForwardVel * cosVal;

	mVel.x = mSlideVelX;
	mVel.y = 0.0f;
	mVel.z = mSlideVelZ;
}

// doRunning - 0x8013B5DC
void TMario::doRunning()
{
	f32 maxSpeed = mIntendedMag;
	if (maxSpeed >= mRunParams.mMaxSpeed.value)
		maxSpeed = mRunParams.mMaxSpeed.value;

	f32 runMult = maxSpeed;

	if (isRunningInWater()) {
		runMult = runMult * mYoshiParams.mRunYoshiMult.value;
	}

	f32 fwdVel = mForwardVel;
	if (fwdVel <= 0.0f) {
		// Accelerate from zero
		mForwardVel = fwdVel + mRunParams.mVelMinusBrake.value;
	} else if (fwdVel <= runMult) {
		// Accelerate towards target
		f32 addVelDiv = mRunParams.mAddVelDiv.value;
		f32 addBase = mRunParams.mAddBase.value;
		mForwardVel = fwdVel + (addBase - fwdVel * addVelDiv);
	} else {
		// Decelerate from above target
		const TBGCheckData* ground = mGroundPlane;
		if (ground->mNormal.y >= mRunParams.mDecStartNrmY.value) {
			// Normal ground - no extra deceleration
		} else {
			f32 decBrake = mRunParams.mDecBrake.value;
			mForwardVel = fwdVel - decBrake;
			mForwardVel = mForwardVel - mYoshiParams.mDecBrake.value;
		}
	}

	if (mForwardVel < 0.0f)
		mForwardVel = 0.0f;

	// Compute angle change speed based on unk380
	s16 angleChange;
	u8 isPumpState;
	if (unk380 == 0 || unk380 == 1)
		isPumpState = 1;
	else
		isPumpState = 0;
	if (isPumpState) {
		s16 minRot = mDeParams.mPumpingRotSpMin.value;
		s16 maxRot = mDeParams.mPumpingRotSpMax.value;
		f32 fwdSpd = mForwardVel;
		f32 scale = 0.03125f;
		angleChange = (s16)((f32)minRot + scale * fwdSpd * (f32)(maxRot - minRot));
	} else {
		s16 minRot = mDeParams.mRunningRotSpMin.value;
		s16 maxRot = mDeParams.mRunningRotSpMax.value;
		f32 fwdSpd = mForwardVel;
		f32 scale = 0.03125f;
		angleChange = (s16)((f32)minRot + scale * fwdSpd * (f32)(maxRot - minRot));
	}

	if (isRunningInWater()) {
		angleChange = (s16)((f32)angleChange * mYoshiParams.mRotYoshiMult.value);
	}

	u8 isInWater;
	if (unk118 & 0x4000)
		isInWater = 1;
	else
		isInWater = 0;
	if (isInWater) {
		angleChange = mRunParams.mDashRotSp.value;
	}

	u8 isInShallow;
	if (unk118 & 0x30000)
		isInShallow = 1;
	else
		isInShallow = 0;
	if (isInShallow) {
		if (mFloorPosition.y < mPosition.y + mRunParams.mSwimDepth.value) {
			isInShallow = 1;
		} else {
			isInShallow = 0;
		}
	}
	if (isInShallow) {
		f32 depth = mFloorPosition.y - mPosition.y;
		f32 swimDepth = mRunParams.mSwimDepth.value;
		f32 brake = 1.0f - mRunParams.mInWaterBrake.value;
		f32 ratio = depth / swimDepth;
		mForwardVel = mForwardVel * (1.0f - ratio * brake);
	}

	s16 yawDiff = (s16)(mIntendedYaw - mFaceAngle.y);
	s16 converged = IConverge((s16)yawDiff, 0, (s16)angleChange, (s16)angleChange);
	mFaceAngle.y = mIntendedYaw - converged;

	slopeProcess();
}

// getSurfingParamsWater - 0x8013ACA0
TMario::TSurfingParams& TMario::getSurfingParamsWater()
{
	switch (unk389) {
	case 1:
		return mSurfingParamsWaterYellow;
	case 2:
		return mSurfingParamsWaterGreen;
	default:
		return mSurfingParamsWaterRed;
	}
}

// doSurfing - 0x8013B198
void TMario::doSurfing()
{
	f32 waterHeight = mPosition.y - mVel.y;
	const TBGCheckData* waterPlane = nullptr;
	checkGroundPlane(mPosition.x, waterHeight, mPosition.z, nullptr,
	                 &waterPlane);

	u16 groundType = waterPlane ? *(u16*)waterPlane : 0;

	u8 isSurfType;
	if (groundType == 0x100 || groundType == 0x101
	    || (u16)(groundType - 0x102) <= 3
	    || groundType == 0x4104)
		isSurfType = 1;
	else
		isSurfType = 0;

	f32 rotMin, rotMax, powMin, powMax;
	u8 color = unk389;

	if (isSurfType) {
		TSurfingParams& params = getSurfingParamsWater();
		rotMin = params.mRotMin.get();

		TSurfingParams& params2 = getSurfingParamsWater();
		rotMax = params2.mRotMax.get();

		TSurfingParams& params3 = getSurfingParamsWater();
		powMin = params3.mPowMin.get();

		TSurfingParams& params4 = getSurfingParamsWater();
		powMax = params4.mPowMax.get();
	} else {
		switch (color) {
		case 1: rotMin = *(f32*)((u8*)this + 0x1BC4 + 0x18); break;
		case 2: rotMin = *(f32*)((u8*)this + 0x1F6C + 0x18); break;
		default: rotMin = *(f32*)((u8*)this + 0x181C + 0x18); break;
		}
		switch (color) {
		case 1: rotMax = *(f32*)((u8*)this + 0x1BC4 + 0x2C); break;
		case 2: rotMax = *(f32*)((u8*)this + 0x1F6C + 0x2C); break;
		default: rotMax = *(f32*)((u8*)this + 0x181C + 0x2C); break;
		}
		switch (color) {
		case 1: powMin = *(f32*)((u8*)this + 0x1BC4 + 0x40); break;
		case 2: powMin = *(f32*)((u8*)this + 0x1F6C + 0x40); break;
		default: powMin = *(f32*)((u8*)this + 0x181C + 0x40); break;
		}
		switch (color) {
		case 1: powMax = *(f32*)((u8*)this + 0x1BC4 + 0x54); break;
		case 2: powMax = *(f32*)((u8*)this + 0x1F6C + 0x54); break;
		default: powMax = *(f32*)((u8*)this + 0x181C + 0x54); break;
		}
	}

	// Clamp intended magnitude to speed range
	f32 clampedMag = 0.03125f * mIntendedMag;
	f32 speedInput = clampedMag;
	if (speedInput > powMax)
		speedInput = powMax;
	if (speedInput < powMin)
		speedInput = powMin;

	// Accelerate or decelerate forward velocity
	f32 fwdVel = mForwardVel;
	if (fwdVel <= 0.0f) {
		// Accelerate forward
		mForwardVel = fwdVel + rotMin;
	} else if (fwdVel <= speedInput) {
		// Under target speed - check ground and accelerate
		u16 gt2 = mGroundPlane ? *(u16*)mGroundPlane : 0;
		u8 isSurf2;
		if (gt2 == 0x100 || gt2 == 0x101
		    || (u16)(gt2 - 0x102) <= 3
		    || gt2 == 0x4104)
			isSurf2 = 1;
		else
			isSurf2 = 0;

		f32 accel;
		if (isSurf2) {
			TSurfingParams& p = getSurfingParamsWater();
			accel = p.mAccel.get();
		} else {
			switch (color) {
			case 1: accel = *(f32*)((u8*)this + 0x1BC4 + 0x68); break;
			case 2: accel = *(f32*)((u8*)this + 0x1F6C + 0x68); break;
			default: accel = *(f32*)((u8*)this + 0x181C + 0x68); break;
			}
		}

		mForwardVel = fwdVel + (1.0f - fwdVel / accel);
	} else {
		// Over target speed - check ground slope
		if (mGroundPlane->mNormal.y >= 0.0f) {
			mForwardVel = fwdVel - 0.3f;
		}
	}

	if (mForwardVel > powMax)
		mForwardVel = powMax;

	// Interpolate rotation speed
	f32 rotRange = speedInput - powMin;
	f32 maxRange = powMax - powMin;
	f32 powRange = rotMax - rotMin;
	f32 rotSpeed;
	if (maxRange > 0.0f)
		rotSpeed = rotMin + (rotRange / maxRange) * powRange;
	else
		rotSpeed = rotMin;

	s16 yawDiff = (s16)(mIntendedYaw - mFaceAngle.y);
	s16 rotSpeedS16 = (s16)rotSpeed;
	s16 converged = IConverge((s16)yawDiff, 0, rotSpeedS16, rotSpeedS16);
	mFaceAngle.y = mIntendedYaw - converged;

	slopeProcess();

	// Check if on surfing ground for special handling
	u16 gt3 = waterPlane ? *(u16*)waterPlane : 0;
	u8 isSurf3;
	if (gt3 == 0x100 || gt3 == 0x101
	    || (u16)(gt3 - 0x102) <= 3
	    || gt3 == 0x4104)
		isSurf3 = 1;
	else
		isSurf3 = 0;
	if (isSurf3) {
		postureControl();
	}
}

void TMario::doPushingAnimation(const Vec& target)
{
	f32 dx = mPosition.x - target.x;
	f32 dz = mPosition.z - target.z;

	if (mForwardVel > 6.0f) {
		setPlayerVelocity(0.0f);
	}

	s16 wallAngle;
	s16 angleDiff;
	if (mWallPlane != nullptr) {
		wallAngle = getWallAngle();
		angleDiff = wallAngle - mFaceAngle.y;
	}

	if (mWallPlane == nullptr) {
		setAnimation(0x6c, 1.0f);
		startVoice(0x7094);
		return;
	}

	s16 extDiff = (s16)angleDiff;
	if (extDiff < -29127 || extDiff > 29127) {
		setAnimation(0x6c, 1.0f);
		startVoice(0x7094);
		return;
	}

	f32 distSq = dz * dz + dx * dx;
	f32 dist;
	if (distSq > 0.0f) {
		f32 root = __frsqrte(distSq);
		f32 refined = 0.5f * root * (3.0f - distSq * (root * root));
		dist = distSq * refined;
		dist = (f32)dist;
	} else {
		dist = distSq;
	}

	f32 speed = 2.0f * dist;

	if ((s16)angleDiff < 0) {
		setAnimation(0x80, speed);
	} else {
		setAnimation(0x7f, speed);
	}

	mFaceAngle.x = 0;
	mModelFaceAngle = (s16)(wallAngle + 0x18000);
}

// running - 0x801398B0
void TMario::running()
{
	u16 timer = mActionTimer;
	mActionTimer = timer + 1;

	// Check held object throw
	TTakeActor* held = mHeldObject;
	BOOL throwResult;
	if (held == nullptr) {
		throwResult = 0;
	} else {
		u8 isJumpHeld;
		if (mInput & 0x2000)
			isJumpHeld = 1;
		else
			isJumpHeld = 0;
		if (!isJumpHeld) {
			throwResult = 0;
		} else {
			u32 heldType = held->mActorType;
			u8 isBitSet;
			if (heldType & 0x10000000)
				isBitSet = 1;
			else
				isBitSet = 0;
			if (isBitSet) {
				throwResult = changePlayerStatus(0x80000588, 0, false);
			} else if (heldType == (u32)0x80000001) {
				throwResult = changePlayerStatus(0x80000588, 0, false);
			} else if (mForwardVel > 0.0f) {
				throwResult = changePlayerStatus(0x80000588, 0, false);
			} else if (canPut()) {
				throwResult = changePlayerStatus(0x80000387, 0, false);
			} else {
				throwResult = 0;
			}
		}
	}

	if (throwResult != 0)
		return;

	// Check jump inputs
	u32 input = mInput;
	if (input & 0x08) {
		if (mForwardVel <= 0.0f || isFrontSlip(0)) {
			changePlayerStatus(0x50, 0, false);
			return;
		}
	}

	if (input & 0x02) {
		changePlayerJumping(0x0887, 0);
		return;
	}

	// Check stick rotate
	int stickDir;
	BOOL rotated = checkStickRotate(&stickDir);
	if (rotated == 1 && mWaterGun != nullptr) {
		if (mWaterGun->isEmitting()) {
			if (stickDir > 0) {
				changePlayerStatus(0x441, 0, false);
			} else {
				changePlayerStatus(0x442, 0, false);
			}
		} else {
			rotated = 0;
		}
	} else {
		rotated = 0;
	}

	if (rotated != 0)
		return;

	// Check crouch/slide
	if (mInput & 0x20) {
		changePlayerStatus(0x04000445, 0, false);
		return;
	}

	// Check turn
	u8 isTurnable;
	u32 pumpState = unk380;
	if (pumpState == 0 || pumpState == 1)
		isTurnable = 1;
	else
		isTurnable = 0;
	u8 shouldTurn;
	if (!isTurnable) {
		if (isRunningTurnning()) {
			shouldTurn = 0;
		} else {
			s16 yawDiff = (s16)(mIntendedYaw - mFaceAngle.y);
			shouldTurn = 1;
			if (yawDiff >= -18204 && yawDiff <= 18204)
				shouldTurn = 0;
		}
	} else {
		shouldTurn = 0;
	}

	if (shouldTurn) {
		if (mForwardVel >= mRunParams.mTurnNeedSp.value) {
			// Set up turn wall checks
			s16 turnAngle = mFaceAngle.y;
			checkPlayerAround(21, (f32)(turnAngle + 0x18000));
			checkPlayerAround(23, (f32)(turnAngle + 0x18000));
			checkPlayerAround(22, (f32)(turnAngle + 0x18000));
			changePlayerStatus(0x0443, 0, false);
			return;
		}
	}

	// Check if on dirt
	u8 isDirty;
	isDirty = isRunningSlipStart();
	if (isDirty) {
		setPlayerVelocity(0.0f);
		changePlayerStatus(0x0C018220, 0, false);
		return;
	}

	// Check slip start
	if (isSlipStart()) {
		TWaterGun* gun = mWaterGun;
		f32 gunHeight = *(f32*)((u8*)gun + 0x1D40);
		*(f32*)((u8*)this + 0x314) = mFloorPosition.y + gunHeight;
		changePlayerStatus(0x088B, 0, false);
		return;
	}

	// Main running logic
	s32 pushWall = 0;
	mActionState = 0;
	Vec prevPos;
	prevPos.x = mPosition.x;
	prevPos.y = mPosition.y;
	prevPos.z = mPosition.z;

	doRunning();
	doRunningAnimation();

	int walkResult = walkProcess();
	switch (walkResult) {
	case 0:
		changePlayerStatus(0x088C, 0, false);
		setAnimation(0x56, 1.0f);
		break;
	case 1:
		slopeProcess();
		break;
	case 2:
	case 3:
		if (isThrowStart()) {
			setPlayerVelocity(0.0f);
		}

		u8 isWallBit;
		if (unk118 & 0x4)
			isWallBit = 1;
		else
			isWallBit = 0;
		if (isWallBit)
			pushWall = 1;
		break;
	}

	if (!pushWall) {
		if (mForwardVel > mDeParams.mClashSpeed.value) {
			checkPlayerAround(12, 0.0f);
			changePlayerDropping(0x000208B0, 0);
			return;
		}

		if (mInput & 0x2) {
			if (mPosition.y + mDeParams.mJumpWallHeight.value > mFloorPosition.y) {
				if (mIntendedMag > 0.0f && mForwardVel > mDeParams.mDashMax.value - 1.0f) {
					mVel.y = 0.0f;
					changePlayerStatus(0x0080088A, 1, false);
					return;
				}
			}
			changePlayerStatus(0x384, 0, false);
			return;
		}

		if (mInput & 0x4) {
			if (mInput & 0x4000) {
				if (mForwardVel > mRunParams.mDoJumpCatchSp.value) {
					changePlayerJumping(0x0888, 0);
					return;
				}
			}

			if (mInput & 0x2) {
				changePlayerTriJump();
				return;
			}

			if (considerRotateStart())
				return;

			if (mInput & 0x10000) {
				if (mForwardVel > mDeParams.mDashMax.value - 1.0f) {
					if (mIntendedMag > 0.0f) {
						mVel.y = 0.0f;
						s16 turnAngle = mFaceAngle.y;
						mFaceAngle.y = (s16)(turnAngle + 0x18000);
						setPlayerVelocity(-1.0f);
						changePlayerStatus(0x02000886, 0, false);
						return;
					}
				}
			}

			const TBGCheckData* wall = mWallPlane;
			if (wall != nullptr) {
				u8 isWallType;
				if (*(u16*)wall == 0x10A)
					isWallType = 1;
				else
					isWallType = 0;
				if (isWallType) {
					s16 wallAngle = matan(wall->mNormal.z + 52, wall->mNormal.x);
					s16 faceAngle = mFaceAngle.y;
					mFaceAngle.y = (s16)(wallAngle + 0x18000);
					mModelFaceAngle = mFaceAngle.y;
					changePlayerStatus(0x3000036B, 0, false);
					return;
				}
			}

			doPushingAnimation(prevPos);
			mFaceAngle.x = 0;
			unk118 = unk118 & ~0x4000;
		}
	}

	doRunningAnimation();

	u8 isInWater;
	if (unk118 & 0x4000)
		isInWater = 1;
	else
		isInWater = 0;
	if (isInWater) {
		setPlayerVelocity(mDeParams.mDashMax.value);
		checkPlayerAround(25, 0.0f);
	}
}

// rotating - 0x80139E80
void TMario::rotating()
{
	if (mInput & 0x2) {
		if (mAction == 0x0441) {
			changePlayerStatus(0x0896, 0, false);
			return;
		} else {
			changePlayerStatus(0x0895, 0, false);
			return;
		}
	}

	setAnimation(244, 1.0f);
	emitRotateShootEffect();
	emitBlurSpinJump();

	mActionTimer++;
	if (mActionTimer > 120) {
		changePlayerStatus(0x0C400201, 0, false);
		return;
	}

	doRunning();
	if (walkProcess() == 0) {
		changePlayerStatus(0x088C, 0, false);
	}

	if (mAction == 0x0441) {
		mModelFaceAngle = (s16)(mActionTimer << 12);
	} else {
		mModelFaceAngle = (s16)(-(mActionTimer << 12));
	}
	return;
}

void TMario::fireDashing()
{
	if (mInput & 0x02) {
		changePlayerStatus(0x000208b4, 0, false);
		return;
	}

	u16 timer = mActionTimer;
	mActionTimer = timer + 1;
	if (timer > 160) {
		changePlayerStatus(0x04000440, 0, false);
		return;
	}

	u8 isWallHit;
	if (unk118 & 0x00030000)
		isWallHit = 1;
	else
		isWallHit = 0;
	if (isWallHit) {
		changePlayerStatus(0x04000440, 0, false);
		return;
	}

	if (mForwardVel < 0.0f)
		mForwardVel = 0.0f;

	if (mForwardVel > 8.0f)
		mForwardVel = 8.0f;

	mForwardVel = FConverge(mForwardVel, 48.0f, 32.0f, 4.0f);

	if (mInput & 0x01) {
		s16 yawDiff = mIntendedYaw - mFaceAngle.y;
		s16 result = IConverge((s16)yawDiff, 0, 1536, 1536);
		mFaceAngle.y = mIntendedYaw - result;
	}

	slopeProcess();
	int walkResult = walkProcess();
	if (walkResult == 0) {
		changePlayerStatus(0x000208b5, 0, false);
	}

	f32 anmSpeed = 0.5f * mForwardVel * 0.1f;
	setAnimation(0x29, anmSpeed);
}

void TMario::walkEnd()
{
	u32 input = mInput;
	if (!(input & 0x10)) {
		bool doJump;
		if (input & 0x08) {
			if (mForwardVel <= 0.1f || isFrontSlip(0)) {
				doJump = true;
			} else {
				doJump = false;
			}
		} else {
			doJump = false;
		}

		if (doJump) {
			changePlayerStatus(0x50, 0, false);
			return;
		}

		input = mInput;
		if (input & 0x02) {
			changePlayerTriJump();
			return;
		}

		if (input & 0x01) {
			changePlayerStatus(0x04000440, 0, false);
			return;
		}

		if (input & 0x8000) {
			changePlayerStatus(0x384, 0, false);
			return;
		}
	}

	int stickDir;
	BOOL rotated = checkStickRotate(&stickDir);
	if (rotated == 1 && mWaterGun != nullptr) {
		if (mWaterGun->isEmitting()) {
			if (stickDir > 0) {
				changePlayerStatus(0x441, 0, false);
			} else {
				changePlayerStatus(0x442, 0, false);
			}
		} else {
			rotated = 0;
		}
	} else {
		rotated = 0;
	}

	if (rotated != 0) {
		// return 1
		return;
	}

	s32 stopped = 0;
	mForwardVel = FConverge(mForwardVel, 0.0f, 4.0f, 0.0f);
	if (0.0f == mForwardVel)
		stopped = 1;

	setPlayerVelocity(mForwardVel);

	if (stopped) {
		changePlayerStatus(0x0C400201, 0, false);
		return;
	}

	int walkResult = walkProcess();
	switch (walkResult) {
	case 0:
		changePlayerStatus(0x088c, 0, false);
		break;
	case 2:
		setPlayerVelocity(0.0f);
		break;
	default:
		break;
	}

	f32 anmSpeed = mForwardVel * 0.25f;
	if (anmSpeed < 0.1f)
		anmSpeed = 0.1f;
	setAnimation(0x48, anmSpeed);
}

void TMario::surfing()
{
	setAnimation(0x6d, 1.0f);

	if (mActionTimer > 0) {
		mActionTimer = mActionTimer - 1;
		return;
	}

	if (mInput & 0x02) {
		mPosition.y = mPosition.y + 1.0f;
		changePlayerStatus(0x0281089a, 0, false);
		return;
	}

	doSurfing();
	walkProcess();
	int walkResult = walkProcess();

	switch (walkResult) {
	case 1:
		return;
	case 0:
		changePlayerStatus(0x0081089b, 0, false);
		return;
	case 2: {
		const TBGCheckData* wall = mWallPlane;
		if (wall == nullptr) {
			setPlayerVelocity(0.0f);
			loserExec();
			return;
		}

		f32 nz = wall->mNormal.z;
		f32 nx = wall->mNormal.x;
		s16 wallAngle = matan(nz, nx);

		s16 faceDiff = wallAngle - mFaceAngle.y;

		const TBGCheckData* ground = mGroundPlane;
		u16 groundType = ground->mBGType;

		u8 isSurfType;
		if (groundType == 0x100 || groundType == 0x101
		    || (u16)(groundType - 0x102) <= 3
		    || groundType == 0x4104)
			isSurfType = 1;
		else
			isSurfType = 0;

		s16 clashAngle;
		f32 clashSpeed;
		if (isSurfType) {
			u8 color = *(u8*)((u8*)this + 0x389);
			switch (color) {
			case 1:
				clashAngle
				    = *(s16*)((u8*)this + 0x19F0 + 0x1D0);
				break;
			case 2:
				clashAngle
				    = *(s16*)((u8*)this + 0x1D98 + 0x1D0);
				break;
			default:
				clashAngle
				    = *(s16*)((u8*)this + 0x1648 + 0x1D0);
				break;
			}

			switch (color) {
			case 1:
				clashSpeed
				    = *(f32*)((u8*)this + 0x19F0 + 0x1BC);
				break;
			case 2:
				clashSpeed
				    = *(f32*)((u8*)this + 0x1D98 + 0x1BC);
				break;
			default:
				clashSpeed
				    = *(f32*)((u8*)this + 0x1648 + 0x1BC);
				break;
			}
		} else {
			u8 color = *(u8*)((u8*)this + 0x389);
			switch (color) {
			case 1:
				clashAngle
				    = *(s16*)((u8*)this + 0x1BC4 + 0x1D0);
				break;
			case 2:
				clashAngle
				    = *(s16*)((u8*)this + 0x1F6C + 0x1D0);
				break;
			default:
				clashAngle
				    = *(s16*)((u8*)this + 0x181C + 0x1D0);
				break;
			}

			switch (color) {
			case 1:
				clashSpeed
				    = *(f32*)((u8*)this + 0x1BC4 + 0x1BC);
				break;
			case 2:
				clashSpeed
				    = *(f32*)((u8*)this + 0x1F6C + 0x1BC);
				break;
			default:
				clashSpeed
				    = *(f32*)((u8*)this + 0x181C + 0x1BC);
				break;
			}
		}

		s16 negClash = -clashAngle;
		if (faceDiff < negClash || clashAngle < faceDiff) {
			if (mForwardVel > clashSpeed) {
				decHP(mDeParams.mHpMax.value);
				changePlayerStatus(0x000208b3, 0, true);
				mForwardVel = -(0.8f * mForwardVel);
				mVel.y = 0.0f;
				return;
			}
		}

		setPlayerVelocity(0.0f);
		return;
	}
	default:
		break;
	}
}

// turnning - 0x8013A39C
void TMario::turnning()
{
	// Check held object throw
	TTakeActor* held = mHeldObject;
	BOOL throwResult;
	if (held == nullptr) {
		throwResult = 0;
	} else {
		u8 isJumpHeld;
		if (mInput & 0x2000)
			isJumpHeld = 1;
		else
			isJumpHeld = 0;
		if (!isJumpHeld) {
			throwResult = 0;
		} else {
			u32 heldType = held->mActorType;
			u8 isBitSet;
			if (heldType & 0x10000000)
				isBitSet = 1;
			else
				isBitSet = 0;
			if (isBitSet) {
				throwResult = changePlayerStatus(0x80000588, 0, false);
			} else if (heldType == (u32)0x80000001) {
				throwResult = changePlayerStatus(0x80000588, 0, false);
			} else if (mForwardVel > 0.0f) {
				throwResult = changePlayerStatus(0x80000588, 0, false);
			} else if (canPut()) {
				throwResult = changePlayerStatus(0x80000387, 0, false);
			} else {
				throwResult = 0;
			}
		}
	}

	if (throwResult != 0)
		return;

	// Check jump
	if (mInput & 0x08) {
		changePlayerStatus(0x50, 0, false);
		return;
	}

	// Check B press
	if (mInput & 0x02) {
		changePlayerJumping(0x0887, 0);
		return;
	}

	// Check stick rotate
	int stickDir;
	BOOL rotated = checkStickRotate(&stickDir);
	if (rotated == 1 && mWaterGun != nullptr) {
		if (mWaterGun->isEmitting()) {
			if (stickDir > 0) {
				changePlayerStatus(0x441, 0, false);
			} else {
				changePlayerStatus(0x442, 0, false);
			}
		} else {
			rotated = 0;
		}
	} else {
		rotated = 0;
	}

	if (rotated != 0)
		return;

	// Check crouch
	if (mInput & 0x20) {
		changePlayerStatus(0x04000445, 0, false);
		return;
	}

	// Check turn state
	u8 shouldStop;
	u32 pumpState = unk380;
	if (pumpState == 0 || pumpState == 1)
		shouldStop = 1;
	else
		shouldStop = 0;
	if (!shouldStop) {
		if (isRunningTurnning()) {
			shouldStop = 0;
		} else {
			s16 yawDiff = (s16)(mIntendedYaw - mFaceAngle.y);
			shouldStop = 1;
			if (yawDiff >= -18204 && yawDiff <= 18204)
				shouldStop = 0;
		}
	}

	if (!shouldStop) {
		changePlayerStatus(0x04000440, 0, false);
		return;
	}

	// Decelerate
	s32 stopped = 0;
	mForwardVel = FConverge(mForwardVel, 0.0f, 0.0f, 0.0f);
	if (0.0f == mForwardVel)
		stopped = 1;

	doRunning();

	if (stopped) {
		mFaceAngle.y = mIntendedYaw;
		setPlayerVelocity(0.0f);
		changePlayerStatus(0x0444, 0, false);
		return;
	}

	if (walkProcess() != 0) {
		// walk ok
	} else {
		changePlayerStatus(0x088C, 0, false);
	}

	if (mForwardVel >= 0.0f) {
		setAnimation(0xBC, 1.0f);
	} else {
		setAnimation(0xBD, 1.0f);

		if (isLast1AnimeFrame()) {
			if (mForwardVel > 0.0f) {
				mFaceAngle.y = mIntendedYaw;
				setPlayerVelocity(-mForwardVel);
				changePlayerStatus(0x04000440, 0, false);
			} else {
				mFaceAngle.y = mIntendedYaw;
				setPlayerVelocity(0.0f);
				changePlayerStatus(0x04000440, 0, false);
			}
		}
	}
}

void TMario::turnEnd()
{
	// Part 1: check held object for throw/put
	BOOL result;
	TTakeActor* held = mHeldObject;
	if (held == nullptr) {
		result = 0;
	} else {
		u8 isJumpHeld;
		if (mInput & 0x2000)
			isJumpHeld = 1;
		else
			isJumpHeld = 0;

		if (!isJumpHeld) {
			result = 0;
		} else {
			u32 heldType = held->mActorType;

			u8 isBitSet;
			if (heldType & 0x10000000)
				isBitSet = 1;
			else
				isBitSet = 0;
			if (isBitSet) {
				result = changePlayerStatus(0x80000588, 0, false);
			} else if (heldType == (u32)0x80000001) {
				result = changePlayerStatus(0x80000588, 0, false);
			} else if (mForwardVel > 16.0f) {
				result = changePlayerStatus(0x80000588, 0, false);
			} else if (canPut()) {
				result = changePlayerStatus(0x80000387, 0, false);
			} else {
				result = 0;
			}
		}
	}

	if (result != 0) {
		// return 1
		return;
	}

	// Part 2: check input flags
	u32 input = mInput;
	if (input & 0x08) {
		changePlayerStatus(0x50, 0, false);
		return;
	}

	if (input & 0x02) {
		changePlayerJumping(0x887, 0);
		return;
	}

	// Part 3: check stick rotate
	int stickDir;
	BOOL rotated = checkStickRotate(&stickDir);
	if (rotated == 1 && mWaterGun != nullptr) {
		if (mWaterGun->isEmitting()) {
			if (stickDir > 0) {
				changePlayerStatus(0x441, 0, false);
			} else {
				changePlayerStatus(0x442, 0, false);
			}
		} else {
			rotated = 0;
		}
	} else {
		rotated = 0;
	}

	if (rotated != 0) {
		// return 1
		return;
	}

	// Part 4: do running and animation
	doRunning();
	setAnimation(0xbd, 1.0f);

	if (walkProcess() == 0) {
		changePlayerStatus(0x088c, 0, false);
	}

	if (isLast1AnimeFrame()) {
		changePlayerStatus(0x04000440, 0, false);
	}

	mFaceAngle.x = 0;
	s16 modelAngle = mModelFaceAngle;
	mModelFaceAngle = (s16)(modelAngle + 0x18000);
}

// slippingBasic - 0x80138F00
void TMario::slippingBasic(int statusOnStop, int slipStatus, int slipArg)
{
	slopeProcess();

	// Check for jump/crouch to enter fire slide
	if (mInput & 0x2) {
		if (walkProcess() == 1) {
			changePlayerStatus(0x02000880, 0, false);
			return;
		}
	}

	// Check for L trigger water landing
	if (mInput & 0x10000) {
		mVel.y = 0.0f;
		changePlayerStatus(0x0080088A, 1, false);
		return;
	}

	int walkResult = walkProcess();
	switch (walkResult) {
	case 0:
		changePlayerStatus(slipStatus, 0, false);
		return;
	case 1:
		setAnimation(0x6C, 1.0f);
		*(u16*)((u8*)this + 0x114) = *(u16*)((u8*)this + 0x114) | 0x8;
		loserExec();
		return;
	case 2:
	case 3:
		if (isThrowStart()) {
			setPlayerVelocity(0.0f);
		}
		break;
	}

	// Check speed for down state
	f32 fwdVel = mForwardVel;
	if (fwdVel < 0.0f)
		fwdVel = -fwdVel;
	if (fwdVel > mDeParams.mClashSpeed.value) {
		checkPlayerAround(12, 0.0f);
	}

	if (isSlipStart()) {
		const TBGCheckData* wall = mWallPlane;
		if (wall == nullptr) {
			// No wall - compute velocity from slide direction
			goto doSlipSound;
		}

		s16 wallAngle = matan(wall->mNormal.z + 52, wall->mNormal.x);

		f32 svx = mSlideVelX;
		f32 svz = mSlideVelZ;
		f32 velSq = svx * svx + svz * svz;
		f32 speed;
		if (velSq > 0.0f) {
			f32 root = __frsqrte(velSq);
			f32 refined = 0.5f * root * (3.0f - velSq * (root * root));
			speed = velSq * refined;
			speed = (f32)speed;
		} else {
			speed = velSq;
		}

		f32 speedHalf = speed * 0.5f;
		if (speedHalf < 0.0f)
			speedHalf = 0.0f;

		s16 slideDirAngle = unk9E;
		s16 wallDiff = (s16)(slideDirAngle - wallAngle);
		s16 wallDiffExt = (s16)wallDiff;
		s16 newAngle = (s16)(wallAngle - wallDiffExt + 0x18000);
		unk9E = newAngle;

		u16 uAngle = unk9E;
		mSlideVelX = speedHalf * JMASSin(uAngle);
		mVel.x = mSlideVelX;

		uAngle = unk9E;
		mSlideVelZ = speedHalf * JMASCos(uAngle);
		mVel.z = mSlideVelZ;

		// Play slip sound
		u8 groundAttr = mWallPlane ? ((u8*)mWallPlane)[6] : 0;
		s32 soundId;
		soundId = *(s32*)((u8*)gpMSound + 0);
		if (gpMSound->gateCheck(soundId)) {
			MSoundSESystem::MSoundSE::startSoundActor(soundId, (const Vec*)&mPosition,
			                                           0, nullptr, 0, 4);
		}
		goto done;
	}

	if (mForwardVel > 0.0f) {
		checkPlayerAround(1, 0.0f);
		changePlayerDropping(0x00020466, 0);
	} else {
		setPlayerVelocity(0.0f);
		changePlayerStatus(statusOnStop, 0, false);
	}

done:
doSlipSound:
	*(u16*)((u8*)this + 0x114) = *(u16*)((u8*)this + 0x114) | 0x8;
}

// slipForeCommon - 0x80138D30
void TMario::slipForeCommon(int statusOnStop, int jumpStatus, int slipStatus, int slipArg)
{
	if (mActionTimer > 20 && canSlipJump()) {
		if (mInput & 0x2) {
			changePlayerJumping(jumpStatus, 0);
			return;
		}
	} else {
		mActionTimer++;
	}

	f32 slideStop = getSlideStopNormal();
	if (doSliding(slideStop)) {
		changePlayerStatus(statusOnStop, 0, false);
	} else {
		slippingBasic(statusOnStop, slipStatus, slipArg);
	}
	return;
}

// slipBackCommon - 0x80138C50
void TMario::slipBackCommon(int statusOnStop, int slipStatus, int slipArg)
{
	u16 timer = mActionTimer;
	if (timer > 20) {
		u32 input = mInput;
		if (!(input & 0x8)) {
			if (input & 0x2) {
				if (canSlipJump()) {
					changePlayerDropping(0x08A6, 0);
					return;
				}
			}
		}
	} else {
		mActionTimer = timer + 1;
	}

	f32 slideStop = getSlideStopNormal();
	if (doSliding(slideStop)) {
		changePlayerStatus(statusOnStop, 0, false);
	} else {
		slippingBasic(statusOnStop, slipStatus, slipArg);
	}
	return;
}

// catching - 0x80138AFC
void TMario::catching()
{
	u32 input = mInput;
	if (!(input & 0x8)) {
		if (input & 0x2) {
			if (mForwardVel > mJumpParams.mRotBroadEnableV.get()) {
				changePlayerStatus(0x02000889, 0, false);
				return;
			} else {
				changePlayerStatus(0x08A6, 0, false);
				return;
			}
		}
	}

	u8 onWater;
	if (unk118 & 0x10) {
		onWater = 1;
	} else {
		onWater = 0;
	}
	if (onWater) {
		mActionState = 1;
	}

	f32 slideStop = getSlideStopCatch();
	if (doSliding(slideStop)) {
		setPlayerVelocity(0.0f);
		changePlayerStatus(902, 0, false);
		return;
	}

	slippingBasic(902, 0x088C, 136);

	if (gpMSound->gateCheck(0x1009)) {
		MSoundSESystem::MSoundSE::startSoundActor(0x1009, (const Vec*)&mPosition, 0,
		                                           nullptr, 0, 4);
	}

	J3DFrameCtrl& frameCtrl = getMotionFrameCtrl();
	if (frameCtrl.getFrame() > 45.0f) {
		getMotionFrameCtrl().setFrame(45.0f);
	}
	return;
}

// oilRun - 0x80138530
void TMario::oilRun()
{
	u32 input = mInput;

	// Check B button for jump
	if (input & 0x2) {
		setPlayerVelocity(0.0f);
		changePlayerJumping(0x02000880, 0);
		return;
	}

	// Check L trigger for water landing
	if (input & 0x10000) {
		mVel.y = 0.0f;
		changePlayerStatus(0x0080088A, 1, false);
		return;
	}

	// Check if velocity is very small
	f32 negThresh = -1.0f;
	f32 velX = mVel.x;
	if (negThresh < velX && velX < 0.0f) {
		f32 velZ = mVel.z;
		if (negThresh < velZ && velZ < 0.0f) {
			setPlayerVelocity(0.0f);
			changePlayerStatus(0x0C400201, 0, false);
			return;
		}
	}

	// Stamp pollution
	gpPollution->stamp(mDirtyParams.mPolSizeRun.value, mPosition.x, mPosition.y,
	                   mPosition.z, 1);

	// Compute angle rotation
	s16 rotSpeed = mDirtyParams.mSlipRotate.value;
	s16 yawDiff = (s16)(mIntendedYaw - mFaceAngle.y);
	s16 converged = IConverge((s16)yawDiff, 0, (s16)rotSpeed, (s16)rotSpeed);
	mFaceAngle.y = mIntendedYaw - converged;

	// Apply stick input to velocity
	u16 stickAngle = mFaceAngle.y;
	f32 sinVal = JMASSin(stickAngle);
	f32 stickMult = mDirtyParams.mSlipRunSp.value;
	mVel.x = mVel.x + mIntendedMag * sinVal * stickMult;

	u16 stickAngle2 = mFaceAngle.y;
	f32 cosVal = JMASCos(stickAngle2);
	mVel.z = mVel.z + mIntendedMag * cosVal * stickMult;

	// Decrement slip timer
	s16 slipTimer = *(s16*)((u8*)this + 0x13C);
	*(s16*)((u8*)this + 0x13C) = slipTimer - 1;
	if (*(s16*)((u8*)this + 0x13C) <= 0) {
		*(s16*)((u8*)this + 0x13C) = 0;
		*(f32*)((u8*)this + 0x138) = 0.0f;
	}

	// Apply friction
	mVel.x = mVel.x * *(f32*)((u8*)this + 0x138);
	mVel.z = mVel.z * *(f32*)((u8*)this + 0x138);

	// Reset forward velocity and slide components
	mForwardVel = 0.0f;
	mSlideVelX = 0.0f;
	mSlideVelZ = 0.0f;

	// Check if stationary
	f32 mag = mIntendedMag;
	if (0.0f == mag) {
		u8 isInWater;
		if (unk118 & 0x4000)
			isInWater = 1;
		else
			isInWater = 0;
		if (isInWater) {
			setAnimation(0x98, 1.0f);
		} else {
			setAnimation(0xC3, 1.0f);
		}
	} else {
		f32 anmSpeed = 0.5f * mag * mDirtyParams.mSlipAnmSpeed.value;
		setAnimation(0x72, anmSpeed);
		startVoice(30931);

		if (gpMSound->gateCheck(0x1001)) {
			MSoundSESystem::MSoundSE::startSoundActor(
			    0x1001, (Vec*)&mPosition, 0, nullptr, 0, 4);
		}
	}

	int walkResult = walkProcess();
	if (walkResult == 0) {
		changePlayerStatus(0x088C, 0, false);
		return;
	}
}

void TMario::oilSlip()
{
	if (mInput & 0x02) {
		setPlayerVelocity(0.0f);
		changePlayerJumping(0x02000880, 0);
		return;
	}

	// Convert s16 rotation speed param to float
	s16 rotSpeed = mDirtyParams.mSlipCatchRotate.value;
	s16 yawDiff = mIntendedYaw - mFaceAngle.y;
	s16 targetRot = (s16)((f32)rotSpeed);
	s16 converged = IConverge((s16)yawDiff, 0, (s16)targetRot, (s16)targetRot);
	mFaceAngle.y = mIntendedYaw - converged;

	// Decrement slip timer
	s16 slipTimer = *(s16*)((u8*)this + 0x13C);
	*(s16*)((u8*)this + 0x13C) = slipTimer - 1;

	if (*(s16*)((u8*)this + 0x13C) <= 0) {
		*(s16*)((u8*)this + 0x13C) = 0;
		*(f32*)((u8*)this + 0x138) = 0.0f;
		changePlayerStatus(0x00800456, 0, false);
	}

	// Pollution stamp
	gpPollution->stamp(mDirtyParams.mPolSizeSlip.value, mPosition.x, mPosition.y,
	                   mPosition.z, 1);

	// Sound effect
	if (gpMSound->gateCheck(0x1141)) {
		MSoundSESystem::MSoundSE::startSoundActor(
		    0x1141, (Vec*)&mPosition, 0, nullptr, 0, 4);
	}

	// Compute velocity from stick input and cos table
	// angle = (u16)(faceAngle.y - intendedYaw), then cos lookup
	u16 stickAngle = (u16)(mFaceAngle.y - mIntendedYaw);
	f32 cosVal = JMASCos(stickAngle);
	f32 stickMult = mDirtyParams.mSlipCatchSp.value;
	mForwardVel = mForwardVel + mIntendedMag * cosVal * stickMult;

	// Apply friction
	mForwardVel = mForwardVel * *(f32*)((u8*)this + 0x138);

	setPlayerVelocity(mForwardVel);

	// Check if speed is near zero (between -1 and 1)
	if (-1.0f < mForwardVel && mForwardVel < 1.0f) {
		setPlayerVelocity(0.0f);
		changePlayerStatus(0x386, 0, false);
		return;
	}

	int walkResult = walkProcess();
	if (walkResult == 0) {
		changePlayerStatus(0x088c, 0, false);
		return;
	}

	// Clamp animation frame to 50
	J3DFrameCtrl& frameCtrl = getMotionFrameCtrl();
	if (frameCtrl.getFrame() > 50.0f) {
		J3DFrameCtrl& frameCtrl2 = getMotionFrameCtrl();
		frameCtrl2.setFrame(50.0f);
	}
}

// downingCommon - 0x801384A8
f32 TMario::downingCommon(int anmId, f32 threshold, int nextState)
{
	f32 prevFrame = setAnimation(anmId, 1.0f);
	if (prevFrame < threshold) {
		slopeProcess();
		mForwardVel *= 0.96f;
		if (mForwardVel * mForwardVel < 1.0f) {
			setPlayerVelocity(0.0f);
		}
	} else if (mForwardVel >= 0.0f) {
		setPlayerVelocity(3.0f);
	} else {
		setPlayerVelocity(-3.0f);
	}

	if (walkProcess() == 0) {
		if (mForwardVel >= 0.0f) {
			changePlayerStatus(0x000208B0, nextState, false);
		} else {
			changePlayerStatus(0x000208B1, nextState, false);
		}
	} else {
		if (isLast1AnimeFrame()) {
			changePlayerStatus(0x0C400201, 0, false);
		}
	}
	return prevFrame;
}

// loserDown - 0x80138384
void TMario::loserDown()
{
	slopeProcess();
	mForwardVel *= 0.9f;
	if (mForwardVel * mForwardVel < 1.0f) {
		setPlayerVelocity(0.0f);
	}

	setAnimation(275, 1.0f);

	switch (mActionState) {
	case 0:
		startVoice(30813);
		mActionState++;
		break;
	case 1:
		if (gpMSound->checkMarioVoicePlaying(0) != 0)
			break;
		mActionTimer = 0;
		mActionState++;
		break;
	case 2:
		if (mActionTimer++ > 60) {
			mActionState++;
		}
		break;
	case 3:
		startVoice(30817);
		mActionState++;
		break;
	case 4:
		break;
	}
	return;
}

// jumpSlipCommon - 0x8013824C
void TMario::jumpSlipCommon(short anmId, u32 status)
{
	if (mInput & 0x1) {
		slopeProcess();
		mForwardVel *= 0.98f;
		if (mForwardVel * mForwardVel < 1.0f) {
			setPlayerVelocity(0.0f);
		}
	} else if (mForwardVel >= 0.0f) {
		mForwardVel = FConverge(mForwardVel, 0.0f, 0.0f, 0.0f);
		slopeProcess();
	} else {
		mVel.x = 0.0f;
	}

	int result = walkProcess();
	switch (result) {
	case 0:
		changePlayerStatus(status, 0, false);
		break;
	case 1:
	case 2:
		setAnimation(108, 1.0f);
		break;
	}

	setAnimation(anmId, 1.0f);
	return;
}

// jumpSlipEvents - 0x80138114
BOOL TMario::jumpSlipEvents(TMario::JumpSlipRecord* record)
{
	if (mInput & 0x10) {
		changePlayerStatus(record->mStatus, 0, false);
		return 1;
	}

	mActionTimer++;
	if (mActionTimer >= record->mTimer) {
		changePlayerStatus(record->mStatus, 0, false);
		return 1;
	}

	u32 input = mInput;
	if (input & 0x2) {
		u32 jumpStatus = record->mJumpStatus;
		if ((jumpStatus - 0x02000000) == 0x0881) {
			if (mForwardVel >= mJumpParams.mSecJumpEnableSp.get()) {
				changePlayerJumping(0x02000881, 0);
				return 1;
			}
		}
		if (jumpStatus == 0x0882) {
			if (mForwardVel >= mJumpParams.mTriJumpEnableSp.get()) {
				changePlayerJumping(0x0882, 0);
				return 1;
			}
		}
		changePlayerJumping(0x02000880, 0);
		return 1;
	} else if (input & 0x4000) {
		mVel.y = 0.0f;
		changePlayerStatus(0x0080088A, 1, false);
		return 1;
	} else if (input & 0x4) {
		changePlayerStatus(record->mFallbackStatus, 0, false);
		return 1;
	}
	return 0;
}

// moveMain - 0x80138000
BOOL TMario::moveMain()
{
	static JumpSlipRecord sRecords[] = {
		{16, 0, 0x0C000230, 0x02000881, 0x0000088C, 0x50},
		{16, 0, 0x0C000232, 0x02000881, 0x0000088C, 0x50},
		{16, 0, 0x0C000231, 0x00000882, 0x0000088C, 0x50},
		{16, 0, 0x0C000233, 0x02000880, 0x0000088C, 0x50},
		{4, 0, 0x0800023A, 0x02000880, 0x0000088C, 0x50},
		{24, 0, 0x0800023B, 0x00000888, 0x0000088C, 0x50},
	};

	BOOL result = 0;
	checkEnforceJump();
	checkController(nullptr);

	u32 action = mAction;
	u8 flag;
	if (action & 0x40000)
		flag = 1;
	else
		flag = 0;
	if (!flag) {
		if (!(action & 0x4045C)) {
			if (!(action & 0x84045D)) {
				if (gpMSound->gateCheck(0x1009)) {
					MSoundSESystem::MSoundSE::startSoundActor(
					    0x1009, (Vec*)&mPosition, 0, nullptr, 0, 4);
				}
			}
		}
	}

	switch (mAction) {
	case 0x04000440:
		running();
		result = 0;
		break;
	case 0x0441:
	case 0x0442:
		rotating();
		result = 0;
		break;
	case 0x0443:
		turnning();
		result = 0;
		break;
	case 0x0444:
		turnEnd();
		result = 0;
		break;
	case 0x04000445: {
		if (!(mInput & 0x10) && (mInput & 0xF)) {
			result = checkAllMotions();
			break;
		}
		int stopped = 0;
		mForwardVel = FConverge(mForwardVel, 0.0f, 4.0f, 4.0f);
		if (mForwardVel == 0.0f)
			stopped = 1;
		slopeProcess();
		if (stopped) {
			changePlayerStatus(0x0C00023D, 0, false);
		} else {
			int wp = walkProcess();
			switch (wp) {
			case 0:
				changePlayerStatus(0x088C, 0, false);
				break;
			case 2:
				if (mForwardVel > 16.0f) {
					playerRefrection(1);
					changePlayerDropping(0x00020462, 0);
				} else {
					setPlayerVelocity(0.0f);
					changePlayerStatus(0x0C00023D, 0, false);
				}
				break;
			}
		}
		setAnimation(15, 1.0f);
		result = 0;
		break;
	}
	case 0x00810446:
		surfing();
		result = 0;
		break;
	case 0x00800447:
		soundTorocco();
		toroccoEffect();
		result = 0;
		break;
	case 0x0400044A:
		walkEnd();
		result = 0;
		break;
	case 0x044C: {
		s16 savedAngle = mFaceAngle.y;
		f32 stopNormal = getSlideStopNormal();
		if (doSliding(stopNormal)) {
			changePlayerStatus(0x0C00023E, 0, false);
		} else {
			slippingBasic(0x0C00023E, 0x0200088E, 15);
			mFaceAngle.y = savedAngle;
			result = 0;
		}
		break;
	}
	case 0x00020449:
		fireDashing();
		result = 0;
		break;
	case 0x00840452:
		slipForeCommon(0x0C00023E, 0x02000880, 0x0200088E, 145);
		result = 0;
		break;
	case 0x00840453:
		slipBackCommon(902, 0x088C, 137);
		result = 0;
		break;
	case 0x00800456:
		catching();
		result = 0;
		break;
	case 0x04808459: {
		setNormalAttackArea();
		if (mInput & 0x8) {
			changePlayerStatus(0x00840452, 0, false);
		} else if (mInput & 0x2) {
			changePlayerJumping(0x02000880, 0);
		} else if (mInput & 0x10) {
			changePlayerStatus(0x04000445, 0, false);
		} else {
			slipForeCommon(0x0C008220, 0x02000880, 0x088C, 151);
		}
		result = 0;
		break;
	}
	case 0x0004045C:
		oilRun();
		result = 0;
		break;
	case 0x0084045D:
		oilSlip();
		result = 0;
		break;
	case 0x0004045E: {
		s16 timer = *(s16*)((u8*)this + 0x13C);
		*(s16*)((u8*)this + 0x13C) = timer - 1;
		if (*(s16*)((u8*)this + 0x13C) <= 0) {
			*(s16*)((u8*)this + 0x13C) = 0;
			*(f32*)((u8*)this + 0x138) = 0.0f;
			changePlayerStatus(0x00800456, 0, false);
		}
		gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z,
		                   mDirtyParams.mPolSizeSlip.value);
		slipBackCommon(902, 0x088C, 137);
		result = 0;
		break;
	}
	case 0x00020460:
		if (mActionTimer == 0) {
			mActionTimer++;
			emitParticle(12);
			rumbleStart(21, mMotorParams.mMotorWall.value);
		}
		downingCommon(1, 86.0f, mActionArg);
		result = 0;
		break;
	case 0x00020461:
		if (mActionTimer == 0) {
			mActionTimer++;
			emitParticle(12);
			rumbleStart(21, mMotorParams.mMotorWall.value);
		}
		downingCommon(44, 42.0f, mActionArg);
		result = 0;
		break;
	case 0x00020462:
		if (mActionTimer == 0) {
			mActionTimer++;
			emitParticle(12);
			rumbleStart(21, mMotorParams.mMotorWall.value);
		}
		downingCommon(123, 88.0f, mActionArg);
		result = 0;
		break;
	case 0x00020463:
		if (mActionTimer == 0) {
			mActionTimer++;
			emitParticle(12);
			rumbleStart(21, mMotorParams.mMotorWall.value);
		}
		downingCommon(124, 80.0f, mActionArg);
		result = 0;
		break;
	case 0x00020464:
		if (mActionTimer == 0) {
			mActionTimer++;
			emitParticle(12);
			rumbleStart(21, mMotorParams.mMotorWall.value);
		}
		downingCommon(116, 200.0f, mActionArg);
		result = 0;
		break;
	case 0x00020465:
		if (mActionTimer == 0) {
			mActionTimer++;
			emitParticle(12);
			rumbleStart(21, mMotorParams.mMotorWall.value);
		}
		downingCommon(117, 100.0f, mActionArg);
		result = 0;
		break;
	case 0x00020466:
		if (mActionTimer == 0) {
			mActionTimer++;
			emitParticle(12);
			rumbleStart(21, mMotorParams.mMotorWall.value);
		}
		downingCommon(138, 128.0f, mActionArg);
		result = 0;
		break;
	case 0x00020467:
		loserDown();
		result = 0;
		break;
	case 0x04000470:
		if (jumpSlipEvents(&sRecords[0])) {
			result = 1;
		} else {
			jumpSlipCommon(78, 0x088C);
			result = 0;
		}
		break;
	case 0x04000471:
		if (jumpSlipEvents(&sRecords[1])) {
			result = 1;
		} else {
			jumpSlipCommon(87, 0x088C);
			result = 0;
		}
		break;
	case 0x04000472:
		if (jumpSlipEvents(&sRecords[2])) {
			result = 1;
		} else {
			jumpSlipCommon(75, 0x088C);
			result = 0;
		}
		break;
	case 0x04000473:
		if (jumpSlipEvents(&sRecords[3])) {
			result = 1;
		} else {
			jumpSlipCommon(190, 0x088C);
			if (result != 2) {
				mFaceAngle.x = 0;
				s16 angle = mModelFaceAngle;
				mModelFaceAngle = angle + 0x8000;
			}
			result = 0;
		}
		break;
	case 0x04000478:
		if (jumpSlipEvents(&sRecords[4])) {
			result = 1;
		} else {
			jumpSlipCommon(192, 0x088C);
			result = 0;
		}
		break;
	case 0x0479: {
		if (!(mInput & 0x4000)) {
			mInput &= ~0x2;
		}
		if (jumpSlipEvents(&sRecords[5])) {
			result = 1;
		} else {
			jumpSlipCommon(152, 0x088C);
			result = 0;
		}
		break;
	}
	default:
		break;
	}

	return result;
}
