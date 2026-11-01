#include <Enemy/Kukku.hpp>
#include <Animal/AnimalBase.hpp>
#include <Enemy/Graph.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <System/Particles.hpp>
#include <macros.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>

namespace {

// How many coins a bird drops when it gets shot down, picked at random from
// the last three entries.
const int cDropCoinNumTable[] = { 3, 3, 1, 2 };

} // namespace

static const char* tori_bastable[] = {
	"/scene/tori/bas/tori_back.bas",
	"/scene/tori/bas/tori_down.bas",
	nullptr,
	"/scene/tori/bas/tori_fall_end.bas",
	"/scene/tori/bas/tori_hit.bas",
	"/scene/tori/bas/tori_wait.bas",
};

TKukkuBall::TKukkuBall(MActor* actor)
    : THitActor("クック玉")
    , mMActor(actor)
    , mFlags(1)
    , unk7C(0)
{
}

// TODO: 99.9%, one register differs.
void TKukkuBall::init()
{
	initHitActor(ACTOR_TYPE_KUKKU, 1, 0x80000000, 30.0f, 30.0f, 0.0f, 0.0f);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	onHitFilter(HIT_FILTER_NO_DAMAGE);
	((TIdxGroupObj*)JDrama::TNameRefGen::search("敵グループ"))
	    ->getChildren()
	    .push_back(this);

	const ResTIMG* texture = (const ResTIMG*)JKRFileLoader::getGlbResource(
	    "/scene/map/pollution/H_ma_rak.bti");
	if (texture)
		SMS_ChangeTextureAll(mMActor->getModel()->getModelData(),
		                     "K_name_dummy", *texture);
}

// TODO: 96.9%, register allocation around the matrix copy.
void TKukkuBall::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mFlags & 1)
		return;

	if (cue & CUE_CALC_ANIM) {
		TPosition3f mtx;
		mtx.identity33();
		mtx.setTrans(mPosition);
		J3DModel* model = mMActor->getModel();
		model->setBaseScale(mScaling);
		MTXCopy(mtx, mMActor->getModel()->getBaseTRMtx());
		mMActor->getModel()->calc();
	}

	if (cue & CUE_MOVE) {
		checkHitActors();
		bind();
	}

	if (!(mFlags & 4))
		mMActor->perform(cue, graphics);
}

// TODO: the map gives 0x3C bytes, so the target calls checkHitActors and
// bind out of line here; ours inlines them.
void TKukkuBall::moveObject()
{
	checkHitActors();
	bind();
}

// Falls with a little air drag and pollutes the ground where it lands.
// TODO: UNUSED size is 0x128 in the map, ours is larger.
void TKukkuBall::bind()
{
	mVelocity.y -= 0.9f;
	mVelocity.scale(0.94f);

	JGeometry::TVec3<f32> next = mPosition;
	next += mVelocity;

	const TBGCheckData* ground;
	if (next.y <= 0.05f
	                  + (gpMap->checkGround(next.x, next.y + mAttackHeight,
	                                        next.z, &ground)
	                     + 1.0f))
		kill();

	gpMap->isTouchedOneWallAndMoveXZ(&next.x, next.y + mAttackHeight, &next.z,
	                                 mAttackRadius);
	mPosition = next;
}

void TKukkuBall::checkHitActors()
{
	THitActor** end = mCollisions + mColCount;
	for (THitActor** it = mCollisions; it != end; ++it) {
		switch ((*it)->getActorType()) {
		case ACTOR_TYPE_MARIO:
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
			kill();
			break;
		}
	}
}

void TKukkuBall::startToMove(const JGeometry::TVec3<f32>& pos,
                             const JGeometry::TVec3<f32>& velocity)
{
	offHitFilter(HIT_FILTER_NO_COLLISION);
	mFlags &= ~1;
	mPosition = pos;
	mVelocity = velocity;
}

void TKukkuBall::kill()
{
	onHitFilter(HIT_FILTER_NO_COLLISION);
	mFlags |= 1;
	gpPollution->pollute(mPosition.x, mPosition.y, mPosition.z, 500.0f);
}

