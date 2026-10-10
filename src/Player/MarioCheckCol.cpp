#include <Player/Mario.hpp>
#include <Player/WaterGun.hpp>
#include <Player/Yoshi.hpp>
#include <Player/ModelWaterManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <MoveBG/MapObjItem2.hpp>
#include <NPC/NpcBase.hpp>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMario::hitNormal(THitActor* actor)
{
	if (checkStatusType(MARIO_STATUS_FLAG_JUMPING) && mVel.y < 0.0f
	    && actor->mPosition.y < mPosition.y) {
		if (mStatus == MARIO_STATUS_HIP_DROP) {
			if (actor->receiveMessage(this, HIT_MESSAGE_HIP_DROP)) {
				if (actor->isActorType(ACTOR_TYPE_HINOKURI2)) {
					changePlayerTriJump();
					unk78 &= ~0x100;
				}
			}
			return;
		}

		if (trampleExec(actor) == TRUE) {
			return;
		}
	}

	if (checkFlag(MARIO_FLAG_UNK200) && actor->mPosition.y > mPosition.y) {
		actor->receiveMessage(this, HIT_MESSAGE_SUPER_HIP_DROP);
		return;
	}

	if (mStatus == MARIO_STATUS_CATCH || mStatus == MARIO_STATUS_OIL_SLIP
	    || mStatus == MARIO_STATUS_JUMP_CATCH) {
		actor->receiveMessage(this, HIT_MESSAGE_PUNCH);
		actor->receiveMessage(this, HIT_MESSAGE_TRAMPLE);
	}

	TWaterGun* wg = mWaterGun;
	if ((int)wg->mCurrentNozzle == 0 && wg->mIsEmitWater != 0) {
		TModelWaterManager::mStaticHitActor.mPosition = mPosition;
		TModelWaterManager::mStaticHitActor.mPosition.y += 80.0f;
		TModelWaterManager::mStaticHitActor.unk68 = 0;
		actor->receiveMessage(&TModelWaterManager::mStaticHitActor,
		                      HIT_MESSAGE_SPRAYED_BY_WATER);
	}
}

// TODO: wrong size! maybe we return the receiveMessage result?
void TMario::hitHipDrop(THitActor* actor)
{
	if (mStatus == MARIO_STATUS_HIP_DROP && mStatusState == 2
	    && actor->mPosition.y < mPosition.y) {
		actor->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
	}
}

void TMario::hitPushup(THitActor* actor)
{
	if (checkStatusType(MARIO_STATUS_FLAG_JUMPING) && mVel.y > 0.0f)
		actor->receiveMessage(this, HIT_MESSAGE_PUSH_UP);
	hitNormal(actor);
}

void TMario::hitPull(THitActor*) { }

void TMario::hitMario(THitActor* actor)
{
	if (mHeldObject != actor && mHolder != actor)
		keepDistance(*actor, 0.0f);
	wantToTakeActor(actor);
	hitNormal(actor);
}

void TMario::hitNpc(THitActor* actor)
{
	if (!checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)
	    && !checkStatusType(MARIO_FLAG_HELMET)
	    && checkStatusType(MARIO_STATUS_FLAG_JUMPING)
	    && mStatus != MARIO_STATUS_HIP_DROP && mVel.y < 0.0f
	    && actor->mPosition.y < mPosition.y
	    && ((TBaseNPC*)actor)->isBeTrampledNpc()) {
		if (trampleExec(actor) == TRUE)
			return;
	}

	keepDistance(*actor, 0.0f);

	if (((TLiveActor*)actor)->checkLiveFlag(LIVE_FLAG_UNK100000))
		wantToTakeActor(actor);
}

void TMario::hitPool(THitActor*) { }

void TMario::wantToTakeActor(THitActor* actor)
{
	if (canTake(actor)) {
		unk384 = actor;
		changePlayerStatus(MARIO_STATUS_TAKE, 0, false);
	}
}

void TMario::hitWantToTake(THitActor* actor)
{
	keepDistance(*actor, 0.0f);
	wantToTakeActor(actor);
}

void TMario::hitBarrel(THitActor* actor)
{
	hitWantToTake(actor);
	if (checkStatusType(MARIO_STATUS_FLAG_JUMPING) && mVel.y < 0.0f
	    && actor->mPosition.y < mPosition.y
	    && mStatus == MARIO_STATUS_HIP_DROP) {
		actor->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
		if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
			TWaterGun* wg     = mWaterGun;
			wg->mCurrentWater = (s32)((const TWaterGun*)wg)
			                        ->getCurrentNozzle()
			                        ->mAmountMax.get();
		}
	}
}

