#include <Enemy/KazekunNerve.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <M3DUtil/MActor.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <Player/MarioAccess.hpp>
#include <System/Particles.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// Kazekun_bastable names slots 0, 1, 3 and 4; slot 2 has no .bas of its own.
// The model's .bck files are indexed alphabetically, which is what puts
// kazekun_vanish at 3 and kazekun_wait at 4 with kazekun_hit at 2.
enum {
	KAZEKUN_ANM_APPEAR = 0, // "kazekun_appear"
	KAZEKUN_ANM_ATTACK = 1, // "kazekun_attack"
	KAZEKUN_ANM_HIT    = 2, // "kazekun_hit", no .bas entry
	KAZEKUN_ANM_VANISH = 3, // "kazekun_vanish"
	KAZEKUN_ANM_WAIT   = 4, // "kazekun_wait"
};

// TODO: these three particle ids have no names in include/System/Particles.hpp
// yet; they belong there. This batch is only allowed to touch Kazekun's own
// two files, so they live here for now. The names come from the .jpa paths
// TKazekun::initParticle loads.
enum {
	KAZEKUN_JPA_MS_KAZE_APPEAR = 0xCF,
	KAZEKUN_JPA_MS_KAZE_WIND   = 0x189,
	KAZEKUN_JPA_MS_KAZE_BLUR   = 0x18A,
};

static const char* Kazekun_bastable[] = {
	"/scene/Kazekun/bas/kazekun_appear.bas",
	"/scene/Kazekun/bas/kazekun_attack.bas",
	nullptr,
	"/scene/Kazekun/bas/kazekun_vanish.bas",
	"/scene/Kazekun/bas/kazekun_wait.bas",
};

// Builds an orthonormal frame whose Z axis points along `dir`, using `up` as
// the hint for the other two. The map has this as a global function living in
// this TU; wireTrap and fireWanwan call it out of line.
//
// TODO: include/MarioUtil/MtxUtil.hpp currently *defines* this as an inline,
// which is wrong -- the map shows one global copy, here. That header has to be
// reduced to a declaration, but it is out of scope for this batch, so this TU
// deliberately does not include it.
void SMS_CalcToDirMatrix(TPosition3f& mtx, const JGeometry::TVec3<f32>& dir,
                         const JGeometry::TVec3<f32>& up)
{
	JGeometry::TVec3<f32> zAxis = dir;
	if (zAxis.isZero())
		zAxis.set(0.0f, 0.0f, 1.0f);
	else
		zAxis.normalize();

	JGeometry::TVec3<f32> xAxis;
	xAxis.cross(up, zAxis);
	if (xAxis.isZero())
		xAxis.set(1.0f, 0.0f, 0.0f);
	else
		xAxis.normalize();

	JGeometry::TVec3<f32> yAxis;
	yAxis.cross(zAxis, xAxis);
	yAxis.normalize();

	mtx.setXDir(xAxis);
	mtx.setYDir(yAxis);
	mtx.setZDir(zAxis);
}

TKazekun::TKazekun(const char* name)
    : TSmallEnemy(name)
{
	mSleepTime = 0;
	onLiveFlag(LIVE_FLAG_UNK10);
}

void TKazekun::init(TLiveManager* live_manager)
{
	mManager = live_manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("kazekun.bmd", 0);

	mSpine->initWith(&TNerveKazekunSearch::theNerve());

	initCollision();
	initParticle();
	initAnmSound();

	mHomePosition.set(mPosition);
	reset();
}

void TKazekun::reset()
{
	mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);

	mPosition.set(getHomePosition());

	setVisible(false);
	setAnmSound(nullptr);
}

// UNUSED, 0x78 in the map: the collision setup init() opens with.
void TKazekun::initCollision()
{
	mHeadHeight       = 40.0f;
	mBodyRadius       = 50.0f;
	mScaledBodyRadius = 50.0f;

	initHitActor(0x10000029, 1, 0x80000000, getBodyRadius(), getHeadHeight(),
	             getBodyRadius(), getHeadHeight());
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

// UNUSED, 0xc4 in the map.
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
		mtx.setQuat(mQuat);
		mtx.setTrans(mPosition);
		getModel()->setBaseTRMtx(mtx);

		if (hasWind())
			updateEffect();
	}
}

