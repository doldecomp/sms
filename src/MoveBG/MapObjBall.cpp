#include <MoveBG/MapObjBall.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <MSound/MSound.hpp>
#include <System/Application.hpp>
#include <System/MarDirector.hpp>
#include <System/FlagManager.hpp>
#include <Player/MarioAccess.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <stdlib.h>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Enemy/PoiHana.hpp>
#include <MoveBG/Item.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

u32 TResetFruit::mFruitLivingTime       = 14400;
f32 TResetFruit::mScaleUpSpeed          = 1.05f;
f32 TResetFruit::mBreakingScaleSpeed    = 0.96f;
u32 TResetFruit::mFruitWaitTimeToAppear = 360;

void TMapObjBall::touchRoof(JGeometry::TVec3<f32>* pos)
{
	if (pos->y > unk140) {
		pos->y = unk140;
	}

	calcReflectingVelocity(unk13C, mMapObjData->mPhysical->unk4->unk4,
	                       &mVelocity);
}

void TMapObjBall::touchWall(JGeometry::TVec3<f32>* pos,
                            TBGWallCheckRecord* record)
{
	if (!isAirborne() && !isActorType(0x400000d0)) {
		f32 speed = getVelocity().length();
		mVelocity.y += unk184 * speed;
	}

	for (int i = 0; i < record->mResultWallsNum; ++i) {
		const TBGCheckData* wall = record->mResultWalls[i];
		f32 velDot               = getVelocity().dot(wall->getNormal());
		if (velDot < 0.0f) {
			f32 dist = pos->dot(wall->getNormal()) + wall->getPlaneDistance();
			pos->x += (mBodyRadius - dist) * wall->getNormal().x;
			pos->z += (mBodyRadius - dist) * wall->getNormal().z;
			f32 bounce = velDot * -(-1.0f + mMapObjData->mPhysical->unk4->unk8);
			mVelocity.x += bounce * wall->getNormal().x;
			mVelocity.z += bounce * wall->getNormal().z;
			if (isActorType(0x400000d0)) {
				if (mScaling.y >= 5.0f) {
					f32 volume = abs(getVelocity().length());
					SMSGetMSound()->startSoundActorWithInfo(
					    MSD_SE_OBJ_WATERMELON_BROLL, &mPosition, nullptr,
					    volume, 0, 0, nullptr, 0, 4);
				} else {
					f32 volume = abs(getVelocity().length());
					SMSGetMSound()->startSoundActorWithInfo(
					    MSD_SE_OBJ_WATERMELON_SROLL, &mPosition, nullptr,
					    volume, 0, 0, nullptr, 0, 4);
				}
			} else {
				SMSGetMSound()->startSoundActorWithInfo(
				    mMapObjData->mSound->unk4->unk0[4], &mPosition, &mVelocity,
				    0.0f, 0, 0, nullptr, 0, 4);
			}
		}
	}
}

void TMapObjBall::touchPollution() { kill(); }

void TMapObjBall::touchWaterSurface() { kill(); }

void TMapObjBall::rebound(JGeometry::TVec3<f32>* wall)
{
	calcReflectingVelocity(mGroundPlane, mMapObjData->mPhysical->unk4->unk4,
	                       &mVelocity);
	wall->y = mGroundHeight;
	onLiveFlag(LIVE_FLAG_AIRBORNE);

	if (isActorType(0x400000d0)) {
		f32 soundY;
		if (mScaling.y >= 5.0f) {
			soundY = abs(mGroundPlane->getNormal().y);
			SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_WATERMELON_BBUND,
			                                        &mPosition, nullptr, soundY,
			                                        0, 0, nullptr, 0, 4);
		} else {
			soundY = abs(mGroundPlane->getNormal().y);
			SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_WATERMELON_SBUND,
			                                        &mPosition, nullptr, soundY,
			                                        0, 0, nullptr, 0, 4);
		}
	} else {
		u32 soundID = mMapObjData->mSound->unk4->unk0[4];
		SMSGetMSound()->startSoundActorWithInfo(soundID, &mPosition, &mVelocity,
		                                        0.0f, 0, 0, nullptr, 0, 4);
	}
}

void TMapObjBall::touchGround(JGeometry::TVec3<f32>* ground)
{
	f32 speed = abs(getVelocity().length());
	if (speed > 0.05f && isActorType(0x400000d0)) {
		if (mScaling.y >= 5.0f) {
			SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_WATERMELON_BROLL,
			                                        &mPosition, nullptr, speed,
			                                        0, 0, nullptr, 0, 4);
		} else {
			SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_WATERMELON_SROLL,
			                                        &mPosition, nullptr, speed,
			                                        0, 0, nullptr, 0, 4);
		}
	}

	if (mGroundPlane->isWaterSurface()) {
		touchWaterSurface();
		ground->set(mPosition);
	} else {
		if (gpPollution->isPolluted(ground->x, ground->y, ground->z)) {
			touchPollution();
			ground->set(mPosition);
		} else {
			if (mVelocity.y > -unk188) {
				offLiveFlag(LIVE_FLAG_AIRBORNE);
				mVelocity.y = 0.0f;
				ground->y   = mGroundHeight;
			} else {
				rebound(ground);
			}

			if (!checkLiveFlag2(LIVE_FLAG_AIRBORNE)) {
				mVelocity.x += unk180 * mGroundPlane->getNormal().x;
				mVelocity.z += unk180 * mGroundPlane->getNormal().z;
			}

			mVelocity.x *= mMapObjData->mPhysical->unk4->unk10;
			mVelocity.z *= mMapObjData->mPhysical->unk4->unk10;

			if (isActorType(0x400000d0)
			    && (abs(mVelocity.x) > mMapObjData->mPhysical->unk4->unkC
			        || abs(mVelocity.z) > mMapObjData->mPhysical->unk4->unkC)) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_SLIP, &mPosition, 0,
				                                nullptr, 0, 4);
			}
		}
	}
}

void TMapObjBall::put()
{
	TMapObjGeneral::put();
	calcCurrentMtx();
}

void TMapObjBall::hold(TTakeActor* holder)
{
	if (getVelocity().length() > 10.0f) {
		return;
	}

	TMapObjGeneral::hold(holder);
	mVelocity.zero();
}

