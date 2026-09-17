#include <Enemy/WireTrap.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/Launcher.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Map/MapWire.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JGeometry/JGQuat4.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <Player/MarioAccess.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: these two ids have no name in include/System/Particles.hpp yet; they
// belong in E_SMS_EFFECT_LOOP_NORMAL next to FIREWANWAN_JPA_MS_CAN_HITYUGE.
// This batch may only touch wireTrap's own two files, so they live here. The
// names come from the .jpa paths init() loads.
enum {
	WIRETRAP_JPA_MS_WRT_BIRI_A = 0x190,
	WIRETRAP_JPA_MS_WRT_BIRI_B = 0x191,
};

// TODO: MtxUtil.hpp defines SMS_CalcToDirMatrix inline, but the map shows it
// as a global function emitted from Kazekun.cpp, and the ROM calls it here.
// Declaring it locally is the only way to get the call without editing
// MarioUtil/MtxUtil.hpp (move the body to Kazekun.cpp and leave a declaration
// in the header).
void SMS_CalcToDirMatrix(TPosition3f&, const JGeometry::TVec3<f32>&,
                         const JGeometry::TVec3<f32>&);

namespace {
// The one material of wire_trap.bmd whose TEV colour the mode tints.
const char cMatName[] = "_mat_1";
// Reflecting traps are red, one-way and searching ones blue.
const GXColorS10 cRedColor  = { 210, 20, 15, 0 };
const GXColorS10 cBlueColor = { 15, 20, 210, 0 };

// UNUSED, 4 bytes in the map: a debug print that the retail build compiled
// down to nothing. Left empty for that reason.
void SMSReportVec(const char* name, const JGeometry::TVec3<f32>& v) { }
} // namespace

TWireTrap::TWireTrap(const char* name)
    : TSpineEnemy(name)
    , unk160(0)
    , mWaterTimer(0)
{
}

void TWireTrap::init(TLiveManager* live_manager)
{
	mManager = live_manager;
	mManager->manageActor(this);

	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("wire_trap.bmd", 0);

	mBinder = (TBinder*)new TWireBinder();

	mSpine->reset();
	mSpine->initWith(&TNerveWaitForever<TLiveActor>::theNerve());

	initCollision();
	initParticle();
}

void TWireTrap::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);

	int speed;
	int mode;
	stream.read(&speed, 4);
	stream.read(&mode, 4);
	stream.read(&mWaitTime, 4);
	int color;
	stream.read(&color, 4);

	mSpeed = speed / 10.0f;

	if (mode == -1)
		mode = WIRETRAP_MODE_RETURN;
	mMoveMode = mode;

	if (color == -1)
		initThisColor(&cRedColor);
	else
		initThisColor(&cBlueColor);

	initWire();
}

// UNUSED, 0x58 in the map.
void TWireTrap::initCollision()
{
	initHitActor(0x10000026, 2, ACTOR_TYPE_PLAYER | ACTOR_TYPE_ENEMY, 20.0f,
	             30.0f, 40.0f, 40.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

// UNUSED, 0x8c in the map.
void TWireTrap::initParticle()
{
	SMS_LoadParticle("/scene/wireTrap/jpa/ms_wrt_biri_a.jpa",
	                 WIRETRAP_JPA_MS_WRT_BIRI_A);
	SMS_LoadParticle("/scene/wireTrap/jpa/ms_wrt_biri_b.jpa",
	                 WIRETRAP_JPA_MS_WRT_BIRI_B);
}

// UNUSED, 0x108 in the map. Binds to the wire under us and picks the travel
// direction from the placed yaw: we start out going whichever way along the
// wire the model is already facing.
void TWireTrap::initWire()
{
	getWireBinder()->init(mPosition);

	mSpine->reset();
	mSpine->setNext(getNerveFromMode(mMoveMode));

	JGeometry::TVec3<f32> facing(
	    JGeometry::TUtil<f32>::one() * JMASin(mRotation.y), 0.0f,
	    JGeometry::TUtil<f32>::one() * JMACos(mRotation.y));

	if (0.0f <= facing.dot(getWireBinder()->getDir()))
		mMoveDir = 1.0f;
	else
		mMoveDir = -1.0f;

	mScaleRate = 1.0f;
}

// UNUSED, 0x6c in the map.
void TWireTrap::initThisColor(const GXColorS10* color)
{
	int index
	    = getModel()->getModelData()->getMaterialName()->getIndex(cMatName);
	SMS_InitPacket_OneTevColor(getModel(), index, GX_TEVREG2, color);
}

BOOL TWireTrap::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		behaveHitWater(sender);
		return TRUE;

	case HIT_MESSAGE_TAKE:
		if (!getHolder()) {
			onHitFlag(HIT_FLAG_NO_COLLISION);
			mHolder = (TTakeActor*)sender;
			return TRUE;
		}
		break;

	case HIT_MESSAGE_THROWN:
	case HIT_MESSAGE_UNK8:
		if (getHolder()) {
			mHolder = nullptr;
			return TRUE;
		}
		break;

	case HIT_MESSAGE_UNKB:
		kill();
		return TRUE;
	}

	return TSpineEnemy::receiveMessage(sender, message);
}

