#include <Enemy/WireTrap.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/Launcher.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Map/MapWire.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
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

// Fabricated, but the ROM's shape: the wrap computes
// `l + std::fmodf((r - l) + (t - l), r - l)`, and two inline levels above
// std::fmodf are what make MWCC emit the weak 0x5c copy the map records for
// this TU instead of expanding it. Same pair as koopajr.cpp's WrapDirectionF
// / WrapRadianF, in degrees.
static inline f32 WrapDirectionF(f32 t, f32 l, f32 r)
{
	return l + std::fmodf((r - l) + (t - l), r - l);
}

static inline f32 WrapAngleF(f32 t) { return WrapDirectionF(t, 0.0f, 360.0f); }

static inline const JGeometry::TVec3<f32>&
WireTrapWireDir(const TWireTrap* trap)
{
	return trap->getWireDir();
}


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

// TODO: fabricated level, not a map symbol. Retail reaches
// `TVec3::set<f32>(f,f,f)` through the TU's local out-of-line copy here
// (`set<f>__Q29JGeometry8TVec3<f>Ffff`, local, 0x10), which needs the
// constructor of the facing temporary at inline depth 3 -- one level more
// than `initWire` gives on its own. Wrapping the whole facing-vs-wire test
// in one TU-local helper supplies that level: it restores retail's `bl`,
// its right-to-left evaluation of `mSpine->reset()` against
// `getNerveFromMode(mMoveMode)`, and the frame's shape, taking load from
// 87.5% to 95.4% (99.9% with cc50's named mode read before reset() and the
// model/binder receivers; the frame then matches but the facing temp sits
// 0x18 low). Before cc50, what was left was 0x10 of low region (retail's facing temp
// sits at 0x80(r1) with the three stream ints packed directly above it at
// 0x8c/0x90/0x94; ours sits at 0x5c with a 4-byte hole above the ints), so
// the real shape is probably a named helper on TWireTrap rather than this
// free function -- the map lists no such symbol, so nothing better is
// committed yet.
static inline f32 WireTrapDirFromAngleY(f32 angle,
                                        const JGeometry::TVec3<f32>& dir)
{
	return JGeometry::TVec3<f32>(JGeometry::TUtil<f32>::one() * JMASin(angle),
	                             0.0f,
	                             JGeometry::TUtil<f32>::one() * JMACos(angle))
	    .dot(dir);
}

// UNUSED, 0x108 in the map. Binds to the wire under us and picks the travel
// direction from the placed yaw: we start out going whichever way along the
// wire the model is already facing.
void TWireTrap::initWire()
{
	TWireBinder* binder = getWireBinder();
	binder->init(mPosition);

	int mode = mMoveMode;
	mSpine->reset();
	mSpine->setNext(getNerveFromMode(mode));

	// The 1.0f factors are live loads from .sdata2 in the ROM, not folded
	// constants, so they came through TUtil<f32>::one() rather than a
	// literal.
	// The facing vector is an unnamed temporary, which is what gives it the
	// stack home retail reads back at 0x80/0x84/0x88(r1); a named `facing`
	// local is scalarised into registers instead and costs load about nine
	// instructions (80.4% against 87.4% here).
	//
	// TODO: retail still reaches that temporary through a *call* to the local
	// out-of-line copy of TVec3::set<f32>(f,f,f) (map:
	// `set<f>__Q29JGeometry8TVec3<f>Ffff`, local, 0x10, present in over twenty
	// objects), and no spelling here can produce that -- see the note on
	// JGVec3.hpp in the batch report. Until then the last nine instructions of
	// the block differ.
	mMoveDir = 0.0f <= WireTrapDirFromAngleY(getRotation().y,
	                                         getWireDir())
	    ? 1.0f
	    : -1.0f;

	mScaleRate = 1.0f;
}

// UNUSED, 0x6c in the map.
void TWireTrap::initThisColor(const GXColorS10* color)
{
	J3DModel* model = getModel();
	int index
	    = model->getModelData()->getMaterialName()->getIndex(cMatName);
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
	SMS_EasyEmitParticle(PARTICLE_MS_ENM_WATHIT, &mPosition, nullptr,
	                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));

	SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0,
	                              0.0f, 0, 0, 4);

	mWaterTimer = 30;

	JGeometry::TVec3<f32> toWater = getPosition();
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

		SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_A, &mPosition, nullptr,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_B, &mPosition, nullptr,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	}
}
// Fabricated level: the momentum both checkHitActors and behaveHitWireTrap
// build, returned by value (retail copies it out of a temporary).
static inline void WireTrapReaction(const TWireTrap* trap)
{
	JGeometry::TVec3<f32> momentum = WireTrapWireDir(trap);
	f32 rate = trap->mWaterTimer > 0 ? 1.0f + trap->getWaterPow() : 1.0f;
	momentum *= trap->mMoveDir * rate;
	momentum *= trap->mSpeed;
}

