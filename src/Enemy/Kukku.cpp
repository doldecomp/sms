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

TKukkuBall::TKukkuBall(MActor* actor)
    : THitActor("クック玉")
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
	if (checkHitFilter(HIT_FILTER_NO_DAMAGE) && unk7C == 0)
		return true;
	return false;
}

void TKukkuBall::kill()
{
	onHitFilter(HIT_FILTER_NO_COLLISION);
	unk6C |= 0x1;
	gpPollution->pollute(mPosition.x, mPosition.y, mPosition.z, 500.0f);
}

void TKukkuBall::startToMove(const JGeometry::TVec3<f32>& pos,
                             const JGeometry::TVec3<f32>& vel)
{
	offHitFilter(HIT_FILTER_NO_COLLISION);
	unk6C &= ~1;
	mPosition = pos;
	mVelocity = vel;
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
    , mMapObj(nullptr)
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
		if (mGroundPlane) {
			normal.set(getGroundPlane()->getNormal());
			normal.normalize();
		} else {
			normal.set(0.0f, 1.0f, 0.0f);
		}

		JGeometry::TQuat4<f32> yaw;
		yaw.setEulerY(mRotation.y * 0.017453294f);
		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		JGeometry::TQuat4<f32> tilt;
		tilt.setRotate(up, normal, 1.0f);
		tilt.mul(tilt, yaw);

		TPosition3f mtx;
		mtx.setQT(tilt, mPosition);
		PSMTXCopy(mtx, getModel()->getBaseTRMtx());
		getModel()->setBaseScale(mScaling);
	} else {
		TSpineEnemy::calcRootMatrix();
	}

	if (isFalling()) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x18d, getModel()->getAnmMtx(unk1A8), 1, this);
	}

	if (getMActor()->checkCurAnm("tori_back", 0)) {
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
	JGeometry::TVec3<f32> toTarget = mCurrentTarget.getPoint();
	toTarget.sub(mPosition);

	f32 dist = toTarget.length();

	if (dist < 100.0f) {
		f32 marchSpeed = getKukkuParams()->mMarchSpeed.get();
		f32 turnSpeed  = getKukkuParams()->mTurnSpeed.get();

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
			tilt.setEulerX(M_PI * getKukkuParams()->mDropAngleX.get());

			JGeometry::TVec3<f32> velocity
			    = polarXZ(mRotation.y, getKukkuParams()->mDropSpeed.get());
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
TKukkuBall* TKukku::getUnusedBall()
{
	for (TKukkuBall** it = mBalls; it != mBalls + ARRAY_COUNT(mBalls); ++it)
		if ((*it)->isDead())
			return *it;
	return nullptr;
}

f32 TKukku::getWaterPowerY() const
{
	return getKukkuParams()->mWaterPowerY.get();
}

void TKukku::getWaterDamageRate() const { }

void TKukku::changeBck(const char* name)
{
	getMActor()->setBck(name);
	setCurAnmSound();
}

BOOL TKukku::isFindOutMario() const
{
	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario -= mPosition;
	toMario.y = 0.0f;
	f32 range = getKukkuParams()->mSearchRange.get();
	return toMario.squared() < range * range;
}

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
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveKukkuFall::theNerve()
	       || nerve == &TNerveKukkuPostFall::theNerve();
}

void TKukku::shotBall()
{
	TKukkuBall* ball = getUnusedBall();
	if (ball) {
		JGeometry::TVec3<f32> velocity(0.0f, 1.0f, 0.0f);
		velocity.setLength(getKukkuParams()->mShootSpeed.get());
		getModel()->getModelData()->getJointName()->getIndex("null_osen");
		ball->startToMove(mPosition, velocity);
		unk1AC = getKukkuParams()->mShootInterval.get();
		SMSGetMSound()->startSoundActor(MSD_SE_EN_TORI_SHIT, &mPosition, 0,
		                                nullptr, 0, 4);
	}
};

void TKukku::decideFlyingAnm()
{
	if (getMActor()->checkCurAnm("tori_back", 0))
		changeBck("tori_back");
	else
		changeBck("tori_wait");
}

BOOL TKukku::doHabataki()
{
	JGeometry::TVec3<f32> velocity = mVelocity;
	velocity.scale(getKukkuParams()->mAirFric.get());
	velocity.y += getKukkuParams()->mUpperVelocityY.get();
	bool stopped = false;
	if (-0.1f < velocity.y) {
		stopped    = true;
		velocity.y = 0.0f;
	}
	mVelocity = velocity;
	return stopped;
}

void TKukku::doRecoverToCurPathNode()
{
	updateRotation();
	mPositionDelta = calcMomentum(getKukkuParams()->mDropAngleX.get());
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
	initHitActor(ACTOR_TYPE_KUKKU_BALL, 1, HIT_CATEGORY_PLAYER, 30.0f, 30.0f,
	             100.0f, 100.0f);
	offHitFilter(HIT_FILTER_NO_COLLISION);
}

void TKukku::initBalls()
{
	TKukkuBall** end = mBalls + ARRAY_COUNT(mBalls);
	for (TKukkuBall** it = mBalls; it != end; ++it) {
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
		kukku->decideFlyingAnm();
	}

	if (kukku->isReachedToGoal()) {
		kukku->goToRandomNextGraphNode();
		if (!kukku->getMActor()->checkCurAnm("tori_wait", 0)) {
			kukku->changeBck("tori_wait");
		}
	}

	if (kukku->isFindOutMario()) {
		if (kukku->getUnk1AC() >= 0)
			kukku->unk1AC--;
		else
			kukku->shotBall();
	}

	kukku->doRecoverToCurPathNode();
	return false;
}

DEFINE_NERVE(TNerveKukkuFall, TLiveActor)
{
	TKukku* kukku = static_cast<TKukku*>(spine->getBody());

	if (spine->getTime() == 0) {
		kukku->changeBck("tori_back");
		J3DFrameCtrl* ctrl = kukku->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		ctrl->setRate(2.0f * SMSGetAnmFrameRate());
		kukku->mVelocity
		    = JGeometry::TVec3<f32>(0.0f, -kukku->getWaterPowerY(), 0.0f);
		kukku->dropCoins();
	}

	if (!kukku->isAirborne()) {
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
		kukku->changeBck("tori_back");
		kukku->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
	}

	if (!kukku->checkCurAnmEnd(0)
	    || kukku->getKukkuParams()->mHabatakiTimer.get() >= spine->getTime()) {
		return false;
	}

	spine->pushAfterCurrent(&TNerveKukkuGraphWander::theNerve());
	return true;
}

DEFINE_NERVE(TNerveKukkuRecoverGraph, TLiveActor)
{
	TKukku* kukku = static_cast<TKukku*>(spine->getBody());

	if (spine->getTime() == 0) {
		kukku->changeBck("tori_back");
		kukku->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
	}

	if (kukku->getKukkuParams()->mHabatakiTimer.get() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveKukkuGraphWander::theNerve());
		return true;
	}

	kukku->doRecoverToCurPathNode(); // calcMomentum here gets out-of-line...
	                                 // double layer?

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
