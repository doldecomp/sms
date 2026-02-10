#include <Enemy/TamaNoko.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/question.hpp>
#include <Strategic/Spine.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MAnmSound.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/MarioMain.hpp>
#include <Camera/CameraShake.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/Conductor.hpp>
#include <NPC/NpcEvent.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const char* tamaNoko_bastable[] = {
	"/scene/tamaNoko/bas/tamaflower_grow1.bas",
	"/scene/tamaNoko/bas/tamanoko_appear1.bas",
	"/scene/tamaNoko/bas/tamanoko_attack1.bas",
	"/scene/tamaNoko/bas/tamanoko_down1.bas",
	"/scene/tamaNoko/bas/tamanoko_land1.bas",
	"/scene/tamaNoko/bas/tamanoko_mogaki1.bas",
	"/scene/tamaNoko/bas/tamanoko_mudland1.bas",
	"/scene/tamaNoko/bas/tamanoko_return1.bas",
	nullptr,
	"/scene/tamaNoko/bas/tamanoko_run1_loop.bas",
	nullptr,
	"/scene/tamaNoko/bas/tamanoko_sleep1.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/tamaNoko/bas/tamanoko_waterhit1.bas",
	"/scene/tamaNoko/bas/tamanoko_waterhit1_loop.bas",
};

int TTamaNoko::mStopOnAirTimeMax = 40;
int TTamaNoko::mRollOnAirTimeMax = 20;

f32 TTamaNoko::mRollOnAirAngle = 360.0f;
f32 TTamaNoko::mDropVelocityY  = -30.0f;

TTamaNokoFlower::TTamaNokoFlower(const TLiveActor* param_1, int param_2,
                                 SDLModelData* param_3, u32 param_4,
                                 const char* name)
    : TSharedParts(param_1, param_2, param_3, param_4, name)
    , unk1C(0)
    , unk2C(nullptr)
    , unk30(nullptr)
    , unk34(0)
    , unk35(0)
{
	unk2C = new MAnmSound(gpMSound);
	unk2C->initAnmSound(nullptr, 1, 0.0f);
}

void TTamaNokoFlower::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 1) {
		if (gpMarDirector->checkUnk124Thing1()) {
			if (unk35 != 0 && unk1C == 0) {
				if (gpMarDirector->checkUnk124Thing2()) {
					unk1C = 1;

					for (int i = 0; i < 5; ++i) {
						JGeometry::TVec3<f32> local_88(0.0f, 0.0f, 350.0f);
						Mtx local_b8;

						f32 s = JMASin((i + 1) * 72.0f);
						f32 c = JMACos((i + 1) * 72.0f);

						local_b8[0][0] = c;
						local_b8[0][1] = 0.0f;
						local_b8[0][2] = s;
						local_b8[0][3] = 0.0f;

						local_b8[1][0] = 0.0f;
						local_b8[1][1] = 1.0f;
						local_b8[1][2] = 0.0f;
						local_b8[1][3] = 0.0f;

						local_b8[2][0] = -s;
						local_b8[2][1] = 0.0f;
						local_b8[2][3] = 0.0f;
						local_b8[2][2] = c;

						MTXMultVec(local_b8, &local_88, &unk20);

						JGeometry::TVec3<f32> local_c4 = unk10->getPosition();
						if (TMapObjBase* mapObj = gpItemManager->makeObjAppear(
						        local_c4.x + local_88.x, local_c4.y,
						        local_c4.z + local_88.z, 0x2000000e, true)) {
							mapObj->mPosition.y = local_c4.y;
							MsVECNormalize(&local_88, &local_88);
							mapObj->mVelocity.set(local_88.x * 4.0f, 20.0f,
							                      local_88.z * 4.0f);
							mapObj->offLiveFlag(LIVE_FLAG_UNK10);
						}
					}
				}
			}

			if (unk35 == 0 && unk18->checkCurBckFromIndex(0)
			    && unk18->curAnmEndsNext()) {
				unk35 = 1;
				TNpcEvent::reviveOneSunflower();
			}
		}
	}

	if (param_1 & 2) {
		TPosition3f magic;
		magic.translation(unk10->mPosition.x, unk10->mPosition.y,
		                  unk10->mPosition.z);
		unk18->getModel()->setBaseTRMtx(magic);
		if (unk2C != nullptr && unk30 != 0) {
			J3DFrameCtrl* ctrl = unk18->getFrameCtrl(0);

			unk20 = unk10->mPosition;

			unk2C->animeLoop(&unk20, ctrl->getCurrentFrame(), ctrl->getRate(),
			                 0, 4);
		}
	}

	if (gpMarDirector->checkUnk124Thing2()
	    && gpMarDirector->checkUnk124Thing1()) {
		if (param_1 & 0x4)
			unk18->viewCalc();

		if (param_1 & 0x200) {
			unk18->entry();
			return;
		}
	}

	unk18->perform(param_1, param_2);
}

