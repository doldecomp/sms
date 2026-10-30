#include <Enemy/Kukku.hpp>
#include <Strategic/ObjModel.hpp>
#include <Map/PollutionManager.hpp>
#include <System/EmitterViewObj.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/MapData.hpp>
#include <Strategic/LiveActor.hpp>
#include <Animal/AnimalBase.hpp>
#include <Strategic/Strategy.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <System/Particles.hpp>

// to match __sinit__
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// placeholder
static const char* tori_bastable[] = { "/scene/tori/bas/tori_back.bas",
	                                   "/scene/tori/bas/tori_down.bas",
	                                   nullptr,
	                                   "/scene/tori/bas/tori_fall_end.bas",
	                                   "/scene/tori/bas/tori_hit.bas",
	                                   "/scene/tori/bas/tori_wait.bas" };

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

int TKukkuBall::init()
{
	initHitActor(ACTOR_TYPE_KUKKU_BALL, 1, HIT_CATEGORY_PLAYER, 30.0f, 30.0f,
	             0.0f, 0.0f);
	TIdxGroupObj* group
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));

	group->push_back(this);
	const ResTIMG* res = static_cast<const ResTIMG*>(
	    JKRFileLoader::getGlbResource("/scene/map/pollution/H_ma_rak.bti"));

	J3DModelData* ptvar6 = mOwner->getModel()->getModelData();
	if (res != nullptr)
		SMS_ChangeTextureAll(ptvar6, "K_name_dummy", *res);

	return (int)ptvar6;
}

void TKukkuBall::perform(u32 cue, JDrama::TGraphics* graphics) { }

BOOL TKukkuBall::isDead() const { }

void TKukkuBall::kill() { }

void TKukkuBall::startToMove(const JGeometry::TVec3<f32>& start,
                             const JGeometry::TVec3<f32>& end)
{
}

void TKukkuBall::checkHitActors() { }

void TKukkuBall::bind() { }

void TKukkuBall::moveObject() { }

TKukku::TKukku(const char* name)
    : TSmallEnemy(name)
{
}

void TKukku::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 0x18);
	mMActor       = mMActorKeeper->createMActor("tori.bmd", 0);

	mSpine->initWith(&TNerveKukkuGraphWander::theNerve());

	mMapObj = TMapObjBaseManager::newAndRegisterObj(
	    "mushroom1upR", JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
	    JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
	    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));

	for (int i = 0; i < unk1A0; i++) {
		mMActor   = mMActorKeeper->createMActor("torifun.bmd", 0);
		mBalls[i] = new TKukkuBall("クック玉");
	}
	initHitActor(ACTOR_TYPE_KUKKU_BALL, 1, HIT_CATEGORY_PLAYER, 30.0f, 30.0f,
	             100.0f, 100.0f);
	offHitFilter(HIT_FILTER_NO_COLLISION);

	initAnmSound();

	gpResourceManager->load("/scene/tori/jpa/ms_cooc_ase.jpa", 0x18c);
	gpResourceManager->load("/scene/tori/jpa/ms_cooc_hane.jpa", 0x18d);
	unk1A8 = getModel()->getModelData()->unkB0->getIndex("center");
}

void TKukku::reset()
{
	unk1A4   = 0;
	unk1AC   = 0;
	mGravity = 0.0f;
	unk1B0   = 0;
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	mScaledBodyRadius = 75.0f;
}

BOOL TKukku::receiveMessage(THitActor* sender, u32 message)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD)) {
		return false;
	}

	switch (message) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP: {
		mSpine->reset();
		mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
		return true;
	};

	default:
		return TSmallEnemy::receiveMessage(sender, message);
	}
}

void TKukku::control()
{
	if (getUnk1A4() > 0)
		unk1A4 -= 1;

	TLiveActor::control();
}

void TKukku::calcRootMatrix()
{
	if (isDying()) {
		JGeometry::TVec3<f32> normal;
		if (mGroundPlane != nullptr) {
			normal = getGroundPlane()->getNormal();
			normal.setLength(1.0f);
		} else {
			normal.set(0.0f, 1.0f, 0.0f);
		}
		JGeometry::TQuat4<f32> yaw;
		yaw.setEulerY(mRotation.y * 0.017453294f * 0.5);
		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		JGeometry::TQuat4<f32> tilt;
		tilt.setRotate(up, normal, 1.0f);
		tilt.mul(tilt, yaw);

		JGeometry::TRotation3<
		    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >
		    mtx;
		mtx.setQuat(tilt); // gets emitted; should be inlined

		PSMTXCopy(mtx, getModel()->getBaseTRMtx());
		getModel()->setBaseScale(mScaling);
	} else {
		TSpineEnemy::calcRootMatrix();
	}

	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	bool falling                        = nerve == &TNerveKukkuFall::theNerve()
	               || nerve == &TNerveKukkuPostFall::theNerve();

	if (falling) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x18d, getModel()->getAnmMtx(unk1A8), 1, this);
	}

	if (mMActor->checkCurAnm("tori_back", 0)) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x18c, getModel()->getAnmMtx(unk1A8), 1, this);
	}
}

void TKukku::bind() { TLiveActor::bind(); }

void TKukku::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);
	for (int i = 0; i < unk1A0; i++) {
		mBalls[i]->perform(cue, graphics);
	}
}