void TMapObjBall::kicked()
{
	if (getVelocity().y > 0.0f)
		return;

	if (getVelocity().y == 0.0f) {
		mVelocity.y = unk178;
	} else {
		mVelocity.y = unk174 * *gpMarioSpeedY - unk160 * getVelocity().y;
	}

	mVelocity.x += unk170 * *gpMarioSpeedX;
	mVelocity.z += unk170 * *gpMarioSpeedZ;

	f32 unkC = mMapObjData->mPhysical->unk4->unkC;
	if (abs(mVelocity.x) < unkC && abs(mVelocity.z) < unkC) {
		mVelocity.x = MsRandF() * 2.0f - 1.0f;
		mVelocity.z = MsRandF() * 2.0f - 1.0f;
	}

	unk194 = 10;
	offLiveFlag(LIVE_FLAG_UNK10);
	onLiveFlag(LIVE_FLAG_AIRBORNE);

	THitActor* marHitActor = SMS_GetMarioHitActor();
	marHitActor->receiveMessage(this, HIT_MESSAGE_ATTACK);
	if (!isActorType(0x400000d0)) {
		SMSGetMSound()->startSoundActor(MSD_SE_MA_KICK_DRIAN, &mPosition, 0,
		                                nullptr, 0, 4);
	}
}

u32 TMapObjBall::touchWater(THitActor* water)
{
	if (isState(STATE_HOLDING) || isState(STATE_APPEARING))
		return 1;

	JGeometry::TVec3<f32> vel;
	vel.set(getVelocity());
	vel.scaleAdd(unk17C, vel, getWaterSpeed(water));
	setVelocity(vel);
	offLiveFlag(LIVE_FLAG_UNK10);
	return 1;
}

void TMapObjBall::boundByActor(THitActor* actor)
{
	JGeometry::TVec3<f32> offsetToActor(actor->mPosition.x - mPosition.x, 0.0f,
	                                    actor->mPosition.z - mPosition.z);

	f32 radius = isActorType(0x400000d0) ? mAttackRadius + actor->mDamageRadius
	                                     : mDamageRadius;
	if (radius * radius
	    < offsetToActor.x * offsetToActor.x + offsetToActor.z * offsetToActor.z)
		return;

	if (offsetToActor.x != 0.0f && offsetToActor.z != 0.0f)
		MsVECNormalize(offsetToActor, offsetToActor);

	if (actor->isActorType(0x80000001)) {
		if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK2000000)) {
			f32 minSpeed = mMapObjData->mPhysical->unk4->unkC;
			if (abs(*gpMarioSpeedX) > minSpeed
			    || abs(*gpMarioSpeedZ) > minSpeed) {
				mVelocity.y += unk150;
				if (!isActorType(0x400000d0))
					SMSGetMSound()->startSoundActor(
					    MSD_SE_MA_KICK_DRIAN, &mPosition, 0, nullptr, 0, 4);
			} else {
				mVelocity.y += unk154;
			}

			mVelocity.x += unk148 * *gpMarioSpeedX - offsetToActor.x * unk14C;
			mVelocity.z += unk148 * *gpMarioSpeedZ - offsetToActor.z * unk14C;
			actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
		}
	} else {
		f32 approach = getVelocity().dot(offsetToActor);
		if (approach >= 0.0f
		    && abs(getVelocity().x) > mMapObjData->mPhysical->unk4->unkC
		    && abs(getVelocity().z) > mMapObjData->mPhysical->unk4->unkC) {
			mVelocity.x -= (1.0f + unk16C) * (offsetToActor.x * approach);
			mVelocity.y += unk168;
			mVelocity.z -= (1.0f + unk16C) * (offsetToActor.z * approach);
			actor->receiveMessage(this, HIT_MESSAGE_UNK10);
			if (!isActorType(0x400000d0))
				SMSGetMSound()->startSoundActor(MSD_SE_IT_DRIAN_BOUND,
				                                &mPosition, 0, nullptr, 0, 4);
		} else {
			mVelocity.x -= offsetToActor.x * unk164;
			mVelocity.y += unk168;
			mVelocity.z -= offsetToActor.z * unk164;
		}
	}

	if (actor->isActorType(0x80000001)
	    && !checkMapObjFlag(MAP_OBJ_FLAG_UNK2000000) && getVelocity().y < 0.0f
	    && gpMarioPos->y + 130.0f < mPosition.y + mBodyRadius) {
		mVelocity.y = -unk160 * getVelocity().y;
		mVelocity.x += unk158 * *gpMarioSpeedX;
		mVelocity.y += unk15C * *gpMarioSpeedY;
		mVelocity.z += unk158 * *gpMarioSpeedZ;
		if (!isActorType(0x400000d0))
			SMSGetMSound()->startSoundActor(MSD_SE_MA_KICK_DRIAN, &mPosition, 0,
			                                nullptr, 0, 4);
	}

	unk194 = 10;
	offLiveFlag(LIVE_FLAG_UNK10);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
}

void TMapObjBall::touchActor(THitActor* actor)
{
	if ((!unk194 == 0 || isState(STATE_HOLDING))
	    || TMapObjBase::isHideObj(actor) || actor->isActorType(0x8000083)
	    || actor->isActorType(0x400000ca) || actor->isActorType(0x400000cc)) {
		return;
	} else {
		if (actor->isActorType(0x80000001) && !isActorType(0x400000d0)
		    && *gpMarioSpeedY != 0.0f) {
			kicked();
		} else {
			boundByActor(actor);
		}
	}
}

