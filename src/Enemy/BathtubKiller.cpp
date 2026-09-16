#include <Enemy/BathtubKiller.hpp>
#include <Enemy/Conductor.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <Map/Map.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <System/Particles.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSoundSE.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const char* bathtubkiller_bastable[] = {
	"/scene/bathtubkiller/bas/bathtubdownkiller_down1.bas",
	nullptr,
	nullptr,
};

TBathtubKillerPersonality::TBathtubKillerPersonality() { }

void TBathtubKillerPersonality::makeFast(const TBathtubKillerParams*) { }

void TBathtubKillerPersonality::makeShine(const TBathtubKillerParams*) { }

void TBathtubKillerPersonality::makeNormal(const TBathtubKillerParams*) { }

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
	mActorType = 0x8000024;
	unk150     = 17;
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

void TBathtubKiller::reset() { }

#pragma dont_inline on
void TBathtubKiller::resetBathtubKiller()
{
	mSpine->initWith(&TNerveBathtubKillerWander::theNerve());
	onLiveFlag(LIVE_FLAG_AIRBORNE);

	unk208 = 0;
	unk20C = 0;
	unk210 = 0;
	unk214 = 0;
	unk218 = 0;

	mQuat.x = 0.0f;
	mQuat.y = 0.0f;
	mQuat.z = 0.0f;
	mQuat.w = 1.0f;
	mVelocity.x = 0.0f;
	mVelocity.y = 0.0f;
	mVelocity.z = 0.0f;
	unk1BC.x = 0.0f;
	unk1BC.y = 0.0f;
	unk1BC.z = 0.0f;
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

		TBathtubKillerParams* params = getSaveParam2();
		unk198 = params->shineAccelerationQuatRate.get();
		unk19C = params->shineChaseAcceleration.get();
		unk1A0 = params->shineChaseSpeed.get();
		unk1A4 = params->shineInitialSpeed.get();
		unk1A8 = params->shineDeadPeriod.get();
	} else {
		unk1D8.r = 0;
		unk1D8.g = 0;
		unk1D8.b = 0;
		unk1D8.a = 0;
		unk1E0 = unk1D8;
		unk1E8 = unk1D8;
		unk1F0 = unk1D8;

		if (unk194 == 2) {
			TBathtubKillerParams* params = getSaveParam2();
			unk198 = params->fastAccelerationQuatRate.get();
			unk19C = params->fastChaseAcceleration.get();
			unk1A0 = params->fastChaseSpeed.get();
			unk1A4 = params->fastInitialSpeed.get();
			unk1A8 = params->fastDeadPeriod.get();
		} else {
			TBathtubKillerParams* params = getSaveParam2();
			unk198 = params->mSLAccelerationQuatRate.get();
			unk19C = params->mSLChaseAcceleration.get();
			unk1A0 = params->mSLChaseSpeed.get();
			unk1A4 = params->mSLInitialSpeed.get();
			unk1A8 = params->mSLDeadPeriod.get();
		}
	}

	unk1FC = 0.0f;
	unk1F8 = getSaveParam2()->mSLColorChangeRateDelta.get();
	unk208 = unk1A8;
	unk20C = getSaveParam2()->mSLLaunchingPeriod.get();
	unk214 = getSaveParam2()->noCollisionAmongKillers.get();
	unk200 = getSaveParam2()->mSLChaseMinY.get();
	unk204 = getSaveParam2()->mSLChaseMaxY.get();

	if (unk194 == 2) {
		f32 random = (rand() * (1.0f / 32768.0f)) * 4.0f;
		f32 colorOffset = 0.0f;
		s32 random1 = (s32)random;
		s32 random2 = (s32)random;
		if (random1 == 0)
			colorOffset = 120.0f;
		else if (random2 == 1)
			colorOffset = 240.0f;
		unk200 += colorOffset;
		unk204 += colorOffset;
	}
}
#pragma dont_inline off

#pragma dont_inline on
void TBathtubKiller::generateItemBathtubKiller() { }
#pragma dont_inline off

void TBathtubKiller::killBathtubKiller() { }

void TBathtubKiller::breakBathtubKiller()
{
	mSpine->pushNerve(&TNerveBathtubKillerBreak::theNerve());
}

void TBathtubKiller::explodeBathtubKiller() { }

