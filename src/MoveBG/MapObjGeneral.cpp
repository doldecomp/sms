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

// Sum level: gives the radius sum retail's f1 = mario + damage operand order
// (and +8 of pool) in waitingToAppear's plain branch.
static inline f32 MOGSum(f32 a, f32 b) { return a + b; }

// The comparisons test the distance against the radii (retail's
// `fcmpo dist, sum; ble`): the object appears once Mario is outside them.
// TODO: 99.9%. The 0x4000005a branch's `mario + damage` fadds has its
// operands swapped. Only reading getDamageRadius() in the sum while keeping
// the (then unused) named local lands it, which is a dead local and refused;
// MOGSum there, a reordered sum and 100.0f first are inert or cost frame.
void TMapObjGeneral::waitingToAppear()
{
	if (isStateTimerEngaged())
		return;

	if (isActorType(0x4000005a)) {
		f32 damageRadius = getDamageRadius();
		if (distToMario(getInitialPosition())
		    > SMS_GetMarioDamageRadius() + damageRadius + 100.0f)
			appear();
	} else {
		f32 damageRadius = getDamageRadius();
		if (distToMario(getInitialPosition())
		    > MOGSum(SMS_GetMarioDamageRadius(), damageRadius))
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

// Binding level over a raw member read, worth +8 of low region in
// TMapObjGeneral::put (batch 127).
static inline TMap* MapObjGeneralGetMap()
{
	TMap* map = gpMap;
	return map;
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
	mGroundHeight = MapObjGeneralGetMap()->checkGround(mPosition, &mGroundPlane);
}

// TODO: 91.8%. Retail derives the sine and cosine table index twice from
// one angle load (cosine first) where ours shares one `sraw` (the
// MapObjItem2 case-5 tell), the sine product's operands are swapped, and the
// frame is 0x38 short (every temporary 0x38 low). Inert or worse: the raw
// s16 argument, TU-local sin/cos table readers (by argument or reading the
// global), per-component helpers, and a TVec3 temporary assigned whole.
void TMapObjGeneral::thrown()
{
	mPosition.set(gpMarioPos->x, gpMarioPos->y, gpMarioPos->z);
	mRotation.set(*gpMarioAngleX, *gpMarioAngleY, *gpMarioAngleZ);

	mGroundHeight = gpMap->checkGround(mPosition, &mGroundPlane);
	unk138        = 0;
	mHolder       = nullptr;

	mVelocity.set(JMASSin((s32)*gpMarioAngleY)
	                      * mMapObjData->mPhysical->unk4->unk2C
	                      * *gpMarioThrowPower
	                  + (mNormalThrowSpeedRate * *gpMarioSpeedX),
	              mMapObjData->mPhysical->unk4->unk30,
	              JMASCos((s32)*gpMarioAngleY)
	                      * mMapObjData->mPhysical->unk4->unk2C
	                      * *gpMarioThrowPower
	                  + (mNormalThrowSpeedRate * *gpMarioSpeedZ));

	offLiveFlag(LIVE_FLAG_UNK10);
	JGeometry::TVec3<f32> vel = mVelocity;
	mPosition.add(vel);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	removeMapCollision();
	offHitFlag(HIT_FLAG_NO_COLLISION);
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
	mPosition     = getHolder()->mPosition;
	mGroundHeight = gpMap->checkGround(getPosition(), &mGroundPlane);
}

static inline const TMapObjSinkData* TMapObjGeneralGetSink(TMapObjGeneral* p)
{
	TMapObjData* data           = p->mMapObjData;
	const TMapObjSinkData* sink = data->mSink;
	return sink;
}

static inline TTakeActor* TMapObjGeneralGetHeldObject(TMapObjGeneral* p)
{
	return p->mHeldObject;
}

void TMapObjGeneral::recovering()
{
	startSound(9);
	if (hasModelOrAnimData(6)) {
		J3DModel* model = getModel();
		MtxPtr mat      = model->getAnmMtx(0);
		f32 fVar1       = mat[1][3] - unk144;
		mDamageHeight += fVar1;
		calcEntryRadius();
		if (mHeldObject)
			mHeldObject->mPosition.y += fVar1;
		unk144 = mat[1][3];
		if (!animIsFinished())
			return;
	} else if (mPosition.y < unk144) {
		mPosition.y += TMapObjGeneralGetSink(this)->unk4;
		if (TMapObjGeneralGetHeldObject(this))
			TMapObjGeneralGetHeldObject(this)->mPosition.y
			    += TMapObjGeneralGetSink(this)->unk4;
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
		if (mPosition.x != getInitialPosition().x
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

static inline const JGeometry::TVec3<f32>&
TMapObjGeneralGetInitialScaling(TMapObjGeneral* p)
{
	const JGeometry::TVec3<f32>& scaling = p->mInitialScaling;
	return scaling;
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

		mScaling.set(TMapObjGeneralGetInitialScaling(this));
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

// TODO: frame 0x28 short (0x28 vs 0x50) and the hit radius loads into f5
// where ours takes f3. Inert: getMapObjData(), raw gpPollution, a named or
// helper-converted u16 size, a table pointer, SMSGetPollution() forks.
void TMapObjGeneral::recover()
{
	SMSGetPollution()->clean(mPosition.x, unk144, mPosition.z,
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

// Binding level worth +8 of low region, landing
// TMapObjGeneral::ensureTakeSituation's frame at 0x20 (batch 124).
static inline bool MapObjGeneralIsStateL0(TMapObjGeneral* p, u32 i)
{
	bool state = p->isState(i);
	return state;
}

static inline bool MapObjGeneralIsState(TMapObjGeneral* p, u32 i)
{
	bool state = MapObjGeneralIsStateL0(p, i);
	return state;
}

void TMapObjGeneral::ensureTakeSituation()
{
	TMapObjBase::ensureTakeSituation();
	if (MapObjGeneralIsState(this, STATE_HOLDING) && mHolder == nullptr) {
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

// Binding level worth +8 of low region, landing TMapObjGeneral::appear's
// frame at 0x28 (batch 121).
static inline u32 MapObjGeneralGetLivingTime(const TMapObjGeneral* p)
{
	u32 livingTime = p->getLivingTime();
	return livingTime;
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
		mStateTimer = MapObjGeneralGetLivingTime(this);

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

// TODO: TMapObjBaseData wants a `const TMapObjPhysicalInfo* getPhysical()`
// accessor; parked here as a TU-local until a header batch adds it.
static inline const TMapObjPhysicalInfo* MapObjGetPhysical(
    const TMapObjData* p)
{
	return p->mPhysical;
}

void TMapObjGeneral::checkWallCollision(JGeometry::TVec3<f32>* param_1)
{
	param_1->y += MapObjGetPhysical(mMapObjData)->unk4->unk1C;

	TBGWallCheckRecord check(*param_1, mBodyRadius, 4,
	                         mMapObjData->mPhysical->mWallCheckFlags);

	bool touched = gpMap->isTouchedWallsAndMoveXZ(&check);

	param_1->y -= MapObjGetPhysical(mMapObjData)->unk4->unk1C;

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

static inline TMapObjPhysicalData*
TMapObjGeneralTouchGroundGetPhysicalData(TMapObjGeneral* p)
{
	TMapObjPhysicalData* data = p->mMapObjData->mPhysical->unk4;
	return data;
}

static inline const TMapObjPhysicalInfo*
TMapObjGeneralTouchGroundGetPhysical(TMapObjGeneral* p)
{
	const TMapObjPhysicalInfo* physical = p->mMapObjData->mPhysical;
	return physical;
}

void TMapObjGeneral::touchGround(JGeometry::TVec3<f32>* param_1)
{
	if (TMapObjGeneralTouchGroundGetPhysical(this) ? true : false) {
		mVelocity.x *= TMapObjGeneralTouchGroundGetPhysicalData(this)->unk10;
		mVelocity.z *= TMapObjGeneralTouchGroundGetPhysicalData(this)->unk10;
	}

	if ((mMapObjData->mPhysical ? true : false)
	    && abs(JGeometry::TVec3<f32>(mVelocity).y)
	           > TMapObjGeneralTouchGroundGetPhysicalData(this)->unkC) {
		param_1->y -= JGeometry::TVec3<f32>(mVelocity).y;
		mVelocity.y *= -TMapObjGeneralTouchGroundGetPhysicalData(this)->unk4;
		if (isCoin(this)) {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_SY_COIN_BOUND, &mPosition, nullptr,
			    abs(JGeometry::TVec3<f32>(mVelocity).y), 0, 0, nullptr, 0, 4);
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

static inline TMapObjPhysicalData*
TMapObjGeneralGetPhysicalData(TMapObjGeneral* p)
{
	TMapObjPhysicalData* physData = p->mMapObjData->mPhysical->unk4;
	return physData;
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
		mVelocity.x *= TMapObjGeneralGetPhysicalData(this)->unk18;
		mVelocity.z *= TMapObjGeneralGetPhysicalData(this)->unk18;

		mVelocity.x = MsClamp<f32>(mVelocity.x, -mBodyRadius, mBodyRadius);
		mVelocity.z = MsClamp<f32>(mVelocity.z, -mBodyRadius, mBodyRadius);

		if (mGroundPlane->mNormal.y == 1.0f) {
			if (abs(mVelocity.x) < TMapObjGeneralGetPhysicalData(this)->unkC)
				mVelocity.x = 0.0f;
			if (abs(mVelocity.z) < TMapObjGeneralGetPhysicalData(this)->unkC)
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

	mLinearVelocity = vec - mPosition;
}

// Binding level worth +8 of low region, landing TMapObjGeneral::control's
// frame at 0x20 (batch 124).
static inline bool
MapObjGeneralCheckMapObjFlagL0(const TMapObjGeneral* p, u32 i)
{
	bool mapObjFlag = p->checkMapObjFlag(i);
	return mapObjFlag;
}

static inline bool MapObjGeneralCheckMapObjFlag(const TMapObjGeneral* p, u32 i)
{
	bool mapObjFlag = MapObjGeneralCheckMapObjFlagL0(p, i);
	return mapObjFlag;
}

void TMapObjGeneral::control()
{
	TMapObjBase::control();
	if (MapObjGeneralCheckMapObjFlag(this, MAP_OBJ_FLAG_CAN_SINK)
	    && isState(STATE_NORMAL) && !isAirborne()
	    && isPollutedGround(mPosition))
		sink();

	work();
}

void TMapObjGeneral::calcRootMatrix()
{
	J3DModel* model = getModel();

	if (isState(STATE_HOLDING) && mHolder) {
		if (mMapObjData->mHold) {
			TMapObjHoldData* hold = mMapObjData->mHold;

			MtxPtr src = mHolder->getTakingMtx();
			MTXCopy(src, hold->unkC->getBaseTRMtx());
			hold->unkC->calc();

			MtxPtr src2 = hold->unk10;
			MTXCopy(src2, model->getBaseTRMtx());
			mPosition.set(src2[0][3], src2[1][3], src2[2][3]);
		} else {
			MtxPtr src = mHolder->getTakingMtx();
			MTXCopy(src, checkMapObjFlag(MAP_OBJ_FLAG_UNK100)
			                 ? model->getAnmMtx(0)
			                 : model->getBaseTRMtx());
			mPosition.set(src[0][3], src[1][3], src[2][3]);
		}
	} else {
		MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x,
		               mPosition.y - mYOffset, mPosition.z, mRotation.x,
		               mRotation.y, mRotation.z);
	}
	model->setBaseScale(mScaling);
}

// Fork over the static flush interval (+8 of pool in perform).
static inline int MapObjGeneralFlushInterval()
{
	return TMapObjGeneral::mNormalFlushInterval;
}

void TMapObjGeneral::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		if (isState(STATE_WAITING_TO_APPEAR))
			waitingToAppear();
	} else {
		// The explicit upcast stops MWCC reusing the timer load from the
		// engaged test, as retail reloads it.
		if (checkMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING)
		    && ((TMapObjBase*)this)->isStateTimerEngaged()
		    && getStateTimer() < getFlushTime()
		    && ((getStateTimer() / MapObjGeneralFlushInterval()) & 1) != 0) {
			return;
		}
	}

	TMapObjBase::perform(cue, graphics);
}

static inline const JGeometry::TVec3<f32>& MapObjGeneralVelocity(TMapObjGeneral* self)
{
	const JGeometry::TVec3<f32>& r = self->getVelocity();
	return r;
}

// The two actor-type tests read the sender's type (retail's `lwz 0x4c` off
// the sender register), not this object's.
// TODO: frame 0x10 short (0x48 vs 0x58); getVelocity() is +8 of it. Inert:
// the base result unnamed or BOOL, getMapObjData(); a TTakeActor binder
// costs 1% of match.
BOOL TMapObjGeneral::receiveMessage(THitActor* sender, u32 message)
{
	int ret = TMapObjBase::receiveMessage(sender, message);
	if (ret)
		return true;

	if (message == HIT_MESSAGE_TAKE && checkMapObjFlag(MAP_OBJ_FLAG_UNK100000)
	    && JGeometry::TVec3<f32>(MapObjGeneralVelocity(this)).isZero()
	    && (isState(STATE_APPEARING) || isState(STATE_NORMAL)
	        || isState(STATE_TOUCHING_PLAYER)
	        || isState(STATE_TOUCHING_WATER))) {
		hold((TTakeActor*)sender);
		return true;
	}

	if (message == HIT_MESSAGE_TAKE && sender->isActorType(0x10000025)
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

	bool fromMario = sender->isActorType(0x80000001);
	if (fromMario
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
