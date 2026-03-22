#include <Player/MarioMain.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/LiveActor.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Map/Map.hpp>
#include <Map/MapWireManager.hpp>
#include <Player/Yoshi.hpp>
#include <Player/Watergun.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>

// NOTE: -inline deferred, so reverse address order.
// getGesso is after receiveMessage in the binary.

BOOL TMario::receiveMessage(THitActor* sender, u32 message)
{
	// 0x24: Check if MARIO_FLAG_IS_PERFORMING is set
	if (checkFlag(MARIO_FLAG_IS_PERFORMING))
		return 0;

	u32 senderType = sender->mActorType;

	// 0x50: Check sender has ACTOR_TYPE_UNK4000000 (bit 2)
	if (senderType & 0x20000000) {
		// List of actor types that should NOT trigger the sound
		BOOL skip = 1;
		if ((senderType - 0x20000000) == 0x0E) skip = 0;
		if ((senderType - 0x20000000) == 0x0F) skip = 0;
		if ((senderType - 0x20000000) == 0x10) skip = 0;
		if ((senderType - 0x20000000) == 0x11) skip = 0;
		if ((senderType - 0x20000000) == 0x13) skip = 0;
		if ((senderType - 0x20000000) == 0x1F) skip = 0;
		if ((senderType - 0x20000000) == 0x26) skip = 0;
		if ((senderType - 0x20000000) == 0x22) skip = 0;
		if ((senderType - 0x20000000) == 0x2A) skip = 0;

		if (skip) {
			if (gpMSound->gateCheck(0x180C)) {
				MSoundSESystem::MSoundSE::startSoundActor(
				    0x180C, (const Vec*)&mPosition, 0, nullptr, 0, 4);
			}
		}
	}

	// message == 7: grab/take
	if (message == 7) {
		u32 aType = sender->mActorType;

		// Check if sender has bit 0 set (odd actor type)
		if (aType & 0x1) {
			// do nothing here, falls through
		} else if (aType & 0x04000000) {
			// do nothing
		} else if (aType & 0x10000000) {
			// do nothing
		} else if (aType & 0x08000000) {
			// fall through to below
		} else {
			// No special flags - check for sound
			goto msg7_check_sound;
		}

		// Play sound 0x193D
		if (gpMSound->gateCheck(0x193D)) {
			MSoundSESystem::MSoundSE::startSoundActor(
			    0x193D, (const Vec*)&mPosition, 0,
			    (JAISound**)&mSound, 0, 4);
		}

msg7_check_sound:
		// Check actor type 0x10000015 (specific actor)
		if (sender->mActorType - 0x10000000 == 0x15) {
			startVoice(0x78CF);
		}

		// Check if sender has bit 0x40000000
		if (sender->mActorType & 0x40000000) {
			if (gpMSound->getMarioVoiceID(0) != 0x78D3) {
				startVoice(0x78D3);
			}
		}

		// Check onYoshi
		if (onYoshi()) {
			if (gpMSound->gateCheck(0x791C)) {
				MSoundSESystem::MSoundSE::startSoundActor(
				    0x791C, (const Vec*)&mPosition, 0, nullptr, 0, 4);
			}
		}

		changePlayerStatus(0x000208B8, 0, false);
		return 1;
	}

	// After message 7, check sender type bits for enemy contact
	senderType = sender->mActorType;

	if (senderType & 0x20000000) {
		// Jump table based on (senderType - 0x20000001) with 60 entries
		u32 tableIdx = (senderType - 0x20000001);
		if (tableIdx > 59)
			goto check_sender_bit3;

		switch (senderType) {
		case 0x20000001: // Enemy type 1
			if (message != 0x0E)
				goto check_sender_bit3;
			// Check MARIO_FLAG_HAS_FLUDD
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				TNozzleBase* nozzle = mWaterGun->getCurrentNozzle();
				mWaterGun->mCurrentWater += nozzle->mEmitParams.mSuckRate.get() / 2;
				nozzle = mWaterGun->getCurrentNozzle();
				s32 maxWater = nozzle->mEmitParams.mAmountMax.get();
				if (mWaterGun->mCurrentWater > maxWater) {
					mWaterGun->mCurrentWater = maxWater;
				}
			}
			emitGetWaterEffect();
			return 1;

		case 0x20000002: // Enemy type 2
			if (message != 0x0E)
				goto check_sender_bit3;
			// Same pattern but multiply by full rate
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				TNozzleBase* nozzle = mWaterGun->getCurrentNozzle();
				mWaterGun->mCurrentWater += nozzle->mEmitParams.mSuckRate.get();
				nozzle = mWaterGun->getCurrentNozzle();
				s32 maxWater = nozzle->mEmitParams.mAmountMax.get();
				if (mWaterGun->mCurrentWater > maxWater) {
					mWaterGun->mCurrentWater = maxWater;
				}
			}
			emitGetWaterEffect();
			return 1;

		case 0x20000003: // HP+8
			if (message != 0x0E)
				goto check_sender_bit3;
			incHP(8);
			emitGetEffect();
			return 1;

		case 0x20000004: // HP+4
			if (message != 0x0E)
				goto check_sender_bit3;
			incHP(4);
			// Check FLUDD
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				TNozzleBase* nozzle = mWaterGun->getCurrentNozzle();
				mWaterGun->mCurrentWater += nozzle->mEmitParams.mSuckRate.get();
				nozzle = mWaterGun->getCurrentNozzle();
				s32 maxWater = nozzle->mEmitParams.mAmountMax.get();
				if (mWaterGun->mCurrentWater > maxWater) {
					mWaterGun->mCurrentWater = maxWater;
				}
			}
			emitGetEffect();
			return 1;

		case 0x20000005: // HP+1
			if (message != 0x0E)
				goto check_sender_bit3;
			incHP(1);
			emitGetEffect();
			return 1;

		case 0x20000006: // Special item with blooper check
			if (message != 0x0E)
				goto check_sender_bit3;
			// Check blooper not-stuck and timer < 120
			if (!*(s8*)((u8*)sender + 0x13A) &&
			    *(s32*)((u8*)sender + 0x13C) < 120) {
				return 0;
			}
			mHealth = *(s16*)((u8*)this + 0x58C);
			// Check FLUDD
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				TNozzleBase* nozzle = mWaterGun->getCurrentNozzle();
				mWaterGun->mCurrentWater += nozzle->mEmitParams.mSuckRate.get();
				nozzle = mWaterGun->getCurrentNozzle();
				s32 maxWater = nozzle->mEmitParams.mAmountMax.get();
				if (mWaterGun->mCurrentWater > maxWater) {
					mWaterGun->mCurrentWater = maxWater;
				}
			}
			TFlagManager::smInstance->incMario(1);
			emitGetEffect();
			return 1;

		case 0x20000007: // Nozzle pickup - Spray
		{
			if (onYoshi())
				return 0;

			mState |= 0x8000;
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				mWaterGun->changeNozzle(TWaterGun::Spray, true);
			}
			unk144 = 3600;
			resetNozzle();
			unk148 = (u32)sender;
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				TNozzleBase* nozzle = mWaterGun->getCurrentNozzle();
				mWaterGun->mCurrentWater = nozzle->mEmitParams.mSuckRate.get();
			}
			emitGetEffect();
			return 1;
		}

		case 0x20000008: // Nozzle pickup - Rocket
		{
			if (onYoshi())
				return 0;

			mState |= 0x8000;
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				mWaterGun->changeNozzle(TWaterGun::Rocket, true);
			}
			unk144 = 3600;
			resetNozzle();
			unk148 = (u32)sender;
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				TNozzleBase* nozzle = mWaterGun->getCurrentNozzle();
				mWaterGun->mCurrentWater = nozzle->mEmitParams.mSuckRate.get();
			}
			emitGetEffect();
			return 1;
		}

		case 0x20000009: // Nozzle pickup - Turbo
		{
			if (onYoshi())
				return 0;

			mState |= 0x8000;
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				mWaterGun->changeNozzle(TWaterGun::Turbo, true);
			}
			unk144 = 3600;
			resetNozzle();
			unk148 = (u32)sender;
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				TNozzleBase* nozzle = mWaterGun->getCurrentNozzle();
				mWaterGun->mCurrentWater = nozzle->mEmitParams.mSuckRate.get();
			}
			emitGetEffect();
			return 1;
		}

		case 0x2000000A: // Hover nozzle (changePlayerStatus + damageExec)
		{
			changePlayerStatus(0x891, 0, false);
			if (onYoshi())
				return 0;

			mState |= 0x8000;
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				mWaterGun->changeNozzle(TWaterGun::Hover, true);
			}
			unk144 = 3600;
			resetNozzle();
			unk148 = (u32)sender;
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				TNozzleBase* nozzle = mWaterGun->getCurrentNozzle();
				mWaterGun->mCurrentWater = nozzle->mEmitParams.mSuckRate.get();
			}
			emitGetEffect();
			return 1;
		}

		case 0x2000000B: // No-keep-pull/direct damage
		{
			if (onYoshi())
				return 0;

			mState |= 0x8000;
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				mWaterGun->changeNozzle(TWaterGun::Spray, true);
			}
			unk144 = 3600;
			resetNozzle();
			unk148 = (u32)sender;
			if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
				TNozzleBase* nozzle = mWaterGun->getCurrentNozzle();
				mWaterGun->mCurrentWater = nozzle->mEmitParams.mSuckRate.get();
			}
			emitGetEffect();
			return 1;
		}

		case 0x2000000C: // Yoshi release flag
		{
			TMarioCap* cap = mCap;
			u16 flags = *(u16*)((u8*)cap + 4);
			*(u16*)((u8*)cap + 4) = flags | 1;
			mHealth = *(s16*)((u8*)this + 0x58C);
			emitGetEffect();
			return 1;
		}

		case 0x2000000D: // Coin pickup
		{
			mCoinCount++;
			emitGetCoinEffect((JGeometry::TVec3<f32>*)&mPosition);
			incHP(1);

			s32 coinFlag = TFlagManager::smInstance->getFlag(0x40002);
			// Check if coins are multiple of 50
			if ((coinFlag - ((((s32)((u32)((s64)0x51EB851F * (s64)coinFlag) >> 32)) >> 4) + ((u32)((s64)0x51EB851F * (s64)coinFlag) >> 63)) * 50) == 0) {
				TFlagManager::smInstance->incMario(1);
				if (gpMSound->gateCheck(0x4841)) {
					MSoundSESystem::MSoundSE::startSoundSystemSE(0x4841, 0, nullptr, 0);
				}
			}
			return 1;
		}

		case 0x2000000E: // Red coin
		{
			incHP(2);
			emitGetCoinEffect((JGeometry::TVec3<f32>*)&mPosition);
			s32 redCoins = TFlagManager::smInstance->getFlag(0x60000);
			s32 adjustedCoins = redCoins + 70;
			gpMarioParticleManager->emitAndBindToPosPtr(adjustedCoins, (const JGeometry::TVec3<f32>*)&mPosition, 0, nullptr);
			return 1;
		}

		case 0x2000000F: // Blue coin
		{
			incHP(2);
			emitGetCoinEffect((JGeometry::TVec3<f32>*)&mPosition);
			return 1;
		}

		case 0x2000003C: // Wire actor
		{
			if (message != 0x0E)
				goto check_sender_bit3;
			if (mAction == 0x1302)
				goto check_sender_bit3;

			unk384 = sender;
			mPosition.x = sender->mPosition.x;
			mPosition.z = sender->mPosition.z;

			s16 angle = (s16)(65536.0f * sender->mAttackHeight);
			mFaceAngle.y = angle;
			mModelFaceAngle = angle;

			setPlayerVelocity(0.0f);
			mHealth = *(s16*)((u8*)this + 0x58C);
			changePlayerStatus(0x1302, 0, true);
			mForwardVel = unk130;
			return 1;
		}

		default:
			goto check_sender_bit3;
		}
	}

