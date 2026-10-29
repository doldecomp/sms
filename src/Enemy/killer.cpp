
#include <Enemy/Killer.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/Walker.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Player/MarioAccess.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <Camera/CameraShake.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* killer_bastable[] = {
	"/scene/killer/bas/downkiller_down1.bas", nullptr, nullptr,
	"/scene/killer/bas/killer_search1.bas",   nullptr,
};

TKiller* gpCurKiller;
bool TKiller::mSerialBomb = true;
u8 TKiller::mSmokeJntNo;
bool TKiller::mTrampleDie = true;
bool TKiller::mRollSw;
f32 TFlyEnemy::mTestSp         = 2.5f;
s32 TFlyEnemy::mInvalidTime    = 200;
f32 TFlyEnemy::mTestMarioSpMax = 12.0f;

TFlyEnemyParams::TFlyEnemyParams(const char* path)
    : TWalkerEnemyParams(path)
    , PARAM_INIT(mSLNormalFlyGravityY, 0.2f)
    , PARAM_INIT(mSLNormalFlySpeed, 10.0f)
    , PARAM_INIT(mSLChaseFlyGravityY, 0.1f)
    , PARAM_INIT(mSLChaseDist, 2000.0f)
    , PARAM_INIT(mSLForceGravityY, 0.1f)
{
	TParams::load(mPrmPath);
}

void TFlyEnemy::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mFlyParams = (TFlyEnemyParams*)getSaveParam();
}

f32 TFlyEnemy::getGravityY() const
{
	if (mSpine->getCurrentNerve() == &TNerveFlyEnemyChaseFly::theNerve())
		return mChaseGravityY;

	return mFlyParams->mSLNormalFlyGravityY.get();
}

void TFlyEnemy::reset()
{
	TWalkerEnemy::reset();
	mFlyTimer      = 0;
	mFlyMode       = FLY_MODE_NEUTRAL;
	mForceChase    = false;
	mChaseGravityY = mFlyParams->mSLNormalFlyGravityY.get();
	mIsHoming      = false;
}

void TFlyEnemy::fly()
{
	JGeometry::TVec3<f32> pos = mPosition;
	pos += mPositionDelta;

	JGeometry::TVec3<f32> velocity = mVelocity;
	velocity.x += SMS_GetMarioSpeedX() / mTestMarioSpMax;
	velocity.z += SMS_GetMarioSpeedZ() / mTestMarioSpMax;
	pos += velocity;
	pos.y += mChaseGravityY;

	mGroundHeight
	    = gpMap->checkGround(pos.x, pos.y + mHeadHeight, pos.z, &mGroundPlane);
	mGroundHeight += 1.0f;

	if (pos.y <= mGroundHeight) {
		if (mFlyTimer > mInvalidTime) {
			offLiveFlag(LIVE_FLAG_AIRBORNE);
			mVelocity.set(0.0f, 0.0f, 0.0f);
			pos.y = mGroundHeight;
		}

		if (mGroundPlane->getActor()
		    && mGroundPlane->getActor()->isActorType(ACTOR_TYPE_BASKET_REVERSE))
			const_cast<TLiveActor*>(mGroundPlane->getActor())->kill();

		if (mGroundPlane->isIllegalData())
			kill();
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}
	mPositionDelta = pos - mPosition;
}

