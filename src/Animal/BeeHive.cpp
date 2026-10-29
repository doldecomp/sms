#include <Animal/BeeHive.hpp>
#include <Animal/boid.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <System/Application.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

namespace {

const f32 cAngleLimit = 0.375f * JGeometry::TUtil<f32>::PI();

bool SMS_IsMarioInWater()
{
	return SMS_CheckMarioFlag(MARIO_FLAG_VISIBLE)
	       || SMS_CheckMarioFlag(MARIO_FLAG_IN_SHALLOW_WATER)
	       || SMS_GetMarioGroundPlane()->isWaterSurface()
	       || SMS_CheckMarioFlag(MARIO_FLAG_IN_WATER);
}

void SMS_EmitWaterHitParticleAndSound(JGeometry::TVec3<f32>* pos)
{
	TPosition3f mtx;
	mtx.translation(pos->x, pos->y, pos->z);
	gpMarioParticleManager->emitAndBindToMtx(PARTICLE_MS_ENM_WATHIT, mtx, 0,
	                                         nullptr);
	SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, pos, 0, 0.0f, 0, 0,
	                              4);
}

} // namespace

TBeeHiveParams::TBeeHiveParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mGiveupTimer, 600)
    , PARAM_INIT(mGiveupRange, 1750.0f)
    , PARAM_INIT(mDecrimentTimer, 60)
    , PARAM_INIT(mRebound, 0.007f)
    , PARAM_INIT(mDecay, 0.98f)
    , PARAM_INIT(mAngleMaxAdd, 0.08f)
    , PARAM_INIT(mShakePower, 0.003f)
    , PARAM_INIT(mFallAngularVel, 0.01f)
    , PARAM_INIT(mSearchRange, 800.0f)
{
	TParams::load(mPrmPath);
}

TBee::TBee(MActor* actor, TBeeHive* hive)
    : TRealoidActor(actor)
{
	mHive = hive;
}

void TBee::init()
{
	initHitActor(ACTOR_TYPE_ENEMY_UNK2F, 1, HIT_CATEGORY_PLAYER, 20.0f, 20.0f,
	             50.0f, 50.0f);

	TIdxGroupObj* enemyGroup
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));
	enemyGroup->getChildren().push_back(this);

	onHitFilter(HIT_FILTER_NO_COLLISION);
}

BOOL TBee::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_TAKE:
		if (mHolder == nullptr) {
			mHolder = (TTakeActor*)sender;
			SMS_EasyEmitParticle(PARTICLE_MS_ENM_WATHIT, &mPosition, nullptr,
			                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
			return TRUE;
		}
		break;
	case HIT_MESSAGE_DETACH:
		if (mHolder != nullptr) {
			mHolder = nullptr;
			return TRUE;
		}
		break;
	case HIT_MESSAGE_UNKB:
		behaveToEat();
		return TRUE;
	}

	return FALSE;
}

void TBee::behaveToEat() { mHive->receiveMessageFromChild(this); }

TBeeHive::TBeeHive(const char* name)
    : TRealoid(name)
{
	mNestItem = nullptr;
}

void TBeeHive::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	onLiveFlag(LIVE_FLAG_UNK8 | LIVE_FLAG_UNK10);
	mSpine->initWith(&TNerveBeeHiveWait::theNerve());
	mInitialPosition = mPosition;
	mActiveBeeNum    = 0;
	mEatenBeeNum     = 0;
	initCollision();
}

void TBeeHive::reset()
{
	mPosition = mInitialPosition;

	TPosition3f mtx;
	MsMtxSetRotRPH(mtx, mRotation.x, 0.0f, mRotation.z);
	mtx.getQuat(mBaseRot);

	mSwingRot.setEulerY(mRotation.y);
	mSwingTargetRot = mSwingRot;

	mSwingAngle     = 0.0f;
	mSwingAngVel    = 0.0f;
	mSwingAngleMax  = 0.015707964f;
	mHitCheckBeeIdx = 0;
	onLiveFlag(LIVE_FLAG_UNK10 | LIVE_FLAG_AIRBORNE);
	mMActor = mMActorKeeper->getMActor("bee_nest.bmd");
	offHitFilter(HIT_FILTER_NO_COLLISION);
}

void TBeeHive::initCollision()
{
	initHitActor(ACTOR_TYPE_ENEMY_UNK31, 0, HIT_CATEGORY_PLAYER, 50.0f, 50.0f,
	             100.0f, 100.0f);
	onHitFilter(HIT_FILTER_NO_ATTACK);
}