// UNUSED, 0x17c in the map. A water hit slows the trap down for half a second,
// pushing it the way the jet came from.
void TWireTrap::behaveHitWater(THitActor* sender)
{
	JGeometry::TVec3<f32> scale(1.0f, 1.0f, 1.0f);
	SMS_EasyEmitParticle(PARTICLE_MS_ENM_WATHIT, &mPosition, nullptr, scale);

	SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0,
	                              0.0f, 0, 0, 4);

	mWaterTimer = 30;

	JGeometry::TVec3<f32> toWater = mPosition;
	toWater -= *gpMarioPos;

	JGeometry::TVec3<f32> dir = getWireBinder()->getDir();
	dir.scale(mMoveDir);

	if (0.0f <= toWater.dot(dir))
		mWaterPower = getSaveParams()->mInWaterPowerRate.get();
	else
		mWaterPower = -getSaveParams()->mInWaterPowerRate.get();
}

void TWireTrap::kill()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD)) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFlag(HIT_FLAG_NO_COLLISION);

		mSpine->reset();
		mSpine->setNext(&TNerveWaitForever<TLiveActor>::theNerve());

		JGeometry::TVec3<f32> scaleA(1.0f, 1.0f, 1.0f);
		SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_A, &mPosition, nullptr,
		                     scaleA);
		JGeometry::TVec3<f32> scaleB(1.0f, 1.0f, 1.0f);
		SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_B, &mPosition, nullptr,
		                     scaleB);
	}
}

// UNUSED, 0x258 in the map. Two reflecting traps that run into each other both
// stop and turn around, but only if neither is still in its post-bump grace
// period.
void TWireTrap::behaveHitWireTrap(TWireTrap* partner,
                                  const JGeometry::TVec3<f32>& mine,
                                  const JGeometry::TVec3<f32>& theirs)
{
	if (mCollideTimer > 0)
		return;
	if (mMoveMode != WIRETRAP_MODE_RETURN)
		return;

	mCollideTimer = 30;

	JGeometry::TVec3<f32> myMomentum = getWireDir();
	f32 myRate;
	if (mWaterTimer > 0)
		myRate = 1.0f + getWaterPow();
	else
		myRate = 1.0f;
	myMomentum.scale(mMoveDir * myRate);
	myMomentum.scale(mSpeed);

	JGeometry::TVec3<f32> hisMomentum = partner->getWireDir();
	f32 hisRate;
	if (partner->mWaterTimer > 0)
		hisRate = 1.0f + partner->getWaterPow();
	else
		hisRate = 1.0f;
	hisMomentum.scale(partner->mMoveDir * hisRate);
	hisMomentum.scale(partner->mSpeed);

	if (mine.dot(theirs) < 0.0f)
		mMoveDir *= -1.0f;

	mSpine->pushNerve(&TNerveWireTrapWait::theNerve());
}

