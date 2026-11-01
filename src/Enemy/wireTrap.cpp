#include <Enemy/WireTrap.hpp>
#include <Enemy/Launcher.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>

namespace {

const char cMatName[]       = "_mat_1";
const GXColorS10 cRedColor  = { 0xD2, 0x14, 0x0F, 0x00 };
const GXColorS10 cBlueColor = { 0x0F, 0x14, 0xD2, 0x00 };

// Debug output that was compiled out of the release build.
void SMSReportVec(const char*, const JGeometry::TVec3<f32>&) { }

} // namespace

// TODO: fake inline, the same one as in enemy.cpp and riccohook.cpp
static inline JGeometry::TVec3<f32> polarXZ(f32 theta, f32 radius)
{
	f32 c = radius * MsCos(theta);
	f32 s = radius * MsSin(theta);
	return JGeometry::TVec3<f32>(s, 0.0f, c);
}

TWireTrap::TWireTrap(const char* name)
    : TSpineEnemy(name)
    , unk160(0)
    , mWaterTimer(0)
{
}

void TWireTrap::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("wire_trap.bmd", 0);
	mBinder       = new TWireBinder;
	mSpine->initWith(&TNerveWaitForever<TLiveActor>::theNerve());
	initCollision();
	initParticle();
}

// TODO: frame-only, ours is 0x10 bytes smaller.
void TWireTrap::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);

	s32 speed;
	stream.read(&speed, 4);
	s32 mode;
	stream.read(&mode, 4);
	stream.read(&mWaitTime, 4);
	s32 color;
	stream.read(&color, 4);

	mMoveSpeed = speed / 10.0f;
	if (mode == -1)
		mode = 0;
	mMoveMode = mode;

	if (color == -1)
		initThisColor(&cRedColor);
	else
		initThisColor(&cBlueColor);

	initWire();
}

void TWireTrap::initCollision()
{
	initHitActor(ACTOR_TYPE_WIRE_TRAP, 2, 0x90000000, 20.0f, 30.0f, 40.0f,
	             40.0f);
	offHitFilter(HIT_FILTER_NO_COLLISION);
}

void TWireTrap::initParticle()
{
	SMS_LoadParticle("/scene/wireTrap/jpa/ms_wrt_biri_a.jpa",
	                 WIRETRAP_JPA_MS_WRT_BIRI_A);
	SMS_LoadParticle("/scene/wireTrap/jpa/ms_wrt_biri_b.jpa",
	                 WIRETRAP_JPA_MS_WRT_BIRI_B);
}

void TWireTrap::initWire()
{
	getWireBinder()->init(mPosition);
	setMoveMode(mMoveMode);

	// Start moving the way the trap faces in the scene file.
	mMoveDir
	    = 0.0f <= polarXZ(mRotation.y, 1.0f).dot(getWireDir()) ? 1.0f : -1.0f;
	mAppearScale = 1.0f;
}

void TWireTrap::initThisColor(const GXColorS10* color)
{
	s32 index
	    = getModel()->getModelData()->getMaterialName()->getIndex(cMatName);
	SMS_InitPacket_OneTevColor(getModel(), index, GX_TEVREG2, color);
}

// TODO: frame-only, ours is 8 bytes smaller.
BOOL TWireTrap::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		behaveHitWater(sender);
		return true;

	case HIT_MESSAGE_TAKE:
		if (mHolder == nullptr) {
			onHitFilter(HIT_FILTER_NO_COLLISION);
			mHolder = (TTakeActor*)sender;
			return true;
		}
		break;

	case HIT_MESSAGE_THROWN:
	case HIT_MESSAGE_DETACH:
		if (mHolder != nullptr) {
			mHolder = nullptr;
			return true;
		}
		break;

	case HIT_MESSAGE_UNKB:
		kill();
		return true;
	}

	return TSpineEnemy::receiveMessage(sender, message);
}

