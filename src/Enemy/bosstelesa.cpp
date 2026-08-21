#include <Enemy/BossTelesa.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/HamuKuri.hpp>
#include <Enemy/Telesa.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraShake.hpp>
#include <GC2D/GCConsole2.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSSetSound.hpp>
#include <JSystem/JAudio/JALibrary/JALModSe.hpp>
#include <MSound/MSoundBGM.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/SharedParts.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <System/Particles.hpp>
#include <dolphin/mtx.h>
#include <math.h>
#include <stdlib.h>

static const char* btelesa_bastable[] = {
	"/scene/btelesa/bas/btelesa_appear.bas",
	"/scene/btelesa/bas/btelesa_bero_hit.bas",
	"/scene/btelesa/bas/btelesa_damage.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/btelesa/bas/btelesa_lick.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/btelesa/bas/btelesa_roll.bas",
	"/scene/btelesa/bas/btelesa_spicy.bas",
	nullptr,
	nullptr,
	"/scene/btelesa/bas/btelesa_wait.bas",
	"/scene/btelesa/bas/btelesa_wet.bas",
};

f32 TBossTelesa::mEnemyGenRate           = 0.5f;
f32 TBossTelesa::mItemGenRate            = 0.1f;
u8 TBossTelesa::mNormalAlpha             = 0x96;
f32 TBossTelesa::mBaseHoseiPosY          = -300.0f;
f32 TBossTelesa::mRouletteUpRate         = 0.03f;
u32 TBossTelesa::mTelesaGenerateInterval = 400;
f32 TBossTelesa::mCameraMoveLimit        = 1000.0f;
f32 TBossTelesa::mCameraMoveSp           = 0.02f;

static inline TBossTelesa* getBoss(TSpineBase<TLiveActor>* spine)
{
	return (TBossTelesa*)spine->getBody();
}

static inline TBubble* getBubble(TSpineBase<TLiveActor>* spine)
{
	return (TBubble*)spine->getBody();
}

static inline TBossTelesa* getSlotOwner(TTelesaSlot* slot)
{
	return (TBossTelesa*)slot->unk1A0;
}

TBubbleSaveLoadParams::TBubbleSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
    , PARAM_INIT(mSLLiveTime, 200)
    , PARAM_INIT(mSLNumDivision, 5)
    , PARAM_INIT(mSLMaxScale, 1.5f)
    , PARAM_INIT(mSLAddPosBase, 50.0f)
    , PARAM_INIT(mSLRateExpand, 1.001f)
    , PARAM_INIT(mSLDeadHeight, 300.0f)
{
	TParams::load(mPrmPath);
}

TBubbleManager::TBubbleManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TBubbleManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBubbleSaveLoadParams("/enemy/bubble.prm");
	TSmallEnemyManager::load(stream);
}

TSmallEnemy* TBubbleManager::createEnemyInstance()
{
	return new TBubble("バブル");
}

void TBubbleManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "btelesa_osenbubbles_ind.bmd", 0x11020000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TBubble::TBubble(const char* name)
    : TWalkerEnemy(name)
    , unk198(nullptr)
    , unk1CC(0.0f)
    , unk1D0(0)
    , unk1D1(0)
{
}

void TBubble::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mActorType = 0x10000020;
	unk150     = 0x11;
	unk194     = (TBubbleSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveBubbleLive::theNerve());
	mMActor->setLightType(3);

	TScreenTexture* tex
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
	SMS_ChangeTextureAll(mMActor->getModel()->getModelData(), "H_ma_rak_dummy",
	                     *tex->getTexture()->getTexInfo());
}

void TBubble::reset()
{
	TWalkerEnemy::reset();
	onLiveFlag(LIVE_FLAG_UNK8);

	TMsRange<f32> scaleRange(50.0f, 150.0f);
	unk1CC = scaleRange.rand();
	unk1D0 = 0;
	unk1D1 = 1;
	unk1D2 = 0;
	unk198 = 0;
	mSpine->initWith(&TNerveBubbleLive::theNerve());
}

void TBubble::split()
{
	s32 numDivision = unk194->mSLNumDivision.get();
	for (int i = 0; i < numDivision; ++i) {
		TBubble* bubble = (TBubble*)gpConductor->makeOneEnemyAppear(
		    mPosition, "バブルマネージャー", 1);
		if (!bubble)
			break;

		bubble->reset();
		bubble->mPosition = mPosition;
		bubble->mPosition.y += unk1CC;
		bubble->unk1CC = 0.0f;
		bubble->unk1D0 = 1;

		TMsRange<f32> velocityRange(-2.0f, 2.0f);
		JGeometry::TVec3<f32> velocity;
		velocity.x = velocityRange.rand();
		velocity.y = velocityRange.rand();
		velocity.z = velocityRange.rand();

		bubble->mVelocity = velocity;
	}
}

f32 TBubble::getGravityY() const
{
	if (unk1D0) {
		if (unk1D1)
			return 0.001f;
		return 0.0f;
	}
	return mGravity;
}

void TBubble::kill()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if (unk198) {
		if (unk1D2)
			unk198->receiveMessage(this, HIT_MESSAGE_THROWN);
		else
			unk198->kill();
		unk198 = nullptr;
	}

	mHitPoints = 1;
	if (mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()) {
		mSpine->reset();
		mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
		mSpine->pushAfterCurrent(mSpine->getDefault());
		onLiveFlag(LIVE_FLAG_UNK20000);
	}

	onLiveFlag(LIVE_FLAG_UNK40);
}

void TBubble::behaveToWater(THitActor*)
{
	if (mSpine->getCurrentNerve() == &TNerveBubbleLive::theNerve()
	    && mMActor->checkCurBckFromIndex(10)) {
		kill();
		TMapObjBase* item = gpItemManager->makeObjAppear(
		    mPosition.x, mPosition.y + 50.0f, mPosition.z, 0x20000002, true);
		if (item)
			((TItem*)item)->killByTimer(0x4B0);
	}
}

void TBubble::attackToMario()
{
	sendAttackMsgToMario();
	kill();
}

void TBubble::calcRootMatrix()
{
	if (!isEaten()) {
		mPosition.y = mGroundHeight + unk1CC + 150.0f;

		MtxPtr mtx = mMActor->getModel()->getBaseTRMtx();
		MsMtxSetXYZRPH(mtx, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
		               mRotation.y, mRotation.z);
		mMActor->getModel()->setBaseScale(mScaling);
	}
}

void TBubble::setDeadAnm() { setBckAnm(9); }

MtxPtr TBubble::getTakingMtx() { return mMActor->unk4->unk20; }

const char** TBubble::getBasNameTable() const { return btelesa_bastable; }

const TNerveBubbleLive& TNerveBubbleLive::theNerve()
{
	static TNerveBubbleLive instance;
	return instance;
}

void TBubble::appendItem() { }

void TBubble::appendEnemy()
{
	unk198 = nullptr;

	TMsRange<f32> enemyKindRange(0.0f, 100.0f);
	f32 randValue = enemyKindRange.rand();

	TSmallEnemy* enemy;
	if (randValue < 50.0f) {
		enemy = (TSmallEnemy*)gpConductor->makeOneEnemyAppear(
		    mPosition, "ポポマネージャー", 1);
		enemy->unk154 = 0.6f;
		enemy->reset();
	} else if (randValue < 100.0f) {
		enemy = (TSmallEnemy*)gpConductor->makeOneEnemyAppear(
		    mPosition, "ボム兵マネージャー", 1);
		enemy->unk154 = 0.3f;
		enemy->reset();
	} else if (randValue < 150.0f) {
		enemy = (TSmallEnemy*)gpConductor->makeOneEnemyAppear(
		    mPosition, "テレサマネージャー", 1);
		if (!enemy)
			return;
		enemy->unk154 = 0.6f;
		enemy->reset();
		((TTelesa*)enemy)->setAttacker();
	} else {
		enemy = (TSmallEnemy*)gpConductor->makeOneEnemyAppear(
		    mPosition, "パックンマネージャー", 1);
		enemy->unk154 = 0.6f;
		enemy->reset();
	}

	if (enemy && enemy->receiveMessage(this, HIT_MESSAGE_TAKE)) {
		enemy->onHitFlag(HIT_FLAG_UNK8000000);
		mHeldObject = enemy;
		JGeometry::TVec3<f32> velocity;
		velocity.set(0.0f, 1.0f, -1.0f);
		enemy->mVelocity = velocity;
		enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);
		unk198 = enemy;
	}
}

BOOL TNerveBubbleLive::execute(TSpineBase<TLiveActor>* spine) const
{
	TBubble* bubble = getBubble(spine);
	if (spine->getTime() == 0) {
		bubble->offHitFlag(HIT_FLAG_NO_COLLISION);

		if (!bubble->unk1D0) {
			bubble->setBckAnm(8);
		} else {
			bubble->setBckAnm(10);
			bubble->setGoalPathMario();
		}

		J3DFrameCtrl* frameCtrl = bubble->mMActor->getFrameCtrl(0);
		TMsRange<f32> frameRange(0.0f, 20.0f);
		frameCtrl->setFrame(frameRange.rand());
		bubble->onLiveFlag(LIVE_FLAG_UNK8);
	} else if (bubble->checkCurAnmEnd(0)) {
		bubble->offHitFlag(HIT_FLAG_NO_COLLISION);
		bubble->setBckAnm(10);
	}

	if (!bubble->unk1D0) {
		if (bubble->unk1CC < bubble->unk194->mSLAddPosBase.get())
			bubble->unk1CC += 1.0f;
	} else {
		if (spine->getTime() > 40 && bubble->unk1D1) {
			JGeometry::TVec3<f32> velocity = bubble->mVelocity;
			velocity.scale(0.98f);
			bubble->mVelocity = velocity;
		} else {
			bubble->walkBehavior(0, 0.8f);
		}

		if (spine->getTime() == 80) {
			bubble->unk1D1    = 0;
			bubble->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
		}
	}

	bubble->unk1CC += 0.001f;
	if (bubble->unk1CC
	    > bubble->mPosition.y + bubble->unk194->mSLDeadHeight.get()) {
		bubble->unk1D2 = 0;
		bubble->kill();
	}

	if (bubble->mScaling.x < bubble->unk194->mSLMaxScale.get()) {
		f32 scale = bubble->mScaling.z * bubble->unk194->mSLRateExpand.get();
		bubble->mScaling.z = scale;
		bubble->mScaling.y = scale;
		bubble->mScaling.x = scale;
	}

	if (spine->getTime() > bubble->unk194->mSLLiveTime.get()) {
		spine->pushAfterCurrent(&TNerveBubbleSplit::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBubbleSplit, TLiveActor)
{
	TBubble* bubble = getBubble(spine);
	if (spine->getTime() == 0) {
		bubble->onHitFlag(HIT_FLAG_NO_COLLISION);
		bubble->split();
	}

	if (spine->getTime() == 10)
		bubble->setBckAnm(9);

	if (bubble->checkCurAnmEnd(0) && bubble->mMActor->checkCurBckFromIndex(9)) {
		bubble->unk1D2 = 0;
		bubble->kill();
	}

	return FALSE;
}

TBossTelesaSaveLoadParams::TBossTelesaSaveLoadParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLDamageRadius, 200)
    , PARAM_INIT(mSLDamageHeight, 100)
    , PARAM_INIT(mSLAttackRadius, 220)
    , PARAM_INIT(mSLAttackHeight, 120)
    , PARAM_INIT(mSLGenAttackerTime, 500)
    , PARAM_INIT(mSLGenBubbleTime, 600)
    , PARAM_INIT(mSLHitAngle, 20.0f)
    , PARAM_INIT(mSLNumGenBubble, 5)
    , PARAM_INIT(mSL1stBubbleSp, 10.0f)
    , PARAM_INIT(mSLHideAreaRadius, 500.0f)
    , PARAM_INIT(mSLSlotItemNum, 5)
    , PARAM_INIT(mSLSlotFruitNum, 10)
    , PARAM_INIT(mSLSlotFirstHitCollectRate, 0.1f)
    , PARAM_INIT(mSLSlotHitCollectRate, 0.1f)
    , PARAM_INIT(mSLTransYOffset, 350.0f)
    , PARAM_INIT(mSLStopSlotTime0, 3000)
    , PARAM_INIT(mSLStopSlotTime1, 2000)
    , PARAM_INIT(mSLStopSlotTime2, 1000)
    , PARAM_INIT(mSLSpicyTime, 2000)
{
	TParams::load(mPrmPath);
}

TBossTelesaManager::TBossTelesaManager(const char* name)
    : TEnemyManager(name)
{
}

void TBossTelesaManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBossTelesaSaveLoadParams("/enemy/bosstelesa.prm");
	TEnemyManager::load(stream);
}

