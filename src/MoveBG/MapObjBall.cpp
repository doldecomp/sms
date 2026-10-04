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

inline f32 squaredVec(JGeometry::TVec3<f32> vec)
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

inline bool objIsNotMoving(JGeometry::TVec3<f32>& vel)
{
	return squaredVec(vel) <= 1 / 262144.0f;
}

void TMapObjBall::touchRoof(JGeometry::TVec3<f32>* param1)
{
	if (param1->y > unk140) {
		param1->y = unk140;
	}

	calcReflectingVelocity(unk13C, mMapObjData->mPhysical->unk4->unk4,
	                       &mVelocity);
}

inline f32 vecLength(JGeometry::TVec3<f32> vec) { return vec.length(); }

void TMapObjBall::touchWall(JGeometry::TVec3<f32>* param1,
                            TBGWallCheckRecord* param2)
{
	if (!checkLiveFlag2(LIVE_FLAG_AIRBORNE) && !isActorType(0x400000d0)) {
		f32 velMag = vecLength(mVelocity);
		mVelocity.y += unk184 * velMag;
	}

	for (s32 iVar1 = 0, iVar3 = 0; iVar1 < param2->mResultWallsNum;
	     ++iVar1, ++iVar3) {
		TBGCheckData* pVar1           = param2->mResultWalls[iVar3];
		JGeometry::TVec3<f32> velCopy = mVelocity;
		f32 fVar4                     = velCopy.dot(pVar1->mNormal);
		if (fVar4 < 0.0f) {
			f32 fVar5 = param1->dot(pVar1->mNormal) + pVar1->mPlaneDistance;
			param1->x += (mBodyRadius - fVar5) * pVar1->mNormal.x;
			param1->z += (mBodyRadius - fVar5) * pVar1->mNormal.z;
			f32 fVar6 = fVar4 * -(-1.0f + mMapObjData->mPhysical->unk4->unk8);
			mVelocity.x += fVar6 * pVar1->mNormal.x;
			mVelocity.z += fVar6 * pVar1->mNormal.z;
			if (isActorType(0x400000d0)) {
				if (mScaling.y >= 5.0f) {
					f32 fVar6 = abs(vecLength(mVelocity));
					SMSGetMSound()->startSoundActorWithInfo(
					    MSD_SE_OBJ_WATERMELON_BROLL, &mPosition, nullptr, fVar6,
					    0, 0, nullptr, 0, 0x4);
				} else {
					f32 fVar6 = abs(vecLength(mVelocity));
					SMSGetMSound()->startSoundActorWithInfo(
					    MSD_SE_OBJ_WATERMELON_SROLL, &mPosition, nullptr, fVar6,
					    0, 0, nullptr, 0, 0x4);
				}
			} else {
				u32 uVar1 = mMapObjData->mSound->unk4->unk0[4];
				SMSGetMSound()->startSoundActorWithInfo(
				    uVar1, &mPosition, &mVelocity, 0.0f, 0, 0, nullptr, 0, 0x4);
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
			soundY = abs(mGroundPlane->mNormal.y);
			SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_WATERMELON_BBUND,
			                                        &mPosition, nullptr, soundY,
			                                        0, 0, nullptr, 0, 4);
		} else {
			soundY = abs(mGroundPlane->mNormal.y);
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
	f32 fVar1 = abs(vecLength(mVelocity));
	if (fVar1 > 0.05f && isActorType(0x400000d0)) {
		if (mScaling.y >= 5.0f) {
			SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_WATERMELON_BROLL,
			                                        &mPosition, nullptr, fVar1,
			                                        0, 0, nullptr, 0, 4);
		} else {
			SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_WATERMELON_SROLL,
			                                        &mPosition, nullptr, fVar1,
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
				mVelocity.x += unk180 * mGroundPlane->mNormal.x;
				mVelocity.z += unk180 * mGroundPlane->mNormal.z;
			}

			mVelocity.x *= mMapObjData->mPhysical->unk4->unk10;
			mVelocity.z *= mMapObjData->mPhysical->unk4->unk10;

			f32 unkC = mMapObjData->mPhysical->unk4->unkC;
			if (isActorType(0x400000d0)
			    && (abs(mVelocity.x) > mMapObjData->mPhysical->unk4->unkC
			        || abs(mVelocity.z) > mMapObjData->mPhysical->unk4->unkC)) {
				SMSGetMSound()->startSoundActor(MSD_SE_MA_SLIP, &mPosition,
				                                nullptr, nullptr, 0, 4);
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
	if (vecLength(mVelocity) > 10.0f) {
		return;
	}

	TMapObjGeneral::hold(holder);
	mVelocity.zero();
}

// This inline probably lives on a class higher up the inheritance chain
inline bool isFalling(JGeometry::TVec3<f32>& vel)
{
	JGeometry::TVec3<f32> velCopy(vel);
	return velCopy.y < 0.0f;
}

void TMapObjBall::kicked()
{
	if (!isFalling(mVelocity)) {
		return;
	}

	kick();
	onLiveFlag(LIVE_FLAG_AIRBORNE);

	THitActor* marHitActor = SMS_GetMarioHitActor();
	marHitActor->receiveMessage(this, HIT_MESSAGE_ATTACK);
	if (!isActorType(0x400000d0)) {
		SMSGetMSound()->startSoundActor(MSD_SE_MA_KICK_DRIAN, &mPosition,
		                                nullptr, nullptr, 0, 4);
	}
}

u32 TMapObjBall::touchWater(THitActor* water)
{
	if (!isState(STATE_HOLDING)) {
		if (isState(STATE_APPEARING)) {
			return 1;
		}

		JGeometry::TVec3<f32> velCopy = mVelocity;
		const JGeometry::TVec3<f32>& waterSpeed
		    = TMapObjBase::getWaterSpeed(water);

		velCopy.x = waterSpeed.x * unk17C + velCopy.x;
		velCopy.y = waterSpeed.y * unk17C + velCopy.y;
		velCopy.z = waterSpeed.z * unk17C + velCopy.z;
		mVelocity = velCopy;
		offLiveFlag(LIVE_FLAG_UNK10);
	}

	return 1;
}

void TMapObjBall::boundByActor(THitActor* actor)
{
	JGeometry::TVec3<f32> offsetToActor;
	offsetToActor.x = actor->mPosition.x - mPosition.x;
	offsetToActor.z = actor->mPosition.z - mPosition.z;
	offsetToActor.y = 0.0f;

	f32 fVar1 = isActorType(0x400000d0) ? mAttackRadius + actor->mDamageRadius
	                                    : mDamageRadius;

	if (!(offsetToActor.x * offsetToActor.x + offsetToActor.z * offsetToActor.z
	      < fVar1 * fVar1)) {
		if (offsetToActor.x != 0.0f && offsetToActor.z != 0.0f) {
			MsVECNormalize(offsetToActor, offsetToActor);
		}
		if (actor->isActorType(0x80000001)) {
			if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK2000000)) {
				f32 unkC = mMapObjData->mPhysical->unk4->unkC;
				if (abs(*gpMarioSpeedX) > unkC || abs(*gpMarioSpeedZ) > unkC) {
					mVelocity.y += unk150;
					if (!isActorType(0x400000d0)) {
						SMSGetMSound()->startSoundActor(MSD_SE_MA_KICK_DRIAN,
						                                &mPosition, nullptr,
						                                nullptr, 0, 4);
					}
				} else {
					mVelocity.y += unk154;
				}

				mVelocity.x
				    += unk148 * *gpMarioSpeedX - offsetToActor.x * unk14C;
				mVelocity.z
				    += unk148 * *gpMarioSpeedZ - offsetToActor.z * unk14C;
				actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
			}
		} else {
			f32 fVar2 = mVelocity.z * offsetToActor.z
			            + mVelocity.x * offsetToActor.x
			            + mVelocity.y * offsetToActor.y;
			if ((fVar2 >= 0.0f)
			    && (abs(mVelocity.x) > mMapObjData->mPhysical->unk4->unkC)
			    && (abs(mVelocity.z) > mMapObjData->mPhysical->unk4->unkC)) {
				mVelocity.x -= (unk16C + 1.0f) * offsetToActor.x * fVar2;
				mVelocity.y += unk168;
				mVelocity.z -= (unk16C + 1.0f) * offsetToActor.z * fVar2;
				actor->receiveMessage(this, HIT_MESSAGE_UNK10);
				if (!isActorType(0x400000d0)) {
					SMSGetMSound()->startSoundActor(MSD_SE_IT_DRIAN_BOUND,
					                                &mPosition, nullptr,
					                                nullptr, 0, 4);
				}
			} else {
				mVelocity.x -= (offsetToActor.x * unk164);
				mVelocity.y += unk168;
				mVelocity.z -= (offsetToActor.z * unk164);
			}
		}
		if (actor->isActorType(0x80000001)
		    && !checkMapObjFlag(MAP_OBJ_FLAG_UNK2000000) && mVelocity.y < 0.0f
		    && gpMarioPos->y + 130.0f < mPosition.y + mBodyRadius) {
			JGeometry::TVec3<f32> vec;
			mVelocity.y = (-unk160) * mVelocity.y;
			mVelocity.x += unk158 * *gpMarioSpeedX;
			mVelocity.y += unk15C * *gpMarioSpeedY;
			mVelocity.z += unk158 * *gpMarioSpeedZ;

			if (!isActorType(0x400000d0)
			    && gpMSound->gateCheck(MSD_SE_MA_KICK_DRIAN)) {
				MSoundSESystem::MSoundSE::startSoundActor(
				    MSD_SE_MA_KICK_DRIAN, mPosition, nullptr, nullptr, 0, 4);
			}
		}
		unk194 = 10;
		offLiveFlag(LIVE_FLAG_UNK10);
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}
}

inline void gekkoCopy(void* dst, void* src)
{
	JGeometry::gekko_ps_copy12(dst, src);
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

	Mtx local48;
	local48[2][3]             = 0.0f;
	local48[1][3]             = 0.0f;
	local48[0][3]             = 0.0f;
	local48[1][2]             = 0.0f;
	local48[0][2]             = 0.0f;
	local48[2][1]             = 0.0f;
	local48[0][1]             = 0.0f;
	local48[2][0]             = 0.0f;
	local48[1][0]             = 0.0f;
	local48[2][2]             = 1.0f;
	local48[1][1]             = 1.0f;
	local48[0][0]             = 1.0f;
	JGeometry::TVec3<f32> vec = mVelocity;
	if (abs(mVelocity.x) < mMapObjData->mPhysical->unk4->unkC) {
		JGeometry::TVec3<f32> vec2 = mVelocity;
		if (abs(mVelocity.z) < mMapObjData->mPhysical->unk4->unkC
		    && mGroundPlane->mNormal.y == 1.0f) {
			mVelocity.x = 0.0f;
			mVelocity.z = 0.0f;
		}
	}

	JGeometry::TVec3<f32> vec3 = mVelocity;
	if (!(abs(mVelocity.x) > mMapObjData->mPhysical->unk4->unkC)) {
		JGeometry::TVec3<f32> vec4 = mVelocity;
		if (!(abs(mVelocity.z) > mMapObjData->mPhysical->unk4->unkC)) {
			goto todo;
		}
	}

	JGeometry::TVec3<f32> vec5 = mVelocity;
	JGeometry::TVec3<f32> vec6 = vec5;
	TMapObjBase::getVerticalVecToTargetXZ(mPosition.x + mVelocity.x,
	                                      mPosition.z + mVelocity.z, &vec);
	JGeometry::TVec3<f32> vec7 = mVelocity;
	JGeometry::TVec3<f32> vec8 = vec7;
	f32 fVar1                  = mVelocity.x;
	f32 fVar2                  = mVelocity.z;
	fVar1                      = fVar1 * fVar1 + fVar2 * fVar2;
	if (fVar1 < 0.0f) {
		fVar2 = JGeometry::TUtil<f32>::inv_sqrt(fVar1);
		fVar1 = fVar1 * (fVar2 / 2.0f) * -(fVar1 * fVar2 * fVar2 - 3.0f);
	}
	f32 dVar8 = 1.0f;
	f32 dVar7 = (fVar1 / mBodyRadius) * 2.0f;
	f32 dVar3 = vec.dot(vec);
	JGeometry::TVec3<f32> local124;
	if (dVar3 <= 1 / 262144.0f) {
		local124.z = 0.0f;
		local124.y = 0.0f;
		local124.x = 0.0f;
	} else {
		dVar3 = JGeometry::TUtil<f32>::inv_sqrt(dVar3);
		local124.scale(dVar8 * dVar3, vec);
	}

	f32 fVar9     = sinf(dVar7);
	dVar3         = fVar9;
	fVar9         = cosf(dVar7);
	dVar7         = fVar9;
	f32 dVar6     = local124.x;
	f32 dVar5     = 1.0f - dVar7;
	dVar8         = local124.y;
	f32 dVar4     = local124.z;
	local48[0][0] = dVar5 * (dVar6 * dVar6) + dVar7;
	fVar9         = dVar8 * (dVar5 * dVar6);
	fVar1         = dVar4 * (dVar5 * dVar6);
	local48[0][1] = fVar9 - (dVar3 * dVar4);
	local48[0][2] = fVar1 + (dVar3 * dVar8);
	local48[1][0] = fVar9 + (dVar3 * dVar4);
	fVar9         = dVar4 * (dVar5 * dVar8);
	local48[1][1] = dVar5 * (dVar8 * dVar8) + dVar7;
	local48[1][2] = fVar9 - (dVar3 * dVar6);
	local48[2][0] = fVar1 - (dVar3 * dVar8);
	local48[2][1] = fVar9 + (dVar3 * dVar6);
	local48[2][2] = dVar5 * (dVar4 * dVar4) + dVar7;
todo:
	Mtx afStack94;
	gekkoCopy(afStack94, *(getModel()->getAnmMtx(0)));
	JGeometry::TVec3<f32> vec9(0.0f);
	PSMTXConcat(local48, afStack94, local48);
	local48[0][3] = mPosition.x;
	local48[1][3] = mPosition.y + mBodyRadius;
	local48[2][3] = mPosition.z;
	if (isActorType(0x40000394) && local48[1][1] > 0.0f) {
		local48[1][3] = -(local48[1][1] * 50.0f - local48[1][3]);
	}
	if (isActorType(0x40000392)) {
		local48[1][3] = -((1.0f - local48[1][1]) * 10.0f - local48[1][3]);
	}

	J3DModel* model = getModel();
	PSMTXCopy(local48, model->getAnmMtx(0));
}

void TMapObjBall::checkWallCollision(JGeometry::TVec3<f32>* wall)
{
	TBGWallCheckRecord wallRecord;
	wallRecord.mCenter.x   = wall->x;
	wallRecord.mCenter.y   = mBodyRadius + wall->y;
	wallRecord.mCenter.z   = wall->z;
	wallRecord.mRadius     = mBodyRadius;
	wallRecord.mFlags      = mMapObjData->mPhysical->mWallCheckFlags;
	wallRecord.mMaxResults = 4;
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
	nodeMatrix[1][3]  = mPosition.z;
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
		TTakeActor* holder = mHolder;
		MtxPtr takingMtx   = holder->getTakingMtx();
		Mtx takingMtxCopy;
		PSMTXCopy(takingMtx, takingMtxCopy);
		takingMtxCopy[2][2] += unk190;
		J3DModel* model = TLiveActor::getModel();
		PSMTXCopy(takingMtxCopy, model->getAnmMtx(0));
	} else {
		if (!objIsNotMoving(mVelocity) || mGroundPlane->mActor != nullptr) {
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

inline void TMapObjBall::kick()
{
	if (mVelocity.y == 0.0f) {
		mVelocity.y = unk178;
	} else {
		mVelocity.y = unk174 * *gpMarioSpeedY - unk160 * mVelocity.y;
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
}

void TResetFruit::checkGroundCollision(JGeometry::TVec3<f32>* ground)
{
	if (gpMarDirector->mMap != 7 && gpMarDirector->mMap != 4) {
		TMapObjGeneral::checkGroundCollision(ground);
	} else if (gpMarDirector->mMap == 4) {
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
	if (gpMarDirector->mMap == 3 && unk1A4 != 0) {
		makeObjDead();
	}

	if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK4000000) && !isStateTimerEngaged()
	    && mColCount == 0) {
		onMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
		makeObjAppeared();
		Mtx scaleMat;
		PSMTXScale(scaleMat, 0.2f, 0.2f, 0.2f);
		MtxPtr nodeMats = getModel()->getAnmMtx(0);
		TMapObjBase::concatOnlyRotFromLeft(scaleMat, getModel()->getAnmMtx(0),
		                                   nodeMats);

		mScaling.y = 0.2f;
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mState = STATE_APPEARING;
		SMSGetMSound()->startSoundActor(MSD_SE_IT_COMMON_APPEAR, &mPosition,
		                                nullptr, nullptr, 0, 4);
	}
}

void TResetFruit::makeObjWaitingToAppear()
{
	mState = STATE_LIVING;
	makeObjDefault();
	makeObjDead();
	calcRootMatrix();
	J3DModel* model = TLiveActor::getModel();
	model->calc();
	mStateTimer = mFruitWaitTimeToAppear;
	offMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
	mState = STATE_WAITING_TO_APPEAR;
	if (gpMarDirector->mMap == 3) {
		if (unk1A4 != 0) {
			makeObjDead();
		}
	}
}

void TResetFruit::thrown()
{
	TMapObjGeneral::thrown();
	mState = STATE_LIVING;
}

void TResetFruit::hold(TTakeActor* actor)
{
	// Most likely a call to TMapObjBall::hold, but calling it doesn't match
	if (!(vecLength(mVelocity) > 10.0f)) {
		TMapObjGeneral::hold(actor);
		mVelocity.zero();
	}

	mVelocity.zero();
	onLiveFlag(LIVE_FLAG_UNK10);
	if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK4000000) && !isStateTimerEngaged()) {
		onMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
		mStateTimer = getLivingTime();
	}
}

void TResetFruit::touchKillSurface()
{
	mState = STATE_LIVING;
	makeObjDefault();
	makeObjDead();
	calcRootMatrix();
	getModel()->calc();
	mStateTimer = mFruitWaitTimeToAppear;
	offMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
	mState = STATE_WAITING_TO_APPEAR;
	if (gpMarDirector->getCurrentMap() == 3 && unk1A4 != 0) {
		makeObjDead();
	}
}

void TResetFruit::touchPollution()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_MOE_FIRE_OFF,
	                                            &mPosition, 0, nullptr);
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_AWAY_INTO_GRAF, &mPosition,
	                                nullptr, nullptr, 0, 4);

	makeObjDefault();
	touchKillSurface();
}

