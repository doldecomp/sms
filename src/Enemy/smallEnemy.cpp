#include <Enemy/SmallEnemy.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
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
	unk58 = gpPollution->getCounterLayer().registerTexStamp(1, unk10, img);
}

void TSmallEnemyManager::createEnemies(int param_1)
{
	TEnemyManager::createEnemies(param_1);
	initSetEnemies();
}

TSmallEnemy* TSmallEnemyManager::getHolder(int param_1)
{
	for (int i = 0; i < getActiveObjNum(); ++i)
		if (i != param_1 && getObj(i)->checkLiveFlag(0x4))
			return (TSmallEnemy*)getObj(i);
	return nullptr;
}

TSmallEnemy::TSmallEnemy(const char* name)
    : TSpineEnemy(name)
    , unk150(0)
    , unk154(1.0f)
    , unk158(1.0f)
    , unk15C(0xffffffff)
    , unk160(0)
    , unk164(0)
    , unk165(0)
    , unk174(0)
    , unk178(0)
    , unk17C(0xffffffff)
    , unk180(0)
    , unk184(0)
    , unk185(0)
    , unk188(1.0f)
    , unk18C(0)
{
}

void TSmallEnemy::setMActorAndKeeper()
{
	unk78 = new TMActorKeeper(getManager(), 1);
	unk74 = getActorKeeper()->createMActorFromNthData(0, 0);
}

static inline f32 randf(f32 l, f32 r)
{
	return rand() * (1.f / (RAND_MAX + 1)) * (r - l) + l;
}

void TSmallEnemy::init(TLiveManager* param_1)
{
	mManager = param_1;
	param_1->manageActor(this);

	setMActorAndKeeper();

	onLiveFlag(0x1);
	if (gpMarDirector->mMap == 2 && gpMarDirector->unk7D == 0)
		onLiveFlag(0x2000);
	onHitFlag(0x1);
	unk158 = 1.0f;

	// TODO: are these f32 pairs some kind of rng interval class?
	TSmallEnemyParams* params1 = getSaveParam();
	mTurnSpeed                 = randf(params1->unk2C4, params1->unk2C8);

	TSmallEnemyParams* params2 = getSaveParam();
	mBodyScale                 = randf(params2->unk2D0, params2->unk2CC);

	unk154 = mBodyScale;
	unkBC  = getSaveParam()->mSLBodyRadius.get();
	unk14C = getSaveParam()->mSLWallRadius.get();
	unkC0  = getSaveParam()->mSLHeadHeight.get();
	unkB8  = mBodyScale * unkBC * 15.0f;

	f32 attackRadius = getSaveParam()->mSLAttackRadius.get();
	f32 attackHeight = getSaveParam()->mSLAttackHeight.get();
	f32 damageRadius = getSaveParam()->mSLDamageRadius.get();
	f32 damageHeight = getSaveParam()->mSLDamageHeight.get();

	initHitActor(0, 5, -0x68000000, attackRadius * mBodyScale,
	             attackHeight * mBodyScale, damageRadius * mBodyScale,
	             damageHeight * mBodyScale);

	unkC4 = TMap::getIllegalCheckData();
	if (!unk124->unk0 || unk124->unk0->isDummy())
		unk124->unk0 = gpConductor->getGraphByName("main");

	// ffs gpMarioAddress??? REALLY?
	unk114.clear();
	initAnmSound();
}

void TSmallEnemy::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);
	stream.read(&unk17C, 4);
	if (unk17C != 0x65)
		unk18C = 1;
	reset();
}

void TSmallEnemy::loadAfter()
{
	unk180 = TItemManager::newAndRegisterCoin(unk17C);
}

void TSmallEnemy::behaveToWater(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() != &TNerveSmallEnemyFreeze::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()) {
		mSpine->pushNerve(&TNerveSmallEnemyFreeze::theNerve());
	}
}

void TSmallEnemy::sendAttackMsgToMario() { SMS_SendMessageToMario(this, 0xE); }

void TSmallEnemy::attackToMario()
{
	sendAttackMsgToMario();

	// TODO: wtf
	volatile JGeometry::TVec3<f32> local_14(0, 0, 0);

	JGeometry::TVec3<f32> local_20;
	local_20.sub(mPosition, *gpMarioPos);
	MsVECNormalize(&local_20, &local_20);
	unkAC.set(local_20);

	JGeometry::TVec3<f32> v;
	v.scale(mBodyScale * unkBC, local_20);
	v += local_14;
	unk94 = v;
}

