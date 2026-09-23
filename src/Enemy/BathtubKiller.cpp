#include <Enemy/BathtubKiller.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/KoopaJr.hpp>
#include <Map/Map.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/MarioStatus.hpp>
#include <Player/WaterGun.hpp>
#include <System/FlagManager.hpp>
#include <System/Particles.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <M3DUtil/MActor.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// TODO: the residual in behaveToWater (54.5%), receiveMessage (58.9%),
// isCollidMove (67.2%), bind (76.1%) and perform (78.6%) is one MWCC inlining
// artifact, not a source difference: every instruction of those bodies matches,
// but the ROM keeps `theNerve()` a `bl` at most comparison sites and emits
// `bl TNerveBase<TLiveActor>::TNerveBase()` inside the expansions it does make,
// while our build expands `theNerve()` at every site with the base constructor
// folded in. The decision is emergent -- the ROM inlines the first comparison
// in attackToMario/isCollidMove/bind and calls it in receiveMessage and
// behaveToWater from identical source -- so it is not a depth or wrapper
// question. Ruled out: a TU-local `isCurrentNerve(spine, nerve)` wrapper
// (52.3%, the expansion survives), and the ROM's own `theNerve` is byte-exact,
// so the callee is not the difference.
//
// TODO: every site that asks "am I already dying?" spells the two-nerve test
// out, because the map records no symbol for a helper that would hold it --
// neither a weak TBathtubKiller method nor a TSpineBase one. A header inline
// that MWCC expands at every site in every TU leaves no trace either, so the
// eight copies below are *consistent* with the map rather than proven by it.
// The ROM's shape (a flag register preset to 1, each comparison materialised as
// a bool, no flag merge) is what the `||` chain gives here, so it is what is
// written out.

static const char* bathtubkiller_bastable[] = {
	"/scene/bathtubkiller/bas/bathtubdownkiller_down1.bas",
	nullptr,
	nullptr,
};

TBathtubKillerPersonality::TBathtubKillerPersonality() { }

void TBathtubKillerPersonality::makeFast(const TBathtubKillerParams* params)
{
	mAccelerationQuatRate = params->fastAccelerationQuatRate.get();
	mChaseAcceleration = params->fastChaseAcceleration.get();
	mChaseSpeed = params->fastChaseSpeed.get();
	mInitialSpeed = params->fastInitialSpeed.get();
	mDeadPeriod = params->fastDeadPeriod.get();
}

void TBathtubKillerPersonality::makeShine(const TBathtubKillerParams* params)
{
	mAccelerationQuatRate = params->shineAccelerationQuatRate.get();
	mChaseAcceleration = params->shineChaseAcceleration.get();
	mChaseSpeed = params->shineChaseSpeed.get();
	mInitialSpeed = params->shineInitialSpeed.get();
	mDeadPeriod = params->shineDeadPeriod.get();
}

