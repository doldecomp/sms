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
#include <M3DUtil/MActorAnm.hpp>
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
		{ "btelesa_osenbubbles_ind.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_MaterialUseIndirect
		      | (2 << J3DMLF_TevStageNumShift),
		  0 },
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
	mActorType = ACTOR_TYPE_EFFECT_OBJ;
	unk150     = 0x11;
	unk194     = (TBubbleSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveBubbleLive::theNerve());
	mMActor->setLightType(3);

	TScreenTexture* tex = static_cast<TScreenTexture*>(
	    JDrama::TNameRefGen::search("スクリーンテクスチャ"));
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
		TItem* item = (TItem*)gpItemManager->makeObjAppear(
		    mPosition.x, mPosition.y + 50.0f, mPosition.z,
		    ACTOR_TYPE_BOTTLE_LARGE, true);
		if (item)
			item->killByTimer(0x4B0);
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

MtxPtr TBubble::getTakingMtx() { return mMActor->getModel()->getBaseTRMtx(); }

const char** TBubble::getBasNameTable() const { return btelesa_bastable; }

DEFINE_NERVE_INSTANCE(TNerveBubbleLive)

void TBubble::appendItem()
{
	unk198            = nullptr;
	TMapObjBase* item = gpItemManager->makeObjAppear(
	    mPosition.x, mPosition.y, mPosition.z, ACTOR_TYPE_ITEM_UNK8, true);
	if (item && item->receiveMessage(this, HIT_MESSAGE_TAKE)) {
		item->appear();
		item->mPosition = mPosition;
		item->mVelocity.set(0.0f, 15.0f, 0.0f);
		item->offLiveFlag(LIVE_FLAG_UNK10);
		mHeldObject = item;
		unk198      = item;
	}
}

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
		enemy->onHitFilter(HIT_CATEGORY_BOSS);
		mHeldObject = enemy;
		JGeometry::TVec3<f32> velocity;
		velocity.set(0.0f, 1.0f, -1.0f);
		enemy->mVelocity = velocity;
		enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);
		unk198 = enemy;
	}
}

