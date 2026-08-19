#include <Enemy/BathtubKiller.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/DirectionCalc.hpp>
#include <Enemy/EffectObj.hpp>
#include <Map/Map.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <System/Particles.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSound.hpp>
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
    , PARAM_INIT(fastAccelerationQuatRate, 0.0f)
    , PARAM_INIT(fastChaseAcceleration, 5.0f)
    , PARAM_INIT(fastChaseSpeed, 40.0f)
    , PARAM_INIT(fastInitialSpeed, 15.0f)
    , PARAM_INIT(fastDeadPeriod, 1800)
    , PARAM_INIT(shineAccelerationQuatRate, 0.0f)
    , PARAM_INIT(shineChaseAcceleration, 5.0f)
    , PARAM_INIT(shineChaseSpeed, 40.0f)
    , PARAM_INIT(shineInitialSpeed, 15.0f)
    , PARAM_INIT(shineDeadPeriod, 1800)
    , PARAM_INIT(mushroomProbability, 0.0f)
    , PARAM_INIT(mSLColorChangeRateDelta, 0.0f)
    , PARAM_INIT(mSLAccelerationQuatRate, 0.0f)
    , PARAM_INIT(mSLChaseAcceleration, 5.0f)
    , PARAM_INIT(mSLChaseSpeed, 40.0f)
    , PARAM_INIT(mSLInitialSpeed, 15.0f)
    , PARAM_INIT(mSLDeadPeriod, 1800)
    , PARAM_INIT(mSLStraightDistance, 0.0f)
    , PARAM_INIT(mSLChaseMinY, 0.0f)
    , PARAM_INIT(mSLChaseMaxY, 0.0f)
    , PARAM_INIT(mSLAboidDistanceY, 0.0f)
    , PARAM_INIT(mSLAboidDistance, 0.0f)
    , PARAM_INIT(mSLChaseDistanceY, 5.0f)
    , PARAM_INIT(mSLChaseDistance, 5.0f)
    , PARAM_INIT(mSLTrampleVelocity, 40.0f)
    , PARAM_INIT(mSLFlyingSpeedMax, 0.0f)
    , PARAM_INIT(mSLFlyingGravityY, 5.0f)
    , PARAM_INIT(mSLBombRange, 1.0f)
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
	    noseMatIdx, GX_TEVREG0, &unk1E0);
	SMS_InitPacket_OneTevColor(
	    getActorKeeper()->getMActor("bathtubkiller_model1.bmd")->getModel(),
	    eyesMatIdx, GX_TEVREG0, &unk1E8);
	SMS_InitPacket_OneTevColor(
	    getActorKeeper()->getMActor("bathtubkiller_model1.bmd")->getModel(),
	    bodyMatIdx, GX_TEVREG0, &unk1D8);
	SMS_InitPacket_OneTevColor(
	    getActorKeeper()->getMActor("bathtubdownkiller_model1.bmd")->getModel(),
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

// TODO: nonmatching, frame 0xc0 vs target 0xd8
void TBathtubKiller::resetBathtubKiller()
{
	mSpine->initWith(&TNerveBathtubKillerWander::theNerve());
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	unk208[0] = 0;
	unk208[1] = 0;
	unk208[2] = 0;
	unk208[3] = 0;
	unk208[4] = 0;
	mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	mVelocity.set(0.0f, 0.0f, 0.0f);
	unk1BC.set(0.0f, 0.0f, 0.0f);
	unk21C = 0;
	unk1D4 = 0;

	if (unk194 == 1) {
		unk1D8.r = 50;
		unk1D8.g = 70;
		unk1D8.b = 160;
		unk1D8.a = 0;
		unk1E0   = unk1D8;
		unk1E8   = unk1D8;
		unk1F0   = unk1D8;
		mPersonality.makeShine(getSaveParam2());
	} else {
		unk1D8.r = 0;
		unk1D8.g = 0;
		unk1D8.b = 0;
		unk1D8.a = 0;
		unk1E0   = unk1D8;
		unk1E8   = unk1D8;
		unk1F0   = unk1D8;

		if (unk194 == 2)
			mPersonality.makeFast(getSaveParam2());
		else
			mPersonality.makeNormal(getSaveParam2());
	}

	unk1FC    = 0.0f;
	unk1F8    = getSaveParam2()->mSLColorChangeRateDelta.get();
	unk208[0] = mPersonality.mDeadPeriod;
	unk208[1] = getSaveParam2()->mSLLaunchingPeriod.get();
	unk208[3] = getSaveParam2()->noCollisionAmongKillers.get();
	unk200    = getSaveParam2()->mSLChaseMinY.get();
	unk204    = getSaveParam2()->mSLChaseMaxY.get();

	if (unk194 == 2) {
		int choice    = MsRandF() * 4.0f;
		f32 variation = 0.0f;
		if (choice == 0)
			variation = 120.0f;
		else if (choice == 1)
			variation = 240.0f;
		unk200 += variation;
		unk204 += variation;
	}
}

// TODO: nonmatching, frame 0x40 vs target 0x48
void TBathtubKiller::generateItemBathtubKiller()
{
	if (unk194 != 1)
		return;

	TMapObjBase* item              = nullptr;
	TFlagManager* flagManager      = TFlagManager::smInstance;
	TBathtubKillerManager* manager = (TBathtubKillerManager*)mManager;
	s32 flag                       = flagManager->getFlag(0x20001);

	if (SMS_GetMarioWaterGun()->mCurrentWater == 0) {
		item = gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z, 0x20000002, true);
	} else if (manager->unk60 == flag && manager->unk69 < 7) {
		manager->generateMushroom(mPosition);
		manager->unk69++;
	} else if (flag <= manager->unk60 + 1 && unk1CC->getNumGripsDead() == 3
	           && manager->unk68 == 0) {
		manager->generateMushroom(mPosition);
		manager->unk68 = 1;
	}

	if (item == nullptr)
		item = gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z, 0x20000002, true);

	if (item != nullptr && item->mActorType == 0x20000002) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emit(
		    PARTICLE_MS_ENM_DISAP_A_W, &item->mPosition, 0, nullptr);
		if (emitter != nullptr)
			emitter->setScale(item->mScaling);
		emitter = gpMarioParticleManager->emit(PARTICLE_MS_ENM_DISAP_B,
		                                       &item->mPosition, 0, nullptr);
		if (emitter != nullptr)
			emitter->setScale(item->mScaling);
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

