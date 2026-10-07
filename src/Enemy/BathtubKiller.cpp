#include <Enemy/BathtubKiller.hpp>
#include <Enemy/Conductor.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/KoopaJr.hpp>
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
#include <Strategic/ActorTypes.hpp>
#include <Map/Map.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
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
	mAccelerationQuatRate = params->fastAccelerationQuatRate.get();
	mChaseAcceleration    = params->fastChaseAcceleration.get();
	mChaseSpeed           = params->fastChaseSpeed.get();
	mInitialSpeed         = params->fastInitialSpeed.get();
	mDeadPeriod           = params->fastDeadPeriod.get();
}

void TBathtubKillerPersonality::makeShine(const TBathtubKillerParams* params)
{
	mAccelerationQuatRate = params->shineAccelerationQuatRate.get();
	mChaseAcceleration    = params->shineChaseAcceleration.get();
	mChaseSpeed           = params->shineChaseSpeed.get();
	mInitialSpeed         = params->shineInitialSpeed.get();
	mDeadPeriod           = params->shineDeadPeriod.get();
}

void TBathtubKillerPersonality::makeNormal(const TBathtubKillerParams* params)
{
	mAccelerationQuatRate = params->mSLAccelerationQuatRate.get();
	mChaseAcceleration    = params->mSLChaseAcceleration.get();
	mChaseSpeed           = params->mSLChaseSpeed.get();
	mInitialSpeed         = params->mSLInitialSpeed.get();
	mDeadPeriod           = params->mSLDeadPeriod.get();
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
    , mBathtub(0)
{
}

void TBathtubKiller::init(TLiveManager* manager)
{
	TSmallEnemy::init(manager);
	mActorType = ACTOR_TYPE_BATHTUB_KILLER;
	unk150     = 0x11;
	onLiveFlag(LIVE_FLAG_UNK10);
	onLiveFlag(LIVE_FLAG_DEAD);
	onLiveFlag(LIVE_FLAG_UNK8);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	mKillerType = 0;
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
	offHitFilter(HIT_FILTER_NO_COLLISION);
	offHitFilter(HIT_FILTER_NO_ATTACK);
	offHitFilter(HIT_FILTER_NO_DAMAGE);
	resetBathtubKiller();
}

void TBathtubKiller::resetBathtubKiller()
{
	mSpine->initWith(&TNerveBathtubKillerWander::theNerve());
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	mLifetimeTimer          = 0;
	mLaunchTimer            = 0;
	mChaseStraightTimer     = 0;
	mNoKillerCollisionTimer = 0;
	mNoCollisionTimer       = 0;
	mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	mVelocity.set(0.0f, 0.0f, 0.0f);
	mAcceleration.set(0.0f, 0.0f, 0.0f);
	mHitMario     = 0;
	mSmokeCounter = 0;
	if (mKillerType == 1) {
		mBodyColor.r = 50;
		mBodyColor.g = 70;
		mBodyColor.b = 160;
		mBodyColor.a = 0;
		mNoseColor   = mBodyColor;
		mEyesColor   = mBodyColor;
		mBaseColor   = mBodyColor;
		mPersonality.makeShine(getSaveParam2());
	} else {
		mBodyColor.r = 0;
		mBodyColor.g = 0;
		mBodyColor.b = 0;
		mBodyColor.a = 0;
		mNoseColor   = mBodyColor;
		mEyesColor   = mBodyColor;
		mBaseColor   = mBodyColor;
		if (mKillerType == 2) {
			mPersonality.makeFast(getSaveParam2());
		} else {
			mPersonality.makeNormal(getSaveParam2());
		}
	}
	mColorPhase             = 0.0f;
	mColorChangeRate        = getSaveParam2()->mSLColorChangeRateDelta.get();
	mLifetimeTimer          = mPersonality.mDeadPeriod;
	mLaunchTimer            = getSaveParam2()->mSLLaunchingPeriod.get();
	mNoKillerCollisionTimer = getSaveParam2()->noCollisionAmongKillers.get();
	mMinChaseHeight         = getSaveParam2()->mSLChaseMinY.get();
	mMaxChaseHeight         = getSaveParam2()->mSLChaseMaxY.get();
	if (mKillerType == 2) {
		f32 random      = 4.0f * MsRandF();
		f32 offset      = 0.0f;
		int randomIndex = (int)random;
		if (randomIndex == 0) {
			offset = 120.0f;
		} else if (randomIndex == 1) {
			offset = 240.0f;
		}
		mMinChaseHeight += offset;
		mMaxChaseHeight += offset;
	}
}

void TBathtubKiller::generateItemBathtubKiller()
{
	if (mKillerType != 1)
		return;

	TMapObjBase* item              = nullptr;
	TBathtubKillerManager* manager = (TBathtubKillerManager*)mManager;
	s32 lives = TFlagManager::getInstance()->getFlag(MSF_LIFE_COUNT);

	if (SMS_GetMarioWaterGun()->mCurrentWater == 0) {
		item = gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z,
		                                    ACTOR_TYPE_BOTTLE_LARGE, true);
	} else if (manager->mInitialLifeCount == lives
	           && manager->mEarlyMushroomRequests < 7) {
		manager->generateMushroom(mPosition);
		manager->mEarlyMushroomRequests += 1;
	} else if (lives <= manager->mInitialLifeCount + 1) {
		if (mBathtub->getNumGripsDead() == 3
		    && manager->mFinalMushroomRequested == 0) {
			manager->generateMushroom(getPosition());
			manager->mFinalMushroomRequested = 1;
		}
	}

	if (!item)
		item = gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z,
		                                    ACTOR_TYPE_BOTTLE_LARGE, true);

	if (!item)
		return;

	if (item->getActorType() == ACTOR_TYPE_BOTTLE_LARGE) {
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

void TBathtubKiller::killBathtubKiller()
{
	mHitMario = 0;
	onLiveFlag(LIVE_FLAG_DEAD);
	stopAnmSound();
}

void TBathtubKiller::breakBathtubKiller()
{
	bool dying
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();

	if (!dying)
		mSpine->pushNerve(&TNerveBathtubKillerBreak::theNerve());
}

void TBathtubKiller::explodeBathtubKiller()
{
	bool dying
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();
	if (!dying)
		mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
}

void TBathtubKiller::bind()
{
	JGeometry::TVec3<f32> position = mPosition;
	position.add(mPositionDelta);
	position.add(mVelocity);
	mVelocity.add(mAcceleration);
	bool dying
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();
	if (!dying) {
		mGroundHeight = gpMap->checkGround(position.x, position.y + mHeadHeight,
		                                   position.z, &mGroundPlane);
		mGroundHeight += 1.0f;
		if (position.y <= 0.05f + mGroundHeight) {
			explodeBathtubKiller();
			mAcceleration.set(0.0f, 0.0f, 0.0f);
			mVelocity.set(mAcceleration);
			position.y = mGroundHeight;
		}
		if (gpMap->isTouchedOneWallAndMoveXZ(&position.x,
		                                     position.y + mHeadHeight,
		                                     &position.z, mBodyRadius))
			explodeBathtubKiller();
	}
	JGeometry::TVec3<f32> delta = position;
	delta.sub(mPosition);
	mPositionDelta = delta;
}

void TBathtubKiller::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);

	if (mBathtub == nullptr)
		mBathtub
		    = static_cast<TBathtub*>(JDrama::TNameRefGen::search("バスタブ"));
	if ((cue & 1) && !checkLiveFlag(LIVE_FLAG_DEAD)) {
		updateTimers();
		if (mLifetimeTimer <= 0)
			explodeBathtubKiller();
		if (!gpMap->isInArea(mPosition.x, mPosition.z))
			killBathtubKiller();
		if (mBathtub->unk29A)
			killBathtubKiller();
	}
	if ((cue & 2) && !checkLiveFlag(LIVE_FLAG_DEAD)) {
		bool dying = mSpine->getCurrentNerve()
		                 == &TNerveBathtubKillerExplosion::theNerve()
		             || mSpine->getCurrentNerve()
		                    == &TNerveBathtubKillerBreak::theNerve();
		if (!dying) {
			makeNoseColor();
			if (++mSmokeCounter >= getSaveParam2()->mSLSmokeInterval.get()) {
				mSmokeCounter = 0;
				mSmokeMatrix.setQuat(mQuat);
				mSmokeMatrix.setTrans(mPosition);
				gpMarioParticleManager->emitAndBindToMtxPtr(0x1BD, mSmokeMatrix,
				                                            1, this);
			}
			f32 distance = mPosition.distance(SMS_GetMarioPos());
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_EN_KILLER_FLY, &mPosition, nullptr, distance, 0, 0,
			    nullptr, 4, 0);
		}
	}
}

