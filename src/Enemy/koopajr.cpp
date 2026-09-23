#include <Enemy/KoopaJr.hpp>
#include <Enemy/Koopa.hpp>
#include <Enemy/BathtubKiller.hpp>
#include <Enemy/BathtubBinder.hpp>
#include <Enemy/Enemy.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <Player/WaterGun.hpp>
#include <Player/NozzleBase.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <System/Particles.hpp>
#include <System/FlagManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
// rogue includes needed for matching the string pool, sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <math.h>
#include <stdlib.h>

static const char* koopajr_bastable[] = {
	"/scene/koopajr/bas/koopajr_damage.bas",
	"/scene/koopajr/bas/koopajr_shoot.bas",
	nullptr,
	"/scene/koopajr/bas/koopajr_yahoo.bas",
};

static const char* koopajrsubmarine_bastable[] = { nullptr };

// The submarine's joints: the KoopaJr seat and the four killer launchers.
static const char* TKoopaJr_jointNameTable[] = {
	"KoopaJr_null",   "killer_null00", "killer_null03",
	"killer_null01", "killer_null04",
};
static int TKoopaJr_jointIndexTable[5];

#define TWO_PI 6.2831855f

// Fabricated, but the ROM's shape: every wrap in this file computes
// `l + mod((r - l) + (t - l), r - l)`, keeping the `t - l` subtraction and
// the `l +` that a literal zero bound would let MWCC fold. Two levels are
// needed rather than one because JGeometry::TUtil<f32>::mod and std::fmodf
// are calls at every ROM site here, which only happens at depth four.
// The wrapped value is named at both levels: each named result is +8 of low
// region in every caller that expands the pair, which is what lands
// TDirectionCalc::absDirection (0x40) and TDirectionCalc::sub (0x38).
static inline f32 WrapDirection(f32 t, f32 l, f32 r)
{
	f32 wrapped = l + JGeometry::TUtil<f32>::mod((r - l) + (t - l), r - l);
	return wrapped;
}

static inline f32 WrapRadian(f32 t)
{
	f32 wrapped = WrapDirection(t, 0.0f, TWO_PI);
	return wrapped;
}

// The same pair over std::fmodf: calcNearerDirection and calcTurnDirection
// wrap with it where normalize() uses TUtil<f32>::mod, so the two families
// cannot share one helper.
static inline f32 WrapDirectionF(f32 t, f32 l, f32 r)
{
	return l + std::fmodf((r - l) + (t - l), r - l);
}

static inline f32 WrapRadianF(f32 t)
{
	return WrapDirectionF(t, 0.0f, TWO_PI);
}

// ---------------------------------------------------------------------------
// TDirectionCalc
// ---------------------------------------------------------------------------

TDirectionCalc::TDirectionCalc() { mDirection = 0.0f; }

TDirectionCalc::TDirectionCalc(f32 direction) { mDirection = direction; }

TDirectionCalc::TDirectionCalc(JGeometry::TVec3<f32> dir)
{
	// The ROM's callers (makeRelativeAngle, checkNerve, TKoopaJr::checkNerve)
	// copy the vector once, which is a default-constructed TDirectionCalc
	// plus makeDirection(); this constructor would copy it twice.
	makeDirection(dir);
}

// UNUSED, 0x4c in the map. Wraps the stored direction into [0, 2pi).
void TDirectionCalc::normalize() { mDirection = WrapRadian(mDirection); }

// Brings dir within pi of the stored direction, so that a turn towards it
// takes the shorter way round.
f32 TDirectionCalc::calcNearerDirection(f32 dir)
{
	// The wrap is written out rather than routed through WrapRadianF: the five
	// statements are what keep this function out of line at
	// TKoopaJrSubmarine::makeRelativeAngle's two call sites (the ROM `bl`s it
	// there, and WrapRadianF's single statement drops it under the depth-1
	// budget: makeRelativeAngle 95.02% -> 26.09%). The cost is that std::fmodf
	// expands here instead of being called (97.33%); the missing level above it
	// is still unidentified. With WrapRadianF this body is 99.8%, but none of
	// named or split diff/other locals, per-branch returns or a named wrapped
	// value lifts it back over the budget (makeRelativeAngle stays ~30%).
	// The body itself closes (100%, frame 0x38) with WrapRadianF plus one
	// named helper result (`f32 wrapped = ...; return wrapped;` in either
	// helper, or `f32 range/offset` named inside WrapDirectionF), but the
	// auto-inline into makeRelativeAngle counts only this body's own
	// statements: extra statements inside the helpers leave it inlined (~30%).
	f32 lo     = 0.0f;
	f32 hi     = TWO_PI;
	f32 range  = hi - lo;
	f32 offset = mDirection - lo;
	mDirection = lo + std::fmodf(range + offset, range);

	if (dir >= mDirection) {
		f32 diff  = dir - mDirection;
		f32 other = TWO_PI - diff;
		if (other < diff)
			dir -= TWO_PI;
	} else {
		f32 diff  = mDirection - dir;
		f32 other = TWO_PI - diff;
		if (other < diff)
			dir += TWO_PI;
	}
	return dir;
}

// TODO: the nearer-direction adjustment is written out here, in
// absDirection and in calcNearerDirection. calcNearerDirection wraps with
// std::fmodf, the others with TUtil::mod, so they cannot share one helper.
f32 TDirectionCalc::sub(f32 dir)
{
	normalize();
	if (dir >= mDirection) {
		f32 diff = dir - mDirection;
		if (TWO_PI - diff < diff)
			dir -= TWO_PI;
	} else {
		f32 diff = mDirection - dir;
		if (TWO_PI - diff < diff)
			dir += TWO_PI;
	}
	return mDirection - dir;
}

// Returns the stored direction turned towards dir by at most step.
f32 TDirectionCalc::calcTurnDirection(f32 dir, f32 step)
{
	// TODO: 98.2%; the frame is 8 short and the wrapped direction lands in
	// f2 where the ROM has f3.
	mDirection = WrapRadianF(mDirection);
	normalize();
	if (dir >= mDirection) {
		f32 diff = dir - mDirection;
		if (TWO_PI - diff < diff)
			dir -= TWO_PI;
	} else {
		f32 diff = mDirection - dir;
		if (TWO_PI - diff < diff)
			dir += TWO_PI;
	}

	f32 turn = step;
	if (dir > mDirection) {
		f32 diff = dir - mDirection;
		if (diff < step)
			turn = diff;
		return mDirection + turn;
	} else {
		f32 diff = mDirection - dir;
		if (diff < step)
			turn = diff;
		return mDirection - turn;
	}
}

void TDirectionCalc::makeDirection(JGeometry::TVec3<f32> dir)
{
	// TODO: the two locals only exist to load z before x, as the original
	// does; atan2f(dir.x, dir.z) loads them the other way round.
	JGeometry::TVec3<f32>* pDir = &dir;
	f32 z                       = pDir->z;
	f32 x                       = pDir->x;
	mDirection                  = atan2f(x, z);
}