void TMapObjBall::calcCurrentMtx()
{
	TRotation3f rot;
	rot.identity();

	if (abs(getVelocity().x) < mMapObjData->mPhysical->unk4->unkC
	    && abs(getVelocity().z) < mMapObjData->mPhysical->unk4->unkC
	    && mGroundPlane->getNormal().y == 1.0f) {
		mVelocity.x = 0.0f;
		mVelocity.z = 0.0f;
	}

	if (abs(getVelocity().x) > mMapObjData->mPhysical->unk4->unkC
	    || abs(getVelocity().z) > mMapObjData->mPhysical->unk4->unkC) {
		JGeometry::TVec3<f32> axis;
		getVerticalVecToTargetXZ(mPosition.x + getVelocity().x,
		                         mPosition.z + getVelocity().z, &axis);
		JGeometry::TVec3<f32> vel = getVelocity();
		f32 dist = JGeometry::TUtil<f32>::sqrt(vel.x * vel.x + vel.z * vel.z);
		rot.setRotate(axis, dist / mBodyRadius * 2.0f);
	}

	TMtx34f anmMtx;
	anmMtx.set(getModel()->getAnmMtx(0));
	anmMtx.ref(0, 3) = 0.0f;
	anmMtx.ref(1, 3) = 0.0f;
	anmMtx.ref(2, 3) = 0.0f;
	MTXConcat(rot, anmMtx, rot);
	rot.ref(0, 3) = mPosition.x;
	rot.ref(1, 3) = mPosition.y + mBodyRadius;
	rot.ref(2, 3) = mPosition.z;
	if (isActorType(0x40000394) && rot.ref(1, 1) > 0.0f)
		rot.ref(1, 3) -= 50.0f * rot.ref(1, 1);
	if (isActorType(0x40000392))
		rot.ref(1, 3) -= 10.0f * (1.0f - rot.ref(1, 1));

	getModel()->setAnmMtx(0, rot);
}

void TMapObjBall::checkWallCollision(JGeometry::TVec3<f32>* wall)
{
	JGeometry::TVec3<f32> center;
	center.x = wall->x;
	center.y = wall->y + mBodyRadius;
	center.z = wall->z;
	TBGWallCheckRecord wallRecord(center, mBodyRadius, 4,
	                              mMapObjData->mPhysical->mWallCheckFlags);
	if (gpMap->isTouchedWallsAndMoveXZ(&wallRecord)) {
		unk138  = wallRecord.mResultWalls[0];
		wall->x = wallRecord.mCenter.x;
		wall->z = wallRecord.mCenter.z;
		touchWall(wall, &wallRecord);
	} else {
		unk138 = nullptr;
	}
}

void TMapObjBall::makeObjDefault()
{
	TMapObjBase::makeObjDefault();
	J3DModel* model   = TLiveActor::getModel();
	MtxPtr nodeMatrix = model->getAnmMtx(0);
	nodeMatrix[0][3]  = mPosition.x;
	nodeMatrix[1][3]  = mPosition.y + mBodyRadius;
	nodeMatrix[2][3]  = mPosition.z;
}

void TMapObjBall::makeObjAppeared()
{
	TMapObjBase::makeObjAppeared();
	calcCurrentMtx();
	J3DModel* model   = TLiveActor::getModel();
	MtxPtr nodeMatrix = model->getAnmMtx(0);
	nodeMatrix[0][3]  = mPosition.x;
	nodeMatrix[1][3]  = mPosition.y + mBodyRadius;
	nodeMatrix[2][3]  = mPosition.z;

	if (isActorType(0x40000394)) {
		if (nodeMatrix[1][1] > 0.0f) {
			nodeMatrix[1][3] -= 50.0f * nodeMatrix[1][1];
		}
	}

	if (isActorType(0x40000392)) {
		nodeMatrix[1][3] -= 10.0f * (1.0f - nodeMatrix[1][1]);
	}

	unkE8 = 0;
}

void TMapObjBall::control()
{
	TMapObjGeneral::control();
	if (unk194 != 0) {
		unk194 -= 1;
	}

	if (isState(STATE_HOLDING)) {
		Mtx mtx;
		MTXCopy(mHolder->getTakingMtx(), mtx);
		mtx[1][3] += unk190;
		getModel()->setAnmMtx(0, mtx);
	} else {
		if (!getVelocity().isZero() || mGroundPlane->getActor() != nullptr) {
			calcCurrentMtx();
		}
	}
}

BOOL TMapObjBall::receiveMessage(THitActor* actor, u32 msg)
{
	if (TMapObjGeneral::receiveMessage(actor, msg) != 0) {
		return TRUE;
	}

	if (msg == HIT_MESSAGE_TAKE) {
		if (checkMapObjFlag(MAP_OBJ_FLAG_UNK100000)) {
			hold(static_cast<TTakeActor*>(actor));
			return TRUE;
		}
	}

	if (actor->isActorType(0x80000001) && !isActorType(0x400000d0)) {
		if (msg != HIT_MESSAGE_TAKE) {
			kicked();
			return TRUE;
		}
	}

	return FALSE;
}