// Water pushes the trap along its wire, away from Mario, for a while.
void TWireTrap::behaveHitWater(THitActor* sender)
{
	SMS_EasyEmitParticle(PARTICLE_MS_ENM_WATHIT, &mPosition, nullptr,
	                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	gpMSound->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0, 0.0f, 0,
	                        0, 4);
	mWaterTimer = 30;

	JGeometry::TVec3<f32> fromMario = mPosition;
	fromMario -= *gpMarioPos;
	JGeometry::TVec3<f32> dir = getWireDir();
	dir.scale(mMoveDir);
	if (0.0f <= fromMario.dot(dir))
		mWaterPow = getParam()->mInWaterPowerRate.get();
	else
		mWaterPow = -getParam()->mInWaterPowerRate.get();
}

void TWireTrap::kill()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	onLiveFlag(LIVE_FLAG_DEAD);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	mSpine->reset();
	mSpine->setNext(&TNerveWaitForever<TLiveActor>::theNerve());
	SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_A, &mPosition, nullptr,
	                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_B, &mPosition, nullptr,
	                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
}

// Two traps that bump into each other bounce apart, unless they were already
// heading the same way.
void TWireTrap::behaveHitWireTrap(TWireTrap* other,
                                  const JGeometry::TVec3<f32>& momentum,
                                  const JGeometry::TVec3<f32>& other_momentum)
{
	if (mHitTimer <= 0 && isReflect()) {
		mHitTimer = 30;
		// The labels are made up: the strings were dropped with the calls.
		SMSReportVec("my momentum", calcMomentum());
		SMSReportVec("other momentum", other->calcMomentum());
		if (momentum.dot(other_momentum) < 0.0f)
			mMoveDir *= -1.0f;
		mSpine->pushNerve(&TNerveWireTrapWait::theNerve());
	}
}

