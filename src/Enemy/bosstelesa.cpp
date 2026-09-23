#include <Enemy/BossTelesaObj.hpp>
#include <Enemy/BossTelesa.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraShake.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/HamuKuri.hpp>
#include <Enemy/Telesa.hpp>
#include <GC2D/GCConsole2.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DPacket.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DVertex.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JGeometry/JGMatrix34.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MSound/BackgroundMusic.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Map/Map.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/SharedParts.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <System/MarDirector.hpp>
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

	getSpine()->initWith(&TNerveBubbleLive::theNerve());
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

// TODO: +0x10 of named block above the TMsRange (TVec3 call temp already
// sits at 0x10). A conductor binder lands the 0x50 frame from the pool
// and pushes that temp; a named gpConductor local hoists the load.
// Also inert: getPosition() at 1/2/4 sites, an unnamed TMsRange, roll or
// enemy declared first.
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

	// TODO: the ROM hoists the mParams load and the mSLAddPosBase read above
	// the mIsSplit branch and loads mFloatHeight second; we load mFloatHeight
	// first. Four bytes of pool below the setGoalPathMario block are the only
	// other residue (frame 0xf8 exact).
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
	    > bubble->mPosition.y + bubble->getSaveParam2()->mSLDeadHeight.get()) {
		bubble->unk1D2 = false;
		bubble->kill();
	}

	if (bubble->mScaling.x < bubble->getSaveParam2()->mSLMaxScale.get()) {
		bubble->mScaling.x = bubble->mScaling.y = bubble->mScaling.z
		    = bubble->mScaling.z * bubble->getSaveParam2()->mSLRateExpand.get();
	}

	if (spine->getTime() > bubble->getSaveParam2()->mSLLiveTime.get()) {
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
	mOwner->checkMessage(sender, message);
	return true;
}

void TBossTelesaBody::checkHit()
{
	unk6C = false;
	for (int i = 0; i < mColCount; ++i) {
		THitActor* other = mCollisions[i];
		if (other->isActorType(0x80000001))
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		else
			mOwner->checkHitObject(other);
	}
}

void TBossTelesaTongue::checkHit()
{
	for (int i = 0; i < mColCount; ++i) {
		THitActor* other = mCollisions[i];
		if (other->isActorType(0x80000001)) {
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		} else if (other->isActorType(0x40000395)) {
			mOwner->setSpicy((TLiveActor*)other);
		} else if (100.0f + mPosition.y < other->mPosition.y) {
			mOwner->checkHitObject(other);
		}
	}
}

BOOL TBossTelesaTongue::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER)
		mOwner->tongueHitWater();

	return true;
}