// The by-value read of the stored direction: the fork puts sinf's argument
// in f0 and moves it into f1, which is the ROM's order, and the named local
// inside it is +8 of low region per expansion.
static inline f32 KoopajrDirectionOf(const TDirectionCalc* p)
{
	f32 direction = p->mDirection;
	return direction;
}

JGeometry::TVec3<f32> TDirectionCalc::calcDirectionVector()
{
	return JGeometry::TVec3<f32>(sinf(KoopajrDirectionOf(this)), 0.0f,
	                             cosf(KoopajrDirectionOf(this)));
}

f32 TDirectionCalc::absDirection(f32 dir)
{
	// The named result puts sub() at depth 1, where the original expands it;
	// fabsf(sub(dir)) nests it one level deeper and leaves a bl.
	f32 diff   = sub(dir);
	f32 result = fabsf(diff);
	return result;
}

f32 TDirectionCalc::d2r(f32 deg)
{
	// TUtil<f32>::PI(), not the literal: an inlined call returning the
	// constant keeps the parameter as the multiply's first operand.
	return deg * JGeometry::TUtil<f32>::PI() / 180.0f;
}

f32 TDirectionCalc::r2d(f32 rad)
{
	return 180.0f * rad / JGeometry::TUtil<f32>::PI();
}

// ---------------------------------------------------------------------------
// Params
// ---------------------------------------------------------------------------

// The .prm defaults are loaded and then overwritten with tuned values.
TKoopaJrParams::TKoopaJrParams(const char* prm)
    : TSpineEnemyParams(prm)
    , PARAM_INIT(mSLLaunchKillerLimit, 10000.0f)
    , PARAM_INIT(mSLDamageRadius, 1000.0f)
    , PARAM_INIT(mSLDamageHeight, 4000.0f)
    , PARAM_INIT(mSLKoopaJrScale, 1.6f)
    , PARAM_INIT(mSLFastLaunchDistance, 10000.0f)
    , PARAM_INIT(mSLDamagePeriod, 360)
    , PARAM_INIT(mSLLaunchKillerPeriod, 1200)
    , PARAM_INIT(mSLLaunchKillerPeriodFast, 360)
{
	TParams::load(mPrmPath);
	mSLLaunchKillerLimit.set(4200.0f);
	mSLDamageRadius.set(100.0f);
	mSLDamageHeight.set(300.0f);
	mSLKoopaJrScale.set(2.0f);
	mSLFastLaunchDistance.set(4600.0f);
	mSLDamagePeriod.set(240);
	mSLLaunchKillerPeriodFast.set(360);
	mSLLaunchKillerPeriod.set(840);
}

// The angle overrides are multiples of pi, folded at float precision into
// .sdata2 (not bound as .sdata temporaries like the plain literals), and each
// site costs 8 bytes of frame: 0x70 needs this one-level wrapper (a bare
// `PI() * k` is 0x50, a folded `M_PI * k` lands in .sdata). The 0.18 value
// (0x3f10c3bd) rules out a double-precision pi.
static inline f32 KoopajrPiTimes(f32 k)
{
	return k * JGeometry::TUtil<f32>::PI();
}

TKoopaJrSubmarineParams::TKoopaJrSubmarineParams(const char* prm)
    : TSpineEnemyParams(prm)
    , PARAM_INIT(killerTargetDistanceMin, 500.0f)
    , PARAM_INIT(killerTargetDistance, 500.0f)
    , PARAM_INIT(bottomHeight, 0.0f)
    , PARAM_INIT(centerZ, 0.0f)
    , PARAM_INIT(aboidKoopaFlameAngle, 0.31415927f)
    , PARAM_INIT(traceMarioAngle, 0.31415927f)
    , PARAM_INIT(mSLWavePhaseVelocity, 0.31415927f)
    , PARAM_INIT(mSLWaveAmplitudeMin, 0.37699112f)
    , PARAM_INIT(mSLWaveAmplitudeMaxLaunch, 0.37699112f)
    , PARAM_INIT(mSLWaveAmplitudeMax, 0.37699112f)
    , PARAM_INIT(mSLSwingPhaseVelocity, 0.31415927f)
    , PARAM_INIT(mSLSwingAmplitudeMin, 0.37699112f)
    , PARAM_INIT(mSLSwingAmplitudeMax, 0.37699112f)
    , PARAM_INIT(mSLRoundAngleVelocity, 0.05f)
    , PARAM_INIT(mSLRoundDistance, 1.0f)
    , PARAM_INIT(mSLAcceleration, 1.0f)
    , PARAM_INIT(mSLRotationSpeed, 1.0f)
    , PARAM_INIT(mSLSpeedMax, 8.0f)
    , PARAM_INIT(mSLKoopaJrSubmarineScale, 1.6f)
    , PARAM_INIT(mSLDamageRadius, 1000.0f)
    , PARAM_INIT(mSLDamageHeight, 4000.0f)
    , PARAM_INIT(shineKillerProbability0, 0.0f)
    , PARAM_INIT(shineKillerProbability1, 0.0f)
    , PARAM_INIT(mSLKillerIntervalFast, 30)
    , PARAM_INIT(mSLKillerInterval, 30)
{
	TParams::load(mPrmPath);
	killerTargetDistanceMin.set(500.0f);
	killerTargetDistance.set(700.0f);
	bottomHeight.set(0.0f);
	centerZ.set(200.0f);
	aboidKoopaFlameAngle.set(KoopajrPiTimes(0.2f));
	traceMarioAngle.set(KoopajrPiTimes(0.1f));
	mSLWavePhaseVelocity.set(KoopajrPiTimes(0.03f));
	mSLWaveAmplitudeMin.set(KoopajrPiTimes(0.04f));
	mSLWaveAmplitudeMaxLaunch.set(KoopajrPiTimes(0.12f));
	mSLWaveAmplitudeMax.set(KoopajrPiTimes(0.1f));
	mSLSwingPhaseVelocity.set(KoopajrPiTimes(0.06f));
	mSLSwingAmplitudeMin.set(KoopajrPiTimes(0.04f));
	mSLSwingAmplitudeMax.set(KoopajrPiTimes(0.18f));
	mSLRoundAngleVelocity.set(0.12f);
	mSLRoundDistance.set(2000.0f);
	mSLAcceleration.set(1.0f);
	mSLRotationSpeed.set(1.0f);
	mSLSpeedMax.set(5.0f);
	mSLKoopaJrSubmarineScale.set(2.0f);
	mSLDamageRadius.set(240.0f);
	mSLDamageHeight.set(120.0f);
	shineKillerProbability0.set(0.5f);
	shineKillerProbability1.set(0.125f);
	mSLKillerInterval.set(90);
	mSLKillerIntervalFast.set(30);
}

