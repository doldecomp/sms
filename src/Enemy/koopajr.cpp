#include <Enemy/KoopaJr.hpp>
#include <Enemy/BathtubBinder.hpp>
#include <Enemy/BathtubKiller.hpp>
#include <Enemy/Koopa.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <System/Particles.hpp>
#include <math.h>
#include <macros.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Map/MapCollisionManager.hpp>

// TODO: absDirection, sub and calcNearerDirection match apart from a 0x10-0x18
// bigger stack frame in the target, which hints at one more inline layer
// (likely an inline member wrapping this helper). calcTurnDirection also has a
// one-register swap after the std::fmodf call.
// The target never inlines calcNearerDirection (makeRelativeAngle calls it
// three times), so the original body has at least 15 statements while this
// helper version has about 10. Getting that right probably also fixes the
// frames above.

// fabricated: moves `direction` by a full turn if that brings it closer to
// `current`.
static inline void nearerDirection(f32 current, f32& direction)
{
	if (direction >= current) {
		f32 diff = direction - current;
		if (2.0f * M_PI - diff < diff)
			direction -= 2.0f * M_PI;
	} else {
		f32 diff = current - direction;
		if (2.0f * M_PI - diff < diff)
			direction += 2.0f * M_PI;
	}
}

TDirectionCalc::TDirectionCalc() { unk0 = 0.0f; }

TDirectionCalc::TDirectionCalc(f32 direction) { unk0 = direction; }

TDirectionCalc::TDirectionCalc(JGeometry::TVec3<f32> vec)
{
	makeDirection(vec);
}

void TDirectionCalc::normalize()
{
	unk0 = MsWrapFmodf(unk0, 0.0f, 2.0f * M_PI);
}

f32 TDirectionCalc::calcNearerDirection(f32 direction)
{
	normalize();
	nearerDirection(unk0, direction);
	return direction;
}

f32 TDirectionCalc::sub(f32 direction)
{
	unk0        = MsWrapMod(unk0, 0.0f, 2.0f * M_PI);
	f32 current = unk0;
	nearerDirection(current, direction);
	return current - direction;
}

f32 TDirectionCalc::calcTurnDirection(f32 target, f32 speed)
{
	normalize();
	unk0        = MsWrapMod(unk0, 0.0f, 2.0f * M_PI);
	f32 current = unk0;
	nearerDirection(current, target);
	f32 step = speed;
	if (target > current) {
		if (target - current < speed)
			step = target - current;
		return current + step;
	} else {
		if (current - target < speed)
			step = current - target;
		return current - step;
	}
}

void TDirectionCalc::makeDirection(JGeometry::TVec3<f32> vec)
{
	unk0 = atan2(vec.x, vec.z);
}

JGeometry::TVec3<f32> TDirectionCalc::calcDirectionVector()
{
	return JGeometry::TVec3<f32>(sin(unk0), 0.0f, cos(unk0));
}

f32 TDirectionCalc::absDirection(f32 direction)
{
	unk0        = MsWrapMod(unk0, 0.0f, 2.0f * M_PI);
	f32 current = unk0;
	nearerDirection(current, direction);
	return fabsf(current - direction);
}

f32 TDirectionCalc::d2r(f32 degrees)
{
	return degrees * JGeometry::TUtil<f32>::PI() / 180.0f;
}

f32 TDirectionCalc::r2d(f32 radians)
{
	return 180.0f * radians / JGeometry::TUtil<f32>::PI();
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

// Joint 0 is where Bowser Jr. sits, 1-4 are the four cannons.
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

static int TKoopaJr_jointIndexTable[5];

int TKoopaJr_getJointIndex(int i) { return TKoopaJr_jointIndexTable[i]; }

TCallbackHitActor::TCallbackHitActor(const char* name, u32 actor_type,
                                     f32 damage_radius, f32 damage_height,
                                     THitActor* owner)
    : THitActor(name)
    , mOwner(owner)
{
	initHitActor(actor_type, 0, 0, 0.0f, 0.0f, damage_radius, damage_height);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	TIdxGroupObj* group
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));
	group->getChildren().push_back(this);
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
	onLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
}