// TODO: 4 bytes too big
void TTamaNokoFlower::setBckAnm(int idx)
{
	getMActor()->setBckFromIndex(0);
	const char** basTable = unk10->getBasNameTable();
	unk30                 = !basTable ? nullptr : basTable[idx];
	if (unk30 != nullptr) {
		unk2C->initAnmSound(JKRGetResource(unk30), 1, 0.0f);
	} else {
		unk2C->initAnmSound(nullptr, 1, 0.0f);
	}
}

TTamaNokoSaveLoadParams::TTamaNokoSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
    , PARAM_INIT(mSLPickUpTime, 60)
    , PARAM_INIT(mSLJumpAttackDist, 200.0f)
    , PARAM_INIT(mSLThrownVY, 50.0f)
    , PARAM_INIT(mSLThrownRateXZ, 0.5f)
    , PARAM_INIT(mSLThrownGravityY, 1.5f)
    , PARAM_INIT(mSLJumpAttackSp, 10.0f)
    , PARAM_INIT(mSLAttackGravityY, 0.2f)
    , PARAM_INIT(mSLSinkTime, 500)
    , PARAM_INIT(mSLWakeUpTimer, 100)
{
	TParams::load(mPrmPath);
}

TTamaNokoManager::TTamaNokoManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TTamaNokoManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);

	unk38 = new TTamaNokoSaveLoadParams("/enemy/tamanoko.prm");
}

void TTamaNokoManager::loadAfter()
{
	TSmallEnemyManager::loadAfter();
	SMS_LoadParticle("/scene/tamaNoko/jpa/ms_mnt_kira.jpa", 0x70);
}

void TTamaNokoManager::initSetEnemies()
{
	void* data = JKRGetResource("/scene/tamaNoko/tamaflower_model1.bmd");
	SDLModelData* modelData
	    = new SDLModelData(J3DModelLoaderDataBase::load(data, 0x10220000));

	for (int i = 0; i < mObjNum; ++i) {
		TTamaNoko* enemy = (TTamaNoko*)unk18[i];
		enemy->unk19C
		    = new TTamaNokoFlower(enemy, 0, modelData, 0x3, "TamaNokoFlower");
	}
}

TSmallEnemy* TTamaNokoManager::createEnemyInstance() { return new TTamaNoko; }

void TTamaNokoManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "tamanoko_model1.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TTamaNoko::TTamaNoko(const char* name)
    : TWalkerEnemy(name)
    , mIsDoingJumpAttack(false)
    , mIsSunkInGround(false)
    , unk1B8(false)
    , unk1B9(true)
    , mWakeUpTimer(0)
{
}

void TTamaNoko::load(JSUMemoryInputStream& stream)
{
	TWalkerEnemy::load(stream);
	reset();
}

