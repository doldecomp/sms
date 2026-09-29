#include <Camera/CameraShake.hpp>
#include <Enemy/BombHei.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/MapData.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Player/MarioAccess.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/BaseParam.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/ParamInst.hpp>
#include <System/Particles.hpp>
#include <System/Params.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

bool TBombHei::mSerialBomb = true;

static const char* bombhei_bastable[] = {
	"/scene/bombhei/bas/downnejibomb_down1.bas",  nullptr, nullptr,
	"/scene/bombhei/bas/nejibomb_land1.bas",      nullptr, nullptr,
	"/scene/bombhei/bas/nejibomb_stop_down1.bas",
};

TBombHeiSaveLoadParams::TBombHeiSaveLoadParams(const char* param_1)
    : TWalkerEnemyParams(param_1)
    , PARAM_INIT(mSLBombTime, 1000)
    , PARAM_INIT(mSLBombRange, 300.0f)
    , PARAM_INIT(mSLThrownVY, 50.0f)
    , PARAM_INIT(mSLThrownRateXZ, 0.5f)
    , PARAM_INIT(mSLThrownGravityY, 1.5f)
    , PARAM_INIT(mSLShootVelocity, 12.0f)
{
	TParams::load(mPrmPath);
}

TBombHeiManager::TBombHeiManager(const char* param_1)
    : TSmallEnemyManager(param_1)
    , unk60(0)
{
}

void TBombHeiManager::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemyManager::load(param_1);
	unk38 = new TBombHeiSaveLoadParams("/enemy/bombhei.prm");
}

void TBombHeiManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "nejibomb_model1.bmd", 0x10230000, 0 },
		{ "downnejibomb_model1.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TBombHeiManager::createEnemyInstance() { return new TBombHei; }

bool TBombHeiManager::canMakeDeadCoin()
{
	if (unk60 < 20) {
		unk60++;
		return true;
	}

	return false;
}

TBombHei::TBombHei(const char* name)
    : TWalkerEnemy(name)
    , unk194(nullptr)
    , unk198(0)
    , unk19C(1)
    , unk1A4(0)
{
}

void TBombHei::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);

	mActorType = 0x1000001E;
	unk150     = 0x11;
	unk194     = (TBombHeiSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveBombHeiGenerate::theNerve());
	if (mInstanceIndex == 0) {
		// TODO: what this loop did is unknown
		for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); ++i) { }
	}
}

void TBombHei::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("nejibomb_model1.bmd", 0);
	mMActorKeeper->createMActor("downnejibomb_model1.bmd", 3);
}

void TBombHei::behaveToWater(THitActor* param_1)
{
	if (isBckAnm(4) || isBckAnm(3)) {
		if (getHitPoints() == 0)
			mSpine->pushNerve(&TNerveBombHeiWaitExplosion::theNerve());
		mSprayedByWaterCooldown = 20;
	}
}

void TBombHei::changeOut()
{
	SMSGetMSound()->startSoundActor(MSD_SE_EN_TELSA_RECOVER, &mPosition, 0,
	                                nullptr, 0, 4);

	onLiveFlag(LIVE_FLAG_DEAD);
	genEventCoin();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mPosition = mJuiceBlock->mPosition;

	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_TLS_CHANGE,
	                                            &mPosition, 0, nullptr);
	getMActor()->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BCK);
	mJuiceBlock->kill();
	mJuiceBlock = nullptr;
}

bool TBombHei::isHitValid(u32 param_1)
{
	if (param_1 == HIT_MESSAGE_UNKB) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFlag(HIT_FLAG_NO_COLLISION);
		genEventCoin();
		return false;
	}

	return false;
}

void TBombHei::kill()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	mHitPoints = 1;
	if (mSpine->getCurrentNerve() != &TNerveBombHeiExplosion::theNerve()) {
		mSpine->reset();
		mSpine->setNext(&TNerveBombHeiExplosion::theNerve());
		mSpine->pushAfterCurrent(&TNerveBombHeiExplosion::theNerve());
	}
	onLiveFlag(LIVE_FLAG_UNK40);
}

