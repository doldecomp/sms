#include <Enemy/SmallEnemy.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/MarDirector.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Application.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <stdlib.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: this definitely means that the no memory msg & zeros are from a
// different header than mtx calc types

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

// NOTE: has to be here for proper rodata order
#include <Map/MapCollisionEntry.hpp>

static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};

int TSmallEnemyManager::mBlockWaitTime   = 400;
f32 TSmallEnemyManager::mBlockMoveSpeed  = 2.0f;
f32 TSmallEnemyManager::mBlockWaitMoveY  = 200.0f;
u32 TSmallEnemyManager::mChangeBlockTime = 2500;
f32 TSmallEnemyManager::mBlockXZScale    = 1.5f;
f32 TSmallEnemyManager::mBlockYScale     = 0.6f;
u8 TSmallEnemyManager::mTestJuiceType    = 0;

bool TSmallEnemy::mIsPolluter    = true;
bool TSmallEnemy::mIsAmpPolluter = true;

TSmallEnemyParams::TSmallEnemyParams(const char* name)
    : TSpineEnemyParams(name)
    , PARAM_INIT(mSLJumpForce, 10.0f)
    , PARAM_INIT(mSLSearchLength, 800.0f)
    , PARAM_INIT(mSLSearchHeight, 600.0f)
    , PARAM_INIT(mSLSearchAware, 400.0f)
    , PARAM_INIT(mSLSearchAngle, 90.0f)
    , PARAM_INIT(mSLGiveUpLength, 2000.0f)
    , PARAM_INIT(mSLGiveUpHeight, 650.0f)
    , PARAM_INIT(mSLAttackWait, 120)
    , PARAM_INIT(mSLFreezeWait, 300)
    , PARAM_INIT(mSLDamageRadius, 80)
    , PARAM_INIT(mSLDamageHeight, 60)
    , PARAM_INIT(mSLAttackRadius, 80)
    , PARAM_INIT(mSLAttackHeight, 60)
    , PARAM_INIT(mSLTurnSpeedLow, 3.0f)
    , PARAM_INIT(mSLTurnSpeedHigh, 5.0f)
    , PARAM_INIT(mSLBodyScaleLow, 0.108f)
    , PARAM_INIT(mSLBodyScaleHigh, 0.11f)
    , PARAM_INIT(mSLGenItemRate, 50.0f)
    , PARAM_INIT(mSLGenEggRate, 10.0f)
    , PARAM_INIT(mSLPolluteRange, 5)
    , PARAM_INIT(mSLWaitTime, 100)
    , PARAM_INIT(mSLPolluteRMin, 1)
    , PARAM_INIT(mSLPolluteRMax, 3)
    , PARAM_INIT(mSLPolluteCycle, 300)
    , PARAM_INIT(mSLStampRange, 5)
    , PARAM_INIT(mSLPolluteInterval, 60)
    , PARAM_INIT(mSLGenerateOnlyDead, 0)
    , unk2C4(0.0f)
    , unk2C8(1.0f)
    , unk2CC(0.0f)
    , unk2D0(1.0f)
{
	TParams::load(mPrmPath);

	unk2C4 = mSLTurnSpeedLow.get();
	unk2C8 = mSLTurnSpeedHigh.get();
	unk2CC = mSLBodyScaleLow.get();
	unk2D0 = mSLBodyScaleHigh.get();
}

TSmallEnemyManager::TSmallEnemyManager(const char* name)
    : TEnemyManager(name)
    , unk58(0)
    , unk5C(0x3C)
{
}

void TSmallEnemyManager::createModelData()
{
	static TModelDataLoadEntry entry = { "default.bmd", 0x10220000, 0 };
	createModelDataArray(&entry);
}

void TSmallEnemyManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
}

void TSmallEnemyManager::loadAfter()
{
	TEnemyManager::loadAfter();
	ResTIMG* img = (ResTIMG*)JKRGetResource("/common/timg/down_osen.bti");
	unk58 = gpPollution->getCounterLayer().registerTexStamp(1, mCapacity, img);
}

void TSmallEnemyManager::createEnemies(int param_1)
{
	TEnemyManager::createEnemies(param_1);
	initSetEnemies();
}

TSmallEnemy* TSmallEnemyManager::getHolder(int param_1)
{
	for (int i = 0; i < getActiveObjNum(); ++i)
		if (i != param_1 && getObj(i)->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
			return (TSmallEnemy*)getObj(i);
	return nullptr;
}

TSmallEnemy::TSmallEnemy(const char* name)
    : TSpineEnemy(name)
    , unk150(0)
    , unk154(1.0f)
    , unk158(1.0f)
    , mCurrentBckAnm(-1)
    , mSprayedByWaterCooldown(0)
    , unk164(0)
    , unk165(0)
    , unk174(0)
    , mJuiceBlock(0)
    , mCoinId(-1)
    , mCoin(nullptr)
    , unk184(0)
    , unk185(0)
    , unk188(1.0f)
    , unk18C(0)
{
}

void TSmallEnemy::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(getManager(), 1);
	mMActor       = getActorKeeper()->createMActorFromNthData(0, 0);
}