void TTamaNoko::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);

	mActorType = 0x10000018;
	unk150     = 0x11;
	mSpine->initWith(&TNerveTamaNokoSleep::theNerve());
	unk198 = (TTamaNokoSaveLoadParams*)getSaveParam();
	if (mInstanceIndex == 0) {
		for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); ++i)
			; // assert?
	}
}

void TTamaNoko::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("tamanoko_model1.bmd", 3);
}

void TTamaNoko::reset()
{
	TWalkerEnemy::reset();
	mIsDoingJumpAttack = false;
	mIsSunkInGround    = false;
	unk1B8             = false;
	unk1B9             = true;
	mScaledBodyRadius  = 280.0f;
	onLiveFlag(LIVE_FLAG_UNK1000);
}

void TTamaNoko::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);
	if (unk19C->unk34)
		unk19C->perform(param_1, param_2);
}

void TTamaNoko::moveObject()
{
	TWalkerEnemy::moveObject();
	if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
	    && isBckAnm(10) && checkCurAnmEnd(0))
		setBckAnm(9);
}

void TTamaNoko::behaveToFindMario()
{
	mSpine->pushAfterCurrent(&TNerveTamaNokoAttack::theNerve());
}

void TTamaNoko::behaveToWater(THitActor*)
{
	mSprayedByWaterCooldown = 0;
	unk165                  = 1;

	if (mSpine->getCurrentNerve() == &TNerveTamaNokoSleep::theNerve()) {
		mSpine->pushNerve(&TNerveTamaNokoHitWater::theNerve());
		return;
	}

	if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()) {
		mSpine->pushNerve(&TNerveTamaNokoAttack::theNerve());
		return;
	}
}

// TODO: fake
static inline JGeometry::TVec3<f32> fromPolar(f32 theta, f32 radius)
{
	return JGeometry::TVec3<f32>(radius * JMASSin(theta * (65536.0f / 360.0f)),
	                             0.0f,
	                             radius * JMASCos(theta * (65536.0f / 360.0f)));
}

void TTamaNoko::walkBehavior(int param_1, f32 param_2)
{
	mTurnSpeed  = getSaveParams2()->mSLTurnSpeedLow.get();
	mMarchSpeed = getSaveParams2()->mSLMarchSpeedLow.get();

	JGeometry::TVec3<f32> local_34 = unkF4.getPoint();
	local_34 -= mPosition;
	VECMag(&local_34);

	f32 rot  = MsWrap(MsGetRotFromZaxisY(local_34), 0.0f, 360.0f);
	f32 diff = MsAngleDiff(rot, mRotation.y);

	f32 fVar3;
	if (diff > 0.0f) {
		fVar3 = diff > mTurnSpeed * param_2 ? mTurnSpeed * param_2 : diff;
	} else {
		fVar3 = diff > -mTurnSpeed * param_2 ? diff : -mTurnSpeed * param_2;
	}

	mRotation.y = MsWrap(mRotation.y + fVar3, 0.0f, 360.0f);

	if (param_1 != 5 && param_1 != 3) {
		JGeometry::TVec3<f32> local_40 = mLinearVelocity;
		local_40 += fromPolar(mRotation.y, mMarchSpeed * param_2);
		mLinearVelocity = local_40;
	}

	if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
	    && mSpine->getTime() % 200 == 1) {
		forceSleep();
	}

	if (mWakeUpTimer > 0
	    && mSpine->getCurrentNerve() == &TNerveTamaNokoSleep::theNerve()) {
		++mWakeUpTimer;
		if (mWakeUpTimer > 400) {
			mWakeUpTimer = 0;
			mSpine->reset();
			mSpine->setNext(&TNerveTamaNokoHitWater::theNerve());
		}
	}
}

void TTamaNoko::behaveToTaken(THitActor*)
{
	if (mSpine->getCurrentNerve() != &TNerveTamaNokoPickUp::theNerve())
		mSpine->pushNerve(&TNerveTamaNokoPickUp::theNerve());
}