// TODO: nonmatching (94%). The float registers in the matrix-to-quaternion
// part are swapped (see the regswap note on TRotation3::getQuat), and the
// target keeps the address of `mtx` in r31 and `this` in r30.
void TWireTrap::calcRootMatrix()
{
	if (mHolder != nullptr) {
		TSpineEnemy::calcRootMatrix();
		return;
	}

	emitEffects();

	// Face along the wire, spinning around that direction.
	JGeometry::TVec3<f32> dir = getDirAtWirePos();
	dir.scale(mMoveDir);
	TPosition3f mtx;
	SMS_CalcToDirMatrix(mtx, dir, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
	JGeometry::TQuat4<f32> rot;
	mtx.getQuat(rot);

	mRotation.z += -17.75f;
	mRotation.z = MsWrapFmodf(mRotation.z, 0.0f, 360.0f);
	JGeometry::TQuat4<f32> spin;
	spin.setRotate(JGeometry::TVec3<f32>(0.0f, 0.0f, 1.0f),
	               0.017453294f * mRotation.z);
	JGeometry::TQuat4<f32> result;
	result.mul(rot, spin);

	mtx.setQuat(result);
	mtx.setTrans(mPosition);
	getModel()->setBaseTRMtx(mtx);

	JGeometry::TVec3<f32> scale = mScaling;
	scale.scale(mAppearScale);
	getModel()->setBaseScale(scale);
}

void TWireTrap::moveObject()
{
	if (getWaterTimer() > 0) {
		--mWaterTimer;
		if (mWaterTimer <= 0)
			mWaterPow = 0.0f;
	}

	updateCollision();
	checkHitActors();
	TLiveActor::moveObject();
	SMSGetMSound()->startSoundActor(MSD_SE_EN_WIRETRAP_WAIT, &mPosition, 0,
	                                nullptr, 0, 4);
}

JGeometry::TVec3<f32> TWireTrap::calcMomentum()
{
	JGeometry::TVec3<f32> momentum = getWireDir();
	momentum
	    *= mMoveDir
	       * (mWaterTimer > 0 ? 1.0f + mWaterPow * mWaterTimer / 30.0f : 1.0f);
	momentum *= mMoveSpeed;

	return momentum;
}

// Right after being sprayed, the trap gets pushed along for a while.
f32 TWireTrap::getWaterPow() const { return mWaterPow * mWaterTimer / 30.0f; }

bool TWireTrap::doReturnMove()
{
	if (getHitTimer() > 0)
		--mHitTimer;

	if (getWireBinder()->isEndWire(mPosition, mMoveDir)) {
		mMoveDir *= -1.0f;
		return true;
	}

	mPositionDelta = calcMomentum();
	return false;
}

bool TWireTrap::doOnewayMove()
{
	if (getHitTimer() > 0)
		--mHitTimer;

	if (getWireBinder()->isEndWire(mPosition, mMoveDir))
		return true;

	mPositionDelta = calcMomentum();
	return false;
}

bool TWireTrap::doSearchMove()
{
	if (!SMS_IsMarioOnWire())
		return false;

	if (getHitTimer() > 0)
		--mHitTimer;

	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario -= mPosition;
	f32 dot  = toMario.dot(getWireBinder()->getDir());
	mMoveDir = dot > 0.0f ? 1 : dot < 0.0f ? -1 : 0;

	mPositionDelta = calcMomentum();
	return false;
}

bool TWireTrap::doScaleUp()
{
	onHitFilter(HIT_FILTER_NO_COLLISION);
	mAppearScale += 1.0f / getParam()->mScaleTimerMax.get();
	if (1.0f <= mAppearScale) {
		mAppearScale = 1.0f;
		offHitFilter(HIT_FILTER_NO_COLLISION);
		return true;
	}
	return false;
}

bool TWireTrap::doScaleDown()
{
	onHitFilter(HIT_FILTER_NO_COLLISION);
	mAppearScale -= 1.0f / getParam()->mScaleTimerMax.get();
	if (mAppearScale <= 0.0f) {
		mAppearScale = 0.0f;
		offHitFilter(HIT_FILTER_NO_COLLISION);
		return true;
	}
	return false;
}

// Jump back to the end of the wire the trap started from.
void TWireTrap::doResetToEdge()
{
	f32 edge = 0.0f < mMoveDir ? 0.0f : 1.0f;
	getWireBinder()->getPoint(&mPosition, edge + 0.01f * mMoveDir);
}

// TODO: frame-only, ours is 0x98 bytes smaller.
void TWireTrap::checkHitActors()
{
	THitActor** end = mCollisions + mColCount;
	for (THitActor** it = mCollisions; it != end; ++it) {
		THitActor* actor = *it;
		switch (actor->getActorType()) {
		case ACTOR_TYPE_MARIO:
			SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK);
			break;

		case ACTOR_TYPE_WIRE_TRAP:
			if (actor != this) {
				TWireTrap* other                     = (TWireTrap*)actor;
				JGeometry::TVec3<f32> momentum       = calcMomentum();
				JGeometry::TVec3<f32> other_momentum = other->calcMomentum();
				behaveHitWireTrap(other, momentum, other_momentum);
				// TODO: passing `other` again looks like a slip, but it is
				// what the binary does (only the debug output reads it).
				other->behaveHitWireTrap(other, other_momentum, momentum);
			}
			break;
		}
	}
}

void TWireTrap::updateCollision()
{
	setHitParams(20.0f * mScaling.x, 30.0f * mScaling.y, 40.0f * mScaling.x,
	             40.0f * mScaling.y);
}

void TWireTrap::emitEffects()
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    WIRETRAP_JPA_MS_WRT_BIRI_A, &mPosition, 1, this);
	if (emitter)
		emitter->setGlobalScale(mScaling);
	emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    WIRETRAP_JPA_MS_WRT_BIRI_B, &mPosition, 1, this);
	if (emitter)
		emitter->setGlobalScale(mScaling);
}

void TWireTrap::setMoveMode(int mode)
{
	mMoveMode = mode;
	mSpine->reset();
	mSpine->setNext(getNerveFromMode(mode));
}

JGeometry::TVec3<f32> TWireTrap::getDirAtWirePos() const
{
	return getWireBinder()->getDirAtPos(mPosition, mMoveDir);
}