void TSmallEnemy::init(TLiveManager* param_1)
{
	mManager = param_1;
	param_1->manageActor(this);

	setMActorAndKeeper();

	onLiveFlag(LIVE_FLAG_DEAD);
	if (gpMarDirector->mMap == 2 && gpMarDirector->unk7D == 0)
		onLiveFlag(LIVE_FLAG_UNK2000);
	onHitFlag(HIT_FLAG_UNK1);
	unk158 = 1.0f;

	// TODO: are these f32 pairs some kind of rng interval class?
	TSmallEnemyParams* params1 = getSaveParam2();
	mTurnSpeed                 = MsRandF(params1->unk2C4, params1->unk2C8);

	TSmallEnemyParams* params2 = getSaveParam2();
	mBodyScale                 = MsRandF(params2->unk2D0, params2->unk2CC);

	unk154            = mBodyScale;
	mBodyRadius       = getSaveParam2()->mSLBodyRadius.get();
	mWallRadius       = getSaveParam2()->mSLWallRadius.get();
	mHeadHeight       = getSaveParam2()->mSLHeadHeight.get();
	mScaledBodyRadius = mBodyScale * mBodyRadius * 15.0f;

	f32 attackRadius = getSaveParam2()->mSLAttackRadius.get();
	f32 attackHeight = getSaveParam2()->mSLAttackHeight.get();
	f32 damageRadius = getSaveParam2()->mSLDamageRadius.get();
	f32 damageHeight = getSaveParam2()->mSLDamageHeight.get();

	initHitActor(0, 5, -0x68000000, attackRadius * mBodyScale,
	             attackHeight * mBodyScale, damageRadius * mBodyScale,
	             damageHeight * mBodyScale);

	mGroundPlane = TMap::getIllegalCheckData();
	if (!unk124->unk0 || unk124->unk0->isDummy())
		unk124->unk0 = gpConductor->getGraphByName("main");

	setGoalPathMario();
	initAnmSound();
}

void TSmallEnemy::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);
	stream.read(&mCoinId, 4);
	if (mCoinId != 0x65)
		unk18C = 1;
	reset();
}

void TSmallEnemy::loadAfter()
{
	mCoin = TItemManager::newAndRegisterCoin(mCoinId);
}

void TSmallEnemy::behaveToWater(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() != &TNerveSmallEnemyFreeze::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()) {
		mSpine->pushNerve(&TNerveSmallEnemyFreeze::theNerve());
	}
}

void TSmallEnemy::sendAttackMsgToMario()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

void TSmallEnemy::attackToMario()
{
	sendAttackMsgToMario();

	// TODO: wtf
	volatile JGeometry::TVec3<f32> local_14(0, 0, 0);

	JGeometry::TVec3<f32> local_20;
	local_20.sub(mPosition, *gpMarioPos);
	MsVECNormalize(&local_20, &local_20);
	mVelocity.set(local_20);

	JGeometry::TVec3<f32> v;
	v.scale(mBodyScale * mBodyRadius, local_20);
	v += local_14;
	mLinearVelocity = v;
}

void TSmallEnemy::reset()
{
	TSpineEnemy::reset();

	TSmallEnemyParams* params1 = getSaveParam2();
	mTurnSpeed                 = MsRandF(params1->unk2C4, params1->unk2C8);

	TSmallEnemyParams* params2 = getSaveParam2();
	mBodyScale                 = MsRandF(params2->unk2D0, params2->unk2CC);

	unk190 = unk154 = mBodyScale;

	mBodyRadius       = getSaveParam2()->mSLBodyRadius.get();
	mWallRadius       = getSaveParam2()->mSLWallRadius.get();
	mHeadHeight       = getSaveParam2()->mSLHeadHeight.get();
	mScaledBodyRadius = mBodyScale * mBodyRadius * 15.0f;

	mHitPoints = getSaveParam2() ? getSaveParam2()->mSLHitPointMax.get() : 1;

	offLiveFlag(LIVE_FLAG_DEAD);
	offLiveFlag(LIVE_FLAG_UNK20000);
	offLiveFlag(LIVE_FLAG_UNK10000);
	offLiveFlag(LIVE_FLAG_UNK2);

	if (getSaveParam2()->mSLGenerateOnlyDead.get())
		offLiveFlag(LIVE_FLAG_UNK800);
	else
		onLiveFlag(LIVE_FLAG_UNK800);

	unk124->mPrevIdx = -1;
	goToShortestNextGraphNode();
	offHitFlag(HIT_FLAG_UNK1);
	mSpine->reset();
	mSpine->pushAfterCurrent(mSpine->getDefault());
	mScaling.set(mBodyScale, mBodyScale, mBodyScale);

	mSprayedByWaterCooldown = 0;
	unk165                  = 0;
	unk184                  = 0;

	f32 attackRadius = getSaveParam2()->getSLAttackRadius();
	f32 attackHeight = getSaveParam2()->getSLAttackHeight();
	f32 damageRadius = getSaveParam2()->getSLDamageRadius();
	f32 damageHeight = getSaveParam2()->getSLDamageHeight();

	f32 scale = mBodyScale;

	mAttackRadius = attackRadius * scale;
	mAttackHeight = attackHeight * scale;
	mDamageRadius = damageRadius * scale;
	mDamageHeight = damageHeight * scale;

	calcEntryRadius();

	unk164 = 0;

	MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x, mPosition.y,
	               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	getMActor()->calc();
}

