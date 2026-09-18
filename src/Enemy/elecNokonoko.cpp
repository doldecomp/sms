#include <Enemy/ElecNokonoko.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <Map/MapData.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <Camera/Camera.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <Player/MarioAccess.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// dennoko_bastable names ten of the model's eighteen .bck slots. The rest are
// recovered from the call sites and from the alphabetical order of the .bck
// files: the turn1_end/loop/start triple at 14..16 and the run1_loop/start
// pair at 10/11 both line up with the animation each nerve asks for, and 17 is
// the wait the idle nerve plays.
// TODO: 4, 5, 8 and 9 have no .bas file and no name; what they are is only a
// guess from where they get played.
enum {
	DENNOKO_ANM_CATCH1        = 0,  // "dennoko_catch1"
	DENNOKO_ANM_DOWN1         = 1,  // "dennoko_down1"
	DENNOKO_ANM_ELEC_DOWN1    = 2,  // "dennoko_elec_down1"
	DENNOKO_ANM_HIT1          = 3,  // "dennoko_hit1"
	DENNOKO_ANM_UNK4          = 4,
	DENNOKO_ANM_MOGAKI_BARE   = 5,  // struggling with no shell on
	DENNOKO_ANM_MOGAKI1_LOOP  = 6,  // "dennoko_mogaki1_loop"
	DENNOKO_ANM_MOGAKI1_START = 7,  // "dennoko_mogaki1_start"
	DENNOKO_ANM_COLLECT       = 8,  // walking over to the thrown shell
	DENNOKO_ANM_READY         = 9,  // winding up before the throw
	DENNOKO_ANM_RUN1_LOOP     = 10, // "dennoko_run1_loop"
	DENNOKO_ANM_RUN1_START    = 11,
	DENNOKO_ANM_SHOOT1        = 12, // "dennoko_shoot1"
	DENNOKO_ANM_SUPPLY1       = 13, // "dennoko_supply1"
	DENNOKO_ANM_TURN1_END     = 14,
	DENNOKO_ANM_TURN1_LOOP    = 15, // "dennoko_turn1_loop"
	DENNOKO_ANM_TURN1_START   = 16,
	DENNOKO_ANM_WAIT1         = 17,
};

static const char* dennoko_bastable[] = {
	"/scene/dennoko/bas/dennoko_catch1.bas",
	"/scene/dennoko/bas/dennoko_down1.bas",
	"/scene/dennoko/bas/dennoko_elec_down1.bas",
	"/scene/dennoko/bas/dennoko_hit1.bas",
	nullptr,
	nullptr,
	"/scene/dennoko/bas/dennoko_mogaki1_loop.bas",
	"/scene/dennoko/bas/dennoko_mogaki1_start.bas",
	nullptr,
	nullptr,
	"/scene/dennoko/bas/dennoko_run1_loop.bas",
	nullptr,
	"/scene/dennoko/bas/dennoko_shoot1.bas",
	"/scene/dennoko/bas/dennoko_supply1.bas",
	nullptr,
	"/scene/dennoko/bas/dennoko_turn1_loop.bas",
	nullptr,
	nullptr,
};

u8 TElecNokonoko::mReflectSw      = true;
u8 TElecNokonoko::mCarapaceJntIndex = 0;

// UNUSED, 0x60 in the map, and nothing references it: the koopa emits the
// shock particle at its own position instead of going through a helper.
// TODO: incorrect size.
static void createNokonokoThunder(JGeometry::TVec3<f32> pos)
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_DNK_SHIBIRE_B,
	                                            &pos, 0, nullptr);
}

// UNUSED, 0x1b8 in the map: inlined into TElecNokonokoManager::load, which is
// its only caller.
TElecNokonokoSaveLoadParams::TElecNokonokoSaveLoadParams(const char* prm)
    : TWalkerEnemyParams(prm)
    , PARAM_INIT(mSLReadyTime, 300)
    , PARAM_INIT(mSLCarapaceGravity, 0.01f)
    , PARAM_INIT(mSLCarapaceSpeed, 5.0f)
    , PARAM_INIT(mSLCarapaceTurnSpeed, 2.0f)
    , PARAM_INIT(mSLCarapaceSpinSpeed, 2.0f)
    , PARAM_INIT(mSLCarapaceShootRange, 500.0f)
    , PARAM_INIT(mSLCarapaceFlyDist, 100.0f)
{
	TParams::load(mPrmPath);
}

TElecNokonokoManager::TElecNokonokoManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TElecNokonokoManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);

	unk38 = new TElecNokonokoSaveLoadParams("/enemy/elecNokonoko.prm");

	mMaterialTable = J3DModelLoaderDataBase::loadMaterialTable(
	    JKRFileLoader::getGlbResource("/scene/dennoko/dennoko_model1.bmt"));
}

void TElecNokonokoManager::initSetEnemies() { }

TSpineEnemy* TElecNokonokoManager::createEnemyInstance()
{
	return new TElecNokonoko("電気ノコノコ");
}

void TElecNokonokoManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "dennoko_model1.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TElecNokonokoManager::clipEnemies(JDrama::TGraphics* graphics)
{
	f32 clipRadius;
	f32 farClip;
	if (unk38 == nullptr) {
		clipRadius = 300.0f;
		farClip    = gpConductor->getCondParams().getEnemyFarClip();
	} else {
		farClip    = unk38->mSLFarClip.get();
		clipRadius = unk38->mSLClipRadius.get();
	}

	SetViewFrustumClipCheckPerspective(gpCamera->getFovy(),
	                                   gpCamera->getAspect(),
	                                   graphics->mNearPlane, farClip);

	for (int i = 0; i < getObjNum(); ++i) {
		TElecNokonoko* nokonoko = getObj(i);

		if (ViewFrustumClipCheck(graphics, &nokonoko->mPosition, clipRadius))
			nokonoko->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		else
			nokonoko->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);

		// The shell has its own clip state, but only once it has been thrown.
		if (!nokonoko->getCarapace()->isUnk150Zero()) {
			if (ViewFrustumClipCheck(
			        graphics, &nokonoko->getCarapace()->mPosition, clipRadius))
				nokonoko->getCarapace()->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
			else
				nokonoko->getCarapace()->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		}
	}
}

void TElecNokonokoManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TEnemyManager::perform(cue, graphics);

	for (int i = 0; i < getActiveObjNum(); ++i)
		getObj(i)->getCarapace()->perform(cue, graphics);
}

TElecNokonoko::TElecNokonoko(const char* name)
    : TWalkerEnemy(name)
    , mCarapace(nullptr)
    , unk198(0)
    , mHasCarapace(0)
{
}

void TElecNokonoko::init(TLiveManager* live_manager)
{
	TWalkerEnemy::init(live_manager);

	mActorType  = 0x1000000A;
	unk150      = DENNOKO_ANM_WAIT1;
	mSaveParams = (TElecNokonokoSaveLoadParams*)getSaveParam();

	mCarapace = new TElecCarapace("ノコノコ甲羅");
	mSpine->initWith(&TNerveWalkerGraphWander::theNerve());
	mCarapace->loadInit(this, "koura_model1.bmd");

	MActor* carapaceActor = mCarapace->getMActor();
	carapaceActor->getModel()->getModelData()->setMaterialTable(
	    ((TElecNokonokoManager*)mManager)->mMaterialTable,
	    (J3DMaterialCopyFlag)3);
	carapaceActor->initDL();
	carapaceActor->getModel()->lock();

	mReadyTimer = TMsRange<s32>(0, 300).rand();

	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TElecNokonoko::rest()
{
	TWalkerEnemy::reset();
	mScaledBodyRadius = 140.0f;
}

void TElecNokonoko::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	rest();
}

void TElecNokonoko::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("dennoko_model1.bmd", 3);

	MActor* actor = mMActor;
	actor->getModel()->getModelData()->setMaterialTable(
	    ((TElecNokonokoManager*)mManager)->mMaterialTable,
	    (J3DMaterialCopyFlag)3);
	actor->initDL();
	actor->getModel()->lock();
}

void TElecNokonoko::moveObject()
{
	TWalkerEnemy::moveObject();

	if (isBckAnm(DENNOKO_ANM_RUN1_START) && checkCurAnmEnd(0))
		setBckAnm(DENNOKO_ANM_RUN1_LOOP);
}

void TElecNokonoko::attackToMario()
{
	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		return;

	TSmallEnemy::attackToMario();

	if (mSpine->getCurrentNerve() != &TNerveElecNokonokoAttack::theNerve()
	    && mSpine->getCurrentNerve()
	           != &TNerveElecNokonokoCollect::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveElecNokonokoShoot::theNerve()
	    && !hasCarapace())
		mSpine->pushNerve(&TNerveElecNokonokoAttack::theNerve());
}

void TElecNokonoko::calcRootMatrix()
{
	TSpineEnemy::calcRootMatrix();

	if (hasCarapace()) {
		// Sparks crawl over the shell on the koopa's back whenever it is
		// wearing it and not being shaken about.
		if (mSpine->getCurrentNerve()
		        != &TNerveElecNokonokoFreeze::theNerve()
		    && mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()
		    && mSpine->getCurrentNerve()
		           != &TNerveElecNokonokoCollect::theNerve()) {
			gpMSound->startSoundActor(MSD_SE_EN_DENNOKO_SPARK1, &mPosition);

			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToMtxPtr(
			        PARTICLE_MS_DNK_BIRI, getMActor()->getModel()->getAnmMtx(7),
			        1, this);
			if (emitter)
				emitter->setGlobalScale(mScaling);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_DNK_SPARK_L, getMActor()->getModel()->getAnmMtx(7), 1,
			    this);
			if (emitter)
				emitter->setGlobalScale(mScaling);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_DNK_SPARK_R, getMActor()->getModel()->getAnmMtx(7), 1,
			    this);
			if (emitter)
				emitter->setGlobalScale(mScaling);
		}
	}

	if (isBckAnm(DENNOKO_ANM_ELEC_DOWN1)) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_DNK_SHIBIRE_A, getMActor()->getModel()->getAnmMtx(0),
		    1, this);
		if (emitter)
			emitter->setGlobalScale(mScaling);

		MtxPtr head = getMActor()->getModel()->getAnmMtx(8);
		mEffectPos.set(head[0][3], head[1][3], head[2][3]);

		emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_DNK_HIBANA, &mEffectPos, 1, this);
		if (emitter)
			emitter->setGlobalScale(mScaling);

		if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(72.0f)) {
			emitter = gpMarioParticleManager->emitAndBindToPosPtr(
			    PARTICLE_MS_BOMB_LIMIT, &mEffectPos, 1, this);
			if (emitter)
				emitter->setGlobalScale(mScaling);
		}
	}
}

void TElecNokonoko::sendAttackMsgToMario()
{
	if (mHasCarapace == 0)
		SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK);
	else
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