void TBathtubKiller::makeNoseColor()
{
	if (mKillerType == 2) {
		mColorPhase += mColorChangeRate;
		if (mColorPhase > 1.0f) {
			mColorPhase      = 1.0f;
			mColorChangeRate = -getSaveParam2()->mSLColorChangeRateDelta.get();
		}
		if (mColorPhase < 0.0f) {
			mColorPhase      = 0.0f;
			mColorChangeRate = getSaveParam2()->mSLColorChangeRateDelta.get();
		}
		mNoseColor.r = (u8)(255.0f * mColorPhase);
	}
}

f32 TBathtubKiller::getBathtubY()
{
	return (*mBathtub->getRootJointMtx())[1][3];
}

void TBathtubKiller::makeInitialVelocity(JGeometry::TVec3<f32> velocity)
{
	f32 speed    = velocity.length();
	f32 maxSpeed = getSaveParam2()->mSLFlyingSpeedMax.get();
	if (speed > maxSpeed) {
		velocity.normalize();
		velocity.scale(maxSpeed);
	}
	mVelocity.set(velocity);
	velocity.normalize();
	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);
	JGeometry::TQuat4<f32> rotation;
	rotation.setRotate(forward, velocity, 1.0f);
	mQuat.mul(rotation, mQuat);
}

