#include <NPC/NpcBase.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <NPC/NpcInitData.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/cameralib.hpp>

void TBaseNPC::initNpcObjCollision_(const TNpcInitInfo* init_info)
{
	u32 iVar4 = 0x4000000;
	u16 uVar5 = 2;
	switch (mActorType) {
	case ACTOR_TYPE_NPC_MONTE_ME:
	case ACTOR_TYPE_NPC_SUNFLOWER_L:
	case ACTOR_TYPE_NPC_SUNFLOWER_S:
	case ACTOR_TYPE_NPC_BOARD:
		uVar5 = 0;
		iVar4 = 0;
		break;
	}

	initHitActor(getActorType(), uVar5, iVar4,
	             init_info->mAttackRadius * mScaling.x,
	             init_info->mAttackHeight * mScaling.y,
	             init_info->mDamageRadius * mScaling.x,
	             init_info->mDamageHeight * mScaling.y);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	if (uVar5 == 0)
		onHitFilter(HIT_FILTER_NO_ATTACK);
}

void TBaseNPC::execNpcObjCollision_()
{
	for (int i = 0; i < mColCount; ++i) {
		bool bVar2;

		if (isNerveWalk()) {
			bVar2 = false;
		} else {
			if (!mCollisions[i]->isHitCategory(HIT_CATEGORY_NPC))
				continue;

			if (!((TBaseNPC*)mCollisions[i])->isNerveWalk())
				continue;

			bVar2 = true;
		}

		JGeometry::TVec3<f32> local_4C(
		    mPosition.x - mCollisions[i]->mPosition.x, 0.0f,
		    mPosition.z - mCollisions[i]->mPosition.z);

		if (bVar2)
			local_4C.negate();

		if (local_4C.isZero()) {
			f32 diffY = mPosition.y;
			diffY -= mCollisions[i]->mPosition.y;
			if (CLBAbs(diffY) < 0.001f) {
				local_4C.set(1.0f, 10.0f, 0.0f);
			} else {
				local_4C.set(0.0f, diffY, 0.0f);
			}
		} else {
			// TODO: recover the shared ABS macro; CLBAbs evaluates only once.
			f32 dVar8
			    = getAttackRadius() + mCollisions[i]->getDamageRadius()
			                  - MsVECMag2(local_4C)
			              >= 0.0f
			          ? getAttackRadius() + mCollisions[i]->getDamageRadius()
			                - MsVECMag2(local_4C)
			          : -(getAttackRadius() + mCollisions[i]->getDamageRadius()
			              - MsVECMag2(local_4C));

			if (dVar8 < 0.001f)
				dVar8 = 0.001f;

			local_4C.setLength(dVar8);
		}

		if (bVar2) {
			mCollisions[i]->mPosition += local_4C;
		} else {
			mPositionDelta += local_4C;
		}
	}
}

void TBaseNPC::setVariableDamageRadius_()
{
	const TNpcInitInfo* initInfo
	    = SMSGetNpcInitData(mActorType - ACTOR_TYPE_NPC_MONTE_M);
	f32 fVar6 = initInfo->mDamageRadius;
	fVar6     = mScaling.x * fVar6;
	f32 fVar7 = fVar6;
	if (isBeTrampledNpc() && !SMS_IsMarioTouchGround4cm()
	    && SMS_GetMarioPos().y > mPosition.y) {
		JGeometry::TVec3<f32> marioPos;
		marioPos.set(SMS_GetMarioPos());
		JGeometry::TVec3<f32> diff;
		diff.sub(marioPos, mPosition);
		diff.y = 0.0f;
		if (diff.squared() < CLBSquared(fVar6 * 3.0f))
			fVar7 = mIndividualParams->mSLDamageRadiusSmall.get();
	}

	mDamageRadius = fVar7;
	calcEntryRadius();
}

// TODO: frame-only mismatch in the subtraction-result stack slots.
// Investigate the canonical TVec3 subtraction inline; do not add padding.
void TBaseNPC::bind()
{
	JGeometry::TVec3<f32> nextPos = mPosition;
	nextPos += mPositionDelta;
	nextPos += mVelocity;

	mVelocity.y -= getGravityY();

	if (mVelocity.y < mVelocityMinY)
		mVelocity.y = mVelocityMinY;

	mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
	    nextPos.x, nextPos.y + mHeadHeight, nextPos.z, &mGroundPlane);
	mGroundHeight += 1.0f;

	if (nextPos.y <= mGroundHeight + 0.05f) {
		if (mGroundPlane && getGroundPlane()->isLegal()) {
			offLiveFlag(LIVE_FLAG_AIRBORNE);
			mVelocity.set(0.0f, 0.0f, 0.0f);
			nextPos.y = mGroundHeight;
		}

		if (mGroundPlane) {
			(void)mGroundPlane;
		}
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	if (checkLiveFlag(LIVE_FLAG_UNK10000000)) {
		gpMap->isTouchedOneWallAndMoveXZ(&nextPos.x, nextPos.y + mHeadHeight,
		                                 &nextPos.z, 150.0f);
	}

	setPositionDelta(nextPos - mPosition);
}
