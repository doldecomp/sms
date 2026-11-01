#include <Enemy/LimitKoopa.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <macros.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>

// Like Bowser's, these nerves keep their theNerve() inline in the class,
// which leaves weak nerve$localstatic0$theNerve__... objects in the map.
#define DECLARE_LIMIT_KOOPA_NERVE(Name, Base)                                  \
	class Name : public Base {                                                 \
	public:                                                                    \
		virtual BOOL execute(TSpineBase<TLiveActor>*) const;                   \
		static const Name& theNerve()                                          \
		{                                                                      \
			static Name nerve;                                                 \
			return nerve;                                                      \
		}                                                                      \
	};

// Base of the nerves that turn Bowser while they run.
class TNerveLimitKoopaTurn : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const = 0;
};

// The turn nerves only show up in the map as their UNUSED nerve$localstatic0
// objects: like TKoopa's, their execute() is inline in the class, so nothing
// else of them is listed. Nothing linked uses them.
// TODO: their bodies are unknown.
#define DECLARE_LIMIT_KOOPA_TURN_NERVE(Name)                                   \
	class Name : public TNerveLimitKoopaTurn {                                 \
	public:                                                                    \
		virtual BOOL execute(TSpineBase<TLiveActor>*) const { return false; }  \
		static const Name& theNerve()                                          \
		{                                                                      \
			static Name nerve;                                                 \
			return nerve;                                                      \
		}                                                                      \
	};

// The destructor chain objects of the nerve statics are numbered in the
// order these are declared, and the linked code pins down the order of all
// but Fall, Flame and the turn nerves.
// TODO: the order of those four among themselves is a guess.
DECLARE_LIMIT_KOOPA_NERVE(TNerveLimitKoopaHipDropJump, TNerveBase<TLiveActor>);
DECLARE_LIMIT_KOOPA_NERVE(TNerveLimitKoopaHipDropStart, TNerveBase<TLiveActor>);
DECLARE_LIMIT_KOOPA_NERVE(TNerveLimitKoopaFall, TNerveBase<TLiveActor>);
DECLARE_LIMIT_KOOPA_NERVE(TNerveLimitKoopaFlame, TNerveLimitKoopaTurn);
DECLARE_LIMIT_KOOPA_NERVE(TNerveLimitKoopaTumble, TNerveLimitKoopaTurn);
DECLARE_LIMIT_KOOPA_NERVE(TNerveLimitKoopaWait, TNerveLimitKoopaTurn);
DECLARE_LIMIT_KOOPA_TURN_NERVE(TNerveLimitKoopaTurnR);
DECLARE_LIMIT_KOOPA_NERVE(TNerveLimitKoopaGetDown, TNerveBase<TLiveActor>);
DECLARE_LIMIT_KOOPA_NERVE(TNerveLimitKoopaStagger, TNerveBase<TLiveActor>);
DECLARE_LIMIT_KOOPA_TURN_NERVE(TNerveLimitKoopaTurnL);
DECLARE_LIMIT_KOOPA_NERVE(TNerveLimitKoopaGetShowered, TNerveBase<TLiveActor>);

