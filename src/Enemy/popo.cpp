#include <Enemy/Popo.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/PathNode.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/ObjModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <Player/WaterGun.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JMath.hpp>
#include <System/Particles.hpp>
#include <System/Application.hpp>
#include <System/MarioGamePad.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

u8 TPopo::mRollSw          = 1;
u8 TPopo::mTriggerSw       = 1;
f32 TPopo::mTestAng_x      = 90.0f;
f32 TPopo::mTestAng_y      = 90.0f;
f32 TPopo::mTestAng_z      = 0.0f;
f32 TPopo::mNozzleOffsetZ  = -15.0f;
u8 TPopo::mCenterJntIndex  = 1;
u8 TPopo::mMouthJntIndex   = 2;
u8 TPopo::mRLegJntIndex    = 5;
u8 TPopo::mLLegJntIndex    = 11;
u8 TPopo::mRHandJntIndex   = 7;
u8 TPopo::mLHandJntIndex   = 9;
f32 TPopo::mTestBodyScale  = 35.0f;
u8 TPopo::mBrkFlag         = 1;
f32 TPopo::mColOffsetY     = 20.0f;
f32 TPopo::mColMinVal      = 0.6f;
u8 TPopo::mLevelShootSw    = 1;
u8 TPopo::mExplosionSw     = 0;

TPopo* gpCurPopo;

// Indexed by BCK animation index, so the holes are real: only slots 0, 5
// and 6 have a sound table.
static const char* popo_bastable[] = {
	"/scene/popo/bas/popo_chase.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/popo/bas/popo_jump.bas",
	"/scene/popo/bas/popo_wait.bas",
};

static int PopoNonScaleCallback(J3DNode* node, int param);
static int PopoPossessedCallback(J3DNode* node, int param);
static int PopoRollCallback(J3DNode* node, int param);

TPopoSaveLoadParams::TPopoSaveLoadParams(const char* prm)
    : TWalkerEnemyParams(prm)
    , PARAM_INIT(mSLMoveDist, 100.0f)
    , PARAM_INIT(mSLMoveGravity, 0.1f)
    , PARAM_INIT(mSLMoveJumpSp, 10.0f)
    , PARAM_INIT(mSLAttackDist, 100.0f)
    , PARAM_INIT(mSLAttackGravity, 0.1f)
    , PARAM_INIT(mSLAttackJumpSp, 10.0f)
    , PARAM_INIT(mSLReleaseSpeed, 10.0f)
    , PARAM_INIT(mSLFlyGravity, 0.0f)
    , PARAM_INIT(mSLFlyLimitTime, 300)
    , PARAM_INIT(mSLExplosionEmitTime, 60)
    , PARAM_INIT(mSLWaterScaleMax, 2.0f)
    , PARAM_INIT(mSLThrownGravity, 0.5f)
    , PARAM_INIT(mSLPumpRate, 0.0001f)
    , PARAM_INIT(mSLLevelLimit, 1.2f)
    , PARAM_INIT(mSLScaleRate, 0.99f)
{
	TParams::load(mPrmPath);
}

TPopoManager::TPopoManager(const char* name)
    : TSmallEnemyManager(name)
    , mIsNozzleFree(1)
    , mFlyWater(nullptr)
    , mExplosionWater(nullptr)
{
	gpCurPopo = nullptr;
	unk5C     = 0;
}

void TPopoManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38           = new TPopoSaveLoadParams("/enemy/popo.prm");
	mFlyWater       = new TWaterEmitInfo("/enemy/popowater.prm");
	mExplosionWater = new TWaterEmitInfo("/enemy/popoexpwater.prm");
}

TSpineEnemy* TPopoManager::createEnemyInstance() { return new TPopo("ポポ"); }

void TPopoManager::initSetEnemies()
{
	TGraphWeb* web = getObj(0)->unk124->getGraph();
	// Nothing left to set up here: the ROM's dead `cmpwi r3, 0` on
	// isDummy()'s result is the trailing guard's own branch, which lands on
	// the epilogue and so is dropped as a fall-through.  (`if (web &&
	// web->isDummy()) return;` is byte-identical.)
	if (!web || web->isDummy())
		return;
}

void TPopoManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "popoH.bmd",
		  J3DMLF_MaterialPEFull | (2 << J3DMLF_TevStageNumShift), 0 },
		{ "popoL.bmd",
		  J3DMLF_MaterialPEFull | (2 << J3DMLF_TevStageNumShift), 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TPopoManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		for (int i = 0; i < getActiveObjNum(); ++i) {
			TPopo* popo = (TPopo*)getObj(i);
			if (popo->mIsLoaded && popo->checkLiveFlag(LIVE_FLAG_DEAD))
				popo->reset();
		}
	}
	TEnemyManager::perform(cue, graphics);
}

BOOL TPopoCollision::receiveMessage(THitActor* sender, u32 message)
{
	if (mPopo->isRollJump())
		return mPopo->receiveMessage(sender, message);
	return FALSE;
}

// UNUSED, 0xa4 in the map: the collisions land on the popo.
void TPopoCollision::checkHit()
{
	for (int i = 0; i < getColNum(); ++i) {
		THitActor* col = getCollision(i);
		if (col->isActorType(0x80000001))
			mPopo->attackToMario();
		else
			mPopo->behaveToHitOthers(col);
	}
}

// UNUSED, 0x10 in the map.
void TPopoCollision::kill() { onHitFlag(HIT_FLAG_NO_COLLISION); }

