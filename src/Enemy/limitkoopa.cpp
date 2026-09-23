#include <Enemy/LimitKoopaNerve.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/KoopaJr.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <System/Particles.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <math.h>
#include <macros.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

namespace {
int KoopaNeckCallBack(J3DNode*, int);
} // namespace

// ---------------------------------------------------------------------------
// Nerves
// ---------------------------------------------------------------------------

BOOL TNerveLimitKoopaHipDropJump::execute(TSpineBase<TLiveActor>* spine) const
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();

	koopa->mAcceleration.set(0.0f, -koopa->getGravityY(), 0.0f);

	if (koopa->mLanded) {
		spine->pushAfterCurrent(&TNerveLimitKoopaWait::theNerve());
		return TRUE;
	}

	return FALSE;
}

// Named-body binder: +8 of low region, the HipDrop-start nerve's frame.
static inline TLimitKoopa* LimitKoopaGetBody(TSpineBase<TLiveActor>* spine)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	return koopa;
}

BOOL TNerveLimitKoopaHipDropStart::execute(TSpineBase<TLiveActor>* spine) const
{
	TLimitKoopa* koopa = LimitKoopaGetBody(spine);

	if (spine->getTime() == 0) {
		koopa->changeBck(KOOPA_ANM_FIRE_START, 2.0f);
		koopa->mTimers[LIMITKOOPA_TIMER_HIPDROP] = 30;
	}

	if (koopa->mTimers[LIMITKOOPA_TIMER_HIPDROP] <= 0) {
		koopa->startHipDrop();
		koopa->mLanded = false;
		spine->pushAfterCurrent(&TNerveLimitKoopaHipDropJump::theNerve());
		return TRUE;
	}

	return FALSE;
}

BOOL TNerveLimitKoopaWait::execute(TSpineBase<TLiveActor>* spine) const
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();

	if (spine->getTime() == 0) {
		koopa->changeBck(KOOPA_ANM_TURN_L, 2.0f);
		koopa->mTimers[LIMITKOOPA_TIMER_WAIT] = 240;
	}

	koopa->moveTurn();

	if (koopa->finishedTurn()
	    && koopa->getMActor()->isCurAnmAlreadyEnd(ANM_TYPE_BCK)) {
		spine->pushAfterCurrent(&TNerveLimitKoopaHipDropStart::theNerve());
		return TRUE;
	}

	return FALSE;
}

BOOL TNerveLimitKoopaTumble::execute(TSpineBase<TLiveActor>* spine) const
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	f32 rate           = koopa->getParam()->getTumbleSpeed();
	koopa->changeBck(KOOPA_ANM_HIPDROP, rate);
	if (koopa->getAnmEnd())
		return TRUE;
	return FALSE;
}

// TODO: UNUSED (0x6c), body not reconstructed.
BOOL TNerveLimitKoopaFall::execute(TSpineBase<TLiveActor>*) const
{
	return FALSE;
}

// TODO: UNUSED (0x170), body not reconstructed.
BOOL TNerveLimitKoopaFlame::execute(TSpineBase<TLiveActor>*) const
{
	return FALSE;
}

BOOL TNerveLimitKoopaStagger::execute(TSpineBase<TLiveActor>* spine) const
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	f32 rate           = koopa->getParam()->getStaggerSpeed();
	koopa->changeBck(KOOPA_ANM_STAGGER, rate);
	if (koopa->getAnmEnd())
		return TRUE;
	return FALSE;
}

BOOL TNerveLimitKoopaGetShowered::execute(TSpineBase<TLiveActor>* spine) const
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	f32 rate           = koopa->getParam()->getWaterhitSpeed();
	koopa->changeBck(KOOPA_ANM_WATERHIT, rate);
	if (koopa->getAnmEnd())
		return TRUE;
	return FALSE;
}

static inline int LimitKoopaAnmIndex(const TLimitKoopa* koopa)
{
	int index = koopa->getAnmIndex();
	return index;
}

static inline TLimitKoopaParams* LimitKoopaGetParam(const TLimitKoopa* koopa)
{
	TLimitKoopaParams* param = koopa->getParam();
	return param;
}