DEFINE_NERVE_EXECUTE(TNerveLimitKoopaHipDropJump, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	koopa->moveHipDrop();
	if (koopa->mOnGround) {
		spine->pushAfterCurrent(&TNerveLimitKoopaWait::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveLimitKoopaHipDropStart, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	if (spine->getTime() == 0) {
		koopa->changeBck(KOOPA_ANM_FIRE_START, 2.0f);
		koopa->mTimers[1] = 30;
	}

	if (koopa->mTimers[1] <= 0) {
		koopa->startHipDrop();
		koopa->mOnGround = false;
		spine->pushAfterCurrent(&TNerveLimitKoopaHipDropJump::theNerve());
		return true;
	}
	return false;
}

// TODO: the target reads rotationSpeed through an out-of-line
// TEnemyManager::getSaveParam here (inlining runs out one level earlier
// than in this version), and its frame is 8 bytes bigger.
DEFINE_NERVE_EXECUTE(TNerveLimitKoopaWait, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	if (spine->getTime() == 0) {
		koopa->changeBck(KOOPA_ANM_TURN_L, 2.0f);
		koopa->mTimers[0] = 240;
	}

	koopa->moveTurn();
	if (koopa->finishedTurn()
	    && koopa->getMActor()->isCurAnmAlreadyEnd(ANM_TYPE_BCK)) {
		spine->pushAfterCurrent(&TNerveLimitKoopaHipDropStart::theNerve());
		return true;
	}
	return false;
}

// TODO: frame-only, here and in the stagger, showered and get-down nerves:
// the target's frames are bigger, as in TKoopa's nerves.
DEFINE_NERVE_EXECUTE(TNerveLimitKoopaTumble, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	koopa->changeBck(KOOPA_ANM_HIPDROP, koopa->getParam()->tumbleSpeed.get());
	if (koopa->endsAnm())
		return true;
	return false;
}

// TODO: UNUSED, 0x6C bytes, and nothing linked shows its body. The size fits
// a single changeBck(KOOPA_ANM_FALL, ...) like TKoopa's fall nerve, but this
// Bowser has no fallSpeed parameter to say what speed.
DEFINE_NERVE_EXECUTE(TNerveLimitKoopaFall, TLiveActor) { return false; }

// TODO: UNUSED, 0x170 bytes, and nothing linked shows its body. This is a
// placeholder: the map has the turn nerves' objects, so some UNUSED code
// used them, and TKoopa's flame nerve is where TKoopa uses its turn nerves.
DEFINE_NERVE_EXECUTE(TNerveLimitKoopaFlame, TLiveActor)
{
	spine->pushNerve(&TNerveLimitKoopaTurnL::theNerve());
	spine->pushNerve(&TNerveLimitKoopaTurnR::theNerve());
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveLimitKoopaStagger, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	koopa->changeBck(KOOPA_ANM_STAGGER, koopa->getParam()->staggerSpeed.get());
	if (koopa->endsAnm())
		return true;
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveLimitKoopaGetShowered, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	koopa->changeBck(KOOPA_ANM_WATERHIT,
	                 koopa->getParam()->waterhitSpeed.get());
	if (koopa->endsAnm())
		return true;
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveLimitKoopaGetDown, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();

	switch (koopa->getAnmIndex()) {
	case KOOPA_ANM_DOWN:
		if (koopa->endsAnm())
			koopa->changeBck(KOOPA_ANM_DOWN_LOOP,
			                 koopa->getParam()->downSpeed.get());
		break;

	case KOOPA_ANM_DOWN_LOOP:
		if (koopa->endsAnm())
			koopa->changeBck(KOOPA_ANM_GETUP,
			                 koopa->getParam()->downSpeed.get());
		break;

	case KOOPA_ANM_GETUP:
		if (koopa->endsAnm())
			return true;
		break;

	default:
		koopa->changeBck(KOOPA_ANM_DOWN, koopa->getParam()->downSpeed.get());
		break;
	}

	return false;
}

TLimitKoopaParts::TLimitKoopaParts(const char* name, u32 actor_type,
                                   TLimitKoopa* koopa, f32 radius)
    : TLiveActor(name)
    , mKoopa(koopa)
{
	// Spelled out rather than through TNameRefGen::search: the target inlines
	// one level deeper here than that allows.
	static_cast<JDrama::TViewObjPtrListT<THitActor>*>(
	    JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(
	        "敵グループ"))
	    ->getChildren()
	    .push_back(this);
	initHitActor(actor_type, 5, 0x80000000, radius, radius, radius, radius);
	onHitFilter(HIT_FILTER_NO_ATTACK);
	onHitFilter(HIT_FILTER_NO_DAMAGE);
	onHitFilter(HIT_FILTER_NO_COLLISION);
}

void TLimitKoopaParts::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TLiveActor::perform(cue, graphics);
	if (cue & 1) {
		for (int i = 0; i < mColCount; i++)
			attack_(mCollisions[i]);
	}
}

// TODO: UNUSED, 0x84 bytes in the map but 0x6C here.
void TLimitKoopaParts::set(const JGeometry::TVec3<f32>& pos, f32 radius,
                           f32 height)
{
	mPosition.set(pos);
	offHitFilter(HIT_FILTER_NO_ATTACK);
	offHitFilter(HIT_FILTER_NO_DAMAGE);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	mAttackRadius = radius;
	mAttackHeight = height;
	mDamageRadius = radius;
	mDamageHeight = height;
	calcEntryRadius();
}

void TLimitKoopaParts::remove()
{
	onHitFilter(HIT_FILTER_NO_ATTACK);
	onHitFilter(HIT_FILTER_NO_DAMAGE);
	onHitFilter(HIT_FILTER_NO_COLLISION);
}

TLimitKoopaFlame::TLimitKoopaFlame(TLimitKoopa* koopa)
    : TLimitKoopaParts("クッパの吐く炎", ACTOR_TYPE_LIMIT_KOOPA_FLAME, koopa,
                       100.0f)
{
}

BOOL TLimitKoopaFlame::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		return false;
	}
	return true;
}