void TResetFruit::touchWaterSurface()
{
	TMapObjBase::emitColumnWater();
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_DRINA_TO_WATER, &mPosition,
	                                nullptr, nullptr, 0, 4);
	touchKillSurface();
}

inline u32 TResetFruit::touchWater(THitActor* water)
{
	if (!isState(STATE_HOLDING) && !isState(STATE_APPEARING)) {
		JGeometry::TVec3<f32> velCopy = mVelocity;
		const JGeometry::TVec3<f32>& waterSpeed
		    = TMapObjBase::getWaterSpeed(water);

		mVelocity.x = waterSpeed.x * unk17C + velCopy.x;
		mVelocity.y = waterSpeed.y * unk17C + velCopy.y;
		mVelocity.z = waterSpeed.z * unk17C + velCopy.z;
		offLiveFlag(LIVE_FLAG_UNK10);
	}

	makeObjLiving();

	return 1;
}

void TResetFruit::hideTouchActor(THitActor* actor)
{
	TMapObjBall::touchActor(actor);
}

void TResetFruit::touchActor(THitActor* actor)
{
	if (!isState(STATE_APPEARING) && !isState(STATE_BREAKING)
	    && !isState(STATE_ROTTING) && !isState(STATE_WAITING_TO_APPEAR)) {
		hideTouchActor(actor);
		if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK4000000) && isState(STATE_NORMAL)
		    && !checkLiveFlag(LIVE_FLAG_UNK10)) {
			makeObjLiving();
		}
	}
}