DEFINE_NERVE_EXECUTE(TNerveBubbleLive, TLiveActor)
{
	TBubble* bubble = (TBubble*)spine->getBody();
	if (spine->getTime() == 0) {
		bubble->offHitFilter(HIT_FILTER_NO_COLLISION);

		if (!bubble->unk1D0) {
			bubble->setBckAnm(8);
		} else {
			bubble->setBckAnm(10);
			bubble->setGoalPath((THitActor*)gpMarioAddress);
		}

		J3DFrameCtrl* frameCtrl = bubble->mMActor->getFrameCtrl(0);
		TMsRange<f32> frameRange(0.0f, 20.0f);
		frameCtrl->setFrame(frameRange.rand());
		bubble->onLiveFlag(LIVE_FLAG_UNK8);
	} else if (bubble->checkCurAnmEnd(0)) {
		bubble->offHitFilter(HIT_FILTER_NO_COLLISION);
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
			bubble->unk1D1 = 0;
			bubble->mVelocity.set(0.0f, 0.0f, 0.0f);
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
	TBubble* bubble = (TBubble*)spine->getBody();
	if (spine->getTime() == 0) {
		bubble->onHitFilter(HIT_FILTER_NO_COLLISION);
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
		{ "btelesa.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_MaterialTexGenFull
		      | J3DMLF_MaterialUseIndirect | J3DMLF_UseUniqueMaterials
		      | (16 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

const f32 bossTelesaDirectionTable[] = {
	1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
};

void TBossTelesaManager::perform(u32 cue, JDrama::TGraphics* gfx)
{
	TEnemyManager::perform(cue, gfx);
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
		if (actor->isActorType(ACTOR_TYPE_MARIO))
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		else
			unk68->checkHitObject(actor);
	}
}

void TBossTelesaTongue::checkHit()
{
	for (int i = 0; i < mColCount; ++i) {
		THitActor* actor = mCollisions[i];
		if (actor->isActorType(ACTOR_TYPE_MARIO))
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		else if (actor->isActorType(ACTOR_TYPE_RED_PEPPER))
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
		if (actor->isHitCategory(HIT_CATEGORY_ENEMY)) {
			if (actor->isActorType(ACTOR_TYPE_DORO_HAMU_KURI))
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

void TTelesaSlot::randomReset()
{
	TMsRange<s32> indexRange(0, 8);

	for (int i = 0; i < 3; ++i) {
		unk13C[i] = (f32)(unk168 * indexRange.rand());
		unk198[i] = false;
	}
}

void TTelesaSlot::calcRootMatrix()
{
	bool rolling = false;

	if (unk138[0] != 0.0f)
		rolling = true;
	if (unk138[1] != 0.0f)
		rolling = true;
	if (unk138[2] != 0.0f)
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
					f32 rate = unk1A0->unk15C->mSLSlotHitCollectRate.get();

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

			if (allStopped)
				unk1A0->fanfale();
		}
	}
}

void TTelesaSlot::moveStart()
{
	unk19C = true;
	unk19B = true;

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

void TTelesaSlot::forceStopSlot(int idx)
{
	f32 collectRate;

	if (!unk19C)
		return;

	collectRate = unk1A0->unk15C->mSLSlotFirstHitCollectRate.get();

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

	if (unk1A4 == unk1A0->unk1A8)
		unk1A4 = 3;

	if (unk1A4 == 0) {
		if (unk1A0->unk370 == 0)
			unk1A4 = 1;
		else if (SMS_GetMarioHP() >= 6)
			unk1A4 = 3;
	}

	unk19C = false;
}

bool TTelesaSlot::isRollDrum()
{
	if (unk198[0])
		return true;
	if (unk198[1])
		return true;
	if (unk198[2])
		return true;

	unk19B = false;
	return false;
}

int TTelesaSlot::getSlotResult()
{
	int result = getDrumResult(0);
	for (int i = 1; i < 3; ++i) {
		if (getDrumResult(i) != result)
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

// UNUSED
void TTelesaSlot::calcObjCollision() { }

// UNUSED
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
    , mBodyCollision(nullptr)
    , mSlot(nullptr)
    , mSlotParts(nullptr)
    , unk18C(0)
    , unk1A8(0)
    , mSpitObjNum(0)
    , mIsSpicy(0)
    , mTelesaManager(0)
    , mMarioHP(0)
    , unk35A(1)
    , unk35B(1)
    , mFruitHitCount(0)
    , mCameraOffsetY(0.0f)
    , mHoseiPosY(0.0f)
    , unk368(0)
    , mSpicyTimer(0)
    , unk370(3)
    , unk384(0)
{
}

void TBossTelesa::init(TLiveManager* manager)
{
	mManager = manager;
	manager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("btelesa.bmd", 3);

	onLiveFlag(LIVE_FLAG_DEAD);
	onHitFilter(HIT_FILTER_NO_COLLISION);

	TSpineEnemyParams* params = getSaveParam();
	if (params) {
		mBodyRadius       = params->mSLBodyRadius.get();
		mWallRadius       = params->mSLWallRadius.get();
		mHeadHeight       = params->mSLHeadHeight.get();
		mScaledBodyRadius = mBodyScale * mBodyRadius;
		resetHitPoints();
	}

	initHitActor(0, 5,
	             HIT_CATEGORY_PLAYER | HIT_CATEGORY_ENEMY | HIT_CATEGORY_BOSS,
	             mBodyRadius, mHeadHeight, mBodyRadius, mHeadHeight);
	mGroundPlane = TMap::getIllegalCheckData();
	setGoalPath((THitActor*)gpMarioAddress);
	initAnmSound();
	unk15C     = static_cast<TBossTelesaSaveLoadParams*>(getSaveParam());
	mActorType = ACTOR_TYPE_BOSS_TELESA;
	onHitFilter(HIT_CATEGORY_PLAYER | HIT_CATEGORY_MAP_OBJECT
	            | HIT_CATEGORY_ENEMY);
	mSpine->initWith(&TNerveBossTelesaFallDemo::theNerve());
	onLiveFlag(LIVE_FLAG_UNK8);

	mMActor->setLightType(1);
	mMActor->markUnk40();
	mMActor->initBckNormalMotionBlend();

	J3DModel* model = mMActor->getModel();
	if (!model->getSkinDeform())
		model->setSkinDeform(new J3DSkinDeform, J3D_DEFORM_ATTACH_FLAG_UNK_1);
	mMActor->resetDL();

	unk348.r = unk348.g = unk348.b = unk348.a = 0xFF;
	unk34C.r = unk34C.g = unk34C.b = unk34C.a = mNormalAlpha;

	for (int i = 0; i < mMActor->getModel()->getModelData()->getMaterialNum();
	     ++i) {
		if (i
		    == getModel()->getModelData()->getMaterialName()->getIndex(
		        "_mat_body")) {
			SMS_InitPacket_TwoTevKColor(mMActor->getModel(), i, GX_KCOLOR0,
			                            &unk34C, GX_KCOLOR1, &unk348);
		} else {
			SMS_InitPacket_OneTevKColor(mMActor->getModel(), i, GX_KCOLOR0,
			                            &unk34C);
		}
	}

	reset();

	for (int i = 0; i < gpMapObjManager->getObjNum(); ++i) {
		if (gpMapObjManager->getObj(i)->isActorType(ACTOR_TYPE_CASINORULET)) {
			unk154    = nullptr;
			unk158    = nullptr;
			mPosition = gpMapObjManager->getObj(i)->mPosition;
		}
	}

	if (mInstanceIndex == 0) {
		for (u8 i = 0; i < mMActor->getModel()->getModelData()->getJointNum();
		     ++i) { }
	}

	mBodyCollision = new TBossTelesaBody("ボステレサ体コリジョン");
	static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"))
	    ->getChildren()
	    .push_back(mBodyCollision);
	mBodyCollision->initHitActor(ACTOR_TYPE_BOSS_TELESA, 5,
	                             HIT_CATEGORY_PLAYER | HIT_CATEGORY_MAP_OBJECT
	                                 | HIT_CATEGORY_ENEMY | HIT_CATEGORY_WATER,
	                             350.0f, 550.0f, 300.0f, 500.0f);
	mBodyCollision->unk68 = this;
	mBodyCollision->offHitFilter(HIT_FILTER_NO_COLLISION);

	mTongueCollision = new TBossTelesaTongue("ボステレサ舌コリジョン");
	static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"))
	    ->getChildren()
	    .push_back(mTongueCollision);
	mTongueCollision->initHitActor(ACTOR_TYPE_BOSS_TELESA, 5,
	                               HIT_CATEGORY_PLAYER
	                                   | HIT_CATEGORY_MAP_OBJECT,
	                               180.0f, 350.0f, 180.0f, 350.0f);
	mTongueCollision->unk68 = this;
	mTongueCollision->offHitFilter(HIT_FILTER_NO_COLLISION);

	mKillSmallEnemyCollision
	    = new TBossTelesaKillSmallEnemy("ボステレサ雑魚敵死コリジョン");
	static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"))
	    ->getChildren()
	    .push_back(mKillSmallEnemyCollision);
	mKillSmallEnemyCollision->initHitActor(ACTOR_TYPE_TELESA, 5,
	                                       HIT_CATEGORY_ENEMY, 400.0f, 300.0f,
	                                       400.0f, 300.0f);
	mKillSmallEnemyCollision->offHitFilter(HIT_FILTER_NO_COLLISION);
	mKillSmallEnemyCollision->unk68 = this;

	mMActor->setLightType(3);

	TScreenTexture* screenTex = static_cast<TScreenTexture*>(
	    JDrama::TNameRefGen::search("スクリーンテクスチャ"));
	SMS_ChangeTextureAll(mMActor->getModel()->getModelData(), "H_ma_rak_dummy",
	                     *screenTex->getTexture()->getTexInfo());

	mMarioHP = SMS_GetMarioHP();
}

void TBossTelesa::loadAfter()
{
	u32 rouletteType = ACTOR_TYPE_CASINORULET;
	if (gpMapObjManager->getObjNumWithActorType(rouletteType) != 0) {
		int found = 0;
		for (int i = 0; i < gpMapObjManager->getObjNum(); ++i) {
			TMapObjBase* actor = gpMapObjManager->getObj(i);
			if (actor->isActorType(rouletteType)) {
				mRoulettes[found] = (TRoulette*)actor;
				found++;
			}
		}
	}

	u32 slotType = ACTOR_TYPE_SROT_RULET;
	if (gpMapObjManager->getObjNumWithActorType(slotType) != 0) {
		for (int i = 0; i < gpMapObjManager->getObjNum(); ++i) {
			TMapObjBase* actor = gpMapObjManager->getObj(i);
			if (actor->isActorType(slotType)) {
				mSlot         = (TTelesaSlot*)actor;
				mSlot->unk1A0 = this;
			}
		}
	}

	int fruitIndex = 0;
	for (int i = 0; i < 6; ++i)
		mFruits[fruitIndex++]
		    = TMapObjBaseManager::newAndRegisterObj("FruitCoconut");
	for (int i = 0; i < 6; ++i)
		mFruits[fruitIndex++]
		    = TMapObjBaseManager::newAndRegisterObj("FruitPapaya");
	for (int i = 0; i < 2; ++i)
		mFruits[fruitIndex++]
		    = TMapObjBaseManager::newAndRegisterObj("FruitPine");
	for (int i = 0; i < 6; ++i)
		mFruits[fruitIndex++]
		    = TMapObjBaseManager::newAndRegisterObj("FruitDurian");

	for (int i = 0; i < 20; ++i) {
		mFruits[i]->onMapObjFlag(TMapObjBase::MAP_OBJ_FLAG_UNK4000000);
		mFruits[i]->makeObjDead();
	}

	mRedPeppers[0]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("唐辛子 0"));
	mRedPeppers[1]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("唐辛子 1"));
	mRedPeppers[2]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("唐辛子 2"));
	mRedPeppers[3]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("唐辛子 3"));
	mRedPeppers[4]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("唐辛子 4"));
	mRedPeppers[5]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("唐辛子 5"));
	mRedPeppers[6]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("唐辛子 6"));
	mRedPeppers[7]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("唐辛子 7"));
	mRedPeppers[8]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("唐辛子 8"));
	mRedPeppers[9]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("唐辛子 9"));
	for (int i = 0; i < 10; ++i)
		mRedPeppers[i]->makeObjDead();

	mCoins[0]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("コイン 0"));
	mCoins[1]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("コイン 1"));
	mCoins[2]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("コイン 2"));
	mCoins[3]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("コイン 3"));
	mCoins[4]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("コイン 4"));
	mCoins[5]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("コイン 5"));
	mCoins[6]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("コイン 6"));
	mCoins[7]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("コイン 7"));
	mCoins[8]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("コイン 8"));
	mCoins[9]
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("コイン 9"));
	for (int i = 0; i < 10; ++i)
		mCoins[i]->makeObjDead();

	mSlot->unk1E4[0] = 0.0f;
	mSlot->unk1E4[1] = 0.0f;
	mSlot->unk1E4[2] = 0.0f;

	mTelesaManager = static_cast<TTelesaManager*>(
	    JDrama::TNameRefGen::search("テレサマネージャー"));

	for (int i = 0; i < 5; ++i)
		TMapObjBaseManager::newAndRegisterObj("bottle_large");

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
	SDLModelData* data = new SDLModelData(J3DModelLoaderDataBase::load(
	    res, J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	             | (2 << J3DMLF_TevStageNumShift)));
	mSlotParts         = new TSharedParts(mSlot, 0, data, 3, "<TSharedParts>");

	TLiveActor* gesso0
	    = static_cast<TLiveActor*>(JDrama::TNameRefGen::search("ゲッソー 0"));
	if (gesso0)
		gesso0->onLiveFlag(LIVE_FLAG_DEAD);

	TLiveActor* gesso1
	    = static_cast<TLiveActor*>(JDrama::TNameRefGen::search("ゲッソー 1"));
	if (gesso1)
		gesso1->onLiveFlag(LIVE_FLAG_DEAD);

	JDrama::TNameRef::loadAfter();
}