BOOL TElecNokonoko::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_UNKD || message == HIT_MESSAGE_UNKB) {
		onLiveFlag(LIVE_FLAG_DEAD);
		kill();
		mCarapace->kill();
	}

	if (message == HIT_MESSAGE_TAKE && mHolder == nullptr) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mHolder = (TTakeActor*)sender;
		return TRUE;
	}

	if ((message == HIT_MESSAGE_PUT || message == HIT_MESSAGE_THROWN)
	    && mHolder == sender) {
		mHolder = nullptr;
		return TRUE;
	}

	if (message == HIT_MESSAGE_TRAMPLE) {
		// Stomped: with the shell on, the shock still gets Mario; without it
		// the koopa goes down.
		if (mHasCarapace == 1) {
			mHitPoints = 1;
			kill();
			return TRUE;
		}
		SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK);
		return FALSE;
	}

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (!changeByJuice())
			behaveToWater(sender);
		else
			mCarapace->kill();
		return TRUE;
	}

	return FALSE;
}

bool TElecNokonoko::isResignationAttack()
{
	f32 range = mSaveParams->getSLCarapaceShootRange();

	if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		// TODO: 82.5%. The retail object copies the goal point into a local,
		// subtracts mPosition into it component by component and only then
		// squares and calls TUtil<f32>::sqrt. Spelling that out (`TVec3 d =
		// point; d -= mPosition; d.length()`) reproduces the stores but
		// expands sqrt, while distance() keeps the `bl sqrt` and drops the
		// stores; no spelling found so far gives both.
		if (unk104.getPoint().distance(mPosition) < range) {
			mSpine->pushAfterCurrent(&TNerveElecNokonokoShoot::theNerve());
			return true;
		}
	}

	return false;
}

void TElecNokonoko::behaveToFindMario()
{
	mSpine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
	mSpine->pushAfterCurrent(&TNerveWalkerAttack::theNerve());
	mSpine->pushAfterCurrent(&TNerveElecNokonokoTurn::theNerve());
	setGoalPathMario();
}

void TElecNokonoko::behaveToWater(THitActor* water)
{
	if (isBckAnm(DENNOKO_ANM_SHOOT1) && getCurAnmFrameNo(0) > 58.0f)
		return;

	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		return;

	if (isBckAnm(DENNOKO_ANM_CATCH1) && mHasCarapace == 0)
		return;

	unk165                  = true;
	mSprayedByWaterCooldown = 0;

	if (mSpine->getCurrentNerve() != &TNerveElecNokonokoFreeze::theNerve())
		mSpine->pushNerve(&TNerveElecNokonokoFreeze::theNerve());
}

// UNUSED, 0x50 in the map.
// TODO: incorrect size.
void TElecNokonoko::catchIn()
{
	mSpine->pushNerve(&TNerveElecNokonokoCollect::theNerve());
}

// UNUSED, 0x44 in the map: pushing the shoot nerve, which
// isResignationAttack does inline instead.
// TODO: incorrect size.
void TElecNokonoko::shootIn()
{
	mSpine->pushAfterCurrent(&TNerveElecNokonokoShoot::theNerve());
}

const char** TElecNokonoko::getBasNameTable() const { return dennoko_bastable; }

void TElecNokonoko::setWaitAnm()
{
	unk198 = 0;
	setBckAnm(DENNOKO_ANM_WAIT1);
}

void TElecNokonoko::setWalkAnm()
{
	if (!isBckAnm(DENNOKO_ANM_RUN1_LOOP))
		setBckAnm(DENNOKO_ANM_RUN1_START);
}

void TElecNokonoko::setRunAnm()
{
	if (!isBckAnm(DENNOKO_ANM_RUN1_LOOP))
		setBckAnm(DENNOKO_ANM_RUN1_START);
}

void TElecNokonoko::setDeadAnm() { setBckAnm(DENNOKO_ANM_DOWN1); }

void TElecNokonoko::setMeltAnm()
{
	setBckAnm(DENNOKO_ANM_ELEC_DOWN1);
	onLiveFlag(LIVE_FLAG_UNK8);

	JGeometry::TVec3<f32> stop(0.0f, 0.0f, 0.0f);
	unk18C            = 3;
	mCarapace->mVelocity = stop;

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_DNK_SHIBIRE_B, getMActor()->getModel()->getAnmMtx(0), 0,
	    nullptr);
	if (emitter)
		emitter->setGlobalScale(mScaling);
}

void TElecNokonoko::genRandomItem()
{
	mCarapace->kill();
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_TLS_CHANGE,
	                                            &mPosition, 0, nullptr);
	TSmallEnemy::genRandomItem();

	// A koopa killed while it still had a shine's worth of juice left drops
	// its coin where the shell landed, not where it died.
	if (checkLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH)) {
		TMapObjBase* coin = gpItemManager->makeObjAppear(
		    mCarapace->mPosition.x, mCarapace->mPosition.y,
		    mCarapace->mPosition.z, 0x2000000E, true);
		if (coin)
			coin->setVelocityAndFlag10(0.0f, 20.0f, 0.0f);
	}
}

// UNUSED, 0x6c in the map: inlined into attackToMario.
bool TElecNokonoko::isShootReady()
{
	if (mSpine->getCurrentNerve() == &TNerveElecNokonokoShoot::theNerve())
		return false;
	return true;
}

