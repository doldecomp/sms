#include <Enemy/BathtubKiller.hpp>
#include <Enemy/Conductor.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Enemy/EffectObj.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <System/FlagManager.hpp>
#include <System/Particles.hpp>
#include <stdlib.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const char* bathtubkiller_bastable[] = {
	"/scene/bathtubkiller/bas/bathtubdownkiller_down1.bas",
	nullptr,
	nullptr,
};

TBathtubKillerPersonality::TBathtubKillerPersonality() { }

void TBathtubKillerPersonality::makeFast(const TBathtubKillerParams* params)
{
	unk0  = params->fastAccelerationQuatRate.get();
	unk4  = params->fastChaseAcceleration.get();
	unk8  = params->fastChaseSpeed.get();
	unkC  = params->fastInitialSpeed.get();
	unk10 = params->fastDeadPeriod.get();
}

void TBathtubKillerPersonality::makeShine(const TBathtubKillerParams* params)
{
	unk0  = params->shineAccelerationQuatRate.get();
	unk4  = params->shineChaseAcceleration.get();
	unk8  = params->shineChaseSpeed.get();
	unkC  = params->shineInitialSpeed.get();
	unk10 = params->shineDeadPeriod.get();
}

void TBathtubKillerPersonality::makeNormal(const TBathtubKillerParams* params)
{
	unk0  = params->mSLAccelerationQuatRate.get();
	unk4  = params->mSLChaseAcceleration.get();
	unk8  = params->mSLChaseSpeed.get();
	unkC  = params->mSLInitialSpeed.get();
	unk10 = params->mSLDeadPeriod.get();
}

TBathtubKillerParams::TBathtubKillerParams(const char* prm)
    : TSmallEnemyParams(prm)
    , PARAM_INIT(fastAccelerationQuatRate, 0.05f)
    , PARAM_INIT(fastChaseAcceleration, 0.2f)
    , PARAM_INIT(fastChaseSpeed, 15.0f)
    , PARAM_INIT(fastInitialSpeed, 50.0f)
    , PARAM_INIT(fastDeadPeriod, 1800)
    , PARAM_INIT(shineAccelerationQuatRate, 0.05f)
    , PARAM_INIT(shineChaseAcceleration, 0.2f)
    , PARAM_INIT(shineChaseSpeed, 15.0f)
    , PARAM_INIT(shineInitialSpeed, 50.0f)
    , PARAM_INIT(shineDeadPeriod, 1800)
    , PARAM_INIT(mushroomProbability, 0.05f)
    , PARAM_INIT(mSLColorChangeRateDelta, 0.05f)
    , PARAM_INIT(mSLAccelerationQuatRate, 0.05f)
    , PARAM_INIT(mSLChaseAcceleration, 0.2f)
    , PARAM_INIT(mSLChaseSpeed, 15.0f)
    , PARAM_INIT(mSLInitialSpeed, 20.0f)
    , PARAM_INIT(mSLDeadPeriod, 1800)
    , PARAM_INIT(mSLStraightDistance, 50.0f)
    , PARAM_INIT(mSLChaseMinY, 50.0f)
    , PARAM_INIT(mSLChaseMaxY, 100.0f)
    , PARAM_INIT(mSLAboidDistanceY, 0.05f)
    , PARAM_INIT(mSLAboidDistance, 500.0f)
    , PARAM_INIT(mSLChaseDistanceY, 200.0f)
    , PARAM_INIT(mSLChaseDistance, 1000.0f)
    , PARAM_INIT(mSLTrampleVelocity, 100.0f)
    , PARAM_INIT(mSLFlyingSpeedMax, 200.0f)
    , PARAM_INIT(mSLFlyingGravityY, 0.1f)
    , PARAM_INIT(mSLBombRange, 300.0f)
    , PARAM_INIT(aboidAngle, 10.0f)
    , PARAM_INIT(mSLChaseStraightPeriod, 360)
    , PARAM_INIT(mSLSmokeInterval, 3)
    , PARAM_INIT(mSLLaunchingPeriod, 360)
    , PARAM_INIT(noCollisionAmongKillers, 360)
{
	TParams::load(mPrmPath);

	fastAccelerationQuatRate.set(0.03f);
	fastChaseAcceleration.set(0.1f);
	fastChaseSpeed.set(10.0f);
	fastInitialSpeed.set(14.0f);
	fastDeadPeriod.set(720);
	shineAccelerationQuatRate.set(0.03f);
	shineChaseAcceleration.set(0.03f);
	shineChaseSpeed.set(7.0f);
	shineInitialSpeed.set(12.0f);
	shineDeadPeriod.set(2400);
	mSLAccelerationQuatRate.set(0.03f);
	mSLChaseAcceleration.set(0.1f);
	mSLChaseSpeed.set(8.0f);
	mSLInitialSpeed.set(12.0f);
	mSLDeadPeriod.set(1440);
	mSLColorChangeRateDelta.set(0.16f);
	mSLStraightDistance.set(400.0f);
	mSLChaseMinY.set(50.0f);
	mSLChaseMaxY.set(100.0f);
	mSLAboidDistanceY.set(100.0f);
	mSLAboidDistance.set(100.0f);
	mSLChaseDistanceY.set(400.0f);
	mSLDamageRadius.set(120);
	mSLDamageHeight.set(100);
	mSLAttackRadius.set(100);
	mSLAttackHeight.set(90);
	mSLTrampleVelocity.set(500.0f);
	mSLFlyingSpeedMax.set(200.0f);
	mSLFlyingGravityY.set(0.06f);
	mSLBombRange.set(500.0f);
	aboidAngle.set(5.0f);
	mSLChaseStraightPeriod.set(420);
	mSLSmokeInterval.set(4);
	mSLLaunchingPeriod.set(240);
	noCollisionAmongKillers.set(480);
	mushroomProbability.set(0.3f);
}

