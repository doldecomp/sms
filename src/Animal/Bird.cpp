#include <Animal/Bird.hpp>
#include <Animal/AnimalBase.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/WireBinder.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/Map.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <System/Application.hpp>
#include <System/StageUtil.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* bird_bastable[] = {
	nullptr,
	"/scene/bird/bas/bird_fly.bas",
	"/scene/bird/bas/bird_open.bas",
	nullptr,
	nullptr,
	"/scene/bird/bas/bird_start.bas",
	"/scene/bird/bas/bird_stop.bas",
	nullptr,
	nullptr,
};

namespace {

const int cRandomAnims[] = { 7, 4, 0, 2, 8 };

const char* const cMatName = "_mat_body1";

const GXColorS10 cColorTable[] = {
	{ 0, 100, 255, 0 },
	{ 0, 200, 0, 0 },
	{ 255, 200, 0, 0 },
	{ 255, 0, 0, 0 },
};

} // namespace

TAnimalBird::TAnimalBird(const char* name)
    : TSpineEnemy(name)
    , unk150(nullptr)
    , unk154(nullptr)
{
}

void TAnimalBird::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("bird_man.bmd", 0);
	mSpine->initWith(&TNerveAnimalBirdWaitOnGround::theNerve());
	initParams();
	initCollision();
	mScaledBodyRadius = 35.0f;
	initAnmSound();
}

void TAnimalBird::initTevColor(const GXColorS10* color)
{
	int idx = getModel()->getModelData()->getMaterialName()->getIndex(cMatName);
	SMS_InitPacket_OneTevColor(getModel(), idx, GX_TEVREG1, color);
}

void TAnimalBird::initCollision()
{
	initHitActor(ACTOR_TYPE_ENEMY_UNK32, 0, 0, 50.0f, 50.0f, 70.0f, 80.0f);
	onHitFilter(HIT_FILTER_NO_ATTACK);
	offHitFilter(HIT_FILTER_NO_COLLISION);
}

void TAnimalBird::initParams()
{
	unk158.x = mPosition.x;
	unk158.y = mPosition.y;
	unk158.z = mPosition.z;
	unk158.y += 90.0f;
	unk164.x = mRotation.x;
	unk164.y = mRotation.y;
	unk164.z = mRotation.z;
	resetHitPoints();
	unk178 = 0;
	unk17C = 0;
	unk170 = 1.0f;
	offLiveFlag(LIVE_FLAG_AIRBORNE);
	unk174 = 1.0f - 0.1f * (MsRandF() - 0.5f);
	if (TWireBinder::isOnWire(mPosition)) {
		unk154 = new TWireBinder;
		unk154->init(mPosition);
	}
}

void TAnimalBird::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);

	s32 eventId;
	stream >> eventId;
	if (eventId >= 0)
		unk150
		    = TMapObjBaseManager::newAndRegisterObjByEventID(eventId, "鳥用");
	else
		unk150 = TMapObjBaseManager::newAndRegisterObjByEventID(100, "");

	switch (unk150->getActorType()) {
	default:
		unk180 = 1;
		break;
	case ACTOR_TYPE_SHINE:
		unk180 = 2;
		break;
	case ACTOR_TYPE_COIN_RED:
		unk180 = 3;
		break;
	case ACTOR_TYPE_COIN_BLUE:
		unk180 = 0;
		if (SMS_isGetBlueCoin(eventId))
			onLiveFlag(LIVE_FLAG_DEAD);
		break;
	}

	initTevColor(&cColorTable[unk180]);
}

void TAnimalBird::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	MSoundSESystem::MSRandPlay::registerTrans(MSD_SE_OBJ_BIRD_DOL_FLYING1,
	                                          &mPosition);
	MSoundSESystem::MSRandPlay::registerTrans(MSD_SE_OBJ_BIRD_DOL_CHUN,
	                                          &mPosition);
}