TSpineEnemy* TBossTelesaManager::createEnemyInstance()
{
	return new TBossTelesa("ボステレサ");
}

void TBossTelesaManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "btelesa.bmd", 0x15300000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

const f32 bossTelesaDirectionTable[] = {
	1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
};

void TBossTelesaManager::perform(u32 flags, JDrama::TGraphics* gfx)
{
	TEnemyManager::perform(flags, gfx);
}

BOOL TBossTelesaBody::receiveMessage(THitActor* sender, u32 message)
{
	return unk68->checkMessage(sender, message);
}

void TBossTelesaBody::checkHit()
{
	unk6C = 0;

	for (int i = 0; i < mColCount; ++i) {
		THitActor* actor = mCollisions[i];
		if (actor->isActorType(0x80000001))
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		else
			unk68->checkHitObject(actor);
	}
}

void TBossTelesaTongue::checkHit()
{
	for (int i = 0; i < mColCount; ++i) {
		THitActor* actor = mCollisions[i];
		if (actor->isActorType(0x80000001))
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		else if (actor->isActorType(0x40000395))
			unk68->setSpicy((TLiveActor*)actor);
		else if (mPosition.y + 100.0f < actor->mPosition.y)
			unk68->checkHitObject(actor);
	}
}

BOOL TBossTelesaTongue::receiveMessage(THitActor*, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER)
		unk68->tongueHitWater();
	return TRUE;
}

void TBossTelesaKillSmallEnemy::checkHit()
{
	unk6C = 0;

	for (int i = 0; i < mColCount; ++i) {
		THitActor* actor = mCollisions[i];
		if (actor->checkActorType(ACTOR_TYPE_ENEMY)) {
			if (actor->isActorType(0x10000013))
				((THamuKuri*)actor)->selectCapHolder();
			((TLiveActor*)actor)->kill();
		}
	}

	JGeometry::TVec3<f32> diff = *gpMarioPos;
	diff -= mPosition;
	diff.y = 0.0f;
	if (MsVECMag2(&diff) < 300.0f) {
		unk68->forceHide();
		unk6C = 1;
	}
}

void TTelesaSlot::initMapObj()
{
	TSlotDrum::initMapObj();
	onLiveFlag(LIVE_FLAG_UNK10);
	unk14C = 160.0f;
	unk150 = mPosition.y;
	unk154 = 2.0f;
	unk158 = 2.0f;
	unk15C = 0.01f;
	unk160 = 0.5f;
	unk164 = 0;
	unk168 = 45;
	unk140 = mDamageRadius / 3.0f;
	unk144 = mDamageHeight;
	unk1DC = new TMapCollisionMove();
	unk1DC->init(2, 0, 0, nullptr);

	TMsRange<s32> indexRange(0, 8);
	for (int i = 0; i < 3; ++i) {
		unk13C[i] = (f32)(unk168 * indexRange.rand());
		unk198[i] = false;
	}
}

#pragma dont_inline on
void TTelesaSlot::randomReset()
{
	TMsRange<s32> indexRange(0, 8);

	for (int i = 0; i < 3; ++i) {
		unk13C[i] = (f32)(unk168 * indexRange.rand());
		unk198[i] = false;
	}
}
#pragma dont_inline off

void TTelesaSlot::calcRootMatrix()
{
	bool rolling = false;

	if (getDrumSpeeds()[0] != 0.0f)
		rolling = true;
	if (getDrumSpeeds()[1] != 0.0f)
		rolling = true;
	if (getDrumSpeeds()[2] != 0.0f)
		rolling = true;

	if (rolling) {
		if (unk1E0) {
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_SLOT_SPIN, &mPosition, 0,
			                                nullptr, 0, 4);
		}

		unk1E0 = 1 - unk1E0;
	}

	TSlotDrum::calcRootMatrix();
}

void TTelesaSlot::moveObject()
{
	TLiveActor::moveObject();

	for (int i = 0; i < unk148; ++i) {
		if (unk1A8[i]) {
			if (getForcastResult(i) == unk1A4) {
				unk198[i] = false;
				unk1A8[i] = false;
			}
		}

		f32 speed = unk138[i];
		if (speed == 0.0f)
			continue;

		if (fabsf(speed) > unk160) {
			unk13C[i] += speed;

			if (!unk198[i]) {
				if (unk138[i] > 0.0f)
					unk138[i] -= unk15C;
				else
					unk138[i] += unk15C;
			}

			if (unk13C[i] >= 360.0f)
				unk13C[i] -= 360.0f;
			if (unk13C[i] <= 0.0f)
				unk13C[i] += 360.0f;
		} else {
			unk13C[i] += speed;

			if (unk13C[i] >= 360.0f)
				unk13C[i] -= 360.0f;
			if (unk13C[i] <= 0.0f)
				unk13C[i] += 360.0f;

			if (unk198[i])
				continue;

			if ((int)fabsf(unk13C[i]) % unk168 != 0)
				continue;

			unk13C[i] = unk168 * (int)(unk13C[i] / (f32)unk168);
			unk138[i] = 0.0f;

			SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_SLT_STOP,
			                                &mPosition, 0, nullptr, 0, 4);

			for (int j = 0; j < unk148; ++j) {
				if (unk198[j]) {
					TBossTelesa* owner = getSlotOwner(this);
					TBossTelesaSaveLoadParams* params
					    = (TBossTelesaSaveLoadParams*)owner->unk15C;
					f32 rate = params->mSLSlotHitCollectRate.get();

					TMsRange<f32> collectRange(0.0f, 1.0f);
					if (collectRange.rand() <= rate) {
						unk1A8[j] = true;
					} else {
						unk198[j] = false;
					}
				}
			}

			bool allStopped = true;
			if (unk138[0] != 0.0f)
				allStopped = false;
			if (unk138[1] != 0.0f)
				allStopped = false;
			if (unk138[2] != 0.0f)
				allStopped = false;

			if (allStopped) {
				TBossTelesa* owner = getSlotOwner(this);
				TTelesaSlot* slot  = (TTelesaSlot*)owner->unk184;

				if (slot->getSlotResult() == 2 || slot->getSlotResult() == 0) {
					owner->unk374.x = 0.0f;
					owner->unk374.y = 0.0f;
					owner->unk374.z = 0.0f;
					gpMarioParticleManager->emit(
					    SCENE_BTELESA_JPA_MS_BTLS_FUBUKI, &owner->unk374, 0,
					    nullptr);

					if (slot->getSlotResult() == 2) {
						SMSGetMSound()->startSoundActor(
						    MSD_SE_BS_TELESA_FANFALE_1, &owner->mPosition, 0,
						    nullptr, 0, 4);
					} else {
						SMSGetMSound()->startSoundActor(
						    MSD_SE_BS_TELESA_FANFALE_2, &owner->mPosition, 0,
						    nullptr, 0, 4);
					}
				} else {
					SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_FANFALE_3,
					                                &owner->mPosition, 0,
					                                nullptr, 0, 4);
				}
			}
		}
	}
}

void TTelesaSlot::moveStart()
{
	unk19C = 1;
	unk19B = 1;

	for (int i = 0; i < 3; ++i) {
		unk198[i] = true;
		unk1A8[i] = false;

		f32 speed = 1.0f;
		if (i == 0)
			speed = -1.0f;
		if (i == 1)
			speed = -0.8f;

		unk138[i] = speed * unk158;
	}
}

u32 TTelesaSlot::touchWater(THitActor*) { return FALSE; }

#pragma dont_inline on
void TTelesaSlot::forceStopSlot(int idx)
{
	f32 collectRate;

	if (!unk19C)
		return;

	TBossTelesaSaveLoadParams* params
	    = (TBossTelesaSaveLoadParams*)getSlotOwner(this)->unk15C;
	collectRate = params->mSLSlotFirstHitCollectRate.get();

	if (SMS_GetMarioHP() == 1)
		collectRate = 0.9f;

	TMsRange<f32> collectRange(0.0f, 1.0f);
	f32 randomValue = collectRange.rand();
	if (randomValue <= collectRate) {
		unk1A4 = 2;
		if (SMS_GetMarioHP() <= 3)
			unk1A4 = 0;
		unk1A8[idx] = true;
	} else {
		unk1A4      = getForcastResult(idx);
		unk198[idx] = false;
	}

	if (unk1A4 == getSlotOwner(this)->unk1A8)
		unk1A4 = 3;

	if (unk1A4 == 0) {
		if (getSlotOwner(this)->unk370 == 0)
			unk1A4 = 1;
		else if (SMS_GetMarioHP() >= 6)
			unk1A4 = 3;
	}

	unk19C = 0;
}
#pragma dont_inline off

bool TTelesaSlot::isRollDrum()
{
	if (unk198[0])
		return true;
	if (unk198[1])
		return true;
	if (unk198[2])
		return true;

	unk19B = 0;
	return false;
}

int TTelesaSlot::getSlotResult()
{
	int result = getResultFromAng(unk13C[0]);
	for (int i = 1; i < 3; ++i) {
		if (getResultFromAng(unk13C[i]) != result)
			return -1;
	}
	return result;
}

int TTelesaSlot::getDrumResult(int index)
{
	return getResultFromAng(unk13C[index]);
}

int TTelesaSlot::getForcastResult(int idx)
{
	f32 ang = unk13C[idx];
	f32 cur = unk138[idx];

	for (;;) {
		if (fabsf(cur) > unk160) {
			ang += cur;
			if (cur > 0.0f)
				cur -= unk15C;
			else
				cur += unk15C;

			if (ang >= 360.0f)
				ang -= 360.0f;
			if (ang <= 0.0f)
				ang += 360.0f;
		} else {
			ang += cur;
			if (ang >= 360.0f)
				ang -= 360.0f;
			if (ang <= 0.0f)
				ang += 360.0f;
			if ((int)fabsf(ang) % unk168 == 0)
				break;
		}
	}

	return getResultFromAng(unk168 * (int)(ang / (f32)unk168));
}

int TTelesaSlot::getResultFromAng(f32 ang)
{
	if (ang < 44.0f)
		return 0;
	if (ang < 89.0f)
		return 1;
	if (ang < 134.0f)
		return 3;
	if (ang < 179.0f)
		return 2;
	if (ang < 224.0f)
		return 0;
	if (ang < 269.0f)
		return 1;
	if (ang < 314.0f)
		return 3;
	if (ang < 359.0f)
		return 2;
	return 2;
}

void TTelesaSlot::calcObjCollision() { }

void TTelesaSlot::entryObjCollision() { }

TBossTelesa::TBossTelesa(const char* name)
    : TSpineEnemy(name)
    , unk150(1)
    , unk154(nullptr)
    , unk158(nullptr)
    , unk15C(nullptr)
    , unk160(-1)
    , unk164(-1)
    , unk168(0.0f)
    , unk16C(nullptr)
    , unk184(nullptr)
    , unk188(nullptr)
    , unk18C(0)
    , unk1A8(0)
    , unk274(0)
    , unk350(0)
    , unk354(0)
    , unk358(0)
    , unk35A(1)
    , unk35B(1)
    , unk35C(0)
    , unk360(0.0f)
    , unk364(0.0f)
    , unk368(0)
    , unk36C(0)
    , unk370(3)
    , unk384(0)
{
}

