#include <MoveBG/MapObjGeneral.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <Strategic/Binder.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/Map.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

u32 TMapObjGeneral::mNormalLivingTime       = 960;
u32 TMapObjGeneral::mNormalFlushTime        = 360;
int TMapObjGeneral::mNormalFlushInterval    = 10;
u32 TMapObjGeneral::mNormalWaitToAppearTime = 360;
f32 TMapObjGeneral::mNormalAppearingScaleUp = 0.01f;
f32 TMapObjGeneral::mNormalThrowSpeedRate   = 0.5f;

bool TMapObjGeneral::isPollutedGround(const JGeometry::TVec3<f32>& v) const
{
	// WTF? Did they forget to refactor this?!
	const JGeometry::TVec3<f32>& p = mInitialPosition;
	if (gpPollution->isPolluted(v.x, p.y, p.z)
	    || gpPollution->isPolluted(v.x - 32.0f, v.y, v.z - 32.0f)
	    || gpPollution->isPolluted(v.x + 32.0f, v.y, v.z - 32.0f)
	    || gpPollution->isPolluted(v.x - 32.0f, v.y, v.z + 32.0f)
	    || gpPollution->isPolluted(v.x + 32.0f, v.y, v.z + 32.0f))
		return true;

	return false;
}

inline f32 distToMario(const JGeometry::TVec3<f32>& v)
{
	f32 l = (v.x - gpMarioPos->x) * (v.x - gpMarioPos->x)
	        + (v.y - gpMarioPos->y) * (v.y - gpMarioPos->y)
	        + (v.z - gpMarioPos->z) * (v.z - gpMarioPos->z);
	return JGeometry::TUtil<f32>::sqrt(l);
}

void TMapObjGeneral::waitingToAppear()
{
	if (isStateTimerEngaged())
		return;

	if (isActorType(0x4000005a)) {
		f32 damageRadius = getDamageRadius();
		if (SMS_GetMarioDamageRadius() + damageRadius + 100.0f
		    > distToMario(mInitialPosition))
			appear();
	} else {
		f32 damageRadius = getDamageRadius();
		if (SMS_GetMarioDamageRadius() + damageRadius
		    > distToMario(mInitialPosition))
			appear();
	}
}

void TMapObjGeneral::waitingToRecover()
{
	if (!isPollutedGround(mInitialPosition))
		recover();
}

void TMapObjGeneral::waitToAppear(s32 waitTime)
{
	if (waitTime == 0)
		mStateTimer = mNormalWaitToAppearTime;
	else
		mStateTimer = waitTime;
	mState = STATE_WAITING_TO_APPEAR;
}

void TMapObjGeneral::sink()
{
	mVelocity.x = mVelocity.y = mVelocity.z = 0.0f;
	onLiveFlag(LIVE_FLAG_UNK10);
	mState = STATE_SINKING;
	unk144 = mPosition.y;
	setUpMapCollision(1);
	startSound(6);
}

void TMapObjGeneral::put()
{
	mHolder                    = nullptr;
	mHolder                    = nullptr;
	s32 preservedTimeTilAppear = getStateTimer();
	makeObjAppeared();
	mStateTimer = preservedTimeTilAppear;
	mPosition.x = JMASSin(*gpMarioAngleY)
	                  * (getDamageRadius() + SMS_GetMarioDamageRadius() + 10.0f)
	              + SMS_GetMarioPos().x;
	mPosition.y = SMS_GetMarioPos().y;
	mPosition.z = JMASCos(*gpMarioAngleY)
	                  * (getDamageRadius() + SMS_GetMarioDamageRadius() + 10.0f)
	              + SMS_GetMarioPos().z;
	offLiveFlag(LIVE_FLAG_UNK10);
	mGroundHeight = gpMap->checkGround(mPosition, &mGroundPlane);
}

void TMapObjGeneral::thrown()
{
	mPosition.set(gpMarioPos->x, gpMarioPos->y, gpMarioPos->z);
	mRotation.set(*gpMarioAngleX, *gpMarioAngleY, *gpMarioAngleZ);

	mGroundHeight = gpMap->checkGround(mPosition, &mGroundPlane);
	unk138        = 0;
	mHolder       = nullptr;

	mVelocity.set(JMASSin((s32)*gpMarioAngleY) * *gpMarioThrowPower
	                      * mMapObjData->mPhysical->unk4->unk2C
	                  + (mNormalThrowSpeedRate * *gpMarioSpeedX),
	              mMapObjData->mPhysical->unk4->unk30,
	              JMASCos((s32)*gpMarioAngleY) * *gpMarioThrowPower
	                      * mMapObjData->mPhysical->unk4->unk2C
	                  + (mNormalThrowSpeedRate * *gpMarioSpeedZ));

	offLiveFlag(LIVE_FLAG_UNK10);
	JGeometry::TVec3<f32> vel = mVelocity;
	mPosition.add(vel);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	removeMapCollision();
	offLiveFlag(LIVE_FLAG_DEAD);
	startAnim(5);
	startSound(5);
	mState = STATE_NORMAL;
}