TBathtubKiller::TBathtubKiller(const char* name)
    : TSmallEnemy(name)
    , unk1CC(0)
{
}

void TBathtubKiller::init(TLiveManager* manager)
{
	TSmallEnemy::init(manager);
	mActorType = 0x08000024;
	unk150     = 0x11;
	onLiveFlag(LIVE_FLAG_UNK10);
	onLiveFlag(LIVE_FLAG_DEAD);
	onLiveFlag(LIVE_FLAG_UNK8);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	unk194 = 0;
	resetBathtubKiller();
}

void TBathtubKiller::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("bathtubkiller_model1.bmd", 0);
	mMActorKeeper->createMActor("bathtubdownkiller_model1.bmd", 3);

	s32 noseMatIdx = getActorKeeper()
	                     ->getMActor("bathtubkiller_model1.bmd")
	                     ->getModel()
	                     ->getModelData()
	                     ->getMaterialName()
	                     ->getIndex("_nosemat1");
	s32 eyesMatIdx = getActorKeeper()
	                     ->getMActor("bathtubkiller_model1.bmd")
	                     ->getModel()
	                     ->getModelData()
	                     ->getMaterialName()
	                     ->getIndex("_eyesmat1");
	s32 bodyMatIdx = getActorKeeper()
	                     ->getMActor("bathtubkiller_model1.bmd")
	                     ->getModel()
	                     ->getModelData()
	                     ->getMaterialName()
	                     ->getIndex("_body1");

	SMS_InitPacket_OneTevColor(
	    getActorKeeper()->getMActor("bathtubkiller_model1.bmd")->getModel(),
	    noseMatIdx, GX_TEVREG0, &mNoseColor);
	SMS_InitPacket_OneTevColor(
	    getActorKeeper()->getMActor("bathtubkiller_model1.bmd")->getModel(),
	    eyesMatIdx, GX_TEVREG0, &mEyesColor);
	SMS_InitPacket_OneTevColor(
	    getActorKeeper()->getMActor("bathtubkiller_model1.bmd")->getModel(),
	    bodyMatIdx, GX_TEVREG0, &mBodyColor);
	SMS_InitPacket_OneTevColor(
	    getActorKeeper()->getMActor("bathtubdownkiller_model1.bmd")->getModel(),
	    0, GX_TEVREG0, &mBaseColor);
}