void TWireTrap::calcRootMatrix()
{
	if (getHolder()) {
		TSpineEnemy::calcRootMatrix();
		return;
	}

	emitEffects();

	TPosition3f mtx;
	JGeometry::TQuat4<f32> spinQuat;
	JGeometry::TQuat4<f32> quat;

	JGeometry::TVec3<f32> dir = getDirAtWirePos();
	dir.scale(mMoveDir);
	SMS_CalcToDirMatrix(mtx, dir, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
	mtx.getQuat(quat);

	// The sparking body spins around its own Z axis as it slides.
	mRotation.z += -17.75f;
	mRotation.z = 0.0f + std::fmodf(360.0f + (mRotation.z - 0.0f), 360.0f);

	spinQuat.setRotate(JGeometry::TVec3<f32>(0.0f, 0.0f, 1.0f),
	                   0.017453294f * mRotation.z);
	quat.mul(quat, spinQuat);

	mtx.setQuat(quat);
	mtx.setTrans(mPosition);
	J3DModel* model = getModel();
	MTXCopy(mtx, model->getBaseTRMtx());

	JGeometry::TVec3<f32> scale = mScaling;
	scale.scale(mScaleRate);
	getModel()->setBaseScale(scale);
}

void TWireTrap::moveObject()
{
	if (getWaterTimer() > 0) {
		mWaterTimer--;
		if (mWaterTimer <= 0)
			mWaterPower = 0.0f;
	}

	updateCollision();
	checkHitActors();
	TLiveActor::moveObject();

	SMSGetMSound()->startSoundActor(MSD_SE_EN_WIRETRAP_WAIT, &mPosition, 0,
	                                nullptr, 0, 4);
}

// UNUSED, 0xdc in the map, so the retail copy is inlined everywhere.
// TODO: ours is 0xf4 because TWireBinder::getDir() returns by value (see
// getWireDir below): the returned temporary costs one extra 12-byte copy, and
// that pushes the body past MWCC's inline budget, so the three moving nerves
// call it instead of expanding it. Fixing getDir's return type fixes all of
// them at once.
void TWireTrap::calcMomentum()
{
	JGeometry::TVec3<f32> momentum = getWireDir();

	f32 rate;
	if (mWaterTimer > 0)
		rate = 1.0f + getWaterPow();
	else
		rate = 1.0f;

	momentum.scale(mMoveDir * rate);
	momentum.scale(mSpeed);
	setLinearVelocity(momentum);
}

// UNUSED, 0x3c in the map: fifteen instructions, which is exactly the signed
// int-to-float conversion plus the product. The "1.0f +" and the timer guard
// live at each call site, not here.
f32 TWireTrap::getWaterPow() const
{
	return mWaterPower * mWaterTimer / 30.0f;
}

// UNUSED, 0x114 in the map.
bool TWireTrap::doReturnMove()
{
	if (getCollideTimer() > 0)
		mCollideTimer--;

	if (getWireBinder()->isEndWire(mPosition, mMoveDir)) {
		mMoveDir *= -1.0f;
		return true;
	}

	calcMomentum();
	return false;
}

// UNUSED, 0x104 in the map.
bool TWireTrap::doOnewayMove()
{
	if (getCollideTimer() > 0)
		mCollideTimer--;

	if (getWireBinder()->isEndWire(mPosition, mMoveDir))
		return true;

	calcMomentum();
	return false;
}

// UNUSED, 0x1b8 in the map. Only chases while Mario is actually hanging on a
// wire; otherwise the trap stays put.
bool TWireTrap::doSearchMove()
{
	if (!SMS_IsMarioOnWire())
		return false;

	if (getCollideTimer() > 0)
		mCollideTimer--;

	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario -= mPosition;

	f32 along = toMario.dot(getWireBinder()->getDir());
	int dir;
	if (along > 0.0f)
		dir = 1;
	else if (along < 0.0f)
		dir = -1;
	else
		dir = 0;
	mMoveDir = dir;

	calcMomentum();
	return false;
}

// UNUSED, 0xa4 in the map.
bool TWireTrap::doScaleUp()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);

	mScaleRate += 1.0f / getSaveParams()->mScaleTimerMax.get();
	if (1.0f <= mScaleRate) {
		mScaleRate = 1.0f;
		offHitFlag(HIT_FLAG_NO_COLLISION);
		return true;
	}

	return false;
}

// UNUSED, 0xa8 in the map.
bool TWireTrap::doScaleDown()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);

	mScaleRate -= 1.0f / getSaveParams()->mScaleTimerMax.get();
	if (mScaleRate <= 0.0f) {
		mScaleRate = 0.0f;
		offHitFlag(HIT_FLAG_NO_COLLISION);
		return true;
	}

	return false;
}

