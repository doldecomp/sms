#include <Animal/BirdNerve.hpp>
#include <Animal/AnimalBase.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/WireBinder.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <JSystem/JGeometry/JGQuat4.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Map/Map.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <System/Application.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// The .bas table: only fly/open/start/stop are named, the rest of the model's
// .bck slots have no ambient sound.
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

// The animations the ActionOnGround nerve picks between; index 8 (walk) is
// special-cased into the WalkOnGround nerve instead of being played here.
const int cRandomAnims[] = { 7, 4, 0, 2, 8 };

const char* const cMatName = "_mat_body1";

// Body tint per mColorIndex: blue coin, yellow coin (the default), shine and
// red coin.
const GXColorS10 cColorTable[] = {
	{ 0, 100, 255, 0 },
	{ 0, 200, 0, 0 },
	{ 255, 200, 0, 0 },
	{ 255, 0, 0, 0 },
};

} // namespace


TAnimalBird::TAnimalBird(const char* name)
    : TSpineEnemy(name)
{
	mItem       = nullptr;
	mWireBinder = nullptr;
}

void TAnimalBird::init(TLiveManager* live_manager)
{
	mManager = live_manager;
	mManager->manageActor(this);

	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("bird_man.bmd", 0);

	mSpine->initWith(&TNerveAnimalBirdWaitOnGround::theNerve());

	initParams();
	initCollision();
	initAnmSound();
}

void TAnimalBird::initTevColor(const GXColorS10* color)
{
	s32 index = getModel()->getModelData()->getMaterialName()->getIndex(
	    cMatName);
	SMS_InitPacket_OneTevColor(getModel(), index, GX_TEVREG1, color);
}

void TAnimalBird::initCollision()
{
	initHitActor(0x10000032, 0, 0, 50.0f, 50.0f, 70.0f, 80.0f);
	onHitFlag(HIT_FLAG_CANNOT_ATTACK);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mScaledBodyRadius = 35.0f;
}

void TAnimalBird::initParams()
{
	mHomePosition.x = mPosition.x;
	mHomePosition.y = mPosition.y;
	mHomePosition.z = mPosition.z;
	mHomePosition.y += 90.0f;

	mHomeRotation.x = mRotation.x;
	mHomeRotation.y = mRotation.y;
	mHomeRotation.z = mRotation.z;

	mHitPoints      = getMaxHitPoints();
	mWaterHitTimer  = 0;
	mFloatingTimer  = 0;
	mTurnDir        = 1.0f;
	offLiveFlag(LIVE_FLAG_AIRBORNE);
	mPowerRate = 1.0f - 0.1f * (MsRandF() - 0.5f);

	if (TWireBinder::isOnWire(mPosition)) {
		mWireBinder = new TWireBinder;
		mWireBinder->init(mPosition);
	}
}

void TAnimalBird::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);

	s32 eventID;
	stream.read(&eventID, 4);

	if (eventID >= 0)
		mItem = TMapObjBaseManager::newAndRegisterObjByEventID(eventID, "鳥用");
	else
		mItem = TMapObjBaseManager::newAndRegisterObjByEventID(100, "");

	switch (mItem->getActorType()) {
	default:
		mColorIndex = 1;
		break;
	case 0x20000013:
		mColorIndex = 2;
		break;
	case 0x2000000F:
		mColorIndex = 3;
		break;
	case 0x20000010:
		mColorIndex = 0;
		checkNotAppear(eventID);
		break;
	}

	initTevColor(&cColorTable[mColorIndex]);
}

// Exact since header round 18: two expansions of MSRegisterRandPlayTrans,
// whose dead 12-byte local is the whole +0x18. See its TODO in
// MSound/MSoundSE.hpp.
void TAnimalBird::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	MSRegisterRandPlayTrans(MSD_SE_OBJ_BIRD_DOL_FLYING1, &mPosition);
	MSRegisterRandPlayTrans(MSD_SE_OBJ_BIRD_DOL_CHUN, &mPosition);
}

