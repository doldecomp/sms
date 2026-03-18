#include <Player/MarioMain.hpp>

#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <System/MarDirector.hpp>
#include <Player/Watergun.hpp>
#include <Player/NozzleBase.hpp>
#include <Player/NozzleTrigger.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <System/MarioGamePad.hpp>

// NOTE: -inline deferred means functions must be in REVERSE address order.

// startTalking - 0x80145D44
BOOL TMario::startTalking()
{
	const TBGCheckData* ground = mGroundPlane;
	u8 isTalking;
	if (ground->mFlags & 0x10) {
		isTalking = 1;
	} else {
		isTalking = 0;
	}

	u8 notTalking;
	if (isTalking == 1) {
		notTalking = 0;
	} else {
		notTalking = 1;
	}

	if (notTalking) {
		f32 height = 1.0f + mFloorPosition.y;
		mPosition.y = height;
		changePlayerStatus(0x10001308, 0, true);
		return 1;
	}
	return 0;
}

// canSleep - 0x80145B88
BOOL TMario::canSleep()
{
	u8 hasFlags;
	if (mState & 0x00030000) {
		hasFlags = 1;
	} else {
		hasFlags = 0;
	}
	if (hasFlags)
		return 0;

	f32 sleepCheckDist = mDeParams.mSleepingCheckDist.value;
	f32 sleepCheckTol = mDeParams.mSleepingCheckHeight.value;
	const TBGCheckData* bgData;

	f32 groundY = gpMap->checkGround(
		mPosition.x - sleepCheckDist, 10.0f + mPosition.y, mPosition.z, &bgData);
	f32 floorY = mFloorPosition.y;
	if (groundY < floorY - sleepCheckTol || floorY + sleepCheckTol < groundY)
		return 0;

	groundY = gpMap->checkGround(
		mPosition.x + sleepCheckDist, 10.0f + mPosition.y, mPosition.z, &bgData);
	floorY = mFloorPosition.y;
	if (groundY < floorY - sleepCheckTol || floorY + sleepCheckTol < groundY)
		return 0;

	groundY = gpMap->checkGround(
		mPosition.x, 10.0f + mPosition.y, mPosition.z - sleepCheckDist, &bgData);
	floorY = mFloorPosition.y;
	if (groundY < floorY - sleepCheckTol || floorY + sleepCheckTol < groundY)
		return 0;

	groundY = gpMap->checkGround(
		mPosition.x, 10.0f + mPosition.y, mPosition.z + sleepCheckDist, &bgData);
	floorY = mFloorPosition.y;
	if (groundY < floorY - sleepCheckTol || floorY + sleepCheckTol < groundY)
		return 0;

	if (gpMap->isTouchedOneWall(mPosition.x, 10.0f + mPosition.y, mPosition.z, 80.0f))
		return 0;

	return 1;
}

// canPut - 0x80145AC8
BOOL TMario::canPut()
{
	u16 faceAngle = mFaceAngle.y;
	TTakeActor* heldObj = mHeldObject;
	s32 idx = (s32)faceAngle >> jmaSinShift;
	f32 sinVal = jmaSinTable[idx];
	f32 cosVal = jmaCosTable[idx];
	f32 x = 100.0f * sinVal + mPosition.x;
	f32 z = 100.0f * cosVal + mPosition.z;
	f32 y = 10.0f + mPosition.y;
	f32 radius = heldObj->mDamageRadius;

	if (gpMap->isTouchedOneWall(x, y, z, radius))
		return 0;

	f32 y2 = 10.0f + mPosition.y;
	f32 radius2 = mHeldObject->mDamageRadius;

	if (gpMap->isTouchedOneWall(mPosition.x, y2, mPosition.z, radius2))
		return 0;

	return 1;
}

