#include <MoveBG/MapObjBall.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/PollutionManager.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
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

void TMapObjBall::touchWall(JGeometry::TVec3<f32>* param_1,
                            TBGWallCheckRecord* param_2)
{
	// Hitting a wall while rolling on the ground pops the ball up a little,
	// scaled by how fast it was going. The watermelon is too heavy for that.
	if (!checkLiveFlag(LIVE_FLAG_AIRBORNE)) {
		if (!isActorType(0x400000D0)) {
			JGeometry::TVec3<f32> vel(mVelocity);
			mVelocity.y += unk184 * vel.length();
		}
	}

	for (int i = 0; i < param_2->mResultWallsNum; ++i) {
		const TBGCheckData* wall = param_2->mResultWalls[i];

		JGeometry::TVec3<f32> vel(mVelocity);
		f32 into = vel.x * wall->mNormal.x + vel.y * wall->mNormal.y
		    + vel.z * wall->mNormal.z;
		if (into >= 0.0f)
			continue;

		// Push the ball back out to exactly one radius from the plane.
		f32 dist = param_1->x * wall->mNormal.x + param_1->y * wall->mNormal.y
		        + param_1->z * wall->mNormal.z
		    + wall->mPlaneDistance;
		param_1->x += (mBodyRadius - dist) * wall->mNormal.x;
		param_1->z += (mBodyRadius - dist) * wall->mNormal.z;

		f32 bounce = into * -(1.0f + mMapObjData->mPhysical->unk4->unk8);
		mVelocity.x += bounce * wall->mNormal.x;
		mVelocity.z += bounce * wall->mNormal.z;

		if (isActorType(0x400000D0)) {
			if (mScaling.y >= 5.0f) {
				JGeometry::TVec3<f32> after(mVelocity);
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_WATERMELON_BROLL, &mPosition, nullptr,
				    abs(after.length()), 0, 0, nullptr, 0, 4);
			} else {
				JGeometry::TVec3<f32> after(mVelocity);
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_WATERMELON_SROLL, &mPosition, nullptr,
				    abs(after.length()), 0, 0, nullptr, 0, 4);
			}
		} else {
			u32 sound = mMapObjData->mSound->unk4->unk0[4];
			SMSGetMSound()->startSoundActorWithInfo(sound, &mPosition,
			                                        (Vec*)&mVelocity, 0.0f, 0,
			                                        0, nullptr, 0, 4);
		}
	}
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

void TMapObjBall::touchGround(JGeometry::TVec3<f32>* param_1)
{
	JGeometry::TVec3<f32> vel(mVelocity);
	f32 speed = abs(vel.length());
	if (speed > 0.05f) {
		if (isActorType(0x400000D0)) {
			// Big and small rolling samples, same split as rebound().
			if (mScaling.y >= 5.0f) {
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_WATERMELON_BROLL, &mPosition, nullptr, speed, 0,
				    0, nullptr, 0, 4);
			} else {
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_WATERMELON_SROLL, &mPosition, nullptr, speed, 0,
				    0, nullptr, 0, 4);
			}
		}
	}

	if (mGroundPlane->isWaterSurface()) {
		touchWaterSurface();
		param_1->set(mPosition);
		return;
	}

	if (gpPollution->isPolluted(param_1->x, param_1->y, param_1->z)) {
		touchPollution();
		param_1->set(mPosition);
		return;
	}

	// A slow enough impact settles instead of bouncing.
	if (mVelocity.y > -unk188) {
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		mVelocity.y = 0.0f;
		param_1->y  = mGroundHeight;
	} else {
		rebound(param_1);
	}

	// Rolling downhill: the ground normal drags the ball along.
	if (!checkLiveFlag(LIVE_FLAG_AIRBORNE)) {
		mVelocity.x += unk180 * mGroundPlane->mNormal.x;
		mVelocity.z += unk180 * mGroundPlane->mNormal.z;
	}

	mVelocity.x *= mMapObjData->mPhysical->unk4->unk10;
	mVelocity.z *= mMapObjData->mPhysical->unk4->unk10;
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

void TMapObjBall::kicked()
{
	// Only a downward or level kick does anything.
	JGeometry::TVec3<f32> vel(mVelocity);
	if (JGeometry::TVec3<f32>(vel).y > 0.0f)
		return;

	if (JGeometry::TVec3<f32>(vel).y == 0.0f) {
		mVelocity.y = unk178;
	} else {
		mVelocity.y = unk174 * SMS_GetMarioSpeedY()
		    - unk160 * JGeometry::TVec3<f32>(vel).y;
	}

	mVelocity.x += unk170 * SMS_GetMarioSpeedX();
	mVelocity.z += unk170 * SMS_GetMarioSpeedZ();

	// A ball kicked straight down would otherwise sit still, so give it a
	// random nudge in XZ.
	f32 minSpeed = mMapObjData->mPhysical->unk4->unkC;
	if (abs(mVelocity.x) < minSpeed && abs(mVelocity.z) < minSpeed) {
		mVelocity.x = 2.0f * MsRandF() - 1.0f;
		mVelocity.z = 2.0f * MsRandF() - 1.0f;
	}

	unk194 = 10;
	offLiveFlag(LIVE_FLAG_UNK10);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);

	if (!isActorType(0x400000D0)) {
		SMSGetMSound()->startSoundActor(MSD_SE_MA_KICK_DRIAN, &mPosition, 0,
		                                nullptr, 0, 4);
	}
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

