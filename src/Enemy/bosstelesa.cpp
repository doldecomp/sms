#include <Enemy/BossTelesa.hpp>

#include <Enemy/Conductor.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Strategic/Spine.hpp>
#include <System/Params.hpp>
#include <System/EmitterViewObj.hpp>
#include <Player/MarioAccess.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>

/*
 * Boss Telesa is the casino boss in Sirena Beach.  The retail TU is a good
 * deal denser than the ordinary Telesa TU: one actor owns three roulette
 * drums, two hit proxies and a small bubble manager.  Keep the data and
 * state-machine names here even while the long animation routines are being
 * brought over from the matching assembly.
 */

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

static const char* bubble_bastable[] = {
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/btelesa/bas/btelesa_lick.bas",
	"/scene/btelesa/bas/btelesa_roll.bas",
	nullptr,
	nullptr,
};

static const TModelDataLoadEntry btelesa_model_data[] = {
	{ "btelesa.bmd", 0x15300000, 0 },
	{ nullptr, 0, 0 },
};

static const TModelDataLoadEntry bubble_model_data[] = {
	{ "btelesa_osenbubbles_ind.bmd", 0x11020000, 0 },
	{ nullptr, 0, 0 },
};

MActorAnmBck* MActor::getCurBckAnmPtr() { return unkC; }

f32 TBossTelesa::mEnemyGenRate          = 0.1f;
f32 TBossTelesa::mItemGenRate           = 0.1f;
u8 TBossTelesa::mNormalAlpha            = 0xFF;
f32 TBossTelesa::mBaseHoseiPosY         = 350.0f;
f32 TBossTelesa::mRouletteUpRate        = 0.1f;
s32 TBossTelesa::mTelesaGenerateInterval = 600;
f32 TBossTelesa::mCameraMoveLimit       = 300.0f;
f32 TBossTelesa::mCameraMoveSp          = 2.0f;

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

TBubbleSaveLoadParams::TBubbleSaveLoadParams(const char* path)
	: TSpineEnemyParams(path)
	, PARAM_INIT(mSLLiveTime, 1200)
	, PARAM_INIT(mSLNumDivision, 3)
	, PARAM_INIT(mSLMaxScale, 1.0f)
	, PARAM_INIT(mSLAddPosBase, 20.0f)
	, PARAM_INIT(mSLRateExpand, 0.01f)
	, PARAM_INIT(mSLDeadHeight, 300.0f)
{
	TParams::load(mPrmPath);
}

/* The slot angle table is deliberately kept as the retail eight-way split. */
int TTelesaSlot::getResultFromAng(f32 angle)
{
	if (angle < 44.0f)
		return 0;
	if (angle < 89.0f)
		return 1;
	if (angle < 134.0f)
		return 3;
	if (angle < 179.0f)
		return 2;
	if (angle < 224.0f)
		return 0;
	if (angle < 269.0f)
		return 1;
	if (angle < 314.0f)
		return 3;
	return 2;
}

int TTelesaSlot::getForcastResult(int drum)
{
	/* The actual animation advances in 45-degree steps.  Forecasting only
	 * needs the final quantized result, so use the same normalization here. */
	f32 angle = unk13C[drum] + unk138[drum];
	while (angle >= 360.0f)
		angle -= 360.0f;
	while (angle <= 0.0f)
		angle += 360.0f;
	return getResultFromAng(45.0f * (s32)(angle / 45.0f));
}

int TTelesaSlot::getSlotResult()
{
	int result = getResultFromAng(unk13C[0]);
	for (int i = 1; i < 3; ++i) {
		if (result != getResultFromAng(unk13C[i]))
			return -1;
	}
	return result;
}

u8 TTelesaSlot::isRollDrum()
{
	if (mRoll0 || mRoll1 || mRoll2)
		return 1;
	mRolling = 0;
	return 0;
}

void TTelesaSlot::moveStart()
{
	mForceStop = 1;
	mRolling   = 1;
	mRoll0     = 1;
	mRoll1     = 1;
	mRoll2     = 1;
	mStop0 = mStop1 = mStop2 = 0;
	unk138[0] = -unk158;
	unk138[1] = -0.8f * unk158;
	unk138[2] = unk158;
}

void TTelesaSlot::randomReset()
{
	for (int i = 0; i < 3; ++i) {
		unk13C[i] = unk168 * (s32)MsRandF(0.0f, 8.0f);
		((&mStop0)[i]) = 0;
	}
}

u32 TTelesaSlot::touchWater(THitActor*) { return 0; }