const JGeometry::TVec3<f32>& TWireTrap::getWireDir() const
{
	return getWireBinder()->getDir();
}

bool TWireTrap::isReflect() const { return mMoveMode == WIRE_TRAP_MODE_RETURN; }

const TNerveBase<TLiveActor>* TWireTrap::getNerveFromMode(int mode)
{
	switch (mode) {
	case WIRE_TRAP_MODE_RETURN:
		return &TNerveWireTrapReturnMove::theNerve();
	case WIRE_TRAP_MODE_ONEWAY:
		return &TNerveWireTrapOnewayMove::theNerve();
	case WIRE_TRAP_MODE_SEARCH:
		return &TNerveWireTrapSearch::theNerve();
	}
	return nullptr;
}

f32 TWireTrap::getRangePosInWire() const
{
	return getWireBinder()->getRangePos(mPosition);
}

TWireBinder* TWireTrap::getWireBinder() { return (TWireBinder*)mBinder; }

const TWireBinder* TWireTrap::getWireBinder() const
{
	return (const TWireBinder*)mBinder;
}

bool TWireTrap::isStartWire() const
{
	return getWireBinder()->isStartWire(mPosition, mMoveDir);
}

bool TWireTrap::isEndWire() const
{
	return getWireBinder()->isEndWire(mPosition, mMoveDir);
}

TWireTrapParams::TWireTrapParams(const char* path)
    : TSpineEnemyParams(path)
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
	static const TModelDataLoadEntry entry[]
	    = { { "wire_trap.bmd", 0x10210000, 0 }, { nullptr, 0, 0 } };
	createModelDataArray(entry);
}

// TODO: frame-only.
DEFINE_NERVE(TNerveWireTrapReturnMove, TLiveActor)
{
	TWireTrap* self = (TWireTrap*)spine->getBody();
	if (self->doReturnMove()) {
		spine->pushAfterCurrent(this);
		spine->pushAfterCurrent(&TNerveWireTrapWait::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveWireTrapOnewayMoveStart, TLiveActor)
{
	TWireTrap* self = (TWireTrap*)spine->getBody();
	if (self->doScaleUp())
		return true;
	return false;
}

// TODO: frame-only.
DEFINE_NERVE(TNerveWireTrapOnewayMove, TLiveActor)
{
	TWireTrap* self = (TWireTrap*)spine->getBody();
	if (spine->getTime() == 0 && self->mAppearScale < 1.0f) {
		spine->pushAfterCurrent(this);
		spine->pushAfterCurrent(&TNerveWireTrapGoWait::theNerve());
		spine->pushAfterCurrent(&TNerveWireTrapOnewayMoveStart::theNerve());
		return true;
	}

	if (self->doOnewayMove()) {
		spine->pushAfterCurrent(this);
		spine->pushAfterCurrent(&TNerveWireTrapWait::theNerve());
		spine->pushAfterCurrent(&TNerveWireTrapOnewayMoveEnd::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveWireTrapOnewayMoveEnd, TLiveActor)
{
	TWireTrap* self = (TWireTrap*)spine->getBody();
	if (self->doScaleDown()) {
		self->doResetToEdge();
		return true;
	}
	return false;
}

// TODO: frame-only.
DEFINE_NERVE(TNerveWireTrapSearch, TLiveActor)
{
	TWireTrap* self = (TWireTrap*)spine->getBody();
	if (self->doSearchMove()) {
		spine->pushAfterCurrent(this);
		spine->pushAfterCurrent(&TNerveWireTrapWait::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveWireTrapWait, TLiveActor)
{
	TWireTrap* self = (TWireTrap*)spine->getBody();
	if (self->mWaitTime < spine->getTime())
		return true;
	return false;
}

DEFINE_NERVE(TNerveWireTrapGoWait, TLiveActor)
{
	TWireTrap* self = (TWireTrap*)spine->getBody();
	if (self->getParam()->mGoTimerMax.get() < spine->getTime())
		return true;
	return false;
}
