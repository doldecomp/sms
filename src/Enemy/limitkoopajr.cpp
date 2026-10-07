#include <Enemy/LimitKoopaJr.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjModel.hpp>

TLimitKoopaJrParams::TLimitKoopaJrParams(const char* param_1)
    : TSpineEnemyParams(param_1)
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

TLimitKoopaJr::TLimitKoopaJr(const char* param_1)
    : TSpineEnemy(param_1)
    , mBathtub(nullptr)
{
	onLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
}

void TLimitKoopaJr::init(TLiveManager* param_1)
{
	mManager = param_1;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("koopajr_model.bmd", 0);
	mMActor->setLightType(1);
	initAnmSound();
	f32 damageHeight
	    = ((TLimitKoopaJrParams*)getSaveParam())->mSLDamageHeight.get();
	initHitActor(ACTOR_TYPE_LIMIT_KOOPA_JR, 1, 0, 0.0f, 0.0f,
	             ((TLimitKoopaJrParams*)getSaveParam())->mSLDamageRadius.get(),
	             damageHeight);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	mSpine->initWith(&TNerveLimitKoopaJrRun::theNerve());
	f32 scale = ((TLimitKoopaJrParams*)getSaveParam())->mSLKoopaJrScale.get();
	mScaling.set(scale, scale, scale);
	resetLimitKoopaJr();
	getModel()->getModelData()->onFlag1OnAllShapes();
}

void TLimitKoopaJr::reset()
{
	TSpineEnemy::reset();
	resetLimitKoopaJr();
}

void TLimitKoopaJr::resetLimitKoopaJr()
{
	mSpine->reset();
	unk158[0] = 0;
	unk158[1] = 0;
	unk158[1]
	    = ((TLimitKoopaJrParams*)getSaveParam())->mSLShotDoodlePeriod.get();
	unk160      = 0.0f;
	unk164      = 0.0f;
	unk168      = 0.0f;
	unk16C      = 1.0f;
	unk178.unk0 = 0.0f;
	unk170.unk0 = 0.0f;
}

void TLimitKoopaJr::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (mKoopa == nullptr) {
		TEnemyManager* manager = static_cast<TEnemyManager*>(
		    JDrama::TNameRefGen::search("クッパマネージャー"));
		mKoopa = (TKoopa*)manager->getObj(0);
	}
	if (mBathtub == nullptr)
		mBathtub
		    = static_cast<TBathtub*>(JDrama::TNameRefGen::search("バスタブ"));
	if (param_1 & CUE_MOVE) {
		updateTimers();
		checkNerve();
	}
	TSpineEnemy::perform(param_1, param_2);
}

void TLimitKoopaJr::bind() { }

void TLimitKoopaJr::calcRootMatrix()
{
	JGeometry::TQuat4<f32> quat;
	quat.setEulerY(unk178.unk0);
	TPosition3f matrix;
	matrix.setQT(quat, mPosition);
	getModel()->setBaseTRMtx(matrix);
	getModel()->setBaseScale(mScaling);
}

void TLimitKoopaJr::startKoopaJrMessage(u32 param_1) { }

void TLimitKoopaJr::emitKoopaJrEffects() { }

void TLimitKoopaJr::setAnimationIndex(int index)
{
	getMActor()->setBckFromIndex(index);
	setAnmSound(getBas(index));
}

void TLimitKoopaJr::updateTimers()
{
	for (int i = 0; i < 2; i++)
		if (unk158[i] > 0)
			unk158[i]--;
}

static const char* koopajr_bastable[] = {
	"/scene/koopajr/bas/koopajr_damage.bas",
	"/scene/koopajr/bas/koopajr_shoot.bas",
	nullptr,
	"/scene/koopajr/bas/koopajr_yahoo.bas",
};

const char** TLimitKoopaJr::getBasNameTable() const { return koopajr_bastable; }

BOOL TLimitKoopaJr::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == HIT_MESSAGE_SPRAYED_BY_WATER)
		return TRUE;
	return FALSE;
}

void TLimitKoopaJr::checkNerve()
{
	// TODO: emitKoopaJrEffects is empty, so this test may be negated
	if (mSpine->getCurrentNerve() == &TNerveLimitKoopaJrWait::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveLimitKoopaJrRun::theNerve())
		emitKoopaJrEffects();
}

