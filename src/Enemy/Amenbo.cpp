#include <Enemy/Amenbo.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <System/Particles.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/MirrorActor.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapMirror.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>

static const char* amenbo_bastable[] = {
	nullptr, "/scene/amenbo/bas/amenbo_hit1_loop.bas",
	nullptr, "/scene/amenbo/bas/amenbo_run1.bas",
	nullptr, nullptr,
	nullptr,
};

namespace {

const char* const cJointNames[] = {
	"effrleg1",
	"effrarm1",
	"efflleg1",
	"efflarm1",
};

}

TAmenbo::TAmenbo(const char* name)
    : TSmallEnemy(name)
{
	onLiveFlag(LIVE_FLAG_UNK8);
}

void TAmenbo::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("amenbo_model1.bmd", 0);

	mSpine->initWith(&TNerveAmenboWalk::theNerve());

	initCollision();
	initParticle();
	initAnmSound();
	unk1E8 = new TMirrorActor("アメンボくんin鏡");
	unk1E8->init(getModel(), 0x18);
	mInitialPosition.set(mPosition);
	mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	mIsChasingMario        = false;
	mWaterGunHitCooldown   = 0;
	mSearchDisableCooldown = 0;
	mOutOfWaterDeathTimer  = 0;
	for (int i = 0; i < 4; ++i) {
		unk1EC[i].mJointIdx
		    = getModel()->getModelData()->getMaterialName()->getIndex(
		        cJointNames[i]);
	}
}

void TAmenbo::reset() { }

void TAmenbo::kill()
{
	unk1E8->unk1A |= 0x1;
	TSmallEnemy::kill();
}

void TAmenbo::initCollision()
{
	initHitActor(0x1000002D, 2, -0x68000000, 60.0f, 120.0f, 100.0f, 150.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mWallRadius = getSaveParam2()->mSLWallRadius.get();
	unk1E4      = new TMapCollisionWarp;
	unk1E4->init("/scene/amenbo/col/amenbo.col", 0x8000, nullptr);
}

void TAmenbo::initParticle()
{
	SMS_LoadParticle("/scene/amenbo/jpa/ms_ame_hamon.jpa", 0x18E);
}

void TAmenbo::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	JGeometry::TVec3<f32> local_14 = mPosition;
	local_14 += mLinearVelocity;
	local_14 += mVelocity;
	local_14.y += mHeadHeight;

	mVelocity.y -= getGravityY();

	if (mVelocity.y < mVelocityMinY)
		mVelocity.y = mVelocityMinY;

	mGroundHeight = gpMap->checkGround(local_14.x, local_14.y + mHeadHeight,
	                                   local_14.z, &mGroundPlane);
	mGroundHeight += 1.0f;
	if (local_14.y <= mGroundHeight + 0.05f) {
		if (mGroundPlane->isIllegalData())
			kill();

		offLiveFlag(LIVE_FLAG_AIRBORNE);

		if (mGroundPlane->isWaterSurface() || mGroundPlane->isSea()
		    || mGroundPlane->isPool()) {
			mVelocity.y           = 0.0f;
			mOutOfWaterDeathTimer = 0;
		} else {
			mVelocity.zero();
			mOutOfWaterDeathTimer += 1;
		}
		local_14.y = mGroundHeight;
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	gpMap->isTouchedOneWallAndMoveXZ(&local_14.x, local_14.y + mHeadHeight,
	                                 &local_14.z, mWallRadius);

	local_14.y -= mHeadHeight;

	mLinearVelocity = local_14 - mPosition;
}

void TAmenbo::control()
{
	if (mWaterGunHitCooldown > 0)
		mWaterGunHitCooldown--;

	if (mSearchDisableCooldown > 0)
		mSearchDisableCooldown--;

	updateCollision();

	if (SMS_AskJumpIntoWaterEffectExist()) {
		checkMarioWaterIn();
	}

	TLiveActor::control();
}

void TAmenbo::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		if (gpMirrorModelManager->isInMirror(mPosition)) {
			if (param_1 & 2) {
				calcRootMatrix();
				mMActor->calc();
			}
			if (param_1 & 4)
				mMActor->viewCalc();
		}
	}
}

void TAmenbo::checkMarioWaterIn()
{
	JGeometry::TVec3<f32> local_60;

	if (!isOverTerritory(&local_60) && mSearchDisableCooldown <= 0) {
		if (isFreeze() && isChangedBlock()) {
			decideTargetOnFingingMario();
			mSpine->reset();
			mSpine->setNext(&TNerveAmenboTurn::theNerve());
			mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
		}
	}
}

