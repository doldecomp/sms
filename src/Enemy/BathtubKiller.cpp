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
#include <Map/Map.hpp>
#include <MSound/MSound.hpp>
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
	mActorType = ACTOR_TYPE_BATHTUB_KILLER;
	unk150     = 0x11;
	onLiveFlag(LIVE_FLAG_UNK10);
	onLiveFlag(LIVE_FLAG_DEAD);
	onLiveFlag(LIVE_FLAG_UNK8);
	onHitFilter(HIT_FILTER_NO_COLLISION);
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
	offHitFilter(HIT_FILTER_NO_COLLISION);
	offHitFilter(HIT_FILTER_NO_ATTACK);
	offHitFilter(HIT_FILTER_NO_DAMAGE);
	resetBathtubKiller();
}

void TBathtubKiller::resetBathtubKiller()
{
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
	s32 shines = TFlagManager::getInstance()->getFlag(MSF_LIFE_COUNT);

	if (SMS_GetMarioWaterGun()->mCurrentWater == 0) {
		item = gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z,
		                                    ACTOR_TYPE_BOTTLE_LARGE, true);
	} else if (manager->unk60 == shines && manager->unk69 < 7) {
		JGeometry::TVec3<f32> pos = mPosition;
		if (!manager->unk64 || manager->unk64->checkLiveFlag(LIVE_FLAG_DEAD))
			manager->unk64 = gpItemManager->makeObjAppear(
			    pos.x, pos.y, pos.z, ACTOR_TYPE_MUSHROOM1UP, true);
		manager->unk69 += 1;
	} else if (shines <= manager->unk60 + 1) {
		if (unk1CC->getNumGripsDead() == 3 && manager->unk68 == 0) {
			JGeometry::TVec3<f32> pos = getPosition();
			if (!manager->unk64
			    || manager->unk64->checkLiveFlag(LIVE_FLAG_DEAD))
				manager->unk64 = gpItemManager->makeObjAppear(
				    pos.x, pos.y, pos.z, ACTOR_TYPE_MUSHROOM1UP, true);
			manager->unk68 = 1;
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
	unk21C = 0;
	onLiveFlag(LIVE_FLAG_DEAD);
	stopAnmSound();
}

void TBathtubKiller::breakBathtubKiller()
{
	mSpine->pushNerve(&TNerveBathtubKillerBreak::theNerve());
}

// TODO: UNUSED size differs; the guard's authored boundary is unresolved.
void TBathtubKiller::explodeBathtubKiller()
{
	mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
}

void TBathtubKiller::bind()
{
	JGeometry::TVec3<f32> next = mPosition;
	next += mPositionDelta;
	next += mVelocity;
	mVelocity += unk1BC;

	if (!isAttackable()) {
		mGroundHeight = gpMap->checkGround(next.x, next.y + mHeadHeight, next.z,
		                                   &mGroundPlane);
		mGroundHeight += 1.0f;
		if (next.y <= mGroundHeight + 0.05f) {
			if (!isAttackable())
				mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());

			unk1BC.x  = 0.0f;
			unk1BC.y  = 0.0f;
			unk1BC.z  = 0.0f;
			mVelocity = unk1BC;
			next.y    = mGroundHeight;
		}

		if (gpMap->isTouchedOneWallAndMoveXZ(&next.x, next.y + mHeadHeight,
		                                     &next.z, mBodyRadius)) {
			if (!isAttackable())
				mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		}
	}

	mPositionDelta = next - mPosition;
}

void TBathtubKiller::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);
	if (unk1CC == nullptr)
		unk1CC
		    = static_cast<TBathtub*>(JDrama::TNameRefGen::search("バスタブ"));

	if ((cue & CUE_MOVE) && !checkLiveFlag(LIVE_FLAG_DEAD)) {
		updateTimers();

		if (unk208 <= 0 && !isAttackable())
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());

		if (!gpMap->isInArea(mPosition.x, mPosition.z))
			killBathtubKiller();

		if (unk1CC->unk29A != 0)
			killBathtubKiller();
	}

	if ((cue & CUE_CALC_ANIM) && !checkLiveFlag(LIVE_FLAG_DEAD)
	    && !isAttackable()) {
		makeNoseColor();

		unk1D4++;
		if (unk1D4 >= getSaveParam2()->mSLSmokeInterval.get()) {
			unk1D4 = 0;
			unk220.setQT(mQuat, mPosition);
			gpMarioParticleManager->emitAndBindToMtxPtr(0x1BD, unk220, 1, this);
		}

		SMSGetMSound()->startSoundActorWithInfo(
		    MSD_SE_EN_KILLER_FLY, &mPosition, nullptr,
		    mPosition.distance(SMS_GetMarioPos()), 0, 0, nullptr, 0, 4);
	}
}

