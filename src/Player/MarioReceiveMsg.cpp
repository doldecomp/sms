#include <Player/MarioMain.hpp>
#include <Player/WaterGun.hpp>
#include <Player/MarioCap.hpp>
#include <Map/MapWire.hpp>
#include <Map/MapWireManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Strategic/HitActor.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

bool TMario::getNozzle(THitActor* sender, TWaterGun::TNozzleType type)
{
	if (onYoshi())
		return FALSE;
	unk118 |= MARIO_FLAG_HAS_FLUDD;
	if (checkFlag(MARIO_FLAG_HAS_FLUDD))
		mWaterGun->changeNozzle(type, true);

	unk144 = 3600;
	resetNozzle();
	unk148 = sender;
	if (checkFlag(MARIO_FLAG_HAS_FLUDD))
		mWaterGun->resetWaterToFull();

	emitGetEffect();
	return TRUE;
}

void TMario::getGesso(THitActor* param_1)
{
	if (mStatus != 0x10000) {
		mFaceAngle.y    = DEG2SHORTANGLE(param_1->mRotation.y);
		mModelFaceAngle = mFaceAngle.y;
		changePlayerStatus(STATUS_SURF, 0, false);
		mStatusTimer = mDeParams.mSurfStartFreezeTime.get();
		emitGetEffect();
		switch (param_1->getActorType()) {
		case 0x400000C5:
			mSurfGesso = gpMapObjManager->mRedGesso;
			unk389     = 0;
			break;

		case 0x400000C6:
			mSurfGesso = gpMapObjManager->mYellowGesso;
			unk389     = 1;
			break;

		default:
		case 0x400000C7:
			unk389     = 2;
			mSurfGesso = gpMapObjManager->mGreenGesso;
			break;
		}
		mSurfGesso->setBck("surfgeso_run1");
		mSurfGesso->getFrameCtrl(0)->setRate(0.5f);
	}
}

void TMario::getCoin()
{
	++mCoinCount;
	emitGetCoinEffect(&mPosition);
	incHP(1);
	if ((TFlagManager::smInstance->getFlag(0x40002) % 50) == 0) {
		TFlagManager::smInstance->incMario(1);
		SMSGetMSound()->startSoundSystemSE(0x4841, 0, nullptr, 0);
	}
}

void TMario::getCoinRed()
{
	incHP(2);
	emitGetCoinEffect(&mPosition);
	int id = TFlagManager::smInstance->getFlag(0x60000) + 70;
	gpMarioParticleManager->emitAndBindToPosPtr(id, &mPosition, 0, this);
}

void TMario::getCoinBlue()
{
	incHP(2);
	emitGetCoinEffect(&mPosition);
}