// waitingCommonEvents - 0x801458A8
BOOL TMario::waitingCommonEvents()
{
	u32 input = mInput;

	if (input & 0x02) {
		if (considerRotateJumpStart())
			return 1;
		changePlayerJumping(0x02000880, 0);
		return 1;
	}

	if (input & 0x04) {
		changePlayerStatus(0x088C, 0, false);
		return 1;
	}

	if (input & 0x08) {
		changePlayerStatus(0x50, 0, false);
		return 1;
	}

	if (input & 0x10) {
		changePlayerStatus(0x0C000227, 0, false);
		return 1;
	}

	if (input & 0x01) {
		s16 faceY = mFaceAngle.y;
		s16 intendedYaw = mIntendedYaw;
		s16 turnSpeed = mDeParams.mWaitingRotSp.value;
		s16 diff = (s16)(intendedYaw - faceY);
		s16 converged = IConverge((int)diff, 0, (int)turnSpeed, (int)turnSpeed);
		mFaceAngle.y = (s16)(intendedYaw - converged);

		if (mIntendedMag > mControllerParams.mStartToWalkLevel.value) {
			emitSmoke(mFaceAngle.y);
			changePlayerStatus(0x04000440, 0, false);
			return 1;
		}
	}

	u8 hasJumpInput;
	if (mState & 0x00004000) {
		hasJumpInput = 1;
	} else {
		hasJumpInput = 0;
	}
	if (hasJumpInput) {
		changePlayerStatus(0x04000440, 0, false);
		return 1;
	}

	if (canSquat()) {
		mForwardVel = 0.0f;
		changePlayerStatus(0x0C018220, 0, false);
		return 1;
	}

	if (mInput & 0x00008000) {
		changePlayerStatus(0x384, 0, false);
		return 1;
	}

	if (rocketCheck()) {
		TWaterGun* gun = mWaterGun;
		f32 rocketHeight = *(f32*)((u8*)gun + 0x1D40);
		mHolderHeightDiff = mFloorPosition.y + rocketHeight;
		changePlayerStatus(0x088B, 0, false);
		return 1;
	}

	if (considerRotateStart())
		return 1;

	return 0;
}

// stopCommon - 0x801457EC
void TMario::stopCommon(int animId, int nextState)
{
	waitProcess();
	setAnimation(animId, 1.0f);

	if (onYoshi()) {
		MActor* actor = *(MActor**)((u8*)mYoshi + 0x34);
		if (actor->curAnmEndsNext(0, 0)) {
			changePlayerStatus(nextState, 0, false);
			return;
		}
	} else {
		if (isLast1AnimeFrame()) {
			changePlayerStatus(nextState, 0, false);
		}
	}
}

// changeMontemanWaitingAnim - 0x801457CC
void TMario::changeMontemanWaitingAnim()
{
	if (mAction != 0x0C400201)
		return;
	mActionState |= 0x02;
}

// waiting - 0x8014552C
BOOL TMario::waiting()
{
	if (waitingCommonEvents())
		return 1;

	if (isMario()) {
		u8 isPumpFive;
		if (mPumpState == 5) {
			isPumpFive = 1;
		} else {
			isPumpFive = 0;
		}
		if (isPumpFive) {
			if (canSleep()) {
				if (mAnimationId == 0xC3) {
					if (isAnimeLoopOrStop()) {
						if (mGroundPlane != nullptr) {
							if (mGroundPlane->mNormal.y > 0.99f) {
								mActionTimer++;
								if (mActionTimer >= 10) {
									changePlayerStatus(0x0C400202, 0, false);
									return 1;
								}
							}
						}
					}
				}
			}
		}
	}

	u16 actionState = mActionState;

	if (actionState & 0x02) {
		setAnimation(0x114, 1.0f);
	} else if (gpMarDirector->unk124 == 3) {
		setAnimation(0xD9, 1.0f);
	} else {
		f32 val368 = *(f32*)((u8*)this + 0x368);
		int isPositive;
		if (val368 > 0.0f) {
			isPositive = 1;
		} else {
			isPositive = 0;
		}

		if (isPositive != 0) {
			setAnimation(0xE7, 1.0f);
		} else if (mPumpState == 5) {
			if (mPrevAction - 0x0C00023D == 0) {
				// fall through to montemanWait
			} else {
				u8 hasFlag;
				if (mState & 0x00000040) {
					hasFlag = 1;
				} else {
					hasFlag = 0;
				}
				if (!hasFlag) {
					goto regularWait;
				}
			}

			// montemanWait
			if (!(actionState & 0x01)) {
				setAnimation(0xDA, 1.0f);

				J3DFrameCtrl* frameCtrl = mModel->unkC;
				if (frameCtrl->checkPass(138.0f)) {
					emitSweat((s16)(mFaceAngle.y - 0x4000));
				}

				if (isLast1AnimeFrame()) {
					mActionState |= 0x01;
				}
				goto doWaitProcess;
			}

			goto regularWait;
		} else {
regularWait:
			if (mHealth <= 3) {
				if (mAnimationId != 0x11D && mAnimationId != 0x127) {
					setAnimation(0x127, 1.0f);
				} else if (mAnimationId == 0x127) {
					if (isLast1AnimeFrame()) {
						setAnimation(0x11D, 1.0f);
					}
				}
			} else {
				if (0.0f == mIntendedMag) {
					setAnimation(0xC3, 1.0f);
				} else {
					setAnimation(0x12C, 1.0f);
				}
			}
		}
	}

doWaitProcess:
	waitProcess();
	return 0;
}

