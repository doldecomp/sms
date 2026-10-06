#include <Enemy/Kazekun.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void SMS_CalcToDirMatrix(TPosition3f& param_1,
                         const JGeometry::TVec3<f32>& param_2,
                         const JGeometry::TVec3<f32>& param_3)
{
	JGeometry::TVec3<f32> zDir = param_2;
	if (zDir.isZero())
		zDir.set(0.0f, 0.0f, 1.0f);
	else
		zDir.normalize();

	JGeometry::TVec3<f32> xDir;
	xDir.cross(param_3, zDir);
	if (xDir.isZero())
		xDir.set(1.0f, 0.0f, 0.0f);
	else
		xDir.normalize();

	JGeometry::TVec3<f32> yDir;
	yDir.cross2(zDir, xDir);
	yDir.normalize();

	param_1.setXDir(xDir);
	param_1.setYDir(yDir);
	param_1.setZDir(zDir);
}

TKazekun::TKazekun(const char* param_1)
    : TSmallEnemy(param_1)
    , unk1B0(0)
{
	onLiveFlag(LIVE_FLAG_UNK10);
}

void TKazekun::init(TLiveManager* param_1)
{
	mManager = param_1;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("kazekun.bmd", 0);
	mSpine->initWith(&TNerveKazekunSearch::theNerve());
	initCollision();
	initParticle();
	initAnmSound();
	unk194.set(mPosition);
	reset();
}

void TKazekun::reset()
{
	unk1A0.set(0.0f, 0.0f, 0.0f, 1.0f);

	mPosition = unk194;

	setVisible(false);
}

void TKazekun::initCollision()
{
	mHeadHeight       = 40.0f;
	mBodyRadius       = 50.0f;
	mScaledBodyRadius = 50.0f;
	initHitActor(0x10000029, 1, HIT_CATEGORY_PLAYER, getBodyRadius(),
	             getHeadHeight(), getBodyRadius(), getHeadHeight());
	onHitFilter(HIT_FILTER_NO_COLLISION);
}

void TKazekun::initParticle()
{
	SMS_LoadParticle("/scene/kazekun/jpa/ms_kaze_appear.jpa",
	                 KAZEKUN_JPA_MS_KAZE_APPEAR);
	SMS_LoadParticle("/scene/kazekun/jpa/ms_kaze_wind.jpa",
	                 KAZEKUN_JPA_MS_KAZE_WIND);
	SMS_LoadParticle("/scene/kazekun/jpa/ms_kaze_blur.jpa",
	                 KAZEKUN_JPA_MS_KAZE_BLUR);
}

void TKazekun::calcRootMatrix()
{
	if (isTaken()) {
		TSpineEnemy::calcRootMatrix();
	} else {
		TPosition3f mtx;
		mtx.setQuat(unk1A0);
		mtx.setTrans(mPosition);
		getModel()->setBaseTRMtx(mtx);

		if (hasWind())
			updateEffect();
	}
}

void TKazekun::bind() { mPositionDelta.add(mVelocity); }

void TKazekun::behaveToWater(THitActor* param_1)
{
	if (isHitWater()) {
		mSpine->reset();
		mSpine->setNext(&TNerveKazekunHitWater::theNerve());
	}
}

bool TKazekun::isHitWater() const
{
	TSpineBase<TLiveActor>* spine = mSpine;
	return spine->getLatestNerve() == &TNerveKazekunTurn::theNerve()
	       || spine->getLatestNerve() == &TNerveKazekunPreAttack::theNerve()
	       || spine->getLatestNerve() == &TNerveKazekunAttack::theNerve();
}

bool TKazekun::isDamage() const { return isHitWater(); }

bool TKazekun::hasWind() const
{
	TSpineBase<TLiveActor>* spine = mSpine;
	return spine->getLatestNerve() == &TNerveKazekunTurn::theNerve()
	       || spine->getLatestNerve() == &TNerveKazekunPreAttack::theNerve()
	       || spine->getLatestNerve() == &TNerveKazekunAttack::theNerve()
	       || spine->getLatestNerve() == &TNerveKazekunHitWater::theNerve();
}

static const char* Kazekun_bastable[] = {
	"/scene/Kazekun/bas/kazekun_appear.bas",
	"/scene/Kazekun/bas/kazekun_attack.bas",
	nullptr,
	"/scene/Kazekun/bas/kazekun_vanish.bas",
	"/scene/Kazekun/bas/kazekun_wait.bas",
};

const char** TKazekun::getBasNameTable() const { return Kazekun_bastable; }

void TKazekun::attackToMario()
{
	if (isDamage())
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

bool TKazekun::isCollidMove(THitActor* param_1) { return false; }

void TKazekun::emitAppearEffect()
{
	gpMarioParticleManager->emit(KAZEKUN_JPA_MS_KAZE_APPEAR, &mPosition, 0,
	                             nullptr);
}

void TKazekun::updateEffect()
{
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    KAZEKUN_JPA_MS_KAZE_WIND, getModel()->getBaseTRMtx(), 1, this);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    KAZEKUN_JPA_MS_KAZE_BLUR, getModel()->getBaseTRMtx(), 1, this);
}