// TODO: 96%. After each manager search the target reads manager->unk38 and
// compares it with 0 without using the result. The same dead check shows up in
// the manager load/loadAfter functions below and in BathtubKiller.cpp and
// coasterkiller.cpp; it is probably a debug assert on the params pointer.
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
	mTimers[TIMER_LAUNCH] = getParam()->mSLLaunchKillerPeriod.get();

	mTimers[TIMER_LAUNCH_FAST] = 0;
}

void TKoopaJr::perform(u32 cue, JDrama::TGraphics* graphics)
{
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

	if (cue & CUE_CALC_ANIM)
		checkSubmarineSwing();

	if (cue & CUE_MOVE) {
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
		mSubmarine->getJointTransByIndex(TKoopaJr_getJointIndex(0), &mPosition);
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
	for (int i = 0; i < ARRAY_COUNT(mTimers); i++)
		if (mTimers[i] > 0)
			mTimers[i]--;
}

const char** TKoopaJr::getBasNameTable() const { return koopajr_bastable; }

BOOL TKoopaJr::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
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

// The submarine rocking hard enough also hurts Bowser Jr.
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

	// Always face Mario.
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

// Bowser Jr. cheers whenever one of his Bullet Bills hits Mario.
void TKoopaJr::checkNerveKillerHit()
{
	for (int i = 0; i < mKillerManager->getActiveObjNum(); i++) {
		if (((TBathtubKiller*)mKillerManager->getObj(i))->mHitMario == 1) {
			const TNerveBase<TLiveActor>* nerve
			    = &TNerveKoopaJrYahoo::theNerve();
			mSpine->pushNerve(nerve);
			return;
		}
	}
}

f32 TKoopaJr::getBathtubY() { return (*mBathtub->getRootJointMtx())[1][3]; }

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

TKoopaJrSubmarine::TKoopaJrSubmarine(const char* name)
    : TSpineEnemy(name)
    , mAnmRate(0.0f)
    , mKoopaJr(nullptr)
{
	offLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
}

// TODO: 94%. The target keeps the .bss base (TKoopaJr_jointIndexTable) in r30
// for the whole function and also uses it for the nerve's destructor record,
// while ours addresses that record on its own.
void TKoopaJrSubmarine::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	initAnmSound();
	f32 damageHeight = getParam()->mSLDamageHeight.get();
	initHitActor(ACTOR_TYPE_LAST_KOOPA_JR_SUBMARINE, 0, 0, 0.0f, 0.0f,
	             getParam()->mSLDamageRadius.get(), damageHeight);
	onHitFilter(HIT_FILTER_NO_COLLISION);

	mRearBody  = new TCallbackHitActor("サブマリンリアボディ",
	                                   ACTOR_TYPE_LAST_KOOPA_JR_SUBMARINE_BODY,
	                                   getParam()->mSLDamageRadius.get(),
	                                   getParam()->mSLDamageHeight.get(), this);
	mFrontBody = new TCallbackHitActor("サブマリンフロントボディ",
	                                   ACTOR_TYPE_LAST_KOOPA_JR_SUBMARINE_BODY,
	                                   getParam()->mSLDamageRadius.get(),
	                                   getParam()->mSLDamageHeight.get(), this);

	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("LastKoopaJrSubmarine.bmd", 0);
	mMActor->setLightType(1);
	mSpine->initWith(&TNerveKoopaJrSubmarineWait::theNerve());

	JUTNameTab* jointNames = getModel()->getModelData()->getJointName();
	for (int i = 0; i < 5; i++)
		TKoopaJr_jointIndexTable[i]
		    = jointNames->getIndex(TKoopaJr_jointNameTable[i]);

	f32 scale = getParam()->mSLKoopaJrSubmarineScale.get();
	mScaling.set(scale, scale, scale);

	mBathtubBinder = new TBathtubBinder;
	mBinder        = mBathtubBinder;
	resetKoopaJrSubmarine();
}

void TKoopaJrSubmarine::reset()
{
	TSpineEnemy::reset();
	resetKoopaJrSubmarine();
}