void TMapObjGeneral::touchingWater()
{
	if (animIsFinished() && hasModelOrAnimData(4))
		startAnim(0);
}

void TMapObjGeneral::touchingPlayer()
{
	if (animIsFinished() && hasModelOrAnimData(4))
		startAnim(0);
}

void TMapObjGeneral::holding()
{
	mPosition     = mHolder->mPosition;
	mGroundHeight = gpMap->checkGround(mPosition, &mGroundPlane);
}

void TMapObjGeneral::recovering()
{
	startSound(9);
	if (hasModelOrAnimData(6)) {
		J3DModel* model = getModel();
		MtxPtr mat      = model->getAnmMtx(0);
		f32 fVar1       = mat[3][1] - unk144;
		mDamageHeight += fVar1;
		calcEntryRadius();
		if (mHeldObject)
			mHeldObject->mPosition.y += fVar1;
		unk144 = mat[3][1];
		if (!animIsFinished())
			return;
	} else if (mPosition.y < unk144) {
		mPosition.y += mMapObjData->mSink->unk4;
		if (mHeldObject)
			mHeldObject->mPosition.y += mMapObjData->mSink->unk4;
		return;
	}

	makeObjRecovered();
}

void TMapObjGeneral::sinking()
{
	mPosition.y -= mMapObjData->mSink->unk0;

	for (int i = 0; i < getColNum(); ++i) {
		if (getCollision(i)->checkActorType(0x1000000)) {
			recover();
			return;
		}
	}

	if (mPosition.y + mMapObjData->mHit->unkC[2].unk4 < unk144) {
		if (mPosition.x != mInitialPosition.x
		    || mPosition.z != mInitialPosition.z) {
			makeObjDefault();
			makeObjAppeared();
		} else {
			makeObjBuried();
		}
	}
}

void TMapObjGeneral::breaking()
{
	if (animIsFinished()) {
		makeObjDead();
		if (checkMapObjFlag(MAP_OBJ_FLAG_RESPAWNING)) {
			makeObjDefault();
			waitToAppear(0);
		}
	}
}

void TMapObjGeneral::appearing()
{
	// TODO: uuuuuuuh...
	if (hasAnim(1)) {
		if (animIsFinished())
			goto uuuh;
		return;
	}

	{
		mScaling.x += mNormalAppearingScaleUp;
		mScaling.y += mNormalAppearingScaleUp;
		mScaling.z += mNormalAppearingScaleUp;
		if (mScaling.x < mInitialScaling.x)
			return;

		mScaling.set(mInitialScaling);
	}

uuuh:
	if (!checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	makeObjAppeared();
}

void TMapObjGeneral::appeared()
{
	if (checkMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING) && !isStateTimerEngaged())
		makeObjDead();
}

void TMapObjGeneral::makeObjRecovered()
{
	makeObjDefault();
	makeObjAppeared();
}

void TMapObjGeneral::makeObjBuried()
{
	unk144 = mPosition.y;
	mPosition.y -= mMapObjData->mHit->unkC[2].unkC;
	onHitFlag(HIT_FLAG_NO_COLLISION);
	removeMapCollision();
	mMActor = nullptr;
	mState  = STATE_BURIED;
}

void TMapObjGeneral::receiveMessageFromPlayer() { startAnim(4); }

u32 TMapObjGeneral::touchWater(THitActor* water)
{
	if (checkMapObjFlag(MAP_OBJ_FLAG_UNK400000)) {
		kill();
		return 1;
	} else {
		if (hasModelOrAnimData(3)) {
			startAnim(3);
			mState = STATE_TOUCHING_WATER;
		}
		return 1;
	}
}

void TMapObjGeneral::touchPlayer(THitActor* player)
{
	TMapObjBase::touchPlayer(player);
	if (hasModelOrAnimData(4)) {
		startAnim(4);
		mState = STATE_TOUCHING_PLAYER;
	}
}

