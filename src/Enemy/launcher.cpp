#include <Enemy/Launcher.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <System/EmitterViewObj.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

const char* clauncher_bastable[] = {
	nullptr,
	"/scene/clauncher/bas/generator_damage1.bas",
	"/scene/clauncher/bas/generator_dead1.bas",
	nullptr,
};

TLauncherParams::TLauncherParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLLaunchPeriod, 180)
{
	TParams::load(mPrmPath);
}

TLauncher::TLauncher(const char* name)
    : TSpineEnemy(name)
    , mState(2)
    , mNextState(0)
    , mTicksSpentInCurState(0)
    , mLaunchCooldown(0)
    , mRegenTimer(0)
{
}

void TLauncher::init(TLiveManager* param_1)
{
	TSpineEnemy::init(param_1);

	mSpine->initWith(&TNerveWaitForever<TLiveActor>::theNerve());
	TLauncherParams* params = getSaveParam2();
	if (params) {
		s32 launchPeriod = params->mSLLaunchPeriod.get();
		mLaunchCooldown  = launchPeriod * MsRandF();
	}

	mActorType = 0x10000014;
	onHitFlag(0x1000000);
	mAttackRadius = 0.0f;
	mAttackHeight = 0.0f;
	mDamageRadius = 100.0f;
	mDamageHeight = 100.0f;
	calcEntryRadius();
	offHitFlag(0x1);
}

BOOL TLauncher::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return false;

	if (mState == STATE_DIE)
		return false;

	if (param_1->getActorType() == 0x1000001) {
		if (param_2 == HIT_MESSAGE_SPRAYED_BY_WATER) {
			gpMarioParticleManager->emit(0xE7, &mPosition, 0, nullptr);
			gpMSound->startSoundSet(0x6802, &mPosition, 0, 0.0f, 0, 0, 4);
			if (mState == STATE_HITBYWATER)
				return true;

			changeState(1);
			return true;
		}
	}

	return false;
}

void TLauncher::bind() { }

void TLauncher::control()
{
	switch (mState) {
	case STATE_INITIAL:
		stateInitial();
		break;

	case STATE_NORMAL:
		stateNormal();
		break;

	case STATE_HITBYWATER:
		stateHitByWater();
		break;

	case STATE_LAUNCH:
		stateLaunch();
		break;

	case STATE_DIE:
		stateDie();
		break;
	}

	if (mState != mNextState) {
		mState                = mNextState;
		mTicksSpentInCurState = 0;
	} else {
		++mTicksSpentInCurState;
	}
}

void TLauncher::resetLaunchTimer()
{
	TLauncherParams* params = getSaveParam2();
	if (!params) {
		mLaunchCooldown = 0;
	} else {
		mLaunchCooldown = params->getLaunchPeriod();
	}
}

TSpineEnemy* TLauncher::getProperEnemy(const char* param_1)
{
	TEnemyManager* manager
	    = (TEnemyManager*)gpConductor->getManagerByName(param_1);
	if (!manager)
		return nullptr;

	TSpineEnemy* enemy = manager->getFarOutEnemy();
	if (!enemy)
		return nullptr;

	enemy->getTracer()->init(unk124->getGraph());

	return enemy;
}

void TLauncher::changeState(int param_1) { mNextState = param_1; }

void TLauncher::stateInitial()
{
	if (mTicksSpentInCurState > 12)
		changeState(STATE_NORMAL);
}

void TLauncher::stateHitByWater()
{
	if (mTicksSpentInCurState == 0)
		decHitPoints();

	if (mTicksSpentInCurState > 12) {
		if (mHitPoints == 0)
			changeState(STATE_DIE);
		else
			changeState(STATE_NORMAL);
	}
}

void TLauncher::stateNormal()
{
	--mLaunchCooldown;

	if (mLaunchCooldown <= 0)
		changeState(STATE_LAUNCH);
}

void TLauncher::stateLaunch()
{
	if (mTicksSpentInCurState > 12) {
		resetLaunchTimer();
		changeState(STATE_NORMAL);
	}
}

void TLauncher::stateDie()
{
	if (mTicksSpentInCurState > 12) {
		kill();
		resetLaunchTimer();
		changeState(STATE_NORMAL);
	}
}

TLauncherManager::TLauncherManager(const char* name)
    : TEnemyManager(name)
{
}

void TLauncherManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TLauncherParams("/enemy/launcher.prm");
	TEnemyManager::load(stream);
}

TCommonLauncher::TCommonLauncher(const char* name)
    : TLauncher(name)
    , unk164(0)
    , mLaunchPeriod(120)
{
}