// TODO: nonmatching, same nerve singleton construction mismatch as
// attackToMario
void TBathtubKiller::bind()
{
	JGeometry::TVec3<f32> nextPos = mPosition;
	nextPos += mLinearVelocity;
	nextPos += mVelocity;
	mVelocity += unk1BC;

	bool inactive
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();

	if (!inactive) {
		mGroundHeight = gpMap->checkGround(nextPos.x, nextPos.y + mHeadHeight,
		                                   nextPos.z, &mGroundPlane);
		mGroundHeight += 1.0f;

		if (nextPos.y <= mGroundHeight + 0.05f) {
			bool groundInactive
			    = mSpine->getCurrentNerve()
			          == &TNerveBathtubKillerExplosion::theNerve()
			      || mSpine->getCurrentNerve()
			             == &TNerveBathtubKillerBreak::theNerve();

			if (!groundInactive)
				mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());

			unk1BC.set(0.0f, 0.0f, 0.0f);
			mVelocity.set(unk1BC);
			nextPos.y = mGroundHeight;
		}

		if (gpMap->isTouchedOneWallAndMoveXZ(
		        &nextPos.x, nextPos.y + mHeadHeight, &nextPos.z, mBodyRadius)) {
			bool wallInactive = mSpine->getCurrentNerve()
			                        == &TNerveBathtubKillerExplosion::theNerve()
			                    || mSpine->getCurrentNerve()
			                           == &TNerveBathtubKillerBreak::theNerve();

			if (!wallInactive)
				mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		}
	}

	mLinearVelocity = nextPos - mPosition;
}