void TSmallEnemy::forceKill()
{
	if ((mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL)
	     || (!mGroundPlane->isDeathPlane() && !mGroundPlane->isPool()
	         && !mGroundPlane->isWaterSurface())
	     || isAirborne() || checkLiveFlag(LIVE_FLAG_UNK10))
	    && !gpMap->isInArea(mPosition.x, mPosition.z)) {

		if (mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()) {
			mSpine->reset();
			mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
			mSpine->pushAfterCurrent(mSpine->getDefault());

			onLiveFlag(LIVE_FLAG_UNK20000);
			mHitPoints = 1;
		}
	}
}

void TSmallEnemy::genRandomItem()
{
	if (!getSaveParam2())
		return;

	setAfterDeadEffect();
	genEventCoin();
	if (!checkLiveFlag(LIVE_FLAG_UNK20000 | LIVE_FLAG_DEAD))
		generateItem();
}

void TSmallEnemy::genEventCoin()
{
	if (checkLiveFlag(LIVE_FLAG_UNK20000))
		return;

	if (mCoin) {
		TCoin* coin;
		if (isActorType(0x2000000E)) {
			coin = (TCoin*)gpItemManager->makeObjAppear(0x2000000E);
		} else {
			coin = mCoin;
			coin->appear();
		}

		if (coin) {
			coin->mPosition = mPosition;
			coin->mVelocity.set(0, 20, 0);
			coin->offLiveFlag(LIVE_FLAG_UNK10);
			--unk18C;
		}
	}

	if (unk18C > 0) {
		for (int i = 0; i < unk18C; ++i) {
			Mtx44 local_c0;

			f32 angle = 360.0f / unk18C * i + mRotation.y;
			f32 s     = JMASin(angle);
			f32 c     = JMACos(angle);

			local_c0[0][0] = c;
			local_c0[0][1] = 0.0f;
			local_c0[0][2] = s;
			local_c0[0][3] = 0.0f;

			local_c0[1][0] = 0.0f;
			local_c0[1][1] = 1.0f;
			local_c0[1][2] = 0.0f;
			local_c0[1][3] = 0.0f;

			local_c0[2][0] = -s;
			local_c0[2][1] = 0.0f;
			local_c0[2][2] = c;
			local_c0[2][3] = 0.0f;

			Vec local_d0;
			local_d0.x = 0.0f;
			local_d0.y = 0.0f;
			local_d0.z = 100.0f;
			if (unk18C == 1)
				local_d0.z = 1.0f;

			MTXMultVec(local_c0, &local_d0, &local_d0);

			TCoin* coin = (TCoin*)gpItemManager->makeObjAppear(
			    mPosition.x + local_d0.x, mPosition.y, mPosition.z + local_d0.z,
			    0x2000000E, false);
			if (coin) {
				coin->mPosition.y = mPosition.y;
				MsVECNormalize(&local_d0, &local_d0);
				// TODO: is this a method on some RandInterval class?
				coin->mVelocity.set(local_d0.x * 4, MsRandF(16.0f, 8.0f),
				                    local_d0.z * 4);
				coin->offLiveFlag(LIVE_FLAG_UNK10);
			}
		}
	}
}

void TSmallEnemy::setAfterDeadEffect()
{
	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0xE4, &mPosition, 0, nullptr)) {
		emitter->unk154.set(mScaling);
		emitter->unk174.set(mScaling);
	}

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0xE6, &mPosition, 0, nullptr)) {
		emitter->unk154.set(mScaling);
		emitter->unk174.set(mScaling);
	}

	if (gpMSound->gateCheck(0x295F))
		MSoundSESystem::MSoundSE::startSoundActor(0x295F, &mPosition, 0,
		                                          nullptr, 0, 4);
}