// TODO: frame-only, ours is 8 bytes smaller. TKoopaJr::resetKoopaJr gets its
// extra 8 bytes from clearing the timers in a loop, but doing the same here
// moves the bottomHeight load back in front of the constants.
void TKoopaJrSubmarine::resetKoopaJrSubmarine()
{
	mSpine->reset();
	mTimers[TIMER_KILLER] = 0;
	setAnimationIndex(0);
	mAnmRate     = getMActor()->getFrameCtrl(0)->getRate();
	mKillerIndex = 0;
	mKillerNum   = 0;
	for (int i = 0; i < 8; i++)
		mKillerTypes[i] = TBathtubKiller::KILLER_NORMAL;
	unk154           = 0.0f;
	unk158           = 0.0f;
	unk15C           = 0.0f;
	unk160           = 1.0f;
	mDirection.unk0  = 0.0f;
	mRoundAngle.unk0 = 0.0f;
	mIsNearRoundPos  = false;
	mIsSprayed       = false;
	mSwingAmplitude  = 0.0f;
	mSwingPhase      = 0.0f;
	mWaveAmplitude   = 0.0f;
	mWavePhase       = 0.0f;
	f32 bottomHeight = getParam()->bottomHeight.get();
	mBathtubBinder->init(150.0f, 100.0f, 150.0f, 100.0f, bottomHeight);
}

void TKoopaJrSubmarine::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		checkKillerLaunch();
		makeCollisionPositions();
		moveSwing();
	}

	if (cue & CUE_MOVE) {
		updateTimers();
		checkNerve();
	}

	TSpineEnemy::perform(cue, graphics);
	mRearBody->perform(cue, graphics);
	mFrontBody->perform(cue, graphics);
}

// TODO: frame-only, ours is 8 bytes bigger.
void TKoopaJrSubmarine::makeCollisionPositions()
{
	JGeometry::TVec3<f32> rear(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 2; i++) {
		int index  = TKoopaJr_getJointIndex(i + 3);
		MtxPtr mtx = getModel()->getAnmMtx(index);
		JGeometry::TVec3<f32> pos;
		pos.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		rear.x += pos.x;
		rear.y += pos.y;
		rear.z += pos.z;
	}
	rear.scale(0.5f);
	mRearBody->mPosition.set(rear.x, rear.y, rear.z);
	getJointTransByIndex(TKoopaJr_jointIndexTable[0], &mFrontBody->mPosition);
}

// fabricated: the bound is evaluated after the value and compared with >=, so
// this is the counterpart of JGeometry::max (JGRotation3.hpp), called with the
// bound first.
static inline f32 minOf(f32 a, f32 b) { return a >= b ? b : a; }

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
	    = MsWrapFmodf(mSwingPhase + getParam()->mSLSwingPhaseVelocity.get(),
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
	    = MsWrapFmodf(mWavePhase + getParam()->mSLWavePhaseVelocity.get(), 0.0f,
	                  2.0f * JGeometry::TUtil<f32>::PI());
}

f32 TKoopaJrSubmarine::getSwingAngle()
{
	return mSwingAmplitude * sinf(mSwingPhase);
}

f32 TKoopaJrSubmarine::getWaveAngle()
{
	return mWaveAmplitude * sinf(mWavePhase);
}

void TKoopaJrSubmarine::bind()
{
	JGeometry::TVec3<f32> next = mPosition;
	next += mPositionDelta;
	next += mVelocity;
	mPositionDelta = next - mPosition;
	mBathtubBinder->bind(this);
}

