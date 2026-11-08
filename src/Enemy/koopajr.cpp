#include <Enemy/KoopaJr.hpp>
#include <Enemy/Koopa.hpp>
#include <Enemy/BathtubBinder.hpp>
#include <Enemy/BathtubKiller.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/Spine.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <System/FlagManager.hpp>
#include <System/FlagManagerFlags.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <System/Particles.hpp>
#include <macros.h>
#include <math.h>

// rogue includes needed for matching sinit
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static int TKoopaJr_jointIndexTable[5];

// fabricated
static inline f32 wrapAngleFmod(f32 value, f32 lo, f32 hi)
{
	f32 range   = hi - lo;
	f32 shifted = value - lo;
	f32 wrapped = std::fmodf(range + shifted, range);
	return lo + wrapped;
}

// fabricated
static inline f32 wrapAngleMod(f32 value, f32 lo, f32 hi)
{
	f32 range   = hi - lo;
	f32 shifted = value - lo;
	f32 wrapped = JGeometry::TUtil<f32>::mod(range + shifted, range);
	return lo + wrapped;
}

static inline void nearerDirection(f32 current, f32& direction)
{
	if (direction >= current) {
		f32 diff = direction - current;
		if (2.0f * JGeometry::TUtil<f32>::PI() - diff < diff)
			direction -= 2.0f * JGeometry::TUtil<f32>::PI();
	} else {
		f32 diff = current - direction;
		if (2.0f * JGeometry::TUtil<f32>::PI() - diff < diff)
			direction += 2.0f * JGeometry::TUtil<f32>::PI();
	}
}

static const f32 sUnusedRodataZero[3] = { 0.0f, 0.0f, 0.0f };
static const f32 sUnusedRodataOne[3]  = { 1.0f, 1.0f, 1.0f };

static const char* koopajr_bastable[] = {
	"/scene/koopajr/bas/koopajr_damage.bas",
	"/scene/koopajr/bas/koopajr_shoot.bas",
	nullptr,
	"/scene/koopajr/bas/koopajr_yahoo.bas",
};

static const char* TKoopaJr_jointNameTable[] = {
	"KoopaJr_null",  "killer_null00", "killer_null03",
	"killer_null01", "killer_null04",
};
inline void TDirectionCalc::normalizeFmod()
{
	unk0 = wrapAngleFmod(unk0, 0.0f, 2.0f * JGeometry::TUtil<f32>::PI());
}

static const char* koopajrsubmarine_bastable[] = { nullptr };
static inline f32 minOf(f32 a, f32 b) { return a >= b ? b : a; }
TDirectionCalc::TDirectionCalc()
    : unk0(0.0f)
{
}

TDirectionCalc::TDirectionCalc(f32 angle)
    : unk0(angle)
{
}

TDirectionCalc::TDirectionCalc(JGeometry::TVec3<f32> dir)
{
	makeDirection(dir);
}

void TDirectionCalc::normalize() { normalizeFmod(); }

// fabricated
#pragma dont_inline on
f32 TDirectionCalc::calcNearerDirection(f32 direction)
{
	normalize();
	nearerDirection(unk0, direction);
	return direction;
}
#pragma dont_inline off

f32 TDirectionCalc::sub(f32 direction)
{
	f32 angle   = unk0;
	unk0        = wrapAngleMod(angle, JGeometry::TUtil<f32>::zero(),
	                           2.0f * JGeometry::TUtil<f32>::PI());
	f32 current = unk0;
	nearerDirection(current, direction);
	return current - direction;
}

f32 TDirectionCalc::calcTurnDirection(f32 target, f32 maxTurn)
{
	normalize();
	unk0        = wrapAngleMod(unk0, 0.0f, 2.0f * JGeometry::TUtil<f32>::PI());
	f32 current = unk0;
	nearerDirection(current, target);
	f32 step = maxTurn;
	if (target > current) {
		if (target - current < maxTurn)
			step = target - current;
		return current + step;
	}
	if (current - target < maxTurn)
		step = current - target;
	return current - step;
}

void TDirectionCalc::makeDirection(JGeometry::TVec3<f32> dir)
{
	unk0 = atan2(dir.x, dir.z);
}

JGeometry::TVec3<f32> TDirectionCalc::calcDirectionVector()
{
	return JGeometry::TVec3<f32>(sin(unk0), 0.0f, cos(unk0));
}

f32 TDirectionCalc::absDirection(f32 direction)
{
	f32 angle   = unk0;
	unk0        = wrapAngleMod(angle, JGeometry::TUtil<f32>::zero(),
	                           2.0f * JGeometry::TUtil<f32>::PI());
	f32 current = unk0;
	nearerDirection(current, direction);
	f32 result = fabsf(current - direction);
	return result;
}

f32 TDirectionCalc::d2r(f32 deg)
{
	return deg * JGeometry::TUtil<f32>::PI() / 180.0f;
}

f32 TDirectionCalc::r2d(f32 rad)
{
	return rad * 180.0f / JGeometry::TUtil<f32>::PI();
}