void TBossTelesa::init(TLiveManager* manager)
{
	mManager = manager;
	manager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(manager, 1);
	mMActor       = mMActorKeeper->createMActor("btelesa.bmd", 3);

	onLiveFlag(LIVE_FLAG_DEAD);
	onHitFlag(HIT_FLAG_NO_COLLISION);

	TSpineEnemyParams* params = getSaveParam();
	if (params) {
		mBodyRadius       = params->mSLBodyRadius.get();
		mWallRadius       = params->mSLWallRadius.get();
		mHeadHeight       = params->mSLHeadHeight.get();
		mScaledBodyRadius = mBodyScale * mBodyRadius;
		mHitPoints = getSaveParam() ? getSaveParam()->mSLHitPointMax.get() : 1;
	}

	initHitActor(0, 5, 0x98000000, mBodyRadius, mHeadHeight, mBodyRadius,
	             mHeadHeight);
	mGroundPlane = TMap::getIllegalCheckData();
	setGoalPathMario();
	initAnmSound();
	unk15C     = getSaveParam();
	mActorType = 0x08000013;
	onHitFlag(0xD0000000);
	mSpine->initWith(&TNerveBossTelesaFallDemo::theNerve());
	onLiveFlag(LIVE_FLAG_UNK8);

	mMActor->setLightType(1);
	mMActor->unk40 = 1;
	mMActor->initNormalMotionBlend();

	J3DModel* model = mMActor->getModel();
	if (!model->getSkinDeform())
		model->setSkinDeform(new J3DSkinDeform, J3D_DEFORM_ATTACH_FLAG_UNK_1);
	mMActor->resetDL();

	unk348.r = 0xFF;
	unk348.g = 0xFF;
	unk348.b = 0xFF;
	unk348.a = 0xFF;

	unk34C.r = mNormalAlpha;
	unk34C.g = mNormalAlpha;
	unk34C.b = mNormalAlpha;
	unk34C.a = mNormalAlpha;

	J3DModelData* modelData = model->getModelData();
	for (u16 i = 0; i < modelData->getMaterialNum(); ++i) {
		s32 bodyMatIndex
		    = getModel()->getModelData()->getMaterialName()->getIndex(
		        "_mat_body");
		if (i == bodyMatIndex) {
			SMS_InitPacket_TwoTevKColor(model, i, GX_KCOLOR0, &unk34C,
			                            GX_KCOLOR1, &unk348);
		} else {
			SMS_InitPacket_OneTevKColor(model, i, GX_KCOLOR0, &unk34C);
		}
	}

	reset();

	for (int i = 0; i < gpMapObjManager->getObjNum(); ++i) {
		TMapObjBase* actor = gpMapObjManager->getObj(i);
		if (actor->mActorType == 0x4000019A) {
			unk154    = nullptr;
			unk158    = nullptr;
			mPosition = actor->mPosition;
		}
	}

	if (mInstanceIndex == 0) {
		for (u8 i = 0; i < mMActor->getModel()->getModelData()->getJointNum();
		     ++i) { }
	}

	unk16C = new TBossTelesaBody("ボステレサ体コリジョン");
	TIdxGroupObj* enemyGroup
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	enemyGroup->getChildren().push_back(unk16C);
	unk16C->initHitActor(0x08000013, 5, 0xD1000000, 350.0f, 550.0f, 300.0f,
	                     500.0f);
	((TBossTelesaBody*)unk16C)->unk68 = this;
	unk16C->offHitFlag(HIT_FLAG_NO_COLLISION);

	unk170     = new TBossTelesaTongue("ボステレサ舌コリジョン");
	enemyGroup = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	enemyGroup->getChildren().push_back(unk170);
	unk170->initHitActor(0x08000013, 5, 0xC0000000, 180.0f, 350.0f, 180.0f,
	                     350.0f);
	((TBossTelesaTongue*)unk170)->unk68 = this;
	unk170->offHitFlag(HIT_FLAG_NO_COLLISION);

	unk174     = new TBossTelesaKillSmallEnemy("ボステレサ雑魚敵死コリジョン");
	enemyGroup = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	enemyGroup->getChildren().push_back(unk174);
	unk174->initHitActor(0x1000000C, 5, 0x10000000, 400.0f, 300.0f, 400.0f,
	                     300.0f);
	unk174->offHitFlag(HIT_FLAG_NO_COLLISION);
	((TBossTelesaKillSmallEnemy*)unk174)->unk68 = this;

	mMActor->setLightType(3);

	JDrama::TViewObj* screenTex
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("スクリーンテクスチャ");
	void* dataOwner  = *(void**)((u8*)screenTex + 0x10);
	ResTIMG* texture = *(ResTIMG**)((u8*)dataOwner + 0x20);
	SMS_ChangeTextureAll(mMActor->unk4->mModelData, "H_ma_rak_dummy", *texture);

	unk358 = SMS_GetMarioHP();
}

void TBossTelesa::loadAfter()
{
	u32 rouletteType = 0x4000019A;
	if ((u32)gpMapObjManager->getObjNumWithActorType(rouletteType) != 0) {
		int found = 0;
		for (int i = 0; i < gpMapObjManager->getObjNum(); ++i) {
			TMapObjBase* actor = gpMapObjManager->getObj(i);
			u8 isRoulette;
			if (actor->mActorType == rouletteType)
				isRoulette = 1;
			else
				isRoulette = 0;
			if (isRoulette) {
				(&unk178)[found] = (TRoulette*)actor;
				found++;
			}
		}
	}

	u32 slotType = 0x400001A6;
	if ((u32)gpMapObjManager->getObjNumWithActorType(slotType) != 0) {
		for (int i = 0; i < gpMapObjManager->getObjNum(); ++i) {
			TMapObjBase* actor = gpMapObjManager->getObj(i);
			u8 isSlot;
			if (actor->mActorType == slotType)
				isSlot = 1;
			else
				isSlot = 0;
			if (isSlot) {
				unk184         = (TTelesaSlot*)actor;
				unk184->unk1A0 = this;
			}
		}
	}

	int fruitIndex = 0;
	for (int i = 0; i < 6; ++i)
		unk2A8[fruitIndex++] = TMapObjBaseManager::newAndRegisterObj(
		    "FruitCoconut", JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	for (int i = 0; i < 6; ++i)
		unk2A8[fruitIndex++] = TMapObjBaseManager::newAndRegisterObj(
		    "FruitPapaya", JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	for (int i = 0; i < 2; ++i)
		unk2A8[fruitIndex++] = TMapObjBaseManager::newAndRegisterObj(
		    "FruitPine", JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	for (int i = 0; i < 6; ++i)
		unk2A8[fruitIndex++] = TMapObjBaseManager::newAndRegisterObj(
		    "FruitDurian", JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));

	for (int i = 0; i < 20; ++i) {
		((TMapObjBase*)unk2A8[i])->onMapObjFlag(0x04000000);
		((TMapObjBase*)unk2A8[i])->makeObjDead();
	}

	unk2F8[0] = JDrama::TNameRefGen::search<TLiveActor>("唐辛子 0");
	unk2F8[1] = JDrama::TNameRefGen::search<TLiveActor>("唐辛子 1");
	unk2F8[2] = JDrama::TNameRefGen::search<TLiveActor>("唐辛子 2");
	unk2F8[3] = JDrama::TNameRefGen::search<TLiveActor>("唐辛子 3");
	unk2F8[4] = JDrama::TNameRefGen::search<TLiveActor>("唐辛子 4");
	unk2F8[5] = JDrama::TNameRefGen::search<TLiveActor>("唐辛子 5");
	unk2F8[6] = JDrama::TNameRefGen::search<TLiveActor>("唐辛子 6");
	unk2F8[7] = JDrama::TNameRefGen::search<TLiveActor>("唐辛子 7");
	unk2F8[8] = JDrama::TNameRefGen::search<TLiveActor>("唐辛子 8");
	unk2F8[9] = JDrama::TNameRefGen::search<TLiveActor>("唐辛子 9");
	for (int i = 0; i < 10; ++i)
		((TMapObjBase*)unk2F8[i])->makeObjDead();

	unk320[0] = JDrama::TNameRefGen::search<TLiveActor>("コイン 0");
	unk320[1] = JDrama::TNameRefGen::search<TLiveActor>("コイン 1");
	unk320[2] = JDrama::TNameRefGen::search<TLiveActor>("コイン 2");
	unk320[3] = JDrama::TNameRefGen::search<TLiveActor>("コイン 3");
	unk320[4] = JDrama::TNameRefGen::search<TLiveActor>("コイン 4");
	unk320[5] = JDrama::TNameRefGen::search<TLiveActor>("コイン 5");
	unk320[6] = JDrama::TNameRefGen::search<TLiveActor>("コイン 6");
	unk320[7] = JDrama::TNameRefGen::search<TLiveActor>("コイン 7");
	unk320[8] = JDrama::TNameRefGen::search<TLiveActor>("コイン 8");
	unk320[9] = JDrama::TNameRefGen::search<TLiveActor>("コイン 9");
	for (int i = 0; i < 10; ++i)
		((TMapObjBase*)unk320[i])->makeObjDead();

	TTelesaSlot* slot = (TTelesaSlot*)unk184;
	slot->unk1E4[0]   = 0.0f;
	slot->unk1E4[1]   = 0.0f;
	slot->unk1E4[2]   = 0.0f;

	unk354 = JDrama::TNameRefGen::search<TObjManager>("テレサマネージャー");

	for (int i = 0; i < 5; ++i)
		TMapObjBaseManager::newAndRegisterObj(
		    "bottle_large", JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
		    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));

	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_fhit.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_FHIT);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_fhit_pe.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_FHIT_PE);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_fhit_gr.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_FHIT_GR);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_fhit_or.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_FHIT_OR);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_damage.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_DAMAGE);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_down.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_DOWN);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_down_pe.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_DOWN_PE);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_down_gr.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_DOWN_GR);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_down_or.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_DOWN_OR);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_spicy_hit.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_SPICY_HIT);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_fubuki.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_FUBUKI);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_yodare1.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_YODARE1);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_yodare2.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_YODARE2);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_yodare3.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_YODARE3);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_ase.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_ASE);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_spicy_a.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_SPICY_A);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_spicy_b.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_SPICY_B);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_spicy_d.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_SPICY_D);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_chika_a.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_CHIKA_A);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_chika_b.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_CHIKA_B);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_glow.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_GLOW);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_spicy_c.jpa",
	                 SCENE_BTELESA_JPA_MS_BTLS_SPICY_C);

	void* res = JKRFileLoader::getGlbResource("/scene/btelesa/srot_waku.bmd");
	SDLModelData* data
	    = new SDLModelData(J3DModelLoaderDataBase::load(res, 0x10220000));
	unk188
	    = new TSharedParts((TLiveActor*)unk184, 0, data, 3, "<TSharedParts>");

	TLiveActor* gesso0 = JDrama::TNameRefGen::search<TLiveActor>("ゲッソー 0");
	if (gesso0)
		gesso0->onLiveFlag(LIVE_FLAG_DEAD);

	TLiveActor* gesso1 = JDrama::TNameRefGen::search<TLiveActor>("ゲッソー 1");
	if (gesso1)
		gesso1->onLiveFlag(LIVE_FLAG_DEAD);

	JDrama::TNameRef::loadAfter();
}

void TBossTelesa::reset()
{
	TSpineEnemy::reset();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	onLiveFlag(LIVE_FLAG_UNK8);
	onLiveFlag(LIVE_FLAG_UNK10);
	onLiveFlag(LIVE_FLAG_HIDDEN);

	unk18C = 0;

	TBossTelesaSaveLoadParams* params = (TBossTelesaSaveLoadParams*)unk15C;
	setHitParams(params->mSLAttackRadius.get(), params->mSLAttackHeight.get(),
	             params->mSLDamageRadius.get(), params->mSLDamageHeight.get());

	SMSGetMarDirector()->fireStartDemoCamera("btelesa_roll_camera", nullptr, -1,
	                                         0.0f, true, nullptr, 0, nullptr,
	                                         JDrama::TFlagT<u16>(0));
}

