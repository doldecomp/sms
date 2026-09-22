#include <Enemy/LimitKoopaJr.hpp>
#include <Enemy/Enemy.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// Only the damage, shoot and yahoo slots carry an ambient sound; the .bck at
// index 2 (run) has none.
static const char* koopajr_bastable[] = {
	"/scene/koopajr/bas/koopajr_damage.bas",
	"/scene/koopajr/bas/koopajr_shoot.bas",
	nullptr,
	"/scene/koopajr/bas/koopajr_yahoo.bas",
};

TLimitKoopaJrParams::TLimitKoopaJrParams(const char* prm)
    : TSpineEnemyParams(prm)
    , PARAM_INIT(mSLAcceleration, 1.0f)
    , PARAM_INIT(mSLRotationSpeed, 1.0f)
    , PARAM_INIT(mSLSpeedMax, 8.0f)
    , PARAM_INIT(mSLRoundAngleVelocity, 0.05f)
    , PARAM_INIT(mSLRoundRadius, 2000.0f)
    , PARAM_INIT(mSLRoundHeight, 0.0f)
    , PARAM_INIT(mSLDamageRadius, 1000.0f)
    , PARAM_INIT(mSLDamageHeight, 4000.0f)
    , PARAM_INIT(mSLKoopaJrScale, 1.6f)
    , PARAM_INIT(mSLShotDoodlePeriod, 1200)
    , PARAM_INIT(mSLDamagePeriod, 360)
{
	TParams::load(mPrmPath);

	// The eleven overrides land in .sdata because TParamRT<T>::set takes a
	// reference, so the literals need an address.
	mSLAcceleration.set(1.0f);
	mSLRotationSpeed.set(1.0f);
	mSLSpeedMax.set(15.0f);
	mSLRoundAngleVelocity.set(0.08f);
	mSLRoundRadius.set(3300.0f);
	mSLRoundHeight.set(5400.0f);
	mSLDamageRadius.set(100.0f);
	mSLDamageHeight.set(300.0f);
	mSLKoopaJrScale.set(2.0f);
	mSLDamagePeriod.set(120);
	mSLShotDoodlePeriod.set(600);
}

TLimitKoopaJr::TLimitKoopaJr(const char* name)
    : TSpineEnemy(name)
    , mBathtub(nullptr)
{
	onLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_UNK100);
}

// The shape loop as its own level keeps the loop counter in the callee-saved
// register that already holds the zero of resetLimitKoopaJr's stores; the raw
// damage params (no reference temporaries) then land the frame.
static inline void LimitKoopaJrShowShapes(J3DModelData* modelData)
{
	for (u16 i = 0; i < modelData->getShapeNum(); ++i)
		modelData->getShapeNodePointer(i)->onFlag(1);
}

void TLimitKoopaJr::init(TLiveManager* live_manager)
{
	mManager = live_manager;
	mManager->manageActor(this);

	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("koopajr_model.bmd", 0);
	mMActor->setLightType(1);

	initAnmSound();

	f32 damageHeight = getSaveParams()->mSLDamageHeight.value;
	initHitActor(0x0800002E, 1, 0, 0.0f, 0.0f,
	             getSaveParams()->mSLDamageRadius.value, damageHeight);
	offHitFlag(HIT_FLAG_NO_COLLISION);

	mSpine->initWith(&TNerveLimitKoopaJrRun::theNerve());

	f32 scale = getSaveParams()->mSLKoopaJrScale.get();
	mScaling.set(scale, scale, scale);

	resetLimitKoopaJr();

	LimitKoopaJrShowShapes(getModel()->getModelData());
}

void TLimitKoopaJr::reset()
{
	TSpineEnemy::reset();
	resetLimitKoopaJr();
}

static inline int LimitKoopaJrShotPeriod(const TLimitKoopaJr* koopa)
{
	TLimitKoopaJrParams* params = koopa->getSaveParams();
	int period                  = params->mSLShotDoodlePeriod.get();
	return period;
}

void TLimitKoopaJr::resetLimitKoopaJr()
{
	mSpine->reset();

	unk158     = 0;
	mShotTimer = 0;
	mShotTimer = LimitKoopaJrShotPeriod(this);

	unk160.x = 0.0f;
	unk160.y = 0.0f;
	unk160.z = 0.0f;
	unk16C   = 1.0f;

	mBodyDirection.mDirection  = 0.0f;
	mRoundDirection.mDirection = 0.0f;
}

