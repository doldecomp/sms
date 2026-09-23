#include <Enemy/KazekunNerve.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MtxUtil.hpp>
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
// TODO: 91.3%. Every instruction matches except that retail keeps zAxis.y and
// zAxis.z in f30/f31 across the xAxis inv_sqrt call while we reload them from
// the stack, and its frame is 0xd0 against our 0xb0. The 0x20 is two extra
// callee-saved FPR slots plus 0x10 of inline temporaries, so retail's body
// creates four more bound temporaries than this spelling does; adding them via
// TUtil<f32>::one() in the two fallbacks buys 8 of the 32 bytes and changes no
// instruction. cross2, setLength(1.0f), setXYZDir and declaring all three axes
// up front were all tried and are no better.
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
	// The identity quaternion's w comes from TUtil<f32>::one() rather than a
	// literal: its inlined return value is the 4-byte temporary that sits
	// below the by-value getHomePosition() copy in the retail frame.
	mQuat.set(0.0f, 0.0f, 0.0f, JGeometry::TUtil<f32>::one());

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

// TODO: 98.1%, and the only difference is the frame: 0x68 in retail against
// our 0x70, i.e. we build 8 bytes more inline temporaries before the local
// matrix. setTrans(x, y, z), getMActor()->getModel() and checkTakeFlag in
// place of isTaken() are all worse, so the extra pair sits inside one of the
// inlined helpers (hasWind or updateEffect), not here. Also inert or worse
// (2026-09-22): raw mHolder tests, mMActor->getModel() in updateEffect,
// three-float setTrans; `hasWind() == true` lands the frame but drops the
// match to 80.8.
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

// UNUSED, 0xf0 in the map: the same three nerves as isHitWater, spelled
// through it. The forwarder is what the map's four-byte gap against
// isHitWater's 0xf4 records, and it is what puts theNerve() one level deeper
// at the attackToMario call site, where retail calls TNerveBase<TLiveActor>::
// TNerveBase() instead of expanding it.
bool TKazekun::isDamage() const { return isHitWater(); }

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
	return dy < -getSaveParams()->mLostOffsetYDown.get()
	    || getSaveParams()->mLostOffsetYUp.get() < dy;
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

// TODO: 93.6%. Reading the three params through raw .value (here and in the
// inlined getAroundRate) closes the 0x28 frame overshoot. Left: FPR
// allocation and scheduling inside the inlined TQuat4::mul/rotate (f27-f30
// rotated by one); rewriting JGeometry::TQuat4<f32>::rotate with scalar
// locals (see the TODO in JGQuat4.hpp) regresses six other callers.
// Retail's named block also sits 4 bytes higher (toMario 0x104, ours 0x100)
// at an equal frame. Inert or worse (2026-09-23): a named clamp `dy` (moves
// toMario only), 0.0025f first, / 400.0f, quat/vel/f31 declared up front,
// named scale, speed and rate locals, and a named angle in getAroundQuat
// (drops the Attack nerve to 68%).
// After the TQuat4::mul rewrite (97.3%) the frame is 0x10 short (0x130
// against 0x140): retail has 8 more bytes below getAroundQuat's mtx and 8
// between vel and its up vector. mAroundDist through get() in getAroundRate
// gives 0x138 with toMario at retail's offset from the top; the other get()
// subsets overshoot or are inert (2026-09-23).
void TKazekun::flyAroundMario()
{
	JGeometry::TVec3<f32> toMario(*gpMarioPos);
	toMario.y += getSaveParams()->mTurnOffsetY.value;
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
	vel.scale(getSaveParams()->mAroundSpeed.value);
	mLinearVelocity = vel;
}

// UNUSED, 0xc0 in the map.
// TODO: incorrect size -- 0xbc, one instruction short. The params pointer has
// to be named for the virtual getSaveParam() call to land before dir.dot(dir),
// and dir.length() is what puts both dot and TUtil<f32>::sqrt out of line at
// the flyAroundMario call site, so the shape is right and the missing
// instruction is somewhere in the clamp.
f32 TKazekun::getAroundRate(const JGeometry::TVec3<f32>& dir) const
{
	TKazekunParams* params = getSaveParams();
	f32 rate               = dir.length() / params->mAroundDist.value;
	return rate < 0.0f ? 0.0f : (rate > 2.0f ? 2.0f : rate);
}