void TMapObjGeneral::recover()
{
	gpPollution->clean(mPosition.x, unk144, mPosition.z,
	                   (u16)(mMapObjData->mHit->unkC[2].unk0 / 6.0f));

	setUpMapCollision(1);
	startAnim(6);
	mState = STATE_RECOVERING;
	setObjHitData(0);
	startSound(8);
	mDamageHeight = 0.0f;
	calcEntryRadius();
	offHitFlag(HIT_FLAG_NO_COLLISION);
	if (hasModelOrAnimData(6)) {
		f32 tmp     = mPosition.y;
		mPosition.y = unk144;
		unk144      = tmp;
		getModel();
	}
}

void TMapObjGeneral::hold(TTakeActor* actor)
{
	if (mMapCollisionManager && mMapCollisionManager->unk8)
		mMapCollisionManager->unk8->remove();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mHolder = actor;
	mState  = STATE_HOLDING;
}

void TMapObjGeneral::ensureTakeSituation()
{
	TMapObjBase::ensureTakeSituation();
	if (isState(STATE_HOLDING) && mHolder == nullptr) {
		mState = STATE_NORMAL;
		offLiveFlag(LIVE_FLAG_UNK10);
	}
}

void TMapObjGeneral::kill()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	removeMapCollision();
	onLiveFlag(LIVE_FLAG_UNK10 | LIVE_FLAG_UNK8);
	mStateTimer = -1;
	startAnim(2);
	mState = STATE_BREAKING;
	startSound(2);
	breaking();
}

void TMapObjGeneral::appear()
{
	makeObjAppeared();
	startAnim(1);
	if (checkMapObjFlag(MAP_OBJ_FLAG_UNK800000)) {
		mScaling.x = mNormalAppearingScaleUp;
		mScaling.y = mNormalAppearingScaleUp;
		mScaling.z = mNormalAppearingScaleUp;
	}

	if (!isActorType(0x20000010)
	    || !TFlagManager::smInstance->getBlueCoinFlag(
	        gpMarDirector->getCurrentMap(), mEventId))
		startSound(1);

	appearing();
	if (checkMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING))
		mStateTimer = getLivingTime();

	mState = STATE_APPEARING;
}

void TMapObjGeneral::work()
{
	switch (mState) {
	case STATE_NORMAL:
		appeared();
		break;
	case STATE_APPEARING:
		appearing();
		break;
	case STATE_BREAKING:
		breaking();
		break;
	case STATE_SINKING:
		sinking();
		break;
	case STATE_RECOVERING:
		recovering();
		break;
	case STATE_TOUCHING_PLAYER:
		touchingPlayer();
		break;
	case STATE_TOUCHING_WATER:
		touchingWater();
		break;
	case STATE_HOLDING:
		holding();
		break;
	case STATE_BURIED:
		waitingToRecover();
		break;
	}
}

void TMapObjGeneral::touchWall(JGeometry::TVec3<f32>* param_1,
                               TBGWallCheckRecord* param_2)
{
	param_1->x = param_2->mCenter.x;
	param_1->z = param_2->mCenter.z;
	calcReflectingVelocity(param_2->mResultWalls[0],
	                       mMapObjData->mPhysical->unk4->unk8, &mVelocity);
}

void TMapObjGeneral::checkWallCollision(JGeometry::TVec3<f32>* param_1)
{
	param_1->y += mMapObjData->mPhysical->unk4->unk1C;

	TBGWallCheckRecord check(*param_1, mBodyRadius, 4,
	                         mMapObjData->mPhysical->mWallCheckFlags);

	bool touched = gpMap->isTouchedWallsAndMoveXZ(&check);

	param_1->y -= mMapObjData->mPhysical->unk4->unk1C;

	if (touched) {
		unk138 = check.mResultWalls[0];
		touchWall(param_1, &check);
	} else {
		unk138 = 0;
	}
}

void TMapObjGeneral::touchRoof(JGeometry::TVec3<f32>* param_1)
{
	param_1->y = unk140;
}

void TMapObjGeneral::checkRoofCollision(JGeometry::TVec3<f32>* param_1)
{
	unk140 = gpMap->checkRoof(param_1->x, param_1->y + mHeadHeight, param_1->z,
	                          &unk13C);
	if (param_1->y + mHeadHeight >= unk140)
		touchRoof(param_1);
}