void TBathtubKillerPersonality::makeNormal(const TBathtubKillerParams* params)
{
	mAccelerationQuatRate = params->mSLAccelerationQuatRate.get();
	mChaseAcceleration = params->mSLChaseAcceleration.get();
	mChaseSpeed = params->mSLChaseSpeed.get();
	mInitialSpeed = params->mSLInitialSpeed.get();
	mDeadPeriod = params->mSLDeadPeriod.get();
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
    , unk1CC(nullptr)
{
}

void TBathtubKiller::init(TLiveManager* manager)
{
	TSmallEnemy::init(manager);
	mActorType = 0x08000024;
	unk150    = 0x11;
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
	mMActor = mMActorKeeper->createMActor("bathtubkiller_model1.bmd", 0);
	mMActorKeeper->createMActor("bathtubdownkiller_model1.bmd", 3);
	int nose = mMActorKeeper->getMActor("bathtubkiller_model1.bmd")
	               ->getModel()
	               ->getModelData()
	               ->getMaterialName()
	               ->getIndex("_nosemat1");
	int eyes = mMActorKeeper->getMActor("bathtubkiller_model1.bmd")
	               ->getModel()
	               ->getModelData()
	               ->getMaterialName()
	               ->getIndex("_eyesmat1");
	int body = mMActorKeeper->getMActor("bathtubkiller_model1.bmd")
	               ->getModel()
	               ->getModelData()
	               ->getMaterialName()
	               ->getIndex("_body1");
	SMS_InitPacket_OneTevColor(
	    mMActorKeeper->getMActor("bathtubkiller_model1.bmd")->getModel(),
	    nose, GX_TEVREG0, &unk1E0);
	SMS_InitPacket_OneTevColor(
	    mMActorKeeper->getMActor("bathtubkiller_model1.bmd")->getModel(),
	    eyes, GX_TEVREG0, &unk1E8);
	SMS_InitPacket_OneTevColor(
	    mMActorKeeper->getMActor("bathtubkiller_model1.bmd")->getModel(),
	    body, GX_TEVREG0, &unk1D8);
	SMS_InitPacket_OneTevColor(
	    mMActorKeeper->getMActor("bathtubdownkiller_model1.bmd")->getModel(),
	    0, GX_TEVREG0, &unk1F0);
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
	JGeometry::TVec3<f32> zero;

	mSpine->initWith(&TNerveBathtubKillerWander::theNerve());
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	unk208 = 0;
	unk20C = 0;
	unk210 = 0;
	unk214 = 0;
	unk218 = 0;
	mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	zero.set(0.0f, 0.0f, 0.0f);
	mVelocity.set(zero);
	mAcceleration.set(0.0f, 0.0f, 0.0f);
	unk21C = 0;
	unk1D4 = 0;

	if (unk194 == 1) {
		unk1D8.r = 50;
		unk1D8.g = 70;
		unk1D8.b = 160;
		unk1D8.a = 0;
		unk1E0 = unk1D8;
		unk1E8 = unk1D8;
		unk1F0 = unk1D8;
		mPersonality.makeShine(getSaveParam2());
	} else {
		unk1D8.r = 0;
		unk1D8.g = 0;
		unk1D8.b = 0;
		unk1D8.a = 0;
		unk1E0 = unk1D8;
		unk1E8 = unk1D8;
		unk1F0 = unk1D8;
		if (unk194 == 2) {
			mPersonality.makeFast(getSaveParam2());
		} else {
			mPersonality.makeNormal(getSaveParam2());
		}
	}

	unk1FC = 0.0f;
	unk1F8 = getSaveParam2()->mSLColorChangeRateDelta.get();
	unk208 = mPersonality.mDeadPeriod;
	unk20C = getSaveParam2()->mSLLaunchingPeriod.get();
	unk214 = getSaveParam2()->noCollisionAmongKillers.get();
	unk200 = getSaveParam2()->mSLChaseMinY.get();
	unk204 = getSaveParam2()->mSLChaseMaxY.get();
	if (unk194 == 2) {
		int choice = 4.0f * MsRandF();
		f32 heightOffset = 0.0f;
		if (choice == 0)
			heightOffset = 120.0f;
		else if (choice == 1)
			heightOffset = 240.0f;
		unk200 += heightOffset;
		unk204 += heightOffset;
	}
}

void TBathtubKiller::generateItemBathtubKiller()
{
	if (unk194 == 1) {
		TMapObjBase* item = nullptr;
		TBathtubKillerManager* manager = (TBathtubKillerManager*)mManager;
		int lives = TFlagManager::getInstance()->getFlag(0x20001);
		if (SMS_GetMarioWaterGun()->getCurrentWater() == 0) {
			item = gpItemManager->makeObjAppear(
			    mPosition.x, mPosition.y, mPosition.z, 0x20000002, true);
		} else {
			if (manager->mInitialLives == lives
			    && manager->mMushroomDropCount < 7) {
				manager->generateMushroom(mPosition);
				++manager->mMushroomDropCount;
			} else if (lives <= manager->mInitialLives + 1
			           && unk1CC->getNumGripsDead() == 3
			           && !manager->mDroppedFinalMushroom) {
				manager->generateMushroom(mPosition);
				manager->mDroppedFinalMushroom = true;
			}
		}
		if (!item)
			item = gpItemManager->makeObjAppear(
			    mPosition.x, mPosition.y, mPosition.z, 0x20000002, true);
		if (item && item->getActorType() == 0x20000002) {
			JPABaseEmitter* emitter = gpMarioParticleManager->emit(
			    PARTICLE_MS_ENM_DISAP_A_W, &item->mPosition, 0, nullptr);
			if (emitter)
				emitter->setGlobalScale(item->getScaling());
			emitter = gpMarioParticleManager->emit(
			    PARTICLE_MS_ENM_DISAP_B, &item->mPosition, 0, nullptr);
			if (emitter)
				emitter->setGlobalScale(item->mScaling);
		}
	}
}

void TBathtubKiller::killBathtubKiller()
{
	unk21C = 0;
	onLiveFlag(LIVE_FLAG_DEAD);
	stopAnmSound();
}

// TODO: the map records 0x14c for this and for explodeBathtubKiller alike; our
// bodies come out 0x120 short of that apiece even though the inlined copies in
// the Break/Explosion nerves are exact. Both are dead symbols, so the shape is
// pinned by the nerves, not by the size.
void TBathtubKiller::breakBathtubKiller()
{
	setDeadBathtubKillerAnm();
	generateItemBathtubKiller();
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBathtubKiller::explodeBathtubKiller()
{
	setDeadBathtubKillerAnm();
	generateExplosion();
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBathtubKiller::bind()
{
	JGeometry::TVec3<f32> nextPos = mPosition;
	nextPos += mLinearVelocity;
	nextPos += mVelocity;

	mVelocity += mAcceleration;

	bool dying
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();
	if (!dying) {
		mGroundHeight = gpMap->checkGround(nextPos.x, nextPos.y + mHeadHeight,
		                                   nextPos.z, &mGroundPlane);
		mGroundHeight += 1.0f;
		if (nextPos.y <= 0.05f + mGroundHeight) {
			bool landingDying
			    = mSpine->getCurrentNerve()
			          == &TNerveBathtubKillerExplosion::theNerve()
			      || mSpine->getCurrentNerve()
			             == &TNerveBathtubKillerBreak::theNerve();
			if (!landingDying)
				mSpine->pushNerve(
				    &TNerveBathtubKillerExplosion::theNerve());
			mAcceleration.set(0.0f, 0.0f, 0.0f);
			mVelocity.set(mAcceleration);
			nextPos.y = mGroundHeight;
		}
		if (gpMap->isTouchedOneWallAndMoveXZ(&nextPos.x,
		                                     nextPos.y + mHeadHeight,
		                                     &nextPos.z, mBodyRadius)) {
			bool wallDying
			    = mSpine->getCurrentNerve()
			          == &TNerveBathtubKillerExplosion::theNerve()
			      || mSpine->getCurrentNerve()
			             == &TNerveBathtubKillerBreak::theNerve();
			if (!wallDying)
				mSpine->pushNerve(
				    &TNerveBathtubKillerExplosion::theNerve());
		}
	}

	mLinearVelocity = nextPos - mPosition;
}

void TBathtubKiller::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);

	if (unk1CC == nullptr)
		unk1CC = (TBathtub*)JDrama::TNameRefGen::search2("バスタブ");

	if ((cue & CUE_MOVE) && !checkLiveFlag(LIVE_FLAG_DEAD)) {
		updateTimers();
		if (unk208 <= 0) {
			bool dying
			    = mSpine->getCurrentNerve()
			          == &TNerveBathtubKillerExplosion::theNerve()
			      || mSpine->getCurrentNerve()
			             == &TNerveBathtubKillerBreak::theNerve();
			if (!dying)
				mSpine->pushNerve(
				    &TNerveBathtubKillerExplosion::theNerve());
		}
		if (!gpMap->isInArea(getPosition().x, getPosition().z)) {
			unk21C = 0;
			onLiveFlag(LIVE_FLAG_DEAD);
			stopAnmSound();
		}
		if (unk1CC->unk29A != 0) {
			unk21C = 0;
			onLiveFlag(LIVE_FLAG_DEAD);
			stopAnmSound();
		}
	}

	if ((cue & CUE_CALC_ANIM) && !checkLiveFlag(LIVE_FLAG_DEAD)) {
		bool dying
		    = mSpine->getCurrentNerve()
		          == &TNerveBathtubKillerExplosion::theNerve()
		      || mSpine->getCurrentNerve()
		             == &TNerveBathtubKillerBreak::theNerve();
		if (!dying) {
			makeNoseColor();
			unk1D4++;
			if (unk1D4 >= getSaveParam2()->mSLSmokeInterval.get()) {
				unk1D4 = 0;
				unk220.setQT(mQuat, mPosition);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    MAP_MAP_MS_KP_KILL_SMOKE, unk220, 1, this);
			}
			f32 distToMario = getPosition().distance(*gpMarioPos);
			gpMSound->startSoundActorWithInfo(MSD_SE_EN_KILLER_FLY,
			                                  getPosition(), nullptr,
			                                  distToMario, 0, 0, nullptr, 0,
			                                  4);
		}
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
		unk1E0.r = (u8)(255.0f * unk1FC);
	}
}