// The center joint rolls the whole body in the direction of travel.
static int PopoRollCallback(J3DNode* node, int param)
{
	if (param == 0) {
		if (gpCurPopo == nullptr)
			return 1;

		J3DJoint* joint = (J3DJoint*)node;
		MtxPtr anmMtx   = gpCurPopo->getModel()->getAnmMtx(joint->getJntNo());
		TPopo* popo     = gpCurPopo;
		Mtx scale;
		scale[0][3] = 0.0f;
		scale[1][3] = 0.0f;
		scale[2][3] = 0.0f;
		f32 s       = popo->mBodyScale;
		scale[0][0] = s;
		scale[0][1] = 0.0f;
		scale[0][2] = 0.0f;
		scale[1][0] = 0.0f;
		scale[1][1] = s;
		scale[1][2] = 0.0f;
		scale[2][0] = 0.0f;
		scale[2][1] = 0.0f;
		scale[2][2] = s;

		Mtx roll;
		if (popo->isRollJump()) {
			MsMtxSetRotX(roll, gpCurPopo->mRollAngle);
		} else {
			f32 s     = JMASSin(0x8000);
			f32 c     = JMASCos(0x8000);
			roll[0][0] = c;
			roll[0][1] = 0.0f;
			roll[0][2] = s;
			roll[0][3] = 0.0f;
			roll[1][0] = 0.0f;
			roll[1][1] = 1.0f;
			roll[1][2] = 0.0f;
			roll[1][3] = 0.0f;
			roll[2][0] = -s;
			roll[2][1] = 0.0f;
			roll[2][2] = c;
			roll[2][3] = 0.0f;
		}
		MTXConcat(anmMtx, roll, anmMtx);
		MTXConcat(anmMtx, scale, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, roll, J3DSys::mCurrentMtx);
		MTXConcat(J3DSys::mCurrentMtx, scale, J3DSys::mCurrentMtx);
	}
	return 1;
}

// The mouth joint swells with the pumped water.
static int PopoPossessedCallback(J3DNode* node, int param)
{
	if (param == 0) {
		TPopo* popo = gpCurPopo;
		if (popo == nullptr || !popo->isUseScaleCallBack())
			return 1;

		f32 pump = gpCurPopo->mPumpScale;
		if (pump < 1.1f)
			return 1;

		J3DJoint* joint = (J3DJoint*)node;
		MtxPtr anmMtx   = gpCurPopo->getModel()->getAnmMtx(joint->getJntNo());
		Mtx scale;
		scale[0][3] = 0.0f;
		scale[1][3] = 0.0f;
		scale[2][3] = 0.0f;
		scale[0][0] = pump;
		scale[0][1] = 0.0f;
		scale[0][2] = 0.0f;
		scale[1][0] = 0.0f;
		scale[1][1] = pump;
		scale[1][2] = 0.0f;
		scale[2][0] = 0.0f;
		scale[2][1] = 0.0f;
		scale[2][2] = pump;
		MTXConcat(anmMtx, scale, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, scale, J3DSys::mCurrentMtx);

		if (gpCurPopo->mIsPumping) {
			MTXCopy(anmMtx, gpCurPopo->mMouthMtx);
			Mtx rot;
			MsMtxSetRotRPH(rot, 0.0f, 270.0f, 0.0f);
			MTXConcat(gpCurPopo->mMouthMtx, rot, gpCurPopo->mMouthMtx);

			TRotation3f* anmRot = (TRotation3f*)anmMtx;
			JGeometry::TVec3<f32> col0;
			anmRot->getXDir(col0);
			gpCurPopo->mEffectScale.y = col0.length();
			JGeometry::TVec3<f32> col1;
			anmRot->getYDir(col1);
			gpCurPopo->mEffectScale.z = col1.length();
			JGeometry::TVec3<f32> col2;
			anmRot->getZDir(col2);
			gpCurPopo->mEffectScale.x = col2.length();

			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToMtxPtr(
			        0x13C, gpCurPopo->mMouthMtx, 1, gpCurPopo);
			if (emitter)
				emitter->setGlobalScale(gpCurPopo->mEffectScale);
		}
	}
	return 1;
}

// The limbs keep the body scale instead of the pump scale.
// TODO: frame 0x80 vs retail 0xc0; getModel binder adds an instruction.
static int PopoNonScaleCallback(J3DNode* node, int param)
{
	if (param == 0) {
		TPopo* popo = gpCurPopo;
		if (popo == nullptr || !popo->isUseScaleCallBack())
			return 1;

		int jntNo     = ((J3DJoint*)node)->getJntNo();
		MtxPtr anmMtx = gpCurPopo->getModel()->getAnmMtx(jntNo);
		Mtx scale;
		scale[2][3] = scale[1][3] = scale[0][3] = 0.0f;
		f32 s       = 0.9f * gpCurPopo->mBodyScale;
		scale[0][0] = s;
		scale[0][1] = 0.0f;
		scale[0][2] = 0.0f;
		scale[1][0] = 0.0f;
		scale[1][1] = s;
		scale[1][2] = 0.0f;
		scale[2][0] = 0.0f;
		scale[2][1] = 0.0f;
		scale[2][2] = s;
		MTXConcat(anmMtx, scale, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, scale, J3DSys::mCurrentMtx);
	}
	return 1;
}

TPopo::TPopo(const char* name)
    : TWalkerEnemy(name)
    , mSaveParams(nullptr)
    , mPumpScale(1.0f)
    , mFlyTimer(0)
    , mBrkFrames(30.0f)
    , mIsLoaded(0)
    , mIsPossessed(0)
    , mRollAngle(0.0f)
    , mIsLevelReached(0)
    , unk1CD(0)
    , mCollision(nullptr)
{
}

void TPopo::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	mInitialPos = mPosition;
	mIsLoaded   = 1;
	reset();
}