void TMapObjBall::initMapObj()
{
	TMapObjGeneral::initMapObj();
	mInitialScaling.set(mScaling);
	switch (mActorType) {
	case 0x400000d0: {
		unk14C      = 4.0f;
		unk150      = 0.0f;
		unk154      = 0.0f;
		unk158      = 0.15f;
		unk15C      = 0.0f;
		unk160      = 0.9f;
		unk164      = 0.06f;
		unk168      = 1.5f;
		unk16C      = 0.5f;
		unk170      = 0.5f;
		unk174      = 0.2f;
		unk178      = 2.5f;
		unk17C      = 0.001f;
		unk180      = 0.3f;
		unk184      = 1.5f;
		unk188      = 1.5f;
		mBodyRadius = 50.0f * mScaling.y;
		unk18C      = mBodyRadius / 3.0f;
		break;
	}
	case 0x40000064: {
		unk148      = 0.6f;
		unk14C      = 2.0f;
		unk150      = 0.02f;
		unk154      = 0.0f;
		unk158      = 0.055f;
		unk15C      = 0.02f;
		unk160      = 0.83f;
		unk170      = 0.9f;
		unk174      = 0.13f;
		unk178      = 20.0f;
		unk164      = 0.5f;
		unk168      = 0.02f;
		unk16C      = 0.5f;
		unk17C      = 1.2f;
		unk180      = 0.8f;
		unk184      = 1.0f;
		unk188      = 1.5f;
		mBodyRadius = 50.0f * mScaling.y;
		unk18C      = mBodyRadius / 3.0f;
		break;
	}
	case 0x40000393: {
		unk148      = 0.6f;
		unk14C      = 0.2f;
		unk150      = 1.3f;
		unk154      = 15.0f;
		unk158      = 0.5f;
		unk15C      = 1.3f;
		unk160      = 1.0f;
		unk170      = 0.9f;
		unk174      = 0.13f;
		unk178      = 20.0f;
		unk164      = 2.0f;
		unk168      = 0.02f;
		unk16C      = 0.3f;
		unk17C      = 0.05f;
		unk180      = 0.5f;
		unk184      = 1.0f;
		unk188      = 1.5f;
		mBodyRadius = 50.0f * mScaling.y;
		unk18C      = 50.0f;
		break;
	}
	case 0x40000390:
	case 0x40000391:
	case 0x40000392: {
		unk148      = 0.4f;
		unk14C      = 0.2f;
		unk150      = 1.3f;
		unk154      = 0.0f;
		unk158      = 1.2f;
		unk15C      = 0.8f;
		unk160      = 0.5f;
		unk170      = 0.9f;
		unk174      = 0.13f;
		unk178      = 20.0f;
		unk164      = 2.0f;
		unk168      = 0.02f;
		unk16C      = 0.3f;
		unk17C      = 0.05f;
		unk180      = 0.5f;
		unk184      = 1.0f;
		unk188      = 1.5f;
		mBodyRadius = 50.0f * mScaling.y;
		unk18C      = 50.0f;
		break;
	}
	case 0x40000394: {
		unk148      = 0.2f;
		unk14C      = 0.0f;
		unk150      = 0.0f;
		unk154      = 0.0f;
		unk158      = 0.0f;
		unk15C      = 0.0f;
		unk160      = 0.0f;
		unk170      = 0.0f;
		unk174      = 0.0f;
		unk178      = 0.0f;
		unk164      = 0.0f;
		unk168      = 0.0f;
		unk16C      = 0.0f;
		unk17C      = 0.05f;
		unk180      = 0.5f;
		unk184      = 1.0f;
		unk188      = 1.5f;
		mBodyRadius = 50.0f * mScaling.y;
		unk18C      = 50.0f;
		break;
	}
	case 0x40000395: {
		unk148      = 0.4f;
		unk14C      = 0.2f;
		unk150      = 1.3f;
		unk154      = 0.0f;
		unk158      = 1.2f;
		unk15C      = 0.8f;
		unk160      = 0.5f;
		unk170      = 0.9f;
		unk174      = 0.13f;
		unk178      = 20.0f;
		unk164      = 2.0f;
		unk168      = 0.02f;
		unk16C      = 0.3f;
		unk17C      = 0.05f;
		unk180      = 0.5f;
		unk184      = 1.0f;
		unk188      = 1.5f;
		mBodyRadius = 50.0f * mScaling.y;
		unk18C      = 50.0f;
		break;
	}
	}

	if (isActorType(0x40000393)) {
		mBodyRadius = mScaling.y * 45.0f;
		unk190      = mBodyRadius;
	}
	if (isActorType(0x40000390)) {
		mBodyRadius = mScaling.y * 40.0f;
		unk190      = 20.0f;
	}
	if (isActorType(0x40000391)) {
		mBodyRadius = mScaling.y * 40.0f;
		unk190      = 20.0f;
	}
	if (isActorType(0x40000392)) {
		unk190 = 10.0f;
	}
}

TMapObjBall::TMapObjBall(const char* name)
    : TMapObjGeneral(name)
{
	unk148            = 0.f;
	unk14C            = 0.f;
	unk150            = 0.f;
	unk154            = 0.f;
	unk158            = 0.f;
	unk15C            = 0.f;
	unk160            = 0.f;
	unk164            = 0.f;
	unk168            = 0.f;
	unk16C            = 0.f;
	unk170            = 0.f;
	unk174            = 0.f;
	unk178            = 0.f;
	unk17C            = 0.0f;
	unk180            = 0.0f;
	unk184            = 0.0f;
	unk188            = 0.0f;
	unk18C            = 0.0f;
	unk190            = 0.0f;
	unk194            = 0;
	mInitialScaling.z = 0.0f;
	mInitialScaling.y = 0.0f;
	mInitialScaling.x = 0.0f;
}

void TResetFruit::checkGroundCollision(JGeometry::TVec3<f32>* ground)
{
	if (gpMarDirector->getCurrentMap() != 7
	    && gpMarDirector->getCurrentMap() != 4) {
		TMapObjGeneral::checkGroundCollision(ground);
	} else if (gpMarDirector->getCurrentMap() == 4) {
		mGroundHeight = gpMap->checkGround(ground->x, ground->y + 200.0f,
		                                   ground->z, &mGroundPlane);
		mGroundHeight += 1.0f;
		if (ground->y <= mGroundHeight) {
			touchGround(ground);
		} else {
			onLiveFlag(LIVE_FLAG_AIRBORNE);
		}
	} else {
		mGroundHeight = gpMap->checkGround(ground->x, ground->y + mHeadHeight,
		                                   ground->z, &mGroundPlane);

		bool bVar1;
		if (mGroundPlane->mBGType
		        == BG_TYPE_EVERYTHING_BUT_MAP_OBJECTS_PHASE_THROUGH
		    || mGroundPlane->mBGType == BG_TYPE_MAP_CHANGE_PHASE_THROUGH) {
			bVar1 = true;
		} else {
			bVar1 = false;
		}

		if (bVar1) {
			mGroundHeight = gpMap->checkGroundExactY(
			    ground->x, mGroundHeight - 200.0f, ground->z, &mGroundPlane);
		}

		mGroundHeight += 1.0f;
		if (ground->y <= mGroundHeight) {
			touchGround(ground);
		} else {
			onLiveFlag(LIVE_FLAG_AIRBORNE);
		}
	}
}

void TResetFruit::waitingToAppear()
{
	if (gpMarDirector->getCurrentMap() == 3 && unk1A4 != 0) {
		makeObjDead();
	}

	if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK4000000) && !isStateTimerEngaged()
	    && mColCount == 0) {
		onMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
		makeObjAppeared();
		Mtx scaleMat;
		MTXScale(scaleMat, 0.2f, 0.2f, 0.2f);
		MtxPtr nodeMats = getModel()->getAnmMtx(0);
		TMapObjBase::concatOnlyRotFromLeft(scaleMat, getModel()->getAnmMtx(0),
		                                   nodeMats);

		mScaling.y = 0.2f;
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mState = STATE_APPEARING;
		SMSGetMSound()->startSoundActor(MSD_SE_IT_COMMON_APPEAR, &mPosition, 0,
		                                nullptr, 0, 4);
	}
}