void TSmallEnemy::reset()
{
	TSpineEnemy::reset();

	TSmallEnemyParams* params1 = getSaveParam();
	mTurnSpeed                 = randf(params1->unk2C4, params1->unk2C8);

	TSmallEnemyParams* params2 = getSaveParam();
	mBodyScale                 = randf(params2->unk2D0, params2->unk2CC);

	unk154 = mBodyScale;
	unkBC  = getSaveParam()->mSLBodyRadius.get();
	unk14C = getSaveParam()->mSLWallRadius.get();
	unkC0  = getSaveParam()->mSLHeadHeight.get();
	unkB8  = mBodyScale * unkBC * 15.0f;

	mHitPoints = getSaveParam() ? getSaveParam()->mSLHitPointMax.get() : 1;

	offLiveFlag(0x1);
	offLiveFlag(0x20000);
	offLiveFlag(0x10000);
	offLiveFlag(0x2);

	if (getSaveParam()->mSLGenerateOnlyDead.get())
		offLiveFlag(0x800);
	else
		onLiveFlag(0x800);

	unk124->mPrevIdx = -1;
	goToShortestNextGraphNode();
	offHitFlag(0x1);
	mSpine->reset();
	mSpine->pushDefault();
	mScaling.set(mBodyScale, mBodyScale, mBodyScale);

	unk160 = 0;
	unk165 = 0;
	unk184 = 0;

	f32 attackRadius = getSaveParam()->mSLAttackRadius.get();
	f32 attackHeight = getSaveParam()->mSLAttackHeight.get();
	f32 damageRadius = getSaveParam()->mSLDamageRadius.get();
	f32 damageHeight = getSaveParam()->mSLDamageHeight.get();

	mAttackRadius = attackRadius * mBodyScale;
	mAttackHeight = attackHeight * mBodyScale;
	mDamageRadius = damageRadius * mBodyScale;
	mDamageHeight = damageHeight * mBodyScale;

	calcEntryRadius();

	unk164 = 0;

	MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x, mPosition.y,
	               mPosition.z, mRotation.x * (65536.0f / 360.0f),
	               mRotation.y * (65536.0f / 360.0f),
	               mRotation.z * (65536.0f / 360.0f));
	getUnk74()->calc();
}

void TSmallEnemy::forceKill()
{
	if ((unkC4->checkFlag2(0x10)
	     || (!(unkC4->unk0 == 0x800 ? true : false) && !unkC4->checkSomething2()
	         && !unkC4->checkSomething())
	     || checkLiveFlag2(0x80) || checkLiveFlag(0x10))
	    && !gpMap->isInArea(mPosition.x, mPosition.z)) {

		if (mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()) {
			mSpine->reset();
			mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
			mSpine->pushDefault();

			onLiveFlag(0x20000);
			mHitPoints = 1;
		}
	}
}

void TSmallEnemy::genRandomItem()
{
	if (!getSaveParam())
		return;

	setAfterDeadEffect();
	genEventCoin();
	if (!checkLiveFlag(0x20001))
		generateItem();
}

