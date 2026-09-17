#include <Enemy/BossTelesaObj.hpp>
#include <Enemy/BossTelesa.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Telesa.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DPacket.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JMath.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/SharedParts.hpp>
#include <Strategic/Spine.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <Map/MapCollisionEntry.hpp>

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

TSmallEnemy* TBubbleManager::createEnemyInstance() { return new TBubble; }

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
    , mEnemyInside(nullptr)
    , mFloatHeight(0.0f)
    , mIsSplit(false)
    , mHasInitialVelocity(false)
{
}

void TBubble::init(TLiveManager* live_manager)
{
	TWalkerEnemy::init(live_manager);

	mActorType = 0x10000020;
	unk150     = 0x11;
	mParams    = (TBubbleSaveLoadParams*)getSaveParam();

	mSpine->initWith(&TNerveBubbleLive::theNerve());
	getMActor()->setLightType(LIGHT_TYPE_INDIRECT);

	TScreenTexture* screenTexture
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
	const ResTIMG* textureInfo = screenTexture->getTexture()->getTexInfo();
	SMS_ChangeTextureAll(getMActor()->getModel()->getModelData(),
	                     "H_ma_rak_dummy", *textureInfo);
}

void TBubble::reset()
{
	TWalkerEnemy::reset();

	onLiveFlag(LIVE_FLAG_UNK8);

	TMsRange<f32> height(50.0f, 150.0f);
	mFloatHeight        = height.rand();
	mIsSplit            = false;
	mHasInitialVelocity = true;
	unk1D2              = false;
	mEnemyInside        = nullptr;

	mSpine->initWith(&TNerveBubbleLive::theNerve());
}

void TBubble::split()
{
	int num = mParams->mSLNumDivision.get();

	for (int i = 0; i < num; ++i) {
		TBubble* bubble = (TBubble*)gpConductor->makeOneEnemyAppear(
		    mPosition, "バブルマネージャー", 1);
		if (!bubble)
			return;

		bubble->reset();
		bubble->mPosition = mPosition;
		bubble->mPosition.y += mFloatHeight;
		bubble->mFloatHeight = 0.0f;
		bubble->mIsSplit     = true;

		TMsRange<f32> speed(-2.0f, 2.0f);
		JGeometry::TVec3<f32> velocity;
		velocity.x = speed.rand();
		velocity.y = speed.rand();
		velocity.z = speed.rand();
		bubble->setVelocity(velocity);
	}
}

f32 TBubble::getGravityY() const
{
	if (mIsSplit) {
		if (mHasInitialVelocity)
			return 0.001f;

		return 0.0f;
	}

	return mGravity;
}

void TBubble::kill()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if (mEnemyInside) {
		if (unk1D2)
			mEnemyInside->receiveMessage(this, HIT_MESSAGE_THROWN);
		else
			mEnemyInside->kill();

		mEnemyInside = nullptr;
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

void TBubble::behaveToWater(THitActor* actor)
{
	if (mSpine->getCurrentNerve() == &TNerveBubbleLive::theNerve()
	    && getMActor()->checkCurBckFromIndex(10)) {
		kill();

		TItem* item = (TItem*)gpItemManager->makeObjAppear(
		    mPosition.x, 20.0f + mPosition.y, mPosition.z, 0x20000002, true);
		if (item)
			item->killByTimer(1200);
	}
}

void TBubble::attackToMario()
{
	sendAttackMsgToMario();
	kill();
}

void TBubble::calcRootMatrix()
{
	if (isEaten())
		return;

	mPosition.y = 150.0f + (mGroundHeight + mFloatHeight);

	MsMtxSetXYZRPH(getMActor()->getModel()->getBaseTRMtx(), mPosition.x,
	               mPosition.y, mPosition.z, mRotation.x, mRotation.y,
	               mRotation.z);

	getMActor()->getModel()->setBaseScale(mScaling);
}

void TBubble::setDeadAnm() { setBckAnm(9); }

MtxPtr TBubble::getTakingMtx() { return getMActor()->getModel()->getBaseTRMtx(); }

const char** TBubble::getBasNameTable() const { return btelesa_bastable; }

// The map puts theNerve() ahead of TBubble::appendItem and execute() behind
// appendEnemy, so this nerve cannot have come from the DEFINE_NERVE macro:
// the two halves are spelled out with the bubble's two helpers in between.
const TNerveBubbleLive& TNerveBubbleLive::theNerve()
{
	static TNerveBubbleLive instance;
	return instance;
}

// TODO: incorrect size. Map records 216 bytes.
void TBubble::appendItem() { }

void TBubble::appendEnemy()
{
	mEnemyInside = nullptr;

	TMsRange<f32> chance(0.0f, 100.0f);
	f32 roll = chance.rand();

	TSmallEnemy* enemy;
	if (roll < 50.0f) {
		enemy = (TSmallEnemy*)gpConductor->makeOneEnemyAppear(
		    mPosition, "ポポマネージャー", 1);
		enemy->unk154 = 0.6f;
		enemy->reset();
	} else if (roll < 100.0f) {
		enemy = (TSmallEnemy*)gpConductor->makeOneEnemyAppear(
		    mPosition, "ボム兵マネージャー", 1);
		enemy->unk154 = 0.3f;
		enemy->reset();
	} else if (roll < 150.0f) {
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
		enemy->setVelocity(JGeometry::TVec3<f32>(0.0f, 2.0f, 10.0f));
		enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);
		mEnemyInside = enemy;
	}
}