f32 TBathtubKiller::getBathtubY()
{
	return (*unk1CC->getRootJointMtx())[1][3];
}

void TBathtubKiller::makeInitialVelocity(JGeometry::TVec3<f32> velocity)
{
	f32 speed = velocity.length();
	f32 speedMax = getSaveParam2()->mSLFlyingSpeedMax.get();
	if (speed > speedMax) {
		velocity.normalize();
		velocity.scale(speedMax);
	}
	mVelocity.set(velocity);

	velocity.normalize();
	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);
	JGeometry::TQuat4<f32> aim;
	aim.setRotate(forward, velocity, 1.0f);
	mQuat.mul(aim);
}

// TODO: dead in the ROM (UNUSED 0x78) and reconstructed from bind()'s
// integration step, which is the only place the acceleration is folded into the
// velocity. Our body is smaller than the map's size.
void TBathtubKiller::moveParabolic()
{
	mVelocity += mAcceleration;
	makeVelocityQuat();
}

void TBathtubKiller::moveChasing()
{
	JGeometry::TVec3<f32> target = *gpMarioPos;
	f32 minY = unk200 + getBathtubY();
	f32 maxY = unk204 + getBathtubY();
	target.y = 0.5f * (minY + maxY);

	JGeometry::TVec3<f32> toTarget;
	toTarget.sub(target, mPosition);
	toTarget.normalize();
	mAcceleration.scale(mPersonality.mChaseAcceleration, toTarget);
	makeAccelerationQuat();

	JGeometry::TVec3<f32> dir;
	mQuat.getZDir(dir);
	dir.normalize();
	if (mPosition.y > maxY) {
		if (0.0f < dir.y)
			dir.y = 0.0f;
	}
	if (mPosition.y < minY)
		dir.y = 0.0f < dir.y ? dir.y : 0.0f;
	mVelocity.scale(mPersonality.mChaseSpeed, dir);
}