void TBombHei::genEventCoin()
{
	TBombHeiManager* manager = (TBombHeiManager*)getManager();
	if (unk1A4 && manager->canMakeDeadCoin()) {
		TCoin* coin = (TCoin*)gpItemManager->makeObjAppear(
		    mPosition.x, mPosition.y, mPosition.z, 0x2000000E, true);
		if (coin) {
			coin->mPosition.y = mPosition.y;

			JGeometry::TVec3<f32> vec = SMS_GetMarioPos() - mPosition;
			MsVECNormalize(&vec, &vec);
			coin->mVelocity.set(vec.x * 20.0f, 20.0f, vec.z * 20.0f);
			coin->offLiveFlag(LIVE_FLAG_UNK10);
		}
	}
}

void TBombHei::setWalkAnm() { setBckAnm(4); }

void TBombHei::setFreezeAnm() { setBckAnm(1); }

void TBombHei::setDeadAnm()
{
	mMActor     = getActorKeeper()->getMActor("downnejibomb_model1.bmd");
	mRotation.y = TMsRange<f32>(0.0f, 360.0f).rand();
	unk19C      = 0;
	setBckAnm(0);
	gpCameraShake->startShake(CAM_SHAKE_MODE_UNK6, 1.0f);
	SMSRumbleMgr->start(0x15, 5, (f32*)nullptr);
}

void TBombHei::calcRootMatrix()
{
	TSpineEnemy::calcRootMatrix();
	if (gpMarDirector->checkFlag(0xF)) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFlag(HIT_FLAG_NO_COLLISION);
	}
	if (isBckAnm(0) && mMActor->getFrameCtrl(0)->checkPass(2.0f)) {
		TEffectExplosion* effect
		    = (TEffectExplosion*)gpConductor->makeOneEnemyAppear(
		        mPosition, "エフェクト爆発マネージャー", 1);
		if (effect)
			effect->generate(mPosition, mScaling);
	}
}

void TBombHei::attackToMario()
{
	if (mSpine->getCurrentNerve() == &TNerveBombHeiExplosion::theNerve())
		SMS_SendMessageToMario(this, HIT_MESSAGE_UNKA);
}

void TBombHei::behaveToTaken(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() == &TNerveBombHeiPickUp::theNerve())
		return;

	if (param_1->isActorType(0x80000001))
		unk1A4 = 1;

	mSpine->pushNerve(&TNerveBombHeiPickUp::theNerve());
}

void TBombHei::behaveToRelease()
{
	if (unk164 == 0
	    && mSpine->getCurrentNerve() != &TNerveBombHeiWalkExplosion::theNerve())
		return;

	if (mSpine->getCurrentNerve() == &TNerveBombHeiThrown::theNerve())
		return;

	mSpine->pushNerve(&TNerveBombHeiThrown::theNerve());
}

void TBombHei::bombIn()
{
	mSpine->pushNerve(&TNerveBombHeiExplosion::theNerve());
}

void TBombHei::reset()
{
	TWalkerEnemy::reset();
	unk198  = 0;
	unk1A4  = 0;
	unk164  = 0;
	unk19C  = 1;
	mMActor = mMActorKeeper->getMActor("nejibomb_model1.bmd");
}

f32 TBombHei::getGravityY() const
{
	if (mSpine->getCurrentNerve() == &TNerveBombHeiThrown::theNerve())
		return unk194->mSLThrownGravityY.get();
	return mGravity;
}

void TBombHei::walkBehavior(int param_1, f32 param_2)
{
	SMSGetMSound()->startSoundActor(MSD_SE_EN_BOMBHEI_ZENMAI, &mPosition, 0,
	                                nullptr, 0, 4);

	TWalkerEnemy::walkBehavior(param_1, param_2);
}

void TBombHei::moveObject()
{
	TWalkerEnemy::moveObject();
	if (mSpine->getCurrentNerve() != &TNerveBombHeiThrown::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveBombHeiExplosion::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveBombHeiGenerate::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveSmallEnemyChange::theNerve()) {
		++unk198;
		if (unk198 > unk194->mSLBombTime.get()) {
			unk164 = 0;
			unk198 = 0;
			if (mSpine->getCurrentNerve()
			    != &TNerveBombHeiWaitExplosion::theNerve())
				mSpine->pushNerve(&TNerveBombHeiWalkExplosion::theNerve());
		}
	}
}