void TResetFruit::makeObjWaitingToAppear()
{
	mState = STATE_LIVING;
	makeObjDefault();
	makeObjDead();
	calcRootMatrix();
	getModel()->calc();
	startStateTimer(mFruitWaitTimeToAppear);
	offMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
	mState = STATE_WAITING_TO_APPEAR;
	if (gpMarDirector->getCurrentMap() == 3 && unk1A4 != 0)
		makeObjDead();
}

void TResetFruit::thrown()
{
	TMapObjGeneral::thrown();
	mState = STATE_LIVING;
}

void TResetFruit::hold(TTakeActor* actor)
{
	TMapObjBall::hold(actor);
	mVelocity.zero();
	onLiveFlag(LIVE_FLAG_UNK10);
	if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK4000000) && !isStateTimerEngaged()) {
		onMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
		startStateTimer(getLivingTime());
	}
}

void TResetFruit::touchPollution()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_MOE_FIRE_OFF,
	                                            &mPosition, 0, nullptr);
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_AWAY_INTO_GRAF, &mPosition, 0,
	                                nullptr, 0, 4);

	makeObjDefault();
	makeObjWaitingToAppear();
}

void TResetFruit::touchWaterSurface()
{
	TMapObjBase::emitColumnWater();
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_DRINA_TO_WATER, &mPosition, 0,
	                                nullptr, 0, 4);
	makeObjWaitingToAppear();
}

u32 TResetFruit::touchWater(THitActor* water)
{
	TMapObjBall::touchWater(water);
	makeObjLiving();
	return 1;
}

void TResetFruit::touchActor(THitActor* actor) { pick(actor); }

void TResetFruit::touchGround(JGeometry::TVec3<f32>* ground)
{
	if (getGroundPlane()->isDeathPlane()) {
		makeObjWaitingToAppear();
		ground->set(getPosition());
	} else {
		TMapObjBall::touchGround(ground);
	}
}

void TResetFruit::makeObjLiving()
{
	if (!isStateTimerEngaged()) {
		onMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
		startStateTimer(getLivingTime());
	}

	offLiveFlag(LIVE_FLAG_UNK10);
	mState = STATE_LIVING;
}

void TResetFruit::pick(THitActor* actor)
{
	if (isState(STATE_APPEARING) || isState(STATE_BREAKING)
	    || isState(STATE_ROTTING) || isState(STATE_WAITING_TO_APPEAR))
		return;

	TMapObjBall::touchActor(actor);
	if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK4000000) && isState(STATE_NORMAL)
	    && !checkLiveFlag(LIVE_FLAG_UNK10))
		makeObjLiving();
}

void TResetFruit::kicked()
{
	if (checkMapObjFlag(MAP_OBJ_FLAG_UNK2000000) || isState(STATE_HOLDING))
		return;

	if (*gpMarioSpeedY < 0.0f)
		return;

	JGeometry::TVec3<f32> toMario(gpMarioPos->x - mPosition.x, 0.0f,
	                              gpMarioPos->z - mPosition.z);
	if (getVelocity().y <= 0.0f && isAirborne()
	    && getVelocity().dot(toMario) > 0.0f) {
		if (getVelocity().y == 0.0f)
			mVelocity.y = unk178;
		else
			mVelocity.y = unk174 * *gpMarioSpeedY - unk160 * getVelocity().y;

		mVelocity.x += unk170 * *gpMarioSpeedX;
		mVelocity.z += unk170 * *gpMarioSpeedZ;

		f32 minSpeed = mMapObjData->mPhysical->unk4->unkC;
		if (abs(mVelocity.x) < minSpeed && abs(mVelocity.z) < minSpeed) {
			mVelocity.x = MsRandF() * 2.0f - 1.0f;
			mVelocity.z = MsRandF() * 2.0f - 1.0f;
		}

		unk194 = 10;
		offLiveFlag(LIVE_FLAG_UNK10);
		SMS_GetMarioHitActor()->receiveMessage(this, HIT_MESSAGE_ATTACK);
		SMSGetMSound()->startSoundActor(MSD_SE_MA_KICK_DRIAN, &mPosition, 0,
		                                nullptr, 0, 4);
	}
}

void TResetFruit::living()
{
	TMapObjBall::control();
	if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK4000000) && !isStateTimerEngaged()) {
		if (mHolder != nullptr) {
			mHolder->receiveMessage(this, HIT_MESSAGE_UNK8);
			mHolder->mHeldObject = nullptr;
			mHolder              = nullptr;
		}

		mVelocity.setAll(0.0f);
		mState = STATE_ROTTING;
	}
}

void TResetFruit::waitEffect()
{
	mPosition.y += mBodyRadius / 2.0f;
	mScaling.set(mInitialScaling);
	emitAndScale(PARTICLE_MS_ENM_DISAP_A_W, 0, &mPosition);
	SMSGetMSound()->startSoundActor(MSD_SE_SMOKE_EFFECT, &mPosition, 0, nullptr,
	                                0, 4);
	startStateTimer(240);
	sleep();
	mState = STATE_WAIT_EFFECT;
}

void TResetFruit::rotting() { }

void TResetFruit::breaking()
{
	Mtx scaleMtx;
	MTXScale(scaleMtx, 1.0f, mBreakingScaleSpeed, 1.0f);
	J3DModel* model = getModel();
	MtxPtr nodeMat  = model->getAnmMtx(0);
	TMapObjBase::concatOnlyRotFromLeft(scaleMtx, nodeMat, nodeMat);
	mScaling.y *= mBreakingScaleSpeed;
	nodeMat[1][3] = mBodyRadius * mScaling.y + mPosition.y;
	if (mScaling.y < 0.2f) {
		waitEffect();
	}
}

void TResetFruit::appearing()
{
	MtxPtr mtx;
	Mtx scaleMtx;
	MTXScale(scaleMtx, mScaleUpSpeed, mScaleUpSpeed, mScaleUpSpeed);
	mtx = getModel()->getAnmMtx(0);
	concatOnlyRotFromLeft(scaleMtx, mtx, mtx);
	mScaling.y        = mScaling.y * mScaleUpSpeed;
	mScaledBodyRadius = mBodyRadius * mScaling.y;
	mtx[1][3]         = mBodyRadius * mScaling.y + mPosition.y;
	if (mScaling.y >= mInitialScaling.y) {
		mScaling.set(mInitialScaling);
		J3DModel* model = TLiveActor::getModel();
		model->calc();
		offHitFlag(HIT_FLAG_NO_COLLISION);
		makeObjAppeared();
		mState = STATE_NORMAL;
	}
}