// The ROM `bl`s forceHide() from the bottom of this body, so forceHide had to
// reach fifteen statements (see the note there); with the call out of line the
// whole function falls out. The block-scoped `actor` is the ROM's second copy
// of the collision pointer (`addi r28, r4, 0` inside the `if`, with the
// unnamed CSE temp kept alive across the materialised bool) and it is also
// worth the +4 of pool the frame needs; the element goes through a named
// `collision` local (the ROM's extra `addi r4, r3, 0`) and Mario's position
// through SMS_GetMarioPos(), whose reference temporary is the last +8.
void TBossTelesaKillSmallEnemy::checkHit()
{
	unk6C = false;

	for (int i = 0; i < mColCount; ++i) {
		THitActor* collision = mCollisions[i];
		if (collision->checkActorType(ACTOR_TYPE_ENEMY)) {
			TLiveActor* actor = (TLiveActor*)collision;

			if (actor->getActorType() == 0x10000013)
				((THamuKuri*)actor)->selectCapHolder();

			actor->kill();
		}
	}

	JGeometry::TVec3<f32> toMario = SMS_GetMarioPos();
	toMario.sub(mPosition);
	toMario.y = 0.0f;

	if (MsVECMag2(toMario) < 300.0f) {
		mOwner->forceHide();
		unk6C = true;
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
	unk140 = getDamageRadius() / 3.0f;
	unk144 = getDamageHeight();

	unk1DC = new TMapCollisionMove;
	unk1DC->init(2, 0, 0, nullptr);

	randomReset();
}

void TTelesaSlot::randomReset()
{
	TMsRange<s32> stop(0, 8);

	for (int i = 0; i < 3; ++i) {
		unk13C[i]     = unk168 * stop.rand();
		mIsRolling[i] = false;
	}
}

void TTelesaSlot::calcRootMatrix()
{
	bool rolling = false;
	for (int i = 0; i < 3; ++i) {
		if (unk138[i] != 0.0f)
			rolling = true;
	}

	if (rolling) {
		// Every other frame, so that the spin loop does not retrigger itself.
		if (unk1E0)
			gpMSound->startSoundActor(MSD_SE_OBJ_SLOT_SPIN, &mPosition);

		unk1E0 = 1 - unk1E0;
	}

	TSlotDrum::calcRootMatrix();
}

// TODO: TTelesaSlot::mOwner wants an accessor in BossTelesaObj.hpp; parked
// here as a TU-local until a header batch adds it.

static inline int TelesaSlotForcastResult(TTelesaSlot* p, int index)
{
	int result = p->getForcastResult(index);
	return result;
}

static inline MSound* TelesaSlotGetMSound()
{
	MSound* sound = gpMSound;
	return sound;
}

static inline TBossTelesa* TelesaSlotBindOwner(const TTelesaSlot* p)
{
	TBossTelesa* owner = p->mOwner;
	return owner;
}

// The 0xd0 frame is three rungs: the owner binder at the params read (+0x10)
// and at the pasted fanfale() body (+8), and the sound fork (+0x10).
// TODO: one callee-saved register left -- the ROM keeps the fanfale() owner in
// r27 where we rank it r26 (a pre-existing gap); every instruction is exact.
void TTelesaSlot::moveObject()
{
	TLiveActor::moveObject();

	for (int i = 0; i < unk148; ++i) {
		if (mForceHit[i]
		    && mForcedResult == TelesaSlotForcastResult(this, i)) {
			mIsRolling[i] = false;
			mForceHit[i]  = false;
		}

		f32 speed = unk138[i];
		if (speed == 0.0f)
			continue;

		if (fabs(speed) > unk160) {
			unk13C[i] += speed;

			if (!mIsRolling[i]) {
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

			if (!mIsRolling[i]) {
				if ((s32)fabs(unk13C[i]) % unk168 == 0) {
					unk13C[i] = unk168 * (s32)(unk13C[i] / (f32)unk168);
					unk138[i] = 0.0f;

					TelesaSlotGetMSound()->startSoundActor(
					    MSD_SE_BS_TELESA_SLT_STOP,
					                          &mPosition, 0, nullptr, 0, 4);

					for (int j = 0; j < unk148; ++j) {
						if (mIsRolling[j]) {
							TMsRange<f32> chance(0.0f, 1.0f);
							f32 rate = TelesaSlotBindOwner(this)
							               ->mParams->mSLSlotHitCollectRate
							               .get();
							if (chance.rand() <= rate)
								mForceHit[j] = true;
							else
								mIsRolling[j] = false;
						}
					}

					bool allStopped = true;
					for (int j = 0; j < 3; ++j) {
						if (unk138[j] != 0.0f)
							allStopped = false;
					}

					if (allStopped)
						TelesaSlotBindOwner(this)->fanfale();
				}
			}
		}
	}
}

void TTelesaSlot::moveStart()
{
	mStopRequested = true;
	unk19B         = true;

	for (int i = 0; i < 3; ++i) {
		mIsRolling[i] = true;
		mForceHit[i]  = false;

		f32 direction = 1.0f;
		if (i == 0)
			direction = -1.0f;
		if (i == 1)
			direction = -0.8f;

		unk138[i] = direction * unk158;
	}
}

u32 TTelesaSlot::touchWater(THitActor* actor) { return 0; }

static inline TBossTelesa* TelesaSlotGetOwner(const TTelesaSlot* p)
{
	return p->mOwner;
}

void TTelesaSlot::forceStopSlot(int index)
{
	TMsRange<f32> chance(0.0f, 1.0f);

	if (!mStopRequested)
		return;

	f32 rate = TelesaSlotGetOwner(this)->getSaveParam2()->mSLSlotFirstHitCollectRate.get();
	if (SMS_GetMarioHP() == 1)
		rate = 0.9f;

	if (chance.rand() <= rate) {
		mForcedResult = 2;
		if (SMS_GetMarioHP() <= 3)
			mForcedResult = 0;

		mForceHit[index] = true;
	} else {
		mForcedResult     = getForcastResult(index);
		mIsRolling[index] = false;
	}

	if (mForcedResult == TelesaSlotGetOwner(this)->unk1A8)
		mForcedResult = 3;

	if (mForcedResult == 0) {
		if (!TelesaSlotGetOwner(this)->unk370)
			mForcedResult = 1;
		else if (SMS_GetMarioHP() >= 6)
			mForcedResult = 3;
	}

	mStopRequested = false;
}

bool TTelesaSlot::isRollDrum()
{
	if (mIsRolling[0])
		return true;
	if (mIsRolling[1])
		return true;
	if (mIsRolling[2])
		return true;

	unk19B = false;
	return false;
}

int TTelesaSlot::getSlotResult()
{
	int result = getDrumResult(0);

	for (int i = 1; i < 3; ++i) {
		int drum = getDrumResult(i);
		if (result != drum)
			return -1;
	}

	return result;
}

int TTelesaSlot::getDrumResult(int index)
{
	return getResultFromAng(unk13C[index]);
}

int TTelesaSlot::getForcastResult(int index)
{
	int guard  = 0;
	f32 angle  = unk13C[index];
	f32 speed  = unk138[index];

	while (true) {
		if (fabs(speed) > unk160) {
			angle += speed;

			if (speed > 0.0f)
				speed -= unk15C;
			else
				speed += unk15C;

			if (angle >= 360.0f)
				angle -= 360.0f;
			if (angle <= 0.0f)
				angle += 360.0f;
		} else {
			angle += speed;

			if (angle >= 360.0f)
				angle -= 360.0f;
			if (angle <= 0.0f)
				angle += 360.0f;

			if ((s32)fabs(angle) % unk168 == 0) {
				angle = unk168 * (s32)(angle / (f32)unk168);
				break;
			}
		}

		if (++guard > 10000)
			break;
	}

	return getResultFromAng(angle);
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
    , unk150(1)
    , unk154(0)
    , unk158(0)
    , mParams(nullptr)
    , unk160(-1)
    , unk164(-1)
    , unk168(0.0f)
    , mBody(nullptr)
    , mSlot(nullptr)
    , mSlotFrame(nullptr)
    , unk18C(false)
    , unk1A8(0)
    , mSlotItemNum(0)
    , unk350(false)
    , mTelesaManager(nullptr)
    , mMarioHP(0)
    , unk35A(true)
    , unk35B(true)
    , unk35C(0)
    , unk360(0.0f)
    , unk364(0.0f)
    , unk368(0)
    , unk36C(0)
    , unk370(3)
    , unk384(false)
{
}

void TBossTelesa::init(TLiveManager* live_manager)
{
	mManager = live_manager;
	live_manager->manageActor(this);

	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("btelesa.bmd", 3);

	onLiveFlag(LIVE_FLAG_DEAD);
	onHitFlag(HIT_FLAG_NO_COLLISION);

	TSpineEnemyParams* params = getSaveParam();
	if (params) {
		mBodyRadius       = params->mSLBodyRadius.get();
		mWallRadius       = params->mSLWallRadius.get();
		mHeadHeight       = params->mSLHeadHeight.get();
		mScaledBodyRadius = mBodyScale * mBodyRadius;
		mHitPoints        = getMaxHitPoints();
	}

	initHitActor(0, 5, 0x98000000, mBodyRadius, mHeadHeight, mBodyRadius,
	             mHeadHeight);
	mGroundPlane = TMap::getIllegalCheckData();

	setGoalPathMario();
	initAnmSound();

	mParams    = (TBossTelesaSaveLoadParams*)getSaveParam();
	mActorType = 0x08000013;
	onHitFlag(0xD0000000);

	mSpine->initWith(&TNerveBossTelesaFallDemo::theNerve());
	onLiveFlag(LIVE_FLAG_UNK8);

	getMActor()->setLightType(LIGHT_TYPE_OBJECT);
	getMActor()->unk40 = true;
	getMActor()->initNormalMotionBlend();

	J3DModel* model = getMActor()->getModel();
	if (model->getSkinDeform() == nullptr) {
		J3DSkinDeform* deform = new J3DSkinDeform;
		model->setSkinDeform(deform, J3D_DEFORM_ATTACH_FLAG_UNK_1);
	}

	getMActor()->resetDL();

	unk348.r = unk348.g = unk348.b = unk348.a = 0xFF;
	unk34C.r = unk34C.g = unk34C.b = unk34C.a = mNormalAlpha;

	for (int i = 0; i < getMActor()->getModel()->getModelData()->getMaterialNum();
	     ++i) {
		if (i
		    == getModel()->getModelData()->getMaterialName()->getIndex(
		        "_mat_body"))
			SMS_InitPacket_TwoTevKColor(getMActor()->getModel(), i, GX_KCOLOR0,
			                            &unk34C, GX_KCOLOR1, &unk348);
		else
			SMS_InitPacket_OneTevKColor(getMActor()->getModel(), i, GX_KCOLOR0,
			                            &unk34C);
	}

	reset();

	// The roulette the boss starts on doubles as its home position.
	for (int i = 0; i < gpMapObjManager->getObjNum(); ++i) {
		if (gpMapObjManager->getObj(i)->isActorType(0x4000019A)) {
			unk154    = 0;
			unk158    = 0;
			mPosition = gpMapObjManager->getObj(i)->getPosition();
		}
	}

	// A leftover: the loop has no body.
	if (mInstanceIndex == 0) {
		for (u8 i = 0; i < getMActor()->getModel()->getModelData()->getJointNum();
		     ++i) {
		}
	}

	mBody = new TBossTelesaBody;
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(mBody);
	mBody->initHitActor(0x08000013, 5, 0xD1000000, 350.0f, 550.0f, 300.0f,
	                    500.0f);
	mBody->mOwner = this;
	mBody->offHitFlag(HIT_FLAG_NO_COLLISION);

	mTongue = new TBossTelesaTongue;
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(mTongue);
	mTongue->initHitActor(0x08000013, 5, 0xC0000000, 180.0f, 350.0f, 180.0f,
	                      350.0f);
	mTongue->mOwner = this;
	mTongue->offHitFlag(HIT_FLAG_NO_COLLISION);

	mKillSmallEnemy = new TBossTelesaKillSmallEnemy;
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(mKillSmallEnemy);
	mKillSmallEnemy->initHitActor(0x1000000C, 5, 0x10000000, 400.0f, 300.0f,
	                              400.0f, 300.0f);
	mKillSmallEnemy->offHitFlag(HIT_FLAG_NO_COLLISION);
	mKillSmallEnemy->mOwner = this;

	getMActor()->setLightType(LIGHT_TYPE_INDIRECT);

	TScreenTexture* screenTexture
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
	SMS_ChangeTextureAll(getMActor()->getModel()->getModelData(),
	                     "H_ma_rak_dummy",
	                     *screenTexture->getTexture()->getTexInfo());

	mMarioHP = SMS_GetMarioHP();
}

// TODO: 99.85%, every instruction but the prologue's `addi r31, r3, 0`
// (ours `mr`). The frame is 0x38 short of retail's 0x370, a uniform shift
// below the default-argument TVec3 blocks: a low-region deficit with no
// legal carrier found (a search binder per site would be a fabricated one).
void TBossTelesa::loadAfter()
{
	if (gpMapObjManager->getObjNumWithActorType(0x4000019A)) {
		int found = 0;
		for (int i = 0; i < gpMapObjManager->getObjNum(); ++i) {
			TMapObjBase* obj = (TMapObjBase*)gpMapObjManager->getObj(i);
			if (obj->isActorType(0x4000019A))
				mRoulettes[found++] = (TRoulette*)obj;
		}
	}

	if (gpMapObjManager->getObjNumWithActorType(0x400001A6)) {
		for (int i = 0; i < gpMapObjManager->getObjNum(); ++i) {
			TTelesaSlot* slot = (TTelesaSlot*)gpMapObjManager->getObj(i);
			if (slot->isActorType(0x400001A6)) {
				mSlot         = slot;
				mSlot->mOwner = this;
			}
		}
	}

	int fruit = 0;
	for (int i = 0; i < 6; ++i) {
		mFruits[fruit]
		    = TMapObjBaseManager::newAndRegisterObj("FruitCoconut");
		fruit++;
	}
	for (int i = 0; i < 6; ++i) {
		mFruits[fruit]
		    = TMapObjBaseManager::newAndRegisterObj("FruitPapaya");
		fruit++;
	}
	for (int i = 0; i < 2; ++i) {
		mFruits[fruit]
		    = TMapObjBaseManager::newAndRegisterObj("FruitPine");
		fruit++;
	}
	for (int i = 0; i < 6; ++i) {
		mFruits[fruit]
		    = TMapObjBaseManager::newAndRegisterObj("FruitDurian");
		fruit++;
	}

	for (int i = 0; i < 20; ++i) {
		mFruits[i]->unkF8 |= TMapObjBase::MAP_OBJ_FLAG_UNK4000000;
		mFruits[i]->makeObjDead();
	}

	mPeppers[0] = JDrama::TNameRefGen::search<TMapObjBase>("唐辛子 0");
	mPeppers[1] = JDrama::TNameRefGen::search<TMapObjBase>("唐辛子 1");
	mPeppers[2] = JDrama::TNameRefGen::search<TMapObjBase>("唐辛子 2");
	mPeppers[3] = JDrama::TNameRefGen::search<TMapObjBase>("唐辛子 3");
	mPeppers[4] = JDrama::TNameRefGen::search<TMapObjBase>("唐辛子 4");
	mPeppers[5] = JDrama::TNameRefGen::search<TMapObjBase>("唐辛子 5");
	mPeppers[6] = JDrama::TNameRefGen::search<TMapObjBase>("唐辛子 6");
	mPeppers[7] = JDrama::TNameRefGen::search<TMapObjBase>("唐辛子 7");
	mPeppers[8] = JDrama::TNameRefGen::search<TMapObjBase>("唐辛子 8");
	mPeppers[9] = JDrama::TNameRefGen::search<TMapObjBase>("唐辛子 9");

	for (int i = 0; i < 10; ++i)
		mPeppers[i]->makeObjDead();

	mCoins[0] = JDrama::TNameRefGen::search<TCoin>("コイン 0");
	mCoins[1] = JDrama::TNameRefGen::search<TCoin>("コイン 1");
	mCoins[2] = JDrama::TNameRefGen::search<TCoin>("コイン 2");
	mCoins[3] = JDrama::TNameRefGen::search<TCoin>("コイン 3");
	mCoins[4] = JDrama::TNameRefGen::search<TCoin>("コイン 4");
	mCoins[5] = JDrama::TNameRefGen::search<TCoin>("コイン 5");
	mCoins[6] = JDrama::TNameRefGen::search<TCoin>("コイン 6");
	mCoins[7] = JDrama::TNameRefGen::search<TCoin>("コイン 7");
	mCoins[8] = JDrama::TNameRefGen::search<TCoin>("コイン 8");
	mCoins[9] = JDrama::TNameRefGen::search<TCoin>("コイン 9");

	for (int i = 0; i < 10; ++i)
		mCoins[i]->makeObjDead();

	mSlot->mRollSp[0] = 0.0f;
	mSlot->mRollSp[1] = 0.0f;
	mSlot->mRollSp[2] = 0.0f;

	mTelesaManager
	    = JDrama::TNameRefGen::search<TTelesaManager>("テレサマネージャー");

	for (int i = 0; i < 5; ++i)
		TMapObjBaseManager::newAndRegisterObj("bottle_large");

	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_fhit.jpa",
	    BTELESA_JPA_MS_BTLS_FHIT);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_fhit_pe.jpa",
	    BTELESA_JPA_MS_BTLS_FHIT_PE);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_fhit_gr.jpa",
	    BTELESA_JPA_MS_BTLS_FHIT_GR);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_fhit_or.jpa",
	    BTELESA_JPA_MS_BTLS_FHIT_OR);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_damage.jpa",
	    BTELESA_JPA_MS_BTLS_DAMAGE);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_down.jpa",
	    BTELESA_JPA_MS_BTLS_DOWN);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_down_pe.jpa",
	    BTELESA_JPA_MS_BTLS_DOWN_PE);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_down_gr.jpa",
	    BTELESA_JPA_MS_BTLS_DOWN_GR);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_down_or.jpa",
	    BTELESA_JPA_MS_BTLS_DOWN_OR);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_spicy_hit.jpa",
	    BTELESA_JPA_MS_BTLS_SPICY_HIT);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_fubuki.jpa",
	    BTELESA_JPA_MS_BTLS_FUBUKI);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_yodare1.jpa",
	    BTELESA_JPA_MS_BTLS_YODARE1);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_yodare2.jpa",
	    BTELESA_JPA_MS_BTLS_YODARE2);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_yodare3.jpa",
	    BTELESA_JPA_MS_BTLS_YODARE3);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_ase.jpa",
	    BTELESA_JPA_MS_BTLS_ASE);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_spicy_a.jpa",
	    BTELESA_JPA_MS_BTLS_SPICY_A);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_spicy_b.jpa",
	    BTELESA_JPA_MS_BTLS_SPICY_B);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_spicy_d.jpa",
	    BTELESA_JPA_MS_BTLS_SPICY_D);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_chika_a.jpa",
	    BTELESA_JPA_MS_BTLS_CHIKA_A);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_chika_b.jpa",
	    BTELESA_JPA_MS_BTLS_CHIKA_B);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_glow.jpa",
	    BTELESA_JPA_MS_BTLS_GLOW);
	SMS_LoadParticle("/scene/btelesa/jpa/ms_btls_spicy_c.jpa",
	    BTELESA_JPA_MS_BTLS_SPICY_C);

	void* frameRes
	    = JKRFileLoader::getGlbResource("/scene/btelesa/srot_waku.bmd");
	SDLModelData* frameModel = new SDLModelData(
	    J3DModelLoaderDataBase::load(frameRes, 0x10220000));
	mSlotFrame = new TSharedParts(mSlot, 0, frameModel, 3);

	TLiveActor* firstGesso = JDrama::TNameRefGen::search<TLiveActor>("ゲッソー 0");
	if (firstGesso)
		firstGesso->onLiveFlag(LIVE_FLAG_DEAD);

	TLiveActor* secondGesso
	    = JDrama::TNameRefGen::search<TLiveActor>("ゲッソー 1");
	if (secondGesso)
		secondGesso->onLiveFlag(LIVE_FLAG_DEAD);

	JDrama::TNameRef::loadAfter();
}