void TAmenbo::updateCollision()
{
	if (!isFreeze() && !mMActor->checkCurAnm("amenbo_hit1_end", 0)) {
		activateJumpBase();
	} else {
		deactivateJumpBase();
	}
}

BOOL TAmenbo::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return false;

	switch (param_2) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
		return false;

	default:
		return TSmallEnemy::receiveMessage(param_1, param_2);
	}
}

void TAmenbo::behaveToWater(THitActor* param_1)
{
	if (mWaterGunHitCooldown <= 0 && isWaterFromWaterGun(param_1)) {
		mWaterGunHitCooldown = 45;
		mSpine->reset();
		mSpine->setNext(&TNerveAmenboHitWater::theNerve());
	}
}

void TAmenbo::calcRootMatrix()
{
	if (isTaken()) {
		TSmallEnemy::calcRootMatrix();
		return;
	}

	TPosition3f mtx;
	mtx.setQT(mQuat, mPosition);
	getModel()->setBaseScale(mScaling);
	getModel()->setBaseTRMtx(mtx);
	getModel()->getBaseTRMtx()[1][3] += mHeadHeight;

	updateRipple();
}

void TAmenbo::forceKill()
{
	if (mOutOfWaterDeathTimer < 600)
		return;

	// amenbo die if no water ;(
	if (mSpine->getLatestNerve() != &TNerveSmallEnemyDie::theNerve()) {
		onLiveFlag(LIVE_FLAG_UNK20000);
		kill();
	}
}

bool TAmenbo::isCollidMove(THitActor* param_1) { return param_1 != this; }

bool TAmenbo::doKeepDistance() { return !isAttacking(); }

void TAmenbo::attackToMario()
{
	if (isAttacking())
		sendAttackMsgToMario();
}

void TAmenbo::setDeadAnm()
{
	mMActor->getFrameCtrl(0)->init(1);
	mMActor->getFrameCtrl(0)->setFrame(0.0f);
}

bool TAmenbo::changeByJuice()
{
	if (!TSmallEnemy::changeByJuice())
		return false;

	unk1E8->unk1A |= 0x1;
	return true;
}

void TAmenbo::updateRipple()
{
	for (int i = 0; i < 4; ++i) {
		EffectOnJoint* effect = &unk1EC[i];
		MtxPtr mtx3           = getModel()->getAnmMtx(effect->mJointIdx);
		effect->mPos.set(mtx3[0][3], mtx3[1][3], mtx3[2][3]);
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToPosPtr(
		        AMENBO_JPA_MS_AME_HAMON, &effect->mPos, 1, effect)) {
			JGeometry::TVec3<f32> scale(3.0f, 3.0f, 3.0f);
			emitter->setScale(scale);
		}
	}
}

void TAmenbo::changeBck(const char* param_1, f32 param_2)
{
	mMActor->setBck(param_1);
	setCurAnmSound();
	mMActor->getFrameCtrl(0)->setRate(SMSGetAnmFrameRate() * param_2);
}

void TAmenbo::prepareWalk()
{
	JGeometry::TQuat4<f32> q;
	q.set(mQuat);

	JGeometry::TVec3<f32> local;
	q.getZDir(local);
	local *= (mIsChasingMario ? getSaveParam2()->mAttackSpeed
	                          : getSaveParam2()->mSpeed)
	             .get();
	mVelocity = local;
}

bool TAmenbo::doWalk()
{
	bool result = false;

	JGeometry::TVec3<f32> vel = mVelocity;
	vel *= getSaveParam2()->mWaterFric.get();
	f32 eps = getSaveParam2()->mEpsilon.get();
	if (vel.x * vel.x + vel.z * vel.z < eps * eps) {
		result = true;
		vel.zero();
	}
	mVelocity = vel;

	return result;
}

// TODO: wrong size, why?
void TAmenbo::doAdjustTarget()
{
	JGeometry::TVec3<f32> vel = mVelocity;
	vel *= 0.9f;
	mVelocity = vel;

	unk1E0 = mMActor->getFrameCtrl(0)->getFrame() * 0.015873017f;

	if (1.0f <= unk1E0)
		unk1E0 = 1.0f;

	mQuat.slerp(unk1C0, unk1D0, unk1E0);
	mQuat.normalize();
}

void TAmenbo::doChangeWaitAnm()
{
	if (checkCurAnmEnd(0)) {
		if (mMActor->checkCurAnm("amenbo_wait1_start", 0))
			changeBck("amenbo_wait1_loop", 1.0f);
		else if (mMActor->checkCurAnm("amenbo_wait1_loop", 0))
			changeBck("amenbo_wait1_end", 1.0f);
		else
			changeBck("amenbo_wait1_start", 1.0f);
	}
}

