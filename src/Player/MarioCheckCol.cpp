#include <Player/MarioMain.hpp>
#include <Player/WaterGun.hpp>
#include <Player/Yoshi.hpp>
#include <Player/ModelWaterManager.hpp>
#include <MarioUtil/MathUtil.hpp>
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

void TMario::hitHipDrop(THitActor*) { }

void TMario::hitPushup(THitActor*) { }

void TMario::hitPull(THitActor*) { }

void TMario::hitMario(THitActor*) { }

void TMario::hitNpc(THitActor*) { }

void TMario::hitPool(THitActor*) { }

void TMario::wantToTakeActor(THitActor*) { }

void TMario::hitWantToTake(THitActor*) { }

void TMario::hitBarrel(THitActor*) { }

void TMario::hitJumpBase(THitActor*) { }

void TMario::hitBrakable(THitActor*) { }

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

void TMario::hitPickUpEnemy(THitActor*) { }

void TMario::hitSurfingBoard(THitActor*) { }

void TMario::hitNoKeepPull(THitActor*) { }

void TMario::checkCollision() { }
