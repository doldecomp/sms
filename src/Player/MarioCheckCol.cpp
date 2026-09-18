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
		if (getStatus() == MARIO_STATUS_HIP_DROP) {
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
		actor->receiveMessage(this, HIT_MESSAGE_SUPER_HIP_DROP);
		return;
	}

	if (getStatus() == MARIO_STATUS_CATCH
	    || getStatus() == MARIO_STATUS_OIL_SLIP
	    || getStatus() == MARIO_STATUS_JUMP_CATCH) {
		actor->receiveMessage(this, HIT_MESSAGE_PUNCH);
		actor->receiveMessage(this, HIT_MESSAGE_TRAMPLE);
	}

	TWaterGun* wg = mWaterGun;
	if (wg->getCurrentNozzleIndex() == 0 && wg->mIsEmitWater != 0) {
		// TODO: 99.7%: the only difference left is the volatile register
		// holding &mStaticHitActor (retail r3, coalesced with the argument
		// copy in r4; ours r7). The instruction stream is identical. Ruled
		// out: no local, a reference local, a TU-static accessor for the
		// static, a cast at the call, `mPosition.set()`, `getPosition()`,
		// and moving the mParticleIndex store earlier (all 93.8-99.6%).
		// Closure batch 129, also rejected (all 8 or 11 differences, frame
		// 0x30 throughout): the stores through the static with the argument
		// through the named local (identical to this spelling), the argument
		// through the static with the stores through the local or a
		// reference, declaring `water` after the first store, and a TU-local
		// `static inline` level taking the pointer by parameter (with or
		// without the receiveMessage inside it).
		// Positively: retail *splits* the address's live range -- `addi r3`
		// then `mr r4, r3`, the first store through r3 and the rest through
		// r4 once `addi r3, r31, 0` claims r3 for the receiver -- while we
		// keep the whole range in r7 and leave r4 a redundant copy. It is a
		// coalescing decision on one volatile register; nothing measured
		// reaches it.
		TWaterHitActor* water = &TModelWaterManager::mStaticHitActor;
		water->mPosition = mPosition;
		water->mPosition.y += 80.0f;
		water->mParticleIndex = 0;
		actor->receiveMessage(water, HIT_MESSAGE_SPRAYED_BY_WATER);
	}
}