void TTamaNoko::behaveToRelease()
{
	if (unk164 != 0
	    && mSpine->getCurrentNerve() != &TNerveTamaNokoThrown::theNerve())
		mSpine->pushNerve(&TNerveTamaNokoThrown::theNerve());
}

BOOL TTamaNoko::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == 0 || param_2 == HIT_MESSAGE_HIP_DROP) {
		if (isHitValid(param_2)) {
			unk184 = 0;
			kill();
		}
		return true;
	}

	if (param_2 == 13) {
		mHitPoints = 0;
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFlag(HIT_FLAG_UNK1);
	}

	if (param_2 == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(0xE7, &param_1->mPosition, 0, nullptr);
		gpMSound->startSoundSet(0x6802, &mPosition, 0, 0.0f, 0, 0, 4);

		if (mSprayedByWaterCooldown == 0) {
			mSprayedByWaterCooldown = 1;
			if (!changeByJuice()) {
				decHpByWater(param_1);
				behaveToWater(param_1);
			}
		}

		return true;
	}

	return false;
}

bool TTamaNoko::isHitValid(u32 param_1)
{
	if (param_1 == 11)
		return false;

	if (!mIsSunkInGround || (isBckAnm(7) && getCurAnmFrameNo(0) > 80.0f))
		return false;

	if (param_1 == 1)
		return true;

	return false;
}

void TTamaNoko::attackToMario()
{
	if (unk164 == 0
	    && mSpine->getCurrentNerve() != &TNerveTamaNokoSleep::theNerve()
	    && (!mIsSunkInGround || !(SMS_GetMarioPos().y > mPosition.y + 10.0f))) {

		SMS_SendMessageToMario(this, 14);
	}
}

bool TTamaNoko::doKeepDistance()
{
	return mSpine->getCurrentNerve() == &TNerveTamaNokoSleep::theNerve()
	           ? true
	           : false;
}

void TTamaNoko::calcRootMatrix()
{
	if (isBckAnm(16)) {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToPosPtr(0x185, &mPosition, 1,
		                                                  this)) {
			emitter->setScale(mScaling);
		}
	}

	if (isBckAnm(11) && mMActor->getFrameCtrl(0)->getCurrentFrame() > 90.0f) {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToPosPtr(0x185, &mPosition, 1,
		                                                  this)) {
			emitter->setScale(mScaling);
		}
	}

	if (isBckAnm(1)) {
		if (mMActor->getFrameCtrl(0)->checkPass(0.0f)
		    || mMActor->getFrameCtrl(0)->checkPass(46.0f)) {
			gpMarioParticleManager->emitAndBindToPosPtr(0x11, &mPosition, 0,
			                                            nullptr);
		}

		if (mMActor->getFrameCtrl(0)->checkPass(25.0f)) {
			if (mGroundPlane->isSand()) {
				landEffect();
			} else {
				gpCameraShake->startShake(CAM_SHAKE_MODE_UNK7, 1.0f);
				SMSRumbleMgr->start(8, 1, (float*)nullptr);
				if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
				        0xB6, &mPosition, 0, nullptr)) {
					emitter->setScale(JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));
				}
				if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
				        0xB7, &mPosition, 0, nullptr)) {
					emitter->setScale(JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));
				}
			}
		}
	}

	if (isBckAnm(7)) {
		if (mMActor->getFrameCtrl(0)->checkPass(110.0f))
			landEffect();

		if (mMActor->getFrameCtrl(0)->checkPass(152.0f)) {
			const TBGCheckData* local_18;
			gpMap->checkGround(mPosition.x, mPosition.y + 500.0f, mPosition.z,
			                   &local_18);
			if (local_18 && !local_18->isWaterSurface()) {
				gpMarioParticleManager->emitAndBindToPosPtr(0x11, &mPosition, 0,
				                                            nullptr);
			}
		}
	}

	if (mSpine->getCurrentNerve() == &TNerveTamaNokoSleep::theNerve()) {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToPosPtr(0x124, &mPosition, 1,
		                                                  this)) {
			emitter->setScale(mScaling);
		}
	}

	TSpineEnemy::calcRootMatrix();
}