void TBossTelesa::reset()
{
	TSpineEnemy::reset();

	onHitFlag(HIT_FLAG_NO_COLLISION);
	onLiveFlag(LIVE_FLAG_UNK8);
	onLiveFlag(LIVE_FLAG_UNK10);
	onLiveFlag(LIVE_FLAG_HIDDEN);
	unk18C = false;

	f32 attackRadius = getSaveParam2()->mSLAttackRadius.get();
	f32 attackHeight = mParams->mSLAttackHeight.get();
	f32 damageRadius = mParams->mSLDamageRadius.get();
	f32 damageHeight = mParams->mSLDamageHeight.get();
	setHitParams(attackRadius, attackHeight, damageRadius, damageHeight);

	SMSGetMarDirector()->fireStartDemoCamera("btelesa_roll_camera", nullptr,
	                                         -1, 0.0f, true, nullptr, 0,
	                                         nullptr, 0);
}

// Returning the camera's `Vec` as a TVec3 by value is what leaves the ROM's
// out-of-line TVec3::set(const Vec&) at both camera reads.
static inline JGeometry::TVec3<f32> BossTelesaGetCameraPos()
{
	return gpCamera->getUnk124Vec();
}

// TODO: 99.9%; every instruction matches but the frame is 0x40 short (0x190
// vs 0x1d0): retail puts operator-'s by-value copy of Mario's position in the
// low region (0xec/0xd0) instead of the named block beside the camera temps.
void TBossTelesa::moveObject()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	JGeometry::TVec3<f32> toCamera = SMS_GetMarioPos() - BossTelesaGetCameraPos();
	if (toCamera.length() < mCameraMoveLimit) {
		unk360 += mCameraMoveSp * (gpMarioPos->y - gpCamera->unk148.y);
		gpCamera->unk290 = unk360;
	} else if (fabsf(unk360) > 1.0f) {
		unk360           = unk360 * mCameraMoveSp;
		gpCamera->unk290 = unk360;
	}

	if (SMS_CheckMarioFlag(0x400))
		gpMSound->startSoundActor(MSD_SE_BS_TELESA_V_LAUGH1, &mPosition, 0,
		                          nullptr, 0, 4);

	if (mSpine->getCurrentNerve() == &TNerveBossTelesaFallDemo::theNerve()) {
		if (mRoulettes[0]->mPosition.y > 5.0f + mRoulettes[1]->mPosition.y) {
			mSlot->mPosition = mRoulettes[0]->mPosition;
			mSlot->mPosition.y += 300.0f;
		}
	}

	if (mSpine->getCurrentNerve()
	    != &TNerveBossTelesaPrepareSlot::theNerve()) {
		getMActor()->setFrameRate(0.0f, ANM_TYPE_BTP);
		getMActor()->getFrameCtrl(ANM_TYPE_BTP)->setFrame(0.0f);
	}

	if (mSpine->getCurrentNerve() == &TNerveBossTelesaDie::theNerve()) {
		u8 maxHitPoints = getMaxHitPoints();
		u8 alpha = MsClamp<u8>(
		    mNormalAlpha + (maxHitPoints - mHitPoints) * 30, 0, 254);

		if ((getMActor()->checkCurBckFromIndex(7)
		     && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() > 50.0f)
		    || getMActor()->checkCurBckFromIndex(6)) {
			if (unk34C.a > alpha)
				unk34C.a -= 1;
		} else {
			if (unk34C.a < 255)
				unk34C.a += 1;
		}
	}

	int spinning = 0;

	JGeometry::TVec3<f32> soundToCamera
	    = SMS_GetMarioPos() - BossTelesaGetCameraPos();

	mSoundPos = mRoulettes[0]->mPosition;
	mSoundPos.x += 0.67f * soundToCamera.x;
	mSoundPos.z += 0.67f * soundToCamera.z;

	for (int i = 0; i < 3; ++i) {
		if (mRoulettes[i]->unk13C != 0.0f)
			spinning += 1;
	}

	switch (spinning) {
	case 1:
		gpMSound->startSoundActor(MSD_SE_BS_TELESA_RLT_MOVE1, &mSoundPos, 0,
		                          nullptr, 0, 4);
		break;

	case 2:
		gpMSound->startSoundActor(MSD_SE_BS_TELESA_RLT_MOVE2, &mSoundPos, 0,
		                          nullptr, 0, 4);
		break;

	case 3:
		gpMSound->startSoundActor(MSD_SE_BS_TELESA_RLT_MOVE3, &mSoundPos, 0,
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

	mPosition.x += mLinearVelocity.x;
	mPosition.y += mLinearVelocity.y;
	mPosition.z += mLinearVelocity.z;
	mPosition.y = 300.0f + mGroundHeight;

	f32 blend = unk168 - 0.05f;
	if (blend > 1.0f)
		blend = 1.0f;
	else if (blend < 0.0f)
		blend = 0.0f;

	unk168 = blend;
	getMActor()->setMotionBlendRatioForBck(unk168);

	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		mBody->mPosition           = mPosition;
		mTongue->mPosition         = mPosition;
		mKillSmallEnemy->mPosition = mPosition;
	} else {
		MtxPtr headMtx = getMActor()->getModel()->getAnmMtx(1);

		mBody->mPosition.set(headMtx[0][3], headMtx[1][3] - 200.0f,
		                     headMtx[2][3]);
		mKillSmallEnemy->mPosition.set(headMtx[0][3], mPosition.y - 350.0f,
		                               headMtx[2][3]);

		MtxPtr tongueMtx = getMActor()->getModel()->getAnmMtx(7);
		mTongue->mPosition.set(tongueMtx[0][3], tongueMtx[1][3] - 350.0f,
		                       tongueMtx[2][3]);
	}

	mBody->checkHit();
	mTongue->checkHit();
	mKillSmallEnemy->checkHit();
}

void TBossTelesa::kill()
{
	if (mSpine->getCurrentNerve() != &TNerveBossTelesaDie::theNerve())
		mSpine->pushNerve(&TNerveBossTelesaDie::theNerve());
}

MtxPtr TBossTelesa::getTakingMtx()
{
	unk278.set(mRoulettes[0]->getMActor()->getModel()->getAnmMtx(1));
	unk278.ref(1, 3) = mRoulettes[0]->mPosition.y - 120.0f;
	return unk278;
}

// TODO: incorrect size. Map records 472 bytes. Only the table is recovered:
// xzTable$3483 has no reference anywhere in the ROM's .text, which is what a
// dead function's static looks like, and the literal numbering puts it in this
// part of the file.
void TBossTelesa::prepareGenerate()
{
	static const f32 xzTable[8]
	    = { 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f };
}