// UNUSED, 0x8c in the map: inlined into calcRootMatrix.
bool TElecNokonoko::isCatchReady()
{
	if (mSpine->getCurrentNerve() == &TNerveElecNokonokoFreeze::theNerve())
		return false;
	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		return false;
	if (mSpine->getCurrentNerve() == &TNerveElecNokonokoCollect::theNerve())
		return false;
	return true;
}

// UNUSED, 0x15c in the map, and nothing references it: it would have made the
// koopa drop whatever it was doing and go and fetch the shell.
// TODO: incorrect size.
void TElecNokonoko::forceCatchReady()
{
	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		return;

	if (mSpine->getCurrentNerve() == &TNerveElecNokonokoCollect::theNerve())
		return;

	mSpine->reset();
	mSpine->setNext(&TNerveElecNokonokoCollect::theNerve());
	setGoalPath(mCarapace->getPosition());
}

// UNUSED, 0xe0 in the map, and nothing references it.
// TODO: incorrect size.
bool TElecNokonoko::isDeadByThunder()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return false;

	if (mSpine->getCurrentNerve() != &TNerveElecNokonokoFreeze::theNerve())
		return false;

	if (!isBckAnm(DENNOKO_ANM_ELEC_DOWN1))
		return false;

	return true;
}

// UNUSED, 0x124 in the map, and nothing references it: the collect nerve does
// the same work inline.
// TODO: incorrect size.
void TElecNokonoko::recoverCarapace()
{
	mHasCarapace = 0;
	mCarapace->kill();
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_TLS_CHANGE,
	                                            &mCarapace->mPosition, 0,
	                                            nullptr);
	setBckAnm(DENNOKO_ANM_CATCH1);
}

TElecCarapace::TElecCarapace(const char* name)
    : TEnemyAttachment(name)
    , mNokonoko(nullptr)
    , mReflector(nullptr)
    , mSpinReverse(true)
    , mStraight(false)
    , mFlying(false)
    , mZigzagCycle(0.0f)
    , mZigzagAngle(0.0f)
    , mReflectTimer(0)
    , mLanded(false)
    , mSpinAngle(0.0f)
    , unk198(0.0f)
{
}

void TElecCarapace::loadInit(TSpineEnemy* host, const char* model)
{
	TEnemyAttachment::loadInit(host, model);

	mNokonoko = (TElecNokonoko*)unk160;

	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);

	initHitActor(0x1000000B, 3, 0x98000000, 80.0f, 80.0f, 60.0f, 60.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);

	unk150 = 0;
	mSpine->initWith(&TNerveElecCarapaceMove::theNerve());

	// Half the shells spin the other way, so a pair thrown together crosses
	// over instead of travelling side by side.
	if (TMsRange<s32>(0, 300).rand() < 150)
		mSpinReverse = false;

	mBodyRadius = 80.0f;
}

void TElecCarapace::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TEnemyAttachment::perform(cue, graphics);

	if (cue & 1) {
		if (mReflectTimer != 0) {
			mReflectTimer++;
			if (mReflectTimer > 5)
				mReflectTimer = 0;
		}
	}

	if (cue & 0x200) {
		if (!isUnk150Zero()
		    && !mNokonoko->checkLiveFlag(LIVE_FLAG_DEAD)) {
			if (checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN))
				return;

			TCircleShadowRequest request;
			request.mPosition = mPosition;

			bool airborne = isAirborne();
			if (!airborne) {
				request.mPosition.y      = mGroundHeight;
				request.mNeedsGroundCheck = 0;
			}

			request.mRadiusZ = request.mRadiusX
			    = mNokonoko->mScaledBodyRadius;
			request.mRotationY = mRotation.y;

			gpBindShadowManager->request(request, mActorType);
		}
	}
}

void TElecCarapace::setBehavior()
{
	if (mNokonoko->checkLiveFlag(LIVE_FLAG_DEAD))
		kill();

	if (mLanded)
		mPosition.y = mGroundHeight;

	mLanded = false;
}

void TElecCarapace::behaveToHitGround()
{
	if (mFlying)
		mLanded = true;

	if (getGroundPlane()->isWaterSurface())
		kill();

	mFlying = false;
	unk168  = 1;
	offLiveFlag(LIVE_FLAG_AIRBORNE);
	mVelocity.set(0.0f, 0.0f, 0.0f);
}

void TElecCarapace::kill() { TEnemyAttachment::kill(); }

void TElecCarapace::behaveToHitWall(const TBGCheckData* wall)
{
	if (mReflectTimer > 0)
		return;

	if (!TElecNokonoko::mReflectSw)
		return;

	mReflectTimer = 1;
	mLanded       = false;
	mFlying       = true;
	mStraight     = true;

	f32 along = -1.5f
	    * (mLinearVelocity.x * wall->mNormal.x
	       + mLinearVelocity.y * wall->mNormal.y
	       + mLinearVelocity.z * wall->mNormal.z);

	mVelocity.x = along * wall->mNormal.x;
	mVelocity.y = 3.0f;
	mVelocity.z = along * wall->mNormal.z;
	mPosition.y = 2.0f + mGroundHeight;

	setGoalPath(mNokonoko->getPosition());
}

f32 TElecCarapace::getNowGravity()
{
	return ((TElecNokonokoSaveLoadParams*)mNokonoko->getSaveParam())
	    ->getSLCarapaceGravity();
}

