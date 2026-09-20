#include <Enemy/Cannon.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/PathNode.hpp>
#include <Enemy/Killer.hpp>
#include <Enemy/BombHei.hpp>
#include <Enemy/Popo.hpp>
#include <Enemy/Igaiga.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JMath.hpp>
#include <System/Particles.hpp>
#include <System/MarDirector.hpp>
#include <System/Application.hpp>
#include <System/FlagManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MAnmSound.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

u8 TCannon::mChorobeiJntIdx      = 4;
u8 TCannon::mChorobeiHandJntIdx  = 4;
f32 TCannon::mVelocityRate       = 0.62f;
f32 TCannon::mSearchRate         = 0.02f;

static const char* cannon_bastable[] = {
	nullptr,
	nullptr,
	"/scene/cannon/bas/CannonDom_break.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/cannon/bas/tyorobe_appear1.bas",
	"/scene/cannon/bas/tyorobe_damage1.bas",
	"/scene/cannon/bas/tyorobe_down1.bas",
	"/scene/cannon/bas/tyorobe_hyde1.bas",
	"/scene/cannon/bas/tyorobe_throw1.bas",
	nullptr,
	nullptr,
	nullptr,
};

TCannonSaveLoadParams::TCannonSaveLoadParams(const char* prm)
    : TSmallEnemyParams(prm)
    , PARAM_INIT(mSLHideDist, 300.0f)
    , PARAM_INIT(mSLBombDist, 2000.0f)
    , PARAM_INIT(mSLKillerDist, 10000.0f)
    , PARAM_INIT(mSLBombInterval, 100)
    , PARAM_INIT(mSLKillerInterval, 50)
    , PARAM_INIT(mSLShootInterval, 100)
    , PARAM_INIT(mSLChorobeiAttackRadius, 100.0f)
    , PARAM_INIT(mSLChorobeiAttackHeight, 100.0f)
    , PARAM_INIT(mSLChorobeiDamageRadius, 100.0f)
    , PARAM_INIT(mSLChorobeiDamageHeight, 100.0f)
    , PARAM_INIT(mSLKillerTransYOffset, -50.0f)
    , PARAM_INIT(mSLBombHeiGenerateRate, 0.7f)
    , PARAM_INIT(mSLThrowXZSpeed, 12.0f)
{
	TParams::load(mPrmPath);
}

TCannonManager::TCannonManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TCannonManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TCannonSaveLoadParams("/enemy/cannon.prm");
}

TSpineEnemy* TCannonManager::createEnemyInstance() { return new TCannon("砲台"); }

// Binding level over a raw member read, worth +8 of low region in the two
// constructors below (the same lever as TLiveActor::initAnmSound).
static inline MAnmSound* ChorobeiAnmSound(const TChorobei* p)
{
	MAnmSound* anmSound = p->mAnmSound;
	return anmSound;
}

TChorobei::TChorobei(TCannon* cannon, int jnt_idx, const char* name)
    : THitActor(name)
    , mCannon(cannon)
    , mParts(nullptr)
    , unk70(0.0f)
    , mAnmSound(nullptr)
    , mAnmSoundName(nullptr)
    , mHeightOffset(300.0f)
{
	mParts = new TSharedParts(mCannon, jnt_idx,
	                          "/scene/cannon/tyorobe_model1.bmd", 0x10020000,
	                          3, "<TSharedParts>");
	if (ChorobeiAnmSound(this))
		return;
	mAnmSound = new MAnmSound(SMSGetMSound());
	mAnmSound->initAnmSound(nullptr, 1, 0.0f);
}

void TChorobei::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!mCannon->checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN
	                            | LIVE_FLAG_CLIPPED_OUT)
	    && unk70 == 0.0f) {
		if (cue & CUE_CALC_ANIM) {
			if (mAnmSound != nullptr && mAnmSoundName != nullptr) {
				J3DFrameCtrl* ctrl = mParts->getMActor()->getFrameCtrl(0);
				mAnmSound->animeLoop((Vec*)&mPosition, ctrl->getFrame(),
				                     ctrl->getRate(), 0, 4);
			}
			Mtx mtx;
			MTXCopy(mParts->getConnectedMtx(), mtx);
			mtx[1][3] += mHeightOffset;
			MTXCopy(mtx, mParts->getMActor()->getModel()->getBaseTRMtx());
			mPosition.x = mtx[0][3];
			mPosition.y = mtx[1][3] - 150.0f;
			mPosition.z = mtx[2][3];
		}

		THitActor::perform(cue, graphics);
		mParts->getMActor()->perform(cue, graphics);
	}
}

// UNUSED, 0xac in the map.
void TChorobei::setBckAnm(int idx)
{
	mParts->getMActor()->setBckFromIndex(idx);
	const char** basTable = mCannon->getBasNameTable();
	mAnmSoundName         = !basTable ? nullptr : basTable[idx];
	if (mAnmSoundName != nullptr)
		mAnmSound->initAnmSound(JKRGetResource(mAnmSoundName), 1, 0.0f);
	else
		mAnmSound->initAnmSound(nullptr, 1, 0.0f);
}

void TChorobei::checkHit()
{
	for (int i = 0; i < getColNum(); ++i) {
		THitActor* col = getCollision(i);
		if (col->isActorType(0x80000001))
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);

		if (col->isActorType(0x1000001E)) {
			TBombHei* bomb  = (TBombHei*)col;
			TCannon* cannon = mCannon;
			if (cannon->mSpine->getCurrentNerve()
			        != &TNerveCannonDamage::theNerve()
			    && bomb->isDamageToCannon()) {
				cannon->mSpine->pushNerve(&TNerveCannonDamage::theNerve());
				bomb->kill();
			}
		}

		if (col->isActorType(0x1000001F)) {
			TKiller* killer = (TKiller*)col;
			if (killer->isRollFly()) {
				mCannon->mSpine->pushNerve(&TNerveCannonDamage::theNerve());
				killer->kill();
			}
		}
	}
}

BOOL TChorobei::receiveMessage(THitActor*, u32) { return FALSE; }

// UNUSED, 0x70 in the map: the appear animation has run out.
bool TChorobei::isUpEnd()
{
	if (mParts->getMActor()->curAnmEndsNext()
	    && mParts->getMActor()->checkCurBckFromIndex(0xC))
		return true;
	unk70 = 0.0f;
	return false;
}