// sleepily - 0x8014540C
BOOL TMario::sleepily()
{
	if (waitingCommonEvents())
		return 1;

	{
		u32* ctrlWork = (u32*)unk108;
		f32 stickX = *(f32*)((u8*)ctrlWork + 0x1c);
		f32 stickY = *(f32*)((u8*)ctrlWork + 0x20);
		if (stickX > 0.0f || stickY > 0.0f) {
			changePlayerStatus(0x0C400201, 0, false);
		}
	}

	if (mActionState == 3) {
		changePlayerStatus(0x0C000203, 0, false);
		return 1;
	}

	switch (mActionState) {
	case 0:
		setAnimation(0x12F, 1.0f);
		break;
	case 1:
		setAnimation(0x130, 1.0f);
		break;
	case 2:
		setAnimation(0x131, 1.0f);
		break;
	}

	if (isLast1AnimeFrame()) {
		mActionState++;
	}

	waitProcess();
	return 0;
}

// sleeping - 0x80145278
BOOL TMario::sleeping()
{
	u32 input = mInput;

	if (!(input & 0xA41F)) {
		u32* ctrlWork = (u32*)unk108;
		f32 stickX = *(f32*)((u8*)ctrlWork + 0x1c);
		f32 stickY = *(f32*)((u8*)ctrlWork + 0x20);
		if (stickX <= 0.0f && stickY <= 0.0f) {
			goto continueSleep;
		}
	}

	// wakeUp
	if (mActionState == 0) {
		startSoundActor(0x7883);
	} else {
		startSoundActor(0x789A);
	}
	changePlayerStatus(0x0C000204, mActionState, false);
	return 1;

continueSleep:
	waitProcess();

	{
		u8 hasFlag;
		if (mSubState & 0x02) {
			hasFlag = 1;
		} else {
			hasFlag = 0;
		}
		if (hasFlag) {
			*(u32*)((u8*)this + 0x1B4) = *(u32*)&mPosition.x;
			*(u32*)((u8*)this + 0x1B8) = *(u32*)&mPosition.y;
			*(u32*)((u8*)this + 0x1BC) = *(u32*)&mPosition.z;
			sleepingEffect();
		}
	}

	switch (mActionState) {
	case 0:
		setAnimation(0x132, 1.0f);
		if (isLast1AnimeFrame()) {
			u16 timer = mActionTimer;
			timer++;
			mActionTimer = timer;
			if ((u16)timer > 40) {
				mActionState++;
			}
		}
		break;
	case 1:
		setAnimation(0x134, 1.0f);
		if (isLast1AnimeFrame()) {
			mActionState++;
		}
		break;
	case 2:
		setAnimation(0x135, 1.0f);
		break;
	}

	return 0;
}

// getSideWalkValues - 0x801451A8
void TMario::getSideWalkValues(E_SIDEWALK_TYPE* outType, f32* outSpeed, f32* outStickMag)
{
	s16 faceY = mFaceAngle.y;
	s16 intendedYaw = mIntendedYaw;
	s16 diff = (s16)(intendedYaw - faceY);
	s16 diffExt = diff;
	u16 diffU = (u16)diffExt;
	s32 idx = (s32)diffU >> jmaSinShift;
	f32 sinVal = jmaSinTable[idx];
	f32 sideComponent = mIntendedMag * sinVal;
	f32 ff8 = mRunParams.mPumpingSlideSp.value;
	f32 sidewalkVel = sideComponent * ff8;

	if (0.0f == sidewalkVel) {
		*outType = (E_SIDEWALK_TYPE)0;
		*outSpeed = getMotionFrameCtrl().getRate();
	} else {
		f32 f100c = mRunParams.mPumpingSlideAnmSp.value;
		f32 speed = sidewalkVel * f100c;
		if (speed < 0.0f)
			speed = -speed;
		*outSpeed = speed;

		if (diffExt > 0) {
			*outType = (E_SIDEWALK_TYPE)1;
		} else {
			*outType = (E_SIDEWALK_TYPE)2;
		}
	}

	*outStickMag = sidewalkVel;
}