void TSmallEnemy::generateItem()
{
	if (MsRandF(0.0f, 100.0f) < getSaveParam2()->mSLGenEggRate.get()
	                                + getSaveParam2()->mSLGenItemRate.get()

	    && !mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL))
		gpMapObjManager->makeObjAppear(mPosition.x, mGroundHeight, mPosition.z,
		                               0x20000008, true);
}

bool TSmallEnemy::isCollidMove(THitActor* param_1)
{
	if ((param_1->mActorType & 0xffff0000) == 0x40000000)
		return false;

	if (!((TLiveActor*)param_1)->isAirborne() && !isAirborne())
		return true;

	return false;
}

void TSmallEnemy::moveObject()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	f32 attackRadius = getSaveParam2()->getSLAttackRadius();
	f32 attackHeight = getSaveParam2()->getSLAttackHeight();
	f32 damageRadius = getSaveParam2()->getSLDamageRadius();
	f32 damageHeight = getSaveParam2()->getSLDamageHeight();

	f32 scale = mBodyScale;

	mAttackRadius = attackRadius * scale;
	mAttackHeight = attackHeight * scale;
	mDamageRadius = damageRadius * scale;
	mDamageHeight = damageHeight * scale;

	calcEntryRadius();
	ensureTakeSituation();
	mLinearVelocity.zero();
	mAngularVelocity.zero();
	control();

	if (!isInhibitedForceMove())
		calcRideMomentum();

	for (int i = 0; i < getColNum(); ++i) {
		THitActor* col = getCollision(i);
		if (col->isActorType(0x80000001)) {
			attackToMario();
			continue;
		}

		if (!isCollidMove(col))
			continue;

		JGeometry::TVec3<f32> v;
		v.zero();

		JGeometry::TVec3<f32> local_74;
		local_74.sub(mPosition, col->mPosition);
		if (local_74.x == 0.0f && local_74.y == 0.0f && local_74.z == 0.0f)
			local_74.x += 1;

		MsVECNormalize(&local_74, &local_74);
		local_74.scale(mMarchSpeed * 3.0f * unk158);

		v.add(local_74);
		mLinearVelocity = v;
	}

	bind();
	forceKill();
	setBehavior();

	mPosition += mLinearVelocity;
	mRotation += mAngularVelocity;

	if (mSprayedByWaterCooldown > 0)
		++mSprayedByWaterCooldown;

	if (mSprayedByWaterCooldown > 30)
		mSprayedByWaterCooldown = 0;

	if (mMapCollisionManager && mMapCollisionManager->getUnk8())
		mMapCollisionManager->getUnk8()->moveSRT(mPosition, mRotation,
		                                         mScaling);

	if (!isInhibitedForceMove())
		calcRidePos();
}

void TSmallEnemy::updateAnmSound() { TSpineEnemy::updateAnmSound(); }

BOOL TSmallEnemy::receiveMessage(THitActor* sender, u32 message)
{
	if (isEatenByYosshi() && message == 4 && !mHolder) {
		onHitFlag(HIT_FLAG_UNK1);
		mHolder = (TTakeActor*)sender;
		behaveToTaken(sender);
		return true;
	}

	if ((message == 6 || message == 7) && mHolder == sender) {
		mHolder = nullptr;
		behaveToRelease();
		offHitFlag(HIT_FLAG_UNK1);
		return true;
	}

	if (message == 0 || message == 1 || message == 3 || message == 11
	    || (mActorType == 0x10000021 && message == 0xC)) {
		if (isHitValid(message)) {
			unk184 = 0;
			kill();
		}
		return true;
	}

	if (message == 13) {
		mHitPoints = 0;
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFlag(HIT_FLAG_UNK1);
	}

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(0xE7, &sender->mPosition, 0, nullptr);
		gpMSound->startSoundSet(0x6802, &mPosition, 0, 0.0f, 0, 0, 4);
		if (mSprayedByWaterCooldown == 0) {
			mSprayedByWaterCooldown = 1;
			if (!changeByJuice()) {
				decHpByWater(sender);
				behaveToWater(sender);
			}
		}

		return true;
	}

	return false;
}