TKoopaJrParams::TKoopaJrParams(const char* path)
    : TSpineEnemyParams(path)
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

TKoopaJrSubmarineParams::TKoopaJrSubmarineParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(killerTargetDistanceMin, 500.0f)
    , PARAM_INIT(killerTargetDistance, 500.0f)
    , PARAM_INIT(bottomHeight, 0.0f)
    , PARAM_INIT(centerZ, 0.0f)
    , PARAM_INIT(aboidKoopaFlameAngle, 0.1f * JGeometry::TUtil<f32>::PI())
    , PARAM_INIT(traceMarioAngle, 0.1f * JGeometry::TUtil<f32>::PI())
    , PARAM_INIT(mSLWavePhaseVelocity, 0.1f * JGeometry::TUtil<f32>::PI())
    , PARAM_INIT(mSLWaveAmplitudeMin, 0.12f * JGeometry::TUtil<f32>::PI())
    , PARAM_INIT(mSLWaveAmplitudeMaxLaunch, 0.12f * JGeometry::TUtil<f32>::PI())
    , PARAM_INIT(mSLWaveAmplitudeMax, 0.12f * JGeometry::TUtil<f32>::PI())
    , PARAM_INIT(mSLSwingPhaseVelocity, 0.1f * JGeometry::TUtil<f32>::PI())
    , PARAM_INIT(mSLSwingAmplitudeMin, 0.12f * JGeometry::TUtil<f32>::PI())
    , PARAM_INIT(mSLSwingAmplitudeMax, 0.12f * JGeometry::TUtil<f32>::PI())
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
	aboidKoopaFlameAngle.set(0.2f * JGeometry::TUtil<f32>::PI());
	traceMarioAngle.set(0.1f * JGeometry::TUtil<f32>::PI());
	mSLWavePhaseVelocity.set(0.03f * JGeometry::TUtil<f32>::PI());
	mSLWaveAmplitudeMin.set(0.04f * JGeometry::TUtil<f32>::PI());
	mSLWaveAmplitudeMaxLaunch.set(0.12f * JGeometry::TUtil<f32>::PI());
	mSLWaveAmplitudeMax.set(0.1f * JGeometry::TUtil<f32>::PI());
	mSLSwingPhaseVelocity.set(0.06f * JGeometry::TUtil<f32>::PI());
	mSLSwingAmplitudeMin.set(0.04f * JGeometry::TUtil<f32>::PI());
	mSLSwingAmplitudeMax.set(0.18f * JGeometry::TUtil<f32>::PI());
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

static int TKoopaJr_getJointIndex(int i) { return TKoopaJr_jointIndexTable[i]; }

TCallbackHitActor::TCallbackHitActor(const char* name, u32 actorType,
                                     f32 damageRadius, f32 damageHeight,
                                     THitActor* owner)
    : THitActor(name)
    , mOwner(owner)
{
	initHitActor(actorType, 0, 0, 0.0f, 0.0f, damageRadius, damageHeight);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"))
	    ->add(this);
}

BOOL TCallbackHitActor::receiveMessage(THitActor* sender, u32 message)
{
	return mOwner->receiveMessage(sender, message);
}

TKoopaJr::TKoopaJr(const char* name)
    : TSpineEnemy(name)
    , mBathtub(nullptr)
    , mKoopa(nullptr)
    , mSubmarine(nullptr)
    , mSubmarineManager(nullptr)
    , mKillerManager(nullptr)
{
	onLiveFlag(LIVE_FLAG_NO_BIND);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
}

// fabricated
void TKoopaJr::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("koopajr_model.bmd", 0);
	mMActor->setLightType(1);
	initAnmSound();
	f32 damageHeight = getParam()->mSLDamageHeight.get();
	initHitActor(ACTOR_TYPE_LAST_KOOPA_JR, 1, 0, 0.0f, 0.0f,
	             getParam()->mSLDamageRadius.get(), damageHeight);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	mSpine->initWith(&TNerveKoopaJrWait::theNerve());
	mKillerManager = static_cast<TEnemyManager*>(
	    JDrama::TNameRefGen::search("バスタブキラーマネージャー"));
	if (mSubmarineManager == nullptr)
		mSubmarineManager = static_cast<TKoopaJrSubmarineManager*>(
		    JDrama::TNameRefGen::search(
		        "クッパジュニアサブマリンマネージャー"));
	f32 scale = getParam()->mSLKoopaJrScale.get();
	mScaling.set(scale, scale, scale);
	resetKoopaJr();
}

void TKoopaJr::reset()
{
	TSpineEnemy::reset();
	resetKoopaJr();
}

void TKoopaJr::resetKoopaJr()
{
	mSpine->reset();
	for (int i = 0; i < ARRAY_COUNT(mTimers); i++)
		mTimers[i] = 0;
	mTimers[TIMER_LAUNCH]      = getParam()->mSLLaunchKillerPeriod.get();
	mTimers[TIMER_LAUNCH_FAST] = 0;
}