// UNUSED, 0x70 in the map: the hide animation has run out.
bool TChorobei::isDownEnd()
{
	if (mParts->getMActor()->curAnmEndsNext()
	    && mParts->getMActor()->checkCurBckFromIndex(0xF)) {
		unk70 = 1.0f;
		return true;
	}
	return false;
}

// The same binding level for the dom's constructor (+8 of low region).
static inline MAnmSound* CannonDomAnmSound(const TCannonDom* p)
{
	MAnmSound* anmSound = p->mAnmSound;
	return anmSound;
}

TCannonDom::TCannonDom(TLiveActor* owner, int jnt_idx, SDLModelData* data,
                       u32 flags, const char* name)
    : TSharedParts(owner, jnt_idx, data, flags, name)
    , mAnmSound(nullptr)
    , mAnmSoundName(nullptr)
    , mIsHidden(0)
    , mPitch(0.0f)
    , mRoll(0.0f)
    , mSwingPhase(0.0f)
{
	TMsRange<f32> range(0.0f, 360.0f);
	mSwingPhase = range.rand();
	if (CannonDomAnmSound(this))
		return;
	mAnmSound = new MAnmSound(SMSGetMSound());
	mAnmSound->initAnmSound(nullptr, 1, 0.0f);
}

void TCannonDom::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (unk10->checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN
	                         | LIVE_FLAG_CLIPPED_OUT))
		return;

	if (cue == CUE_CALC_ANIM) {
		if (mAnmSound != nullptr && mAnmSoundName != nullptr) {
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(0);
			mAnmSound->animeLoop((Vec*)&unk10->mPosition, ctrl->getFrame(),
			                     ctrl->getRate(), 0, 4);
		}
		MtxPtr mtx = getConnectedMtx();
		Mtx rot;
		MsMtxSetRotRPH(rot, mPitch, mRoll, 0.0f);
		MTXConcat(mtx, rot, mtx);
		MTXCopy(mtx, getMActor()->getModel()->getBaseTRMtx());
	}
	getMActor()->perform(cue, graphics);
}

// UNUSED, 0xa8 in the map.
void TCannonDom::setBckAnm(int idx)
{
	getMActor()->setBckFromIndex(idx);
	const char** basTable = unk10->getBasNameTable();
	mAnmSoundName         = !basTable ? nullptr : basTable[idx];
	if (mAnmSoundName != nullptr)
		mAnmSound->initAnmSound(JKRGetResource(mAnmSoundName), 1, 0.0f);
	else
		mAnmSound->initAnmSound(nullptr, 1, 0.0f);
}

TCannon::TCannon(const char* name)
    : TSmallEnemy(name)
    , mAttachedBomb(nullptr)
    , mChorobei(nullptr)
    , mSingleDom(nullptr)
    , mShootMtx(nullptr)
    , mDomIdx(0)
    , unk218(0)
    , mBombThrown(0)
    , mBombScale(0.0f)
    , mStage(1)
    , mMarioAnmPlaying(0)
    , mAimAtMario(1)
    , mMareGate(nullptr)
    , mObjCollision(nullptr)
    , mShootMode(0)
    , mPrevYaw(0.0f)
{
	mDomRoll[0] = 0.0f;
	mDomRoll[1] = 0.0f;
	mDomRoll[2] = 0.0f;
}

void TCannon::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	reset();
	mHitPoints  = getMaxHitPoints();
	mStage      = gpApplication.mCurrArea.unk0;
	mInitialPos = mPosition;
}

void TCannon::loadAfter()
{
	SMS_LoadParticle("/scene/cannon/jpa/ms_cannon_a.jpa", 0xE8);
	SMS_LoadParticle("/scene/cannon/jpa/ms_cannon_b.jpa", 0xE9);
	SMS_LoadParticle("/scene/cannon/jpa/ms_cannon_c.jpa", 0xEA);
	SMS_LoadParticle("/scene/cannon/jpa/ms_cannon_d.jpa", 0xEB);
	SMS_LoadParticle("/scene/cannon/jpa/ms_cannon_e.jpa", 0xEC);
	SMS_LoadParticle("/scene/cannon/jpa/ms_cannon_smoke.jpa", 0x166);

	if (mStage == 5) {
		mMareGate = JDrama::TNameRefGen::search<TSpineEnemy>("efMareGate");
		mMareGate->kill();
	}
}

// Binding level worth +16 of low region, landing TCannon::init's frame at
// 0x1a0 (batch 124).
static inline f32
CannonGetSLChorobeiAttackHeight(const TCannonSaveLoadParams* p)
{
	f32 sLChorobeiAttackHeight = p->getSLChorobeiAttackHeight();
	return sLChorobeiAttackHeight;
}

// Binding level worth +16 of low region, landing TCannon::init's frame at
// 0x1a0 (batch 124).
static inline f32
CannonGetSLChorobeiAttackRadius(const TCannonSaveLoadParams* p)
{
	f32 sLChorobeiAttackRadius = p->getSLChorobeiAttackRadius();
	return sLChorobeiAttackRadius;
}