static inline MActor* PopoInitActor(TPopo* popo)
{
	MActor* actor = popo->getMActor();
	return actor;
}

void TPopo::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mActorType = 0x1000000D;
	if (mInstanceIndex == 0) {
		for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); ++i) { }
	}
	unk150      = 0x11;
	mSaveParams = (TPopoSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveWalkerGraphWander::theNerve());
	onHitFlag(HIT_FLAG_UNK8000000);

	PopoInitActor(this)->setJointCallback(mCenterJntIndex, &PopoRollCallback);
	mMActorKeeper->getMActor("popoL.bmd")
	    ->setJointCallback(mCenterJntIndex, &PopoRollCallback);
	PopoInitActor(this)->setJointCallback(mMouthJntIndex,
	                                     &PopoPossessedCallback);
	PopoInitActor(this)->setJointCallback(mRLegJntIndex, &PopoNonScaleCallback);
	PopoInitActor(this)->setJointCallback(mLLegJntIndex, &PopoNonScaleCallback);
	PopoInitActor(this)->setJointCallback(mRHandJntIndex, &PopoNonScaleCallback);
	PopoInitActor(this)->setJointCallback(mLHandJntIndex, &PopoNonScaleCallback);
	unk188 = 0.0f;

	mCollision = new TPopoCollision("ポポコリジョン");
	// Named search is -8 against the 6-site getMActor binder's +0x48, landing
	// the frame.
	// TODO: JGadget iterator temps still group 0xc off (stride-8-vs-12 class).
	TIdxGroupObj* group
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	group->getChildren().push_back(mCollision);
	mCollision->initHitActor(0x1000000D, 2, 0x98000000, 80.0f, 80.0f, 80.0f,
	                         80.0f);
	getCollision()->onHitFlag(HIT_FLAG_NO_COLLISION);
	mCollision->mPopo = this;
}

void TPopo::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);
	mCollision->THitActor::perform(cue, graphics);
}

void TPopo::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("popoH.bmd", 3);
	mMActorKeeper->createMActor("popoL.bmd", 3);
}

void TPopo::reset()
{
	gpCurPopo = this;
	TWalkerEnemy::reset();
	unk165            = 0;
	mIsPossessed      = 0;
	mPumpScale        = 1.0f;
	mRollAngle        = 0.0f;
	mFlyTimer         = 0;
	mScaledBodyRadius = 15.0f * (mBodyScale * mBodyRadius);
	unk190            = 0.2f;
	expandCollision();
	mMActor = mMActorKeeper->getMActor("popoL.bmd");
	if (mIsLoaded) {
		onLiveFlag(LIVE_FLAG_UNK10);
		mSpine->initWith(&TNervePopoWait::theNerve());
		mPosition = mInitialPos;
		offLiveFlag(LIVE_FLAG_UNK800);
	}
	getCollision()->onHitFlag(HIT_FLAG_NO_COLLISION);
	unk18C = 0;
}

static inline MActor* PopoTriggerActor(TPopo* popo)
{
	MActor* actor = popo->getMActor();
	return actor;
}

static inline MSound* PopoTriggerSound()
{
	MSound* sound = SMSGetMSound();
	return sound;
}

static inline TWaterGun* PopoTriggerGun()
{
	TWaterGun* gun = SMS_GetMarioWaterGun();
	return gun;
}

bool TPopo::checkTrigger()
{
	mIsPumping = 0;
	if (gpMarioOriginal->onYoshi()
	    || (s32)PopoTriggerGun()->mCurrentNozzle != 0) {
		kill();
		return false;
	}

	SMS_SendMessageToMario(this, HIT_MESSAGE_UNK5);
	f32 scaleMax = mSaveParams->getSLWaterScaleMax();
	u8 analogR   = gpMarioOriginal->mGamePad->mCompSPos[3];
	if (analogR > 20) {
		mIsPumping = 1;
		f32 pump   = mPumpScale;
		if (PopoTriggerSound()->gateCheck(0x20C2))
			MSoundSESystem::MSoundSE::startSoundActorWithInfo(
			    0x20C2, &mPosition, nullptr, pump, 0, 0, nullptr, 0, 4);
		mSprayedByWaterCooldown = 0;
		unk165                  = 1;
		f32 pumped              = analogR * mSaveParams->getSLPumpRate();
		mPumpScale += pumped;
		if (mPumpScale > scaleMax) {
			mPumpScale = scaleMax;
			if (!mBrkFlag)
				PopoTriggerActor(this)->setFrameRate(SMSGetAnmFrameRate(),
				                                     ANM_TYPE_BRK);
		}
		f32 scaleMax2 = mSaveParams->getSLWaterScaleMax();
		if (mBrkFlag)
			PopoTriggerActor(this)->getFrameCtrl(ANM_TYPE_BRK)->setFrame(
			    mBrkFrames * mPumpScale / scaleMax2);
	}

	if (gpMarioOriginal->mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_0x400)
	    || !mTriggerSw) {
		if (mLevelShootSw)
			mIsLevelReached = 1;
		else if (mPumpScale >= scaleMax)
			mIsLevelReached = 1;
	}

	if (mLevelShootSw) {
		if (mPumpScale < scaleMax - 0.1f && mPumpScale > 1.0f)
			mPumpScale *= mSaveParams->getSLScaleRate();
	}

	f32 levelLimit = mSaveParams->getSLLevelLimit();
	if (analogR < 20 && (mIsLevelReached || mPumpScale > levelLimit)) {
		if (SMSGetMSound()->gateCheck(0x28CD))
			MSoundSESystem::MSoundSE::startSoundActor(0x28CD, &mPosition, 0,
			                                          nullptr, 0, 4);
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
		return true;
	}

	mScaledBodyRadius
	    = (8.0f * mPumpScale + 8.0f) * (mBodyScale * mBodyRadius);
	if (mPumpScale >= scaleMax)
		PopoTriggerActor(this)->getFrameCtrl(ANM_TYPE_BTP)->setFrame(5.0f);
	return false;
}