void TBeeHive::initBoids()
{
	for (int i = 0; i < unk150->mNumBoids; ++i) {
		TRealoidActor* bee = getRealoid(i);
		bee->init();
		bee->onFlag(TRealoidActor::FLAG_UNK2);
	}
}

void TBeeHive::setBoidBaseParams()
{
	unk150->mBaseSpeed         = 25.0f;
	unk150->mNeighborRadius    = 80.0f;
	unk150->mYawSpeed          = 8.0f;
	unk150->mPitchSpeed        = 8.0f;
	unk150->mMaxPitch          = 85.0f;
	unk150->mAlignmentStrength = 0.001f;
}

void TBeeHive::setBoidParamOnAttacking()
{
	setBoidBaseParams();
	unk150->setGoal((THitActor*)gpMarioAddress);
	unk150->mGoalOffset = JGeometry::TVec3<f32>(0.0f, 200.0f, 0.0f);
}

void TBeeHive::setBoidParamOnWaiting()
{
	setBoidBaseParams();
	unk150->setUnk38(mPosition);
	unk150->mGoalOffset = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
}

void TBeeHive::setBoidParamOnMarioWaterIn()
{
	setBoidBaseParams();
	unk150->setGoal((THitActor*)gpMarioAddress);
	unk150->mGoalOffset = JGeometry::TVec3<f32>(0.0f, 500.0f, 0.0f);
}

void TBeeHive::receiveMessageFromChild(TBee* bee)
{
	if (bee->checkFlag(TRealoidActor::FLAG_UNK4))
		return;

	bee->onFlag(TRealoidActor::FLAG_UNK4);
	bee->onHitFilter(HIT_FILTER_NO_COLLISION);

	TMapObjBase* obj = mBeeCoins[mEatenBeeNum];
	if (mEatenBeeNum != unk150->getBoidNum() - 1)
		obj = gpItemManager->makeObjAppear(ACTOR_TYPE_COIN);

	if (obj != nullptr) {
		obj->appear();
		obj->mPosition = bee->mPosition;
		obj->setVelocityAndFlag10(0.0f, 15.0f, 0.0f);
	}

	++mEatenBeeNum;
}

void TBeeHive::load(JSUMemoryInputStream& stream)
{
	loadDefault(stream, "bee_body.bmd", 2);
	loadCoin(stream);
	mMActorKeeper->createMActor("bee_nest_break.bmd", 3);
	mMActor = mMActorKeeper->createMActor("bee_nest.bmd", 3);
	initBoids();
	reset();
}

void TBeeHive::loadCoin(JSUMemoryInputStream& stream)
{
	u32 itemEventId;
	u32 coinEventId;
	stream >> itemEventId;
	stream >> coinEventId;

	mNestItem = TMapObjBaseManager::newAndRegisterObjByEventID(itemEventId, "");

	mBeeCoins             = new TMapObjBase*[unk150->mNumBoids];
	TMapObjBase** it      = mBeeCoins;
	TMapObjBase** lastOne = mBeeCoins + unk150->mNumBoids;
	lastOne--;
	for (; it != lastOne; ++it)
		*it = TMapObjBaseManager::newAndRegisterObj("coin");

	*it = TMapObjBaseManager::newAndRegisterObjByEventID(coinEventId, "");
	if (*it == nullptr)
		*it = TMapObjBaseManager::newAndRegisterObj("coin");
}

TRealoidActor* TBeeHive::createRealoidActor(MActor* actor)
{
	return new TBee(actor, this);
}

BOOL TBeeHive::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER: {
		JGeometry::TVec3<f32> dir = mPosition;
		dir -= SMS_GetMarioPos();
		setShakePower(dir);
		SMS_EmitWaterHitParticleAndSound(&sender->mPosition);
		return TRUE;
	}
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
	case HIT_MESSAGE_PUNCH:
		if (isWaiting()) {
			mSpine->reset();
			mSpine->setNext(&TNerveBeeHiveFall::theNerve());
		}
		return TRUE;
	}

	return FALSE;
}

void TBeeHive::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TRealoid::perform(cue, graphics);
	TSpineEnemy::perform(cue, graphics);
}

void TBeeHive::control()
{
	controlCollision();
	TLiveActor::control();
	controlSound();
}