void TKoopaJr::perform(u32 cue, JDrama::TGraphics* graphics)
{
	u32 cueFlags = cue;
	if (mSubmarine == nullptr) {
		mSubmarine = (TKoopaJrSubmarine*)mSubmarineManager->getObj(0);
		mSubmarine->setKoopaJr(this);
	}
	if (mKoopa == nullptr)
		mKoopa = (TKoopa*)static_cast<TEnemyManager*>(
		             JDrama::TNameRefGen::search("クッパマネージャー"))
		             ->getObj(0);
	if (mBathtub == nullptr)
		mBathtub
		    = static_cast<TBathtub*>(JDrama::TNameRefGen::search("バスタブ"));

	if (cueFlags & CUE_CALC_ANIM)
		checkSubmarineSwing();

	if (cueFlags & CUE_MOVE) {
		updateTimers();
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
		int jointIndex = TKoopaJr_getJointIndex(0);
		mSubmarine->getJointTransByIndex(jointIndex, &mPosition);
		MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x, mPosition.y,
		               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	}
	model->setBaseScale(mScaling);
}

void TKoopaJr::startKoopaJrMessage(u32) { }
void TKoopaJr::emitKoopaJrEffects() { }
void TKoopaJr::setAnimationIndex(int index)
{
	getMActor()->setBckFromIndex(index);
	setAnmSound(getBas(index));
}
void TKoopaJr::updateTimers()
{
	for (int i = 0; i < 3; i++)
		if (mTimers[i] > 0)
			mTimers[i]--;
}

const char** TKoopaJr::getBasNameTable() const { return koopajr_bastable; }
BOOL TKoopaJr::receiveMessage(THitActor* sender, u32 message)
{
	u32 messageId = message;
	if (messageId == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);
		gpMSound->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0, 0.0f,
		                        0, 0, 4);
		damageKoopaJr();
		return true;
	}
	return false;
}

void TKoopaJr::damageKoopaJr()
{
	mTimers[TIMER_DAMAGE] = getParam()->mSLDamagePeriod.get();
	startDamageNerve();
}

void TKoopaJr::checkSubmarineSwing()
{
	if (mSubmarine->mSwingAmplitude
	    < 0.5f * mSubmarine->getParam()->mSLSwingAmplitudeMax.get())
		return;
	mTimers[TIMER_DAMAGE] = getParam()->mSLDamagePeriod.get();
	startDamageNerve();
}

void TKoopaJr::startDamageNerve()
{
	if (mSpine->getCurrentNerve() == &TNerveKoopaJrWait::theNerve())
		mSpine->pushNerve(&TNerveKoopaJrDamage::theNerve());
	if (mSpine->getCurrentNerve() == &TNerveKoopaJrLaunch::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveKoopaJrYahoo::theNerve())
		mSpine->setNext(&TNerveKoopaJrDamage::theNerve());
}

void TKoopaJr::checkNerve()
{
	if (mBathtub->unk29A)
		return;

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
	toMario.sub(SMS_GetMarioPos(), mPosition);
	toMario.y = 0.0f;
	TDirectionCalc direction(toMario);

	mRotation.y = TDirectionCalc::r2d(direction.unk0);
}

void TKoopaJr::checkNerveKillerLaunchNormal()
{
	if (mTimers[TIMER_LAUNCH] <= 0) {
		int num = mBathtub->getNumKillerLaunchable();
		if (num != 0) {
			mSubmarine->prepareKillerLaunch(num);
			const TNerveBase<TLiveActor>* nerve
			    = &TNerveKoopaJrLaunch::theNerve();
			mSpine->pushNerve(nerve);
			const TNerveBase<TLiveActor>* submarineNerve
			    = &TNerveKoopaJrSubmarineCannonOpenClose::theNerve();
			mSubmarine->mSpine->pushNerve(submarineNerve);
		}
	}
}

void TKoopaJr::checkNerveKillerLaunchFast()
{
	if (mTimers[TIMER_LAUNCH_FAST] <= 0) {
		int num = mBathtub->getNumKillerBurstable();
		if (num != 0) {
			mSubmarine->prepareKillerLaunchFast(num);
			const TNerveBase<TLiveActor>* nerve
			    = &TNerveKoopaJrLaunch::theNerve();
			mSpine->pushNerve(nerve);
			const TNerveBase<TLiveActor>* submarineNerve
			    = &TNerveKoopaJrSubmarineCannonOpenClose::theNerve();
			mSubmarine->mSpine->pushNerve(submarineNerve);
		}
	}
}

void TKoopaJr::checkNerveKillerHit()
{
	for (int i = 0; i < mKillerManager->getActiveObjNum(); i++) {
		TBathtubKiller* killer = (TBathtubKiller*)mKillerManager->getObj(i);
		if (killer->mHitMario == 1) {
			const TNerveBase<TLiveActor>* nerve
			    = &TNerveKoopaJrYahoo::theNerve();
			mSpine->pushNerve(nerve);
			return;
		}
	}
}

f32 TKoopaJr::getBathtubY() { }