void TTamaNoko::requestShadow()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_UNK8 | LIVE_FLAG_UNK2)) {
		if (!checkLiveFlag(LIVE_FLAG_UNK200 | LIVE_FLAG_CLIPPED_OUT)
		    || checkLiveFlag(LIVE_FLAG_UNK400)) {
			TCircleShadowRequest local_2c;

			JGeometry::TVec3<f32> local_38;
			if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
				MActor* actor = getMActor();
				// TODO: incorrect inlines used here!
				local_38.x = actor->getModel()->getAnmMtx(1)[0][3];
				local_38.y = mGroundHeight;
				local_38.z = actor->getModel()->getAnmMtx(1)[2][3];
				if (!isAirborne())
					local_2c.unk1D = 0;
			} else {
				local_38 = mPosition;
				if (!isAirborne()) {
					local_2c.unk1D = 0;
					local_38.y     = mGroundHeight;
				}
			}

			local_2c.unk0  = local_38;
			local_2c.unkC  = mScaledBodyRadius;
			local_2c.unk10 = local_2c.unkC;
			local_2c.unk1C = getShadowType();
			local_2c.unk14 = mRotation.y;
			if (checkLiveFlag(LIVE_FLAG_UNK400)) {
				gpBindShadowManager->forceRequest(local_2c, getActorType());
			} else {
				gpBindShadowManager->request(local_2c, getActorType());
			}
		}

		if (!checkLiveFlag(LIVE_FLAG_UNK200 | LIVE_FLAG_CLIPPED_OUT)
		    && !checkActorType(ACTOR_TYPE_UNK40000000)) {
			gpQuestionManager->request(mPosition, mScaledBodyRadius);
		}
	}
}

#pragma dont_inline on
void TTamaNoko::landEffect()
{
	if (mGroundPlane->isSand()) {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0x14, &mPosition, 0, nullptr)) {
			emitter->setScale(mScaling);
		}
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0x53, &mPosition, 0, nullptr)) {
			emitter->setScale(mScaling);
		}
	}

	const TBGCheckData* local_10;
	gpMap->checkGround(mPosition.x, mPosition.y + 500.0f, mPosition.z,
	                   &local_10);
	if (local_10 && local_10->isWaterSurface()) {
		generateEffectColumWater();
	} else {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0x14, &mPosition, 0, nullptr)) {
			emitter->setScale(mScaling);
		}
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0x13, &mPosition, 0, nullptr)) {
			emitter->setScale(mScaling);
		}
	}

	gpCameraShake->startShake(CAM_SHAKE_MODE_UNK7, 1.0f);
	SMSRumbleMgr->start(8, 1, (float*)nullptr);
}
#pragma dont_inline off

void TTamaNoko::forceWakeUp() { }

void TTamaNoko::forceSleep()
{
	updateSquareToMario();
	f32 giveUpLen = getSaveParams2()->mSLGiveUpLength.get();
	if (mDistToMarioSquared > giveUpLen * giveUpLen) {
		setBckAnm(11);
		mSpine->reset();
		mSpine->setNext(mSpine->getDefault());
	}
}

void TTamaNoko::setAfterDeadEffect()
{
	TSmallEnemy::setAfterDeadEffect();
	unk19C->unk34 = 1;
	unk19C->setBckAnm(0);

	gpMarioParticleManager->emitAndBindToPosPtr(0xCC, &mPosition, 0, nullptr);
	if (gpMSound->gateCheck(0x295F))
		MSoundSESystem::MSoundSE::startSoundActor(0x295F, &mPosition, 0,
		                                          nullptr, 0, 4);
}

const char** TTamaNoko::getBasNameTable() const { return tamaNoko_bastable; }