void TAmenbo::decideTarget()
{
	mIsChasingMario = false;

	JGeometry::TVec3<f32> local_cc;

	if (isOverTerritory(&local_cc)) {
		local_cc.normalize();
		setWalkDir(local_cc);
		return;
	}

	if (isFindOutMario()) {
		decideTargetOnFingingMario();
		return;
	}

	JGeometry::TQuat4<f32> q;
	q.setRotate(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f),
	            (1.5f - MsRandF()) * M_PI);
	setWalkDir(q);
}

void TAmenbo::decideTargetOnFingingMario()
{
	JGeometry::TVec3<f32> diff = SMS_GetMarioPos();
	diff -= mPosition;
	diff.y = 0.0f;
	diff.normalize();

	setWalkDir(diff);

	mIsChasingMario        = true;
	mSearchDisableCooldown = getSaveParam2()->mIsNotSearchTimerMax.get();
}

void TAmenbo::setWalkDir(const JGeometry::TVec3<f32>& param_1)
{
	unk1D0.setRotate(JGeometry::TVec3<f32>(0.0f, 0.0f, 1.0f), param_1, 1.0f);
	unk1C0 = getQuat();
	unk1E0 = 0.0f;
}

void TAmenbo::setWalkDir(const JGeometry::TQuat4<f32>& param_1)
{
	unk1C0 = getQuat();
	unk1D0.set(param_1);
	unk1D0.mul(unk1D0, mQuat);
	unk1E0 = 0.0f;
}

bool TAmenbo::isWalking() const
{
	return mSpine->getLatestNerve() == &TNerveAmenboWalk::theNerve();
}

bool TAmenbo::isStartMoving() const
{
	return mMActor->getFrameCtrl(0)->checkPass(63.0f);
}

bool TAmenbo::isFindOutMario() const
{
	if (!SMS_CheckMarioFlag(0x10000)
	    && !SMS_GetMarioGroundPlane()->isWaterSurface()
	    && !SMS_CheckMarioFlag(0x20000))
		return false;

	JGeometry::TVec3<f32> diff = SMS_GetMarioPos();
	diff -= mPosition;

	f32 range = getSaveParam2()->mSearchRange.get();
	if (abs(diff.y) < getSaveParam2()->mSearchHeight.get()) {
		diff.y = 0;
		if (diff.squared() < range * range)
			return true;
	}

	return false;
}

bool TAmenbo::isOverTerritory(JGeometry::TVec3<f32>* param_1) const
{
	param_1->set(mInitialPosition);
	*param_1 -= mPosition;
	param_1->y = 0.0f;
	f32 range  = getSaveParam2()->mTerritoryRange.get();
	return param_1->squared() < range * range;
}

bool TAmenbo::isAttacking() const
{
	bool result;
	if (mSpine->getLatestNerve() != &TNerveAmenboHitWater::theNerve())
		result = true;
	else if (mMActor->checkCurAnm("amenbo_hit1_end", 0))
		result = true;
	else
		result = false;

	return result;
}

bool TAmenbo::isFreeze() const
{
	return mSpine->getLatestNerve() != &TNerveAmenboHitWater::theNerve();
}

bool TAmenbo::isChangedBlock() const
{
	return mSpine->getLatestNerve() == &TNerveSmallEnemyChange::theNerve();
}

bool TAmenbo::isWaterFromWaterGun(THitActor* param_1)
{
	return gpModelWaterManager->getParticleFlag((TWaterHitActor*)param_1)
	       & 0x40;
}

void TAmenbo::activateJumpBase()
{
	if (isActivateJumpBase())
		deactivateJumpBase();

	unk1E4->setUpTrans(mPosition);
	onHitFlag(HIT_FLAG_UNK2);
	mDamageRadius = 215.0f;
	mDamageHeight = 120.0f;
	calcEntryRadius();
	onLiveFlag(LIVE_FLAG_UNK10);
}

void TAmenbo::deactivateJumpBase()
{
	if (!isActivateJumpBase())
		return;

	unk1E4->remove();
	offHitFlag(HIT_FLAG_UNK2);
	setDamageParams(100.0f, 150.0f);
	offLiveFlag(LIVE_FLAG_UNK10);
}

bool TAmenbo::isActivateJumpBase() const { return unk1E4->isSetUp(); }