void TCannon::init(TLiveManager* manager)
{
	static const char* sCannonDomPartsJointTable[]
	    = { "nullC", "nullB", "nullA" };

	TSmallEnemy::init(manager);
	mActorType  = 0x1000001C;
	unk150      = 0x11;
	mSaveParams = (TCannonSaveLoadParams*)getSaveParam();
	setBckAnm(3);

	void* domResource     = JKRGetResource("/scene/cannon/cannon_Dom.bmd");
	SDLModelData* domData = new SDLModelData(
	    J3DModelLoaderDataBase::load(domResource, 0x10050000));

	mInitialYaw = mRotation.y;
	mStage      = gpApplication.mCurrArea.unk0;
	if (mStage == 5 || mStage == 9) {
		mSpine->initWith(&TNerveCannonSearch::theNerve());
		if (mStage == 9) {
			mAimAtMario = 0;
			setGoalPath(
			    TPathNode(JGeometry::TVec3<f32>(-565.0f, 8500.0f, 7675.0f)));
		} else {
			setGoalPathMario();
		}

		mChorobei = new TChorobei(this, 0, "チョロベー");
		for (u8 i = 0; i < mChorobei->mParts->getMActor()
		                       ->getModel()
		                       ->getModelData()
		                       ->getJointNum();
		     ++i) { }
		mChorobei->initHitActor(0x1000001D, 3, 0x90000000,
		                        CannonGetSLChorobeiAttackRadius(mSaveParams),
		                        CannonGetSLChorobeiAttackHeight(mSaveParams),
		                        mSaveParams->getSLChorobeiDamageRadius(),
		                        mSaveParams->getSLChorobeiDamageHeight());
		TIdxGroupObj* group
		    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
		group->getChildren().push_back(mChorobei);

		JUTNameTab* jointNames
		    = getMActor()->getModel()->getModelData()->getJointName();
		for (int i = 0; i < 3; ++i) {
			int jnt  = jointNames->getIndex(sCannonDomPartsJointTable[i]);
			mDoms[i] = new TCannonDom(this, jnt, domData, 3, "砲身");
			mDomCollisions[i] = new TMapCollisionMove;
			mDomCollisions[i]->init("/cannon/CannonDom", 2, this);
			mDomCollisions[i]->setUpTrans(mPosition);
		}
		mFutaCollision = new TMapCollisionMove;
		mFutaCollision->init("/cannon/CannonFuta", 2, this);
		mFutaCollision->setUpTrans(mPosition);
	} else {
		mSpine->initWith(&TNerveCannonObject::theNerve());
		int jnt = getMActor()
		              ->getModel()
		              ->getModelData()
		              ->getJointName()
		              ->getIndex("nullA");
		mSingleDom = new TCannonDom(this, jnt, domData, 3, "砲身");
		mSingleDom->mIsHidden = 1;
	}

	void* marioResource     = JKRGetResource("/scene/cannon/hodai_mario.bmd");
	SDLModelData* marioData = new SDLModelData(
	    J3DModelLoaderDataBase::load(marioResource, 0x10010000));
	mMarioParts = new TSharedParts(this, 0, marioData, 3, "<TSharedParts>");
	for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); ++i) { }

	entryObjCollision();
}

void TCannon::reset()
{
	TSmallEnemy::reset();
	mHitPoints = getMaxHitPoints();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mDomIdx     = 1;
	mHeadHeight = 40.0f;
	onLiveFlag(LIVE_FLAG_UNK10);
	mPrevYaw = mRotation.y;
	if (mStage == 9) {
		mAimAtMario = 0;
		setGoalPath(
		    TPathNode(JGeometry::TVec3<f32>(-565.0f, 8500.0f, 7675.0f)));
	}
}

void TCannon::moveObject()
{
	TSmallEnemy::moveObject();
	if (mStage != 5 && mStage != 9)
		return;

	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		mChorobei->mPosition = getPosition();
	} else {
		MtxPtr mtx             = getModel()->getAnmMtx(mChorobeiJntIdx);
		mChorobei->mPosition.x = mtx[0][3];
		mChorobei->mPosition.y = mtx[1][3] - 100.0f;
		mChorobei->mPosition.z = mtx[2][3];
	}
	mChorobei->checkHit();

	// The slot order (the zeroing vector above `vel`) says both were
	// function-scope locals and the zero one was declared first.
	JGeometry::TVec3<f32> zeroVel;
	JGeometry::TVec3<f32> vel(mVelocity);
	mPosition.y += vel.y;
	mVelocity.y -= getGravityY();
	if (mPosition.y < mInitialPos.y) {
		zeroVel.set(0.0f, 0.0f, 0.0f);
		mVelocity   = zeroVel;
		mPosition.y = mInitialPos.y;
	}

	updateAttachPos();
}

BOOL TCannon::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == 0x40000235 && message == HIT_MESSAGE_TAKE
	    && mHolder == nullptr) {
		mHolder = (TTakeActor*)sender;
		return TRUE;
	}
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER)
		return TRUE;
	return FALSE;
}

// UNUSED, 0x12c in the map: the lid's collision quad follows the barrel joint.
void TCannon::calcObjCollision()
{
	static const f32 xzTable[4][2] = {
		{ 1.0f, -1.0f },
		{ 1.0f, 1.0f },
		{ -1.0f, 1.0f },
		{ -1.0f, -1.0f },
	};

	mHeadHeight = 50.0f;
	JGeometry::TVec3<f32> base(mPosition);
	base.y = getMActor()->getModel()->getAnmMtx(4)[1][3];
	for (int i = 0; i < 4; ++i) {
		mObjVertices[i] = base;
		mObjVertices[i].x += 200.0f * xzTable[i][0];
		mObjVertices[i].z += 200.0f * xzTable[i][1];
	}
	mObjCollision->setVertexData(0, mObjVertices[2], mObjVertices[1],
	                             mObjVertices[0]);
	mObjCollision->setVertexData(1, mObjVertices[0], mObjVertices[3],
	                             mObjVertices[2]);
}

// UNUSED, 0x58 in the map.
void TCannon::entryObjCollision()
{
	mObjCollision = new TMapCollisionMove;
	mObjCollision->init(2, 0, 0, nullptr);
}

const char** TCannon::getBasNameTable() const { return cannon_bastable; }

// Binding level over the Mario-parts actor: +8 of low region at each of the
// first two sites, landing TCannon::perform's frame at 0x188.
static inline MActor* CannonMarioPartsMActor(TCannon* p)
{
	MActor* actor = p->mMarioParts->getMActor();
	return actor;
}