void TResetFruit::control()
{
	switch (mState) {
	case STATE_NORMAL: {
		offHitFlag(HIT_FLAG_NO_COLLISION);
		for (int i = 0; i < mColCount; ++i)
			pick(mCollisions[i]);

		if (mGroundPlane->getActor() != nullptr) {
			calcCurrentMtx();
		}
		break;
	}
	case STATE_LIVING: {
		offHitFlag(HIT_FLAG_NO_COLLISION);
		if (gpMarDirector->getCurrentMap() == 4
		    && checkLiveFlag(LIVE_FLAG_UNK10)) {
			offLiveFlag(LIVE_FLAG_UNK10);
		}

		if (mGroundPlane->getActor() != nullptr) {
			if (checkLiveFlag(LIVE_FLAG_UNK10)) {
				offLiveFlag(LIVE_FLAG_UNK10);
			}

			const TLiveActor* actor = mGroundPlane->getActor();
			if (mPosition.y < mGroundHeight + 200.0f
			    && !actor->isActorType(0x400000cd)) {
				if (actor->isActorType(0x400000cd)) {
					f32 prevUnk198 = unk198;
					unk198         = SMS_GetSandRiseUpRatio(actor);
					if (unk198 > 0.05f && unk198 > prevUnk198) {
						mVelocity.y += 20.0f;
					}
				}
			}
		} else {
			unk198 = 0.0f;
		}

		living();
		break;
	}
	case STATE_HOLDING: {
		living();
		break;
	}
	case STATE_APPEARING:
	case STATE_BREAKING:
		TMapObjBall::control();
		break;
	case STATE_ROTTING:
		waitEffect();
		break;
	case STATE_WAIT_EFFECT:
		if (!isStateTimerEngaged()) {
			mFruitColor.r = 0xff;
			mFruitColor.g = 0xff;
			mFruitColor.b = 0xff;
			awake();
			makeObjWaitingToAppear();
		}
		break;
	}
}

void TResetFruit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (gpMarDirector->getCurrentMap() == 7) {
		if (isState(STATE_HOLDING) || !getVelocity().isZero()) {
			if (checkLiveFlag(LIVE_FLAG_UNK200)) {
				offLiveFlag(LIVE_FLAG_UNK200);
			}
		} else {
			if (!gpCubeArea->isInAreaCube(mPosition) && isState(STATE_LIVING)) {
				if (mPosition.x != mInitialPosition.x
				    || mPosition.z != mInitialPosition.z) {
					makeObjWaitingToAppear();
					return;
				}
			}
		}
	}

	TMapObjGeneral::perform(cue, graphics);
}

void TResetFruit::killByTimer(int timer)
{
	startStateTimer(timer);
	onMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
	mState = STATE_LIVING;
}

void TResetFruit::makeObjAppeared()
{
	if (checkMapObjFlag(MAP_OBJ_FLAG_UNK4000000)) {
		makeObjDefault();
	}

	TMapObjBase::makeObjAppeared();
	calcCurrentMtx();
	J3DModel* model = getModel();
	MtxPtr nodeMat  = model->getAnmMtx(0);
	nodeMat[0][3]   = mPosition.x;
	nodeMat[1][3]   = mPosition.y + mBodyRadius;
	nodeMat[2][3]   = mPosition.z;
	if (isActorType(0x40000394) && nodeMat[1][1] > 0.0f) {
		nodeMat[1][3] -= 50.0f * nodeMat[1][1];
	}

	if (isActorType(0x40000392)) {
		nodeMat[1][3] -= 10.0f * (1.0f - nodeMat[1][1]);
	}

	unkE8 = 0;
	if (checkMapObjFlag(MAP_OBJ_FLAG_UNK4000000)) {
		mState = STATE_LIVING;
	}
}

BOOL TResetFruit::receiveMessage(THitActor* actor, u32 msg)
{
	BOOL res;
	if (msg == HIT_MESSAGE_UNKB) {
		if (isState(STATE_NORMAL) || isState(STATE_HOLDING)
		    || isState(STATE_LIVING)) {
			makeObjWaitingToAppear();
			res = TRUE;
		} else {
			res = FALSE;
		}
	} else if (msg == HIT_MESSAGE_UNKD) {
		kill();
		res = TRUE;
	} else {
		if (isState(STATE_NORMAL) || isState(STATE_HOLDING)
		    || isState(STATE_LIVING)) {
			pick(actor);
			if (TMapObjGeneral::receiveMessage(actor, msg) != 0) {
				res = TRUE;
			} else {
				if (msg == HIT_MESSAGE_TAKE
				    && checkMapObjFlag(MAP_OBJ_FLAG_UNK100000)) {
					hold(static_cast<TTakeActor*>(actor));
					res = TRUE;
				} else if (actor->isActorType(0x80000001)
				           && !isActorType(0x400000d0)
				           && msg != HIT_MESSAGE_TAKE) {
					kicked();
					res = TRUE;
				} else {
					res = FALSE;
				}
			}

			if (msg == HIT_MESSAGE_PUT && isState(STATE_NORMAL)) {
				mState = STATE_LIVING;
			}
		} else {
			res = FALSE;
		}
	}

	return res;
}

void TResetFruit::initMapObj()
{
	TMapObjBall::initMapObj();

	J3DModel* model = TLiveActor::getModel();
	SMS_InitPacket_OneTevColor(model, 0, GX_TEVREG0, &mFruitColor);
}

TResetFruit::TResetFruit(const char* name)
    : TMapObjBall(name)
{
	unk198        = 0.0f;
	unk1A4        = 0;
	mFruitColor.r = 0xff;
	mFruitColor.g = 0xff;
	mFruitColor.b = 0xff;
	mFruitColor.a = 0xff;
}