void TFlyEnemy::calcChaseParam()
{
	JGeometry::TVec3<f32> diff(gpMarioPos->x - mPosition.x,
	                           gpMarioPos->y - mPosition.y,
	                           gpMarioPos->z - mPosition.z);
	diff.x *= 1.1f;
	diff.z *= 1.1f;
	JGeometry::TVec3<f32> goal;
	goal.add(mPosition, diff);
	if (mIsHoming)
		setGoalPath(TPathNode(goal));

	if (diff.y > 100.0f || fabsf(diff.y) < 100.0f) {
		if (mIsHoming) {
			if (mFlyMode != FLY_MODE_HOMING && mSprayedByWaterCooldown == 0)
				mChaseGravityY = mFlyParams->mSLChaseFlyGravityY.get();
			mFlyMode = FLY_MODE_HOMING;
		} else {
			mPosition.y -= 1.0f;
		}
		return;
	}

	mChaseGravityY = mFlyParams->mSLNormalFlyGravityY.get();
	JGeometry::TVec3<f32> velocity(0.0f, 0.0f, 0.0f);
	if (mFlyMode != FLY_MODE_HOMING || diff.y > 150.0f) {
		mFlyMode = FLY_MODE_DIVE;
		MsVECNormalize(&diff, &diff);
		f32 speed      = mFlyParams->mSLNormalFlySpeed.get();
		velocity.x     = diff.x * speed;
		velocity.z     = diff.z * speed;
		mChaseGravityY = mFlyParams->mSLForceGravityY.get();
	} else {
		mPosition.y -= 3.0f;
	}

	velocity.x = velocity.z = 0.0f;
	mVelocity               = velocity;
}

void TFlyEnemy::bind()
{
	if (mSpine->getCurrentNerve() == &TNerveFlyEnemyChaseFly::theNerve()
	    || mFlyTimer < mInvalidTime)
		fly();
	else
		TLiveActor::bind();
}

// fabricated
static inline JGeometry::TVec3<f32> fromPolar(f32 speed, f32 angle)
{
	return JGeometry::TVec3<f32>(speed * MsSin(angle), 0.0f,
	                             speed * MsCos(angle));
}

void TFlyEnemy::flyMove()
{
	JGeometry::TVec3<f32> velocity = mVelocity;
	velocity.scale(0.9f);
	mVelocity                  = velocity;
	JGeometry::TVec3<f32> diff = getUnkF4().getPoint();
	diff -= mPosition;
	VECMag(&diff);
	f32 angle   = MsWrap(MsGetRotFromZaxisY(diff), 0.0f, 360.0f);
	f32 delta   = MsAngleDiff(angle, mRotation.y);
	delta       = delta > 0.0f ? MsMin(delta, getTurnSpeed())
	                           : MsMax(delta, -getTurnSpeed());
	mRotation.y = MsWrap(mRotation.y + delta, 0.0f, 360.0f);
	JGeometry::TVec3<f32> deltaPos = mPositionDelta;
	deltaPos += fromPolar(mMarchSpeed, mRotation.y);
	mPositionDelta = deltaPos;
}

DEFINE_NERVE(TNerveFlyEnemyNormalFly, TLiveActor)
{
	TFlyEnemy* self = (TFlyEnemy*)spine->getBody();

	if (spine->getTime() == 0)
		self->setNormalFlyAnm();

	if (self->isForceChase() && self->getFlyTimer() > 500) {
		self->updateSquareToMario();
		f32 dist = self->getFlyParams()->mSLChaseDist.get();
		dist *= dist;
		if (self->mDistToMarioSquared < dist) {
			spine->pushAfterCurrent(&TNerveFlyEnemyChaseFly::theNerve());
			return true;
		}
	} else if (!self->isGold() && self->isFindMario(1.0f)
	           && self->getFlyTimer() > 100) {
		self->updateSquareToMario();
		f32 dist = self->getFlyParams()->mSLChaseDist.get();
		dist *= dist;
		if (self->mDistToMarioSquared < dist) {
			spine->pushAfterCurrent(&TNerveFlyEnemyChaseFly::theNerve());
			return true;
		}
	}

	self->mRotation.x = MsGetRotFromZaxis(self->getVelocity()).x;
	f32 scale = MsClamp(1.05f * self->mScaling.x, 0.0f, self->mBodyScale);
	self->mScaling.x = self->mScaling.y = self->mScaling.z = scale;
	return false;
}