f32 TTamaNoko::getGravityY() const
{
	if (mSpine->getCurrentNerve() == &TNerveTamaNokoAttack::theNerve())
		return unk198->mSLAttackGravityY.get();

	if (mSpine->getCurrentNerve() == &TNerveTamaNokoThrown::theNerve())
		return unk198->mSLThrownGravityY.get();

	return mGravity;
}

void TTamaNoko::setWalkAnm() { setBckAnm(10); }

void TTamaNoko::setDeadAnm() { setBckAnm(3); }

BOOL TTamaNoko::isReachedToGoal() const
{
	JGeometry::TVec3<f32> pos = unk104.getPoint();
	pos -= mPosition;
	pos.y = 0.0f;
	if (pos.x == 0.0f && pos.z == 0.0f)
		return true;

	if (MsVECMag2(&pos) < 200.0f)
		return true;

	return false;
}

bool TTamaNoko::isCollidMove(THitActor* param_1)
{
	if (param_1->getActorType() == getActorType())
		return true;

	if (mSpine->getCurrentNerve() == &TNerveTamaNokoSleep::theNerve()) {
		param_1->receiveMessage(this, 0);
		return true;
	}

	return false;
}

// NOTE: tamanoko is sleeping inside of it's shell, zzzz
DEFINE_NERVE(TNerveTamaNokoSleep, TLiveActor)
{
	TTamaNoko* self = (TTamaNoko*)spine->getBody();

	if (!self->isBckAnm(15) && self->checkCurAnmEnd(0)) {
		self->setBckAnm(15);
		self->getMActor()->setFrameRate(0, 0.0f);
	}

	self->walkBehavior(5, 0.0f);
	return false;
}

// 3 in 1: chase mario & try to initiate & land a jump attack
DEFINE_NERVE(TNerveTamaNokoAttack, TLiveActor)
{
	TTamaNoko* self = (TTamaNoko*)spine->getBody();

	// Initially, start chasing mario
	if (spine->getTime() == 0) {
		if (!self->isBckAnm(9))
			self->setBckAnm(10);

		self->setGoalPathMario();
	}

	JGeometry::TVec3<f32> local_48 = self->getVelocity();
	if (local_48.y < -1.0f) {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        0x186, self->getMActor()->getModel()->getAnmMtx(1), 1, self)) {
			emitter->setScale(self->mScaling);
		}
	}

	// If doing jump attack -- we're basically playing out the jump arc,
	// a tiny lil 360 and then a crash landing.
	if (self->mIsDoingJumpAttack) {
		if (!self->isAirborne()) {
			self->landEffect();
			self->mIsDoingJumpAttack = false;
			spine->pushAfterCurrent(&TNerveTamaNokoDown::theNerve());
			return true;
		}

		JGeometry::TVec3<f32> local_54 = self->getVelocity();
		if (local_54.y < 0.0f) {
			MtxPtr mtx = self->getMActor()->getModel()->getAnmMtx(1);
			self->unk1AC.set(mtx[0][3], mtx[1][3] - 200.0f, mtx[2][3]);
			if (JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToPosPtr(
			        0x105, &self->unk1AC, 1, self)) {
				emitter->setScale(self->mScaling);
			}

			if (self->mAirAttackTimer < TTamaNoko::mStopOnAirTimeMax) {
				self->onLiveFlag(LIVE_FLAG_UNK10);

				// NOTE: at the top of the arc, we do a lil 360 spin
				f32 delta
				    = TTamaNoko::mRollOnAirAngle / TTamaNoko::mRollOnAirTimeMax;
				if (self->mAirAttackTimer < TTamaNoko::mRollOnAirTimeMax)
					self->mRotation.y += delta;

				++self->mAirAttackTimer;
			} else {
				self->setVelocity(JGeometry::TVec3<f32>(
				    0.0f, TTamaNoko::mDropVelocityY, 0.0f));
				self->offLiveFlag(LIVE_FLAG_UNK10);
			}
		}
	} else {
		// Not doing jump attack -- chase mario & try to do the jump attack
		if (self->checkCurAnmEnd(0)) {
			if (self->isBckAnm(10)) {
				self->setBckAnm(9);
			} else if (self->isBckAnm(9)) {
				self->updateSquareToMario();
				f32 searchLen = self->getSaveParams2()->mSLSearchLength.get();
				if (searchLen * searchLen < self->getDistToMarioSquared()) {
					self->unk1B8 = true;
					self->setBckAnm(8);
				} else {
					f32 jumpAttackDist
					    = self->getSaveParams2()->mSLJumpAttackDist.get();
					if (!self->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
					    && self->getDistToMarioSquared()
					           < jumpAttackDist * jumpAttackDist)
						self->setBckAnm(8);
				}
			} else if (self->isBckAnm(8)) {
				if (self->unk1B8) {
					self->unk1B8 = false;
					spine->pushAfterCurrent(
					    &TNerveWalkerGraphWander::theNerve());
					return true;
				}

				if (SMS_CheckMarioFlag(0x10000)
				    || SMS_IsMarioStatusTypeSwimming()
				    || SMS_GetMarioGrPlane()->isWaterSurface()) {
					spine->pushAfterCurrent(
					    &TNerveWalkerGraphWander::theNerve());
					return true;
				}

				if (gpMarioOriginal->isInvincible())
					spine->pushNerve(&TNerveTamaNokoWait::theNerve());

				self->setBckAnm(2);
			}
		}

		// The "preparing to jump" animation?
		if (self->isBckAnm(2)) {
			if (self->getMActor()->getFrameCtrl(0)->checkPass(63.0f)) {
				// ...and jump attack!
				self->unk1A0 = self->calcVelocityToJumpToY(
				    SMS_GetMarioPos(),
				    self->getSaveParams2()->getJumpAttackSp(),
				    self->getGravityY());
				self->unk1A0.x *= 2.0f;
				self->unk1A0.z *= 2.0f;
				self->mPosition.y += 30.0f;
				self->setVelocity(self->unk1A0);
				self->onLiveFlag(LIVE_FLAG_AIRBORNE);
				self->mIsDoingJumpAttack = true;
				self->mAirAttackTimer    = 0;
			}
			self->walkBehavior(3, 0.5f);
		} else {
			self->walkBehavior(2, 1.0f);
		}
	}

	return false;
}