BOOL TNerveLimitKoopaGetDown::execute(TSpineBase<TLiveActor>* spine) const
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();

	switch (LimitKoopaAnmIndex(koopa)) {
	case KOOPA_ANM_DOWN:
		if (koopa->getAnmEnd())
			koopa->changeBck(KOOPA_ANM_DOWN_WAIT,
			                 LimitKoopaGetParam(koopa)->getDownSpeed());
		break;
	case KOOPA_ANM_DOWN_WAIT:
		if (koopa->getAnmEnd())
			koopa->changeBck(KOOPA_ANM_GETUP,
			                 koopa->getParam()->getDownSpeed());
		break;
	case KOOPA_ANM_GETUP:
		if (koopa->getAnmEnd())
			return TRUE;
		break;
	default:
		koopa->changeBck(KOOPA_ANM_DOWN,
		                 koopa->getParam()->getDownSpeed());
		break;
	}

	return FALSE;
}

// ---------------------------------------------------------------------------
// TLimitKoopaParts and its four hit boxes
// ---------------------------------------------------------------------------
TLimitKoopaParts::TLimitKoopaParts(const char* name, u32 actor_type,
                                   TLimitKoopa* owner, f32 radius)
    : TLiveActor(name)
    , mOwner(owner)
{
	// Spelled out rather than through search<T>: one level shallower, so the
	// generator accessors and TNameRef::search expand inside loadAfter's four
	// part constructors as they do in retail.
	((TIdxGroupObj*)JDrama::TNameRefGen::getInstance()
	     ->getRootNameRef()
	     ->search("敵グループ"))
	    ->getChildren()
	    .push_back(this);
	initHitActor(actor_type, 5, 0x80000000, radius, radius, radius, radius);
	onHitFlag(HIT_FLAG_CANNOT_ATTACK);
	onHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TLimitKoopaParts::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TLiveActor::perform(cue, graphics);

	if (cue & CUE_MOVE) {
		for (int i = 0; i < mColCount; i++)
			attack_(mCollisions[i]);
	}
}

// UNUSED (0x84).
void TLimitKoopaParts::set(const JGeometry::TVec3<f32>& position, f32 radius,
                           f32 height)
{
	if (height <= 0.0f)
		height = 2.0f * radius;
	mPosition.set(position);
	offHitFlag(HIT_FLAG_CANNOT_ATTACK);
	offHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mAttackRadius = radius;
	mAttackHeight = height;
	mDamageRadius = radius;
	mDamageHeight = height;
	calcEntryRadius();
}

// UNUSED (0x28).
void TLimitKoopaParts::remove()
{
	onHitFlag(HIT_FLAG_CANNOT_ATTACK);
	onHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

TLimitKoopaFlame::TLimitKoopaFlame(TLimitKoopa* owner)
    : TLimitKoopaParts("クッパの吐く炎", 0x08000030, owner, 100.0f)
{
}

BOOL TLimitKoopaFlame::receiveMessage(THitActor*, u32 message)
{
	// A signed `cmpwi` on a u32 parameter plus the `beq case; b default` pair
	// is a one-arm switch, not an if/else (rules card, "Structure").
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		return FALSE;
	}

	return TRUE;
}

void TLimitKoopaFlame::attack_(THitActor* other)
{
	if (other->receiveMessage(this, HIT_MESSAGE_UNKA)) {
		mOwner->stopFlame();
	}
}

TLimitKoopaHand::TLimitKoopaHand(TLimitKoopa* owner)
    : TLimitKoopaParts("クッパの手", 0x08000032, owner, 100.0f)
{
}

BOOL TLimitKoopaHand::receiveMessage(THitActor*, u32) { return TRUE; }

void TLimitKoopaHand::attack_(THitActor* other)
{
	other->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

TLimitKoopaHead::TLimitKoopaHead(TLimitKoopa* owner)
    : TLimitKoopaParts("クッパの頭", 0x08000031, owner, 100.0f)
{
}

// Owner binder (+8 of the frame).
static inline TLimitKoopa* LimitKoopaHeadOwner(TLimitKoopaHead* head)
{
	TLimitKoopa* koopa = head->mOwner;
	return koopa;
}


BOOL TLimitKoopaHead::receiveMessage(THitActor*, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		LimitKoopaHeadOwner(this)->getShowered();
		break;
	}
	return TRUE;
}