void TBathtubKiller::moveStraight()
{
	JGeometry::TVec3<f32> dir;
	mQuat.getZDir(dir);
	dir.y = 0.0f;
	dir.normalize();
	dir.scale(mPersonality.mChaseSpeed);
	mVelocity.set(dir);
	makeVelocityQuat();
}

void TBathtubKiller::makeVelocityQuat()
{
	makeQuat(mVelocity, mPersonality.mAccelerationQuatRate, 0.1f);
}

void TBathtubKiller::makeAccelerationQuat()
{
	makeQuat(mAcceleration, mPersonality.mAccelerationQuatRate, 0.1f);
}

// TODO: 90.8%, frame 0x218 against the ROM's 0x1e0. This is the open
// JGQuat4.hpp item already recorded in that header: an inlined
// TQuat4::setRotate keeps its two TQuat4 temporaries' stack homes at every
// expansion site, and this function has three of them.
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

// TODO: dead in the ROM (UNUSED 0x40c) and not reconstructed. The name and the
// size say it is makeQuat's sibling that spins the killer about its own forward
// axis instead of pitching it, but nothing in the TU inlines it, so there is no
// evidence for the body.
void TBathtubKiller::makeScrewQuat(JGeometry::TVec3<f32> axis, f32 moveAmountY,
                                   f32 moveAmountX)
{
	JGeometry::TVec3<f32> normAxis = axis;
	normAxis.normalize();

	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);

	JGeometry::TQuat4<f32> steer;
	steer.setRotate(forward, normAxis, moveAmountY);
	mQuat.mul(steer);

	JGeometry::TQuat4<f32> screw;
	screw.setRotate(forward, moveAmountX);
	mQuat.mul(screw);

	mQuat.normalize();
}

f32 TBathtubKiller::getGravityY() const
{
	return getSaveParam2()->mSLFlyingGravityY.get();
}

void TBathtubKiller::calcRootMatrix()
{
	TPosition3f mtx;
	mtx.setQT(mQuat, mPosition);
	getModel()->setBaseScale(mScaling);
	getModel()->setBaseTRMtx(mtx);
}