BOOL TAnimalBird::receiveMessage(THitActor* sender, u32 message)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return FALSE;

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		SMS_EasyEmitParticle(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                     nullptr,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		gpMSound->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &sender->mPosition,
		                        0, 0.0f, 0, 0, 4);
		behaveHitWater();
		return TRUE;
	}

	if (message == HIT_MESSAGE_TAKE && mHolder == nullptr) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mHolder = (TTakeActor*)sender;
		SMS_EasyEmitParticle(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                     nullptr,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		return TRUE;
	}

	if ((message == HIT_MESSAGE_PUT || message == HIT_MESSAGE_THROWN)
	    && mHolder == (TTakeActor*)sender) {
		mHolder = nullptr;
		offHitFlag(HIT_FLAG_NO_COLLISION);
		return TRUE;
	}

	// Two more equality tests rather than a switch: the ROM compares 0xB and
	// then 0 with plain cmplwi and no pivot tree.
	if (message == HIT_MESSAGE_UNKB) {
		mHolder = nullptr;
		if (isChanged() == false) {
			mSpine->reset();
			mSpine->setNext(&TNerveAnimalBirdChangeToCoin::theNerve());
		} else {
			kill();
		}
		return TRUE;
	}

	if (message == HIT_MESSAGE_TRAMPLE) {
		if (sender->isActorType(0x1000000D)) {
			if (isChanged() == false) {
				mSpine->reset();
				mSpine->setNext(&TNerveAnimalBirdChangeToCoin::theNerve());
			} else {
				receiveMessage(this, HIT_MESSAGE_SPRAYED_BY_WATER);
			}
			return TRUE;
		}
	}

	return TSpineEnemy::receiveMessage(sender, message);
}

void TAnimalBird::calcRootMatrix()
{
	if (mHolder != nullptr) {
		MtxPtr mtx = mHolder->getTakingMtx();
		getModel()->setBaseTRMtx(mtx);
	} else {
		TSpineEnemy::calcRootMatrix();
	}

	getModel()->getBaseTRMtx()[1][3] += 35.0f;
}

void TAnimalBird::moveObject()
{
	if (mWaterHitTimer > 0)
		mWaterHitTimer -= 1;

	checkFalling();
	checkChangeToItem();
	updateSound();

	TLiveActor::moveObject();
}

// Binding level over a raw member read, worth +8 of low region in
// TAnimalBird::bind (batch 127).
static inline TWireBinder* BirdWireBinder(const TAnimalBird* p)
{
	TWireBinder* wireBinder = p->mWireBinder;
	return wireBinder;
}

void TAnimalBird::bind()
{
	if (isCheckWithWireBinder() == false)
		TLiveActor::bind();
	else
		BirdWireBinder(this)->bind(this);
}

const char** TAnimalBird::getBasNameTable() const { return bird_bastable; }

void TAnimalBird::behaveHitWater()
{
	if (mWaterHitTimer <= 0) {
		mWaterHitTimer = getSaveParams()->mWaterproofTimerMax.get();
		if (checkLiveFlag(LIVE_FLAG_AIRBORNE))
			decHitPoints();
	}
}

bool TAnimalBird::isOnGroundNerve() const
{
	// The spine goes into a local: the ROM keeps it in one register across
	// the three theNerve() calls instead of re-reading the member.
	TSpineBase<TLiveActor>* spine = mSpine;
	return spine->getLatestNerve()
	        == &TNerveAnimalBirdWaitOnGround::theNerve()
	    || spine->getLatestNerve()
	        == &TNerveAnimalBirdActionOnGround::theNerve()
	    || spine->getLatestNerve()
	        == &TNerveAnimalBirdWalkOnGround::theNerve();
}

void TAnimalBird::checkFalling()
{
	if (isOnGroundNerve()) {
		if (checkLiveFlag(LIVE_FLAG_AIRBORNE)) {
			if (getSaveParams()->mFloatingTimerMax.get()
			    < ++mFloatingTimer) {
				mSpine->reset();
				mSpine->setNext(&TNerveAnimalBirdTakeoff::theNerve());
			}
		} else {
			mFloatingTimer = 0;
		}
	}
}