void TMario::hitJumpBase(THitActor* actor)
{
	keepDistance(*actor, 0.0f);
	if ((s8)((TJumpBase*)actor)->unk138 == 0)
		wantToTakeActor(actor);
}

void TMario::hitBrakable(THitActor* actor)
{
	if (checkStatusType(MARIO_STATUS_FLAG_JUMPING) && mVel.y < 0.0f
	    && actor->mPosition.y < mPosition.y
	    && mStatus == MARIO_STATUS_HIP_DROP) {
		actor->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
	}
}

void TMario::hangPole(THitActor* actor)
{
	if (!checkStatusType(MARIO_STATUS_FLAG_UNK100000)) {
		// TODO: dirty, needs inlines
		u8 canHang = 0;
		if (mHeldObject == nullptr && !onYoshi())
			canHang = 1;

		u8 inHangStatus;
		if (canHang == 0) {
			inHangStatus = 0;
		} else {
			// TODO: inlines
			u32 statLo = mStatus & MARIO_STATUS_TYPE_AND_ID_MASK;
			if (statLo >= 0x80 && statLo <= 0x9F) {
				inHangStatus = 1;
			} else {
				if (checkStatusType(MARIO_STATUS_FLAG_UNK200000))
					inHangStatus = 1;
				else
					inHangStatus = 0;
			}
		}

		if (inHangStatus == 1) {
			f32 dz   = actor->mPosition.z - mPosition.z;
			f32 dx   = actor->mPosition.x - mPosition.x;
			f32 dist = std::sqrtf(dx * dx + dz * dz);
			if (dist == 0.0f)
				dist = 1.0f;

			f32 a = JMASSin(mFaceAngle.y) * (dx / dist)
			        + JMASCos(mFaceAngle.y) * (dz / dist);

			f32 b = 50.0f + actor->getDamageRadius()
			        + mBarParams.mCatchRadius.get();

			bool canCatch = true;
			if (mPrevStatus & MARIO_STATUS_FLAG_UNK100000)
				canCatch = false;

			if (a < mBarParams.mCatchAngle.get())
				canCatch = false;

			if (dist > b)
				canCatch = false;

			if (mPosition.y < 100.0f + actor->mPosition.y)
				canCatch = false;

			if (canCatch == true) {
				dropObject();
				mHolder     = (TTakeActor*)actor;
				mVel.y      = 0.0f;
				mForwardVel = 0.0f;
				changePlayerStatus(MARIO_STATUS_BAR_HANG, 0, false);
				actor->receiveMessage(this, HIT_MESSAGE_ATTACH);
				mHolderHeightDiff = mPosition.y - actor->mPosition.y;
				return;
			}
		}

		keepDistance(actor->mPosition,
		             ((TTakeActor*)actor)->getRadiusAtY(mPosition.y), 0.0f);
	}
}

void TMario::hitPickUpEnemy(THitActor* actor)
{
	if (((TSmallEnemy*)actor)->unk164 != 0
	    && !checkStatusType(MARIO_STATUS_FLAG_JUMPING)) {
		hitWantToTake(actor);
		return;
	}
	hitNormal(actor);
	if (((TSmallEnemy*)actor)->doKeepDistance())
		keepDistance(*actor, 0.0f);
}

void TMario::hitSurfingBoard(THitActor*) { }

// As in we pull but don't "keep" the object, cuz it's a tentacle/tail?
void TMario::hitNoKeepPull(THitActor* actor)
{
	if (mStatus != MARIO_STATUS_PULLING && mStatus != MARIO_STATUS_PULL_JUMP
	    && canTake(actor) && actor->receiveMessage(this, HIT_MESSAGE_TAKE)) {
		changePlayerStatus(MARIO_STATUS_PULLING, 0, false);
		setAnimation(ANIM_HOLD, 1.0f);
		mHeldObject = (TTakeActor*)actor;
	} else {
		hitNormal(actor);
	}
}

