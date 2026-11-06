#include <Enemy/LimitKoopa.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSSetSound.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <System/Particles.hpp>

// TODO: first pass, still NonMatching.

// rogue includes needed for matching sinit & bss
#include <Map/MapCollisionEntry.hpp>
#include <MSound/MSoundBGM.hpp>

inline const TNerveLimitKoopaHipDropJump&
TNerveLimitKoopaHipDropJump::theNerve()
{
	static TNerveLimitKoopaHipDropJump nerve;
	return nerve;
}

inline const TNerveLimitKoopaHipDropStart&
TNerveLimitKoopaHipDropStart::theNerve()
{
	static TNerveLimitKoopaHipDropStart nerve;
	return nerve;
}

inline const TNerveLimitKoopaTurnR& TNerveLimitKoopaTurnR::theNerve()
{
	static TNerveLimitKoopaTurnR nerve;
	return nerve;
}

inline const TNerveLimitKoopaTurnL& TNerveLimitKoopaTurnL::theNerve()
{
	static TNerveLimitKoopaTurnL nerve;
	return nerve;
}

inline const TNerveLimitKoopaTumble& TNerveLimitKoopaTumble::theNerve()
{
	static TNerveLimitKoopaTumble nerve;
	return nerve;
}

inline const TNerveLimitKoopaWait& TNerveLimitKoopaWait::theNerve()
{
	static TNerveLimitKoopaWait nerve;
	return nerve;
}

inline const TNerveLimitKoopaFlame& TNerveLimitKoopaFlame::theNerve()
{
	static TNerveLimitKoopaFlame nerve;
	return nerve;
}

inline const TNerveLimitKoopaGetDown& TNerveLimitKoopaGetDown::theNerve()
{
	static TNerveLimitKoopaGetDown nerve;
	return nerve;
}

inline const TNerveLimitKoopaStagger& TNerveLimitKoopaStagger::theNerve()
{
	static TNerveLimitKoopaStagger nerve;
	return nerve;
}

inline const TNerveLimitKoopaFall& TNerveLimitKoopaFall::theNerve()
{
	static TNerveLimitKoopaFall nerve;
	return nerve;
}

inline const TNerveLimitKoopaGetShowered&
TNerveLimitKoopaGetShowered::theNerve()
{
	static TNerveLimitKoopaGetShowered nerve;
	return nerve;
}

enum TLimitKoopaPartType {
	LIMIT_KOOPA_PART_FLAME = 0x08000030,
	LIMIT_KOOPA_PART_HEAD  = 0x08000031,
	LIMIT_KOOPA_PART_HAND  = 0x08000032,
	LIMIT_KOOPA_PART_BODY  = 0x08000033,
};

enum TLimitKoopaAnm {
	LIMIT_KOOPA_ANM_DOWN,
	LIMIT_KOOPA_ANM_DOWN_LOOP,
	LIMIT_KOOPA_ANM_FALL,
	LIMIT_KOOPA_ANM_FIRE_END,
	LIMIT_KOOPA_ANM_FIRE_LOOP,
	LIMIT_KOOPA_ANM_FIRE_START,
	LIMIT_KOOPA_ANM_FIRST,
	LIMIT_KOOPA_ANM_GETUP,
	LIMIT_KOOPA_ANM_HIPDROP,
	LIMIT_KOOPA_ANM_STAGGER,
	LIMIT_KOOPA_ANM_TURN_L,
	LIMIT_KOOPA_ANM_TURN_R,
	LIMIT_KOOPA_ANM_WAIT,
	LIMIT_KOOPA_ANM_WAIT_LOOP,
	LIMIT_KOOPA_ANM_WATERHIT,
};