// ---------------------------------------------------------------------------
// TCallbackHitActor
// ---------------------------------------------------------------------------

// UNUSED, 0x18 in the map.
static int TKoopaJr_getJointIndex(int i) { return TKoopaJr_jointIndexTable[i]; }

// UNUSED, 0x12c in the map: inlined twice into TKoopaJrSubmarine::init.
TCallbackHitActor::TCallbackHitActor(const char* name, u32 actor_type,
                                     f32 radius, f32 height, THitActor* owner)
    : THitActor(name)
{
	mOwner = owner;
	initHitActor(actor_type, 0, 0, 0.0f, 0.0f, radius, height);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
}

BOOL TCallbackHitActor::receiveMessage(THitActor* sender, u32 message)
{
	return mOwner->receiveMessage(sender, message);
}

// ---------------------------------------------------------------------------
// TKoopaJr
// ---------------------------------------------------------------------------

TKoopaJr::TKoopaJr(const char* name)
    : TSpineEnemy(name)
{
	mBathtub          = nullptr;
	mKoopa            = nullptr;
	mSubmarine        = nullptr;
	mSubmarineManager = nullptr;
	mKillerManager    = nullptr;
	onLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_UNK100);
}

void TKoopaJr::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("koopajr_model.bmd", 0);
	mMActor->setLightType(1);
	initAnmSound();
	f32 height = getSaveParams()->mSLDamageHeight.get();
	f32 radius = getSaveParams()->mSLDamageRadius.get();
	initHitActor(0x08000028, 1, 0, 0.0f, 0.0f, radius, height);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mSpine->initWith(&TNerveKoopaJrWait::theNerve());
	mKillerManager = JDrama::TNameRefGen::search<TEnemyManager>(
	    "バスタブキラーマネージャー");
	// Two discarded `getActiveObjNum()` calls, as in
	// TKoopaJrSubmarineManager::load/loadAfter and
	// TBathtubKillerManager::load: the ROM reads the manager's params
	// pointer and compares it against null with no branch and no use of the
	// result, which is all that survives of the inline's leading `if
	// (!unk38)` guard once the value is thrown away.
	mKillerManager->getActiveObjNum();
	if (mSubmarineManager == nullptr)
		mSubmarineManager = JDrama::TNameRefGen::search<TEnemyManager>(
		    "クッパジュニアサブマリンマネージャー");
	mSubmarineManager->getActiveObjNum();
	// The scale is read straight off getSaveParam(): the getSaveParams()
	// wrapper is one inline level worth +8 of low region here, and the ROM's
	// frame (0xe0) has room for only two of this function's three reads.
	f32 scale = ((TKoopaJrParams*)getSaveParam())->mSLKoopaJrScale.get();
	mScaling.set(scale, scale, scale);
	resetKoopaJr();
}

// Binding level over the save-params accessor, worth +8 of low region in
// TKoopaJr::reset (frame ladder 271).
static inline TKoopaJrParams* KoopaJrGetParams(const TKoopaJr* p)
{
	TKoopaJrParams* params = p->getSaveParams();
	return params;
}

void TKoopaJr::reset()
{
	TSpineEnemy::reset();
	resetKoopaJr();
}

// UNUSED, 0x64 in the map.
void TKoopaJr::resetKoopaJr()
{
	mSpine->reset();
	mTimers[KOOPAJR_TIMER_DAMAGE]      = 0;
	mTimers[KOOPAJR_TIMER_LAUNCH]      = 0;
	mTimers[KOOPAJR_TIMER_FAST_LAUNCH] = 0;
	mTimers[KOOPAJR_TIMER_LAUNCH]
	    = KoopaJrGetParams(this)->mSLLaunchKillerPeriod.get();
	mTimers[KOOPAJR_TIMER_FAST_LAUNCH] = 0;
}

// TODO: 99.9%. The timers now match; the frame is 0x18 short of retail's
// 0x150, which shifts every stack slot of the inlined nerve checks.
void TKoopaJr::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mSubmarine == nullptr) {
		mSubmarine = (TKoopaJrSubmarine*)mSubmarineManager->getObj(0);
		mSubmarine->setKoopaJr(this);
	}
	if (mKoopa == nullptr)
		mKoopa = (TKoopa*)JDrama::TNameRefGen::search<TEnemyManager>(
		             "クッパマネージャー")
		             ->getObj(0);
	if (mBathtub == nullptr)
		mBathtub = JDrama::TNameRefGen::search<TBathtub>("バスタブ");

	if (cue & 2)
		checkSubmarineSwing();

	// The ROM tests the demo flag here and again at the top of checkNerve():
	// the expansion keeps checkNerve()'s demo block as unreachable code
	// between a `bne` and a `beq` on the same compare.
	if (cue & 1) {
		updateTimers();
		if (!mBathtub->unk29A)
			checkNerve();
	}

	TSpineEnemy::perform(cue, graphics);
}

void TKoopaJr::calcRootMatrix()
{
	J3DModel* model = getModel();
	if (mBathtub->unk29A) {
		getModel()->setBaseTRMtx(mBathtub->getKoopaJrMtxInDemo());
	} else {
		mSubmarine->getJointTransByIndex(TKoopaJr_getJointIndex(0), &mPosition);
		MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x, mPosition.y,
		               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	}
	model->setBaseScale(getScaling());
}

// UNUSED, 0x2c in the map.
void TKoopaJr::startKoopaJrMessage(u32 message)
{
	// TODO: body unknown; nothing in the TU inlines a 0x2c helper.
	receiveMessage(this, message);
}

// UNUSED, 0x4 in the map.
void TKoopaJr::emitKoopaJrEffects() { }

// UNUSED, 0x70 in the map.
void TKoopaJr::setAnimationIndex(int index)
{
	getMActor()->setBckFromIndex(index);
	const char** table = getBasNameTable();
	setAnmSound(table == nullptr ? nullptr : table[index]);
}

// UNUSED, 0x4c in the map. The timers are an array: MWCC's unrolled indexed
// loop is what materialises each element's address for the store.
void TKoopaJr::updateTimers()
{
	for (int i = 0; i < KOOPAJR_TIMER_NUM; ++i)
		if (mTimers[i] > 0)
			mTimers[i]--;
}

const char** TKoopaJr::getBasNameTable() const { return koopajr_bastable; }

// Binding level over a raw member read, worth +8 of low region in
// TKoopaJr::receiveMessage (batch 127).
static inline MSound* KoopajrGetMSound()
{
	MSound* mSound = gpMSound;
	return mSound;
}

BOOL TKoopaJr::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(0xE7, &sender->mPosition, 0, nullptr);
		KoopajrGetMSound()->startSoundSet(0x6802, &mPosition, 0, 0.0f, 0, 0, 4);
		damageKoopaJr();
		return TRUE;
	}
	return FALSE;
}