DEFINE_NERVE(TNerveKoopaJrWait, TLiveActor)
{
	TKoopaJr* self = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setAnimationIndex(2);
		self->mTimers[TKoopaJr::TIMER_LAUNCH]
		    = self->getParam()->mSLLaunchKillerPeriod.get();
		self->mTimers[TKoopaJr::TIMER_LAUNCH_FAST]
		    = self->getParam()->mSLLaunchKillerPeriodFast.get();
	}
	return false;
}

DEFINE_NERVE(TNerveKoopaJrDamage, TLiveActor)
{
	TKoopaJr* self = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0)
		self->setAnimationIndex(0);

	if (self->mTimers[TKoopaJr::TIMER_DAMAGE] <= 0
	    && self->getMActor()->isCurAnmAlreadyEnd(0))
		return true;
	return false;
}

DEFINE_NERVE(TNerveKoopaJrDemo, TLiveActor)
{
	TKoopaJr* self = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0)
		self->setAnimationIndex(0);
	return false;
}

DEFINE_NERVE(TNerveKoopaJrLaunch, TLiveActor)
{
	TKoopaJr* self = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0)
		self->setAnimationIndex(1);

	if (self->getMActor()->isCurAnmAlreadyEnd(0))
		return true;
	return false;
}

DEFINE_NERVE(TNerveKoopaJrYahoo, TLiveActor)
{
	TKoopaJr* self = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0)
		self->setAnimationIndex(3);

	if (self->getMActor()->isCurAnmAlreadyEnd(0))
		return true;
	return false;
}

TKoopaJrManager::TKoopaJrManager(const char* name)
    : TEnemyManager(name)
{
}

void TKoopaJrManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "koopajr_model.bmd",
		  J3DMLF_MaterialColorLightOn | J3DMLF_MaterialPEFull
		      | J3DMLF_MaterialTexGenFull | J3DMLF_UseUniqueMaterials
		      | (2 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
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
	static const char* onetimeFilenames[] = {
		"/scene/koopajr/jpa/ms_koopajr_killer.jpa",
	};

	for (int i = 0; i < 1; i++)
		SMS_LoadParticle(onetimeFilenames[i], 0xEF + i);
}

TSpineEnemy* TKoopaJrManager::createEnemyInstance() { return nullptr; }

// fabricated
#pragma dont_inline on
TKoopaJrSubmarine::TKoopaJrSubmarine(const char* name)
    : TSpineEnemy(name)
    , mAnmRate(0.0f)
    , mKoopaJr(nullptr)
{
	offLiveFlag(LIVE_FLAG_NO_BIND);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
}
#pragma dont_inline off

void TKoopaJrSubmarine::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	initAnmSound();

	f32 damageHeight = getParam()->mSLDamageHeight.get();
	f32 damageRadius = getParam()->mSLDamageRadius.get();
	initHitActor(ACTOR_TYPE_BOSS_UNK20, 0, 0, 0.0f, 0.0f, damageRadius,
	             damageHeight);
	onHitFilter(HIT_FILTER_NO_COLLISION);

	mRearBody
	    = new TCallbackHitActor("サブマリンリアボディ", ACTOR_TYPE_BOSS_UNK2D,
	                            getParam()->mSLDamageRadius.get(),
	                            getParam()->mSLDamageHeight.get(), this);
	mFrontBody = new TCallbackHitActor("サブマリンフロントボディ",
	                                   ACTOR_TYPE_BOSS_UNK2D,
	                                   getParam()->mSLDamageRadius.get(),
	                                   getParam()->mSLDamageHeight.get(), this);

	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("LastKoopaJrSubmarine.bmd", 0);
	mMActor->setLightType(1);

	mSpine->initWith(&TNerveKoopaJrSubmarineWait::theNerve());

	JUTNameTab* jointNames = getModel()->getModelData()->getJointName();
	for (int i = 0; i < 5; ++i)
		TKoopaJr_jointIndexTable[i]
		    = jointNames->getIndex(TKoopaJr_jointNameTable[i]);

	f32 scale  = getParam()->mSLKoopaJrSubmarineScale.get();
	mScaling.x = scale;
	mScaling.y = scale;
	mScaling.z = scale;

	mBathtubBinder = new TBathtubBinder();
	mBinder        = mBathtubBinder;
	resetKoopaJrSubmarine();
}

void TKoopaJrSubmarine::reset()
{
	TSpineEnemy::reset();
	resetKoopaJrSubmarine();
}

void TKoopaJrSubmarine::resetKoopaJrSubmarine()
{
	mSpine->reset();
	mTimers[TIMER_KILLER] = 0;
	setAnimationIndex(0);
	mAnmRate            = getMActor()->getFrameCtrl(0)->getRate();
	mKillerIndex        = 0;
	mKillerNum          = 0;
	int killerTypeCount = 8;
	for (int i = 0; i < killerTypeCount; i++)
		mKillerTypes[i] = TBathtubKiller::KILLER_NORMAL;
	unk154                     = 0.0f;
	unk158                     = 0.0f;
	unk15C                     = 0.0f;
	unk160                     = 1.0f;
	mDirection.unk0            = 0.0f;
	mRoundAngle.unk0           = 0.0f;
	mIsNearRoundPos            = false;
	mIsSprayed                 = false;
	mSwingAmplitude            = 0.0f;
	mSwingPhase                = 0.0f;
	mWaveAmplitude             = 0.0f;
	mWavePhase                 = 0.0f;
	f32 bottomHeight           = getParam()->bottomHeight.get();
	TBathtubBinder* bathBinder = mBathtubBinder;
	bathBinder->init(150.0f, 100.0f, 150.0f, 100.0f, bottomHeight);
}