void TKazekun::bind() { mLinearVelocity.add(mVelocity); }

void TKazekun::behaveToWater(THitActor* water)
{
	if (isHitWater()) {
		mSpine->reset();
		mSpine->setNext(&TNerveKazekunHitWater::theNerve());
	}
}

// UNUSED, 0xf4 in the map.
bool TKazekun::isHitWater() const
{
	TSpineBase<TLiveActor>* spine = mSpine;
	return spine->getLatestNerve() == &TNerveKazekunTurn::theNerve()
	    || spine->getLatestNerve() == &TNerveKazekunPreAttack::theNerve()
	    || spine->getLatestNerve() == &TNerveKazekunAttack::theNerve();
}

// UNUSED, 0xf0 in the map.
bool TKazekun::isDamage() const
{
	TSpineBase<TLiveActor>* spine = mSpine;
	return spine->getLatestNerve() == &TNerveKazekunTurn::theNerve()
	    || spine->getLatestNerve() == &TNerveKazekunPreAttack::theNerve()
	    || spine->getLatestNerve() == &TNerveKazekunAttack::theNerve();
}

// UNUSED, 0x104 in the map: the wind and the motion blur only show while the
// spirit is flying, not while it is appearing or vanishing.
bool TKazekun::hasWind() const
{
	TSpineBase<TLiveActor>* spine = mSpine;
	return spine->getLatestNerve() == &TNerveKazekunTurn::theNerve()
	    || spine->getLatestNerve() == &TNerveKazekunPreAttack::theNerve()
	    || spine->getLatestNerve() == &TNerveKazekunAttack::theNerve()
	    || spine->getLatestNerve() == &TNerveKazekunHitWater::theNerve();
}

const char** TKazekun::getBasNameTable() const { return Kazekun_bastable; }

void TKazekun::attackToMario()
{
	if (isDamage())
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

bool TKazekun::isCollidMove(THitActor* other) { return false; }

// UNUSED, 0x38 in the map.
void TKazekun::emitAppearEffect()
{
	gpMarioParticleManager->emit(KAZEKUN_JPA_MS_KAZE_APPEAR, &mPosition, 0,
	                             nullptr);
}

// UNUSED, 0x64 in the map: the two looping emitters bound to the model's own
// matrix, so they travel with it.
void TKazekun::updateEffect()
{
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    KAZEKUN_JPA_MS_KAZE_WIND, getModel()->getBaseTRMtx(), 1, this);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    KAZEKUN_JPA_MS_KAZE_BLUR, getModel()->getBaseTRMtx(), 1, this);
}

// UNUSED, 0x94 in the map: Mario has climbed out of the spirit's reach.
bool TKazekun::isGiveUpAround() const
{
	f32 dy = gpMarioPos->y - mHomePosition.y;
	return dy < -getSaveParams()->getLostOffsetYDown()
	    || getSaveParams()->getLostOffsetYUp() < dy;
}

// UNUSED, 0x38 in the map.
void TKazekun::changeBck(const char* name)
{
	getMActor()->setBck(name);
	setCurAnmSound();
}

void TKazekun::setDeadAnm()
{
	getMActor()->getFrameCtrl(ANM_TYPE_BCK)->init(1);
	getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setFrame(0.0f);
}

void TKazekun::flyAroundMario()
{
	JGeometry::TVec3<f32> toMario(*gpMarioPos);
	toMario.y += getSaveParams()->getTurnOffsetY();
	toMario.sub(mPosition);

	f32 f31 = (toMario.y < -400.0f
	               ? -400.0f
	               : (toMario.y > 400.0f ? 400.0f : toMario.y))
	    * 0.0025f;
	toMario.y = 0.0f;

	JGeometry::TQuat4<f32> quat;
	getAroundQuat(quat, toMario, getAroundRate(toMario));
	mQuat = quat;

	JGeometry::TVec3<f32> vel;
	vel.set(0.0f, 0.0f, 1.0f);
	quat.rotate(vel, vel);
	vel.y = f31;
	vel.scale(1.0f + fabsf(f31));
	vel.scale(getSaveParams()->getAroundSpeed());
	mLinearVelocity = vel;
}