// UNUSED, 0x240 in the map.
void TKoopaJr::damageKoopaJr()
{
	mTimers[KOOPAJR_TIMER_DAMAGE] = getSaveParams()->mSLDamagePeriod.get();
	startDamageNerve();
}

// UNUSED, 0x274 in the map.
void TKoopaJr::checkSubmarineSwing()
{
	if (mSubmarine->mSwingAmplitude
	    < 0.5f * mSubmarine->getSaveParams()->mSLSwingAmplitudeMax.get())
		return;
	// The damage is written out rather than through damageKoopaJr(): the ROM
	// expands startDamageNerve() one level shallower here than a call to
	// damageKoopaJr() would put it.
	mTimers[KOOPAJR_TIMER_DAMAGE] = getSaveParams()->mSLDamagePeriod.get();
	startDamageNerve();
}

// UNUSED, 0x230 in the map.
void TKoopaJr::startDamageNerve()
{
	if (mSpine->getCurrentNerve() == &TNerveKoopaJrWait::theNerve())
		mSpine->pushNerve(&TNerveKoopaJrDamage::theNerve());
	if (mSpine->getCurrentNerve() == &TNerveKoopaJrLaunch::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveKoopaJrYahoo::theNerve())
		mSpine->setNext(&TNerveKoopaJrDamage::theNerve());
}

// UNUSED, 0x4a0 in the map.
void TKoopaJr::checkNerve()
{
	if (mBathtub->unk29A) {
		if (mSpine->getCurrentNerve() != &TNerveKoopaJrDemo::theNerve())
			mSpine->pushNerve(&TNerveKoopaJrDemo::theNerve());
	}
	if (mSpine->getCurrentNerve() == &TNerveKoopaJrWait::theNerve()) {
		checkNerveKillerLaunchNormal();
		checkNerveKillerLaunchFast();
		checkNerveKillerHit();
	}
	JGeometry::TVec3<f32> toMario;
	toMario.sub(*gpMarioPos, mPosition);
	toMario.y = 0.0f;
	TDirectionCalc toMarioDir;
	toMarioDir.makeDirection(toMario);
	mRotation.y = TDirectionCalc::r2d(toMarioDir.get());
}

void TKoopaJr::checkNerveKillerLaunchNormal()
{
	if (mTimers[KOOPAJR_TIMER_LAUNCH] > 0)
		return;
	int num = mBathtub->getNumKillerLaunchable();
	if (num == 0)
		return;
	mSubmarine->prepareKillerLaunch(num);
	getSpine()->pushNerve(&TNerveKoopaJrLaunch::theNerve());
	const TNerveBase<TLiveActor>* nerve
	    = &TNerveKoopaJrSubmarineCannonOpenClose::theNerve();
	mSubmarine->mSpine->pushNerve(nerve);
	mSubmarine->setAnimationIndex(0);
}

void TKoopaJr::checkNerveKillerLaunchFast()
{
	if (mTimers[KOOPAJR_TIMER_FAST_LAUNCH] > 0)
		return;
	int num = mBathtub->getNumKillerBurstable();
	if (num == 0)
		return;
	mSubmarine->prepareKillerLaunchFast(num);
	getSpine()->pushNerve(&TNerveKoopaJrLaunch::theNerve());
	const TNerveBase<TLiveActor>* nerve
	    = &TNerveKoopaJrSubmarineCannonOpenClose::theNerve();
	mSubmarine->mSpine->pushNerve(nerve);
	mSubmarine->setAnimationIndex(0);
}

// Cheers when a killer has been sent back at the bathtub. The spine goes
// through getSpine() at every pushNerve site in this family: the raw member
// ranks above the nerve address and swaps the two scratch registers the
// inlined TSpineBase::pushNerve uses.
void TKoopaJr::checkNerveKillerHit()
{
	for (int i = 0; i < mKillerManager->getActiveObjNum(); ++i) {
		if ((s32)((TBathtubKiller*)mKillerManager->getObj(i))->unk21C == 1) {
			getSpine()->pushNerve(&TNerveKoopaJrYahoo::theNerve());
			return;
		}
	}
}

// UNUSED, 0x34 in the map.
f32 TKoopaJr::getBathtubY()
{
	// TODO: body unknown.
	return mBathtub->mPosition.y;
}

DEFINE_NERVE(TNerveKoopaJrWait, TLiveActor)
{
	TKoopaJr* koopaJr = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0) {
		koopaJr->setAnimationIndex(2);
		koopaJr->mTimers[KOOPAJR_TIMER_LAUNCH]
		    = koopaJr->getSaveParams()->mSLLaunchKillerPeriod.get();
		koopaJr->mTimers[KOOPAJR_TIMER_FAST_LAUNCH]
		    = koopaJr->getSaveParams()->mSLLaunchKillerPeriodFast.get();
	}
	return false;
}

DEFINE_NERVE(TNerveKoopaJrDamage, TLiveActor)
{
	TKoopaJr* koopaJr = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0)
		koopaJr->setAnimationIndex(0);
	if (koopaJr->mTimers[KOOPAJR_TIMER_DAMAGE] <= 0
	    && koopaJr->getMActor()->isCurAnmAlreadyEnd(0))
		return true;
	return false;
}

DEFINE_NERVE(TNerveKoopaJrDemo, TLiveActor)
{
	TKoopaJr* koopaJr = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0)
		koopaJr->setAnimationIndex(0);
	return false;
}

DEFINE_NERVE(TNerveKoopaJrLaunch, TLiveActor)
{
	TKoopaJr* koopaJr = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0)
		koopaJr->setAnimationIndex(1);
	if (koopaJr->getMActor()->isCurAnmAlreadyEnd(0))
		return true;
	return false;
}

DEFINE_NERVE(TNerveKoopaJrYahoo, TLiveActor)
{
	TKoopaJr* koopaJr = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0)
		koopaJr->setAnimationIndex(3);
	if (koopaJr->getMActor()->isCurAnmAlreadyEnd(0))
		return true;
	return false;
}

// ---------------------------------------------------------------------------
// TKoopaJrManager
// ---------------------------------------------------------------------------

TKoopaJrManager::TKoopaJrManager(const char* name)
    : TEnemyManager(name)
{
}

void TKoopaJrManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "koopajr_model.bmd", 0x54220000, 0 },
		{ nullptr },
	};
	createModelDataArray(entry);
}

void TKoopaJrManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk38 = new TKoopaJrParams("/enemy/koopajr.prm");
}

void TKoopaJrManager::loadAfter()
{
	static const char* onetimeFilenames[]
	    = { "/scene/koopajr/jpa/ms_koopajr_killer.jpa" };
	for (int i = 0; i < 1; ++i)
		SMS_LoadParticle(onetimeFilenames[i], 0xEF + i);
}

// The KoopaJr is placed by the scene, not created here.
TSpineEnemy* TKoopaJrManager::createEnemyInstance() { return nullptr; }