void TBossTelesa::reset()
{
	TSpineEnemy::reset();
	onHitFilter(HIT_FILTER_NO_COLLISION);
	onLiveFlag(LIVE_FLAG_UNK8);
	onLiveFlag(LIVE_FLAG_UNK10);
	onLiveFlag(LIVE_FLAG_HIDDEN);

	unk18C = 0;

	TBossTelesaSaveLoadParams* params = unk15C;
	setHitParams(params->mSLAttackRadius.get(), params->mSLAttackHeight.get(),
	             params->mSLDamageRadius.get(), params->mSLDamageHeight.get());

	SMSGetMarDirector()->fireStartDemoCamera("btelesa_roll_camera", nullptr, -1,
	                                         0.0f, true, nullptr, 0, nullptr,
	                                         JDrama::TFlagT<u16>(0));
}

void TBossTelesa::moveObject()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	JGeometry::TVec3<f32> cameraPos;
	cameraPos.set(gpCamera->getUnk124());
	JGeometry::TVec3<f32> marioDiff = *gpMarioPos - cameraPos;
	f32 cameraDist                  = marioDiff.length();

	if (cameraDist < mCameraMoveLimit) {
		mCameraOffsetY += mCameraMoveSp * (gpMarioPos->y - gpCamera->unk148.y);
		gpCamera->unk290 = mCameraOffsetY;
	} else if (fabsf(mCameraOffsetY) > 1.0f) {
		mCameraOffsetY *= mCameraMoveSp;
		gpCamera->unk290 = mCameraOffsetY;
	}

	if (SMS_CheckMarioFlag(MARIO_FLAG_GAME_OVER)) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_V_LAUGH1, &mPosition,
		                                0, nullptr, 0, 4);
	}

	if (mSpine->getCurrentNerve() == &TNerveBossTelesaFallDemo::theNerve()) {
		if (mRoulettes[0]->mPosition.y > mRoulettes[1]->mPosition.y + 5.0f) {
			mSlot->mPosition = mRoulettes[0]->mPosition;
			mSlot->mPosition.y += 300.0f;
		}
	}

	if (mSpine->getCurrentNerve() != &TNerveBossTelesaPrepareSlot::theNerve()) {
		mMActor->setFrameRate(0.0f, 3);
		mMActor->getFrameCtrl(3)->setFrame(0.0f);
	}

	if (isInDamage()) {
		u8 maxHp       = getMaxHitPoints();
		u8 targetAlpha = mNormalAlpha + (maxHp - mHitPoints) * 30;
		targetAlpha    = MsClamp<u8>(targetAlpha, 0, 0xFE);

		if ((mMActor->checkCurBckFromIndex(7)
		     && mMActor->getFrameCtrl(0)->getFrame() > 50.0f)
		    || mMActor->checkCurBckFromIndex(6)) {
			if (unk34C.a > targetAlpha)
				--unk34C.a;
		} else if (unk34C.a < 0xFF) {
			++unk34C.a;
		}
	}

	JGeometry::TVec3<f32> cameraDiff = *gpMarioPos - cameraPos;
	unk19C                           = mRoulettes[0]->mPosition;
	unk19C.x += 0.67f * cameraDiff.x;
	unk19C.z += 0.67f * cameraDiff.z;

	switch (getRollingRouletteNum()) {
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

	mPositionDelta.zero();
	mRotationDelta.zero();
	control();

	for (int i = 0; i < mColCount; ++i) {
		if (mCollisions[i]->isActorType(ACTOR_TYPE_MARIO))
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
	}

	bind();
	mPosition += mPositionDelta;
	mRotation += mRotationDelta;
	mPosition.y = mGroundHeight + 300.0f;

	unk168 = MsClamp(unk168 - 0.05f, 0.0f, 1.0f);
	mMActor->setBckMotionBlendRatio(unk168);

	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		mBodyCollision->mPosition           = mPosition;
		mTongueCollision->mPosition         = mPosition;
		mKillSmallEnemyCollision->mPosition = mPosition;
	} else {
		J3DModel* model = mMActor->getModel();
		MtxPtr node1    = model->getAnmMtx(1);
		MtxPtr node7    = model->getAnmMtx(7);

		mBodyCollision->mPosition.set(node1[0][3], node1[1][3] - 200.0f,
		                              node1[2][3]);
		mKillSmallEnemyCollision->mPosition.set(
		    node1[0][3], mPosition.y - 350.0f, node1[2][3]);
		mTongueCollision->mPosition.set(node7[0][3], node7[1][3] - 350.0f,
		                                node7[2][3]);
	}

	mBodyCollision->checkHit();
	mTongueCollision->checkHit();
	mKillSmallEnemyCollision->checkHit();
}

void TBossTelesa::kill()
{
	if (mSpine->getCurrentNerve() != &TNerveBossTelesaDie::theNerve())
		mSpine->pushNerve(&TNerveBossTelesaDie::theNerve());
}

MtxPtr TBossTelesa::getTakingMtx()
{
	mTakingMtx.set(mRoulettes[0]->mMActor->getModel()->getAnmMtx(1));
	mTakingMtx.mMtx[1][3] = mRoulettes[0]->mPosition.y - 120.0f;
	return mTakingMtx.mMtx;
}

// UNUSED
void TBossTelesa::prepareGenerate() { }