void TLimitKoopaHead::attack_(THitActor* other)
{
	other->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

TLimitKoopaBody::TLimitKoopaBody(TLimitKoopa* owner)
    : TLimitKoopaParts("クッパの体", 0x08000033, owner, 100.0f)
{
}

BOOL TLimitKoopaBody::receiveMessage(THitActor*, u32) { return TRUE; }

void TLimitKoopaBody::attack_(THitActor* other)
{
	other->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

namespace {
// TODO: the ROM's body is a single `blr`; whatever the neck callback used
// to do was optimised away (Koopa.cpp still has a 0x9d4-byte version).
int KoopaNeckCallBack(J3DNode*, int) { return 1; }
} // namespace
// ---------------------------------------------------------------------------
// TLimitKoopa
// ---------------------------------------------------------------------------

TLimitKoopa::TLimitKoopa(const char* name)
    : TSpineEnemy(name)
    , mBodyDirection()
{
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	offLiveFlag(LIVE_FLAG_UNK100);
	onLiveFlag(LIVE_FLAG_UNK10);
	mScaledBodyRadius = 1600.0f;
}

void TLimitKoopa::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);
}

// TODO: 99.8%, instruction-exact; the frame is 0x168 against retail's 0x188.
// Levers inside TLimitKoopaParts' constructor (a named group pointer) or a
// factory level around each `new` push the constructors out of line; a named
// `new` result for the head/body pair is +8 only.
void TLimitKoopa::loadAfter()
{
	JDrama::TNameRef::loadAfter();

	for (int i = 0; i < 10; i++)
		mFlames[i] = new TLimitKoopaFlame(this);

	for (int i = 0; i < 2; i++)
		mHands[i] = new TLimitKoopaHand(this);

	mHead = new TLimitKoopaHead(this);
	mBody = new TLimitKoopaBody(this);
}

// TODO: 91.8%.
static inline MActorAnmBck* LimitKoopaAnmBck(const TLimitKoopa* p)
{
	MActorAnmBck* bck = p->getMActor()->getAnmBck();
	return bck;
}

void TLimitKoopa::init(TLiveManager* manager)
{
	mBodyRadius = 800.0f;
	mHeadHeight = 2000.0f;
	TSpineEnemy::init(manager);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	onHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	offHitFlag(HIT_FLAG_CANNOT_ATTACK);
	mSpine->initWith(&TNerveLimitKoopaWait::theNerve());

	// TODO: every instruction of init() now matches; the frame is 0x68
	// against retail's 0x70 and nothing in the body touches the stack, so
	// the residue is a purely dead low region 8 bytes short -- an 8-byte
	// non-trivial local of one of the inlined callees here (theNerve(),
	// TSpineBase::initWith, the getAnmBck test). The named
	// `MActorAnmBck* bck` local this replaced cost one `mr r3, r0` and, as
	// a named local, *shrank* the frame by another 8 (0x60).
	if (LimitKoopaAnmBck(this))
		getMActor()->getAnmBck()->initSimpleMotionBlend(0x10);

	unk170 = 0.0f;
	reset();

	JUTNameTab* joints = getModel()->getModelData()->getJointName();
	// The ROM walks every joint name here and does nothing with them.
	for (u16 i = 0; i < joints->getNameNum(); i++) { }

	mAgoJntIndex  = joints->getIndex("ago");
	mHeadJntIndex = joints->getIndex("head");
	mNeckJntIndex = joints->getIndex("neck");

	J3DModelData* modelData = getModel()->getModelData();
	J3DJoint* neck = modelData->getJointNodePointer(mHeadJntIndex);
	neck->setCallBack(&KoopaNeckCallBack);
	neck->setCallBackUserData(this);
}

static inline f32 LimitKoopaWaitSpeed(const TLimitKoopa* p)
{
	TLimitKoopaParams* param = p->getParam();
	f32 speed                = param->waitSpeed.get();
	return speed;
}

void TLimitKoopa::reset()
{
	TSpineEnemy::reset();
	changeBck(KOOPA_ANM_WAIT, LimitKoopaWaitSpeed(this));
	mSpine->reset();
	mTimers[LIMITKOOPA_TIMER_WAIT]     = 0;
	mTimers[LIMITKOOPA_TIMER_HIPDROP]  = 0;
	mTimers[LIMITKOOPA_TIMER_FLAME]    = 0;
	mLanded        = true;
	mBodyDirection.mDirection = 0.0f;
}

