#include <Enemy/KoopaJr.hpp>

#include <Enemy/BathtubBinder.hpp>
#include <Enemy/BathtubKiller.hpp>
#include <Enemy/Koopa.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <System/Particles.hpp>
#include <macros.h>

#include <math.h>

TDirectionCalc::TDirectionCalc()
    : mDirection(0.0f)
{
}

TDirectionCalc::TDirectionCalc(f32 direction)
    : mDirection(direction)
{
}

TDirectionCalc::TDirectionCalc(JGeometry::TVec3<f32> direction)
{
	makeDirection(direction);
}

// UNUSED - 0x4c size from map
void TDirectionCalc::normalize()
{
	mDirection = MsRepeat(mDirection, 0.0f, TAU);
}

f32 TDirectionCalc::calcNearerDirection(f32 direction)
{
	normalize();

	// Another converge
	if (direction >= mDirection) {
		f32 difference = direction - mDirection;
		if (TAU - difference < difference)
			direction -= TAU;
	} else {
		f32 difference = mDirection - direction;
		if (TAU - difference < difference)
			direction += TAU;
	}
	return direction;
}

f32 TDirectionCalc::sub(f32 direction)
{
	return mDirection - calcNearerDirection(direction);
}

f32 TDirectionCalc::calcTurnDirection(f32 direction, f32 turn_speed)
{
	normalize();
	direction = calcNearerDirection(direction);

	// And you know it, a different converge function...
	f32 result = turn_speed;
	if (direction > mDirection) {
		if (direction - mDirection < turn_speed) {
			result = direction - mDirection;
		}
		result = mDirection + result;
	} else {
		if (mDirection - direction < turn_speed) {
			result = mDirection - direction;
		}
		result = mDirection - result;
	}
	return result;
}

void TDirectionCalc::makeDirection(JGeometry::TVec3<f32> direction)
{
	mDirection = atan2(direction.x, direction.z);
}

JGeometry::TVec3<f32> TDirectionCalc::calcDirectionVector()
{
	return JGeometry::TVec3<f32>(sin(mDirection), 0.0f, cos(mDirection));
}

f32 TDirectionCalc::absDirection(f32 direction)
{
	f32 difference = mDirection - calcNearerDirection(direction);
	return abs(difference);
}

f32 TDirectionCalc::d2r(f32 direction)
{
	return direction * JGeometry::TUtil<f32>::PI() / 180.0f;
}

f32 TDirectionCalc::r2d(f32 direction) { return 180.0f * direction / M_PI; }

TKoopaJrParams::TKoopaJrParams(const char* name)
    : TSpineEnemyParams(name)
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

TKoopaJrSubmarineParams::TKoopaJrSubmarineParams(const char* name)
    : TSpineEnemyParams(name)
    , PARAM_INIT(killerTargetDistanceMin, 500.0f)
    , PARAM_INIT(killerTargetDistance, 500.0f)
    , PARAM_INIT(bottomHeight, 0.0f)
    , PARAM_INIT(centerZ, 0.0f)
    , PARAM_INIT(aboidKoopaFlameAngle, JGeometry::TUtil<f32>::PI() * 0.1f)
    , PARAM_INIT(traceMarioAngle, JGeometry::TUtil<f32>::PI() * 0.1f)
    , PARAM_INIT(mSLWavePhaseVelocity, JGeometry::TUtil<f32>::PI() * 0.1f)
    , PARAM_INIT(mSLWaveAmplitudeMin, JGeometry::TUtil<f32>::PI() * 0.12f)
    , PARAM_INIT(mSLWaveAmplitudeMaxLaunch, JGeometry::TUtil<f32>::PI() * 0.12f)
    , PARAM_INIT(mSLWaveAmplitudeMax, JGeometry::TUtil<f32>::PI() * 0.12f)
    , PARAM_INIT(mSLSwingPhaseVelocity, JGeometry::TUtil<f32>::PI() * 0.1f)
    , PARAM_INIT(mSLSwingAmplitudeMin, JGeometry::TUtil<f32>::PI() * 0.12f)
    , PARAM_INIT(mSLSwingAmplitudeMax, JGeometry::TUtil<f32>::PI() * 0.12f)
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
	aboidKoopaFlameAngle.set(JGeometry::TUtil<f32>::PI() * 0.2f);
	traceMarioAngle.set(JGeometry::TUtil<f32>::PI() * 0.1f);
	mSLWavePhaseVelocity.set(JGeometry::TUtil<f32>::PI() * 0.03f);
	mSLWaveAmplitudeMin.set(JGeometry::TUtil<f32>::PI() * 0.04f);
	mSLWaveAmplitudeMaxLaunch.set(JGeometry::TUtil<f32>::PI() * 0.12f);
	mSLWaveAmplitudeMax.set(JGeometry::TUtil<f32>::PI() * 0.1f);
	mSLSwingPhaseVelocity.set(JGeometry::TUtil<f32>::PI() * 0.06f);
	mSLSwingAmplitudeMin.set(JGeometry::TUtil<f32>::PI() * 0.04f);
	mSLSwingAmplitudeMax.set(JGeometry::TUtil<f32>::PI() * 0.18f);
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