void TLimitKoopaJr::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mKoopa == nullptr)
		mKoopa = ((TEnemyManager*)JDrama::TNameRefGen::search<TEnemyManager>(
		             "クッパマネージャー"))
		             ->getObj(0);

	if (mBathtub == nullptr)
		mBathtub = JDrama::TNameRefGen::search<THitActor>("バスタブ");

	if (cue & 1) {
		updateTimers();
		checkNerve();
	}

	TSpineEnemy::perform(cue, graphics);
}

void TLimitKoopaJr::bind() { }

void TLimitKoopaJr::calcRootMatrix()
{
	JGeometry::TQuat4<f32> quat;
	quat.setEulerY(mBodyDirection.mDirection);

	// setQT is the level that keeps TRotation3::setQuat the ROM's bl; spelled
	// as setQuat + setTrans it expands in place.
	TPosition3f mtx;
	mtx.setQT(quat, mPosition);

	MtxPtr src = (MtxPtr)mtx;
	getModel()->setBaseTRMtx(src);

	getModel()->setBaseScale(mScaling);
}

// TODO: incorrect size. Map records 44 bytes and this is 60; the body is a
// guess -- mKoopa is the only thing a "start message" would forward to, and
// the ROM has no call site left to read the argument's meaning from.
void TLimitKoopaJr::startKoopaJrMessage(u32 message)
{
	mKoopa->receiveMessage(this, message);
}

void TLimitKoopaJr::emitKoopaJrEffects() { }

void TLimitKoopaJr::setAnimationIndex(int index)
{
	getMActor()->setBckFromIndex(index);

	const char** table = getBasNameTable();
	setAnmSound(table == nullptr ? nullptr : table[index]);
}

// TODO: incorrect size. Map records 52 bytes, ours 44: the ROM materialises
// &unk158 and &mShotTimer and stores through the pointer while still loading
// the value at a displacement off `this` (visible in perform, 97.9%). Neither
// a static helper taking int& nor one taking int* reproduces it -- MWCC folds
// the address back into the store and, worse, emits a symbol the map lacks.
// TODO: the map's updateTimers is 0x34 = 13 instructions, i.e. six per timer
// plus the blr, and `perform`'s expansion materialises each timer's address
// (`addi r4, this, 0x158`) before loading it. The same six-per-timer shape
// holds for TKoopaJr (0x4c, three timers), TTinKoopa (0x4c) and
// TKoopaJrSubmarine (0x1c), so the level is shared. Refuted: a TU-local
// `static inline decreaseTimer(int*)` and, structural pass IX, the same helper
// with an `int&` parameter -- MWCC folds `&member` back into a direct member
// access either way and the body stays 0x2c. Note retail's *load* keeps the
// direct `0x158(this)` form and only the store goes through the bound
// address, so whatever creates it is not a plain reference binding.
void TLimitKoopaJr::updateTimers()
{
	if (unk158 > 0)
		unk158 -= 1;

	if (mShotTimer > 0)
		mShotTimer -= 1;
}

const char** TLimitKoopaJr::getBasNameTable() const { return koopajr_bastable; }

BOOL TLimitKoopaJr::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER)
		return TRUE;

	return FALSE;
}

// The second theNerve() is only evaluated for its static-init side effect:
// emitKoopaJrEffects() is empty, so MWCC keeps the registration but drops the
// comparison and the branch.
void TLimitKoopaJr::checkNerve()
{
	if (mSpine->getCurrentNerve() != &TNerveLimitKoopaJrWait::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveLimitKoopaJrRun::theNerve())
		emitKoopaJrEffects();
}

// TODO: 77.7%. The arithmetic and call order match; what is left is the frame
// (0x118 vs 0xf0), the batched vs interleaved stores of the new position, and
// cross()'s store order: retail stores forward.x and forward.y, reloads
// offset.x, then computes and stores forward.z. That is not JGVec3.hpp's to
// fix -- all three alternative shapes were measured there and each regresses
// a dozen other call sites, so the order has to come from this site.
// fabricated
static inline void LimitKoopaJrNormalize(JGeometry::TVec3<f32>* v)
{
	v->normalize();
}