void TTelesaSlot::forceStopSlot(int drum)
{
	if (!mForceStop)
		return;
	mForceResult = getForcastResult(drum);
	((&mRoll0)[drum]) = 0;
	mForceStop = 0;
}

void TTelesaSlot::calcRootMatrix() { TSlotDrum::calcRootMatrix(); }

void TTelesaSlot::moveObject() { TSlotDrum::moveObject(); }

void TTelesaSlot::initMapObj()
{
	TSlotDrum::initMapObj();
	mLiveFlag |= 0x10;
	unk14C = 160.0f;
	unk150 = mPosition.y;
	unk154 = 2.0f;
	unk158 = 2.0f;
	unk15C = 0.01f;
	unk160 = 0.5f;
	unk168 = 45;
	randomReset();
}

void TTelesaSlot::entryObjCollision() { }

void TTelesaSlot::calcObjCollision() { }

int TTelesaSlot::getDrumResult(int drum)
{
	return getResultFromAng(unk13C[drum]);
}

TBossTelesa::TBossTelesa(const char* name)
	: TSpineEnemy(name)
	, mBattleState(1)
	, mBattleTimer(0)
	, mBattlePhase(0)
	, mMario(nullptr)
	, mCurrentItem(-1)
	, mCurrentFruit(-1)
	, mFallSpeed(0.0f)
	, mBodyCollision(nullptr)
	, mTongueCollision(nullptr)
	, mKillSmallEnemy(nullptr)
	, mItemList(nullptr)
	, mBubbleList(nullptr)
	, mSaveParams(nullptr)
	, mSlot(nullptr)
	, mTakingActor(nullptr)
	, mIsSpicy(false)
	, mDamageBlend(0.0f)
	, mDamageBlendSpeed(0.0f)
	, mDamageBlendTarget(0.0f)
	, mDamageTimer(0)
	, mStateTimer(0)
	, mItem0(0)
	, mItem1(0)
	, mItem2(0)
	, mItem3(0)
	, mFruit0(0)
	, mFruit1(0)
	, mFruit2(0)
	, mAlpha(mNormalAlpha)
	, mDamage(0)
	, mGenerateTimer(0)
	, mMarioHP(1)
	, mForceRestart(1)
	, mInDamage(1)
	, mRouletteTimer(0)
	, mRoulettePosY(0.0f)
	, mRouletteSp(0.0f)
	, mSlotResult(0)
	, mSlotItemCount(0)
	, mSlotStop(0)
	, mEffectPos()
	, mDead(0)
{
}

const char** TBossTelesa::getBasNameTable() const { return btelesa_bastable; }

void TBossTelesa::forceHide()
{
	mDead = 1;
	offLiveFlag(LIVE_FLAG_UNK200);
	onLiveFlag(LIVE_FLAG_HIDDEN);
}

void TBossTelesa::fanfale() { }

bool TBossTelesa::isForceRestart() { return mForceRestart != 0; }

void TBossTelesa::rollRouletteCircle()
{
	if (mSlot)
		mSlot->moveStart();
}

void TBossTelesa::forceAllItemKill()
{
	mItem0 = mItem1 = mItem2 = mItem3 = 0;
	mFruit0 = mFruit1 = mFruit2 = 0;
}

bool TBossTelesa::checkAllItemDead()
{
	return mItem0 == 0 && mItem1 == 0 && mItem2 == 0 && mItem3 == 0;
}

bool TBossTelesa::checkSlot() { return mSlot != nullptr; }

void TBossTelesa::fruitCollisionOn() { mSlotStop = 0; }

void TBossTelesa::generateSlotItem()
{
	++mSlotItemCount;
	mGenerateTimer = 0;
}

int TBossTelesa::checkSlotResult() { return mSlot ? mSlotResult : -1; }

void TBossTelesa::slotStop()
{
	if (mSlot)
		mSlot->forceStopSlot(0);
}

void TBossTelesa::slotStart() { rouletteStart(); }

bool TBossTelesa::isInDamage() { return mInDamage != 0; }

void TBossTelesa::setBckAnm(int index)
{
	if (getMActor())
		getMActor()->setBckFromIndex(index);
}