// TODO: nonmatching, callee-saved set differs (target stmw r27) and bss
// addressing; some locals or inline shapes are missing
void TBathtubKiller::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);

	if (unk1CC == nullptr)
		unk1CC = JDrama::TNameRefGen::search<TBathtub>("バスタブ");

	if ((cue & CUE_MOVE) && !checkLiveFlag(LIVE_FLAG_DEAD)) {
		updateTimers();
		if (unk208[0] <= 0) {
			bool inactive = mSpine->getCurrentNerve()
			                    == &TNerveBathtubKillerExplosion::theNerve()
			                || mSpine->getCurrentNerve()
			                       == &TNerveBathtubKillerBreak::theNerve();

			if (!inactive)
				mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		}

		if (!gpMap->isInArea(mPosition.x, mPosition.z))
			killBathtubKiller();

		if (unk1CC->unk29A)
			killBathtubKiller();
	}

	if ((cue & CUE_CALC_ANIM) && !checkLiveFlag(LIVE_FLAG_DEAD)) {
		bool inactive = mSpine->getCurrentNerve()
		                    == &TNerveBathtubKillerExplosion::theNerve()
		                || mSpine->getCurrentNerve()
		                       == &TNerveBathtubKillerBreak::theNerve();

		if (!inactive) {
			makeNoseColor();
			unk1D4++;
			if (unk1D4 >= getSaveParam2()->mSLSmokeInterval.get()) {
				unk1D4 = 0;
				unk220.setQT(mQuat, mPosition);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    MAP_MAP_MS_KP_KILL_SMOKE, unk220, 1, this);
			}

			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_EN_KILLER_FLY, &mPosition, nullptr,
			    mPosition.distance(SMS_GetMarioPos()), 0, 0, nullptr, 0, 4);
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

f32 TBathtubKiller::getBathtubY() { return (*unk1CC->getRootJointMtx())[1][3]; }

// TODO: nonmatching, fmadds/fnmsubs grouping differs (try rewriting the
// B - A * C forms); frame 0xe0 vs target 0xc8
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
	unk1BC.set(0.0f, -getGravityY(), 0.0f);
	makeQuat(mVelocity, 1.0f, 0.1f);
}

// TODO: nonmatching, register allocation; frame 0x88 vs target 0xa8
void TBathtubKiller::moveChasing()
{
	JGeometry::TVec3<f32> chasePoint(*gpMarioPos);
	f32 minY     = getBathtubY() + unk200;
	f32 maxY     = getBathtubY() + unk204;
	chasePoint.y = 0.5f * (minY + maxY);

	JGeometry::TVec3<f32> direction;
	direction.sub(chasePoint, mPosition);
	direction.normalize();
	unk1BC.scale(mPersonality.mChaseAcceleration, direction);
	makeAccelerationQuat();

	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);
	forward.normalize();
	if (mPosition.y > maxY)
		forward.y = 0.0f >= forward.y ? forward.y : 0.0f;
	if (mPosition.y < minY)
		forward.y = 0.0f >= forward.y ? 0.0f : forward.y;
	mVelocity.scale(mPersonality.mChaseSpeed, forward);
}

void TBathtubKiller::moveStraight()
{
	JGeometry::TVec3<f32> velocity;
	mQuat.getZDir(velocity);
	velocity.y = 0.0f;
	velocity.normalize();
	velocity *= mPersonality.mChaseSpeed;
	mVelocity.set(velocity);
	makeVelocityQuat();
}

void TBathtubKiller::makeVelocityQuat()
{
	makeQuat(mVelocity, mPersonality.mAccelerationQuatRate, 0.1f);
}