// ---------------------------------------------------------------------------
// TKoopaJrSubmarine
// ---------------------------------------------------------------------------

TKoopaJrSubmarine::TKoopaJrSubmarine(const char* name)
    : TSpineEnemy(name)
{
	mAnmRate = 0.0f;
	mKoopaJr = nullptr;
	offLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_UNK100);
}

// TODO: the ROM hoists `...bss.0` (TKoopaJr_jointIndexTable) into r30 in the
// prologue and reaches the Wait nerve's dtor chain at +0x50 and the table by
// `stwx`; we name both. Same open class as TTinKoopa::init: MWCC needs 3+ live
// same-section references before it uses the section base, we have 2.
void TKoopaJrSubmarine::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	initAnmSound();
	f32 height = getSaveParams()->mSLDamageHeight.get();
	f32 radius = getSaveParams()->mSLDamageRadius.get();
	initHitActor(0x08000020, 0, 0, 0.0f, 0.0f, radius, height);
	onHitFlag(HIT_FLAG_NO_COLLISION);

	mRearBody = new TCallbackHitActor(
	    "サブマリンリアボディ", 0x0800002D, getSaveParams()->mSLDamageRadius.get(),
	    getSaveParams()->mSLDamageHeight.get(), this);
	mFrontBody = new TCallbackHitActor(
	    "サブマリンフロントボディ", 0x0800002D,
	    getSaveParams()->mSLDamageRadius.get(),
	    getSaveParams()->mSLDamageHeight.get(), this);

	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor = mMActorKeeper->createMActor("LastKoopaJrSubmarine.bmd", 0);
	mMActor->setLightType(1);
	mSpine->initWith(&TNerveKoopaJrSubmarineWait::theNerve());

	JUTNameTab* jointNames = getModel()->getModelData()->getJointName();
	for (int i = 0; i < 5; ++i)
		TKoopaJr_jointIndexTable[i]
		    = jointNames->getIndex(TKoopaJr_jointNameTable[i]);

	f32 scale = getSaveParams()->mSLKoopaJrSubmarineScale.get();
	mScaling.set(scale, scale, scale);

	mBathtubBinder = new TBathtubBinder();
	mBinder        = mBathtubBinder;
	resetKoopaJrSubmarine();
}

void TKoopaJrSubmarine::reset()
{
	TSpineEnemy::reset();
	resetKoopaJrSubmarine();
}

// Binding level over a raw member read, worth +8 of low region in
// TKoopaJrSubmarine::resetKoopaJrSubmarine (batch 127).
static inline TBathtubBinder* KoopajrBathtubBinder(const TKoopaJrSubmarine* p)
{
	TBathtubBinder* bathtubBinder = p->mBathtubBinder;
	return bathtubBinder;
}

void TKoopaJrSubmarine::resetKoopaJrSubmarine()
{
	mSpine->reset();
	mTimers[KOOPAJR_SUBMARINE_TIMER_KILLER] = 0;
	setAnimationIndex(0);
	mAnmRate     = getMActor()->getFrameCtrl(0)->getRate();
	mKillerIndex = 0;
	mKillerNum   = 0;
	for (int i = 0; i < 8; ++i)
		mKillerTypes[i] = 0;
	unk154                    = 0.0f;
	unk158                    = 0.0f;
	unk15C                    = 0.0f;
	unk160                    = 1.0f;
	mBodyDirection.mDirection = 0.0f;
	mDirection.mDirection     = 0.0f;
	mIsNearTarget             = false;
	mIsDamaged                = false;
	mSwingAmplitude           = 0.0f;
	mSwingPhase               = 0.0f;
	mWaveAmplitude            = 0.0f;
	mWavePhase                = 0.0f;
	f32 bottom = getSaveParams()->bottomHeight.get();
	KoopajrBathtubBinder(this)->init(150.0f, 100.0f, 150.0f, 100.0f, bottom);
}

void TKoopaJrSubmarine::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & 2) {
		checkKillerLaunch();
		makeCollisionPositions();
		moveSwing();
	}
	if (cue & 1) {
		updateTimers();
		checkNerve();
	}
	TSpineEnemy::perform(cue, graphics);
	mRearBody->perform(cue, graphics);
	mFrontBody->perform(cue, graphics);
}

// The rear hit box sits between the two rear launchers, the front one on
// the KoopaJr seat.
// TODO: frame 0x70 against retail's 0x80; every instruction matches. A TVec3
// accumulator reaches 0x78 here but is then inlined into perform(), which
// retail keeps out of line.
void TKoopaJrSubmarine::makeCollisionPositions()
{
	f32 x = 0.0f;
	f32 y = 0.0f;
	f32 z = 0.0f;
	for (int i = 0; i < 2; ++i) {
		MtxPtr mtx = getModel()->getAnmMtx(TKoopaJr_getJointIndex(i + 3));
		x += mtx[0][3];
		y += mtx[1][3];
		z += mtx[2][3];
	}
	x *= 0.5f;
	y *= 0.5f;
	z *= 0.5f;
	JGeometry::TVec3<f32> center(x, y, z);
	mRearBody->mPosition.set(center.x, center.y, center.z);
	getJointTransByIndex(TKoopaJr_getJointIndex(0), &mFrontBody->mPosition);
}

void TKoopaJrSubmarine::moveSwing()
{
	if (mIsDamaged) {
		mIsDamaged = false;
		mSwingAmplitude += 0.06283186f;
		mSwingAmplitude = JGeometry::min(
		    getSaveParams()->mSLSwingAmplitudeMax.get(), mSwingAmplitude);
	}
	mSwingAmplitude -= 0.009424779f;
	mSwingAmplitude = JGeometry::max(
	    getSaveParams()->mSLSwingAmplitudeMin.get(), mSwingAmplitude);
	if (getSwingAmplitude() <= 0.0f)
		mSwingPhase = 0.0f;
	mSwingPhase = WrapRadianF(
	    mSwingPhase + getSaveParams()->mSLSwingPhaseVelocity.get());

	f32 speedRate = mVelocity.length() / getSaveParams()->mSLSpeedMax.get();
	if (getKillerTimer() > 0) {
		mWaveAmplitude += 0.03141593f;
		mWaveAmplitude = JGeometry::min(
		    getSaveParams()->mSLWaveAmplitudeMaxLaunch.get(), mWaveAmplitude);
	}
	if (speedRate > 0.5f) {
		mWaveAmplitude += 0.03141593f;
		mWaveAmplitude = JGeometry::min(
		    getSaveParams()->mSLWaveAmplitudeMax.get(), mWaveAmplitude);
	}
	mWaveAmplitude -= 0.018849557f;
	mWaveAmplitude = JGeometry::max(
	    getSaveParams()->mSLWaveAmplitudeMin.get(), mWaveAmplitude);
	if (getWaveAmplitude() <= 0.0f)
		mWavePhase = 0.0f;
	mWavePhase = WrapRadianF(
	    mWavePhase + getSaveParams()->mSLWavePhaseVelocity.get());
}