BOOL TAnimalBird::receiveMessage(THitActor* sender, u32 message)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return false;

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		SMS_EasyEmitParticle(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                     nullptr, JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
		                              &sender->mPosition, 0, 0.0f, 0, 0, 4);
		behaveHitWater();
		return true;
	}

	if (message == HIT_MESSAGE_TAKE && mHolder == nullptr) {
		onHitFilter(HIT_FILTER_NO_COLLISION);
		mHolder = (TTakeActor*)sender;
		SMS_EasyEmitParticle(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                     nullptr, JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		return true;
	}

	if ((message == HIT_MESSAGE_PUT || message == HIT_MESSAGE_THROWN)
	    && mHolder == sender) {
		mHolder = nullptr;
		offHitFilter(HIT_FILTER_NO_COLLISION);
		return true;
	}

	if (message == HIT_MESSAGE_UNKB) {
		mHolder = nullptr;
		if (mSpine->getLatestNerve()
		    != &TNerveAnimalBirdChangeToCoin::theNerve()) {
			mSpine->reset();
			mSpine->setNext(&TNerveAnimalBirdChangeToCoin::theNerve());
		} else {
			kill();
		}
		return true;
	}

	if (message == HIT_MESSAGE_TRAMPLE
	    && sender->isActorType(ACTOR_TYPE_POPO)) {
		if (mSpine->getLatestNerve()
		    != &TNerveAnimalBirdChangeToCoin::theNerve()) {
			mSpine->reset();
			mSpine->setNext(&TNerveAnimalBirdChangeToCoin::theNerve());
		} else {
			receiveMessage(this, HIT_MESSAGE_SPRAYED_BY_WATER);
		}
		return true;
	}

	return TSpineEnemy::receiveMessage(sender, message);
}

void TAnimalBird::calcRootMatrix()
{
	if (mHolder != nullptr)
		getModel()->setBaseTRMtx(mHolder->getTakingMtx());
	else
		TSpineEnemy::calcRootMatrix();

	getModel()->getBaseTRMtx()[1][3] += 35.0f;
}

void TAnimalBird::moveObject()
{
	if (unk178 > 0)
		--unk178;

	checkFalling();
	checkChangeToItem();
	updateSound();
	TLiveActor::moveObject();
}

void TAnimalBird::bind()
{
	if (!isCheckWithWireBinder())
		TLiveActor::bind();
	else
		unk154->bind(this);
}

const char** TAnimalBird::getBasNameTable() const { return bird_bastable; }

void TAnimalBird::behaveHitWater()
{
	if (unk178 <= 0) {
		unk178 = getSaveParam2()->mWaterproofTimerMax.get();
		if (checkLiveFlag(LIVE_FLAG_AIRBORNE))
			decHitPoints();
	}
}

bool TAnimalBird::isOnGroundNerve() const
{
	TSpineBase<TLiveActor>* spine = mSpine;
	return spine->getLatestNerve() == &TNerveAnimalBirdWaitOnGround::theNerve()
	       || spine->getLatestNerve()
	              == &TNerveAnimalBirdActionOnGround::theNerve()
	       || spine->getLatestNerve()
	              == &TNerveAnimalBirdWalkOnGround::theNerve();
}

void TAnimalBird::checkFalling()
{
	if (isOnGroundNerve()) {
		if (checkLiveFlag(LIVE_FLAG_AIRBORNE)) {
			if (getSaveParam2()->mFloatingTimerMax.get() < ++unk17C) {
				mSpine->reset();
				mSpine->setNext(&TNerveAnimalBirdTakeoff::theNerve());
			}
		} else {
			unk17C = 0;
		}
	}
}

void TAnimalBird::checkChangeToItem()
{
	if (!isChangeToItem() && mHitPoints == 0) {
		mSpine->reset();
		mSpine->setNext(&TNerveAnimalBirdChangeToCoin::theNerve());
	}
}

void TAnimalBird::checkNotAppear(s32) { }

void TAnimalBird::updateSound()
{
	if (isFlying())
		SMSGetMSound()->startSeRandPlay(MSD_SE_OBJ_BIRD_DOL_FLYING1,
		                                mInstanceIndex);

	if (isOnGroundNerve())
		SMSGetMSound()->startSeRandPlay(MSD_SE_OBJ_BIRD_DOL_CHUN,
		                                mInstanceIndex);
}

bool TAnimalBird::isWantToFly() const
{
	return (unk178 > 0 || isFindMario()) && MsRandF() < 0.5f;
}

bool TAnimalBird::isWantToAction() const
{
	int time = mSpine->getTime() - getSaveParam2()->mActionTimer.get();
	if (time < 0)
		return false;

	f32 rate = (f32)time / getSaveParam2()->mActionTimerAdd.get();
	return MsRandF() < rate;
}

bool TAnimalBird::isWantToRest() const { return false; }

bool TAnimalBird::isFindMario() const
{
	f32 dy = fabsf(gpMarioPos->y - mPosition.y);
	if (getSaveParam2()->mSearchHeight.get() < dy)
		return false;

	return isInSight(SMS_GetMarioPos(),
	                 unk174 * getSaveParam2()->mSearchLength.get(),
	                 unk174 * getSaveParam2()->mSearchAngle.get(),
	                 unk174 * getSaveParam2()->mSearchAware.get());
}