static const char* koopajr_bastable[] = {
	"/scene/koopajr/bas/koopajr_damage.bas",
	"/scene/koopajr/bas/koopajr_shoot.bas",
	nullptr,
	"/scene/koopajr/bas/koopajr_yahoo.bas",
};
static const char* koopajrsubmarine_bastable[] = { nullptr };
static const char* TKoopaJr_jointNameTable[]   = {
    "KoopaJr_null",  "killer_null00", "killer_null03",
    "killer_null01", "killer_null04",
};
static int TKoopaJr_jointIndexTable[5];

// UNUSED - 0x18 size from map
int TKoopaJr_getJointIndex(int index)
{
	return TKoopaJr_jointIndexTable[index];
}

// UNUSED - 0x12c size from map
TCallbackHitActor::TCallbackHitActor(const char* name, u32 type, f32 radius,
                                     f32 height, THitActor* actor)
    : THitActor(name)
    , unk68(actor)
{
	initHitActor(type, 0, 0, 0.0f, 0.0f, radius, height);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	TIdxGroupObj* group
	    = (TIdxGroupObj*)(JDrama::TNameRefGen::search("敵グループ"));
	group->getChildren().push_back(this);
}

BOOL TCallbackHitActor::receiveMessage(THitActor* sender, u32 message)
{
	return unk68->receiveMessage(sender, message);
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

void TKoopaJr::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("koopajr_model.bmd", 0);
	mMActor->setLightType(1);
	initAnmSound();
	f32 height = getParam()->mSLDamageHeight.get();
	initHitActor(0x08000028, 1, 0, 0.0f, 0.0f,
	             getParam()->mSLDamageRadius.get(), height);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	mSpine->initWith(&TNerveKoopaJrWait::theNerve());
	mKillerManager = (TBathtubKillerManager*)JDrama::TNameRefGen::search(
	    "バスタブキラーマネージャー");
	mKillerManager->getActiveObjNum();

	if (mSubmarineManager == nullptr)
		mSubmarineManager
		    = (TKoopaJrSubmarineManager*)JDrama::TNameRefGen::search(
		        "クッパジュニアサブマリンマネージャー");
	mSubmarineManager->getActiveObjNum();

	f32 scale = getParam()->mSLKoopaJrScale.get();
	mScaling.set(scale, scale, scale);
	resetKoopaJr();
}

void TKoopaJr::reset()
{
	TSpineEnemy::reset();
	resetKoopaJr();
}

// UNUSED - 0x64 size from map
void TKoopaJr::resetKoopaJr()
{
	mSpine->reset();

	for (int i = 0; i < ARRAY_COUNT(mTimers); ++i)
		mTimers[i] = 0;

	mTimers[TIMER_KILLER_LAUNCH_NORMAL]
	    = getParam()->mSLLaunchKillerPeriod.get();
	mTimers[TIMER_KILLER_LAUNCH_FAST] = 0;
}

void TKoopaJr::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mSubmarine == nullptr) {
		mSubmarine = (TKoopaJrSubmarine*)mSubmarineManager->getObj(0);
		mSubmarine->setKoopaJr(this);
	}

	if (mKoopa == nullptr) {
		TEnemyManager* manager
		    = (TEnemyManager*)JDrama::TNameRefGen::search("クッパマネージャー");
		mKoopa = (TKoopa*)manager->getObj(0);
	}

	if (mBathtub == nullptr) {
		mBathtub = (TBathtub*)JDrama::TNameRefGen::search("バスタブ");
	}

	if (cue & CUE_CALC_ANIM) {
		checkSubmarineSwing();
	}

	if (cue & CUE_MOVE) {
		updateTimers();
		if (!mBathtub->getUnk29A()) {
			checkNerve();
		}
	}

	TSpineEnemy::perform(cue, graphics);
}

void TKoopaJr::calcRootMatrix()
{
	J3DModel* model = getModel();
	if (mBathtub->unk29A) {
		MtxPtr matrix = mBathtub->getKoopaJrMtxInDemo();
		getModel()->setBaseTRMtx(matrix);
	} else {
		mSubmarine->getJointTransByIndex(TKoopaJr_getJointIndex(0), &mPosition);
		MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x, mPosition.y,
		               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	}
	model->setBaseScale(mScaling);
}