// TODO: nonmatching, the target reads fireSpeed through an out-of-line
// TEnemyManager::getSaveParam.
void TLimitKoopaFlame::attack_(THitActor* actor)
{
	if (actor->receiveMessage(this, HIT_MESSAGE_BURN))
		mKoopa->stopFlame();
}

TLimitKoopaHand::TLimitKoopaHand(TLimitKoopa* koopa)
    : TLimitKoopaParts("クッパの手", ACTOR_TYPE_LIMIT_KOOPA_HAND, koopa, 100.0f)
{
}

BOOL TLimitKoopaHand::receiveMessage(THitActor* sender, u32 message)
{
	return true;
}

void TLimitKoopaHand::attack_(THitActor* actor)
{
	actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

TLimitKoopaHead::TLimitKoopaHead(TLimitKoopa* koopa)
    : TLimitKoopaParts("クッパの頭", ACTOR_TYPE_LIMIT_KOOPA_HEAD, koopa, 100.0f)
{
}

// TODO: nonmatching, the target calls TSpineBase::pushNerve out of line.
BOOL TLimitKoopaHead::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		mKoopa->getShowered();
		break;
	}
	return true;
}

void TLimitKoopaHead::attack_(THitActor* actor)
{
	actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

TLimitKoopaBody::TLimitKoopaBody(TLimitKoopa* koopa)
    : TLimitKoopaParts("クッパの体", ACTOR_TYPE_LIMIT_KOOPA_BODY, koopa, 100.0f)
{
}

BOOL TLimitKoopaBody::receiveMessage(THitActor* sender, u32 message)
{
	return true;
}

void TLimitKoopaBody::attack_(THitActor* actor)
{
	actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

namespace {

// Unlike Bowser's, this neck callback does nothing: the neck doesn't follow
// Mario.
BOOL KoopaNeckCallBack(J3DNode* node, int calc) { return TRUE; }

} // namespace

TLimitKoopa::TLimitKoopa(const char* name)
    : TSpineEnemy(name)
{
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
	onLiveFlag(LIVE_FLAG_UNK10);
	mScaledBodyRadius = 1600.0f;
}

void TLimitKoopa::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);
}

void TLimitKoopa::loadAfter()
{
	JDrama::TNameRef::loadAfter();

	for (int i = 0; i < ARRAY_COUNT(mFlames); i++)
		mFlames[i] = new TLimitKoopaFlame(this);
	for (int i = 0; i < ARRAY_COUNT(mHands); i++)
		mHands[i] = new TLimitKoopaHand(this);
	mHead = new TLimitKoopaHead(this);
	mBody = new TLimitKoopaBody(this);
}