void TAnimalBird::checkChangeToItem()
{
	if (isChangeToItem()) {
		mSpine->reset();
		mSpine->setNext(&TNerveAnimalBirdChangeToCoin::theNerve());
	}
}

void TAnimalBird::checkNotAppear(s32 event_id)
{
	if (TFlagManager::getInstance()->getBlueCoinFlag(
	        SMSGetMarDirector()->getCurrentMap(), event_id))
		onLiveFlag(LIVE_FLAG_DEAD);
}

void TAnimalBird::updateSound()
{
	if (isFlying())
		gpMSound->startSeRandPlay(MSD_SE_OBJ_BIRD_DOL_FLYING1,
		                          mInstanceIndex);

	if (isOnGroundNerve())
		gpMSound->startSeRandPlay(MSD_SE_OBJ_BIRD_DOL_CHUN, mInstanceIndex);
}

bool TAnimalBird::isWantToFly() const
{
	return (mWaterHitTimer > 0 || isFindMario()) && MsRandF() < 0.5f;
}

bool TAnimalBird::isWantToAction() const
{
	s32 over = mSpine->getTime() - getSaveParams()->mActionTimer.get();
	if (over < 0)
		return false;

	f32 chance = (f32)over / (f32)getSaveParams()->mActionTimerAdd.get();
	return MsRandF() < chance;
}

bool TAnimalBird::isWantToRest() const
{
	return getSaveParams()->mWalkTimer.get() < mSpine->getTime();
}

bool TAnimalBird::isFindMario() const
{
	if (getSaveParams()->mSearchHeight.get()
	    < fabsf(SMS_GetMarioPos().y - mPosition.y))
		return false;

	return isInSight(SMS_GetMarioPos(),
	                 mPowerRate * getSaveParams()->mSearchLength.get(),
	                 mPowerRate * getSaveParams()->mSearchAngle.get(),
	                 mPowerRate * getSaveParams()->mSearchAware.get());
}

// TODO: incorrect size. Map records 152 bytes; the early-return spelling is
// 24 bytes bigger still and costs moveObject two points.
bool TAnimalBird::isChangeToItem() const
{
	return !isChanged() && getHitPoints() == 0;
}

bool TAnimalBird::isGroundShaken() const { return false; }

bool TAnimalBird::isCheckWithWireBinder() const
{
	return mWireBinder != nullptr
	    && (isOnGroundNerve()
	        || mSpine->getLatestNerve()
	            == &TNerveAnimalBirdPreLanding::theNerve());
}

// TU-local, name unknown: one inline level between isChanged() and theNerve()
// is what makes retail emit `li r3, instance$; bl TNerveBase<TLiveActor>()`
// inside both of receiveMessage's inlined isChanged() guards while the two
// `setNext(&theNerve())` arguments two statements later still expand the base
// constructor (codegen-tells.md, "Research batch 251": the guard expands at
// theNerve level 1 and calls the base ctor at level 2, and an argument is
// evaluated at the caller's level). receiveMessage 96.9 -> 100.0 and
// isChanged's own out-of-line body stays at the map's 0x90, which pins the
// level to the inside of isChanged rather than the call site.
// TODO: the level overshoots the two UNUSED sizes that measured it --
// checkChangeToItem is now 0xec against the map's 0x110 (theNerve itself goes
// out of line at level 3 there) and isChangeToItem 0xa8 against 0x98 -- so the
// real spelling is probably a named TAnimalBird predicate, not a free helper.
// It also reshuffles the weak DEFINE_NERVE bodies (validate-symbol-order's
// compiler-controlled warning).
static inline const TNerveBase<TLiveActor>* BirdChangeToCoinNerve()
{
	return &TNerveAnimalBirdChangeToCoin::theNerve();
}

// Binding level over mItem: +8 of low region per expansion, the same
// pointer-member form that closed TAnimalBird::bind.
static inline TMapObjBase* BirdItem(const TAnimalBird* p)
{
	TMapObjBase* item = p->mItem;
	return item;
}

bool TAnimalBird::isChanged() const
{
	return mSpine->getLatestNerve() == BirdChangeToCoinNerve();
}

bool TAnimalBird::isFlying() const
{
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveAnimalBirdGraphWander::theNerve()
	    || nerve == &TNerveAnimalBirdComeback::theNerve();
}