void TKoopaJrSubmarine::perform(u32 cue, JDrama::TGraphics* graphics)
{
	u32 cueFlags = cue;
	if (cueFlags & CUE_CALC_ANIM) {
		checkKillerLaunch();
		makeCollisionPositions();
		moveSwing();
	}

	if (cueFlags & CUE_MOVE) {
		updateTimers();
		checkNerve();
	}

	TSpineEnemy::perform(cue, graphics);
	mRearBody->perform(cue, graphics);
	mFrontBody->perform(cue, graphics);
}

void TKoopaJrSubmarine::makeCollisionPositions()
{
	JGeometry::TVec3<f32> rear(0.0f, 0.0f, 0.0f);
	int jointCount = 2;
	for (int i = 0; i < jointCount; i++) {
		int index  = TKoopaJr_getJointIndex(i + 3);
		MtxPtr mtx = getModel()->getAnmMtx(index);
		JGeometry::TVec3<f32> pos;
		pos.x = mtx[0][3];
		pos.y = mtx[1][3];
		pos.z = mtx[2][3];
		rear.x += pos.x;
		rear.y += pos.y;
		rear.z += pos.z;
	}
	rear.scale(0.5f);
	mRearBody->mPosition.set(rear.x, rear.y, rear.z);
	getJointTransByIndex(TKoopaJr_jointIndexTable[0], &mFrontBody->mPosition);
}

void TKoopaJrSubmarine::moveSwing()
{
	if (mIsSprayed) {
		mIsSprayed = false;
		mSwingAmplitude += 0.02f * JGeometry::TUtil<f32>::PI();
		mSwingAmplitude
		    = minOf(getParam()->mSLSwingAmplitudeMax.get(), mSwingAmplitude);
	}
	mSwingAmplitude -= 0.003f * JGeometry::TUtil<f32>::PI();
	mSwingAmplitude = JGeometry::max(getParam()->mSLSwingAmplitudeMin.get(),
	                                 mSwingAmplitude);
	if (mSwingAmplitude <= 0.0f)
		mSwingPhase = 0.0f;
	mSwingPhase
	    = wrapAngleFmod(mSwingPhase + getParam()->mSLSwingPhaseVelocity.get(),
	                    0.0f, 2.0f * JGeometry::TUtil<f32>::PI());

	f32 speedRate = mVelocity.length() / getParam()->mSLSpeedMax.get();
	if (mTimers[TIMER_KILLER] > 0) {
		mWaveAmplitude += 0.01f * JGeometry::TUtil<f32>::PI();
		mWaveAmplitude = minOf(getParam()->mSLWaveAmplitudeMaxLaunch.get(),
		                       mWaveAmplitude);
	}
	if (speedRate > 0.5f) {
		mWaveAmplitude += 0.01f * JGeometry::TUtil<f32>::PI();
		mWaveAmplitude
		    = minOf(getParam()->mSLWaveAmplitudeMax.get(), mWaveAmplitude);
	}
	mWaveAmplitude -= 0.006f * JGeometry::TUtil<f32>::PI();
	mWaveAmplitude
	    = JGeometry::max(getParam()->mSLWaveAmplitudeMin.get(), mWaveAmplitude);
	if (mWaveAmplitude <= 0.0f)
		mWavePhase = 0.0f;
	mWavePhase
	    = wrapAngleFmod(mWavePhase + getParam()->mSLWavePhaseVelocity.get(),
	                    0.0f, 2.0f * JGeometry::TUtil<f32>::PI());
}

f32 TKoopaJrSubmarine::getSwingAngle() { }
f32 TKoopaJrSubmarine::getWaveAngle() { }
void TKoopaJrSubmarine::bind()
{
	JGeometry::TVec3<f32> nextPosition;
	nextPosition = mPosition;
	nextPosition.add(mPositionDelta);
	nextPosition.add(mVelocity);

	mPositionDelta = nextPosition - mPosition;
	mBathtubBinder->bind(this);
}