bool TSmallEnemy::changeByJuice()
{
	if (gpModelWaterManager->unk5D5F == 1 || gpModelWaterManager->unk5D5F == 2
	    || gpModelWaterManager->unk5D5F == 3
	    || TSmallEnemyManager::mTestJuiceType != 0) {

		if (mJuiceBlock)
			return true;

		TJuiceBlock* block = (TJuiceBlock*)gpMapObjManager->makeObjAppear(
		    mPosition.x, mPosition.y, mPosition.z, 0x400002C6, true);
		mJuiceBlock = block;

		if (!mJuiceBlock)
			return false;

		mJuiceBlock->mScaling.set(0.1f, 0.1f, 0.1f);
		mJuiceBlock->unk140.set(0.0f, 0.0f, 0.0f);
		mJuiceBlock->mRotation.set(0.0f, mRotation.y, 0.0f);

		mJuiceBlock->unk14C = this;
		mJuiceBlock->offLiveFlag(LIVE_FLAG_UNK2);
		onHitFlag(HIT_FLAG_UNK1);
		onLiveFlag(LIVE_FLAG_UNK2);
		onLiveFlag(LIVE_FLAG_UNK10);

		mSpine->pushNerve(&TNerveSmallEnemyChange::theNerve());

		unk185 = gpModelWaterManager->unk5D5F;
		if (TSmallEnemyManager::mTestJuiceType != 0)
			unk185 = TSmallEnemyManager::mTestJuiceType;

		switch (unk185) {
		case 1:
			block->unk138.x = 0x1C2;
			block->unk138.y = 0xDC;
			block->unk138.z = 0x50;
			break;
		case 2:
			block->unk138.x = 0x118;
			block->unk138.y = 0x6E;
			block->unk138.z = 0x154;
			break;
		case 3:
			block->unk138.x = 0x1E0;
			block->unk138.y = 0xFA;
			block->unk138.z = 0x154;
			break;
		}

		offLiveFlag(LIVE_FLAG_UNK800);
		return true;
	}

	return false;
}

int TSmallEnemy::getChangeBlockTime()
{
	return TSmallEnemyManager::mChangeBlockTime;
}

bool TSmallEnemy::changeMove()
{
	if (TSmallEnemyManager::mBlockWaitTime * 0.2f <= mSpine->getTime()) {
		f32 time = TSmallEnemyManager::mBlockWaitTime * 0.2f;

		mJuiceBlock->mPosition.y += unk188 * 2.0f
		                            * JMASin(mSpine->getTime() * 130.0f / time)
		                            * TSmallEnemyManager::mBlockWaitMoveY;

		mJuiceBlock->mRotation.y += mSpine->getTime() * 1080.0f / time;
	} else {
		if (mSpine->getTime() > TSmallEnemyManager::mBlockWaitTime) {
			if (mSpine->getTime() > getChangeBlockTime() - 200) {
				if (mSpine->getTime() % 20 < 10) {
					mJuiceBlock->onLiveFlag(LIVE_FLAG_UNK2);
				} else {
					mJuiceBlock->offLiveFlag(LIVE_FLAG_UNK2);
				}
			}

			switch (unk185) {
			case 2: {
				JGeometry::TVec3<f32> local_38(0.0f, 0.0f, 1.0f);
				Mtx afStack_68;
				MsMtxSetRotRPH(afStack_68, 0.0f, mRotation.y, 0.0f);
				MTXMultVec(afStack_68, &local_38, &local_38);
				mJuiceBlock->mPosition.x
				    += local_38.x * TSmallEnemyManager::mBlockMoveSpeed;
				mJuiceBlock->mPosition.z
				    += local_38.z * TSmallEnemyManager::mBlockMoveSpeed;

				if (gpMap->isTouchedOneWallAndMoveXZ(
				        &mJuiceBlock->mPosition.x, mJuiceBlock->mPosition.y,
				        &mJuiceBlock->mPosition.z, mBodyRadius * 20.0f))
					return 1;

				JGeometry::TVec3<f32> local_74 = mJuiceBlock->mPosition;
				local_74.x += local_38.x * 300.0f;
				local_74.z += local_38.z * 300.0f;

				const TBGCheckData* local_2C;
				f32 d = gpMap->checkGround(local_74.x, local_74.y + mHeadHeight,
				                           local_74.z, &local_2C);
				if (d > mJuiceBlock->mPosition.y)
					return 1;
				break;
			}

			case 3: {
				mJuiceBlock->mPosition.y += TSmallEnemyManager::mBlockMoveSpeed;
				const TBGCheckData* local_2C;
				f32 d = gpMap->checkRoof(mJuiceBlock->mPosition.x,
				                         mJuiceBlock->mPosition.y + mHeadHeight,
				                         mJuiceBlock->mPosition.z, &local_2C);
				if (local_2C && mJuiceBlock->mPosition.y + mHeadHeight > d
				    && local_2C->mActor != mJuiceBlock)
					return 1;
				break;
			}

			case 0:
			case 1:
			case 4:
				return 0;
			}

			Vec local_80;
			local_80.x = 0.2f;
			local_80.y = 0.0f;
			local_80.z = 0.0f;
			Mtx afStack_b0;
			MsMtxSetRotRPH(afStack_b0, 0.0f, mRotation.y, 0.0f);
			MTXMultVec(afStack_b0, &local_80, &local_80);
			if (mSpine->getTime() % 30 > 15) {
				mJuiceBlock->mPosition.y += 0.5f;
				mJuiceBlock->mPosition.x += local_80.x;
				mJuiceBlock->mPosition.z += local_80.z;
			} else {
				mJuiceBlock->mPosition.y -= 0.5f;
				mJuiceBlock->mPosition.x -= local_80.x;
				mJuiceBlock->mPosition.z -= local_80.z;
			}
		}
	}

	return 0;
}