void TBeeHive::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	JGeometry::TVec3<f32> pos = mPosition;
	pos += mPositionDelta;
	pos += mVelocity;

	mVelocity.y -= getGravityY();
	if (mVelocity.y < mVelocityMinY)
		mVelocity.y = mVelocityMinY;

	f32 offset = -60.0f * (-1.0f + MsCos(57.295776f * mSwingAngle));

	mGroundHeight = gpMap->checkGround(pos.x, offset + (pos.y + mHeadHeight),
	                                   pos.z, &mGroundPlane);
	mGroundHeight += 1.0f;
	if (pos.y + offset <= 0.05f + mGroundHeight) {
		if (mGroundPlane->checkFlag(0x10))
			kill();
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		mVelocity.set(0.0f, 0.0f, 0.0f);
		pos.y = mGroundHeight - offset;
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	gpMap->isTouchedOneWallAndMoveXZ(&pos.x, pos.y + mHeadHeight, &pos.z,
	                                 mBodyRadius);

	mPositionDelta = pos - mPosition;
}

void TBeeHive::controlCollision()
{
	int num   = mActiveBeeNum;
	int index = mHitCheckBeeIdx;

	TRealoidActor* bee = getRealoid(index);
	bee->checkHitActors();
	bee->onHitFilter(HIT_FILTER_NO_ATTACK);

	if (num <= index + 1)
		(void)num; // TODO: wtf?

	if (num <= ++mHitCheckBeeIdx)
		mHitCheckBeeIdx = 0;

	int next = mHitCheckBeeIdx;
	if (num <= next)
		next = 0;

	TRealoidActor* nextBee = getRealoid(next);
	if (!(nextBee->checkFlag(TRealoidActor::FLAG_UNK2_OR_UNK4)))
		nextBee->offHitFilter(HIT_FILTER_NO_ATTACK);
}

void TBeeHive::controlSound()
{
	if (mActiveBeeNum == 0)
		return;

	JGeometry::TVec3<f32> sum(0.0f, 0.0f, 0.0f);
	int count = 0;
	for (int i = 0; i < mActiveBeeNum; ++i) {
		TRealoidActor* bee = getRealoid(i);
		if (!(bee->checkFlag(TRealoidActor::FLAG_UNK2_OR_UNK4))) {
			++count;
			sum += bee->mPosition;
		}
	}

	if (count != 0) {
		sum.scale(1.0f / count);
		mBeeSoundPos.set(sum);
		SMSGetMSound()->startBeeSe(&mBeeSoundPos, count);
	}
}

void TBeeHive::calcRootMatrix()
{
	JGeometry::TQuat4<f32> shake;
	JGeometry::TQuat4<f32> quat = mBaseRot;
	shake.setEulerX(mSwingAngle);
	quat.mul(quat, mSwingRot);
	quat.mul(quat, shake);

	TPosition3f mtx;
	mtx.setSQT(mScaling, quat, mPosition);
	mtx.ref(1, 3) += 120.0f;
	getModel()->setBaseTRMtx(mtx);
}

void TBeeHive::prepareWait()
{
	onLiveFlag(LIVE_FLAG_UNK10 | LIVE_FLAG_AIRBORNE);
	if (mActiveBeeNum < 3) {
		for (int i = 0; i < 3; ++i)
			appearBee(i);
		mActiveBeeNum = 3;
	}
}

bool TBeeHive::doWait()
{
	f32 prevVel = mSwingAngVel;
	f32 force   = mSwingAngle * -getSaveParam2()->mRebound.get();
	mSwingAngVel += force;
	mSwingAngVel *= getSaveParam2()->mDecay.get();
	mSwingAngle += mSwingAngVel;

	if (mSwingAngle < -mSwingAngleMax || mSwingAngleMax < mSwingAngle) {
		mSwingAngleMax += getSaveParam2()->mAngleMaxAdd.get();
		mSwingAngleMax = JGeometry::TUtil<f32>::clamp(
		    mSwingAngleMax, 0.0f, JGeometry::TUtil<f32>::halfPI());
		mSwingAngle = JGeometry::TUtil<f32>::clamp(mSwingAngle, -mSwingAngleMax,
		                                           mSwingAngleMax);
		mSwingAngVel = 0.0f;
		SMSGetMSound()->startSoundActorWithInfo(
		    MSD_SE_EN_BEENEST_SWING, &mPosition, nullptr, fabsf(mSwingAngle), 0,
		    0, nullptr, 0, 4);
	}

	if (!JGeometry::TUtil<f32>::epsilonEquals(0.0f, mSwingAngVel,
	                                          JGeometry::TUtil<f32>::epsilon())
	    && prevVel * mSwingAngVel <= 0.0f) {
		SMSGetMSound()->startSoundActorWithInfo(
		    MSD_SE_EN_BEENEST_SWING, &mPosition, nullptr, fabsf(mSwingAngle), 0,
		    0, nullptr, 0, 4);
	}

	mSwingRot.slerp(mSwingTargetRot, 0.01f);
	mSwingRot.normalize();

	JGeometry::TVec3<f32> diff = SMS_GetMarioPos();
	diff -= mPosition;
	f32 range = getSaveParam2()->mSearchRange.get();
	if (diff.squared() <= range * range)
		setBoidParamOnAttacking();
	else
		setBoidParamOnWaiting();

	return cAngleLimit <= fabsf(mSwingAngle);
}