// UNUSED, 0x38 in the map.
f32 TKoopaJrSubmarine::getSwingAngle()
{
	return mSwingAmplitude * sinf(mSwingPhase);
}

// UNUSED, 0x38 in the map.
f32 TKoopaJrSubmarine::getWaveAngle()
{
	return mWaveAmplitude * sinf(mWavePhase);
}

void TKoopaJrSubmarine::bind()
{
	JGeometry::TVec3<f32> next(mPosition);
	next.add(mLinearVelocity);
	next.add(mVelocity);
	// `a = b - c` reaches the map's out-of-line TVec3::sub: operator= is one
	// inline level and the difference nested in its argument two more.
	mLinearVelocity = next - mPosition;
	mBathtubBinder->bind(this);
}

void TKoopaJrSubmarine::calcRootMatrix()
{
	if (mKoopaJr->mBathtub->unk29A) {
		getModel()->setBaseTRMtx(mKoopaJr->mBathtub->getSubmarineMtxInDemo());
	} else {
		JGeometry::TQuat4<f32> swing;
		swing.setEulerZ(getSwingAngle());
		JGeometry::TQuat4<f32> wave;
		wave.setEulerX(getWaveAngle());
		JGeometry::TQuat4<f32> yaw;
		yaw.setEulerY(mBodyDirection.get());

		JGeometry::TQuat4<f32> q;
		q.mul(yaw, swing);
		// TODO: 98.5%. Since the no-locals TQuat4::mul(a, b) both products
		// load in retail's order; left are the frame (0x1c8 against 0x1e0:
		// 4 more above q, 0x14 more below mtx) and the ROM loading
		// mWavePhase before saving the first cosf. Inert or worse
		// (2026-09-23): a second quaternion for the product (frame exact,
		// 88.5), named angles, one-argument mul(wave), q = yaw then
		// mul(swing), every declaration order of the three quaternions,
		// raw centerZ.
		q.mul(q, wave);

		JGeometry::TVec3<f32> center(0.0f, 0.0f,
		                             getSaveParams()->centerZ.get());
		TPosition3f offset;
		offset.translation(center);

		JGeometry::TVec3<f32> trans;
		trans = center;
		trans.negate();
		trans.add(getPosition());

		TPosition3f mtx;
		mtx.setQT(q, trans);
		mtx.concat(offset);
		getModel()->setBaseTRMtx(mtx);
	}
	getModel()->setBaseScale(getScaling());
}

BOOL TKoopaJrSubmarine::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(0xE7, &sender->mPosition, 0, nullptr);
		gpMSound->startSoundSet(0x6802, &mPosition, 0, 0.0f, 0, 0, 4);
		damageKoopaJrSubmarine();
		return TRUE;
	}
	return FALSE;
}

// UNUSED, 0xc in the map.
void TKoopaJrSubmarine::damageKoopaJrSubmarine() { mIsDamaged = true; }

// UNUSED, 0x70 in the map.
void TKoopaJrSubmarine::setAnimationIndex(int index)
{
	getMActor()->setBckFromIndex(index);
	const char** table = getBasNameTable();
	setAnmSound(table == nullptr ? nullptr : table[index]);
}

// UNUSED, 0x80 in the map.
void TKoopaJrSubmarine::prepareKillerLaunch(int num)
{
	if (num > 8)
		num = 8;
	mKillerIndex = 0;
	mKillerNum   = num;
	for (int i = 0; i < mKillerNum; ++i)
		mKillerTypes[i] = 0;
	if (appearShineKiller(mKillerNum))
		mKillerTypes[mKillerNum - 1] = 1;
}

// UNUSED, 0x84 in the map.
void TKoopaJrSubmarine::prepareKillerLaunchFast(int num)
{
	if (num > 8)
		num = 8;
	mKillerIndex = 0;
	mKillerNum   = num;
	for (int i = 0; i < mKillerNum; ++i)
		mKillerTypes[i] = 2;
	if (appearShineKiller(mKillerNum))
		mKillerTypes[mKillerNum - 1] = 1;
}

// Binding level worth +8 of low region, landing
// TKoopaJrSubmarine::appearShineKiller's frame at 0x70 (batch 121).
static inline TKoopaJrSubmarineParams*
KoopajrGetSaveParams(const TKoopaJrSubmarine* p)
{
	TKoopaJrSubmarineParams* saveParams = p->getSaveParams();
	return saveParams;
}

// Rolls for a shine killer; the odds grow as Mario's tank runs dry.
bool TKoopaJrSubmarine::appearShineKiller(int)
{
	f32 probability;
	s32 currentWater = SMS_GetMarioWaterGun()->getCurrentWater();
	if (currentWater == 0) {
		probability = 0.5f;
	} else if (((TBathtubKillerManager*)mKoopaJr->mKillerManager)
	               ->mInitialLives
	           == TFlagManager::getInstance()->getFlag(0x20001)) {
		probability = 0.5f;
	} else {
		const TWaterGun* gun = SMS_GetMarioWaterGun();
		s32 maxWater = gun->getCurrentNozzle()->mEmitParams.mAmountMax.get();
		s32 curWater = SMS_GetMarioWaterGun()->mCurrentWater;
		f32 p0
		    = KoopajrGetSaveParams(this)->shineKillerProbability0.get();
		probability  = ((f32)curWater / (f32)maxWater)
		                  * (getSaveParams()->shineKillerProbability1.get()
		                     - p0)
		              + p0;
	}
	bool result = false;
	if ((1.0f / 32768.0f) * rand() < probability)
		result = true;
	return result;
}

// UNUSED, 0x100 in the map.
bool TKoopaJrSubmarine::checkKillerLaunch()
{
	if (mSpine->getCurrentNerve()
	        == &TNerveKoopaJrSubmarineLaunchKiller::theNerve()
	    && mKillerIndex < mKillerNum
	    && mTimers[KOOPAJR_SUBMARINE_TIMER_KILLER] <= 0) {
		launchKiller();
		// The signed compare is what says the type went through an int.
		int type = mKillerTypes[mKillerIndex];
		if (type == 2)
			mTimers[KOOPAJR_SUBMARINE_TIMER_KILLER]
			    = getSaveParams()->mSLKillerIntervalFast.get();
		else
			mTimers[KOOPAJR_SUBMARINE_TIMER_KILLER]
			    = getSaveParams()->mSLKillerInterval.get();
		++mKillerIndex;
		return true;
	}
	return false;
}