// TODO: 80%. The quaternion products and the final concat use other operand
// orders than these JGeometry helpers (the target adds the translation column
// with separate fadds at the end), and one more float register is live here.
void TKoopaJrSubmarine::calcRootMatrix()
{
	TBathtub* bathtub = mKoopaJr->mBathtub;
	if (bathtub->unk29A) {
		getModel()->setBaseTRMtx(bathtub->getSubmarineMtxInDemo());
	} else {
		JGeometry::TQuat4<f32> swing;
		swing.setEulerZ(getSwingAngle());
		JGeometry::TQuat4<f32> wave;
		wave.setEulerX(getWaveAngle());
		JGeometry::TQuat4<f32> dir;
		dir.setEulerY(mDirection.unk0);
		JGeometry::TQuat4<f32> rot;
		rot.mul(dir, swing);
		rot.mul(rot, wave);

		// Rock around a point centerZ in front of the model's origin.
		JGeometry::TVec3<f32> center(0.0f, 0.0f, getParam()->centerZ.get());
		TPosition3f pivot;
		pivot.translation(center);
		JGeometry::TVec3<f32> trans = center;
		trans.negate();
		trans += mPosition;
		TPosition3f mtx;
		mtx.setQT(rot, trans);
		mtx.concat(pivot);
		getModel()->setBaseTRMtx(mtx);
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

// The last Bullet Bill of a volley may be a shining one, which drops an item
// when it is destroyed (see TBathtubKiller). The chance is one half while
// FLUDD's tank is empty or Mario hasn't lost a life since the fight started.
// Otherwise it goes from shineKillerProbability0 with an empty tank to
// shineKillerProbability1 with a full one.
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

	bool appear = false;
	if (MsRandF() < probability)
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
	int cannon = mKillerIndex % 4;
	TBathtubKiller* killer
	    = (TBathtubKiller*)mKoopaJr->mKillerManager->getDeadEnemy();
	if (killer) {
		killer->mKillerType = mKillerTypes[mKillerIndex];
		killer->reset();
		int jointIndex = cannon + 1;
		MtxPtr mtx = getModel()->getAnmMtx(TKoopaJr_getJointIndex(jointIndex));
		JGeometry::TVec3<f32> pos;
		pos.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		killer->mPosition.set(pos);
		JGeometry::TVec3<f32> dir;
		dir.set(mtx[0][2], mtx[1][2], mtx[2][2]);
		makeKillerVelocity(killer, dir);
		if (gpMSound->gateCheck(MSD_SE_EN_KILLER_FIRE))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_EN_KILLER_FIRE, &killer->mPosition, 0, nullptr, 0, 4);
	}
}

// TODO: 98%. The target's cross product stores x before computing y and z and
// reloads toMario.x (so it is an inline that writes through `this` as it
// goes), loads 0.2 * PI into f30 before the cross product, and passes `this`
// to calcVelocityToJumpToY with mr instead of addi.
void TKoopaJrSubmarine::makeKillerVelocity(TBathtubKiller* killer,
                                           JGeometry::TVec3<f32> dir)
{
	if (mKillerTypes[mKillerIndex] == TBathtubKiller::KILLER_FAST) {
		// Fast ones go up first and then fan out towards Mario.
		dir.set(0.0f, 1.0f, 0.0f);
		JGeometry::TVec3<f32> marioPos = SMS_GetMarioPos();
		JGeometry::TVec3<f32> toMario;
		toMario.sub(marioPos, killer->mPosition);
		toMario.y = 0.0f;
		toMario.normalize();

		JGeometry::TVec3<f32> axis;
		axis.cross(dir, toMario);
		axis.normalize();

		JGeometry::TQuat4<f32> rot;
		rot.setRotate(axis, 0.2f * M_PI);
		rot.rotate(dir);

		axis      = toMario;
		f32 small = 0.05f * JGeometry::TUtil<f32>::PI();
		f32 big   = 0.1f * JGeometry::TUtil<f32>::PI();
		f32 angle;
		int cannon = mKillerIndex % 4;
		if (cannon == 0)
			angle = -small;
		else if (cannon == 1)
			angle = small;
		else if (cannon == 2)
			angle = -big;
		else if (cannon == 3)
			angle = big;
		rot.setRotate(axis, angle);
		rot.rotate(dir);
		dir.normalize();
		dir.scale(killer->mPersonality.mInitialSpeed);
	} else {
		// Normal ones are lobbed so they come down a bit in front of Mario.
		dir.normalize();
		dir.scale(killer->mPersonality.mInitialSpeed);

		JGeometry::TVec3<f32> target = *gpMarioPos;
		target.y                     = mKoopaJr->getBathtubY();
		JGeometry::TVec3<f32> diff;
		diff.x   = target.x - killer->mPosition.x;
		diff.y   = 0.0f;
		diff.z   = target.z - killer->mPosition.z;
		f32 dist = diff.length() - getParam()->killerTargetDistance.get();
		f32 reach
		    = JGeometry::max(dist, getParam()->killerTargetDistanceMin.get());
		JGeometry::TVec3<f32> offset;
		offset.normalize(diff);
		offset.scale(reach);
		JGeometry::TVec3<f32> goal;
		goal.add(killer->mPosition, offset);
		goal.y = target.y;
		dir    = calcVelocityToJumpToY(
            goal, dir.y, killer->getSaveParam2()->mSLFlyingGravityY.get());
	}
	killer->makeInitialVelocity(dir);
}