void TCannon::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);

	if (mMarioAnmPlaying) {
		CannonMarioPartsMActor(this)->perform(cue, graphics);
		if ((cue & CUE_MOVE) && CannonMarioPartsMActor(this)->curAnmEndsNext())
			mMarioAnmPlaying = 0;

		if ((cue & CUE_CALC_ANIM) && mStage == 1) {
			if (mMarioParts->getMActor()->getFrameCtrl(0)->checkPass(174.0f)) {
				MtxPtr mtx = mSingleDom->getMActor()->getModel()->getAnmMtx(0);
				gpMarioParticleManager->emitAndBindToMtxPtr(0xE8, mtx, 0,
				                                            nullptr);
				gpMarioParticleManager->emitAndBindToMtxPtr(0xE9, mtx, 0,
				                                            nullptr);
				gpMarioParticleManager->emitAndBindToMtxPtr(0xEA, mtx, 0,
				                                            nullptr);
				gpMarioParticleManager->emitAndBindToMtxPtr(0xEB, mtx, 0,
				                                            nullptr);
				gpMarioParticleManager->emitAndBindToMtxPtr(0xEC, mtx, 0,
				                                            nullptr);
			}
			if (mMarioParts->getMActor()->getFrameCtrl(0)->getFrame()
			    > 175.0f) {
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    0x166, mMarioParts->getMActor()->getModel()->getAnmMtx(0),
				    1, this);
			}
		}
	}

	if (mStage == 5 || mStage == 9) {
		mChorobei->perform(cue, graphics);
		if (cue & CUE_ENTRY) {
			if (mSpine->getCurrentNerve() == &TNerveCannonDamage::theNerve()) {
				mChorobei->mParts->getMActor()->offMakeDL();
				SMS_AddDamageFogEffect(
				    mChorobei->mParts->getMActor()->getModel()->getModelData(),
				    mPosition, graphics);
			}
		}

		for (int i = 0; i < 3; ++i) {
			TCannonDom* dom = mDoms[i];
			if (dom->mIsHidden)
				continue;

			if (cue == CUE_MOVE) {
				f32 bombDist = mSaveParams->getSLBombDist();
				if (bombDist * bombDist < mDistToMarioSquared
				    && mStage == 5) {
					dom->mRoll = mDomRoll[i];
					if (i != mDomIdx
					    && mSpine->getCurrentNerve()
					           != &TNerveCannonObject::theNerve()) {
						mDoms[i]->mSwingPhase += 1.0f;
						if (mDoms[i]->mSwingPhase > 360.0f)
							mDoms[i]->mSwingPhase -= 360.0f;
						if (i == 2)
							mDoms[i]->mPitch
							    = -10.0f * JMASin(0.5f * mDoms[i]->mSwingPhase);
						else
							mDoms[i]->mPitch
							    = -20.0f * JMASin(0.5f * mDoms[i]->mSwingPhase);
					}
				} else {
					dom->mSwingPhase = 0.0f;
					mDoms[i]->mPitch *= 0.99f;
					mDomCollisions[i]->moveMtx(mDoms[i]->getConnectedMtx());
				}
			}
			mDoms[i]->perform(cue, graphics);
		}
	} else {
		if (!mSingleDom->mIsHidden)
			mSingleDom->perform(cue, graphics);
	}
}

void TCannon::calcRootMatrix()
{
	if (mSpine->getCurrentNerve() != &TNerveCannonObject::theNerve())
		calcObjCollision();

	if (mHolder) {
		MtxPtr mtx = mHolder->getTakingMtx();
		if (mSpine->getCurrentNerve() == &TNerveCannonObject::theNerve()) {
			MTXCopy(mtx, getModel()->getBaseTRMtx());
			mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		} else {
			if (gpMarDirector->isDemoModeNow())
				mRotation.y = -80.0f;
			mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
			MsMtxSetXYZRPH(getMActor()->getModel()->getBaseTRMtx(), mPosition.x,
			               mPosition.y, mPosition.z,
			               DEG2SHORTANGLE(mRotation.x),
			               DEG2SHORTANGLE(mRotation.y),
			               DEG2SHORTANGLE(mRotation.z));
		}
	} else {
		TSpineEnemy::calcRootMatrix();
	}

	if (mChorobei && mChorobei->mParts->getMActor()->checkCurBckFromIndex(0xE)
	    && mChorobei->mParts->getMActor()->getFrameCtrl(0)->checkPass(2.0f)) {
		for (int i = 0; i < 3; ++i) {
			MtxPtr mtx = mDoms[i]->getConnectedMtx();
			mEffectPos.set(mtx[0][3], mtx[1][3], mtx[2][3]);
			mDoms[i]->setBckAnm(2);
			gpMarioParticleManager->emit(0x14, &mEffectPos, 0, nullptr);
			gpMarioParticleManager->emit(0x13, &mEffectPos, 0, nullptr);
			gpMarioParticleManager->emit(0x12, &mEffectPos, 0, nullptr);
		}
	}
}

MtxPtr TCannon::getTakingMtx()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		mTakingMtx.translation(mChorobei->getPosition().x,
		                       800.0f + mChorobei->mPosition.y,
		                       mChorobei->mPosition.z);
		return (MtxPtr)mTakingMtx;
	}

	mTakingMtx.translation(mChorobei->mPosition.x,
	                       mChorobei->mParts->getMActor()->getModel()->getAnmMtx(
	                           mChorobeiHandJntIdx)[1][3],
	                       mChorobei->mPosition.z);
	return (MtxPtr)mTakingMtx;
}

void TCannon::bombSet()
{
	f32 r       = TMsRange<f32>(0.0f, 1.0f).rand();
	mBombThrown = 0;
	mHeldBomb   = nullptr;

	TBombHei* bomb;
	if (r < mSaveParams->getSLBombHeiGenerateRate()) {
		bomb = (TBombHei*)gpConductor->makeOneEnemyAppear(
		    mPosition, "ボム兵マネージャー", 1);
	} else if (TMsRange<int>(0, 100).rand() % 2 == 1) {
		bomb = (TBombHei*)gpConductor->makeOneEnemyAppear(
		    mPosition, "ポポマネージャー", 1);
		if (bomb)
			((TPopo*)bomb)->thrownByChorobei();
	} else {
		bomb = (TBombHei*)gpConductor->makeOneEnemyAppear(
		    mPosition, "ハムクリマネージャー", 1);
	}

	hitHead(bomb);
}

void TCannon::bombShoot()
{
	if (mHeldBomb == nullptr)
		return;

	JGeometry::TVec3<f32> dir(gpMarioPos->x - mPosition.x, 0.0f,
	                          gpMarioPos->z - mPosition.z);
	if (dir.x == 0.0f && dir.z == 0.0f)
		dir.x = 1.0f;
	MsVECNormalize((Vec*)&dir, (Vec*)&dir);

	Mtx mtx;
	MsMtxSetRotRPH(mtx, 0.0f,
	               mRotation.y + TMsRange<f32>(-30.0f, 30.0f).rand(), 0.0f);

	f32 speed = mSaveParams->getSLThrowXZSpeed();
	dir.y     = speed;
	dir.x *= speed;
	dir.z *= speed;
	if (mBombThrown) {
		TBombHei* bomb = mHeldBomb;
		bomb->mVelocity.set(dir);
		bomb->offLiveFlag(LIVE_FLAG_UNK10);
	} else {
		TBombHei* bomb  = mHeldBomb;
		bomb->mVelocity = dir;
		bomb->onLiveFlag(LIVE_FLAG_AIRBORNE);
		bomb->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
	}
	mHeldBomb->mPosition.y += 2.0f;
	mHeldBomb->receiveMessage(this, HIT_MESSAGE_PUT);
}