void TPopo::behaveToWater(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve()
	    || mSpine->getCurrentNerve() == &TNervePopoExplosion::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		return;
	if (mSpine->getCurrentNerve() == &TNervePopoPossessedNozzle::theNerve()) {
		mSprayedByWaterCooldown = 0;
		return;
	}

	if (isAirborne()) {
		// getPosition() and SMS_GetMarioPos() are each an 8-byte
		// reference temporary; the pair is the frame here.
		// TODO: the two vector temporaries still sit 4 bytes low
		// (frame-gaps.md's "4 low" class).
		JGeometry::TVec3<f32> vel(mVelocity);
		JGeometry::TVec3<f32> push(getPosition().x - SMS_GetMarioPos().x,
		                           0.0f,
		                           getPosition().z - SMS_GetMarioPos().z);
		MsVECNormalize((Vec*)&push, (Vec*)&push);
		push.scale(12.0f);
		push.y = -1.0f;
		push.add(vel);
		mVelocity = push;
		return;
	}

	if (mSpine->getCurrentNerve() != &TNerveSmallEnemyFreeze::theNerve())
		mSpine->pushNerve(&TNerveSmallEnemyFreeze::theNerve());
}

f32 TPopo::getGravityY() const
{
	f32 gravity = mGravity;
	if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveWalkerEscape::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve())
		// Read raw: TParamT::get() returns const T&, and the reference
		// temporary is 8 bytes of frame here. getSLMoveGravity() is +8,
		// mSLMoveGravity.get() and getSaveParams()->getSLMoveGravity()
		// both +0x10.
		return mSaveParams->mSLMoveGravity.value;

	if (mSpine->getCurrentNerve() == &TNervePopoAttack::theNerve())
		gravity = mSaveParams->getSLAttackGravity();
	else if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve())
		gravity = mSaveParams->getSLFlyGravity();
	else if (mSpine->getCurrentNerve() == &TNervePopoThrown::theNerve())
		gravity = mSaveParams->getSLThrownGravity();
	return gravity;
}

// The nozzle test reads the water gun through a bound local: that binding is
// 8 bytes of low region, which is what the frame wants here.
// TODO: still 4 bytes short below setGoalPathMario's block.
static inline TWaterGun* PopoWaterGun()
{
	TWaterGun* gun = SMS_GetMarioWaterGun();
	return gun;
}

void TPopo::behaveToFindMario()
{
	TPopoManager* manager = (TPopoManager*)mManager;
	if (SMS_CheckMarioFlag(MARIO_FLAG_HAS_FLUDD) && manager->mIsNozzleFree
	    && (s32)PopoWaterGun()->mCurrentNozzle == 0
	    && !gpMarioOriginal->onYoshi()) {
		setGoalPathMario();
		getSpine()->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
		mSpine->pushAfterCurrent(&TNervePopoAttack::theNerve());
	} else {
		mSpine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
	}
}

void TPopo::walkBehavior(int param_1, f32 param_2)
{
	if (!isAirborne()) {
		JGeometry::TVec3<f32> goal(unk104.getPoint());
		JGeometry::TVec3<f32> dir;
		dir.set(unk104.getPoint().x - mPosition.x, 0.0f,
		        unk104.getPoint().z - mPosition.z);
		if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f)
			dir.x += 1.0f;
		MsVECNormalize((Vec*)&dir, (Vec*)&dir);

		TMsRange<f32> range(-20.0f, 20.0f);
		f32 dist    = mSaveParams->getSLMoveDist();
		f32 jumpSp  = mSaveParams->getSLMoveJumpSp();
		f32 scatter = 1.0f;
		if (mSpine->getCurrentNerve() == &TNervePopoAttack::theNerve()) {
			jumpSp  = mSaveParams->getSLAttackJumpSp();
			dist    = mSaveParams->getSLAttackDist();
			scatter = 10.0f;
			setBckAnm(0);
		}
		goal.x = scatter * range.rand() + (dir.x * dist + mPosition.x);
		goal.z = scatter * range.rand() + (dir.z * dist + mPosition.z);
		goal.y = mPosition.y;

		f32 rate = 1.0f;
		if (mSpine->getCurrentNerve() == &TNerveWalkerEscape::theNerve()) {
			rate = 1.2f;
			setBckAnm(5);
		}
		mVelocity = calcVelocityToJumpToY(goal, jumpSp * rate, getGravityY());
		mPosition.y += 2.0f;
		onLiveFlag(LIVE_FLAG_AIRBORNE);
		if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()) {
			setGoalPath(TPathNode(goal));
			setBckAnm(5);
		}
	} else {
		if (mVelocity.y > 1.5f)
			mPosition.y += 0.5f * mVelocity.y;
		if (mVelocity.y < -1.0f)
			mPosition.y += 0.2f * mVelocity.y;
	}

	JGeometry::TVec3<f32> vel(mVelocity);
	mRollAngle += 1.0f;
	if (mSpine->getCurrentNerve() == &TNervePopoAttack::theNerve())
		mRollAngle += 2.0f;
	if (mRollAngle > 360.0f)
		mRollAngle -= 360.0f;
	if (!mRollSw)
		mRollAngle = 0.0f;

	if (JGeometry::TVec3<f32>(mVelocity).y > 0.0f)
		walkToCurPathNode(0.0f, mTurnSpeed, 0.0f);
}