DEFINE_NERVE(TNerveFlyEnemyChaseFly, TLiveActor)
{
	TFlyEnemy* self = (TFlyEnemy*)spine->getBody();

	if (spine->getTime() == 0) {
		self->mLaunchVelocity = self->mVelocity;
		self->calcChaseParam();
		self->setChaseFlyAnm();
	}

	f32 speed = 1.0f;
	if (!self->isHoming()) {
		f32 marchSpeed          = self->mMarchSpeed * self->mMarchSpeed;
		self->mLaunchVelocity.y = 0.1f;
		speed                   = self->mLaunchVelocity.length();
		speed /= marchSpeed;
		speed *= TFlyEnemy::mTestSp;
	} else if (self->getFlyMode() == TFlyEnemy::FLY_MODE_DIVE) {
		speed = 2.0f;
	}

	self->walkBehavior(2, speed);

	if ((self->isHoming() && self->isReachedToGoalXZ())
	    || (self->getFlyMode() == TFlyEnemy::FLY_MODE_DIVE
	        && self->mPosition.y < 100.0f + self->mGroundHeight))
		self->calcChaseParam();

	switch (self->getFlyMode()) {
	case TFlyEnemy::FLY_MODE_DIVE:
	case TFlyEnemy::FLY_MODE_NEUTRAL:
		self->walkBehavior(3, 1.0f);
		break;
	case TFlyEnemy::FLY_MODE_HOMING:
		self->flyMove();
		break;
	}

	if (self->getFlyMode() != TFlyEnemy::FLY_MODE_HOMING
	    && self->mPosition.y > 200.0f + self->mGroundHeight) {
		f32 marchSpeed = self->mMarchSpeed;
		JGeometry::TVec3<f32> dir(self->mMarchSpeed,
		                          -40.0f * self->getGravityY(), marchSpeed);
		self->mRotation.x           = MsGetRotFromZaxis(dir).x;
		JGeometry::TVec3<f32> delta = self->mPositionDelta;
		delta.y                     = self->getGravityY();
		self->mRotation.x           = MsGetRotFromZaxis(delta).x;
	} else {
		self->mRotation.x *= 0.99f;
	}

	self->flyBehavior();
	f32 scale        = MsClamp(1.1f * self->mScaling.x, 0.0f, self->mBodyScale);
	self->mScaling.x = self->mScaling.y = self->mScaling.z = scale;
	return false;
}

TKillerSaveLoadParams::TKillerSaveLoadParams(const char* path)
    : TFlyEnemyParams(path)
    , PARAM_INIT(mSLWaterAddGravityY, 1.0f)
    , PARAM_INIT(mSLChaseTimer, 1000)
    , PARAM_INIT(mSLBombRange, 300.0f)
{
	TParams::load(mPrmPath);
}

TKillerManager::TKillerManager(const char* name)
    : TSmallEnemyManager(name)
{
	gpCurKiller = nullptr;
}

TFlyEnemyParams::~TFlyEnemyParams() { }

void TKillerManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TKillerSaveLoadParams("/enemy/killer.prm");
}

void TKillerManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "killer_model1.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (2 << J3DMLF_TevStageNumShift),
		  0 },
		{ "downkiller_model1.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (2 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TKillerManager::createEnemyInstance() { return new TKiller; }

static BOOL KillerBodyCallback(J3DNode* node, int timing)
{
	if (timing == 0) {
		TKiller* self = gpCurKiller;
		if (!self || !TKiller::mRollSw || !self->isRollFly())
			return TRUE;

		u16 joint     = ((J3DJoint*)node)->getJntNo();
		MtxPtr matrix = gpCurKiller->getModel()->getAnmMtx(joint);

		TPosition3f scale;
		scale.setTrans(0.0f, 0.0f, 0.0f);
		scale.setScale(gpCurKiller->mBodyScale, gpCurKiller->mBodyScale,
		               gpCurKiller->mBodyScale);

		// TODO: a MtxPtr ptr = rotation; hack works but not completely
		TRotation3f rotation;
		MsMtxSetRotZ(rotation, gpCurKiller->getRollAngle());

		MTXConcat(matrix, rotation, matrix);
		MTXConcat(matrix, scale, matrix);
		MTXConcat(J3DSys::mCurrentMtx, rotation, J3DSys::mCurrentMtx);
		MTXConcat(J3DSys::mCurrentMtx, scale, J3DSys::mCurrentMtx);
	}
	return TRUE;
}

TKiller::TKiller(const char* name)
    : TFlyEnemy(name)
{
}

void TKiller::init(TLiveManager* manager)
{
	TFlyEnemy::init(manager);
	mActorType    = ACTOR_TYPE_ENEMY_UNK1F;
	unk150        = 0x11;
	mKillerParams = (TKillerSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveFlyEnemyNormalFly::theNerve());
	onLiveFlag(LIVE_FLAG_FORCE_SHADOW);
	offLiveFlag(LIVE_FLAG_UNK800);
	onHitFilter(HIT_CATEGORY_MAP_OBJECT);

	J3DModel* model = mMActor->getModel();
	if (!model->getSkinDeform()) {
		J3DSkinDeform* deform = new J3DSkinDeform;
		model->setSkinDeform(deform, J3D_DEFORM_ATTACH_FLAG_UNK_1);
	}

	mMActor->resetDL();
	if (mInstanceIndex == 0)
		for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); ++i) { }

	mMActor->setJointCallback(1, KillerBodyCallback);
	unk188 = 0.0f;
}