// TODO: nonmatching, the head joint index is truncated to u16 later in the
// target (clrlslwi at the use), like in TKoopa::init.
void TLimitKoopa::init(TLiveManager* manager)
{
	mBodyRadius = 800.0f;
	mHeadHeight = 2000.0f;
	TSpineEnemy::init(manager);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	onHitFilter(HIT_FILTER_NO_DAMAGE);
	offHitFilter(HIT_FILTER_NO_ATTACK);
	mSpine->initWith(&TNerveLimitKoopaWait::theNerve());
	getMActor()->initBckSimpleMotionBlend(16);
	unk170 = 0.0f;
	reset();

	JUTNameTab* names = getModel()->getModelData()->getJointName();
	for (u16 i = 0; i < names->getNameNum(); i++)
		;
	mJawJoint  = names->getIndex("ago");
	mHeadJoint = names->getIndex("head");
	mNeckJoint = names->getIndex("neck");

	J3DJoint* head
	    = getModel()->getModelData()->getJointNodePointer(mHeadJoint);
	head->setCallBack(KoopaNeckCallBack);
	head->setCallBackUserData(this);
}

void TLimitKoopa::reset()
{
	TSpineEnemy::reset();
	changeBck(KOOPA_ANM_WAIT, getParam()->waitSpeed.get());
	resetLimitKoopa();
}

void TLimitKoopa::resetLimitKoopa()
{
	mSpine->reset();
	mTimers[0]      = 0;
	mTimers[1]      = 0;
	mTimers[2]      = 0;
	mOnGround       = true;
	mDirection.unk0 = 0.0f;
}