void TBathtubKiller::moveParabolic()
{
	mAcceleration.set(0.0f, -getGravityY(), 0.0f);
	makeVelocityQuat();
}

void TBathtubKiller::moveChasing()
{
	JGeometry::TVec3<f32> target = SMS_GetMarioPos();
	f32 minY                     = mMinChaseHeight + getBathtubY();
	f32 maxY                     = mMaxChaseHeight + getBathtubY();
	target.y                     = 0.5f * (minY + maxY);
	JGeometry::TVec3<f32> direction;
	direction.sub(target, mPosition);
	direction.normalize();
	mAcceleration.scale(mPersonality.mChaseAcceleration, direction);
	makeAccelerationQuat();
	mQuat.getZDir(direction);
	direction.normalize();
	if (mPosition.y > maxY)
		direction.y = 0.0f >= direction.y ? direction.y : 0.0f;
	if (mPosition.y < minY)
		direction.y = 0.0f >= direction.y ? 0.0f : direction.y;
	mVelocity.scale(mPersonality.mChaseSpeed, direction);
}

void TBathtubKiller::moveStraight()
{
	JGeometry::TVec3<f32> direction;
	mQuat.getZDir(direction);
	direction.y = 0.0f;
	direction.normalize();
	direction.scale(mPersonality.mChaseSpeed);
	mVelocity.set(direction);
	makeQuat(mVelocity, mPersonality.mAccelerationQuatRate, 0.1f);
}

void TBathtubKiller::makeVelocityQuat() { makeQuat(mVelocity, 1.0f, 0.1f); }

void TBathtubKiller::makeAccelerationQuat()
{
	makeQuat(mAcceleration, mPersonality.mAccelerationQuatRate, 0.1f);
}

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
	mQuat.mul(steer, mQuat);

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
		mQuat.mul(steer, mQuat);
	}

	mQuat.normalize();
}

void TBathtubKiller::makeScrewQuat(JGeometry::TVec3<f32>, f32, f32)
{
	// TODO: dead-stripped, with no surviving callers to recover the rotation.
}

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

BOOL TBathtubKiller::receiveMessage(THitActor*, u32 message)
{
	if (message == 3 || message == 0 || message == 1) {
		breakBathtubKiller();
		return TRUE;
	}
	if (message == 10) {
		explodeBathtubKiller();
		return TRUE;
	}
	if (message == 13) {
		kill();
		return TRUE;
	}
	if (message == 15) {
		jumpBehavior();
		return TRUE;
	}
	return FALSE;
}

void TBathtubKiller::attackToMario()
{
	bool dying
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();
	if (!dying && SMS_GetMarioPos().y < mPosition.y) {
		mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		SMS_SendMessageToMario(this, 14);
		JGeometry::TVec3<f32> direction(0.0f, 1.0f, 0.0f);
		SMS_ThrowMario(direction, 60.0f);
		mHitMario = 1;
	}
}

bool TBathtubKiller::isCollidMove(THitActor* actor)
{
	bool dying
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();
	if (dying)
		return false;
	if (actor->isActorType(ACTOR_TYPE_BOSS_UNK29)) {
		explodeBathtubKiller();
		return true;
	}
	if (actor->isActorType(ACTOR_TYPE_BOSS_UNK21)
	    || actor->isActorType(ACTOR_TYPE_BOSS_UNK2A)
	    || actor->isActorType(ACTOR_TYPE_BOSS_UNK2C)) {
		explodeBathtubKiller();
		actor->receiveMessage(this, 14);
		return true;
	}
	if (actor->isActorType(ACTOR_TYPE_BATHTUB_KILLER)
	    && mNoKillerCollisionTimer <= 0) {
		explodeBathtubKiller();
		return true;
	}
	return true;
}