void TBathtubKiller::makeNoseColor()
{
	if (unk194 == 2) {
		unk1FC += unk1F8;
		if (unk1FC > 1.0f) {
			unk1FC = 1.0f;
			unk1F8 = -getSaveParam2()->mSLColorChangeRateDelta.get();
		}
		if (unk1FC < 0.0f) {
			unk1FC = 0.0f;
			unk1F8 = getSaveParam2()->mSLColorChangeRateDelta.get();
		}
		mNoseColor.r = (u8)(50.0f * unk1FC);
	}
}

f32 TBathtubKiller::getBathtubY() { return (*unk1CC->getRootJointMtx())[1][3]; }

void TBathtubKiller::makeInitialVelocity(JGeometry::TVec3<f32> velocity)
{
	f32 curSpeed = velocity.length();
	f32 maxSpeed = getSaveParam2()->mSLFlyingSpeedMax.get();
	if (curSpeed > maxSpeed) {
		velocity.normalize();
		velocity *= maxSpeed;
	}

	mVelocity.x = velocity.x;
	mVelocity.y = velocity.y;
	mVelocity.z = velocity.z;
	velocity.normalize();

	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);

	JGeometry::TQuat4<f32> rotation;
	rotation.setRotate(forward, velocity, 1.0f);
	mQuat.mul(rotation, mQuat);
}

void TBathtubKiller::moveParabolic() { }

// TODO: stack layout and normalization FPR allocation are nonmatching.
void TBathtubKiller::moveChasing()
{
	JGeometry::TVec3<f32> target = *gpMarioPos;
	f32 minY                     = getBathtubY() + unk200;
	f32 maxY                     = getBathtubY() + unk204;
	target.y                     = (minY + maxY) * 0.5f;

	JGeometry::TVec3<f32> displacement;
	displacement.sub(target, mPosition);
	JGeometry::TVec3<f32> direction;
	direction.normalize(displacement);
	f32 acceleration = unk198.unk4;
	unk1BC.scale(acceleration, direction);
	makeQuat(unk1BC, unk198.unk0, 0.1f);

	JGeometry::TVec3<f32> velocity;
	mQuat.getZDir(velocity);
	velocity.normalize();

	if (mPosition.y > maxY)
		velocity.y = 0.0f >= velocity.y ? velocity.y : 0.0f;
	if (mPosition.y < minY)
		velocity.y = 0.0f >= velocity.y ? 0.0f : velocity.y;

	mVelocity.scale(unk198.unk8, velocity);
}

void TBathtubKiller::moveStraight()
{
	JGeometry::TVec3<f32> vec;
	mQuat.getZDir(vec);
	vec.y = 0.0f;
	vec.normalize();
	vec.scale(unk198.unk8);
	mVelocity.set(vec);
	makeQuat(mVelocity, unk198.unk0, 0.1f);
}

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

BOOL TBathtubKiller::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SUPER_HIP_DROP || message == HIT_MESSAGE_TRAMPLE
	    || message == HIT_MESSAGE_HIP_DROP) {
		if (!isAttackable())
			breakBathtubKiller();
		return true;
	} else if (message == HIT_MESSAGE_BURN) {
		if (!isAttackable())
			explodeBathtubKiller();
		return true;
	} else if (message == HIT_MESSAGE_UNKD) {
		kill();
		return true;
	} else if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		behaveToWater(sender);
		return true;
	}

	return false;
}

// fabricated: preserves the bool return of each current-nerve comparison.
template <class Nerve>
static inline bool isCurrentNerve(const TSpineBase<TLiveActor>* spine)
{
	return spine->getCurrentNerve() == &Nerve::theNerve();
}

// TODO: isAttackable's nested constructor and boolean lowering are nonmatching.
void TBathtubKiller::attackToMario()
{
	if (isAttackable() == FALSE && gpMarioPos->y < mPosition.y) {
		mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		JGeometry::TVec3<f32> throwDirection(0.0f, 1.0f, 0.0f);
		SMS_ThrowMario(throwDirection, 60.0f);
		unk21C = 1;
	}
}

// TODO: current-nerve boolean lowering and nested nerve inlining differ.
bool TBathtubKiller::isCollidMove(THitActor* actor)
{
	if (isAttackable())
		return false;

	if (actor->isActorType(ACTOR_TYPE_BOSS_UNK29)) {
		if (isAttackable() == FALSE)
			explodeBathtubKiller();
		return true;
	}

	if (actor->isActorType(ACTOR_TYPE_BOSS_UNK21)
	    || actor->isActorType(ACTOR_TYPE_BOSS_UNK2A)
	    || actor->isActorType(ACTOR_TYPE_BOSS_UNK2C)) {
		if (isAttackable() == FALSE)
			explodeBathtubKiller();
		actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
		return true;
	}

	if (actor->isActorType(ACTOR_TYPE_BATHTUB_KILLER) && unk214 <= 0) {
		if (isAttackable() == FALSE)
			explodeBathtubKiller();
		return true;
	}

	return true;
}