void TSmallEnemy::scalingChangeActor()
{
	f32 xzScale = MsClamp(mJuiceBlock->unk140.x + 0.02f, 0.0f,
	                      TSmallEnemyManager::mBlockXZScale);

	mJuiceBlock->unk140.x = mJuiceBlock->unk140.z = xzScale;
	mJuiceBlock->mScaling.x = mJuiceBlock->mScaling.z = xzScale;

	f32 yScale              = MsClamp(mJuiceBlock->unk140.y + 0.01f, 0.0f,
	                                  TSmallEnemyManager::mBlockYScale);
	mJuiceBlock->unk140.y   = yScale;
	mJuiceBlock->mScaling.y = yScale;
}

void TSmallEnemy::changeOut()
{
	if (gpMSound->gateCheck(0x293D))
		MSoundSESystem::MSoundSE::startSoundActor(0x293D, &mPosition, 0,
		                                          nullptr, 0, 4);

	kill();
	mJuiceBlock->mPosition = mPosition;

	gpMarioParticleManager->emitAndBindToPosPtr(0xCD, &mPosition, 0, nullptr);
	getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
	mJuiceBlock->kill();
	mJuiceBlock = nullptr;
}

void TSmallEnemy::decHpByWater(THitActor* param_1)
{
	// TODO: not actually a TWaterHItActor, IDK what it is
	s16 uVar2
	    = gpModelWaterManager->getParticleAttack((TWaterHitActor*)param_1);
	if (uVar2 < 1)
		uVar2 = 1;

	if (mHitPoints < uVar2) {
		mHitPoints = 0;
		return;
	}

	mHitPoints -= uVar2;
}

void TSmallEnemy::kill()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	mHitPoints = 1;
	if (mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()) {
		mSpine->reset();
		mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
		mSpine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());

		onLiveFlag(LIVE_FLAG_UNK40);
	}
}

bool TSmallEnemy::isFindMario(float param_1)
{
	bool b = true;
	if (gpMarDirector->unk124 != 3 && gpMarDirector->unk124 != 4)
		b = false;
	if (b)
		return false;

	if (isAirborne())
		return false;

	bool result = false;

	if (!isMarioInWater() && isFindMarioFromParam(param_1))
		result = true;

	return result;
}

bool TSmallEnemy::isMarioInWater() const
{
	bool bVar1 = true;
	bool bVar3 = true;
	bool bVar2 = true;
	if (!SMS_CheckMarioFlag(0x2) && !SMS_CheckMarioFlag(0x10000))
		bVar2 = false;
	if (!bVar2 && !(*gpMarioGroundPlane)->isWaterSurface())
		bVar3 = false;

	if (!bVar3 && !SMS_CheckMarioFlag(0x20000))
		bVar1 = false;

	return bVar1;
}

#pragma dont_inline on
bool TSmallEnemy::isFindMarioFromParam(float param_1) const
{
	TSmallEnemyParams* prms = getSaveParam2();

	f32 searchHeight = prms->mSLSearchHeight.get();

	if (abs(SMS_GetMarioPos().y - mPosition.y) < searchHeight) {

		JGeometry::TVec3<f32> marioPos = SMS_GetMarioPos();

		f32 searchLength = prms->mSLSearchLength.get();
		f32 searchAngle  = prms->mSLSearchAngle.get();
		f32 searchAware  = prms->mSLSearchAware.get();

		if (isInSight(marioPos, searchLength * param_1, searchAngle * param_1,
		              searchAware * param_1))
			return true;
		else
			return false;
	}

	return false;
}
#pragma dont_inline off

void TSmallEnemy::generateEffectColumWater()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		return;

	TEffectColumWater* enemy
	    = (TEffectColumWater*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト水柱マネージャー", 0);

	if (!enemy)
		return;

	enemy->generate(mPosition, mScaling);

	if (gpMSound->gateCheck(0x286D))
		MSoundSESystem::MSoundSE::startSoundActor(0x286D, &mPosition, 0,
		                                          nullptr, 0, 4);
}