void TBossTelesa::offAllCollision()
{
	if (mBodyCollision)
		mBodyCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
	if (mTongueCollision)
		mTongueCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBossTelesa::onAllCollision()
{
	if (mBodyCollision)
		mBodyCollision->onHitFlag(HIT_FLAG_NO_COLLISION);
	if (mTongueCollision)
		mTongueCollision->onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBossTelesa::openWaterPlace() { }

void TBossTelesa::tongueHitWater() { }

BOOL TBossTelesa::checkMessage(THitActor* sender, u32 message)
{
	return receiveMessage(sender, message);
}

void TBossTelesa::prepareGenerate() { mGenerateTimer = 0; }

void TBossTelesa::rouletteStart()
{
	mRouletteTimer = 0;
	mSlotStop      = 0;
	if (mSlot)
		mSlot->moveStart();
}

void TBossTelesa::genAttacker() { mGenerateTimer = mSaveParams ? mSaveParams->mSLGenAttackerTime.get() : 500; }

void TBossTelesa::flashItem(int item)
{
	mSlotResult = item;
	mDamageTimer = 0;
}

u8 TBossTelesa::slotFall()
{
	if (!mSlot)
		return 1;
	return mSlot->isRollDrum() == 0;
}

u8 TBossTelesa::rouletteFall()
{
	if (mRouletteTimer > 0)
		--mRouletteTimer;
	return mRouletteTimer == 0;
}

void TBossTelesa::damageRecover()
{
	if (mDamageTimer > 0)
		--mDamageTimer;
	if (mDamageTimer == 0)
		mDamage = 0;
}

void TBossTelesa::setSpicy(TLiveActor* actor)
{
	if (actor)
		actor->onLiveFlag(LIVE_FLAG_UNK10);
	mIsSpicy = 1;
}

BOOL TBossTelesa::checkHitObject(THitActor* actor)
{
	return actor != nullptr && actor->checkActorType(ACTOR_TYPE_PLAYER);
}

BOOL TBossTelesa::receiveMessage(THitActor*, u32) { return TRUE; }

void TBossTelesa::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSpineEnemy::perform(cue, graphics);
	if (cue & CUE_MOVE)
		damageRecover();
}

void TBossTelesa::calcRootMatrix() { TSpineEnemy::calcRootMatrix(); }

MtxPtr TBossTelesa::getTakingMtx() { return getModel() ? getModel()->getBaseTRMtx() : nullptr; }

void TBossTelesa::kill()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD)) {
		mDead = 1;
		TSpineEnemy::kill();
	}
}

void TBossTelesa::moveObject()
{
	TSpineEnemy::moveObject();
	if (mGenerateTimer > 0)
		--mGenerateTimer;
}

void TBossTelesa::reset()
{
	TSpineEnemy::reset();
	mDead = 0;
	mDamage = 0;
	mGenerateTimer = 0;
	forceAllItemKill();
}

void TBossTelesa::loadAfter()
{
	mMarioHP = 1;
	if (!mSaveParams)
		mSaveParams = static_cast<TBossTelesaSaveLoadParams*>(getSaveParam());
}

void TBossTelesa::init(TLiveManager* manager)
{
	TSpineEnemy::init(manager);
	mSaveParams = static_cast<TBossTelesaSaveLoadParams*>(getSaveParam());
	mBodyCollision = new TBossTelesaBody(this);
	mTongueCollision = new TBossTelesaTongue(this);
	mKillSmallEnemy = new TBossTelesaKillSmallEnemy(this);
}

BOOL TBossTelesaKillSmallEnemy::receiveMessage(THitActor* sender, u32 message)
{
	return THitActor::receiveMessage(sender, message);
}

void TBossTelesaKillSmallEnemy::checkHit()
{
	mHit = gpMarioPos != nullptr && mPosition.distance(*gpMarioPos) < 300.0f;
	if (mHit && mOwner)
		mOwner->forceHide();
}

BOOL TBossTelesaTongue::receiveMessage(THitActor* sender, u32 message)
{
	return THitActor::receiveMessage(sender, message);
}

void TBossTelesaTongue::checkHit() { }

BOOL TBossTelesaBody::receiveMessage(THitActor* sender, u32 message)
{
	if (mOwner && (message == HIT_MESSAGE_TRAMPLE || message == HIT_MESSAGE_SPRAYED_BY_WATER))
		mOwner->damageRecover();
	return THitActor::receiveMessage(sender, message);
}

void TBossTelesaBody::checkHit() { }

TBossTelesaManager::TBossTelesaManager(const char* name) : TEnemyManager(name) { }

void TBossTelesaManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TEnemyManager::perform(cue, graphics);
}

void TBossTelesaManager::createModelData() { createModelDataArray(btelesa_model_data); }

TSpineEnemy* TBossTelesaManager::createEnemyInstance() { return new TBossTelesa; }

void TBossTelesaManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBossTelesaSaveLoadParams("/enemy/bosstelesa.prm");
	TEnemyManager::load(stream);
}