bool TKazekun::isGiveUpAround() const
{
	f32 offsetY = SMS_GetMarioY() - unk194.y;

	return offsetY < -((TKazekunParams*)getSaveParam())->mLostOffsetYDown.get()
	       || ((TKazekunParams*)getSaveParam())->mLostOffsetYUp.get() < offsetY;
}

void TKazekun::changeBck(const char* name)
{
	mMActor->setBck(name);
	setCurAnmSound();
}

void TKazekun::setDeadAnm()
{
	mMActor->getFrameCtrl(ANM_TYPE_BCK)->init(1);
	mMActor->getFrameCtrl(ANM_TYPE_BCK)->setFrame(0.0f);
}

void TKazekun::flyAroundMario()
{
	JGeometry::TVec3<f32> toMario = SMS_GetMarioPos();
	toMario.y += ((TKazekunParams*)getSaveParam())->mTurnOffsetY.get();
	toMario -= mPosition;

	f32 climb
	    = JGeometry::TUtil<f32>::clamp(toMario.y, -400.0f, 400.0f) * 0.0025f;
	toMario.y = 0.0f;

	JGeometry::TQuat4<f32> quat;
	getAroundQuat(quat, toMario, getAroundRate(toMario));
	unk1A0 = quat;

	JGeometry::TVec3<f32> velocity;
	velocity.set(0.0f, 0.0f, 1.0f);
	quat.rotate(velocity, velocity);
	velocity.y = climb;
	velocity.scale(1.0f + fabsf(climb));
	velocity.scale(((TKazekunParams*)getSaveParam())->mAroundSpeed.get());
	mPositionDelta = velocity;
}

f32 TKazekun::getAroundRate(const JGeometry::TVec3<f32>& dir) const
{
	return JGeometry::TUtil<f32>::clamp(
	    dir.length() / ((TKazekunParams*)getSaveParam())->mAroundDist.get(),
	    0.0f, 2.0f);
}

void TKazekun::getAroundQuat(JGeometry::TQuat4<f32>& quat,
                             const JGeometry::TVec3<f32>& dir, f32 rate)
{
	TPosition3f mtx;
	JGeometry::TQuat4<f32> rotation;
	JGeometry::TVec3<f32> axis;
	JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
	SMS_CalcToDirMatrix(mtx, dir, up);
	mtx.getQuat(quat);
	mtx.getYDir(axis);
	rotation.setRotate(axis, (2.0f - rate) * (M_PI / 2.0f));
	quat.mul(quat, rotation);
}

bool TKazekun::doAttackPose(bool start)
{
	JGeometry::TVec3<f32> toMario = SMS_GetMarioPos();
	toMario -= mPosition;
	toMario.y = 0.0f;

	if (start) {
		JGeometry::TQuat4<f32> quat;
		getAroundQuat(quat, toMario, 1.0f);
		unk1A0 = quat;

		JGeometry::TVec3<f32> velocity(
		    0.0f, 0.0f, ((TKazekunParams*)getSaveParam())->mPoseSpeed.get());
		quat.rotate(velocity, velocity);
		mVelocity = velocity;
	}

	JGeometry::TVec3<f32> spinAxis;
	unk1A0.rotate(JGeometry::TVec3<f32>(-1.0f, 0.0f, 0.0f), spinAxis);

	JGeometry::TQuat4<f32> spin;
	spin.setRotate(
	    spinAxis,
	    M_PI * ((TKazekunParams*)getSaveParam())->mPoseOmegaRate.get());
	spin.mul(spin, unk1A0);
	unk1A0 = spin;

	JGeometry::TVec3<f32> forward(0.0f, 0.0f, mVelocity.length());
	spin.rotate(forward, forward);
	mVelocity = forward;

	return false;
}

void TKazekun::doAttack(bool start)
{
	if (start) {
		JGeometry::TVec3<f32> dir = getUnk104().getPoint();
		dir -= mPosition;
		dir.setLength(((TKazekunParams*)getSaveParam())->mAttackSpeed.get());
		mVelocity = dir;
	}

	JGeometry::TQuat4<f32> quat = unk1A0;
	JGeometry::TQuat4<f32> target;
	getAroundQuat(target, mVelocity, 2.0f);
	quat.slerp(target, 0.1f);
	quat.normalize();
	unk1A0 = quat;
}

void TKazekun::setVisible(bool visible)
{
	if (visible) {
		offLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_UNK8);
	} else {
		onLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_UNK8);
		setAnmSound(nullptr);
	}
}

