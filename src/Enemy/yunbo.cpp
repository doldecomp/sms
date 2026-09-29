#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/Yumbo.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JGadget/std-list.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Map/Map.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/BaseParam.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/ParamInst.hpp>
#include <System/Params.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TYumboSeed::TYumboSeed(MActor* param_1, const TYumbo& param_2)
    : THitActor("ユンボ種")
    , unk68(&param_2)
    , unk6C(param_1)
    , unk70(1)
{
}

void TYumboSeed::init()
{
	initHitActor(0x1000002A, 1, 0x80000000, 30.0f, 30.0f, 0.0f, 0.0f);
	TIdxGroupObj* group
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));
	group->getChildren().push_back(this);
}

void TYumboSeed::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk70 & 1)
		return;
	if (param_1 & 2) {
		TPosition3f matrix;
		matrix.identity33();
		matrix.setTrans(mPosition);
		unk6C->getModel()->setBaseScale(mScaling);
		unk6C->getModel()->setBaseTRMtx(matrix);
		unk6C->getModel()->calc();
	}
	if (param_1 & 1) {
		unk78.y -= ((TYumboParams*)unk68->getSaveParam())->mSeedGravityY.get();
		mPosition += unk78;
		unk78.scale(((TYumboParams*)unk68->getSaveParam())->mSeedAirFric.get());
		checkHitActors();
		if (--unk74 == 0) {
			unk70 |= 1;
			onHitFlag(HIT_FLAG_NO_COLLISION);
		}
	}
	if (!(unk70 & 4))
		unk6C->perform(param_1, param_2);
}

void TYumboSeed::checkHitActors()
{
	THitActor** end = mCollisions + mColCount;
	for (THitActor** it = mCollisions; it != end; ++it) {
		switch ((*it)->getActorType()) {
		case 0x80000001:
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
			unk70 |= 1;
			break;
		}
	}
}

void TYumboSeed::startToMove(const JGeometry::TVec3<f32>& param_1,
                             const JGeometry::TVec3<f32>& param_2, int param_3)
{
	unk70 &= ~1;
	mPosition = param_1;
	unk78     = param_2;
	unk74     = param_3;
	mScaling.set(2.0f, 2.0f, 2.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	onHitFlag(HIT_FLAG_CANNOT_GET_HIT);
}

TYumbo::TYumbo(const char* param_1)
    : TSmallEnemy(param_1)
{
	onLiveFlag(LIVE_FLAG_UNK10);
}

static const char* sambohead_bastable[] = {
	"/scene/sambohead/bas/flower_shoot.bas",
	"/scene/sambohead/bas/samboHead_crash.bas",
	"/scene/sambohead/bas/samboHead_dance.bas",
	"/scene/sambohead/bas/samboHead_down.bas",
	"/scene/sambohead/bas/samboHead_Fhide.bas",
	"/scene/sambohead/bas/samboHead_hit.bas",
	"/scene/sambohead/bas/samboHead_hit_end.bas",
	"/scene/sambohead/bas/samboHead_jump_end.bas",
	"/scene/sambohead/bas/samboHead_jump_start.bas",
	nullptr,
	"/scene/sambohead/bas/samboHead_set.bas",
	"/scene/sambohead/bas/samboHead_turn.bas",
	nullptr,
};

void TYumbo::init(TLiveManager* param_1)
{
	mManager = param_1;
	mManager->manageActor(this);
	initMActorAndKeeper();
	mSpine->initWith(&TNerveYumboDancing::theNerve());
	TYumboSeed** end = unk194 + 16;
	for (TYumboSeed** it = unk194; it != end; ++it) {
		*it = new TYumboSeed(getActorKeeper()->createMActor("samboSeed.bmd", 3),
		                     *this);
		(*it)->init();
	}
	initCollision();
	initAnmSound();
	unk1D4 = getModel()->getModelData()->getJointName()->getIndex("center");
}

void TYumbo::reset() { }

void TYumbo::initMActorAndKeeper()
{
	mMActorKeeper  = new TMActorKeeper(mManager, 18);
	MActor* yumbo  = getActorKeeper()->createMActor("yumbo.bmd", 0);
	MActor* flower = getActorKeeper()->createMActor("flower.bmd", 0);
	setMaterialToMActor(flower, ((TYumboManager*)mManager)->unk60);
	mMActor = yumbo;
}

void TYumbo::setMaterialToMActor(MActor* param_1, J3DMaterialTable* param_2)
{
	param_1->getModel()->getModelData()->setMaterialTable(param_2,
	                                                      J3DMatCopyFlag_All);
	param_1->initDL();
	param_1->getModel()->lock();
}

void TYumbo::initCollision()
{
	initHitActor(0x1000002A, 1, 0x80000000, 97.5f, 225.0f, 90.0f, 225.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mGroundHeight = gpMap->checkGround(mPosition.x, mPosition.y + mHeadHeight,
	                                   mPosition.z, &mGroundPlane);
	mScaledBodyRadius = 75.0f;
	mScaling.set(1.5f, 1.5f, 1.5f);
}

BOOL TYumbo::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return false;

	switch (param_2) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
		if (!isFreeze())
			return false;
		behaveHitAttack();
		return true;

	default:
		return TSmallEnemy::receiveMessage(param_1, param_2);
	}
}