BOOL TMario::receiveMessage(THitActor* sender, u32 message)
{
	if (checkFlag(MARIO_FLAG_GAME_OVER))
		return FALSE;

	// Generic "Mario hit by enemy" thump sound.
	if (sender->checkActorType(0x20000000)) {
		bool playThump = true;
		if (sender->mActorType == 0x2000000E)
			playThump = false;
		if (sender->mActorType == 0x2000000F)
			playThump = false;
		if (sender->mActorType == 0x20000010)
			playThump = false;
		if (sender->mActorType == 0x20000011)
			playThump = false;
		if (sender->mActorType == 0x20000013)
			playThump = false;
		if (sender->mActorType == 0x2000001F)
			playThump = false;
		if (sender->mActorType == 0x20000026)
			playThump = false;
		if (sender->mActorType == 0x20000022)
			playThump = false;
		if (sender->mActorType == 0x2000002A)
			playThump = false;

		if (playThump == true)
			SMSGetMSound()->startSoundActor(0x180C, &mPosition, 0, nullptr, 0,
			                                4);
	}

	// HIT_MESSAGE_UNK7 - damage trigger.
	if (message == HIT_MESSAGE_UNK7) {
		if (sender->checkActorType(ACTOR_TYPE_PLAYER)
		    || sender->checkActorType(ACTOR_TYPE_UNK4000000)
		    || sender->checkActorType(ACTOR_TYPE_ENEMY)
		    || sender->checkActorType(ACTOR_TYPE_BOSS)) {
			SMSGetMSound()->startSoundActor(0x193D, &mPosition, 0, &mSound, 0,
			                                4);
		}
		if (sender->mActorType == 0x10000015)
			startVoice(0x78CF);

		if (sender->checkActorType(ACTOR_TYPE_UNK40000000)
		    && gpMSound->getMarioVoiceID(0) != 0x78D3) {
			startVoice(0x78D3);
		}
		if (onYoshi())
			SMSGetMSound()->startSoundActor(0x791C, &mPosition, 0, nullptr, 0,
			                                4);

		changePlayerStatus(STATUS_THROWN_DOWN, 0, false);
		return TRUE;
	}

	// Pickups (items, coins, water bottles, nozzles).
	if (sender->checkActorType(ACTOR_TYPE_UNK20000000)) {
		switch (sender->mActorType) {
		case 0x20000001: // small water restore
			if (message == HIT_MESSAGE_ATTACK) {
				if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
					mWaterGun->addWater(mWaterGun->getMaxWater() / 2);
				}
				emitGetWaterEffect();
				return TRUE;
			}
			break;
		case 0x20000002: // full water restore
			if (message == HIT_MESSAGE_ATTACK) {
				if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
					mWaterGun->addWater(mWaterGun->getMaxWater());
				}
				emitGetWaterEffect();
				return TRUE;
			}
			break;
		case 0x2000002C: // heart (+8 HP)
			if (message == HIT_MESSAGE_ATTACK) {
				incHP(8);
				emitGetEffect();
				return TRUE;
			}
			break;
		case 0x20000003: // +4 HP fruit and water
			if (message == HIT_MESSAGE_ATTACK) {
				incHP(4);
				if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
					mWaterGun->addWater(mWaterGun->getMaxWater());
				}
				emitGetEffect();
				return TRUE;
			}
			break;
		case 0x20000004: // +1 HP fruit
			if (message == HIT_MESSAGE_ATTACK) {
				incHP(1);
				emitGetEffect();
				return TRUE;
			}
			break;
		case 0x20000005:
		case 0x20000006:
		case 0x20000007: // collectible fruit
			if (message == HIT_MESSAGE_ATTACK) {
				if (*(s8*)((u8*)sender + 0x13A) == 0
				    && !(*(s32*)((u8*)sender + 0x13C) < 120 ? true : false)) {
					mHealth = mDeParams.mHpMax.get();
					if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
						mWaterGun->addWater(mWaterGun->getMaxWater());
					}
					TFlagManager::smInstance->incMario(1);
					emitGetEffect();
					return TRUE;
				}
				return FALSE;
			}
			break;

		case 0x20000022: // pickup rocket nozzle
			return getNozzle(sender, TWaterGun::Rocket);
		case 0x20000026: // pickup hover nozzle
			return getNozzle(sender, TWaterGun::Hover);
		case 0x2000002A: // pickup turbo nozzle
			return getNozzle(sender, TWaterGun::Turbo);
		case 0x2000002B: // pickup underwater (helmet) nozzle
			changePlayerStatus(STATUS_DIVE, 0, false);
			return getNozzle(sender, TWaterGun::Underwater);
		case 0x2000001F: // pickup spray nozzle
			getNozzle(sender, TWaterGun::Spray);
			return TRUE;

		case 0x2000003C: // shirt/cap pickup
			mCap->setModelActive(TMarioCap::E_CAP_MODEL_HAT);
			mHealth = mDeParams.mHpMax.get();
			emitGetEffect();
			return TRUE;
		case 0x2000000E: // yellow coin
			getCoin();
			return TRUE;
		case 0x2000000F: // red coin
			getCoinRed();
			return TRUE;
		case 0x20000010: // blue coin
			getCoinBlue();
			return TRUE;
		case 0x20000013: // 1-up shroom / pickup-action
			if (message == HIT_MESSAGE_ATTACK && mStatus != STATUS_WIN_DEMO) {
				unk384          = sender;
				mPosition.x     = sender->mPosition.x;
				mPosition.z     = sender->mPosition.z;
				mFaceAngle.y    = DEG2SHORTANGLE(*(f32*)((u8*)sender + 0x11C));
				mModelFaceAngle = mFaceAngle.y;
				setPlayerVelocity(0.0f);
				mHealth = mDeParams.mHpMax.get();
				unk12C  = unk130;
				changePlayerStatus(STATUS_WIN_DEMO, 0, true);
				return TRUE;
			}
			break;
		}
	}

	// Bosses / NPC-class actors with bit 0x40000000.
	if (sender->checkActorType(ACTOR_TYPE_UNK40000000)) {
		switch (sender->getActorType()) {
		case 0x4000001D: // lamp trap iron
			if (message == HIT_MESSAGE_UNKA && !isInvincible()) {
				damageExec(sender, mDmgParamsLampTrapIron.mDamage.get(),
				           mDmgParamsLampTrapIron.mDownType.get(),
				           mDmgParamsLampTrapIron.mWaterEmit.get(),
				           mDmgParamsLampTrapIron.mMinSpeed.get(),
				           mDmgParamsLampTrapIron.mMotor.get(),
				           mDmgParamsLampTrapIron.mDirty.get(),
				           mDmgParamsLampTrapIron.mInvincibleTime.get());
				changePlayerStatus(STATUS_FIRE_DOWN, 1, false);
				SMSGetMSound()->startSoundActor(0x1813, &mPosition, 0, nullptr,
				                                0, 4);

				gpMarioParticleManager->emitAndBindToPosPtr(6, &mPosition, 0,
				                                            nullptr);
				return TRUE;
			}
			break;
		case 0x4000001E: // lamp trap spike
			if (message == HIT_MESSAGE_ATTACK && !isInvincible()) {
				damageExec(sender, mDmgParamsLampTrapSpike.mDamage.get(),
				           mDmgParamsLampTrapSpike.mDownType.get(),
				           mDmgParamsLampTrapSpike.mWaterEmit.get(),
				           mDmgParamsLampTrapSpike.mMinSpeed.get(),
				           mDmgParamsLampTrapSpike.mMotor.get(),
				           mDmgParamsLampTrapSpike.mDirty.get(),
				           mDmgParamsLampTrapSpike.mInvincibleTime.get());
				return TRUE;
			}
			break;
		case 0x400000C5: // red gesso
		case 0x400000C6: // yellow gesso
		case 0x400000C7: // green gesso
			getGesso(sender);
			return TRUE;
		case 0x40000258: // grabbable hangable thing
			if (message == HIT_MESSAGE_TAKE) {
				mHolder = (TTakeActor*)sender;
				changePlayerStatus(STATUS_NOMOTION, 0, false);
				return TRUE;
			}
			break;
		case 0x4000025B: // fire-class
			if (message == HIT_MESSAGE_UNKA) {
				if (!isInvincible()) {
					damageExec(sender, mDmgParamsFire.mDamage.get(),
					           mDmgParamsFire.mDownType.get(),
					           mDmgParamsFire.mWaterEmit.get(),
					           mDmgParamsFire.mMinSpeed.get(),
					           mDmgParamsFire.mMotor.get(),
					           mDmgParamsFire.mDirty.get(),
					           mDmgParamsFire.mInvincibleTime.get());
					startVoice(0x78CF);
					changePlayerStatus(STATUS_THROWN_DOWN, 1, false);
					return TRUE;
				}
				return FALSE;
			}
			break;
		case 0x40000098: { // wire/zipline
			if (mHolder == nullptr) {
				if (mStatus == 0x892 && mVel.y > 0.0f)
					return FALSE;
				if (mStatus == 0x893 && mVel.y > 0.0f)
					return FALSE;
				if (mStatus == 0x208BA)
					return FALSE;
				if (onYoshi())
					return FALSE;
				mHolder             = (TTakeActor*)sender;
				TMapWireActor* wire = (TMapWireActor*)sender;
				wire->getTipPoints(&mWireStartPos, &mWireEndPos);
				mWirePosRatio = wire->getPosInWire();
				wireMove(0.0f);
				unk118 &= ~0x100;

				JGeometry::TVec3<f32> diff = mWireEndPos - mWireStartPos;
				int dirToEnd   = (s16)(matan(diff.z, diff.x) - mFaceAngle.y);
				mWireBounceVel = 0.2f * -mVel.y;
				mWireSag       = 0.0f;

				bool flipDir;
				if (mStatus == 0x893 || mStatus == 0x80088A) {
					flipDir = true;
				} else if (mVel.y < 0.0f) {
					flipDir = false;
				} else {
					flipDir = true;
				}
				if (flipDir == true) {
					if (dirToEnd > 0) {
						JGeometry::TVec3<f32> tmp = mWireStartPos;
						mWireStartPos             = mWireEndPos;
						mWireEndPos               = tmp;
						mWirePosRatio             = 1.0f - mWirePosRatio;
					}
					changeWireHanging();
					return TRUE;
				}
				if (dirToEnd <= -0x4000 || dirToEnd > 0x4000) {
					JGeometry::TVec3<f32> tmp = mWireStartPos;
					mWireStartPos             = mWireEndPos;
					mWireEndPos               = tmp;
					mWirePosRatio             = 1.0f - mWirePosRatio;
				}
				changePlayerStatus(STATUS_WIRE_WAIT, 0, false);
				return TRUE;
			} else {
				return FALSE;
			}
		}
		}
	}

	// Enemy-class actors with bit 0x10000000.
	if (sender->checkActorType(0x10000000)) {
		switch (sender->getActorType()) {
		case 0x10000003: // hamukuri (namekuri variant)
			if (message == HIT_MESSAGE_ATTACK && !isInvincible()) {
				damageExec(sender, mDmgParamsNamekuri.mDamage.get(),
				           mDmgParamsNamekuri.mDownType.get(),
				           mDmgParamsNamekuri.mWaterEmit.get(),
				           mDmgParamsNamekuri.mMinSpeed.get(),
				           mDmgParamsNamekuri.mMotor.get(),
				           mDmgParamsNamekuri.mDirty.get(),
				           mDmgParamsNamekuri.mInvincibleTime.get());
				return TRUE;
			}
			break;

		case 0x10000037:
		case 0x10000013: // dispatched as enemy common, drops cap
			if (message == HIT_MESSAGE_ATTACK && !isInvincible()) {
				mCap->setModelInactive(TMarioCap::E_CAP_MODEL_HAT);
				damageExec(sender, mDmgParamsEnemyCommon.mDamage.get(),
				           mDmgParamsEnemyCommon.mDownType.get(),
				           mDmgParamsEnemyCommon.mWaterEmit.get(),
				           mDmgParamsEnemyCommon.mMinSpeed.get(),
				           mDmgParamsEnemyCommon.mMotor.get(),
				           mDmgParamsEnemyCommon.mDirty.get(),
				           mDmgParamsEnemyCommon.mInvincibleTime.get());
				return TRUE;
			}
			break;

		case 0x4000019B: // hamukuri (alternate)
		case 0x10000002: // hamukuri
			if (message == HIT_MESSAGE_ATTACK && !isInvincible()) {
				damageExec(sender, mDmgParamsHamakuri.mDamage.get(),
				           mDmgParamsHamakuri.mDownType.get(),
				           mDmgParamsHamakuri.mWaterEmit.get(),
				           mDmgParamsHamakuri.mMinSpeed.get(),
				           mDmgParamsHamakuri.mMotor.get(),
				           mDmgParamsHamakuri.mDirty.get(),
				           mDmgParamsHamakuri.mInvincibleTime.get());
				return TRUE;
			}
			break;
		case 0x1000000A: // electric carapace / electric noko
			if (message == HIT_MESSAGE_ATTACK && !isInvincible()) {
				damageExec(sender, mDmgParamsHamakuri.mDamage.get(),
				           mDmgParamsHamakuri.mDownType.get(),
				           mDmgParamsHamakuri.mWaterEmit.get(),
				           mDmgParamsHamakuri.mMinSpeed.get(),
				           mDmgParamsHamakuri.mMotor.get(),
				           mDmgParamsHamakuri.mDirty.get(),
				           mDmgParamsHamakuri.mInvincibleTime.get());
				return TRUE;
			}
			// fallthrough

		case 0x1000000B:
		case 0x10000021:
		case 0x10000034: // elec attacker
			if (message == 9 && !isInvincible()) {
				elecEffect();
				changePlayerStatus(STATUS_ELECTRIC_DAMAGE, 0, false);
				return TRUE;
			}
			keepDistance(sender->mPosition, sender->getDamageRadius() + 30.0f,
			             0.0f);
			return TRUE;

		case 0x1000001F: // killer (bullet bill)
			if ((message == HIT_MESSAGE_UNKA || message == HIT_MESSAGE_ATTACK)
			    && !isInvincible()) {
				damageExec(sender, mDmgParamsKiller.mDamage.get(),
				           mDmgParamsKiller.mDownType.get(),
				           mDmgParamsKiller.mWaterEmit.get(),
				           mDmgParamsKiller.mMinSpeed.get(),
				           mDmgParamsKiller.mMotor.get(),
				           mDmgParamsKiller.mDirty.get(),
				           mDmgParamsKiller.mInvincibleTime.get());
				changePlayerStatus(STATUS_FIRE_DOWN, 1, false);
				SMSGetMSound()->startSoundActor(0x1813, &mPosition, 0, nullptr,
				                                0, 4);

				gpMarioParticleManager->emitAndBindToPosPtr(6, &mPosition, 0,
				                                            nullptr);
				return TRUE;
			}
			break;

		case 0x0800000F:
		case 0x08000010:
		case 0x08000011:
		case 0x08000012:
		case 0x10000004:
		case 0x10000006:
		case 0x10000007:
		case 0x10000008:
		case 0x10000009:
		case 0x1000000C:
		case 0x10000010:
		case 0x10000012:
		case 0x10000014:
		case 0x10000016:
		case 0x10000017:
		case 0x10000018:
		case 0x10000019:
		case 0x1000001A:
		case 0x1000001B:
		case 0x1000001C:
		case 0x1000001D:
		case 0x10000020:
		case 0x10000022:
		case 0x10000024:
		case 0x10000025:
		case 0x10000029:
		case 0x1000002A:
		case 0x1000002C:
		case 0x1000002D:
		case 0x1000002E:
		case 0x1000002F:
		case 0x10000033:
		case 0x10000036: // common enemy
			if (message == HIT_MESSAGE_ATTACK && !isInvincible()) {
				damageExec(sender, mDmgParamsEnemyCommon.mDamage.get(),
				           mDmgParamsEnemyCommon.mDownType.get(),
				           mDmgParamsEnemyCommon.mWaterEmit.get(),
				           mDmgParamsEnemyCommon.mMinSpeed.get(),
				           mDmgParamsEnemyCommon.mMotor.get(),
				           mDmgParamsEnemyCommon.mDirty.get(),
				           mDmgParamsEnemyCommon.mInvincibleTime.get());
				return TRUE;
			}
			keepDistance(sender->mPosition, sender->getDamageRadius() + 30.0f,
			             0.0f);
			return TRUE;
		case 0x40000053: // boss-graffito-tongue / sea attacker
			if (checkFlag(0x30000) && message == HIT_MESSAGE_ATTACK
			    && !isInvincible()) {
				damageExec(sender, mDmgParamsEnemyCommon.mDamage.get(),
				           mDmgParamsEnemyCommon.mDownType.get(),
				           mDmgParamsEnemyCommon.mWaterEmit.get(),
				           mDmgParamsEnemyCommon.mMinSpeed.get(),
				           mDmgParamsEnemyCommon.mMotor.get(),
				           mDmgParamsEnemyCommon.mDirty.get(),
				           mDmgParamsEnemyCommon.mInvincibleTime.get());
				return TRUE;
			}
			break;
		case 0x1000002B:
		case 0x1000000D: // glistening enemy
			if (message == HIT_MESSAGE_UNK5) {
				if (checkFlag(MARIO_FLAG_HAS_FLUDD))
					mWaterGun->mFlags |= 0x4;
			}
			break;
		case 0x10000015: // poihana
			if (!isInvincible() && message == HIT_MESSAGE_ATTACK) {
				damageExec(sender, mDmgParamsPoihana.mDamage.get(),
				           mDmgParamsPoihana.mDownType.get(),
				           mDmgParamsPoihana.mWaterEmit.get(),
				           mDmgParamsPoihana.mMinSpeed.get(),
				           mDmgParamsPoihana.mMotor.get(),
				           mDmgParamsPoihana.mDirty.get(),
				           mDmgParamsPoihana.mInvincibleTime.get());
				return TRUE;
			}
			break;
		case 0x10000005:
		case 0x1000000E:
		case 0x10000011:
		case 0x1000001E:
		case 0x10000026: // hot/fire-touching enemy
			if (message == HIT_MESSAGE_ATTACK && !isInvincible()) {
				damageExec(sender, mDmgParamsFire.mDamage.get(),
				           mDmgParamsFire.mDownType.get(),
				           mDmgParamsFire.mWaterEmit.get(),
				           mDmgParamsFire.mMinSpeed.get(),
				           mDmgParamsFire.mMotor.get(),
				           mDmgParamsFire.mDirty.get(),
				           mDmgParamsFire.mInvincibleTime.get());
				return TRUE;
			}
			if (((u32)(message - 9) <= 1U) && !isInvincible()) {
				damageExec(sender, mDmgParamsFire.mDamage.get(),
				           mDmgParamsFire.mDownType.get(),
				           mDmgParamsFire.mWaterEmit.get(),
				           mDmgParamsFire.mMinSpeed.get(),
				           mDmgParamsFire.mMotor.get(),
				           mDmgParamsFire.mDirty.get(),
				           mDmgParamsFire.mInvincibleTime.get());
				changePlayerStatus(STATUS_FIRE_DOWN, 1, false);
				SMSGetMSound()->startSoundActor(0x1813, &mPosition, 0, nullptr,
				                                0, 4);

				gpMarioParticleManager->emitAndBindToPosPtr(6, &mPosition, 0,
				                                            nullptr);
				return TRUE;
			}
			break;
		}
	}

	// Remaining range-based dispatch on raw mActorType.
	switch (sender->mActorType) {
	case 0x08000029: // hot/fire-only
		if (message == HIT_MESSAGE_UNKA) {
			if (!isInvincible()) {
				damageExec(sender, mDmgParamsFire.mDamage.get(),
				           mDmgParamsFire.mDownType.get(),
				           mDmgParamsFire.mWaterEmit.get(),
				           mDmgParamsFire.mMinSpeed.get(),
				           mDmgParamsFire.mMotor.get(),
				           mDmgParamsFire.mDirty.get(),
				           mDmgParamsFire.mInvincibleTime.get());
				changePlayerStatus(STATUS_FIRE_DOWN, 1, false);
				SMSGetMSound()->startSoundActor(0x1813, &mPosition, 0, nullptr,
				                                0, 4);

				gpMarioParticleManager->emitAndBindToPosPtr(6, &mPosition, 0,
				                                            nullptr);
			}
			return TRUE;
		}
		return FALSE;

	case 0x0800002A:
	case 0x0800002C:
		if (message == HIT_MESSAGE_ATTACK && !isInvincible()) {
			damageExec(sender, mDmgParamsEnemyCommon.mDamage.get(),
			           mDmgParamsEnemyCommon.mDownType.get(),
			           mDmgParamsEnemyCommon.mWaterEmit.get(),
			           mDmgParamsEnemyCommon.mMinSpeed.get(),
			           mDmgParamsEnemyCommon.mMotor.get(),
			           mDmgParamsEnemyCommon.mDirty.get(),
			           mDmgParamsEnemyCommon.mInvincibleTime.get());
			return TRUE;
		}
		break;

	case 0x08000001: // hinokuri-class
		if (message == HIT_MESSAGE_ATTACK && !isInvincible()) {
			damageExec(sender, mDmgParamsHinokuri.mDamage.get(),
			           mDmgParamsHinokuri.mDownType.get(),
			           mDmgParamsHinokuri.mWaterEmit.get(),
			           mDmgParamsHinokuri.mMinSpeed.get(),
			           mDmgParamsHinokuri.mMotor.get(),
			           mDmgParamsHinokuri.mDirty.get(),
			           mDmgParamsHinokuri.mInvincibleTime.get());
			return TRUE;
		}
		if (message == HIT_MESSAGE_UNK3 && !isInvincible()) {
			unk118 |= 0x800;
			if (!checkStatusFlag(STATUS_FLAG_JUMPING)) {
				rumbleStart(0x15, 0x0A);
			}
			return TRUE;
		}
		break;

	case 0x08000005:
		break;

	case 0x08000013: // BG tentacle
		switch (message) {
		case HIT_MESSAGE_TAKE:
			if (mHeldObject == nullptr && mHolder == nullptr) {
				mHolder = (TTakeActor*)sender;
				changePlayerStatus(STATUS_WAIT, 0, false);
				return TRUE;
			}
			break;
		case HIT_MESSAGE_ATTACK:
			if (!isInvincible()) {
				damageExec(sender, mDmgParamsBGTentacle.mDamage.get(),
				           mDmgParamsBGTentacle.mDownType.get(),
				           mDmgParamsBGTentacle.mWaterEmit.get(),
				           mDmgParamsBGTentacle.mMinSpeed.get(),
				           mDmgParamsBGTentacle.mMotor.get(),
				           mDmgParamsBGTentacle.mDirty.get(),
				           mDmgParamsBGTentacle.mInvincibleTime.get());
				return TRUE;
			}
			break;
		case HIT_MESSAGE_UNK8:
			if (checkFlag(0x1000)) {
				changePlayerStatus(STATUS_DIVE, 0, true);
			} else {
				changePlayerStatus(STATUS_WAIT, 0, false);
			}
			mHolder = nullptr;
			return TRUE;
		}
		break;

	case 0x0800000B:
	case 0x0800000C:
	case 0x0800000D:
	case 0x0800000E: // fire enemy / boss-eel-tear-equivalent
		if (message == HIT_MESSAGE_UNKA && !isInvincible()) {
			damageExec(sender, mDmgParamsFire.mDamage.get(),
			           mDmgParamsFire.mDownType.get(),
			           mDmgParamsFire.mWaterEmit.get(),
			           mDmgParamsFire.mMinSpeed.get(),
			           mDmgParamsFire.mMotor.get(), mDmgParamsFire.mDirty.get(),
			           mDmgParamsFire.mInvincibleTime.get());
			changePlayerStatus(STATUS_FIRE_DOWN, 1, false);
			SMSGetMSound()->startSoundActor(0x1813, &mPosition, 0, nullptr, 0,
			                                4);

			gpMarioParticleManager->emitAndBindToPosPtr(6, &mPosition, 0,
			                                            nullptr);
			return TRUE;
		}
		break;

	case 0x08000006:
	case 0x08000007:
	case 0x08000008:
	case 0x08000010:
	case 0x08000011:
	case 0x08000012:
	case 0x0800001F:
	case 0x08000022:
	case 0x08000023:
	case 0x08000027:
	case 0x1000000F:
	case 0x10000035:
		switch (message) {
		case HIT_MESSAGE_TAKE:
			if (!isInvincible() && mHeldObject == nullptr
			    && mHolder == nullptr) {
				mHolder = (TTakeActor*)sender;
				changePlayerStatus(STATUS_TAKEN, 0, false);
				return TRUE;
			}
			break;
		case HIT_MESSAGE_ATTACK:
			if (!isInvincible()) {
				damageExec(sender, mDmgParamsBGTentacle.mDamage.get(),
				           mDmgParamsBGTentacle.mDownType.get(),
				           mDmgParamsBGTentacle.mWaterEmit.get(),
				           mDmgParamsBGTentacle.mMinSpeed.get(),
				           mDmgParamsBGTentacle.mMotor.get(),
				           mDmgParamsBGTentacle.mDirty.get(),
				           mDmgParamsBGTentacle.mInvincibleTime.get());
				return TRUE;
			}
			break;
		case HIT_MESSAGE_UNK8:
			if (checkFlag(0x1000)) {
				changePlayerStatus(STATUS_DIVE, 0, true);
			} else {
				changePlayerStatus(STATUS_WAIT, 0, false);
			}
			mHolder = nullptr;
			return TRUE;
		}
		break;

	case 0x08000024: // boss-eel-class
		if (!isInvincible() && message == HIT_MESSAGE_ATTACK
		    && (((mStatus - 0x800000) != 0x8A9) || mStatusState != 3)) {
			damageExec(sender, mDmgParamsBGTentacle.mDamage.get(),
			           mDmgParamsBGTentacle.mDownType.get(),
			           mDmgParamsBGTentacle.mWaterEmit.get(),
			           mDmgParamsBGTentacle.mMinSpeed.get(),
			           mDmgParamsBGTentacle.mMotor.get(),
			           mDmgParamsBGTentacle.mDirty.get(),
			           mDmgParamsBGTentacle.mInvincibleTime.get());
			changePlayerStatus(STATUS_THROWN_DOWN, 0, false);
			return TRUE;
		}
		// fallthrough

	case 0x08000014:
	case 0x08000015: // common enemy-2
		if (message == HIT_MESSAGE_ATTACK && !isInvincible()) {
			damageExec(sender, mDmgParamsHanachanBoss.mDamage.get(),
			           mDmgParamsHanachanBoss.mDownType.get(),
			           mDmgParamsHanachanBoss.mWaterEmit.get(),
			           mDmgParamsHanachanBoss.mMinSpeed.get(),
			           mDmgParamsHanachanBoss.mMotor.get(),
			           mDmgParamsHanachanBoss.mDirty.get(),
			           mDmgParamsHanachanBoss.mInvincibleTime.get());
			return TRUE;
		}
		// fallthrough

	case 0x4000002A:
	case 0x4000002C: { // big spinning enemy with rotation-based attack window
		if (mInput & 0x8000) {
			s16 attackAngle = getAttackAngle(sender);
			s16 prevYaw     = mFaceAngle.y;
			s16 attackYaw   = attackAngle - prevYaw;
			if (attackYaw > -0x2000 && attackYaw < 0x2000) {
				f32 angleF = (sender->mRotation.y * (1.0f / 360.0f)) * 65536.0f;
				s16 frontYaw  = (s16)angleF;
				s16 frontDiff = frontYaw - prevYaw;
				if (frontDiff > -0x2000 && frontDiff < 0x2000) {
					mPosition    = sender->mPosition;
					mFaceAngle.y = frontYaw;
					changePlayerStatus(0x1320, 0, false);
					if (isHolding()) {
						unk380 = 2;
						setAnimation(0xE9, 1.0f);
					} else {
						setAnimation(0x5F, 1.0f);
					}
					startVoice(0x78E5);
					return TRUE;
				}
				if (frontDiff < -0x6000 || frontDiff > 0x6000) {
					if (message == 0x11) {
						mPosition    = sender->mPosition;
						mFaceAngle.y = frontYaw + 0x8000;
						changePlayerStatus(0x1321, 0, false);
						if (isHolding()) {
							unk380 = 2;
							setAnimation(0xE9, 1.0f);
						} else {
							setAnimation(0x60, 1.0f);
						}
						startVoice(0x78E5);
						return TRUE;
					}
					if (!isHolding()) {
						mPosition    = sender->mPosition;
						mFaceAngle.y = frontYaw + 0x8000;
						changePlayerStatus(0x1321, 0, false);
						setAnimation(0x139, 1.0f);
						return FALSE;
					}
				}
			}
		}
		break;
	}

	case 0x80000001:
	case 0x80000002:
		if (!isInvincible()) {
			switch (message) {
			case HIT_MESSAGE_TAKE:
				if (mHeldObject == nullptr && mHolder == nullptr) {
					mHolder = (TTakeActor*)sender;
					changePlayerStatus(STATUS_TAKEN, 0, false);
					return TRUE;
				}
				break;
			case HIT_MESSAGE_UNK6:
			case HIT_MESSAGE_UNK7:
				mHolder = nullptr;
				changePlayerStatus(STATUS_JUMP, 0, false);
				setPlayerVelocity(40.0f);
				mVel.y = 10.0f;
				unk78 &= ~0x100;
				return TRUE;
			case HIT_MESSAGE_ATTACK:
				damageExec(sender, mDmgParamsEnemyMario.mDamage.get(),
				           mDmgParamsEnemyMario.mDownType.get(),
				           mDmgParamsEnemyMario.mWaterEmit.get(),
				           mDmgParamsEnemyMario.mMinSpeed.get(),
				           mDmgParamsEnemyMario.mMotor.get(),
				           mDmgParamsEnemyMario.mDirty.get(),
				           mDmgParamsEnemyMario.mInvincibleTime.get());
				return TRUE;
			}
		}
		break;

	case 0x4000002D:
	case 0x4000002E:
	case 0x40000032:
	case 0x40000034:
	case 0x40000035:
	case 0x40000036:
	case 0x40000037:
	case 0x40000039:
	case 0x4000003A:
	case 0x4000005A:
		if (message == HIT_MESSAGE_UNK8) {
			changePlayerStatus(STATUS_WAIT, 0, false);
			mHeldObject = nullptr;
		}
		break;

	case 0x080000C0:
		if (mStatus != STATUS_WARP_IN && message == HIT_MESSAGE_TAKE) {
			mHolder = (TTakeActor*)sender;
			if (!checkStatusFlag(STATUS_FLAG_JUMPING)) {
				setAnimation(0x4D, 1.0f);
				s16 endFrame = getMotionFrameCtrl().getEnd();
				getMotionFrameCtrl().setFrame((f32)endFrame);
			}
			changePlayerDropping(STATUS_WARP_IN, 0);
			return TRUE;
		}
		break;

	case 0x400000A5:
	case 0x4000009C: // bird/butterfly: keep distance only
		if (message == HIT_MESSAGE_ATTACK) {
			keepDistance(sender->mPosition, sender->getDamageRadius() + 30.0f,
			             0.0f);
			return TRUE;
		}
		break;

	case 0x40000064:
	case 0x40000393: // fruit kick targets
		if (unk150 <= 0) {
			unk14E = mDeParams.mKickFreezeTime.get();
			rumbleStart(0x15, mMotorParams.mMotorTrample.get());
			calcDamagePos(sender->mPosition);
			kickFruitEffect();
			return TRUE;
		}
		break;

	case 0x40000246:
		break;
	}

	return FALSE;
}