bool TKukkuBall::isDead() const
{
	if ((mFlags & 1) && unk7C == 0)
		return true;
	return false;
}

TEnemyCoinUnit::TEnemyCoinUnit(int) { }

void TEnemyCoinUnit::init() { }

TMapObjBase* TEnemyCoinUnit::getUnusedItem() { return nullptr; }

TKukku::TKukku(const char* name)
    : TSmallEnemy(name)
    , mOneUp(nullptr)
{
}

void TKukku::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 4);
	mMActor       = mMActorKeeper->createMActor("tori.bmd", 0);
	mSpine->initWith(&TNerveKukkuGraphWander::theNerve());
	mOneUp = TMapObjBaseManager::newAndRegisterObj("mushroom1upR");
	initBalls();
	initCollision();
	initAnmSound();
	initParticle();
	mCenterJoint
	    = getModel()->getModelData()->getJointName()->getIndex("center");
	reset();
}

void TKukku::initBalls()
{
	for (TKukkuBall** it = mBalls; it != mBalls + ARRAY_COUNT(mBalls); ++it) {
		*it = new TKukkuBall(mMActorKeeper->createMActor("torifun.bmd", 3));
		(*it)->init();
	}
}

void TKukku::initCollision()
{
	initHitActor(ACTOR_TYPE_KUKKU, 1, 0x80000000, 30.0f, 30.0f, 100.0f, 100.0f);
	offHitFilter(HIT_FILTER_NO_COLLISION);
}

void TKukku::initParticle()
{
	SMS_LoadParticle("/scene/tori/jpa/ms_cooc_ase.jpa", KUKKU_JPA_MS_COOC_ASE);
	SMS_LoadParticle("/scene/tori/jpa/ms_cooc_hane.jpa",
	                 KUKKU_JPA_MS_COOC_HANE);
}

void TKukku::reset()
{
	unk1A4      = 0;
	mShootTimer = 0;
	mGravity    = 0.0f;
	mDroppedNum = 0;
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	mScaledBodyRadius = 75.0f;
}

BOOL TKukku::receiveMessage(THitActor* sender, u32 message)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return false;

	switch (message) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
		behaveHitTrample();
		return true;
	}

	return TSmallEnemy::receiveMessage(sender, message);
}

void TKukku::control()
{
	if (getUnk1A4() > 0)
		unk1A4--;
	TLiveActor::control();
}

// TODO: 96%, register allocation only.
void TKukku::calcRootMatrix()
{
	if (isDying()) {
		// Lie down flat on whatever the bird fell on.
		JGeometry::TVec3<f32> up;
		if (mGroundPlane) {
			up.set(mGroundPlane->getNormal());
			up.normalize();
		} else {
			up.set(0.0f, 1.0f, 0.0f);
		}

		JGeometry::TQuat4<f32> yaw;
		yaw.setEulerY(0.017453294f * mRotation.y);
		JGeometry::TQuat4<f32> tilt;
		tilt.setRotate(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), up, 1.0f);
		tilt.mul(tilt, yaw);

		TPosition3f mtx;
		mtx.setQT(tilt, mPosition);
		getModel()->setBaseTRMtx(mtx);
		getModel()->setBaseScale(mScaling);
	} else {
		TSpineEnemy::calcRootMatrix();
	}

	updateEffect();
}

void TKukku::bind() { TLiveActor::bind(); }

void TKukku::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);
	for (TKukkuBall** it = mBalls; it != mBalls + ARRAY_COUNT(mBalls); ++it)
		(*it)->perform(cue, graphics);
}

// Feathers while falling, sweat while flapping back up.
void TKukku::updateEffect()
{
	if (isFalling())
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    KUKKU_JPA_MS_COOC_HANE, getModel()->getAnmMtx(mCenterJoint), 1,
		    this);
	if (mMActor->checkCurAnm("tori_back", 0))
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    KUKKU_JPA_MS_COOC_ASE, getModel()->getAnmMtx(mCenterJoint), 1,
		    this);
}

// Water knocks the bird out of the sky.
void TKukku::behaveToWater(THitActor*)
{
	if (!isFalling() && !isRecoveringGraph()) {
		mSpine->reset();
		mSpine->setNext(&TNerveKukkuFall::theNerve());
	}
}