bool TAnimalBird::isChangeToItem() const
{
	return mSpine->isNerve(&TNerveAnimalBirdChangeToCoin::theNerve());
}

bool TAnimalBird::isGroundShaken() const { return false; }

bool TAnimalBird::isCheckWithWireBinder() const
{
	return unk154 != nullptr
	       && (isOnGroundNerve()
	           || mSpine->getLatestNerve()
	                  == &TNerveAnimalBirdPreLanding::theNerve());
}

bool TAnimalBird::isChanged() const { return false; }

bool TAnimalBird::isFlying() const
{
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveAnimalBirdGraphWander::theNerve()
	       || nerve == &TNerveAnimalBirdComeback::theNerve();
}

void TAnimalBird::doDropCoin()
{
	onLiveFlag(LIVE_FLAG_DEAD);
	if (unk150->isActorType(ACTOR_TYPE_SHINE)) {
		unk150->JSGSetTranslation(mPosition);
		((TShine*)unk150)->appearWithDemo("鳥シャインカメラ");
	} else {
		TMapObjBase* obj = unk150->isActorType(ACTOR_TYPE_COIN)
		                       ? gpItemManager->makeObjAppear(ACTOR_TYPE_COIN)
		                       : unk150;
		if (obj != nullptr) {
			obj->appear();
			obj->JSGSetTranslation(mPosition);
			obj->mVelocity.set(0.0f, -10.0f, 0.0f);
			obj->offLiveFlag(LIVE_FLAG_UNK10);
			obj->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}
	}
}

void TAnimalBird::doFlyToCurPathNode()
{
	JGeometry::TVec3<f32> diff = getUnkF4().getPoint();
	diff -= mPosition;

	f32 dist = diff.length();
	if (dist < 100.0f)
		return;

	f32 marchSpeed = getMyMarchSpeed();
	marchSpeed *= SMSGetAnmFrameRate();
	f32 turnSpeed = getSaveParam2()->mTurnSpeed.get();
	turnSpeed *= SMSGetAnmFrameRate();
	if (dist <= 2.0f * calcMinimumTurnRadius(marchSpeed, turnSpeed))
		turnSpeed = calcTurnSpeedToReach(marchSpeed, 0.5f * dist);

	TAnimalBase::getRotationFlyToDir(&mRotation, diff, marchSpeed, turnSpeed);

	JGeometry::TQuat4<f32> quat = SMS_Eular2Quat(mRotation);
	JGeometry::TVec3<f32> vel(0.0f, 0.0f, marchSpeed);
	quat.rotate(vel);
	vel.scale(1.0f - getWaterDamageRate());
	vel.y -= getWaterPowerY();
	mPositionDelta = vel;
}

void TAnimalBird::doWalk()
{
	mGravity = 0.15f;

	f32 torque  = getSaveParam2()->mWalkingTorqueY.get();
	mRotation.y = MsWrap(unk170 * (torque * SMSGetAnmFrameRate()) + mRotation.y,
	                     0.0f, 360.0f);

	JGeometry::TQuat4<f32> quat = SMS_Eular2Quat(mRotation);
	JGeometry::TVec3<f32> vel(0.0f, 0.0f, getSaveParam2()->mWalkingSpeed.get());
	quat.rotate(vel);
	mPositionDelta = vel;
}

bool TAnimalBird::doLanding(bool init)
{
	if (init) {
		f32 marchSpeed              = getMyMarchSpeed() * SMSGetAnmFrameRate();
		JGeometry::TQuat4<f32> quat = SMS_Eular2Quat(mRotation);
		JGeometry::TVec3<f32> vel;
		quat.rotate(JGeometry::TVec3<f32>(0.0f, 0.0f, marchSpeed), vel);
		mVelocity = JGeometry::TVec3<f32>(vel.x, 0.0f, vel.z);
	}

	bool landed = false;
	JGeometry::TVec3<f32> delta(0.0f, 0.0f, 0.0f);
	getFootGroundHeight();
	if (checkLiveFlag(LIVE_FLAG_AIRBORNE))
		delta.y = -getSaveParam2()->mLandingGravityY.get();
	else
		landed = true;

	mRotation.x = unk164.x;
	mRotation.z = unk164.z;

	f32 torque = getSaveParam2()->mLandingTorqueY.get();
	torque *= SMSGetAnmFrameRate();
	f32 diff     = MsAngleDiff(unk164.y, mRotation.y);
	f32 rotDelta = diff < -torque ? -torque : diff > torque ? torque : diff;
	mRotation.y  = MsWrap(mRotation.y + rotDelta, 0.0f, 360.0f);

	mPositionDelta = delta;

	JGeometry::TVec3<f32> fwd(0.0f, 0.0f, getVelocity().length());
	fwd.scale(getSaveParam2()->mLandingFric.get());
	SMS_Eular2Quat(mRotation).rotate(fwd);
	mVelocity = fwd;

	return landed && fabsf(rotDelta) < 0.01f;
}