void TBathtubKiller::behaveToWater(THitActor*) { breakBathtubKiller(); }

const char** TBathtubKiller::getBasNameTable() const
{
	return bathtubkiller_bastable;
}

void TBathtubKiller::setNormalBathtubKillerAnm()
{
	mMActor = getActorKeeper()->getMActor("bathtubkiller_model1.bmd");
	setBckAnm(1);
}

void TBathtubKiller::setChaseBathtubKillerAnm()
{
	mMActor = getActorKeeper()->getMActor("bathtubkiller_model1.bmd");
	setBckAnm(1);
}

void TBathtubKiller::setStraightBathtubKillerAnm()
{
	mMActor = getActorKeeper()->getMActor("bathtubkiller_model1.bmd");
	setBckAnm(2);
}

void TBathtubKiller::setDeadBathtubKillerAnm()
{
	mMActor = getActorKeeper()->getMActor("bathtubdownkiller_model1.bmd");
	setBckAnm(0);
	mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	mAcceleration.set(0.0f, 0.0f, 0.0f);
	// TODO: the temporary sits 4 bytes lower than the original in both death
	// nerves.
	mVelocity = JGeometry::TVec3<f32>(0, 0, 0);
	onLiveFlag(LIVE_FLAG_UNK8);
	mNoseColor = mBodyColor;
}

void TBathtubKiller::updateTimers()
{
	if (mLifetimeTimer > 0)
		--mLifetimeTimer;
	if (mLaunchTimer > 0)
		--mLaunchTimer;
	if (mChaseStraightTimer > 0)
		--mChaseStraightTimer;
	if (mNoKillerCollisionTimer > 0)
		--mNoKillerCollisionTimer;
	if (mNoCollisionTimer > 0)
		--mNoCollisionTimer;
}

bool TBathtubKiller::isAttackable()
{
	if (!mBathtub->isKillerAttackable())
		return false;
	if (mKillerType == 2) {
		JGeometry::TVec3<f32> mario    = SMS_GetMarioPos();
		mario.y                        = 0.0f;
		JGeometry::TVec3<f32> position = mPosition;
		position.y                     = 0.0f;
		JGeometry::TVec3<f32> bathtub  = mBathtub->mPosition;
		bathtub.y                      = 0.0f;
		if (position.distance(bathtub) > 100.0f + mario.distance(bathtub))
			return false;
	}
	return true;
}

bool TBathtubKiller::isAboided()
{
	if (mPosition.y > 5.0f + (mMaxChaseHeight + getBathtubY()))
		return false;
	JGeometry::TVec3<f32> mario    = SMS_GetMarioPos();
	JGeometry::TVec3<f32> position = mPosition;
	f32 distanceY                  = fabsf(mario.y - position.y);
	mario.y                        = 0.0f;
	position.y                     = 0.0f;
	JGeometry::TVec3<f32> direction;
	direction.sub(mario, position);
	f32 distance = direction.length();
	if (distanceY > getSaveParam2()->mSLAboidDistanceY.get()
	    && distance <= getSaveParam2()->mSLAboidDistance.get())
		return true;
	if (distance > getSaveParam2()->mSLStraightDistance.get())
		return false;
	if (SMS_GetMarioStatus() == MARIO_STATUS_HANGING) {
		mNoCollisionTimer = 240;
		onHitFilter(HIT_FILTER_NO_COLLISION);
		return true;
	}
	direction.normalize();
	TDirectionCalc target(direction);
	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);
	TDirectionCalc current(forward);
	f32 angle = current.absDirection(target.unk0);
	if (angle > TDirectionCalc::d2r(getSaveParam2()->aboidAngle.get()))
		return false;
	return true;
}

bool TBathtubKiller::canChase()
{
	if (mLaunchTimer > 0)
		return false;
	f32 distance = getSaveParam2()->mSLChaseDistanceY.get();
	if (mPosition.y > mMinChaseHeight + getBathtubY() + distance)
		return false;
	return true;
}