void TLimitKoopaJr::moveRun()
{
	f32 angleVelocity
	    = 0.017453294f * getSaveParams()->mSLRoundAngleVelocity.get();

	TDirectionCalc target = calcTargetDirection();

	TDirectionCalc next(
	    mRoundDirection.calcTurnDirection(target.get(), angleVelocity));
	f32 turn                   = next.sub(mRoundDirection.mDirection);
	mRoundDirection.mDirection = next.get();

	mRoundRadius = getSaveParams()->mSLRoundRadius.get();

	JGeometry::TVec3<f32> offset = mRoundDirection.calcDirectionVector();
	offset.scale(mRoundRadius);

	const JGeometry::TVec3<f32>& center = mBathtub->mPosition;
	f32 x                               = center.x + offset.x;
	f32 y                               = center.y + offset.y;
	f32 z                               = center.z + offset.z;
	mPosition.set(x, y, z);
	mPosition.y = getSaveParams()->mSLRoundHeight.get();

	offset.normalize();

	JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
	JGeometry::TVec3<f32> forward;
	forward.cross(up, offset);
	forward.normalize();

	if (turn < 0.0f) {
		forward.x = -forward.x;
		forward.y = -forward.y;
		forward.z = -forward.z;
	}

	// Through the same forwarder as moveWait: at this depth it keeps
	// setLength expanded but pushes its `squared()`'s dot to 5 and its
	// scale(f, const&) to 4, which is the pair of `bl`s the ROM has here.
	// The two normalize() calls above expand all the way for retail too.
	JGeometry::TVec3<f32> dir(forward);
	LimitKoopaJrNormalize(&dir);

	TDirectionCalc bodyTarget(dir);
	mBodyDirection.mDirection = mBodyDirection.calcTurnDirection(
	    bodyTarget.get(),
	    mBodyDirection.d2r(getSaveParams()->mSLRotationSpeed.get()));
}

bool TLimitKoopaJr::canRun()
{
	if (mRoundDirection.absDirection(calcTargetDirection().get())
	    <= 0.62831855f)
		return false;

	return true;
}

bool TLimitKoopaJr::canYahoo() { return SMS_IsMarioStatusTypeJumping(); }

// The ROM calls TVec3::setLength(const TVec3&, f32) out of line here, which
// puts it at inline depth 4 -- one level below what `dir.normalize()` reaches
// on its own (normalize 2, setLength 3, where the four-statement body still
// fits the depth-3 budget). The forwarder below supplies that level: the Wait
// nerve goes 90.0 -> 95.8 and this body drops from 336 bytes to 292 against
// the map's 288. Retail's own name for the level is unrecoverable; a
// normalize step folded into a direction helper is the likely shape.
void TLimitKoopaJr::moveWait()
{
	JGeometry::TVec3<f32> toMario;
	toMario.x = gpMarioPos->x - mPosition.x;
	toMario.y = gpMarioPos->y - mPosition.y;
	toMario.z = gpMarioPos->z - mPosition.z;
	toMario.y = 0.0f;

	JGeometry::TVec3<f32> dir = toMario;
	LimitKoopaJrNormalize(&dir);

	TDirectionCalc target(dir);
	mBodyDirection.mDirection = mBodyDirection.calcTurnDirection(
	    target.get(),
	    mBodyDirection.d2r(getSaveParams()->mSLRotationSpeed.get()));
}

// TODO: UNUSED, 0x9c in the map and 0xac here. Returning `f32` (the
// direction, `return calc.get();`, callers reading it straight) lands the map
// size exactly and cuts the Run nerve's frame from 0x68 to 0x50 (retail 0x60),
// but costs TNerveLimitKoopaJrWait 0.01 (frame 0xb8 -> 0xa0 against 0xb0), so
// it is parked until the Wait nerve's moveWait paste is understood.
TDirectionCalc TLimitKoopaJr::calcTargetDirection()
{
	TDirectionCalc calc;

	const JGeometry::TVec3<f32>& bathtubPos = mBathtub->mPosition;
	JGeometry::TVec3<f32> toMario;
	toMario.x = bathtubPos.x - gpMarioPos->x;
	toMario.y = bathtubPos.y - gpMarioPos->y;
	toMario.z = bathtubPos.z - gpMarioPos->z;
	toMario.y = 0.0f;
	calc.makeDirection(toMario);

	return calc;
}