// UNUSED - 0x2c size from map
void TKoopaJr::startKoopaJrMessage(u32 message)
{
	// Probably startAppearBalloon call?
}

// UNUSED - 0x4 size from map
void TKoopaJr::emitKoopaJrEffects()
{
	// Unimplemented
}

// UNUSED - 0x70 size from map
void TKoopaJr::setAnimationIndex(int index)
{
	mMActor->setBckFromIndex(index);
	setAnmSound(getBas(index));
}

// UNUSED - 0x4c size from map
void TKoopaJr::updateTimers()
{
	for (int i = 0; i < ARRAY_COUNT(mTimers); ++i)
		if (mTimers[i] > 0)
			--mTimers[i];
}

const char** TKoopaJr::getBasNameTable() const { return koopajr_bastable; }

BOOL TKoopaJr::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);

		SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0,
		                              0.0f, 0, 0, 4);
		damageKoopaJr();
		return TRUE;
	}
	return FALSE;
}

// UNUSED - 0x240 size from map
void TKoopaJr::damageKoopaJr()
{
	mTimers[TIMER_DAMAGE] = getParam()->mSLDamagePeriod.get();
	startDamageNerve();
}

// UNUSED - 0x274 size from map
void TKoopaJr::checkSubmarineSwing()
{
	f32 maxSwing = 0.5f * mSubmarine->getParam()->mSLSwingAmplitudeMax.get();
	if (!mSubmarine->isSwingBelowLimit(maxSwing)) {
		mTimers[TIMER_DAMAGE] = getParam()->mSLDamagePeriod.get();
		startDamageNerve();
	}
}

// UNUSED - 0x230 size from map
void TKoopaJr::startDamageNerve()
{
	if (mSpine->getCurrentNerve() == &TNerveKoopaJrWait::theNerve())
		mSpine->pushNerve(&TNerveKoopaJrDamage::theNerve());
	if (mSpine->getCurrentNerve() == &TNerveKoopaJrLaunch::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveKoopaJrYahoo::theNerve())
		mSpine->setNext(&TNerveKoopaJrDamage::theNerve());
}

// UNUSED - 0x418 size from map
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
	JGeometry::TVec3<f32> direction;
	direction.sub(SMS_GetMarioPos(), mPosition);
	direction.y = 0.0f;
	TDirectionCalc calc(direction);
	mRotation.y = TDirectionCalc::r2d(calc.mDirection);
}

void TKoopaJr::checkNerveKillerLaunchNormal()
{
	if (mTimers[TIMER_KILLER_LAUNCH_NORMAL] > 0)
		return;
	int count = mBathtub->getNumKillerLaunchable();
	if (count == 0)
		return;
	mSubmarine->prepareKillerLaunch(count);
	pushNerve(&TNerveKoopaJrLaunch::theNerve());
	mSubmarine->pushNerve(&TNerveKoopaJrSubmarineCannonOpenClose::theNerve());
#if defined(VERSION_GMSP01) || defined(VERSION_GMSE01)
	mSubmarine->setAnimationIndex(0);
#endif
}

void TKoopaJr::checkNerveKillerLaunchFast()
{
	if (mTimers[TIMER_KILLER_LAUNCH_FAST] > 0)
		return;
	int count = mBathtub->getNumKillerBurstable();
	if (count == 0)
		return;
	mSubmarine->prepareKillerLaunchFast(count);
	pushNerve(&TNerveKoopaJrLaunch::theNerve());
	mSubmarine->pushNerve(&TNerveKoopaJrSubmarineCannonOpenClose::theNerve());
#if defined(VERSION_GMSP01) || defined(VERSION_GMSE01)
	mSubmarine->setAnimationIndex(0);
#endif
}

void TKoopaJr::checkNerveKillerHit()
{
	for (int i = 0; i < mKillerManager->getActiveObjNum(); ++i) {
		TBathtubKiller* killer = (TBathtubKiller*)mKillerManager->getObj(i);
		if (killer->isHitMario()) {
			pushNerve(&TNerveKoopaJrYahoo::theNerve());
			return;
		}
	}
}

// UNUSED - 0x34 size from map
f32 TKoopaJr::getBathtubY()
{
	// Idk if they would cast stuff to TPosition3f* or not, probably not?
	// (*mBathtub->getRootJointMtx())[1][3]; matches as well and is used in
	// BathtubKiller
	const TPosition3f* matrix = (const TPosition3f*)mBathtub->getRootJointMtx();
	return matrix->at(1, 3);
}

