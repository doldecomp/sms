#include <Animal/BeeHive.hpp>
#include <Animal/boid.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JMath.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
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
#include <Strategic/Strategy.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

typedef JGeometry::TPosition3<
    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >
    TBeeHiveMtx;

namespace {

// 67.5 degrees. The initialiser goes through halfPI(), so it cannot be folded
// at compile time and lands in .sbss with a store in __sinit.
f32 cAngleLimit = 0.75f * JGeometry::TUtil<f32>::halfPI();

// Mario counts as "in the water" both while swimming and while standing on a
// water surface, which is why the ground plane is consulted too.
bool SMS_IsMarioInWater()
{
	return SMS_CheckMarioFlag(MARIO_FLAG_VISIBLE)
	    || SMS_CheckMarioFlag(MARIO_FLAG_IN_SHALLOW_WATER)
	    || SMS_GetMarioGroundPlane()->isWaterSurface()
	    || SMS_CheckMarioFlag(MARIO_FLAG_IN_WATER);
}

void SMS_EmitWaterHitParticleAndSound(JGeometry::TVec3<f32>* position)
{
	JGeometry::TPosition3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >
	    mtx;
	f32 x = position->x;
	f32 y = position->y;
	f32 z = position->z;
	mtx.identity33();
	mtx.ref(0, 3) = x;
	mtx.ref(1, 3) = y;
	mtx.ref(2, 3) = z;

	gpMarioParticleManager->emitAndBindToMtx(PARTICLE_MS_ENM_WATHIT, mtx, 0,
	                                         nullptr);
	gpMSound->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, position, 0, 0.0f, 0, 0,
	                        4);
}

} // namespace

TBeeHiveParams::TBeeHiveParams(const char* prm)
    : TSpineEnemyParams(prm)
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
	mBeeHive = hive;
}

void TBee::init()
{
	initHitActor(0x1000002F, 1, -0x80000000, 20.0f, 20.0f, 50.0f, 50.0f);

	TIdxGroupObj* group
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	group->getChildren().push_back(this);

	onHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TBee::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_TAKE:
		if (getHolder() == nullptr) {
			TTakeActor* holder = (TTakeActor*)sender;
			mHolder            = holder;
			SMS_EasyEmitParticle(PARTICLE_MS_ENM_WATHIT, &mPosition, nullptr,
			                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
			return TRUE;
		}
		break;
	case HIT_MESSAGE_UNK8:
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

// TODO: incorrect size. Map records 0xfc bytes for this UNUSED symbol and no
// emitted code resembles it, so the body below is a guess: a bee that has
// reached Mario tells the hive and takes itself out of the swarm.
void TBee::behaveToEat()
{
	mBeeHive->receiveMessageFromChild(this);
}

TBeeHive::TBeeHive(const char* name)
    : TRealoid(name)
{
	mDropItem = nullptr;
}

void TBeeHive::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);

	onLiveFlag(LIVE_FLAG_UNK8 | LIVE_FLAG_UNK10);

	mSpine->initWith(&TNerveBeeHiveWait::theNerve());

	mHomePosition = mPosition;

	mBeeNum      = 0;
	mEatenBeeNum = 0;

	initCollision();
}