void TKiller::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("killer_model1.bmd", 3);
	mMActorKeeper->createMActor("downkiller_model1.bmd", 3);
	int nose = getActorKeeper()
	               ->getMActor("killer_model1.bmd")
	               ->getModel()
	               ->getModelData()
	               ->getMaterialName()
	               ->getIndex("_nosemat1");
	int eyes = getActorKeeper()
	               ->getMActor("killer_model1.bmd")
	               ->getModel()
	               ->getModelData()
	               ->getMaterialName()
	               ->getIndex("_eyesmat1");
	int body = getActorKeeper()
	               ->getMActor("killer_model1.bmd")
	               ->getModel()
	               ->getModelData()
	               ->getMaterialName()
	               ->getIndex("_body1");
	SMS_InitPacket_OneTevColor(mMActor->getModel(), nose, GX_TEVREG0,
	                           &mNoseColor);
	SMS_InitPacket_OneTevColor(mMActor->getModel(), eyes, GX_TEVREG0,
	                           &mEyesColor);
	SMS_InitPacket_OneTevColor(mMActor->getModel(), body, GX_TEVREG0,
	                           &mBodyColor);
	SMS_InitPacket_OneTevColor(
	    mMActorKeeper->getMActor("downkiller_model1.bmd")->getModel(), body,
	    GX_TEVREG0, &mDownBodyColor);
}

void TKiller::behaveToWater(THitActor*)
{
	if (mSpine->getCurrentNerve() == &TNerveFlyEnemyNormalFly::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveFlyEnemyChaseFly::theNerve())
		genEventCoin();

	if (mSpine->getCurrentNerve() != &TNerveKillerExplosion::theNerve()) {
		mSpine->pushNerve(&TNerveKillerExplosion::theNerve());
		onHitFilter(HIT_FILTER_NO_COLLISION);
		mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
	}
}

void TKiller::genEventCoin()
{
	MtxPtr rotationMtx;
	int count = 2;
	if (mIsGold)
		count = 8;

	for (int i = 0; i < count; ++i) {
		JGeometry::TVec3<f32> dir(0.0f, 0.0f, 30.0f);
		TRotation3f rotation;
		rotationMtx = rotation; // TODO: fakematch
		MsMtxSetRotY(rotationMtx, 360.0f * (1.0f / count) * (i + 1));
		MTXMultVec(rotationMtx, &dir, &dir);

		TMapObjBase* coin = gpItemManager->makeObjAppear(
		    mPosition.x + dir.x, mPosition.y, mPosition.z + dir.z,
		    ACTOR_TYPE_COIN, true);
		if (coin) {
			coin->mPosition.y = mPosition.y;
			MsVECNormalize(&dir, &dir);
			coin->mVelocity.set(3.0f * dir.x, 20.0f, 3.0f * dir.z);
			coin->offLiveFlag(LIVE_FLAG_UNK10);
		}
	}
}

bool TKiller::isHitValid(u32 message)
{
	if (message == HIT_MESSAGE_UNKB) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFilter(HIT_FILTER_NO_COLLISION);
		genEventCoin();
		return false;
	}

	if (mTrampleDie)
		mChaseGravityY -= 12.0f;

	return false;
}