DEFINE_NERVE(TNerveKoopaJrWait, TLiveActor)
{
	TKoopaJr* self = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setAnimationIndex(2);
		self->mTimers[TKoopaJr::TIMER_KILLER_LAUNCH_NORMAL]
		    = self->getParam()->mSLLaunchKillerPeriod.get();
		self->mTimers[TKoopaJr::TIMER_KILLER_LAUNCH_FAST]
		    = self->getParam()->mSLLaunchKillerPeriodFast.get();
	}
	return FALSE;
}

DEFINE_NERVE(TNerveKoopaJrDamage, TLiveActor)
{
	TKoopaJr* self = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0)
		self->setAnimationIndex(0);
	if (!self->isTimerActive(TKoopaJr::TIMER_DAMAGE)
	    && self->getMActor()->isCurAnmAlreadyEnd(ANM_TYPE_BCK))
		return TRUE;
	return FALSE;
}

DEFINE_NERVE(TNerveKoopaJrDemo, TLiveActor)
{
	TKoopaJr* self = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0)
		self->setAnimationIndex(0);
	return FALSE;
}

DEFINE_NERVE(TNerveKoopaJrLaunch, TLiveActor)
{
	TKoopaJr* self = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0)
		self->setAnimationIndex(1);
	if (self->getMActor()->isCurAnmAlreadyEnd(ANM_TYPE_BCK))
		return TRUE;
	return FALSE;
}

DEFINE_NERVE(TNerveKoopaJrYahoo, TLiveActor)
{
	TKoopaJr* self = (TKoopaJr*)spine->getBody();
	if (spine->getTime() == 0)
		self->setAnimationIndex(3);
	if (self->getMActor()->isCurAnmAlreadyEnd(ANM_TYPE_BCK))
		return TRUE;
	return FALSE;
}

TKoopaJrManager::TKoopaJrManager(const char* name)
    : TEnemyManager(name)
{
}

void TKoopaJrManager::createModelData()
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

void TKoopaJrManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk38 = new TKoopaJrParams("/enemy/koopajr.prm");
}

void TKoopaJrManager::loadAfter()
{
	static const char* onetimeFilenames[]
	    = { "/scene/koopajr/jpa/ms_koopajr_killer.jpa" };
	for (int i = 0; i < ARRAY_COUNT(onetimeFilenames); ++i)
		SMS_LoadParticle(onetimeFilenames[i],
		                 SCENE_TINKOOPA_JPA_MS_MKP_KILLER + i);
}

TSpineEnemy* TKoopaJrManager::createEnemyInstance() { return nullptr; }

TKoopaJrSubmarine::TKoopaJrSubmarine(const char* name)
    : TSpineEnemy(name)
    , unk164(0.0f)
    , unk16C(0.0f)
    , unk188(0.0f)
    , mKoopaJr(nullptr)
{
	offLiveFlag(LIVE_FLAG_NO_BIND);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
}