void TMapObjBall::touchActor(THitActor* param_1)
{
	// unk194 is a short cooldown after a kick, so one kick cannot chain.
	if (unk194 != 0)
		return;
	if (isState(STATE_HOLDING))
		return;
	if (isHideObj(param_1))
		return;
	if (param_1->isActorType(0x08000083)
	    || param_1->isActorType(0x400000CA)
	    || param_1->isActorType(0x400000CC))
		return;

	if (param_1->isActorType(0x80000001)) {
		if (!isActorType(0x400000D0) && SMS_GetMarioSpeedY() != 0.0f) {
			kicked();
			return;
		}
	}

	boundByActor(param_1);
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

void TMapObjBall::initMapObj()
{
	TMapObjGeneral::initMapObj();

	mInitialScaling.x = mScaling.x;
	mInitialScaling.y = mScaling.y;
	mInitialScaling.z = mScaling.z;

	// Per-kind physics. The layout is identical in every arm, so the switch
	// is really a table of tunables keyed on the object type.
	switch (mActorType) {
	case 0x400000D0: // watermelon
		unk14C = 4.0f;
		unk150 = 0.0f;
		unk154 = 0.0f;
		unk158 = 0.15f;
		unk15C = 0.0f;
		unk160 = 0.9f;
		unk164 = 0.06f;
		unk168 = 1.5f;
		unk16C = 0.5f;
		unk170 = 0.5f;
		unk174 = 0.2f;
		unk178 = 2.5f;
		unk17C = 0.001f;
		unk180 = 0.3f;
		unk184 = 1.5f;
		unk188 = 1.5f;
		mBodyRadius = 50.0f * mScaling.y;
		unk18C      = mBodyRadius / 3.0f;
		break;

	case 0x40000064:
		unk148 = 0.6f;
		unk14C = 2.0f;
		unk150 = 0.02f;
		unk154 = 0.0f;
		unk158 = 0.055f;
		unk15C = 0.02f;
		unk160 = 0.83f;
		unk170 = 0.9f;
		unk174 = 0.13f;
		unk178 = 20.0f;
		unk164 = 0.5f;
		unk168 = 0.02f;
		unk16C = 0.5f;
		unk17C = 1.2f;
		unk180 = 0.8f;
		unk184 = 1.0f;
		unk188 = 1.5f;
		mBodyRadius = 50.0f * mScaling.y;
		unk18C      = mBodyRadius / 3.0f;
		break;

	case 0x40000390:
	case 0x40000391:
	case 0x40000392:
		unk148 = 0.4f;
		unk14C = 0.2f;
		unk150 = 1.3f;
		unk154 = 0.0f;
		unk158 = 1.2f;
		unk15C = 0.8f;
		unk160 = 0.5f;
		unk170 = 0.9f;
		unk174 = 0.13f;
		unk178 = 20.0f;
		unk164 = 2.0f;
		unk168 = 0.02f;
		unk16C = 0.3f;
		unk17C = 0.05f;
		unk180 = 0.5f;
		unk184 = 1.0f;
		unk188 = 1.5f;
		mBodyRadius = 50.0f * mScaling.y;
		unk18C      = 50.0f;
		break;

	case 0x40000393:
		unk148 = 0.6f;
		unk14C = 0.2f;
		unk150 = 1.3f;
		unk154 = 15.0f;
		unk158 = 0.5f;
		unk15C = 1.3f;
		unk160 = 1.0f;
		unk170 = 0.9f;
		unk174 = 0.13f;
		unk178 = 20.0f;
		unk164 = 2.0f;
		unk168 = 0.02f;
		unk16C = 0.3f;
		unk17C = 0.05f;
		unk180 = 0.5f;
		unk184 = 1.0f;
		unk188 = 1.5f;
		mBodyRadius = 50.0f * mScaling.y;
		unk18C      = 50.0f;
		break;

	case 0x40000394:
		unk148 = 0.2f;
		unk14C = 0.0f;
		unk150 = 0.0f;
		unk154 = 0.0f;
		unk158 = 0.0f;
		unk15C = 0.0f;
		unk160 = 0.0f;
		unk170 = 0.0f;
		unk174 = 0.0f;
		unk178 = 0.0f;
		unk164 = 0.0f;
		unk168 = 0.0f;
		unk16C = 0.0f;
		unk17C = 0.05f;
		unk180 = 0.5f;
		unk184 = 1.0f;
		unk188 = 1.5f;
		mBodyRadius = 50.0f * mScaling.y;
		unk18C      = 50.0f;
		break;

	case 0x40000395:
		unk148 = 0.4f;
		unk14C = 0.2f;
		unk150 = 1.3f;
		unk154 = 0.0f;
		unk158 = 1.2f;
		unk15C = 0.8f;
		unk160 = 0.5f;
		unk170 = 0.9f;
		unk174 = 0.13f;
		unk178 = 20.0f;
		unk164 = 2.0f;
		unk168 = 0.02f;
		unk16C = 0.3f;
		unk17C = 0.05f;
		unk180 = 0.5f;
		unk184 = 1.0f;
		unk188 = 1.5f;
		mBodyRadius = 50.0f * mScaling.y;
		unk18C      = 50.0f;
		break;

	}

	// unk190 is the lift applied while the ball is carried.
	if (isActorType(0x40000393)) {
		mBodyRadius = 45.0f * mScaling.y;
		unk190      = mBodyRadius;
	}
	if (isActorType(0x40000390)) {
		mBodyRadius = 40.0f * mScaling.y;
		unk190      = 20.0f;
	}
	if (isActorType(0x40000391)) {
		mBodyRadius = 40.0f * mScaling.y;
		unk190      = 20.0f;
	}
	if (isActorType(0x40000392))
		unk190 = 10.0f;
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