void TMario::checkCollision()
{
	if (checkStatusType(MARIO_STATUS_FLAG_UNK1000))
		return;

	TYoshi* yoshi = mYoshi;
	BOOL yoshiActive;
	if (yoshi->mState == TYoshi::STATE_UNMOUNTED
	    || yoshi->mState == TYoshi::STATE_UNK2)
		yoshiActive = 1;
	else
		yoshiActive = 0;

	if (yoshiActive == 1) {
		const JGeometry::TVec3<f32>& yt = yoshi->getTranslation();
		if (yt.y <= mPosition.y && mPosition.y < 100.0f + yt.y) {
			f32 dz   = yt.z - mPosition.z;
			f32 dx   = yt.x - mPosition.x;
			f32 dist = std::sqrtf(dx * dx + dz * dz);

			if (checkStatusType(MARIO_STATUS_FLAG_JUMPING) && !isHolding()
			    && mVel.y < 0.0f && yt.y < mPosition.y && mStatus != 0x89C
			    && mStatus != MARIO_STATUS_THROWN_DOWN
			    && mStatus != MARIO_STATUS_BACK_JUMP && dist < 180.0f) {
				mPosition       = mYoshi->getTranslation();
				mFaceAngle.y    = mYoshi->mEggRotSpeed;
				mModelFaceAngle = mFaceAngle.y;

				if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
					unk3E8              = mWaterGun->mSecondNozzle;
					const TWaterGun* wg = mWaterGun;
					unk3EC              = (f32)(wg->mCurrentWater
                                   / wg->getCurrentNozzle()->mAmountMax.get());
				}
				mYoshi->ride();
				onFlag(MARIO_FLAG_HAS_FLUDD);
				if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
					mWaterGun->changeNozzle(TWaterGun::Yoshi, true);
				}
				changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
				return;
			}

			keepDistance(yt, 80.0f, 0.0f);
		}
	}

	for (s32 i = 0; i < (s32)mColCount; i++) {
		if (mCollisions[i]->isHitCategory(HIT_CATEGORY_NPC)) {
			hitNpc(mCollisions[i]);
			continue;
		}

		// TODO: switch still a bit wrong!
		switch (mCollisions[i]->getActorType()) {
		case ACTOR_TYPE_MARIO:
			hitMario(mCollisions[i]);
			keepDistance(*mCollisions[i], 0.0f);
			break;

		case ACTOR_TYPE_ITEM_UNK8:
		case ACTOR_TYPE_ITEM_UNKA:
		case ACTOR_TYPE_ITEM_UNKC:
			hitNormal(mCollisions[i]);
			break;

		case ACTOR_TYPE_HINOKURI2:
		case ACTOR_TYPE_BOSS_EEL:
		case ACTOR_TYPE_BOSS_UNK13:
		case ACTOR_TYPE_BATHTUB_KILLER:
		case ACTOR_TYPE_ENEMY_UNK1:
		case ACTOR_TYPE_HAMU_KURI:
		case ACTOR_TYPE_NAME_KURI:
		case ACTOR_TYPE_PAKKUN:
		case ACTOR_TYPE_ELEC_NOKONOKO:
		case ACTOR_TYPE_TELESA:
		case ACTOR_TYPE_POPO:
		case ACTOR_TYPE_HANE_HAMU_KURI:
		case ACTOR_TYPE_DANGO_HAMU_KURI:
		case ACTOR_TYPE_FIRE_HAMU_KURI:
		case ACTOR_TYPE_PUKU_PUKU:
		case ACTOR_TYPE_DORO_HAMU_KURI:
		case ACTOR_TYPE_IGAIGA:
		case ACTOR_TYPE_GOROGORO:
		case ACTOR_TYPE_HANA_SAMBO:
		case ACTOR_TYPE_SAMBO_HEAD:
		case ACTOR_TYPE_DPT_CANNON:
		case ACTOR_TYPE_ENEMY_UNK1D:
		case ACTOR_TYPE_ENEMY_UNK1F:
		case ACTOR_TYPE_EFFECT_OBJ:
		case ACTOR_TYPE_HAUNT_LEG:
		case ACTOR_TYPE_ENEMY_UNK2E:
		case ACTOR_TYPE_ENEMY_UNK31:
		case ACTOR_TYPE_DORO_HANE_KURI:
		case ACTOR_TYPE_CASINORULET:
			hitNormal(mCollisions[i]);
			break;

		case ACTOR_TYPE_BOSS_PAKKUN_HEAD:
			hitHipDrop(mCollisions[i]);
			break;

		case ACTOR_TYPE_KUMOKUN:
			hitNormal(mCollisions[i]);
			if (((TSmallEnemy*)mCollisions[i])->doKeepDistance())
				keepDistance(*mCollisions[i], 0.0f);
			// fall through

		case ACTOR_TYPE_AMI_NOKO:
			hitHipDrop(mCollisions[i]);
			// fall through

		case ACTOR_TYPE_AMIKING:
			if (mStatus == MARIO_STATUS_FENCE_PUNCH
			    && 5.0f <= getMotionFrameCtrl().getFrame()
			    && getMotionFrameCtrl().getFrame() < 9.0f) {
				mCollisions[i]->receiveMessage(this, HIT_MESSAGE_PUNCH);
			}
			if (mStatus == MARIO_STATUS_KICK_ROOF
			    && 9.0f <= getMotionFrameCtrl().getFrame()
			    && getMotionFrameCtrl().getFrame() < 13.0f) {
				mCollisions[i]->receiveMessage(this, HIT_MESSAGE_PUNCH);
			}
			break;

		case ACTOR_TYPE_TAMA_NOKO:
		case ACTOR_TYPE_BOMB_HEI:
			hitPickUpEnemy(mCollisions[i]);
			break;

		case ACTOR_TYPE_MAME_GESSO:
			if (((TSmallEnemy*)mCollisions[i])->doKeepDistance())
				keepDistance(*mCollisions[i], 0.0f);
			else
				hitPickUpEnemy(mCollisions[i]);
			break;

		case ACTOR_TYPE_BOSS_EEL_TOOTH:
		case ACTOR_TYPE_BOSS_EEL_COLLISION:
		case ACTOR_TYPE_DEBU_TELESA:
		case ACTOR_TYPE_SROT_RULET:
			keepDistance(*mCollisions[i], 0.0f);
			break;

		case ACTOR_TYPE_GESSO:
		case ACTOR_TYPE_FIRE_WANWAN:
		case ACTOR_TYPE_POI_HANA:
		case ACTOR_TYPE_YUMBO:
		case ACTOR_TYPE_AMENBO:
			hitNormal(mCollisions[i]);
			if (((TSmallEnemy*)mCollisions[i])->doKeepDistance())
				keepDistance(*mCollisions[i], 0.0f);
			break;

		case ACTOR_TYPE_CHUU_HANA:
			hitNormal(mCollisions[i]);
			break;

		case ACTOR_TYPE_BOSS_UNKB:
		case ACTOR_TYPE_BOSS_UNKC:
		case ACTOR_TYPE_BOSS_PAKKUN:
		case ACTOR_TYPE_BOSS_PAKKUN_ATTACK:
		case ACTOR_TYPE_BOSS_UNK14:
		case ACTOR_TYPE_BOSS_UNK15:
		case ACTOR_TYPE_SAMBO_FLOWER:
		case ACTOR_TYPE_ENEMY_UNK35:
			keepDistance(*mCollisions[i], 0.0f);
			break;

		case ACTOR_TYPE_E_MARIO:
		case ACTOR_TYPE_BOSS_GESSO:
		case ACTOR_TYPE_BOSS_GESSO_TAKE_HIT:
		case ACTOR_TYPE_BIANCO_GATE_KEEPER:
			keepDistance(*mCollisions[i], 0.0f);
			break;

		case ACTOR_TYPE_BOSS_GESSO_TENTACLE:
		case ACTOR_TYPE_BOSS_UNK8:
		case ACTOR_TYPE_BOSS_UNKD:
		case ACTOR_TYPE_YOSHI_TONGUE:
		case ACTOR_TYPE_FIRE_WANWAN_TAIL_HIT:
			hitNoKeepPull(mCollisions[i]);
			break;

		case ACTOR_TYPE_NOZZLE_BOX:
			hitNormal(mCollisions[i]);
			keepDistance(*mCollisions[i], 0.0f);
			break;

		case ACTOR_TYPE_FOOTBALL:
			hitPushup(mCollisions[i]);
			break;

		case ACTOR_TYPE_MAP_OBJECT_UNK2:
			hitBrakable(mCollisions[i]);
			break;

		case ACTOR_TYPE_WOOD_BARREL:
		case ACTOR_TYPE_BARREL_OIL:
			hitBarrel(mCollisions[i]);
			break;

		case ACTOR_TYPE_EGG_YOSHI:
		case ACTOR_TYPE_MAP_OBJECT_UNK10:
		case ACTOR_TYPE_ARROW_BOARD_LR:
		case ACTOR_TYPE_GENERAL_HIT_OBJ:
		case ACTOR_TYPE_MAP_OBJECT_UNK30:
		case ACTOR_TYPE_PLANT_FLOWER:
		case ACTOR_TYPE_DRUM_CAN:
		case ACTOR_TYPE_DPT_WEATHERCOCK:
		case ACTOR_TYPE_BIA_BELL:
		case ACTOR_TYPE_BIA_WATERMILL00:
		case ACTOR_TYPE_MINI_WINDMILL_L:
		case ACTOR_TYPE_WATERMELON_STATIC:
		case ACTOR_TYPE_MERRY_POLE:
		case ACTOR_TYPE_COASTER:
		case ACTOR_TYPE_COGWHEEL_POT:
		case ACTOR_TYPE_EX_BOTTLE:
		case ACTOR_TYPE_MAP_OBJ_NAIL:
		case ACTOR_TYPE_FRUIT_COVER_PINE:
			keepDistance(*mCollisions[i], 0.0f);
			break;

		case ACTOR_TYPE_DPTLIGHT:
		case ACTOR_TYPE_TELEGRAPH_POLE_L:
		case ACTOR_TYPE_POLE_NORMAL:
		case ACTOR_TYPE_MAP_OBJECT_UNK32:
		case ACTOR_TYPE_PALM_NORMAL:
		case ACTOR_TYPE_PALM_OUGI:
		case ACTOR_TYPE_PALM_SAGO:
		case ACTOR_TYPE_PALM_NATUME:
		case ACTOR_TYPE_BANANA_TREE:
		case ACTOR_TYPE_FRUIT_TREE:
		case ACTOR_TYPE_MOYASI:
		case ACTOR_TYPE_MAP_OBJECT_UNK47:
		case ACTOR_TYPE_FLUFF:
		case ACTOR_TYPE_ELASTIC_CODE:
		case ACTOR_TYPE_MONTE_ROOT:
		case ACTOR_TYPE_MONTE_GOAL_FLAG:
			hangPole(mCollisions[i]);
			break;

		case ACTOR_TYPE_JUMPBASE:
			hitJumpBase(mCollisions[i]);
			break;

		case ACTOR_TYPE_FRUIT_COCONUT:
		case ACTOR_TYPE_FRUIT_PAPAYA:
		case ACTOR_TYPE_FRUIT_PINE:
		case ACTOR_TYPE_FRUIT_BANANA:
		case ACTOR_TYPE_RED_PEPPER:
			hitWantToTake(mCollisions[i]);
			break;

		case ACTOR_TYPE_FRUIT_DURIAN:
			hitPushup(mCollisions[i]);
			break;

		case ACTOR_TYPE_BREAKABLE_BLOCK:
			hitBrakable(mCollisions[i]);
			break;

		case ACTOR_TYPE_BOSS_MANTA:
		case ACTOR_TYPE_BOSS_UNK12:
		case ACTOR_TYPE_SLEEP_BOSS_HANACHAN:
		case ACTOR_TYPE_BOSS_UNK17:
		case ACTOR_TYPE_BOSS_UNK18:
		case ACTOR_TYPE_BOSS_UNK19:
		case ACTOR_TYPE_BOSS_UNK1A:
		case ACTOR_TYPE_BOSS_UNK1B:
		case ACTOR_TYPE_BOSS_UNK1C:
		case ACTOR_TYPE_BOSS_UNK1D:
		case ACTOR_TYPE_BOSS_UNK1E:
		case ACTOR_TYPE_COASTER_KILLER:
		case ACTOR_TYPE_BOSS_UNK20:
		case ACTOR_TYPE_BOSS_UNK21:
		case ACTOR_TYPE_EFFECT_ENEMY:
		case ACTOR_TYPE_POLLUTE_OBJ:
		case ACTOR_TYPE_ENEMY_UNK9:
		case ACTOR_TYPE_ELEC_CARAPACE:
		case ACTOR_TYPE_LAUNCHER:
		case ACTOR_TYPE_ENEMY_UNK23:
		case ACTOR_TYPE_SEAL:
		case ACTOR_TYPE_ENEMY_UNK26:
		case ACTOR_TYPE_ENEMY_UNK32:
		case ACTOR_TYPE_ENEMY_DAMAGE_OBJ:
		case ACTOR_TYPE_MAP_OBJECT_UNK33:
		case ACTOR_TYPE_PALM_LEAF:
		case ACTOR_TYPE_MAP_OBJ_TREE_SCALE:
		case ACTOR_TYPE_BARREL_FLOAT:
		case ACTOR_TYPE_STOP_ROCK:
			break;
		}
	}
}
