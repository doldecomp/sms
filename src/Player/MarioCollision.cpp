#include <Player/Mario.hpp>

#include <M3DUtil/M3UModelMario.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MSound/MAnmSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <System/Particles.hpp>
#include <System/MarioGamePad.hpp>
#include <Player/WaterGun.hpp>
#include <Camera/CameraShake.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMario::rumbleStart(int channelDataIdx, int repeatCount)
{
	if (this == gpMarioOriginal) {
		SMSRumbleMgr->start(channelDataIdx, repeatCount, (f32*)nullptr);
	}
}

void TMario::incHP(int hp)
{
	// volatile u32 padding[10];
	if (isUnderWater() || checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)) {
		unk12C += hp;
		if (unk12C > unk130) {
			unk12C = unk130;
		} else {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_HP_RECOVER, 0, nullptr,
			                                   0);
		}
		return;
	}

	mHealth += hp;
	if (mHealth > mDeParams.mHpMax.get()) {
		mHealth = mDeParams.mHpMax.get();
	} else {
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_HP_RECOVER, 0, nullptr, 0);
	}
}

void TMario::decHP(int hp)
{
	// volatile u32 padding[2];
	if (isUnderWater() || checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)) {
		unk12C -= hp;

		for (int i = 0; i < 10; ++i) {
			bubbleFromMouth(i);
		}

		if (unk12C < 1.0f) {
			loserExec();
			changePlayerStatus(MARIO_STATUS_SWIM_DOWN, 0, true);
		}
		return;
	}

	mHealth -= hp;
	if (0 >= mHealth) {
		mHealth = 0;
		loserExec();
	}
}

s16 TMario::getAttackAngle(const THitActor* other)
{
	f32 dx = other->mPosition.x - mPosition.x;
	f32 dz = other->mPosition.z - mPosition.z;
	return matan(dz, dx);
}

void TMario::dropObject()
{
	if (mHeldObject != nullptr) {
		mHeldObject->receiveMessage(this, HIT_MESSAGE_UNK7);
		mHeldObject = nullptr;
	}
}

bool TMario::isTakeSituation(THitActor* object)
{
	if (unk14C > 0)
		return false;

	if (checkStatusFlag(MARIO_STATUS_FLAG_UNK80000000))
		return false; // Airborn?

	if (checkStatusFlag(MARIO_STATUS_FLAG_UNK10000000))
		return false;

	if (mStatus == MARIO_STATUS_CATCH)
		return false;

	if (onYoshi())
		return false;

	if (!isUpperState(UPPER_STATE_IDLE))
		return false;

	if (mHeldObject != nullptr)
		return false;

	if (mHolder != nullptr)
		return false;

	s16 attackAngle = getAttackAngle(object) - mFaceAngle.y;
	if (attackAngle <= -0x2aaa) {
		return false;
	}

	if (attackAngle >= 0x2aaa) {
		return false;
	}

	f32 dist = JGeometry::TVec3<f32>(object->mPosition - mPosition).length();
	if (dist > mAttackRadius + object->getDamageRadius()) {
		return false;
	}

	return true;
}

bool TMario::canTake(THitActor* object)
{
	return mGamePad->checkMeaning(TMarioGamePad::MEANING_0x100)
	       && isTakeSituation(object);
}