void TBossTelesa::moveObject()
{
	if (mLiveFlag & LIVE_FLAG_DEAD)
		return;

	JGeometry::TVec3<f32> cameraPos;
	cameraPos.set(gpCamera->unk124);
	JGeometry::TVec3<f32> marioDiff = *gpMarioPos;
	marioDiff.sub(cameraPos);
	f32 cameraDist = marioDiff.length();

	if (cameraDist < mCameraMoveLimit) {
		unk360 += mCameraMoveSp * (gpMarioPos->y - gpCamera->unk148.y);
		gpCamera->unk290 = unk360;
	} else if (fabsf(unk360) > 1.0f) {
		unk360 *= mCameraMoveSp;
		gpCamera->unk290 = unk360;
	}

	if (*gpMarioFlag & 0x400) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_V_LAUGH1, &mPosition,
		                                0, nullptr, 0, 4);
	}

	if (mSpine->getCurrentNerve() == &TNerveBossTelesaFallDemo::theNerve()) {
		if (unk178 && unk17C
		    && unk178->mPosition.y > unk17C->mPosition.y + 5.0f) {
			unk184->mPosition = unk178->mPosition;
			unk184->mPosition.y += 300.0f;
		}
	}

	if (mSpine->getCurrentNerve() != &TNerveBossTelesaPrepareSlot::theNerve()) {
		mMActor->setFrameRate(0.0f, 3);
		mMActor->getFrameCtrl(3)->setFrame(0.0f);
	}

	if (mSpine->getCurrentNerve() == &TNerveBossTelesaDie::theNerve()) {
		u8 maxHp = getSaveParam() ? getSaveParam()->mSLHitPointMax.get() : 1;
		u8 targetAlpha = mNormalAlpha + (maxHp - mHitPoints) * 30;
		if (targetAlpha > 0xFE)
			targetAlpha = 0xFE;

		if ((mMActor->checkCurBckFromIndex(7)
		     && mMActor->getFrameCtrl(0)->getFrame() > 50.0f)
		    || mMActor->checkCurBckFromIndex(6)) {
			if (unk34C.a > targetAlpha)
				--unk34C.a;
		} else if (unk34C.a < 0xFF) {
			++unk34C.a;
		}
	}

	JGeometry::TVec3<f32> cameraDiff = *gpMarioPos;
	cameraDiff.sub(gpCamera->unk124);
	unk19C = unk178->mPosition;
	unk19C.x += 0.67f * cameraDiff.x;
	unk19C.z += 0.67f * cameraDiff.z;

	int movingRoulettes = 0;
	if (unk178->unk13C != 0.0f)
		++movingRoulettes;
	if (unk17C->unk13C != 0.0f)
		++movingRoulettes;
	if (unk180->unk13C != 0.0f)
		++movingRoulettes;

	switch (movingRoulettes) {
	case 1:
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_RLT_MOVE1, &unk19C, 0,
		                                nullptr, 0, 4);
		break;
	case 2:
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_RLT_MOVE2, &unk19C, 0,
		                                nullptr, 0, 4);
		break;
	case 3:
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_RLT_MOVE3, &unk19C, 0,
		                                nullptr, 0, 4);
		break;
	}

	mLinearVelocity.zero();
	mAngularVelocity.zero();
	control();

	for (int i = 0; i < mColCount; ++i) {
		if (mCollisions[i]->isActorType(0x80000001))
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
	}

	bind();
	mPosition += mLinearVelocity;
	mRotation += mAngularVelocity;
	mPosition.y = mGroundHeight + 300.0f;

	unk168 = MsClamp(unk168 - 0.05f, 0.0f, 1.0f);
	mMActor->setMotionBlendRatioForBck(unk168);

	if (mLiveFlag & LIVE_FLAG_CLIPPED_OUT) {
		unk16C->mPosition = mPosition;
		unk170->mPosition = mPosition;
		unk174->mPosition = mPosition;
	} else {
		J3DModel* model = mMActor->getModel();
		MtxPtr node1    = model->mNodeMatrices[1];
		MtxPtr node7    = model->mNodeMatrices[7];

		unk16C->mPosition.set(node1[0][3], node1[1][3] - 200.0f, node1[2][3]);
		unk174->mPosition.set(node1[0][3], mPosition.y - 350.0f, node1[2][3]);
		unk170->mPosition.set(node7[0][3], node7[1][3] - 350.0f, node7[2][3]);
	}

	unk16C->checkHit();
	unk170->checkHit();
	unk174->checkHit();
}

void TBossTelesa::kill()
{
	if (mSpine->getCurrentNerve() != &TNerveBossTelesaDie::theNerve())
		mSpine->pushNerve(&TNerveBossTelesaDie::theNerve());
}

MtxPtr TBossTelesa::getTakingMtx()
{
	unk278.set(unk178->mMActor->unk4->mNodeMatrices[1]);
	unk278.mMtx[1][3] = unk178->mPosition.y - 120.0f;
	return unk278.mMtx;
}

void TBossTelesa::prepareGenerate() { unk35C = 0; }

void TBossTelesa::calcRootMatrix()
{
	mMActor->getModel()->setBaseScale(mScaling);

	f32 offsetY = unk364;
	if (offsetY > 0.0f)
		offsetY = 0.0f;
	else if (offsetY < mBaseHoseiPosY)
		offsetY = mBaseHoseiPosY;

	TPosition3f translateMtx;
	translateMtx.translation(
	    mPosition.x,
	    mPosition.y
	        + ((TBossTelesaSaveLoadParams*)unk15C)->mSLTransYOffset.get()
	        + offsetY,
	    mPosition.z);

	Mtx rotateMtx;
	MsMtxSetRotRPH(rotateMtx, mRotation.x, mRotation.y, mRotation.z);
	PSMTXConcat(translateMtx, rotateMtx, translateMtx);
	PSMTXCopy(translateMtx, mMActor->getModel()->getBaseTRMtx());

	TTelesaSlot* slot = (TTelesaSlot*)unk184;
	if (slot) {
		slot->mRotation = mRotation;

		f32 slotOffsetY = -700.0f;
		MtxPtr slotMtx;
		if (mMActor->checkCurBckFromIndex(4) || mMActor->checkCurBckFromIndex(0)
		    || mMActor->checkCurBckFromIndex(2)
		    || mMActor->checkCurBckFromIndex(5)) {
			slotMtx = mMActor->getModel()->mNodeMatrices[1];
			if (!mMActor->checkCurBckFromIndex(0))
				slotOffsetY = -2400.0f;
		} else {
			slotMtx = mMActor->getModel()->mNodeMatrices[0];
		}

		slot->mPosition.x = slotMtx[0][3];
		slot->mPosition.y = unk364 + slotMtx[1][3] + slotOffsetY;
		slot->mPosition.z = slotMtx[2][3];
	}

	for (u16 i = 0; i < mMActor->getModel()->getModelData()->getMaterialNum();
	     ++i) {
		Mtx lightMtx;
		SMS_GetLightPerspectiveForEffectMtx(lightMtx);
		mMActor->getModel()
		    ->getModelData()
		    ->getMaterialNodePointer(i)
		    ->getTexMtx(1)
		    ->setEffectMtx(lightMtx);
	}

	unk374.set(0.0f, 0.0f, 0.0f);
	gpMarioParticleManager->emit(SCENE_BTELESA_JPA_MS_BTLS_CHIKA_A, &unk374, 1,
	                             this);
	gpMarioParticleManager->emit(SCENE_BTELESA_JPA_MS_BTLS_CHIKA_B, &unk374, 1,
	                             this);

	MtxPtr baseMtx = mMActor->getModel()->mNodeMatrices[1];
	unk374.set(baseMtx[0][3], baseMtx[1][3], baseMtx[2][3]);
	gpMarioParticleManager->emitAndBindToPosPtr(SCENE_BTELESA_JPA_MS_BTLS_GLOW,
	                                            &unk374, 1, this);

	if (!mMActor->checkCurBckFromIndex(4) && !mMActor->checkCurBckFromIndex(6)
	    && !mMActor->checkCurBckFromIndex(12)
	    && !mMActor->checkCurBckFromIndex(13)) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_YODARE1,
		    mMActor->getModel()->mNodeMatrices[5], 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_YODARE2,
		    mMActor->getModel()->mNodeMatrices[5], 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_YODARE3,
		    mMActor->getModel()->mNodeMatrices[10], 1, this);
	}

	if (mMActor->checkCurBckFromIndex(1)
	    && mMActor->getFrameCtrl(0)->getFrame() < 20.0f) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_SPICY_HIT,
		    mMActor->getModel()->mNodeMatrices[5], 0, nullptr);
	}

	if (mMActor->checkCurBckFromIndex(12)) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_FIRE, &mPosition, 0,
		                                nullptr, 0, 4);

		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_ASE,
		    mMActor->getModel()->mNodeMatrices[1], 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_SPICY_A,
		    mMActor->getModel()->mNodeMatrices[9], 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_SPICY_B,
		    mMActor->getModel()->mNodeMatrices[9], 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_SPICY_D,
		    mMActor->getModel()->mNodeMatrices[9], 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_SPICY_C,
		    mMActor->getModel()->mNodeMatrices[9], 3, this);
	}

	if (mMActor->checkCurBckFromIndex(14)
	    && mMActor->getFrameCtrl(0)->checkPass(40.0f)) {
		if (mSpine->getCurrentNerve()
		    == &TNerveBossTelesaSpitSlotItem::theNerve()) {
			generateSlotItem();
		} else {
			genAttacker();
		}
	}
}

void TBossTelesa::perform(u32 flags, JDrama::TGraphics* gfx)
{
	if ((flags & 0x200)
	    && !(mLiveFlag & (LIVE_FLAG_DEAD | LIVE_FLAG_CLIPPED_OUT))) {
		if (mSpine->getCurrentNerve() == &TNerveBossTelesaDie::theNerve()
		    && unk350) {
			mMActor->offMakeDL();
			SMS_AddDamageFogEffect(mMActor->unk4->mModelData, mPosition, gfx);
		}
	}

	offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
	TSpineEnemy::perform(flags, gfx);

	unk16C->THitActor::perform(flags, gfx);
	unk170->THitActor::perform(flags, gfx);
	unk174->THitActor::perform(flags, gfx);
	if (unk184)
		unk184->testPerform(flags, gfx);
	if (unk188)
		unk188->testPerform(flags, gfx);
}

BOOL TBossTelesa::receiveMessage(THitActor*, u32) { return FALSE; }

BOOL TBossTelesa::checkMessage(THitActor* actor, u32 message)
{
	if (message == HIT_MESSAGE_TRAMPLE) {
		bool isMario;
		if (actor->mActorType == (ACTOR_TYPE_PLAYER | 1))
			isMario = true;
		else
			isMario = false;
		if (!isMario) {
			if (mSpine->getCurrentNerve()
			    == &TNerveBossTelesaPrepareSlot::theNerve())
				mSpine->pushNerve(&TNerveBossTelesaSpit::theNerve());
		}
	}

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (mSpine->getCurrentNerve()
		    == &TNerveBossTelesaPrepareSlot::theNerve())
			mSpine->pushNerve(&TNerveBossTelesaFreeze::theNerve());
	}

	return TRUE;
}

void TBossTelesa::checkHitObject(THitActor* actor)
{
	unk380 = -1;

	if ((actor->mActorType & ACTOR_TYPE_MASK) != ACTOR_TYPE_UNK40000000)
		return;
	if (mSpine->getCurrentNerve() != &TNerveBossTelesaPrepareSlot::theNerve())
		return;

	switch (actor->mActorType) {
	case 0x40000390:
		unk348.r = 0xE6;
		unk348.g = 0x64;
		unk348.b = 0xB4;
		unk380   = SCENE_BTELESA_JPA_MS_BTLS_FHIT_PE;
		kill();
		break;
	case 0x40000391:
	case 0x40000392:
		unk348.r = 0xE6;
		unk348.g = 0xB4;
		unk348.b = 0;
		unk380   = SCENE_BTELESA_JPA_MS_BTLS_FHIT_OR;
		kill();
		break;
	case 0x40000393:
		unk348.r = 0x96;
		unk348.g = 0x32;
		unk348.b = 0xE6;
		unk380   = SCENE_BTELESA_JPA_MS_BTLS_FHIT_GR;
		kill();
		break;
	case 0x40000395:
		break;
	default:
		return;
	}

	if (!unk350 && actor->mActorType != 0x40000395) {
		if (unk35A) {
			unk35A = 0;
			gpMarDirector->mConsole->startAppearBalloon(0xE000F, true);
		}
		unk35C++;
		if (unk35C > 2)
			gpMarDirector->mConsole->startAppearBalloon(0xE0010, true);
	} else {
		unk35C = 0;
	}

	unk374 = actor->mPosition;
	gpMarioParticleManager->emit(SCENE_BTELESA_JPA_MS_BTLS_FHIT, &unk374, 0,
	                             nullptr);
	if (unk380 >= 0)
		gpMarioParticleManager->emit(unk380, &unk374, 0, nullptr);

	if (unk350) {
		gpMarioParticleManager->emit(SCENE_BTELESA_JPA_MS_BTLS_DAMAGE, &unk374,
		                             0, nullptr);
	} else {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_FRUIT_HIT, &mPosition,
		                                0, nullptr, 0, 4);
	}

	((TMapObjBase*)actor)->makeObjDead();
}