void TLimitKoopaJr::moveRun()
{
	f32 angleVelocity
	    = 0.017453294f
	      * ((TLimitKoopaJrParams*)getSaveParam())->mSLRoundAngleVelocity.get();
	TDirectionCalc direction(
	    unk170.calcTurnDirection(calcTargetDirection(), angleVelocity));
	f32 turn     = direction.sub(unk170.unk0);
	unk170.unk0  = direction.unk0;
	mRoundRadius = ((TLimitKoopaJrParams*)getSaveParam())->mSLRoundRadius.get();
	JGeometry::TVec3<f32> offset = unk170.calcDirectionVector();
	offset.scale(mRoundRadius);
	JGeometry::TVec3<f32> position;
	position.add(mBathtub->mPosition, offset);
	mPosition.set(position);
	mPosition.y = ((TLimitKoopaJrParams*)getSaveParam())->mSLRoundHeight.get();
	offset.normalize();
	JGeometry::TVec3<f32> tangent;
	tangent.cross(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), offset);
	tangent.normalize();
	if (turn < 0.0f)
		tangent.negate();
	makeDirection(tangent);
}

bool TLimitKoopaJr::canRun()
{
	if (unk170.absDirection(calcTargetDirection()) <= 0.62831855f)
		return false;
	return true;
}

bool TLimitKoopaJr::canYahoo() { return SMS_IsMarioStatusTypeJumping(); }

void TLimitKoopaJr::moveWait()
{
	JGeometry::TVec3<f32> offset;
	offset.sub(SMS_GetMarioPos(), mPosition);
	offset.y = 0.0f;
	makeDirection(offset);
}

f32 TLimitKoopaJr::calcTargetDirection()
{
	TDirectionCalc direction;
	JGeometry::TVec3<f32> offset;
	offset.sub(mBathtub->mPosition, SMS_GetMarioPos());
	offset.y = 0.0f;
	direction.makeDirection(offset);
	return direction.unk0;
}

void TLimitKoopaJr::makeDirection(JGeometry::TVec3<f32> param_1)
{
	param_1.normalize();
	TDirectionCalc direction(param_1);
	f32 rotationSpeed = TDirectionCalc::d2r(
	    ((TLimitKoopaJrParams*)getSaveParam())->mSLRotationSpeed.get());
	unk178.unk0 = unk178.calcTurnDirection(direction.unk0, rotationSpeed);
}

DEFINE_NERVE(TNerveLimitKoopaJrRun, TLiveActor)
{
	TLimitKoopaJr* actor = (TLimitKoopaJr*)spine->getBody();
	if (spine->getTime() == 0) {
		actor->setAnimationIndex(2);
	}
	if (!actor->canRun()) {
		spine->pushAfterCurrent(&TNerveLimitKoopaJrWait::theNerve());
		return true;
	}
	actor->moveRun();
	return false;
}

DEFINE_NERVE(TNerveLimitKoopaJrWait, TLiveActor)
{
	TLimitKoopaJr* actor = (TLimitKoopaJr*)spine->getBody();
	if (spine->getTime() == 0) {
		actor->setAnimationIndex(2);
	}
	if (actor->canRun()) {
		spine->pushAfterCurrent(&TNerveLimitKoopaJrRun::theNerve());
		return true;
	}
	if (actor->canYahoo()) {
		spine->pushAfterCurrent(&TNerveLimitKoopaJrYahoo::theNerve());
		return true;
	}
	if (actor->unk158[1] <= 0) {
		spine->pushAfterCurrent(&TNerveLimitKoopaJrLaunch::theNerve());
		return true;
	}
	actor->moveWait();
	return false;
}

DEFINE_NERVE(TNerveLimitKoopaJrLaunch, TLiveActor)
{
	TLimitKoopaJr* actor = (TLimitKoopaJr*)spine->getBody();
	if (spine->getTime() == 0) {
		actor->setAnimationIndex(1);
		actor->unk158[1] = ((TLimitKoopaJrParams*)actor->getSaveParam())
		                       ->mSLShotDoodlePeriod.get();
	}

	if (actor->getMActor()->curAnmEndsNext()) {
		spine->pushAfterCurrent(&TNerveLimitKoopaJrWait::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveLimitKoopaJrYahoo, TLiveActor)
{
	TLimitKoopaJr* actor = (TLimitKoopaJr*)spine->getBody();
	if (spine->getTime() == 0) {
		actor->setAnimationIndex(3);
	}

	if (actor->getMActor()->curAnmEndsNext()) {
		spine->pushAfterCurrent(&TNerveLimitKoopaJrWait::theNerve());
		return true;
	}
	return false;
}

TLimitKoopaJrManager::TLimitKoopaJrManager(const char* param_1)
    : TEnemyManager(param_1)
{
}

void TLimitKoopaJrManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "koopajr_model.bmd", 0x14240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TLimitKoopaJrManager::load(JSUMemoryInputStream& param_1)
{
	TEnemyManager::load(param_1);
	unk38 = new TLimitKoopaJrParams("/enemy/limitkoopajr.prm");
}

void TLimitKoopaJrManager::loadAfter() { }

TSpineEnemy* TLimitKoopaJrManager::createEnemyInstance() { return nullptr; }