void TAnimalBird::doDropCoin()
{
	TMapObjBase* item = BirdItem(this);
	u32 type          = item->getActorType();
	if (type == 0x20000013 ? true : false) {
		item->JSGSetTranslation(mPosition);
		((TShine*)mItem)->appearWithDemo("鳥シャインカメラ");
	} else {
		TMapObjBase* obj;
		if (type == 0x2000000E ? true : false)
			obj = gpItemManager->makeObjAppear(0x2000000E);
		else
			obj = item;

		if (obj != nullptr) {
			obj->appear();
			obj->JSGSetTranslation(mPosition);
			obj->mVelocity.set(0.0f, -10.0f, 0.0f);
			obj->offLiveFlag(LIVE_FLAG_UNK10);
			obj->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}
	}
}

// TODO: 95.3%. Instruction-identical except the quaternion temporary's
// register numbering inside the inlined rotate; frame exact.
void TAnimalBird::doFlyToCurPathNode()
{
	JGeometry::TVec3<f32> toGoal = getUnkF4().getPoint();
	toGoal.sub(mPosition);

	f32 distance = toGoal.length();
	if (distance < 100.0f)
		return;

	f32 marchSpeed = getMyMarchSpeed() * SMSGetAnmFrameRate();
	f32 turnRate  = getSaveParams()->mTurnSpeed.get();
	f32 turnSpeed = turnRate * SMSGetAnmFrameRate();

	if (distance <= 2.0f * calcMinimumTurnRadius(marchSpeed, turnSpeed))
		turnSpeed = calcTurnSpeedToReach(marchSpeed, 0.5f * distance);

	TAnimalBase::getRotationFlyToDir(&mRotation, toGoal, marchSpeed,
	                                 turnSpeed);

	JGeometry::TQuat4<f32> quat = SMS_Eular2Quat(mRotation);
	JGeometry::TVec3<f32> velocity(0.0f, 0.0f, marchSpeed);
	quat.rotate(velocity, velocity);
	velocity.scale(1.0f - getWaterDamageRate());
	velocity.y -= getWaterPowerY();
	mLinearVelocity = velocity;
}

void TAnimalBird::doWalk()
{
	mGravity = 0.15f;

	f32 torque  = getSaveParams()->mWalkingTorqueY.get();
	mRotation.y = MsWrap(mTurnDir * (torque * SMSGetAnmFrameRate())
	                         + mRotation.y,
	                     0.0f, 360.0f);

	JGeometry::TQuat4<f32> quat = SMS_Eular2Quat(mRotation);
	JGeometry::TVec3<f32> velocity(0.0f, 0.0f,
	                               getSaveParams()->mWalkingSpeed.get());
	quat.rotate(velocity, velocity);
	mLinearVelocity = velocity;
}

// TODO: 85.8%. Two residuals. (1) Retail never computes the rotated y of the
// take-off velocity: with `velocity.y = 0.0f` right after the rotate MWCC is
// expected to kill the dead store and its arithmetic, and ours keeps four
// fmadds plus the frame they need (0x170 vs 0x158). (2) `velocity.length()`
// expands TUtil<f32>::sqrt here while retail calls it -- the same per-call-site
// inconsistency the catalog records for MapObjBall and amiNoko; a named speed
// local changes nothing.
bool TAnimalBird::doLanding(bool takeoff)
{
	if (takeoff) {
		f32 marchSpeed = getMyMarchSpeed() * SMSGetAnmFrameRate();
		JGeometry::TQuat4<f32> quat = SMS_Eular2Quat(mRotation);
		JGeometry::TVec3<f32> dir(0.0f, 0.0f, marchSpeed);
		JGeometry::TVec3<f32> velocity;
		quat.rotate(dir, velocity);
		velocity.y = 0.0f;
		mVelocity  = velocity;
	}

	bool landed = false;

	JGeometry::TVec3<f32> acceleration;
	acceleration.zero();

	getFootGroundHeight();

	if (checkLiveFlag(LIVE_FLAG_AIRBORNE))
		acceleration.y = -getSaveParams()->mLandingGravityY.get();
	else
		landed = true;

	mRotation.x = mHomeRotation.x;
	mRotation.z = mHomeRotation.z;

	f32 torqueRate = getSaveParams()->mLandingTorqueY.get();
	f32 torque     = torqueRate * SMSGetAnmFrameRate();
	f32 turn       = JGeometry::TUtil<f32>::clamp(
	    MsAngleDiff(mHomeRotation.y, mRotation.y), -torque, torque);
	mRotation.y = MsWrap(mRotation.y + turn, 0.0f, 360.0f);

	mLinearVelocity = acceleration;

	JGeometry::TVec3<f32> velocity = mVelocity;
	JGeometry::TVec3<f32> forward(0.0f, 0.0f, velocity.length());
	forward.scale(getSaveParams()->mLandingFric.get());
	SMS_Eular2Quat(mRotation).rotate(forward, forward);
	mVelocity = forward;

	return landed && fabsf(turn) < 0.01f;
}