void TKoopaJrSubmarine::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	initAnmSound();
	f32 height = getParam()->mSLDamageHeight.get();
	initHitActor(0x08000020, 0, 0, 0.0f, 0.0f,
	             getParam()->mSLDamageRadius.get(), height);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	unk1A4 = new TCallbackHitActor("サブマリンリアボディ", 0x0800002D,
	                               getParam()->mSLDamageRadius.get(),
	                               getParam()->mSLDamageHeight.get(), this);
	unk1A8 = new TCallbackHitActor("サブマリンフロントボディ", 0x0800002D,
	                               getParam()->mSLDamageRadius.get(),
	                               getParam()->mSLDamageHeight.get(), this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("LastKoopaJrSubmarine.bmd", 0);
	mMActor->setLightType(1);
	mSpine->initWith(&TNerveKoopaJrSubmarineWait::theNerve());
	JUTNameTab* names = getModel()->getModelData()->getJointName();
	for (int i = 0; i < ARRAY_COUNT(TKoopaJr_jointNameTable); ++i)
		TKoopaJr_jointIndexTable[i]
		    = names->getIndex(TKoopaJr_jointNameTable[i]);
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

void TKoopaJrSubmarine::resetKoopaJrSubmarine()
{
	mSpine->reset();

	// Definitely fake, but int made stack too large here
	// I am fairly certain it is a for loop tho
	for (s8 i = 0; i < ARRAY_COUNT(unk150); ++i)
		unk150[i] = 0;

	setAnimationIndex(0);
	unk188 = mMActor->getFrameCtrl(ANM_TYPE_BCK)->getRate();
	unk180 = 0;
	unk184 = 0;
	for (int i = 0; i < ARRAY_COUNT(unk178); ++i)
		unk178[i] = 0;
	unk154.set(0.0f, 0.0f, 0.0f, 1.0f);
	unk16C.mDirection = 0.0f;
	unk164.mDirection = 0.0f;
	unk170            = 0;
	unk18C            = 0;
	unk190            = 0.0f;
	unk194            = 0.0f;
	unk198            = 0.0f;
	unk19C            = 0.0f;

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
	unk1A4->perform(cue, graphics);
	unk1A8->perform(cue, graphics);
}

void TKoopaJrSubmarine::makeCollisionPositions()
{
	f32 f31 = 0.0f;
	f32 f30 = 0.0f;
	f32 f29 = 0.0f;
	for (int i = 0; i < 2; ++i) {
		MtxPtr matrix = getModel()->getAnmMtx(TKoopaJr_getJointIndex(i + 3));
		f31 += matrix[0][3];
		f30 += matrix[1][3];
		f29 += matrix[2][3];
	}
	f31 *= 0.5f;
	f30 *= 0.5f;
	f29 *= 0.5f;
	unk1A4->mPosition.set(f31, f30, f29);
	getJointTransByIndex(TKoopaJr_getJointIndex(0), &unk1A8->mPosition);
}

void TKoopaJrSubmarine::moveSwing()
{
	if (unk18C) {
		unk18C = 0;
		unk190 += (M_PI * 2.0f) / 100.0f;
		f32 amplitude = unk190;
		f32 limit1    = getParam()->mSLSwingAmplitudeMax.get();
		unk190        = limit1 >= amplitude ? amplitude : limit1;
	}
	unk190 -= (M_PI / 100.0f) * 0.3f;
	f32 amplitude = unk190;
	f32 limit2    = getParam()->mSLSwingAmplitudeMin.get();
	unk190        = limit2 >= amplitude ? limit2 : amplitude;
	if (unk190 <= 0.0f)
		unk194 = 0.0f;
	unk194
	    = MsRepeat(unk194 + getParam()->mSLSwingPhaseVelocity.get(), 0.0f, TAU);
	f32 speed = mVelocity.length() / getParam()->mSLSpeedMax.get();

	for (int i = 0; i < ARRAY_COUNT(unk150); ++i) {
		if (unk150[i] > 0) {
			unk198 += M_PI / 100.0f;
			f32 amplitude = unk198;
			f32 limit3    = getParam()->mSLWaveAmplitudeMaxLaunch.get();
			unk198        = limit3 >= amplitude ? amplitude : limit3;
		}
	}
	if (speed > 0.5f) {
		unk198 += M_PI / 100.0f;
		f32 amplitude = unk198;
		f32 limit4    = getParam()->mSLWaveAmplitudeMax.get();
		unk198        = limit4 >= amplitude ? amplitude : limit4;
	}
	unk198 -= ((M_PI * 2.0f) / 100.0f) * 0.3f;
	amplitude  = unk198;
	f32 limit5 = getParam()->mSLWaveAmplitudeMin.get();
	unk198     = limit5 >= amplitude ? limit5 : amplitude;
	if (unk198 <= 0.0f)
		unk19C = 0.0f;
	unk19C
	    = MsRepeat(unk19C + getParam()->mSLWavePhaseVelocity.get(), 0.0f, TAU);
}
// UNUSED - 0x38 size from map
f32 TKoopaJrSubmarine::getSwingAngle() { return unk190 * sin(unk194); }
// UNUSED - 0x38 size from map
f32 TKoopaJrSubmarine::getWaveAngle() { return unk198 * sin(unk19C); }

void TKoopaJrSubmarine::bind()
{
	JGeometry::TVec3<f32> pos = mPosition;
	pos += mPositionDelta;
	pos += mVelocity;
	mPositionDelta = pos - mPosition;
	mBathtubBinder->bind(this);
}

void TKoopaJrSubmarine::calcRootMatrix()
{
	if (mKoopaJr->mBathtub->unk29A) {
		MtxPtr matrix = mKoopaJr->mBathtub->getSubmarineMtxInDemo();
		getModel()->setBaseTRMtx(matrix);
	} else {
		JGeometry::TQuat4<f32> swing, wave, yaw, rotation;
		swing.setEulerZ(getSwingAngle());
		wave.setEulerX(getWaveAngle());
		yaw.setEulerY(unk16C.mDirection);
		rotation.mul(yaw, swing);
		rotation.mul(rotation, wave);
		f32 centerZ = getParam()->centerZ.get();
		JGeometry::TVec3<f32> center(0.0f, 0.0f, centerZ);
		TPosition3f translate;
		translate.translation(center);
		JGeometry::TVec3<f32> position = center;
		position.negate();
		position += mPosition;
		TPosition3f matrix;
		matrix.setQT(rotation, position);
		matrix.concat(matrix, translate);
		getModel()->setBaseTRMtx(matrix);
	}
	getModel()->setBaseScale(mScaling);
}

BOOL TKoopaJrSubmarine::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);
		SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0,
		                              0.0f, 0, 0, 4);
		damageKoopaJrSubmarine();
		return TRUE;
	}
	return FALSE;
}