BOOL TNerveBubbleLive::execute(TSpineBase<TLiveActor>* spine) const
{
	TBubble* bubble = (TBubble*)spine->getBody();

	if (spine->getTime() == 0) {
		bubble->offHitFlag(HIT_FLAG_NO_COLLISION);

		if (!bubble->mIsSplit) {
			bubble->setBckAnm(8);
		} else {
			bubble->setBckAnm(10);
			bubble->setGoalPathMario();
		}

		J3DFrameCtrl* frameCtrl
		    = bubble->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		TMsRange<f32> startFrame(0.0f, 20.0f);
		frameCtrl->setFrame(startFrame.rand());

		bubble->onLiveFlag(LIVE_FLAG_UNK8);
	} else if (bubble->checkCurAnmEnd(ANM_TYPE_BCK)) {
		bubble->offHitFlag(HIT_FLAG_NO_COLLISION);
		bubble->setBckAnm(10);
	}

	if (!bubble->mIsSplit) {
		if (bubble->mFloatHeight < bubble->mParams->mSLAddPosBase.get())
			bubble->mFloatHeight += 2.0f;
	} else {
		if (spine->getTime() > 40 && bubble->mHasInitialVelocity) {
			JGeometry::TVec3<f32> velocity = bubble->getVelocity();
			JGeometry::TVec3<f32> damped(velocity.x, velocity.y, velocity.z);
			damped.scale(0.98f);
			bubble->setVelocity(damped);
		} else
			bubble->walkBehavior(0, 1.0f);

		if (spine->getTime() == 80) {
			bubble->mHasInitialVelocity = false;
			bubble->setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
		}
	}

	bubble->mFloatHeight += 0.001f;
	if (bubble->mFloatHeight
	    > bubble->mPosition.y + bubble->mParams->mSLDeadHeight.get()) {
		bubble->unk1D2 = false;
		bubble->kill();
	}

	if (bubble->mScaling.x < bubble->mParams->mSLMaxScale.get()) {
		bubble->mScaling.x = bubble->mScaling.y = bubble->mScaling.z
		    = bubble->mScaling.z * bubble->mParams->mSLRateExpand.get();
	}

	if (spine->getTime() > bubble->mParams->mSLLiveTime.get()) {
		spine->pushAfterCurrent(&TNerveBubbleSplit::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBubbleSplit, TLiveActor)
{
	TBubble* bubble = (TBubble*)spine->getBody();

	if (spine->getTime() == 0) {
		bubble->onHitFlag(HIT_FLAG_NO_COLLISION);
		bubble->split();
	}

	if (spine->getTime() == 10)
		bubble->setBckAnm(9);

	if (bubble->checkCurAnmEnd(ANM_TYPE_BCK)
	    && bubble->getMActor()->checkCurBckFromIndex(9)) {
		bubble->unk1D2 = false;
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
	return new TBossTelesa;
}

void TBossTelesaManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "btelesa.bmd", 0x15300000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

void TBossTelesaManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TEnemyManager::perform(cue, graphics);
}

BOOL TBossTelesaBody::receiveMessage(THitActor* sender, u32 message)
{
	return false;
}

// TODO: incorrect size. Map records 160 bytes.
bool TBossTelesaBody::checkHit() { return false; }

// TODO: incorrect size. Map records 568 bytes.
bool TBossTelesaTongue::checkHit() { return false; }

BOOL TBossTelesaTongue::receiveMessage(THitActor* sender, u32 message)
{
	return false;
}

void TBossTelesaKillSmallEnemy::checkHit() { }

void TTelesaSlot::initMapObj() { }

void TTelesaSlot::randomReset() { }

void TTelesaSlot::calcRootMatrix() { }

void TTelesaSlot::moveObject() { }

void TTelesaSlot::moveStart() { }

u32 TTelesaSlot::touchWater(THitActor* actor) { return 0; }

void TTelesaSlot::forceStopSlot(int index) { }

bool TTelesaSlot::isRollDrum() { return false; }

int TTelesaSlot::getSlotResult() { return -1; }

// TODO: incorrect size. Map records 44 bytes.
int TTelesaSlot::getDrumResult(int index) { return 0; }

int TTelesaSlot::getForcastResult(int index) { return 0; }

int TTelesaSlot::getResultFromAng(f32 angle) { return 0; }

// TODO: incorrect size. Map records 412 bytes.
void TTelesaSlot::calcObjCollision() { }

// TODO: incorrect size. Map records 88 bytes.
void TTelesaSlot::entryObjCollision() { }

f32 TBossTelesa::mEnemyGenRate           = 0.5f;
f32 TBossTelesa::mItemGenRate            = 0.1f;
u8 TBossTelesa::mNormalAlpha             = 150;
f32 TBossTelesa::mBaseHoseiPosY          = -300.0f;
f32 TBossTelesa::mRouletteUpRate         = 0.03f;
s32 TBossTelesa::mTelesaGenerateInterval = 400;
f32 TBossTelesa::mCameraMoveLimit        = 1000.0f;
f32 TBossTelesa::mCameraMoveSp           = 0.02f;

bool TBossTelesa::mGenerateTelesa;
bool TBossTelesa::mGenerateBubble;
int TBossTelesa::mEnemyTestType;

TBossTelesa::TBossTelesa(const char* name)
    : TSpineEnemy(name)
{
}

void TBossTelesa::init(TLiveManager* live_manager) { }

void TBossTelesa::loadAfter() { }

void TBossTelesa::reset() { }

void TBossTelesa::moveObject() { }

void TBossTelesa::kill() { }

MtxPtr TBossTelesa::getTakingMtx() { return nullptr; }

// TODO: incorrect size. Map records 472 bytes.
void TBossTelesa::prepareGenerate() { }

void TBossTelesa::calcRootMatrix() { }

void TBossTelesa::perform(u32 cue, JDrama::TGraphics* graphics) { }

BOOL TBossTelesa::receiveMessage(THitActor* sender, u32 message)
{
	return false;
}

// TODO: incorrect size. Map records 588 bytes.
BOOL TBossTelesa::checkMessage(THitActor* sender, u32 message) { return false; }

void TBossTelesa::checkHitObject(THitActor* actor) { }

void TBossTelesa::setSpicy(TLiveActor* actor) { }

void TBossTelesa::damageRecover() { }

// TODO: incorrect size. Map records 284 bytes.
void TBossTelesa::tongueHitWater() { }

bool TBossTelesa::rouletteFall() { return false; }

bool TBossTelesa::slotFall() { return false; }

// TODO: incorrect size. Map records 44 bytes.
void TBossTelesa::openWaterPlace() { }

void TBossTelesa::flashItem(int index) { }

// TODO: incorrect size. Map records 48 bytes.
void TBossTelesa::onAllCollision() { }

// TODO: incorrect size. Map records 48 bytes.
void TBossTelesa::offAllCollision() { }

const char** TBossTelesa::getBasNameTable() const { return btelesa_bastable; }

void TBossTelesa::genAttacker() { }

// TODO: incorrect size. Map records 212 bytes.
void TBossTelesa::setBckAnm(int index) { }

// TODO: incorrect size. Map records 140 bytes.
bool TBossTelesa::isInDamage() { return false; }

void TBossTelesa::rouletteStart() { }

// TODO: incorrect size. Map records 64 bytes.
void TBossTelesa::slotStart() { }

// TODO: incorrect size. Map records 168 bytes.
void TBossTelesa::slotStop() { }

// TODO: incorrect size. Map records 76 bytes.
int TBossTelesa::checkSlotResult() { return 0; }

void TBossTelesa::generateSlotItem() { }

// TODO: incorrect size. Map records 136 bytes.
void TBossTelesa::fruitCollisionOn() { }

// TODO: incorrect size. Map records 212 bytes.
void TBossTelesa::checkSlot() { }

// TODO: incorrect size. Map records 84 bytes.
bool TBossTelesa::checkAllItemDead() { return false; }

void TBossTelesa::forceAllItemKill() { }

// TODO: incorrect size. Map records 116 bytes.
void TBossTelesa::rollRouletteCircle() { }

// TODO: incorrect size. Map records 84 bytes.
bool TBossTelesa::isForceRestart() { return false; }

void TBossTelesa::forceHide() { }

// TODO: incorrect size. Map records 496 bytes.
void TBossTelesa::fanfale() { }

// TODO: no nerve body below is reconstructed; each carries its map size.

// TODO: incorrect size. Map records 2248 bytes.
DEFINE_NERVE(TNerveBossTelesaDie, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 304 bytes.
DEFINE_NERVE(TNerveBossTelesaSpit, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 472 bytes.
DEFINE_NERVE(TNerveBossTelesaHide, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 512 bytes.
DEFINE_NERVE(TNerveBossTelesaHideWait, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 972 bytes.
DEFINE_NERVE(TNerveBossTelesaAppear, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 692 bytes.
DEFINE_NERVE(TNerveBossTelesaSlotStart, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 592 bytes.
DEFINE_NERVE(TNerveBossTelesaSpitSlotItem, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1640 bytes.
DEFINE_NERVE(TNerveBossTelesaPrepareSlot, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 432 bytes.
DEFINE_NERVE(TNerveBossTelesaFreeze, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 492 bytes.
DEFINE_NERVE(TNerveBossTelesaFallDemo, TLiveActor) { return FALSE; }