// squating - 0x80144DEC
BOOL TMario::squating()
{
	u32 input = mInput;

	if (input & 0x04) {
		changePlayerStatus(0x088C, 0, false);
		return 1;
	}

	if (input & 0x08) {
		changePlayerStatus(0x50, 0, false);
		return 1;
	}

	if (input & 0x10) {
		changePlayerStatus(0x0C018222, 0, false);
		return 1;
	}

	if (!(input & 0x4000) && !(input & 0x200)) {
		changePlayerStatus(0x0C018222, 0, false);
		return 1;
	}

	TWaterGun* gun = mWaterGun;
	if (gun == nullptr) {
		changePlayerStatus(0x0C018222, 0, false);
		return 1;
	}

	{
		u8 hasFlag;
		if (mState & 0x00008000) {
			hasFlag = 1;
		} else {
			hasFlag = 0;
		}
		if (!hasFlag) {
			changePlayerStatus(0x0C018222, 0, false);
			return 1;
		}
	}

	if (input & 0x02) {
		TMarioGamePad* pad = mGamePad;
		if (pad->mMeaning & 0x2000) {
			if (gun != nullptr) {
				if (*(u8*)((u8*)gun + 0x1C84) == 0) {
					rumbleStart(21, mMotorParams.mMotorHipDrop.value);
					changePlayerStatus(0x0883, 0, false);
					return 1;
				}
			}
		}
	}

	{
		TNozzleBase* nozzle = gun->getCurrentNozzle();
		u8 nozzleKind = *(u8*)((u8*)nozzle + 0x18);
		if (nozzleKind == 1) {
			TWaterGun* gun2 = mWaterGun;
			u8 canSpray;
			if (gun2->mCurrentWater == 0) {
				canSpray = 0;
			} else {
				TNozzleBase* nozzle2 = gun2->getCurrentNozzle();
				s32 kind = nozzle2->getNozzleKind();
				if (kind == 1) {
					TNozzleTrigger* trigger = (TNozzleTrigger*)gun2->getCurrentNozzle();
					if (trigger->unk385 == 1) {
						canSpray = 1;
					} else {
						canSpray = 0;
					}
				} else {
					if (gun2->getCurrentNozzle()->unk378 > 0.0f) {
						canSpray = 1;
					} else {
						canSpray = 0;
					}
				}
			}

			if (canSpray) {
				TWaterGun* gun3 = mWaterGun;
				f32 rocketHeight = *(f32*)((u8*)gun3 + 0x1D40);
				mHolderHeightDiff = mFloorPosition.y + rocketHeight;
				changePlayerStatus(0x088B, 0, false);
				return 1;
			}
		}
	}

	{
		TMarioGamePad* pad = mGamePad;
		u32 meaning = pad->mMeaning;

		if (meaning & 0x2000) {
			E_SIDEWALK_TYPE sideType;
			f32 sideSpeed;
			f32 stickMag;
			getSideWalkValues(&sideType, &sideSpeed, &stickMag);

			switch (sideType) {
			case 0:
				setAnimation(0x98, 1.0f);
				break;
			case 1:
				setAnimation(0x7F, sideSpeed);
				break;
			case 2:
				setAnimation(0x80, sideSpeed);
				break;
			}

			u16 faceAngle = mFaceAngle.y;
			s32 cosIdx = (s32)faceAngle >> jmaSinShift;
			f32 cosVal = jmaCosTable[cosIdx << 2 >> 2];
			mPosition.x = stickMag * cosVal + mPosition.x;

			faceAngle = mFaceAngle.y;
			s32 sinIdx = (s32)faceAngle >> jmaSinShift;
			f32 sinVal = jmaSinTable[sinIdx << 2 >> 2];
			mPosition.z = -(stickMag * sinVal) + mPosition.z;
		} else if (meaning & 0x0800) {
			f32 analogStick = *(f32*)((u8*)pad + 0xA8);
			int isPositive = 1;
			f32 absVal = __fabsf(analogStick);
			if (analogStick < 0.0f)
				isPositive = 0;

			f32 threshold = mControllerParams.mSquatRotMidAnalog.value;
			f32 maxSpeed = mControllerParams.mSquatRotMidValue.value;
			f32 turnSpeed;

			if (absVal < threshold) {
				turnSpeed = maxSpeed * (absVal / threshold);
			} else {
				f32 range = 1.0f - threshold;
				f32 excess = absVal - threshold;
				f32 speedRange = 1.0f - maxSpeed;
				turnSpeed = speedRange * (excess / range) + maxSpeed;
			}

			if (!isPositive)
				turnSpeed = -turnSpeed;

			s16 maxTurnRate = mDeParams.mWaitingRotSp.value;
			s32 negRate = -maxTurnRate;
			s16 faceY = mFaceAngle.y;
			s16 angleDelta = (s16)(s32)(turnSpeed * (f64)negRate);
			mFaceAngle.y = (s16)(faceY + angleDelta);

			setAnimation(0x98, 1.0f);
		}
	}

	waitProcess();
	return 0;
}

