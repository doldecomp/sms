#include <Enemy/PoiHana.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <System/EmitterViewObj.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/Spine.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Map/MapData.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/Item.hpp>
#include <Player/MarioAccess.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

const char* poihana_bastable[] = {
	"/scene/poihana/bas/poihana_dash.bas",
	"/scene/poihana/bas/poihana_death.bas",
	"/scene/poihana/bas/poihana_getup.bas",
	"/scene/poihana/bas/poihana_humi.bas",
	nullptr,
	nullptr,
	"/scene/poihana/bas/poihana_sleep_start.bas",
	nullptr,
	"/scene/poihana/bas/poihana_throw.bas",
	nullptr,
	"/scene/poihana/bas/poihana_waken.bas",
	"/scene/poihana/bas/poihana_walk.bas",
	"/scene/poihana/bas/poihana_waterhit.bas",
	"/scene/poihana/bas/poihana_waterhit2.bas",
};

TPoihanaSaveLoadParams::TPoihanaSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
    , PARAM_INIT(mSLThrowSpeed, 100.0f)
    , PARAM_INIT(mSLBackThrowVal, 0.5f)
    , PARAM_INIT(mSLSleepFrame, 1000)
    , PARAM_INIT(mSLWakeFrame, 2000)
    , PARAM_INIT(mSLTrapJumpMinSpY, 10.0f)
    , PARAM_INIT(mSLTrapJumpMaxSpY, 10.0f)
    , PARAM_INIT(mSLTrapJumpMaxSpXZ, 8.0f)
    , PARAM_INIT(mSLTrapJumpMinSpXZ, 8.0f)
    , PARAM_INIT(mSLTrapJumpGravity, 1.0f)
{
	TParams::load(mPrmPath);
}

TPoiHanaManager::TPoiHanaManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TPoiHanaManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TPoihanaSaveLoadParams("/enemy/poihana.prm");
}

TSmallEnemy* TPoiHanaManager::createEnemyInstance()
{
	if (gpApplication.mCurrArea.unk0 == 0x38)
		return new TPoiHana;
	return nullptr;
}

void TPoiHanaManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TEnemyManager::perform(param_1, param_2);
}

void TPoiHanaManager::initSetEnemies()
{
	int bodyIdx
	    = getObj(0)->getModel()->getModelData()->getMaterialName()->getIndex(
	        "_body");

	for (int i = 0; i < mObjNum; ++i) {
		TPoiHana* poiHana = (TPoiHana*)unk18[i];
		SMS_InitPacket_OneTevColor(poiHana->getMActor()->getModel(), bodyIdx,
		                           GX_TEVREG0, &poiHana->unk1C0);
	}
}

BOOL TPoiHanaCollision::receiveMessage(THitActor* param_1, u32 param_2)
{
	unk68->receiveMessage(param_1, param_2);
}

void TPoiHanaCollision::checkHit()
{
	for (int i = 0; i < getColNum(); ++i) {
		THitActor* col = getCollision(i);
		if (col->isActorType(0x80000001))
			unk68->attackToMario();
		else
			unk68->behaveToHitOthers(col);
	}
}

void TPoiHanaCollision::kill() { onHitFlag(HIT_FLAG_NO_COLLISION); }

u8 TPoiHana::mMouthJntIndex = 6;
u8 TPoiHana::mSleepVersion  = 1;
u8 TPoiHana::mBodyJntIndex  = 0;

TPoiHana::TPoiHana(const char* name)
    : TWalkerEnemy(name)
    , unk194(false)
    , unk195(true)
    , unk198(0.0f)
    , unk1A0(true)
    , mGoToSleepTimer(0)
    , unk1A8(true)
    , mIsTrapped(false)
    , unk1BC(nullptr)
    , unk1C8(true)
{
}

void TPoiHana::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	reset();
}

void TPoiHana::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = 0x10000015;
	unk150     = 17;
	onHitFlag(HIT_FLAG_UNK40000000);
	mGoToSleepTimer = mInstanceIndex * -250;
	if (mSleepVersion != 0 && !unk1A0)
		mSpine->initWith(&TNervePoihanaSleep::theNerve());
	else
		mSpine->initWith(&TNerveWalkerGraphWander::theNerve());

	unk19C = (TPoihanaSaveLoadParams*)getSaveParam();
	unk1BC = new TPoiHanaCollision;

	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(unk1BC);

	unk1BC->initHitActor(0, 2, 0x80000000,
	                     unk19C->mSLAttackRadius.get() * mBodyScale,
	                     unk19C->mSLAttackHeight.get() * mBodyScale,
	                     unk19C->mSLDamageRadius.get() * mBodyScale,
	                     unk19C->mSLDamageHeight.get() * mBodyScale);

	unk1BC->unk68 = this;

	unk1C0.r = 0xff41;
	unk1C0.g = 0x8;
	unk1C0.b = 0x12F;
}