void TBossTelesa::calcRootMatrix()
{
	mMActor->getModel()->setBaseScale(mScaling);

	f32 offsetY = mHoseiPosY;
	if (offsetY > 0.0f)
		offsetY = 0.0f;
	else if (offsetY < mBaseHoseiPosY)
		offsetY = mBaseHoseiPosY;

	TPosition3f translateMtx;
	translateMtx.translation(
	    mPosition.x, mPosition.y + unk15C->mSLTransYOffset.get() + offsetY,
	    mPosition.z);

	Mtx rotateMtx;
	MsMtxSetRotRPH(rotateMtx, mRotation.x, mRotation.y, mRotation.z);
	MTXConcat(translateMtx, rotateMtx, translateMtx);
	MTXCopy(translateMtx, mMActor->getModel()->getBaseTRMtx());

	if (mSlot) {
		mSlot->mRotation = mRotation;

		f32 slotOffsetY = -700.0f;
		MtxPtr slotMtx;
		if (mMActor->checkCurBckFromIndex(4) || mMActor->checkCurBckFromIndex(0)
		    || mMActor->checkCurBckFromIndex(2)
		    || mMActor->checkCurBckFromIndex(5)) {
			slotMtx = mMActor->getModel()->getAnmMtx(1);
			if (!mMActor->checkCurBckFromIndex(0))
				slotOffsetY = -2400.0f;
		} else {
			slotMtx = mMActor->getModel()->getAnmMtx(0);
		}

		mSlot->mPosition.x = slotMtx[0][3];
		mSlot->mPosition.y = mHoseiPosY + slotMtx[1][3] + slotOffsetY;
		mSlot->mPosition.z = slotMtx[2][3];
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

	mEffectPos.set(0.0f, 0.0f, 0.0f);
	gpMarioParticleManager->emit(SCENE_BTELESA_JPA_MS_BTLS_CHIKA_A, &mEffectPos,
	                             1, this);
	gpMarioParticleManager->emit(SCENE_BTELESA_JPA_MS_BTLS_CHIKA_B, &mEffectPos,
	                             1, this);

	MtxPtr baseMtx = mMActor->getModel()->getAnmMtx(1);
	mEffectPos.set(baseMtx[0][3], baseMtx[1][3], baseMtx[2][3]);
	gpMarioParticleManager->emitAndBindToPosPtr(SCENE_BTELESA_JPA_MS_BTLS_GLOW,
	                                            &mEffectPos, 1, this);

	if (!mMActor->checkCurBckFromIndex(4) && !mMActor->checkCurBckFromIndex(6)
	    && !mMActor->checkCurBckFromIndex(12)
	    && !mMActor->checkCurBckFromIndex(13)) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_YODARE1,
		    mMActor->getModel()->getAnmMtx(5), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_YODARE2,
		    mMActor->getModel()->getAnmMtx(5), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_YODARE3,
		    mMActor->getModel()->getAnmMtx(10), 1, this);
	}

	if (mMActor->checkCurBckFromIndex(1)
	    && mMActor->getFrameCtrl(0)->getFrame() < 20.0f) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_SPICY_HIT,
		    mMActor->getModel()->getAnmMtx(5), 0, nullptr);
	}

	if (mMActor->checkCurBckFromIndex(12)) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_FIRE, &mPosition, 0,
		                                nullptr, 0, 4);

		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_ASE, mMActor->getModel()->getAnmMtx(1), 1,
		    this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_SPICY_A,
		    mMActor->getModel()->getAnmMtx(9), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_SPICY_B,
		    mMActor->getModel()->getAnmMtx(9), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_SPICY_D,
		    mMActor->getModel()->getAnmMtx(9), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_BTELESA_JPA_MS_BTLS_SPICY_C,
		    mMActor->getModel()->getAnmMtx(9), 3, this);
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

void TBossTelesa::perform(u32 cue, JDrama::TGraphics* gfx)
{
	if ((cue & CUE_ENTRY)
	    && !checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_CLIPPED_OUT)) {
		if (mSpine->getCurrentNerve() == &TNerveBossTelesaDie::theNerve()
		    && mIsSpicy) {
			mMActor->offMakeDL();
			SMS_AddDamageFogEffect(mMActor->getModel()->getModelData(),
			                       mPosition, gfx);
		}
	}

	offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
	TSpineEnemy::perform(cue, gfx);

	mBodyCollision->THitActor::perform(cue, gfx);
	mTongueCollision->THitActor::perform(cue, gfx);
	mKillSmallEnemyCollision->THitActor::perform(cue, gfx);
	if (mSlot)
		mSlot->testPerform(cue, gfx);
	if (mSlotParts)
		mSlotParts->testPerform(cue, gfx);
}

BOOL TBossTelesa::receiveMessage(THitActor*, u32) { return FALSE; }