DEFINE_NERVE_EXECUTE(TNerveLimitKoopaHipDropJump, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	koopa->moveHipDrop();
	if (koopa->getUnk168()) {
		spine->pushAfterCurrent(&TNerveLimitKoopaWait::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveLimitKoopaHipDropStart, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	if (spine->getTime() == 0) {
		koopa->changeBck(LIMIT_KOOPA_ANM_FIRE_START, 2.0f);
		koopa->setTimer(1, 30);
	}
	if (koopa->getTimer(1) <= 0) {
		koopa->startHipDrop();
		koopa->setUnk168(false);
		spine->pushAfterCurrent(&TNerveLimitKoopaHipDropJump::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveLimitKoopaWait, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	if (spine->getTime() == 0) {
		koopa->changeBck(LIMIT_KOOPA_ANM_TURN_L, 2.0f);
		koopa->setTimer(0, 240);
	}
	koopa->moveTurn();
	if (koopa->finishedTurn()
	    && koopa->getMActor()->isCurAnmAlreadyEnd(ANM_TYPE_BCK)) {
		spine->pushAfterCurrent(&TNerveLimitKoopaHipDropStart::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveLimitKoopaTumble, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	koopa->changeBck(LIMIT_KOOPA_ANM_HIPDROP,
	                 koopa->getParam()->getTumbleSpeed());
	if (koopa->endsAnm())
		return true;
	return false;
}

// Unused: GMSJ01/D38J01 mario.MAP, 0x6C bytes.
// Unused: D38J01 marioL.MAP, 0x90 bytes.
DEFINE_NERVE_EXECUTE(TNerveLimitKoopaFall, TLiveActor) { }

// Unused: GMSJ01/D38J01 mario.MAP, 0x170 bytes.
// Unused: D38J01 marioL.MAP, 0x1B8 bytes.
DEFINE_NERVE_EXECUTE(TNerveLimitKoopaFlame, TLiveActor) { }

DEFINE_NERVE_EXECUTE(TNerveLimitKoopaStagger, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	koopa->changeBck(LIMIT_KOOPA_ANM_STAGGER,
	                 koopa->getParam()->getStaggerSpeed());
	if (koopa->endsAnm())
		return true;
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveLimitKoopaGetShowered, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	koopa->changeBck(LIMIT_KOOPA_ANM_WATERHIT,
	                 koopa->getParam()->getWaterhitSpeed());
	if (koopa->endsAnm())
		return true;
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveLimitKoopaGetDown, TLiveActor)
{
	TLimitKoopa* koopa = (TLimitKoopa*)spine->getBody();
	switch (koopa->getAnmIndex()) {
	case LIMIT_KOOPA_ANM_DOWN:
		if (koopa->endsAnm())
			koopa->changeBck(LIMIT_KOOPA_ANM_DOWN_LOOP,
			                 koopa->getParam()->getDownSpeed());
		break;
	case LIMIT_KOOPA_ANM_DOWN_LOOP:
		if (koopa->endsAnm())
			koopa->changeBck(LIMIT_KOOPA_ANM_GETUP,
			                 koopa->getParam()->getDownSpeed());
		break;
	case LIMIT_KOOPA_ANM_GETUP:
		if (koopa->endsAnm())
			return true;
		break;
	default:
		koopa->changeBck(LIMIT_KOOPA_ANM_DOWN,
		                 koopa->getParam()->getDownSpeed());
		break;
	}
	return false;
}

TLimitKoopaParts::TLimitKoopaParts(const char* name, u32 actor_type,
                                   TLimitKoopa* koopa, f32 radius)
    : TLiveActor(name)
    , unkF4(koopa)
{
	((TIdxGroupObj*)JDrama::TNameRefGen::search("敵グループ"))
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
	if (cue & CUE_MOVE)
		for (int i = 0; i < mColCount; ++i)
			attack_(mCollisions[i]);
}

void TLimitKoopaParts::set(const JGeometry::TVec3<f32>& pos, f32 radius,
                           f32 height)
{
	if (height <= 0.0f)
		height = 2.0f * radius;
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
    : TLimitKoopaParts("クッパの吐く炎", LIMIT_KOOPA_PART_FLAME, koopa, 100.0f)
{
}

BOOL TLimitKoopaFlame::receiveMessage(THitActor*, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		return false;
	default:
		return true;
	}
}

void TLimitKoopaFlame::attack_(THitActor* actor)
{
	if (actor->receiveMessage(this, HIT_MESSAGE_BURN)) {
		unkF4->stopFlame();
	}
}

TLimitKoopaHand::TLimitKoopaHand(TLimitKoopa* koopa)
    : TLimitKoopaParts("クッパの手", LIMIT_KOOPA_PART_HAND, koopa, 100.0f)
{
}

BOOL TLimitKoopaHand::receiveMessage(THitActor*, u32) { return true; }

void TLimitKoopaHand::attack_(THitActor* actor)
{
	actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

TLimitKoopaHead::TLimitKoopaHead(TLimitKoopa* koopa)
    : TLimitKoopaParts("クッパの頭", LIMIT_KOOPA_PART_HEAD, koopa, 100.0f)
{
}

BOOL TLimitKoopaHead::receiveMessage(THitActor*, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		unkF4->getShowered();
		break;
	}
	return true;
}

void TLimitKoopaHead::attack_(THitActor* actor)
{
	actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

TLimitKoopaBody::TLimitKoopaBody(TLimitKoopa* koopa)
    : TLimitKoopaParts("クッパの体", LIMIT_KOOPA_PART_BODY, koopa, 100.0f)
{
}

BOOL TLimitKoopaBody::receiveMessage(THitActor*, u32) { return true; }

void TLimitKoopaBody::attack_(THitActor* actor)
{
	actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

namespace {
BOOL KoopaNeckCallBack(J3DNode*, int) { return true; }
} // namespace

TLimitKoopa::TLimitKoopa(const char* name)
    : TSpineEnemy(name)
{
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
	onLiveFlag(LIVE_FLAG_NO_BIND);
	mScaledBodyRadius = 1600.0f;
}

void TLimitKoopa::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);
}

void TLimitKoopa::loadAfter()
{
	TSpineEnemy::loadAfter();
	for (int i = 0; i < 10; ++i)
		unk178[i] = new TLimitKoopaFlame(this);
	for (int i = 0; i < 2; ++i)
		unk1A0[i] = new TLimitKoopaHand(this);
	unk1A8 = new TLimitKoopaHead(this);
	unk1AC = new TLimitKoopaBody(this);
}

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
	for (u16 i = 0; i < names->getNameNum(); ++i) { }
	unk1B8                  = names->getIndex("ago");
	unk1B0                  = names->getIndex("head");
	unk1B4                  = names->getIndex("neck");
	J3DModelData* modelData = getModel()->getModelData();
	J3DJoint* head          = modelData->getJointNodePointer(unk1B0);
	head->setCallBack(KoopaNeckCallBack);
	head->setCallBackUserData(this);
}

void TLimitKoopa::reset()
{
	TSpineEnemy::reset();
	changeBck(LIMIT_KOOPA_ANM_WAIT, getParam()->getWaitSpeed());
	resetLimitKoopa();
}

void TLimitKoopa::resetLimitKoopa()
{
	mSpine->reset();
	unk150[0]   = 0;
	unk150[1]   = 0;
	unk150[2]   = 0;
	unk168      = true;
	unk16C.unk0 = 0.0f;
}

void TLimitKoopa::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSpineEnemy::perform(cue, graphics);
	for (int i = 0; i < 10; ++i)
		unk178[i]->perform(cue, graphics);
	unk1A8->perform(cue, graphics);
	unk1A0[0]->perform(cue, graphics);
	unk1A0[1]->perform(cue, graphics);
	unk1AC->perform(cue, graphics);
	if (cue & CUE_MOVE) {
		setUpHitActors();
		updateTimers();
	}
	if (cue & CUE_CALC_ANIM) {
		if (isBreathing()) {
			breathFlame();
		}
	}
}

void TLimitKoopa::bind()
{
	JGeometry::TVec3<f32> position = mPosition;
	position.add(mPositionDelta);
	position.add(mVelocity);
	mVelocity.add(unk15C);
	mGroundHeight = 3500.0f;
	mGroundHeight += 1.0f;
	if (position.y <= 0.05f + mGroundHeight) {
		position.y = mGroundHeight;
		unk168     = true;
		moveStop();
	}
	JGeometry::TVec3<f32> delta = position;
	delta.sub(mPosition);
	mPositionDelta.set(delta);
}

// Unused: GMSJ01/D38J01 mario.MAP, 0x20 bytes.
// Unused: D38J01 marioL.MAP, 0x20 bytes.
void TLimitKoopa::moveStop()
{
	unk15C.set(0.0f, 0.0f, 0.0f);
	mVelocity.set(0.0f, 0.0f, 0.0f);
}

void TLimitKoopa::startHipDrop()
{
	JGeometry::TVec3<f32> velocity(0.0f, 1.0f, 0.0f);
	velocity.scale(getParam()->getHipDropInitialSpeedY());
	JGeometry::TVec3<f32> target = SMS_GetMarioPos();
	target.y                     = mGroundHeight;
	JGeometry::TVec3<f32> offset;
	offset.sub(target, mPosition);
	JGeometry::TVec3<f32> destination;
	destination.add(mPosition, offset);
	velocity    = calcVelocityToJumpToY(destination, velocity.y,
	                                    getParam()->getHipDropGravityY());
	f32 squared = velocity.squared();
	f32 length  = JGeometry::TUtil<f32>::sqrt(squared);
	if (length > 200.0f) {
		velocity.normalize();
		velocity.scale(200.0f);
	}
	mVelocity.set(velocity);
}

// Unused: GMSJ01/D38J01 mario.MAP, 0x4C bytes.
// Unused: D38J01 marioL.MAP, 0x4C bytes.
void TLimitKoopa::moveHipDrop() { unk15C.set(0.0f, -getGravityY(), 0.0f); }

void TLimitKoopa::moveTurn() { makeDirection(calcTargetDirection()); }

bool TLimitKoopa::finishedTurn()
{
	if (unk16C.absDirection(calcTargetDirection()) > 0.31415927f)
		return false;
	return true;
}

f32 TLimitKoopa::calcTargetDirection()
{
	TDirectionCalc target;
	JGeometry::TVec3<f32> direction;
	direction.sub(SMS_GetMarioPos(), mPosition);
	direction.y = 0.0f;
	target.makeDirection(direction);
	return target.getUnk0();
}

// Unused: GMSJ01/D38J01 mario.MAP, 0x5C bytes.
// Unused: D38J01 marioL.MAP, 0x5C bytes.
void TLimitKoopa::makeDirection(f32 direction)
{
	f32 rotationSpeed = TDirectionCalc::d2r(getParam()->getRotationSpeed());
	unk16C.unk0       = unk16C.calcTurnDirection(direction, rotationSpeed);
}

f32 TLimitKoopa::getGravityY() const
{
	return getParam()->getHipDropGravityY();
}

BOOL TLimitKoopa::receiveMessage(THitActor* actor, u32 message)
{
	return TSpineEnemy::receiveMessage(actor, message);
}

void TLimitKoopa::calcRootMatrix()
{
	f32 scale = getParam()->getBodyScale();
	mScaling.set(scale, scale, scale);
	mRotation.y = TDirectionCalc::r2d(unk16C.getUnk0());
	TSpineEnemy::calcRootMatrix();
}

// Unused
void TLimitKoopa::stopFlame()
{
	changeBck(LIMIT_KOOPA_ANM_FIRE_END, getParam()->getFireSpeed());
}

// Unused
void TLimitKoopa::breathFlame()
{
	JGeometry::TVec3<f32> scale(getParam()->getFlameScale());
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    KOOPA_JPA_MS_KP_FIRE_E, getHeadMtx(),
	    TMarioParticleManager::EFFECT_LOOP_INDIRECT, this);
	if (emitter)
		emitter->setGlobalScale(scale);
	emitMtxParticle(KOOPA_JPA_MS_KP_FIRE_D, getHeadMtx(), scale);
	emitMtxParticle(KOOPA_JPA_MS_KP_FIRE_C, getHeadMtx(), scale);
	emitMtxParticle(KOOPA_JPA_MS_KP_FIRE_B, getHeadMtx(), scale);
	emitMtxParticle(KOOPA_JPA_MS_KP_FIRE_A, getHeadMtx(), scale);
}

void TLimitKoopa::setUpHitActors()
{
	TPosition3f* head = (TPosition3f*)getHeadMtx();
	if (isBreathing()) {
		f32 rate = 1.0f;
		if (getMActor()->getCurAnmIdx(ANM_TYPE_BCK)
		    == LIMIT_KOOPA_ANM_FIRE_START) {
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			rate = (ctrl->getFrame() - 125.0f) / (ctrl->getEnd() - 125.0f);
		}
		for (int i = 0; i < 10; ++i) {
			f32 height = getParam()->getFlameHeight();
			f32 radius = getParam()->getFlameRadius();
			f32 length = 0.8f * ((2.0f + i * 2) * radius) * rate;
			JGeometry::TVec3<f32> pos;
			head->mult(JGeometry::TVec3<f32>(length, 0.0f, 0.0f), pos);
			pos.y = mPosition.y;
			unk178[i]->set(pos, radius, height);
		}
	} else {
		for (int i = 0; i < 10; ++i)
			unk178[i]->remove();
	}
	TPosition3f* jaw = (TPosition3f*)getMActor()->getModel()->getAnmMtx(unk1B8);
	JGeometry::TVec3<f32> headPos;
	jaw->getTrans(headPos);
	headPos.y -= 200.0f;
	unk1A8->set(headPos, getParam()->getHeadRadius(), 0.0f);
}

// Unused: GMSJ01/D38J01 mario.MAP, 0x70 bytes.
// Unused: D38J01 marioL.MAP, 0x94 bytes.
void TLimitKoopa::setAnimationIndex(int) { }

void TLimitKoopa::changeBck(int index, f32 speed)
{
	MActor* actor = getMActor();
	if (!actor->checkCurBckFromIndex(index))
		actor->setBckFromIndex(index);
	getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(speed);
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
	if (getAnmIndex() == LIMIT_KOOPA_ANM_FIRE_LOOP)
		return true;
	else if (getAnmIndex() == LIMIT_KOOPA_ANM_FIRE_START
	         && getAnmFrame() >= 127.0f)
		return true;
	else
		return false;
}

// Unused: GMSJ01/D38J01 mario.MAP, 0x180 bytes.
// Unused: D38J01 marioL.MAP, 0x1C4 bytes.
f32 TLimitKoopa::getFlameDir() const { }

// Unused: GMSJ01/D38J01 mario.MAP, 0x48 bytes.
// Unused: D38J01 marioL.MAP, 0x48 bytes.
bool TLimitKoopa::isFlaming() const { }

// Unused: GMSJ01/D38J01 mario.MAP, 0xE8 bytes.
// Unused: D38J01 marioL.MAP, 0xE8 bytes.
f32 TLimitKoopa::getNeckFocus() const { }

// Unused: GMSJ01/D38J01 mario.MAP, 0x2A0 bytes.
// Unused: D38J01 marioL.MAP, 0x2A0 bytes.
void TLimitKoopa::getDown() { }

void TLimitKoopa::getShowered()
{
	if (&TNerveLimitKoopaTumble::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveLimitKoopaGetDown::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveLimitKoopaStagger::theNerve() == mSpine->getCurrentNerve()) {
		mSpine->setNext(&TNerveLimitKoopaGetShowered::theNerve());
	}
	mSpine->pushNerve(&TNerveLimitKoopaGetShowered::theNerve());
}

// Unused: GMSJ01/D38J01 mario.MAP, 0x240 bytes.
// Unused: D38J01 marioL.MAP, 0x240 bytes.
void TLimitKoopa::stagger(bool) { }

// Unused: GMSJ01/D38J01 mario.MAP, 0x8 bytes.
// Unused: D38J01 marioL.MAP, 0x8 bytes.
bool TLimitKoopa::isTumbling() const { }

f32 TLimitKoopa::getAnmFrame() const
{
	return getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
}

// Unused: GMSJ01/D38J01 mario.MAP, 0x90 bytes.
// Unused: D38J01 marioL.MAP, 0x90 bytes.
void TLimitKoopa::fall() { }

f32 TLimitKoopa::getAnmEnd() const
{
	return getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd();
}

MtxPtr TLimitKoopa::getHeadMtx() const
{
	return getMActor()->getModel()->getAnmMtx(unk1B0);
}

// Unused: GMSJ01/D38J01 mario.MAP, 0x68 bytes.
// Unused: D38J01 marioL.MAP, 0x68 bytes.
void TLimitKoopa::checkMarioWhichSide() { }

void TLimitKoopa::updateTimers()
{
	for (int i = 0; i < 3; ++i)
		if (unk150[i] > 0)
			--unk150[i];
}

TLimitKoopaParams* TLimitKoopa::getParam() const
{
	return ((TLimitKoopaManager*)mManager)->getSaveParam();
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
	TEnemyManager::loadAfter();
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_a.jpa", 0x1C0);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_b.jpa", 0x1C1);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_c.jpa", 0x1C2);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_d.jpa", 0x1C3);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_e.jpa", 0x1F3);
}