// Sibling `if`s, not `else if`: the walker test has to reload getCurrentNerve
// after the first arm's theNerve expansions. The nerves are exclusive, so the
// missing retail `b` past the second test is dead. Frame lands at 0xa0 through
// getSpine() on the four compares, SMS_GetMarioPos, and getPosition components.
// TODO: TVec3 temporaries sit 4 bytes high; `else if`/`return` restores the
// `b` but CSEs the nerve back into r28.
void TPopo::attackToMario()
{
	TPopoManager* manager = (TPopoManager*)mManager;
	if (getSpine()->getCurrentNerve() == &TNervePopoAttack::theNerve()
	    || getSpine()->getCurrentNerve() == &TNervePopoWait::theNerve()) {
		if (manager->mIsNozzleFree)
			mSpine->pushNerve(&TNervePopoPossessedNozzle::theNerve());
	}
	if (getSpine()->getCurrentNerve() == &TNerveWalkerEscape::theNerve()
	    || getSpine()->getCurrentNerve()
	           == &TNerveWalkerGraphWander::theNerve()) {
		sendAttackMsgToMario();
		JGeometry::TVec3<f32> push(0.0f, 0.0f, 0.0f);
		JGeometry::TVec3<f32> dir(getPosition().x - SMS_GetMarioPos().x,
		                          getPosition().y - SMS_GetMarioPos().y,
		                          getPosition().z - SMS_GetMarioPos().z);
		MsVECNormalize((Vec*)&dir, (Vec*)&dir);
		mVelocity.x = dir.x;
		mVelocity.z = dir.z;
		dir.scale(mBodyScale * mBodyRadius);
		push.add(dir);
		mLinearVelocity = push;
	}
}

void TPopo::calcRootMatrix()
{
	gpCurPopo = this;
	MtxPtr centerMtx = getModel()->getAnmMtx(mCenterJntIndex);
	mCollision->mPosition.set(centerMtx[0][3], centerMtx[1][3],
	                          centerMtx[2][3]);

	if (mIsPossessed) {
		unk190 = 0.8f * mPumpScale / mSaveParams->getSLWaterScaleMax();
		if (unk190 < mColMinVal)
			unk190 = mColMinVal;
		expandCollision();
		getModel()->setBaseScale(mScaling);

		TPosition3f mtx;
		if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve()) {
			mtx.translation(mPosition.x, mPosition.y, mPosition.z);
		} else {
			MTXCopy(SMS_GetMarioWaterGun()->getEmitMtx(0), (MtxPtr)mtx);
			// TODO: the original keeps each column vector on the stack and
			// squares it from memory (unfused); every spelling tried here is
			// scalar-replaced and fused instead. Making
			// TRotation3::getXDir/getYDir/getZDir assign per component -- which
			// is what the interleaved lfs/stfs here asks for -- gains 0.05
			// points and costs JPABaseEmitter::calcEmitterGlobalParams 5.6, so
			// the columns have to be read out here instead (rocket.cpp already
			// does that; see the note on getXDir in JGRotation3.hpp).
			JGeometry::TVec3<f32> col0;
			mtx.getXDir(col0);
			f32 len0 = col0.length();
			JGeometry::TVec3<f32> col1;
			mtx.getYDir(col1);
			f32 len1 = col1.length();
			JGeometry::TVec3<f32> col2;
			mtx.getZDir(col2);
			f32 len2 = col2.length();
			if (len0 != 0.0f) {
				mtx.ref(0, 0) /= len0;
				mtx.ref(1, 0) /= len0;
				mtx.ref(2, 0) /= len0;
			}
			if (len1 != 0.0f) {
				mtx.ref(0, 1) /= len1;
				mtx.ref(1, 1) /= len1;
				mtx.ref(2, 1) /= len1;
			}
			if (len2 != 0.0f) {
				mtx.ref(0, 2) /= len2;
				mtx.ref(1, 2) /= len2;
				mtx.ref(2, 2) /= len2;
			}

			TPosition3f nozzle;
			nozzle.translation(7.0f * mPumpScale + mNozzleOffsetZ, 0.0f, 0.0f);
			MTXConcat((MtxPtr)mtx, (MtxPtr)nozzle, (MtxPtr)mtx);
			TPosition3f body;
			body.translation(mTestBodyScale * mPumpScale, 0.0f, 0.0f);
			MTXConcat((MtxPtr)mtx, (MtxPtr)body, (MtxPtr)body);
			mPosition.x = body.ref(0, 3);
			mPosition.y = body.ref(1, 3) - mColOffsetY * mPumpScale;
			mPosition.z = body.ref(2, 3);

			if (mIsPumping) {
				MtxPtr pumpMtx = mPumpMtx;
				MTXCopy(getMActor()->getModel()->getAnmMtx(mCenterJntIndex),
				        pumpMtx);
				pumpMtx[0][3] = body.ref(0, 3);
				pumpMtx[1][3] = body.ref(1, 3);
				pumpMtx[2][3] = body.ref(2, 3);
				JPABaseEmitter* emitter
				    = gpMarioParticleManager->emitAndBindToMtxPtr(
				        0x13D, pumpMtx, 1, this);
				if (emitter)
					emitter->setGlobalScale(mEffectScale);
			}
		}

		Mtx rot;
		MsMtxSetRotRPH(rot, mTestAng_x, mTestAng_y, mTestAng_z);
		MTXConcat((MtxPtr)mtx, rot, (MtxPtr)mtx);
		MTXCopy((MtxPtr)mtx, getModel()->getBaseTRMtx());
	} else {
		TSpineEnemy::calcRootMatrix();
	}
}