inline void playCoinSound(const JGeometry::TVec3<f32>& pos,
                          const JGeometry::TVec3<f32>& vel)
{
	f32 a = abs(JGeometry::TVec3<f32>(vel).y);
	if (gpMSound->gateCheck(MSD_SE_SY_COIN_BOUND)) {
		MSoundSESystem::MSoundSE::startSoundActorWithInfo(
		    MSD_SE_SY_COIN_BOUND, pos, nullptr, a, 0, 0, nullptr, 0, 4);
	}
}

void TMapObjGeneral::touchGround(JGeometry::TVec3<f32>* param_1)
{
	if (mMapObjData->mPhysical ? true : false) {
		mVelocity.x *= mMapObjData->mPhysical->unk4->unk10;
		mVelocity.z *= mMapObjData->mPhysical->unk4->unk10;
	}

	if ((mMapObjData->mPhysical ? true : false)
	    && abs(JGeometry::TVec3<f32>(mVelocity).y)
	           > mMapObjData->mPhysical->unk4->unkC) {
		param_1->y -= JGeometry::TVec3<f32>(mVelocity).y;
		mVelocity.y *= -mMapObjData->mPhysical->unk4->unk4;
		if (isCoin(this)) {
			playCoinSound(mPosition, mVelocity);
		} else {
			startSound(4);
		}
	} else {
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		mVelocity.x = mVelocity.y = mVelocity.z = 0.0f;
		onLiveFlag(LIVE_FLAG_UNK10);
		param_1->y = mGroundHeight;
	}
}

void TMapObjGeneral::checkGroundCollision(JGeometry::TVec3<f32>* param_1)
{
	mGroundHeight = gpMap->checkGround(param_1->x, param_1->y + mHeadHeight,
	                                   param_1->z, &mGroundPlane);
	mGroundHeight += 1.0f;
	if (param_1->y <= mGroundHeight) {
		touchGround(param_1);
	} else if (!mGroundActor)
		onLiveFlag(LIVE_FLAG_AIRBORNE);
}

void TMapObjGeneral::calcVelocity()
{
	if (checkLiveFlag2(LIVE_FLAG_AIRBORNE)) {
		f32 dVar5 = getGravityY();
		mVelocity.y -= dVar5;

		mVelocity.y = MsClamp<f32>(mVelocity.y, -mBodyRadius, mBodyRadius);
	}

	const TMapObjPhysicalInfo* piVar4 = mMapObjData->mPhysical;
	if (piVar4 ? (u8)1 : (u8)0) {
		mVelocity.x *= mMapObjData->mPhysical->unk4->unk18;
		mVelocity.z *= mMapObjData->mPhysical->unk4->unk18;

		mVelocity.x = MsClamp<f32>(mVelocity.x, -mBodyRadius, mBodyRadius);
		mVelocity.z = MsClamp<f32>(mVelocity.z, -mBodyRadius, mBodyRadius);

		if (mGroundPlane->mNormal.y == 1.0f) {
			if (abs(mVelocity.x) < mMapObjData->mPhysical->unk4->unkC)
				mVelocity.x = 0.0f;
			if (abs(mVelocity.z) < mMapObjData->mPhysical->unk4->unkC)
				mVelocity.z = 0.0f;
		}
	}
}

void TMapObjGeneral::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	if (mBinder != nullptr) {
		mBinder->bind(this);
		return;
	}

	calcVelocity();
	JGeometry::TVec3<f32> vec = getPosition();
	vec.add(mLinearVelocity);
	vec.add(mVelocity);
	checkGroundCollision(&vec);
	if (checkMapObjFlag(MAP_OBJ_FLAG_ENABLE_WALL_COLLISION))
		checkWallCollision(&vec);

	if (checkMapObjFlag(MAP_OBJ_FLAG_ENABLE_ROOF_COLLISION)) {
		JGeometry::TVec3<f32> vel = mVelocity;
		if (vel.y > 0.0f)
			checkRoofCollision(&vec);
	}

	if (mGroundPlane->isIllegalData()) {
		kill();
		return;
	}

	if (!checkLiveFlag2(LIVE_FLAG_AIRBORNE)) {
		JGeometry::TVec3<f32> vel     = mVelocity;
		JGeometry::TVec3<f32> velCopy = vel;
		if (velCopy.x == 0.0f) {
			JGeometry::TVec3<f32> velCopy2 = vel;
			if (velCopy2.y == 0.0f) {
				JGeometry::TVec3<f32> velCopy3 = vel;
				if (velCopy3.z == 0.0f) {
					onLiveFlag(LIVE_FLAG_UNK10);
				}
			}
		}
	}

	mLinearVelocity = vec - mLinearVelocity;
}