void TRandomFruit::initMapObj()
{
	s32 fruitNum = MsRandF() * 5.0f;
	switch (fruitNum) {
	case 0: {
		snprintf(mFruitName, sizeof(mFruitName), "FruitCoconut");
		break;
	}
	case 1: {
		snprintf(mFruitName, sizeof(mFruitName), "FruitDurian");
		break;
	}
	case 2: {
		snprintf(mFruitName, sizeof(mFruitName), "FruitPapaya");
		break;
	}
	case 3: {
		snprintf(mFruitName, sizeof(mFruitName), "FruitPine");
		break;
	}
	case 4:
	case 5:
	default: {
		snprintf(mFruitName, sizeof(mFruitName), "FruitPine");
		break;
	}
	}

	unkF4 = mFruitName;
	TMapObjBall::initMapObj();
	SMS_InitPacket_OneTevColor(getModel(), 0, GX_TEVREG0, &mFruitColor);
}

TRandomFruit::TRandomFruit(const char* name)
    : TResetFruit(name)
{
	memset(mFruitName, 0, sizeof(mFruitName));
}

void TCoverFruit::calcRootMatrix()
{
	if (mHolder != nullptr) {
		MtxPtr takingMtx = mHolder->getTakingMtx();
		getModel()->setBaseTRMtx(takingMtx);
		mPosition.set(takingMtx[0][3], takingMtx[1][3], takingMtx[2][3]);
	} else {
		MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x,
		               mPosition.y - mYOffset, mPosition.z, mRotation.x,
		               mRotation.y, mRotation.z);
	}

	getModel()->setBaseScale(mScaling);
}

BOOL TCoverFruit::receiveMessage(THitActor* actor, u32 msg)
{
	if (actor->isActorType(0x8000083) && msg == HIT_MESSAGE_TAKE) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mHolder = static_cast<TTakeActor*>(actor);
		return TRUE;
	}

	if (msg == HIT_MESSAGE_UNKB) {
		kill();
		TFlagManager::smInstance->setBool(true, 0x1038b);
		return TRUE;
	}

	return FALSE;
}

void TCoverFruit::loadAfter()
{
	TMapObjBase::loadAfter();
	if (TFlagManager::smInstance->getBool(0x1038b)) {
		makeObjDead();
	}
}

void TBigWatermelon::touchWaterSurface()
{
	TMapObjBase::emitColumnWater();
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_DRINA_TO_WATER, &mPosition, 0,
	                                nullptr, 0, 4);
	kill();
}

void TBigWatermelon::touchWall(JGeometry::TVec3<f32>* wall,
                               TBGWallCheckRecord* record)
{
	TMapObjBall::touchWall(wall, record);
}

void TBigWatermelon::rebound(JGeometry::TVec3<f32>* surface)
{
	if (isState(STATE_LANDED)) {
		kill();
		*surface = mPosition;
	} else {
		TMapObjBase::calcReflectingVelocity(
		    mGroundPlane, mMapObjData->mPhysical->unk4->unk4, &mVelocity);
		surface->y = mGroundHeight;
		onLiveFlag(LIVE_FLAG_AIRBORNE);
		if (isActorType(0x400000d0)) {
			f32 volume;
			if (mScaling.y >= 5.0f) {
				volume = abs(mGroundPlane->getNormal().y);
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_WATERMELON_BBUND, &mPosition, nullptr, volume, 0,
				    0, nullptr, 0, 4);
			} else {
				volume = abs(mGroundPlane->getNormal().y);
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_WATERMELON_SBUND, &mPosition, nullptr, volume, 0,
				    0, nullptr, 0, 4);
			}
		} else {
			u32 soundID = mMapObjData->mSound->unk4->unk0[4];
			SMSGetMSound()->startSoundActorWithInfo(
			    soundID, &mPosition, &mVelocity, 0.0f, 0, 0, nullptr, 0, 4);
		}
		if (isState(STATE_LAUNCHED)) {
			mState = STATE_LANDED;
		}
	}
}

void TBigWatermelon::touchGround(JGeometry::TVec3<f32>* ground)
{
	TMapObjBall::touchGround(ground);
}

void TBigWatermelon::touchActor(THitActor* actor)
{
	if (!isState(STATE_APPEARING)) {
		if (isState(STATE_NORMAL) || getVelocity().y < 0.0f) {
			kill();
		} else {
			if (actor->isActorType(0x80000001)) {
				if (mPosition.distance(actor->mPosition) < mBodyRadius * 0.6f) {
					kill();
					return;
				}
			}

			if (actor->isActorType(0x10000015)) {
				if (static_cast<TPoiHana*>(actor)->isMoving()) {
					if (!(abs(mVelocity.y)
					      < mMapObjData->mPhysical->unk4->unkC)) {
						return;
					}

					mVelocity.y += 30.0f;
					mState = STATE_LAUNCHED;
					return;
				}
			}

			if (unk194 == 0 && !isState(STATE_HOLDING)) {
				if (!TMapObjBase::isHideObj(actor)
				    && !actor->isActorType(0x8000083)
				    && !actor->isActorType(0x400000ca)
				    && !actor->isActorType(0x400000cc)) {
					if (actor->isActorType(0x80000001)
					    && !isActorType(0x400000d0) && *gpMarioSpeedY != 0.0f) {
						kicked();
					} else {
						TMapObjBall::boundByActor(actor);
					}
				}
			}
		}
	}
}

void TBigWatermelon::kill()
{
	TMapObjBase::emitAndScale(MAPOBJ_WATERMELON_BOMB, 0, &mPosition);
	TMapObjBase::emitAndScale(MAPOBJ_WATERMELON_BOMB_A, 0, &mPosition);
	TMapObjBase::emitAndScale(MAPOBJ_WATERMELON_BOMB_B, 0, &mPosition);
	JGeometry::TVec3<f32> vec(1.0f, 1.0f, 1.0f);
	TMapObjBase::emitAndScale(MAPOBJ_WATERMELON_SHRINK_A, 0, &mPosition, vec);
	TMapObjBase::emitAndScale(MAPOBJ_WATERMELON_SHRINK_B, 0, &mPosition, vec);
	TWaterEmitInfo* emitInfo = unk198;
	emitInfo->mPos.value     = mPosition;
	gpModelWaterManager->emitRequest(*unk198);
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_WATERMELON_BLOCK, &mPosition, 0,
	                                nullptr, 0, 4);

	if (unk19C < 10) {
		TMapObjBase* mapObj = gpItemManager->makeObjAppear(
		    mPosition.x, mPosition.y, mPosition.z, 0x2000000e, true);

		if (mapObj != nullptr) {
			mapObj->mVelocity.set(0.0f, 25.0f, 0.0f);
			mapObj->offLiveFlag(LIVE_FLAG_UNK10);
			++unk19C;
		}
	}
	TMapObjGeneral::kill();
}