void TBathtubKiller::makeAccelerationQuat()
{
	makeQuat(unk1BC, mPersonality.mAccelerationQuatRate, 0.1f);
}

// TODO: nonmatching, heavy FP register renumbering; frame 0x218 vs target 0x1e0
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
	TPosition3f mtx;
	mtx.setQT(mQuat, mPosition);
	getModel()->setBaseScale(mScaling);
	getModel()->setBaseTRMtx(mtx);
}

// TODO: nonmatching, our guarded static local emits its init$ check here
// but the target does not; structure differs
BOOL TBathtubKiller::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SUPER_HIP_DROP
	    || message <= HIT_MESSAGE_HIP_DROP) {
		bool inactive = mSpine->getCurrentNerve()
		                    == &TNerveBathtubKillerExplosion::theNerve()
		                || mSpine->getCurrentNerve()
		                       == &TNerveBathtubKillerBreak::theNerve();

		if (!inactive)
			mSpine->pushNerve(&TNerveBathtubKillerBreak::theNerve());
		return true;
	}

	if (message == HIT_MESSAGE_UNKA) {
		bool inactive = mSpine->getCurrentNerve()
		                    == &TNerveBathtubKillerExplosion::theNerve()
		                || mSpine->getCurrentNerve()
		                       == &TNerveBathtubKillerBreak::theNerve();

		if (!inactive)
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		return true;
	}

	if (message == HIT_MESSAGE_UNKD) {
		kill();
		return true;
	}

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		behaveToWater(sender);
		return true;
	}

	return false;
}

// TODO: nonmatching, our nerve singleton constructs inline (vtable store)
// where the target calls the TNerveBase constructor out of line
void TBathtubKiller::attackToMario()
{
	JGeometry::TVec3<f32> throwVelocity;
	bool inactive
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();

	if (!inactive && gpMarioPos->y < mPosition.y) {
		mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		throwVelocity.set(0.0f, 1.0f, 0.0f);
		SMS_ThrowMario(throwVelocity, 60.0f);
		unk21C = 1;
	}
}

// TODO: nonmatching, same nerve singleton construction mismatch as
// attackToMario
bool TBathtubKiller::isCollidMove(THitActor* actor)
{
	bool inactive
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();
	if (inactive)
		return false;

	if (actor->isActorType(0x08000029)) {
		bool inactive = mSpine->getCurrentNerve()
		                    == &TNerveBathtubKillerExplosion::theNerve()
		                || mSpine->getCurrentNerve()
		                       == &TNerveBathtubKillerBreak::theNerve();
		if (!inactive)
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		return true;
	}

	if (actor->isActorType(0x08000021) || actor->isActorType(0x0800002A)
	    || actor->isActorType(0x0800002C)) {
		bool inactive = mSpine->getCurrentNerve()
		                    == &TNerveBathtubKillerExplosion::theNerve()
		                || mSpine->getCurrentNerve()
		                       == &TNerveBathtubKillerBreak::theNerve();
		if (!inactive)
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
		return true;
	}

	if (actor->isActorType(0x08000024) && unk208[3] <= 0) {
		bool inactive = mSpine->getCurrentNerve()
		                    == &TNerveBathtubKillerExplosion::theNerve()
		                || mSpine->getCurrentNerve()
		                       == &TNerveBathtubKillerBreak::theNerve();
		if (!inactive)
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		return true;
	}

	return true;
}

void TBathtubKiller::behaveToWater(THitActor*)
{
	bool inactive
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();

	if (!inactive)
		mSpine->pushNerve(&TNerveBathtubKillerBreak::theNerve());
}

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
	unk1BC.set(0.0f, 0.0f, 0.0f);
	mVelocity = JGeometry::TVec3<f32>(0, 0, 0);
	onLiveFlag(LIVE_FLAG_UNK8);
	unk1E0 = unk1D8;
}

void TBathtubKiller::updateTimers()
{
	for (int i = 0; i < 5; ++i) {
		if (unk208[i] > 0)
			unk208[i]--;
	}
}