// TODO: nonmatching, the target keeps the scale on the stack and calls
// TVec3::set out of line for all but the first emitter, which is why the
// map has a local set<f> in this unit. Inlining runs out earlier there.
void TLimitKoopa::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSpineEnemy::perform(cue, graphics);

	for (int i = 0; i < ARRAY_COUNT(mFlames); i++)
		mFlames[i]->perform(cue, graphics);
	mHead->perform(cue, graphics);
	mHands[0]->perform(cue, graphics);
	mHands[1]->perform(cue, graphics);
	mBody->perform(cue, graphics);

	if (cue & 1) {
		setUpHitActors();
		updateTimers();
	}

	if (cue & 2) {
		if (isBreathing()) {
			JGeometry::TVec3<f32> scale(getParam()->flameScale.get());

			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToMtxPtr(
			        SCENE_KOOPA_JPA_MS_KP_FIRE_E, getHeadMtx(), 3, this);
			if (emitter)
				emitter->setGlobalScale(scale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_KOOPA_JPA_MS_KP_FIRE_D, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(scale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_KOOPA_JPA_MS_KP_FIRE_C, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(scale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_KOOPA_JPA_MS_KP_FIRE_B, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(scale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_KOOPA_JPA_MS_KP_FIRE_A, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(scale);
		}
	}
}

// Bowser has no map collision: he falls under mAcceleration until he reaches
// a fixed floor height.
void TLimitKoopa::bind()
{
	JGeometry::TVec3<f32> next = mPosition;
	next += mPositionDelta;
	next += mVelocity;
	mVelocity += mAcceleration;

	mGroundHeight = 3500.0f;
	mGroundHeight += 1.0f;
	if (next.y <= 0.05f + mGroundHeight) {
		next.y    = mGroundHeight;
		mOnGround = true;
		moveStop();
	}

	mPositionDelta = next - mPosition;
}

void TLimitKoopa::moveStop()
{
	mAcceleration.set(0.0f, 0.0f, 0.0f);
	mVelocity.set(0.0f, 0.0f, 0.0f);
}

// Jump towards the spot below Mario.
// The speedY and gravity locals are a guess: the target never inlines this
// function, so its body has at least 15 statements.
// TODO: nonmatching, the target reuses the squared length from length()
// for normalize(), and its frame is 0x20 bigger.
void TLimitKoopa::startHipDrop()
{
	JGeometry::TVec3<f32> velocity(0.0f, 1.0f, 0.0f);
	velocity.scale(getParam()->hipDropInitialSpeedY.get());

	JGeometry::TVec3<f32> target = *gpMarioPos;
	target.y                     = mGroundHeight;
	JGeometry::TVec3<f32> diff;
	diff.sub(target, mPosition);
	JGeometry::TVec3<f32> dest;
	dest.add(mPosition, diff);

	f32 speedY  = velocity.y;
	f32 gravity = getParam()->hipDropGravityY.get();
	velocity    = calcVelocityToJumpToY(dest, speedY, gravity);
	if (velocity.length() > 200.0f) {
		velocity.normalize();
		velocity.scale(200.0f);
	}
	mVelocity.set(velocity);
}

void TLimitKoopa::moveHipDrop()
{
	mAcceleration.set(0.0f, -getGravityY(), 0.0f);
}

void TLimitKoopa::moveTurn() { makeDirection(calcTargetDirection()); }

bool TLimitKoopa::finishedTurn()
{
	if (mDirection.absDirection(calcTargetDirection()) > 0.31415927f)
		return false;
	return true;
}

f32 TLimitKoopa::calcTargetDirection()
{
	TDirectionCalc direction;
	JGeometry::TVec3<f32> offset;
	offset.sub(*gpMarioPos, mPosition);
	offset.y = 0.0f;
	direction.makeDirection(offset);
	return direction.unk0;
}

void TLimitKoopa::makeDirection(f32 dir)
{
	f32 rotationSpeed = TDirectionCalc::d2r(getParam()->rotationSpeed.get());
	mDirection.unk0   = mDirection.calcTurnDirection(dir, rotationSpeed);
}

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
	f32 scale = getParam()->bodyScale.get();
	mScaling.set(scale, scale, scale);
	mRotation.y = TDirectionCalc::r2d(mDirection.unk0);
	TSpineEnemy::calcRootMatrix();
}

void TLimitKoopa::stopFlame()
{
	changeBck(KOOPA_ANM_FIRE_END, getParam()->fireSpeed.get());
}

// TODO: UNUSED, 0x1B8 bytes, and nothing linked shows its body.
void TLimitKoopa::breathFlame() { }

// Line the flames up in front of the head while breathing fire, and put the
// head's hit volume on the jaw.
// TODO: nonmatching, scheduling and register differences in the flame loop
// and a frame 0x28 bigger in the target.
void TLimitKoopa::setUpHitActors()
{
	MtxPtr head = getHeadMtx();
	if (isBreathing()) {
		// the flame grows out of the mouth over the end of the start animation
		f32 rate = 1.0f;
		if (getAnmIndex() == KOOPA_ANM_FIRE_START) {
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			rate = (ctrl->getFrame() - 125.0f) / (ctrl->getEnd() - 125.0f);
		}

		for (int i = 0; i < ARRAY_COUNT(mFlames); i++) {
			f32 height = getParam()->flameHeight.get();
			f32 radius = getParam()->flameRadius.get();
			f32 dist   = 0.8f * ((2.0f + i * 2) * radius) * rate;
			JGeometry::TVec3<f32> offset(dist, 0.0f, 0.0f);
			JGeometry::TVec3<f32> pos;
			((TPosition3f*)head)->mult(offset, pos);
			pos.y = mPosition.y;
			if (height <= 0.0f)
				height = 2.0f * radius;
			mFlames[i]->set(pos, radius, height);
		}
	} else {
		for (int i = 0; i < ARRAY_COUNT(mFlames); i++)
			mFlames[i]->remove();
	}

	MtxPtr jaw = getMActor()->getModel()->getAnmMtx(mJawJoint);
	f32 radius = getParam()->headRadius.get();
	mHead->set(JGeometry::TVec3<f32>(jaw[0][3], jaw[1][3] - 200.0f, jaw[2][3]),
	           radius, 2.0f * radius);
}

// TODO: UNUSED, 0x70 bytes, and nothing linked shows its body.
void TLimitKoopa::setAnimationIndex(int index) { }

void TLimitKoopa::changeBck(int index, f32 rate)
{
	getMActor()->setBck(index);
	getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
}

int TLimitKoopa::getAnmIndex() const
{
	return getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
}

bool TLimitKoopa::endsAnm() const
{
	return getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr);
}

bool TLimitKoopa::isBreathing() const
{
	if (getAnmIndex() == KOOPA_ANM_FIRE_LOOP)
		return true;
	if (getAnmIndex() == KOOPA_ANM_FIRE_START && getAnmFrame() >= 127.0f)
		return true;
	return false;
}

// TODO: UNUSED, 0x180 bytes, and nothing linked shows its body.
f32 TLimitKoopa::getFlameDir() const { return 0.0f; }

// TODO: UNUSED, 0x48 bytes, and nothing linked shows its body.
bool TLimitKoopa::isFlaming() const { return false; }

// TODO: UNUSED, 0xE8 bytes, and nothing linked shows its body.
f32 TLimitKoopa::getNeckFocus() const { return 0.0f; }

// TODO: UNUSED, and nothing linked shows its body. It is rebuilt from
// TKoopa::getDown without the nerves this Bowser lacks, but compiles to 0x294
// bytes instead of the map's 0x2A0.
void TLimitKoopa::getDown()
{
	if (&TNerveLimitKoopaFall::theNerve() == mSpine->getCurrentNerve()
	    || &TNerveLimitKoopaTumble::theNerve() == mSpine->getCurrentNerve())
		return;

	if (&TNerveLimitKoopaStagger::theNerve() == mSpine->getCurrentNerve()
	    || &TNerveLimitKoopaGetShowered::theNerve()
	           == mSpine->getCurrentNerve())
		mSpine->setNext(&TNerveLimitKoopaGetDown::theNerve());
	mSpine->pushNerve(&TNerveLimitKoopaGetDown::theNerve());
}

void TLimitKoopa::getShowered()
{
	if (&TNerveLimitKoopaTumble::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveLimitKoopaGetDown::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveLimitKoopaStagger::theNerve() == mSpine->getCurrentNerve())
		mSpine->setNext(&TNerveLimitKoopaGetShowered::theNerve());
	mSpine->pushNerve(&TNerveLimitKoopaGetShowered::theNerve());
}

// UNUSED, and nothing linked shows its body. It is rebuilt from
// TKoopa::stagger without the nerves this Bowser lacks: the nerve destructors
// placed right after it in the map are the ones it references, and the size
// matches the map's.
void TLimitKoopa::stagger(bool force)
{
	if (!force
	    && &TNerveLimitKoopaFlame::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveLimitKoopaTumble::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveLimitKoopaGetDown::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveLimitKoopaGetShowered::theNerve() == mSpine->getCurrentNerve())
		return;

	mSpine->pushNerve(&TNerveLimitKoopaStagger::theNerve());
}

// TODO: UNUSED, 8 bytes, so this is the only thing it can be.
bool TLimitKoopa::isTumbling() const { return false; }

f32 TLimitKoopa::getAnmFrame() const
{
	return getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
}

// UNUSED, and nothing linked shows its body. The Fall nerve's destructor
// right after it in the map says it uses that nerve, and this is TKoopa::fall
// without the turn to 180 degrees (there is no 180.0f in this unit's
// constants); the size matches the map's.
void TLimitKoopa::fall() { mSpine->setNext(&TNerveLimitKoopaFall::theNerve()); }

f32 TLimitKoopa::getAnmEnd() const
{
	return getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd();
}

MtxPtr TLimitKoopa::getHeadMtx() const
{
	return getMActor()->getModel()->getAnmMtx(mHeadJoint);
}

// TODO: UNUSED, 0x68 bytes, and nothing linked shows its body. The order of
// the float constants says it is the first function to use 0.0f.
void TLimitKoopa::checkMarioWhichSide() { }

void TLimitKoopa::updateTimers()
{
	for (int i = 0; i < ARRAY_COUNT(mTimers); i++)
		if (mTimers[i] > 0)
			mTimers[i]--;
}

const TLimitKoopaParams* TLimitKoopa::getParam() const
{
	return (const TLimitKoopaParams*)((TEnemyManager*)mManager)->getSaveParam();
}

TLimitKoopaManager::TLimitKoopaManager(const char* name)
    : TEnemyManager(name)
{
}

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
	                 SCENE_KOOPA_JPA_MS_KP_FIRE_A);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_b.jpa",
	                 SCENE_KOOPA_JPA_MS_KP_FIRE_B);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_c.jpa",
	                 SCENE_KOOPA_JPA_MS_KP_FIRE_C);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_d.jpa",
	                 SCENE_KOOPA_JPA_MS_KP_FIRE_D);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_e.jpa",
	                 SCENE_KOOPA_JPA_MS_KP_FIRE_E);
}