// Landing after jump attack
DEFINE_NERVE(TNerveTamaNokoDown, TLiveActor)
{
	TTamaNoko* self = (TTamaNoko*)spine->getBody();

	if (spine->getTime() == 0) {
		// If hit sand -- we get stuck
		if (self->getGroundPlane()->isSand()) {
			spine->pushAfterCurrent(&TNerveTamaNokoSink::theNerve());
			if (TEffectColumSand* sand
			    = (TEffectColumSand*)gpConductor->makeOneEnemyAppear(
			        self->mPosition, "エフェクト砂柱マネージャー", 1)) {
				JGeometry::TVec3<f32> local_1c(1.2f, 0.5f, 1.2f);
				sand->generate(self->mPosition, local_1c);
			}
			return true;
		}

		if (gpMSound->gateCheck(0x28F9))
			MSoundSESystem::MSoundSE::startSoundActor(0x28F9, &self->mPosition,
			                                          0, nullptr, 0, 4);
		self->unk164 = 1;
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK7, 1.0f);
		self->setBckAnm(4);
	}

	if (spine->getTime() == 10)
		SMSRumbleMgr->start(0x13, &self->mPosition);

	// If didn't land on sand -- we pick ourselves back up after a moment
	int pickUpTime = self->getSaveParams2()->mSLPickUpTime.get();
	if (self->checkCurAnmEnd(0) && spine->getTime() > pickUpTime) {
		self->unk164 = 0;
		spine->pushAfterCurrent(&TNerveTamaNokoWait::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveTamaNokoPickUp, TLiveActor)
{
	TTamaNoko* self = (TTamaNoko*)spine->getBody();

	if (spine->getTime() == 0 && self->unk164 == 0)
		return true;

	if (self->checkCurAnmEnd(0)
	    && spine->getTime() > self->getSaveParams2()->mSLPickUpTime.get()) {
		self->unk164 = 0;
		self->attackToMario();
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveTamaNokoThrown, TLiveActor)
{
	TTamaNoko* self = (TTamaNoko*)spine->getBody();

	if (spine->getTime() == 0) {
		TTamaNokoSaveLoadParams* params = self->getSaveParams2();

		int angle = *gpMarioAngleY;
		f32 fVar2 = *gpMarioThrowPower;
		f32 s     = JMASSin(angle);
		f32 c     = JMASCos(angle);
		f32 fVar3 = params->mSLThrownRateXZ.get();
		f32 fVar4 = params->mSLThrownVY.get();

		self->setVelocity(
		    JGeometry::TVec3<f32>(fVar3 * fVar2 * c, fVar4, fVar3 * fVar2 * s));

		self->mPosition.y += 2.0f;

		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	if (!self->isAirborne()) {
		self->unk164 = 0;
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveTamaNokoSink, TLiveActor)
{
	TTamaNoko* self = (TTamaNoko*)spine->getBody();

	if (spine->getTime() == 0) {
		self->mIsSunkInGround = true;
		self->setBckAnm(6);
	}

	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(6))
			self->setBckAnm(5);

		int sinkTime              = self->getSaveParams2()->mSLSinkTime.get();
		TTamaNokoManager* manager = (TTamaNokoManager*)self->mManager;

		// The less tamanokos remain, the faster they get back up
		f32 fVar1 = 0.0f;
		for (int i = 0; i < manager->getObjNum(); ++i)
			if (!manager->getObj(i)->checkLiveFlag(LIVE_FLAG_DEAD))
				fVar1 += 1.0f;
		fVar1 /= manager->getObjNum();

		sinkTime *= fVar1;
		if (spine->getTime() > sinkTime) {
			if (self->isBckAnm(7)) {
				self->mIsSunkInGround = false;
				spine->pushAfterCurrent(&TNerveTamaNokoWait::theNerve());
				return true;
			}
			self->setBckAnm(7);
		}
	}

	return false;
}

// NOTE: lil shaking when mario sprays water on a sleeping tamanoko
DEFINE_NERVE(TNerveTamaNokoHitWater, TLiveActor)
{
	TTamaNoko* self = (TTamaNoko*)spine->getBody();

	if (spine->getTime() < 2) {
		self->setBckAnm(15);
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
	}

	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(15)) {
			self->setBckAnm(16);
		} else if (self->isBckAnm(16)) {
			if (spine->getTime() > self->unk198->mSLWakeUpTimer.get()
			    || !self->unk1B9) {
				self->unk1B9 = false;
				if (!self->isBckAnm(1))
					self->setBckAnm(1);
			} else if (self->unsetUnk165()) {
				self->setBckAnm(16);
			} else {
				spine->reset();
				spine->setNext(&TNerveTamaNokoSleep::theNerve());
				spine->pushAfterCurrent(&TNerveTamaNokoSleep::theNerve());
				return true;
			}
		} else if (self->isBckAnm(1)) {
			spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
			spine->pushAfterCurrent(&TNerveTamaNokoWait::theNerve());
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNerveTamaNokoWait, TLiveActor)
{
	TTamaNoko* self = (TTamaNoko*)spine->getBody();

	if (spine->getTime() < 2) {
		self->setBckAnm(14);
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
	}

	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(14)) {
			self->setBckAnm(13);
		} else if (self->isBckAnm(13)) {
			if (spine->getTime() > self->unk198->mSLWaitTime.get()
			    && self->isBckAnm(13))
				self->setBckAnm(12);
		} else if (self->isBckAnm(12)) {
			spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
			return true;
		}
	}

	return false;
}