void TElecCarapace::appear()
{
	if (unk150 != 0)
		return;

	unk150    = 1;
	mPosition = mNokonoko->mPosition;

	f32 scale  = mNokonoko->mScaling.x;
	unk164     = scale;
	mScaling.z = scale;
	mScaling.y = scale;
	mScaling.x = scale;

	mBodyRadius = 50.0f;
	mReflector  = nullptr;
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TElecCarapace::shoot()
{
	mReflectTimer = 0;

	if (unk150 == 2)
		return;

	JGeometry::TVec3<f32> toMario = *gpMarioPos - mPosition;

	JGeometry::TVec3<f32> goal(toMario);
	MsVECNormalize((Vec*)&goal, (Vec*)&goal);

	f32 flyDist = mNokonoko->getSaveParams()->getSLCarapaceFlyDist();
	goal.x *= flyDist;
	goal.y *= mPosition.y;
	goal.z *= flyDist;
	goal.x += mPosition.x;
	goal.y += mPosition.y;
	goal.z += mPosition.z;

	mSpinReverse  = mSpinReverse == 0;
	mSpinAngle    = 0.0f;
	unk150        = 2;
	mFlying       = false;
	unk168        = 0;
	mLanded       = false;
	mStraight     = false;
	offHitFlag(HIT_FLAG_NO_COLLISION);

	mSpine->initWith(&TNerveElecCarapaceMove::theNerve());
	setGoalPath(goal);

	// TODO: 89.4%. The retail object reaches the distance through three real
	// calls (TVec3::sub, TVec3::dot, TUtil<f32>::sqrt); distance() gets the
	// sqrt call but expands the other two, and (point - mPosition).length()
	// gets sqrt and sub's copy but still expands sub and dot.
	f32 cycle    = TMsRange<f32>(3.0f, 5.0f).rand();
	mZigzagCycle = cycle * unk104.getPoint().distance(mPosition);
	mZigzagAngle = TMsRange<f32>(20.0f, 30.0f).rand();
}

// UNUSED, 0x160 in the map, and nothing references it: shoot() sets the two
// zigzag values inline.
// TODO: incorrect size.
void TElecCarapace::setZigParameter()
{
	mZigzagCycle
	    = TMsRange<f32>(3.0f, 5.0f).rand() * unk104.getPoint().distance(mPosition);
	mZigzagAngle = TMsRange<f32>(20.0f, 30.0f).rand();
}

void TElecCarapace::bind()
{
	control();
	TEnemyAttachment::bind();
}

void TElecCarapace::calcRootMatrix()
{
	MsMtxSetXYZRPH(getMActor()->getModel()->getBaseTRMtx(), mPosition.x,
	               mPosition.y, mPosition.z, mRotation.x,
	               mRotation.y + mSpinAngle, mRotation.z);
	getMActor()->getModel()->setBaseScale(mScaling);

	gpMSound->startSoundActor(MSD_SE_EN_DENNOKO_SPARK2, &mPosition);

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_DNK_BIRI, getMActor()->getModel()->getAnmMtx(2), 1, this);
	if (emitter)
		emitter->setGlobalScale(mNokonoko->mScaling);

	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_DNK_SPARK_L, getMActor()->getModel()->getAnmMtx(2), 1, this);
	if (emitter)
		emitter->setGlobalScale(mNokonoko->mScaling);

	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_DNK_SPARK_R, getMActor()->getModel()->getAnmMtx(2), 1, this);
	if (emitter)
		emitter->setGlobalScale(mNokonoko->mScaling);
}

void TElecCarapace::sendMessage()
{
	for (int i = 0; i < getColNum(); i++) {
		THitActor* other = getCollision(i);

		if (other->isActorType(0x80000001)) {
			// Mario: shock him and then sit still for a second.
			if (SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK)) {
				onHitFlag(HIT_FLAG_NO_COLLISION);
				if (mSpine->getCurrentNerve()
				    != &TNerveElecCarapaceWait::theNerve())
					mSpine->pushNerve(&TNerveElecCarapaceWait::theNerve());
			}
		} else if (other == mNokonoko) {
			offHitFlag(HIT_FLAG_NO_COLLISION);
		} else if (other->isActorType(0x01000001)) {
			// Mario's water jet. The retail object draws three angles out of
			// a 0..360 range five times over and uses none of them, so the
			// five splashes those angles aimed are gone and only the draws
			// are left.
			// TODO: what the three angles fed is not recoverable from the
			// retail object.
			TMsRange<s32> spread(0, 360);
			for (int j = 0; j < 5; j++) {
				s32 yaw   = spread.rand();
				s32 pitch = spread.rand();
				s32 roll  = spread.rand();
			}
		} else if (TElecNokonoko::mReflectSw) {
			reflect(other);
		}
	}
}

BOOL TElecCarapace::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_UNKD) {
		gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_TLS_CHANGE,
		                                            &mPosition, 0, nullptr);
		kill();
	}

	if (message == HIT_MESSAGE_TRAMPLE)
		SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK);

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER)
		return TRUE;

	return FALSE;
}