// TODO: current-nerve comparison helper boundaries are unresolved; the target
// materializes each equality as a bool before combining them.
void TBathtubKiller::behaveToWater(THitActor*)
{
	bool attackable = isCurrentNerve<TNerveBathtubKillerExplosion>(mSpine)
	                  || isCurrentNerve<TNerveBathtubKillerBreak>(mSpine);
	if (!attackable)
		breakBathtubKiller();
}

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

void TBathtubKiller::updateTimers()
{
	if (unk208 > 0)
		unk208--;
	if (unk20C > 0)
		unk20C--;
	if (unk210 > 0)
		unk210--;
	if (unk214 > 0)
		unk214--;
	if (unk218 > 0)
		unk218--;
}

bool TBathtubKiller::isAttackable()
{
	return mSpine->getCurrentNerve()
	           == &TNerveBathtubKillerExplosion::theNerve()
	       || mSpine->getCurrentNerve()
	              == &TNerveBathtubKillerBreak::theNerve();
}

// TODO: stack layout and vector-math codegen are nonmatching.
bool TBathtubKiller::isAboided()
{
	if (mPosition.y > getBathtubY() + unk204 + 5.0f)
		return false;

	JGeometry::TVec3<f32> target(*gpMarioPos,
	                             JGeometry::TVec3<f32>::ASSIGN_COPY);
	JGeometry::TVec3<f32> position(mPosition,
	                               JGeometry::TVec3<f32>::ASSIGN_COPY);
	f32 distanceY = fabsf(target.y - position.y);
	target.y      = 0.0f;
	position.y    = 0.0f;

	JGeometry::TVec3<f32> direction;
	direction.sub(target, position);
	f32 distance = direction.length();
	if (distanceY > getSaveParam2()->mSLAboidDistanceY.get()
	    && distance <= getSaveParam2()->mSLAboidDistance.get())
		return true;

	if (distance > getSaveParam2()->mSLStraightDistance.get())
		return false;

	if (SMS_GetMarioStatus() == MARIO_STATUS_HANGING) {
		unk218 = 240;
		onHitFilter(HIT_FILTER_NO_COLLISION);
		return true;
	}

	direction.normalize();
	TDirectionCalc targetDirection(
	    JGeometry::TVec3<f32>(direction, JGeometry::TVec3<f32>::ASSIGN_COPY));
	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);
	TDirectionCalc currentDirection(
	    JGeometry::TVec3<f32>(forward, JGeometry::TVec3<f32>::ASSIGN_COPY));
	f32 angle = currentDirection.absDirection(targetDirection.unk0);
	if (angle > TDirectionCalc::d2r(getSaveParam2()->aboidAngle.get()))
		return false;
	return true;
}

// TODO: UNUSED size differs (map 0x98); distance inlining is unresolved.
bool TBathtubKiller::canChase()
{
	if (!unk1CC->isKillerAttackable())
		return false;

	if (unk194 == 2) {
		JGeometry::TVec3<f32> target   = *gpMarioPos;
		target.y                       = 0.0f;
		JGeometry::TVec3<f32> position = mPosition;
		position.y                     = 0.0f;
		JGeometry::TVec3<f32> bathtub  = unk1CC->mPosition;
		bathtub.y                      = 0.0f;

		if (position.distance(bathtub) > target.distance(bathtub) + 100.0f)
			return false;
	}
	return true;
}

void TBathtubKiller::generateExplosion() { }

DEFINE_NERVE(TNerveBathtubKillerWander, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0) {
		self->mMActor
		    = self->mMActorKeeper->getMActor("bathtubdownkiller_model1.bmd");
		self->setBckAnm(1);
	}

	if (!self->canChase()) {
		spine->pushNerve(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}

	bool chase;
	if (self->unk20C > 0) {
		chase = false;
	} else if (self->mPosition.y > self->getBathtubY() + self->unk200) {
		chase = false;
	} else {
		chase = true;
	}

	if (chase) {
		spine->pushNerve(&TNerveBathtubKillerChase::theNerve());
		return TRUE;
	}

	self->unk1BC.set(0.0f, -self->getGravityY(), 0.0f);
	self->makeQuat(self->mVelocity, 1.0f, 0.1f);
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerChase, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0) {
		self->mMActor
		    = self->mMActorKeeper->getMActor("bathtubdownkiller_model1.bmd");
		self->setBckAnm(1);
	}

	if (!self->unk1CC->isKillerAttackable()) {
		spine->pushNerve(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}

	if (self->isAboided()) {
		if (self->unk194 == 1)
			spine->pushNerve(&TNerveBathtubKillerChaseStraight::theNerve());
		else
			spine->pushNerve(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}

	self->moveChasing();
	return FALSE;
}

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
		self->onHitFilter(HIT_FILTER_NO_COLLISION);
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

	unk60 = TFlagManager::getInstance()->getFlag(MSF_LIFE_COUNT);
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
