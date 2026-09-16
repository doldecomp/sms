#include <MoveBG/MapObjBall.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMapObjBall::touchRoof(JGeometry::TVec3<f32>* param_1)
{
	if (param_1->y > unk140)
		param_1->y = unk140;

	calcReflectingVelocity(unk13C, mMapObjData->mPhysical->unk4->unk4,
	                       &mVelocity);
}

void TMapObjBall::touchPollution() { kill(); }

void TMapObjBall::touchWaterSurface() { kill(); }

void TMapObjBall::rebound(JGeometry::TVec3<f32>* param_1)
{
	calcReflectingVelocity(mGroundPlane, mMapObjData->mPhysical->unk4->unk4,
	                       &mVelocity);
	param_1->y = mGroundHeight;
	onLiveFlag(LIVE_FLAG_AIRBORNE);

	if (isActorType(0x400000D0)) {
		// The watermelon has a big and a small bounce sample, chosen by how
		// far it has been scaled up.
		if (mScaling.y >= 5.0f) {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_WATERMELON_BBUND, &mPosition, nullptr,
			    abs(mGroundPlane->mNormal.y), 0, 0, nullptr, 0, 4);
		} else {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_WATERMELON_SBUND, &mPosition, nullptr,
			    abs(mGroundPlane->mNormal.y), 0, 0, nullptr, 0, 4);
		}
	} else {
		u32 sound = mMapObjData->mSound->unk4->unk0[4];
		SMSGetMSound()->startSoundActorWithInfo(sound, &mPosition,
		                                        (Vec*)&mVelocity, 0.0f, 0, 0,
		                                        nullptr, 0, 4);
	}
}

void TMapObjBall::put()
{
	TMapObjGeneral::put();
	calcCurrentMtx();
}

void TMapObjBall::hold(TTakeActor* param_1)
{
	// A ball still moving fast cannot be picked up.
	// TODO: the original calls JGeometry::TUtil<f32>::sqrt out of line here
	// (it is emitted weak from boid.cpp); our build inlines it. That is an
	// -inline deferred budget decision taken over the whole TU, so it may
	// settle once the rest of MapObjBall.cpp is written.
	JGeometry::TVec3<f32> vel(mVelocity);
	if (vel.length() > 10.0f)
		return;

	TMapObjGeneral::hold(param_1);
	mVelocity.set(0.0f, 0.0f, 0.0f);
}

u32 TMapObjBall::touchWater(THitActor* param_1)
{
	if (isState(STATE_HOLDING) || isState(STATE_APPEARING))
		return 1;

	// The current drags the ball along, scaled by the per-kind unk17C.
	JGeometry::TVec3<f32> vel(mVelocity);
	JGeometry::TVec3<f32> pushed;
	pushed.set(vel);

	const JGeometry::TVec3<f32>& flow = getWaterSpeed(param_1);
	pushed.x += flow.x * unk17C;
	pushed.y += flow.y * unk17C;
	pushed.z += flow.z * unk17C;
	mVelocity = pushed;

	offLiveFlag(LIVE_FLAG_UNK10);
	return 1;
}

void TMapObjBall::checkWallCollision(JGeometry::TVec3<f32>* param_1)
{
	JGeometry::TVec3<f32> centre;
	centre.x = param_1->x;
	centre.y = param_1->y + mBodyRadius;
	centre.z = param_1->z;

	TBGWallCheckRecord check(centre, mBodyRadius, 4,
	                         mMapObjData->mPhysical->mWallCheckFlags);

	if (gpMap->isTouchedWallsAndMoveXZ(&check)) {
		unk138   = check.mResultWalls[0];
		param_1->x = centre.x;
		param_1->z = centre.z;
		touchWall(param_1, &check);
		return;
	}

	unk138 = nullptr;
}

void TMapObjBall::makeObjDefault()
{
	TMapObjBase::makeObjDefault();

	MtxPtr mtx  = getModel()->getAnmMtx(0);
	mtx[0][3] = mPosition.x;
	mtx[1][3] = mPosition.y + mBodyRadius;
	mtx[2][3] = mPosition.z;
}

void TMapObjBall::makeObjAppeared()
{
	TMapObjBase::makeObjAppeared();
	calcCurrentMtx();

	MtxPtr mtx = getModel()->getAnmMtx(0);
	mtx[0][3] = mPosition.x;
	mtx[1][3] = mPosition.y + mBodyRadius;
	mtx[2][3] = mPosition.z;

	if (isActorType(0x40000394)) {
		if (mtx[1][1] > 0.0f)
			mtx[1][3] = -(50.0f * mtx[1][1] - mtx[1][3]);
	}

	if (isActorType(0x40000392))
		mtx[1][3] = -(10.0f * (1.0f - mtx[1][1]) - mtx[1][3]);

	unkE8 = 0;
}

void TMapObjBall::control()
{
	TMapObjGeneral::control();

	if (unk194 != 0)
		unk194 -= 1;

	if (isState(STATE_HOLDING)) {
		// While carried the ball rides the holder's matrix, lifted clear of
		// the hand by unk190.
		Mtx mtx;
		MTXCopy(mHolder->getTakingMtx(), mtx);
		mtx[1][3] += unk190;
		MTXCopy(mtx, getModel()->getAnmMtx(0));
		return;
	}

	JGeometry::TVec3<f32> vel(mVelocity);
	if (!vel.isZero() || mGroundPlane->getActor() != nullptr)
		calcCurrentMtx();
}

BOOL TMapObjBall::receiveMessage(THitActor* sender, u32 message)
{
	if (TMapObjGeneral::receiveMessage(sender, message))
		return TRUE;

	if (message == HIT_MESSAGE_TAKE && (unkF8 & 0x100000)) {
		hold((TTakeActor*)sender);
		return TRUE;
	}

	// Mario walking into a ball kicks it, except for the watermelon and
	// except when he is trying to pick it up.
	if (sender->isActorType(0x80000001)) {
		if (!isActorType(0x400000D0) && message != HIT_MESSAGE_TAKE) {
			kicked();
			return TRUE;
		}
	}

	return FALSE;
}

TMapObjBall::TMapObjBall(const char* name)
    : TMapObjGeneral(name)
{
	unk148 = 0.0f;
	unk14C = 0.0f;
	unk150 = 0.0f;
	unk154 = 0.0f;
	unk158 = 0.0f;
	unk15C = 0.0f;
	unk160 = 0.0f;
	unk164 = 0.0f;
	unk168 = 0.0f;
	unk16C = 0.0f;
	unk170 = 0.0f;
	unk174 = 0.0f;
	unk178 = 0.0f;
	unk17C = 0.0f;
	unk180 = 0.0f;
	unk184 = 0.0f;
	unk188 = 0.0f;
	unk18C = 0.0f;
	unk190 = 0.0f;
	unk194 = 0;

	mInitialScaling.z = 0.0f;
	mInitialScaling.y = 0.0f;
	mInitialScaling.x = 0.0f;
}