void TPoiHana::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("default.bmd", 3);
}

void TPoiHana::reset()
{
	TWalkerEnemy::reset();
	unk198          = mGroundHeight;
	unk194          = false;
	unk195          = false;
	mGoToSleepTimer = 0;
	unk1A8          = true;
	mIsTrapped      = false;
}

void TPoiHana::moveObject()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		unk1BC->mPosition = mPosition;
	} else {
		MtxPtr mtx = getModel()->getAnmMtx(mBodyJntIndex);

		unk1BC->mPosition.x = mtx[0][3];
		unk1BC->mPosition.y = mtx[1][3];
		unk1BC->mPosition.z = mtx[2][3];
	}

	unk1BC->checkHit();

	TWalkerEnemy::moveObject();
	if (isAirborne())
		return;
	if (mIsTrapped)
		return;

	if (mSpine->getCurrentNerve() != &TNervePoihanaTrapped::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()
	    && isOnTrap()) {

		mSpine->pushNerve(&TNervePoihanaTrapped::theNerve());
	}
}

void TPoiHana::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);
	unk1BC->THitActor::perform(param_1, param_2);
}

void TPoiHana::bind()
{
	if (mSpine->getCurrentNerve() == &TNervePoihanaTrapped::theNerve()
	    && isAirborne() && mVelocity.y > 0.0f) {
		mPosition += mVelocity;
		mVelocity.y -= getGravityY();
		if (mVelocity.y < mVelocityMinY)
			mVelocity.y = mVelocityMinY;
		return;
	}

	TLiveActor::bind();
}

// TODO: off by 4 bytes
bool TPoiHana::isOnTrap()
{
	const TLiveActor* groundActor = mGroundPlane->mActor;
	if (groundActor == nullptr) {
		unk198 = mGroundHeight;
	} else if (groundActor->getActorType() == 0x400000CD) {
		MtxPtr mtx = groundActor->getModel()->getAnmMtx(0);
		if (mtx[1][1] >= 0.1f) {
			if (!mIsTrapped) {
				unk1A8 = false;
				if (mGroundHeight > unk198) {
					unk1A8      = true;
					mHeadHeight = 350.0f;
				}
			}

			return true;
		}
	}

	return false;
}

void TPoiHana::behaveToWater(THitActor* param_1)
{
	unk165 = 1;
	if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveSmallEnemyJump::theNerve()) {
		mSpine->pushNerve(&TNervePoihanaFreeze::theNerve());
	}
}

void TPoiHana::attackToMario()
{
	if (mSpine->getCurrentNerve() == &TNervePoihanaSleep::theNerve()) {
		unk1A0 = true;
		unk194 = true;
		offLiveFlag(LIVE_FLAG_UNK10);
		return;
	}

	if (mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve()) {
		mSpine->pushNerve(&TNervePoihanaThrow::theNerve());
		return;
	}

	if (mSpine->getCurrentNerve() != &TNervePoihanaThrow::theNerve()
	    && mSpine->getCurrentNerve() != &TNervePoihanaFreeze::theNerve()) {
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		return;
	}
}

void TPoiHana::setWalkAnm() { setBckAnm(11); }

void TPoiHana::setWaitAnm() { setBckAnm(9); }

void TPoiHana::setRunAnm() { setBckAnm(0); }

void TPoiHana::setFreezeAnm() { setBckAnm(12); }

void TPoiHana::setDeadAnm()
{
	unk1BC->kill();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mHitPoints = 1;
	if (!unk184)
		unk18C = 3;
	setBckAnm(1);

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0xC0, &mPosition, 0, nullptr)) {
		emitter->setScale(mScaling);
	}

	if (!mGroundPlane->isSand()) {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0x10, &mPosition, 0, nullptr)) {
			emitter->setScale(mScaling);
		}
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0x11, &mPosition, 0, nullptr)) {
			emitter->setScale(mScaling);
		}
	} else {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0x53, &mPosition, 0, nullptr)) {
			emitter->setScale(mScaling);
		}
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0x10, &mPosition, 0, nullptr)) {
			emitter->setScale(mScaling);
		}
	}
}