void TResetFruit::touchGround(JGeometry::TVec3<f32>* ground)
{
	if (getGroundPlane()->isDeathPlane()) {
		mState = STATE_LIVING;
		makeObjDefault();
		makeObjDead();
		calcRootMatrix();
		J3DModel* model = TLiveActor::getModel();
		model->calc();
		mStateTimer = mFruitWaitTimeToAppear;
		offMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
		mState = STATE_WAITING_TO_APPEAR;
		if (gpMarDirector->getCurrentMap() == 3) {
			if (unk1A4 != 0) {
				makeObjDead();
			}
		}

		ground->set(getPosition());
	} else {
		TMapObjBall::touchGround(ground);
	}
}

void TResetFruit::makeObjLiving()
{
	if (!isStateTimerEngaged()) {
		onMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
		mStateTimer = getLivingTime();
	}

	offLiveFlag(LIVE_FLAG_UNK10);
	mState = STATE_LIVING;
}

void TResetFruit::kicked()
{
	if (!checkMapObjFlag(MAP_OBJ_FLAG_UNK2000000) && !isState(STATE_HOLDING)) {
		if (*gpMarioSpeedY < 0.0f) {
			return;
		} else {
			if (mVelocity.y <= 0.0f && checkLiveFlag2(LIVE_FLAG_AIRBORNE)
			    && mVelocity.x * gpMarioPos->x - mPosition.x
			               + mVelocity.z * gpMarioPos->z - mPosition.z
			               + mVelocity.y * 0.0f
			           > 0.0f) {
				kick();
				SMS_GetMarioHitActor()->receiveMessage(this,
				                                       HIT_MESSAGE_ATTACK);
				SMSGetMSound()->startSoundActor(
				    MSD_SE_MA_KICK_DRIAN, &mPosition, nullptr, nullptr, 0, 4);
			}
		}
	}
}