// UNUSED - 0xc size from map
void TKoopaJrSubmarine::damageKoopaJrSubmarine() { unk18C = 1; }

// UNUSED - 0x70 size from map
void TKoopaJrSubmarine::setAnimationIndex(int index)
{
	mMActor->setBckFromIndex(index);
	setAnmSound(getBas(index));
}
// UNUSED - 0x80 size from map
void TKoopaJrSubmarine::prepareKillerLaunch(int count)
{
	if (count > ARRAY_COUNT(unk178))
		count = ARRAY_COUNT(unk178);
	unk180 = 0;
	unk184 = count;
	for (int i = 0; i < unk184; ++i)
		unk178[i] = TBathtubKiller::KILLER_NORMAL;
	if (appearShineKiller(unk184))
		unk178[unk184 - 1] = TBathtubKiller::KILLER_SHINE;
}

// UNUSED - 0x84 size from map
void TKoopaJrSubmarine::prepareKillerLaunchFast(int count)
{
	if (count > ARRAY_COUNT(unk178))
		count = ARRAY_COUNT(unk178);
	unk180 = 0;
	unk184 = count;
	for (int i = 0; i < unk184; ++i)
		unk178[i] = TBathtubKiller::KILLER_FAST;
	if (appearShineKiller(unk184))
		unk178[unk184 - 1] = TBathtubKiller::KILLER_SHINE;
}

bool TKoopaJrSubmarine::appearShineKiller(int count)
{
	f32 probability;
	if (SMS_GetMarioWaterGun()->getCurrentWater() == 0) {
		probability = 0.5f;
	} else if (mKoopaJr->mKillerManager->mInitialLifeCount
	           == TFlagManager::getInstance()->getFlag(MSF_LIFE_COUNT)) {

		probability = 0.5f;
	} else {
		s32 maxWater
		    = SMS_GetMarioWaterGun()->getCurrentNozzle()->mAmountMax.get();
		s32 water      = SMS_GetMarioWaterGun()->getCurrentWater();
		f32 low        = getParam()->shineKillerProbability0.get();
		f32 high       = getParam()->shineKillerProbability1.get();
		f32 waterRatio = (f32)water / (f32)maxWater;
		probability    = waterRatio * (high - low) + low;
	}
	bool result = false;
	if (rand() * (1.0f / 32768.0f) < probability)
		result = true;
	return result;
}

// UNUSED - 0x100 size from map
void TKoopaJrSubmarine::checkKillerLaunch()
{
	if (mSpine->getCurrentNerve()
	        == &TNerveKoopaJrSubmarineLaunchKiller::theNerve()
	    && unk180 < unk184) {
		for (int i = 0; i < ARRAY_COUNT(unk150); ++i) {
			if (unk150[i] <= 0) {
				launchKiller();
				if (unk178[unk180] == TBathtubKiller::KILLER_FAST) {
					unk150[i] = getParam()->mSLKillerIntervalFast.get();
				} else {
					unk150[i] = getParam()->mSLKillerInterval.get();
				}
				++unk180;
				return;
			}
		}
	}
}

void TKoopaJrSubmarine::launchKiller()
{
	s32 index              = unk180 % 4;
	TBathtubKiller* killer = mKoopaJr->getKillerManager()->getDeadEnemy();
	if (killer == nullptr)
		return;
	killer->setKillerType(unk178[unk180]);
	killer->reset();
	int joint     = TKoopaJr_getJointIndex(index + 1);
	MtxPtr matrix = getModel()->getAnmMtx(joint);
	killer->mPosition.set(matrix[0][3], matrix[1][3], matrix[2][3]);
	JGeometry::TVec3<f32> direction;
	direction.x = matrix[0][2];
	direction.y = matrix[1][2];
	direction.z = matrix[2][2];
	makeKillerVelocity(killer, direction);
	SMSGetMSound()->startSoundActor(MSD_SE_EN_KILLER_FIRE, &killer->mPosition,
	                                0, nullptr, 0, 4);
}