void TElecCarapace::reflect(THitActor* other)
{
	if (mReflector == other)
		return;

	mLanded    = false;
	mReflector = other;
	mFlying    = true;
	mStraight  = false;

	JGeometry::TVec3<f32> away(other->mPosition.x - mPosition.x, 0.0f,
	                           other->mPosition.z - mPosition.z);
	if (away.x == 0.0f && away.y == 0.0f && away.z == 0.0f)
		away.x += 1.0f;
	MsVECNormalize((Vec*)&away, (Vec*)&away);

	// Snap the bounce onto whichever world axis the hit came from.
	f32 axisX = 0.0f;
	f32 axisZ = 0.0f;
	if (fabsf(away.z / away.x) > 1.0f) {
		if (other->mPosition.z > mPosition.z)
			axisZ = 1.0f;
		else
			axisZ = -1.0f;
	} else if (other->mPosition.x > mPosition.x) {
		axisX = 1.0f;
	} else {
		axisX = -1.0f;
	}

	f32 along = -7.0f * (away.x * axisX + away.y * 0.0f + away.z * axisZ);
	mVelocity.x = away.x * along;
	mVelocity.y = 2.0f;
	mVelocity.z = away.z * along;
	mPosition.y = 2.0f + mGroundHeight;

	mSpinReverse = false;
	if ((mVelocity.x > 0.0f && mVelocity.z > 0.0f)
	    || (mVelocity.x < 0.0f && mVelocity.z < 0.0f))
		mSpinReverse = true;

	setGoalPath(mNokonoko->getPosition());
}

// UNUSED, 0x94 in the map: the shell's own step, which the move nerve does
// inline.
// TODO: incorrect size.
void TElecCarapace::move()
{
	if (mStraight)
		walkToCurPathNode(mNokonoko->getSaveParams()->getSLCarapaceSpeed(),
		                  mNokonoko->getSaveParams()->getSLCarapaceTurnSpeed(),
		                  0.0f);
	else
		zigzagToCurPathNode(
		    mNokonoko->getSaveParams()->getSLCarapaceSpeed(),
		    mNokonoko->getSaveParams()->getSLCarapaceTurnSpeed(), mZigzagCycle,
		    mZigzagAngle);
}

// UNUSED, 0x8c in the map.
// TODO: incorrect size.
bool TElecCarapace::isMove()
{
	if (mSpine->getCurrentNerve() == &TNerveElecCarapaceMove::theNerve())
		return true;
	return false;
}

DEFINE_NERVE(TNerveElecNokonokoShoot, TLiveActor)
{
	TElecNokonoko* nokonoko = (TElecNokonoko*)spine->getBody();

	if (spine->getTime() == 0)
		nokonoko->setBckAnm(DENNOKO_ANM_READY);

	if (nokonoko->isBckAnm(DENNOKO_ANM_READY)) {
		if (nokonoko->checkCurAnmEnd(0)) {
			nokonoko->setBckAnm(DENNOKO_ANM_SHOOT1);
			nokonoko->unk198 = 0;
		}
	} else if (nokonoko->isBckAnm(DENNOKO_ANM_SHOOT1)) {
		if (nokonoko->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(60.0f))
			nokonoko->getCarapace()->appear();

		if (nokonoko->getCurAnmFrameNo(0) < 62.0f)
			nokonoko->walkToCurPathNode(0.0f, nokonoko->getTurnSpeed(), 0.0f);

		if (nokonoko->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(62.0f)) {
			nokonoko->getCarapace()->shoot();
			nokonoko->mHasCarapace = 1;
		}

		if (nokonoko->checkCurAnmEnd(0)) {
			spine->pushAfterCurrent(&TNerveElecNokonokoCollect::theNerve());
			return TRUE;
		}
	}

	return FALSE;
}

DEFINE_NERVE(TNerveElecNokonokoCollect, TLiveActor)
{
	TElecNokonoko* nokonoko = (TElecNokonoko*)spine->getBody();

	if (spine->getTime() == 0) {
		if (!nokonoko->isBckAnm(DENNOKO_ANM_CATCH1))
			nokonoko->setBckAnm(DENNOKO_ANM_COLLECT);
		nokonoko->setGoalPath(nokonoko->getCarapace());
	}

	// The koopa only walks while the shell is still rolling; once the shell
	// is waiting for it, it stands still and the animation freezes.
	if (nokonoko->getCarapace()->mSpine->getCurrentNerve()
	    != &TNerveElecCarapaceWait::theNerve())
		nokonoko->getMActor()->setFrameRate(SMSGetAnmFrameRate(),
		                                    ANM_TYPE_BCK);
	else
		nokonoko->getMActor()->setFrameRate(0.0f, ANM_TYPE_BCK);

	if (nokonoko->isBckAnm(DENNOKO_ANM_CATCH1)) {
		int frame = (int)nokonoko->getCurAnmFrameNo(0);
		if (frame > 20)
			nokonoko->getCarapace()->onHitFlag(HIT_FLAG_NO_COLLISION);
		if (frame > 32) {
			nokonoko->mHasCarapace = 0;
			nokonoko->getCarapace()->kill();
		}
		if (nokonoko->checkCurAnmEnd(0))
			return TRUE;
	} else if (spine->getTime() > 800
	           || nokonoko->getCarapace()->checkLiveFlag(LIVE_FLAG_DEAD)) {
		spine->pushAfterCurrent(&TNerveElecNokonokoRebirth::theNerve());
		return TRUE;
	}

	nokonoko->walkToCurPathNode(0.0f, nokonoko->getTurnSpeed(), 0.0f);
	return FALSE;
}