void TBeeHive::prepareFall()
{
	offLiveFlag(LIVE_FLAG_UNK10);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	SMSGetMSound()->startSoundActor(MSD_SE_EN_BEENEST_OFF, &mPosition, 0,
	                                nullptr, 0, 4);

	JGeometry::TVec3<f32> dir;
	mSwingRot.getZDir(dir);
	dir.scale(1.0f);
	mSwingAngVel = getSaveParam2()->mFallAngularVel.get();
	mVelocity    = dir;
}

bool TBeeHive::doFall()
{
	mSwingAngle += mSwingAngVel;
	return !checkLiveFlag(LIVE_FLAG_AIRBORNE);
}

void TBeeHive::appearBee(int index)
{
	TRealoidActor* bee = getRealoid(index);
	if (bee->checkFlag(TRealoidActor::FLAG_UNK4))
		return;
	if (!bee->checkFlag(TRealoidActor::FLAG_UNK2))
		return;

	bee->offFlag(TRealoidActor::FLAG_UNK2);
	bee->offHitFilter(HIT_FILTER_NO_COLLISION);
	TBoid* boid     = unk150->getBoid(index);
	boid->mPosition = mPosition;
}

void TBeeHive::disappearBee(int index)
{
	TRealoidActor* bee = getRealoid(index);
	if (!bee->checkFlag(TRealoidActor::FLAG_UNK2)) {
		bee->onFlag(TRealoidActor::FLAG_UNK2);
		bee->onHitFilter(HIT_FILTER_NO_COLLISION);
	}
}

void TBeeHive::decrimentBee()
{
	if (mActiveBeeNum > 3) {
		--mActiveBeeNum;
		disappearBee(mActiveBeeNum);
	}
}

void TBeeHive::appearAllBees()
{
	for (int i = 0; i < unk150->mNumBoids; ++i)
		appearBee(i);
	mActiveBeeNum = unk150->mNumBoids;
}

void TBeeHive::dropCoin()
{
	TMapObjBase* obj = mNestItem;
	if (obj == nullptr)
		return;

	if (obj->isActorType(ACTOR_TYPE_COIN))
		obj = gpItemManager->makeObjAppear(ACTOR_TYPE_COIN);

	if (obj != nullptr) {
		obj->appear();
		obj->JSGSetTranslation(mPosition);
		obj->setVelocityAndFlag10(0.0f, 0.0f, 0.0f);
		obj->onLiveFlag(LIVE_FLAG_AIRBORNE);
		mNestItem = nullptr;
	}
}

void TBeeHive::prepareBreak()
{
	mMActor = mMActorKeeper->getMActor("bee_nest_break.bmd");
	mMActor->setBck("bee_nest_break");
	onHitFilter(HIT_FILTER_NO_COLLISION);
	SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_A, &mPosition, this,
	                     JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));
	SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_B, &mPosition, this,
	                     JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));
	SMSGetMSound()->startSoundActor(MSD_SE_EN_BEENEST_LAND, &mPosition, 0,
	                                nullptr, 0, 4);
}

void TBeeHive::prepareVanish() { onLiveFlag(LIVE_FLAG_HIDDEN); }

void TBeeHive::prepareReset()
{
	reset();
	offLiveFlag(LIVE_FLAG_HIDDEN);
	mScaling.set(0.0f, 0.0f, 0.0f);
}

bool TBeeHive::doScaling()
{
	bool result = false;
	mScaling.x += 0.01f;
	if (1.0f <= mScaling.x) {
		mScaling.x = 1.0f;
		result     = true;
		mScaling.y = 1.0f;
		mScaling.z = 1.0f;
	} else {
		mScaling.y += 0.01f;
		mScaling.z += 0.01f;
	}
	return result;
}

// fabricated
inline int signOf(const f32& v) { return v > 0.0f ? 1 : v < 0.0f ? -1 : 0; }