void TKoopaJrSubmarine::makeKillerVelocity(TBathtubKiller* killer,
                                           JGeometry::TVec3<f32> direction)
{
	int killerType = unk178[unk180];
	if (killerType == TBathtubKiller::KILLER_FAST) {
		direction.set(0.0f, 1.0f, 0.0f);
		JGeometry::TVec3<f32> mario = SMS_GetMarioPos();
		JGeometry::TVec3<f32> toMario;
		toMario.sub(mario, killer->mPosition);
		toMario.y = 0.0f;
		toMario.normalize();
		JGeometry::TVec3<f32> axis;
		// TODO: TVec3::cross
		axis.x              = direction.y * toMario.z - direction.z * toMario.y;
		axis.y              = direction.z * toMario.x - direction.x * toMario.z;
		axis.z              = direction.x * toMario.y - direction.y * toMario.x;
		const f32 tiltAngle = M_PI / 5.0f;
		f32 spreadAngle     = tiltAngle;
		axis.normalize();
		JGeometry::TQuat4<f32> rotation;
		rotation.setRotate(axis, tiltAngle);
		rotation.rotate(direction);
		axis            = toMario;
		int launchIndex = unk180 % 4;
		if (launchIndex == 0)
			spreadAngle = -TDirectionCalc::d2r(9.0f);
		else if (launchIndex == 1)
			spreadAngle = TDirectionCalc::d2r(9.0f);
		else if (launchIndex == 2)
			spreadAngle = -TDirectionCalc::d2r(18.0f);
		else if (launchIndex == 3)
			spreadAngle = TDirectionCalc::d2r(18.0f);
		rotation.setRotate(axis, spreadAngle);
		rotation.rotate(direction);
		direction.normalize();
		direction.scale(killer->mPersonality.mInitialSpeed);
	} else {
		direction.normalize();
		direction.scale(killer->mPersonality.mInitialSpeed);
		JGeometry::TVec3<f32> mario = SMS_GetMarioPos();
		mario.y                     = mKoopaJr->getBathtubY();
		JGeometry::TVec3<f32> toMario;
		toMario.sub(mario, killer->mPosition);
		toMario.y = 0.0f;

		f32 distance
		    = toMario.length() - getParam()->killerTargetDistance.get();
		f32 targetDistance = getParam()->killerTargetDistanceMin.get();
		if (distance >= targetDistance)
			targetDistance = distance;

		toMario.normalize();
		toMario.scale(targetDistance);
		JGeometry::TVec3<f32> target;
		target.add(killer->mPosition, toMario);
		target.y  = mario.y;
		direction = calcVelocityToJumpToY(
		    target, direction.y,
		    killer->getSaveParam2()->mSLFlyingGravityY.get());
	}
	killer->makeInitialVelocity(direction);
}

// UNUSED - 0x4 size from map
void TKoopaJrSubmarine::emitKoopaJrSubmarineEffects()
{
	// Unimplemented
}

// UNUSED - 0x1c size from map
void TKoopaJrSubmarine::updateTimers()
{
	for (int i = 0; i < ARRAY_COUNT(unk150); ++i)
		if (unk150[i] > 0)
			--unk150[i];
}

// UNUSED - 0x8 size from map
void TKoopaJrSubmarine::setKoopaJr(TKoopaJr* actor) { mKoopaJr = actor; }

const char** TKoopaJrSubmarine::getBasNameTable() const
{
	return koopajrsubmarine_bastable;
}

void TKoopaJrSubmarine::makeRelativeAngle()
{
	f32 flameDirection
	    = TDirectionCalc::d2r(mKoopaJr->getKoopa()->getFlameDirDegree());
	f32 flameDifference = unk164.absDirection(flameDirection);

	JGeometry::TVec3<f32> direction;
	direction.sub(SMS_GetMarioPos(), mKoopaJr->getBathtub()->mPosition);
	direction.y = 0.0f;
	TDirectionCalc marioDirection(direction);
	f32 marioDifference = unk164.absDirection(marioDirection.mDirection);
	f32 target          = unk164.mDirection;
	if (mKoopaJr->getKoopa()->isFlaming()
	    && flameDifference <= getParam()->aboidKoopaFlameAngle.get())
		target = flameDirection + JGeometry::TUtil<f32>::PI();
	else if (marioDifference > getParam()->traceMarioAngle.get())
		target = marioDirection.mDirection;

	unk164.mDirection = unk164.calcTurnDirection(
	    target,
	    (M_PI * (1.0f / 180.0f)) * getParam()->mSLRoundAngleVelocity.get());
}