bool TBathtubKiller::isAttackable()
{
	if (!unk1CC->isKillerAttackable())
		return false;

	if (unk194 == 2) {
		JGeometry::TVec3<f32> marioPos(*gpMarioPos);
		marioPos.y = 0.0f;
		JGeometry::TVec3<f32> selfPos(mPosition);
		selfPos.y = 0.0f;
		JGeometry::TVec3<f32> bathtubPos(unk1CC->mPosition);
		bathtubPos.y = 0.0f;

		f32 marioDistance = marioPos.distance(bathtubPos);
		f32 selfDistance  = selfPos.distance(bathtubPos);
		if (selfDistance > 100.0f + marioDistance)
			return false;
	}
	return true;
}

// TODO: nonmatching, frame 0xe0 vs target 0xf0
bool TBathtubKiller::isAboided()
{
	f32 bathtubY = (*unk1CC->getRootJointMtx())[1][3];
	if (mPosition.y > 5.0f + (unk204 + bathtubY))
		return false;

	JGeometry::TVec3<f32> marioPos(*gpMarioPos);
	JGeometry::TVec3<f32> selfPos(mPosition);
	f32 verticalDistance = fabsf(marioPos.y - selfPos.y);
	marioPos.y           = 0.0f;
	selfPos.y            = 0.0f;

	f32 differenceY;
	f32 differenceX;
	differenceX            = marioPos.x - selfPos.x;
	differenceY            = marioPos.y - selfPos.y;
	f32 differenceZ        = marioPos.z - selfPos.z;
	f32 differenceSquaredX = differenceX * differenceX;
	f32 differenceSquaredY = differenceY * differenceY;
	f32 differenceSquaredZ = differenceZ * differenceZ;
	f32 horizontalDistance = JGeometry::TUtil<f32>::sqrt(
	    differenceSquaredZ + (differenceSquaredX + differenceSquaredY));

	if (verticalDistance > getSaveParam2()->mSLAboidDistanceY.get()
	    && horizontalDistance <= getSaveParam2()->mSLAboidDistance.get())
		return true;

	if (horizontalDistance > getSaveParam2()->mSLStraightDistance.get())
		return false;

	if (SMS_GetMarioStatus() == MARIO_STATUS_HANGING) {
		unk208[4] = 240;
		onHitFlag(HIT_FLAG_NO_COLLISION);
		return true;
	}

	JGeometry::TVec3<f32> direction(differenceX, differenceY, differenceZ);
	direction.normalize();
	TDirectionCalc targetDirection(direction);

	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);
	TDirectionCalc forwardDirection(forward);

	f32 angle = forwardDirection.absDirection(targetDirection.mDirection);
	if (angle > TDirectionCalc::d2r(getSaveParam2()->aboidAngle.get()))
		return false;
	return true;
}

bool TBathtubKiller::canChase()
{
	if (unk208[1] > 0)
		return false;

	f32 chaseDistance = getSaveParam2()->mSLChaseDistanceY.get();
	f32 bathtubY      = getBathtubY();
	if (mPosition.y > unk200 + bathtubY + chaseDistance)
		return false;
	return true;
}

void TBathtubKiller::generateExplosion()
{
	TEffectExplosion* effect
	    = (TEffectExplosion*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト爆発マネージャー", 1);
	if (effect != nullptr)
		effect->generate(mPosition, mScaling);
}

DEFINE_NERVE(TNerveBathtubKillerWander, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0)
		self->setNormalBathtubKillerAnm();

	if (!self->isAttackable()) {
		spine->pushAfterCurrent(&TNerveBathtubKillerStraight::theNerve());
		return true;
	}

	if (self->canChase()) {
		spine->pushAfterCurrent(&TNerveBathtubKillerChase::theNerve());
		return true;
	}

	self->moveParabolic();
	return false;
}