void TKoopaJrSubmarine::calcRootMatrix()
{
	if (mKoopaJr->mBathtub->unk29A) {
		MtxPtr mtx = mKoopaJr->mBathtub->getSubmarineMtxInDemo();
		PSMTXCopy(mtx, getModel()->getBaseTRMtx());
	} else {
		f32 swingHalf = 0.5f * (mSwingAmplitude * sinf(mSwingPhase));
		f32 swingSin  = sinf(swingHalf);
		f32 swingCos  = cosf(swingHalf);

		f32 waveHalf = 0.5f * (mWaveAmplitude * sinf(mWavePhase));
		f32 waveSin  = sinf(waveHalf);
		f32 waveCos  = cosf(waveHalf);

		f32 yawHalf = 0.5f * mDirection.unk0;
		f32 yawSin  = sinf(yawHalf);
		f32 yawCos  = cosf(yawHalf);

		JGeometry::TQuat4<f32> q;
		q.x = yawSin * swingSin;
		q.y = yawSin * swingCos;
		q.z = yawCos * swingSin;
		q.w = yawCos * swingCos;

		JGeometry::TQuat4<f32> waveQ(waveSin, 0.0f, 0.0f, waveCos);
		JGeometry::TQuat4<f32> finalQ;
		finalQ.mul(q, waveQ);

		f32 center = getParam()->centerZ.get();
		JGeometry::TVec3<f32> centerOffset(0.0f, 0.0f, center);
		TPosition3f centerMtx;
		centerMtx.translation(centerOffset);

		JGeometry::TVec3<f32> origin;
		origin = centerOffset;
		origin.negate();
		origin.add(mPosition);

		TPosition3f mtx;
		mtx.setQuat(finalQ);
		mtx.setTrans(origin);
		mtx.concat(centerMtx);

		PSMTXCopy(mtx.mMtx, getModel()->getBaseTRMtx());
	}

	getModel()->setBaseScale(mScaling);
}

BOOL TKoopaJrSubmarine::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);
		gpMSound->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0, 0.0f,
		                        0, 0, 4);
		damageKoopaJrSubmarine();
		return true;
	}
	return false;
}

void TKoopaJrSubmarine::damageKoopaJrSubmarine() { mIsSprayed = true; }
void TKoopaJrSubmarine::setAnimationIndex(int index)
{
	getMActor()->setBckFromIndex(index);
	setAnmSound(getBas(index));
}
void TKoopaJrSubmarine::prepareKillerLaunch(int num)
{
	if (num > 8)
		num = 8;
	mKillerIndex = 0;
	mKillerNum   = num;
	for (int i = 0; i < mKillerNum; i++)
		mKillerTypes[i] = TBathtubKiller::KILLER_NORMAL;
	if (appearShineKiller(mKillerNum))
		mKillerTypes[mKillerNum - 1] = TBathtubKiller::KILLER_SHINE;
}

void TKoopaJrSubmarine::prepareKillerLaunchFast(int num)
{
	if (num > 8)
		num = 8;
	mKillerIndex = 0;
	mKillerNum   = num;
	for (int i = 0; i < mKillerNum; i++)
		mKillerTypes[i] = TBathtubKiller::KILLER_FAST;
	if (appearShineKiller(mKillerNum))
		mKillerTypes[mKillerNum - 1] = TBathtubKiller::KILLER_SHINE;
}

bool TKoopaJrSubmarine::appearShineKiller(int)
{
	f32 probability;
	if (SMS_GetMarioWaterGun()->mCurrentWater == 0) {
		probability = 0.5f;
	} else if (TFlagManager::smInstance->getFlag(MSF_LIFE_COUNT)
	           == ((TBathtubKillerManager*)mKoopaJr->mKillerManager)
	                  ->mInitialLifeCount) {
		probability = 0.5f;
	} else {
		s32 maxWater = SMS_GetMarioWaterGun()->getMaxWater();
		s32 water    = SMS_GetMarioWaterGun()->mCurrentWater;
		f32 p0       = getParam()->shineKillerProbability0.get();
		f32 p1       = getParam()->shineKillerProbability1.get();
		f32 rate     = (f32)water / (f32)maxWater;
		probability  = rate * (p1 - p0) + p0;
	}

	bool appear   = false;
	f32 threshold = probability;
	if (MsRandF() < threshold)
		appear = true;
	return appear;
}

void TKoopaJrSubmarine::checkKillerLaunch()
{
	if (mSpine->getCurrentNerve()
	        == &TNerveKoopaJrSubmarineLaunchKiller::theNerve()
	    && mKillerIndex < mKillerNum && mTimers[TIMER_KILLER] <= 0) {
		launchKiller();
		if (mKillerTypes[mKillerIndex] == TBathtubKiller::KILLER_FAST)
			mTimers[TIMER_KILLER] = getParam()->mSLKillerIntervalFast.get();
		else
			mTimers[TIMER_KILLER] = getParam()->mSLKillerInterval.get();
		mKillerIndex++;
	}
}

void TKoopaJrSubmarine::launchKiller()
{
	int jointSlot = mKillerIndex % 4;
	TBathtubKiller* killer
	    = (TBathtubKiller*)mKoopaJr->mKillerManager->getDeadEnemy();
	if (!killer)
		return;

	killer->mKillerType = mKillerTypes[mKillerIndex];
	killer->reset();

	int jointIndex = jointSlot + 1;
	MtxPtr mtx     = getModel()->getAnmMtx(TKoopaJr_getJointIndex(jointIndex));
	killer->mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);

	JGeometry::TVec3<f32> direction;
	direction.x = mtx[0][2];
	direction.y = mtx[1][2];
	direction.z = mtx[2][2];
	makeKillerVelocity(killer, direction);

	if (gpMSound->gateCheck(MSD_SE_EN_KILLER_FIRE))
		MSoundSESystem::MSoundSE::startSoundActor(
		    MSD_SE_EN_KILLER_FIRE, &killer->mPosition, 0, nullptr, 0, 4);
}