// TODO: incorrect size. Map records 388 bytes.
void TAnimalBird::doGotoRandomNextGraphNode()
{
	goToRandomNextGraphNode();

	JGeometry::TVec3<f32> goal = getUnk104().getPoint();
	goal.x += 200.0f * (MsRandF() - 0.5f);
	goal.y += 200.0f * (MsRandF() - 0.5f);
	goal.z += 200.0f * (MsRandF() - 0.5f);

	setGoalPath(TPathNode(goal));
}

void TAnimalBird::setGoalToComeback() { setGoalPath(TPathNode(mHomePosition)); }

void TAnimalBird::setParamsOnFloating()
{
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	mGravity       = 0.0f;
	mFloatingTimer = 0;
}

void TAnimalBird::setParamsOnLanding() { }

void TAnimalBird::setBckAnm(int index)
{
	getMActor()->setBckFromIndex(index);
	setCurAnmSound();
}

f32 TAnimalBird::getMyMarchSpeed() const
{
	return mPowerRate * getSaveParams()->mMarchSpeed.get();
}

f32 TAnimalBird::getWaterDamageRate() const
{
	return (f32)mWaterHitTimer
	    / (f32)getSaveParams()->mWaterproofTimerMax.get();
}

f32 TAnimalBird::getWaterPowerY() const
{
	return getSaveParams()->mWaterPowerY.get() * getWaterDamageRate();
}

f32 TAnimalBird::getFootGroundHeight()
{
	JGeometry::TVec3<f32> point;

	if (mWireBinder != nullptr) {
		mWireBinder->getPoint(&point, mHomePosition);
		return point.y;
	}

	return gpMap->checkGround(mPosition, &mGroundPlane);
}

TAnimalBirdParams::TAnimalBirdParams(const char* prm)
    : TSpineEnemyParams(prm)
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

// Exact since header round 18: two expansions of MSCreateRandPlayVec give
// 0x30 and getObjNum() over the raw mObjNum the last 8 (lever pair, the same
// shape TAnimalBase::loadAfter needs). See MSound/MSoundSE.hpp.
void TAnimalBirdManager::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	MSCreateRandPlayVec(MSD_SE_OBJ_BIRD_DOL_FLYING1, getObjNum());
	MSCreateRandPlayVec(MSD_SE_OBJ_BIRD_DOL_CHUN, getObjNum());
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
	TAnimalBird* bird = (TAnimalBird*)spine->getBody();

	if (spine->getTime() == 0)
		bird->setBckAnm(TAnimalBird::BIRD_ANM_WAIT);

	if (bird->isWantToFly()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdTakeoff::theNerve());
		return TRUE;
	}

	if (bird->checkCurAnmEnd(0)) {
		if (bird->isWantToAction()) {
			spine->pushAfterCurrent(
			    &TNerveAnimalBirdActionOnGround::theNerve());
			return TRUE;
		}
	}

	return FALSE;
}