// UNUSED, 0x5c in the map.
void TCannon::bombScaleUp()
{
	if (mHeldBomb) {
		mHeldBomb->mScaling.x
		    = MsClamp(mHeldBomb->mScaling.x + 0.2f * mBombScale, 0.0f,
		              mBombScale);
		// TODO: the original really does overwrite the clamped value with
		// the cannon's own x scale on all three axes.
		mHeldBomb->mScaling.set(mScaling.x, mScaling.x, mScaling.x);
	}
}

// UNUSED, 0x154 in the map: hands a freshly made bomb to the chorobei.
void TCannon::hitHead(TBombHei* bomb)
{
	if (mHeldBomb == nullptr) {
		mHeldBomb = bomb;
		if (bomb) {
			bomb->reset();
			bomb->getMActor()->setFrameRate(0.0f, 0);
		}
	}

	if (mHeldBomb) {
		mHeldBomb->mPosition = mChorobei->mPosition;
		mBombScale           = mHeldBomb->mScaling.x;
		mHeldBomb->mScaling.set(0.0f, 0.0f, 0.0f);
		mHeldBomb->mRotation = mRotation;
		if (mHeldBomb->receiveMessage(this, HIT_MESSAGE_TAKE))
			mHeldObject = mHeldBomb;
	}
}

// UNUSED, 0x1c0 in the map: a bomb sitting on the chorobei's head.
void TCannon::updateAttachPos()
{
	if (mAttachedBomb) {
		// `a = b + c` reaches the map's out-of-line TVec3::add: operator= is
		// one inline level and the sum nested in its argument two more.
		mAttachedBomb->mPosition = mAttachOffset + mChorobei->mPosition;
		mAttachedBomb->offLiveFlag(LIVE_FLAG_AIRBORNE);
		if (mSpine->getCurrentNerve() == &TNerveCannonClose::theNerve()) {
			mAttachedBomb->kill();
			mAttachedBomb = nullptr;
			return;
		}
		if (!mAttachedBomb->doKeepDistance()) {
			mAttachedBomb = nullptr;
			mSpine->pushNerve(&TNerveCannonDamage::theNerve());
		}
	}
}

void TCannon::killerShoot()
{
	if (mAimAtMario) {
		TKiller* killer = (TKiller*)gpConductor->makeOneEnemyAppear(
		    mPosition, "キラーマネージャー", 0);
		if (!killer)
			return;

		killer->reset();
		mShootMtx = mDoms[mDomIdx]->getMActor()->getModel()->getBaseTRMtx();
		mShootMtx = mDoms[mDomIdx]->getMActor()->getModel()->getAnmMtx(1);
		TPosition3f mtx;
		mtx.translation(0.0f, -60.0f, 150.0f);
		MTXConcat(mShootMtx, (MtxPtr)mtx, (MtxPtr)mtx);
		killer->mPosition.set(mtx.ref(0, 3), mtx.ref(1, 3), mtx.ref(2, 3));

		f32 marioSpeedX = *gpMarioSpeedX;
		f32 marioSpeedZ = *gpMarioSpeedZ;
		JGeometry::TVec3<f32> target(*gpMarioPos);
		TMsRange<f32> range(-300.0f, 300.0f);
		target.x += range.rand();
		switch (mDomIdx) {
		case 0:
			target.z -= 2.0f * abs(range.rand());
			break;
		case 2:
			target.z += 2.0f * abs(range.rand());
			break;
		}

		JGeometry::TVec3<f32> vel
		    = killer->calcVelocityToJumpToY(target, 5.0f, killer->getGravityY());
		// `a = b - c` is the spelling that puts TVec3::sub at inline depth 4
		// (copy ctor 1, operator- 2, operator-= 3), which is where retail
		// `bl`s it; `diff(target); diff.sub(...)` reaches sub at depth 1 and
		// expands. The retail copy has exactly one copy out of operator-'s
		// by-value operand, so there is no second `diff2` local.
		JGeometry::TVec3<f32> diff = target - mPosition;
		f32 dist                   = MsVECMag2((Vec*)&diff);

		killer->unk1A5 = 0;
		TMsRange<int> irange(0, 100);
		f32 flyTime = abs(dist / (vel.x * mVelocityRate));
		f32 rate    = mVelocityRate;
		if (irange.rand() % 5 == 0) {
			killer->unk1A5 = 1;
		} else {
			if (*gpMarioSpeedX > 2.0f)
				rate = 0.55f;
			if (*gpMarioSpeedX < -2.0f)
				rate = 0.68f;
		}

		f32 dx = marioSpeedX * flyTime;
		f32 dz = marioSpeedZ * flyTime;
		JGeometry::TVec3<f32> lead(target.x + mSearchRate * dx, target.y,
		                           target.z + mSearchRate * dz);
		vel = killer->calcVelocityToJumpToY(lead, 5.0f, killer->getGravityY());
		vel.scale(rate);

		f32 yaw = MsAngleWrap(MsGetRotFromZaxisY(vel));
		killer->mRotation.set(0.0f, yaw, 0.0f);
		killer->mScaling.set(0.1f, 0.1f, 0.1f);
		if (gpMarDirector->mState == TMarDirector::STATE_UNK1) {
			vel.x *= 0.2f;
			vel.y *= 0.4f;
			vel.z *= 0.2f;
			killer->mScaling.set(0.6f, 0.6f, 0.6f);
		}
		killer->setColorType();
		killer->unk1A8   = vel;
		killer->mVelocity = vel;
		killer->onLiveFlag(LIVE_FLAG_AIRBORNE);

		JGeometry::TVec3<f32> toMario = *gpMarioPos - mPosition;
		target.x += toMario.x;
		target.z += toMario.z;
		killer->setGoalPath(TPathNode(target));

		if (gpMSound->gateCheck(MSD_SE_EN_KILLER_FIRE))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_EN_KILLER_FIRE, &killer->mPosition, 0, nullptr, 0, 4);
		if (gpMSound->gateCheck(MSD_SE_EN_KILLER_FLY))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_EN_KILLER_FLY, &killer->mPosition, 0, nullptr, 0, 4);
	} else {
		TIgaiga* igaiga = (TIgaiga*)gpConductor->makeOneEnemyAppear(
		    mPosition, "イガイガマネージャー", 1);
		if (!igaiga)
			return;

		igaiga->reset();
		mShootMtx = mDoms[mDomIdx]->getMActor()->getModel()->getBaseTRMtx();
		mShootMtx = mDoms[mDomIdx]->getMActor()->getModel()->getAnmMtx(1);
		TPosition3f mtx;
		mtx.translation(0.0f, -60.0f, 150.0f);
		MTXConcat(mShootMtx, (MtxPtr)mtx, (MtxPtr)mtx);
		igaiga->mPosition.set(mtx.ref(0, 3), mtx.ref(1, 3), mtx.ref(2, 3));

		JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
		    0xCB, &igaiga->mPosition, 0, DEG2SHORTANGLE(igaiga->mRotation.y),
		    0, 0, nullptr);
		if (emitter) {
			JGeometry::TVec3<f32> scale(1.5f, 1.5f, 1.5f);
			scale.x *= mScaling.x;
			scale.y *= mScaling.y;
			scale.z *= mScaling.z;
			emitter->setGlobalScale(scale);
		}

		JGeometry::TVec3<f32> point;
		igaiga->unk124->getGraph()->getGraphNode(0).getPoint((Vec*)&point);
		mKillerGoal               = point;
		JGeometry::TVec3<f32> vel = igaiga->calcVelocityToJumpToY(
		    point, 10.0f, igaiga->getGravityY());
		igaiga->mRotation = mRotation;
		igaiga->shoot(vel);
	}
}