// TODO: the inlined conditions match, but the UNUSED body is 112 vs 116 bytes.
void TMario::hitHipDrop(THitActor* actor)
{
	if (mStatus == MARIO_STATUS_HIP_DROP
	    && (mStatusState == 2 || mStatusState == 3)
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
			                        ->mEmitParams.mAmountMax.get();
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
		if (getHeldObject() == nullptr && !onYoshi())
			canHang = 1;

		u8 inHangStatus;
		if (canHang == 0) {
			inHangStatus = 0;
		} else {
			// TODO: inlines
			u32 statLo = getStatus() & MARIO_STATUS_TYPE_AND_ID_MASK;
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
			f32 dx   = actor->mPosition.x - mPosition.x;
			f32 dz   = actor->mPosition.z - mPosition.z;
			f32 dist = std::sqrtf(dx * dx + dz * dz);
			if (dist == 0.0f)
				dist = 1.0f;

			f32 b = 50.0f + actor->getDamageRadius()
			        + mBarParams.mCatchRadius.get();

			f32 sinY = JMASSin(mFaceAngle.y);
			f32 cosY = JMASCos(mFaceAngle.y);
			f32 nx   = dx / dist;
			f32 nz   = dz / dist;
			f32 a    = sinY * nx + cosY * nz;

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

// TODO: GMSE01 instructions match apart from stack operands: frame 0x1E0 vs
// 0x238, sqrt temporary 0x54 vs 0xA8, and later spills shifted by 0x58.
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
			    && mVel.y < 0.0f && yt.y < mPosition.y && getStatus() != 0x89C
			    && getStatus() != MARIO_STATUS_THROWN_DOWN
			    && getStatus() != MARIO_STATUS_BACK_JUMP && dist < 180.0f) {
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

	for (s32 i = 0; i < (s32)getColNum(); i++) {
		u32 colType = getCollision(i)->getActorType();
		if (getCollision(i)->checkActorType(ACTOR_TYPE_UNK4000000)) {
			hitNpc(getCollision(i));
			continue;
		}

		switch (colType) {
		// Other mario (enemy mario?)
		case 0x80000001:
			hitMario(getCollision(i));
			keepDistance(*getCollision(i), 0.0f);
			break;

		// Some item?
		case 0x20000008:
		case 0x2000000A:
		case 0x2000000C:
			hitNormal(getCollision(i));
			break;

		// Most crap: namekuri, hamukuri, etc
		case 0x8000001:
		case 0x8000003:
		case 0x8000013:
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
			hitNormal(getCollision(i));
			break;

		// ???
		case 0x8000011:
			hitHipDrop(getCollision(i));
			break;

		// Kumokun
		case 0x1000002C:
			hitNormal(getCollision(i));
			if (((TSmallEnemy*)getCollision(i))->doKeepDistance())
				keepDistance(*getCollision(i), 0.0f);
			// fall through

		// ???
		case 0x10000021:
			// The one site in this loop that reads the array directly: the
			// accessor puts the base address in r3 instead of r4 here, which
			// is the last difference in the function.
			hitHipDrop(mCollisions[i]);
			// fall through

		// Amiking
		case 0x10000034:
			if (getStatus() == MARIO_STATUS_FENCE_PUNCH
			    && 5.0f <= getMotionFrameCtrl().getFrame()
			    && getMotionFrameCtrl().getFrame() < 9.0f) {
				getCollision(i)->receiveMessage(this, HIT_MESSAGE_PUNCH);
			}
			if (getStatus() == MARIO_STATUS_KICK_ROOF
			    && 9.0f <= getMotionFrameCtrl().getFrame()
			    && getMotionFrameCtrl().getFrame() < 13.0f) {
				getCollision(i)->receiveMessage(this, HIT_MESSAGE_PUNCH);
			}
			break;

		// Tama noko and something else
		case 0x10000018:
		case 0x1000001E:
			hitPickUpEnemy(getCollision(i));
			break;

		// Mame gesso
		case 0x10000008: {
			TSmallEnemy* enemy = (TSmallEnemy*)getCollision(i);
			if (enemy->doKeepDistance())
				keepDistance(*getCollision(i), 0.0f);
			else
				hitPickUpEnemy(getCollision(i));
			break;
		}

		// R1: keepDistance (cases sharing L_80161364 leaf)
		case 0x8000022:
		case 0x8000023:
		case 0x10000033:
		case 0x400001A6:
			keepDistance(*getCollision(i), 0.0f);
			break;

		// P: hitNormal + virt[0x19C] check + keepDist
		case 0x10000007:
		case 0x1000000E:
		case 0x10000015:
		case 0x1000002A:
		case 0x1000002D:
			hitNormal(getCollision(i));
			if (((TSmallEnemy*)getCollision(i))->doKeepDistance())
				keepDistance(*getCollision(i), 0.0f);
			break;

		// A3: hitNormal (placed between P and R2 for body emission order)
		case 0x10000016:
			hitNormal(getCollision(i));
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
			keepDistance(*getCollision(i), 0.0f);
			break;

		// Damaging parts of boss gesso and other stuff
		case 0x8000002:
		case 0x8000005:
		case 0x8000007:
		case 0x10000022:
			keepDistance(*getCollision(i), 0.0f);
			break;

		// Enemies with pull-able parts --
		// boss gesso tentacles/nose, fire wanwans, etc
		case 0x8000006:
		case 0x8000008:
		case 0x800000D:
		case 0x8000083:
		case 0x10000028:
			hitNoKeepPull(getCollision(i));
			break;

		// Nozzle box
		case 0x20000068:
			hitNormal(getCollision(i));
			keepDistance(*getCollision(i), 0.0f);
			break;

		// Football, balloon ball, coconut
		case 0x40000064:
			hitPushup(getCollision(i));
			break;

		// ???
		case 0x40000002:
			hitBrakable(getCollision(i));
			break;

		// Water & oil barrels
		case 0x4000005A:
		case 0x4000005C:
			hitBarrel(getCollision(i));
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
			keepDistance(*getCollision(i), 0.0f);
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
			hangPole(getCollision(i));
			break;

		// jump base
		case 0x40000017:
			hitJumpBase(getCollision(i));
			break;

		// fruits
		case 0x40000390:
		case 0x40000391:
		case 0x40000392:
		case 0x40000394:
		case 0x40000395:
			hitWantToTake(getCollision(i));
			break;

		// durian fruit
		case 0x40000393:
			hitPushup(getCollision(i));
			break;

		// various breakable blocks
		case 0x400002BC:
			hitBrakable(getCollision(i));
			break;

		// empty cases
		case 0x8000004:
		case 0x8000012:
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