void TBossTelesa::setSpicy(TLiveActor* actor)
{
	if (mSpine->getCurrentNerve() == &TNerveBossTelesaSpitSlotItem::theNerve())
		return;
	if (mMActor->checkCurBckFromIndex(1))
		return;

	unk350 = 1;
	unk36C = 0;
	unk164 = mMActor->getCurAnmIdx(0);
	unk160 = 1;
	unk168 = 1.0f;

	mMActor->setBckOldMotionBlendAnmPtr(mMActor->getBckAnm());

	mMActor->setBckFromIndex(1);
	mMActor->setMotionBlendRatioForBck(unk168);

	const char** basTable = getBasNameTable();
	const char* basName;
	if (!basTable)
		basName = nullptr;
	else
		basName = basTable[1];
	setAnmSound(basName);

	if (unk35B) {
		unk35B = 0;
		gpMarDirector->mConsole->startAppearBalloon(0xE0011, true);
	}

	actor->kill();
}

void TBossTelesa::damageRecover()
{
	f32 zero = 0.0f;

	for (int i = 0; i < 20; ++i) {
		TLiveActor* actor = unk2A8[i];
		if (!(actor->mLiveFlag & LIVE_FLAG_DEAD)) {
			if (actor->mHolder == nullptr)
				SMS_SendMessageToMario(actor, HIT_MESSAGE_UNK8);

			((TMapObjBase*)actor)->makeObjDead();
			gpMarioParticleManager->emit(PARTICLE_MS_TLS_CHANGE,
			                             &actor->mPosition, 0, nullptr);
			actor->mPosition.set(zero, zero, zero);
		}
	}

	for (int i = 0; i < 10; ++i) {
		TLiveActor* actor = unk2F8[i];
		if (!(actor->mLiveFlag & LIVE_FLAG_DEAD)) {
			if (actor->mHolder == nullptr)
				SMS_SendMessageToMario(actor, HIT_MESSAGE_UNK8);

			((TMapObjBase*)actor)->makeObjDead();
			gpMarioParticleManager->emit(PARTICLE_MS_TLS_CHANGE,
			                             &actor->mPosition, 0, nullptr);
			actor->mPosition.set(zero, zero, zero);
		}

		actor = unk320[i];
		if (!(actor->mLiveFlag & LIVE_FLAG_DEAD)) {
			((TMapObjBase*)actor)->makeObjDead();
			gpMarioParticleManager->emit(PARTICLE_MS_TLS_CHANGE,
			                             &actor->mPosition, 0, nullptr);
			actor->mPosition.set(zero, zero, zero);
		}
	}

	if (unk350) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_ESCAPE, &mPosition, 0,
		                                nullptr, 0, 4);
	} else {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_DISAPPEAR, &mPosition,
		                                0, nullptr, 0, 4);
	}

	mSpine->pushAfterCurrent(&TNerveBossTelesaHide::theNerve());
	unk368 = 0;
}

void TBossTelesa::tongueHitWater()
{
	if (mSpine->getCurrentNerve() == &TNerveBossTelesaAppear::theNerve())
		mSpine->pushNerve(&TNerveBossTelesaSlotStart::theNerve());
}

bool TBossTelesa::rouletteFall()
{
	TRoulette* roulette0 = (TRoulette*)unk178;
	TRoulette* roulette1 = (TRoulette*)unk17C;

	if (roulette0->mPosition.y > roulette1->mPosition.y) {
		roulette0->mPosition.y -= 1.0f;
		roulette0->mMActor->setBck("rulet00");
	} else {
		roulette0->mPosition.y = roulette1->mPosition.y;
		return TRUE;
	}

	if (roulette0->mPosition.y > roulette1->mPosition.y + 3.0f) {
		if (SMS_SendMessageToMario(this, HIT_MESSAGE_TAKE))
			mHeldObject = (TTakeActor*)SMS_GetMarioHitActor();
	} else {
		if (SMS_SendMessageToMario(this, HIT_MESSAGE_UNK8)) {
			SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_RLT_SET,
			                                &mPosition, 0, nullptr, 0, 4);
			mHeldObject = nullptr;
		}
	}

	SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_V_LAUGH1B, &mPosition, 0,
	                                nullptr, 0, 4);

	SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_RLT_DOWN, &mPosition, 0,
	                                nullptr, 0, 4);

	gpMarioOriginal->mGamePad->onNeutralMarioKey();
	return FALSE;
}

bool TBossTelesa::slotFall()
{
	if (unk184->mPosition.y > unk178->mPosition.y - 800.0f) {
		unk184->mPosition.y -= 5.0f;
		return FALSE;
	}

	unk184->mPosition.y -= 1.0f;

	if (unk184->mPosition.y < unk178->mPosition.y - 900.0f) {
		int rolling = 0;
		if (unk178->unk13C != 0.0f)
			rolling = 1;
		if (unk17C->unk13C != 0.0f)
			++rolling;
		if (unk180->unk13C != 0.0f)
			++rolling;

		if (rolling != 3)
			rouletteStart();
	}

	if (unk184->mPosition.y < unk178->mPosition.y - 1100.0f)
		return TRUE;

	THitActor* switchActor     = unk178->unk150;
	switchActor->mAttackRadius = 280.0f;
	switchActor->mAttackHeight = 100.0f;
	switchActor->mDamageRadius = 280.0f;
	switchActor->mDamageHeight = 100.0f;
	switchActor->calcEntryRadius();

	switchActor                = unk17C->unk150;
	switchActor->mAttackRadius = 280.0f;
	switchActor->mAttackHeight = 100.0f;
	switchActor->mDamageRadius = 280.0f;
	switchActor->mDamageHeight = 100.0f;
	switchActor->calcEntryRadius();

	return FALSE;
}

void TBossTelesa::openWaterPlace() { }

void TBossTelesa::flashItem(int result)
{
	int phase = result % 16;

	int index = 0;
	for (int i = 0; i < 5; ++i) {
		TLiveActor* actor = unk2A8[index++];
		u32* liveFlag     = &actor->mLiveFlag;
		if (!(*liveFlag & LIVE_FLAG_DEAD) && actor->mHolder == nullptr) {
			if (phase < 8)
				*liveFlag |= LIVE_FLAG_HIDDEN;
			else
				*liveFlag &= ~LIVE_FLAG_HIDDEN;
		}

		actor    = unk2A8[index++];
		liveFlag = &actor->mLiveFlag;
		if (!(*liveFlag & LIVE_FLAG_DEAD) && actor->mHolder == nullptr) {
			if (phase < 8)
				*liveFlag |= LIVE_FLAG_HIDDEN;
			else
				*liveFlag &= ~LIVE_FLAG_HIDDEN;
		}

		actor    = unk2A8[index++];
		liveFlag = &actor->mLiveFlag;
		if (!(*liveFlag & LIVE_FLAG_DEAD) && actor->mHolder == nullptr) {
			if (phase < 8)
				*liveFlag |= LIVE_FLAG_HIDDEN;
			else
				*liveFlag &= ~LIVE_FLAG_HIDDEN;
		}

		actor    = unk2A8[index++];
		liveFlag = &actor->mLiveFlag;
		if (!(*liveFlag & LIVE_FLAG_DEAD) && actor->mHolder == nullptr) {
			if (phase < 8)
				*liveFlag |= LIVE_FLAG_HIDDEN;
			else
				*liveFlag &= ~LIVE_FLAG_HIDDEN;
		}
	}

	for (int i = 0; i < 10; ++i) {
		TLiveActor* actor = unk2F8[i];
		u32* liveFlag     = &actor->mLiveFlag;
		if (!(*liveFlag & LIVE_FLAG_DEAD) && actor->mHolder == nullptr) {
			if (phase < 8)
				*liveFlag |= LIVE_FLAG_HIDDEN;
			else
				*liveFlag &= ~LIVE_FLAG_HIDDEN;
		}

		actor    = unk320[i];
		liveFlag = &actor->mLiveFlag;
		if (!(*liveFlag & LIVE_FLAG_DEAD)) {
			if (phase < 8)
				*liveFlag |= LIVE_FLAG_HIDDEN;
			else
				*liveFlag &= ~LIVE_FLAG_HIDDEN;
		}
	}
}

void TBossTelesa::onAllCollision()
{
	offHitFlag(HIT_FLAG_NO_COLLISION);
	unk16C->offHitFlag(HIT_FLAG_NO_COLLISION);
	unk170->offHitFlag(HIT_FLAG_NO_COLLISION);
	unk174->offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBossTelesa::offAllCollision()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	unk16C->onHitFlag(HIT_FLAG_NO_COLLISION);
	unk170->onHitFlag(HIT_FLAG_NO_COLLISION);
	unk174->onHitFlag(HIT_FLAG_NO_COLLISION);
}

const char** TBossTelesa::getBasNameTable() const { return btelesa_bastable; }

void TBossTelesa::genAttacker()
{
	if (unk150) {
		TTelesa* telesa = (TTelesa*)gpConductor->makeOneEnemyAppear(
		    mPosition, "テレサマネージャー", 1);
		if (telesa)
			telesa->initAttacker(this);
		return;
	}

	TBossTelesaSaveLoadParams* params = (TBossTelesaSaveLoadParams*)unk15C;
	s32 count                         = params->mSLNumGenBubble.get();
	MtxPtr rootMtx                    = mMActor->unk4->mNodeMatrices[5];
	f32 angleStep                     = 180.0f / count;
	f32 angleOffset                   = count * 0.5f;

	for (int i = 0; i < count; ++i) {
		TBubble* bubble = (TBubble*)gpConductor->makeOneEnemyAppear(
		    mPosition, "バブルマネージャー", 1);
		if (!bubble)
			return;

		Mtx rot;
		Vec velocity;
		velocity.x = 0.0f;
		velocity.y = 0.0f;
		velocity.z = -50.0f;
		MsMtxSetRotRPH(rot, mRotation.x,
		               mRotation.y - angleOffset + angleStep * i, mRotation.z);
		PSMTXMultVec(rot, &velocity, &velocity);
		MsVECNormalize(&velocity, &velocity);

		velocity.y = 2.0f;
		f32 speed  = params->mSL1stBubbleSp.get();
		velocity.x *= speed;
		velocity.z *= speed;

		bubble->mPosition.x = rootMtx[0][3];
		bubble->mPosition.y = rootMtx[1][3] - 50.0f;
		bubble->mPosition.z = rootMtx[2][3];
		bubble->mVelocity   = velocity;
		bubble->mPosition.y += 10.0f;
		bubble->onLiveFlag(LIVE_FLAG_AIRBORNE);

		TMsRange<f32> itemRange(0.0f, 1.0f);
		f32 itemRoll = itemRange.rand();
		if (itemRoll < mItemGenRate) {
			bubble->unk198    = nullptr;
			TMapObjBase* item = gpItemManager->makeObjAppear(
			    bubble->mPosition.x, bubble->mPosition.y, bubble->mPosition.z,
			    0x20000008, true);
			if (item && item->receiveMessage(bubble, HIT_MESSAGE_TAKE)) {
				item->appear();
				item->mPosition = bubble->mPosition;
				item->mVelocity.set(0.0f, 15.0f, 0.0f);
				item->offLiveFlag(LIVE_FLAG_UNK10);
				bubble->mHeldObject = item;
				bubble->unk198      = item;
			}
		} else {
			TMsRange<f32> enemyRange(0.0f, 1.0f);
			f32 enemyRoll = enemyRange.rand();
			if (enemyRoll < mEnemyGenRate)
				bubble->appendEnemy();
		}
	}
}

void TBossTelesa::setBckAnm(int index)
{
	if (mMActor)
		mMActor->setBckFromIndex(index);
}