void TBeeHive::reset()
{
	mPosition = mHomePosition;

	JGeometry::TRotation3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >
	    rot;
	MsMtxSetRotRPH(rot, mRotation.x, 0.0f, mRotation.z);
	rot.getQuat(mBaseRotation);

	mRotation168.setEulerY(mRotation.y);
	mGoalRotation = mRotation168;

	mSwingAngle    = 0.0f;
	mSwingSpeed    = 0.0f;
	mSwingAngleMax = 0.015707964f;

	mCheckBeeIndex = 0;

	onLiveFlag(LIVE_FLAG_UNK10 | LIVE_FLAG_AIRBORNE);

	mMActor = mMActorKeeper->getMActor("bee_nest.bmd");

	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBeeHive::initCollision()
{
	initHitActor(0x10000031, 0, -0x80000000, 50.0f, 50.0f, 100.0f, 100.0f);
	onHitFlag(HIT_FLAG_CANNOT_ATTACK);
}

void TBeeHive::initBoids()
{
	for (int i = 0; i < unk150->getBoidNum(); ++i) {
		TRealoidActor* bee = unk154[i];
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

// TODO: the three setBoidParam* helpers, and every nerve that inlines them,
// are one call short: the ROM calls `JGeometry::TVec3<f>::set<f>(f,f,f)` out of
// line for the TPathNode's zero initialiser, while we store the three zeros in
// place. Writing Enemy/PathNode.hpp's `TPathNode(THitActor*)` as
// `TPathNode(THitActor* actor) : unk4(0.0f, 0.0f, 0.0f)` puts the set() one
// inline level deeper and reproduces the call (Attack 95.6 -> 98.5, Break
// 94.0 -> 97.9, MarioWaterIn 94.5 -> 98.2, doWait 91.6 -> 92.8) -- but the same
// change regresses about fifteen other units that construct TPathNodes
// (fishoid, Butterfly, gesso, hamukuri, namekuri, smallEnemy, walkerEnemy,
// bossgesso, telesa ...), so it is not a global fix. Whatever the original
// wrote, its effect is per-call-site; do not push the member initialiser into
// the shared header on this unit's evidence alone.
void TBeeHive::setBoidParamOnAttacking()
{
	setBoidBaseParams();
	unk150->mGoal       = TPathNode((THitActor*)gpMarioAddress);
	unk150->mGoalOffset = JGeometry::TVec3<f32>(0.0f, 200.0f, 0.0f);
}

void TBeeHive::setBoidParamOnWaiting()
{
	setBoidBaseParams();
	unk150->mGoal       = TPathNode(mPosition);
	unk150->mGoalOffset = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
}

void TBeeHive::setBoidParamOnMarioWaterIn()
{
	setBoidBaseParams();
	unk150->mGoal       = TPathNode((THitActor*)gpMarioAddress);
	unk150->mGoalOffset = JGeometry::TVec3<f32>(0.0f, 500.0f, 0.0f);
}

void TBeeHive::receiveMessageFromChild(TBee* child)
{
	if (child->checkFlag(TRealoidActor::FLAG_UNK4))
		return;

	child->onFlag(TRealoidActor::FLAG_UNK4);
	child->onHitFlag(HIT_FLAG_NO_COLLISION);

	TMapObjBase* coin = mCoins[mEatenBeeNum];
	if (mEatenBeeNum != unk150->getBoidNum() - 1)
		coin = gpItemManager->makeObjAppear(0x2000000E);

	if (coin != nullptr) {
		coin->appear();
		coin->mPosition = child->mPosition;
		coin->setVelocityAndFlag10(0.0f, 15.0f, 0.0f);
	}

	mEatenBeeNum += 1;
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

// The last slot of the coin array is the object named by the second event ID
// in the map data; if that ID resolves to nothing the hive drops a plain coin
// instead.
void TBeeHive::loadCoin(JSUMemoryInputStream& stream)
{
	u32 dropEventId;
	u32 lastEventId;
	stream.read(&dropEventId, 4);
	stream.read(&lastEventId, 4);

	mDropItem
	    = TMapObjBaseManager::newAndRegisterObjByEventID(dropEventId, "");

	mCoins = new TMapObjBase*[unk150->getBoidNum()];

	TMapObjBase** it  = mCoins;
	TMapObjBase** end = mCoins + unk150->getBoidNum() - 1;
	for (; it != end; ++it)
		*it = TMapObjBaseManager::newAndRegisterObj("coin");

	*it = TMapObjBaseManager::newAndRegisterObjByEventID(lastEventId, "");
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
		JGeometry::TVec3<f32> toMario = mPosition;
		toMario.sub(*gpMarioPos);

		setShakePower(toMario);

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

// The hive keeps swinging while it is still roped up, so it does its own
// gravity/ground work instead of TLiveActor::bind(): the pendulum lifts the
// hive as it swings out, and that lift has to be taken off the ground probe.
void TBeeHive::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	JGeometry::TVec3<f32> nextPos = getPosition();
	nextPos.add(mLinearVelocity);
	nextPos.add(mVelocity);

	mVelocity.y -= getGravityY();
	if (mVelocity.y < TLiveActor::mVelocityMinY)
		mVelocity.y = TLiveActor::mVelocityMinY;

	f32 swingDrop
	    = -60.0f
	      * (-1.0f
	         + JMASCos((s16)DEG2SHORTANGLE(57.295776f * mSwingAngle)));

	mGroundHeight = gpMap->checkGround(nextPos.x,
	                                   nextPos.y + mHeadHeight + swingDrop,
	                                   nextPos.z, &mGroundPlane);
	mGroundHeight += 1.0f;

	if (nextPos.y + swingDrop <= 0.05f + mGroundHeight) {
		if (mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL))
			kill();
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		mVelocity.set(0.0f, 0.0f, 0.0f);
		nextPos.y = mGroundHeight - swingDrop;
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	gpMap->isTouchedOneWallAndMoveXZ(&nextPos.x, nextPos.y + mHeadHeight,
	                                 &nextPos.z, mBodyRadius);

	mLinearVelocity = nextPos - mPosition;
}

// Only one bee is collision-checked per frame; the rest have their collision
// switched off so Mario cannot be hit by the whole swarm at once.
void TBeeHive::controlCollision()
{
	int index = mCheckBeeIndex;
	int num   = mBeeNum;

	TRealoidActor* bee = unk154[index];
	bee->checkHitActors();
	bee->onHitFlag(HIT_FLAG_CANNOT_ATTACK);

	index += 1;
	if (num <= index)
		index = 0;

	int bumped     = mCheckBeeIndex + 1;
	mCheckBeeIndex = bumped;
	if (num <= bumped)
		mCheckBeeIndex = 0;

	int next = mCheckBeeIndex;
	if (num <= next)
		next = 0;

	TRealoidActor* nextBee = unk154[next];
	if (!nextBee->checkFlag(TRealoidActor::FLAG_UNK2_OR_UNK4))
		nextBee->offHitFlag(HIT_FLAG_CANNOT_ATTACK);
}

void TBeeHive::controlSound()
{
	int num = mBeeNum;
	if (num == 0)
		return;

	JGeometry::TVec3<f32> sum(0.0f, 0.0f, 0.0f);
	int alive = 0;

	for (int i = 0; i < num; ++i) {
		TRealoidActor* bee = unk154[i];
		if (bee->checkFlag(TRealoidActor::FLAG_UNK2_OR_UNK4))
			continue;

		alive += 1;
		sum.x += bee->mPosition.x;
		sum.y += bee->mPosition.y;
		sum.z += bee->mPosition.z;
	}

	if (alive == 0)
		return;

	sum.scale(1.0f / alive);
	mBeeCenter.set(sum.x, sum.y, sum.z);

	gpMSound->startBeeSe(mBeeCenter, alive);
}

// TODO: 81.2%. Both products are the *two-argument* mul(a, b) written in
// place: the standalone `fmuls` MWCC leaves for the second source term is
// `this->w * other.x` here, and per the overload tell in
// docs/catalog/codegen-tells.md that is mul(quat, other). The copy goes
// through J3DModel::setBaseTRMtx, which is why the ROM holds &rot in r31
// across getModel(). What is left is the frame, 0xc0 against the ROM's 0xa0:
// the named block (swing, quat, rot) now sits where the ROM has it relative to
// the top, but the dead inline-temporary region below rot is 0x50 against
// 0x30. setSQ+setTrans, one-argument mul and rot.mMtx indexing are all worse.
void TBeeHive::calcRootMatrix()
{
	JGeometry::TQuat4<f32> swing;
	JGeometry::TQuat4<f32> quat = mBaseRotation;
	swing.setEulerX(mSwingAngle);

	quat.mul(quat, mRotation168);
	quat.mul(quat, swing);

	TBeeHiveMtx rot;
	rot.setSQT(mScaling, quat, mPosition);
	rot.ref(1, 3) += 120.0f;

	getModel()->setBaseTRMtx(rot);
}

void TBeeHive::prepareWait()
{
	onLiveFlag(LIVE_FLAG_UNK10 | LIVE_FLAG_AIRBORNE);

	if (mBeeNum < 3) {
		for (int i = 0; i < 3; ++i)
			appearBee(i);
		mBeeNum = 3;
	}

	setBoidParamOnWaiting();
}

// Runs the pendulum one step and returns true once the hive has swung past
// cAngleLimit, which is what snaps it off its rope.
bool TBeeHive::doWait()
{
	f32 prevSpeed = mSwingSpeed;

	f32 accel = mSwingAngle * -getSaveParams()->mRebound.get();
	mSwingSpeed = mSwingSpeed + accel;
	mSwingSpeed *= getSaveParams()->mDecay.get();
	mSwingAngle += mSwingSpeed;

	if (mSwingAngle < -mSwingAngleMax || mSwingAngleMax < mSwingAngle) {
		mSwingAngleMax += getSaveParams()->mAngleMaxAdd.get();
		mSwingAngleMax = JGeometry::TUtil<f32>::clamp(
		    mSwingAngleMax, 0.0f, JGeometry::TUtil<f32>::halfPI());
		mSwingAngle = JGeometry::TUtil<f32>::clamp(
		    mSwingAngle, -mSwingAngleMax, mSwingAngleMax);
		mSwingSpeed = 0.0f;

		gpMSound->startSoundActorWithInfo(MSD_SE_EN_BEENEST_SWING, &mPosition,
		                                  nullptr, fabsf(mSwingAngle), 0, 0,
		                                  nullptr, 0, 4);
	}

	// The swing just crossed its turning point: creak once per half period.
	if (!JGeometry::TUtil<f32>::epsilonEquals(
	        0.0f, mSwingSpeed, JGeometry::TUtil<f32>::epsilon())
	    && prevSpeed * mSwingSpeed <= 0.0f) {
		gpMSound->startSoundActorWithInfo(MSD_SE_EN_BEENEST_SWING, &mPosition,
		                                  nullptr, fabsf(mSwingAngle), 0, 0,
		                                  nullptr, 0, 4);
	}

	// TODO: the inlined slerp() below folds `1.0f - param_2` to 0.99f in the
	// epsilon branch but recomputes it with an fsubs in the acos branch, where
	// the ROM loads the folded constant in both. That is inside
	// JGeometry::TQuat4<f>::slerp in JGQuat4.hpp, not here.
	mRotation168.slerp(mGoalRotation, 0.01f);
	mRotation168.normalize();

	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario.sub(mPosition);

	f32 range = getSaveParams()->mSearchRange.get();
	if (toMario.squared() <= range * range)
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

	JGeometry::TVec3<f32> velocity;
	mRotation168.getZDir(velocity);
	velocity.scale(1.0f);

	mSwingSpeed = getSaveParams()->mFallAngularVel.get();
	mVelocity   = velocity;
}

// UNUSED (0x24), and dead: TNerveBeeHiveFall::execute spells the swing step
// and the landing test out itself -- it branches straight on the airborne flag
// instead of materialising a bool, which an inlined bool-returning helper
// always does. The shape is taken from doWait()/doScaling(), the unit's other
// two "advance the state and say whether it is over" helpers; that is also
// what makes the body the map's 0x24 rather than 0x14.
bool TBeeHive::doFall()
{
	mSwingAngle += mSwingSpeed;
	return !checkLiveFlag(LIVE_FLAG_AIRBORNE);
}

void TBeeHive::appearBee(int index)
{
	TRealoidActor* bee = unk154[index];

	if (bee->checkFlag(TRealoidActor::FLAG_UNK4))
		return;
	if (!bee->checkFlag(TRealoidActor::FLAG_UNK2))
		return;

	bee->offFlag(TRealoidActor::FLAG_UNK2);
	bee->offHitFlag(HIT_FLAG_NO_COLLISION);

	unk150->getBoid(index)->mPosition = mPosition;
}

void TBeeHive::disappearBee(int index)
{
	TRealoidActor* bee = unk154[index];

	if (bee->checkFlag(TRealoidActor::FLAG_UNK2))
		return;

	bee->onFlag(TRealoidActor::FLAG_UNK2);
	bee->onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBeeHive::decrimentBee()
{
	if (mBeeNum > 3) {
		mBeeNum -= 1;
		disappearBee(mBeeNum);
	}
}

void TBeeHive::appearAllBees()
{
	for (int i = 0; i < unk150->getBoidNum(); ++i)
		appearBee(i);

	mBeeNum = unk150->getBoidNum();
}

void TBeeHive::dropCoin()
{
	TMapObjBase* item = mDropItem;
	if (item == nullptr)
		return;

	if (item->isActorType(0x2000000E))
		item = gpItemManager->makeObjAppear(0x2000000E);

	if (item == nullptr)
		return;

	item->appear();
	item->JSGSetTranslation(mPosition);
	item->setVelocityAndFlag10(0.0f, 0.0f, 0.0f);
	item->onLiveFlag(LIVE_FLAG_AIRBORNE);

	mDropItem = nullptr;
}

void TBeeHive::prepareBreak()
{
	mMActor = mMActorKeeper->getMActor("bee_nest_break.bmd");
	mMActor->setBck("bee_nest_break");

	onHitFlag(HIT_FLAG_NO_COLLISION);

	SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_A, &mPosition, this,
	                     JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));
	SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_B, &mPosition, this,
	                     JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));

	gpMSound->startSoundActor(MSD_SE_EN_BEENEST_LAND, &mPosition, 0, nullptr,
	                          0, 4);
}

void TBeeHive::prepareVanish() { onLiveFlag(LIVE_FLAG_HIDDEN); }

void TBeeHive::prepareReset()
{
	reset();
	offLiveFlag(LIVE_FLAG_HIDDEN);
	mScaling.set(0.0f, 0.0f, 0.0f);
}

// Grows the hive back after a reset; true once it is at full size again.
bool TBeeHive::doScaling()
{
	bool grown = false;

	mScaling.x += 0.01f;
	if (1.0f <= mScaling.x) {
		mScaling.set(1.0f, 1.0f, 1.0f);
		grown = true;
	} else {
		mScaling.y += 0.01f;
		mScaling.z += 0.01f;
	}

	return grown;
}

bool TBeeHive::isMissMario() const
{
	if (mBeeNum == 0)
		return false;

	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario.sub(getCenterOfGravity());

	f32 range = getSaveParams()->mGiveupRange.get();
	return range * range <= toMario.squared();
}

// UNUSED (0x2d4). receiveMessage() inlines this, and it has to: the extra
// inline level is what keeps normalize()'s dot/inv_sqrt/scale and setRotate
// out of line there, exactly as the ROM has them.
// TODO: our body is 0x2e0, twelve bytes over the map. The out-of-line
// TQuat4<f>::setRotate copy this TU emits is also 98.8% and differs only by a
// permutation of f28..f31 around the inlined cross()/length(), i.e. inside
// JGQuat4.hpp, not here.
void TBeeHive::setShakePower(const JGeometry::TVec3<f32>& to_mario)
{
	if (isFalling())
		return;

	JGeometry::TVec3<f32> dir = to_mario;
	dir.y                     = 0.0f;
	dir.normalize();

	f32 sign;
	if (fabsf(mSwingSpeed) < 0.0001f)
		sign = 1.0f;
	else
		sign = (f32)(mSwingSpeed > 0.0f ? 1
		                                : (mSwingSpeed < 0.0f ? -1 : 0));

	JGeometry::TVec3<f32> up(0.0f, 0.0f, 1.0f);
	mGoalRotation.setRotate(up, dir, 1.0f);

	mSwingSpeed += sign * getSaveParams()->mShakePower.get();
}

// Mean position of every bee that is currently out, used as the swarm's
// "where is it" for the give-up test.
// TODO: frame 0x50 against retail's 0x68, and retail reloads the boid
// array for every unrolled element as if `center` were address-taken. The
// old statement-body TVec3 copy constructor gave that (92.9%); with the
// Vec-base one, inert: `+=`, `add(center, p)`, `*=`, `div(num)`, `zero()`,
// `center = TVec3(0)`, `return center * k` (worse); an extra
// `ret = center; return ret;` reaches 90.7% but is invented.
JGeometry::TVec3<f32> TBeeHive::getCenterOfGravity() const
{
	int num = mBeeNum;
	if (num == 0)
		return JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);

	JGeometry::TVec3<f32> center;
	center.set(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < num; ++i)
		center.add(unk150->getBoid(i)->mPosition);

	center.scale(1.0f / num);
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

// TODO: incorrect size. Map records 0x14; the drop item is cleared by
// dropCoin(), so testing it is the cheapest "has not broken yet" the class
// offers, but no emitted code uses this helper.
bool TBeeHive::isNotBreakOnceYet() const { return mDropItem != nullptr; }

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
	TBeeHive* hive = (TBeeHive*)spine->getBody();

	if (spine->getTime() == 0)
		hive->prepareWait();

	if (hive->doWait()) {
		spine->pushAfterCurrent(&TNerveBeeHiveFall::theNerve());
		return TRUE;
	}

	if (hive->getSaveParams()->mDecrimentTimer.get() < spine->getTime()) {
		hive->decrimentBee();
		spine->pushAfterCurrent(this);
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBeeHiveFall, TLiveActor)
{
	TBeeHive* hive = (TBeeHive*)spine->getBody();

	if (spine->getTime() == 0) {
		hive->prepareFall();
		hive->dropCoin();
	}

	hive->mSwingAngle += hive->mSwingSpeed;

	if (!hive->checkLiveFlag(LIVE_FLAG_AIRBORNE)) {
		spine->pushAfterCurrent(&TNerveBeeHiveBreak::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBeeHiveBreak, TLiveActor)
{
	TBeeHive* hive = (TBeeHive*)spine->getBody();

	if (spine->getTime() == 0) {
		hive->prepareBreak();
		hive->appearAllBees();
		hive->setBoidParamOnAttacking();
	}

	if (hive->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveBeeHiveAttack::theNerve());
		hive->onLiveFlag(LIVE_FLAG_HIDDEN);
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBeeHiveAttack, TLiveActor)
{
	TBeeHive* hive = (TBeeHive*)spine->getBody();

	if (spine->getTime() == 0)
		hive->setBoidParamOnAttacking();

	if (SMS_IsMarioInWater()) {
		spine->pushAfterCurrent(&TNerveBeeHiveMarioWaterIn::theNerve());
		return TRUE;
	}

	if (hive->isMissMario()) {
		spine->pushAfterCurrent(&TNerveBeeHiveReset::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBeeHiveMarioWaterIn, TLiveActor)
{
	TBeeHive* hive = (TBeeHive*)spine->getBody();

	if (spine->getTime() == 0)
		hive->setBoidParamOnMarioWaterIn();

	if (hive->getSaveParams()->mGiveupTimer.get() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveBeeHiveReset::theNerve());
		return TRUE;
	}

	if (!SMS_IsMarioInWater()) {
		spine->pushAfterCurrent(&TNerveBeeHiveAttack::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBeeHiveReset, TLiveActor)
{
	TBeeHive* hive = (TBeeHive*)spine->getBody();

	if (spine->getTime() == 0)
		hive->prepareReset();

	bool grown = hive->doScaling();
	if (grown) {
		spine->pushAfterCurrent(&TNerveBeeHiveWait::theNerve());
		return TRUE;
	}

	return FALSE;
}