BOOL TBathtubKiller::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SUPER_HIP_DROP
	    || message <= HIT_MESSAGE_HIP_DROP) {
		bool dying
		    = mSpine->getCurrentNerve()
		          == &TNerveBathtubKillerExplosion::theNerve()
		      || mSpine->getCurrentNerve()
		             == &TNerveBathtubKillerBreak::theNerve();
		if (!dying)
			mSpine->pushNerve(&TNerveBathtubKillerBreak::theNerve());
		return TRUE;
	}

	if (message == HIT_MESSAGE_UNKA) {
		bool dying
		    = mSpine->getCurrentNerve()
		          == &TNerveBathtubKillerExplosion::theNerve()
		      || mSpine->getCurrentNerve()
		             == &TNerveBathtubKillerBreak::theNerve();
		if (!dying)
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		return TRUE;
	}

	if (message == HIT_MESSAGE_UNKD) {
		kill();
		return TRUE;
	}

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		behaveToWater(sender);
		return TRUE;
	}

	return FALSE;
}

void TBathtubKiller::attackToMario()
{
	bool dying
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();
	if (!dying && gpMarioPos->y < mPosition.y) {
		mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), 60.0f);
		unk21C = 1;
	}
}

bool TBathtubKiller::isCollidMove(THitActor* other)
{
	bool dying
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();
	if (dying)
		return false;

	// Another killer bursting next to us.
	if (other->isActorType(0x08000029)) {
		bool hitDying
		    = mSpine->getCurrentNerve()
		          == &TNerveBathtubKillerExplosion::theNerve()
		      || mSpine->getCurrentNerve()
		             == &TNerveBathtubKillerBreak::theNerve();
		if (!hitDying)
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		return true;
	}

	if (other->isActorType(0x08000021) || other->isActorType(0x0800002A)
	    || other->isActorType(0x0800002C)) {
		bool hitDying
		    = mSpine->getCurrentNerve()
		          == &TNerveBathtubKillerExplosion::theNerve()
		      || mSpine->getCurrentNerve()
		             == &TNerveBathtubKillerBreak::theNerve();
		if (!hitDying)
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		other->receiveMessage(this, HIT_MESSAGE_ATTACK);
		return true;
	}

	// Two killers launched together are allowed to overlap for a while.
	if (other->isActorType(0x08000024) && unk214 <= 0) {
		bool hitDying
		    = mSpine->getCurrentNerve()
		          == &TNerveBathtubKillerExplosion::theNerve()
		      || mSpine->getCurrentNerve()
		             == &TNerveBathtubKillerBreak::theNerve();
		if (!hitDying)
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		return true;
	}

	return true;
}

void TBathtubKiller::behaveToWater(THitActor* water)
{
	bool dying
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();
	if (!dying)
		mSpine->pushNerve(&TNerveBathtubKillerBreak::theNerve());
}

const char** TBathtubKiller::getBasNameTable() const
{
	return bathtubkiller_bastable;
}

void TBathtubKiller::setNormalBathtubKillerAnm()
{
	mMActor = mMActorKeeper->getMActor("bathtubkiller_model1.bmd");
	setBckAnm(1);
}

void TBathtubKiller::setChaseBathtubKillerAnm()
{
	mMActor = mMActorKeeper->getMActor("bathtubkiller_model1.bmd");
	setBckAnm(1);
}

void TBathtubKiller::setStraightBathtubKillerAnm()
{
	mMActor = mMActorKeeper->getMActor("bathtubkiller_model1.bmd");
	setBckAnm(2);
}

void TBathtubKiller::setDeadBathtubKillerAnm()
{
	mMActor = mMActorKeeper->getMActor("bathtubdownkiller_model1.bmd");
	setBckAnm(0);
	mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	mAcceleration.set(0.0f, 0.0f, 0.0f);
	mVelocity = JGeometry::TVec3<f32>(0, 0, 0);
	onLiveFlag(LIVE_FLAG_UNK8);
	unk1E0 = unk1D8;
}

// TODO: the ROM materialises each timer's address in its own register before
// the test (`addi r4, r30, 0x208`; the load still folds to `0x208(r30)` but the
// store goes through `0(r4)`), and the map's updateTimers is 20 bytes -- exactly
// five instructions, one per timer -- larger than the direct form below. A
// TU-static helper taking the timer by `int&` or by `int*` (both spellings
// tried, including read-into-a-local-then-store) is folded straight back by
// MWCC and changes nothing. The remaining hypothesis is that 0x208..0x218 are
// one `int mTimers[5]` member and this is a fully unrolled loop over it, whose
// indexed element addresses would survive; that needs the array to be plausible
// at the fifteen sites that use the five timers by name, which it is not yet.
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