void TAnimalBird::doGotoRandomNextGraphNode()
{
	goToRandomNextGraphNode();
	JGeometry::TVec3<f32> point = getUnk104().getPoint();
	point.x += 200.0f * (MsRandF() - 0.5f);
	point.y += 200.0f * (MsRandF() - 0.5f);
	point.z += 200.0f * (MsRandF() - 0.5f);
	setGoalPath(TPathNode(point));
}

void TAnimalBird::setGoalToComeback() { setGoalPath(TPathNode(unk158)); }

void TAnimalBird::setParamsOnFloating()
{
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	mGravity = 0.0f;
	unk17C   = 0;
}

void TAnimalBird::setParamsOnLanding() { }

void TAnimalBird::setBckAnm(int index)
{
	mMActor->setBckFromIndex(index);
	setCurAnmSound();
}

f32 TAnimalBird::getMyMarchSpeed() const
{
	return unk174 * getSaveParam2()->mMarchSpeed.get();
}

f32 TAnimalBird::getWaterDamageRate() const
{
	return (f32)unk178 / getSaveParam2()->mWaterproofTimerMax.get();
}

f32 TAnimalBird::getWaterPowerY() const
{
	return getSaveParam2()->mWaterPowerY.get() * getWaterDamageRate();
}

f32 TAnimalBird::getFootGroundHeight()
{
	if (unk154 != nullptr) {
		JGeometry::TVec3<f32> point;
		unk154->getPoint(&point, unk158);
		return point.y;
	}

	return gpMap->checkGround(mPosition, &mGroundPlane);
}

TAnimalBirdParams::TAnimalBirdParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mMarchSpeed, 5.0f)
    , PARAM_INIT(mTurnSpeed, 0.1f)
    , PARAM_INIT(mReturnTimer, 1800)
    , PARAM_INIT(mSearchLength, 800.0f)
    , PARAM_INIT(mSearchHeight, 600.0f)
    , PARAM_INIT(mSearchAware, 400.0f)
    , PARAM_INIT(mSearchAngle, 90.0f)
    , PARAM_INIT(mActionTimer, 100)
    , PARAM_INIT(mWaterproofTimerMax, 45)
    , PARAM_INIT(mFloatingTimerMax, 30)
    , PARAM_INIT(mLandingGravityY, 1.0f)
    , PARAM_INIT(mLandingTorqueY, 2.0f)
    , PARAM_INIT(mWalkingTorqueY, 0.5f)
    , PARAM_INIT(mWalkingSpeed, 2.0f)
    , PARAM_INIT(mWalkTimer, 100)
    , PARAM_INIT(mLandingFric, 0.95f)
    , PARAM_INIT(mActionTimerAdd, 300)
    , PARAM_INIT(mWaterPowerY, 15.0f)
{
	TParams::load(mPrmPath);
}

TAnimalBirdManager::TAnimalBirdManager(const char* name)
    : TEnemyManager(name)
{
}

void TAnimalBirdManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TAnimalBirdParams("/Animal/bird.prm");
	TEnemyManager::load(stream);
}

void TAnimalBirdManager::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	MSoundSESystem::MSRandPlay::createRandPlayVec(MSD_SE_OBJ_BIRD_DOL_FLYING1,
	                                              mObjNum);
	MSoundSESystem::MSRandPlay::createRandPlayVec(MSD_SE_OBJ_BIRD_DOL_CHUN,
	                                              mObjNum);
}

void TAnimalBirdManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "bird_man.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