void TKiller::setDeadAnm()
{
	mMActor = getActorKeeper()->getMActor("downkiller_model1.bmd");
	setBckAnm(0);

	TEffectExplosion* effect
	    = (TEffectExplosion*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト爆発マネージャー", 1);
	if (effect)
		effect->generate(mPosition, mScaling);

	gpCameraShake->startShake(CAM_SHAKE_MODE_UNK6, 1.0f);
	SMSRumbleMgr->start(0x15, 5, (f32*)nullptr);
}

void TKiller::attackToMario()
{
	if (gpMarioPos->y < mPosition.y) {
		if (mSpine->getCurrentNerve() != &TNerveKillerExplosion::theNerve()) {
			mSpine->pushNerve(&TNerveKillerExplosion::theNerve());
			sendAttackMsgToMario();
		} else {
			SMS_SendMessageToMario(this, HIT_MESSAGE_BURN);
		}
	}
}

const char** TKiller::getBasNameTable() const { return killer_bastable; }

void TKiller::setNormalFlyAnm()
{
	mMActor = getActorKeeper()->getMActor("killer_model1.bmd");
	setBckAnm(2);
	mRollAngle = 0.0f;
	mFlyTimer  = 0;
}

void TKiller::setChaseFlyAnm() { setBckAnm(3); }

bool TKiller::isRollFly()
{
	if (mSpine->getCurrentNerve() == &TNerveFlyEnemyChaseFly::theNerve()) {
		if (isBckAnm(1))
			return true;
	}

	return false;
}

bool TKiller::isCollidMove(THitActor* actor)
{
	if (actor->isActorType(ACTOR_TYPE_BASKET_REVERSE))
		((TLiveActor*)actor)->kill();

	if (mSerialBomb) {
		if (mSpine->getCurrentNerve() == &TNerveFlyEnemyChaseFly::theNerve())
			mSpine->pushNerve(&TNerveKillerExplosion::theNerve());
	}

	return true;
}

void TKiller::flyBehavior()
{
	mTurnSpeed = mKillerParams->mSLTurnSpeedLow.get();

	if (mSpine->getTime() > mKillerParams->mSLChaseTimer.get())
		mChaseGravityY -= mKillerParams->mSLWaterAddGravityY.get();

	if (checkCurAnmEnd(ANM_TYPE_BCK)) {
		if (isBckAnm(3))
			setBckAnm(1);
	}
	mRollAngle += 2.5f;
}

void TKiller::changeOut()
{
	SMSGetMSound()->startSoundActor(MSD_SE_EN_TELSA_RECOVER, &mPosition, 0,
	                                nullptr, 0, 4);
	onLiveFlag(LIVE_FLAG_DEAD);
	genEventCoin();
	onHitFilter(HIT_FILTER_NO_COLLISION);
	mPosition = mJuiceBlock->mPosition;
	gpMarioParticleManager->emitAndBindToPosPtr(
	    PARTICLE_MS_TLS_CHANGE, &mPosition,
	    TMarioParticleManager::EFFECT_ONETIME_NORMAL, nullptr);
	MActor* actor = mMActor;
	actor->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BCK);
	mJuiceBlock->kill();
	mJuiceBlock = nullptr;
}

void TKiller::reset()
{
	gpCurKiller = this;
	TFlyEnemy::reset();
	TMsRange<f32> range(0.0f, 1.0f);
	mDownBodyColor.r = mDownBodyColor.g = mDownBodyColor.b = 0;
	mBodyColor.r = mBodyColor.g = mBodyColor.b = 0;
	mIsGold                                    = false;

	if (range.rand() < 0.05f) {
		mIsGold          = true;
		mBodyColor.r     = 200;
		mBodyColor.g     = 185;
		mBodyColor.b     = 0;
		mDownBodyColor.r = 255;
		mDownBodyColor.g = 225;
		mDownBodyColor.b = 70;
	}
}