bool TBeeHive::isMissMario() const
{
	if (mActiveBeeNum == 0)
		return false;

	JGeometry::TVec3<f32> diff = SMS_GetMarioPos();
	diff -= getCenterOfGravity();
	f32 range = getSaveParam2()->mGiveupRange.get();
	return range * range <= diff.squared();
}

void TBeeHive::setShakePower(const JGeometry::TVec3<f32>& dir)
{
	if (isFalling())
		return;

	JGeometry::TVec3<f32> horizontal = dir;
	horizontal.y                     = 0.0f;
	horizontal.normalize();

	f32 sign = fabsf(mSwingAngVel) < 0.0001f ? 1.0f : signOf(mSwingAngVel);

	mSwingTargetRot.setRotate(JGeometry::TVec3<f32>(0.0f, 0.0f, 1.0f),
	                          horizontal, 1.0f);
	mSwingAngVel += sign * getSaveParam2()->mShakePower.get();
}

JGeometry::TVec3<f32> TBeeHive::getCenterOfGravity() const
{
	if (mActiveBeeNum == 0)
		return JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);

	JGeometry::TVec3<f32> center(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < mActiveBeeNum; ++i)
		center += getBoidLeader()->getBoid(i)->mPosition;
	center /= mActiveBeeNum;
	return center;
}

bool TBeeHive::isFalling() const
{
	return mSpine->getLatestNerve() == &TNerveBeeHiveFall::theNerve();
}

bool TBeeHive::isWaiting() const
{
	return mSpine->getLatestNerve() == &TNerveBeeHiveWait::theNerve();
}

bool TBeeHive::isNotBreakOnceYet() const { }

TBeeHiveManager::TBeeHiveManager(const char* name)
    : TEnemyManager(name)
{
}

void TBeeHiveManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBeeHiveParams("/Animal/beehive.prm");
	TEnemyManager::load(stream);
}

void TBeeHiveManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "bee_body.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ "bee_nest.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ "bee_nest_break.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

DEFINE_NERVE(TNerveBeeHiveWait, TLiveActor)
{
	TBeeHive* self = (TBeeHive*)spine->getBody();
	if (spine->getTime() == 0) {
		self->prepareWait();
		self->setBoidParamOnWaiting();
	}

	if (self->doWait()) {
		spine->pushAfterCurrent(&TNerveBeeHiveFall::theNerve());
		return true;
	}

	if (self->getSaveParam2()->mDecrimentTimer.get() < spine->getTime()) {
		self->decrimentBee();
		spine->pushAfterCurrent(this);
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBeeHiveFall, TLiveActor)
{
	TBeeHive* self = (TBeeHive*)spine->getBody();
	if (spine->getTime() == 0) {
		self->prepareFall();
		self->dropCoin();
	}

	if (self->doFall()) {
		spine->pushAfterCurrent(&TNerveBeeHiveBreak::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBeeHiveBreak, TLiveActor)
{
	TBeeHive* self = (TBeeHive*)spine->getBody();
	if (spine->getTime() == 0) {
		self->prepareBreak();
		self->appearAllBees();
		self->setBoidParamOnAttacking();
	}

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveBeeHiveAttack::theNerve());
		self->prepareVanish();
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBeeHiveAttack, TLiveActor)
{
	TBeeHive* self = (TBeeHive*)spine->getBody();
	if (spine->getTime() == 0)
		self->setBoidParamOnAttacking();

	if (SMS_IsMarioInWater()) {
		spine->pushAfterCurrent(&TNerveBeeHiveMarioWaterIn::theNerve());
		return true;
	}

	if (self->isMissMario()) {
		spine->pushAfterCurrent(&TNerveBeeHiveReset::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBeeHiveMarioWaterIn, TLiveActor)
{
	TBeeHive* self = (TBeeHive*)spine->getBody();
	if (spine->getTime() == 0)
		self->setBoidParamOnMarioWaterIn();

	if (self->getSaveParam2()->mGiveupTimer.get() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveBeeHiveReset::theNerve());
		return true;
	}

	if (!SMS_IsMarioInWater()) {
		spine->pushAfterCurrent(&TNerveBeeHiveAttack::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBeeHiveReset, TLiveActor)
{
	TBeeHive* self = (TBeeHive*)spine->getBody();
	if (spine->getTime() == 0)
		self->prepareReset();

	if (self->doScaling()) {
		spine->pushAfterCurrent(&TNerveBeeHiveWait::theNerve());
		return true;
	}

	return false;
}