void TKukku::behaveHitTrample()
{
	mSpine->reset();
	mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
}

// TODO: UNUSED size is 0x2C8 in the map, ours is far larger, so the target
// keeps some of its helpers out of line.
void TKukku::doFlyToCurPathNode()
{
	if (isReachedToGoal()) {
		goToRandomNextGraphNode();
		if (!mMActor->checkCurAnm("tori_wait", 0))
			changeBck("tori_wait");
	}

	if (isFindOutMario()) {
		if (getShootTimer() >= 0)
			mShootTimer--;
		else
			shotBall();
	}

	updateRotation();
	mPositionDelta = calcMomentum(getParam()->mMarchSpeed.get());
}

// TODO: 99.0%, one register differs in the path node point.
void TKukku::updateRotation()
{
	JGeometry::TVec3<f32> diff = mCurrentTarget.getPoint();
	diff -= mPosition;

	f32 dist = diff.length();
	if (dist < 100.0f)
		return;

	f32 marchSpeed = getParam()->mMarchSpeed.get();
	f32 turnSpeed  = getParam()->mTurnSpeed.get();
	if (dist <= 2.0f * calcMinimumTurnRadius(marchSpeed, turnSpeed))
		turnSpeed = calcTurnSpeedToReach(marchSpeed, 0.5f * dist);

	TAnimalBase::getRotationFlyToDir(&mRotation, diff, marchSpeed, turnSpeed);

	// No banking or pitching while tumbling down.
	mRotation.x *= isFalling() ? 0.0f : 1.0f;
	mRotation.z *= isFalling() ? 0.0f : 1.0f;
}

// TODO: 96.4%, registers differ. The other spellings tried were worse.
JGeometry::TVec3<f32> TKukku::calcMomentum(f32 speed)
{
	JGeometry::TQuat4<f32> rot = SMS_Eular2Quat(mRotation);
	JGeometry::TVec3<f32> momentum;
	momentum.set(0.0f, 0.0f, speed);
	rot.rotate(momentum, momentum);
	return momentum;
}

// TODO: UNUSED size is 0xA0 in the map, so calcMomentum stays a call in the
// target; ours inlines it.
void TKukku::doRecoverToCurPathNode()
{
	updateRotation();
	mPositionDelta = calcMomentum(getParam()->mMarchSpeed.get());
}

// Flap back up after the fall. Returns true once the climb has stopped.
bool TKukku::doHabataki()
{
	JGeometry::TVec3<f32> velocity = mVelocity;
	velocity.scale(getParam()->mAirFric.get());
	velocity.y += getParam()->mUpperVelocityY.get();
	bool stopped = false;
	if (-0.1f < velocity.y) {
		stopped    = true;
		velocity.y = 0.0f;
	}
	mVelocity = velocity;
	return stopped;
}

void TKukku::decideFlyingAnm()
{
	if (mMActor->checkCurAnm("tori_back", 0))
		changeBck("tori_back");
	else
		changeBck("tori_wait");
}

// TODO: UNUSED size is 0x20C in the map, ours is 0x30 smaller.
void TKukku::shotBall()
{
	TKukkuBall* ball = getUnusedBall();
	if (ball) {
		JGeometry::TVec3<f32> velocity(0.0f, 1.0f, 0.0f);
		velocity.setLength(getParam()->mShootSpeed.get());
		// TODO: the joint is looked up but never used; the goop starts at
		// the bird's position instead.
		getModel()->getModelData()->getJointName()->getIndex("null_osen");
		ball->startToMove(mPosition, velocity);
		mShootTimer = getParam()->mShootInterval.get();
		SMSGetMSound()->startSoundActor(MSD_SE_EN_TORI_SHIT, &mPosition, 0,
		                                nullptr, 0, 4);
	}
}

// TODO: fake inline, the same one as in enemy.cpp and riccohook.cpp
static inline JGeometry::TVec3<f32> polarXZ(f32 theta, f32 radius)
{
	f32 c = radius * MsCos(theta);
	f32 s = radius * MsSin(theta);
	return JGeometry::TVec3<f32>(s, 0.0f, c);
}