BOOL TMario::trampleExec(THitActor* param_1)
{
	if (!checkStatusFlag(MARIO_STATUS_FLAG_JUMPING))
		return false;

	if (mStatus == MARIO_STATUS_DIVE)
		return false;

	if (param_1->receiveMessage(this, HIT_MESSAGE_TRAMPLE) == FALSE)
		return false;

	if (mStatus == MARIO_STATUS_BROAD_JUMP) {
		changePlayerStatus(MARIO_STATUS_BACK_JUMP, 0, false);
	} else {
		switch (mAnimationId) {
		case ANIM_STEP1:
			setAnimation(ANIM_STEP2, 1.0f);
			break;
		case ANIM_STEP2:
		case ANIM_STEP3:
			setAnimation(ANIM_STEP3, 1.0f);
			mModel->getFrameCtrl(0).setFrame(0.0f);
			break;
		default:
			setAnimation(ANIM_STEP1, 1.0f);
			break;
		}
		changePlayerStatus(MARIO_STATUS_TRAMPLE, 0, false);
		setStatusToJumping(MARIO_STATUS_TRAMPLE, 0);
	}

	rumbleStart(0x15, mMotorParams.mMotorTrample.get());

	if (param_1->checkActorType(ACTOR_TYPE_UNK4000000)) {
		JGeometry::TVec3<f32> scale(0.5f);
		SMS_EasyEmitParticle(PARTICLE_MS_FUMI_B, &mPosition, this, scale);
		SMS_EasyEmitParticle(PARTICLE_MS_FUMI_C, &mPosition, this, scale);
	} else {
		emitParticle(7);
		emitParticle(8);
		emitParticle(9);
	}

	unk78 &= ~0x100;
	mModel->getFrameCtrl(0).setAttribute(0);

	if (!param_1->isActorType(0x20000008)
	    && !param_1->isActorType(0x2000000a)) {
		u32 trampleCt = getTrampleCt();
		// Probably an inline
		if (SMSGetMSound()->gateCheck(MSD_SE_MA_KICK_ENEMY)) {
			MSoundSESystem::MSoundSE::startSoundActorWithInfo(
			    MSD_SE_MA_KICK_ENEMY, &mPosition, nullptr, 0.0f, trampleCt, 0,
			    nullptr, 0, 4);
		}
	}
	return true;
}

void TMario::resetNozzle() { }

void TMario::normalizeNozzle()
{
	// volatile u32 padding[2];
	if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
		mWaterGun->changeNozzle(TWaterGun::Spray, true);
		unk144 = -1;
		unk148 = 0;
	}
}

void TMario::loserExec()
{
	// volatile u32 padding[2];
	if (mStatus != MARIO_STATUS_SWIM_DOWN && mStatus != MARIO_STATUS_ELEC_DOWN
	    && mStatus != MARIO_STATUS_SWIM_P_DOWN
	    && mStatus != MARIO_STATUS_DOWN_LOSER) {
		onFlag(MARIO_FLAG_GAME_OVER);
		mHealth = 0;

		gpMSound->startSoundSystemSE(MSD_SE_SY_HP_DOWN, 0, nullptr, 0);

		mAnmSound->stop();

		if (mYoshi != nullptr) {
			mYoshi->kill();
		}

		if (checkStatusFlag(MARIO_STATUS_FLAG_SWIMMING)) {
			if (unk12C < 1.0f) {
				changePlayerStatus(MARIO_STATUS_SWIM_DOWN, 0, true);
			} else {
				changePlayerStatus(MARIO_STATUS_SWIM_P_DOWN, 0, true);
			}
			return;
		}
		if (mStatus == MARIO_STATUS_ELECTRIC_DAMAGE) {
			changePlayerStatus(MARIO_STATUS_ELEC_DOWN, 0, true);
		} else {
			changePlayerStatus(MARIO_STATUS_DOWN_LOSER, 0, true);
		}
	}
}

void TMario::floorDamageExec(int damage, int damageAnimType, int waterEmit,
                             int rumbleFrames)
{
	if (isInvincible())
		return;

	mFloorHitActor.mPosition.x = mPosition.x + JMASSin(mFaceAngle.y);
	mFloorHitActor.mPosition.z = mPosition.z + JMASCos(mFaceAngle.y);
	damageExec(&mFloorHitActor, damage, damageAnimType, waterEmit, 16.0f,
	           rumbleFrames, 0.0f, 60);
}