void TPopo::kill()
{
	releaseNozzle();
	TSmallEnemy::kill();
	getCollision()->onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TPopo::forceKill()
{
	const TBGCheckData* ground = mGroundPlane;
	if ((!ground->isIllegalData()
	     && (ground->isDeathPlane() || ground->isPool()
	         || ground->isWaterSurface())
	     && !isAirborne() && !checkLiveFlag(LIVE_FLAG_UNK10))
	    || !gpMap->isInArea(mPosition.x, mPosition.z)) {
		if (mSpine->getCurrentNerve() == &TNervePopoExplosion::theNerve())
			return;
		mSpine->reset();
		mSpine->setNext(&TNervePopoExplosion::theNerve());
		mSpine->pushAfterCurrent(mSpine->getDefault());
		onLiveFlag(LIVE_FLAG_UNK20000);
		mHitPoints = 1;
	}
}

void TPopo::bind()
{
	mCollision->checkHit();
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	if ((mSpine->getCurrentNerve() == &TNervePopoPossessedNozzle::theNerve()
	     && mPumpScale > 1.2f && mExplosionSw)
	    || mSpine->getCurrentNerve() == &TNervePopoFly::theNerve()) {
		mGroundHeight = gpMap->checkGround(
		    mPosition.x, mPosition.y + mHeadHeight, mPosition.z, &mGroundPlane);
		if (mPosition.y <= 30.0f + mGroundHeight
		    || (abs(JGeometry::TVec3<f32>(mVelocity).x) < 1.0f
		        && abs(JGeometry::TVec3<f32>(mVelocity).z) < 1.0f))
			mSpine->pushNerve(&TNervePopoExplosion::theNerve());

		TBGWallCheckRecord record(mPosition.x, mPosition.y, mPosition.z,
		                          mPumpScale * (mBodyScale * mWallRadius), 1,
		                          0);
		if (gpMap->isTouchedWallsAndMoveXZ(&record))
			mSpine->pushNerve(&TNervePopoExplosion::theNerve());
		else if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve())
			TLiveActor::bind();
	} else {
		TLiveActor::bind();
	}
}

bool TPopo::isHitValid(u32 message)
{
	if (message == HIT_MESSAGE_UNKB)
		return true;
	if (message <= HIT_MESSAGE_HIP_DROP)
		mSpine->pushNerve(&TNervePopoExplosion::theNerve());
	return false;
}

// UNUSED, 0x8c in the map: the body rolls except while flying.
bool TPopo::isRollJump()
{
	if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve())
		return false;
	return true;
}

// UNUSED, 0xf0 in the map: the joint callbacks only act on the possessed or
// flying body.
bool TPopo::isUseScaleCallBack()
{
	if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve()
	    || mSpine->getCurrentNerve() == &TNervePopoExplosion::theNerve()
	    || mIsPossessed)
		return true;
	return false;
}

bool TPopo::isFindMario(float scale)
{
	if (mSpine->getTime() > 100
	    && !gpMarioOriginal->checkFlag(MARIO_FLAG_VISIBLE)) {
		TSmallEnemyParams* params = (TSmallEnemyParams*)getSaveParam();
		// Retail copies Mario's position into a stack temp before the
		// call (`lfs 8/4/0`, `stfs 0/4/8`): a three-argument `set`, not
		// the `TVec3&` SMS_GetMarioPos returns. Only the x read goes
		// through the accessor -- its reference temporary is the 8 bytes
		// of low region the frame wants.
		JGeometry::TVec3<f32> marioPos;
		marioPos.set(SMS_GetMarioPos().x, gpMarioPos->y, gpMarioPos->z);
		f32 length = params->getSLSearchLength();
		length *= scale;
		f32 angle = params->getSLSearchAngle();
		angle *= scale;
		f32 aware = params->getSLSearchAware();
		aware *= scale;
		if (isInSight(marioPos, length, angle, aware))
			return true;
	}
	return false;
}

bool TPopo::isCollidMove(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve()
	    && param_1->receiveMessage(this, HIT_MESSAGE_TRAMPLE))
		mSpine->pushNerve(&TNervePopoExplosion::theNerve());
	return false;
}

// UNUSED, 0x24 in the map: hands the nozzle back.
void TPopo::releaseNozzle()
{
	if (mIsPossessed) {
		((TPopoManager*)mManager)->mIsNozzleFree = 1;
		mIsPossessed                             = 0;
	}
}

static inline TWaterEmitInfo* PopoFlyWater(TPopoManager* manager)
{
	TWaterEmitInfo* water = manager->mFlyWater;
	return water;
}

static inline TPopoManager* PopoFlyManager(TPopo* popo)
{
	TPopoManager* manager = (TPopoManager*)popo->mManager;
	return manager;
}

static inline J3DModel* PopoFlyModel(TPopo* popo)
{
	return popo->getModel();
}

void TPopo::flyBehavior()
{
	mFlyTimer++;
	if (mFlyTimer > mSaveParams->getSLFlyLimitTime()) {
		mFlyTimer = 0;
		mSpine->pushNerve(&TNervePopoExplosion::theNerve());
	}

	if (mPumpScale > 1.0f)
		mPumpScale *= 0.999f;

	MtxPtr mtx;
	JGeometry::TVec3<f32> pos;
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		pos = mPosition;
	} else {
		mtx = PopoFlyModel(this)->getAnmMtx(mMouthJntIndex);
		pos.x      = mtx[0][3];
		pos.y      = mtx[1][3];
		pos.z      = mtx[2][3];
	}
	TPopoManager* manager = PopoFlyManager(this);
	PopoFlyWater(manager)->mPos.value = pos;
	gpModelWaterManager->emitRequest(*manager->mFlyWater);

	if (gpMSound->gateCheck(0x20CE))
		MSoundSESystem::MSoundSE::startSoundActor(0x20CE, &mPosition, 0,
		                                          nullptr, 0, 4);
}