// Scatter a few coins around the bird, and a 1-up once ten have dropped.
// TODO: 98.9%, mRotation.y is loaded after getParam() here, before it in
// the target.
void TKukku::dropCoins()
{
	if (mDroppedNum > 10)
		return;

	if (mDroppedNum == 10 && mOneUp) {
		mDroppedNum++;
		mOneUp->appear();
		mOneUp->JSGSetTranslation(mPosition);
		mOneUp->setVelocityAndFlag10(0.0f, 0.0f, 0.0f);
		return;
	}

	int index = 4.0f * MsRandF();
	if (index < 1)
		index = 1;
	else if (index > 3)
		index = 3;
	int num = cDropCoinNumTable[index];

	JGeometry::TQuat4<f32> step;
	step.setEulerY(6.2831855f / num);
	JGeometry::TQuat4<f32> tilt;
	tilt.setEulerX(3.1415927f * getParam()->mDropAngleX.get());

	JGeometry::TVec3<f32> velocity
	    = polarXZ(mRotation.y, getParam()->mDropSpeed.get());
	tilt.rotate(velocity);
	step.rotate(velocity);

	for (int i = 0; i < num; i++) {
		TMapObjBase* coin = gpItemManager->makeObjAppear(ACTOR_TYPE_COIN);
		if (!coin)
			break;
		coin->appear();
		coin->JSGSetTranslation(mPosition);
		coin->mVelocity.set(velocity);
		coin->offLiveFlag(LIVE_FLAG_UNK10);
		if (++mDroppedNum == 10)
			break;
		step.rotate(velocity);
	}
}

bool TKukku::isFalling() const
{
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveKukkuFall::theNerve()
	       || nerve == &TNerveKukkuPostFall::theNerve();
}

bool TKukku::isRecoveringGraph() const
{
	if (mSpine->getLatestNerve() == &TNerveKukkuRecoverGraph::theNerve())
		return true;
	return false;
}

bool TKukku::isDying() const
{
	return mSpine->getLatestNerve() == &TNerveSmallEnemyDie::theNerve();
}

bool TKukku::isFindOutMario() const
{
	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario -= mPosition;
	toMario.y = 0.0f;
	f32 range = getParam()->mSearchRange.get();
	return toMario.squared() < range * range;
}

void TKukku::changeBck(const char* name)
{
	mMActor->setBck(name);
	setCurAnmSound();
}

// TODO: 96.2%, an mr where the target has addi.
void TKukku::setDeadAnm() { changeBck("tori_down"); }

void TKukku::setAfterDeadEffect()
{
	TSmallEnemy::setAfterDeadEffect();
	gpPollution->stamp(((TSmallEnemyManager*)mManager)->getUnk58(), mPosition.x,
	                   mPosition.y, mPosition.z, 1000.0f);
}

// TODO: UNUSED and nothing linked shows its value.
f32 TKukku::getWaterDamageRate() const { return 0.0f; }

f32 TKukku::getWaterPowerY() const { return getParam()->mWaterPowerY.get(); }

// TODO: UNUSED size is 0x4C in the map, ours is 4 bytes larger.
TKukkuBall* TKukku::getUnusedBall()
{
	for (TKukkuBall** it = mBalls; it != mBalls + ARRAY_COUNT(mBalls); ++it)
		if ((*it)->isDead())
			return *it;
	return nullptr;
}

TKukkuParams::TKukkuParams(const char* path)
    : TSmallEnemyParams(path)
    , PARAM_INIT(mMarchSpeed, 3.0f)
    , PARAM_INIT(mTurnSpeed, 0.2f)
    , PARAM_INIT(mWaterPowerY, 12.0f)
    , PARAM_INIT(mShootSpeed, 1.0f)
    , PARAM_INIT(mShootInterval, 60)
    , PARAM_INIT(mSearchRange, 800.0f)
    , PARAM_INIT(mHabatakiTimer, 45)
    , PARAM_INIT(mAirFric, 0.97f)
    , PARAM_INIT(mUpperVelocityY, 0.0f)
    , PARAM_INIT(mDropSpeed, 5.0f)
    , PARAM_INIT(mDropAngleX, -0.25f)
{
	TParams::load(mPrmPath);
}