// TODO: UNUSED (0x2c), body not reconstructed.
void TLimitKoopa::resetLimitKoopa() { }

// TODO: 99.9%, instruction-exact; the frame is 0x118 against retail's 0x170
// (everything below breathFlame's scale vector, 0xf4 here and 0x144 there).
// The restored isBreathing/breathFlame/updateTimers bodies are at their map
// sizes. Measured and not taken (fabricated binders only): a named MtxPtr in
// LimitKoopaEmitFlame +0x10, a named model binder for the head matrix at the
// four puffs +0x20 and at the first +0x10, a bool binder over isBreathing +8
// (best 0x148); a by-value scale parameter breaks the code.
void TLimitKoopa::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSpineEnemy::perform(cue, graphics);

	for (int i = 0; i < 10; i++)
		mFlames[i]->perform(cue, graphics);
	mHead->perform(cue, graphics);
	mHands[0]->perform(cue, graphics);
	mHands[1]->perform(cue, graphics);
	mBody->perform(cue, graphics);

	if (cue & CUE_MOVE) {
		setUpHitActors();
		updateTimers();
	}

	if (cue & CUE_CALC_ANIM) {
		if (isBreathing())
			breathFlame();
	}
}

// TODO: 74.7%. The ROM calls JGeometry::TVec3<f32>::sub(a, b) out of line for
// the last step and our build expands it either spelling, which also makes
// this a leaf where the ROM saves LR. Same per-call-site TVec3 inlining
// divergence as in killer.cpp's TFlyEnemy::fly.
void TLimitKoopa::bind()
{
	JGeometry::TVec3<f32> nextPos(mPosition);
	nextPos.add(mLinearVelocity);
	nextPos.add(mVelocity);
	mVelocity.add(mAcceleration);

	mGroundHeight = 3500.0f;
	mGroundHeight += 1.0f;
	if (nextPos.y <= 0.05f + mGroundHeight) {
		nextPos.y = mGroundHeight;
		mLanded   = true;
		mAcceleration.set(0.0f, 0.0f, 0.0f);
		mVelocity.set(0.0f, 0.0f, 0.0f);
	}

	// `a = b - c` reaches the map's out-of-line TVec3::sub: operator= is one
	// inline level and the difference nested in its argument two more.
	mLinearVelocity = nextPos - mPosition;
}

// TODO: UNUSED (0x20), body not reconstructed.
void TLimitKoopa::moveStop() { }

// TODO: 91.6%, frame 0xb0 vs 0xb8. Retail calls this from
// TNerveLimitKoopaHipDropStart::execute, so the body has 15+ statements: the
// goal is a `diff` plus `goal.add(pos, diff)` pair (retail reloads pos.y/.z
// after each goal store), and the jump's launch speed and the length are
// named. Raw mPosition in both is what orders dy after the z loads. Left:
// retail's normalize() reuses the squared length the `len` test computed (sq
// stays in f1, `fmr f2, f1` in the zero branch) where we recompute it; inert:
// sqrt(squared()), sqrt(dot()), setLength(1.0f), setLength(200.0f),
// normalize(velocity), setLength(v, 1.0f), a named gravity, a named sq. The
// low region is 8 short; lever-search reaches it only with a fabricated
// ground-height binder.
void TLimitKoopa::startHipDrop()
{
	// One local carries the jump: first the straight-up launch speed, then the
	// velocity the jump solver returns. Reusing it is what keeps the vector in
	// memory instead of being promoted into registers.
	JGeometry::TVec3<f32> velocity(0.0f, 1.0f, 0.0f);
	velocity.scale(getParam()->hipDropInitialSpeedY.get());

	JGeometry::TVec3<f32> target(SMS_GetMarioPos());
	target.y = getGroundHeight();

	JGeometry::TVec3<f32> diff;
	diff.sub(target, mPosition);
	JGeometry::TVec3<f32> goal;
	goal.add(mPosition, diff);

	f32 speedY = velocity.y;
	velocity = calcVelocityToJumpToY(goal, speedY,
	                                 getParam()->hipDropGravityY.get());
	f32 len = velocity.length();
	if (len > 200.0f) {
		velocity.normalize();
		velocity.scale(200.0f);
	}

	mVelocity.set(velocity);
}

// TODO: UNUSED (0x4c), body not reconstructed.
void TLimitKoopa::moveHipDrop() { }