// TODO: incorrect size. Map records 256 bytes and this is 164. Nothing calls
// it -- calcTargetDirection spells the same block out against mBathtub, which
// is what the three live sites show -- so the extra 92 bytes are unexplained;
// a setLength/normalize step on the result would be the obvious candidate.
TDirectionCalc TLimitKoopaJr::makeDirection(JGeometry::TVec3<f32> point)
{
	TDirectionCalc calc;

	JGeometry::TVec3<f32> toMario;
	toMario.x = point.x - gpMarioPos->x;
	toMario.y = point.y - gpMarioPos->y;
	toMario.z = point.z - gpMarioPos->z;
	toMario.y = 0.0f;
	calc.makeDirection(toMario);

	return calc;
}

static inline TLimitKoopaJr* LimitKoopaJrGetBody(TSpineBase<TLiveActor>* spine)
{
	TLimitKoopaJr* body = (TLimitKoopaJr*)spine->getBody();
	return body;
}

DEFINE_NERVE(TNerveLimitKoopaJrRun, TLiveActor)
{
	TLimitKoopaJr* koopaJr = (TLimitKoopaJr*)spine->getBody();

	if (spine->getTime() == 0)
		koopaJr->setAnimationIndex(TLimitKoopaJr::LIMITKOOPAJR_ANM_RUN);

	if (!koopaJr->canRun()) {
		spine->pushAfterCurrent(&TNerveLimitKoopaJrWait::theNerve());
		return TRUE;
	}

	koopaJr->moveRun();
	return FALSE;
}

DEFINE_NERVE(TNerveLimitKoopaJrWait, TLiveActor)
{
	TLimitKoopaJr* koopaJr = (TLimitKoopaJr*)spine->getBody();

	if (spine->getTime() == 0)
		koopaJr->setAnimationIndex(TLimitKoopaJr::LIMITKOOPAJR_ANM_RUN);

	if (koopaJr->canRun()) {
		spine->pushAfterCurrent(&TNerveLimitKoopaJrRun::theNerve());
		return TRUE;
	}

	if (koopaJr->canYahoo()) {
		spine->pushAfterCurrent(&TNerveLimitKoopaJrYahoo::theNerve());
		return TRUE;
	}

	if (koopaJr->mShotTimer <= 0) {
		spine->pushAfterCurrent(&TNerveLimitKoopaJrLaunch::theNerve());
		return TRUE;
	}

	koopaJr->moveWait();
	return FALSE;
}

DEFINE_NERVE(TNerveLimitKoopaJrLaunch, TLiveActor)
{
	TLimitKoopaJr* koopaJr = (TLimitKoopaJr*)spine->getBody();

	if (spine->getTime() == 0) {
		koopaJr->setAnimationIndex(TLimitKoopaJr::LIMITKOOPAJR_ANM_DAMAGE);
		koopaJr->mShotTimer
		    = koopaJr->getSaveParams()->mSLShotDoodlePeriod.get();
	}

	if (koopaJr->getMActor()->curAnmEndsNext(0, nullptr)) {
		spine->pushAfterCurrent(&TNerveLimitKoopaJrWait::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveLimitKoopaJrYahoo, TLiveActor)
{
	TLimitKoopaJr* koopaJr = LimitKoopaJrGetBody(spine);

	if (spine->getTime() == 0)
		koopaJr->setAnimationIndex(TLimitKoopaJr::LIMITKOOPAJR_ANM_YAHOO);

	if (koopaJr->getMActor()->curAnmEndsNext(0, nullptr)) {
		spine->pushAfterCurrent(&TNerveLimitKoopaJrWait::theNerve());
		return TRUE;
	}

	return FALSE;
}

TLimitKoopaJrManager::TLimitKoopaJrManager(const char* name)
    : TEnemyManager(name)
{
}

void TLimitKoopaJrManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "koopajr_model.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_MaterialTexGenFull
		      | J3DMLF_UseUniqueMaterials | (4 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

void TLimitKoopaJrManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk38 = new TLimitKoopaJrParams("/enemy/limitkoopajr.prm");
}

void TLimitKoopaJrManager::loadAfter() { }

TSpineEnemy* TLimitKoopaJrManager::createEnemyInstance() { return nullptr; }