// TODO: out of line 0x1a4 against the map's 0x19c. Retail measures Mario's
// distance first and keeps bathtubPos.y/.z live in f5/f6 across both sqrt
// blocks; we reload them. Naming the Mario distance fixes the order but costs
// the Chase nerve 8 bytes of frame; flipped comparison, top declarations and
// a named reference to the tub position were inert.
bool TBathtubKiller::isAttackable()
{
	if (!unk1CC->isKillerAttackable())
		return false;

	// A shine killer gives up once Mario is closer to the tub than it is.
	if (unk194 == 2) {
		JGeometry::TVec3<f32> marioPos = *gpMarioPos;
		marioPos.y = 0.0f;
		JGeometry::TVec3<f32> myPos = mPosition;
		myPos.y = 0.0f;
		JGeometry::TVec3<f32> bathtubPos = unk1CC->getPosition();
		bathtubPos.y = 0.0f;
		if (myPos.distance(bathtubPos)
		    > 100.0f + marioPos.distance(bathtubPos))
			return false;
	}

	return true;
}

bool TBathtubKiller::isAboided()
{
	if (mPosition.y > 5.0f + (unk204 + getBathtubY()))
		return false;

	JGeometry::TVec3<f32> marioPos = *gpMarioPos;
	JGeometry::TVec3<f32> myPos = mPosition;
	f32 heightDiff = fabsf(marioPos.y - myPos.y);
	marioPos.y = 0.0f;
	myPos.y = 0.0f;
	f32 diffY;
	f32 diffX;
	f32 diffZ = marioPos.z - myPos.z;
	diffX = marioPos.x - myPos.x;
	diffY = marioPos.y - myPos.y;
	f32 squaredX = diffX * diffX;
	f32 squaredY = diffY * diffY;
	f32 squaredZ = diffZ * diffZ;
	f32 distXZ = JGeometry::TUtil<f32>::sqrt(squaredX + squaredY + squaredZ);

	if (heightDiff > getSaveParam2()->mSLAboidDistanceY.get())
		if (distXZ <= getSaveParam2()->mSLAboidDistance.get())
			return true;

	if (distXZ > getSaveParam2()->mSLStraightDistance.get())
		return false;

	if (SMS_GetMarioStatus() == MARIO_STATUS_HANGING) {
		unk218 = 240;
		onHitFlag(HIT_FLAG_NO_COLLISION);
		return true;
	}

	// The ROM holds the scalar deltas across the preceding tests, then
	// materialises this vector only for the direction calculation.
	JGeometry::TVec3<f32> toMario;
	toMario.set(diffX, diffY, diffZ);
	toMario.normalize();
	TDirectionCalc marioDir(toMario);

	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);
	TDirectionCalc myDir(forward);

	f32 direction = myDir.absDirection(marioDir.get());
	f32 aboidAngle = TDirectionCalc::d2r(getSaveParam2()->aboidAngle.get());
	if (direction > aboidAngle)
		return false;

	return true;
}

bool TBathtubKiller::canChase()
{
	if (unk20C > 0)
		return false;

	f32 chaseDistanceY = getSaveParam2()->mSLChaseDistanceY.get();
	f32 minY = unk200 + getBathtubY();
	if (mPosition.y > minY + chaseDistanceY)
		return false;

	return true;
}