// UNUSED, 0x24 in the map: the next barrel takes the next shot.
void TCannon::endKillerShoot()
{
	mDomIdx++;
	if (mDomIdx >= 3)
		mDomIdx = 0;
}

// UNUSED, 0x118 in the map: a non-fatal hit.
void TCannon::damage()
{
	setFreezeAnm();
	mChorobei->setBckAnm(0xD);
	MtxPtr mtx = mChorobei->mParts->getMActor()->getModel()->getAnmMtx(0);
	mEffectPos.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    0xC7, &mEffectPos, 0, nullptr);
	if (emitter)
		emitter->setGlobalScale(mChorobei->mScaling);
}

// Binding level over a raw member read: a register lever in
// TCannon::setKillerGoalPoint at an unchanged frame (batch 127).
static inline u8 CannonAimAtMario(const TCannon* p)
{
	u8 aimAtMario = p->mAimAtMario;
	return aimAtMario;
}

void TCannon::setKillerGoalPoint()
{
	if (CannonAimAtMario(this)) {
		s16 angle = TMsRange<f32>(0.0f, 360000.0f).rand();
		JGeometry::TVec3<f32> pos(*gpMarioPos);
		pos.x += 500.0f * JMASCos(angle);
		pos.z += 500.0f * JMASSin(angle);
		setGoalPath(TPathNode(pos));
	} else {
		setGoalPath(TPathNode(mKillerGoal));
	}
	mDoms[mDomIdx]->setBckAnm(1);
}

// UNUSED, 0x20 in the map.
void TCannon::deadCannon()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mChorobei->onHitFlag(HIT_FLAG_NO_COLLISION);
}

// UNUSED, 0x4 in the map.
void TCannon::startDemo() { }

// UNUSED, 0x4 in the map.
void TCannon::startMarioDemo() { }

// UNUSED, 0x2c in the map.
void TCannon::turnToGoal() { walkToCurPathNode(0.0f, mTurnSpeed, 0.0f); }

bool TCannon::isObject()
{
	if (isBckAnm(4) && checkCurAnmEnd(0))
		return true;
	return false;
}

// UNUSED, 0x3c in the map. TODO: no call site survives.
void TCannon::killShootAct()
{
	mShootMode = 0;
	mDomIdx    = 0;
}

// UNUSED, 0xd0 in the map: the Mare gate effect grows out of the cannon.
void TCannon::gateOpen()
{
	if (mMareGate) {
		if (mMareGate->checkLiveFlag(LIVE_FLAG_DEAD)) {
			mMareGate->reset();
			mMareGate->mPosition = mPosition;
			mMareGate->mScaling.set(0.27f, 0.02f, 0.27f);
			mMareGate->getMActor()->setBtk("maregate");
		}
		mMareGate->mScaling.y
		    = MsClamp(1.01f * mMareGate->mScaling.y, 0.0f, 0.22f);
	}
}

void TCannon::startChorobeiShout() { }

// Reproduces retail's out-of-line `bl JGeometry::TVec3<f>::sub` in
// TNerveCannonSearch::execute: the subtraction sits one inline level down.
static inline f32 CannonRotYToPoint(const JGeometry::TVec3<f32>& from,
                                    const JGeometry::TVec3<f32>& to)
{
	JGeometry::TVec3<f32> diff = to - from;
	return MsGetRotFromZaxis(diff).y;
}

DEFINE_NERVE(TNerveCannonOpen, TLiveActor)
{
	TCannon* cannon = (TCannon*)spine->getBody();

	if (spine->getTime() == 0) {
		cannon->mChorobei->setBckAnm(0xC);
		cannon->setBckAnm(3);
	}

	if (cannon->mChorobei->isUpEnd() && cannon->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveCannonSearch::theNerve());
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveCannonSearch, TLiveActor)
{
	TCannon* cannon = (TCannon*)spine->getBody();

	cannon->updateSquareToMario();
	f32 dist = cannon->getDistToMarioSquared();
	if (spine->getTime() == 0) {
		f32 bombDist = cannon->getSaveParams()->getSLBombDist();
		if (dist < bombDist * bombDist)
			cannon->setGoalPathMario();
		cannon->getChorobei()->setBckAnm(0x13);
	}

	if (cannon->getChorobei()->mParts->getMActor()->curAnmEndsNext()
	    && cannon->getChorobei()->mParts->getMActor()->checkCurBckFromIndex(0x13))
		cannon->getChorobei()->setBckAnm(0x12);

	f32 hideDist = cannon->getSaveParams()->getSLHideDist();
	if (dist < hideDist * hideDist) {
		spine->pushAfterCurrent(&TNerveCannonClose::theNerve());
		return TRUE;
	}

	f32 bombDist2 = cannon->getSaveParams()->getSLBombDist();
	if (dist < bombDist2 * bombDist2) {
		if (spine->getTime()
		    > cannon->getSaveParams()->getSLBombInterval()) {
			cannon->mShootMode = 1;
			spine->pushAfterCurrent(&TNerveCannonShoot::theNerve());
			return TRUE;
		}
	} else if (cannon->mStage == 5 || cannon->mStage == 9) {
		f32 killerDist = cannon->getSaveParams()->getSLKillerDist();
		if (dist < killerDist * killerDist
		    && spine->getTime()
		           > cannon->getSaveParams()->getSLShootInterval()) {
			cannon->mShootMode = 0;
			spine->pushAfterCurrent(&TNerveCannonSearch::theNerve());
			spine->pushAfterCurrent(&TNerveCannonForceBombShoot::theNerve());
			spine->pushAfterCurrent(&TNerveCannonShoot::theNerve());
			spine->pushAfterCurrent(&TNerveCannonShoot::theNerve());
			spine->pushAfterCurrent(&TNerveCannonShoot::theNerve());
			return TRUE;
		}
	}

	if (cannon->mPrevYaw != cannon->mRotation.y) {
		cannon->mPrevYaw = cannon->mRotation.y;
		if (gpMSound->gateCheck(MSD_SE_EN_CANNON_MOVE))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_EN_CANNON_MOVE, &cannon->mPosition, 0, nullptr, 0, 4);
	}

	if (gpApplication.mCurrArea.unk0 == 5
	    && gpMarDirector->mState == TMarDirector::STATE_UNK1) {
		cannon->mRotation.y
		    = CannonRotYToPoint(cannon->mPosition, *gpMarioPos);
	} else {
		cannon->turnToGoal();
	}
	return FALSE;
}