void TBathtubKiller::generateExplosion()
{
	TSpineEnemy* effect = gpConductor->makeOneEnemyAppear(
	    mPosition, "エフェクト爆発マネージャー", 1);
	if (effect)
		static_cast<TEffectExplosion*>(effect)->generate(mPosition, mScaling);
}

DEFINE_NERVE(TNerveBathtubKillerWander, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0)
		self->setNormalBathtubKillerAnm();
	if (!self->isAttackable()) {
		spine->pushAfterCurrent(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}
	if (self->canChase()) {
		spine->pushAfterCurrent(&TNerveBathtubKillerChase::theNerve());
		return TRUE;
	}
	self->moveParabolic();
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerChase, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0)
		self->setChaseBathtubKillerAnm();
	if (!self->isAttackable()) {
		spine->pushAfterCurrent(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}
	if (self->isAboided()) {
		if (self->mKillerType == 1)
			spine->pushAfterCurrent(
			    &TNerveBathtubKillerChaseStraight::theNerve());
		else
			spine->pushAfterCurrent(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}
	self->moveChasing();
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerChaseStraight, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setStraightBathtubKillerAnm();
		self->mChaseStraightTimer
		    = self->getSaveParam2()->mSLChaseStraightPeriod.get();
	}
	if (!self->isAttackable()) {
		spine->pushAfterCurrent(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}
	if (self->mNoCollisionTimer <= 0)
		self->offHitFilter(HIT_FILTER_NO_COLLISION);
	if (self->mChaseStraightTimer <= 0) {
		spine->pushAfterCurrent(&TNerveBathtubKillerChase::theNerve());
		return TRUE;
	}
	self->moveStraight();
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerStraight, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0)
		self->setStraightBathtubKillerAnm();
	if (self->mNoCollisionTimer <= 0)
		self->offHitFilter(HIT_FILTER_NO_COLLISION);

	self->moveStraight();
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerBreak, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setDeadBathtubKillerAnm();
		self->generateItemBathtubKiller();
		self->onHitFilter(HIT_FILTER_NO_COLLISION);
	}

	if (self->checkCurAnmEnd(0)) {
		self->mHitMario = 0;
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

		self->generateExplosion();
		self->onHitFilter(HIT_FILTER_NO_COLLISION);
	}

	if (self->checkCurAnmEnd(0)) {
		self->mHitMario = 0;
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

	mInitialLifeCount = TFlagManager::getInstance()->getFlag(MSF_LIFE_COUNT);
	mMushroom         = nullptr;
	mFinalMushroomRequested = 0;
	mEarlyMushroomRequests  = 0;

	// TODO: the original reads unk38 here and drops the result.
	static const char* loopFilenames[1] = {
		"/scene/map/map/ms_kp_kill_smoke.jpa",
	};
	for (int i = 0; i < 1; ++i)
		SMS_LoadParticle(loopFilenames[i], 0x1BD + i);
}

void TBathtubKillerManager::generateMushroom(JGeometry::TVec3<f32> position)
{
	if (!mMushroom || mMushroom->checkLiveFlag(LIVE_FLAG_DEAD))
		mMushroom = gpItemManager->makeObjAppear(
		    position.x, position.y, position.z, ACTOR_TYPE_MUSHROOM1UP, true);
}

int TBathtubKillerManager::countActiveKillers()
{
	int count = 0;
	for (int i = 0; i < getActiveObjNum(); ++i) {
		if (!getObj(i)->checkLiveFlag(LIVE_FLAG_DEAD))
			count += 1;
	}
	return count;
}

int TBathtubKillerManager::countActiveShineKillers()
{
	// TODO: dead-stripped. The variant test is inferred from the name and
	// the 12-byte size difference from countActiveKillers.
	int count = 0;
	for (int i = 0; i < getActiveObjNum(); ++i) {
		TBathtubKiller* killer = static_cast<TBathtubKiller*>(getObj(i));
		if (!killer->checkLiveFlag(LIVE_FLAG_DEAD) && killer->mKillerType == 1)
			++count;
	}
	return count;
}

void TBathtubKillerManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "bathtubkiller_model1.bmd",
		  J3DMLF_MaterialColorLightOn | J3DMLF_MaterialPEFull
		      | J3DMLF_UseUniqueMaterials | (3 << J3DMLF_TevStageNumShift),
		  0 },
		{ "bathtubdownkiller_model1.bmd",
		  J3DMLF_MaterialColorLightOn | J3DMLF_MaterialPEFull
		      | J3DMLF_UseUniqueMaterials | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TBathtubKillerManager::createEnemyInstance()
{
	return new TBathtubKiller;
}