void TKoopaJrSubmarine::makeRoundVelocity()
{
	JGeometry::TVec3<f32> target = unk164.calcDirectionVector();
	target.scale(unk168);
	TBathtub* bathtub = mKoopaJr->getBathtub();
	JGeometry::TVec3<f32> direction(
	    bathtub->mPosition.x + target.x - mPosition.x, 0.0f,
	    bathtub->mPosition.z + target.z - mPosition.z);
	if (direction.length() < 100.0f) {
		unk170 = 1;
		return;
	}
	unk170 = 0;
	direction.normalize();
	direction.scale(getParam()->mSLAcceleration.get());
	mVelocity += direction;
	f32 speed    = mVelocity.length();
	f32 maxSpeed = getParam()->mSLSpeedMax.get();
	if (speed > maxSpeed) {
		mVelocity.normalize();
		mVelocity.scale(maxSpeed);
	}
}

// UNUSED - 0x18c size from map
void TKoopaJrSubmarine::makeDirection()
{
	if (!unk170) {
		JGeometry::TVec3<f32> direction = mVelocity;
		direction.normalize();
		TDirectionCalc calc(direction);
		f32 rotationSpeed
		    = TDirectionCalc::d2r(getParam()->mSLRotationSpeed.get());
		unk16C.mDirection
		    = unk16C.calcTurnDirection(calc.mDirection, rotationSpeed);
	}
}

void TKoopaJrSubmarine::checkNerve()
{
	if (mKoopaJr->getSpine()->getCurrentNerve()
	    == &TNerveKoopaJrWait::theNerve()) {
		makeRelativeAngle();
		unk168 = getParam()->mSLRoundDistance.get();
		makeRoundVelocity();
	}
	mVelocity.scale(0.95f);
	makeDirection();
	if (mSpine->getCurrentNerve() == &TNerveKoopaJrSubmarineWait::theNerve())
		return;
	if (mSpine->getCurrentNerve()
	    == &TNerveKoopaJrSubmarineCannonOpenClose::theNerve()) {
		if (unk180 == 0) {
			J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(ANM_TYPE_BCK);
			if (ctrl->checkPass(30.0f)) {
				ctrl->setRate(0.0f);
				pushNerve(&TNerveKoopaJrSubmarineLaunchKiller::theNerve());
			}
		}
	} else if (mSpine->getCurrentNerve()
	           == &TNerveKoopaJrSubmarineLaunchKiller::theNerve()) {
		emitKoopaJrSubmarineEffects();
	}
}

DEFINE_NERVE(TNerveKoopaJrSubmarineWait, TLiveActor)
{
	TKoopaJrSubmarine* self = (TKoopaJrSubmarine*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setAnimationIndex(0);
		self->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(0.0f);
	}
	return FALSE;
}

DEFINE_NERVE(TNerveKoopaJrSubmarineCannonOpenClose, TLiveActor)
{
	TKoopaJrSubmarine* self = (TKoopaJrSubmarine*)spine->getBody();
	if (spine->getTime() == 0) {
#if !defined(VERSION_GMSP01) && !defined(VERSION_GMSE01)
		self->setAnimationIndex(0);
#endif
		self->resumeAnimation();
	}
	if (self->getMActor()->isCurAnmAlreadyEnd(ANM_TYPE_BCK))
		return TRUE;
	return FALSE;
}

DEFINE_NERVE(TNerveKoopaJrSubmarineLaunchKiller, TLiveActor)
{
	TKoopaJrSubmarine* self = (TKoopaJrSubmarine*)spine->getBody();
	if (self->unk180 == self->unk184) {
		for (int i = 0; i < ARRAY_COUNT(self->unk150); ++i) {
			if (self->unk150[i] <= 0) {
				self->resumeAnimation();
				return TRUE;
			}
		}
	}
	return FALSE;
}

TKoopaJrSubmarineManager::TKoopaJrSubmarineManager(const char* name)
    : TEnemyManager(name)
{
}

void TKoopaJrSubmarineManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "LastKoopaJrSubmarine.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_MaterialTexGenFull
		      | J3DMLF_UseUniqueMaterials | (4 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TKoopaJrSubmarineManager::load(JSUMemoryInputStream& stream)
{
	getActiveObjNum();
	TEnemyManager::load(stream);
	unk38 = new TKoopaJrSubmarineParams("/enemy/koopajrsubmarine.prm");
	getActiveObjNum();
}

void TKoopaJrSubmarineManager::loadAfter()
{
	TEnemyManager::loadAfter();
	getActiveObjNum();
}

TSpineEnemy* TKoopaJrSubmarineManager::createEnemyInstance()
{
	return new TKoopaJrSubmarine;
}