// UNUSED, 0x48 in the map. Teleports the shrunk trap back to the edge it will
// grow out of again.
void TWireTrap::doResetToEdge()
{
	getWireBinder()->getPoint(&mPosition,
	                          0.01f * mMoveDir
	                              + (0.0f < mMoveDir ? 0.0f : 1.0f));
}

void TWireTrap::checkHitActors()
{
	THitActor** end = &mCollisions[mColCount];
	for (THitActor** it = mCollisions; it != end; ++it) {
		switch ((*it)->getActorType()) {
		case 0x80000001:
			SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK);
			break;

		case 0x10000026: {
			TWireTrap* trap = (TWireTrap*)*it;
			if (trap == this)
				break;

			JGeometry::TVec3<f32> myMomentum = getWireBinder()->getDir();
			f32 myRate;
			if (mWaterTimer > 0)
				myRate = 1.0f + getWaterPow();
			else
				myRate = 1.0f;
			myMomentum.scale(mMoveDir * myRate);
			myMomentum.scale(mSpeed);

			JGeometry::TVec3<f32> hisMomentum
			    = trap->getWireBinder()->getDir();
			f32 hisRate;
			if (trap->mWaterTimer > 0)
				hisRate = 1.0f + trap->getWaterPow();
			else
				hisRate = 1.0f;
			hisMomentum.scale(trap->mMoveDir * hisRate);
			hisMomentum.scale(trap->mSpeed);

			behaveHitWireTrap(trap, myMomentum, hisMomentum);
			// TODO: retail passes `trap` here too, so the momenta the second
			// expansion recomputes are both the partner's. Kept because the
			// ROM does it; it reads like a copy-paste slip for `this`.
			trap->behaveHitWireTrap(trap, hisMomentum, myMomentum);
			break;
		}
		}
	}
}

// UNUSED, 0x54 in the map.
void TWireTrap::updateCollision()
{
	setHitParams(20.0f * mScaling.x, 30.0f * mScaling.y, 40.0f * mScaling.x,
	             40.0f * mScaling.y);
}

// UNUSED, 0xc8 in the map. The map has no
// SMS_EasyEmitParticle<E_SMS_EFFECT_LOOP_NORMAL> instantiation anywhere in the
// binary (only the ONETIME one, in six objects), and 0xc8 is far too big for
// two calls to it, so the original spelled the helper's two statements out
// here with the looping flag.
void TWireTrap::emitEffects()
{
	JPABaseEmitter* emitterA = gpMarioParticleManager->emitAndBindToPosPtr(
	    WIRETRAP_JPA_MS_WRT_BIRI_A, &mPosition, 1, this);
	if (emitterA)
		emitterA->setGlobalScale(mScaling);

	JPABaseEmitter* emitterB = gpMarioParticleManager->emitAndBindToPosPtr(
	    WIRETRAP_JPA_MS_WRT_BIRI_B, &mPosition, 1, this);
	if (emitterB)
		emitterB->setGlobalScale(mScaling);
}

// UNUSED, 0x160 in the map.
void TWireTrap::setMoveMode(int mode)
{
	mMoveMode = mode;
	mSpine->reset();
	mSpine->setNext(getNerveFromMode(mMoveMode));
}

// UNUSED, 0x3c in the map.
JGeometry::TVec3<f32> TWireTrap::getDirAtWirePos() const
{
	return getWireBinder()->getDirAtPos(mPosition, mMoveDir);
}

// TODO: the ROM's copy is 0xc bytes -- `lwz 0x88; addi 8; blr` -- so it hands
// back a reference. Reproducing that needs one word changed in
// include/Enemy/WireBinder.hpp: TWireBinder::getDir() must return
// `const JGeometry::TVec3<f32>&`, not a value. mDir is private, so this TU
// cannot get at it any other way. This batch may not edit that header, so the
// value return stays and every caller that copies the result pays for it.
JGeometry::TVec3<f32> TWireTrap::getWireDir() const
{
	return getWireBinder()->getDir();
}

// UNUSED, 0x14 in the map.
BOOL TWireTrap::isReflect() const
{
	return mMoveMode == WIRETRAP_MODE_RETURN;
}

const TNerveBase<TLiveActor>* TWireTrap::getNerveFromMode(int mode)
{
	switch (mode) {
	case WIRETRAP_MODE_RETURN:
		return &TNerveWireTrapReturnMove::theNerve();
	case WIRETRAP_MODE_ONEWAY:
		return &TNerveWireTrapOnewayMove::theNerve();
	case WIRETRAP_MODE_SEARCH:
		return &TNerveWireTrapSearch::theNerve();
	}

	return nullptr;
}