void TKiller::setColorType()
{
	if (mIsGold)
		mIsHoming = false;

	if (mIsHoming) {
		mBodyColor.r     = 70;
		mBodyColor.g     = 20;
		mBodyColor.b     = 70;
		mDownBodyColor.r = 70;
		mDownBodyColor.g = 20;
		mDownBodyColor.b = 70;
	}
}

void TKiller::bind()
{
	TFlyEnemy::bind();
	++mFlyTimer;

	if (mSpine->getCurrentNerve() != &TNerveKillerExplosion::theNerve()) {
		if (!isAirborne() && mFlyTimer > mInvalidTime) {
			mSpine->pushNerve(&TNerveKillerExplosion::theNerve());
		} else if (mFlyTimer > mInvalidTime) {
			TBGWallCheckRecord record(mPosition.x, mPosition.y + mHeadHeight,
			                          mPosition.z, 2.0f * mBodyRadius, 1, 0);
			if (gpMap->isTouchedWallsAndMoveXZ(&record)) {
				if (record.mResultWalls[0]->getActor()
				    && record.mResultWalls[0]->getActor()->isActorType(
				        ACTOR_TYPE_BASKET_REVERSE))
					const_cast<TLiveActor*>(record.mResultWalls[0]->getActor())
					    ->kill();
				mSpine->pushNerve(&TNerveKillerExplosion::theNerve());
			}
		}
	}

	if (isAirborne()) {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_KILLER_FLY, &mPosition, 0,
		                                nullptr, 0, 4);
		mRotation.x   = MsClamp(mRotation.x, -25.0f, 90.0f);
		MtxPtr matrix = mTrailEmitterTransform;
		MsMtxSetXYZRPH(matrix, mPosition.x, mPosition.y, mPosition.z,
		               mRotation.x, mRotation.y, mRotation.z);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_KIL_SMOKE, matrix,
		    TMarioParticleManager::EFFECT_LOOP_NORMAL, this);
	}
}

void TKiller::calcRootMatrix()
{
	if (SMSGetMarDirector()->checkFlag(
	        TMarDirector::DIRECTOR_FLAG_SHINE_GET_PENDING
	        | TMarDirector::DIRECTOR_FLAG_STAGE_TRANSITION_PENDING
	        | TMarDirector::DIRECTOR_FLAG_ACTOR_DEMO_STAGE_TRANSITION_PENDING
	        | TMarDirector::DIRECTOR_FLAG_GATE_DEMO_STAGE_TRANSITION_PENDING)) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFilter(HIT_FILTER_NO_COLLISION);
	}

	if (isBckAnm(2)) {
		if (mIsGold) {
			mNoseColor.r = mEyesColor.r = 170;
			mNoseColor.g = mEyesColor.g = 140;
			mNoseColor.b = mEyesColor.b = 0;
		} else {
			mEyesColor.r = mEyesColor.g = mEyesColor.b = 0;
			mNoseColor.r = mNoseColor.g = mNoseColor.b = 0;
		}
	}

	if (isBckAnm(3)) {
		mNoseColor.b = 0;
		mNoseColor.g = 0;
		mEyesColor.r = 0;

		if (mSpine->getTime() % 10 < 5) {
			mNoseColor.r = 0;
			mEyesColor.g = mEyesColor.b = 0;
		}

		if (mIsGold) {
			if (mSpine->getTime() % 10 < 5) {
				mNoseColor.r = 170;
				mNoseColor.g = 140;
				mNoseColor.b = 0;
			} else {
				mNoseColor.r = 180;
				mNoseColor.g = 140;
				mNoseColor.b = 150;
			}
		}

		if (mIsHoming) {
			if (mSpine->getTime() % 10 < 5) {
				mNoseColor.r = mEyesColor.r = 200;
				mNoseColor.g                = 0;
				mNoseColor.b                = 0;
			} else {
				mNoseColor.r = 70;
				mNoseColor.g = 20;
				mNoseColor.b = 70;
			}
		}
	}

	if (isBckAnm(1)) {
		mEyesColor.r = 0;
		mEyesColor.g = mEyesColor.b = 0;
		mNoseColor.g = mNoseColor.b = 0;

		f32 wave = fabsf(MsSin(360.0f * mSpine->getTime() / 120.0f));
		if (mIsGold) {
			mBodyColor.r = 170;
			mBodyColor.g = 140;
			mBodyColor.b = 0;
			mNoseColor.r = (u8)(10.0f * wave + 160.0f);
			mNoseColor.g = (u8)(30.0f * wave + 140.0f);
			mNoseColor.b = (u8)(150.0f * wave);
		}

		if (mIsHoming) {
			mBodyColor.r     = 70;
			mBodyColor.g     = 20;
			mBodyColor.b     = 70;
			mDownBodyColor.r = 70;
			mDownBodyColor.g = 20;
			mDownBodyColor.b = 70;
			mNoseColor.r     = (u8)(130.0f * wave + 70.0f);
			mNoseColor.g     = (u8)(20.0f - 20.0f * wave);
			mNoseColor.b     = (u8)(70.0f - 70.0f * wave);
		}
	}
	gpCurKiller = this;
	mRotation.x = MsClamp(mRotation.x, -25.0f, 90.0f);
	TSpineEnemy::calcRootMatrix();
}