// UNUSED, 0xc0 in the map.
f32 TKazekun::getAroundRate(const JGeometry::TVec3<f32>& dir) const
{
	TKazekunParams* params = getSaveParams();
	f32 rate               = dir.length() / params->mAroundDist.get();
	return rate < 0.0f ? 0.0f : (rate > 2.0f ? 2.0f : rate);
}

// UNUSED, 0x3d8 in the map: turns `quat` into the heading that flies around
// `dir`, banked `rate` of the way from a full right angle (rate 0) to straight
// at `dir` (rate 2). The rotation axis is the frame's own up vector, which is
// what makes the spirit bank into the turn instead of yawing flat.
void TKazekun::getAroundQuat(JGeometry::TQuat4<f32>& quat,
                             const JGeometry::TVec3<f32>& dir, f32 rate)
{
	TPosition3f mtx;
	JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
	SMS_CalcToDirMatrix(mtx, dir, up);
	mtx.getQuat(quat);

	f32 halfAngle = 0.5f * ((2.0f - rate) * 1.5707964f);

	JGeometry::TVec3<f32> axis;
	mtx.getYDir(axis);

	JGeometry::TQuat4<f32> around;
	around.xyz().scale(sinf(halfAngle), axis);
	around.w = cosf(halfAngle);

	quat.mul(quat, around);
}

void TKazekun::doAttackPose(bool start)
{
	JGeometry::TVec3<f32> toMario(*gpMarioPos);
	toMario.sub(mPosition);
	toMario.y = 0.0f;

	if (start) {
		TPosition3f mtx;
		JGeometry::TVec3<f32> up;
		up.set(0.0f, 1.0f, 0.0f);
		SMS_CalcToDirMatrix(mtx, toMario, up);

		JGeometry::TQuat4<f32> quat;
		mtx.getQuat(quat);

		JGeometry::TQuat4<f32> lean;
		lean.xyz().scale(sinf(0.7853982f),
		                 JGeometry::TVec3<f32>(mtx.at(0, 1), mtx.at(1, 1),
		                                       mtx.at(2, 1)));
		lean.w = cosf(0.7853982f);

		quat.mul(lean);
		mQuat = quat;

		JGeometry::TVec3<f32> vel(0.0f, 0.0f, getSaveParams()->getPoseSpeed());
		quat.rotate(vel);
		mVelocity = vel;
	}

	JGeometry::TVec3<f32> down;
	mQuat.rotate(JGeometry::TVec3<f32>(0.0f, -1.0f, 0.0f), down);

	JGeometry::TQuat4<f32> spin;
	spin.setRotate(down, 3.1415927f * getSaveParams()->getPoseOmegaRate());
	spin.mul(mQuat);
	mQuat = spin;

	JGeometry::TVec3<f32> vel = mVelocity;
	JGeometry::TVec3<f32> forward(0.0f, 0.0f, vel.length());
	mQuat.rotate(forward);
	mVelocity = forward;
}

// UNUSED, 0x4b0 in the map, and dead: nothing in the TU has the right shape
// for a call to it, and doAttackPose is what the nerves use instead. Left
// unreconstructed rather than guessed at 300 instructions.
// TODO: incorrect size. Map records 1200 bytes.
void TKazekun::doAttack(bool start) { }

// UNUSED, 0x4c in the map: the spirit is invisible between attack runs.
void TKazekun::setVisible(bool visible)
{
	if (visible)
		offLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_UNK8);
	else
		onLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_UNK8);
}

TKazekunParams::TKazekunParams(const char* prm)
    : TSmallEnemyParams(prm)
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