DEFINE_NERVE(TNerveCannonShoot, TLiveActor)
{
	TCannon* cannon = (TCannon*)spine->getBody();

	if (spine->getTime() == 0) {
		if (cannon->mShootMode == 0)
			cannon->setKillerGoalPoint();
		else
			cannon->mChorobei->setBckAnm(0x11);
	}

	if (cannon->mShootMode) {
		if (cannon->mChorobei->mParts->getMActor()->checkCurBckFromIndex(
		        0x11)) {
			if (cannon->mChorobei->mParts->getMActor()->curAnmEndsNext()) {
				cannon->mChorobei->setBckAnm(0x10);
				cannon->bombSet();
			}
			cannon->turnToGoal();
		} else if (cannon->mChorobei->mParts->getMActor()->checkCurBckFromIndex(
		               0x10)) {
			if (cannon->mChorobei->mParts->getMActor()->curAnmEndsNext()) {
				spine->pushAfterCurrent(&TNerveCannonSearch::theNerve());
				return TRUE;
			}
			if (cannon->mChorobei->mParts->getMActor()
			        ->getFrameCtrl(0)
			        ->checkPass(38.0f))
				cannon->bombShoot();
			if (cannon->mChorobei->mParts->getMActor()
			        ->getFrameCtrl(0)
			        ->getFrame()
			    > 26.0f)
				cannon->bombScaleUp();
		}
	} else {
		if (spine->getTime() < 40)
			cannon->turnToGoal();
		if (spine->getTime() == 40)
			cannon->killerShoot();
		if (spine->getTime()
		    > cannon->getSaveParams()->getSLKillerInterval()) {
			cannon->endKillerShoot();
			return TRUE;
		}
	}
	return FALSE;
}

DEFINE_NERVE(TNerveCannonForceBombShoot, TLiveActor)
{
	TCannon* cannon = (TCannon*)spine->getBody();

	if (spine->getTime() == 0) {
		f32 bombDist = cannon->getSaveParams()->getSLBombDist();
		if (cannon->mDistToMarioSquared < 2.0f * (bombDist * bombDist))
			cannon->mChorobei->setBckAnm(0x11);
		else
			return TRUE;
	}

	if (cannon->mChorobei->mParts->getMActor()->checkCurBckFromIndex(0x11)) {
		if (cannon->mChorobei->mParts->getMActor()->curAnmEndsNext()) {
			cannon->mChorobei->setBckAnm(0x10);
			cannon->bombSet();
		}
		cannon->turnToGoal();
	} else if (cannon->mChorobei->mParts->getMActor()->checkCurBckFromIndex(
	               0x10)) {
		if (cannon->mChorobei->mParts->getMActor()->curAnmEndsNext())
			return TRUE;
		if (cannon->mChorobei->mParts->getMActor()->getFrameCtrl(0)->checkPass(
		        38.0f))
			cannon->bombShoot();
		if (cannon->mChorobei->mParts->getMActor()->getFrameCtrl(0)->getFrame()
		    > 26.0f)
			cannon->bombScaleUp();
	}
	return FALSE;
}

// Binding levels worth +8 each of low region, landing TNerveCannonClose's
// frame at 0xb8 (the body binder plus the chorobei binder at two sites).
static inline TCannon* CannonCloseBody(TSpineBase<TLiveActor>* spine)
{
	TCannon* cannon = (TCannon*)spine->getBody();
	return cannon;
}

static inline TChorobei* CannonCloseChorobei(TCannon* p)
{
	TChorobei* chorobei = p->mChorobei;
	return chorobei;
}

DEFINE_NERVE(TNerveCannonClose, TLiveActor)
{
	TCannon* cannon = CannonCloseBody(spine);

	if (spine->getTime() < 2) {
		cannon->deadCannon();
		CannonCloseChorobei(cannon)->setBckAnm(0xF);
		cannon->mEffectPos = cannon->mPosition;
		cannon->mEffectPos.y += 300.0f;
		gpMarioParticleManager->emitAndBindToPosPtr(0xC9, &cannon->mEffectPos,
		                                            0, nullptr);
	}

	if (CannonCloseChorobei(cannon)->isDownEnd()) {
		if (!cannon->isBckAnm(0))
			cannon->setBckAnm(0);
	}

	if (cannon->isBckAnm(0)
	    && cannon->getMActor()->getFrameCtrl(0)->checkPass(10.0f)) {
		cannon->mEffectPos = cannon->mPosition;
		cannon->mEffectPos.y += 290.0f;
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    0x11, &cannon->mEffectPos, 0, nullptr);
		if (emitter) {
			JGeometry::TVec3<f32> scale(1.5f, 1.5f, 1.5f);
			emitter->setGlobalScale(scale);
		}
	}

	cannon->updateSquareToMario();
	f32 hideDist = cannon->getSaveParams()->getSLHideDist();
	hideDist *= 3.0f;
	hideDist *= hideDist;
	if (cannon->mDistToMarioSquared > hideDist) {
		cannon->offHitFlag(HIT_FLAG_NO_COLLISION);
		cannon->mChorobei->offHitFlag(HIT_FLAG_NO_COLLISION);
		spine->pushAfterCurrent(&TNerveCannonOpen::theNerve());
		return TRUE;
	}

	cannon->mFutaCollision->moveMtx(
	    cannon->getMActor()->getModel()->getAnmMtx(4));
	return FALSE;
}