bool TKiller::isFindMario(f32 scale)
{
	TSmallEnemyParams* params = (TSmallEnemyParams*)getSaveParam();
	if (fabsf(gpMarioPos->y - mPosition.y) < params->getSLSearchHeight()) {
		JGeometry::TVec3<f32> pos(gpMarioPos->x, gpMarioPos->y, gpMarioPos->z);
		f32 length = params->getSLSearchLength();
		f32 angle  = params->getSLSearchAngle();
		f32 aware  = params->getSLSearchAware();
		length *= scale;
		angle *= scale;
		aware *= scale;
		if (isInSight(pos, length, angle, aware))
			return true;

		return false;
	}
	return false;
}

DEFINE_NERVE(TNerveKillerExplosion, TLiveActor)
{
	TKiller* self = (TKiller*)spine->getBody();
	if (spine->getTime() == 0) {
		self->mBombScaleMax
		    = ((TKillerSaveLoadParams*)self->getSaveParam())->mSLBombRange.get()
		      * self->getBodyScale() / self->getAttackRadius();
		self->mRotation.x = 0.0f;
		self->setDeadAnm();
		if (!self->isAirborne()) {
			if (self->getGroundPlane()->isWaterSurface()) {
				TEffectBombColumWater* effect
				    = (TEffectBombColumWater*)gpConductor->makeOneEnemyAppear(
				        self->mPosition, "エフェクト爆発水柱マネージャー", 1);
				if (effect) {
					JGeometry::TVec3<f32> scale(2.0f, 2.0f, 2.0f);
					effect->generate(self->mPosition, scale);
				}
			}

			if (self->getGroundPlane()->isSand()) {
				TEffectColumSand* effect
				    = (TEffectColumSand*)gpConductor->makeOneEnemyAppear(
				        self->mPosition, "エフェクト砂柱マネージャー", 1);
				if (effect) {
					JGeometry::TVec3<f32> scale(0.6f, 0.9f, 0.6f);
					effect->generate(self->mPosition, scale);
				}
			}
		}
		SMSRumbleMgr->start(0x13, &self->mPosition);
	}

	if (self->unk190 < self->mBombScaleMax) {
		self->unk190 *= 1.3f;
	} else {
		self->onHitFilter(HIT_FILTER_NO_COLLISION);
		if (self->checkCurAnmEnd(ANM_TYPE_BCK)) {
			self->onLiveFlag(LIVE_FLAG_DEAD);
			self->onLiveFlag(LIVE_FLAG_UNK8);
			self->offLiveFlag(LIVE_FLAG_UNK10000);
			self->mHolder = nullptr;
			self->stopAnmSound();
			spine->reset();
			spine->setNext(&TNerveSmallEnemyDie::theNerve());
			spine->pushAfterCurrent(spine->getDefault());
			self->mPosition.y -= 200.0f;
			return true;
		}
	}

	self->expandCollision();
	return false;
}
