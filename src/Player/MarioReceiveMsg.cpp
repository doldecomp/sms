#include <Player/Mario.hpp>
#include <Player/WaterGun.hpp>
#include <Player/MarioCap.hpp>
#include <Map/MapWire.hpp>
#include <Map/MapWireManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjItem2.hpp>
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
	onFlag(MARIO_FLAG_HAS_FLUDD);
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
		changePlayerStatus(MARIO_STATUS_SURF, 0, false);
		mStatusTimer = mDeParams.mSurfStartFreezeTime.get();
		emitGetEffect();
		switch (param_1->getActorType()) {
		case ACTOR_TYPE_SURF_GESO_RED:
			mSurfGesso     = gpMapObjManager->mRedGesso;
			mSurfGessoType = SURF_GESSO_TYPE_RED;
			break;

		case ACTOR_TYPE_SURF_GESO_YELLOW:
			mSurfGesso     = gpMapObjManager->mYellowGesso;
			mSurfGessoType = SURF_GESSO_TYPE_YELLOW;
			break;

		default:
		case ACTOR_TYPE_SURF_GESO_GREEN:
			mSurfGessoType = SURF_GESSO_TYPE_GREEN;
			mSurfGesso     = gpMapObjManager->mGreenGesso;
			break;
		}
		mSurfGesso->setBck("surfgeso_run1");
		mSurfGesso->getFrameCtrl(ANM_TYPE_BCK)->setRate(0.5f);
	}
}

void TMario::getCoin()
{
	++mCoinCount;
	emitGetCoinEffect(&mPosition);
	incHP(1);
	if ((TFlagManager::getInstance()->getFlag(MSF_GOLD_COIN_COUNT) % 50) == 0) {
		TFlagManager::getInstance()->incMario(1);
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_1UP, 0, nullptr, 0);
	}
}