DEFINE_NERVE(TNerveElecNokonokoTurn, TLiveActor)
{
	TElecNokonoko* nokonoko = (TElecNokonoko*)spine->getBody();

	if (spine->getTime() == 0) {
		nokonoko->setBckAnm(DENNOKO_ANM_TURN1_START);
		nokonoko->setGoalPathMario();
	}

	if (nokonoko->isBckAnm(DENNOKO_ANM_TURN1_LOOP)
	    && MsIsInSight(nokonoko->mPosition, nokonoko->mRotation.y, *gpMarioPos,
	                   ((TSmallEnemyParams*)nokonoko->getSaveParam())->getSLSearchLength(), 60.0f,
	                   0.0f))
		nokonoko->setBckAnm(DENNOKO_ANM_TURN1_END);

	if (nokonoko->checkCurAnmEnd(0)) {
		if (nokonoko->isBckAnm(DENNOKO_ANM_TURN1_START))
			nokonoko->setBckAnm(DENNOKO_ANM_TURN1_LOOP);
		else if (nokonoko->isBckAnm(DENNOKO_ANM_TURN1_END))
			return TRUE;
	}

	// Standing exactly on top of the shell leaves the turn with no direction
	// to aim at, so nudge along X.
	if (nokonoko->mPosition.x - nokonoko->getCarapace()->mPosition.x == 0.0f
	    && nokonoko->mPosition.z - nokonoko->getCarapace()->mPosition.z
	           == 0.0f)
		nokonoko->mPosition.x += 1.0f;

	nokonoko->walkToCurPathNode(0.0f, nokonoko->getTurnSpeed(), 0.0f);

	if (spine->getTime() > 500)
		return TRUE;

	return FALSE;
}

DEFINE_NERVE(TNerveElecNokonokoFreeze, TLiveActor)
{
	TElecNokonoko* nokonoko = (TElecNokonoko*)spine->getBody();

	if (spine->getTime() == 0) {
		if (nokonoko->hasCarapace()) {
			nokonoko->setBckAnm(DENNOKO_ANM_HIT1);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_DNK_SHIBIRE_B,
			    nokonoko->getMActor()->getModel()->getAnmMtx(0), 0, nullptr);
		} else {
			nokonoko->setBckAnm(DENNOKO_ANM_MOGAKI1_START);
		}
	}

	if (nokonoko->isBckAnm(DENNOKO_ANM_HIT1)
	    && nokonoko->getCurAnmFrameNo(0) < 25.0f) {
		MtxPtr head = nokonoko->getMActor()->getModel()->getAnmMtx(8);
		nokonoko->mEffectPos.set(head[0][3], head[1][3], head[2][3]);

		JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToPosPtr(
		        PARTICLE_MS_DNK_HIBANA, &nokonoko->mEffectPos, 1, nokonoko);
		if (emitter)
			emitter->setGlobalScale(nokonoko->mScaling);
	}

	if (nokonoko->checkCurAnmEnd(0)) {
		if (nokonoko->isBckAnm(DENNOKO_ANM_MOGAKI1_START)) {
			nokonoko->setBckAnm(DENNOKO_ANM_MOGAKI1_LOOP);
		} else if (nokonoko->isBckAnm(DENNOKO_ANM_MOGAKI1_LOOP)) {
			bool first      = nokonoko->unk165;
			if (first)
				nokonoko->unk165 = false;

			if (!first && nokonoko->hasCarapace())
				nokonoko->setBckAnm(DENNOKO_ANM_MOGAKI_BARE);
			else
				nokonoko->setBckAnm(DENNOKO_ANM_MOGAKI1_LOOP);
		} else {
			return TRUE;
		}
	}

	if (spine->getTime() > 400) {
		spine->reset();
		spine->setDefaultNext();
		spine->pushAfterCurrent(&TNerveElecNokonokoRebirth::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveElecNokonokoRebirth, TLiveActor)
{
	TElecNokonoko* nokonoko = (TElecNokonoko*)spine->getBody();

	if (spine->getTime() == 0) {
		nokonoko->setBckAnm(DENNOKO_ANM_SUPPLY1);
		nokonoko->mHasCarapace = 0;
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_TLS_CHANGE, &nokonoko->getCarapace()->mPosition, 0,
		    nullptr);
		nokonoko->getCarapace()->kill();
	}

	if (nokonoko->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(88.0f))
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_TLS_CHANGE, &nokonoko->mPosition, 0, nullptr);

	if (nokonoko->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveElecNokonokoAttack, TLiveActor)
{
	TElecNokonoko* nokonoko = (TElecNokonoko*)spine->getBody();

	if (spine->getTime() == 0 || !nokonoko->isBckAnm(DENNOKO_ANM_HIT1))
		nokonoko->setBckAnm(DENNOKO_ANM_HIT1);

	if (nokonoko->checkCurAnmEnd(0))
		return TRUE;

	return FALSE;
}