bool TBossTelesa::isInDamage() { return unk35B != 0; }

void TBossTelesa::rouletteStart()
{
	TMsRange<f32> speedRange(0.05f, 0.1f);
	TMsRange<f32> directionRange(-1.0f, 1.0f);
	f32 dir = directionRange.rand();

	TSpineEnemyParams* params = getSaveParam();
	u8 maxHitPoints = params ? getSaveParam()->mSLHitPointMax.get() : 1;
	f32 hpSpeed = (maxHitPoints - mHitPoints) * TBossTelesa::mRouletteUpRate;

	for (int i = 0; i < 3; ++i) {
		f32 direction = 1.0f;
		if (dir > 0.0f)
			direction = -1.0f;
		if (i == 0 || i == 2)
			direction = -direction;

		f32 speed                         = speedRange.rand();
		((TRoulette**)&unk178)[i]->unk144 = direction * (speed + hpSpeed);

		speed             = speedRange.rand();
		unk184->unk1E4[i] = direction * (speed + hpSpeed);
	}

	for (int i = 0; i < 3; ++i)
		((TRoulette**)&unk178)[i]->setRollSp(unk184->unk1E4[i]);

	SMSRumbleMgr->start(0x14, 0xf, (f32*)nullptr);
	gpCameraShake->startShake(CAM_SHAKE_MODE_UNK23, 1.0f);
}

void TBossTelesa::slotStart()
{
	unk18C = 1;
	unk184->moveStart();
	((TTelesaManager*)unk354)->telesaForceKill();
}

void TBossTelesa::slotStop()
{
	if (unk184)
		unk184->forceStopSlot(0);
}

int TBossTelesa::checkSlotResult()
{
	return unk184 ? unk184->getSlotResult() : -1;
}

void TBossTelesa::generateSlotItem()
{
	static const char* manNameTable[] = {
		"バブルマネージャー",   "ハムクリマネージャー",
		"ヤキグリマネージャー", "ボム兵マネージャー",
		"ポイハナマネージャー", "電気ノコノコマネージャー",
		"ポポマネージャー",     "ゲッソーマネージャー",
		"とびプクマネージャー",
	};

	unk368 = 0;
	unk274 = 0;

	TTelesaSlot* slot = (TTelesaSlot*)unk184;
	s32 result        = slot->getResultFromAng(slot->unk13C[0]);
	for (int i = 1; i < 3; ++i) {
		if (slot->getResultFromAng(slot->unk13C[i]) != result) {
			result = -1;
			break;
		}
	}
	unk1A8 = result;

	TBossTelesaSaveLoadParams* params = (TBossTelesaSaveLoadParams*)unk15C;
	s32 slotItemNum                   = params->mSLSlotItemNum.get();
	MtxPtr rootMtx                    = mMActor->unk4->mNodeMatrices[5];
	f32 angleStep                     = 120.0f / slotItemNum;
	f32 angleOffset                   = angleStep * slotItemNum * 0.5f;

	if (unk1A8 == 2) {
		s32 numFruit = params->mSLSlotFruitNum.get();
		if (numFruit > 20)
			numFruit = 20;

		TMsRange<s32> angleIndexRange(0, numFruit);
		s32 angleIndex = angleIndexRange.rand();

		for (int i = 0; i < numFruit; ++i) {
			if (unk2A8[i]->mHolder)
				continue;

			Mtx rot;
			Vec dir;
			Vec normalizedDir;
			dir.x = 0.0f;
			dir.y = 0.0f;
			dir.z = 200.0f;

			f32 fruitAngleStep = 160.0f / numFruit;
			MsMtxSetRotRPH(rot, mRotation.x,
			               mRotation.y - angleOffset
			                   + fruitAngleStep * angleIndex,
			               mRotation.z);

			if (numFruit > 0) {
				angleIndex += i;
				while (angleIndex >= numFruit)
					angleIndex -= numFruit;
				while (angleIndex < 0)
					angleIndex += numFruit;
			} else {
				angleIndex = 0;
			}

			PSMTXMultVec(rot, &dir, &dir);
			MsVECNormalize(&dir, &normalizedDir);

			Vec velocity;
			TMsRange<f32> fruitSpeedRange(6.0f, 10.0f);

			if (i == 0 || i == 4) {
				((TMapObjBase*)unk2F8[i])->makeObjAppeared();
				((TMapObjBase*)unk2F8[i])->offLiveFlag(LIVE_FLAG_HIDDEN);

				velocity.x = normalizedDir.x * fruitSpeedRange.rand();
				velocity.y = -2.0f;
				velocity.z = normalizedDir.z * fruitSpeedRange.rand();
				if (i == 0) {
					velocity.x
					    = normalizedDir.x * fruitSpeedRange.rand() * 2.0f;
					velocity.z
					    = normalizedDir.z * fruitSpeedRange.rand() * 2.0f;
				}

				((TMapObjBase*)unk2F8[i])->mVelocity = velocity;
				((TMapObjBase*)unk2F8[i])->offLiveFlag(LIVE_FLAG_UNK10);
				((TMapObjBase*)unk2F8[i])->mRotation.set(0.0f, 90.0f, 0.0f);
				unk1AC[unk274] = unk2F8[i];
			} else {
				((TMapObjBase*)unk2A8[i])->makeObjAppeared();
				((TMapObjBase*)unk2A8[i])->offLiveFlag(LIVE_FLAG_HIDDEN);

				velocity.x = normalizedDir.x * fruitSpeedRange.rand();
				velocity.y = -2.0f;
				velocity.z = normalizedDir.z * fruitSpeedRange.rand();

				((TMapObjBase*)unk2A8[i])->mVelocity = velocity;
				((TMapObjBase*)unk2A8[i])->offLiveFlag(LIVE_FLAG_UNK10);
				unk1AC[unk274] = unk2A8[i];
			}

			unk1AC[i]->onHitFlag(HIT_FLAG_NO_COLLISION);
			unk1AC[i]->mScaling.set(1.5f, 1.5f, 1.5f);
			unk1AC[unk274]->mPosition.x = rootMtx[0][3] + dir.x;
			unk1AC[unk274]->mPosition.y = rootMtx[1][3] - 50.0f;
			unk1AC[unk274]->mPosition.z = rootMtx[2][3] + dir.z;
			unk274++;
		}
	} else if (unk1A8 == 0) {
		s32 numItems = params->mSLSlotFruitNum.get();
		if (numItems > 10)
			numItems = 10;

		if (unk370)
			unk370--;
		else
			unk370 = 0;

		f32 itemAngleStep   = 120.0f / numItems;
		f32 itemAngleOffset = itemAngleStep * numItems * 0.5f;
		for (int i = 0; i < numItems; ++i) {
			if (i >= 10)
				return;

			Mtx rot;
			Vec dir;
			dir.x = 0.0f;
			dir.y = 0.0f;
			dir.z = 250.0f;
			MsMtxSetRotRPH(rot, mRotation.x,
			               mRotation.y - itemAngleOffset + itemAngleStep * i,
			               mRotation.z);
			PSMTXMultVec(rot, &dir, &dir);
			MsVECNormalize(&dir, &dir);
			dir.y = 10.0f;

			Vec velocity;
			TMsRange<f32> itemSpeedRange(0.8f, 3.5f);
			f32 speed  = params->mSL1stBubbleSp.get();
			velocity.x = dir.x * speed * itemSpeedRange.rand();
			velocity.y = dir.y;
			velocity.z = dir.z * speed * itemSpeedRange.rand();

			TMapObjBase* item = gpItemManager->makeObjAppeared(0x2000000E);
			item->mPosition.x = rootMtx[0][3];
			item->mPosition.y = rootMtx[1][3] - 250.0f;
			item->mPosition.z = rootMtx[2][3];
			item->mVelocity   = velocity;
			item->offLiveFlag(LIVE_FLAG_UNK10);
			item->mScaling.set(0.0f, 0.0f, 0.0f);
			((TItem*)item)->killByTimer(0x3C0);

			unk1AC[unk274] = unk320[i];
			unk1AC[unk274]->offLiveFlag(LIVE_FLAG_HIDDEN);
			unk274++;
		}
	} else {
		s32 count = slotItemNum;
		s32 kind  = 0;
		switch (unk1A8) {
		case -1:
			count *= 2;
			break;
		case 1:
			if (mHitPoints > 2)
				kind = 1;
			else
				kind = 2;
			break;
		case 3:
			break;
		}

		s32 maxKind = 7;
		if (mHitPoints == 1)
			maxKind = 8;

		TMsRange<s32> randomKindRange(1, maxKind);
		s32 randomKind = randomKindRange.rand();

		for (int i = 0; i < count; ++i) {
			if (unk1A8 == 3) {
				if ((i & 1) == 0)
					randomKind++;
				if (randomKind > maxKind)
					randomKind = 1;
				kind = randomKind;
			}

			TSmallEnemy* enemy = (TSmallEnemy*)gpConductor->makeOneEnemyAppear(
			    mPosition, manNameTable[kind], 2);
			if (!enemy)
				continue;

			if (kind != 0) {
				unk1AC[unk274] = enemy;
				unk274++;
			}

			Mtx rot;
			Vec dir;
			dir.x = 0.0f;
			dir.y = 0.0f;
			dir.z = 200.0f;
			MsMtxSetRotRPH(rot, mRotation.x,
			               mRotation.y - angleOffset + angleStep * i,
			               mRotation.z);
			PSMTXMultVec(rot, &dir, &dir);
			MsVECNormalize(&dir, &dir);
			dir.y = 2.0f;

			Vec velocity;
			TMsRange<f32> enemySpeedRange(0.5f, 1.0f);
			f32 speed  = params->mSL1stBubbleSp.get();
			velocity.x = dir.x * speed * enemySpeedRange.rand();
			velocity.y = dir.y * (2.0f + enemySpeedRange.rand());
			velocity.z = dir.z * speed * enemySpeedRange.rand();

			enemy->mPosition.x = rootMtx[0][3];
			enemy->mPosition.y = rootMtx[1][3] - 250.0f;
			enemy->mPosition.z = rootMtx[2][3];
			enemy->mVelocity   = velocity;
			enemy->mPosition.y += 10.0f;
			enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);
			PSMTXCopy(rootMtx, enemy->mMActor->unk4->unk20);
			enemy->mMActor->calc();
			enemy->initAttacker(this);
		}
	}
}

void TBossTelesa::fruitCollisionOn() { unk370 = 0; }

bool TBossTelesa::checkSlot()
{
	if (!unk184->isRollDrum() && unk18C) {
		unk18C = 0;
		mSpine->pushAfterCurrent(&TNerveBossTelesaSpitSlotItem::theNerve());
		return true;
	}

	return false;
}

bool TBossTelesa::checkAllItemDead()
{
	for (int i = 0; i < unk274; ++i)
		if (!(unk1AC[i]->mLiveFlag & LIVE_FLAG_DEAD))
			return false;
	return true;
}

void TBossTelesa::forceAllItemKill()
{
	f32 zero = 0.0f;

	for (int i = 0; i < unk274; ++i) {
		if (unk1AC[i]->mHolder) {
			SMS_SendMessageToMario(unk1AC[i], HIT_MESSAGE_UNK8);
			unk1AC[i]->mHolder = nullptr;
		}

		unk1AC[i]->mPosition.set(zero, zero, zero);
		unk1AC[i]->onHitFlag(HIT_FLAG_NO_COLLISION);

		if (!(unk1AC[i]->mLiveFlag & LIVE_FLAG_DEAD)) {
			unk1AC[i]->kill();
			gpMarioParticleManager->emit(PARTICLE_MS_TLS_CHANGE,
			                             &unk1AC[i]->mPosition, 0, nullptr);
		}
	}
}

void TBossTelesa::rollRouletteCircle()
{
	if (unk184)
		unk184->moveStart();
}

bool TBossTelesa::isForceRestart() { return unk35A != 0; }