void TBathtubKiller::bind()
{
	JGeometry::TVec3<f32> next = mPosition;
	next += mLinearVelocity;
	next += mVelocity;
	mVelocity += unk1BC;

	if (!isAttackable()) {
		mGroundHeight = gpMap->checkGround(next.x, next.y + mHeadHeight,
		                                   next.z, &mGroundPlane);
		mGroundHeight += 1.0f;
		if (next.y <= mGroundHeight + 0.05f) {
			if (!isAttackable())
				mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());

			unk1BC.x = 0.0f;
			unk1BC.y = 0.0f;
			unk1BC.z = 0.0f;
			mVelocity = unk1BC;
			next.y     = mGroundHeight;
		}

		if (gpMap->isTouchedOneWallAndMoveXZ(
		        &next.x, next.y + mHeadHeight, &next.z, mBodyRadius)) {
			if (!isAttackable())
				mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		}
	}

	mLinearVelocity = next - mPosition;
}

void TBathtubKiller::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);
	if (unk1CC == nullptr)
		unk1CC = JDrama::TNameRefGen::search<TBathtub>("バスタブ");

	if ((cue & CUE_MOVE) && !checkLiveFlag(LIVE_FLAG_DEAD)) {
		updateTimers();

		if (unk208 <= 0 && !isAttackable())
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());

		if (!gpMap->isInArea(mPosition.x, mPosition.z)) {
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

	if ((cue & CUE_CALC_ANIM) && !checkLiveFlag(LIVE_FLAG_DEAD)
	    && !isAttackable()) {
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
			unk1E0.r = 50.0f * unk1FC;
		}

		unk1D4++;
		if (unk1D4 >= getSaveParam2()->mSLSmokeInterval.get()) {
			unk1D4 = 0;
			static_cast<TPosition3f&>(unk220).setQT(mQuat, mPosition);
			gpMarioParticleManager->emitAndBindToMtxPtr(0x1BD, unk220, 1,
			                                            this);
		}

		if (SMSGetMSound()->gateCheck(MSD_SE_EN_KILLER_FLY)) {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_EN_KILLER_FLY, &mPosition, nullptr,
			    mPosition.distance(SMS_GetMarioPos()), 0, 0, nullptr, 0, 4);
		}
	}
}

void TBathtubKiller::makeNoseColor() { }

f32 TBathtubKiller::getBathtubY() { return 0.0f; }

void TBathtubKiller::makeInitialVelocity(JGeometry::TVec3<f32> velocity)
{
	f32 speed = velocity.length();
	f32 maxSpeed = getSaveParam2()->mSLFlyingSpeedMax.get();
	if (speed > maxSpeed) {
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
	mQuat.mul(rotation);
}

void TBathtubKiller::moveParabolic() { }

void TBathtubKiller::moveChasing()
{
	JGeometry::TVec3<f32> target = *gpMarioPos;
	f32 minY = (*unk1CC->getRootJointMtx())[1][3] + unk200;
	f32 maxY = (*unk1CC->getRootJointMtx())[1][3] + unk204;
	target.y = (minY + maxY) * 0.5f;

	JGeometry::TVec3<f32> direction;
	direction.sub(target, mPosition);
	direction.normalize();
	unk1BC.scale(unk19C, direction);
	makeQuat(unk1BC, unk198, 0.1f);

	JGeometry::TVec3<f32> velocity;
	mQuat.getZDir(velocity);
	velocity.normalize();

	if (mPosition.y > maxY && 0.0f < velocity.y)
		velocity.y = 0.0f;
	if (mPosition.y < minY && 0.0f > velocity.y)
		velocity.y = 0.0f;

	mVelocity.scale(unk1A0, velocity);
}

void TBathtubKiller::moveStraight()
{
	JGeometry::TVec3<f32> velocity;
	mQuat.getZDir(velocity);
	velocity.y = 0.0f;
	velocity.normalize();
	velocity.scale(unk1A0);
	mVelocity.set(velocity);
	makeQuat(mVelocity, unk198, 0.1f);
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

void TBathtubKiller::calcRootMatrix() { }

BOOL TBathtubKiller::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SUPER_HIP_DROP
	    || message == HIT_MESSAGE_TRAMPLE || message == HIT_MESSAGE_HIP_DROP) {
		if (!isAttackable())
			mSpine->pushNerve(&TNerveBathtubKillerBreak::theNerve());
		return true;
	} else if (message == HIT_MESSAGE_UNKA) {
		if (!isAttackable())
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		return true;
	} else if (message == HIT_MESSAGE_UNKD) {
		attackToMario();
		return true;
	} else if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		behaveToWater(sender);
		return true;
	}

	return false;
}

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

bool TBathtubKiller::isCollidMove(THitActor* actor)
{
	if (isAttackable() == TRUE)
		return false;

	if (actor->isActorType(0x80000029)) {
		if (isAttackable() == FALSE)
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		return true;
	}

	if (actor->isActorType(0x80000021)
	    || actor->isActorType(0x8000002A)
	    || actor->isActorType(0x8000002C)) {
		if (isAttackable() == FALSE)
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
		return true;
	}

	if (actor->isActorType(0x80000024) && unk214 <= 0) {
		if (isAttackable() == FALSE)
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		return true;
	}

	return true;
}