DEFINE_NERVE(TNerveElecCarapaceMove, TLiveActor)
{
	TElecCarapace* carapace = (TElecCarapace*)spine->getBody();

	f32 spinSpeed
	    = carapace->getNokonoko()->getSaveParams()->getSLCarapaceSpinSpeed();
	f32 speed
	    = carapace->getNokonoko()->getSaveParams()->getSLCarapaceSpeed();
	f32 turnSpeed
	    = carapace->getNokonoko()->getSaveParams()->getSLCarapaceTurnSpeed();

	if (carapace->mStraight)
		carapace->walkToCurPathNode(speed, turnSpeed, 0.0f);
	else
		carapace->zigzagToCurPathNode(speed, turnSpeed,
		                              carapace->mZigzagCycle,
		                              carapace->mZigzagAngle);

	carapace->mSpinAngle += spinSpeed;
	if (carapace->mSpinAngle > 360.0f)
		carapace->mSpinAngle -= 360.0f;

	if (carapace->mLanded) {
		// Once the shell has landed it homes on the koopa, and a near miss
		// hands it back over.
		// TODO: the retail object copies the goal point into a local and
		// subtracts mPosition into it in place before squaring, then calls
		// TUtil<f32>::sqrt; distance() gets the call but not the stores, and
		// `TVec3 d = point; d -= mPosition; d.length()` gets the stores but
		// expands sqrt. Same open shape as TElecNokonoko::isResignationAttack
		// and TElecCarapace::shoot.
		f32 catchRange = 64.0f
		    * carapace->getNokonoko()->getSaveParams()->getSLCarapaceSpeed();
		if (carapace->getUnk104().getPoint().distance(carapace->mPosition)
		    < catchRange) {
			TElecNokonoko* nokonoko = carapace->getNokonoko();
			if (nokonoko->mSpine->getCurrentNerve()
			    != &TNerveElecNokonokoCollect::theNerve()) {
				if (nokonoko->mSpine->getCurrentNerve()
				        != &TNerveSmallEnemyDie::theNerve()
				    && nokonoko->mSpine->getCurrentNerve()
				           != &TNerveElecNokonokoFreeze::theNerve()
				    && nokonoko->mSpine->getCurrentNerve()
				           != &TNerveElecNokonokoCollect::theNerve())
					nokonoko->mSpine->setNext(
					    &TNerveElecNokonokoCollect::theNerve());
			}

			spine->pushAfterCurrent(&TNerveElecCarapaceReturn::theNerve());
			return TRUE;
		}
	}

	JGeometry::TVec3<f32> toGoal(carapace->getUnk104().getPoint());
	toGoal.sub(carapace->mPosition);
	toGoal.y = 0.0f;

	if (!carapace->mFlying && MsVECMag2((Vec*)&toGoal) < 100.0f) {
		if (carapace->mLanded) {
			spine->pushAfterCurrent(&TNerveElecCarapaceReturn::theNerve());
			return TRUE;
		}

		carapace->mLanded = true;
		carapace->setGoalPath(carapace->getNokonoko()->getPosition());
	}

	return FALSE;
}

DEFINE_NERVE(TNerveElecCarapaceWait, TLiveActor)
{
	if (spine->getTime() > 60)
		return TRUE;

	return FALSE;
}

// Instruction-identical to the ROM apart from r30/r31 being swapped (the
// retail object keeps the spine in r31 and the shell in r30) and the operand
// order of the three clamp compares.
DEFINE_NERVE(TNerveElecCarapaceReturn, TLiveActor)
{
	TElecCarapace* carapace = (TElecCarapace*)spine->getBody();

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> home(carapace->mNokonoko->mPosition);
		carapace->mReturnStep.set(
		    0.015625f * (home.x - carapace->mPosition.x),
		    0.015625f * (home.y - carapace->mPosition.y),
		    0.015625f * (home.z - carapace->mPosition.z));

		if (carapace->mNokonoko->isBckAnm(DENNOKO_ANM_COLLECT))
			carapace->mNokonoko->setBckAnm(DENNOKO_ANM_CATCH1);
	}

	// The koopa keeps being asked for the catch animation over the first
	// twenty frames, so a late arrival still gets picked up.
	if (spine->getTime() < 20) {
		if (carapace->mNokonoko->isBckAnm(DENNOKO_ANM_COLLECT))
			carapace->mNokonoko->setBckAnm(DENNOKO_ANM_CATCH1);
	}

	TElecNokonoko* nokonoko = carapace->mNokonoko;
	if (nokonoko->mSpine->getCurrentNerve()
	        == &TNerveElecNokonokoFreeze::theNerve()
	    && VECMag(nokonoko->mPosition - nokonoko->mCarapace->mPosition)
	           < 200.0f) {
		// Shocked with the shell almost home: the koopa melts away and the
		// shell pops instead of being caught.
		carapace->mNokonoko->onLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		carapace->mNokonoko->kill();
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_TLS_CHANGE, &carapace->mPosition, 0, nullptr);
	}

	carapace->mSpinAngle
	    += carapace->mNokonoko->getSaveParams()->getSLCarapaceSpinSpeed();
	if (carapace->mSpinAngle > 360.0f)
		carapace->mSpinAngle -= 360.0f;

	carapace->mPosition.x += carapace->mReturnStep.x;
	carapace->mPosition.y += carapace->mReturnStep.y;
	carapace->mPosition.z += carapace->mReturnStep.z;

	// Clamp per axis so the shell never overshoots the koopa.
	JGeometry::TVec3<f32> home(carapace->mNokonoko->mPosition);

	if (carapace->mReturnStep.x > 0.0f) {
		if (carapace->mPosition.x > home.x)
			carapace->mPosition.x = home.x;
	} else if (carapace->mPosition.x < home.x) {
		carapace->mPosition.x = home.x;
	}

	if (carapace->mReturnStep.y > 0.0f) {
		if (carapace->mPosition.y > home.y)
			carapace->mPosition.y = home.y;
	} else if (carapace->mPosition.y < home.y) {
		carapace->mPosition.y = home.y;
	}

	if (carapace->mReturnStep.z > 0.0f) {
		if (carapace->mPosition.z > home.z)
			carapace->mPosition.z = home.z;
	} else if (carapace->mPosition.z < home.z) {
		carapace->mPosition.z = home.z;
	}

	if (carapace->mNokonoko->checkLiveFlag(LIVE_FLAG_DEAD)) {
		carapace->mScaling.y *= 0.8f;
		if (carapace->mScaling.y < 0.01f)
			carapace->kill();
	}

	return FALSE;
}