DEFINE_NERVE(TNerveAnimalBirdWaitOnGround, TLiveActor)
{
	TAnimalBird* self = (TAnimalBird*)spine->getBody();
	if (spine->getTime() == 0)
		self->setBckAnm(7);

	if (self->isWantToFly()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdTakeoff::theNerve());
		return true;
	}

	if (self->checkCurAnmEnd(0)) {
		if (self->isWantToAction()) {
			spine->pushAfterCurrent(
			    &TNerveAnimalBirdActionOnGround::theNerve());
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNerveAnimalBirdActionOnGround, TLiveActor)
{
	TAnimalBird* self = (TAnimalBird*)spine->getBody();
	if (spine->getTime() == 0) {
		int anm = cRandomAnims[(int)(5.0f * MsRandF())];
		if (anm == 8) {
			spine->pushAfterCurrent(&TNerveAnimalBirdWalkOnGround::theNerve());
			return true;
		}

		MActor* mactor = self->mMActor;
		if (!mactor->checkCurBckFromIndex(anm))
			mactor->setBckFromIndex(anm);
	}

	if (self->isWantToFly()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdTakeoff::theNerve());
		return true;
	}

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveAnimalBirdWaitOnGround::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveAnimalBirdWalkOnGround, TLiveActor)
{
	TAnimalBird* self = (TAnimalBird*)spine->getBody();
	if (spine->getTime() == 0) {
		self->unk170 *= -1.0f;
		self->setBckAnm(8);
	}

	if (self->isWantToFly()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdTakeoff::theNerve());
		return true;
	}

	self->doWalk();

	if (self->getSaveParam2()->mWalkTimer.get() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdWaitOnGround::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveAnimalBirdTakeoff, TLiveActor)
{
	TAnimalBird* self = (TAnimalBird*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(5);
		self->setParamsOnFloating();
		J3DFrameCtrl* ctrl = self->mMActor->getFrameCtrl(0);
		ctrl->setRate(3.0f * ctrl->getRate());
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_BIRD_DOL_TO_FLY1,
		                                &self->mPosition, 0, nullptr, 0, 4);
	}

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveAnimalBirdGraphWander::theNerve());
		self->setParamsOnFloating();
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveAnimalBirdGraphWander, TLiveActor)
{
	TAnimalBird* self = (TAnimalBird*)spine->getBody();
	if (spine->getTime() == 0) {
		self->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
		self->unk124->reset();
		self->goToShortestNextGraphNode();
	}

	if (spine->getTime() == 0 || self->isReachedToGoal()) {
		self->doGotoRandomNextGraphNode();
		if (self->mPosition.y <= self->getUnkF4().getPoint().y)
			self->setBckAnm(1);
		else
			self->setBckAnm(3);
	}

	self->checkCurAnmEnd(0);

	if (self->getSaveParam2()->mReturnTimer.get() < self->mSpine->getTime()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdComeback::theNerve());
		return true;
	}

	self->doFlyToCurPathNode();
	return false;
}

DEFINE_NERVE(TNerveAnimalBirdChangeToCoin, TLiveActor)
{
	TAnimalBird* self = (TAnimalBird*)spine->getBody();
	if (spine->getTime() == 0)
		self->doDropCoin();

	return true;
}

DEFINE_NERVE(TNerveAnimalBirdComeback, TLiveActor)
{
	TAnimalBird* self = (TAnimalBird*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setGoalToComeback();
		self->setBckAnm(3);
	}

	self->doFlyToCurPathNode();

	if (self->isFindMario()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdGraphWander::theNerve());
		return true;
	}

	if (self->isReachedToGoal()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdPreLanding::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveAnimalBirdPreLanding, TLiveActor)
{
	TAnimalBird* self = (TAnimalBird*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(1);
		J3DFrameCtrl* ctrl = self->mMActor->getFrameCtrl(0);
		ctrl->setRate(1.5f * ctrl->getRate());
		self->doLanding(true);
	}

	if (self->isFindMario()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdGraphWander::theNerve());
		return true;
	}

	if (self->doLanding(false)) {
		spine->pushAfterCurrent(&TNerveAnimalBirdLanding::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveAnimalBirdLanding, TLiveActor)
{
	TAnimalBird* self  = (TAnimalBird*)spine->getBody();
	J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(0);
	if (spine->getTime() == 0) {
		self->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
		self->setBckAnm(5);
		ctrl->setAttribute(J3DFrameCtrl::ATTR_ONCE_AND_RESET);
		ctrl->setFrame(ctrl->getEnd());
		ctrl->setRate(-1.0f * ctrl->getRate());
	}

	if (self->isFindMario()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdGraphWander::theNerve());
		return true;
	}

	if (ctrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE)) {
		spine->pushAfterCurrent(&TNerveAnimalBirdWaitOnGround::theNerve());
		return true;
	}

	return false;
}