void TBathtubKiller::generateExplosion()
{
	TEffectExplosion* explosion
	    = (TEffectExplosion*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト爆発マネージャー", 1);
	if (explosion)
		explosion->generate(mPosition, mScaling);
}

DEFINE_NERVE(TNerveBathtubKillerWander, TLiveActor)
{
	TBathtubKiller* killer = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0)
		killer->setNormalBathtubKillerAnm();

	if (!killer->isAttackable()) {
		spine->pushAfterCurrent(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}

	if (killer->canChase()) {
		spine->pushAfterCurrent(&TNerveBathtubKillerChase::theNerve());
		return TRUE;
	}

	killer->mAcceleration.set(0.0f, -killer->getGravityY(), 0.0f);
	killer->makeQuat(killer->mVelocity, 1.0f, 0.1f);
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerChase, TLiveActor)
{
	TBathtubKiller* killer = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0)
		killer->setChaseBathtubKillerAnm();

	if (!killer->isAttackable()) {
		spine->pushAfterCurrent(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}

	if (killer->isAboided()) {
		if (killer->unk194 == 1)
			spine->pushAfterCurrent(
			    &TNerveBathtubKillerChaseStraight::theNerve());
		else
			spine->pushAfterCurrent(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}

	killer->moveChasing();
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerChaseStraight, TLiveActor)
{
	TBathtubKiller* killer = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0) {
		killer->setStraightBathtubKillerAnm();
		killer->unk210
		    = killer->getSaveParam2()->mSLChaseStraightPeriod.get();
	}

	if (!killer->isAttackable()) {
		spine->pushAfterCurrent(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}

	if (killer->unk218 <= 0)
		killer->offHitFlag(HIT_FLAG_NO_COLLISION);

	if (killer->unk210 <= 0) {
		spine->pushAfterCurrent(&TNerveBathtubKillerChase::theNerve());
		return TRUE;
	}

	killer->moveStraight();
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerStraight, TLiveActor)
{
	TBathtubKiller* killer = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0)
		killer->setStraightBathtubKillerAnm();

	if (killer->unk218 <= 0)
		killer->offHitFlag(HIT_FLAG_NO_COLLISION);

	killer->moveStraight();
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerBreak, TLiveActor)
{
	TBathtubKiller* killer = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0)
		killer->breakBathtubKiller();
	if (killer->checkCurAnmEnd(0)) {
		killer->killBathtubKiller();
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerExplosion, TLiveActor)
{
	TBathtubKiller* killer = (TBathtubKiller*)spine->getBody();
	if (spine->getTime() == 0)
		killer->explodeBathtubKiller();
	if (killer->checkCurAnmEnd(0)) {
		killer->killBathtubKiller();
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
	// Two discarded `getActiveObjNum()` calls: the ROM reads mParams and
	// compares it against null twice here, with no branch and no use of the
	// result, exactly as in loadAfter.
	getActiveObjNum();
	TSmallEnemyManager::load(stream);
	unk38 = new TBathtubKillerParams("/enemy/bathtubkiller.prm");
	getActiveObjNum();
}

void TBathtubKillerManager::loadAfter()
{
	TSmallEnemyManager::loadAfter();
	TMapObjBaseManager::newAndRegisterObj("mushroom1up");
	TMapObjBaseManager::newAndRegisterObj("mushroom1up");
	mInitialLives = TFlagManager::getInstance()->getFlag(0x20001);
	mMushroom = nullptr;
	mDroppedFinalMushroom = false;
	mMushroomDropCount = 0;
	// The ROM reads mParams and compares it against null here with no branch
	// and no use of the result. `getActiveObjNum()` opens with exactly that
	// test, so a discarded call to it is what is left of whatever used the
	// count; it also accounts for the whole 32-byte frame difference.
	getActiveObjNum();
	static const char* loopFilenames[] = {
		"/scene/map/map/ms_kp_kill_smoke.jpa",
	};
	// TODO: 90.6%. The residual is scheduling inside the inlined
	// SMS_LoadParticle: the ROM hoists the flag's address and the stored `1`
	// into r30/r31 before the test, ours materialises the `1` after the load
	// call.
	SMS_LoadParticle(loopFilenames[0], MAP_MAP_MS_KP_KILL_SMOKE);
}

void TBathtubKillerManager::generateMushroom(JGeometry::TVec3<f32> position)
{
	if (!mMushroom || mMushroom->checkLiveFlag(LIVE_FLAG_DEAD))
		mMushroom = gpItemManager->makeObjAppear(
		    position.x, position.y, position.z, 0x20000005, true);
}

int TBathtubKillerManager::countActiveKillers()
{
	int count = 0;
	for (int i = 0; i < getActiveObjNum(); ++i) {
		if (!getObj(i)->checkLiveFlag(LIVE_FLAG_DEAD))
			++count;
	}
	return count;
}

int TBathtubKillerManager::countActiveShineKillers()
{
	int count = 0;
	for (int i = 0; i < getActiveObjNum(); ++i) {
		if (!getObj(i)->checkLiveFlag(LIVE_FLAG_DEAD)
		    && ((TBathtubKiller*)getObj(i))->unk194 == 1)
			++count;
	}
	return count;
}

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