// UNUSED, 0x2c in the map.
f32 TWireTrap::getRangePosInWire() const
{
	return getWireBinder()->getRangePos(mPosition);
}

// UNUSED, 0x8 in the map, and dead: every caller reaches the binder through
// the const overload below.
TWireBinder* TWireTrap::getWireBinder()
{
	return (TWireBinder*)mBinder;
}

TWireBinder* TWireTrap::getWireBinder() const
{
	return (TWireBinder*)mBinder;
}

// UNUSED, 0x30 in the map -- the same size as isEndWire, so this really
// returns the binder's answer.
// TODO: TWireBinder::isStartWire() is declared `void` in
// include/Enemy/WireBinder.hpp, so the value cannot be forwarded yet. That
// header is out of scope for this batch; changing its return type to `bool`
// (like isEndWire) is the fix.
void TWireTrap::isStartWire() const
{
	getWireBinder()->isStartWire(mPosition, mMoveDir);
}

// UNUSED, 0x30 in the map.
BOOL TWireTrap::isEndWire() const
{
	return getWireBinder()->isEndWire(mPosition, mMoveDir);
}

// UNUSED, 0x100 in the map.
TWireTrapParams::TWireTrapParams(const char* prm)
    : TSpineEnemyParams(prm)
    , PARAM_INIT(mInWaterPowerRate, 0.9f)
    , PARAM_INIT(mScaleTimerMax, 60)
    , PARAM_INIT(mGoTimerMax, 90)
{
	TParams::load(mPrmPath);
}

TWireTrapManager::TWireTrapManager(const char* name)
    : TEnemyManager(name)
{
}

void TWireTrapManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TWireTrapParams("/enemy/wiretrap.prm");
	TEnemyManager::load(stream);
}

void TWireTrapManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "wire_trap.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

DEFINE_NERVE(TNerveWireTrapReturnMove, TLiveActor)
{
	TWireTrap* trap = (TWireTrap*)spine->getBody();

	if (trap->doReturnMove()) {
		spine->pushAfterCurrent(this);
		spine->pushAfterCurrent(&TNerveWireTrapWait::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveWireTrapOnewayMoveStart, TLiveActor)
{
	TWireTrap* trap = (TWireTrap*)spine->getBody();

	if (trap->doScaleUp())
		return TRUE;

	return FALSE;
}

DEFINE_NERVE(TNerveWireTrapOnewayMove, TLiveActor)
{
	TWireTrap* trap = (TWireTrap*)spine->getBody();

	if (spine->getTime() == 0 && trap->mScaleRate < 1.0f) {
		spine->pushAfterCurrent(this);
		spine->pushAfterCurrent(&TNerveWireTrapGoWait::theNerve());
		spine->pushAfterCurrent(&TNerveWireTrapOnewayMoveStart::theNerve());
		return TRUE;
	}

	if (trap->doOnewayMove()) {
		spine->pushAfterCurrent(this);
		spine->pushAfterCurrent(&TNerveWireTrapWait::theNerve());
		spine->pushAfterCurrent(&TNerveWireTrapOnewayMoveEnd::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveWireTrapOnewayMoveEnd, TLiveActor)
{
	TWireTrap* trap = (TWireTrap*)spine->getBody();

	if (trap->doScaleDown()) {
		trap->doResetToEdge();
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveWireTrapSearch, TLiveActor)
{
	TWireTrap* trap = (TWireTrap*)spine->getBody();

	if (trap->doSearchMove()) {
		spine->pushAfterCurrent(this);
		spine->pushAfterCurrent(&TNerveWireTrapWait::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveWireTrapWait, TLiveActor)
{
	TWireTrap* trap = (TWireTrap*)spine->getBody();

	if (trap->mWaitTime < spine->getTime())
		return TRUE;

	return FALSE;
}

DEFINE_NERVE(TNerveWireTrapGoWait, TLiveActor)
{
	TWireTrap* trap = (TWireTrap*)spine->getBody();

	if (trap->getSaveParams()->mGoTimerMax.get() < spine->getTime())
		return TRUE;

	return FALSE;
}