void TMario::getCoinRed()
{
	incHP(2);
	emitGetCoinEffect(&mPosition);
	int id = TFlagManager::getInstance()->getFlag(MSF_RED_COIN_COUNT) + 70;
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

	if (sender->isHitCategory(HIT_CATEGORY_ITEM)) {
		bool playThump = true;
		if (sender->mActorType == ACTOR_TYPE_COIN)
			playThump = false;
		if (sender->mActorType == ACTOR_TYPE_COIN_RED)
			playThump = false;
		if (sender->mActorType == ACTOR_TYPE_COIN_BLUE)
			playThump = false;
		if (sender->mActorType == ACTOR_TYPE_HIDE_OBJ)
			playThump = false;
		if (sender->mActorType == ACTOR_TYPE_SHINE)
			playThump = false;
		if (sender->mActorType == ACTOR_TYPE_WATERGUN_ITEM)
			playThump = false;
		if (sender->mActorType == ACTOR_TYPE_NORMAL_NOZZLE_ITEM)
			playThump = false;
		if (sender->mActorType == ACTOR_TYPE_ROCKET_NOZZLE_ITEM)
			playThump = false;
		if (sender->mActorType == ACTOR_TYPE_BACK_NOZZLE_ITEM)
			playThump = false;

		if (playThump == true)
			SMSGetMSound()->startSoundActor(MSD_SE_MA_GET_ITEM, &mPosition, 0,
			                                nullptr, 0, 4);
	}

	if (message == HIT_MESSAGE_THROWN) {
		if (sender->isHitCategory(HIT_CATEGORY_PLAYER)
		    || sender->isHitCategory(HIT_CATEGORY_NPC)
		    || sender->isHitCategory(HIT_CATEGORY_ENEMY)
		    || sender->isHitCategory(HIT_CATEGORY_BOSS)) {
			SMSGetMSound()->startSoundActor(MSD_SE_MA_HANEAGARI, &mPosition, 0,
			                                &mSound, 0, 4);
		}
		if (sender->mActorType == ACTOR_TYPE_POI_HANA)
			startVoice(MSD_SE_MV27_SPRISE_01);

		if (sender->isHitCategory(HIT_CATEGORY_MAP_OBJECT)
		    && gpMSound->getMarioVoiceID(0) != MSD_SE_MV28_SPRISE_SMALL_01) {
			startVoice(MSD_SE_MV28_SPRISE_SMALL_01);
		}
		if (onYoshi())
			SMSGetMSound()->startSoundActor(MSD_SE_YV_SURPRISED, &mPosition, 0,
			                                nullptr, 0, 4);

		changePlayerStatus(MARIO_STATUS_THROWN_DOWN, 0, false);
		return TRUE;
	}

	if (sender->isHitCategory(HIT_CATEGORY_ITEM)) {
		switch (sender->mActorType) {
		case ACTOR_TYPE_BOTTLE_SHORT:
			if (message == HIT_MESSAGE_ATTACK) {
				if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
					mWaterGun->addWater(mWaterGun->getMaxWater() / 2);
				}
				emitGetWaterEffect();
				return TRUE;
			}
			break;
		case ACTOR_TYPE_BOTTLE_LARGE:
			if (message == HIT_MESSAGE_ATTACK) {
				if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
					mWaterGun->addWater(mWaterGun->getMaxWater());
				}
				emitGetWaterEffect();
				return TRUE;
			}
			break;
		case ACTOR_TYPE_BOSS_EEL_TEARS:
			if (message == HIT_MESSAGE_ATTACK) {
				incHP(8);
				emitGetEffect();
				return TRUE;
			}
			break;
		case ACTOR_TYPE_ITEM_UNK3:
			if (message == HIT_MESSAGE_ATTACK) {
				incHP(4);
				if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
					mWaterGun->addWater(mWaterGun->getMaxWater());
				}
				emitGetEffect();
				return TRUE;
			}
			break;
		case ACTOR_TYPE_ITEM_UNK4:
			if (message == HIT_MESSAGE_ATTACK) {
				incHP(1);
				emitGetEffect();
				return TRUE;
			}
			break;
		case ACTOR_TYPE_MUSHROOM1UP:
		case ACTOR_TYPE_MUSHROOM1UP_R:
		case ACTOR_TYPE_MUSHROOM1UP_X:
			if (message == HIT_MESSAGE_ATTACK) {
				TMushroom1up* mushroom = static_cast<TMushroom1up*>(sender);
				if (mushroom->unk13A == 0
				    && !(mushroom->unk13C < 120 ? true : false)) {
					mHealth = mDeParams.mHPMax.get();
					if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
						mWaterGun->addWater(mWaterGun->getMaxWater());
					}
					TFlagManager::getInstance()->incMario(1);
					emitGetEffect();
					return TRUE;
				}
				return FALSE;
			}
			break;

		case ACTOR_TYPE_ROCKET_NOZZLE_ITEM:
			return getNozzle(sender, TWaterGun::Rocket);
		case ACTOR_TYPE_NORMAL_NOZZLE_ITEM:
			return getNozzle(sender, TWaterGun::Hover);
		case ACTOR_TYPE_BACK_NOZZLE_ITEM:
			return getNozzle(sender, TWaterGun::Turbo);
		case ACTOR_TYPE_ITEM_UNK2B:
			changePlayerStatus(MARIO_STATUS_DIVE, 0, false);
			return getNozzle(sender, TWaterGun::Underwater);
		case ACTOR_TYPE_WATERGUN_ITEM:
			getNozzle(sender, TWaterGun::Spray);
			return TRUE;

		case ACTOR_TYPE_MARIO_CAP:
			mCap->setModelActive(TMarioCap::E_CAP_MODEL_HAT);
			mHealth = mDeParams.mHPMax.get();
			emitGetEffect();
			return TRUE;
		case ACTOR_TYPE_COIN:
			getCoin();
			return TRUE;
		case ACTOR_TYPE_COIN_RED:
			getCoinRed();
			return TRUE;
		case ACTOR_TYPE_COIN_BLUE:
			getCoinBlue();
			return TRUE;
		case ACTOR_TYPE_SHINE:
			if (message == HIT_MESSAGE_ATTACK
			    && mStatus != MARIO_STATUS_WIN_DEMO) {
				unk384       = sender;
				mPosition.x  = sender->mPosition.x;
				mPosition.z  = sender->mPosition.z;
				mFaceAngle.y = DEG2SHORTANGLE(
				    static_cast<TMapObjBase*>(sender)->mInitialRotation.y);
				mModelFaceAngle = mFaceAngle.y;
				setPlayerVelocity(0.0f);
				mHealth = mDeParams.mHPMax.get();
				mAir    = mMaxAir;
				changePlayerStatus(MARIO_STATUS_WIN_DEMO, 0, true);
				return TRUE;
			}
			break;
		}
	}

	if (sender->isHitCategory(HIT_CATEGORY_MAP_OBJECT)) {
		switch (sender->getActorType()) {
		case ACTOR_TYPE_LAMPTRAPIRON:
			if (message == HIT_MESSAGE_BURN && !isInvincible()) {
				damageExec(sender, mDmgParamsLampTrapIron.mDamage.get(),
				           mDmgParamsLampTrapIron.mDownType.get(),
				           mDmgParamsLampTrapIron.mWaterEmit.get(),
				           mDmgParamsLampTrapIron.mMinSpeed.get(),
				           mDmgParamsLampTrapIron.mMotor.get(),
				           mDmgParamsLampTrapIron.mDirty.get(),
				           mDmgParamsLampTrapIron.mInvincibleTime.get());
				changePlayerStatus(MARIO_STATUS_FIRE_DOWN, 1, false);
				SMSGetMSound()->startSoundActor(MSD_SE_MA_DAMAGE_FIRE,
				                                &mPosition, 0, nullptr, 0, 4);

				gpMarioParticleManager->emitAndBindToPosPtr(6, &mPosition, 0,
				                                            nullptr);
				return TRUE;
			}
			break;
		case ACTOR_TYPE_LAMPTRAPSPIKE:
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
		case ACTOR_TYPE_SURF_GESO_RED:
		case ACTOR_TYPE_SURF_GESO_YELLOW:
		case ACTOR_TYPE_SURF_GESO_GREEN:
			getGesso(sender);
			return TRUE;
		case ACTOR_TYPE_BATH:
			if (message == HIT_MESSAGE_TAKE) {
				mHolder = (TTakeActor*)sender;
				changePlayerStatus(MARIO_STATUS_NOMOTION, 0, false);
				return TRUE;
			}
			break;
		case ACTOR_TYPE_BATH_WATER:
			if (message == HIT_MESSAGE_BURN) {
				if (!isInvincible()) {
					damageExec(sender, mDmgParamsFire.mDamage.get(),
					           mDmgParamsFire.mDownType.get(),
					           mDmgParamsFire.mWaterEmit.get(),
					           mDmgParamsFire.mMinSpeed.get(),
					           mDmgParamsFire.mMotor.get(),
					           mDmgParamsFire.mDirty.get(),
					           mDmgParamsFire.mInvincibleTime.get());
					startVoice(MSD_SE_MV27_SPRISE_01);
					changePlayerStatus(MARIO_STATUS_THROWN_DOWN, 1, false);
					return TRUE;
				}
				return FALSE;
			}
			break;
		case ACTOR_TYPE_MAP_WIRE_ACTOR: {
			if (mHolder == nullptr) {
				if (mStatus == MARIO_STATUS_WIRE_JUMP && mVel.y > 0.0f)
					return FALSE;
				if (mStatus == MARIO_STATUS_WIRE_ROLL_JUMP && mVel.y > 0.0f)
					return FALSE;
				if (mStatus == MARIO_STATUS_WIRE_HANG_LAND_SAFE_DOWN)
					return FALSE;
				if (onYoshi())
					return FALSE;
				mHolder             = (TTakeActor*)sender;
				TMapWireActor* wire = (TMapWireActor*)sender;
				wire->getTipPoints(&mWireStartPos, &mWireEndPos);
				mWirePosRatio = wire->getPosInWire();
				wireMove(0.0f);
				offFlag(MARIO_FLAG_UNK100);

				JGeometry::TVec3<f32> diff = mWireEndPos - mWireStartPos;
				int dirToEnd   = (s16)(matan(diff.z, diff.x) - mFaceAngle.y);
				mWireBounceVel = 0.2f * -mVel.y;
				mWireSag       = 0.0f;

				bool flipDir;
				if (mStatus == MARIO_STATUS_WIRE_ROLL_JUMP
				    || mStatus == MARIO_STATUS_JUMP_CATCH) {
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
				changePlayerStatus(MARIO_STATUS_WIRE_WAIT, 0, false);
				return TRUE;
			} else {
				return FALSE;
			}
		}
		}
	}

	if (sender->isHitCategory(HIT_CATEGORY_ENEMY)) {
		switch (sender->getActorType()) {
		case ACTOR_TYPE_NAME_KURI:
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

		case ACTOR_TYPE_DORO_HANE_KURI:
		case ACTOR_TYPE_DORO_HAMU_KURI:
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

		case ACTOR_TYPE_SROTDRAM:
		case ACTOR_TYPE_HAMU_KURI:
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
		case ACTOR_TYPE_ELEC_NOKONOKO:
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

		case ACTOR_TYPE_ELEC_CARAPACE:
		case ACTOR_TYPE_AMI_NOKO:
		case ACTOR_TYPE_AMIKING:
			if (message == HIT_MESSAGE_ELECTRIC_SHOCK && !isInvincible()) {
				elecEffect();
				changePlayerStatus(MARIO_STATUS_ELECTRIC_DAMAGE, 0, false);
				return TRUE;
			}
			keepDistance(sender->mPosition, sender->getDamageRadius() + 30.0f,
			             0.0f);
			return TRUE;

		case ACTOR_TYPE_ENEMY_UNK1F:
			if ((message == HIT_MESSAGE_BURN || message == HIT_MESSAGE_ATTACK)
			    && !isInvincible()) {
				damageExec(sender, mDmgParamsKiller.mDamage.get(),
				           mDmgParamsKiller.mDownType.get(),
				           mDmgParamsKiller.mWaterEmit.get(),
				           mDmgParamsKiller.mMinSpeed.get(),
				           mDmgParamsKiller.mMotor.get(),
				           mDmgParamsKiller.mDirty.get(),
				           mDmgParamsKiller.mInvincibleTime.get());
				changePlayerStatus(MARIO_STATUS_FIRE_DOWN, 1, false);
				SMSGetMSound()->startSoundActor(MSD_SE_MA_DAMAGE_FIRE,
				                                &mPosition, 0, nullptr, 0, 4);

				gpMarioParticleManager->emitAndBindToPosPtr(6, &mPosition, 0,
				                                            nullptr);
				return TRUE;
			}
			break;

		case ACTOR_TYPE_BOSS_PAKKUN:
		case ACTOR_TYPE_BOSS_PAKKUN_ATTACK:
		case ACTOR_TYPE_BOSS_PAKKUN_HEAD:
		case ACTOR_TYPE_BOSS_UNK12:
		case ACTOR_TYPE_PAKKUN:
		case ACTOR_TYPE_POLLUTE_OBJ:
		case ACTOR_TYPE_GESSO:
		case ACTOR_TYPE_MAME_GESSO:
		case ACTOR_TYPE_ENEMY_UNK9:
		case ACTOR_TYPE_TELESA:
		case ACTOR_TYPE_DANGO_HAMU_KURI:
		case ACTOR_TYPE_PUKU_PUKU:
		case ACTOR_TYPE_LAUNCHER:
		case ACTOR_TYPE_CHUU_HANA:
		case ACTOR_TYPE_IGAIGA:
		case ACTOR_TYPE_TAMA_NOKO:
		case ACTOR_TYPE_GOROGORO:
		case ACTOR_TYPE_HANA_SAMBO:
		case ACTOR_TYPE_SAMBO_HEAD:
		case ACTOR_TYPE_DPT_CANNON:
		case ACTOR_TYPE_ENEMY_UNK1D:
		case ACTOR_TYPE_EFFECT_OBJ:
		case ACTOR_TYPE_BIANCO_GATE_KEEPER:
		case ACTOR_TYPE_SEAL:
		case ACTOR_TYPE_HAUNT_LEG:
		case ACTOR_TYPE_KAZEKUN:
		case ACTOR_TYPE_YUMBO:
		case ACTOR_TYPE_KUMOKUN:
		case ACTOR_TYPE_AMENBO:
		case ACTOR_TYPE_ENEMY_UNK2E:
		case ACTOR_TYPE_ENEMY_UNK2F:
		case ACTOR_TYPE_DEBU_TELESA:
		case ACTOR_TYPE_ENEMY_DAMAGE_OBJ:
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
		case ACTOR_TYPE_DAMAGE_OBJ:
			if (checkFlag(MARIO_FLAG_IN_ANY_WATER)
			    && message == HIT_MESSAGE_ATTACK && !isInvincible()) {
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
		case ACTOR_TYPE_ROCKET:
		case ACTOR_TYPE_POPO:
			if (message == HIT_MESSAGE_ATTACH) {
				if (checkFlag(MARIO_FLAG_HAS_FLUDD))
					mWaterGun->onFlag(TWaterGun::WATER_GUN_FLAG_UNK4);
			}
			break;
		case ACTOR_TYPE_POI_HANA:
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
		case ACTOR_TYPE_EFFECT_ENEMY:
		case ACTOR_TYPE_FIRE_WANWAN:
		case ACTOR_TYPE_FIRE_HAMU_KURI:
		case ACTOR_TYPE_BOMB_HEI:
		case ACTOR_TYPE_ENEMY_UNK26:
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
				changePlayerStatus(MARIO_STATUS_FIRE_DOWN, 1, false);
				SMSGetMSound()->startSoundActor(MSD_SE_MA_DAMAGE_FIRE,
				                                &mPosition, 0, nullptr, 0, 4);

				gpMarioParticleManager->emitAndBindToPosPtr(6, &mPosition, 0,
				                                            nullptr);
				return TRUE;
			}
			break;
		}
	}

	switch (sender->mActorType) {
	case ACTOR_TYPE_BOSS_UNK29:
		if (message == HIT_MESSAGE_BURN) {
			if (!isInvincible()) {
				damageExec(sender, mDmgParamsFire.mDamage.get(),
				           mDmgParamsFire.mDownType.get(),
				           mDmgParamsFire.mWaterEmit.get(),
				           mDmgParamsFire.mMinSpeed.get(),
				           mDmgParamsFire.mMotor.get(),
				           mDmgParamsFire.mDirty.get(),
				           mDmgParamsFire.mInvincibleTime.get());
				changePlayerStatus(MARIO_STATUS_FIRE_DOWN, 1, false);
				SMSGetMSound()->startSoundActor(MSD_SE_MA_DAMAGE_FIRE,
				                                &mPosition, 0, nullptr, 0, 4);

				gpMarioParticleManager->emitAndBindToPosPtr(6, &mPosition, 0,
				                                            nullptr);
			}
			return TRUE;
		}
		return FALSE;

	case ACTOR_TYPE_BOSS_UNK2A:
	case ACTOR_TYPE_BOSS_UNK2C:
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
		// fallthrough

	case ACTOR_TYPE_HINOKURI2:
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
		if (message == HIT_MESSAGE_SUPER_HIP_DROP && !isInvincible()) {
			onFlag(MARIO_FLAG_GROUND_POUND_SIT_UP);
			if (!checkStatusType(MARIO_STATUS_FLAG_JUMPING)) {
				rumbleStart(0x15, 0x0A);
			}
			return TRUE;
		}
		break;

	case ACTOR_TYPE_BOSS_GESSO:
		break;

	case ACTOR_TYPE_BOSS_UNK13:
		switch (message) {
		case HIT_MESSAGE_TAKE:
			if (mHeldObject == nullptr && mHolder == nullptr) {
				mHolder = (TTakeActor*)sender;
				changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
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
		case HIT_MESSAGE_DETACH:
			if (checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)) {
				changePlayerStatus(MARIO_STATUS_DIVE, 0, true);
			} else {
				changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
			}
			mHolder = nullptr;
			return TRUE;
		}
		break;

	case ACTOR_TYPE_BOSS_UNKB:
	case ACTOR_TYPE_BOSS_UNKC:
	case ACTOR_TYPE_BOSS_UNKD:
	case ACTOR_TYPE_BOSS_UNKE:
		if (message == HIT_MESSAGE_BURN && !isInvincible()) {
			damageExec(sender, mDmgParamsFire.mDamage.get(),
			           mDmgParamsFire.mDownType.get(),
			           mDmgParamsFire.mWaterEmit.get(),
			           mDmgParamsFire.mMinSpeed.get(),
			           mDmgParamsFire.mMotor.get(), mDmgParamsFire.mDirty.get(),
			           mDmgParamsFire.mInvincibleTime.get());
			changePlayerStatus(MARIO_STATUS_FIRE_DOWN, 1, false);
			SMSGetMSound()->startSoundActor(MSD_SE_MA_DAMAGE_FIRE, &mPosition,
			                                0, nullptr, 0, 4);

			gpMarioParticleManager->emitAndBindToPosPtr(6, &mPosition, 0,
			                                            nullptr);
			return TRUE;
		}
		break;

	case ACTOR_TYPE_BOSS_EEL:
	case ACTOR_TYPE_BOSS_MANTA:
	case ACTOR_TYPE_BOSS_GESSO_TENTACLE:
	case ACTOR_TYPE_BOSS_GESSO_TAKE_HIT:
	case ACTOR_TYPE_BOSS_UNK8:
	case ACTOR_TYPE_COASTER_KILLER:
	case ACTOR_TYPE_BOSS_EEL_TOOTH:
	case ACTOR_TYPE_BOSS_EEL_COLLISION:
	case ACTOR_TYPE_BOSS_UNK27:
	case ACTOR_TYPE_HANE_HAMU_KURI:
	case ACTOR_TYPE_ENEMY_UNK35:
		switch (message) {
		case HIT_MESSAGE_TAKE:
			if (!isInvincible() && mHeldObject == nullptr
			    && mHolder == nullptr) {
				mHolder = (TTakeActor*)sender;
				changePlayerStatus(MARIO_STATUS_TAKEN, 0, false);
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
		case HIT_MESSAGE_DETACH:
			if (checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)) {
				changePlayerStatus(MARIO_STATUS_DIVE, 0, true);
			} else {
				changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
			}
			mHolder = nullptr;
			return TRUE;
		}
		break;

	case ACTOR_TYPE_BATHTUB_KILLER:
		if (!isInvincible() && message == HIT_MESSAGE_ATTACK
		    && (((mStatus - 0x800000) != 0x8A9) || mStatusState != 3)) {
			damageExec(sender, mDmgParamsBGTentacle.mDamage.get(),
			           mDmgParamsBGTentacle.mDownType.get(),
			           mDmgParamsBGTentacle.mWaterEmit.get(),
			           mDmgParamsBGTentacle.mMinSpeed.get(),
			           mDmgParamsBGTentacle.mMotor.get(),
			           mDmgParamsBGTentacle.mDirty.get(),
			           mDmgParamsBGTentacle.mInvincibleTime.get());
			changePlayerStatus(MARIO_STATUS_THROWN_DOWN, 0, false);
			return TRUE;
		}
		// fallthrough

	case ACTOR_TYPE_BOSS_UNK14:
	case ACTOR_TYPE_BOSS_UNK15:
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
		break;

	case ACTOR_TYPE_DOOR: {
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
					changePlayerStatus(MARIO_STATUS_DOOR_OPEN_R, 0, false);
					if (isHolding()) {
						mUpperState = UPPER_STATE_HOLDING_OBJECT;
						setAnimation(ANIM_DOOR_KICK, 1.0f);
					} else {
						setAnimation(ANIM_DOOR_OPENR, 1.0f);
					}
					startVoice(MSD_SE_MV31_OPEN_DOOR_01);
					return TRUE;
				}
				if (frontDiff < -0x6000 || frontDiff > 0x6000) {
					if (message == HIT_MESSAGE_UNK11) {
						mPosition    = sender->mPosition;
						mFaceAngle.y = frontYaw + 0x8000;
						changePlayerStatus(MARIO_STATUS_DOOR_OPEN_L, 0, false);
						if (isHolding()) {
							mUpperState = UPPER_STATE_HOLDING_OBJECT;
							setAnimation(ANIM_DOOR_KICK, 1.0f);
						} else {
							setAnimation(ANIM_DOOR_OPENL, 1.0f);
						}
						startVoice(MSD_SE_MV31_OPEN_DOOR_01);
						return TRUE;
					}
					if (!isHolding()) {
						mPosition    = sender->mPosition;
						mFaceAngle.y = frontYaw + 0x8000;
						changePlayerStatus(MARIO_STATUS_DOOR_OPEN_L, 0, false);
						setAnimation(ANIM_DOOR_GACHA_L, 1.0f);
						return FALSE;
					}
				}
			}
		}
		break;
	}

	case ACTOR_TYPE_E_MARIO:
	case ACTOR_TYPE_MARIO:
		if (!isInvincible()) {
			switch (message) {
			case HIT_MESSAGE_TAKE:
				if (mHeldObject == nullptr && mHolder == nullptr) {
					mHolder = (TTakeActor*)sender;
					changePlayerStatus(MARIO_STATUS_TAKEN, 0, false);
					return TRUE;
				}
				break;
			case HIT_MESSAGE_PUT:
			case HIT_MESSAGE_THROWN:
				mHolder = nullptr;
				changePlayerStatus(MARIO_STATUS_JUMP, 0, false);
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

	case ACTOR_TYPE_DPTLIGHT:
	case ACTOR_TYPE_TELEGRAPH_POLE_L:
	case ACTOR_TYPE_MAP_OBJECT_UNK32:
	case ACTOR_TYPE_PALM_NORMAL:
	case ACTOR_TYPE_PALM_OUGI:
	case ACTOR_TYPE_PALM_SAGO:
	case ACTOR_TYPE_PALM_NATUME:
	case ACTOR_TYPE_BANANA_TREE:
	case ACTOR_TYPE_FRUIT_TREE:
	case ACTOR_TYPE_WOOD_BARREL:
		if (message == HIT_MESSAGE_DETACH) {
			changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
			mHeldObject = nullptr;
		}
		break;

	case ACTOR_TYPE_MODEL_GATE:
		if (mStatus != MARIO_STATUS_WARP_IN && message == HIT_MESSAGE_TAKE) {
			mHolder = (TTakeActor*)sender;
			if (!checkStatusType(MARIO_STATUS_FLAG_JUMPING)) {
				setAnimation(ANIM_JUMP, 1.0f);
				s16 endFrame = getMotionFrameCtrl().getEnd();
				getMotionFrameCtrl().setFrame((f32)endFrame);
			}
			changePlayerDropping(MARIO_STATUS_WARP_IN, 0);
			return TRUE;
		}
		break;

	case ACTOR_TYPE_LEAF_BOAT_ROTTEN:
	case ACTOR_TYPE_LEAF_BOAT:
		if (message == HIT_MESSAGE_ATTACK) {
			keepDistance(sender->mPosition, sender->getDamageRadius() + 30.0f,
			             0.0f);
			return TRUE;
		}
		break;

	case ACTOR_TYPE_FOOTBALL:
	case ACTOR_TYPE_FRUIT_DURIAN:
		if (mFreezeImmunityTimer <= 0) {
			mFreezeTimer = mDeParams.mKickFreezeTime.get();
			rumbleStart(0x15, mMotorParams.mMotorWall.get());
			calcDamagePos(sender->mPosition);
			kickFruitEffect();
			return TRUE;
		}
		break;

	case ACTOR_TYPE_MONTE_GOAL_FLAG:
		break;
	}

	return FALSE;
}