void TBathtubKiller::reset()
{
	TSmallEnemy::reset();
	offLiveFlag(LIVE_FLAG_DEAD);
	offLiveFlag(LIVE_FLAG_UNK8);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	offHitFlag(HIT_FLAG_CANNOT_ATTACK);
	offHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	resetBathtubKiller();
}

void TBathtubKiller::resetBathtubKiller()
{
	char trash[0x18];
	mSpine->initWith(&TNerveBathtubKillerWander::theNerve());
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	unk208 = 0;
	unk20C = 0;
	unk210 = 0;
	unk214 = 0;
	unk218 = 0;
	mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	mVelocity.set(0.0f, 0.0f, 0.0f);
	unk1BC.set(0.0f, 0.0f, 0.0f);
	unk21C = 0;
	unk1D4 = 0;
	if (unk194 == 1) {
		mBodyColor.r = 50;
		mBodyColor.g = 70;
		mBodyColor.b = 160;
		mBodyColor.a = 0;
		mNoseColor   = mBodyColor;
		mEyesColor   = mBodyColor;
		mBaseColor   = mBodyColor;
		unk198.makeShine(getSaveParam2());
	} else {
		mBodyColor.r = 0;
		mBodyColor.g = 0;
		mBodyColor.b = 0;
		mBodyColor.a = 0;
		mNoseColor   = mBodyColor;
		mEyesColor   = mBodyColor;
		mBaseColor   = mBodyColor;
		if (unk194 == 2) {
			unk198.makeFast(getSaveParam2());
		} else {
			unk198.makeNormal(getSaveParam2());
		}
	}
	unk1FC = 0.0f;
	unk1F8 = getSaveParam2()->mSLColorChangeRateDelta.get();
	unk208 = unk198.unk10;
	unk20C = getSaveParam2()->mSLLaunchingPeriod.get();
	unk214 = getSaveParam2()->noCollisionAmongKillers.get();
	unk200 = getSaveParam2()->mSLChaseMinY.get();
	unk204 = getSaveParam2()->mSLChaseMaxY.get();
	if (unk194 == 2) {
		f32 random      = 4.0f * MsRandF();
		f32 offset      = 0.0f;
		int randomIndex = (int)random;
		if (randomIndex == 0) {
			offset = 120.0f;
		} else if (randomIndex == 1) {
			offset = 240.0f;
		}
		unk200 += offset;
		unk204 += offset;
	}
}

void TBathtubKiller::generateItemBathtubKiller()
{
	if (unk194 != 1)
		return;

	TMapObjBase* item              = nullptr;
	TBathtubKillerManager* manager = (TBathtubKillerManager*)mManager;
	s32 shines = TFlagManager::getInstance()->getFlag(0x20001);

	if (SMS_GetMarioWaterGun()->mCurrentWater == 0) {
		item = gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z, 0x20000002, true);
	} else if (manager->unk60 == shines && manager->unk69 < 7) {
		JGeometry::TVec3<f32> pos = mPosition;
		if (!manager->unk64 || manager->unk64->checkLiveFlag(LIVE_FLAG_DEAD))
			manager->unk64 = gpItemManager->makeObjAppear(pos.x, pos.y, pos.z,
			                                              0x20000005, true);
		manager->unk69 += 1;
	} else if (shines <= manager->unk60 + 1) {
		if (unk1CC->getNumGripsDead() == 3 && manager->unk68 == 0) {
			JGeometry::TVec3<f32> pos = getPosition();
			if (!manager->unk64
			    || manager->unk64->checkLiveFlag(LIVE_FLAG_DEAD))
				manager->unk64 = gpItemManager->makeObjAppear(
				    pos.x, pos.y, pos.z, 0x20000005, true);
			manager->unk68 = 1;
		}
	}

	if (!item)
		item = gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z, 0x20000002, true);

	if (!item)
		return;

	if (item->getActorType() == 0x20000002) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emit(
		    PARTICLE_MS_ENM_DISAP_A_W, &item->mPosition, 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(item->mScaling);

		emitter = gpMarioParticleManager->emit(PARTICLE_MS_ENM_DISAP_B,
		                                       &item->mPosition, 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(item->mScaling);
	}
}