// squatStandup - 0x80144CD0
BOOL TMario::squatStandup()
{
	u32 input = mInput;

	if (input & 0x04) {
		changePlayerStatus(0x088C, 0, false);
		return 1;
	}

	if (input & 0x08) {
		changePlayerStatus(0x50, 0, false);
		return 1;
	}

	if (input & 0x02) {
		changePlayerStatus(0x02000880, 0, false);
		return 1;
	}

	if (input & 0x01) {
		changePlayerStatus(0x04000440, 0, false);
		return 1;
	}

	waitProcess();

	if (mAction - 0x0C000223 == 0) {
		setAnimation(0x121, 1.0f);
	} else {
		setAnimation(0x96, 1.0f);
	}

	if (isLast1AnimeFrame()) {
		changePlayerStatus(0x0C400201, 0, false);
	}

	return 0;
}

// jumpEndCommon - 0x80144C50
BOOL TMario::jumpEndCommon(int animId, int nextState)
{
	waitProcess();
	setAnimation(animId, 1.0f);

	if (isLast1AnimeFrame()) {
		return changePlayerStatus(nextState, 0, false);
	}
	return 0;
}

// jumpEndEvents - 0x80144BD8
BOOL TMario::jumpEndEvents(u32 nextState)
{
	u32 input = mInput;

	if (input & 0x10) {
		changePlayerStatus(0x0C400201, 0, false);
		return 1;
	}

	if (input & 0x02) {
		if (nextState == 0) {
			changePlayerTriJump();
		} else {
			changePlayerJumping(nextState, 0);
		}
		return 1;
	}

	if (input & 0x0F) {
		checkAllMotions();
		return 1;
	}

	return 0;
}