void TWireTrap::behaveHitWireTrap(
    TWireTrap* partner, const JGeometry::TVec3<f32>& mine,
    const JGeometry::TVec3<f32>& theirs)
{
	if (mCollideTimer > 0)
		return;
	if (mMoveMode != WIRETRAP_MODE_RETURN)
		return;

	mCollideTimer = 30;

	WireTrapReaction(this);
	WireTrapReaction(partner);

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

	// The sparking body spins around its own Z axis as it slides. The
	// redundant `- 0.0f` / `0.0f +` are in the ROM, so the wrap was written
	// against a zero lower bound rather than simplified away: the wrap goes
	// through the two-level helper pair below, which is what makes MWCC call
	// the weak std::fmodf the map lists here rather than expand it.
	mRotation.z += -17.75f;
	mRotation.z = WrapAngleF(mRotation.z);

	spinQuat.setRotate(JGeometry::TVec3<f32>(0.0f, 0.0f, 1.0f),
	                   0.017453294f * mRotation.z);
	quat.mul(spinQuat, quat);

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
// Note the `*=`: `momentum.scale(v)` would be one inline level shallower and
// MWCC expands `TVec3::scale` there, while the ROM calls it out of line from
// every nerve. Going through `operator*=` restores that call and took the
// three moving nerves from 66-91% to 99%.
//
// TODO: the last residue in all three nerves is that retail calls
// `TWireTrap::getWireDir()` out of line from here (one `bl`, 0xc, byte-exact)
// while MWCC expands it into its own two accessor calls, and that its frame is
// 16 bytes deeper. getWireDir sits at inline depth three here, where a
// one-statement body always fits, so two more levels are missing above it.
// Measured and rejected: a `const TVec3&` binding of getWireDir() before the
// copy (99.5/99.0/98.4 -> 98.5/96.2/93.2) and `momentum.set(getWireDir())`
// (-> 97.9/93.3/96.2).
void TWireTrap::calcMomentum()
{
	JGeometry::TVec3<f32> momentum = WireTrapWireDir(this);
	f32 rate = getWaterTimer() > 0 ? 1.0f + getWaterPow() : 1.0f;
	f32 k = mMoveDir * rate;
	momentum *= k;
	momentum *= mSpeed;
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
	mMoveDir = along > 0.0f ? 1 : (along < 0.0f ? -1 : 0);

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

// Fabricated level: pushes getWireBinder()/getDir() one inline level down,
// where retail calls both out of line.
static inline const JGeometry::TVec3<f32>& WireTrapBinderDir(const TWireTrap* t)
{
	return t->getWireBinder()->getDir();
}

// Fabricated level: the momentum of either trap, returned by value.
static inline JGeometry::TVec3<f32> WireTrapMomentum(const TWireTrap* trap)
{
	JGeometry::TVec3<f32> momentum = WireTrapBinderDir(trap);
	f32 rate;
	if (trap->mWaterTimer > 0)
		rate = 1.0f + trap->getWaterPow();
	else
		rate = 1.0f;
	momentum *= trap->mMoveDir * rate;
	momentum *= trap->mSpeed;
	return momentum;
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

			// TODO: retail copies each momentum once (temporary to named); the
			// by-value return copies twice. An out-parameter helper changes the
			// inline depths (scale expands, or the helper is refused one level
			// down) and a `const TVec3&` binding expands scale too.
			JGeometry::TVec3<f32> myMomentum  = WireTrapMomentum(this);
			JGeometry::TVec3<f32> hisMomentum = WireTrapMomentum(trap);

			behaveHitWireTrap(trap, myMomentum, hisMomentum);
			trap->behaveHitWireTrap(this, hisMomentum, myMomentum);
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

// 0xc in the map -- `lwz 0x88; addi 8; blr` -- so this forwards
// TWireBinder::getDir()'s reference rather than copying the vector out.
const JGeometry::TVec3<f32>& TWireTrap::getWireDir() const
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
BOOL TWireTrap::isStartWire() const
{
	return getWireBinder()->isStartWire(mPosition, mMoveDir);
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
