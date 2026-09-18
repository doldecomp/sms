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
	case 0x4000006:
	case 0x400001A:
	case 0x400001B:
	case 0x400001D:
		uVar5 = 0;
		iVar4 = 0;
		break;
	}

	initHitActor(getActorType(), uVar5, iVar4,
	             init_info->mAttackRadius * mScaling.x,
	             init_info->mAttackHeight * mScaling.y,
	             init_info->mDamageRadius * mScaling.x,
	             init_info->mDamageHeight * mScaling.y);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	if (uVar5 == 0)
		onHitFlag(HIT_FLAG_CANNOT_ATTACK);
}

void TBaseNPC::execNpcObjCollision_()
{
	for (int i = 0; i < mColCount; ++i) {
		bool bVar2;

		if (isNerveWalk()) {
			bVar2 = false;
		} else {
			if (!mCollisions[i]->checkActorType(0x4000000))
				continue;

			if (!((TBaseNPC*)mCollisions[i])->isNerveWalk())
				continue;

			bVar2 = true;
		}

		JGeometry::TVec3<f32> local_4C(
		    getPosition().x - mCollisions[i]->mPosition.x, 0.0f,
		    getPosition().z - mCollisions[i]->mPosition.z);

		if (bVar2)
			local_4C.negate();

		if (local_4C.squared() <= JGeometry::TUtil<f32>::epsilon()) {
			f32 diffY = getPosition().y;
			diffY -= mCollisions[i]->mPosition.y;

			f32 absY = diffY >= 0.0f ? diffY : -diffY;

			if (absY < 0.001f) {
				local_4C.x = 1.0f;
				local_4C.y = 10.0f;
				local_4C.z = 0.0f;
			} else {
				local_4C.x = 0.0f;
				local_4C.y = diffY;
				local_4C.z = 0.0f;
			}
		} else {
			f32 overlap;
			if (mAttackRadius + mCollisions[i]->mDamageRadius
			        - MsVECMag2(local_4C)
			    >= 0.0f) {
				overlap = mAttackRadius + mCollisions[i]->mDamageRadius
				          - MsVECMag2(local_4C);
			} else {
				overlap = -(mAttackRadius + mCollisions[i]->mDamageRadius
				            - MsVECMag2(local_4C));
			}

			f32 dVar8 = overlap;
			if (overlap < 0.001f)
				dVar8 = 0.001f;

			local_4C.setLength(dVar8);
		}

		if (bVar2) {
			mCollisions[i]->mPosition += local_4C;
		} else {
			mLinearVelocity += local_4C;
		}
	}
}

// Binding level worth +16 of low region, landing
// TBaseNPC::setVariableDamageRadius_'s frame at 0x70 (batch 124).
static inline const JGeometry::TVec3<f32>& NpcCollisionGetPosition(const TBaseNPC* p)
{
	const JGeometry::TVec3<f32>& position = p->getPosition();
	return position;
}

void TBaseNPC::setVariableDamageRadius_()
{
	const TNpcInitInfo* initInfo = SMSGetNpcInitData(mActorType - 0x4000001);
	// TODO: 24 bytes of low region short (0x70 vs 0x58). getScaling().x and
	// NpcCollisionGetPosition(this).y are +8 each and the ladder then saturates
	// (getActorType(), setDamageRadius(), a named CLBSquared result, a named
	// squared() result are all +0); NpcCollisionGetPosition(this) inside the sub() is a third
	// +8 but costs 0.2 of score. The one remaining `~` is fmuls' operand
	// order, which no spelling of the product moved.
	f32 base                     = getScaling().x * initInfo->mDamageRadius;
	f32 fVar6                    = base;
	if (isBeTrampledNpc() && !SMS_IsMarioTouchGround4cm()
	    && SMS_GetMarioPos().y > NpcCollisionGetPosition(this).y) {
		JGeometry::TVec3<f32> diff;
		diff.sub(SMS_GetMarioPos(), mPosition);
		diff.y = 0.0f;
		if (diff.squared() < CLBSquared(base * 3.0f))
			fVar6 = mIndividualParams->mSLDamageRadiusSmall.get();
	}

	mDamageRadius = fVar6;
	calcEntryRadius();
}

void TBaseNPC::bind()
{
	// TODO: frame size and every named local are right; the only residue is
	// the 12-byte argument temporary of `nextPos - mPosition`, which retail
	// puts at 0x10(r1) (the bottom of the low region) and we put at 0x28(r1),
	// just under nextPos. Rejected: a named or const-reference `diff` local,
	// `nextPos -= mPosition`, and a `diff.sub(nextPos, mPosition)` call (all
	// 89-95%).
	JGeometry::TVec3<f32> nextPos = mPosition;
	nextPos += mLinearVelocity;
	nextPos += mVelocity;

	mVelocity.y -= getGravityY();

	if (mVelocity.y < mVelocityMinY)
		mVelocity.y = mVelocityMinY;

	mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
	    nextPos.x, nextPos.y + mHeadHeight, nextPos.z, &mGroundPlane);
	mGroundHeight += 1.0f;

	if (nextPos.y <= mGroundHeight + 0.05f) {
		if (getGroundPlane() && getGroundPlane()->isLegal()) {
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

	setLinearVelocity(nextPos - mPosition);
}