check_sender_bit3:
	// Check sender type bit 3 (0x10000000 = ACTOR_TYPE_ENEMY)
	if (sender->mActorType & 0x10000000) {
		u32 eType = sender->mActorType;

		// Large binary search tree on enemy actor types
		// This is an enormous switch statement with many cases
		// Each case calls damageExec with different TEParams

		switch (eType) {
		case 0x10000003: // Enemy common
			if (message == 0x0E) {
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsEnemyCommon.mDamage.get(),
					           mDmgParamsEnemyCommon.mDownType.get(),
					           mDmgParamsEnemyCommon.mWaterEmit.get(),
					           mDmgParamsEnemyCommon.mMinSpeed.get(),
					           mDmgParamsEnemyCommon.mMotor.get(),
					           mDmgParamsEnemyCommon.mDirty.get(),
					           mDmgParamsEnemyCommon.mInvincibleTime.get());
					return 1;
				}
			}
			goto default_enemy_handle;

		case 0x10000013: // Namekuri (snail enemy)
			if (message == 0x0E) {
				if (!isInvincible()) {
					// Clear sticky flag on cap
					TMarioCap* cap = mCap;
					u16 flags = *(u16*)((u8*)cap + 4);
					*(u16*)((u8*)cap + 4) = flags & ~1;
					// damageExec with Namekuri params
					damageExec(sender,
					           mDmgParamsNamekuri.mDamage.get(),
					           mDmgParamsNamekuri.mDownType.get(),
					           mDmgParamsNamekuri.mWaterEmit.get(),
					           mDmgParamsNamekuri.mMinSpeed.get(),
					           mDmgParamsNamekuri.mMotor.get(),
					           mDmgParamsNamekuri.mDirty.get(),
					           mDmgParamsNamekuri.mInvincibleTime.get());
					return 1;
				}
			}
			goto default_enemy_handle;

		case 0x10000002: // Hamakuri
		case 0x1000019B: // Also hamakuri variant
			if (message == 0x0E) {
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsHamakuri.mDamage.get(),
					           mDmgParamsHamakuri.mDownType.get(),
					           mDmgParamsHamakuri.mWaterEmit.get(),
					           mDmgParamsHamakuri.mMinSpeed.get(),
					           mDmgParamsHamakuri.mMotor.get(),
					           mDmgParamsHamakuri.mDirty.get(),
					           mDmgParamsHamakuri.mInvincibleTime.get());
					return 1;
				}
			}
			goto default_enemy_handle;

		case 0x1000000A: // Hinokuri
			if (message == 0x0E) {
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsHinokuri.mDamage.get(),
					           mDmgParamsHinokuri.mDownType.get(),
					           mDmgParamsHinokuri.mWaterEmit.get(),
					           mDmgParamsHinokuri.mMinSpeed.get(),
					           mDmgParamsHinokuri.mMotor.get(),
					           mDmgParamsHinokuri.mDirty.get(),
					           mDmgParamsHinokuri.mInvincibleTime.get());
					return 1;
				}
			} else if (message == 9) {
				if (!isInvincible()) {
					elecEffect();
					changePlayerStatus(0x20338, 0, false);
					return 1;
				}
			}
			// Fall through to keepDistance
			{
				f32 height = sender->mAttackHeight;
				keepDistance(sender->mPosition, 0.0f + height, 0.0f);
			}
			return 1;

		case 0x1000001F: // Boss (damageExec with sound + particles)
			if (message == 0x0A || message == 0x0E) {
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsBGTentacle.mDamage.get(),
					           mDmgParamsBGTentacle.mDownType.get(),
					           mDmgParamsBGTentacle.mWaterEmit.get(),
					           mDmgParamsBGTentacle.mMinSpeed.get(),
					           mDmgParamsBGTentacle.mMotor.get(),
					           mDmgParamsBGTentacle.mDirty.get(),
					           mDmgParamsBGTentacle.mInvincibleTime.get());

					changePlayerStatus(0x000208B7, 1, false);

					if (gpMSound->gateCheck(0x1813)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    0x1813, (const Vec*)&mPosition, 0, nullptr, 0, 4);
					}
					gpMarioParticleManager->emitAndBindToPosPtr(
					    6, (const JGeometry::TVec3<f32>*)&mPosition, 0, nullptr);
					return 1;
				}
			}
			goto default_enemy_handle;

		case 0x1000001E: // Boss eel / special enemy contact
			if (message == 0x0E) {
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsBossEel.mDamage.get(),
					           mDmgParamsBossEel.mDownType.get(),
					           mDmgParamsBossEel.mWaterEmit.get(),
					           mDmgParamsBossEel.mMinSpeed.get(),
					           mDmgParamsBossEel.mMotor.get(),
					           mDmgParamsBossEel.mDirty.get(),
					           mDmgParamsBossEel.mInvincibleTime.get());
					return 1;
				}
			}
			goto default_enemy_handle;

		case 0x10000004: // Fire enemy
		case 0x10000006:
		case 0x10000010:
		case 0x10000022:
		case 0x10000033:
		case 0x10000036:
			if (message == 0x0E) {
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsFire.mDamage.get(),
					           mDmgParamsFire.mDownType.get(),
					           mDmgParamsFire.mWaterEmit.get(),
					           mDmgParamsFire.mMinSpeed.get(),
					           mDmgParamsFire.mMotor.get(),
					           mDmgParamsFire.mDirty.get(),
					           mDmgParamsFire.mInvincibleTime.get());
					return 1;
				}
			}
			// Fall through to keepDistance
			{
				f32 height = sender->mAttackHeight;
				keepDistance(sender->mPosition, 0.0f + height, 0.0f);
			}
			return 1;

		case 0x10000053: // Special boss
		{
			// Check mState bits 14-15
			if (!(mState & 0x0003C000))
				goto default_msg;
			if (message != 0x0E)
				goto default_msg;
			if (!isInvincible()) {
				damageExec(sender,
				           mDmgParamsFire.mDamage.get(),
				           mDmgParamsFire.mDownType.get(),
				           mDmgParamsFire.mWaterEmit.get(),
				           mDmgParamsFire.mMinSpeed.get(),
				           mDmgParamsFire.mMotor.get(),
				           mDmgParamsFire.mDirty.get(),
				           mDmgParamsFire.mInvincibleTime.get());
				return 1;
			}
			goto default_msg;
		}

		case 0x1000000D:
		case 0x1000002B: // Water surface damage (with poison flag)
			if (message == 5) {
				if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
					u16 flags2 = *(u16*)((u8*)mWaterGun + 4);
					*(u16*)((u8*)mWaterGun + 4) = flags2 | 4;
				}
				goto default_msg;
			}
			// Fall through
			goto default_msg;

		case 0x10000015: // Poihana
			if (!isInvincible()) {
				if (message != 0x0E)
					goto default_msg;
				damageExec(sender,
				           mDmgParamsPoihana.mDamage.get(),
				           mDmgParamsPoihana.mDownType.get(),
				           mDmgParamsPoihana.mWaterEmit.get(),
				           mDmgParamsPoihana.mMinSpeed.get(),
				           mDmgParamsPoihana.mMotor.get(),
				           mDmgParamsPoihana.mDirty.get(),
				           mDmgParamsPoihana.mInvincibleTime.get());
				return 1;
			}
			goto default_msg;

		case 0x1000000F:
		case 0x10000012:
		case 0x10000018:
		case 0x1000000E:
		case 0x10000021:
		case 0x10000035:
		case 0x10000029:
		case 0x10000030:
			// Enemy common (default damage)
			if (message == 0x0E) {
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsEnemyCommon.mDamage.get(),
					           mDmgParamsEnemyCommon.mDownType.get(),
					           mDmgParamsEnemyCommon.mWaterEmit.get(),
					           mDmgParamsEnemyCommon.mMinSpeed.get(),
					           mDmgParamsEnemyCommon.mMotor.get(),
					           mDmgParamsEnemyCommon.mDirty.get(),
					           mDmgParamsEnemyCommon.mInvincibleTime.get());
					return 1;
				}
			}
			// Fall through to keepDistance
			{
				f32 height = sender->mAttackHeight;
				keepDistance(sender->mPosition, 0.0f + height, 0.0f);
			}
			return 1;

		case 0x10000024: // Hanachan boss damage
		{
			if (!isInvincible()) {
				if (message != 0x0E)
					goto default_msg;
				// Check action state
				if (mAction == 0x800008A9) {
					if (mActionState == 3)
						goto default_msg;
				}
				damageExec(sender,
				           mDmgParamsHanachanBoss.mDamage.get(),
				           mDmgParamsHanachanBoss.mDownType.get(),
				           mDmgParamsHanachanBoss.mWaterEmit.get(),
				           mDmgParamsHanachanBoss.mMinSpeed.get(),
				           mDmgParamsHanachanBoss.mMotor.get(),
				           mDmgParamsHanachanBoss.mDirty.get(),
				           mDmgParamsHanachanBoss.mInvincibleTime.get());

				changePlayerStatus(0x000208B8, 0, false);
				return 1;
			}
			goto default_msg;
		}

		case 0x10000026: // Specific enemy (just keepDistance)
			goto default_enemy_fallthrough;

		default:
		default_enemy_handle:
			// Default enemy: check message type and apply damage or keepDistance
			if (message == 9) {
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsEnemyCommon.mDamage.get(),
					           mDmgParamsEnemyCommon.mDownType.get(),
					           mDmgParamsEnemyCommon.mWaterEmit.get(),
					           mDmgParamsEnemyCommon.mMinSpeed.get(),
					           mDmgParamsEnemyCommon.mMotor.get(),
					           mDmgParamsEnemyCommon.mDirty.get(),
					           mDmgParamsEnemyCommon.mInvincibleTime.get());

					changePlayerStatus(0x000208B7, 1, false);

					if (gpMSound->gateCheck(0x1813)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    0x1813, (const Vec*)&mPosition, 0, nullptr, 0, 4);
					}
					gpMarioParticleManager->emitAndBindToPosPtr(
					    6, (const JGeometry::TVec3<f32>*)&mPosition, 0, nullptr);
					return 1;
				}
			}
		default_enemy_fallthrough:
			{
				f32 height = sender->mAttackHeight;
				keepDistance(sender->mPosition, 0.0f + height, 0.0f);
			}
			return 1;
		}
	}

	// Check sender type for various other actor types
	{
		u32 aType = sender->mActorType;

		switch (aType) {
		case 0x080000C0: // Water surface contact
		{
			if (mAction == 0x1336)
				return 0;
			if (message != 4)
				return 0;

			unk384 = (THitActor*)sender;
			// Check state flag for grounded
			if (!(mAction & 0x800)) {
				setAnimation(77, 0.0f);
				J3DFrameCtrl& ctrl = getMotionFrameCtrl();
				s16 frameAngle = *(s16*)((u8*)&ctrl + 8);
				getMotionFrameCtrl();
				// Convert s16 to float and set
				// ... complex frame setting
			}
			changePlayerDropping(0x1336, 0);
			return 1;
		}

		case 0x08000013: // NPC interaction (talk)
		{
			if (message == 8) {
				if (message == 8) {
					switch (message) {
					case 4:
						// Take
						if (*(u32*)((u8*)this + 0x6C) != 0) return 0;
						if (*(u32*)((u8*)this + 0x68) != 0) return 0;
						unk384 = (THitActor*)sender;
						changePlayerStatus(0x0C400201, 0, false);
						return 1;
					case 8:
						// Release
						if (checkFlag(0x1000)) {
							changePlayerStatus(0x891, 0, true);
						} else {
							changePlayerStatus(0x0C400201, 0, false);
						}
						*(u32*)((u8*)this + 0x68) = 0;
						return 1;
					default:
						return 0;
					}
				}
			}
			return 0;
		}

		case 0x08000001: // Basic enemy contact/damage
		{
			if (message == 0x0E) {
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsEnemyMario.mDamage.get(),
					           mDmgParamsEnemyMario.mDownType.get(),
					           mDmgParamsEnemyMario.mWaterEmit.get(),
					           mDmgParamsEnemyMario.mMinSpeed.get(),
					           mDmgParamsEnemyMario.mMotor.get(),
					           mDmgParamsEnemyMario.mDirty.get(),
					           mDmgParamsEnemyMario.mInvincibleTime.get());
					return 1;
				}
			}
			return 0;
		}

		case 0x08000003:
		case 0x0800001F:
		case 0x08000027: // General enemy interaction
		{
			switch (message) {
			case 4: // Take
				if (!isInvincible()) {
					if (*(u32*)((u8*)this + 0x6C) != 0) return 0;
					if (*(u32*)((u8*)this + 0x68) != 0) return 0;
					unk384 = (THitActor*)sender;
					changePlayerStatus(0x10020370, 0, false);
					return 1;
				}
				return 0;
			case 0x0E: // Touch
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsHanachanBoss.mDamage.get(),
					           mDmgParamsHanachanBoss.mDownType.get(),
					           mDmgParamsHanachanBoss.mWaterEmit.get(),
					           mDmgParamsHanachanBoss.mMinSpeed.get(),
					           mDmgParamsHanachanBoss.mMotor.get(),
					           mDmgParamsHanachanBoss.mDirty.get(),
					           mDmgParamsHanachanBoss.mInvincibleTime.get());
					return 1;
				}
				return 0;
			case 8: // Release
				if (checkFlag(0x1000)) {
					changePlayerStatus(0x891, 0, true);
				} else {
					changePlayerStatus(0x0C400201, 0, false);
				}
				*(u32*)((u8*)this + 0x68) = 0;
				return 1;
			default:
				return 0;
			}
		}

		case 0x08000029: // Hit from enemy
		{
			if (message == 0x0A) {
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsKiller.mDamage.get(),
					           mDmgParamsKiller.mDownType.get(),
					           mDmgParamsKiller.mWaterEmit.get(),
					           mDmgParamsKiller.mMinSpeed.get(),
					           mDmgParamsKiller.mMotor.get(),
					           mDmgParamsKiller.mDirty.get(),
					           mDmgParamsKiller.mInvincibleTime.get());

					changePlayerStatus(0x000208B7, 1, false);

					if (gpMSound->gateCheck(0x1813)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    0x1813, (const Vec*)&mPosition, 0, nullptr, 0, 4);
					}
					gpMarioParticleManager->emitAndBindToPosPtr(
					    6, (const JGeometry::TVec3<f32>*)&mPosition, 0, nullptr);
					return 1;
				}
			}
			return 0;
		}

		case 0x0800002A:
		case 0x0800002C: // Hit from object
		{
			if (message == 0x0E) {
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsFire.mDamage.get(),
					           mDmgParamsFire.mDownType.get(),
					           mDmgParamsFire.mWaterEmit.get(),
					           mDmgParamsFire.mMinSpeed.get(),
					           mDmgParamsFire.mMotor.get(),
					           mDmgParamsFire.mDirty.get(),
					           mDmgParamsFire.mInvincibleTime.get());
					return 1;
				}
			}
			goto default_enemy_contact;
		}

		case 0x0800000B: // Specific enemy damage
		{
			if (message == 0x0A) {
				if (!isInvincible()) {
					damageExec(sender,
					           mDmgParamsKiller.mDamage.get(),
					           mDmgParamsKiller.mDownType.get(),
					           mDmgParamsKiller.mWaterEmit.get(),
					           mDmgParamsKiller.mMinSpeed.get(),
					           mDmgParamsKiller.mMotor.get(),
					           mDmgParamsKiller.mDirty.get(),
					           mDmgParamsKiller.mInvincibleTime.get());

					changePlayerStatus(0x000208B7, 1, false);

					if (gpMSound->gateCheck(0x1813)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    0x1813, (const Vec*)&mPosition, 0, nullptr, 0, 4);
					}
					gpMarioParticleManager->emitAndBindToPosPtr(
					    6, (const JGeometry::TVec3<f32>*)&mPosition, 0, nullptr);
					return 1;
				}
			}
			return 0;
		}

		case 0x0800002D: // Hit and release
		{
			if (message == 8) {
				changePlayerStatus(0x0C400201, 0, false);
				*(u32*)((u8*)this + 0x6C) = 0;
				return 0;
			}
			return 0;
		}

		case 0x4000002C: // Wire actor grab
		{
			if (!(mInput & 0x8000))
				return 0;
			s16 attackAngle = getAttackAngle(sender);
			s16 angleDiff = (s16)(attackAngle - mFaceAngle.y);
			s16 sAngle = (s16)(65536.0f * sender->mAttackHeight * 1.0f);
			s16 adjAngle = (s16)(sAngle - mFaceAngle.y);
			if (angleDiff > -8192 && angleDiff < 8192) {
				if (adjAngle > -8192 && adjAngle < 8192) {
					mPosition.x = sender->mPosition.x;
					mPosition.y = sender->mPosition.y;
					mPosition.z = sender->mPosition.z;
					mFaceAngle.y = sAngle;
					changePlayerStatus(0x1320, 0, false);
					if (*(u32*)((u8*)this + 0x6C)) {
						mPumpState = 2;
						setAnimation(233, 0.0f);
					} else {
						setAnimation(95, 0.0f);
					}
					startVoice(0x78E5);
					return 1;
				}
			}
			if (adjAngle < -24576 || adjAngle > 24576) {
				if (message == 0x11) {
					mPosition.x = sender->mPosition.x;
					mPosition.y = sender->mPosition.y;
					mPosition.z = sender->mPosition.z;
					s16 newAngle = sAngle + 0x8000;
					mFaceAngle.y = newAngle;
					changePlayerStatus(0x1321, 0, false);
					if (*(u32*)((u8*)this + 0x6C)) {
						mPumpState = 2;
						setAnimation(233, 0.0f);
					} else {
						setAnimation(96, 0.0f);
					}
					startVoice(0x78E5);
					return 1;
				}
				if (*(u32*)((u8*)this + 0x6C))
					return 0;
				mPosition.x = sender->mPosition.x;
				mPosition.y = sender->mPosition.y;
				mPosition.z = sender->mPosition.z;
				s16 newAngle2 = sAngle + 0x8000;
				mFaceAngle.y = newAngle2;
				changePlayerStatus(0x1321, 0, false);
				setAnimation(313, 0.0f);
				return 0;
			}
			return 0;
		}

		case 0x80000001: // Player contact
		{
			if (!isInvincible()) {
				if (message >= 8)
					goto handle_player_msg;
				if (message == 5)
					return 0;
				if (message >= 6)
					goto handle_player_throw;
				if (message >= 4)
					goto handle_player_take;
				return 0;

			handle_player_msg:
				if (message == 0x0E)
					goto handle_player_damage;
				return 0;

			handle_player_take:
				if (*(u32*)((u8*)this + 0x6C) != 0) return 0;
				if (*(u32*)((u8*)this + 0x68) != 0) return 0;
				unk384 = (THitActor*)sender;
				changePlayerStatus(0x10020370, 0, false);
				return 1;

			handle_player_throw:
				*(u32*)((u8*)this + 0x68) = 0;
				changePlayerStatus(0x02000880, 0, false);
				setPlayerVelocity(0.0f);
				mVel.y = 0.0f;
				unk78 &= ~0x100;
				return 1;

			handle_player_damage:
				damageExec(sender,
				           mDmgParamsEnemyMario.mDamage.get(),
				           mDmgParamsEnemyMario.mDownType.get(),
				           mDmgParamsEnemyMario.mWaterEmit.get(),
				           mDmgParamsEnemyMario.mMinSpeed.get(),
				           mDmgParamsEnemyMario.mMotor.get(),
				           mDmgParamsEnemyMario.mDirty.get(),
				           mDmgParamsEnemyMario.mInvincibleTime.get());
				return 1;
			}
			return 0;
		}

		case 0x08000005: // NPC greeting
			return 0;

		case 0x40000032:
		case 0x40000038:
		case 0x0800002B:
		case 0x40000033:
		case 0x40000246:
		case 0x40000393:
			return 0;

		case 0x40000064:
		case 0x4000009C:
		case 0x400000A5:
			// Damage touch (keepDistance with rumble)
			if (message == 0x0E) {
				if (*(s16*)((u8*)this + 0x150) > 0)
					return 0;
				s16 timerVal = *(s16*)((u8*)this + 0x938);
				*(s16*)((u8*)this + 0x14E) = timerVal;
				rumbleStart(21, *(s16*)((u8*)this + 0x27F8));
				calcDamagePos(sender->mPosition);
				kickFruitEffect();
				return 1;
			}
			return 0;

		default:
		default_msg:
		default_enemy_contact:
			return 0;
		}
	}

	return 0;
}

void TMario::getGesso(THitActor* sender)
{
	if (mAction == 0x10000)
		return;

	s16 angle = (s16)(65536.0f * sender->mRotation.y);
	mFaceAngle.y = angle;
	mModelFaceAngle = (s16)mFaceAngle.y;

	changePlayerStatus(0x810446, 0, false);

	*(u16*)((u8*)this + 0x86) = *(u16*)((u8*)this + 0x94C);
	emitGetEffect();

	// Set gesso type based on sender's actor type
	u32 aType = sender->mActorType;
	if (aType == 0x400000C5) {
		mSurfGesso = gpMapObjManager->mRedGesso;
		unk389 = 0;
	} else if (aType == 0x400000C6) {
		mSurfGesso = gpMapObjManager->mYellowGesso;
		unk389 = 1;
	} else {
		unk389 = 2;
		mSurfGesso = gpMapObjManager->mGreenGesso;
	}

	mSurfGesso->setBck("surfgesso_wait01");
	mSurfGesso->getFrameCtrl(0)->setRate(1.0f);
}