bool TPoiHana::isHitValid(u32 param_1)
{
	if (param_1 == 11)
		return true;

	if (mSpine->getCurrentNerve() == &TNervePoihanaFreeze::theNerve()) {
		setBckAnm(3);
		genRandomItem();
	}

	return false;
}

bool TPoiHana::isCollidMove(THitActor* param_1)
{
	if ((param_1->getActorType() & ACTOR_TYPE_MASK) == ACTOR_TYPE_UNK40000000) {
		if (((TMapObjBase*)param_1)->isHideObj(param_1))
			return false;

		if (param_1->getActorType() == 0x4000019A
		    || param_1->getActorType() == 0x400000D0)
			return false;

		if (mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve()) {
			mSpine->pushNerve(&TNervePoihanaFreeze::theNerve());
			JGeometry::TVec3<f32> vel = mLinearVelocity;
			mLinearVelocity.x *= -2.0f;
			mLinearVelocity.y *= 5.0f;
			mLinearVelocity.z *= -2.0f;
			mVelocity = vel;

			mPosition.y += 10.0f;
			onLiveFlag(LIVE_FLAG_AIRBORNE);
		}
		return true;
	}

	if (!((TLiveActor*)param_1)->isAirborne() && !isAirborne())
		return true;

	return false;
}

void TPoiHana::walkBehavior(int param_1, float param_2)
{
	TWalkerEnemy::walkBehavior(param_1, param_2);

	if (mSleepVersion && param_1 == 0) {
		mGoToSleepTimer += 1;
		if (checkCurAnmEnd(0)) {
			if (mGoToSleepTimer
			    > unk19C->mSLWakeFrame.get() + mInstanceIndex * 100) {
				mGoToSleepTimer = 0;

				// TODO: random interval class
				mGoToSleepTimer = MsRandF(-500.0f, 500.0f);

				mSpine->setNext(&TNervePoihanaSleep::theNerve());
				unk195 = true;
			}
		}
	}
}

BOOL TPoiHana::isInhibitedForceMove()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return true;
	return false;
}

bool TPoiHana::doKeepDistance()
{
	if (!SMS_IsMarioTouchGround4cm())
		return false;

	return true;
}

void TPoiHana::calcRootMatrix()
{
	TSpineEnemy::calcRootMatrix();

	if (isBckAnm(3) || isBckAnm(4)) {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToPosPtr(0x12F, &mPosition, 1,
		                                                  this)) {
			emitter->setScale(mScaling);
		}
	}

	if (isBckAnm(5)) {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToPosPtr(0x124, &mPosition, 1,
		                                                  this)) {
			emitter->setScale(mScaling);
		}
	}

	if (isBckAnm(12) || isBckAnm(13)) {
		if (mMActor->getFrameCtrl(0)->checkPass(18.0f)) {
			if (!mGroundPlane->isSand()) {
				if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
				        0x10, &mPosition, 0, nullptr)) {
					emitter->setScale(mScaling);
				}
				if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
				        0x11, &mPosition, 0, nullptr)) {
					emitter->setScale(mScaling);
				}
			} else {
				if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
				        0x53, &mPosition, 0, nullptr)) {
					emitter->setScale(mScaling);
				}
				if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
				        0x10, &mPosition, 0, nullptr)) {
					emitter->setScale(mScaling);
				}
			}
		}
	}

	if (isBckAnm(2) && mGroundPlane->isSand()
	    && mMActor->getFrameCtrl(0)->checkPass(34.0f)) {
		gpMarioParticleManager->emit(0x53, &mPosition, 0, nullptr);
	}
}

void TPoiHana::generateItem()
{
	if (unk195)
		unk195 = false;
}

f32 TPoiHana::getGravityY() const
{
	f32 gravity = TLiveActor::getGravityY();
	if (mSpine->getCurrentNerve() == &TNervePoihanaTrapped::theNerve())
		gravity = unk19C->mSLTrapJumpGravity.get();
	return gravity;
}

void TPoiHana::resetHeadHeight()
{
	mHeadHeight = getSaveParam()->mSLHeadHeight.get();
}