// Rotation-speed read for moveTurn: two levels (the named params binder under
// this reader) put TEnemyManager::getSaveParam() at depth 5 in the Wait nerve,
// where retail calls it, and land the nerve's frame.
static inline f32 LimitKoopaRotationSpeed(const TLimitKoopa* koopa)
{
	return LimitKoopaGetParam(koopa)->rotationSpeed.get();
}


// UNUSED (0xc0).
void TLimitKoopa::moveTurn()
{
	TDirectionCalc target = calcTargetDirection();
	mBodyDirection.mDirection = mBodyDirection.calcTurnDirection(
	    target.get(),
	    TDirectionCalc::d2r(LimitKoopaRotationSpeed(this)));
}

// UNUSED (0xb4).
bool TLimitKoopa::finishedTurn()
{
	TDirectionCalc facing;
	JGeometry::TVec3<f32> toMario;
	toMario.x = SMS_GetMarioPos().x - getPosition().x;
	toMario.y = SMS_GetMarioPos().y - getPosition().y;
	toMario.z = SMS_GetMarioPos().z - getPosition().z;
	toMario.y = 0.0f;
	facing.makeDirection(toMario);
	if (mBodyDirection.absDirection(facing.get()) > 0.31415927f)
		return false;
	return true;
}

// UNUSED (0x94).
TDirectionCalc TLimitKoopa::calcTargetDirection()
{
	TDirectionCalc calc;
	JGeometry::TVec3<f32> delta;
	delta.sub(SMS_GetMarioPos(), mPosition);
	delta.y = 0.0f;
	calc.makeDirection(delta);
	return calc;
}

// TODO: UNUSED (0x5c), body not reconstructed.
f32 TLimitKoopa::makeDirection(f32) { return 0.0f; }

f32 TLimitKoopa::getGravityY() const
{
	return getParam()->hipDropGravityY.get();
}

BOOL TLimitKoopa::receiveMessage(THitActor* sender, u32 message)
{
	return TSpineEnemy::receiveMessage(sender, message);
}

void TLimitKoopa::calcRootMatrix()
{
	TLimitKoopaParams* params = getParam();
	f32 scale                 = params->bodyScale.get();
	mScaling.x                = scale;
	mScaling.y                = scale;
	mScaling.z                = scale;
	// TODO: the ROM leaves the parameter pointer in r3 across this call, so
	// r2d never receives a `this`. That reads as a static member in the
	// original; TDirectionCalc lives in the shared KoopaJr.hpp, so the
	// declaration is left alone here.
	mRotation.y
	    = TDirectionCalc::r2d(mBodyDirection.get());
	TSpineEnemy::calcRootMatrix();
}

// Fire-speed read: one level over getParam(). As an argument of the inlined
// changeBck() it sits one level deeper than moveTurn's reader, so it needs
// one binder fewer to put TEnemyManager::getSaveParam() at depth 5.
static inline f32 LimitKoopaFireSpeed(const TLimitKoopa* koopa)
{
	TLimitKoopaParams* params = koopa->getParam();
	return params->fireSpeed.get();
}

// UNUSED (0x78).
void TLimitKoopa::stopFlame()
{
	changeBck(KOOPA_ANM_FIRE_END, LimitKoopaFireSpeed(this));
}

// One flame puff after the first: the extra level is what puts
// TVec3::set<f>(const TVec3&) inside setGlobalScale() at depth 4, where retail
// calls it for these four emitters but not for the first.
static inline void LimitKoopaEmitFlame(TLimitKoopa* koopa, s32 id,
                                       const JGeometry::TVec3<f32>& scale)
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    id, koopa->getHeadMtx(), 1, koopa);
	if (emitter)
		emitter->setGlobalScale(scale);
}

// UNUSED (0x1b8).
void TLimitKoopa::breathFlame()
{
	f32 s = getParam()->flameScale.get();
	JGeometry::TVec3<f32> scale(s, s, s);

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    KOOPA_JPA_MS_KP_FIRE_E, getHeadMtx(), 3, this);
	if (emitter)
		emitter->setGlobalScale(scale);

	LimitKoopaEmitFlame(this, KOOPA_JPA_MS_KP_FIRE_D, scale);
	LimitKoopaEmitFlame(this, KOOPA_JPA_MS_KP_FIRE_C, scale);
	LimitKoopaEmitFlame(this, KOOPA_JPA_MS_KP_FIRE_B, scale);
	LimitKoopaEmitFlame(this, KOOPA_JPA_MS_KP_FIRE_A, scale);
}