void TMapObjGeneral::control()
{
	TMapObjBase::control();
	if (checkMapObjFlag(MAP_OBJ_FLAG_CAN_SINK) && isState(STATE_NORMAL)
	    && !isAirborne() && isPollutedGround(mPosition))
		sink();

	work();
}

void TMapObjGeneral::calcRootMatrix()
{
	J3DModel* model = getModel();

	if (isState(STATE_HOLDING) && mHolder) {
		if (mMapObjData->mHold) {
			TMapObjHoldData* hold = mMapObjData->mHold;

			MtxPtr src = getTakingMtx();
			MTXCopy(src, hold->unkC->getBaseTRMtx());
			hold->unkC->calc();

			MtxPtr src2 = hold->unk10;
			MTXCopy(src2, model->getBaseTRMtx());
			mPosition.set(src2[3][0], src2[3][1], src2[3][2]);
		} else {
			MtxPtr src = getTakingMtx();
			MTXCopy(src, checkMapObjFlag(MAP_OBJ_FLAG_UNK100)
			                 ? model->getAnmMtx(0)
			                 : model->getBaseTRMtx());
			mPosition.set(src[3][0], src[3][1], src[3][2]);
		}
	} else {
		JGeometry::TVec3<f32> pos(mPosition.x, mPosition.y - mYOffset,
		                          mPosition.z);
		MsMtxSetXYZRPH(model->getBaseTRMtx(), pos.x, pos.y, pos.z, mRotation.x,
		               mRotation.y, mRotation.z);
	}
	model->setBaseScale(mScaling);
}

void TMapObjGeneral::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 1) {
		if (isState(STATE_WAITING_TO_APPEAR))
			waitingToAppear();
	} else {
		if (checkMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING) && isStateTimerEngaged()
		    && getStateTimer() < getFlushTime()
		    && ((getStateTimer() / mNormalFlushInterval) & 1) != 0) {
			return;
		}
	}

	TMapObjBase::perform(param_1, param_2);
}

BOOL TMapObjGeneral::receiveMessage(THitActor* sender, u32 message)
{
	int ret = TMapObjBase::receiveMessage(sender, message);
	if (ret)
		return true;

	if (message == HIT_MESSAGE_TAKE && checkMapObjFlag(MAP_OBJ_FLAG_UNK100000)
	    && JGeometry::TVec3<f32>(mVelocity).isZero()
	    && (isState(STATE_APPEARING) || isState(STATE_NORMAL)
	        || isState(STATE_TOUCHING_PLAYER)
	        || isState(STATE_TOUCHING_WATER))) {
		hold((TTakeActor*)sender);
		return true;
	}

	if (message == HIT_MESSAGE_TAKE && isActorType(0x10000025)
	    && (isState(STATE_APPEARING) || isState(STATE_NORMAL))) {
		hold((TTakeActor*)sender);
		return 1;
	}

	if (message == HIT_MESSAGE_PUT && isState(STATE_HOLDING)) {
		put();
		return true;
	}

	if (message == HIT_MESSAGE_THROWN && isState(STATE_HOLDING)
	    && mMapObjData->mPhysical != nullptr) {
		thrown();
		return true;
	}

	if (message == HIT_MESSAGE_HIP_DROP
	    && checkMapObjFlag(MAP_OBJ_FLAG_UNK200000)) {
		kill();
		return true;
	}

	if (isActorType(0x80000001)
	    && (message == HIT_MESSAGE_TRAMPLE
	        || message == HIT_MESSAGE_HIP_DROP)) {
		receiveMessageFromPlayer();
		return true;
	}

	if (message == HIT_MESSAGE_UNKB
	    && checkMapObjFlag(MAP_OBJ_FLAG_UNK200000)) {
		kill();
	}

	return false;
}

void TMapObjGeneral::loadAfter()
{
	TMapObjBase::loadAfter();
	if (checkMapObjFlag(MAP_OBJ_FLAG_CAN_SINK) && isPollutedGround(mPosition))
		makeObjBuried();
}

TMapObjGeneral::TMapObjGeneral(const char* name)
    : TMapObjBase(name)
    , unk138(0)
    , unk13C(0)
    , unk140(0.0f)
    , unk144(0.0f)
{
}
