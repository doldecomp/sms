#include <Enemy/BossTelesa.hpp>

#include <Enemy/Conductor.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
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

f32 TBossTelesa::mEnemyGenRate           = 0.1f;
f32 TBossTelesa::mItemGenRate            = 0.1f;
u8 TBossTelesa::mNormalAlpha             = 0xFF;
f32 TBossTelesa::mBaseHoseiPosY          = 350.0f;
f32 TBossTelesa::mRouletteUpRate         = 0.1f;
s32 TBossTelesa::mTelesaGenerateInterval = 600;
f32 TBossTelesa::mCameraMoveLimit        = 300.0f;
f32 TBossTelesa::mCameraMoveSp           = 2.0f;

/* The slot angle table is deliberately kept as the retail eight-way split. */

/* The retail nerve bodies are intentionally kept small here.  The important
 * transition points (hide, roulette start, spit and bubble split) are now
 * named and can be filled in independently without reintroducing anonymous
 * assembly-only classes. */

/* Keep a named TU initializer so the list of state singletons has a stable
 * home when the remaining retail constructors are enabled. */

extern "C" static void __sinit_bosstelesa_cpp() { }

TBubbleSaveLoadParams::TBubbleSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
    , PARAM_INIT(mSLLiveTime, 1200)
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

TSmallEnemy* TBubbleManager::createEnemyInstance() { return new TBubble; }

void TBubbleManager::createModelData()
{
	createModelDataArray(bubble_model_data);
}

TBubble::TBubble(const char* name)
    : TWalkerEnemy(name)
    , mCarriedEnemy(nullptr)
    , mTakingMatrix()
    , mHeight(0.0f)
    , mIsSplit(0)
    , mIsMoving(0)
{
}

void TBubble::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mActorType  = 0x10000020;
	unk150      = 0x11;
	mSaveParams = static_cast<TBubbleSaveLoadParams*>(getSaveParam());
	mSpine->initWith(&TNerveBubbleLive::theNerve());
	mMActor->setLightType(3);
}

void TBubble::reset()
{
	TWalkerEnemy::reset();
	onLiveFlag(LIVE_FLAG_UNK8);
	mHeight       = MsRandF(50.0f, 150.0f);
	mIsSplit      = 0;
	mIsMoving     = 1;
	mNoEffect     = 0;
	mCarriedEnemy = nullptr;
}

void TBubble::split() { mIsSplit = 1; }

f32 TBubble::getGravityY() const
{
	if (mIsSplit)
		return mIsMoving ? 0.001f : 0.0f;
	return mGravity;
}

void TBubble::kill()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD))
		TSmallEnemy::kill();
}

void TBubble::behaveToWater(THitActor*)
{
	if (mIsMoving)
		kill();
}

void TBubble::attackToMario()
{
	sendAttackMsgToMario();
	kill();
}

void TBubble::calcRootMatrix()
{
	if (!isEaten()) {
		mPosition.y = 150.0f + (mGroundHeight + mHeight);
		MsMtxSetXYZRPH(getMActor()->getModel()->getBaseTRMtx(), mPosition.x,
		               mPosition.y, mPosition.z, mRotation.x, mRotation.y,
		               mRotation.z);
		getMActor()->getModel()->setBaseScale(mScaling);
	}
}

void TBubble::setDeadAnm() { setBckAnm(9); }

MtxPtr TBubble::getTakingMtx()
{
	return getMActor()->getModel()->getBaseTRMtx();
}

const char** TBubble::getBasNameTable() const { return bubble_bastable; }

const TNerveBubbleLive& TNerveBubbleLive::theNerve()
{
	static TNerveBubbleLive instance;
	return instance;
}

void TBubble::appendItem() { }

void TBubble::appendEnemy() { mCarriedEnemy = nullptr; }

BOOL TNerveBubbleLive::execute(TSpineBase<TLiveActor>* spine) const
{
	return spine->getTime() > 80;
}