void TSmallEnemy::setBckAnm(int index)
{
	mCurrentBckAnm = index;
	getMActor()->setBckFromIndex(index);
	const char** table = getBasNameTable();
	setAnmSound(!table ? nullptr : table[index]);
}

void TSmallEnemy::expandCollision()
{
	f32 attackRadius = getSaveParam2()->getSLAttackRadius();
	f32 attackHeight = getSaveParam2()->getSLAttackHeight();
	f32 damageRadius = getSaveParam2()->getSLDamageRadius();
	f32 damageHeight = getSaveParam2()->getSLDamageHeight();

	f32 expansionFactor = unk190 / unk154;

	mAttackRadius = attackRadius * expansionFactor;
	mAttackHeight = attackHeight * expansionFactor;
	mDamageRadius = damageRadius * expansionFactor;
	mDamageHeight = damageHeight * expansionFactor;

	calcEntryRadius();
}

bool TSmallEnemy::isEaten()
{
	MtxPtr mtx;
	if (mHolder && mHolder->getHeldObject() == this && (mtx = getTakingMtx())) {
		getModel()->setBaseTRMtx(mtx);
		mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);

		return true;
	} else {
		return false;
	}
}

bool TSmallEnemy::isHitWallInBound()
{
	mWallRadius = 25.0f;
	TBGWallCheckRecord local_3C(mPosition.x, mPosition.y + mHeadHeight,
	                            mPosition.z, mWallRadius * mBodyScale * 1.1f, 1,
	                            0);

	if (gpMap->isTouchedWallsAndMoveXZ(&local_3C)) {
		f32 sVar2 = matan(local_3C.mResultWalls[0]->mNormal.z,
		                  local_3C.mResultWalls[0]->mNormal.x)
		            * (360.0f / 65536.0f);

		JGeometry::TVec3<f32> v(mVelocity.x, 0.0f, mVelocity.z);
		if (v.dot(local_3C.mResultWalls[0]->mNormal) > 0.0f)
			return false;

		mPosition.x = local_3C.mCenter.x;
		mPosition.z = local_3C.mCenter.z;
		mRotation.y = sVar2;

		onLiveFlag(LIVE_FLAG_UNK10);
		onHitFlag(HIT_FLAG_UNK1);

		return true;
	}

	return false;
}

void TSmallEnemy::behaveToHitOthers(THitActor* param_1)
{
	if (!isCollidMove(param_1))
		return;

	JGeometry::TVec3<f32> result(0.0f, 0.0f, 0.0f);

	JGeometry::TVec3<f32> local_14;
	local_14.sub(mPosition, param_1->getPosition());

	if (local_14.x == 0.0f && local_14.y == 0.0f && local_14.z == 0.0f)
		local_14.x += 1.0f;

	MsVECNormalize(&local_14, &local_14);

	local_14.scale(mMarchSpeed * 3.0f * unk158);

	result += local_14;
	mLinearVelocity = result;
}

void TSmallEnemy::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	// TODO: wtf is this inline???
	bool bVar2 = true;
	if (gpMarDirector->unk124 != 3 && gpMarDirector->unk124 != 4) {
		bVar2 = false;
	}
	if (!bVar2) {
		bVar2 = true;
		if (gpMarDirector->unk124 != 1 && gpMarDirector->unk124 != 2)
			bVar2 = false;
		if (bVar2) {
			performOnlyDraw(param_1, param_2);
			return;
		}
	}

	TSpineEnemy::perform(param_1, param_2);
}

DEFINE_NERVE(TNerveSmallEnemyDie, TLiveActor)
{
	TSmallEnemy* self = (TSmallEnemy*)spine->getBody();
	if (spine->getTime() == 0) {
		self->decHitPoints();

		if (self->getHitPoints() == 0) {
			self->onHitFlag(HIT_FLAG_UNK1);
			if (self->getGroundPlane()->isWaterSurface() && !self->isAirborne())
				self->generateEffectColumWater();
		}

		if (self->checkLiveFlag(LIVE_FLAG_UNK10000)) {
			self->setMeltAnm();
		} else {
			if (self->checkUnk150(0x20)) {
				TSmallEnemyManager* man
				    = (TSmallEnemyManager*)self->getManager();
				gpPollution->stamp(man->getUnk58(), self->getPosition().x,
				                   self->getPosition().y, self->getPosition().z,
				                   man->getSaveParam2()->getSLStampRange()
				                       * 32.0f * self->getUnk158());
			}
			if (self->getHitPoints() == 0) {
				self->onLiveFlag(LIVE_FLAG_UNK8);
				self->setDeadAnm();
				self->setDeadEffect();
			} else {
				return true;
			}
		}
	}

	int uVar8 = ((TSmallEnemyManager*)self->getManager())->unk5C;
	if (self->getGroundPlane()->isWaterSurface())
		uVar8 = 0;

	if (self->checkCurAnmEnd(0)
	        && spine->getTime()
	               > uVar8 + self->getMActor()->getFrameCtrl(0)->getEndFrame()
	    || spine->getTime() > 360 || self->getUnk184() != 0) {
		self->genRandomItem();
		self->onHitFlag(HIT_FLAG_UNK1);
		self->onLiveFlag(LIVE_FLAG_DEAD);
		self->onLiveFlag(LIVE_FLAG_UNK8);
		self->offLiveFlag(LIVE_FLAG_UNK2);
		self->offLiveFlag(LIVE_FLAG_UNK10000);
		self->mHolder = nullptr;
		self->stopAnmSound();

		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushAfterCurrent(spine->getDefault());

		return true;
	} else {
		return false;
	}
}

