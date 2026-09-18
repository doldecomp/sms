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
			if (!getCollision(i)->checkActorType(0x4000000))
				continue;

			if (!((TBaseNPC*)getCollision(i))->isNerveWalk())
				continue;

			bVar2 = true;
		}

		JGeometry::TVec3<f32> local_4C(
		    getPosition().x - getCollision(i)->getPosition().x, 0.0f,
		    getPosition().z - getCollision(i)->getPosition().z);

		if (bVar2)
			local_4C.negate();

		if (local_4C.squared() <= JGeometry::TUtil<f32>::epsilon()) {
			f32 diffY = getPosition().y;
			diffY -= getCollision(i)->mPosition.y;

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
			getCollision(i)->mPosition += local_4C;
		} else {
			mLinearVelocity += local_4C;
		}
	}
}

// Binding level worth +16 of low region, landing
// TBaseNPC::setVariableDamageRadius_'s frame at 0x70 (batch 124).
static inline const JGeometry::TVec3<f32>&
NpcCollisionGetPosition(const TBaseNPC* p)
{
	const JGeometry::TVec3<f32>& position = p->getPosition();
	return position;
}

void TBaseNPC::setVariableDamageRadius_()
{
	// Exact. The product's `fmuls` operand order is what the missing
	// `initInfo` local was hiding: with the lookup inlined into the product
	// the scaling load lands in f0 and retail's `fmuls f30, f0, f1` comes
	// out. `mActorType` and `getActorType()` are interchangeable here, and
	// swapping the factors or splitting the product into `base *= ...` all
	// cost 0.1-0.4.
	f32 base = getScaling().x
	           * SMSGetNpcInitData(mActorType - 0x4000001)->mDamageRadius;
	f32 fVar6 = base;
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
	// TODO: 99.9%, frame 0x48 exact, one `~`: the 12-byte argument temporary
	// of `nextPos - mPosition`. Retail allocates it at the bottom of the low
	// region (0x10, with 0xc..0x10 lost to 8-byte alignment) and then the
	// other 24 bytes of region above it; we get the same 24 bytes at
	// 0xc..0x28 and the temporary at 0x28, directly under nextPos. So the
	// residue is allocation *order*, not size: retail allocates the last
	// statement's temporary first, as MWCC does for the three scale steps of
	// MarioParticle's TWarpInCallBack, and here we allocate forward. Dropping
	// the statement entirely leaves frame 0x30, so the 24 bytes come from the
	// earlier statements either way. Rejected, all frame-neutral or worse: a
	// named or const-reference `diff` local, `nextPos -= mPosition`,
	// `diff.sub(nextPos, mPosition)` (89-95%), `nextPos - getPosition()` and
	// the TU-local binding position (99.6%), `add()` for the two `+=`
	// (identical), `mGroundPlane` for `getGroundPlane()` (99.8%), and a dead
	// named `TVec3` after nextPos (+16 of frame).
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