bool TPoiHana::isMoving()
{
	if (mSpine->getCurrentNerve() == &TNervePoihanaSleep::theNerve()
	    || mSpine->getCurrentNerve() == &TNervePoihanaFreeze::theNerve())
		return false;

	return true;
}

void TPoiHana::genEventCoin()
{
	if (!mCoin)
		return;

	TCoin* coin;
	if (mCoin->isActorType(0x2000000E)) {
		coin = (TCoin*)gpItemManager->makeObjAppear(0x2000000E);
	} else {
		coin = mCoin;
		coin->appear();
	}

	if (!coin)
		return;

	coin->mPosition = mPosition;

	JGeometry::TVec3<f32> vec = mPosition - SMS_GetMarioPos();
	MsVECNormalize(&vec, &vec);
	coin->mVelocity.set(vec.x * 10.0f, 18.0f, vec.z * 10.0f);

	coin->offLiveFlag(LIVE_FLAG_UNK10);
	mCoin = nullptr;
}

bool TPoiHana::changeByJuice()
{
	if (TSmallEnemy::changeByJuice()) {
		unk1BC->kill();
		return true;
	}

	return false;
}

const char** TPoiHana::getBasNameTable() const { return poihana_bastable; }

void TPoiHanaRed::init(TLiveManager* manager)
{
	TPoiHana::init(manager);
	unk1C0.r = 0x11B;
	unk1C0.g = 0xFFCB;
	unk1C0.b = 0xFF86;
	unk1C8   = false;
}

void TPoiHanaRed::attackToMario()
{
	if (mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve())
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);

	TPoiHana::attackToMario();
}

TSleepPoiHana::TSleepPoiHana(const char* name)
    : TPoiHana(name)
{
}

void TSleepPoiHana::load(JSUMemoryInputStream& stream)
{
	unk1A0 = false;
	TPoiHana::load(stream);
}

DEFINE_NERVE(TNervePoihanaSleep, TLiveActor)
{
	TPoiHana* self = (TPoiHana*)spine->getBody();

	if (spine->getTime() == 0) {
		if (!self->unk1C8) {
			self->mGoToSleepTimer = 0;
			spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
			self->offLiveFlag(LIVE_FLAG_UNK10);
			return true;
		}

		self->setBckAnm(6);
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	if (!self->unk1A0 && !self->isAirborne())
		self->onLiveFlag(LIVE_FLAG_UNK10);

	self->mGoToSleepTimer += 1;
	if ((self->unk194 || self->unsetUnk165()) && !self->isBckAnm(10)) {
		if (gpMSound->gateCheck(0x2955))
			MSoundSESystem::MSoundSE::startSoundActor(0x2955, &self->mPosition,
			                                          0, nullptr, 0, 4);

		self->setBckAnm(10);
		self->getMActor()->getFrameCtrl(0)->setFrame(148.0f);
	}

	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(6)) {
			self->setBckAnm(5);
		} else if (self->isBckAnm(5)) {
			if (self->unk1A0
			    && self->mGoToSleepTimer > self->unk19C->mSLSleepFrame.get())
				self->setBckAnm(10);
		} else {
			if (self->isBckAnm(10)) {
				self->mGoToSleepTimer = 0;
				spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
				self->offLiveFlag(LIVE_FLAG_UNK10);
				return true;
			}

			if (!self->unk1C8) {
				self->mGoToSleepTimer = 0;
				spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
				self->offLiveFlag(LIVE_FLAG_UNK10);
				return true;
			}

			self->setBckAnm(6);
		}
	}

	return 0;
}