TKazekunManager::TKazekunManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TKazekunManager::load(JSUMemoryInputStream& stream)
{
	TKazekunParams* params = new TKazekunParams("/enemy/kazekun.prm");
	unk38                  = params;

	params->mSLAttackRadius.set(50);
	params->mSLAttackHeight.set(40);
	params->mSLDamageRadius.set(50);
	params->mSLDamageHeight.set(40);

	TSmallEnemyManager::load(stream);

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
	TKazekun* kazekun = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0)
		kazekun->reset();

	kazekun->updateSquareToMario();

	f32 dist = kazekun->getSaveParams()->mAppearDist.get();
	if (kazekun->getDistToMarioSquared() <= dist * dist) {
		spine->pushAfterCurrent(&TNerveKazekunAppear::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveKazekunAppear, TLiveActor)
{
	TKazekun* kazekun = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		kazekun->setVisible(true);
		kazekun->emitAppearEffect();
		kazekun->changeBck("kazekun_appear");
	}

	if (kazekun->checkCurAnmEnd(ANM_TYPE_BCK)) {
		spine->pushAfterCurrent(&TNerveKazekunTurn::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveKazekunTurn, TLiveActor)
{
	TKazekun* kazekun = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		kazekun->changeBck("kazekun_wait");
		kazekun->offHitFlag(HIT_FLAG_NO_COLLISION);
	}

	kazekun->flyAroundMario();

	if (kazekun->isGiveUpAround()) {
		spine->pushAfterCurrent(&TNerveKazekunDisappear::theNerve());
		return TRUE;
	}

	int time = spine->getTime();
	if (kazekun->getSaveParams()->mAroundTime.get() < (f32)time) {
		spine->pushAfterCurrent(&TNerveKazekunPreAttack::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveKazekunPreAttack, TLiveActor)
{
	TKazekun* kazekun = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		kazekun->doAttackPose(true);
		SMSGetMSound()->startSoundActor(MSD_SE_EN_KAZEKUN_READY,
		                                &kazekun->mPosition, 0, nullptr, 0, 4);
		kazekun->setAnmSound(nullptr);
	}

	if (kazekun->getSaveParams()->getPoseTime()
	        * kazekun->getSaveParams()->mDicideTiming.get()
	    < spine->getTime()) {
		JGeometry::TVec3<f32> marioPos = SMS_GetMarioPos();
		kazekun->setGoalPath(TPathNode(marioPos));
	}

	kazekun->doAttackPose(false);

	if (kazekun->getSaveParams()->getPoseTime() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveKazekunAttack::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveKazekunAttack, TLiveActor)
{
	TKazekun* kazekun = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		kazekun->changeBck("kazekun_attack");

		JGeometry::TVec3<f32> toGoal(kazekun->getUnk104().getPoint());
		toGoal.sub(kazekun->mPosition);
		toGoal.setLength(toGoal, kazekun->getSaveParams()->getAttackSpeed());
		kazekun->mVelocity = toGoal;

		JGeometry::TQuat4<f32> quat = kazekun->mQuat;
		JGeometry::TVec3<f32> dir   = kazekun->mVelocity;
		kazekun->getAroundQuat(quat, dir, 2.0f);
		kazekun->mQuat = quat;
	}

	JGeometry::TQuat4<f32> quat = kazekun->mQuat;
	JGeometry::TVec3<f32> dir   = kazekun->mVelocity;
	kazekun->getAroundQuat(quat, dir, 2.0f);
	kazekun->mQuat = quat;

	JGeometry::TVec3<f32> vel = kazekun->mVelocity;
	vel.scale(kazekun->getSaveParams()->getAirFric());
	kazekun->mVelocity = vel;

	if (vel.squared() < 1.0f) {
		spine->pushAfterCurrent(&TNerveKazekunDisappear::theNerve());
		kazekun->mSleepTime = kazekun->getSaveParams()->getResetTime();
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveKazekunDisappear, TLiveActor)
{
	TKazekun* kazekun = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		kazekun->changeBck("kazekun_vanish");
		kazekun->emitAppearEffect();
		kazekun->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
		kazekun->onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (kazekun->checkCurAnmEnd(ANM_TYPE_BCK)) {
		spine->pushAfterCurrent(&TNerveKazekunWait::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveKazekunWait, TLiveActor)
{
	TKazekun* kazekun = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		kazekun->setVisible(false);
		kazekun->setAnmSound(nullptr);
	}

	if (kazekun->mSleepTime < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveKazekunSearch::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveKazekunHitWater, TLiveActor)
{
	TKazekun* kazekun = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0) {
		kazekun->changeBck("kazekun_hit");
		SMSGetMSound()->startSoundActor(MSD_SE_EN_KAZEKUN_DOWN,
		                                &kazekun->mPosition, 0, nullptr, 0, 4);
	}

	if (kazekun->checkCurAnmEnd(ANM_TYPE_BCK)) {
		spine->pushAfterCurrent(&TNerveKazekunDisappear::theNerve());
		int sleep           = kazekun->getSaveParams()->getResetTimeHitting();
		kazekun->mSleepTime = sleep;
		return TRUE;
	}

	kazekun->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
	return FALSE;
}