void TBossTelesa::calcRootMatrix()
{
	mMActor->getModel()->setBaseScale(mScaling);

	TPosition3f mtx;
	Mtx rotation;

	// TODO: retail hoists mSLTransYOffset.get() into f3 before mBaseHoseiPosY
	// (same delay slot as the z load). Named getSaveParam2() recovered the
	// params load and took 98.1 -> 99.1; frame is still 8 short of 0x2a8.
	TBossTelesaSaveLoadParams* params = getSaveParam2();
	mtx.translation(mPosition.x,
	                mPosition.y + params->mSLTransYOffset.get()
	                    + MsClamp<f32>(unk364, mBaseHoseiPosY, 0.0f),
	                mPosition.z);
	MsMtxSetRotRPH(rotation, mRotation.x, mRotation.y, mRotation.z);
	MTXConcat(mtx, rotation, mtx);
	MTXCopy(mtx, mMActor->getModel()->getBaseTRMtx());

	// The slot machine hangs off whichever joint the current animation uses.
	if (mSlot) {
		mSlot->mRotation = mRotation;

		f32 offsetY = -700.0f;
		MtxPtr jointMtx;

		if (getMActor()->checkCurBckFromIndex(4)
		    || getMActor()->checkCurBckFromIndex(0)
		    || getMActor()->checkCurBckFromIndex(2)
		    || getMActor()->checkCurBckFromIndex(5)) {
			jointMtx = getMActor()->getModel()->getAnmMtx(1);
			if (!getMActor()->checkCurBckFromIndex(0))
				offsetY = -2400.0f;
		} else {
			jointMtx = getMActor()->getModel()->getAnmMtx(0);
		}

		mSlot->mPosition.x = jointMtx[0][3];
		mSlot->mPosition.y = unk364 + (jointMtx[1][3] + offsetY);
		mSlot->mPosition.z = jointMtx[2][3];
	}

	for (u16 i = 0;
	     i < getMActor()->getModel()->getModelData()->getMaterialNum(); ++i) {
		Mtx44 effectMtx;
		SMS_GetLightPerspectiveForEffectMtx(effectMtx);
		getMActor()
		    ->getModel()
		    ->getModelData()
		    ->getMaterialNodePointer(i)
		    ->getTexMtx(1)
		    ->setEffectMtx(effectMtx);
	}

	unk374.set(0.0f, 0.0f, 0.0f);
	gpMarioParticleManager->emit(BTELESA_JPA_MS_BTLS_CHIKA_A, &unk374, 1, this);
	gpMarioParticleManager->emit(BTELESA_JPA_MS_BTLS_CHIKA_B, &unk374, 1, this);

	MtxPtr headMtx = getMActor()->getModel()->getAnmMtx(1);
	unk374.set(headMtx[0][3], headMtx[1][3], headMtx[2][3]);
	gpMarioParticleManager->emitAndBindToPosPtr(BTELESA_JPA_MS_BTLS_GLOW,
	    &unk374, 1, this);

	if (!getMActor()->checkCurBckFromIndex(4)
	    && !getMActor()->checkCurBckFromIndex(6)
	    && !getMActor()->checkCurBckFromIndex(12)
	    && !getMActor()->checkCurBckFromIndex(13)
	    && !getMActor()->checkCurBckFromIndex(3)) {
		gpMarioParticleManager->emitAndBindToMtxPtr(BTELESA_JPA_MS_BTLS_YODARE1,
		    getMActor()->getModel()->getAnmMtx(5), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(BTELESA_JPA_MS_BTLS_YODARE2,
		    getMActor()->getModel()->getAnmMtx(5), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(BTELESA_JPA_MS_BTLS_YODARE3,
		    getMActor()->getModel()->getAnmMtx(10), 1, this);
	}

	if (getMActor()->checkCurBckFromIndex(1)
	    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() < 20.0f) {
		gpMarioParticleManager->emitAndBindToMtxPtr(BTELESA_JPA_MS_BTLS_SPICY_HIT,
		    getMActor()->getModel()->getAnmMtx(5), 0, nullptr);
	}

	if (getMActor()->checkCurBckFromIndex(12)) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_FIRE, &mPosition, 0,
		                                nullptr, 0, 4);

		gpMarioParticleManager->emitAndBindToMtxPtr(BTELESA_JPA_MS_BTLS_ASE,
		    getMActor()->getModel()->getAnmMtx(1), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(BTELESA_JPA_MS_BTLS_SPICY_A,
		    getMActor()->getModel()->getAnmMtx(9), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(BTELESA_JPA_MS_BTLS_SPICY_B,
		    getMActor()->getModel()->getAnmMtx(9), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(BTELESA_JPA_MS_BTLS_SPICY_D,
		    getMActor()->getModel()->getAnmMtx(9), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(BTELESA_JPA_MS_BTLS_SPICY_C,
		    getMActor()->getModel()->getAnmMtx(9), 3, this);
	}

	if (getMActor()->checkCurBckFromIndex(14)
	    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(40.0f)) {
		if (mSpine->getCurrentNerve()
		    == &TNerveBossTelesaSpitSlotItem::theNerve()) {
			generateSlotItem();
			return;
		}

		genAttacker();
	}
}

void TBossTelesa::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_ENTRY
	    && !checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_CLIPPED_OUT)) {
		if (mSpine->getCurrentNerve() == &TNerveBossTelesaDie::theNerve()
		    && unk350) {
			getMActor()->offMakeDL();
			SMS_AddDamageFogEffect(getMActor()->getModel()->getModelData(),
			                       mPosition, graphics);
		}
	}

	offLiveFlag(LIVE_FLAG_CLIPPED_OUT);

	TSpineEnemy::perform(cue, graphics);
	mBody->THitActor::perform(cue, graphics);
	mTongue->THitActor::perform(cue, graphics);
	mKillSmallEnemy->THitActor::perform(cue, graphics);

	if (mSlot)
		mSlot->testPerform(cue, graphics);
	if (mSlotFrame)
		mSlotFrame->testPerform(cue, graphics);
}

BOOL TBossTelesa::receiveMessage(THitActor* sender, u32 message)
{
	return false;
}


// TODO: incorrect size. Map records 588 bytes.
// TODO: incomplete, 0x24c in the map. Only the part the body's collision
// forwards here is reconstructed: the two nerve pushes, whose theNerve()
// guards retail reaches one inline level below TBossTelesaBody::receiveMessage
// (the `bl TNerveBase<TLiveActor>::TNerveBase()` at both push sites).
BOOL TBossTelesa::checkMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_TRAMPLE) {
		if (!sender->isActorType(0x80000001)) {
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

	return false;
}

static inline TGCConsole2* BossTelesaHitGetConsole()
{
	TGCConsole2* console = gpMarDirector->getConsole();
	return console;
}

void TBossTelesa::checkHitObject(THitActor* actor)
{
	unk380 = -1;

	if ((actor->getActorType() & ACTOR_TYPE_MASK) != ACTOR_TYPE_UNK40000000)
		return;

	if (mSpine->getCurrentNerve() != &TNerveBossTelesaPrepareSlot::theNerve())
		return;

	switch (actor->getActorType()) {
	case 0x40000390: // FruitCoconut
		unk348.r = 0xE6;
		unk348.g = 0x64;
		unk348.b = 0xB4;
		unk380   = BTELESA_JPA_MS_BTLS_FHIT_PE;
		kill();
		break;

	case 0x40000391: // FruitPapaya
	case 0x40000392: // FruitPine
		unk348.r = 0xE6;
		unk348.g = 0xB4;
		unk348.b = 0;
		unk380   = BTELESA_JPA_MS_BTLS_FHIT_OR;
		kill();
		break;

	case 0x40000393: // FruitDurian
		unk348.r = 0x96;
		unk348.g = 0x32;
		unk348.b = 0xE6;
		unk380   = BTELESA_JPA_MS_BTLS_FHIT_GR;
		kill();
		break;

	case 0x40000395: // RedPepper -- swallowed without taking damage
		break;

	default:
		return;
	}

	if (!unk350 && actor->getActorType() != 0x40000395) {
		if (unk35A) {
			unk35A = false;
			BossTelesaHitGetConsole()->startAppearBalloon(0xF, true);
		}

		unk35C += 1;
		if (unk35C > 2)
			BossTelesaHitGetConsole()->startAppearBalloon(0x10, true);
	} else {
		unk35C = 0;
	}

	unk374 = actor->mPosition;
	gpMarioParticleManager->emit(BTELESA_JPA_MS_BTLS_FHIT, &unk374, 0, nullptr);

	if (unk380 >= 0)
		gpMarioParticleManager->emit(unk380, &unk374, 0, nullptr);

	if (unk350)
		gpMarioParticleManager->emit(BTELESA_JPA_MS_BTLS_DAMAGE, &unk374, 0,
		    nullptr);
	else
		gpMSound->startSoundActor(MSD_SE_BS_TELESA_FRUIT_HIT, &mPosition, 0,
		                          nullptr, 0, 4);

	TMapObjBase* fruit = (TMapObjBase*)actor;
	fruit->makeObjDead();
}

// Binding level worth +16 of low region, landing TBossTelesa::setSpicy's
// frame at 0x58 (batch 121).
static inline TGCConsole2* BosstelesaGetConsoleInner(TMarDirector* p)
{
	TGCConsole2* console = p->getConsole();
	return console;
}

static inline TGCConsole2* BosstelesaGetConsole(TMarDirector* p)
{
	TGCConsole2* console = BosstelesaGetConsoleInner(p);
	return console;
}

void TBossTelesa::setSpicy(TLiveActor* actor)
{
	if (mSpine->getCurrentNerve() != &TNerveBossTelesaSpitSlotItem::theNerve()
	    && !getMActor()->checkCurBckFromIndex(1)) {
		unk350 = true;
		unk36C = 0;
		setBckAnm(1);

		if (unk35B) {
			unk35B = false;
			BosstelesaGetConsole(gpMarDirector)->startAppearBalloon(0x11, true);
		}

		actor->kill();
	}
}

/// Binds an actor's position before it is reset to the origin.
static inline JGeometry::TVec3<f32>* bindPosition(TLiveActor* actor)
{
	JGeometry::TVec3<f32>* pos = &actor->mPosition;
	return pos;
}

void TBossTelesa::damageRecover()
{
	for (int i = 0; i < 20; ++i) {
		if (!mFruits[i]->checkLiveFlag(LIVE_FLAG_DEAD)) {
			if (mFruits[i]->mHolder == nullptr)
				SMS_SendMessageToMario(mFruits[i], HIT_MESSAGE_UNK8);

			mFruits[i]->makeObjDead();
			gpMarioParticleManager->emit(PARTICLE_MS_TLS_CHANGE,
			                             &mFruits[i]->mPosition, 0, nullptr);
			JGeometry::TVec3<f32> origin;
			origin.set(0.0f, 0.0f, 0.0f);
			mFruits[i]->mPosition.set(origin);
		}
	}

	for (int i = 0; i < 10; ++i) {
		if (!mPeppers[i]->checkLiveFlag(LIVE_FLAG_DEAD)) {
			if (mPeppers[i]->mHolder == nullptr)
				SMS_SendMessageToMario(mPeppers[i], HIT_MESSAGE_UNK8);

			mPeppers[i]->makeObjDead();
			gpMarioParticleManager->emit(PARTICLE_MS_TLS_CHANGE,
			                             &mPeppers[i]->mPosition, 0, nullptr);
			bindPosition(mPeppers[i])->set(0.0f, 0.0f, 0.0f);
		}

		if (!mCoins[i]->checkLiveFlag(LIVE_FLAG_DEAD)) {
			mCoins[i]->makeObjDead();
			gpMarioParticleManager->emit(PARTICLE_MS_TLS_CHANGE,
			                             &mCoins[i]->mPosition, 0, nullptr);
			bindPosition(mCoins[i])->set(0.0f, 0.0f, 0.0f);
		}
	}

	if (unk350)
		gpMSound->startSoundActor(MSD_SE_BS_TELESA_ESCAPE, &mPosition, 0,
		                          nullptr, 0, 4);
	else
		gpMSound->startSoundActor(MSD_SE_BS_TELESA_DISAPPEAR, &mPosition, 0,
		                          nullptr, 0, 4);

	mSpine->pushAfterCurrent(&TNerveBossTelesaHide::theNerve());
	unk368 = 0;
}

// TODO: incorrect size. Map records 284 bytes.
// TODO: incomplete, 0x11c in the map. Only the nerve push the tongue's spray
// message forwards here is reconstructed; retail reaches TNerveBase<
// TLiveActor>'s constructor one inline level below TBossTelesaTongue::
// receiveMessage because of this call.
void TBossTelesa::tongueHitWater()
{
	if (mSpine->getCurrentNerve() == &TNerveBossTelesaAppear::theNerve())
		mSpine->pushNerve(&TNerveBossTelesaSlotStart::theNerve());
}

static inline MSound* BossTelesaRouletteFallGetMSound()
{
	MSound* sound = gpMSound;
	return sound;
}

bool TBossTelesa::rouletteFall()
{
	f32 y      = mRoulettes[0]->mPosition.y;
	f32 restY  = mRoulettes[1]->mPosition.y;

	if (y > restY) {
		mRoulettes[0]->mPosition.y = y - 2.0f;
		mRoulettes[0]->getMActor()->setBck("rulet00");

		if (mRoulettes[0]->mPosition.y > 3.0f + mRoulettes[1]->mPosition.y) {
			if (SMS_SendMessageToMario(this, HIT_MESSAGE_TAKE))
				mHeldObject = (TTakeActor*)SMS_GetMarioHitActor();
		} else if (SMS_SendMessageToMario(this, HIT_MESSAGE_UNK8)) {
			BossTelesaRouletteFallGetMSound()->startSoundActor(MSD_SE_BS_TELESA_RLT_SET, &mPosition, 0,
			                          nullptr, 0, 4);
			mHeldObject = nullptr;
		}

		BossTelesaRouletteFallGetMSound()->startSoundActor(MSD_SE_BS_TELESA_V_LAUGH1B, &mPosition, 0,
		                          nullptr, 0, 4);
		BossTelesaRouletteFallGetMSound()->startSoundActor(MSD_SE_BS_TELESA_RLT_DOWN, &mPosition, 0,
		                          nullptr, 0, 4);
		gpMarioOriginal->mGamePad->onNeutralMarioKey();

		return false;
	}

	mRoulettes[0]->mPosition.y = restY;

	return true;
}

bool TBossTelesa::slotFall()
{
	TLiveActor* slot = mSlot;
	f32 y            = slot->mPosition.y;
	if (y > mRoulettes[0]->mPosition.y - 800.0f) {
		mSlot->mPosition.y = y - 5.0f;
		return false;
	}

	mSlot->mPosition.y = y - 1.0f;

	if (mSlot->mPosition.y < mRoulettes[0]->mPosition.y - 900.0f) {
		bool restart = isForceRestart();
		if (restart)
			rouletteStart();
	}

	if (mSlot->mPosition.y < mRoulettes[0]->mPosition.y - 1100.0f)
		return true;

	TRouletteSw* sw0 = mRoulettes[0]->unk150;
	sw0->setHitParams(280.0f, 100.0f, 280.0f, 100.0f);
	TRouletteSw* sw1 = mRoulettes[1]->unk150;
	sw1->setHitParams(280.0f, 100.0f, 280.0f, 100.0f);

	return false;
}

// TODO: incorrect size. Map records 44 bytes.
void TBossTelesa::openWaterPlace() { }

// TODO: the unrolled loop's counter takes r8 in retail (r6 here), and the
// coin's flag address is still missing. Inert: s32 phase, i++, a coin flag
// reference, flags read through the reference, a shared TU-local helper.
void TBossTelesa::flashItem(int timer)
{
	int phase = timer % 16;

	for (int i = 0; i < 20; ++i) {
		// The reference is what the ROM's `addi rN, obj, 0xf0` before the
		// dead test comes from; the accessors keep the base+offset form.
		TMapObjBase* fruit = mFruits[i];
		u32& fruitFlags    = fruit->mLiveFlag;
		if (!fruit->checkLiveFlag(LIVE_FLAG_DEAD)
		    && fruit->mHolder == nullptr) {
			if (phase < 8)
				fruitFlags |= LIVE_FLAG_HIDDEN;
			else
				fruitFlags &= ~LIVE_FLAG_HIDDEN;
		}
	}

	for (int i = 0; i < 10; ++i) {
		TMapObjBase* pepper = mPeppers[i];
		u32& pepperFlags    = pepper->mLiveFlag;
		if (!pepper->checkLiveFlag(LIVE_FLAG_DEAD)
		    && pepper->mHolder == nullptr) {
			if (phase < 8)
				pepperFlags |= LIVE_FLAG_HIDDEN;
			else
				pepperFlags &= ~LIVE_FLAG_HIDDEN;
		}

		// TODO: the ROM computes &coin->mLiveFlag here as well, but binding a
		// reference the way the fruit and pepper loops do makes MWCC fuse the
		// address into an `lwzu` and costs more than the two instructions.
		TCoin* coin = mCoins[i];
		if (!coin->checkLiveFlag(LIVE_FLAG_DEAD)) {
			if (phase < 8)
				coin->onLiveFlag(LIVE_FLAG_HIDDEN);
			else
				coin->offLiveFlag(LIVE_FLAG_HIDDEN);
		}
	}
}

void TBossTelesa::onAllCollision()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mBody->onHitFlag(HIT_FLAG_NO_COLLISION);
	mTongue->onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBossTelesa::offAllCollision()
{
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mBody->offHitFlag(HIT_FLAG_NO_COLLISION);
	mTongue->offHitFlag(HIT_FLAG_NO_COLLISION);
}

const char** TBossTelesa::getBasNameTable() const { return btelesa_bastable; }

// The ROM's frame needs one inline level at the first makeOneEnemyAppear and
// none at the second: the binder is +8 there and +0x10 at both sites.
static inline TConductor* BossTelesaGenAttackerConductor()
{
	TConductor* conductor = gpConductor;
	return conductor;
}

// TODO: every instruction matches; `chance` sits at 0x54 instead of 0x58, so
// some 4-byte named slot above it is still missing (speed, item, bubble and
// declaration-order moves are inert).
void TBossTelesa::genAttacker()
{
	if (unk150) {
		TTelesa* telesa = (TTelesa*)BossTelesaGenAttackerConductor()->makeOneEnemyAppear(
		    mPosition, "テレサマネージャー", 1);
		if (telesa)
			telesa->initAttacker(this);

		return;
	}

	JGeometry::TVec3<f32> velocity;
	Mtx mtx;
	MtxPtr mouthMtx = getMActor()->getModel()->getAnmMtx(5);
	int num         = mParams->mSLNumGenBubble.get();
	f32 step        = 180.0f / (f32)num;
	f32 halfSpread  = step * (f32)num / 2.0f;

	for (int i = 0; i < num; ++i) {
		TBubble* bubble = (TBubble*)gpConductor->makeOneEnemyAppear(
		    mPosition, "バブルマネージャー", 1);
		if (!bubble)
			return;

		velocity.set(0.0f, 0.0f, -50.0f);
		MsMtxSetRotRPH(mtx, mRotation.x,
		               (step * (f32)i) + (mRotation.y - halfSpread),
		               mRotation.z);
		MTXMultVec(mtx, velocity, velocity);
		MsVECNormalize(velocity, velocity);
		velocity.y = 2.0f;

		f32 speed = mParams->mSL1stBubbleSp.get();
		velocity.x *= speed;
		velocity.z *= speed;

		bubble->mPosition.set(mouthMtx[0][3], mouthMtx[1][3] - 50.0f,
		                      mouthMtx[2][3]);
		bubble->setVelocity(velocity);
		bubble->mPosition.y += 10.0f;
		bubble->onLiveFlag(LIVE_FLAG_AIRBORNE);

		TMsRange<f32> chance(0.0f, 1.0f);
		if (chance.rand() < mItemGenRate) {
			bubble->mEnemyInside = nullptr;

			TItem* item = (TItem*)gpItemManager->makeObjAppear(
			    bubble->mPosition.x, bubble->mPosition.y, bubble->mPosition.z,
			    0x20000008, true);
			if (item && item->receiveMessage(bubble, HIT_MESSAGE_TAKE)) {
				item->appear();
				item->mPosition = bubble->mPosition;
				item->mVelocity.set(0.0f, 15.0f, 0.0f);
				item->offLiveFlag(LIVE_FLAG_UNK10);
				bubble->mHeldObject  = item;
				bubble->mEnemyInside = item;
			}
		} else if (chance.rand() < mEnemyGenRate) {
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
	getMActor()->setMotionBlendRatioForBck(unk168);

	const char** table = getBasNameTable();
	setAnmSound(table == nullptr ? nullptr : table[index]);
}

// TODO: incorrect size. Map records 140 bytes.
bool TBossTelesa::isInDamage() { return false; }

static inline RumbleMgr* BossTelesaRouletteGetRumbleMgr()
{
	RumbleMgr* rumble = SMSRumbleMgr;
	return rumble;
}

static inline TCameraShake* BossTelesaRouletteGetCameraShake()
{
	TCameraShake* shake = gpCameraShake;
	return shake;
}

// TODO: both TMsRange locals sit 4 low (0x4c/0x44, retail 0x50/0x48) and
// retail keeps speedUp in f26, sign in f27 (ours swapped). Inert (co2): sign
// declared before the loop, speedUp declared first.
void TBossTelesa::rouletteStart()
{
	// A real ROM bug: the count is never used, so only the three speed loads
	// and compares survive.
	int spinning = 0;
	for (int i = 0; i < 3; ++i) {
		if (mRoulettes[i]->unk13C != 0.0f)
			spinning += 1;
	}

	TMsRange<f32> speedRange(0.05f, 0.1f);
	TMsRange<f32> directionRange(-1.0f, 1.0f);

	f32 direction = directionRange.rand();
	// TODO: the u8 local is what puts the ROM's `clrlwi` after the merge of
	// getMaxHitPoints()' two arms; TSpineEnemy::getMaxHitPoints() probably
	// returned u8 rather than u32 (open shared-header fix in Enemy.hpp).
	u8 maxHitPoints = getMaxHitPoints();
	f32 speedUp     = mRouletteUpRate * (f32)(maxHitPoints - mHitPoints);

	for (int i = 0; i < 3; ++i) {
		f32 sign;
		if (direction > 0.0f) {
			sign = -1.0f;
			if (i == 0 || i == 2)
				sign = 1.0f;
		} else {
			sign = 1.0f;
			if (i == 0 || i == 2)
				sign = -1.0f;
		}

		mRoulettes[i]->unk144 = sign * (speedUp + speedRange.rand());
		mSlot->mRollSp[i]     = sign * (speedUp + speedRange.rand());
	}

	rollRouletteCircle();

	BossTelesaRouletteGetRumbleMgr()->start(0x14, 0xF, (f32*)nullptr);
	// mCamShakeNameSave[0x23] is "/Camera/shakeBTelesaRoll.prm".
	BossTelesaRouletteGetCameraShake()->startShake((EnumCamShakeMode)0x23,
	                                               1.0f);
}

void TBossTelesa::slotStart()
{
	unk18C = true;
	mSlot->moveStart();
	mTelesaManager->telesaForceKill();
}

bool TBossTelesa::slotStop()
{
	if (!mSlot->isRollDrum() && unk18C) {
		unk18C = false;
		mSpine->pushAfterCurrent(&TNerveBossTelesaSpitSlotItem::theNerve());
		return true;
	}

	return false;
}

bool TBossTelesa::checkSlotResult()
{
	if (mSlot->getSlotResult() == 0)
		return true;

	return false;
}

// TODO: frame 0x2a8 vs 0x2c0 and one callee-saved FPR short (retail saves
// f16: the i == 0 pepper block takes f17/f16 for its rand ranges, ours reuses
// f18/f17). Both fruit-count clamps load into r0 and `mr` into the saved
// register; retail loads straight into it. The retail `li r3, 1` on both arms of
// the lastManager choice matches with a `firstManager = 1` set in each arm,
// which the source has no reason for, so it is left out.
void TBossTelesa::generateSlotItem()
{
	static const char* manNameTable[] = {
		"バブルマネージャー",     "ハムクリマネージャー",
		"ヤキグリマネージャー",   "ボム兵マネージャー",
		"ポイハナマネージャー",   "電気ノコノコマネージャー",
		"ポポマネージャー",       "ゲッソーマネージャー",
		"とびプクマネージャー",
	};

	unk368       = 0;
	mSlotItemNum = 0;
	unk1A8       = mSlot->getSlotResult();

	int itemNum     = mParams->mSLSlotItemNum.get();
	int result      = unk1A8;
	MtxPtr mouthMtx = getMActor()->getModel()->getAnmMtx(5);
	f32 step        = 120.0f / (f32)itemNum;
	f32 halfSpread  = step * (f32)itemNum / 2.0f;

	if (result == 2) {
		int num = mParams->mSLSlotFruitNum.get();
		if (num > 20)
			num = 20;

		TMsRange<s32> startRange(0, num);
		int slot = startRange.rand();

		for (int i = 0; i < num; ++i) {
			if (mFruits[i]->mHolder != nullptr)
				continue;

			JGeometry::TVec3<f32> velocity(0.0f, 0.0f, 200.0f);
			Mtx mtx;
			MsMtxSetRotRPH(mtx, mRotation.x,
			               ((160.0f / (f32)num) * (f32)slot)
			                   + (mRotation.y - halfSpread),
			               mRotation.z);

			slot = MsWrap(slot + i, 0, num);

			MTXMultVec(mtx, velocity, velocity);

			JGeometry::TVec3<f32> direction;
			MsVECNormalize(velocity, direction);

			TMsRange<f32> speedRange(6.0f, 10.0f);

			if (i == 0 || i == 4) {
				mPeppers[i]->makeObjAppeared();
				mPeppers[i]->offLiveFlag(LIVE_FLAG_HIDDEN);

				f32 speedZ = direction.z * speedRange.rand();
				f32 speedX = direction.x * speedRange.rand();
				TMapObjBase* pepper = mPeppers[i];
				pepper->mVelocity.set(speedX, -2.0f, speedZ);
				pepper->offLiveFlag(LIVE_FLAG_UNK10);

				if (i == 0) {
					f32 fastZ = 2.0f * (direction.z * speedRange.rand());
					f32 fastX = 2.0f * (direction.x * speedRange.rand());
					TMapObjBase* pepper = mPeppers[i];
					pepper->mVelocity.set(fastX, -2.0f, fastZ);
					pepper->offLiveFlag(LIVE_FLAG_UNK10);
				}

				mPeppers[i]->mRotation.set(0.0f, 90.0f, 0.0f);
				mSlotItems[mSlotItemNum] = mPeppers[i];
			} else {
				mFruits[i]->makeObjAppeared();
				mFruits[i]->offLiveFlag(LIVE_FLAG_HIDDEN);

				f32 speedZ = direction.z * speedRange.rand();
				f32 speedX = direction.x * speedRange.rand();
				TMapObjBase* fruit = mFruits[i];
				fruit->mVelocity.set(speedX, -2.0f, speedZ);
				fruit->offLiveFlag(LIVE_FLAG_UNK10);

				mSlotItems[mSlotItemNum] = mFruits[i];
			}

			mSlotItems[i]->onHitFlag(HIT_FLAG_NO_COLLISION);
			mSlotItems[i]->mScaling.set(1.5f, 1.5f, 1.5f);
			mSlotItems[mSlotItemNum]->mPosition.set(
			    mouthMtx[0][3] + velocity.x, mouthMtx[1][3] - 50.0f,
			    mouthMtx[2][3] + velocity.z);

			mSlotItemNum += 1;
		}

		return;
	}

	if (result == 0) {
		int num = mParams->mSLSlotFruitNum.get();
		if (num > 10)
			num = 10;

		f32 coinStep       = 120.0f / (f32)num;
		f32 coinHalfSpread = coinStep * (f32)num / 2.0f;

		if (unk370)
			unk370 -= 1;
		else
			unk370 = 0;

		for (int i = 0; i < num; ++i) {
			if (i >= 10)
				return;

			JGeometry::TVec3<f32> velocity(0.0f, 0.0f, 250.0f);
			Mtx mtx;
			MsMtxSetRotRPH(mtx, mRotation.x,
			               (coinStep * (f32)i)
			                   + (mRotation.y - coinHalfSpread),
			               mRotation.z);
			MTXMultVec(mtx, velocity, velocity);
			MsVECNormalize(velocity, velocity);

			TMsRange<f32> speedRange(0.8f, 3.5f);
			velocity.y = 10.0f;

			f32 baseSpeed = mParams->mSL1stBubbleSp.get();
			velocity.x    = velocity.x * (baseSpeed * speedRange.rand());
			velocity.z *= baseSpeed * speedRange.rand();

			TMapObjBase* coin = gpItemManager->makeObjAppeared(0x2000000E);
			coin->mPosition.set(mouthMtx[0][3], mouthMtx[1][3] - 250.0f,
			                    mouthMtx[2][3]);
			coin->mVelocity.set(velocity.x, velocity.y, velocity.z);
			coin->offLiveFlag(LIVE_FLAG_UNK10);
			coin->mRotation.set(0.0f, 0.0f, 0.0f);
			((TItem*)coin)->killByTimer(960);

			mSlotItems[mSlotItemNum] = mCoins[i];
			mSlotItems[mSlotItemNum]->offLiveFlag(LIVE_FLAG_HIDDEN);
			mSlotItemNum += 1;
		}

		return;
	}

	int count        = itemNum;
	int managerIndex = 0;

	switch (result) {
	case 3:
		break;

	case -1:
		count *= 2;
		managerIndex = 0;
		break;

	case 1:
		if (mHitPoints > 2)
			managerIndex = 1;
		else
			managerIndex = 2;
		break;
	}

	int lastManager = 7;
	if (mHitPoints == 1)
		lastManager = 8;

	TMsRange<s32> managerRange(1, lastManager);
	int manager = managerRange.rand();

	for (int i = 0; i < count; ++i) {
		if (unk1A8 == 3) {
			if (i % 2 == 0)
				manager += 1;
			if (manager > lastManager)
				manager = 1;

			managerIndex = manager;
		}

		TSpineEnemy* enemy = gpConductor->makeOneEnemyAppear(
		    mPosition, manNameTable[managerIndex], 2);
		if (!enemy)
			continue;

		if (managerIndex != 0) {
			mSlotItems[mSlotItemNum] = enemy;
			mSlotItemNum += 1;
		}

		JGeometry::TVec3<f32> velocity(0.0f, 0.0f, 200.0f);
		Mtx mtx;
		MsMtxSetRotRPH(mtx, mRotation.x,
		               (step * (f32)i) + (mRotation.y - halfSpread),
		               mRotation.z);
		MTXMultVec(mtx, velocity, velocity);
		MsVECNormalize(velocity, velocity);
		velocity.y = 2.0f;

		f32 baseSpeed = mParams->mSL1stBubbleSp.get();
		TMsRange<f32> speedRange(0.5f, 1.0f);
		velocity.x = velocity.x * (baseSpeed * speedRange.rand());
		velocity.y = velocity.y * (2.0f + speedRange.rand());
		velocity.z *= baseSpeed * speedRange.rand();

		enemy->mPosition.set(mouthMtx[0][3], mouthMtx[1][3] - 250.0f,
		                     mouthMtx[2][3]);
		enemy->mVelocity = velocity;
		enemy->mPosition.y += 10.0f;
		enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);

		MTXCopy(mouthMtx, enemy->getMActor()->getModel()->getBaseTRMtx());
		enemy->getMActor()->calc();
		((TWalkerEnemy*)enemy)->initAttacker(this);
	}
}

// TODO: incorrect size. Map records 136 bytes.
void TBossTelesa::fruitCollisionOn() { }

// TODO: incorrect size. Map records 212 bytes.
void TBossTelesa::checkSlot() { }

bool TBossTelesa::checkAllItemDead()
{
	if (unk1A8 == -1)
		return true;

	for (int i = 0; i < mSlotItemNum; ++i) {
		if (!mSlotItems[i]->checkLiveFlag(LIVE_FLAG_DEAD))
			return false;
	}

	return true;
}

// The ROM calls this out of line at all three sites, all at depth 1, so its
// source cost fifteen statements; the loop alone is twelve. The three names
// below are the missing three: each is an intermediate the function computes
// anyway, so all three are zero codegen (the body stays instruction-exact
// against the ROM, only the 0x10 frame residue is left), and together they
// take forceHide to 99.9% and the Die and PrepareSlot nerves from 90.0/86.5
// to 98.7/98.4. `slot` is the address the ROM binds in r30 and reloads
// through at every use; `zero` is the callee-saved f31 the ROM holds the
// literal in across the loop.
// TODO: +8 frame (0x38 vs 0x40), instruction-exact. A holder binder is
// +0x10; a particle-manager binder lands the frame but swaps r28/r30.
void TBossTelesa::forceAllItemKill()
{
	f32 zero = 0.0f;

	for (int i = 0; i < mSlotItemNum; ++i) {
		TLiveActor** slot = &mSlotItems[i];

		THitActor* holder = (*slot)->getHolder();
		if (holder != nullptr) {
			SMS_SendMessageToMario(*slot, HIT_MESSAGE_UNK8);
			(*slot)->mHolder = nullptr;
		}

		(*slot)->mPosition.set(zero, zero, zero);
		(*slot)->onHitFlag(HIT_FLAG_NO_COLLISION);

		if (!(*slot)->checkLiveFlag(LIVE_FLAG_DEAD)) {
			(*slot)->kill();
			gpMarioParticleManager->emit(PARTICLE_MS_TLS_CHANGE,
			                             &(*slot)->mPosition, 0, nullptr);
		}
	}
}

void TBossTelesa::rollRouletteCircle()
{
	for (int i = 0; i < 3; ++i)
		mRoulettes[i]->setRollSp(mSlot->mRollSp[i]);
}

bool TBossTelesa::isForceRestart()
{
	int spinning = 0;

	for (int i = 0; i < 3; ++i) {
		if (mRoulettes[i]->unk13C != 0.0f)
			spinning += 1;
	}

	return spinning != 3;
}

// The ROM calls this out of line from TBossTelesaKillSmallEnemy::checkHit, at
// depth 1, so its source cost fifteen statements; the three early returns are
// twelve of them (a bare `return;` costs one, unlike `return <local>;`) and
// naming `checkCurBckFromIndex`'s first result is the fifteenth -- it is zero
// codegen because the BOOL dies into the branch, where naming the nerve
// instead hoists the spine load above the theNerve() guard and costs four
// instructions. The two `startSoundActor` calls use the two-argument overload
// for the 0x10 of low region the 0x48 frame needs (+8 per expansion).
void TBossTelesa::forceHide()
{
	if (mSpine->getCurrentNerve() == &TNerveBossTelesaDie::theNerve())
		return;
	BOOL playing = getMActor()->checkCurBckFromIndex(4);
	if (playing)
		return;
	if (getMActor()->checkCurBckFromIndex(0))
		return;

	forceAllItemKill();
	unk368 = 0;

	if (unk350)
		gpMSound->startSoundActor(MSD_SE_BS_TELESA_ESCAPE, &mPosition);
	else
		gpMSound->startSoundActor(MSD_SE_BS_TELESA_DISAPPEAR, &mPosition);

	mSpine->reset();
	mSpine->setNext(&TNerveBossTelesaHide::theNerve());
}

// Called from TTelesaSlot::moveObject once all three drums have stopped. The
// out-of-line copy the map sizes at 496 bytes expands getSlotResult three
// times; at the call site it sits one level deeper and stays a call.
void TBossTelesa::fanfale()
{
	if (mSlot->getSlotResult() == 2 || mSlot->getSlotResult() == 0) {
		unk374.set(0.0f, 0.0f, 0.0f);
		gpMarioParticleManager->emit(BTELESA_JPA_MS_BTLS_FUBUKI, &unk374, 0, nullptr);

		if (mSlot->getSlotResult() == 2)
			gpMSound->startSoundActor(MSD_SE_BS_TELESA_FANFALE_1, &mPosition, 0,
			                          nullptr, 0, 4);
		else
			gpMSound->startSoundActor(MSD_SE_BS_TELESA_FANFALE_2, &mPosition, 0,
			                          nullptr, 0, 4);
	} else {
		gpMSound->startSoundActor(MSD_SE_BS_TELESA_FANFALE_3, &mPosition, 0,
		                          nullptr, 0, 4);
	}
}

// TODO: no nerve body below is reconstructed; each carries its map size.

// TODO: instruction-identical; retail's frame is 0x60 larger (0x128 vs 0xc8)
// with no slot in the body using it, so some inlined callee reserves more.
DEFINE_NERVE(TNerveBossTelesaDie, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();

	if (spine->getTime() == 0) {
		boss->unk388 = 0;

		if (boss->unk350)
			boss->decHitPoints();

		boss->onAllCollision();

		if (boss->mHitPoints) {
			if (boss->unk350) {
				boss->getMActor()->setBrkFromIndex(1);
				boss->setBckAnm(2);
				// mCamShakeNameSave[0x1F] is "/Camera/shakeBTelesaDamage.prm".
				gpCameraShake->startShake((EnumCamShakeMode)0x1F, 1.0f);
			} else {
				boss->setBckAnm(5);
				boss->getMActor()->setBrkFromIndex(0);
				// mCamShakeNameSave[0x20] is "/Camera/shakeBTelesaHit.prm".
				gpCameraShake->startShake((EnumCamShakeMode)0x20, 1.0f);
			}
		} else {
			MSBgm::stopBGM(MSD_BGM_BOSS, 10);
			// mCamShakeNameSave[0x21] is "/Camera/shakeBTelesaDown.prm".
			gpCameraShake->startShake((EnumCamShakeMode)0x21, 1.0f);

			boss->setBckAnm(3);
			boss->getMActor()->setBrkFromIndex(1);
			boss->mSlot->mScaling.set(0.0f, 0.0f, 0.0f);

			gpMSound->startSoundActor(MSD_SE_BS_TELESA_DOWN, &boss->mPosition,
			                          0, nullptr, 0, 4);
		}
	}

	if (boss->mHitPoints == 0) {
		if (boss->checkCurAnmEnd(ANM_TYPE_BCK)) {
		if (boss->unk388 == 0) {
			MtxPtr headMtx = boss->getMActor()->getModel()->getAnmMtx(1);
			boss->unk374.set(headMtx[0][3], headMtx[1][3], headMtx[2][3]);

			gpMarioParticleManager->emit(BTELESA_JPA_MS_BTLS_DOWN,
			    &boss->unk374, 0, nullptr);

			if (boss->unk380 == BTELESA_JPA_MS_BTLS_FHIT_PE)
				gpMarioParticleManager->emit(BTELESA_JPA_MS_BTLS_DOWN_PE,
				    &boss->unk374, 0, nullptr);
			else if (boss->unk380 == BTELESA_JPA_MS_BTLS_FHIT_GR)
				gpMarioParticleManager->emit(BTELESA_JPA_MS_BTLS_DOWN_GR,
				    &boss->unk374, 0, nullptr);
			else
				gpMarioParticleManager->emit(BTELESA_JPA_MS_BTLS_DOWN_OR,
				    &boss->unk374, 0, nullptr);

			boss->forceAllItemKill();

			for (int i = 0; i < 3; ++i) {
				boss->mRoulettes[i]->unk144 = 0.0f;
				boss->mSlot->mRollSp[i]     = 0.0f;
			}

			boss->rollRouletteCircle();
		}

		if (boss->unk388 > 240) {
			boss->unk388 = 0;
			gpItemManager->makeShineAppearWithDemo(
			    "シャイン（ボス用）", "ボスシャインカメラ", boss->mPosition.x,
			    boss->mPosition.y, boss->mPosition.z);

			boss->onLiveFlag(LIVE_FLAG_DEAD);
			boss->onLiveFlag(LIVE_FLAG_UNK8);
			boss->offLiveFlag(0x10000);
			boss->mHolder = nullptr;

			boss->onAllCollision();

			boss->stopAnmSound();
			spine->reset();

			return TRUE;
		}

		boss->unk388 += 1;
		}
	} else if (boss->checkCurAnmEnd(ANM_TYPE_BCK)) {
		if (boss->getMActor()->checkCurBckFromIndex(5)) {
			boss->getMActor()->setBrkFromIndex(2);
			boss->setBckAnm(7);
		} else if (boss->getMActor()->checkCurBckFromIndex(7)) {
			boss->setBckAnm(6);
		} else {
			SMS_ResetDamageFogEffect(
			    boss->getMActor()->getModel()->getModelData());

			if (boss->getMActor()->checkCurBckFromIndex(6)) {
				boss->offAllCollision();

				boss->setBckAnm(15);
				boss->getMActor()->setBtpFromIndex(2);

				spine->reset();
				spine->setNext(&TNerveBossTelesaPrepareSlot::theNerve());
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

// Binding level worth +8 of low region, landing
// TNerveBossTelesaHide::execute's frame at 0x40 (batch 121).
static inline MActor* BosstelesaGetMActor(const TBossTelesa* p)
{
	MActor* mActor = p->getMActor();
	return mActor;
}

// Two-local binding level worth +8 of low region in
// TNerveBossTelesaSpit::execute (frame ladder 271).
static inline J3DFrameCtrl* BosstelesaGetBckCtrl(const TBossTelesa* p)
{
	MActor* mActor      = p->getMActor();
	J3DFrameCtrl* frame = mActor->getFrameCtrl(ANM_TYPE_BCK);
	return frame;
}

static inline TBossTelesa* BosstelesaGetBody(TSpineBase<TLiveActor>* spine)
{
	TBossTelesa* body = (TBossTelesa*)spine->getBody();
	return body;
}

DEFINE_NERVE(TNerveBossTelesaSpit, TLiveActor)
{
	TBossTelesa* boss = BosstelesaGetBody(spine);

	if (spine->getTime() == 0
	    || !boss->getMActor()->checkCurBckFromIndex(14)) {
		boss->setBckAnm(14);
	} else if (BosstelesaGetBckCtrl(boss)->checkPass(40.0f)) {
		boss->genAttacker();
	}

	if (boss->checkCurAnmEnd(ANM_TYPE_BCK))
		return TRUE;

	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaHide, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();

	if (!BosstelesaGetMActor(boss)->checkCurBckFromIndex(4)) {
		boss->setBckAnm(4);
		boss->getMActor()->setBtpFromIndex(2);
	}

	if (boss->checkCurAnmEnd(ANM_TYPE_BCK)) {
		boss->onAllCollision();

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
		boss->unk350 = false;

		u8 maxHitPoints = boss->getMaxHitPoints();
		boss->unk34C.a = MsClamp<u8>(
		    TBossTelesa::mNormalAlpha
		        + (maxHitPoints - boss->mHitPoints) * 30,
		    0, 254);

		boss->mSlot->mScaling.set(0.0f, 0.0f, 0.0f);
		boss->getMActor()->setBrkFromIndex(2);

		s16 end = boss->getMActor()->getFrameCtrl(ANM_TYPE_BRK)->getEnd();
		boss->getMActor()->getFrameCtrl(ANM_TYPE_BRK)->setFrame(end);
	} else {
		// Another unused distance, as in TNerveBossTelesaFallDemo.
		// `a = b - c`, not a copy plus an in-place sub: only the former
		// reaches inline depth 4 (copy ctor 1, operator- 2, operator-= 3),
		// where the ROM `bl`s TVec3::sub.
		// TODO: the sub temporary sits at 0x48, retail 0x38 (the open
		// `a = b - c` class); getPosition()/SMS_GetMarioPos() are inert.
		JGeometry::TVec3<f32> toMario = boss->mPosition - *gpMarioPos;

		if (spine->getTime() > 400 && !boss->mKillSmallEnemy->unk6C) {
			spine->pushAfterCurrent(&TNerveBossTelesaAppear::theNerve());
			boss->offLiveFlag(LIVE_FLAG_HIDDEN);

			return TRUE;
		}
	}

	return FALSE;
}

// The ROM `bl`s randomReset() from TNerveBossTelesaAppear (its only call
// site in the object) while inlining it in TTelesaSlot::initMapObj: the call
// sits one inline level down here, where its body is over the budget.
//
// TODO: TNerveBossTelesaAppear is instruction-exact but its frame is 0x58
// against the ROM's 0x80; moving the scaling reset or the whole first arm into
// this level, or naming the slot, leaves the frame where it is.
static inline void BosstelesaResetSlot(TBossTelesa* boss)
{
	boss->mSlot->randomReset();
}

DEFINE_NERVE(TNerveBossTelesaAppear, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();

	if (spine->getTime() == 0
	    && !boss->getMActor()->checkCurBckFromIndex(0)) {
		boss->setBckAnm(0);

		if (!boss->unk384) {
			boss->unk384 = true;
			MSBgm::startBGM(MSD_BGM_BOSS);
		}

		boss->mSlot->mScaling.set(1.0f, 1.0f, 1.0f);
		BosstelesaResetSlot(boss);

		boss->offAllCollision();
	} else if (boss->checkCurAnmEnd(ANM_TYPE_BCK)
	           && !boss->getMActor()->checkCurBckFromIndex(15)) {
		boss->setBckAnm(15);
		boss->getMActor()->setBtpFromIndex(2);
	}

	if (boss->getMActor()->checkCurBckFromIndex(0)
	    && boss->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(40.0f)) {
		// mCamShakeNameSave[0x22] is "/Camera/shakeBTelesaAppear.prm".
		gpCameraShake->startShake((EnumCamShakeMode)0x22, 1.0f);
		SMSGetMSound()->startSoundActor(MSD_SE_BS_TELESA_SLT_LAND, &boss->mPosition,
		                          0, nullptr, 0, 4);
	}

	if (spine->getTime() > 800) {
		u8 maxHitPoints = boss->getMaxHitPoints();
		if (spine->getTime()
		        % (TBossTelesa::mTelesaGenerateInterval
		           + (maxHitPoints - boss->mHitPoints) * 100)
		    == 1)
			boss->setBckAnm(14);
	}

	boss->unk364 *= 0.9f;

	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaSlotStart, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();

	if (spine->getTime() == 0)
		boss->setBckAnm(11);

	if (BosstelesaGetMActor(boss)->checkCurBckFromIndex(11)) {
		if (boss->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(53.0f))
			boss->slotStart();

		if (boss->checkCurAnmEnd(ANM_TYPE_BCK)) {
			boss->setBckAnm(15);
			BosstelesaGetMActor(boss)->setBtpFromIndex(2);
			boss->mSlot->forceStopSlot(1);
		}
	}

	if (boss->slotStop())
		return TRUE;

	boss->unk364 *= 0.99f;

	return FALSE;
}

static inline TLiveActor* BosstelesaGetSlotItem(TBossTelesa* p, int index)
{
	TLiveActor* item = p->mSlotItems[index];
	return item;
}

DEFINE_NERVE(TNerveBossTelesaSpitSlotItem, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();

	if (!BosstelesaGetMActor(boss)->checkCurBckFromIndex(14)
	    && boss->unk364 < TBossTelesa::mBaseHoseiPosY - 300.0f) {
		boss->setBckAnm(14);
	} else if (boss->checkCurAnmEnd(ANM_TYPE_BCK) && spine->getTime() > 600) {
		spine->pushAfterCurrent(&TNerveBossTelesaPrepareSlot::theNerve());
		// TODO: one instruction left -- the ROM shares the zero it stores in
		// unk368 with the loop's byte offset (`mr r3, r4`, ours `li r6, 0`);
		// `i = 0` spelt as the stored value and `i++` are inert.
		boss->unk368 = 0;

		for (int i = 0; i < boss->mSlotItemNum; ++i) {
			TLiveActor* item = BosstelesaGetSlotItem(boss, i);
			if (!item->checkLiveFlag(LIVE_FLAG_DEAD)) {
				if (!item->isActorType(0x2000000E)) {
					if (!item->isActorType(0x20000002))
						item->offHitFlag(HIT_FLAG_NO_COLLISION);
				}
			}
		}

		return TRUE;
	} else if (spine->getTime() > 200) {
		boss->unk364 -= 2.0f;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBossTelesaPrepareSlot, TLiveActor)
{
	TBossTelesa* boss = (TBossTelesa*)spine->getBody();

	if (spine->getTime() == 0) {
		boss->setBckAnm(15);
		BosstelesaGetMActor(boss)->setBtpFromIndex(2);
	}

	if (boss->unk350) {
		boss->unk36C += 1;

		if (boss->checkCurAnmEnd(ANM_TYPE_BCK)) {
			if (BosstelesaGetMActor(boss)->checkCurBckFromIndex(1)) {
				boss->setBckAnm(12);
				BosstelesaGetMActor(boss)->setBtpFromIndex(1);
			} else if (BosstelesaGetMActor(boss)->checkCurBckFromIndex(12)) {
				if (boss->unk36C > boss->getSaveParam2()->mSLSpicyTime.get())
					boss->setBckAnm(13);
			} else {
				boss->unk36C = 0;
				boss->unk350 = false;

				u8 maxHitPoints = boss->getMaxHitPoints();
				boss->unk34C.a = MsClamp<u8>(
				    TBossTelesa::mNormalAlpha
				        + (maxHitPoints - boss->mHitPoints) * 30,
				    0, 254);

				boss->setBckAnm(15);
				BosstelesaGetMActor(boss)->setBtpFromIndex(2);
			}
		}
	}

	boss->unk368 += 1;

	int timeLimit = boss->getSaveParam2()->mSLStopSlotTime0.get();
	if (boss->mHitPoints == 2)
		timeLimit = boss->getSaveParam2()->mSLStopSlotTime1.get();
	if (boss->mHitPoints == 1)
		timeLimit = boss->getSaveParam2()->mSLStopSlotTime2.get();

	if (boss->checkSlotResult())
		timeLimit = (int)((f32)timeLimit / 2.0f);

	if (boss->unk368 > timeLimit - 120) {
		int remaining = timeLimit - boss->unk368;
		boss->flashItem(remaining);
	}

	if (BosstelesaGetMActor(boss)->checkCurBckFromIndex(15)) {
		if (boss->checkAllItemDead() || boss->unk368 > timeLimit) {
			boss->unk368 = 0;
			boss->forceAllItemKill();

			if (boss->unk350)
				gpMSound->startSoundActor(MSD_SE_BS_TELESA_ESCAPE,
				                          &boss->mPosition, 0, nullptr, 0, 4);
			else
				gpMSound->startSoundActor(MSD_SE_BS_TELESA_DISAPPEAR,
				                          &boss->mPosition, 0, nullptr, 0, 4);

			spine->reset();
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

	if (BosstelesaGetMActor(boss)->checkCurBckFromIndex(16)) {
		if (boss->checkCurAnmEnd(ANM_TYPE_BCK)) {
			boss->unk350 = false;

			u8 maxHitPoints = boss->getMaxHitPoints();
			boss->unk34C.a = MsClamp<u8>(
			    TBossTelesa::mNormalAlpha
			        + (maxHitPoints - boss->mHitPoints) * 30,
			    0, 254);

			return TRUE;
		}
	} else {
		boss->setBckAnm(16);
		gpMSound->startSoundActor(MSD_SE_BS_TELESA_THANKYOU,
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

		boss->getMActor()->setFrameRate(0.0f, ANM_TYPE_BCK);
		boss->mSlot->mScaling.set(0.0f, 0.0f, 0.0f);
	}

	if (boss->rouletteFall()) {
		// The distance is never used: a leftover from the demo camera work.
		// TODO: 12 bytes of frame and one extra copy-out. Retail's operator-
		// builds the difference straight in the named local (one temp at
		// 0x34, `bl sub` at depth 1); we copy the returned temp into it.
		// Measured: `= boss->mPosition; toMario.sub(...)` and a bare
		// expression statement both inline `sub` (90.2); a `const&` binding
		// and direct-init are identical to this (95.1).
		JGeometry::TVec3<f32> toMario = boss->mPosition - *gpMarioPos;

		if (boss->slotFall()) {
			boss->offAllCollision();

			spine->reset();
			spine->setNext(&TNerveBossTelesaHideWait::theNerve());
			spine->pushAfterCurrent(&TNerveBossTelesaHideWait::theNerve());

			return TRUE;
		}
	}

	return FALSE;
}
