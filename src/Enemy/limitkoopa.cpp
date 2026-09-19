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

BOOL TNerveLimitKoopaHipDropStart::execute(TSpineBase<TLiveActor>* spine) const
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();

	if (spine->getTime() == 0) {
		koopa->changeBck(KOOPA_ANM_FIRE_START, 2.0f);
		koopa->mHipDropTimer = 30;
	}

	if (koopa->mHipDropTimer <= 0) {
		koopa->startHipDrop();
		koopa->mLanded = false;
		spine->pushAfterCurrent(&TNerveLimitKoopaHipDropJump::theNerve());
		return TRUE;
	}

	return FALSE;
}

// TODO: 71.9%. Both TDirectionCalc locals and the two makeDirection calls are
// in place; the residual is the d2r/r2d `this` setup noted in calcRootMatrix
// plus the 0x30-byte frame gap it drags along.
BOOL TNerveLimitKoopaWait::execute(TSpineBase<TLiveActor>* spine) const
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();

	if (spine->getTime() == 0) {
		koopa->changeBck(KOOPA_ANM_TURN_L, 2.0f);
		koopa->mWaitTimer = 240;
	}

	TDirectionCalc toMario;
	JGeometry::TVec3<f32> delta;
	delta.sub(SMS_GetMarioPos(), koopa->getPosition());
	delta.y = 0.0f;
	toMario.makeDirection(delta);

	TLimitKoopaParams* params
	    = (TLimitKoopaParams*)((TEnemyManager*)koopa->mManager)
	          ->getSaveParam();
	// TODO: the ROM calls TDirectionCalc::d2r and ::r2d without setting a
	// `this`, so they are static members in the original. KoopaJr.hpp
	// declares them non-static and is shared with koopajr.cpp, so the extra
	// address setup stays until that header may be changed.
	koopa->mBodyDirection = koopa->mBodyDirection.calcTurnDirection(
	    toMario.get(),
	    TDirectionCalc::d2r(params->rotationSpeed.get()));

	TDirectionCalc facing;
	JGeometry::TVec3<f32> toMario2(
	    SMS_GetMarioPos().x - koopa->getPosition().x,
	    SMS_GetMarioPos().y - koopa->getPosition().y,
	    SMS_GetMarioPos().z - koopa->getPosition().z);
	toMario2.y = 0.0f;
	facing.makeDirection(toMario2);

	if (koopa->mBodyDirection.absDirection(facing.get()) <= 0.31415927f
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

BOOL TNerveLimitKoopaGetDown::execute(TSpineBase<TLiveActor>* spine) const
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();

	switch (koopa->getAnmIndex()) {
	case KOOPA_ANM_DOWN:
		if (koopa->getAnmEnd())
			koopa->changeBck(KOOPA_ANM_DOWN_WAIT,
			                 koopa->getParam()->getDownSpeed());
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
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
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

// TODO: UNUSED (0x84), body not reconstructed.
void TLimitKoopaParts::set(const JGeometry::TVec3<f32>&, f32, f32) { }

// TODO: UNUSED (0x28), body not reconstructed.
void TLimitKoopaParts::remove() { }

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
		TLimitKoopa* koopa = mOwner;
		koopa->changeBck(KOOPA_ANM_FIRE_END,
		                 koopa->getParam()->fireSpeed.get());
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

// TODO: 73.8%. The three nerve comparisons and the two pushes are in the
// ROM's order, but it compares the message signed and keeps one more of the
// theNerve() guards expanded than our build does.
BOOL TLimitKoopaHead::receiveMessage(THitActor*, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		TLimitKoopa* koopa = mOwner;
		if (koopa->mSpine->getCurrentNerve()
		        != &TNerveLimitKoopaTumble::theNerve()
		    && koopa->mSpine->getCurrentNerve()
		           != &TNerveLimitKoopaGetDown::theNerve()) {
			if (koopa->mSpine->getCurrentNerve()
			    == &TNerveLimitKoopaStagger::theNerve())
				koopa->mSpine->setNext(
				    &TNerveLimitKoopaGetShowered::theNerve());
			koopa->mSpine->pushNerve(
			    &TNerveLimitKoopaGetShowered::theNerve());
		}
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

// TODO: 88.5%. The four allocation loops are right; the residual is the
// name-ref group lookup, which the ROM spells out as a searchF on the
// generator root rather than through the search<T> template.
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
	if (getMActor()->getAnmBck())
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

void TLimitKoopa::reset()
{
	TSpineEnemy::reset();
	changeBck(KOOPA_ANM_WAIT, getParam()->waitSpeed.get());
	mSpine->reset();
	mWaitTimer     = 0;
	mHipDropTimer  = 0;
	mFlameTimer    = 0;
	mLanded        = true;
	mBodyDirection.mDirection = 0.0f;
}

// TODO: UNUSED (0x2c), body not reconstructed.
void TLimitKoopa::resetLimitKoopa() { }

// TODO: 78.4%. The five particle emitters and the timer block are in place;
// the ROM reaches TVec3::set<f32>(const&) out of line for four of the five
// setGlobalScale calls and expands it for the first, which our build does not
// reproduce (the same per-call-site divergence as bind()).
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
		if (getAnmIndex() == KOOPA_ANM_FIRE_LOOP
		    || (getAnmIndex() == KOOPA_ANM_FIRE_START
		        && getAnmFrame() >= 127.0f)) {
			f32 scale = getParam()->flameScale.get();
			JGeometry::TVec3<f32> flameScale(scale, scale, scale);

			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToMtxPtr(
			        KOOPA_JPA_MS_KP_FIRE_E, getHeadMtx(), 3, this);
			if (emitter)
				emitter->setGlobalScale(flameScale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    KOOPA_JPA_MS_KP_FIRE_D, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(flameScale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    KOOPA_JPA_MS_KP_FIRE_C, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(flameScale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    KOOPA_JPA_MS_KP_FIRE_B, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(flameScale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    KOOPA_JPA_MS_KP_FIRE_A, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(flameScale);
		}
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

// TODO: 86.7%, and the ROM calls this from
// TNerveLimitKoopaHipDropStart::execute while MWCC still inlines it here,
// which is why that nerve scores 0%. The lever *is* the depth-1 statement
// budget after all: this body is 10 statements, a plain method is inlined at
// depth 1 up to 14, and exactly five extra zero-codegen statements here take
// TNerveLimitKoopaHipDropStart::execute from 0.0% to 99.9% (four do not) while
// costing this body nothing. So retail's startHipDrop is a 15-statement body
// and five statements of it are missing from this reconstruction. Do not pad
// -- find them. Not them, all measured: naming the three goal deltas
// (`f32 dx = target.x - getPosition().x;` ...) neither counts nor reproduces
// retail's schedule (-0.3); building `goal` as three assignments instead of
// the three-argument constructor costs 2.0 and 8 bytes of frame; a named
// `f32 speed = velocity.length();` before the test costs 0.3; and per
// docs/catalog/codegen-tells.md "Closure 241" a named local feeding the next
// statement's call does not count either.
//
// Two codegen residues are left to read: retail re-loads getPosition().y and
// .z for the additions after computing all three differences (we keep both in
// FPRs), and retail's inlined normalize() re-uses the squared length that the
// length() test just computed where we recompute x*x + y*y + z*z.
void TLimitKoopa::startHipDrop()
{
	// One local carries the jump: first the straight-up launch speed, then the
	// velocity the jump solver returns. Reusing it is what keeps the vector in
	// memory instead of being promoted into registers.
	JGeometry::TVec3<f32> velocity(0.0f, 1.0f, 0.0f);
	velocity.scale(getParam()->hipDropInitialSpeedY.get());

	JGeometry::TVec3<f32> target(SMS_GetMarioPos());
	target.y = mGroundHeight;

	JGeometry::TVec3<f32> goal(getPosition().x + (target.x - getPosition().x),
	                           getPosition().y + (target.y - getPosition().y),
	                           getPosition().z + (target.z - getPosition().z));

	velocity = calcVelocityToJumpToY(goal, velocity.y,
	                                 getParam()->hipDropGravityY.get());
	if (velocity.length() > 200.0f) {
		velocity.normalize();
		velocity.scale(200.0f);
	}

	mVelocity.set(velocity);
}

// TODO: UNUSED (0x4c), body not reconstructed.
void TLimitKoopa::moveHipDrop() { }

// TODO: UNUSED (0xc0), body not reconstructed.
void TLimitKoopa::moveTurn() { }

// TODO: UNUSED (0xb4), body not reconstructed.
BOOL TLimitKoopa::finishedTurn() { return FALSE; }

// TODO: UNUSED (0x94), body not reconstructed.
void TLimitKoopa::calcTargetDirection() { }

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

// TODO: UNUSED (0x78), body not reconstructed.
void TLimitKoopa::stopFlame() { }

// TODO: UNUSED (0x1b8), body not reconstructed.
void TLimitKoopa::breathFlame() { }

// TODO: 69.2%. The flame loop's matrix reads and the "no flames" branch's
// unrolled flag stores are close but the parameter fetch order inside the
// loop is not established yet.
void TLimitKoopa::setUpHitActors()
{
	MtxPtr headMtx = getMActor()->getModel()->getAnmMtx(mHeadJntIndex);

	if (getAnmIndex() == KOOPA_ANM_FIRE_LOOP
	    || (getAnmIndex() == KOOPA_ANM_FIRE_START
	        && getAnmFrame() >= 127.0f)) {
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
			THitActor* flame          = mFlames[i];
			f32 along = 0.8f * ((2.0f + (f32)(i * 2)) * radius) * spread;
			if (height == 0.0f)
				height = 2.0f * radius;

			flame->mPosition.x = headMtx[0][3]
			                     + (0.0f * headMtx[0][2]
			                        + (along * headMtx[0][0]
			                           + 0.0f * headMtx[0][1]));
			flame->mPosition.y = mPosition.y;
			flame->mPosition.z = headMtx[2][3]
			                     + (0.0f * headMtx[2][2]
			                        + (along * headMtx[2][0]
			                           + 0.0f * headMtx[2][1]));
			flame->offHitFlag(HIT_FLAG_CANNOT_ATTACK);
			flame->offHitFlag(HIT_FLAG_CANNOT_GET_HIT);
			flame->offHitFlag(HIT_FLAG_NO_COLLISION);
			flame->mAttackRadius = radius;
			flame->mAttackHeight = height;
			flame->mDamageRadius = radius;
			flame->mDamageHeight = height;
			flame->calcEntryRadius();
		}
	} else {
		for (int i = 0; i < 10; i++) {
			mFlames[i]->onHitFlag(HIT_FLAG_CANNOT_ATTACK);
			mFlames[i]->onHitFlag(HIT_FLAG_CANNOT_GET_HIT);
			mFlames[i]->onHitFlag(HIT_FLAG_NO_COLLISION);
		}
	}

	MtxPtr agoMtx    = getMActor()->getModel()->getAnmMtx(mAgoJntIndex);
	THitActor* head  = mHead;
	f32 headRadius   = getParam()->headRadius.get();
	head->mPosition.x = agoMtx[0][3];
	head->mPosition.y = agoMtx[1][3] - 200.0f;
	head->mPosition.z = agoMtx[2][3];
	head->offHitFlag(HIT_FLAG_CANNOT_ATTACK);
	head->offHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	head->offHitFlag(HIT_FLAG_NO_COLLISION);
	head->mAttackRadius = headRadius;
	head->mAttackHeight = 2.0f * headRadius;
	head->mDamageRadius = headRadius;
	head->mDamageHeight = 2.0f * headRadius;
	head->calcEntryRadius();
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

// TODO: UNUSED (0x84), body not reconstructed.
BOOL TLimitKoopa::isBreathing() const { return FALSE; }

// TODO: UNUSED (0x180), body not reconstructed.
JGeometry::TVec3<f32> TLimitKoopa::getFlameDir() const
{
	return JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
}

// TODO: UNUSED (0x48), body not reconstructed.
BOOL TLimitKoopa::isFlaming() const { return FALSE; }

// TODO: UNUSED (0xe8), body not reconstructed. TKoopa::getNeckFocus() returns
// f32 (the yaw the neck callback aims at) and this is the same boss on the same
// model, so this one does too.
f32 TLimitKoopa::getNeckFocus() const { return 0.0f; }

// TODO: UNUSED (0x2a0), body not reconstructed.
void TLimitKoopa::getDown() { }

// TODO: UNUSED (0x23c), body not reconstructed.
void TLimitKoopa::getShowered() { }

// TODO: UNUSED (0x240), body not reconstructed.
void TLimitKoopa::stagger(bool) { }

// UNUSED (0x8).
BOOL TLimitKoopa::isTumbling() const { return FALSE; }

// UNUSED (0x2c).
f32 TLimitKoopa::getAnmFrame() const
{
	return getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
}

// TODO: UNUSED (0x90), body not reconstructed.
void TLimitKoopa::fall() { }

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
	if (mWaitTimer > 0)
		mWaitTimer--;
	if (mHipDropTimer > 0)
		mHipDropTimer--;
	if (mFlameTimer > 0)
		mFlameTimer--;
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