bool TBombHei::isCollidMove(THitActor* param_1)
{
	if (mSerialBomb && param_1->isActorType(0x1000001E)
	    && ((TBombHei*)param_1)->isExplosion()) {
		if (mSpine->getCurrentNerve() != &TNerveBombHeiExplosion::theNerve())
			mSpine->pushNerve(&TNerveBombHeiExplosion::theNerve());
	}
	if (param_1->isActorType(0x08000013)) {
		if (mSpine->getCurrentNerve() == &TNerveBombHeiExplosion::theNerve())
			param_1->receiveMessage(this, HIT_MESSAGE_TRAMPLE);
		if (mSpine->getCurrentNerve() == &TNerveBombHeiThrown::theNerve())
			mSpine->pushNerve(&TNerveBombHeiExplosion::theNerve());
	}
	return true;
}

void TBombHei::forceKill()
{
	if (mGroundPlane->isIllegalData())
		return;

	if (mGroundPlane->isDeathPlane() || mGroundPlane->isPool()
	    || mGroundPlane->isWaterSurface()) {
		if (isAirborne())
			return;

		if (checkLiveFlag(LIVE_FLAG_UNK10))
			return;

		if (mSpine->getCurrentNerve() != &TNerveBombHeiExplosion::theNerve()) {
			mSpine->reset();
			mSpine->setNext(&TNerveBombHeiExplosion::theNerve());
			mSpine->pushAfterCurrent(mSpine->getDefault());

			onLiveFlag(LIVE_FLAG_UNK20000);
			mHitPoints = 1;
		}
	}
}

bool TBombHei::isExplosion()
{
	if (mSpine->getCurrentNerve() == &TNerveBombHeiExplosion::theNerve())
		return true;
	return false;
}

bool TBombHei::isDamageToCannon()
{
	if (mSpine->getCurrentNerve() == &TNerveBombHeiThrown::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveBombHeiExplosion::theNerve()) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		return true;
	}
	return false;
}

const char** TBombHei::getBasNameTable() const { return bombhei_bastable; }

DEFINE_NERVE(TNerveBombHeiGenerate, TLiveActor)
{
	TBombHei* self = (TBombHei*)spine->getBody();
	if (spine->getTime() == 0) {
		self->mMActor
		    = self->getActorKeeper()->getMActor("nejibomb_model1.bmd");
		self->setBckAnm(2);
		self->getMActor()->setBtpFromIndex(1);
		self->getMActor()->setFrameRate(0.0f, ANM_TYPE_BTP);
	}
	if (self->getHolder())
		self->getMActor()->setFrameRate(0.0f, ANM_TYPE_BCK);
	if (!self->isAirborne() && !self->getHolder()) {
		if (self->isBckAnm(2)) {
			self->setBckAnm(3);
		} else if (self->checkCurAnmEnd(0)) {
			spine->pushAfterCurrent(&TNerveBombHeiAttack::theNerve());
			return true;
		}
	} else if (!self->isBckAnm(2)) {
		self->mMActor
		    = self->getActorKeeper()->getMActor("nejibomb_model1.bmd");
		self->setBckAnm(2);
		self->getMActor()->setBtpFromIndex(1);
		self->getMActor()->setFrameRate(0.0f, ANM_TYPE_BTP);
	}
	return false;
}

DEFINE_NERVE(TNerveBombHeiAttack, TLiveActor)
{
	TBombHei* self = (TBombHei*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setWalkAnm();
		self->unk164 = 0;
		self->setGoalPath(TPathNode((THitActor*)gpMarioAddress));
	}
	self->walkBehavior(2, 1.0f);
	return false;
}

DEFINE_NERVE(TNerveBombHeiWalkExplosion, TLiveActor)
{
	TBombHei* self = (TBombHei*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(5);
		self->getMActor()->setBtpFromIndex(0);
	} else if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveBombHeiExplosion::theNerve());
		return true;
	}
	if ((int)self->getMActor()->getFrameCtrl(ANM_TYPE_BTP)->getFrame() % 40
	    == 0)
		SMSGetMSound()->startSoundActor(MSD_SE_EN_BOMBHEI_COUNT,
		                                &self->mPosition, 0, nullptr, 0, 4);
	self->walkBehavior(2, 0.6f);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_BOMB_LIMIT, self->getMActor()->getModel()->getAnmMtx(1), 1,
	    self);
	return false;
}