DEFINE_NERVE(TNerveAnimalBirdActionOnGround, TLiveActor)
{
	TAnimalBird* bird = (TAnimalBird*)spine->getBody();

	if (spine->getTime() == 0) {
		int anm = cRandomAnims[(int)(5.0f * MsRandF())];
		if (anm == TAnimalBird::BIRD_ANM_WALK) {
			spine->pushAfterCurrent(
			    &TNerveAnimalBirdWalkOnGround::theNerve());
			return TRUE;
		}

		MActor* actor = bird->getMActor();
		if (!actor->checkCurBckFromIndex(anm))
			actor->setBckFromIndex(anm);
	}

	if (bird->isWantToFly()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdTakeoff::theNerve());
		return TRUE;
	}

	if (bird->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveAnimalBirdWaitOnGround::theNerve());
		return TRUE;
	}

	return FALSE;
}

// TODO: 75.9%. Retail's inlined TQuat4::rotate keeps its TQuat4 temporary in
// memory and calls both TVec4<f32>::TVec4() and the TU-local
// set<f>__Q29JGeometry8TVec3<f>Ffff; that local instantiation is the open
// JGVec3.hpp problem in docs/catalog (our in-class member template always
// expands), so the quaternion temporary is scalarised here and the whole
// expansion renumbers.
//
// Both of this TU's validate-symbol-order failures are that one fact. A local
// template instantiation is emitted immediately after the first function in
// *emission* order that needs its out-of-line body, so the map's
// `execute__WalkOnGround, set<f>, MsWrap<f>, theNerve__WalkOnGround` says this
// function calls both; ours inlines both, MsWrap's first real caller is
// doLanding thirty symbols later, and set<f> is never called at all -- hence
// one MISSING and one ORDER error from a single missing refusal. The same
// three inlines (MsWrap, TVec3::set<f>, TVec4::TVec4) flip together in
// TAnimalBase::execWalk, so it is the caller-size family, not a spelling
// here.
DEFINE_NERVE(TNerveAnimalBirdWalkOnGround, TLiveActor)
{
	TAnimalBird* bird = (TAnimalBird*)spine->getBody();

	if (spine->getTime() == 0) {
		bird->mTurnDir *= -1.0f;
		bird->setBckAnm(TAnimalBird::BIRD_ANM_WALK);
	}

	if (bird->isWantToFly()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdTakeoff::theNerve());
		return TRUE;
	}

	// Retail calls doWalk() rather than spelling its body out here: at depth 2
	// MsWrap<f32> refuses to inline, which is exactly the `bl MsWrap<float>`
	// the ROM has (a pasted body puts it at depth 1, where it expands into two
	// wrap loops). TQuat4::rotate still goes out of line for us where retail
	// expands it -- that is JGQuat4.hpp's known-open "inlined direction" local
	// set, not this call site.
	bird->doWalk();

	if (bird->isWantToRest()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdWaitOnGround::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveAnimalBirdTakeoff, TLiveActor)
{
	TAnimalBird* bird = (TAnimalBird*)spine->getBody();

	if (spine->getTime() == 0) {
		bird->setBckAnm(TAnimalBird::BIRD_ANM_START);
		bird->setParamsOnFloating();
		J3DFrameCtrl* ctrl = bird->getMActor()->getFrameCtrl(0);
		ctrl->setRate(3.0f * ctrl->getRate());
		gpMSound->startSoundActor(MSD_SE_OBJ_BIRD_DOL_TO_FLY1, &bird->mPosition);
	}

	if (bird->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveAnimalBirdGraphWander::theNerve());
		bird->setParamsOnFloating();
		return TRUE;
	}

	return FALSE;
}

// TODO: 99.8%. Instruction-identical except the frame is 0x18 too large
// (0xb8 vs 0xa0). bird->mSpine (not the spine parameter) is the return-timer
// compare; getSpine() adds another +8. The extra 0x10 was already there from
// the inlined doGotoRandomNextGraphNode TPathNode temps.
DEFINE_NERVE(TNerveAnimalBirdGraphWander, TLiveActor)
{
	TAnimalBird* bird = (TAnimalBird*)spine->getBody();

	if (spine->getTime() == 0) {
		bird->setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
		bird->getTracer()->reset();
		bird->goToShortestNextGraphNode();
	}

	if (spine->getTime() == 0 || bird->isReachedToGoal()) {
		bird->doGotoRandomNextGraphNode();

		if (bird->mPosition.y <= bird->getUnkF4().getPoint().y)
			bird->setBckAnm(TAnimalBird::BIRD_ANM_STOP);
		else
			bird->setBckAnm(TAnimalBird::BIRD_ANM_FLY);
	}

	bird->checkCurAnmEnd(0);

	if (bird->getSaveParams()->mReturnTimer.get()
	    < bird->mSpine->getTime()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdComeback::theNerve());
		return TRUE;
	}

	bird->doFlyToCurPathNode();
	return FALSE;
}