void TYumbo::moveObject()
{
	updateEffect();
	updateCollision();
	updateSquareToMario();
	TSmallEnemy::moveObject();
}

void TYumbo::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);
	TYumboSeed** end = unk194 + 16;
	for (TYumboSeed** it = unk194; it != end; ++it)
		(*it)->perform(param_1, param_2);
}

void TYumbo::behaveToWater(THitActor* param_1)
{
	if (!isWaterproof()) {
		mSpine->reset();
		mSpine->setNext(&TNerveYumboFreeze::theNerve());
	}
}

void TYumbo::behaveHitAttack()
{
	mSpine->reset();
	mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
}

void TYumbo::updateCollision()
{
	if (isFreeze() || isDead())
		onHitFlag(HIT_FLAG_CANNOT_ATTACK);
	else
		offHitFlag(HIT_FLAG_CANNOT_ATTACK);
}

void TYumbo::updateEffect()
{
	if (isFreeze()) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_POI_KIZETSU, &mPosition, 1, this);
		if (emitter)
			emitter->setGlobalScale(mScaling);
	}
	if (mMActor->checkCurAnm("sambohead_dance", 0)) {
		MtxPtr mtx = getModel()->getAnmMtx(unk1D4);
		gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_YNB_ONPU, mtx,
		                                            1, this);
	}
}

bool TYumbo::isFindOutMario() const
{
	f32 height = ((TYumboParams*)getSaveParam())->getSLSearchHeight();
	if (abs(SMS_GetMarioPos().y - mPosition.y) < height) {
		JGeometry::TVec3<f32> pos(SMS_GetMarioPos().x, mPosition.y,
		                          SMS_GetMarioPos().z);
		f32 length = ((TYumboParams*)getSaveParam())->getSLSearchLength();
		f32 angle  = ((TYumboParams*)getSaveParam())->getSLSearchAngle();
		f32 aware  = ((TYumboParams*)getSaveParam())->getSLSearchAware();
		if (isInSight(pos, length, angle, aware))
			return true;
		return false;
	}
	return false;
}

bool TYumbo::isWantToAppear() const
{
	f32 height = ((TYumboParams*)getSaveParam())->getSLGiveUpHeight();
	if (height <= abs(SMS_GetMarioPos().y - mPosition.y))
		return true;
	JGeometry::TVec3<f32> diff = SMS_GetMarioPos();
	diff -= mPosition;
	diff.y     = 0.0f;
	f32 length = ((TYumboParams*)getSaveParam())->getSLGiveUpLength();
	return length * length < diff.squared();
}

bool TYumbo::isAllSeedBroken() const
{
	TYumboSeed* const* end = unk194 + 16;
	for (TYumboSeed* const* it = unk194; it != end; ++it) {
		if (!((*it)->unk70 & 1))
			return false;
	}
	return true;
}

void TYumbo::shotSeeds()
{
	TYumboSeed* seed = getUnusedSeed();
	if (!seed)
		return;

	mMActor->setBck(0);
	setCurAnmSound();
	JGeometry::TVec3<f32> dir = SMS_GetMarioPos();
	dir -= mPosition;
	dir.y += 200.0f * (0.5f + MsRandF());
	dir.setLength(((TYumboParams*)getSaveParam())->mShootSpeed.get());

	f32 angle = MsGetRotFromZaxisY(dir);
	JGeometry::TQuat4<f32> yaw;
	yaw.setEulerY(-(0.017453294f * angle));
	yaw.rotate(dir, dir);

	JGeometry::TQuat4<f32> spin;
	spin.setEulerY(2.0f * M_PI * MsRandF());
	JGeometry::TQuat4<f32> tilt;
	tilt.setEulerX(-M_PI * ((TYumboParams*)getSaveParam())->mShootAngleX.get());
	JGeometry::TQuat4<f32> rotation;
	rotation.mul(spin, tilt);
	rotation.rotate(dir, dir);

	seed->startToMove(mPosition, dir,
	                  ((TYumboParams*)getSaveParam())->mSeedLife.get());
}

void TYumbo::lookatMario()
{
	JGeometry::TVec3<f32> diff = SMS_GetMarioPos();
	diff -= mPosition;
	mRotation.y = MsGetRotFromZaxisY(diff);
}

void TYumbo::changeToFlower()
{
	mMActor = getActorKeeper()->getMActor("flower.bmd");
}

void TYumbo::changeToYumbo()
{
	mMActor = getActorKeeper()->getMActor("yumbo.bmd");
}