TAmenboParams::TAmenboParams(const char* path)
    : TSmallEnemyParams(path)
    , PARAM_INIT(mSpeed, 10.0f)
    , PARAM_INIT(mAttackSpeed, 15.0f)
    , PARAM_INIT(mWaterFric, 0.995f)
    , PARAM_INIT(mEpsilon, 1.0f)
    , PARAM_INIT(mIsNotSearchTimerMax, 240)
    , PARAM_INIT(mSearchRange, 1000.0f)
    , PARAM_INIT(mSearchHeight, 200.0f)
    , PARAM_INIT(mTerritoryRange, 5000.0f)
    , PARAM_INIT(mHitWaterTimer, 600)
    , PARAM_INIT(mPreAttackTimer, 90)
{
	TParams::load(mPrmPath);
}

TAmenboManager::TAmenboManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TAmenboManager::load(JSUMemoryInputStream& stream)
{
	TAmenboParams* params = new TAmenboParams("/enemy/amenbo.prm");

	unk38 = params;

	params->mSLAttackRadius.set(60);
	params->mSLAttackHeight.set(120);
	params->mSLDamageRadius.set(100);
	params->mSLDamageHeight.set(150);
	params->mSLWallRadius.set(150.0f);

	TSmallEnemyManager::load(stream);
}

void TAmenboManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "amenbo_model1.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

const char** TAmenbo::getBasNameTable() const { return amenbo_bastable; }

DEFINE_NERVE(TNerveAmenboSearch, TLiveActor)
{
	TAmenbo* self = (TAmenbo*)spine->getBody();
	if (spine->getTime() == 0) {
		self->decideTarget();
		spine->pushAfterCurrent(&TNerveAmenboTurn::theNerve());
		return true;
	}

	if (self->checkCurAnmEnd(0)) {
		self->decideTargetOnFingingMario();
		spine->pushAfterCurrent(&TNerveAmenboTurn::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveAmenboTurn, TLiveActor)
{
	TAmenbo* self = (TAmenbo*)spine->getBody();
	if (spine->getTime() == 0) {
		self->changeBck("amenbo_run1", self->mIsChasingMario ? 3.0f : 1.0f);
	}

	self->doAdjustTarget();

	if (self->isStartMoving()) {
		if (self->mIsChasingMario) {
			spine->pushAfterCurrent(&TNerveAmenboPreAttack::theNerve());
		} else {
			spine->pushAfterCurrent(&TNerveAmenboWalk::theNerve());
		}
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveAmenboPreAttack, TLiveActor)
{
	TAmenbo* self = (TAmenbo*)spine->getBody();
	if (spine->getTime() == 0) {
		self->changeBck("amenbo_hit1_loop", 2.0f);
	}

	if (self->getSaveParam2()->mPreAttackTimer.get() < spine->getTime()) {
		self->changeBck("amenbo_run1", self->mIsChasingMario ? 3.0f : 1.0f);
		self->getMActor()->getFrameCtrl(0)->setFrame(63.0f);
		spine->pushAfterCurrent(&TNerveAmenboWalk::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveAmenboWalk, TLiveActor)
{
	TAmenbo* self = (TAmenbo*)spine->getBody();

	if (spine->getTime() == 0) {
		self->prepareWalk();
	}

	if (self->doWalk()) {
		spine->pushAfterCurrent(&TNerveAmenboSearch::theNerve());
		return true;
	}

	self->doChangeWaitAnm();

	return false;
}

DEFINE_NERVE(TNerveAmenboHitWater, TLiveActor)
{
	TAmenbo* self = (TAmenbo*)spine->getBody();

	if (spine->getTime() == 0) {
		if (self->getMActor()->checkCurAnm("amenbo_hit1_end", 0)
		    || self->getMActor()->checkCurAnm("amenbo_hit1_loop", 0)) {
			self->changeBck("amenbo_hit1_loop", 1.0f);
		} else if (!self->getMActor()->checkCurAnm("amenbo_hit1_start", 0)) {
			self->changeBck("amenbo_hit1_start", 1.0f);
		}
	}

	self->setVelocity(JGeometry::TVec3<f32>(0.0f));

	if (self->checkCurAnmEnd(0)) {
		if (self->getMActor()->checkCurAnm("amenbo_hit1_start", 0))
			self->changeBck("amenbo_hit1_loop", 1.0f);
		else if (self->getMActor()->checkCurAnm("amenbo_hit1_loop", 0)
		         && self->getSaveParam2()->mHitWaterTimer.get()
		                < spine->getTime())
			self->changeBck("amenbo_hit1_end", 1.0f);
		else if (self->getMActor()->checkCurAnm("amenbo_hit1_end", 0)) {
			spine->pushAfterCurrent(&TNerveAmenboSearch::theNerve());
			return true;
		}
	}

	return false;
}