DEFINE_NERVE(TNerveBubbleSplit, TLiveActor)
{
	if (spine->getTime() == 0)
		static_cast<TBubble*>(spine->getBody())->split();
	return spine->getTime() > 10;
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
	return new TBossTelesa;
}

void TBossTelesaManager::createModelData()
{
	createModelDataArray(btelesa_model_data);
}

void TBossTelesaManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TEnemyManager::perform(cue, graphics);
}

BOOL TBossTelesaBody::receiveMessage(THitActor* sender, u32 message)
{
	if (mOwner
	    && (message == HIT_MESSAGE_TRAMPLE
	        || message == HIT_MESSAGE_SPRAYED_BY_WATER))
		mOwner->damageRecover();
	return THitActor::receiveMessage(sender, message);
}

void TBossTelesaBody::checkHit() { }

MActorAnmBck* MActor::getCurBckAnmPtr() { return unkC; }

void TBossTelesaTongue::checkHit() { }

BOOL TBossTelesaTongue::receiveMessage(THitActor* sender, u32 message)
{
	return THitActor::receiveMessage(sender, message);
}

void TBossTelesaKillSmallEnemy::checkHit()
{
	mHit = gpMarioPos != nullptr && mPosition.distance(*gpMarioPos) < 300.0f;
	if (mHit && mOwner)
		mOwner->forceHide();
}

BOOL TBossTelesaKillSmallEnemy::receiveMessage(THitActor* sender, u32 message)
{
	return THitActor::receiveMessage(sender, message);
}

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

void TTelesaSlot::randomReset()
{
	for (int i = 0; i < 3; ++i) {
		unk13C[i]      = unk168 * (s32)MsRandF(0.0f, 8.0f);
		((&mStop0)[i]) = 0;
	}
}

void TTelesaSlot::calcRootMatrix() { TSlotDrum::calcRootMatrix(); }

void TTelesaSlot::moveObject() { TSlotDrum::moveObject(); }

void TTelesaSlot::moveStart()
{
	mForceStop = 1;
	mRolling   = 1;
	mRoll0     = 1;
	mRoll1     = 1;
	mRoll2     = 1;
	mStop0 = mStop1 = mStop2 = 0;
	unk138[0]                = -unk158;
	unk138[1]                = -0.8f * unk158;
	unk138[2]                = unk158;
}

u32 TTelesaSlot::touchWater(THitActor*) { return 0; }

void TTelesaSlot::forceStopSlot(int drum)
{
	if (!mForceStop)
		return;
	mForceResult      = getForcastResult(drum);
	((&mRoll0)[drum]) = 0;
	mForceStop        = 0;
}

u8 TTelesaSlot::isRollDrum()
{
	if (mRoll0)
		return 1;
	if (mRoll1)
		return 1;
	if (mRoll2)
		return 1;
	mRolling = 0;
	return 0;
}

int TTelesaSlot::getSlotResult()
{
	int result = getResultFromAng(unk13C[0]);
	int i      = 1;
	do {
		if (result != getResultFromAng(unk13C[i]))
			return -1;
		++i;
	} while (i < 3);
	return result;
}

int TTelesaSlot::getDrumResult(int drum)
{
	return getResultFromAng(unk13C[drum]);
}

int TTelesaSlot::getForcastResult(int drum)
{
	f32 angle = unk13C[drum];
	f32 speed = unk138[drum];

	do {
		if (fabsf(speed) > unk160) {
			angle += speed;
			if (speed > 0.0f)
				speed -= unk15C;
			else
				speed += unk15C;

			if (angle >= 360.0f)
				angle -= 360.0f;
			if (angle <= 0.0f)
				angle += 360.0f;
			continue;
		}

		angle += speed;
		if (angle >= 360.0f)
			angle -= 360.0f;
		if (angle <= 0.0f)
			angle += 360.0f;
	} while ((s32)fabsf(angle) % unk168 != 0);

	return getResultFromAng((f32)(unk168 * (s32)(angle / (f32)unk168)));
}

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
	if (angle < 359.0f)
		return 2;
	return 2;
}

void TTelesaSlot::calcObjCollision() { }

