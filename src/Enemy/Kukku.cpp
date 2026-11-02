#include <Enemy/Kukku.hpp>
#include <Strategic/ObjModel.hpp>
#include <Map/PollutionManager.hpp>
#include <System/EmitterViewObj.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/MapData.hpp>
#include <M3DUtil/MActor.hpp>
#include <Strategic/LiveActor.hpp>
#include <Animal/AnimalBase.hpp>
#include <Strategic/Strategy.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <System/Particles.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Map/Map.hpp>
#include <Enemy/Graph.hpp>
#include <MSound/MSound.hpp>
#include <macros.h>

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

static const int cDropCoinNumTable[] = { 3, 3, 1, 2 };

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

TKukkuBall::TKukkuBall(MActor* actor, const char* name)
    : THitActor(name)
    , actor(actor)
    , unk6C(1)
    , unk7C(0)
{
}

void TKukkuBall::init()
{
	initHitActor(ACTOR_TYPE_KUKKU_BALL, 1, HIT_CATEGORY_PLAYER, 30.0f, 30.0f,
	             0.0f, 0.0f);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	onHitFilter(HIT_FILTER_NO_DAMAGE);
	TIdxGroupObj* group
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));

	group->getChildren().push_back(this);
	const ResTIMG* res = static_cast<const ResTIMG*>(
	    JKRFileLoader::getGlbResource("/scene/map/pollution/H_ma_rak.bti"));

	if (res)
		SMS_ChangeTextureAll(actor->getModel()->getModelData(), "K_name_dummy",
		                     *res);
}

void TKukkuBall::perform(u32 cue, JDrama::TGraphics* graphics)
{

	if (checkHitFilter(HIT_FILTER_NO_DAMAGE))
		return;

	if (cue & CUE_CALC_ANIM) {
		TPosition3f mtx;
		mtx.identity33();
		mtx.setTrans(mPosition);
		J3DModel* model = actor->getModel();
		model->setBaseScale(mScaling);
		MTXCopy(mtx, actor->getModel()->getBaseTRMtx());
		actor->getModel()->calc();
	}

	if (cue & CUE_MOVE) {
		checkHitActors();
		bind();
	}

	if (!checkHitFilter(HIT_FILTER_NO_DAMAGE))
		actor->perform(cue, graphics);
}

BOOL TKukkuBall::isDead() const
{
	return checkHitFilter(HIT_FILTER_NO_DAMAGE) && unk7C == 0 ? true : false;
}

void TKukkuBall::kill() { }

void TKukkuBall::startToMove(const JGeometry::TVec3<f32>& start,
                             const JGeometry::TVec3<f32>& end)
{
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

void TKukkuBall::moveObject() { }

TEnemyCoinUnit::TEnemyCoinUnit(int) { }

void TEnemyCoinUnit::init() { }

TMapObjBase* TEnemyCoinUnit::getUnusedItem() { return nullptr; }

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

	mMapObj = TMapObjBaseManager::newAndRegisterObj("mushroom1upR");

	initBalls();
	initCollision();
	initAnmSound();
	initParticle();
	unk1A8 = getModel()->getModelData()->unkB0->getIndex("center");
	reset();
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
	for (TKukkuBall** it = mBalls; it != mBalls + ARRAY_COUNT(mBalls); it++)
		(*it)->perform(cue, graphics);
}