// TODO: 94.9%. The flame and head boxes go through TLimitKoopaParts::set
// (which defaults a non-positive height to twice the radius; the head passes
// 0) and remove, both at their map sizes. The flame position is the head
// matrix's rows 0 and 2 dotted with an `(along, 0, 0)` offset, built in the
// TVec3 constructor; the row temporaries are what fill retail's 0x1a0 frame.
// Left: the loop's FPR assignment (retail gives the 0.0f constant f31 and
// spread f27; ours f27 and f28) and the head's x/z load order. Inert: an else
// arm for spread, along folded into the offset, a zero-initialised offset,
// headPos through set() or a temporary.
void TLimitKoopa::setUpHitActors()
{
	MtxPtr headMtx = getMActor()->getModel()->getAnmMtx(mHeadJntIndex);

	if (isBreathing()) {
		f32 spread = 1.0f;
		if (getAnmIndex() == KOOPA_ANM_FIRE_START) {
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			spread = (ctrl->getFrame() - 125.0f)
			         / ((f32)ctrl->getEnd() - 125.0f);
		}

		for (int i = 0; i < 10; i++) {
			TLimitKoopaParams* params = getParam();
			f32 height                = params->flameHeight.get();
			f32 radius                = params->flameRadius.get();
			f32 along = 0.8f * ((2.0f + (f32)(i * 2)) * radius) * spread;
			JGeometry::TVec3<f32> offset(along, 0.0f, 0.0f);
			JGeometry::TVec3<f32> pos(
			    headMtx[0][3]
			        + offset.dot(JGeometry::TVec3<f32>(
			            headMtx[0][0], headMtx[0][1], headMtx[0][2])),
			    mPosition.y,
			    headMtx[2][3]
			        + offset.dot(JGeometry::TVec3<f32>(
			            headMtx[2][0], headMtx[2][1], headMtx[2][2])));
			mFlames[i]->set(pos, radius, height);
		}
	} else {
		for (int i = 0; i < 10; i++)
			mFlames[i]->remove();
	}

	MtxPtr agoMtx  = getMActor()->getModel()->getAnmMtx(mAgoJntIndex);
	JGeometry::TVec3<f32> headPos(agoMtx[0][3], agoMtx[1][3], agoMtx[2][3]);
	headPos.y -= 200.0f;
	f32 headRadius = getParam()->headRadius.get();
	mHead->set(headPos, headRadius, 0.0f);
}

// TODO: UNUSED (0x70), body not reconstructed.
void TLimitKoopa::setAnimationIndex(int) { }

// UNUSED (0x78). Every nerve in this unit selects its animation through this,
// which is why the pattern below repeats in each execute().
void TLimitKoopa::changeBck(int index, f32 rate)
{
	MActor* actor = mMActor;
	if (!actor->checkCurBckFromIndex(index))
		actor->setBckFromIndex(index);
	mMActor->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
}

// UNUSED (0x28).
int TLimitKoopa::getAnmIndex() const
{
	return getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
}

// TODO: UNUSED (0x3c), body not reconstructed.
BOOL TLimitKoopa::endsAnm() const { return FALSE; }

// UNUSED (0x84).
bool TLimitKoopa::isBreathing() const
{
	if (getAnmIndex() == KOOPA_ANM_FIRE_LOOP)
		return true;
	if (getAnmIndex() == KOOPA_ANM_FIRE_START && getAnmFrame() >= 127.0f)
		return true;
	return false;
}

// TODO: UNUSED (0x180), body not reconstructed.
JGeometry::TVec3<f32> TLimitKoopa::getFlameDir() const
{
	return JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
}

// UNUSED (0x48). Same body as TKoopa::isFlaming, which the ROM keeps.
BOOL TLimitKoopa::isFlaming() const
{
	switch (getMActor()->getCurAnmIdx(ANM_TYPE_BCK)) {
	case KOOPA_ANM_FIRE_END:
	case KOOPA_ANM_FIRE_LOOP:
	case KOOPA_ANM_FIRE_START:
		return true;
	}
	return false;
}