void TTelesaSlot::entryObjCollision() { }

TBossTelesa::TBossTelesa(const char* name)
    : TSpineEnemy(name)
    , mBattleState(1)
    , mBattleTimer(0)
    , mBattlePhase(0)
    , mSaveParams(nullptr)
    , mCurrentItem(-1)
    , mCurrentFruit(-1)
    , mFallSpeed(0.0f)
    , mBodyCollision(nullptr)
    , mSlot(nullptr)
    , mTakingActor(nullptr)
    , mIsSpicy(0)
    , mDamageTimer(0)
    , mStateTimer(0)
    , mDamage(0)
    , mGenerateTimer(0)
    , mMarioHP(0)
    , mForceRestart(1)
    , mInDamage(1)
    , mRouletteTimer(0)
    , mRoulettePosY(0.0f)
    , mRouletteSp(0.0f)
    , mSlotResult(0)
    , mSlotItemCount(0)
    , mSlotStop(3)
    , mDead(0)
{
}

void TBossTelesa::init(TLiveManager* manager)
{
	TSpineEnemy::init(manager);
	mSaveParams      = static_cast<TBossTelesaSaveLoadParams*>(getSaveParam());
	mBodyCollision   = new TBossTelesaBody(this);
	mTongueCollision = new TBossTelesaTongue(this);
	mKillSmallEnemy  = new TBossTelesaKillSmallEnemy(this);
}

void TBossTelesa::loadAfter()
{
	mMarioHP = 1;
	if (!mSaveParams)
		mSaveParams = static_cast<TBossTelesaSaveLoadParams*>(getSaveParam());
}

void TBossTelesa::reset()
{
	TSpineEnemy::reset();
	mDead          = 0;
	mDamage        = 0;
	mGenerateTimer = 0;
	forceAllItemKill();
}

void TBossTelesa::moveObject()
{
	TSpineEnemy::moveObject();
	if (mGenerateTimer > 0)
		--mGenerateTimer;
}

void TBossTelesa::kill()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD)) {
		mDead = 1;
		TSpineEnemy::kill();
	}
}

MtxPtr TBossTelesa::getTakingMtx()
{
	return getModel() ? getModel()->getBaseTRMtx() : nullptr;
}

void TBossTelesa::prepareGenerate() { mGenerateTimer = 0; }

void TBossTelesa::calcRootMatrix() { TSpineEnemy::calcRootMatrix(); }

void TBossTelesa::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSpineEnemy::perform(cue, graphics);
	if (cue & CUE_MOVE)
		damageRecover();
}

BOOL TBossTelesa::receiveMessage(THitActor*, u32) { return TRUE; }

BOOL TBossTelesa::checkMessage(THitActor* sender, u32 message)
{
	return receiveMessage(sender, message);
}

BOOL TBossTelesa::checkHitObject(THitActor* actor)
{
	return actor != nullptr && actor->checkActorType(ACTOR_TYPE_PLAYER);
}

void TBossTelesa::setSpicy(TLiveActor* actor)
{
	if (actor)
		actor->onLiveFlag(LIVE_FLAG_UNK10);
	mIsSpicy = 1;
}

void TBossTelesa::damageRecover()
{
	if (mDamageTimer > 0)
		--mDamageTimer;
	if (mDamageTimer == 0)
		mDamage = 0;
}

void TBossTelesa::tongueHitWater() { }

u8 TBossTelesa::rouletteFall()
{
	if (mRouletteTimer > 0)
		--mRouletteTimer;
	return mRouletteTimer == 0;
}

u8 TBossTelesa::slotFall()
{
	if (!mSlot)
		return 1;
	return mSlot->isRollDrum() == 0;
}

void TBossTelesa::openWaterPlace() { }

void TBossTelesa::flashItem(int item)
{
	mSlotResult  = item;
	mDamageTimer = 0;
}