BOOL TBossTelesa::checkMessage(THitActor* actor, u32 message)
{
	if (message == HIT_MESSAGE_TRAMPLE) {
		if (!actor->isActorType(ACTOR_TYPE_MARIO)) {
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
	mFruitHitEffect = -1;

	if (actor->getHitCategory() != HIT_CATEGORY_MAP_OBJECT)
		return;
	if (mSpine->getCurrentNerve() != &TNerveBossTelesaPrepareSlot::theNerve())
		return;

	switch (actor->getActorType()) {
	case ACTOR_TYPE_FRUIT_COCONUT:
		unk348.r        = 0xE6;
		unk348.g        = 0x64;
		unk348.b        = 0xB4;
		mFruitHitEffect = SCENE_BTELESA_JPA_MS_BTLS_FHIT_PE;
		kill();
		break;
	case ACTOR_TYPE_FRUIT_PAPAYA:
	case ACTOR_TYPE_FRUIT_PINE:
		unk348.r        = 0xE6;
		unk348.g        = 0xB4;
		unk348.b        = 0;
		mFruitHitEffect = SCENE_BTELESA_JPA_MS_BTLS_FHIT_OR;
		kill();
		break;
	case ACTOR_TYPE_FRUIT_DURIAN:
		unk348.r        = 0x96;
		unk348.g        = 0x32;
		unk348.b        = 0xE6;
		mFruitHitEffect = SCENE_BTELESA_JPA_MS_BTLS_FHIT_GR;
		kill();
		break;
	case ACTOR_TYPE_RED_PEPPER:
		break;
	default:
		return;
	}

	if (!mIsSpicy && !actor->isActorType(ACTOR_TYPE_RED_PEPPER)) {
		if (unk35A) {
			unk35A = 0;
			SMSGetMarDirector()->getConsole()->startAppearBalloon(0xE000F,
			                                                      true);
		}
		mFruitHitCount++;
		if (mFruitHitCount > 2)
			SMSGetMarDirector()->getConsole()->startAppearBalloon(0xE0010,
			                                                      true);
	} else {
		mFruitHitCount = 0;
	}

	mEffectPos = actor->mPosition;
	gpMarioParticleManager->emit(SCENE_BTELESA_JPA_MS_BTLS_FHIT, &mEffectPos, 0,
	                             nullptr);
	if (mFruitHitEffect >= 0)
		gpMarioParticleManager->emit(mFruitHitEffect, &mEffectPos, 0, nullptr);

	if (mIsSpicy) {
		gpMarioParticleManager->emit(SCENE_BTELESA_JPA_MS_BTLS_DAMAGE,
		                             &mEffectPos, 0, nullptr);
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

	mIsSpicy    = 1;
	mSpicyTimer = 0;
	setBckAnm(1);

	if (unk35B) {
		unk35B = 0;
		SMSGetMarDirector()->getConsole()->startAppearBalloon(0xE0011, true);
	}

	actor->kill();
}

void TBossTelesa::damageRecover()
{
	for (int i = 0; i < 20; ++i) {
		if (!mFruits[i]->checkLiveFlag(LIVE_FLAG_DEAD)) {
			if (mFruits[i]->mHolder == nullptr)
				SMS_SendMessageToMario(mFruits[i], HIT_MESSAGE_DETACH);

			mFruits[i]->makeObjDead();
			gpMarioParticleManager->emit(PARTICLE_MS_TLS_CHANGE,
			                             &mFruits[i]->mPosition, 0, nullptr);
			mFruits[i]->mPosition.set(0.0f, 0.0f, 0.0f);
		}
	}

	for (int i = 0; i < 10; ++i) {
		if (!mRedPeppers[i]->checkLiveFlag(LIVE_FLAG_DEAD)) {
			if (mRedPeppers[i]->mHolder == nullptr)
				SMS_SendMessageToMario(mRedPeppers[i], HIT_MESSAGE_DETACH);

			mRedPeppers[i]->makeObjDead();
			gpMarioParticleManager->emit(
			    PARTICLE_MS_TLS_CHANGE, &mRedPeppers[i]->mPosition, 0, nullptr);
			mRedPeppers[i]->mPosition.set(0.0f, 0.0f, 0.0f);
		}

		if (!mCoins[i]->checkLiveFlag(LIVE_FLAG_DEAD)) {
			mCoins[i]->makeObjDead();
			gpMarioParticleManager->emit(PARTICLE_MS_TLS_CHANGE,
			                             &mCoins[i]->mPosition, 0, nullptr);
			mCoins[i]->mPosition.set(0.0f, 0.0f, 0.0f);
		}
	}

	if (mIsSpicy) {
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
	TRoulette* roulette0 = mRoulettes[0];
	TRoulette* roulette1 = mRoulettes[1];

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
		if (SMS_SendMessageToMario(this, HIT_MESSAGE_DETACH)) {
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
	if (mSlot->mPosition.y > mRoulettes[0]->mPosition.y - 800.0f) {
		mSlot->mPosition.y -= 5.0f;
		return FALSE;
	}

	mSlot->mPosition.y -= 1.0f;

	if (mSlot->mPosition.y < mRoulettes[0]->mPosition.y - 900.0f) {
		int rolling = 0;
		if (mRoulettes[0]->unk13C != 0.0f)
			rolling = 1;
		if (mRoulettes[1]->unk13C != 0.0f)
			++rolling;
		if (mRoulettes[2]->unk13C != 0.0f)
			++rolling;

		if (rolling != 3)
			rouletteStart();
	}

	if (mSlot->mPosition.y < mRoulettes[0]->mPosition.y - 1100.0f)
		return TRUE;

	mRoulettes[0]->unk150->setHitParams(280.0f, 100.0f, 280.0f, 100.0f);
	mRoulettes[1]->unk150->setHitParams(280.0f, 100.0f, 280.0f, 100.0f);

	return FALSE;
}

// UNUSED
void TBossTelesa::openWaterPlace() { }

void TBossTelesa::flashItem(int result)
{
	int phase = result % 16;

	int index = 0;
	for (int i = 0; i < 5; ++i) {
		TLiveActor* actor = mFruits[index++];
		if (!actor->checkLiveFlag(LIVE_FLAG_DEAD)
		    && actor->mHolder == nullptr) {
			if (phase < 8)
				actor->onLiveFlag(LIVE_FLAG_HIDDEN);
			else
				actor->offLiveFlag(LIVE_FLAG_HIDDEN);
		}

		actor = mFruits[index++];
		if (!actor->checkLiveFlag(LIVE_FLAG_DEAD)
		    && actor->mHolder == nullptr) {
			if (phase < 8)
				actor->onLiveFlag(LIVE_FLAG_HIDDEN);
			else
				actor->offLiveFlag(LIVE_FLAG_HIDDEN);
		}

		actor = mFruits[index++];
		if (!actor->checkLiveFlag(LIVE_FLAG_DEAD)
		    && actor->mHolder == nullptr) {
			if (phase < 8)
				actor->onLiveFlag(LIVE_FLAG_HIDDEN);
			else
				actor->offLiveFlag(LIVE_FLAG_HIDDEN);
		}

		actor = mFruits[index++];
		if (!actor->checkLiveFlag(LIVE_FLAG_DEAD)
		    && actor->mHolder == nullptr) {
			if (phase < 8)
				actor->onLiveFlag(LIVE_FLAG_HIDDEN);
			else
				actor->offLiveFlag(LIVE_FLAG_HIDDEN);
		}
	}

	for (int i = 0; i < 10; ++i) {
		TLiveActor* actor = mRedPeppers[i];
		if (!actor->checkLiveFlag(LIVE_FLAG_DEAD)
		    && actor->mHolder == nullptr) {
			if (phase < 8)
				actor->onLiveFlag(LIVE_FLAG_HIDDEN);
			else
				actor->offLiveFlag(LIVE_FLAG_HIDDEN);
		}

		actor = mCoins[i];
		if (!actor->checkLiveFlag(LIVE_FLAG_DEAD)) {
			if (phase < 8)
				actor->onLiveFlag(LIVE_FLAG_HIDDEN);
			else
				actor->offLiveFlag(LIVE_FLAG_HIDDEN);
		}
	}
}

void TBossTelesa::onAllCollision()
{
	offHitFilter(HIT_FILTER_NO_COLLISION);
	mBodyCollision->offHitFilter(HIT_FILTER_NO_COLLISION);
	mTongueCollision->offHitFilter(HIT_FILTER_NO_COLLISION);
}

void TBossTelesa::offAllCollision()
{
	onHitFilter(HIT_FILTER_NO_COLLISION);
	mBodyCollision->onHitFilter(HIT_FILTER_NO_COLLISION);
	mTongueCollision->onHitFilter(HIT_FILTER_NO_COLLISION);
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

	s32 count       = unk15C->mSLNumGenBubble.get();
	MtxPtr rootMtx  = mMActor->getModel()->getAnmMtx(5);
	f32 angleStep   = 180.0f / count;
	f32 angleOffset = angleStep * count * 0.5f;

	for (int i = 0; i < count; ++i) {
		TBubble* bubble = (TBubble*)gpConductor->makeOneEnemyAppear(
		    mPosition, "バブルマネージャー", 1);
		if (!bubble)
			return;

		Mtx rot;
		JGeometry::TVec3<f32> velocity;
		velocity.set(0.0f, 0.0f, -50.0f);
		MsMtxSetRotRPH(rot, mRotation.x,
		               mRotation.y - angleOffset + angleStep * i, mRotation.z);
		MTXMultVec(rot, &velocity, &velocity);
		MsVECNormalize(&velocity, &velocity);

		velocity.y = 2.0f;
		f32 speed  = unk15C->mSL1stBubbleSp.get();
		velocity.x *= speed;
		velocity.z *= speed;

		bubble->mPosition.set(rootMtx[0][3], rootMtx[1][3] - 50.0f,
		                      rootMtx[2][3]);
		bubble->mVelocity = velocity;
		bubble->mPosition.y += 10.0f;
		bubble->onLiveFlag(LIVE_FLAG_AIRBORNE);

		TMsRange<f32> itemRange(0.0f, 1.0f);
		f32 itemRoll = itemRange.rand();
		if (itemRoll < mItemGenRate) {
			bubble->appendItem();
		} else {
			f32 enemyRoll = itemRange.rand();
			if (enemyRoll < mEnemyGenRate)
				bubble->appendEnemy();
		}
	}
}

void TBossTelesa::setBckAnm(int index)
{
	unk164 = getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
	unk160 = index;
	unk168 = 1.0f;

	getMActor()->setBckOldMotionBlendAnmPtr(getMActor()->getCurBckAnmPtr());

	getMActor()->setBckFromIndex(index);
	getMActor()->setBckMotionBlendRatio(unk168);

	setAnmSound(getBas(index));
}

bool TBossTelesa::isInDamage()
{
	if (mSpine->getCurrentNerve() == &TNerveBossTelesaDie::theNerve())
		return true;
	return false;
}

// TODO: frame is 0x10 bytes short; a getter for TRoulette::unk13C closes it
// here but makes slotFall 8 bytes too big.
void TBossTelesa::rouletteStart()
{
	getRollingRouletteNum();
	TMsRange<f32> speedRange(0.05f, 0.1f);
	TMsRange<f32> directionRange(-1.0f, 1.0f);
	f32 dir = directionRange.rand();

	u8 maxHitPoints = getMaxHitPoints();
	f32 hpSpeed = (maxHitPoints - mHitPoints) * TBossTelesa::mRouletteUpRate;

	for (int i = 0; i < 3; ++i) {
		f32 direction = 1.0f;
		if (dir > 0.0f)
			direction = -1.0f;
		if (i == 0 || i == 2)
			direction = -direction;

		f32 speed             = speedRange.rand();
		mRoulettes[i]->unk144 = direction * (hpSpeed + speed);

		speed            = speedRange.rand();
		mSlot->unk1E4[i] = direction * (hpSpeed + speed);
	}

	rollRouletteCircle();

	SMSRumbleMgr->start(0x14, 0xf, (f32*)nullptr);
	gpCameraShake->startShake(CAM_SHAKE_MODE_UNK23, 1.0f);
}

void TBossTelesa::slotStart()
{
	unk18C = 1;
	mSlot->moveStart();
	mTelesaManager->telesaForceKill();
}

void TBossTelesa::slotStop()
{
	mRoulettes[0]->unk144 = 0.0f;
	mSlot->unk1E4[0]      = 0.0f;
	mRoulettes[1]->unk144 = 0.0f;
	mSlot->unk1E4[1]      = 0.0f;
	mRoulettes[2]->unk144 = 0.0f;
	mSlot->unk1E4[2]      = 0.0f;
	rollRouletteCircle();
}

// UNUSED
int TBossTelesa::checkSlotResult() { return 0; }

void TBossTelesa::generateSlotItem()
{
	static const char* manNameTable[] = {
		"バブルマネージャー",   "ハムクリマネージャー",
		"ヤキグリマネージャー", "ボム兵マネージャー",
		"ポイハナマネージャー", "電気ノコノコマネージャー",
		"ポポマネージャー",     "ゲッソーマネージャー",
		"とびプクマネージャー",
	};

	unk368      = 0;
	mSpitObjNum = 0;

	unk1A8 = mSlot->getSlotResult();

	s32 slotItemNum = unk15C->mSLSlotItemNum.get();
	MtxPtr rootMtx  = mMActor->getModel()->getAnmMtx(5);
	f32 angleStep   = 120.0f / slotItemNum;
	f32 angleOffset = angleStep * slotItemNum * 0.5f;

	if (unk1A8 == 2) {
		s32 numFruit = unk15C->mSLSlotFruitNum.get();
		if (numFruit > 20)
			numFruit = 20;

		TMsRange<s32> angleIndexRange(0, numFruit);
		s32 angleIndex = angleIndexRange.rand();

		for (int i = 0; i < numFruit; ++i) {
			if (mFruits[i]->mHolder)
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

			MTXMultVec(rot, &dir, &dir);
			MsVECNormalize(&dir, &normalizedDir);

			TMsRange<f32> fruitSpeedRange(6.0f, 10.0f);

			if (i == 0 || i == 4) {
				mRedPeppers[i]->makeObjAppeared();
				mRedPeppers[i]->offLiveFlag(LIVE_FLAG_HIDDEN);
				mRedPeppers[i]->mVelocity.set(
				    normalizedDir.x * fruitSpeedRange.rand(), -2.0f,
				    normalizedDir.z * fruitSpeedRange.rand());
				mRedPeppers[i]->offLiveFlag(LIVE_FLAG_UNK10);
				if (i == 0) {
					mRedPeppers[i]->mVelocity.set(
					    normalizedDir.x * fruitSpeedRange.rand() * 2.0f, -2.0f,
					    normalizedDir.z * fruitSpeedRange.rand() * 2.0f);
					mRedPeppers[i]->offLiveFlag(LIVE_FLAG_UNK10);
				}
				mRedPeppers[i]->mRotation.set(0.0f, 90.0f, 0.0f);
				mSpitObjs[mSpitObjNum] = mRedPeppers[i];
			} else {
				mFruits[i]->makeObjAppeared();
				mFruits[i]->offLiveFlag(LIVE_FLAG_HIDDEN);

				mFruits[i]->mVelocity.set(
				    normalizedDir.x * fruitSpeedRange.rand(), -2.0f,
				    normalizedDir.z * fruitSpeedRange.rand());
				mFruits[i]->offLiveFlag(LIVE_FLAG_UNK10);
				mSpitObjs[mSpitObjNum] = mFruits[i];
			}

			mSpitObjs[i]->onHitFilter(HIT_FILTER_NO_COLLISION);
			mSpitObjs[i]->mScaling.set(1.5f, 1.5f, 1.5f);
			mSpitObjs[mSpitObjNum]->mPosition.set(rootMtx[0][3] + dir.x,
			                                      rootMtx[1][3] - 50.0f,
			                                      rootMtx[2][3] + dir.z);
			mSpitObjNum++;
		}
	} else if (unk1A8 == 0) {
		s32 numItems = unk15C->mSLSlotFruitNum.get();
		if (numItems > 10)
			numItems = 10;

		f32 itemAngleStep   = 120.0f / numItems;
		f32 itemAngleOffset = itemAngleStep * numItems * 0.5f;

		if (unk370)
			unk370--;
		else
			unk370 = 0;

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
			MTXMultVec(rot, &dir, &dir);
			MsVECNormalize(&dir, &dir);
			dir.y = 10.0f;

			JGeometry::TVec3<f32> velocity;
			TMsRange<f32> itemSpeedRange(0.8f, 3.5f);
			f32 speed  = unk15C->mSL1stBubbleSp.get();
			velocity.x = dir.x * speed * itemSpeedRange.rand();
			velocity.y = dir.y;
			velocity.z = dir.z * speed * itemSpeedRange.rand();

			TItem* item
			    = (TItem*)gpItemManager->makeObjAppeared(ACTOR_TYPE_COIN);
			item->mPosition.set(rootMtx[0][3], rootMtx[1][3] - 250.0f,
			                    rootMtx[2][3]);
			item->mVelocity.set(velocity);
			item->offLiveFlag(LIVE_FLAG_UNK10);
			item->mScaling.set(0.0f, 0.0f, 0.0f);
			item->killByTimer(0x3C0);

			mSpitObjs[mSpitObjNum] = mCoins[i];
			mSpitObjs[mSpitObjNum]->offLiveFlag(LIVE_FLAG_HIDDEN);
			mSpitObjNum++;
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
				if (i % 2 == 0)
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
				mSpitObjs[mSpitObjNum] = enemy;
				mSpitObjNum++;
			}

			Mtx rot;
			Vec dir;
			dir.x = 0.0f;
			dir.y = 0.0f;
			dir.z = 200.0f;
			MsMtxSetRotRPH(rot, mRotation.x,
			               mRotation.y - angleOffset + angleStep * i,
			               mRotation.z);
			MTXMultVec(rot, &dir, &dir);
			MsVECNormalize(&dir, &dir);
			dir.y = 2.0f;

			JGeometry::TVec3<f32> velocity;
			TMsRange<f32> enemySpeedRange(0.5f, 1.0f);
			f32 speed  = unk15C->mSL1stBubbleSp.get();
			velocity.x = dir.x * speed * enemySpeedRange.rand();
			velocity.y = dir.y * (2.0f + enemySpeedRange.rand());
			velocity.z = dir.z * speed * enemySpeedRange.rand();

			enemy->mPosition.set(rootMtx[0][3], rootMtx[1][3] - 250.0f,
			                     rootMtx[2][3]);
			enemy->mVelocity = velocity;
			enemy->mPosition.y += 10.0f;
			enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);
			MTXCopy(rootMtx, enemy->mMActor->getModel()->getBaseTRMtx());
			enemy->mMActor->calc();
			enemy->initAttacker(this);
		}
	}
}

void TBossTelesa::fruitCollisionOn()
{
	for (int i = 0; i < mSpitObjNum; ++i) {
		TLiveActor* actor = mSpitObjs[i];
		if (!actor->checkLiveFlag(LIVE_FLAG_DEAD)
		    && !actor->isActorType(ACTOR_TYPE_COIN)
		    && !actor->isActorType(ACTOR_TYPE_BOTTLE_LARGE))
			actor->offHitFilter(HIT_FILTER_NO_COLLISION);
	}
}

bool TBossTelesa::checkSlot()
{
	if (!mSlot->isRollDrum() && unk18C) {
		unk18C = 0;
		mSpine->pushAfterCurrent(&TNerveBossTelesaSpitSlotItem::theNerve());
		return true;
	}

	return false;
}

bool TBossTelesa::checkAllItemDead()
{
	if (unk1A8 == -1)
		return true;
	for (int i = 0; i < mSpitObjNum; ++i)
		if (!mSpitObjs[i]->checkLiveFlag(LIVE_FLAG_DEAD))
			return false;
	return true;
}

void TBossTelesa::forceAllItemKill()
{
	for (int i = 0; i < mSpitObjNum; ++i) {
		if (mSpitObjs[i]->mHolder) {
			SMS_SendMessageToMario(mSpitObjs[i], HIT_MESSAGE_DETACH);
			mSpitObjs[i]->mHolder = nullptr;
		}

		mSpitObjs[i]->mPosition.set(0.0f, 0.0f, 0.0f);
		mSpitObjs[i]->onHitFilter(HIT_FILTER_NO_COLLISION);

		if (!mSpitObjs[i]->checkLiveFlag(LIVE_FLAG_DEAD)) {
			mSpitObjs[i]->kill();
			gpMarioParticleManager->emit(PARTICLE_MS_TLS_CHANGE,
			                             &mSpitObjs[i]->mPosition, 0, nullptr);
		}
	}
}

void TBossTelesa::rollRouletteCircle()
{
	for (int i = 0; i < 3; ++i)
		mRoulettes[i]->setRollSp(mSlot->unk1E4[i]);
}

// UNUSED
bool TBossTelesa::isForceRestart() { return false; }

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

	if (mIsSpicy) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_ESCAPE, &mPosition, 0,
		                                nullptr, 0, 4);
	} else {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_DISAPPEAR, &mPosition,
		                                0, nullptr, 0, 4);
	}

	mSpine->reset();
	mSpine->setNext(&TNerveBossTelesaHide::theNerve());
}