// TODO: UNUSED (0xe8), body not reconstructed. TKoopa::getNeckFocus() returns
// f32 (the yaw the neck callback aims at) and this is the same boss on the same
// model, so this one does too.
f32 TLimitKoopa::getNeckFocus() const { return 0.0f; }

// TODO: UNUSED (0x2a0), body not reconstructed.
void TLimitKoopa::getDown() { }

// The push is one level below getShowered(): retail calls pushNerve out of
// line while the setNext beside it is expanded.
static inline void LimitKoopaPushShowered(TLimitKoopa* koopa)
{
	koopa->mSpine->pushNerve(&TNerveLimitKoopaGetShowered::theNerve());
}


// Spine binder at the setNext site (+8 of the head's receiveMessage frame).
static inline TSpineBase<TLiveActor>* LimitKoopaSpine(TLimitKoopa* koopa)
{
	TSpineBase<TLiveActor>* spine = koopa->mSpine;
	return spine;
}


// UNUSED (0x23c).
void TLimitKoopa::getShowered()
{
	if (&TNerveLimitKoopaTumble::theNerve() == mSpine->getCurrentNerve()
	    || &TNerveLimitKoopaGetDown::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveLimitKoopaStagger::theNerve() == mSpine->getCurrentNerve())
		LimitKoopaSpine(this)->setNext(&TNerveLimitKoopaGetShowered::theNerve());
	LimitKoopaPushShowered(this);
}

// TODO: UNUSED (0x240), body not reconstructed.
void TLimitKoopa::stagger(bool) { }

// UNUSED (0x8).
BOOL TLimitKoopa::isTumbling() const { return FALSE; }

// UNUSED (0x2c).
f32 TLimitKoopa::getAnmFrame() const
{
	return getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
}

// UNUSED (0x90): TKoopa::fall (0x98) without its yaw reset.
void TLimitKoopa::fall() { mSpine->setNext(&TNerveLimitKoopaFall::theNerve()); }

// UNUSED (0x48).
BOOL TLimitKoopa::getAnmEnd() const
{
	return getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr);
}

// UNUSED (0x1c).
MtxPtr TLimitKoopa::getHeadMtx() const
{
	return getMActor()->getModel()->getAnmMtx(mHeadJntIndex);
}

// TODO: UNUSED (0x68), body not reconstructed.
void TLimitKoopa::checkMarioWhichSide() { }

// UNUSED (0x4c).
void TLimitKoopa::updateTimers()
{
	for (int i = 0; i < 3; i++)
		if (mTimers[i] > 0)
			mTimers[i]--;
}

// UNUSED (0xc).
TLimitKoopaParams* TLimitKoopa::getParam() const
{
	// The named manager is one inline level: without it
	// TEnemyManager::getSaveParam() expands one level deeper at every caller
	// of getParam(), and the three TNerveLimitKoopa*::execute bodies plus
	// TLimitKoopa::calcRootMatrix miss. Spelling it
	// `(TLimitKoopaParams*)getSaveParam()` over TSpineEnemy's accessor
	// instead is one level too many (whole unit 87.90 -> 85.23).
	TEnemyManager* manager = (TEnemyManager*)mManager;
	return (TLimitKoopaParams*)manager->getSaveParam();
}

// ---------------------------------------------------------------------------
// TLimitKoopaManager
// ---------------------------------------------------------------------------

TLimitKoopaManager::TLimitKoopaManager(const char* name)
    : TEnemyManager(name)
{
}

// The Corona Mountain Bowser is placed by the scene, never spawned.
TSpineEnemy* TLimitKoopaManager::createEnemyInstance() { return nullptr; }

void TLimitKoopaManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "koopa_model.bmd", 0x14240000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

void TLimitKoopaManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk38 = new TLimitKoopaParams("/enemy/limitkoopa.prm");
}

void TLimitKoopaManager::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_a.jpa",
	                 KOOPA_JPA_MS_KP_FIRE_A);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_b.jpa",
	                 KOOPA_JPA_MS_KP_FIRE_B);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_c.jpa",
	                 KOOPA_JPA_MS_KP_FIRE_C);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_d.jpa",
	                 KOOPA_JPA_MS_KP_FIRE_D);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_e.jpa",
	                 KOOPA_JPA_MS_KP_FIRE_E);
}
