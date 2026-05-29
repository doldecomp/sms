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
	if (checkStatusFlag(STATUS_FLAG_JUMPING) && mVel.y < 0.0f
	    && actor->mPosition.y < mPosition.y) {
		if (mStatus == STATUS_HIP_DROP) {
			if (actor->receiveMessage(this, HIT_MESSAGE_HIP_DROP)) {
				if (actor->isActorType(0x8000001)) {
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
		actor->receiveMessage(this, HIT_MESSAGE_UNK3);
		return;
	}

	if (mStatus == STATUS_CATCH || mStatus == STATUS_OIL_SLIP
	    || mStatus == STATUS_JUMP_CATCH) {
		actor->receiveMessage(this, HIT_MESSAGE_PUNCH);
		actor->receiveMessage(this, HIT_MESSAGE_TRAMPLE);
	}

	TWaterGun* wg = mWaterGun;
	if ((int)wg->mCurrentNozzle == 0 && wg->mIsEmitWater != 0) {
		TModelWaterManager::mStaticHitActor.mPosition   = mPosition;
		TModelWaterManager::mStaticHitActor.mPosition.y = mPosition.y + 80.0f;
		TModelWaterManager::mStaticHitActor.unk68       = 0;
		actor->receiveMessage(&TModelWaterManager::mStaticHitActor,
		                      HIT_MESSAGE_SPRAYED_BY_WATER);
	}
}

// TODO: wrong size! maybe we return the receiveMessage result?
void TMario::hitHipDrop(THitActor* actor)
{
	if (mStatus == STATUS_HIP_DROP && mStatusState == 2
	    && actor->mPosition.y < mPosition.y) {
		actor->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
	}
}

void TMario::hitPushup(THitActor* actor)
{
	if (checkStatusFlag(STATUS_FLAG_JUMPING) && mVel.y > 0.0f)
		actor->receiveMessage(this, HIT_MESSAGE_UNK2);
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
	    && !checkStatusFlag(MARIO_FLAG_HELMET)
	    && checkStatusFlag(STATUS_FLAG_JUMPING) && mStatus != STATUS_HIP_DROP
	    && mVel.y < 0.0f && actor->mPosition.y < mPosition.y
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
		changePlayerStatus(STATUS_TAKE, 0, false);
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
	if (checkStatusFlag(STATUS_FLAG_JUMPING) && mVel.y < 0.0f
	    && actor->mPosition.y < mPosition.y && mStatus == STATUS_HIP_DROP) {
		actor->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
		if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
			TWaterGun* wg     = mWaterGun;
			wg->mCurrentWater = (s32)((const TWaterGun*)wg)
			                        ->getCurrentNozzle()
			                        ->mEmitParams.mAmountMax.get();
		}
	}
}

void TMario::hitJumpBase(THitActor* actor)
{
	keepDistance(*actor, 0.0f);
	if (((TJumpBase*)actor)->unk134 == 0)
		wantToTakeActor(actor);
}

void TMario::hitBrakable(THitActor* actor)
{
	if (checkStatusFlag(STATUS_FLAG_JUMPING) && mVel.y < 0.0f
	    && actor->mPosition.y < mPosition.y && mStatus == STATUS_HIP_DROP) {
		actor->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
	}
}

void TMario::hangPole(THitActor* actor)
{
	if (!checkStatusFlag(STATUS_FLAG_UNK100000)) {
		u8 canHang = 0;
		if (mHeldObject == nullptr && !onYoshi())
			canHang = 1;

		u8 inHangStatus;
		if (canHang == 0) {
			inHangStatus = 0;
		} else {
			u32 statLo = mStatus & STATUS_TYPE_AND_ID_MASK;
			if (statLo >= 0x80 && statLo <= 0x9F) {
				inHangStatus = 1;
			} else {
				if (checkStatusFlag(STATUS_FLAG_UNK200000))
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
			if (mPrevStatus & STATUS_FLAG_UNK100000)
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
				changePlayerStatus(STATUS_BAR_HANG, 0, false);
				actor->receiveMessage(this, HIT_MESSAGE_UNK5);
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
	    && !checkStatusFlag(STATUS_FLAG_JUMPING)) {
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
	if (mStatus != STATUS_PULLING && mStatus != STATUS_PULL_JUMP
	    && canTake(actor) && actor->receiveMessage(this, HIT_MESSAGE_TAKE)) {
		changePlayerStatus(STATUS_PULLING, 0, false);
		setAnimation(ANIM_HOLD, 1.0f);
		mHeldObject = (TTakeActor*)actor;
	} else {
		hitNormal(actor);
	}
}

void TMario::checkCollision()
{
	if (checkStatusFlag(STATUS_FLAG_UNK1000))
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

			if (checkStatusFlag(STATUS_FLAG_JUMPING) && isHolding()
			    && mVel.y < 0.0f && yt.y < mPosition.y && mStatus != 0x89C
			    && mStatus != STATUS_THROWN_DOWN && mStatus != STATUS_BACK_JUMP
			    && dist < 180.0f) {
				mPosition       = mYoshi->getTranslation();
				mFaceAngle.y    = mYoshi->mEggRotSpeed;
				mModelFaceAngle = mFaceAngle.y;

				if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
					unk3E8              = mWaterGun->mSecondNozzle;
					const TWaterGun* wg = mWaterGun;
					unk3EC              = (f32)(wg->mCurrentWater
                                   / wg->getCurrentNozzle()
                                         ->mEmitParams.mAmountMax.get());
				}
				mYoshi->ride();
				unk118 |= 0x8000;
				if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
					mWaterGun->changeNozzle(TWaterGun::Yoshi, true);
				}
				changePlayerStatus(STATUS_WAIT, 0, false);
				return;
			}

			keepDistance(yt, 80.0f, 0.0f);
		}
	}

	for (s32 i = 0; i < (s32)mColCount; i++) {
		if (mCollisions[i]->checkActorType(ACTOR_TYPE_UNK4000000)) {
			hitNpc(mCollisions[i]);
			continue;
		}

		// TODO: switch still a bit wrong!
		switch (mCollisions[i]->getActorType()) {
		// Other mario (enemy mario?)
		case 0x80000001:
			hitMario(mCollisions[i]);
			keepDistance(*mCollisions[i], 0.0f);
			break;

		// Some item?
		case 0x20000008:
		case 0x2000000A:
		case 0x2000000C:
			hitNormal(mCollisions[i]);
			break;

		// Most crap: namekuri, hamukuri, etc
		case 0x8000001:
		case 0x8000003:
		case 0x8000013:
		case 0x8000016:
		case 0x8000017:
		case 0x8000018:
		case 0x8000019:
		case 0x800001A:
		case 0x800001B:
		case 0x800001C:
		case 0x800001D:
		case 0x800001E:
		case 0x800001F:
		case 0x8000020:
		case 0x8000021:
		case 0x8000024:
		case 0x10000001:
		case 0x10000002:
		case 0x10000003:
		case 0x10000004:
		case 0x1000000A:
		case 0x1000000C:
		case 0x1000000D:
		case 0x1000000F:
		case 0x10000010:
		case 0x10000011:
		case 0x10000012:
		case 0x10000013:
		case 0x10000017:
		case 0x10000019:
		case 0x1000001A:
		case 0x1000001B:
		case 0x1000001C:
		case 0x1000001D:
		case 0x1000001F:
		case 0x10000020:
		case 0x10000025:
		case 0x1000002E:
		case 0x10000031:
		case 0x10000037:
		case 0x4000019A:
			hitNormal(mCollisions[i]);
			break;

		// ???
		case 0x8000011:
			hitHipDrop(mCollisions[i]);
			break;

		// Kumokun
		case 0x1000002C:
			hitNormal(mCollisions[i]);
			if (((TSmallEnemy*)mCollisions[i])->doKeepDistance())
				keepDistance(*mCollisions[i], 0.0f);
			// fall through

		// ???
		case 0x10000021:
			hitHipDrop(mCollisions[i]);
			// fall through

		// Amiking
		case 0x10000034:
			if (mStatus == STATUS_FENCE_PUNCH
			    && 5.0f <= getMotionFrameCtrl().getFrame()
			    && getMotionFrameCtrl().getFrame() < 9.0f) {
				mCollisions[i]->receiveMessage(this, HIT_MESSAGE_PUNCH);
			}
			if (mStatus == STATUS_KICK_ROOF
			    && 9.0f <= getMotionFrameCtrl().getFrame()
			    && getMotionFrameCtrl().getFrame() < 13.0f) {
				mCollisions[i]->receiveMessage(this, HIT_MESSAGE_PUNCH);
			}
			break;

		// Tama noko and something else
		case 0x10000018:
		case 0x1000001E:
			hitPickUpEnemy(mCollisions[i]);
			break;

		// Mame gesso
		case 0x10000008:
			if (((TSmallEnemy*)mCollisions[i])->doKeepDistance())
				keepDistance(*mCollisions[i], 0.0f);
			else
				hitPickUpEnemy(mCollisions[i]);
			break;

		// R1: keepDistance (cases sharing L_80161364 leaf)
		case 0x8000022:
		case 0x10000033:
		case 0x400001A6:
			keepDistance(*mCollisions[i], 0.0f);
			break;

		// P: hitNormal + virt[0x19C] check + keepDist
		case 0x10000007:
		case 0x1000000E:
		case 0x10000015:
		case 0x1000002A:
		case 0x1000002D:
			hitNormal(mCollisions[i]);
			if (((TSmallEnemy*)mCollisions[i])->doKeepDistance())
				keepDistance(*mCollisions[i], 0.0f);
			break;

		// A3: hitNormal (placed between P and R2 for body emission order)
		case 0x10000016:
			hitNormal(mCollisions[i]);
			break;

		// ???
		case 0x800000B:
		case 0x800000C:
		case 0x800000F:
		case 0x8000010:
		case 0x8000014:
		case 0x8000015:
		case 0x10000027:
		case 0x10000035:
			keepDistance(*mCollisions[i], 0.0f);
			break;

		// Damaging parts of boss gesso and other stuff
		case 0x8000002:
		case 0x8000005:
		case 0x8000007:
		case 0x10000022:
		case 0x8000023:
			keepDistance(*mCollisions[i], 0.0f);
			break;

		// Enemies with pull-able parts --
		// boss gesso tentacles/nose, fire wanwans, etc
		case 0x8000006:
		case 0x8000008:
		case 0x800000D:
		case 0x8000083:
		case 0x10000028:
			hitNoKeepPull(mCollisions[i]);
			break;

		// Nozzle box
		case 0x20000068:
			hitNormal(mCollisions[i]);
			keepDistance(*mCollisions[i], 0.0f);
			break;

		// Football, balloon ball, coconut
		case 0x40000064:
			hitPushup(mCollisions[i]);
			break;

		// ???
		case 0x40000002:
			hitBrakable(mCollisions[i]);
			break;

		// Water & oil barrels
		case 0x4000005A:
		case 0x4000005C:
			hitBarrel(mCollisions[i]);
			break;

		// Misc default-ish stuff -- just don't clip inside
		case 0x20000009:
		case 0x40000010:
		case 0x4000001B:
		case 0x40000026:
		case 0x40000030:
		case 0x40000046:
		case 0x4000005D:
		case 0x4000007E:
		case 0x4000009E:
		case 0x4000009F:
		case 0x400000A0:
		case 0x400000DB:
		case 0x40000136:
		case 0x40000139:
		case 0x40000228:
		case 0x40000233:
		case 0x40000264:
		case 0x40000396:
			keepDistance(*mCollisions[i], 0.0f);
			break;

		// Poles, trees, etc -- "climbable" stuff
		case 0x4000002D:
		case 0x4000002E:
		case 0x4000002F:
		case 0x40000032:
		case 0x40000034:
		case 0x40000035:
		case 0x40000036:
		case 0x40000037:
		case 0x40000039:
		case 0x4000003A:
		case 0x4000003C:
		case 0x40000047:
		case 0x40000049:
		case 0x400000BB:
		case 0x40000244:
		case 0x40000246:
			hangPole(mCollisions[i]);
			break;

		// jump base
		case 0x40000017:
			hitJumpBase(mCollisions[i]);
			break;

		// fruits
		case 0x40000390:
		case 0x40000391:
		case 0x40000392:
		case 0x40000394:
		case 0x40000395:
			hitWantToTake(mCollisions[i]);
			break;

		// durian fruit
		case 0x40000393:
			hitPushup(mCollisions[i]);
			break;

		// various breakable blocks
		case 0x400002BC:
			hitBrakable(mCollisions[i]);
			break;

		// empty cases
		case 0x8000004:
		case 0x8000012:
		case 0x10000005:
		case 0x10000006:
		case 0x10000009:
		case 0x1000000B:
		case 0x10000014:
		case 0x10000023:
		case 0x10000024:
		case 0x10000026:
		case 0x10000032:
		case 0x10000036:
		case 0x40000033:
		case 0x40000038:
		case 0x4000003B:
		case 0x4000005B:
		case 0x4000022B:
			break;
		}
	}
}