void TResetFruit::breaking()
{
	Mtx scaleMtx;
	PSMTXScale(scaleMtx, 1.0f, mBreakingScaleSpeed, 1.0f);
	J3DModel* model = getModel();
	MtxPtr nodeMat  = model->getAnmMtx(0);
	TMapObjBase::concatOnlyRotFromLeft(scaleMtx, nodeMat, nodeMat);
	mScaling.y *= mBreakingScaleSpeed;
	nodeMat[1][3] = mBodyRadius * mScaling.y + mPosition.y;
	if (mScaling.y < 0.2f) {
		mPosition.y += mBodyRadius / 2.0f;
		mScaling.set(mInitialScaling);
		emitAndScale(PARTICLE_MS_ENM_DISAP_A_W, 0, &mPosition);

		SMSGetMSound()->startSoundActor(MSD_SE_SMOKE_EFFECT, &mPosition,
		                                nullptr, nullptr, 0, 4);
		mStateTimer = 240;
		sleep();
		mState = STATE_WAIT_EFFECT;
	}
}

void TResetFruit::appearing()
{
	MtxPtr scaleMatCopy;
	Mtx scaleMat;
	PSMTXScale(scaleMat, mScaleUpSpeed, mScaleUpSpeed, mScaleUpSpeed);
	J3DModel* model2 = TLiveActor::getModel();
	scaleMatCopy     = model2->getAnmMtx(0);
	TMapObjBase::concatOnlyRotFromLeft(scaleMat, scaleMatCopy, scaleMatCopy);
	mScaling.y         = mScaling.y * mScaleUpSpeed;
	mScaledBodyRadius  = mBodyRadius * mScaling.y;
	scaleMatCopy[1][3] = mBodyRadius * mScaling.y + mPosition.y;
	if (mScaling.y >= mInitialScaling.y) {
		mScaling.set(mInitialScaling);
		J3DModel* model = TLiveActor::getModel();
		model->calc();
		offHitFlag(HIT_FLAG_NO_COLLISION);
		makeObjAppeared();
		mState = STATE_NORMAL;
	}
}