DEFINE_NERVE(TNerveSmallEnemyFreeze, TLiveActor)
{
	TSmallEnemy* self = (TSmallEnemy*)spine->getBody();

	int freezeTime = self->getSaveParam2()->getSLFreezeWait();

	if (spine->getTime() == 0)
		self->setFreezeAnm();
	if (self->checkCurAnmEnd(0) && spine->getTime() >= freezeTime)
		return true;
	else
		return false;
}

DEFINE_NERVE(TNerveSmallEnemyJump, TLiveActor)
{
	TSmallEnemy* self = (TSmallEnemy*)spine->getBody();

	if (spine->getTime() == 0) {
		if (self->checkLiveFlag2(0x8000)
		    || self->checkLiveFlag(LIVE_FLAG_UNK40000))
			return true;

		self->jumpBehavior();

		JGeometry::TVec3<f32> v = self->getVelocity();
		v.y = self->getSaveParam2()->getSLJumpForce() * self->getBodyScale();
		self->setVelocity(v);

		self->onLiveFlag(LIVE_FLAG_UNK8000);
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	if (!self->isAirborne())
		return true;
	else
		return false;
}

// TODO: kinda matches below but not for use-cases above...
struct RandInterval {
	volatile f32 min;
	volatile f32 max;

	RandInterval(f32 min, f32 max)
	    : min(min)
	    , max(max)
	{
	}

	f32 get() const
	{
		f32 range = max - min;
		f32 r     = rand() * (1.f / (RAND_MAX + 1));
		f32 r2    = r * range;
		return r2 + min;
	}
};

DEFINE_NERVE(TNerveSmallEnemyHitWaterJump, TLiveActor)
{
	TSmallEnemy* self = (TSmallEnemy*)spine->getBody();

	if (spine->getTime() == 0) {
		if (self->checkLiveFlag2(0x8000)
		    || self->checkLiveFlag(LIVE_FLAG_UNK40000))
			return true;

		self->setWaitAnm();
		self->jumpBehavior();

		JGeometry::TVec3<f32> v = self->mVelocity;
		v.y                     = self->getSaveParam2()->mSLJumpForce.get();
		self->mVelocity         = v;

		self->onLiveFlag(LIVE_FLAG_UNK8000);
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);

		// TODO: random interval class?
		RandInterval ri(30.0f, 100.0f);
		self->mRotation.y += ri.get();
	}

	RandInterval ri(4.0f, 10.0f);
	self->mRotation.y += ri.get();

	if (!self->isAirborne() || spine->getTime() > 360) {
		self->endHitWaterJump();
		return true;
	} else {
		return false;
	}
}

DEFINE_NERVE(TNerveSmallEnemyWait, TLiveActor)
{
	TSmallEnemy* self = (TSmallEnemy*)spine->getBody();

	if (spine->getTime() == 0)
		self->setWaitAnm();

	if (self->checkCurAnmEnd(0)
	    && spine->getTime() > self->getSaveParam2()->getSLWaitTime())
		return true;
	else
		return false;
}

DEFINE_NERVE(TNerveSmallEnemyChange, TLiveActor)
{
	TSmallEnemy* self = (TSmallEnemy*)spine->getBody();

	int changeTime = self->getChangeBlockTime();

	if (spine->getTime() == 0) {
		self->getMActor()->setFrameRate(0.0f, 0);
		gpMarioParticleManager->emitAndBindToPosPtr(0xCD, &self->getPosition(),
		                                            0, nullptr);
	}
	self->scalingChangeActor();
	if (self->changeMove() || spine->getTime() > changeTime) {
		self->changeOut();
		return true;
	} else {
		return false;
	}
}