// UNUSED, 0x3d8 in the map: turns `quat` into the heading that flies around
// `dir`, banked `rate` of the way from a full right angle (rate 0) to straight
// at `dir` (rate 2). The rotation axis is the frame's own up vector, which is
// what makes the spirit bank into the turn instead of yawing flat.
// The up vector is an unnamed argument: retail builds it right before the call
// and materialises its address first (r5), which a named `up` does not.
// TODO: incorrect size -- 0x3e0 against the map's 0x3d8, two instructions over
// (0x3dc with a named `up`, which misorders the argument registers at both
// inlined sites in TNerveKazekunAttack).
void TKazekun::getAroundQuat(JGeometry::TQuat4<f32>& quat,
                             const JGeometry::TVec3<f32>& dir, f32 rate)
{
	TPosition3f mtx;
	JGeometry::TQuat4<f32> around;
	JGeometry::TVec3<f32> axis;

	SMS_CalcToDirMatrix(mtx, dir, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
	mtx.getQuat(quat);
	mtx.getYDir(axis);

	// rate 0 turns a full right angle away from `dir`, rate 2 heads straight
	// along it. Spelling this as setRotate rather than a named half-angle plus
	// scale/cos keeps the helper cheap enough for MWCC to expand it inside the
	// inlined doAttack, which is what the retail Attack nerve does.
	around.setRotate(axis, (2.0f - rate) * 1.5707964f);

	quat.mul(quat, around);
}

// TODO: 82.4%. The speed is the length of an unnamed copy of mVelocity:
// that is what keeps the copy in memory and makes MWCC call
// TUtil<f32>::sqrt out of line, as retail does (a named copy, by assignment
// or construction, expands sqrt). Left: the frame is 0x218 in retail against
// our 0x280. Retail keeps spin above toMario and forward above the velocity
// copy; the rest of the gap is the rotate temporaries (quat.rotateInPlace at
// the start site alone is -0x10 at equal match), which is the JGQuat4 rotate
// body, not this function. Retail also materialises &mQuat in r30 for
// spin.mul: a `const TQuat4<f32>& cur = mQuat` read by the down rotate and
// the mul reproduces it (83.7) but is an invented binder, so it is not used.
// Inert: spin declared first, forward declared before the copy, vel.set with
// a raw param read, one-argument in-place rotates (worse).
bool TKazekun::doAttackPose(bool start)
{
	JGeometry::TVec3<f32> toMario(*gpMarioPos);
	toMario.sub(mPosition);
	toMario.y = 0.0f;

	if (start) {
		// rate 1 is halfway between flying around Mario and flying
		// straight at him, so the pose leans 45 degrees into the charge;
		// MWCC folds getAroundQuat's angle down to sinf/cosf(pi/4).
		JGeometry::TQuat4<f32> quat;
		getAroundQuat(quat, toMario, 1.0f);
		mQuat = quat;

		TKazekunParams* params = getSaveParams();
		JGeometry::TVec3<f32> vel(0.0f, 0.0f,
		                          params->mPoseSpeed.get());
		quat.rotate(vel, vel);
		mVelocity = vel;
	}

	JGeometry::TVec3<f32> down;
	mQuat.rotate(JGeometry::TVec3<f32>(0.0f, -1.0f, 0.0f), down);

	JGeometry::TQuat4<f32> spin;
	spin.setRotate(down, 3.1415927f * getSaveParams()->getPoseOmegaRate());
	spin.mul(spin, mQuat);
	mQuat = spin;

	JGeometry::TVec3<f32> forward(0.0f, 0.0f,
	                              JGeometry::TVec3<f32>(mVelocity).length());
	spin.rotate(forward, forward);
	mVelocity = forward;

	return false;
}

// UNUSED, 0x4b0 in the map. TNerveKazekunAttack inlines it twice: once with
// `start` true, to aim the charge at the goal path node, and once with it
// false, to keep easing the facing towards the flight direction. Only the
// charge gets the full right angle (rate 2), which MWCC folds getAroundQuat's
// angle down to sinf/cosf(0).
// TODO: incorrect size -- 0x4d0 against the map's 0x4b0, eight instructions
// over. The inlined copies in the nerve are only eight bytes of frame off, so
// the surplus is in this out-of-line copy's own expansion of getAroundQuat.
void TKazekun::doAttack(bool start)
{
	if (start) {
		changeBck("kazekun_attack");

		JGeometry::TVec3<f32> toGoal(getUnk104().getPoint());
		toGoal.sub(mPosition);
		toGoal.setLength(getSaveParams()->getAttackSpeed());
		mVelocity = toGoal;
	}

	JGeometry::TQuat4<f32> target;
	JGeometry::TQuat4<f32> quat = mQuat;
	JGeometry::TVec3<f32> dir   = mVelocity;

	getAroundQuat(target, dir, 2.0f);

	quat.slerp(target, 0.1f);
	quat.normalize();
	mQuat = quat;
}

// UNUSED, 0x4c in the map: the spirit is invisible between attack runs.
// TODO: incorrect size -- this is 0x2c, eight instructions short of the map's
// 0x4c, yet all three call sites (reset, the Appear nerve and the Wait nerve)
// match byte-for-byte with just this flag pair, so the inlined body is right
// and the out-of-line copy did something more. The map also lists a weak
// SMS_EasyEmitParticle<E_SMS_EFFECT_LOOP_INDIRECT> instantiation for this TU
// that nothing in the reconstruction accounts for; this function and doAttack
// are the only places it can hide.
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

// TODO: 99.9%, every instruction right. Frame 0x1e8 against retail's 0x1f0:
// retail's `vel` sits 0xc higher (right under the saved registers) and the
// second inlined getAroundQuat block (mtx 0x8c, up 0xd8) 4 bytes lower.
// Inert or worse after the TQuat4::mul rewrite (2026-09-23): the five
// doAttack orders of target/quat/dir, quat assigned after declaration,
// toGoal.sub(getPosition()), a named attack speed, raw mAirFric/mResetTime
// reads and their pairs (these restore the size but shift every slot).
DEFINE_NERVE(TNerveKazekunAttack, TLiveActor)
{
	TKazekun* kazekun = (TKazekun*)spine->getBody();

	if (spine->getTime() == 0)
		kazekun->doAttack(true);

	kazekun->doAttack(false);

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

// TODO: 99.9%. The zero-velocity temporary sits at 0x38 in retail and 0x34
// here, so retail builds one more four-byte inline temporary ahead of it; the
// frame size itself already agrees.
// By-value level over the reset-time parameter: +4 of low pool, landing the
// velocity temporary at retail's slot in TNerveKazekunHitWater::execute.
static inline int KazekunResetTimeHitting(const TKazekun* kazekun)
{
	return kazekun->getSaveParams()->getResetTimeHitting();
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
		int sleep           = KazekunResetTimeHitting(kazekun);
		kazekun->mSleepTime = sleep;
		return TRUE;
	}

	kazekun->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
	return FALSE;
}