// TODO: 98.7%. Frame exact. Residue is only r30/r31 ranking (retail ranks
// the else-arm obj above bird). Named getActorType() plus BirdItem binder
// closed the type-reload and the 0x10 under-frame.
DEFINE_NERVE(TNerveAnimalBirdChangeToCoin, TLiveActor)
{
	TAnimalBird* bird = (TAnimalBird*)spine->getBody();

	if (spine->getTime() == 0) {
		bird->onLiveFlag(LIVE_FLAG_DEAD);

		TMapObjBase* item = BirdItem(bird);
		u32 type          = item->getActorType();
		if (type == 0x20000013 ? true : false) {
			item->JSGSetTranslation(bird->mPosition);
			((TShine*)bird->mItem)->appearWithDemo("鳥シャインカメラ");
		} else {
			TMapObjBase* obj;
			if (type == 0x2000000E ? true : false)
				obj = gpItemManager->makeObjAppear(0x2000000E);
			else
				obj = item;

			if (obj != nullptr) {
				obj->appear();
				obj->JSGSetTranslation(bird->mPosition);
				obj->mVelocity.set(0.0f, -10.0f, 0.0f);
				obj->offLiveFlag(LIVE_FLAG_UNK10);
				obj->onLiveFlag(LIVE_FLAG_AIRBORNE);
			}
		}
	}

	return TRUE;
}

DEFINE_NERVE(TNerveAnimalBirdComeback, TLiveActor)
{
	TAnimalBird* bird = (TAnimalBird*)spine->getBody();

	if (spine->getTime() == 0) {
		bird->setGoalToComeback();
		bird->setBckAnm(TAnimalBird::BIRD_ANM_FLY);
	}

	bird->doFlyToCurPathNode();

	if (bird->isFindMario()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdGraphWander::theNerve());
		return TRUE;
	}

	if (bird->isReachedToGoal()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdPreLanding::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveAnimalBirdPreLanding, TLiveActor)
{
	TAnimalBird* bird = (TAnimalBird*)spine->getBody();

	if (spine->getTime() == 0) {
		bird->setBckAnm(TAnimalBird::BIRD_ANM_STOP);
		J3DFrameCtrl* ctrl = bird->getMActor()->getFrameCtrl(0);
		ctrl->setRate(1.5f * ctrl->getRate());
		bird->doLanding(true);
	}

	if (bird->isFindMario()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdGraphWander::theNerve());
		return TRUE;
	}

	if (bird->doLanding(false)) {
		spine->pushAfterCurrent(&TNerveAnimalBirdLanding::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveAnimalBirdLanding, TLiveActor)
{
	TAnimalBird* bird = (TAnimalBird*)spine->getBody();

	J3DFrameCtrl* ctrl = bird->getMActor()->getFrameCtrl(0);

	if (spine->getTime() == 0) {
		bird->setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
		bird->setBckAnm(TAnimalBird::BIRD_ANM_START);
		ctrl->setAttribute(J3DFrameCtrl::ATTR_ONCE_AND_RESET);
		ctrl->setFrame(ctrl->getEnd());
		ctrl->setRate(-1.0f * ctrl->getRate());
	}

	if (bird->isFindMario()) {
		spine->pushAfterCurrent(&TNerveAnimalBirdGraphWander::theNerve());
		return TRUE;
	}

	if (ctrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE)) {
		spine->pushAfterCurrent(&TNerveAnimalBirdWaitOnGround::theNerve());
		return TRUE;
	}

	return FALSE;
}