TBubble::TBubble(const char* name)
	: TWalkerEnemy(name)
	, mSaveParams(nullptr)
	, mCarriedEnemy(nullptr)
	, mHeight(0.0f)
	, mIsSplit(0)
	, mIsMoving(1)
	, mNoEffect(0)
{
}

const char** TBubble::getBasNameTable() const { return bubble_bastable; }

MtxPtr TBubble::getTakingMtx()
{
	return getModel() ? getModel()->getBaseTRMtx() : nullptr;
}

f32 TBubble::getGravityY() const
{
	if (mIsSplit)
		return mIsMoving ? 0.001f : 0.0f;
	return mGravity;
}

void TBubble::setDeadAnm() { setBckAnm(9); }

void TBubble::attackToMario()
{
	sendAttackMsgToMario();
	kill();
}

void TBubble::appendEnemy() { mCarriedEnemy = nullptr; }

void TBubble::appendItem() { }

void TBubble::calcRootMatrix()
{
	if (!isEaten())
		mPosition.y = 150.0f + mHeight;
	TWalkerEnemy::calcRootMatrix();
}

void TBubble::behaveToWater(THitActor*)
{
	if (mIsMoving)
		kill();
}

void TBubble::kill()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD))
		TSmallEnemy::kill();
}

void TBubble::split() { mIsSplit = 1; }

void TBubble::reset()
{
	TWalkerEnemy::reset();
	mHeight = MsRandF(50.0f, 150.0f);
	mIsSplit = 0;
	mIsMoving = 1;
	mNoEffect = 0;
	mCarriedEnemy = nullptr;
}

void TBubble::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mSaveParams = static_cast<TBubbleSaveLoadParams*>(getSaveParam());
}

TBubbleManager::TBubbleManager(const char* name) : TSmallEnemyManager(name) { }

void TBubbleManager::createModelData() { createModelDataArray(bubble_model_data); }

TSmallEnemy* TBubbleManager::createEnemyInstance() { return new TBubble; }

void TBubbleManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBubbleSaveLoadParams("/enemy/bubble.prm");
	TSmallEnemyManager::load(stream);
}

/* The retail nerve bodies are intentionally kept small here.  The important
 * transition points (hide, roulette start, spit and bubble split) are now
 * named and can be filled in independently without reintroducing anonymous
 * assembly-only classes. */
DEFINE_NERVE(TNerveBossTelesaFallDemo, TLiveActor)
{
	TBossTelesa* boss = static_cast<TBossTelesa*>(spine->getBody());
	if (spine->getTime() == 0)
		boss->rouletteStart();
	return boss->rouletteFall() && boss->slotFall();
}

DEFINE_NERVE(TNerveBossTelesaFreeze, TLiveActor)
{
	return spine->getTime() > 0 && spine->getBody()->checkCurAnmEnd(0);
}

DEFINE_NERVE(TNerveBossTelesaPrepareSlot, TLiveActor)
{
	return spine->getTime() > 30;
}

DEFINE_NERVE(TNerveBossTelesaSpitSlotItem, TLiveActor)
{
	return spine->getTime() > 20;
}

DEFINE_NERVE(TNerveBossTelesaSlotStart, TLiveActor)
{
	if (spine->getTime() == 0)
		static_cast<TBossTelesa*>(spine->getBody())->rouletteStart();
	return spine->getTime() > 60;
}

DEFINE_NERVE(TNerveBossTelesaAppear, TLiveActor)
{
	return spine->getTime() > 30;
}

DEFINE_NERVE(TNerveBossTelesaHideWait, TLiveActor)
{
	return spine->getTime() > 60;
}

DEFINE_NERVE(TNerveBossTelesaHide, TLiveActor)
{
	if (spine->getTime() == 0)
		static_cast<TBossTelesa*>(spine->getBody())->forceHide();
	return spine->getTime() > 30;
}

DEFINE_NERVE(TNerveBossTelesaSpit, TLiveActor)
{
	return spine->getTime() > 20;
}

DEFINE_NERVE(TNerveBossTelesaDie, TLiveActor)
{
	if (spine->getTime() == 0)
		static_cast<TBossTelesa*>(spine->getBody())->kill();
	return false;
}

DEFINE_NERVE(TNerveBubbleSplit, TLiveActor)
{
	if (spine->getTime() == 0)
		static_cast<TBubble*>(spine->getBody())->split();
	return spine->getTime() > 10;
}

DEFINE_NERVE(TNerveBubbleLive, TLiveActor)
{
	return spine->getTime() > 80;
}

/* Keep a named TU initializer so the list of state singletons has a stable
 * home when the remaining retail constructors are enabled. */
extern "C" static void __sinit_bosstelesa_cpp() { }