bool TYumbo::isWaterproof() const
{
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveYumboFreeze::theNerve()
	       || nerve == &TNerveSmallEnemyDie::theNerve()
	       || nerve == &TNerveYumboHiding::theNerve()
	       || nerve == &TNerveYumboAttack::theNerve();
}

bool TYumbo::isFreeze() const
{
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveYumboFreeze::theNerve();
}

bool TYumbo::isDead() const
{
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveSmallEnemyDie::theNerve();
}

TYumboSeed* TYumbo::getUnusedSeed()
{
	TYumboSeed** end = unk194 + 16;
	for (TYumboSeed** it = unk194; it != end; ++it) {
		TYumboSeed* seed = *it;
		if (seed->unk70 & 1)
			return seed;
	}
	return nullptr;
}

void TYumbo::setDeadAnm() { setBckAnm(3); }

bool TYumbo::doKeepDistance() { return isFreeze(); }

void TYumbo::attackToMario()
{
	if (!isFreeze())
		sendAttackMsgToMario();
}

TYumboParams::TYumboParams(const char* param_1)
    : TSmallEnemyParams(param_1)
    , PARAM_INIT(mRecoverTimer, 600)
    , PARAM_INIT(mShootSpeed, 55.0f)
    , PARAM_INIT(mShootAngleX, 0.2f)
    , PARAM_INIT(mSeedLife, 80)
    , PARAM_INIT(mSeedAirFric, 0.94f)
    , PARAM_INIT(mSeedGravityY, 0.9f)
{
	TParams::load(mPrmPath);
}

TYumboManager::TYumboManager(const char* param_1)
    : TSmallEnemyManager(param_1)
    , unk60(nullptr)
{
}

void TYumboManager::load(JSUMemoryInputStream& param_1)
{
	TYumboParams* params = new TYumboParams("/enemy/Yumbo.prm");

	unk38 = params;
	TSmallEnemyManager::load(param_1);

	params->mSLAttackRadius.set(97);
	params->mSLAttackHeight.set(225);
	params->mSLDamageRadius.set(90);
	params->mSLDamageHeight.set(225);
	loadMaterialTable(&unk60, "/scene/samboHead/flower_blue.bmt");
}

void TYumboManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "yumbo.bmd", 0x10210000, 0 },
		{ "flower.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TYumboManager::loadMaterialTable(J3DMaterialTable** param_1,
                                      const char* param_2)
{
	*param_1
	    = J3DModelLoaderDataBase::loadMaterialTable(JKRGetResource(param_2));
}

const char** TYumbo::getBasNameTable() const { return sambohead_bastable; }

DEFINE_NERVE(TNerveYumboDancing, TLiveActor)
{
	TYumbo* self = (TYumbo*)spine->getBody();
	if (spine->getTime() == 0)
		self->setBckAnm(2);
	self->lookatMario();
	if (self->isFindOutMario()) {
		spine->pushAfterCurrent(&TNerveYumboHiding::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveYumboHiding, TLiveActor)
{
	TYumbo* self = (TYumbo*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(4);
		self->getMActor()->getFrameCtrl(0)->setRate(3.0f
		                                            * SMSGetAnmFrameRate());
		self->unk1D8 = 0;
		SMSGetMSound()->startSoundActor(MSD_SE_EN_YUMBO_SINK, &self->mPosition,
		                                0, nullptr, 0, 4);
	}

	if (!self->unk1D8 && self->getMActor()->getFrameCtrl(0)->checkPass(34.0f)) {
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_AP_ROCK, &self->mPosition,
		                             0, nullptr);
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_AP_SMOKE, &self->mPosition,
		                             0, nullptr);
		self->unk1D8 = 1;
	}

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveYumboAttack::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveYumboAppearing, TLiveActor)
{
	TYumbo* self = (TYumbo*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeToYumbo();
		self->setBckAnm(10);
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_AP_ROCK, &self->mPosition,
		                             0, nullptr);
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_AP_SMOKE, &self->mPosition,
		                             0, nullptr);
	}

	self->lookatMario();

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveYumboDancing::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveYumboAttack, TLiveActor)
{
	TYumbo* self = (TYumbo*)spine->getBody();
	if (spine->getTime() == 0) {
		self->changeToFlower();
		self->shotSeeds();
	}
	if (self->isWantToAppear()) {
		spine->pushAfterCurrent(&TNerveYumboAppearing::theNerve());
		return true;
	}
	TYumboParams* params = (TYumboParams*)self->getSaveParam();
	if (params->mSeedLife.get() / 16 < spine->getTime()) {
		spine->pushAfterCurrent(this);
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveYumboFreeze, TLiveActor)
{
	TYumbo* self = (TYumbo*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(11);
		SMSGetMSound()->startSoundActor(MSD_SE_EN_COMMON_TWINKLE,
		                                &self->mPosition, 0, nullptr, 0, 4);
	}

	TYumboParams* params = (TYumboParams*)self->getSaveParam();
	if (params->mRecoverTimer.get() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveYumboDancing::theNerve());
		return true;
	}

	return false;
}