void TMario::floorDamageExec(const TMario::TEParams& params)
{
	if (isInvincible())
		return;

	mFloorHitActor.mPosition.x = mPosition.x + JMASSin(mFaceAngle.y);
	mFloorHitActor.mPosition.z = mPosition.z + JMASCos(mFaceAngle.y);
	damageExec(&mFloorHitActor, params.mDamage.get(), params.mDownType.get(),
	           params.mWaterEmit.get(), params.mMinSpeed.get(),
	           params.mMotor.get(), params.mDamage.get(),
	           params.mInvincibleTime.get());
}

// Closest i got, but i think this is wrong, but probably functionally
// equivalent? I kinda suspect they didn't use this many helper functions? The
// double epsilon check confused me
void TMario::calcDamagePos(const JGeometry::TVec3<f32>& pos)
{
	JGeometry::TVec3<f32> offset = pos - mPosition;
	if (offset.isZero()) {
		unk19C = mPosition;
		return;
	}
	offset.normalize();
	unk19C = mPosition + offset * 50.0f;
}

void TMario::damageExec(THitActor* hittingActor, int damage, int damageAnimType,
                        int waterEmit, f32 knockbackSpeed, int rumbleFrames,
                        f32 pollutionAmount, s16 invincibilityFrames)
{
	// volatile u32 padding[10];
	u32 animationTypes[16] = {
		MARIO_STATUS_SAFE_BACK_DOWN,
		MARIO_STATUS_SHORT_BACK_DOWN,
		MARIO_STATUS_BACK_DOWN,
		MARIO_STATUS_WAIT,
		MARIO_STATUS_JUMP_SHORT_BACK_DOWN,
		MARIO_STATUS_JUMP_SHORT_BACK_DOWN,
		MARIO_STATUS_JUMP_BACK_DOWN,
		MARIO_STATUS_WAIT,
		MARIO_STATUS_SAFE_FORE_DOWN,
		MARIO_STATUS_SHORT_FORE_DOWN,
		MARIO_STATUS_FORE_DOWN,
		MARIO_STATUS_WAIT,
		MARIO_STATUS_JUMP_SHORT_FORE_DOWN,
		MARIO_STATUS_JUMP_SHORT_FORE_DOWN,
		MARIO_STATUS_JUMP_FORE_DOWN,
		MARIO_STATUS_WAIT,
	};

	if (isInvincible())
		return;
	if (checkFlag(MARIO_FLAG_GAME_OVER))
		return;
	if (mHealth == 0)
		return;

	if (onYoshi()) {
		getOffYoshi(true);
		// Probably an inline
		if (gpMSound->gateCheck(MSD_SE_YV_DAMAGE)) {
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_YV_DAMAGE, &mPosition, 0, nullptr, 0, 4);
		}
		return;
	}

	u32 animOffset1 = checkStatusFlag(MARIO_STATUS_FLAG_JUMPING) ? 1 : 0;
	if (onYoshi()) {
		animOffset1 = true;
	}

	if (damageAnimType == 3) {
		if (mStatus == MARIO_STATUS_RUN || mStatus == MARIO_STATUS_OIL_RUN) {
			setUpperDamageRun();
		}
	} else {
		s16 attackAngle = getAttackAngle(hittingActor);
		s16 prevAngle   = mFaceAngle.y;
		mFaceAngle.y    = attackAngle;
		s16 angleDiff   = attackAngle - prevAngle;

		if (mForwardVel < knockbackSpeed) {
			setPlayerVelocity(knockbackSpeed);
		}

		u32 animOffset2 = 0;
		if ((angleDiff >= -0x4000) && (angleDiff <= 0x4000)) {
			animOffset2 = 0;
			mForwardVel *= -1.0f;
		} else {
			animOffset2 = 1;
			mFaceAngle.y += 0x8000;
		}

		// Inline?
		bool canPlayAnimation = true;
		if (mStatus == MARIO_STATUS_TOROCCO)
			canPlayAnimation = false;

		if (mStatus == MARIO_STATUS_DIVE)
			canPlayAnimation = false;

		if (checkStatusFlag(MARIO_STATUS_FLAG_SWIMMING))
			canPlayAnimation = true;

		if (canPlayAnimation) {
			// I don't think this is correct, but was the closest i could get
			u32 statusIdx = animationTypes[damageAnimType + animOffset1 * 4
			                               + animOffset2 * 8];
			if (mHolder == nullptr || mHolder->isActorType(0x40000098)) {
				// Knocked from a wire hang by damage?
				changePlayerDropping(MARIO_STATUS_WIRE_HANG_LAND_SAFE_DOWN, 0);
			} else {
				changePlayerDropping(statusIdx, 0);
			}
		}
	}
	unk14C = invincibilityFrames;
	decHP(damage);
	if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
		for (int i = 0; i < waterEmit; ++i) {
			if (mWaterGun->damage()) {
				unk154->mPos.value = mWaterGun->mEmitPos[0];
				gpModelWaterManager->emitRequest(*unk154);
			}
		}
	}

	if (rumbleFrames > 0 && this == gpMarioOriginal) {
		SMSRumbleMgr->start(0x15, rumbleFrames, (f32*)nullptr);
	}

	// I hope this is bcs of inlines
	if (damageAnimType != 3) {
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK2, 1.0f);
	}

	if (damageAnimType != 3) {
		calcDamagePos(hittingActor->mPosition);
		emitParticle(0xb, &unk19C);
		emitParticle(0xc, &unk19C);
		emitParticle(0xa, &unk19C);
	}

	if (mHealth > 0) {
		if (damage > 1) {
			startVoice(MSD_SE_MV01_DAMAGE_BIG_01);
		} else {
			if (damageAnimType == 0) {
				startVoice(MSD_SE_MV03_DAMAGE_LITLE_01);
			} else {
				startVoice(MSD_SE_MV02_DAMAGE_MID_01);
			}
		}
	}

	unk134 += pollutionAmount;
	dirtyLimitCheck();
}