void TBathtubKiller::behaveToWater(THitActor*)
{
	if (!isAttackable())
		breakBathtubKiller();
}

const char** TBathtubKiller::getBasNameTable() const
{
	return bathtubkiller_bastable;
}

void TBathtubKiller::setNormalBathtubKillerAnm() { }

void TBathtubKiller::setChaseBathtubKillerAnm() { }

void TBathtubKiller::setStraightBathtubKillerAnm() { }

void TBathtubKiller::setDeadBathtubKillerAnm() { }

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
	       || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();
}

bool TBathtubKiller::isAboided() { return false; }

bool TBathtubKiller::canChase() { return false; }

void TBathtubKiller::generateExplosion()
{
	gpConductor->makeOneEnemyAppear(mPosition, "エフェクト爆発マネージャー", 1);
}

DEFINE_NERVE(TNerveBathtubKillerWander, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0) {
		self->mMActor = self->mMActorKeeper->getMActor(
		    "bathtubdownkiller_model1.bmd");
		self->setBckAnm(1);
	}

	if (!self->canChase()) {
		spine->pushNerve(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}

	bool chase;
	if (self->unk20C > 0) {
		chase = false;
	} else if (self->mPosition.y
	           > self->getBathtubY() + self->unk200) {
		chase = false;
	} else {
		chase = true;
	}

	if (chase) {
		spine->pushNerve(&TNerveBathtubKillerChase::theNerve());
		return TRUE;
	}

	self->unk1BC.set(0.0f, -self->getGravityY(), 0.0f);
	self->makeQuat(self->mVelocity, 0.03f, 0.01f);
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerChase, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0) {
		self->mMActor = self->mMActorKeeper->getMActor(
		    "bathtubdownkiller_model1.bmd");
		self->setBckAnm(1);
	}

	if (!self->unk1CC->isKillerAttackable()) {
		spine->pushNerve(&TNerveBathtubKillerStraight::theNerve());
		return TRUE;
	}

	if (self->isAboided()) {
		if (self->unk194 == 1)
			spine->pushNerve(
			    &TNerveBathtubKillerChaseStraight::theNerve());
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
		self->mMActor = self->mMActorKeeper->getMActor(
		    "bathtubdownkiller_model1.bmd");
		self->setBckAnm(2);
	}

	if (self->unk218 <= 0)
		self->offHitFlag(HIT_FLAG_NO_COLLISION);

	self->moveStraight();
	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerBreak, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0) {
		self->mMActor = self->mMActorKeeper->getMActor(
		    "bathtubdownkiller_model1.bmd");
		self->setBckAnm(0);
		self->mQuat.x = 0.0f;
		self->mQuat.y = 0.0f;
		self->mQuat.z = 0.0f;
		self->mQuat.w = 1.0f;
		self->unk1BC.x = 0.0f;
		self->unk1BC.y = 0.0f;
		self->unk1BC.z = 0.0f;
		JGeometry::TVec3<f32> velocity;
		velocity.set(0, 0, 0);
		self->mLinearVelocity = velocity;
		self->onLiveFlag(LIVE_FLAG_UNK8);
		self->unk1E0 = self->unk1D8;
		self->generateItemBathtubKiller();
		self->onLiveFlag(LIVE_FLAG_DEAD);
	}

	if (self->checkCurAnmEnd(0)) {
		self->unk21C = 0;
		self->onLiveFlag(LIVE_FLAG_DEAD);
		self->stopAnmSound();
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBathtubKillerExplosion, TLiveActor) { return FALSE; }

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

	static const char* loopFilenames[] = {
		"/scene/map/map/ms_kp_kill_smoke.jpa",
	};
	SMS_LoadParticle(loopFilenames[0], 0x1BD);
}

void TBathtubKillerManager::generateMushroom(JGeometry::TVec3<f32>) { }

int TBathtubKillerManager::countActiveKillers() { return 0; }

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

static void forceSdata2Order(void)
{
	(void)1.0f;
	(void)0.0f;
	(void)4503601774854144.0;
	(void)2.0f;
	(void)3.81469727e-06f;
	(void)0.100000001f;
	(void)0.5f;
	(void)3.0f;
	(void)100.0f;
	(void)5.0f;
	(void)60.0f;
	(void)0.785398185f;
	(void)255.0f;
	(void)0.0500000007f;
	(void)3.05175781e-05f;
	(void)4.0f;
	(void)120.0f;
	(void)240.0f;
	(void)0.200000003f;
	(void)15.0f;
	(void)50.0f;
	(void)20.0f;
	(void)500.0f;
	(void)200.0f;
	(void)1000.0f;
	(void)300.0f;
	(void)10.0f;
}