void TBossTelesa::forceHide()
{
	if (mSpine->getCurrentNerve() == &TNerveBossTelesaDie::theNerve())
		return;
	if (mMActor->checkCurBckFromIndex(4))
		return;
	if (mMActor->checkCurBckFromIndex(0))
		return;

	forceAllItemKill();
	unk368 = 0;

	if (unk350) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_ESCAPE, &mPosition, 0,
		                                nullptr, 0, 4);
	} else {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_DISAPPEAR, &mPosition,
		                                0, nullptr, 0, 4);
	}

	mSpine->reset();
	mSpine->setNext(&TNerveBossTelesaHide::theNerve());
}

void TBossTelesa::fanfale() { }

DEFINE_NERVE(TNerveBossTelesaDie, TLiveActor)
{
	TBossTelesa* boss = getBoss(spine);
	if (spine->getTime() == 0) {
		boss->unk388 = 0;
		if (boss->unk350 && boss->mHitPoints != 0)
			--boss->mHitPoints;

		boss->onHitFlag(HIT_FLAG_NO_COLLISION);
		boss->unk16C->onHitFlag(HIT_FLAG_NO_COLLISION);
		boss->unk170->onHitFlag(HIT_FLAG_NO_COLLISION);

		if (boss->mHitPoints != 0) {
			if (boss->unk350) {
				boss->mMActor->setBrkFromIndex(1);
				boss->unk164 = boss->mMActor->getCurAnmIdx(0);
				boss->unk160 = 2;
				boss->unk168 = 1.0f;

				MActor* anmActor = boss->mMActor;
				anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

				boss->mMActor->setBckFromIndex(2);
				boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

				const char** basTable = boss->getBasNameTable();
				const char* basName;
				if (!basTable)
					basName = nullptr;
				else
					basName = basTable[2];
				boss->setAnmSound(basName);
				gpCameraShake->startShake(CAM_SHAKE_MODE_UNK1F, 1.0f);
			} else {
				boss->unk164 = boss->mMActor->getCurAnmIdx(0);
				boss->unk160 = 5;
				boss->unk168 = 1.0f;

				MActor* anmActor = boss->mMActor;
				anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

				boss->mMActor->setBckFromIndex(5);
				boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

				const char** basTable = boss->getBasNameTable();
				const char* basName;
				if (!basTable)
					basName = nullptr;
				else
					basName = basTable[5];
				boss->setAnmSound(basName);
				boss->mMActor->setBrkFromIndex(0);
				gpCameraShake->startShake(CAM_SHAKE_MODE_UNK20, 1.0f);
			}
		} else {
			MSBgm::stopBGM(0x8001000D, 10);
			gpCameraShake->startShake(CAM_SHAKE_MODE_UNK21, 1.0f);
			boss->unk164 = boss->mMActor->getCurAnmIdx(0);
			boss->unk160 = 3;
			boss->unk168 = 1.0f;

			MActor* anmActor = boss->mMActor;
			anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

			boss->mMActor->setBckFromIndex(3);
			boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

			const char** basTable = boss->getBasNameTable();
			const char* basName;
			if (!basTable)
				basName = nullptr;
			else
				basName = basTable[3];
			boss->setAnmSound(basName);
			boss->mMActor->setBrkFromIndex(1);
			boss->unk184->mScaling.set(0.0f, 0.0f, 0.0f);

			SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_DOWN,
			                                &boss->mPosition, 0, nullptr, 0, 4);
		}
	}

	if (boss->mHitPoints == 0) {
		if (boss->checkCurAnmEnd(0)) {
			if (boss->unk388 == 0) {
				MtxPtr mtx = boss->mMActor->getModel()->mNodeMatrices[1];
				boss->unk374.set(mtx[0][3], mtx[1][3], mtx[2][3]);

				gpMarioParticleManager->emit(SCENE_BTELESA_JPA_MS_BTLS_DOWN,
				                             &boss->unk374, 0, nullptr);
				if (boss->unk380 == SCENE_BTELESA_JPA_MS_BTLS_FHIT_PE) {
					gpMarioParticleManager->emit(
					    SCENE_BTELESA_JPA_MS_BTLS_DOWN_PE, &boss->unk374, 0,
					    nullptr);
				} else if (boss->unk380 == SCENE_BTELESA_JPA_MS_BTLS_FHIT_GR) {
					gpMarioParticleManager->emit(
					    SCENE_BTELESA_JPA_MS_BTLS_DOWN_GR, &boss->unk374, 0,
					    nullptr);
				} else {
					gpMarioParticleManager->emit(
					    SCENE_BTELESA_JPA_MS_BTLS_DOWN_OR, &boss->unk374, 0,
					    nullptr);
				}

				boss->forceAllItemKill();
				boss->unk178->unk144    = 0.0f;
				boss->unk184->unk1E4[0] = 0.0f;
				boss->unk17C->unk144    = 0.0f;
				boss->unk184->unk1E4[1] = 0.0f;
				boss->unk180->unk144    = 0.0f;
				boss->unk184->unk1E4[2] = 0.0f;
				for (int i = 0; i < 3; ++i)
					((TRoulette**)&boss->unk178)[i]->setRollSp(
					    boss->unk184->unk1E4[i]);
			}

			if (boss->unk388 > 240) {
				boss->unk388 = 0;
				gpItemManager->makeShineAppearWithDemo(
				    "シャイン（ボス用）", "ボスシャインカメラ",
				    boss->mPosition.x, boss->mPosition.y, boss->mPosition.z);
				boss->onLiveFlag(LIVE_FLAG_DEAD);
				boss->onLiveFlag(LIVE_FLAG_UNK8);
				boss->offLiveFlag(LIVE_FLAG_UNK10000);
				boss->mHolder = nullptr;
				boss->onHitFlag(HIT_FLAG_NO_COLLISION);
				boss->unk16C->onHitFlag(HIT_FLAG_NO_COLLISION);
				boss->unk170->onHitFlag(HIT_FLAG_NO_COLLISION);
				boss->stopAnmSound();
				spine->reset();
				return TRUE;
			}

			boss->unk388++;
		}
		return FALSE;
	}

	if (boss->checkCurAnmEnd(0)) {
		if (boss->mMActor->checkCurBckFromIndex(5)) {
			boss->mMActor->setBrkFromIndex(2);
			boss->unk164 = boss->mMActor->getCurAnmIdx(0);
			boss->unk160 = 7;
			boss->unk168 = 1.0f;

			MActor* anmActor = boss->mMActor;
			anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

			boss->mMActor->setBckFromIndex(7);
			boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

			const char** basTable = boss->getBasNameTable();
			const char* basName;
			if (!basTable)
				basName = nullptr;
			else
				basName = basTable[7];
			boss->setAnmSound(basName);
		} else if (boss->mMActor->checkCurBckFromIndex(7)) {
			boss->unk164 = boss->mMActor->getCurAnmIdx(0);
			boss->unk160 = 6;
			boss->unk168 = 1.0f;

			MActor* anmActor = boss->mMActor;
			anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

			boss->mMActor->setBckFromIndex(6);
			boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

			const char** basTable = boss->getBasNameTable();
			const char* basName;
			if (!basTable)
				basName = nullptr;
			else
				basName = basTable[6];
			boss->setAnmSound(basName);
		} else {
			SMS_ResetDamageFogEffect(boss->mMActor->getModel()->getModelData());

			if (boss->mMActor->checkCurBckFromIndex(6)) {
				boss->offHitFlag(HIT_FLAG_NO_COLLISION);
				boss->unk16C->offHitFlag(HIT_FLAG_NO_COLLISION);
				boss->unk170->offHitFlag(HIT_FLAG_NO_COLLISION);

				boss->unk164 = boss->mMActor->getCurAnmIdx(0);
				boss->unk160 = 15;
				boss->unk168 = 1.0f;

				MActor* anmActor = boss->mMActor;
				anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

				boss->mMActor->setBckFromIndex(15);
				boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

				const char** basTable = boss->getBasNameTable();
				const char* basName;
				if (!basTable)
					basName = nullptr;
				else
					basName = basTable[15];
				boss->setAnmSound(basName);
				boss->mMActor->setBtpFromIndex(2);
				spine->pushAfterCurrent(
				    &TNerveBossTelesaPrepareSlot::theNerve());
			} else {
				boss->damageRecover();
			}
			return TRUE;
		}
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaSpit, TLiveActor)
{
	TBossTelesa* boss = getBoss(spine);
	if (spine->getTime() == 0 || !boss->mMActor->checkCurBckFromIndex(14)) {
		boss->unk164 = boss->mMActor->getCurAnmIdx(0);
		boss->unk160 = 14;
		boss->unk168 = 1.0f;

		MActor* anmActor = boss->mMActor;
		anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

		boss->mMActor->setBckFromIndex(14);
		boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

		const char** basTable = boss->getBasNameTable();
		const char* basName;
		if (!basTable)
			basName = nullptr;
		else
			basName = basTable[14];
		boss->setAnmSound(basName);
	} else {
		if (boss->mMActor->getFrameCtrl(0)->checkPass(40.0f))
			boss->genAttacker();
	}

	if (boss->checkCurAnmEnd(0))
		return true;
	return false;
}