void TBossTelesa::onAllCollision()
{
	if (mBodyCollision)
		mBodyCollision->onHitFlag(HIT_FLAG_NO_COLLISION);
	if (mTongueCollision)
		mTongueCollision->onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBossTelesa::offAllCollision()
{
	if (mBodyCollision)
		mBodyCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
	if (mTongueCollision)
		mTongueCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
}

const char** TBossTelesa::getBasNameTable() const { return btelesa_bastable; }

void TBossTelesa::genAttacker()
{
	mGenerateTimer = mSaveParams ? mSaveParams->mSLGenAttackerTime.get() : 500;
}

void TBossTelesa::setBckAnm(int index)
{
	if (getMActor())
		getMActor()->setBckFromIndex(index);
}

bool TBossTelesa::isInDamage() { return mInDamage != 0; }

void TBossTelesa::rouletteStart()
{
	mRouletteTimer = 0;
	mSlotStop      = 0;
	if (mSlot)
		mSlot->moveStart();
}

void TBossTelesa::slotStart() { rouletteStart(); }

void TBossTelesa::slotStop()
{
	if (mSlot)
		mSlot->forceStopSlot(0);
}

int TBossTelesa::checkSlotResult() { return mSlot ? mSlotResult : -1; }

void TBossTelesa::generateSlotItem()
{
	++mSlotItemCount;
	mGenerateTimer = 0;
}

void TBossTelesa::fruitCollisionOn() { mSlotStop = 0; }

bool TBossTelesa::checkSlot() { return mSlot != nullptr; }

bool TBossTelesa::checkAllItemDead()
{
	return mItem0 == 0 && mItem1 == 0 && mItem2 == 0 && mItem3 == 0;
}

void TBossTelesa::forceAllItemKill()
{
	mItem0 = mItem1 = mItem2 = mItem3 = 0;
	mFruit0 = mFruit1 = mFruit2 = 0;
}

void TBossTelesa::rollRouletteCircle()
{
	if (mSlot)
		mSlot->moveStart();
}

bool TBossTelesa::isForceRestart() { return mForceRestart != 0; }

void TBossTelesa::forceHide()
{
	mDead = 1;
	offLiveFlag(LIVE_FLAG_UNK200);
	onLiveFlag(LIVE_FLAG_HIDDEN);
}

void TBossTelesa::fanfale() { }

DEFINE_NERVE(TNerveBossTelesaDie, TLiveActor)
{
	if (spine->getTime() == 0)
		static_cast<TBossTelesa*>(spine->getBody())->kill();
	return false;
}

DEFINE_NERVE(TNerveBossTelesaSpit, TLiveActor) { return spine->getTime() > 20; }

DEFINE_NERVE(TNerveBossTelesaHide, TLiveActor)
{
	if (spine->getTime() == 0)
		static_cast<TBossTelesa*>(spine->getBody())->forceHide();
	return spine->getTime() > 30;
}

DEFINE_NERVE(TNerveBossTelesaHideWait, TLiveActor)
{
	return spine->getTime() > 60;
}

DEFINE_NERVE(TNerveBossTelesaAppear, TLiveActor)
{
	return spine->getTime() > 30;
}

DEFINE_NERVE(TNerveBossTelesaSlotStart, TLiveActor)
{
	if (spine->getTime() == 0)
		static_cast<TBossTelesa*>(spine->getBody())->rouletteStart();
	return spine->getTime() > 60;
}

DEFINE_NERVE(TNerveBossTelesaSpitSlotItem, TLiveActor)
{
	return spine->getTime() > 20;
}

DEFINE_NERVE(TNerveBossTelesaPrepareSlot, TLiveActor)
{
	return spine->getTime() > 30;
}

DEFINE_NERVE(TNerveBossTelesaFreeze, TLiveActor)
{
	return spine->getTime() > 0 && spine->getBody()->checkCurAnmEnd(0);
}

DEFINE_NERVE(TNerveBossTelesaFallDemo, TLiveActor)
{
	TBossTelesa* boss = static_cast<TBossTelesa*>(spine->getBody());
	if (spine->getTime() == 0)
		boss->rouletteStart();
	return boss->rouletteFall() && boss->slotFall();
}