void TKoopaJrSubmarine::makeKillerVelocity(TBathtubKiller* killer,
                                           JGeometry::TVec3<f32> direction)
{
	int queuedType = mKillerTypes[mKillerIndex];
	if (queuedType == 2) {
		direction.set(0.0f, 1.0f, 0.0f);

		JGeometry::TVec3<f32> marioPosition = *gpMarioPos;
		JGeometry::TVec3<f32> toMario;
		toMario.sub(marioPosition, killer->mPosition);
		toMario.y = 0.0f;
		toMario.normalize();

		f32 fanAngle = 0.62831855f;
		JGeometry::TVec3<f32> axis;
		axis.x = direction.y * toMario.z - direction.z * toMario.y;
		axis.y = direction.z * toMario.x - direction.x * toMario.z;
		axis.z = direction.x * toMario.y - direction.y * toMario.x;
		axis.normalize();

		JGeometry::TQuat4<f32> rotation;
		rotation.setRotate(axis, 0.62831855f);
		rotation.rotate(direction, direction);

		axis     = toMario;
		int slot = mKillerIndex % 4;
		if (slot == 0) {
			f32 angle = 0.15707964f;
			fanAngle  = -angle;
		} else if (slot == 1) {
			fanAngle = 0.15707964f;
		} else if (slot == 2) {
			f32 angle = 0.31415927f;
			fanAngle  = -angle;
		} else if (slot == 3) {
			fanAngle = 0.31415927f;
		}

		rotation.setRotate(axis, fanAngle);
		rotation.rotate(direction, direction);

		direction.normalize();
		direction.scale(killer->mPersonality.mInitialSpeed);
	} else {
		direction.normalize();
		direction.scale(killer->mPersonality.mInitialSpeed);

		JGeometry::TVec3<f32> target = *gpMarioPos;
		Mtx* rootMtx                 = mKoopaJr->mBathtub->getRootJointMtx();
		target.y                     = (*rootMtx)[1][3];

		JGeometry::TVec3<f32> toTarget(target.x - killer->mPosition.x, 0.0f,
		                               target.z - killer->mPosition.z);
		f32 distance
		    = toTarget.length() - getParam()->killerTargetDistance.get();
		f32 offset = getParam()->killerTargetDistanceMin.get();
		if (distance >= offset)
			offset = distance;

		toTarget.normalize();
		toTarget.scale(offset);

		JGeometry::TVec3<f32> jumpTarget;
		jumpTarget.add(killer->mPosition, toTarget);
		jumpTarget.y = target.y;

		direction = calcVelocityToJumpToY(
		    jumpTarget, direction.y,
		    killer->getSaveParam2()->mSLFlyingGravityY.get());
	}

	killer->makeInitialVelocity(direction);
}

void TKoopaJrSubmarine::emitKoopaJrSubmarineEffects() { }
void TKoopaJrSubmarine::updateTimers()
{
	for (int i = 0; i < 1; i++)
		if (mTimers[i] > 0)
			mTimers[i]--;
}
void TKoopaJrSubmarine::setKoopaJr(TKoopaJr* koopaJr) { mKoopaJr = koopaJr; }
const char** TKoopaJrSubmarine::getBasNameTable() const
{
	return koopajrsubmarine_bastable;
}

void TKoopaJrSubmarine::makeRelativeAngle()
{
	f32 flameAngle = TDirectionCalc::d2r(mKoopaJr->mKoopa->getFlameDirDegree());
	f32 nearerFlame = mRoundAngle.calcNearerDirection(flameAngle);
	f32 flameDiff   = fabsf(mRoundAngle.unk0 - nearerFlame);

	JGeometry::TVec3<f32> marioDiff;
	marioDiff.sub(*gpMarioPos, mKoopaJr->mBathtub->mPosition);
	marioDiff.y = 0.0f;

	TDirectionCalc marioDirection;
	marioDirection.makeDirection(marioDiff);
	f32 marioAngle  = marioDirection.unk0;
	f32 nearerMario = mRoundAngle.calcNearerDirection(marioAngle);
	f32 current     = mRoundAngle.unk0;
	f32 marioAbs    = fabsf(current - nearerMario);

	if (mKoopaJr->mKoopa->isFlaming()
	    && flameDiff <= getParam()->aboidKoopaFlameAngle.get()) {
		current = flameAngle + JGeometry::TUtil<f32>::PI();
	} else if (marioAbs > getParam()->traceMarioAngle.get()) {
		current = marioAngle;
	}

	f32 turn         = getParam()->mSLRoundAngleVelocity.get() * 0.017453294f;
	mRoundAngle.unk0 = wrapAngleMod(mRoundAngle.unk0, 0.0f,
	                                2.0f * JGeometry::TUtil<f32>::PI());
	f32 nearer       = mRoundAngle.calcNearerDirection(current);
	f32 direction    = mRoundAngle.unk0;
	if (nearer > direction) {
		f32 diff = nearer - direction;
		if (diff < turn)
			turn = diff;
		direction += turn;
	} else {
		f32 diff = direction - nearer;
		if (diff < turn)
			turn = diff;
		direction -= turn;
	}
	mRoundAngle.unk0 = direction;
}