DEFINE_NERVE(TNervePoihanaFreeze, TLiveActor)
{
	TPoiHana* self = (TPoiHana*)spine->getBody();

	if (spine->getTime() == 0) {
		if (self->getGroundPlane()->isSand()) {
			self->setBckAnm(12);
		} else {
			self->setBckAnm(13);
		}
		self->unk1BC->kill();
	}

	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(12) || self->isBckAnm(3)) {
			self->setBckAnm(4);
			self->mIsTrapped = false;
		} else if (self->isBckAnm(4)) {
			TPoihanaSaveLoadParams* params
			    = (TPoihanaSaveLoadParams*)self->getSaveParam();

			if (spine->getTime() > params->mSLFreezeWait.get())
				self->setBckAnm(2);
			else
				self->setBckAnm(4);
		} else if (self->isBckAnm(2) || self->isBckAnm(13)) {
			self->unk1BC->offHitFlag(HIT_FLAG_NO_COLLISION);
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNervePoihanaThrow, TLiveActor)
{
	TPoiHana* self = (TPoiHana*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(8);
		self->mThrowTimer = 1;
	}

	if (self->getMActor()->getFrameCtrl(0)->checkPass(4.0f)
	    && self->mThrowTimer > 0)
		self->getMActor()->setFrameRate(0.0f, 0);

	if (self->mThrowTimer > 0) {
		self->mThrowTimer += 1;
		if (self->mThrowTimer > 16) {
			SMS_SendMessageToMario(self, HIT_MESSAGE_UNK7);
			f32 backThrowVal = self->unk19C->mSLBackThrowVal.get();
			Mtx afStack_4c;
			MsMtxSetRotRPH(afStack_4c, self->mPosition.x, self->mPosition.y,
			               self->mPosition.z);
			JGeometry::TVec3<f32> local_58(0.0f, 1.0f, -backThrowVal);
			MTXMultVec(afStack_4c, &local_58, &local_58);
			SMS_ThrowMario(local_58, self->unk19C->mSLThrowSpeed.get());
			self->mThrowTimer = 0;
			self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
			self->mThrowTimer = 0;
		}
	}

	if (self->mThrowTimer == 4) {
		SMSRumbleMgr->start(0x15, 0xf, (float*)nullptr);
		MtxPtr mtx
		    = self->mMActor->getModel()->getAnmMtx(TPoiHana::mMouthJntIndex);
		gpMarioParticleManager->emitAndBindToMtxPtr(0xB, mtx, 0, nullptr);
		gpMarioParticleManager->emitAndBindToMtxPtr(0x39, mtx, 0, nullptr);
	}

	if (self->checkCurAnmEnd(0))
		return true;

	return false;
}

DEFINE_NERVE(TNervePoihanaTrapped, TLiveActor)
{
	TPoiHana* self = (TPoiHana*)spine->getBody();

	if (spine->getTime() < 60) {
		if (!self->isAirborne() || spine->getTime() < 2) {
			self->mIsTrapped = true;
			self->mPosition.y += 150.0f;
			self->onLiveFlag(LIVE_FLAG_AIRBORNE);
			if (self->unk1A8) {
				// TODO: rand interval class
				volatile f32 trapJumpMaxSpY
				    = self->unk19C->mSLTrapJumpMaxSpY.get();
				volatile f32 trapJumpMaxSpXZ
				    = self->unk19C->mSLTrapJumpMaxSpXZ.get();
				volatile f32 trapJumpMinSpY
				    = self->unk19C->mSLTrapJumpMinSpY.get();
				volatile f32 trapJumpMinSpXZ
				    = self->unk19C->mSLTrapJumpMinSpXZ.get();

				JGeometry::TVec3<f32> local_48;
				const TLiveActor* groundActor
				    = self->getGroundPlane()->getActor();
				if (groundActor)
					local_48 = self->mPosition - groundActor->mPosition;
				else
					local_48 = self->mPosition - SMS_GetMarioPos();
				if (local_48.x == 0.0f && local_48.y == 0.0f
				    && local_48.z == 0.0f)
					local_48.x = 1.0f;

				VECNormalize(&local_48, &local_48);
				// TODO: rand interval class
				local_48.x *= MsRandF(trapJumpMinSpXZ, trapJumpMaxSpXZ);
				local_48.y = MsRandF(trapJumpMinSpY, trapJumpMaxSpY);
				local_48.z *= MsRandF(trapJumpMinSpXZ, trapJumpMaxSpXZ);

				self->mVelocity             = local_48;
				self->mCurrentFlungVelocity = local_48;
				self->setBckAnm(7);
			} else {
				self->setFreezeAnm();
			}
		}
	} else if (!self->isAirborne()) {
		if (self->unk1A8) {
			if (self->getGroundPlane()->getActor() == nullptr
			    || !self->isOnTrap()) {
				spine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());
				return true;
			} else {
				self->mCurrentFlungVelocity *= 0.7f;
				self->mVelocity = self->mCurrentFlungVelocity;
				self->mPosition.y += 50.0f;
				self->onLiveFlag(LIVE_FLAG_AIRBORNE);
			}
		} else {
			self->resetHeadHeight();
			spine->pushAfterCurrent(&TNervePoihanaFreeze::theNerve());
			return true;
		}
	}

	return false;
}