void TMario::considerTake()
{
	// volatile u32 missingStack[6];
	bool check = false;

	if (isUpperState(UPPER_STATE_HOLDING_OBJECT))
		check = true;

	if (isUpperState(UPPER_STATE_UNK3))
		check = true;

	if (mStatus == MARIO_STATUS_TAKE
	    || checkStatusFlag(MARIO_STATUS_FLAG_UNK80000000))
		check = true;

	if (mStatus == MARIO_STATUS_PULLING || mStatus == MARIO_STATUS_PULL_JUMP
	    || mStatus == MARIO_STATUS_OIL_PULLING)
		check = true;

	if (mHeldObject != nullptr && mHeldObject->mHolder != this) {
		mHeldObject = nullptr;
	}

	if (mHolder != nullptr && mHolder->mHeldObject != this)
		mHolder = nullptr;

	if (mHeldObject != nullptr && !check) {
		mHeldObject->receiveMessage(this, HIT_MESSAGE_UNK7);
		mHeldObject->receiveMessage(this, HIT_MESSAGE_UNK8);
		mHeldObject = nullptr;
	}

	if (mHolder != nullptr) {
		// Probably an inline
		BOOL check2 = false;
		u32 test    = mStatus & MARIO_STATUS_TYPE_AND_ID_MASK;
		if ((0x150 <= test && 0x15c >= test) || (0x140 <= test && test <= 0x143)
		    || checkStatusFlag(MARIO_STATUS_FLAG_UNK1000)
		    || mStatus == MARIO_STATUS_TAKEN) {
			check2 = true;
		}

		if (!check2) {
			mHolder->receiveMessage(this, HIT_MESSAGE_UNK8);
			mHolder->receiveMessage(this, HIT_MESSAGE_UNK7);
			mHolder = nullptr;
		}
	}
}