void TBathtubKiller::killBathtubKiller() { }

void TBathtubKiller::breakBathtubKiller()
{
	bool dying = mSpine->getCurrentNerve()
	             == &TNerveBathtubKillerExplosion::theNerve();
	if (!dying)
		dying = mSpine->getCurrentNerve()
		        == &TNerveBathtubKillerBreak::theNerve();

	if (!dying)
		mSpine->pushNerve(&TNerveBathtubKillerBreak::theNerve());
}

void TBathtubKiller::explodeBathtubKiller() { }

void TBathtubKiller::bind() { }

void TBathtubKiller::perform(u32 cue, JDrama::TGraphics* graphics)
{
	// TODO: only the bathtub lookup is reconstructed; the cue handling that
	// follows it in the object is still missing.
	TSmallEnemy::perform(cue, graphics);

	if (unk1CC == nullptr)
		unk1CC
		    = static_cast<TBathtub*>(JDrama::TNameRefGen::search("バスタブ"));
}

void TBathtubKiller::makeNoseColor() { }

f32 TBathtubKiller::getBathtubY() { return 0.0f; }

void TBathtubKiller::makeInitialVelocity(JGeometry::TVec3<f32>) { }

void TBathtubKiller::moveParabolic() { }

void TBathtubKiller::moveChasing() { }

void TBathtubKiller::moveStraight() { }

void TBathtubKiller::makeVelocityQuat() { }

void TBathtubKiller::makeAccelerationQuat() { }

void TBathtubKiller::makeQuat(JGeometry::TVec3<f32> axis, f32 moveAmountY,
                              f32 moveAmountX)
{
	JGeometry::TVec3<f32> normAxis = axis;
	normAxis.normalize();

	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);

	JGeometry::TVec3<f32> up;
	mQuat.getYDir(up);

	JGeometry::TQuat4<f32> steer;
	steer.setRotate(forward, normAxis, moveAmountY);
	mQuat.mul(steer);

	// Y-axis rotation
	JGeometry::TVec3<f32> right;
	right.cross(forward, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
	if (right.length() > 0.0f) {
		right.normalize();

		JGeometry::TQuat4<f32> tiltQuat;
		tiltQuat.setRotate(right, M_PI / 2.0f);

		JGeometry::TVec3<f32> curUp;
		tiltQuat.rotate(forward, curUp);

		steer.setRotate(up, curUp, moveAmountX);
		mQuat.mul(steer);
	}

	mQuat.normalize();
}

void TBathtubKiller::makeScrewQuat(JGeometry::TVec3<f32>, f32, f32) { }

f32 TBathtubKiller::getGravityY() const
{
	return getSaveParam2()->mSLFlyingGravityY.get();
}

void TBathtubKiller::calcRootMatrix()
{
	TPosition3f pos;
	pos.setQT(mQuat, mPosition);
	getModel()->setBaseScale(mScaling);
	getModel()->setBaseTRMtx(pos);
}

BOOL TBathtubKiller::receiveMessage(THitActor*, u32) { return false; }

void TBathtubKiller::attackToMario() { }

bool TBathtubKiller::isCollidMove(THitActor*) { return false; }

void TBathtubKiller::behaveToWater(THitActor*) { breakBathtubKiller(); }

const char** TBathtubKiller::getBasNameTable() const
{
	return bathtubkiller_bastable;
}

void TBathtubKiller::setNormalBathtubKillerAnm() { }

void TBathtubKiller::setChaseBathtubKillerAnm() { }

void TBathtubKiller::setStraightBathtubKillerAnm() { }