void TKukku::behaveToWater(THitActor* sender)
{
	if (mSpine->getLatestNerve() != &TNerveKukkuFall::theNerve()) {
		if (mSpine->getLatestNerve() != &TNerveKukkuRecoverGraph::theNerve()) {
			mSpine->setNext(&TNerveKukkuFall::theNerve());
		}
	}
}

void TKukku::updateRotation()
{
	JGeometry::TVec3<f32> toTarget = unkF4.getPoint();
	toTarget.sub(mPosition);

	f32 dist = toTarget.length();

	if (dist >= 100.0f) {
		f32 marchSpeed = ((TKukkuParams*)getSaveParam())->mMarchSpeed.get();
		f32 turnSpeed  = ((TKukkuParams*)getSaveParam())->mTurnSpeed.get();

		if (dist <= 2.0f * calcMinimumTurnRadius(marchSpeed, turnSpeed)) {
			turnSpeed = calcTurnSpeedToReach(marchSpeed, 0.5f * dist);
		}

		TAnimalBase::getRotationFlyToDir(&mRotation, toTarget, marchSpeed,
		                                 turnSpeed);

		mRotation.x *= isFalling() ? 0.0f : 1.0f;
		mRotation.z *= isFalling() ? 0.0f : 1.0f;
	}
}

JGeometry::TVec3<f32> TKukku::calcMomentum(f32 deltaTime)
{

	JGeometry::TQuat4<f32> quat = SMS_Eular2Quat(mRotation);
	JGeometry::TVec3<f32> momentum(0.0f, 0.0f, deltaTime);

	quat.rotate(momentum);
	return momentum;
}

void TKukku::dropCoins() { }

void TKukku::setDeadAnm()
{
	getMActor()->setBck("tori_dead");
	setCurAnmSound();
}

void TKukku::setAfterDeadEffect()
{
	TSmallEnemy::setAfterDeadEffect();
	TSmallEnemyManager* manager = (TSmallEnemyManager*)getManager();
	gpPollution->stamp(manager->getUnk58(), mPosition.x, mPosition.y,
	                   mPosition.z, 1000.0f);
}

// UNUSED
void TKukku::getUnusedBall() { }

void TKukku::getWaterPowerY() const { }

void TKukku::getWaterDamageRate() const { }

void TKukku::changeBck(int) { }

BOOL TKukku::isFindOutMario() const { }

BOOL TKukku::isDying() const
{
	return mSpine->getLatestNerve() == &TNerveSmallEnemyDie::theNerve();
}

BOOL TKukku::isRecoveringGraph() const
{
	return mSpine->getLatestNerve() == &TNerveKukkuRecoverGraph::theNerve();
}

BOOL TKukku::isFalling() const
{
	return mSpine->getLatestNerve() == &TNerveKukkuFall::theNerve()
	       || mSpine->getLatestNerve() == &TNerveKukkuPostFall::theNerve();
}

void TKukku::shotBall() { };

void TKukku::decideFlyingAnm() { }

void TKukku::doHabataki() { }

void TKukku::doRecoverToCurPathNode()
{
	updateRotation();
	f32 momentum = ((TKukkuParams*)getSaveParam2())->mDropAngleX.get();
	calcMomentum(momentum);
}

void TKukku::doFlyToCurPathNode() { }

void TKukku::behaveHitTrample() { }

void TKukku::updateEffect() { }

void TKukku::initParticle() { }

void TKukku::initCollision() { }

void TKukku::initBalls() { }

void TKukku::getUnusedItem(TEnemyCoinUnit*) { }

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

void TKukkuManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		"tori.bmd",
		0,
		0,
	};

	createModelDataArray(entry);
}

const char** TKukku::getBasNameTable() const { return tori_bastable; }

DEFINE_NERVE(TNerveKukkuGraphWander, TLiveActor) { }

DEFINE_NERVE(TNerveKukkuFall, TLiveActor)
{
	TKukku* kukku = static_cast<TKukku*>(spine->getBody());

	if (spine->getTime() == 0) {
		kukku->getMActor()->setBck("tori_back");
		kukku->setCurAnmSound();
		kukku->mVelocity.zero();
	}
}

DEFINE_NERVE(TNerveKukkuPostFall, TLiveActor)
{
	TKukku* kukku = static_cast<TKukku*>(spine->getBody());

	if (spine->getTime() == 0) {
		kukku->getMActor()->setBck("tori_wait");
		kukku->setCurAnmSound();
		kukku->mVelocity.zero();
	}

	if (!kukku->checkCurAnmEnd(0)
	    || spine->getTime() <= ((TKukkuParams*)kukku->getSaveParam2())
	                               ->mHabatakiTimer.get()) {
		return false;
	}

	spine->pushNerve(&TNerveKukkuGraphWander::theNerve());

	return true;
}

DEFINE_NERVE(TNerveKukkuRecoverGraph, TLiveActor)
{
	TKukku* kukku = static_cast<TKukku*>(spine->getBody());

	if (spine->getTime() == 0) {
		kukku->getMActor()->setBck("tori_back");
		kukku->setCurAnmSound();
	}

	if (spine->getTime()
	    > ((TKukkuParams*)kukku->getSaveParam2())->mHabatakiTimer.get()) {
		spine->pushNerve(&TNerveKukkuGraphWander::theNerve());
		return true;
	}

	kukku->doRecoverToCurPathNode();

	return false;
}

DEFINE_NERVE(TNerveKikkuHit, TLiveActor) { }