void TCommonLauncher::init(TLiveManager* param_1)
{
	mManager = param_1;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("generator_model1.bmd", 0);
	mSpine->initWith(&TNerveWaitForever<TLiveActor>::theNerve());

	mLaunchCooldown = mLaunchPeriod * MsRandF();

	mMActor->setLightType(1);
	initHitActor(0x10000014, 1, -0x7f000000, 150.0f, 100.0f, 150.0f, 100.0f);
	offHitFlag(0x1);

	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);

	onLiveFlag(LIVE_FLAG_UNK8);
	onLiveFlag(LIVE_FLAG_UNK10);

	ResTIMG* res = (ResTIMG*)JKRFileLoader::getGlbResource(
	    "/scene/map/pollution/H_ma_rak.bti");
	if (res)
		SMS_ChangeTextureAll(getModel()->getModelData(), "Q_gene_dummy_I4",
		                     *res);

	initAnmSound();
	getMActor()->offMakeDL();

	mHitPoints = getSaveParam() ? getSaveParam()->mSLHitPointMax.get() : 1;
	mHitPoints = 5;

	mRotation.x = MsWrap(mRotation.x + 270.0f, 0.0f, 360.0f);
}

void TCommonLauncher::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);
	unk164 = stream.readString();
	stream.read(&mLaunchPeriod, 4);
}

void TCommonLauncher::changeBck(int param_1)
{
	mMActor->setBckFromIndex(param_1);
	setCurAnmSound();
}

void TCommonLauncher::stateInitial() { changeState(STATE_NORMAL); }

void TCommonLauncher::stateHitByWater()
{
	if (mTicksSpentInCurState == 0) {
		changeBck(1);
		decHitPoints();
	}

	if (mMActor->curAnmEndsNext()) {
		if (mHitPoints == 0)
			changeState(STATE_DIE);
		else
			changeState(STATE_NORMAL);
	}
}

void TCommonLauncher::stateNormal()
{
	if (mTicksSpentInCurState == 0)
		changeBck(3);

	if (mLaunchCooldown > 0)
		--mLaunchCooldown;

	if (mLaunchCooldown > 0)
		return;

	mLaunchCooldown = mLaunchPeriod;

	TEnemyManager* manager
	    = (TEnemyManager*)gpConductor->getManagerByName(unk164);
	if (!manager)
		return;

	TSpineEnemy* enemy = manager->getFarOutEnemy();
	if (!enemy)
		return;

	changeState(3);
}

void TCommonLauncher::stateLaunch()
{
	if (mTicksSpentInCurState == 0)
		changeBck(0);

	if (mTicksSpentInCurState == 0x3E) {
		TSpineEnemy* enemy = getProperEnemy(unk164);
		if (enemy) {
			JGeometry::TVec3<f32> local_14 = mRotation;

			local_14.x = MsWrap(local_14.x - 270.0f, 0.0f, 360.0f);

			Mtx mtx;
			MsMtxSetRotRPH(mtx, local_14.x, local_14.y, local_14.z);
			JGeometry::TVec3<f32> local_20(0.0f, 4.0f, 0.0f);
			local_14.set(0.0f, 0.0f, 0.0f);
			MTXMultVec(mtx, &local_20, &local_20);
			enemy->resetSRTV(mPosition, local_14, enemy->mScaling, local_20);
		}
	}

	if (mMActor->curAnmEndsNext()) {
		mLaunchCooldown = mLaunchPeriod;
		changeState(STATE_NORMAL);
	}
}

void TCommonLauncher::stateDie()
{
	if (mTicksSpentInCurState == 0)
		changeBck(2);

	if (mMActor->curAnmEndsNext() && mMActor->checkCurBckFromIndex(2)) {
		MtxPtr mtx = getMActor()->getModel()->getAnmMtx(0);

		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(0xD0, mtx, 0,
		                                                  nullptr)) {
			SMSSetEmitterPolColor(emitter, 6);
			emitter->setScale(mScaling);
		}

		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(0xE4, mtx, 0,
		                                                  nullptr)) {
			emitter->setScale(mScaling);
		}

		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(0xE6, mtx, 0,
		                                                  nullptr)) {
			emitter->setScale(mScaling);
		}

		kill();
		onHitFlag(HIT_FLAG_UNK1);
		resetLaunchTimer();
		changeState(STATE_NORMAL);
	}
}

const char** TCommonLauncher::getBasNameTable() const
{
	return clauncher_bastable;
}

void TCommonLauncher::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSpineEnemy::perform(param_1, param_2);
	if ((param_1 & 2) && mMActor->checkCurBckFromIndex(1)) {
		MtxPtr mtx = mMActor->getModel()->getAnmMtx(0);

		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(0x12D, mtx, 1,
		                                                  this)) {
			SMSSetEmitterPolColor(emitter, 6);
			emitter->setScale(mScaling);
		}
	}

	if ((param_1 & 1) && mState == STATE_NORMAL && mHitPoints < 5) {
		mRegenTimer += 1;
		if (mRegenTimer > 1200) {
			mRegenTimer = 0;
			mHitPoints += 1;
		}
	}

	if (param_1 & 1) {
		for (int i = 0; i < mColCount; ++i)
			if (mCollisions[i]->isActorType(0x80000001))
				SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
	}
}

TCommonLauncherManager::TCommonLauncherManager(const char* name)
    : TLauncherManager(name)
{
}

void TCommonLauncherManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "generator_model1.bmd", 0x11240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TCommonLauncherManager::initJParticle() { }

void TCommonLauncherManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
}