DEFINE_NERVE(TNerveCannonDamage, TLiveActor)
{
	TCannon* cannon = (TCannon*)spine->getBody();

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> scale(2.0f, 2.0f, 2.0f);
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    0xC4, &cannon->mPosition, 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(scale);
		emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    0xC5, &cannon->mPosition, 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(scale);
		emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    0xC6, &cannon->mPosition, 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(scale);

		cannon->decHitPoints();
		if (cannon->mHitPoints == 0) {
			if (cannon->mHeldBomb)
				cannon->mHeldBomb->kill();
			cannon->mChorobei->setBckAnm(0xD);
			if (gpApplication.mCurrArea.unk0 == 5)
				SMSRumbleMgr->start(0x18, (f32*)nullptr);
			else
				SMSRumbleMgr->start(0x17, (f32*)nullptr);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    0xC8,
			    cannon->mChorobei->mParts->getMActor()->getModel()->getAnmMtx(
			        8),
			    0, nullptr);
			if (emitter)
				emitter->setGlobalScale(cannon->mChorobei->mScaling);
			emitter = gpMarioParticleManager->emitAndBindToPosPtr(
			    0xC7, &cannon->mEffectPos, 0, nullptr);
			if (emitter)
				emitter->setGlobalScale(cannon->mChorobei->mScaling);

			if (gpApplication.mCurrArea.unk0 == 5) {
				cannon->mDemoCamPos   = cannon->mPosition;
				cannon->mDemoCamPos.y = 0.0f;
				SMSGetMarDirector()->fireStartDemoCamera(
				    "tyorocam_pinna", &cannon->mDemoCamPos, -1,
				    cannon->mRotation.y, true, nullptr, 0, nullptr,
				    JDrama::TFlagT<u16>(0));
			} else {
				SMSGetMarDirector()->fireStartDemoCamera(
				    "tyorocam_mare", nullptr, -1, 0.0f, true, nullptr, 0,
				    nullptr, JDrama::TFlagT<u16>(0));
			}
			cannon->deadCannon();
		} else {
			cannon->damage();
		}

		cannon->mVelocity = JGeometry::TVec3<f32>(0.0f, 4.0f, 0.0f);
		cannon->onLiveFlag(LIVE_FLAG_AIRBORNE);
		cannon->mPosition.y += 10.0f;
	}

	if (cannon->mChorobei->mParts->getMActor()->curAnmEndsNext()) {
		SMS_ResetDamageFogEffect(
		    cannon->mChorobei->mParts->getMActor()->getModel()->getModelData());
		if (cannon->mHitPoints == 0) {
			spine->pushAfterCurrent(&TNerveCannonDamageDemo::theNerve());
			return TRUE;
		}
		spine->pushAfterCurrent(&TNerveCannonSearch::theNerve());
		return TRUE;
	}
	return FALSE;
}

static inline MSound* CannonObjectSound()
{
	MSound* sound = gpMSound;
	return sound;
}

static inline TConductor* CannonDamageDemoConductor()
{
	TConductor* conductor = gpConductor;
	return conductor;
}

static inline MActor* CannonDamageDemoMActor(TCannon* cannon)
{
	TChorobei* chorobei = cannon->mChorobei;
	MActor* actor = chorobei->mParts->getMActor();
	return actor;
}

static inline TCannon* CannonDamageDemoBody(TSpineBase<TLiveActor>* spine)
{
	TCannon* cannon = (TCannon*)spine->getBody();
	return cannon;
}

DEFINE_NERVE(TNerveCannonDamageDemo, TLiveActor)
{
	TCannon* cannon = CannonDamageDemoBody(spine);

	if (spine->getTime() == 0)
		cannon->mChorobei->setBckAnm(0xE);

	if (spine->getTime() > 120
	    && CannonDamageDemoMActor(cannon)->curAnmEndsNext()) {
		if (!cannon->isBckAnm(4)) {
			cannon->setBckAnm(4);
			if (gpMSound->gateCheck(MSD_SE_EN_CANNON_DOWN))
				MSoundSESystem::MSoundSE::startSoundActor(
				    MSD_SE_EN_CANNON_DOWN, &cannon->mPosition, 0, nullptr, 0,
				    4);
			TEffectExplosion* explosion
			    = (TEffectExplosion*)CannonDamageDemoConductor()->makeOneEnemyAppear(
			        cannon->mPosition, "エフェクト爆発マネージャー", 1);
			if (explosion) {
				JGeometry::TVec3<f32> scale(2.5f, 2.5f, 2.5f);
				explosion->generate(cannon->mPosition, scale);
			}
			TFlagManager::smInstance->setBool(true, 0x5000C);
			spine->pushAfterCurrent(&TNerveCannonObject::theNerve());
			return TRUE;
		}
	}
	return FALSE;
}

static inline J3DFrameCtrl* CannonObjectFrameCtrl(TCannon* cannon)
{
	J3DFrameCtrl* ctrl = cannon->getMActor()->getFrameCtrl(0);
	return ctrl;
}

DEFINE_NERVE(TNerveCannonObject, TLiveActor)
{
	TCannon* cannon = (TCannon*)spine->getBody();

	if (spine->getTime() == 0) {
		if (!cannon->isBckAnm(4)) {
			cannon->setBckAnm(4);
			J3DFrameCtrl* ctrl = CannonObjectFrameCtrl(cannon);
			ctrl->setFrame(ctrl->getEnd());
		}
		if (cannon->mChorobei)
			cannon->mChorobei->unk70 = 1.0f;
	}

	if (cannon->isBckAnm(4)
	    && cannon->getMActor()->getFrameCtrl(0)->checkPass(60.0f)
	    && CannonObjectSound()->gateCheck(MSD_SE_EN_CANNON_LAND))
		MSoundSESystem::MSoundSE::startSoundActor(
		    MSD_SE_EN_CANNON_LAND, &cannon->mPosition, 0, nullptr, 0, 4);

	if (spine->getTime() > 150)
		cannon->gateOpen();

	return FALSE;
}