void TBigWatermelon::appearing()
{
	TMapObjGeneral::appearing();
	MtxPtr nodeMats = getModel()->getAnmMtx(0);
	calcRootMatrix();
	J3DModel* model = getModel();
	model->calc();
	nodeMats[1][3]
	    = mBodyRadius * (mScaling.y / mInitialScaling.y) + mPosition.y;
	mScaledBodyRadius = mScaling.x * 50.0f;
	mDamageRadius     = mScaling.x * 50.0f;
	calcEntryRadius();
	if (isState(STATE_NORMAL)) {
		mActorType    = 0x400000d0;
		mAttackRadius = mScaling.x * 50.0f;
		calcEntryRadius();
	} else {
		mActorType    = 0x400000db;
		mAttackRadius = 0.0f;
		calcEntryRadius();
	}
}

void TBigWatermelon::control()
{
	TMapObjBall::control();

	switch (mState) {
	case STATE_NORMAL: {
		if (checkLiveFlag(LIVE_FLAG_UNK10)) {
			offLiveFlag(LIVE_FLAG_UNK10);
		}
		const TLiveActor* groundPlaneActor = mGroundPlane->getActor();
		if (mPosition.y < mGroundHeight + 200.0f
		    && groundPlaneActor != nullptr) {
			if (mGroundPlane->getActor()->isActorType(0x400000cd)
			    || mGroundPlane->getActor()->isActorType(0x400000cd)) {
				f32 prevUnk1A0 = unk1A0;
				unk1A0 = SMS_GetSandRiseUpRatio(mGroundPlane->getActor());
				if (unk1A0 > 0.05f && unk1A0 > prevUnk1A0) {
					mVelocity.y += 20.0f;
				}
			}
		}
	} break;
	case STATE_APPEARING:
	case STATE_WAITING_TO_APPEAR:
	case STATE_LAUNCHED:
	case STATE_LANDED:
		break;
	case STATE_GOAL: {
		if (!isStateTimerEngaged()) {
			JGeometry::TVec3<f32> vec(1.0f, 1.0f, 1.0f);
			TMapObjBase::emitAndScale(MAPOBJ_WATERMELON_SHRINK_A, 0, &mPosition,
			                          vec);
			TMapObjBase::emitAndScale(MAPOBJ_WATERMELON_SHRINK_B, 0, &mPosition,
			                          vec);
			startStateTimer(30);
		}
		if (TMapObjBase::animIsFinished()) {
			makeObjDead();
		}
		break;
	}
	}
}

void TBigWatermelon::startEvent()
{
	if (strcmp(getName(), "スイカ（大）") == 0) {
		mPosition.set(-4660.0f, 1300.0f, 13600.0f);
		offMapObjFlag(MAP_OBJ_FLAG_UNK100);
		onLiveFlag(LIVE_FLAG_UNK10);
		mVelocity.zero();
		onLiveFlag(LIVE_FLAG_UNK10);
		TMapObjBase::startAnim(7);
		// getter?
		TMarDirector* director = gpMarDirector;
		director->fireStartDemoCamera("スイカゴールカメラ", &mPosition, -1,
		                              0.0f, true, nullptr, 0, nullptr, 0);
		gpItemManager->makeShineAppearWithDemoOffset(
		    "シャイン（お化けスイカ用）", "スイカシャインカメラ", 0.0f, 0.0f,
		    0.0f);
		startStateTimer(380);
		mState = 13;
	} else {
		for (s32 i = 0; i < 10; ++i) {
			TItem* item = static_cast<TItem*>(gpItemManager->makeObjAppear(
			    gpMarioPos->x, gpMarioPos->y, gpMarioPos->z, 0x2000000e, true));
			if (item != nullptr) {
				f32 randZ         = 20.0f * (MsRandF() - 0.5f);
				f32 randY         = 20.0f * MsRandF() + 20.0f;
				item->mVelocity.x = 20.0f * (MsRandF() - 0.5f);
				item->mVelocity.y = randY;
				item->mVelocity.z = randZ;
				item->offLiveFlag(LIVE_FLAG_UNK10);
				item->unk14C = 960;
			}
		}
		makeObjDead();
	}
}

void TBigWatermelon::checkWallCollision(JGeometry::TVec3<f32>* wall)
{
	TMapObjGeneral::checkWallCollision(wall);
}

BOOL TBigWatermelon::receiveMessage(THitActor* actor, u32 msg)
{
	if (actor->isActorType(0x80000001)) {
		boundByActor(actor);
		return TRUE;
	}

	return TMapObjBall::receiveMessage(actor, msg);
}

void TBigWatermelon::loadAfter()
{
	TMapObjGeneral::loadAfter();
	TLiveActor* shine = static_cast<TLiveActor*>(
	    JDrama::TNameRefGen::search("シャイン（お化けスイカ用）"));
	shine->mPosition.set(-4659.0f, 460.0f, 13620.0f);
}

void TBigWatermelon::initMapObj()
{
	TMapObjBall::initMapObj();
	SMS_LoadParticle("/scene/mapObj/watermelon_bomb.jpa",
	                 MAPOBJ_WATERMELON_BOMB);
	SMS_LoadParticle("/scene/mapObj/watermelon_bomb_a.jpa",
	                 MAPOBJ_WATERMELON_BOMB_A);
	SMS_LoadParticle("/scene/mapObj/watermelon_bomb_b.jpa",
	                 MAPOBJ_WATERMELON_BOMB_B);
	SMS_LoadParticle("/scene/mapObj/watermelon_shrink_a.jpa",
	                 MAPOBJ_WATERMELON_SHRINK_A);
	SMS_LoadParticle("/scene/mapObj/watermelon_shrink_b.jpa",
	                 MAPOBJ_WATERMELON_SHRINK_B);

	TWaterEmitInfo* emitInfo = new TWaterEmitInfo("/watermelon.prm");
	unk198                   = emitInfo;
}

TBigWatermelon::TBigWatermelon(const char* name)
    : TMapObjBall(name)
{
	unk198 = nullptr;
	unk19C = 0;
	unk1A0 = 0;
}