void TKoopaJrSubmarine::makeRoundVelocity()
{
	JGeometry::TVec3<f32> offset = mRoundAngle.calcDirectionVector();
	offset.scale(mRoundDistance);
	TBathtub* bathtub                   = mKoopaJr->mBathtub;
	const JGeometry::TVec3<f32>& center = bathtub->mPosition;
	JGeometry::TVec3<f32> diff;
	diff.x = (center.x + offset.x) - mPosition.x;
	diff.y = 0.0f;
	diff.z = (center.z + offset.z) - mPosition.z;
	if (diff.length() < 100.0f) {
		mIsNearRoundPos = true;
		return;
	}

	mIsNearRoundPos = false;
	JGeometry::TVec3<f32> dir;
	dir.normalize(diff);
	dir.scale(getParam()->mSLAcceleration.get());
	mVelocity += dir;
	f32 speed = mVelocity.length();
	f32 max   = getParam()->mSLSpeedMax.get();
	if (speed > max) {
		mVelocity.normalize();
		mVelocity.scale(max);
	}
}

void TKoopaJrSubmarine::makeDirection()
{
	JGeometry::TVec3<f32> dir = mVelocity;
	dir.normalize();
	TDirectionCalc target(dir);
	mDirection.unk0 = mDirection.calcTurnDirection(
	    target.unk0, TDirectionCalc::d2r(getParam()->mSLRotationSpeed.get()));
}

void TKoopaJrSubmarine::checkNerve()
{
	if (mKoopaJr->mSpine->getCurrentNerve() == &TNerveKoopaJrWait::theNerve()) {
		makeRelativeAngle();
		mRoundDistance = getParam()->mSLRoundDistance.get();
		makeRoundVelocity();
	}

	mVelocity.scale(0.95f);
	if (!mIsNearRoundPos)
		makeDirection();

	if (mSpine->getCurrentNerve() == &TNerveKoopaJrSubmarineWait::theNerve()) {
	} else if (mSpine->getCurrentNerve()
	           == &TNerveKoopaJrSubmarineCannonOpenClose::theNerve()) {
		if (mKillerIndex == 0) {
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(0);
			if (ctrl->checkPass(30.0f)) {
				ctrl->setRate(0.0f);
				const TNerveBase<TLiveActor>* nerve
				    = &TNerveKoopaJrSubmarineLaunchKiller::theNerve();
				mSpine->pushNerve(nerve);
			}
		}
	} else if (mSpine->getCurrentNerve()
	           == &TNerveKoopaJrSubmarineLaunchKiller::theNerve()) {
	}
}

DEFINE_NERVE(TNerveKoopaJrSubmarineWait, TLiveActor)
{
	TKoopaJrSubmarine* self = (TKoopaJrSubmarine*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setAnimationIndex(0);
		self->getMActor()->getFrameCtrl(0)->setRate(0.0f);
	}
	return false;
}

DEFINE_NERVE(TNerveKoopaJrSubmarineCannonOpenClose, TLiveActor)
{
	TKoopaJrSubmarine* self = (TKoopaJrSubmarine*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setAnimationIndex(0);
		J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(0);
		ctrl->setRate(self->mAnmRate);
	}

	if (self->getMActor()->isCurAnmAlreadyEnd(0))
		return true;
	return false;
}

DEFINE_NERVE(TNerveKoopaJrSubmarineLaunchKiller, TLiveActor)
{
	TKoopaJrSubmarine* self = (TKoopaJrSubmarine*)spine->getBody();
	s32 killerIndex         = self->mKillerIndex;
	if (killerIndex == self->mKillerNum
	    && self->mTimers[TKoopaJrSubmarine::TIMER_KILLER] <= 0) {
		J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(0);
		ctrl->setRate(self->mAnmRate);
		return true;
	}
	return false;
}

TKoopaJrSubmarineManager::TKoopaJrSubmarineManager(const char* name)
    : TEnemyManager(name)
{
}

void TKoopaJrSubmarineManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "LastKoopaJrSubmarine.bmd",
		  J3DMLF_MaterialColorLightOn | J3DMLF_MaterialPEFull
		      | J3DMLF_MaterialTexGenFull | J3DMLF_UseUniqueMaterials
		      | (2 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TKoopaJrSubmarineManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk38 = new TKoopaJrSubmarineParams("/enemy/koopajrsubmarine.prm");
}

void TKoopaJrSubmarineManager::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	if (!unk38)
		return;
}

TSpineEnemy* TKoopaJrSubmarineManager::createEnemyInstance()
{
	return new TKoopaJrSubmarine("クッパジュニアサブマリン");
}