DEFINE_NERVE(TNerveBossTelesaHide, TLiveActor)
{
	TBossTelesa* boss = getBoss(spine);
	if (!boss->mMActor->checkCurBckFromIndex(4)) {
		boss->unk164 = boss->mMActor->getCurAnmIdx(0);
		boss->unk160 = 4;
		boss->unk168 = 1.0f;

		MActor* anmActor = boss->mMActor;
		anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

		boss->mMActor->setBckFromIndex(4);
		boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

		const char** basTable = boss->getBasNameTable();
		const char* basName;
		if (!basTable)
			basName = nullptr;
		else
			basName = basTable[4];
		boss->setAnmSound(basName);
		boss->mMActor->setBtpFromIndex(2);
	}

	if (boss->checkCurAnmEnd(0)) {
		boss->onHitFlag(HIT_FLAG_NO_COLLISION);
		boss->unk16C->onHitFlag(HIT_FLAG_NO_COLLISION);
		boss->unk170->onHitFlag(HIT_FLAG_NO_COLLISION);
		SMSRumbleMgr->start(0x14, 0xF, (f32*)nullptr);
		boss->rouletteStart();
		spine->pushAfterCurrent(&TNerveBossTelesaHideWait::theNerve());
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaHideWait, TLiveActor)
{
	TBossTelesa* boss = getBoss(spine);
	if (spine->getTime() == 0) {
		boss->onLiveFlag(LIVE_FLAG_HIDDEN);
		boss->unk350 = 0;

		u8 maxHp = boss->getSaveParam()
		               ? boss->getSaveParam()->mSLHitPointMax.get()
		               : 1;
		u8 alpha = TBossTelesa::mNormalAlpha + (maxHp - boss->mHitPoints) * 30;
		alpha    = MsClamp<u8>(alpha, 0, 0xFE);
		boss->unk34C.a = alpha;

		boss->unk184->mScaling.set(0.0f, 0.0f, 0.0f);
		boss->mMActor->setBrkFromIndex(2);
		s16 endFrame = boss->mMActor->getFrameCtrl(5)->getEnd();
		boss->mMActor->getFrameCtrl(5)->setFrame((f32)endFrame);
	} else {
		JGeometry::TVec3<f32> pos = boss->mPosition;
		pos.sub(*gpMarioPos);

		if (spine->getTime() > 400
		    && ((TBossTelesaKillSmallEnemy*)boss->unk174)->unk6C == 0) {
			spine->pushAfterCurrent(&TNerveBossTelesaAppear::theNerve());
			boss->offLiveFlag(LIVE_FLAG_HIDDEN);
			return TRUE;
		}
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaAppear, TLiveActor)
{
	TBossTelesa* boss = getBoss(spine);
	if (spine->getTime() == 0) {
		if (!boss->mMActor->checkCurBckFromIndex(0)) {
			boss->unk164 = boss->mMActor->getCurAnmIdx(0);
			boss->unk160 = 0;
			boss->unk168 = 1.0f;

			MActor* anmActor = boss->mMActor;
			anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

			boss->mMActor->setBckFromIndex(0);
			boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

			const char** basTable = boss->getBasNameTable();
			const char* basName;
			if (!basTable)
				basName = nullptr;
			else
				basName = basTable[0];
			boss->setAnmSound(basName);

			if (!boss->unk384) {
				boss->unk384 = 1;
				MSBgm::startBGM(0x8001000D);
			}

			boss->unk184->mScaling.set(1.0f, 1.0f, 1.0f);
			boss->unk184->randomReset();
			boss->offHitFlag(HIT_FLAG_NO_COLLISION);
			boss->unk16C->offHitFlag(HIT_FLAG_NO_COLLISION);
			boss->unk170->offHitFlag(HIT_FLAG_NO_COLLISION);
		}
	} else if (boss->checkCurAnmEnd(0)
	           && !boss->mMActor->checkCurBckFromIndex(15)) {
		boss->unk164 = boss->mMActor->getCurAnmIdx(0);
		boss->unk160 = 15;
		boss->unk168 = 1.0f;

		MActor* anmActor = boss->mMActor;
		anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

		boss->mMActor->setBckFromIndex(15);
		boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

		const char** basTable = boss->getBasNameTable();
		const char* basName;
		if (!basTable)
			basName = nullptr;
		else
			basName = basTable[15];
		boss->setAnmSound(basName);
		boss->mMActor->setBtpFromIndex(2);
	}

	if (boss->mMActor->checkCurBckFromIndex(0)
	    && boss->mMActor->getFrameCtrl(0)->checkPass(78.0f)) {
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK22, 1.0f);
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_SLT_LAND,
		                                &boss->mPosition, 0, nullptr, 0, 4);
	}

	if (spine->getTime() > 800) {
		u8 maxHp     = boss->getSaveParam()
		                   ? boss->getSaveParam()->mSLHitPointMax.get()
		                   : 1;
		int interval = TBossTelesa::mTelesaGenerateInterval
		               + (maxHp - boss->mHitPoints) * 100;
		if (spine->getTime() % interval == 1) {
			boss->unk164 = boss->mMActor->getCurAnmIdx(0);
			boss->unk160 = 14;
			boss->unk168 = 1.0f;

			MActor* anmActor = boss->mMActor;
			anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

			boss->mMActor->setBckFromIndex(14);
			boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

			const char** basTable = boss->getBasNameTable();
			const char* basName;
			if (!basTable)
				basName = nullptr;
			else
				basName = basTable[14];
			boss->setAnmSound(basName);
		}
	}

	boss->unk364 *= 0.96f;
	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaSlotStart, TLiveActor)
{
	TBossTelesa* boss = getBoss(spine);
	if (spine->getTime() == 0) {
		boss->unk164 = boss->mMActor->getCurAnmIdx(0);
		boss->unk160 = 11;
		boss->unk168 = 1.0f;

		MActor* anmActor = boss->mMActor;
		anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

		boss->mMActor->setBckFromIndex(11);
		boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

		const char** basTable = boss->getBasNameTable();
		const char* basName;
		if (!basTable)
			basName = nullptr;
		else
			basName = basTable[11];
		boss->setAnmSound(basName);
	}

	if (boss->mMActor->checkCurBckFromIndex(11)) {
		if (boss->mMActor->getFrameCtrl(0)->checkPass(53.0f)) {
			boss->slotStart();
		}

		if (boss->checkCurAnmEnd(0)) {
			boss->unk164 = boss->mMActor->getCurAnmIdx(0);
			boss->unk160 = 15;
			boss->unk168 = 1.0f;

			MActor* anmActor = boss->mMActor;
			anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

			boss->mMActor->setBckFromIndex(15);
			boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

			const char** basTable = boss->getBasNameTable();
			const char* basName;
			if (!basTable)
				basName = nullptr;
			else
				basName = basTable[15];
			boss->setAnmSound(basName);
			boss->mMActor->setBtpFromIndex(2);
			boss->unk184->forceStopSlot(1);
		}
	}

	if (boss->checkSlot())
		return TRUE;

	boss->unk364 *= 0.99f;
	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaSpitSlotItem, TLiveActor)
{
	TBossTelesa* boss = getBoss(spine);
	if (!boss->mMActor->checkCurBckFromIndex(14)
	    && boss->unk364 < TBossTelesa::mBaseHoseiPosY - 200.0f) {
		boss->unk164 = boss->mMActor->getCurAnmIdx(0);
		boss->unk160 = 14;
		boss->unk168 = 1.0f;

		MActor* anmActor = boss->mMActor;
		anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

		boss->mMActor->setBckFromIndex(14);
		boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

		const char** basTable = boss->getBasNameTable();
		const char* basName;
		if (!basTable)
			basName = nullptr;
		else
			basName = basTable[14];
		boss->setAnmSound(basName);
	} else if (boss->checkCurAnmEnd(0) && spine->getTime() > 600) {
		spine->pushAfterCurrent(&TNerveBossTelesaPrepareSlot::theNerve());
		boss->unk368 = 0;
		for (int i = 0; i < boss->unk274; ++i) {
			TLiveActor* actor = boss->unk1AC[i];
			if (!(actor->mLiveFlag & LIVE_FLAG_DEAD)
			    && actor->mActorType != 0x2000000E
			    && actor->mActorType != 0x20000002)
				actor->offHitFlag(HIT_FLAG_NO_COLLISION);
		}
		return TRUE;
	} else if (spine->getTime() > 200) {
		boss->unk364 -= 100.0f;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaPrepareSlot, TLiveActor)
{
	TBossTelesa* boss = getBoss(spine);
	if (spine->getTime() == 0) {
		boss->unk164 = boss->mMActor->getCurAnmIdx(0);
		boss->unk160 = 15;
		boss->unk168 = 1.0f;

		MActor* anmActor = boss->mMActor;
		anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

		boss->mMActor->setBckFromIndex(15);
		boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

		const char** basTable = boss->getBasNameTable();
		const char* basName;
		if (!basTable)
			basName = nullptr;
		else
			basName = basTable[15];
		boss->setAnmSound(basName);
		boss->mMActor->setBtpFromIndex(2);
	}

	if (boss->unk350) {
		boss->unk36C++;
		if (boss->checkCurAnmEnd(0)) {
			if (boss->mMActor->checkCurBckFromIndex(1)) {
				boss->unk164 = boss->mMActor->getCurAnmIdx(0);
				boss->unk160 = 12;
				boss->unk168 = 1.0f;

				MActor* anmActor = boss->mMActor;
				anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

				boss->mMActor->setBckFromIndex(12);
				boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

				const char** basTable = boss->getBasNameTable();
				const char* basName;
				if (!basTable)
					basName = nullptr;
				else
					basName = basTable[12];
				boss->setAnmSound(basName);
				boss->mMActor->setBtpFromIndex(1);
			} else if (boss->mMActor->checkCurBckFromIndex(12)) {
				TBossTelesaSaveLoadParams* params
				    = (TBossTelesaSaveLoadParams*)boss->unk15C;
				if (boss->unk36C > params->mSLSpicyTime.get()) {
					boss->unk164 = boss->mMActor->getCurAnmIdx(0);
					boss->unk160 = 13;
					boss->unk168 = 1.0f;

					MActor* anmActor = boss->mMActor;
					anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

					boss->mMActor->setBckFromIndex(13);
					boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

					const char** basTable = boss->getBasNameTable();
					const char* basName;
					if (!basTable)
						basName = nullptr;
					else
						basName = basTable[13];
					boss->setAnmSound(basName);
				}
			} else {
				boss->unk36C = 0;
				boss->unk350 = 0;

				u8 maxHp = boss->getSaveParam()
				               ? boss->getSaveParam()->mSLHitPointMax.get()
				               : 1;
				u8 alpha = TBossTelesa::mNormalAlpha
				           + (maxHp - boss->mHitPoints) * 30;
				if (alpha > 0xFE)
					alpha = 0xFE;

				boss->unk34C.a = alpha;

				boss->unk164 = boss->mMActor->getCurAnmIdx(0);
				boss->unk160 = 15;
				boss->unk168 = 1.0f;

				MActor* anmActor = boss->mMActor;
				anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

				boss->mMActor->setBckFromIndex(15);
				boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

				const char** basTable = boss->getBasNameTable();
				const char* basName;
				if (!basTable)
					basName = nullptr;
				else
					basName = basTable[15];
				boss->setAnmSound(basName);
				boss->mMActor->setBtpFromIndex(2);
			}
		}
	}

	boss->unk368++;

	TBossTelesaSaveLoadParams* params
	    = (TBossTelesaSaveLoadParams*)boss->unk15C;
	int waitTime = params->mSLStopSlotTime0.get();
	if (boss->mHitPoints == 2)
		waitTime = params->mSLStopSlotTime1.get();
	if (boss->mHitPoints == 1)
		waitTime = params->mSLStopSlotTime2.get();

	if (boss->unk184->getSlotResult() == 0)
		waitTime = waitTime * 0.5f;

	if (boss->unk368 > waitTime - 120)
		boss->flashItem(waitTime - boss->unk368);

	if (boss->mMActor->checkCurBckFromIndex(15)) {
		BOOL done;
		if (boss->unk1A8 == -1) {
			done = TRUE;
		} else {
			done = TRUE;
			for (int i = 0; i < boss->unk274; ++i) {
				if (!(boss->unk1AC[i]->mLiveFlag & LIVE_FLAG_DEAD)) {
					done = FALSE;
					break;
				}
			}
		}

		if (done || boss->unk368 > waitTime) {
			boss->unk368 = 0;
			boss->forceAllItemKill();

			if (boss->unk350) {
				SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_ESCAPE,
				                                &boss->mPosition, 0, nullptr, 0,
				                                4);
			} else {
				SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_DISAPPEAR,
				                                &boss->mPosition, 0, nullptr, 0,
				                                4);
			}

			spine->setNext(&TNerveBossTelesaHide::theNerve());
			spine->pushAfterCurrent(&TNerveBossTelesaHide::theNerve());
			return TRUE;
		}
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaFreeze, TLiveActor)
{
	TBossTelesa* boss = getBoss(spine);
	if (boss->mMActor->checkCurBckFromIndex(16)) {
		if (boss->checkCurAnmEnd(0)) {
			boss->unk350 = 0;

			u8 maxHp = boss->getSaveParam()
			               ? boss->getSaveParam()->mSLHitPointMax.get()
			               : 1;
			u8 alpha
			    = TBossTelesa::mNormalAlpha + (maxHp - boss->mHitPoints) * 30;
			alpha = MsClamp<u8>(alpha, 0, 0xFE);

			boss->unk34C.a = alpha;
			return TRUE;
		}
	} else {
		boss->unk164 = boss->mMActor->getCurAnmIdx(0);
		boss->unk160 = 16;
		boss->unk168 = 1.0f;

		MActor* anmActor = boss->mMActor;
		anmActor->setBckOldMotionBlendAnmPtr(anmActor->getBckAnm());

		boss->mMActor->setBckFromIndex(16);
		boss->mMActor->setMotionBlendRatioForBck(boss->unk168);

		const char** basTable = boss->getBasNameTable();
		const char* basName;
		if (!basTable)
			basName = nullptr;
		else
			basName = basTable[16];
		boss->setAnmSound(basName);

		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_THANKYOU,
		                                &boss->mPosition, 0, nullptr, 0, 4);
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaFallDemo, TLiveActor)
{
	TBossTelesa* boss = getBoss(spine);
	if (spine->getTime() == 0) {
		boss->onLiveFlag(LIVE_FLAG_HIDDEN);
		if (SMS_SendMessageToMario(boss, HIT_MESSAGE_TAKE))
			boss->mHeldObject = (TTakeActor*)SMS_GetMarioHitActor();
		boss->mMActor->setFrameRate(0.0f, 0);
		boss->unk184->mScaling.set(0.0f, 0.0f, 0.0f);
	}

	if (boss->rouletteFall()) {
		JGeometry::TVec3<f32> diff = boss->mPosition;
		diff.sub(*gpMarioPos);

		if (boss->slotFall()) {
			boss->offHitFlag(HIT_FLAG_NO_COLLISION);
			boss->unk16C->offHitFlag(HIT_FLAG_NO_COLLISION);
			boss->unk170->offHitFlag(HIT_FLAG_NO_COLLISION);
			spine->reset();
			spine->setNext(&TNerveBossTelesaHideWait::theNerve());
			spine->pushAfterCurrent(&TNerveBossTelesaHideWait::theNerve());
			return TRUE;
		}
	}

	return FALSE;
}