TKazekunParams::TKazekunParams(const char* param_1)
    : TSmallEnemyParams(param_1)
    , PARAM_INIT(mAppearDist, 1000.0f)
    , PARAM_INIT(mAroundDist, 400.0f)
    , PARAM_INIT(mAroundSpeed, 30.0f)
    , PARAM_INIT(mAroundTime, 600)
    , PARAM_INIT(mAttackSpeed, 30.0f)
    , PARAM_INIT(mAirFric, 0.97f)
    , PARAM_INIT(mResetTime, 300)
    , PARAM_INIT(mResetTimeHitting, 1500)
    , PARAM_INIT(mPoseTime, 120)
    , PARAM_INIT(mDicideTiming, 0.1f)
    , PARAM_INIT(mTurnOffsetY, 200.0f)
    , PARAM_INIT(mLostOffsetYUp, 500.0f)
    , PARAM_INIT(mLostOffsetYDown, 500.0f)
    , PARAM_INIT(mPoseSpeed, 7.6f)
    , PARAM_INIT(mPoseOmegaRate, 0.04f)
{
	TParams::load(mPrmPath);
}

TKazekunManager::TKazekunManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
}

void TKazekunManager::load(JSUMemoryInputStream& param_1)
{
	TKazekunParams* params = new TKazekunParams("/enemy/kazekun.prm");
	unk38                  = params;

	params->mSLAttackRadius.set(50);
	params->mSLAttackHeight.set(40);
	params->mSLDamageRadius.set(50);
	params->mSLDamageHeight.set(40);

	TSmallEnemyManager::load(param_1);
	unk5C = 0;
}

void TKazekunManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "kazekun.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

DEFINE_NERVE(TNerveKazekunSearch, TLiveActor)
{
	TKazekun* self = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0)
		self->reset();

	self->updateSquareToMario();

	f32 appearDist = ((TKazekunParams*)self->getSaveParam())->mAppearDist.get();
	if (self->getDistToMarioSquared() <= appearDist * appearDist) {
		spine->pushAfterCurrent(&TNerveKazekunAppear::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKazekunAppear, TLiveActor)
{
	TKazekun* self = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setVisible(true);
		self->emitAppearEffect();
		self->changeBck("kazekun_appear");
	}

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveKazekunTurn::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKazekunTurn, TLiveActor)
{
	TKazekun* self = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck("kazekun_wait");
		self->offHitFilter(HIT_FILTER_NO_COLLISION);
	}

	self->flyAroundMario();

	if (self->isGiveUpAround()) {
		spine->pushAfterCurrent(&TNerveKazekunDisappear::theNerve());
		return true;
	}

	if (((TKazekunParams*)self->getSaveParam())->mAroundTime.get()
	    < (f32)spine->getTime()) {
		spine->pushAfterCurrent(&TNerveKazekunPreAttack::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKazekunPreAttack, TLiveActor)
{
	TKazekun* self = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->doAttackPose(true);
		SMSGetMSound()->startSoundActor(MSD_SE_EN_KAZEKUN_READY,
		                                &self->mPosition, 0, nullptr, 0, 4);
		self->setAnmSound(nullptr);
	}

	if (((TKazekunParams*)self->getSaveParam())->mPoseTime.get()
	        * ((TKazekunParams*)self->getSaveParam())->mDicideTiming.get()
	    < spine->getTime()) {
		self->setGoalPath(TPathNode(SMS_GetMarioPos()));
	}

	self->doAttackPose(false);
	if (((TKazekunParams*)self->getSaveParam())->mPoseTime.get()
	    < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveKazekunAttack::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveKazekunAttack, TLiveActor)
{
	TKazekun* self = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck("kazekun_attack");
		self->doAttack(true);
	}
	self->doAttack(false);

	JGeometry::TVec3<f32> velocity = self->mVelocity;
	velocity.scale(((TKazekunParams*)self->getSaveParam())->mAirFric.get());
	self->mVelocity = velocity;
	if (velocity.squared() < 1.0f) {
		spine->pushAfterCurrent(&TNerveKazekunDisappear::theNerve());
		self->unk1B0
		    = ((TKazekunParams*)self->getSaveParam())->mResetTime.get();
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKazekunDisappear, TLiveActor)
{
	TKazekun* self = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck("kazekun_vanish");
		self->emitAppearEffect();
		self->mVelocity = JGeometry::TVec3<f32>(0.0f);
		self->onHitFilter(HIT_FILTER_NO_COLLISION);
	}

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveKazekunWait::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKazekunWait, TLiveActor)
{
	TKazekun* self = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setVisible(false);
	}

	if (self->unk1B0 < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveKazekunSearch::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKazekunHitWater, TLiveActor)
{
	TKazekun* self = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck("kazekun_hit");
		SMSGetMSound()->startSoundActor(MSD_SE_EN_KAZEKUN_DOWN,
		                                &self->mPosition, 0, nullptr, 0, 4);
	}

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveKazekunDisappear::theNerve());
		self->unk1B0
		    = ((TKazekunParams*)self->getSaveParam())->mResetTimeHitting.get();
		return true;
	}

	self->mVelocity = JGeometry::TVec3<f32>(0.0f);
	return false;
}