void TBossTelesa::fanfale()
{
	if (mSlot->getSlotResult() == 2 || mSlot->getSlotResult() == 0) {
		mEffectPos.set(0.0f, 0.0f, 0.0f);
		gpMarioParticleManager->emit(SCENE_BTELESA_JPA_MS_BTLS_FUBUKI,
		                             &mEffectPos, 0, nullptr);

		if (mSlot->getSlotResult() == 2) {
			SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_FANFALE_1,
			                                &mPosition, 0, nullptr, 0, 4);
		} else {
			SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_FANFALE_2,
			                                &mPosition, 0, nullptr, 0, 4);
		}
	} else {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_FANFALE_3, &mPosition,
		                                0, nullptr, 0, 4);
	}
}

DEFINE_NERVE(TNerveBossTelesaDie, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();
	if (spine->getTime() == 0) {
		boss->unk388 = 0;
		if (boss->mIsSpicy)
			boss->decHitPoints();

		boss->offAllCollision();

		if (boss->getHitPoints() != 0) {
			if (boss->mIsSpicy) {
				boss->getMActor()->setBrkFromIndex(1);
				boss->setBckAnm(2);
				gpCameraShake->startShake(CAM_SHAKE_MODE_UNK1F, 1.0f);
			} else {
				boss->setBckAnm(5);
				boss->getMActor()->setBrkFromIndex(0);
				gpCameraShake->startShake(CAM_SHAKE_MODE_UNK20, 1.0f);
			}
		} else {
			MSBgm::stopBGM(MSD_BGM_MAP_SELECT, 10);
			gpCameraShake->startShake(CAM_SHAKE_MODE_UNK21, 1.0f);
			boss->setBckAnm(3);
			boss->getMActor()->setBrkFromIndex(1);
			boss->mSlot->mScaling.set(0.0f, 0.0f, 0.0f);

			SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_DOWN,
			                                &boss->mPosition, 0, nullptr, 0, 4);
		}
	}

	if (boss->getHitPoints() == 0) {
		if (boss->checkCurAnmEnd(0)) {
			if (boss->unk388 == 0) {
				MtxPtr mtx = boss->getMActor()->getModel()->getAnmMtx(1);
				boss->mEffectPos.set(mtx[0][3], mtx[1][3], mtx[2][3]);

				gpMarioParticleManager->emit(SCENE_BTELESA_JPA_MS_BTLS_DOWN,
				                             &boss->mEffectPos, 0, nullptr);
				if (boss->mFruitHitEffect
				    == SCENE_BTELESA_JPA_MS_BTLS_FHIT_PE) {
					gpMarioParticleManager->emit(
					    SCENE_BTELESA_JPA_MS_BTLS_DOWN_PE, &boss->mEffectPos, 0,
					    nullptr);
				} else if (boss->mFruitHitEffect
				           == SCENE_BTELESA_JPA_MS_BTLS_FHIT_GR) {
					gpMarioParticleManager->emit(
					    SCENE_BTELESA_JPA_MS_BTLS_DOWN_GR, &boss->mEffectPos, 0,
					    nullptr);
				} else {
					gpMarioParticleManager->emit(
					    SCENE_BTELESA_JPA_MS_BTLS_DOWN_OR, &boss->mEffectPos, 0,
					    nullptr);
				}

				boss->forceAllItemKill();
				boss->slotStop();
			}

			if (boss->unk388 > 240) {
				boss->unk388 = 0;
				gpItemManager->makeShineAppearWithDemo(
				    "シャイン（ボス用）", "ボスシャインカメラ",
				    boss->mPosition.x, boss->mPosition.y, boss->mPosition.z);
				boss->onLiveFlag(LIVE_FLAG_DEAD);
				boss->onLiveFlag(LIVE_FLAG_UNK8);
				boss->offLiveFlag(TBossTelesa::LIVE_FLAG_UNK10000);
				boss->mHolder = nullptr;
				boss->offAllCollision();
				boss->stopAnmSound();
				spine->reset();
				return TRUE;
			}

			boss->unk388++;
		}
		return FALSE;
	}

	if (boss->checkCurAnmEnd(0)) {
		if (boss->getMActor()->checkCurBckFromIndex(5)) {
			boss->getMActor()->setBrkFromIndex(2);
			boss->setBckAnm(7);
		} else if (boss->getMActor()->checkCurBckFromIndex(7)) {
			boss->setBckAnm(6);
		} else {
			SMS_ResetDamageFogEffect(
			    boss->getMActor()->getModel()->getModelData());

			if (boss->getMActor()->checkCurBckFromIndex(6)) {
				boss->onAllCollision();

				boss->setBckAnm(15);
				boss->getMActor()->setBtpFromIndex(2);
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
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();
	if (spine->getTime() == 0 || !boss->getMActor()->checkCurBckFromIndex(14)) {
		boss->setBckAnm(14);
	} else {
		if (boss->getMActor()->getFrameCtrl(0)->checkPass(40.0f))
			boss->genAttacker();
	}

	if (boss->checkCurAnmEnd(0))
		return true;
	return false;
}

DEFINE_NERVE(TNerveBossTelesaHide, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();
	if (!boss->getMActor()->checkCurBckFromIndex(4)) {
		boss->setBckAnm(4);
		boss->getMActor()->setBtpFromIndex(2);
	}

	if (boss->checkCurAnmEnd(0)) {
		boss->offAllCollision();
		SMSRumbleMgr->start(0x14, 0xF, (f32*)nullptr);
		boss->rouletteStart();
		spine->pushAfterCurrent(&TNerveBossTelesaHideWait::theNerve());
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaHideWait, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();
	if (spine->getTime() == 0) {
		boss->onLiveFlag(LIVE_FLAG_HIDDEN);
		boss->mIsSpicy = 0;

		u8 maxHp = boss->getMaxHitPoints();
		u8 alpha
		    = TBossTelesa::mNormalAlpha + (maxHp - boss->getHitPoints()) * 30;
		alpha          = MsClamp<u8>(alpha, 0, 0xFE);
		boss->unk34C.a = alpha;

		boss->mSlot->mScaling.set(0.0f, 0.0f, 0.0f);
		boss->getMActor()->setBrkFromIndex(2);
		s16 endFrame = boss->getMActor()->getFrameCtrl(5)->getEnd();
		boss->getMActor()->getFrameCtrl(5)->setFrame((f32)endFrame);
	} else {
		JGeometry::TVec3<f32> pos = boss->mPosition - *gpMarioPos;

		if (spine->getTime() > 400
		    && boss->mKillSmallEnemyCollision->unk6C == 0) {
			spine->pushAfterCurrent(&TNerveBossTelesaAppear::theNerve());
			boss->offLiveFlag(LIVE_FLAG_HIDDEN);
			return TRUE;
		}
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaAppear, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();
	if (spine->getTime() == 0) {
		if (!boss->getMActor()->checkCurBckFromIndex(0)) {
			boss->setBckAnm(0);

			if (!boss->unk384) {
				boss->unk384 = 1;
				MSBgm::startBGM(MSD_BGM_MAP_SELECT);
			}

			boss->mSlot->mScaling.set(1.0f, 1.0f, 1.0f);
			boss->mSlot->randomReset();
			boss->onAllCollision();
		}
	} else if (boss->checkCurAnmEnd(0)
	           && !boss->getMActor()->checkCurBckFromIndex(15)) {
		boss->setBckAnm(15);
		boss->getMActor()->setBtpFromIndex(2);
	}

	if (boss->getMActor()->checkCurBckFromIndex(0)
	    && boss->getMActor()->getFrameCtrl(0)->checkPass(78.0f)) {
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK22, 1.0f);
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_SLT_LAND,
		                                &boss->mPosition, 0, nullptr, 0, 4);
	}

	if (spine->getTime() > 800) {
		u8 maxHp     = boss->getMaxHitPoints();
		int interval = TBossTelesa::mTelesaGenerateInterval
		               + (maxHp - boss->getHitPoints()) * 100;
		if (spine->getTime() % interval == 1) {
			boss->setBckAnm(14);
		}
	}

	boss->mHoseiPosY *= 0.96f;
	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaSlotStart, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();
	if (spine->getTime() == 0) {
		boss->setBckAnm(11);
	}

	if (boss->getMActor()->checkCurBckFromIndex(11)) {
		if (boss->getMActor()->getFrameCtrl(0)->checkPass(53.0f)) {
			boss->slotStart();
		}

		if (boss->checkCurAnmEnd(0)) {
			boss->setBckAnm(15);
			boss->getMActor()->setBtpFromIndex(2);
			boss->mSlot->forceStopSlot(1);
		}
	}

	if (boss->checkSlot())
		return TRUE;

	boss->mHoseiPosY *= 0.99f;
	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaSpitSlotItem, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();
	if (!boss->getMActor()->checkCurBckFromIndex(14)
	    && boss->mHoseiPosY < TBossTelesa::mBaseHoseiPosY - 200.0f) {
		boss->setBckAnm(14);
	} else if (boss->checkCurAnmEnd(0) && spine->getTime() > 600) {
		spine->pushAfterCurrent(&TNerveBossTelesaPrepareSlot::theNerve());
		boss->unk368 = 0;
		boss->fruitCollisionOn();
		return TRUE;
	} else if (spine->getTime() > 200) {
		boss->mHoseiPosY -= 100.0f;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaPrepareSlot, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();
	if (spine->getTime() == 0) {
		boss->setBckAnm(15);
		boss->getMActor()->setBtpFromIndex(2);
	}

	if (boss->mIsSpicy) {
		boss->mSpicyTimer++;
		if (boss->checkCurAnmEnd(0)) {
			if (boss->getMActor()->checkCurBckFromIndex(1)) {
				boss->setBckAnm(12);
				boss->getMActor()->setBtpFromIndex(1);
			} else if (boss->getMActor()->checkCurBckFromIndex(12)) {
				if (boss->mSpicyTimer > boss->unk15C->mSLSpicyTime.get()) {
					boss->setBckAnm(13);
				}
			} else {
				boss->mSpicyTimer = 0;
				boss->mIsSpicy    = 0;

				u8 maxHp = boss->getMaxHitPoints();
				u8 alpha = TBossTelesa::mNormalAlpha
				           + (maxHp - boss->getHitPoints()) * 30;
				if (alpha > 0xFE)
					alpha = 0xFE;

				boss->unk34C.a = alpha;

				boss->setBckAnm(15);
				boss->getMActor()->setBtpFromIndex(2);
			}
		}
	}

	boss->unk368++;

	TBossTelesaSaveLoadParams* params = boss->unk15C;
	int waitTime                      = params->mSLStopSlotTime0.get();
	if (boss->getHitPoints() == 2)
		waitTime = params->mSLStopSlotTime1.get();
	if (boss->getHitPoints() == 1)
		waitTime = params->mSLStopSlotTime2.get();

	if (boss->mSlot->getSlotResult() == 0)
		waitTime = waitTime * 0.5f;

	if (boss->unk368 > waitTime - 120)
		boss->flashItem(waitTime - boss->unk368);

	if (boss->getMActor()->checkCurBckFromIndex(15)) {
		if (boss->checkAllItemDead() || boss->unk368 > waitTime) {
			boss->unk368 = 0;
			boss->forceAllItemKill();

			if (boss->mIsSpicy) {
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
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();
	if (boss->getMActor()->checkCurBckFromIndex(16)) {
		if (boss->checkCurAnmEnd(0)) {
			boss->mIsSpicy = 0;

			u8 maxHp = boss->getMaxHitPoints();
			u8 alpha = TBossTelesa::mNormalAlpha
			           + (maxHp - boss->getHitPoints()) * 30;
			alpha = MsClamp<u8>(alpha, 0, 0xFE);

			boss->unk34C.a = alpha;
			return TRUE;
		}
	} else {
		boss->setBckAnm(16);

		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_THANKYOU,
		                                &boss->mPosition, 0, nullptr, 0, 4);
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaFallDemo, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();
	if (spine->getTime() == 0) {
		boss->onLiveFlag(LIVE_FLAG_HIDDEN);
		if (SMS_SendMessageToMario(boss, HIT_MESSAGE_TAKE))
			boss->mHeldObject = (TTakeActor*)SMS_GetMarioHitActor();
		boss->getMActor()->setFrameRate(0.0f, 0);
		boss->mSlot->mScaling.set(0.0f, 0.0f, 0.0f);
	}

	if (boss->rouletteFall()) {
		JGeometry::TVec3<f32> diff = boss->mPosition - *gpMarioPos;

		if (boss->slotFall()) {
			boss->onAllCollision();
			spine->reset();
			spine->setNext(&TNerveBossTelesaHideWait::theNerve());
			spine->pushAfterCurrent(&TNerveBossTelesaHideWait::theNerve());
			return TRUE;
		}
	}

	return FALSE;
}