void TKoopaJrSubmarine::emitKoopaJrSubmarineEffects() { }

void TKoopaJrSubmarine::updateTimers()
{
	for (int i = 0; i < ARRAY_COUNT(mTimers); i++)
		if (mTimers[i] > 0)
			mTimers[i]--;
}

void TKoopaJrSubmarine::setKoopaJr(TKoopaJr* koopa_jr) { mKoopaJr = koopa_jr; }

static const char* koopajrsubmarine_bastable[] = { nullptr };

const char** TKoopaJrSubmarine::getBasNameTable() const
{
	return koopajrsubmarine_bastable;
}

// Pick where around the bathtub to go: away from Bowser's flames, else
// towards Mario once he gets too far off to the side.
// TODO: 54%. The target calls calcNearerDirection instead of inlining it, see
// the TDirectionCalc note at the top of the file.
void TKoopaJrSubmarine::makeRelativeAngle()
{
	f32 flameDir = TDirectionCalc::d2r(mKoopaJr->mKoopa->getFlameDirDegree());
	TBathtub* bathtub = mKoopaJr->mBathtub;
	f32 flameDiff
	    = fabsf(mRoundAngle.unk0 - mRoundAngle.calcNearerDirection(flameDir));

	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario -= bathtub->mPosition;
	toMario.y    = 0.0f;
	f32 marioDir = TDirectionCalc(toMario).unk0;

	f32 target    = mRoundAngle.unk0;
	f32 marioDiff = fabsf(target - mRoundAngle.calcNearerDirection(marioDir));

	if (mKoopaJr->mKoopa->isFlaming()
	    && flameDiff <= getParam()->aboidKoopaFlameAngle.get())
		target = flameDir + JGeometry::TUtil<f32>::PI();
	else if (marioDiff > getParam()->traceMarioAngle.get())
		target = marioDir;

	f32 speed = 0.017453294f * getParam()->mSLRoundAngleVelocity.get();
	mRoundAngle.unk0
	    = MsWrapMod(mRoundAngle.unk0, 0.0f, 2.0f * JGeometry::TUtil<f32>::PI());
	f32 nearer  = mRoundAngle.calcNearerDirection(target);
	f32 current = mRoundAngle.unk0;
	if (nearer > current) {
		if (nearer - current < speed)
			speed = nearer - current;
		mRoundAngle.unk0 = current + speed;
	} else {
		if (current - nearer < speed)
			speed = current - nearer;
		mRoundAngle.unk0 = current - speed;
	}
}

// Accelerate towards the point on the circle around the bathtub.
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
	if (self->mKillerIndex == self->mKillerNum
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

// TODO: the target checks unk38 against 0 before and after the base load and
// has a 0x40 bigger frame, see TKoopaJr::init.
void TKoopaJrSubmarineManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk38 = new TKoopaJrSubmarineParams("/enemy/koopajrsubmarine.prm");
}

// TODO: same unk38 check as load(), with a 0x30 bigger frame.
void TKoopaJrSubmarineManager::loadAfter() { TEnemyManager::loadAfter(); }

// TODO: the target calls the TKoopaJrSubmarine constructor here instead of
// inlining it, although that constructor matches and is small.
TSpineEnemy* TKoopaJrSubmarineManager::createEnemyInstance()
{
	return new TKoopaJrSubmarine("クッパジュニアサブマリン");
}