DEFINE_NERVE(TNerveBathtubKillerChase, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0)
		self->setChaseBathtubKillerAnm();

	if (!self->isAttackable()) {
		spine->pushAfterCurrent(&TNerveBathtubKillerStraight::theNerve());
		return true;
	}

	if (self->isAboided()) {
		if (self->unk194 == 1)
			spine->pushAfterCurrent(
			    &TNerveBathtubKillerChaseStraight::theNerve());
		else
			spine->pushAfterCurrent(&TNerveBathtubKillerStraight::theNerve());
		return true;
	}

	self->moveChasing();
	return false;
}

DEFINE_NERVE(TNerveBathtubKillerChaseStraight, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setStraightBathtubKillerAnm();
		self->unk208[2] = self->getSaveParam2()->mSLChaseStraightPeriod.get();
	}

	if (!self->isAttackable()) {
		spine->pushAfterCurrent(&TNerveBathtubKillerStraight::theNerve());
		return true;
	}

	if (self->unk208[4] <= 0)
		self->offHitFlag(HIT_FLAG_NO_COLLISION);

	if (self->unk208[2] <= 0) {
		spine->pushAfterCurrent(&TNerveBathtubKillerChase::theNerve());
		return true;
	}

	self->moveStraight();
	return false;
}

DEFINE_NERVE(TNerveBathtubKillerStraight, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0)
		self->setStraightBathtubKillerAnm();

	if (self->unk208[4] <= 0)
		self->offHitFlag(HIT_FLAG_NO_COLLISION);

	self->moveStraight();
	return false;
}

DEFINE_NERVE(TNerveBathtubKillerBreak, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0)
		self->breakBathtubKiller();

	if (self->checkCurAnmEnd(0)) {
		self->killBathtubKiller();
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBathtubKillerExplosion, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0)
		self->explodeBathtubKiller();

	if (self->checkCurAnmEnd(0)) {
		self->killBathtubKiller();
		return true;
	}

	return false;
}

TBathtubKillerManager::TBathtubKillerManager(const char* name)
    : TSmallEnemyManager(name)
{
}

// TODO: nonmatching, target re-reads a field and compares before returning;
// logic is missing here
void TBathtubKillerManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TBathtubKillerParams("/enemy/bathtubkiller.prm");
}

// TODO: nonmatching, structural: gParticleFlagLoaded addressing and register
// roles differ
void TBathtubKillerManager::loadAfter()
{
	TSmallEnemyManager::loadAfter();
	TMapObjBaseManager::newAndRegisterObj("mushroom1up");
	TMapObjBaseManager::newAndRegisterObj("mushroom1up");
	unk60 = TFlagManager::smInstance->getFlag(0x20001);
	unk64 = nullptr;
	unk68 = 0;
	unk69 = 0;

	static const char* loopFilenames[] = {
		"/scene/map/map/ms_kp_kill_smoke.jpa",
	};
	for (int i = 0; i < 1; ++i)
		SMS_LoadParticle(loopFilenames[i], 0x1bd + i);
}

void TBathtubKillerManager::generateMushroom(JGeometry::TVec3<f32> pos)
{
	if (unk64 == nullptr || unk64->checkLiveFlag(LIVE_FLAG_DEAD))
		unk64 = gpItemManager->makeObjAppear(pos.x, pos.y, pos.z, 0x20000005,
		                                     true);
}

int TBathtubKillerManager::countActiveKillers()
{
	int result = 0;
	for (int i = 0; i < getActiveObjNum(); ++i)
		if (!getObj(i)->checkLiveFlag(LIVE_FLAG_DEAD))
			++result;
	return result;
}

int TBathtubKillerManager::countActiveShineKillers()
{
	int result = 0;
	for (int i = 0; i < getActiveObjNum(); ++i) {
		TBathtubKiller* killer = (TBathtubKiller*)getObj(i);
		if (!killer->checkLiveFlag(LIVE_FLAG_DEAD) && killer->unk194 == 1)
			++result;
	}
	return result;
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