void TKoopaJrSubmarine::launchKiller()
{
	int launcher           = mKillerIndex % 4;
	TBathtubKiller* killer = (TBathtubKiller*)mKoopaJr->mKillerManager
	                             ->getDeadEnemy();
	if (killer == nullptr)
		return;
	killer->unk194 = mKillerTypes[mKillerIndex];
	killer->reset();
	// TODO: the ROM adds the 1 with its own addi after reset() instead of
	// folding it into the table load's offset, and its frame is 0x18 longer.
	MtxPtr mtx = getModel()->getAnmMtx(TKoopaJr_getJointIndex(launcher + 1));
	killer->mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	JGeometry::TVec3<f32> dir;
	dir.x = mtx[0][2];
	dir.y = mtx[1][2];
	dir.z = mtx[2][2];
	makeKillerVelocity(killer, dir);
	if (gpMSound->gateCheck(0x285D))
		MSoundSESystem::MSoundSE::startSoundActor(0x285D, &killer->mPosition,
		                                          0, nullptr, 0, 4);
}

// TODO: shared-header candidate. The ROM's cross product here stores each
// component as soon as it is computed and reloads the operands after every
// store; JGeometry::TVec3::cross batches all three through locals first.
static inline void KoopajrCross(JGeometry::TVec3<f32>& dst,
                                const JGeometry::TVec3<f32>& a,
                                const JGeometry::TVec3<f32>& b)
{
	dst.x = a.y * b.z - a.z * b.y;
	dst.y = a.z * b.x - a.x * b.z;
	dst.z = a.x * b.y - a.y * b.x;
}

// Shine killers go up and then arc towards Mario; the others aim at a spot
// on the bathtub's rim.
// TODO: 96.6%. Frame 0x190 vs 0x178 (low temporaries 0x1c too many), both
// TQuat4::rotate expansions colour their FPRs differently, and the cross
// product reloads dir after each store where retail reloads toMario instead.
void TKoopaJrSubmarine::makeKillerVelocity(TBathtubKiller* killer,
                                           JGeometry::TVec3<f32> dir)
{
	if ((int)mKillerTypes[mKillerIndex] == 2) {
		dir.set(0.0f, 1.0f, 0.0f);
		JGeometry::TVec3<f32> marioPos(*gpMarioPos);
		JGeometry::TVec3<f32> toMario;
		toMario.sub(marioPos, killer->mPosition);
		toMario.y = 0.0f;
		toMario.normalize();

		f32 angle = 0.2f * M_PI;
		JGeometry::TVec3<f32> axis;
		KoopajrCross(axis, dir, toMario);
		axis.normalize();
		JGeometry::TQuat4<f32> q;
		q.setRotate(axis, 0.2f * M_PI);
		q.rotate(dir, dir);

		axis    = toMario;
		int idx = mKillerIndex % 4;
		if (idx == 0)
			angle = -KoopajrPiTimes(0.05f);
		else if (idx == 1)
			angle = KoopajrPiTimes(0.05f);
		else if (idx == 2)
			angle = -KoopajrPiTimes(0.1f);
		else if (idx == 3)
			angle = KoopajrPiTimes(0.1f);
		q.setRotate(axis, angle);
		q.rotate(dir, dir);
		dir.normalize();
		dir.scale(killer->mPersonality.mInitialSpeed);
	} else {
		dir.normalize();
		dir.scale(killer->mPersonality.mInitialSpeed);

		JGeometry::TVec3<f32> target(*gpMarioPos);
		target.y = (*mKoopaJr->mBathtub->getRootJointMtx())[1][3];

		JGeometry::TVec3<f32> toMario;
		toMario.sub(target, killer->mPosition);
		toMario.y    = 0.0f;
		f32 dist     = toMario.length()
		           - getSaveParams()->killerTargetDistance.get();
		f32 distMin = JGeometry::max(
		    dist, getSaveParams()->killerTargetDistanceMin.get());
		toMario.normalize();
		toMario.scale(distMin);
		JGeometry::TVec3<f32> goal;
		goal.add(killer->mPosition, toMario);
		goal.y = target.y;
		dir    = calcVelocityToJumpToY(
            goal, dir.y, killer->getSaveParam2()->mSLFlyingGravityY.get());
	}
	killer->makeInitialVelocity(dir);
}

// UNUSED, 0x4 in the map.
void TKoopaJrSubmarine::emitKoopaJrSubmarineEffects() { }

// UNUSED, 0x1c in the map. A one-element timer array: the unrolled indexed
// loop is what materialises the timer's address for the store in perform().
void TKoopaJrSubmarine::updateTimers()
{
	for (int i = 0; i < KOOPAJR_SUBMARINE_TIMER_NUM; ++i)
		if (mTimers[i] > 0)
			mTimers[i]--;
}

// UNUSED, 0x8 in the map.
void TKoopaJrSubmarine::setKoopaJr(TKoopaJr* koopaJr) { mKoopaJr = koopaJr; }

const char** TKoopaJrSubmarine::getBasNameTable() const
{
	return koopajrsubmarine_bastable;
}

// Turns the round direction away from Koopa's flame and towards Mario.
// TODO: 99.6%; frame 0xd8 against 0xf0 with every instruction right. The
// ROM has 4 more bytes above toMario and its atan2f copy 0x10 lower with
// 0x24 more above it. Raw mDirection reads (all sites: 0xb8), raw tub
// position (0xc8), SMS_GetMarioPos(), and declaring toMarioDir first or
// before toMario are inert or move the wrong way.
void TKoopaJrSubmarine::makeRelativeAngle()
{
	f32 flameDir
	    = TDirectionCalc::d2r(mKoopaJr->mKoopa->getFlameDirDegree());
	f32 nearerFlame = mDirection.calcNearerDirection(flameDir);
	f32 flameDiff   = fabsf(mDirection.get() - nearerFlame);

	JGeometry::TVec3<f32> toMario;
	toMario.sub(*gpMarioPos, mKoopaJr->mBathtub->getPosition());
	toMario.y = 0.0f;
	// The by-value TVec3 parameter is the copy the ROM makes before atan2f.
	TDirectionCalc toMarioDir;
	toMarioDir.makeDirection(toMario);
	f32 marioDir    = toMarioDir.get();
	f32 nearerMario = mDirection.calcNearerDirection(marioDir);
	f32 target      = mDirection.get();
	f32 marioDiff   = fabsf(target - nearerMario);

	if (mKoopaJr->mKoopa->isFlaming()
	    && flameDiff <= getSaveParams()->aboidKoopaFlameAngle.get())
		target = flameDir + JGeometry::TUtil<f32>::PI();
	else if (marioDiff > getSaveParams()->traceMarioAngle.get())
		target = marioDir;

	// Degrees to radians written out, constant first: d2r() would be an
	// fmuls plus an fdivs where the ROM has one fmuls by pi/180.
	f32 step
	    = 0.017453294f * getSaveParams()->mSLRoundAngleVelocity.get();
	mDirection.normalize();
	f32 dir = mDirection.calcNearerDirection(target);
	f32 turned;
	if (dir > mDirection.get()) {
		f32 diff = dir - mDirection.get();
		if (diff < step)
			step = diff;
		turned = mDirection.get();
		turned += step;
	} else {
		f32 diff = mDirection.get() - dir;
		if (diff < step)
			step = diff;
		turned = mDirection.get() - step;
	}
	mDirection.mDirection = turned;
}