TKukkuManager::TKukkuManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TKukkuManager::load(JSUMemoryInputStream& stream)
{
	TKukkuParams* params = new TKukkuParams("/enemy/kukku.prm");
	unk38                = params;
	params->mSLAttackRadius.set(30);
	params->mSLAttackHeight.set(30);
	params->mSLDamageRadius.set(100);
	params->mSLDamageHeight.set(100);
	params->mSLBodyRadius.set(5.0f);
	TSmallEnemyManager::load(stream);
}

void TKukkuManager::initCoins() { }

void TKukkuManager::createModelData()
{
	static const TModelDataLoadEntry entry[]
	    = { { "tori.bmd", 0x10210000, 0 }, { nullptr, 0, 0 } };
	createModelDataArray(entry);
}

const char** TKukku::getBasNameTable() const { return tori_bastable; }

// TODO: 75.3%. In the target the helpers inside shotBall and isFindOutMario
// (getUnusedBall, the sub in -=, setLength's body) sit one inline level
// shallower than doFlyToCurPathNode puts them, and updateRotation stays a
// call. Writing doFlyToCurPathNode's body straight into this nerve lines all
// of that up but then calcMomentum's own helpers inline too deeply, and the
// map sizes of doFlyToCurPathNode (0x2C8) and doRecoverToCurPathNode (0xA0)
// still disagree, so the real layout is not known yet.
DEFINE_NERVE(TNerveKukkuGraphWander, TLiveActor)
{
	TKukku* self = (TKukku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
		self->unk124->reset();
		self->goToShortestNextGraphNode();
		self->decideFlyingAnm();
	}
	self->doFlyToCurPathNode();
	return false;
}

// TODO: UNUSED and nothing linked shows its body. It is rebuilt from the
// "tori_hit" string it leaves in the pool and from the other nerves; the
// size matches the map, the details are a guess.
DEFINE_NERVE(TNerveKukkuHit, TLiveActor)
{
	TKukku* self = (TKukku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->changeBck("tori_hit");
	}

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveKukkuGraphWander::theNerve());
		return true;
	}
	return false;
}

// TODO: 99.8%, frame-only.
DEFINE_NERVE(TNerveKukkuFall, TLiveActor)
{
	TKukku* self = (TKukku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->changeBck("tori_wait");
		J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		ctrl->setRate(2.0f * SMSGetAnmFrameRate());
		self->mVelocity
		    = JGeometry::TVec3<f32>(0.0f, -self->getWaterPowerY(), 0.0f);
		self->dropCoins();
	}

	// Hitting the ground finishes it off.
	if (!self->checkLiveFlag(LIVE_FLAG_AIRBORNE)) {
		SMS_EasyEmitParticle(PARTICLE_MS_POPO_BOMB_A, &self->mPosition, nullptr,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		SMS_EasyEmitParticle(PARTICLE_MS_POPO_BOMB_B, &self->mPosition, nullptr,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		spine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());
		return true;
	}

	if (self->doHabataki()) {
		spine->pushAfterCurrent(&TNerveKukkuRecoverGraph::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveKukkuPostFall, TLiveActor)
{
	TKukku* self = (TKukku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->changeBck("tori_back");
		self->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
	}

	if (self->checkCurAnmEnd(0)
	    && self->getParam()->mHabatakiTimer.get() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveKukkuGraphWander::theNerve());
		return true;
	}
	return false;
}

// TODO: 83.4%, the target calls calcMomentum out of line; ours inlines it.
// This is also why TVec3::set<f32> is emitted after this nerve instead of
// after GraphWander, which the symbol check reports as an order error.
DEFINE_NERVE(TNerveKukkuRecoverGraph, TLiveActor)
{
	TKukku* self = (TKukku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->changeBck("tori_back");
		self->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
	}

	if (self->getParam()->mHabatakiTimer.get() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveKukkuGraphWander::theNerve());
		return true;
	}

	self->doRecoverToCurPathNode();
	return false;
}
