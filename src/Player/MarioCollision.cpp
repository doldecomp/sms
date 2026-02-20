#include <Player/MarioMain.hpp>

#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MSound/MSoundSE.hpp>
#include <System/MarioGamePad.hpp>
#include <Player/Watergun.hpp>
#include <Camera/CameraShake.hpp>
#include <System/Particles.hpp>

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
			// Probably an inline
			if (gpMSound->gateCheck(0x4801)) {
				MSoundSESystem::MSoundSE::startSoundSystemSE(0x4801, 0, nullptr,
				                                             0);
			}
		}
		return;
	}

	mHealth += hp;
	if (mHealth > mDeParams.mHpMax.get()) {
		mHealth = mDeParams.mHpMax.get();
	} else {
		// Probably an inline
		if (gpMSound->gateCheck(0x4801)) {
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x4801, 0, nullptr, 0);
		}
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
			changePlayerStatus(0x224E0, 0, true);
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
		mHeldObject->receiveMessage(this, 7);
		mHeldObject = nullptr;
	}
}

bool TMario::isTakeSituation(THitActor* object)
{
	if (unk14C > 0) {
		return false;
	}
	// Probably inline
	if (checkActionFlag(0x80000000)) {
		return false; // Airborn?
	}

	// Probably inline
	if (checkActionFlag(0x10000000)) {
		return false;
	}
	if (mAction == 0x800456) {
		return false;
	}

	if (onYoshi()) {
		return false;
	}

	// Probably an inline
	if (!checkUnk380(5)) {
		return false;
	}

	if (mHeldObject != nullptr) {
		return false;
	}

	if (mHolder != nullptr) {
		return false;
	}

	s16 attackAngle = getAttackAngle(object) - mFaceAngle.y;
	if (attackAngle <= -0x2aaa) {
		return false;
	}

	if (attackAngle >= 0x2aaa) {
		return false;
	}

	f32 dist = (object->mPosition - mPosition).length();
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

bool TMario::trampleExec(THitActor* param_1)
{
	if (!checkActionFlag(0x800)) {
		return false;
	}

	if (mAction == 0x891) {
		return false;
	}

	if (param_1->receiveMessage(this, 0) == FALSE) {
		return false;
	}

	if (mAction == 0x888) {
		changePlayerStatus(0x883, 0, false);
	} else {
		switch (unk0FA) {
		case 0xd1:
			setAnimation(0xd2, 1.0f);
			break;
		case 0xd2:
		case 0xd3:
			setAnimation(0xd3, 1.0f);
			mModel->getFrameCtrl(0).setFrame(0.0f);
			break;
		default:
			setAnimation(0xd1, 1.0f);
			break;
		}
		changePlayerStatus(0x2000890, 0, false);
		setStatusToJumping(0x2000890, 0);
	}

	rumbleStart(0x15, mMotorParams.mMotorTrample.get());

	if (param_1->checkActorType(ACTOR_TYPE_UNK4000000)) {
		// SMS_EasyEmitParticle looks funky
		// SMS_EasyEmitParticle(8, mPosition, this);
		// SMS_EasyEmitParticle(9, mPosition, this);
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
		if (gpMSound->gateCheck(0x1818)) {
			MSoundSESystem::MSoundSE::startSoundActorWithInfo(
			    0x1818, &mPosition, nullptr, 0.0f, trampleCt, 0, nullptr, 0, 4);
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
	if (mAction != 0x224e0 && mAction != 0x21313 && mAction != 0x224e1
	    && mAction != 0x1000192a) {
		unk118 |= MARIO_FLAG_GAME_OVER;
		mHealth = 0;

		// Probably an inline
		if (gpMSound->gateCheck(0x480c)) {
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x480c, 0, nullptr, 0);
		}

		mAnmSound->stop();

		if (mYoshi != nullptr) {
			mYoshi->kill();
		}

		if (checkActionFlag(0x2000)) {
			if (unk12C < 1.0f) {
				changePlayerStatus(0x224e0, 0, true);
			} else {
				changePlayerStatus(0x224e1, 0, true);
			}
			return;
		}
		if (mAction == 0x20338) {
			changePlayerStatus(0x21313, 0, true);
		} else {
			changePlayerStatus(0x1000192a, 0, true);
		}
	}
}

void TMario::floorDamageExec(const TMario::TEParams& params)
{
	if (isInvincible()) {
		return;
	}
	mFloorHitActor.mPosition.x = mPosition.x + JMASSin(mFaceAngle.y);
	mFloorHitActor.mPosition.z = mPosition.z + JMASCos(mFaceAngle.y);
	damageExec(&mFloorHitActor, params.mDamage.get(), params.mDownType.get(),
	           params.mWaterEmit.get(), params.mMinSpeed.get(),
	           params.mMotor.get(), params.mDamage.get(),
	           params.mInvincibleTime.get());
}
void TMario::floorDamageExec(int damage, int damageAnimType, int waterEmit,
                             int rumbleFrames)
{
	if (isInvincible()) {
		return;
	}
	mFloorHitActor.mPosition.x = mPosition.x + JMASSin(mFaceAngle.y);
	mFloorHitActor.mPosition.z = mPosition.z + JMASCos(mFaceAngle.y);
	damageExec(&mFloorHitActor, damage, damageAnimType, waterEmit, 16.0f,
	           rumbleFrames, 0.0f, 0x3c);
}

// Closest i got, but i think this is wrong, but probably functionally
// equivalent? I kinda suspect they didn't use this many helper functions? The
// double epsilon check confused me
void TMario::calcDamagePos(const JGeometry::TVec3<f32>& pos)
{
	JGeometry::TVec3<f32> offset = pos - mPosition;
	offset.normalize();
	unk19C = mPosition + offset * 50.0f;
}

void TMario::damageExec(THitActor* hittingActor, int damage, int damageAnimType,
                        int waterEmit, f32 knockbackSpeed, int rumbleFrames,
                        f32 pollutionAmount, s16 invincibilityFrames)
{
	// volatile u32 padding[10];
	u32 animationTypes[16] = {
		0x20464, 0x20462,   0x20460, 0xC400201, 0x208b0, 0x208b0,
		0x208b3, 0xC400201, 0x20465, 0x20463,   0x20461, 0xC400201,
		0x208B1, 0x208B1,   0x208B2, 0xC400201,
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
		if (gpMSound->gateCheck(0x7918)) {
			MSoundSESystem::MSoundSE::startSoundActor(0x7918, &mPosition, 0,
			                                          nullptr, 0, 4);
		}
		return;
	}

	u32 animOffset1 = checkActionFlag(0x800) ? 1 : 0;
	if (onYoshi()) {
		animOffset1 = true;
	}

	if (damageAnimType == 3) {
		if (mAction == 0x4000440 || mAction == 0x4045C) {
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
		if (mAction == 0x800447) {
			canPlayAnimation = false;
		}
		if (mAction == 0x891) {
			canPlayAnimation = false;
		}

		if (checkActionFlag(0x2000)) {
			canPlayAnimation = true;
		}

		if (canPlayAnimation) {
			// I don't think this is correct, but was the closest i could get
			u32 animationIdx = animationTypes[damageAnimType + animOffset1 * 4
			                                  + animOffset2 * 8];
			// Possibly inlined check for held by specific thing
			// M art in shine gates?
			if (mHolder == nullptr || mHolder->isActorType(0x40000098)) {
				changePlayerDropping(0x208BA, 0);
			} else {
				changePlayerDropping(animationIdx, 0);
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
			startVoice(0x7830);
		} else {
			if (damageAnimType == 0) {
				startVoice(0x783b);
			} else {
				startVoice(0x7833);
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

	if (checkUnk380(2)) {
		check = true;
	}

	if (checkUnk380(3)) {
		check = true;
	}

	if (mAction == 899 || checkActionFlag(0x80000000)) {
		check = true;
	}

	if (mAction == 0x560 || mAction == 0x894 || mAction == 0x40561) {
		check = true;
	}

	if (mHeldObject != nullptr && mHeldObject->mHolder != this) {
		mHeldObject = nullptr;
	}

	if (mHolder != nullptr && mHolder->mHeldObject != this) {
		mHolder = nullptr;
	}

	if (mHeldObject != nullptr && !check) {
		mHeldObject->receiveMessage(this, 7);
		mHeldObject->receiveMessage(this, 8);
		mHeldObject = nullptr;
	}

	if (mHolder != nullptr) {
		// Probably an inline
		BOOL check2 = false;
		u32 test    = mAction & 0x1ff;
		if ((0x150 <= test && 0x15c >= test) || (0x140 <= test && test <= 0x143)
		    || checkActionFlag(0x1000) || mAction == 0x10020370) {
			check2 = true;
		}

		if (!check2) {
			mHolder->receiveMessage(this, 8);
			mHolder->receiveMessage(this, 7);
			mHolder = nullptr;
		}
	}
}