// Frame 0x90: bound pump (two sites), bound manager, bound mExplosionWater.
// mNum's address and value have to be taken after the named pump/scaleMax
// loads so they sit between those lfs and the fdivs; `num = n; num *= ratio`
// keeps the product in the converted-int FPR.
// TODO: the two vector temporaries still sit 4 bytes low (open "4 low" class).
static inline f32 PopoPumpScale(TPopo* popo)
{
	f32 scale = popo->mPumpScale;
	return scale;
}

static inline TPopoManager* PopoExplosionManager(TPopo* popo)
{
	TLiveManager* live    = popo->mManager;
	TPopoManager* manager = (TPopoManager*)live;
	return manager;
}

static inline TWaterEmitInfo* PopoExplosionWater(TPopoManager* manager)
{
	TWaterEmitInfo* water = manager->mExplosionWater;
	return water;
}

void TPopo::explosion()
{
	if (PopoPumpScale(this) > 1.0f)
		mPumpScale *= 0.9f;

	TPopoManager* manager = PopoExplosionManager(this);
	JGeometry::TVec3<f32> pos(mPosition);
	pos.y += 100.0f;
	if (getSpine()->getTime() % 2 == 0) {
		JGeometry::TVec3<f32>& dirValue
		    = PopoExplosionWater(manager)->mDir.value;
		JGeometry::TVec3<f32> dir(dirValue);
		dir.y *= -1.0f;
		dirValue = dir;
	}

	TWaterEmitInfo* water = PopoExplosionWater(manager);
	f32 pump              = PopoPumpScale(this);
	f32 scaleMax          = mSaveParams->mSLWaterScaleMax.value;
	s32& numValue         = water->mNum.value;
	f32 num               = numValue;
	num *= pump / scaleMax;
	if (num < 2.0f)
		num = 2.0f;
	numValue = num;
	PopoExplosionWater(manager)->mPos.value = pos;
	gpModelWaterManager->emitRequest(*manager->mExplosionWater);
}

static inline MSound* PopoPossessedSound()
{
	MSound* sound = gpMSound;
	return sound;
}

static inline MActor* PopoPossessedActor(TPopo* popo)
{
	MActor* actor = popo->getMActor();
	return actor;
}

void TPopo::possessedIn()
{
	TMActorKeeper* keeper = mMActorKeeper;
	mMActor               = keeper->getMActor("popoH.bmd");
	setBckAnm(3);
	PopoPossessedActor(this)->setBtpFromIndex(0);
	getMActor()->setFrameRate(0.0f, ANM_TYPE_BTP);
	if (!mExplosionSw)
		onHitFlag(HIT_FLAG_NO_COLLISION);
	getMActor()->setBrkFromIndex(0);
	getMActor()->getFrameCtrl(ANM_TYPE_BRK)->setFrame(0.0f);
	mBrkFrames = 30.0f;
	getMActor()->setFrameRate(0.0f, ANM_TYPE_BRK);
	offLiveFlag(LIVE_FLAG_UNK10);
	mRollAngle   = 90.0f;
	mIsPossessed = 1;
	if (PopoPossessedSound()->gateCheck(0x2861))
		MSoundSESystem::MSoundSE::startSoundActor(0x2861, &mPosition, 0,
		                                          nullptr, 0, 4);
	mIsLevelReached = 0;
	unk1CD          = 0;
}

// UNUSED, 0x88 in the map.
void TPopo::explosionEffect()
{
	MtxPtr mtx = getMActor()->getModel()->getAnmMtx(mCenterJntIndex);
	mEffectPos.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	gpMarioParticleManager->emit(0xA1, &mEffectPos, 0, nullptr);
	gpMarioParticleManager->emit(0xA2, &mEffectPos, 0, nullptr);
	if (gpMSound->gateCheck(0x297F))
		MSoundSESystem::MSoundSE::startSoundActor(0x297F, &mPosition, 0,
		                                          nullptr, 0, 4);
}

void TPopo::thrownByChorobei()
{
	mSpine->initWith(&TNervePopoThrown::theNerve());
}

const char** TPopo::getBasNameTable() const { return popo_bastable; }

// Binding level worth +16 of low region, landing
// TNervePopoPossessedNozzle::execute's frame at 0x40 (batch 124).
static inline MActor* PopoGetMActor(const TPopo* p)
{
	MActor* mActor = p->getMActor();
	return mActor;
}

DEFINE_NERVE(TNervePopoPossessedNozzle, TLiveActor)
{
	TPopo* popo = (TPopo*)spine->getBody();

	if (spine->getTime() == 0) {
		TPopoManager* manager = (TPopoManager*)popo->mManager;
		if (!manager->mIsNozzleFree) {
			spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
			return TRUE;
		}
		manager->mIsNozzleFree = 0;
		popo->possessedIn();
	}

	if (popo->checkCurAnmEnd(0)) {
		if (popo->unsetUnk165()) {
			popo->setBckAnm(3);
			PopoGetMActor(popo)->setFrameRate(SMSGetAnmFrameRate(),
			                                  ANM_TYPE_BTP);
		} else {
			popo->setBckAnm(4);
			popo->getMActor()->getFrameCtrl(ANM_TYPE_BTP)->setFrame(0.0f);
			popo->getMActor()->setFrameRate(0.0f, ANM_TYPE_BTP);
		}
	}

	if (popo->checkTrigger()) {
		spine->pushAfterCurrent(&TNervePopoFly::theNerve());
		return TRUE;
	}
	return FALSE;
}