void TSmallEnemy::genEventCoin()
{
	if (checkLiveFlag(0x20000))
		return;

	if (unk180) {
		TCoin* coin;
		if (isActorType(0x2000000E)) {
			coin = (TCoin*)gpItemManager->makeObjAppear(0x2000000E);
		} else {
			coin = unk180;
			coin->appear();
		}

		if (coin) {
			coin->mPosition = mPosition;
			coin->unkAC.set(0, 20, 0);
			coin->offLiveFlag(0x10);
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
				// TODO: is this randf thing a method on some RandInterval
				// class?
				coin->unkAC.set(local_d0.x * 4, randf(16.0f, 8.0f),
				                local_d0.z * 4);
				coin->offLiveFlag(0x10);
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
	if (randf(0.0f, 100.0f) < getSaveParam()->mSLGenEggRate.get()
	                              + getSaveParam()->mSLGenItemRate.get()

	    && !unkC4->checkFlag2(0x10))
		gpMapObjManager->makeObjAppear(mPosition.x, unkC8, mPosition.z,
		                               0x20000008, true);
}

bool TSmallEnemy::isCollidMove(THitActor* param_1)
{
	if ((param_1->mActorType & 0xffff0000) == 0x40000000)
		return false;

	if (!((TLiveActor*)param_1)->checkLiveFlag2(0x80) && !checkLiveFlag2(0x80))
		return true;

	return false;
}

void TSmallEnemy::moveObject()
{
	if (checkLiveFlag(0x1))
		return;

	f32 attackRadius = getSaveParam()->mSLAttackRadius.get() * mBodyScale;
	f32 attackHeight = getSaveParam()->mSLAttackHeight.get() * mBodyScale;
	f32 damageRadius = getSaveParam()->mSLDamageRadius.get() * mBodyScale;
	f32 damageHeight = getSaveParam()->mSLDamageHeight.get() * mBodyScale;

	mAttackRadius = attackRadius;
	mAttackHeight = attackHeight;
	mDamageRadius = damageRadius;
	mDamageHeight = damageHeight;

	calcEntryRadius();
	ensureTakeSituation();
	unk94.zero();
	unkA0.zero();
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
		unk94 = v;
	}

	bind();
	forceKill();
	setBehavior();

	mPosition += unk94;
	mRotation += unkA0;

	if (unk160 > 0)
		++unk160;

	if (unk160 > 0x1E)
		unk160 = 0;

	if (unkEC && unkEC->getUnk8())
		unkEC->getUnk8()->moveSRT(mPosition, mRotation, mScaling);

	if (!isInhibitedForceMove())
		calcRidePos();
}

void TSmallEnemy::updateAnmSound() { TSpineEnemy::updateAnmSound(); }

BOOL TSmallEnemy::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (isEatenByYosshi() && param_2 == 4 && !unk68) {
		onHitFlag(0x1);
		unk68 = (TTakeActor*)param_1;
		behaveToTaken(param_1);
		return true;
	}

	if ((param_2 == 6 || param_2 == 7) && unk68 == param_1) {
		unk68 = nullptr;
		behaveToRelease();
		offHitFlag(0x1);
		return true;
	}

	if (param_2 == 0 || param_2 == 1 || param_2 == 3 || param_2 == 11
	    || (mActorType == 0x10000021 && param_2 == 0xC)) {
		if (isHitValid(param_2)) {
			unk184 = 0;
			kill();
		}
		return true;
	}

	if (param_2 == 13) {
		mHitPoints = 0;
		onLiveFlag(0x1);
		onHitFlag(0x1);
	}

	if (param_2 == 15) {
		gpMarioParticleManager->emit(0xE7, &param_1->mPosition, 0, nullptr);
		gpMSound->startSoundSet(0x6802, &mPosition, 0, 0.0f, 0, 0, 4);
		if (unk160 == 0) {
			unk160 = 1;
			if (!changeByJuice()) {
				decHpByWater(param_1);
				behaveToWater(param_1);
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

		if (unk178)
			return true;

		TJuiceBlock* block = (TJuiceBlock*)gpMapObjManager->makeObjAppear(
		    mPosition.x, mPosition.y, mPosition.z, 0x400002C6, true);
		unk178 = block;

		if (!unk178)
			return false;

		unk178->mScaling.set(0.1f, 0.1f, 0.1f);
		unk178->unk140.set(0.0f, 0.0f, 0.0f);
		unk178->mRotation.set(0.0f, mRotation.y, 0.0f);

		unk178->unk14C = this;
		unk178->offLiveFlag(0x2);
		onHitFlag(0x1);
		onLiveFlag(0x2);
		onLiveFlag(0x10);

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

		offLiveFlag(0x800);
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
	if (TSmallEnemyManager::mBlockWaitTime * 0.2f <= mSpine->getUnk20()) {
		f32 time = TSmallEnemyManager::mBlockWaitTime * 0.2f;

		unk178->mPosition.y += unk188 * 2.0f
		                       * JMASin(mSpine->getUnk20() * 130.0f / time)
		                       * TSmallEnemyManager::mBlockWaitMoveY;

		unk178->mRotation.y += mSpine->getUnk20() * 1080.0f / time;
	} else {
		if (mSpine->getUnk20() > TSmallEnemyManager::mBlockWaitTime) {
			if (mSpine->getUnk20() > getChangeBlockTime() - 200) {
				if (mSpine->getUnk20() % 20 < 10) {
					unk178->onLiveFlag(0x2);
				} else {
					unk178->offLiveFlag(0x2);
				}
			}

			switch (unk185) {
			case 2: {
				JGeometry::TVec3<f32> local_38(0.0f, 0.0f, 1.0f);
				Mtx afStack_68;
				MsMtxSetRotRPH(afStack_68, 0.0f, mRotation.y, 0.0f);
				MTXMultVec(afStack_68, &local_38, &local_38);
				unk178->mPosition.x
				    += local_38.x * TSmallEnemyManager::mBlockMoveSpeed;
				unk178->mPosition.z
				    += local_38.z * TSmallEnemyManager::mBlockMoveSpeed;

				if (gpMap->isTouchedOneWallAndMoveXZ(
				        &unk178->mPosition.x, unk178->mPosition.y,
				        &unk178->mPosition.z, unkBC * 20.0f))
					return 1;

				JGeometry::TVec3<f32> local_74 = unk178->mPosition;
				local_74.x += local_38.x * 300.0f;
				local_74.z += local_38.z * 300.0f;

				const TBGCheckData* local_2C;
				f32 d = gpMap->checkGround(local_74.x, local_74.y + unkC0,
				                           local_74.z, &local_2C);
				if (d > unk178->mPosition.y)
					return 1;
				break;
			}

			case 3: {
				unk178->mPosition.y += TSmallEnemyManager::mBlockMoveSpeed;
				const TBGCheckData* local_2C;
				f32 d = gpMap->checkRoof(unk178->mPosition.x,
				                         unk178->mPosition.y + unkC0,
				                         unk178->mPosition.z, &local_2C);
				if (local_2C && unk178->mPosition.y + unkC0 > d
				    && local_2C->unk44 != unk178)
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
			if (mSpine->mTimer % 30 > 15) {
				unk178->mPosition.y += 0.5f;
				unk178->mPosition.x += local_80.x;
				unk178->mPosition.z += local_80.z;
			} else {
				unk178->mPosition.y -= 0.5f;
				unk178->mPosition.x -= local_80.x;
				unk178->mPosition.z -= local_80.z;
			}
		}
	}

	return 0;
}

void TSmallEnemy::scalingChangeActor()
{
	f32 xzScale = MsClamp(unk178->unk140.x + 0.02f, 0.0f,
	                      TSmallEnemyManager::mBlockXZScale);

	unk178->unk140.x = unk178->unk140.z = xzScale;
	unk178->mScaling.x = unk178->mScaling.z = xzScale;

	f32 yScale         = MsClamp(unk178->unk140.y + 0.01f, 0.0f,
	                             TSmallEnemyManager::mBlockYScale);
	unk178->unk140.y   = yScale;
	unk178->mScaling.y = yScale;
}

void TSmallEnemy::changeOut()
{
	if (gpMSound->gateCheck(0x293D))
		MSoundSESystem::MSoundSE::startSoundActor(0x293D, &mPosition, 0,
		                                          nullptr, 0, 4);

	kill();
	unk178->mPosition = mPosition;

	gpMarioParticleManager->emitAndBindToPosPtr(0xCD, &mPosition, 0, nullptr);
	getUnk74()->setFrameRate(SMSGetAnmFrameRate(), 0);
	unk178->kill();
	unk178 = nullptr;
}

void TSmallEnemy::decHpByWater(THitActor* param_1)
{
	// TODO: not actually a TWaterHItActor, IDK what it is
	s16 uVar2 = gpModelWaterManager->unk614[((TWaterHitActor*)param_1)->unk68];
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
	if (!checkLiveFlag(0x1))
		return;

	mHitPoints = 1;
	if (mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()) {
		mSpine->reset();
		mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
		mSpine->pushRaw(&TNerveSmallEnemyDie::theNerve());

		onLiveFlag(0x40);
	}
}

bool TSmallEnemy::isFindMario(float param_1)
{
	bool b = true;
	if (gpMarDirector->unk124 != 3 && gpMarDirector->unk124 != 4)
		b = false;
	if (b)
		return false;

	if (checkLiveFlag2(0x80))
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
	TSmallEnemyParams* prms = getSaveParam();
	if (fabsf(gpMarioPos->y - mPosition.y) < prms->mSLSearchHeight.get()) {

		JGeometry::TVec3<f32> marioPos(*gpMarioPos);
		if (isInSight(marioPos, prms->mSLSearchLength.get() * param_1,
		              prms->mSLSearchAngle.get() * param_1,
		              prms->mSLSearchAware.get() * param_1))
			return true;
		else
			return false;
	}

	return false;
}
#pragma dont_inline off

void TSmallEnemy::generateEffectColumWater()
{
	if (!checkLiveFlag(0x4))
		return;

	TLiveActor* enemy = gpConductor->makeOneEnemyAppear(
	    mPosition, "エフェクト水柱マネージャー", 0);
	// TODO: need TEffectColumWater
}

void TSmallEnemy::setBckAnm(int param_1)
{
	unk15C = param_1;
	getUnk74()->setBckFromIndex(param_1);
	const char** table = getBasNameTable();
	setAnmSound(!table ? nullptr : table[param_1]);
}

void TSmallEnemy::expandCollision()
{
	f32 attackRadius = getSaveParam()->mSLAttackRadius.get();
	f32 attackHeight = getSaveParam()->mSLAttackHeight.get();
	f32 damageRadius = getSaveParam()->mSLDamageRadius.get();
	f32 damageHeight = getSaveParam()->mSLDamageHeight.get();

	mAttackRadius = attackRadius * (unk190 / unk154);
	mAttackHeight = attackHeight * (unk190 / unk154);
	mDamageRadius = damageRadius * (unk190 / unk154);
	mDamageHeight = damageHeight * (unk190 / unk154);

	calcEntryRadius();
}

bool TSmallEnemy::isEaten()
{
	MtxPtr mtx;
	if (unk68 && unk68->unk6C == this && (mtx = getTakingMtx())) {
		getModel()->setBaseTRMtx(mtx);
		mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);

		return true;
	} else {
		return false;
	}
}

bool TSmallEnemy::isHitWallInBound()
{
	unk14C = 25.0f;
	TBGWallCheckRecord local_3C(mPosition.x, mPosition.y + unkC0, mPosition.z,
	                            unk14C * mBodyScale * 1.1f, 1, 0);

	if (gpMap->isTouchedWallsAndMoveXZ(&local_3C)) {
		f32 sVar2
		    = matan(local_3C.unk1C[0]->mNormal.z, local_3C.unk1C[0]->mNormal.x)
		      * (360.0f / 65536.0f);

		JGeometry::TVec3<f32> v(unkAC.x, 0.0f, unkAC.z);
		if (v.dot(local_3C.unk1C[0]->mNormal) > 0.0f)
			return false;

		mPosition.x = local_3C.unk0.x;
		mPosition.z = local_3C.unk0.z;
		mRotation.y = sVar2;

		onLiveFlag(0x10);
		onHitFlag(0x1);

		return true;
	}

	return false;
}

void TSmallEnemy::behaveToHitOthers(THitActor* param_1)
{
	if (isCollidMove(param_1))
		return;

	JGeometry::TVec3<f32> local_18(0.0f, 0.0f, 0.0f);

	JGeometry::TVec3<f32> local_24;
	local_24.sub(mPosition, param_1->mPosition);
	if (local_24.x == 0.0f && local_24.y == 0.0f && local_24.z == 0.0f)
		local_24.x += 1.0f;
	MsVECNormalize(&local_24, &local_24);

	unk94.scaleAdd(mMarchSpeed * 3.0f * unk158, local_18, local_24);
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
	TSmallEnemy* self = (TSmallEnemy*)spine->unk0;
	if (spine->mTimer == 0) {
		if (self->mHitPoints != 0)
			--self->mHitPoints;

		if (self->mHitPoints == 0) {
			self->onLiveFlag(0x1);
			if (self->unkC4->checkSomething2() && !self->checkLiveFlag2(0x80))
				self->kill();
		}

		if (self->checkLiveFlag(0x10000)) {
			self->setMeltAnm();
		} else {
			if (self->unk150 & 0x20) {
				TSmallEnemyManager* man = (TSmallEnemyManager*)self->mManager;
				gpPollution->stamp(man->getUnk58(), self->mPosition.x,
				                   self->mPosition.y, self->mPosition.z,
				                   man->getSaveParam()->mSLStampRange.get()
				                       * 32.0f * self->getUnk158());
			}
			if (!self->mHitPoints) {
				self->onLiveFlag(0x8);
				self->setDeadAnm();
				self->setDeadEffect();
			} else {
				return true;
			}
		}
	}

	int uVar8 = ((TSmallEnemyManager*)self->mManager)->unk5C;
	if (self->unkC4->checkSomething())
		uVar8 = 0;

	// TODO: missing some stuff
	if (self->checkCurAnmEnd(0)
	        && spine->getUnk20()
	               > uVar8 + self->getUnk74()->getFrameCtrl(0)->getEndFrame()
	    || spine->getUnk20() > 360 || self->getUnk184() != 0) {
		self->genRandomItem();
		self->onHitFlag(0x1);
		self->onLiveFlag(0x1);
		self->onLiveFlag(0x8);
		self->offLiveFlag(0x2);
		self->offLiveFlag(0x10000);
		self->unk68 = nullptr;
		self->stopAnmSound();

		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushDefault();

		return true;
	} else {
		return false;
	}
}

DEFINE_NERVE(TNerveSmallEnemyFreeze, TLiveActor)
{
	TSmallEnemy* self = (TSmallEnemy*)spine->getBody();

	int freezeTime = self->getSaveParam()->mSLFreezeWait.get();

	if (spine->getUnk20() == 0)
		self->setFreezeAnm();
	if (self->checkCurAnmEnd(0) && spine->getUnk20() >= freezeTime)
		return true;
	else
		return false;
}

DEFINE_NERVE(TNerveSmallEnemyJump, TLiveActor)
{
	TSmallEnemy* self = (TSmallEnemy*)spine->getBody();

	if (spine->getUnk20() == 0) {
		if (self->checkLiveFlag2(0x8000) || self->checkLiveFlag(0x40000))
			return true;

		self->jumpBehavior();

		JGeometry::TVec3<f32> v = self->unkAC;
		v.y *= self->getSaveParam()->mSLJumpForce.get();
		self->unkAC = v;

		self->onLiveFlag(0x8000);
		self->onLiveFlag(0x80);
	}

	if (!self->checkLiveFlag2(0x80))
		return true;
	else
		return false;
}

DEFINE_NERVE(TNerveSmallEnemyHitWaterJump, TLiveActor)
{
	TSmallEnemy* self = (TSmallEnemy*)spine->getBody();

	if (spine->getUnk20() == 0) {
		if (self->checkLiveFlag2(0x8000) || self->checkLiveFlag(0x40000))
			return true;

		self->setWaitAnm();
		self->jumpBehavior();

		JGeometry::TVec3<f32> v = self->unkAC;
		v.y                     = self->getSaveParam()->mSLJumpForce.get();
		self->unkAC             = v;

		self->onLiveFlag(0x8000);
		self->onLiveFlag(0x80);

		// TODO: random interval class?
		self->mRotation.y += randf(30.0f, 100.0f);
	}

	self->mRotation.y += randf(4.0f, 10.0f);

	if (!self->checkLiveFlag2(0x80) || spine->getUnk20() > 360) {
		self->endHitWaterJump();
		return true;
	} else {
		return false;
	}
}

DEFINE_NERVE(TNerveSmallEnemyWait, TLiveActor)
{
	TSmallEnemy* self = (TSmallEnemy*)spine->getBody();

	if (spine->getUnk20() == 0)
		self->setWaitAnm();

	if (self->checkCurAnmEnd(0)
	    && spine->getUnk20() > self->getSaveParam()->mSLWaitTime.get())
		return true;
	else
		return false;
}

DEFINE_NERVE(TNerveSmallEnemyChange, TLiveActor)
{
	TSmallEnemy* self = (TSmallEnemy*)spine->getBody();

	int changeTime = self->getChangeBlockTime();

	if (spine->getUnk20() == 0) {
		self->getUnk74()->setFrameRate(0.0f, 0);
		gpMarioParticleManager->emitAndBindToPosPtr(0xCD, &self->mPosition, 0,
		                                            nullptr);
	}
	self->scalingChangeActor();
	if (self->changeMove() || spine->getUnk20() > changeTime) {
		self->changeOut();
		return true;
	} else {
		return false;
	}
}