void TKukku::behaveToWater(THitActor* sender)
{
	if (!isFalling() && isRecoveringGraph()) {
		mSpine->reset();
		mSpine->setNext(&TNerveKukkuFall::theNerve());
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

void TKukku::dropCoins()
{
	if (unk1B0 <= 10) {
		if (unk1B0 == 10 && mMapObj) {
			unk1B0++;
			mMapObj->appear();
			mMapObj->JSGSetTranslation(mPosition);
			mMapObj->setVelocityAndFlag10(0.0f, 0.0f, 0.0f);
		} else {
			int index = 4.0f * MsRandF();
			if (index < 1)
				index = 1;
			else if (index > 3)
				index = 3;
			int num = cDropCoinNumTable[index];

			JGeometry::TQuat4<f32> step;
			step.setEulerY((M_PI * 2) / num);
			JGeometry::TQuat4<f32> tilt;
			tilt.setEulerX(
			    M_PI * ((TKukkuParams*)getSaveParam2())->mDropAngleX.get());

			f32 c = ((TKukkuParams*)getSaveParam2())->mDropSpeed.get()
			        * MsCos(mRotation.y);
			f32 s = ((TKukkuParams*)getSaveParam2())->mDropSpeed.get()
			        * MsSin(mRotation.y);
			JGeometry::TVec3<f32> velocity;
			velocity.set(c, 0.0f, s);
			tilt.rotate(velocity);
			step.rotate(velocity);

			for (int i = 0; i < num; i++) {
				TMapObjBase* coin
				    = gpItemManager->makeObjAppear(ACTOR_TYPE_COIN);
				if (!coin)
					break;
				coin->appear();
				coin->JSGSetTranslation(mPosition);
				coin->setVelocityAndFlag10(velocity.x, velocity.y, velocity.z);
				if (++unk1B0 == 10)
					break;
				step.rotate(velocity);
			}
		}
	}
}

void TKukku::setDeadAnm() { changeBck("tori_dead"); }

void TKukku::setAfterDeadEffect()
{
	TSmallEnemy::setAfterDeadEffect();
	TSmallEnemyManager* manager = (TSmallEnemyManager*)getManager();
	gpPollution->stamp(manager->getUnk58(), mPosition.x, mPosition.y,
	                   mPosition.z, 1000.0f);
}

// UNUSED
void TKukku::getUnusedBall() { }

f32 TKukku::getWaterPowerY() const
{
	return ((TKukkuParams*)getSaveParam2())->mWaterPowerY.get();
}

void TKukku::getWaterDamageRate() const { }

void TKukku::changeBck(const char* name)
{
	getMActor()->setBck(name);
	setCurAnmSound();
}

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

BOOL TKukku::doHabataki() { }

void TKukku::doRecoverToCurPathNode()
{
	updateRotation();
	f32 momentum = ((TKukkuParams*)getSaveParam2())->mDropAngleX.get();
	calcMomentum(momentum);
}

void TKukku::doFlyToCurPathNode() { }

void TKukku::behaveHitTrample() { }

void TKukku::updateEffect() { }

void TKukku::initParticle()
{
	SMS_LoadParticle("/scene/tori/jpa/ms_cooc_ase.jpa", COOC_JPA_MS_ASE);
	SMS_LoadParticle("/scene/tori/jpa/ms_cooc_hane.jpa", COOC_JPA_MS_HANE);
}

void TKukku::initCollision()
{
	initHitActor(ACTOR_TYPE_KUKKU_BALL, 1, 0x80000000, 30.0f, 30.0f, 100.0f,
	             100.0f);
	offHitFilter(HIT_FILTER_NO_COLLISION);
}

void TKukku::initBalls()
{
	for (TKukkuBall** it = mBalls; it != mBalls + ARRAY_COUNT(mBalls); ++it) {
		*it = new TKukkuBall(mMActorKeeper->createMActor("torifun.bmd", 3));
		(*it)->init();
	}
}

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

DEFINE_NERVE(TNerveKukkuGraphWander, TLiveActor)
{
	TKukku* kukku = static_cast<TKukku*>(spine->getBody());

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
		kukku->mVelocity        = zero;
		kukku->unk124->mPrevIdx = -1;
		kukku->goToShortestNextGraphNode();

		if (kukku->getMActor()->checkCurAnm("tori_back", 0)) {
			kukku->changeBck("tori_back");
		} else {
			kukku->changeBck("tori_wait");
		}
	}

	if (kukku->isReachedToGoal()) {
		kukku->goToRandomNextGraphNode();
		if (!kukku->getMActor()->checkCurAnm("tori_wait", 0)) {
			kukku->changeBck("tori_wait");
		}
	}

	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario.sub(kukku->mPosition);
	toMario.y = 0.0f;
	f32 range = ((TKukkuParams*)kukku->getSaveParam())->mSearchRange.get();

	if (toMario.squared() < range * range) {
		if (kukku->unk1AC >= 0) {
			kukku->unk1AC--;
		} else {
			TMapObjBase* coin = nullptr;

			if (coin != nullptr) {
				JGeometry::TVec3<f32> shot(0.0f, 1.0f, 0.0f);
				f32 shootSpeed
				    = ((TKukkuParams*)kukku->getSaveParam())->mShootSpeed.get();
				shot.setLength(shootSpeed); // normalise, scale

				kukku->getModel()->getModelData()->getJointName()->getIndex(
				    "null_osen");

				JGeometry::TVec3<f32> pos = kukku->mPosition;
				coin->onHitFilter(HIT_FILTER_NO_COLLISION);

				// coin->onHitFilter(...);   // clear bit 0 at +0x64
				// coin->clear bit 0 at +0x6C;
				coin->mPosition = pos;
				coin->mVelocity = shot; // +0x70

				kukku->unk1AC = ((TKukkuParams*)kukku->getSaveParam())
				                    ->mShootInterval.get();
				SMSGetMSound()->startSoundActor(0x28f3, &kukku->mPosition, 0,
				                                nullptr, 0, 4);
			}
		}
	}

	kukku->updateRotation();

	f32 speed = ((TKukkuParams*)kukku->getSaveParam())->mMarchSpeed.get();
	kukku->mVelocity = kukku->calcMomentum(speed);
	return false;
}

DEFINE_NERVE(TNerveKukkuFall, TLiveActor)
{
	TKukku* kukku = static_cast<TKukku*>(spine->getBody());

	if (spine->getTime() == 0) {
		kukku->changeBck("tori_back");
		kukku->mVelocity.zero();
		kukku->dropCoins();
	}

	if (kukku->isAirborne()) {
		SMS_EasyEmitParticle(PARTICLE_MS_POPO_BOMB_A, &kukku->mPosition,
		                     nullptr, JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		SMS_EasyEmitParticle(PARTICLE_MS_POPO_BOMB_B, &kukku->mPosition,
		                     nullptr, JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		spine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());
		return true;
	}

	if (kukku->doHabataki()) {
		spine->pushAfterCurrent(&TNerveKukkuRecoverGraph::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveKukkuPostFall, TLiveActor)
{
	TKukku* kukku = static_cast<TKukku*>(spine->getBody());

	if (spine->getTime() == 0) {
		kukku->changeBck("tori_wait");
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
		kukku->changeBck("tori_back");
	}

	if (spine->getTime()
	    > ((TKukkuParams*)kukku->getSaveParam2())->mHabatakiTimer.get()) {
		spine->pushNerve(&TNerveKukkuGraphWander::theNerve());
		return true;
	}

	kukku->doRecoverToCurPathNode();

	return false;
}

DEFINE_NERVE(TNerveKikkuHit, TLiveActor)
{
	TKukku* kukku = (TKukku*)spine->getBody();
	if (spine->getTime() == 0) {
		kukku->changeBck("tori_hit");
	}

	if (kukku->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveKukkuGraphWander::theNerve());
		return true;
	}
	return false;
}