// waitMain - 0x80144300
BOOL TMario::waitMain()
{
	BOOL result = 0;

	checkEnforceJump();

	checkController(nullptr);

	setNormalAttackArea();

	// Check held object for wall collision
	TTakeActor* heldObj = mHeldObject;
	if (heldObj != nullptr) {
		u8 hasInput;
		if (mInput & 0x2000) {
			hasInput = 1;
		} else {
			hasInput = 0;
		}
		if (hasInput) {
			if (heldObj->mActorType == (u32)0x80000001) {
				changePlayerStatus(0x80000588, 0, false);
			} else {
				int putResult = canPut();
				if (putResult) {
					changePlayerStatus(0x80000387, 0, false);
				}
			}
		}
	}

	u32 action = mAction;

	if (action == 0x0C400201) {
		result = waiting();
	} else if (action == 0x0C400203) {
		result = sleepily();
	} else if (action == 0x0C000203) {
		result = sleeping();
	} else if (action == 0x0C000204) {
		// Wakeup
		u32 input = mInput;
		if (input & 0x04) {
			sleepingEffectKill();
			changePlayerStatus(0x088C, 0, false);
		} else if (input & 0x08) {
			sleepingEffectKill();
			changePlayerStatus(0x50, 0, false);
		} else if (waitingCommonEvents()) {
			sleepingEffectKill();
			result = 1;
			goto end;
		} else {
			waitProcess();
			int animId;
			if (mActionArg == 0) {
				animId = 0x133;
			} else {
				animId = 0x136;
			}
			setAnimation(animId, 1.0f);
			if (isLast1AnimeFrame()) {
				sleepingEffectKill();
				changePlayerStatus(0x0C400201, 0, false);
			} else {
				result = 0;
			}
		}
		result = result;
	} else if (action == 0x0C018220) {
		result = squating();
	} else if (action == 0x0C018222 || action == 0x0C000223) {
		result = squatStandup();
	} else if (action >= 0x0C00022F && action <= 0x0C00022F) {
		// action == 0x0C00022F: squat landing
		u32 input = mInput;
		if (input & 0x04) {
			sleepingEffectKill();
			changePlayerStatus(0x088C, 0, false);
		} else if (input & 0x08) {
			sleepingEffectKill();
			changePlayerStatus(0x50, 0, false);
		} else {
			waitProcess();
			setAnimation(0xF3, 1.0f);
			if (isLast1AnimeFrame()) {
				changePlayerStatus(0x0C400201, 0, false);
			}
			result = 0;
		}
		result = result;
	} else if (action == 0x0C000230) {
		// jumpEnd - landing type 1
		if (jumpEndEvents(0)) {
			result = 1;
		} else {
			waitProcess();
			setAnimation(0x4E, 1.0f);
			if (isLast1AnimeFrame()) {
				changePlayerStatus(0x0C400201, 0, false);
			}
			result = 0;
		}
	} else if (action == 0x0C000232) {
		// jumpEnd - landing type 2
		if (jumpEndEvents(0)) {
			result = 1;
		} else {
			waitProcess();
			setAnimation(0x4B, 1.0f);
			if (isLast1AnimeFrame()) {
				changePlayerStatus(0x0C400201, 0, false);
			}
			result = 0;
		}
	} else if (action == 0x0C000233) {
		// jumpEnd - landing type 3 (broadjump/fire)
		if (jumpEndEvents(0)) {
			result = 1;
		} else {
			waitProcess();
			setAnimation(0x57, 1.0f);
			if (isLast1AnimeFrame()) {
				changePlayerStatus(0x0C400201, 0, false);
			}
			result = 0;
		}
	} else if (action == 0x0C000233) {
		// Duplicate - skip
	} else if (action == 0x0C400202) {
		// sleepily transition?
		result = 0;
	} else if (action == 0x0C000233 + 0) {
		// fire jump end
		if (jumpEndEvents(0)) {
			result = 1;
		} else {
			waitProcess();
			setAnimation(0xBE, 1.0f);
			if (isLast1AnimeFrame()) {
				changePlayerStatus(0x0C400201, 0, false);
			}
			// Reset stickLate and update modelFaceAngle
			result = 0;
		}
	} else if (action == 0x80000A36) {
		// throw end
		checkThrowObject();
		jumpEndCommon(0x65, 0x0C400201);
		result = 0;
	} else if (action == 0x08000239) {
		// pullEnd
		mInput &= ~0x2010;
		if (jumpEndEvents(0)) {
			result = 1;
		} else {
			waitProcess();
			setAnimation(0x28, 1.0f);
			if (isLast1AnimeFrame()) {
				changePlayerStatus(0x0C400201, 0, false);
			}
			result = 0;
		}
	} else if (action == 0x0800023B) {
		// uTurnJumpEnd
		mInput &= ~0x2000;
		if (jumpEndEvents(0x02000880)) {
			if (mAction - 0x04000440 == 0) {
				changePlayerStatus(0x0C018222, 0, false);
			} else {
				result = 1;
			}
		} else {
			waitProcess();
			setAnimation(0x98, 1.0f);
			if (isLast1AnimeFrame()) {
				changePlayerStatus(0x0C018222, 0, false);
			}
			result = 0;
		}
	} else if (action == 0x0080023C) {
		// broadJumpEnd
		mActionState = 1;
		u32 input = mInput;
		if (input & 0x04) {
			changePlayerStatus(0x088C, 0, false);
		} else if (input & 0x08) {
			changePlayerStatus(0x00840452, 0, false);
		} else {
			waitProcess();
			setAnimation(0x3A, 1.0f);
			if (isLast1AnimeFrame()) {
				changePlayerStatus(0x0C00023E, 0, false);
			}
			result = 0;
		}
		result = result;
	} else if (action == 0x0C00023D) {
		// hipAttackEnd
		u32 input = mInput;
		if (!(input & 0x10) && (input & 0x0F)) {
			checkAllMotions();
		} else {
			stopCommon(0x10, 0x0C400201);
			result = 0;
		}
		result = result;
	} else if (action == 0x0C00023F) {
		// slipEnd
		u32 input = mInput;
		if (input & 0x0F) {
			checkAllMotions();
		} else {
			stopCommon(0x8F, 0x0C400201);
			result = 0;
		}
		result = result;
	} else {
		result = 0;
	}

end:
	return result;
}