void TResetFruit::unknownInline()
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

void TResetFruit::control()
{
	switch (mState) {
	case STATE_NORMAL: {
		offHitFlag(HIT_FLAG_NO_COLLISION);
		for (s32 iVar6 = 0, iVar5 = 0; iVar6 < mColCount; ++iVar6, ++iVar5) {
			THitActor* hitActor = mCollisions[iVar5];
			TResetFruit::touchActor(hitActor);
		}

		if (mGroundPlane->mActor != nullptr) {
			calcCurrentMtx();
		}
		break;
	}
	case STATE_LIVING: {
		offHitFlag(HIT_FLAG_NO_COLLISION);
		if (gpMarDirector->mMap == 4 && checkLiveFlag(LIVE_FLAG_UNK10)) {
			offLiveFlag(LIVE_FLAG_UNK10);
		}

		if (mGroundPlane->mActor != nullptr) {
			if (checkLiveFlag(LIVE_FLAG_UNK10)) {
				offLiveFlag(LIVE_FLAG_UNK10);
			}

			const TLiveActor* actor = mGroundPlane->mActor;
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

		unknownInline();
		break;
	}
	case STATE_HOLDING: {
		unknownInline();
		break;
	}
	case STATE_APPEARING:
	case STATE_BREAKING: {
		TMapObjGeneral::control();
		if (unk194 != 0) {
			unk194 -= 1;
		}

		if (isState(STATE_HOLDING)) {
			Mtx takingMtx;
			PSMTXCopy(mHolder->getTakingMtx(), takingMtx);
			takingMtx[2][2] += unk190;
			PSMTXCopy(takingMtx, getModel()->getAnmMtx(0));
		} else {
			if (!objIsNotMoving(mVelocity) || mGroundPlane->mActor != nullptr) {
				calcCurrentMtx();
			}
		}
		break;
	}
	case STATE_ROTTING: {
		mPosition.y += mBodyRadius / 2.0f;
		mScaling.set(mInitialScaling);
		emitAndScale(PARTICLE_MS_ENM_DISAP_A_W, 0, &mPosition);
		SMSGetMSound()->startSoundActor(MSD_SE_SMOKE_EFFECT, &mPosition,
		                                nullptr, nullptr, 0, 4);

		mStateTimer = 240;
		sleep();
		mState = STATE_WAIT_EFFECT;
		break;
	}
	case STATE_WAIT_EFFECT: {
		if (!isStateTimerEngaged()) {
			mFruitColor.r = 0xff;
			mFruitColor.g = 0xff;
			mFruitColor.b = 0xff;
			awake();
			mState = STATE_LIVING;
			makeObjDefault();
			makeObjDead();
			calcRootMatrix();
			getModel()->calc();
			mStateTimer = mFruitWaitTimeToAppear;
			offMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
			mState = STATE_WAITING_TO_APPEAR;
			if (gpMarDirector->mMap == 3 && unk1A4 != 0) {
				makeObjDead();
			}
		}
		break;
	}
	}
}

void TResetFruit::perform(u32 param1, JDrama::TGraphics* graphics)
{
	if (gpMarDirector->mMap == 7) {
		if (isState(STATE_HOLDING) || !objIsNotMoving(mVelocity)) {
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

	TMapObjGeneral::perform(param1, graphics);
}

void TResetFruit::killByTimer(int timer)
{
	mStateTimer = timer;
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
			TResetFruit::touchActor(actor);
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
	memset((void*)&mFruitName, 0, sizeof(mFruitName));
}

void TCoverFruit::calcRootMatrix()
{
	if (mHolder != nullptr) {
		MtxPtr takingMtx = mHolder->getTakingMtx();
		getModel()->setBaseTRMtx(takingMtx);
		mPosition.set(takingMtx[0][3], takingMtx[1][3], takingMtx[2][3]);
	} else {
		MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x, mPosition.z,
		               mRotation.x, mRotation.y, mPosition.y - mYOffset,
		               mRotation.z);
	}

	getModel()->setBaseScale(mScaling);
}

BOOL TCoverFruit::receiveMessage(THitActor* actor, u32 msg)
{
	if (actor->isActorType(0x8000083) && msg == HIT_MESSAGE_TAKE) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mHolder = static_cast<TTakeActor*>(actor);
		return TRUE;
	} else if (msg == HIT_MESSAGE_UNKB) {
		kill();
		TFlagManager::smInstance->setBool(true, 0x1038b);
		return TRUE;
	} else {
		return FALSE;
	}
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
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_DRINA_TO_WATER, &mPosition,
	                                nullptr, nullptr, 0, 4);
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
		(*surface) = mPosition;
	} else {
		TMapObjBase::calcReflectingVelocity(
		    mGroundPlane, mMapObjData->mPhysical->unk4->unk4, &mVelocity);
		surface->y = mGroundHeight;
		onLiveFlag(LIVE_FLAG_AIRBORNE);
		if (isActorType(0x400000d0)) {
			f32 fVar1;
			if (mScaling.y >= 5.0f) {
				fVar1 = abs(mGroundPlane->mNormal.y);
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_WATERMELON_BBUND, &mPosition, nullptr, fVar1, 0,
				    0, nullptr, 0, 4);
			} else {
				fVar1 = abs(mGroundPlane->mNormal.y);
				SMSGetMSound()->startSoundActorWithInfo(
				    MSD_SE_OBJ_WATERMELON_SBUND, &mPosition, nullptr, fVar1, 0,
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
		if (isState(STATE_NORMAL) || isFalling(mVelocity)) {
			kill();
		} else {
			if (actor->isActorType(0x80000001)) {
				f32 fVar1 = mPosition.x - actor->mPosition.x;
				f32 fVar2 = mPosition.y - actor->mPosition.y;
				f32 fVar3 = mPosition.z - actor->mPosition.z;
				f32 fVar4 = fVar2 * fVar2;
				fVar4 += fVar3 * fVar3 + fVar1 * fVar1;
				if (fVar4 <= 0.0f) {
					fVar2 = __frsqrte(fVar4);
					fVar4 = fVar4 * fVar2 / 2.0f
					        * -(fVar4 * fVar2 * fVar2 - 3.0f);
				}
				if (fVar4 < mBodyRadius * 0.6f) {
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
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_WATERMELON_BLOCK, &mPosition,
	                                nullptr, nullptr, 0, 4);

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
	TMapObjGeneral::control();
	if (unk194 != 0) {
		unk194 -= 1;
	}

	if (isState(STATE_HOLDING)) {
		TTakeActor* holder = mHolder;
		MtxPtr takingMtx   = holder->getTakingMtx();
		Mtx takingMtxCopy;
		PSMTXCopy(takingMtx, takingMtxCopy);
		takingMtxCopy[2][2] += unk190;
		J3DModel* model = TLiveActor::getModel();
		PSMTXCopy(takingMtxCopy, model->getAnmMtx(0));
	} else {
		if (!objIsNotMoving(mVelocity) || mGroundPlane->mActor != nullptr) {
			calcCurrentMtx();
		}
	}

	switch (mState) {
	case STATE_NORMAL: {
		if (checkLiveFlag(LIVE_FLAG_UNK10)) {
			offLiveFlag(LIVE_FLAG_UNK10);
		}
		const TLiveActor* groundPlaneActor = mGroundPlane->mActor;
		if (mPosition.y < mGroundHeight + 200.0f
		    && groundPlaneActor != nullptr) {
			if (mGroundPlane->mActor->isActorType(0x400000cd)
			    || mGroundPlane->mActor->isActorType(0x400000cd)) {
				f32 prevUnk1A0 = unk1A0;
				unk1A0         = SMS_GetSandRiseUpRatio(mGroundPlane->mActor);
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
			mStateTimer = 30;
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
		mStateTimer = 380;
		mState      = 13;
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
		TMapObjBall::boundByActor(actor);
		return TRUE;
	}

	if (TMapObjGeneral::receiveMessage(actor, msg)) {
		return TRUE;
	} else {
		if (msg == HIT_MESSAGE_TAKE
		    && checkMapObjFlag(MAP_OBJ_FLAG_UNK100000)) {
			hold(static_cast<TTakeActor*>(actor));
			return TRUE;
		} else if (actor->isActorType(0x80000001) && !isActorType(0x400000d0)
		           && msg != HIT_MESSAGE_TAKE) {
			kicked();
			return TRUE;
		} else {
			return FALSE;
		}
	}
}

void TBigWatermelon::loadAfter()
{
	TMapObjGeneral::loadAfter();
	TLiveActor* foo = static_cast<TLiveActor*>(
	    JDrama::TNameRefGen::search("シャイン（お化けスイカ用）"));
	foo->mPosition.set(-4659.0f, 460.0f, 13620.0f);
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