void TBathtubKiller::setDeadBathtubKillerAnm()
{
	mMActor = getActorKeeper()->getMActor("bathtubdownkiller_model1.bmd");
	setBckAnm(0);
	mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	unk1BC.set(0.0f, 0.0f, 0.0f);
	// TODO: the temporary sits 4 bytes lower than the original in both death
	// nerves.
	mVelocity = JGeometry::TVec3<f32>(0, 0, 0);
	onLiveFlag(LIVE_FLAG_UNK8);
	mNoseColor = mBodyColor;
}

void TBathtubKiller::updateTimers() { }

bool TBathtubKiller::isAttackable() { return false; }

bool TBathtubKiller::isAboided() { return false; }

bool TBathtubKiller::canChase() { return false; }

void TBathtubKiller::generateExplosion() { }

DEFINE_NERVE(TNerveBathtubKillerWander, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveBathtubKillerChase, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveBathtubKillerChaseStraight, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveBathtubKillerStraight, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0) {
		self->mMActor
		    = self->getActorKeeper()->getMActor("bathtubkiller_model1.bmd");
		self->setBckAnm(2);
	}
	if (self->unk218 <= 0)
		self->offHitFlag(HIT_FLAG_NO_COLLISION);

	JGeometry::TVec3<f32> vec;
	self->mQuat.getZDir(vec);
	vec.normalize();
	vec.scale(self->unk198.unk8);
	self->mVelocity.set(vec);
	self->makeQuat(self->mVelocity, self->unk198.unk0, 0.1f);
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerBreak, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setDeadBathtubKillerAnm();
		self->generateItemBathtubKiller();
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (self->checkCurAnmEnd(0)) {
		self->unk21C = 0;
		self->onLiveFlag(LIVE_FLAG_DEAD);
		self->stopAnmSound();
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerExplosion, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setDeadBathtubKillerAnm();

		TSpineEnemy* effectBase = gpConductor->makeOneEnemyAppear(
		    self->mPosition, "エフェクト爆発マネージャー", 1);
		if (effectBase != nullptr) {
			TEffectExplosion* effect = (TEffectExplosion*)effectBase;
			effect->generate(self->mPosition, self->mScaling);
		}
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (self->checkCurAnmEnd(0)) {
		self->unk21C = 0;
		self->onLiveFlag(LIVE_FLAG_DEAD);
		self->stopAnmSound();
		return TRUE;
	}
	return FALSE;
}

TBathtubKillerManager::TBathtubKillerManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TBathtubKillerManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TBathtubKillerParams("/enemy/bathtubkiller.prm");
}

void TBathtubKillerManager::loadAfter()
{
	TSmallEnemyManager::loadAfter();

	TMapObjBaseManager::newAndRegisterObj("mushroom1up");
	TMapObjBaseManager::newAndRegisterObj("mushroom1up");

	unk60 = TFlagManager::getInstance()->getFlag(0x20001);
	unk64 = nullptr;
	unk68 = 0;
	unk69 = 0;

	// TODO: the original reads unk38 here and drops the result.
	static const char* loopFilenames[1] = {
		"/scene/map/map/ms_kp_kill_smoke.jpa",
	};
	for (int i = 0; i < 1; ++i)
		SMS_LoadParticle(loopFilenames[i], 0x1BD + i);
}

void TBathtubKillerManager::generateMushroom(JGeometry::TVec3<f32>) { }

int TBathtubKillerManager::countActiveKillers()
{
	int count = 0;
	for (int i = 0; i < getActiveObjNum(); ++i) {
		if (!getObj(i)->checkLiveFlag(LIVE_FLAG_DEAD))
			count += 1;
	}
	return count;
}

int TBathtubKillerManager::countActiveShineKillers() { return 0; }

void TBathtubKillerManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "bathtubkiller_model1.bmd", 0x50230000, 0 },
		{ "bathtubdownkiller_model1.bmd", 0x50210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TBathtubKillerManager::createEnemyInstance()
{
	return new TBathtubKiller;
}