static inline int PopoSpineTime(TSpineBase<TLiveActor>* spine)
{
	return spine->getTime();
}

// The two-local form of the `getBody()` binder: 0x10 of low region, which is
// what this nerve's frame wants.
// TODO: setGoalPathMario's TPathNode temporary still sits 4 bytes low.
static inline TPopo* PopoAttackBody(TSpineBase<TLiveActor>* spine)
{
	TLiveActor* body = spine->getBody();
	TPopo* popo      = (TPopo*)body;
	return popo;
}

DEFINE_NERVE(TNervePopoAttack, TLiveActor)
{
	TPopo* popo = PopoAttackBody(spine);

	if (PopoSpineTime(spine) == 0)
		popo->setGoalPathMario();

	if (!popo->isAirborne()) {
		if (!((TPopoManager*)popo->getManager())->mIsNozzleFree)
			return TRUE;
		if (gpMarioOriginal->checkFlag(MARIO_FLAG_VISIBLE))
			return TRUE;
		if (abs(gpMarioPos->y - popo->mPosition.y)
		    > popo->getSaveParam2()->getSLGiveUpHeight())
			return TRUE;
		if (popo->isResignationAttack())
			return TRUE;
	}

	popo->walkBehavior(0, 1.0f);
	return FALSE;
}

static inline TPopo* PopoFlyBody(TSpineBase<TLiveActor>* spine)
{
	TLiveActor* body = spine->getBody();
	TPopo* popo      = (TPopo*)body;
	return popo;
}

static inline TWaterGun* PopoFlyGun()
{
	TWaterGun* gun = SMS_GetMarioWaterGun();
	return gun;
}

static inline TPopoSaveLoadParams* PopoFlyParams(TPopo* popo)
{
	TPopoSaveLoadParams* params = popo->getSaveParams();
	return params;
}

// TODO: vel temporary still sits 4 bytes high (retail 0x68, ours 0x6c):
// the open "4 high after the pool is full" class.
DEFINE_NERVE(TNervePopoFly, TLiveActor)
{
	TPopo* popo = PopoFlyBody(spine);

	if (spine->getTime() == 0) {
		popo->setBckAnm(2);
		MtxPtr emitMtx = PopoFlyGun()->getEmitMtx(0);
		f32 speed      = PopoFlyParams(popo)->getSLReleaseSpeed();
		speed *= popo->mPumpScale
		         / popo->getSaveParams()->getSLWaterScaleMax();
		JGeometry::TVec3<f32> vel;
		vel.x = speed * emitMtx[0][0];
		vel.y = speed * emitMtx[1][0];
		vel.z = speed * emitMtx[2][0];
		popo->mVelocity = vel;
		popo->onLiveFlag(LIVE_FLAG_AIRBORNE);
		popo->releaseNozzle();
		// Reusing speed for the yaw lands MsGetRotFromZaxisY in f1 with
		// no fmr; a second named local reintroduces it (MapEventMare 359).
		speed = MsGetRotFromZaxisY(vel);
		popo->mRotation.set(0.0f, MsWrap(speed, 0.0f, 360.0f), 0.0f);
		if (TPopo::mExplosionSw)
			popo->offHitFlag(HIT_FLAG_NO_COLLISION);
	} else if (!popo->isAirborne()) {
		spine->pushAfterCurrent(&TNervePopoExplosion::theNerve());
		return TRUE;
	}

	if (spine->getTime() > 5) {
		popo->offHitFlag(HIT_FLAG_NO_COLLISION);
		popo->mCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
	}
	popo->flyBehavior();
	return FALSE;
}

static inline int PopoExplosionEmitTime(TPopo* popo)
{
	TPopoSaveLoadParams* params = popo->getSaveParams();
	int emitTime                = params->getSLExplosionEmitTime();
	return emitTime;
}

static inline TPopo* PopoExplosionBody(TSpineBase<TLiveActor>* spine)
{
	TPopo* popo = (TPopo*)spine->getBody();
	return popo;
}

DEFINE_NERVE(TNervePopoExplosion, TLiveActor)
{
	TPopo* popo = PopoExplosionBody(spine);

	if (spine->getTime() == 0) {
		popo->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
		popo->getMActor()->setFrameRate(0.0f, ANM_TYPE_BCK);
		popo->releaseNozzle();
		popo->onHitFlag(HIT_FLAG_NO_COLLISION);
		popo->onLiveFlag(LIVE_FLAG_UNK8);
		popo->explosionEffect();
	}

	if (spine->getTime() > PopoExplosionEmitTime(popo)) {
		popo->onLiveFlag(LIVE_FLAG_DEAD);
		popo->onLiveFlag(LIVE_FLAG_UNK8);
		popo->offLiveFlag(LIVE_FLAG_HIDDEN);
		popo->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		popo->mHolder = nullptr;
		popo->stopAnmSound();
		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushAfterCurrent(spine->getDefault());
		return TRUE;
	}

	popo->explosion();
	return FALSE;
}

DEFINE_NERVE(TNervePopoWait, TLiveActor)
{
	TPopo* popo = (TPopo*)spine->getBody();

	if (spine->getTime() == 0) {
		popo->onLiveFlag(LIVE_FLAG_UNK10);
		popo->setBckAnm(6);
		popo->setGoalPathMario();
	}
	popo->walkToCurPathNode(0.0f, popo->mTurnSpeed, 0.0f);
	return FALSE;
}

DEFINE_NERVE(TNervePopoThrown, TLiveActor)
{
	TPopo* popo = (TPopo*)spine->getBody();

	if (spine->getTime() > 30 && !popo->isAirborne()) {
		spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
		return TRUE;
	}
	return FALSE;
}