DEFINE_NERVE(TNerveBombHeiWaitExplosion, TLiveActor)
{
	TBombHei* self = (TBombHei*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(6);
		self->unk164 = 1;
	}
	if (self->unk164) {
		if (self->getMActor()->getFrameCtrl(0)->checkPass(60.0f))
			self->getMActor()->setFrameRate(0.0f, ANM_TYPE_BCK);
		if (self->getMActor()->getFrameCtrl(0)->checkPass(10.0f)) {
			self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BTP);
			if (self->getCurAnmFrameNo(ANM_TYPE_BTP) >= 1.0f)
				self->getMActor()->setFrameRate(0.0f, ANM_TYPE_BTP);
		}
	} else {
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BCK);
		if (!self->getMActor()->checkCurAnmFromIndex(0, ANM_TYPE_BTP))
			self->getMActor()->setBtpFromIndex(0);
		if (self->checkCurAnmEnd(0) && spine->getTime() > 150) {
			spine->pushAfterCurrent(&TNerveBombHeiExplosion::theNerve());
			return true;
		}
		if ((int)self->getMActor()->getFrameCtrl(ANM_TYPE_BTP)->getFrame() % 40
		    == 0)
			SMSGetMSound()->startSoundActor(MSD_SE_EN_BOMBHEI_COUNT,
			                                &self->mPosition, 0, nullptr, 0, 4);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_BOMB_LIMIT, self->getMActor()->getModel()->getAnmMtx(1),
		    1, self);
	}
	return false;
}

DEFINE_NERVE(TNerveBombHeiPickUp, TLiveActor)
{
	TBombHei* self = (TBombHei*)spine->getBody();

	if (spine->getTime() == 0 && self->unk164 == 0)
		return true;

	return false;
}

DEFINE_NERVE(TNerveBombHeiThrown, TLiveActor)
{
	TBombHei* self = (TBombHei*)spine->getBody();
	if (spine->getTime() == 0) {
		TBombHeiSaveLoadParams* params
		    = (TBombHeiSaveLoadParams*)self->getSaveParam();
		f32 power = *gpMarioThrowPower;
		f32 rate  = params->mSLThrownRateXZ.get();
		f32 c     = JMASCos(SMS_GetMarioAngleY());
		f32 s     = JMASSin(SMS_GetMarioAngleY());
		JGeometry::TVec3<f32> velocity;
		velocity.x = rate * (power * s);
		velocity.y = params->mSLThrownVY.get();
		velocity.z = rate * (power * c);
		self->setVelocity(velocity);
		self->mPosition.y += 2.0f;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
	}
	if (spine->getTime() == 120)
		self->offHitFlag(HIT_FLAG_NO_COLLISION);
	if (!self->isAirborne()) {
		self->genEventCoin();
		spine->pushAfterCurrent(&TNerveBombHeiExplosion::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBombHeiExplosion, TLiveActor)
{
	TBombHei* self = (TBombHei*)spine->getBody();
	if (spine->getTime() == 0) {
		TBombHeiSaveLoadParams* params
		    = (TBombHeiSaveLoadParams*)self->getSaveParam();
		self->unk1A0 = params->mSLBombRange.get() * self->getBodyScale()
		               / self->getAttackRadius();
		self->setDeadAnm();
		self->onLiveFlag(LIVE_FLAG_UNK8);
		if (self->getHolder() == gpMarioAddress)
			self->sendAttackMsgToMario();
		self->unk164 = 0;
		if (self->getGroundPlane()->isWaterSurface()) {
			TEffectBombColumWater* effect
			    = (TEffectBombColumWater*)gpConductor->makeOneEnemyAppear(
			        self->mPosition, "エフェクト爆発水柱マネージャー", 1);
			if (effect) {
				JGeometry::TVec3<f32> scale(2.0f, 2.0f, 2.0f);
				effect->generate(self->mPosition, scale);
			}
		}
		if (self->getGroundPlane()->isSand()) {
			TEffectColumSand* effect
			    = (TEffectColumSand*)gpConductor->makeOneEnemyAppear(
			        self->mPosition, "エフェクト砂柱マネージャー", 1);
			if (effect) {
				JGeometry::TVec3<f32> scale(0.7f, 0.7f, 0.7f);
				effect->generate(self->mPosition, scale);
			}
		}
	}
	if (self->unk190 < self->unk1A0) {
		self->unk190 *= 1.2f;
	} else if (self->checkCurAnmEnd(0)) {
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		self->onLiveFlag(LIVE_FLAG_DEAD);
		self->onLiveFlag(LIVE_FLAG_UNK8);
		self->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		self->mHolder = nullptr;
		self->stopAnmSound();
		spine->reset();
		spine->setNext(spine->getDefault());
		spine->pushAfterCurrent(spine->getDefault());
		return true;
	}
	self->expandCollision();
	return false;
}