// Accelerates towards the point mRoundDistance out from the tub's centre
// along the round direction.
void TKoopaJrSubmarine::makeRoundVelocity()
{
	JGeometry::TVec3<f32> round(mDirection.calcDirectionVector());
	round.scale(mRoundDistance);
	JGeometry::TVec3<f32> toGoal;
	const JGeometry::TVec3<f32>& center = mKoopaJr->mBathtub->getPosition();
	toGoal.x = (center.x + round.x) - mPosition.x;
	toGoal.y = 0.0f;
	toGoal.z = (center.z + round.z) - mPosition.z;
	if (toGoal.length() < 100.0f) {
		mIsNearTarget = true;
		return;
	}
	mIsNearTarget = false;
	toGoal.normalize();
	toGoal.scale(getSaveParams()->mSLAcceleration.get());
	mVelocity.add(toGoal);
	// TODO: 99.6%; the tub position read through getPosition() lands the
	// frame (0xa0), but the round vector sits 4 low (0x70 against 0x74),
	// the set<float> temporary 0x10 high, and the normalised x and y land
	// in f31/f29 where the ROM has f29/f31. Reordering toGoal's component
	// stores, its constructor or set() spellings, a named goal vector and
	// moving `center` above or below the vectors are all inert or worse.
	f32 speed    = mVelocity.length();
	f32 speedMax = getSaveParams()->mSLSpeedMax.get();
	if (speed > speedMax) {
		mVelocity.normalize();
		mVelocity.scale(speedMax);
	}
}

// UNUSED, 0x18c in the map: turns the hull towards the way it is moving.
void TKoopaJrSubmarine::makeDirection()
{
	if (!mIsNearTarget) {
		JGeometry::TVec3<f32> v(mVelocity);
		v.normalize();
		// Named: the ROM copies the vector once (makeDirection's by-value
		// parameter), calls atan2f and keeps the result in f31 before it
		// fetches the rotation speed.
		// TODO: the frame is 0x28 short in checkNerve's expansion.
		TDirectionCalc calc;
		calc.makeDirection(v);
		f32 dir                   = calc.get();
		mBodyDirection.mDirection = mBodyDirection.calcTurnDirection(
		    dir,
		    TDirectionCalc::d2r(getSaveParams()->mSLRotationSpeed.get()));
	}
}

void TKoopaJrSubmarine::checkNerve()
{
	if (mKoopaJr->mSpine->getCurrentNerve() == &TNerveKoopaJrWait::theNerve()) {
		makeRelativeAngle();
		mRoundDistance = getSaveParams()->mSLRoundDistance.get();
		makeRoundVelocity();
	}
	mVelocity.scale(0.95f);
	makeDirection();

	if (mSpine->getCurrentNerve() == &TNerveKoopaJrSubmarineWait::theNerve())
		return;
	if (mSpine->getCurrentNerve()
	    == &TNerveKoopaJrSubmarineCannonOpenClose::theNerve()) {
		if (mKillerIndex == 0) {
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(0);
			if (ctrl->checkPass(30.0f)) {
				ctrl->setRate(0.0f);
				mSpine->pushNerve(
				    &TNerveKoopaJrSubmarineLaunchKiller::theNerve());
			}
		}
	} else if (mSpine->getCurrentNerve()
	           == &TNerveKoopaJrSubmarineLaunchKiller::theNerve()) {
	}
}

DEFINE_NERVE(TNerveKoopaJrSubmarineWait, TLiveActor)
{
	TKoopaJrSubmarine* submarine = (TKoopaJrSubmarine*)spine->getBody();
	if (spine->getTime() == 0) {
		submarine->setAnimationIndex(0);
		submarine->getMActor()->getFrameCtrl(0)->setRate(0.0f);
	}
	return false;
}

DEFINE_NERVE(TNerveKoopaJrSubmarineCannonOpenClose, TLiveActor)
{
	TKoopaJrSubmarine* submarine = (TKoopaJrSubmarine*)spine->getBody();
	if (spine->getTime() == 0) {
		J3DFrameCtrl* ctrl = submarine->getMActor()->getFrameCtrl(0);
		ctrl->setRate(submarine->mAnmRate);
	}
	if (submarine->getMActor()->isCurAnmAlreadyEnd(0))
		return true;
	return false;
}

DEFINE_NERVE(TNerveKoopaJrSubmarineLaunchKiller, TLiveActor)
{
	TKoopaJrSubmarine* submarine = (TKoopaJrSubmarine*)spine->getBody();
	if (submarine->getKillerIndex() == submarine->getKillerNum()
	    && submarine->mTimers[KOOPAJR_SUBMARINE_TIMER_KILLER] <= 0) {
		J3DFrameCtrl* ctrl = submarine->getMActor()->getFrameCtrl(0);
		ctrl->setRate(submarine->mAnmRate);
		return true;
	}
	return false;
}

// ---------------------------------------------------------------------------
// TKoopaJrSubmarineManager
// ---------------------------------------------------------------------------

TKoopaJrSubmarineManager::TKoopaJrSubmarineManager(const char* name)
    : TEnemyManager(name)
{
}

void TKoopaJrSubmarineManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "LastKoopaJrSubmarine.bmd", 0x54220000, 0 },
		{ nullptr },
	};
	createModelDataArray(entry);
}

void TKoopaJrSubmarineManager::load(JSUMemoryInputStream& stream)
{
	// Same discarded query as loadAfter(), once before and once after the
	// params are built.
	getActiveObjNum();
	TEnemyManager::load(stream);
	unk38 = new TKoopaJrSubmarineParams("/enemy/koopajrsubmarine.prm");
	getActiveObjNum();
}

void TKoopaJrSubmarineManager::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	// The discarded call is real: its body opens with a null test on the
	// params, which is the dead lwz/cmplwi the ROM leaves here.
	getActiveObjNum();
}

// The name is the constructor's default argument, as in TIgaigaManager and
// TPakkunManager, where that one level is exactly what makes the ROM `bl` the
// constructor instead of expanding it.
// TODO: this constructor is still one statement short of the refusal --
// measured with zero-codegen fillers, one flips the site from 3.6% to 100% and
// costs the constructor nothing. Mem-initialisers are not it: neither
// `mDirection()` nor `mDirection(0.0f), mBodyDirection(0.0f)` counts towards
// the budget (both measured, both inert), so the missing statement is a real
// one in the body. Also inert: `setKoopaJr(nullptr)` (the map's UNUSED 0x8
// setter) in place of the mKoopaJr store.
TSpineEnemy* TKoopaJrSubmarineManager::createEnemyInstance()
{
	return new TKoopaJrSubmarine;
}
