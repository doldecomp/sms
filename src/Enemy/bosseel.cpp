
#include <Enemy/BossEel.hpp>
#include <Enemy/Conductor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/SharedParts.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/Application.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

TBEelTearsDrop::TBEelTearsDrop(TBEelTears* owner, int jointIndex,
                               SDLModelData* modelData, const char* name)
    : THitActor(name)
    , mSharedParts(nullptr)
    , mOwner(owner)
{
	mSharedParts
	    = new TSharedParts(owner, jointIndex, modelData, 0, "<TSharedParts>");
	initHitActor(0x2000002C, 3, ACTOR_TYPE_PLAYER,
	             owner->mTearsParams->mSLTearsDropAttackRadius.get(),
	             owner->mTearsParams->mSLTearsDropAttackHeight.get(),
	             owner->mTearsParams->mSLTearsDropDamageRadius.get(),
	             owner->mTearsParams->mSLTearsDropDamageHeight.get());
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
	mActive = false;

	TScreenTexture* screenTexture
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
	MActor* actor = mSharedParts->getMActor();
	SMS_ChangeTextureAll(actor->getModel()->getModelData(), "M_dummy",
	                     *screenTexture->getTexture()->getTexInfo());
	actor->setBckFromIndex(0);
	actor->setLightType(3);
}

void TBEelTearsDrop::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);
	if (cue & CUE_MOVE) {
		mPosition.y += mRiseSpeed;
		if (mPosition.y > gpMarioPos->y + 2000.0f)
			mActive = false;
	}
	if (cue & CUE_CALC_ANIM) {
		MsMtxSetXYZRPH(
		    mSharedParts->getMActor()->getModel()->getBaseTRMtx(), mPosition.x,
		    mPosition.y, mPosition.z, (s16)(mRotation.x * 182.04445f),
		    (s16)(mRotation.y * 182.04445f), (s16)(mRotation.z * 182.04445f));
		f32 scale = mOwner->mTearsParams->mTearsDropScaleLow;
		mScaling.set(scale, scale, scale);
		mSharedParts->getMActor()->getModel()->setBaseScale(mScaling);
	}
	mSharedParts->getMActor()->perform(cue, graphics);
}

// UNUSED: retail mario.MAP size 0x12C. TODO: human-review the original spawn
// sequencing; no linked assembly survives for this function.
void TBEelTearsDrop::generate(JGeometry::TVec3<f32>& position)
{
	mPosition = position;
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mActive    = true;
	mRiseSpeed = mOwner->mTearsParams->mSLTearsUpSpeed.get();

	f32 scale = MsRandF(mOwner->mTearsParams->mTearsDropScaleLow,
	                    mOwner->mTearsParams->mTearsDropScaleHigh);
	mScaling.set(scale, scale, scale);
	mSharedParts->getMActor()->setBckFromIndex(0);
	mSharedParts->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
}

TBEelTearsSaveLoadParams::TBEelTearsSaveLoadParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLTearsUpSpeed, 5.0f)
    , PARAM_INIT(mSLTearsDamageUpSpeed, 1.0f)
    , PARAM_INIT(mSLTearsLiveHeight, 2000.0f)
    , PARAM_INIT(mSLTearsSplitNum, 2)
    , PARAM_INIT(mSLTearsDamageRadius, 600)
    , PARAM_INIT(mSLTearsDamageHeight, 400)
    , PARAM_INIT(mSLTearsAttackRadius, 500)
    , PARAM_INIT(mSLTearsAttackHeight, 300)
    , PARAM_INIT(mSLTearsDropDamageRadius, 600)
    , PARAM_INIT(mSLTearsDropDamageHeight, 400)
    , PARAM_INIT(mSLTearsDropAttackRadius, 500)
    , PARAM_INIT(mSLTearsDropAttackHeight, 300)
    , PARAM_INIT(mSLHighPolyDistY, 3000.0f)
    , PARAM_INIT(mSLHitAnmFrameRate, 1.0f)
    , PARAM_INIT(mSLBodyScaleLow, 1.0f)
    , PARAM_INIT(mSLBodyScaleHigh, 1.0f)
    , PARAM_INIT(mSLTearsDropScaleLow, 1.0f)
    , PARAM_INIT(mSLTearsDropScaleHigh, 1.0f)
    , mBodyScaleLow(0.0f)
    , mBodyScaleHigh(1.0f)
    , mTearsDropScaleLow(0.0f)
    , mTearsDropScaleHigh(1.0f)
{
	TParams::load(mPrmPath);
	mBodyScaleLow       = mSLBodyScaleLow.get();
	mBodyScaleHigh      = mSLBodyScaleHigh.get();
	mTearsDropScaleLow  = mSLTearsDropScaleLow.get();
	mTearsDropScaleHigh = mSLTearsDropScaleHigh.get();
}

TBEelTearsManager::TBEelTearsManager(const char* name)
    : TEnemyManager(name)
{
}

void TBEelTearsManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBEelTearsSaveLoadParams("/enemy/bossEelTears.prm");
	TEnemyManager::load(stream);
}

void TBEelTearsManager::loadAfter()
{
	SMS_LoadParticle("/scene/bossEelTears/ms_meo_tear_bomb.jpa", 0xD5);
	SMS_LoadParticle("/scene/bossEelTears/ms_meo_tear_awaget.jpa", 0xD6);
	SMS_LoadParticle("/scene/bossEelTears/ms_meo_tear_awa.jpa", 0x19D);
}

void TBEelTearsManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "tears.bmd", 0x11240000, 0 },
		{ "tears_waterhit.bmd", 0x11240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TBEelTearsManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM)
		clipEnemies(graphics);

	for (int i = 0; i < getActiveObjNum(); ++i)
		getObj(i)->perform(cue, graphics);

	for (int i = 0; i < 30; ++i) {
		if (mTearsDrops[i]->mActive)
			mTearsDrops[i]->perform(cue, graphics);
	}
}

TSpineEnemy* TBEelTearsManager::createEnemyInstance()
{
	return new TBEelTears("めおとウナギ涙");
}

void TBEelTearsManager::createEnemies(int count)
{
	TEnemyManager::createEnemies(count);

	void* resource
	    = JKRFileLoader::getGlbResource("/scene/bossEelTears/tears_drop.bmd");
	SDLModelData* modelData
	    = new SDLModelData(J3DModelLoaderDataBase::load(resource, 0x11240000));
	TBEelTears* owner = static_cast<TBEelTears*>(getObj(0));
	for (int i = 0; i < 30; ++i)
		mTearsDrops[i] = new TBEelTearsDrop(owner, 0, modelData, "涙粒");
}

void TBEelTearsManager::splitTears(JGeometry::TVec3<f32>& position)
{
	position.y += 600.0f;
	int tearsLeft = getSaveParam()->mSLTearsSplitNum.get();

	for (int i = 0; i < 30; ++i) {
		TBEelTearsDrop* drop = mTearsDrops[i];
		if (drop->mActive)
			continue;

		JGeometry::TVec3<f32> dropPosition = position;
		dropPosition.x += MsRandF(-250.0f, 250.0f);
		dropPosition.y += MsRandF(-250.0f, 250.0f);
		dropPosition.z += MsRandF(-250.0f, 250.0f);

		--tearsLeft;
		drop->offHitFlag(HIT_FLAG_NO_COLLISION);
		drop->mActive    = true;
		drop->mRiseSpeed = MsRandF(4.0f, 6.0f);
		drop->mPosition  = dropPosition;

		rand();
		f32 scale = MsRandF(drop->mOwner->mTearsParams->mTearsDropScaleLow,
		                    drop->mOwner->mTearsParams->mTearsDropScaleHigh);
		drop->mScaling.set(scale, scale, scale);

		if (tearsLeft < 0)
			break;
	}
}

TBEelTears::TBEelTears(const char* name)
    : TSpineEnemy(name)
    , mTearsParams(nullptr)
    , mHighPoly(true)
    , mStateTimer(0)
    , mWaterSurface(nullptr)
    , mRecoverCollision(nullptr)
{
}

void TBEelTears::init(TLiveManager* manager)
{
	TSpineEnemy::init(manager);
	mActorType = 0x08000003;
	onHitFlag(HIT_FLAG_UNK8000000);
	setMActorAndKeeper();
	mTearsParams = static_cast<TBEelTearsSaveLoadParams*>(getSaveParam());
	mSpine->initWith(&TNerveBEelTearsGenerate::theNerve());

	TScreenTexture* screenTexture
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
	const ResTIMG& screenTexInfo = *screenTexture->getTexture()->getTexInfo();

	MActor* tearsActor = mMActorKeeper->getMActor("tears.bmd");
	tearsActor->getModel()->setSkinDeform(new J3DSkinDeform,
	                                      J3D_DEFORM_ATTACH_FLAG_UNK_1);
	tearsActor->resetDL();
	SMS_ChangeTextureAll(tearsActor->getModel()->getModelData(), "M_dummy",
	                     screenTexInfo);
	tearsActor->setLightType(3);

	MActor* waterHitActor = mMActorKeeper->getMActor("tears_waterhit.bmd");
	SMS_ChangeTextureAll(waterHitActor->getModel()->getModelData(), "M_dummy",
	                     screenTexInfo);
	waterHitActor->setLightType(3);

	onLiveFlag(LIVE_FLAG_DEAD);
	mBodyScale
	    = MsRandF(mTearsParams->mBodyScaleLow, mTearsParams->mBodyScaleHigh);

	mRecoverCollision = new TBossEelTearsRecoverCollision(
	    mMActor->getModel()->getAnmMtx(0), "回復コリジョン");
	mRecoverCollision->initCollision();
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(mRecoverCollision);
	mRecoverCollision->onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBEelTears::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("tears.bmd", 0);
	mMActorKeeper->createMActor("tears_waterhit.bmd", 0);
}

void TBEelTears::moveObject()
{
	mVelocity.x *= 0.9f;
	mVelocity.z *= 0.9f;
	mPosition.x += mVelocity.x;
	mPosition.z += mVelocity.z;

	if (!mRecoverCollision->mColliding && mWaterSurface != nullptr
	    && (mPosition.y > gpMarioPos->y + 3000.0f
	        || mPosition.y > mWaterSurface->getPoint2().x
	                             + mTearsParams->mSLTearsLiveHeight.get())) {
		kill();
		return;
	}

	setHitParams(mTearsParams->mSLTearsAttackRadius.get() * mScaling.x,
	             mTearsParams->mSLTearsAttackHeight.get() * mScaling.x,
	             mTearsParams->mSLTearsDamageRadius.get() * mScaling.x,
	             mTearsParams->mSLTearsDamageHeight.get() * mScaling.x);

	for (int i = 0; i < getColNum(); ++i) {
		THitActor* actor = getCollision(i);
		if (actor->isActorType(0x80000001)) {
			if (mSpine->getCurrentNerve()
			    == &TNerveBEelTearsMoveUp::theNerve()) {
				SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
				mSpine->setNext(&TNerveBEelTearsSplit::theNerve());
			}
		} else {
			JGeometry::TVec3<f32> push = mPosition;
			push.sub(actor->mPosition);
			if (push.isZero())
				push.x += 1.0f;
			MsVECNormalize(&push, &push);
			push.scale(5.0f);
			mLinearVelocity = push;
		}
	}

	TLiveActor::moveObject();
}

void TBEelTears::calcRootMatrix()
{
	if (mWaterSurface == nullptr) {
		TSpineEnemy::calcRootMatrix();
		return;
	}

	if (mPosition.y < mWaterSurface->getPoint2().x)
		mPosition.y = mWaterSurface->getPoint2().x;
	TSpineEnemy::calcRootMatrix();

	if (mSpine->getCurrentNerve() != &TNerveBEelTearsGenerate::theNerve())
		return;

	TPosition3f transform;
	transform.setTrans(mPosition);
	transform.ref(0, 3) += (mWaterSurface->mMaxY - transform.at(0, 3)) * 0.1f;
	transform.ref(2, 3)
	    += (mWaterSurface->getPoint3().y - transform.at(2, 3)) * 0.1f;

	f32 scale = mTearsParams->mSLBodyScaleLow.get();
	mScaling.set(scale, scale, scale);
	mMActor->getModel()->setBaseScale(mScaling);
	mMActor->getModel()->setBaseTRMtx(transform);
}

void TBEelTears::perform(u32 cue, JDrama::TGraphics* graphics)
{
	mRecoverCollision->perform(cue, graphics);
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if (cue & CUE_MOVE)
		control();

	if (cue & CUE_CALC_ANIM) {
		Mtx effectMtx;
		SMS_GetLightPerspectiveForEffectMtx(effectMtx);
		mMActor->getModel()
		    ->getModelData()
		    ->getMaterialNodePointer(0)
		    ->getTexGenBlock()
		    ->getTexMtx(1)
		    ->setEffectMtx(effectMtx);
		requestShadow();
		mMActor->frameUpdate();
	}

	if (checkLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT))
		return;

	if (cue & CUE_CALC_ANIM) {
		calcRootMatrix();
		mMActor->calc();
	}
	if (cue & CUE_CALC_VIEW)
		mMActor->viewCalc();
	if (cue & CUE_ENTRY)
		mMActor->entry();
	THitActor::perform(cue, graphics);
}

BOOL TBEelTears::receiveMessage(THitActor*, u32 message)
{
	if (message != HIT_MESSAGE_SPRAYED_BY_WATER)
		return false;

	mStateTimer = 60;
	if (mSpine->getCurrentNerve() == &TNerveBEelTearsMoveUp::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveOilBallStay::theNerve())
		mSpine->setNext(&TNerveBEelTearsWaterHit::theNerve());

	if (mSpine->getCurrentNerve() == &TNerveBEelTearsWaterHit::theNerve())
		mMActor->setFrameRate(SMSGetAnmFrameRate(), 0);
	return true;
}

void TBEelTears::kill()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mRecoverCollision->onHitFlag(HIT_FLAG_NO_COLLISION);
	onLiveFlag(LIVE_FLAG_DEAD);
}

void TBEelTears::reset()
{
	mActorType = 0x08000003;
	TSpineEnemy::reset();
	mSpine->initWith(&TNerveBEelTearsGenerate::theNerve());
	onLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT
	            | LIVE_FLAG_UNK100 | LIVE_FLAG_UNK200 | LIVE_FLAG_UNK400
	            | LIVE_FLAG_UNK8);
	mGroundPlane = gpMap->getIllegalCheckData();
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

// UNUSED: retail mario.MAP size 0x108. TODO: human-review the actor swap and
// particle setup; no linked assembly survives for this function.
void TBEelTears::setBubble()
{
	mMActor = mMActorKeeper->getMActor("tears_waterhit.bmd");
	mMActor->setBckFromIndex(0);
	mMActor->setFrameRate(SMSGetAnmFrameRate(), 0);
	mRecoverCollision->mRecovering = false;
	mRecoverCollision->mColliding  = false;
}

// UNUSED: retail mario.MAP size 0x48. TODO: human-review the recovery-state
// transition; no linked assembly survives for this function.
void TBEelTears::setRecoverTears()
{
	mRecoverCollision->mRecovering = true;
	mRecoverCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
	mSpine->setNext(&TNerveBEelTearsMarioRecover::theNerve());
}

// UNUSED: retail mario.MAP size 0x12C. TODO: human-review the original effect
// and sound calls; no linked assembly survives for this function.
void TBEelTears::deadEffect()
{
	static_cast<TBEelTearsManager*>(mManager)->splitTears(mPosition);
	mRecoverCollision->onHitFlag(HIT_FLAG_NO_COLLISION);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	onLiveFlag(LIVE_FLAG_DEAD);
}

const char** TBEelTears::getBasNameTable() const
{
	static const char* bossEelTears_bastable[]
	    = { nullptr, nullptr, nullptr, nullptr };
	return bossEelTears_bastable;
}

DEFINE_NERVE(TNerveBEelTearsGenerate, TLiveActor)
{
	TBEelTears* tears = static_cast<TBEelTears*>(spine->getBody());
	if (spine->getTime() == 0) {
		tears->mMActor = tears->mMActorKeeper->getMActor("tears.bmd");
		tears->mMActor->setBckFromIndex(2);
	}

	if (tears->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveBEelTearsMoveUp::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBEelTearsMoveUp, TLiveActor)
{
	TBEelTears* tears = static_cast<TBEelTears*>(spine->getBody());
	if (spine->getTime() == 0) {
		tears->mMActor = tears->mMActorKeeper->getMActor("tears.bmd");
		tears->mMActor->setBckFromIndex(1);
	}
	tears->mPosition.y += tears->mTearsParams->mSLTearsUpSpeed.get();
	return false;
}

DEFINE_NERVE(TNerveBEelTearsWaterHit, TLiveActor)
{
	TBEelTears* tears = static_cast<TBEelTears*>(spine->getBody());
	if (spine->getTime() == 0) {
		SMSGetMSound()->startSoundActor(0x8926, &tears->mPosition, 0, nullptr,
		                                0, 4);
		tears->mMActor = tears->mMActorKeeper->getMActor("tears_waterhit.bmd");
		tears->mMActor->setBckFromIndex(3);
	}

	--tears->mStateTimer;
	f32 frameRate = tears->mTearsParams->mSLHitAnmFrameRate.get();
	if (tears->mStateTimer < 0) {
		tears->mMActor->setFrameRate(-frameRate * SMSGetAnmFrameRate(), 0);
		if (tears->getCurAnmFrameNo(0) < 1.0f)
			return true;
	} else {
		tears->mMActor->setFrameRate(frameRate * SMSGetAnmFrameRate(), 0);
	}

	if (tears->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveBEelTearsMarioRecover::theNerve());
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    0xD5, &tears->mPosition, 0, nullptr);
		if (emitter)
			emitter->setScale(tears->mScaling);

		tears->mRecoverCollision->mColliding  = false;
		tears->mRecoverCollision->mRecovering = true;
		tears->mRecoverCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
		tears->mRecoverCollision->mPosition = tears->mPosition;
		static_cast<TBEelTearsManager*>(tears->mManager)
		    ->splitTears(tears->mPosition);
		SMSGetMSound()->startSoundActor(0x8927, &tears->mPosition, 0, nullptr,
		                                0, 4);
		tears->onLiveFlag(LIVE_FLAG_HIDDEN);
		return true;
	}

	if (tears->mHighPoly)
		tears->mPosition.y += tears->mTearsParams->mSLTearsDamageUpSpeed.get();
	return false;
}

DEFINE_NERVE(TNerveBEelTearsMarioRecover, TLiveActor)
{
	TBEelTears* tears = static_cast<TBEelTears*>(spine->getBody());
	if (!tears->mRecoverCollision->mRecovering) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    0xD6, gpMarioPos, 0, nullptr);
		if (emitter)
			emitter->setScale(tears->mScaling);
		tears->kill();
		return true;
	}

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    0x19D, &tears->mPosition, 1, tears);
	if (emitter)
		emitter->setScale(tears->mScaling);

	if (spine->getTime() > 1000) {
		tears->kill();
		return true;
	}
	if (tears->mRecoverCollision->mColliding) {
		tears->mPosition.y += tears->mTearsParams->mSLTearsUpSpeed.get();
		tears->mRecoverCollision->mPosition.y = tears->mPosition.y;
	}
	return false;
}

DEFINE_NERVE(TNerveBEelTearsSplit, TLiveActor)
{
	TBEelTears* tears = static_cast<TBEelTears*>(spine->getBody());
	if (spine->getTime() == 0) {
		SMSGetMSound()->startSoundActor(0x8926, &tears->mPosition, 0, nullptr,
		                                0, 4);
		tears->mMActor = tears->mMActorKeeper->getMActor("tears_waterhit.bmd");
		tears->mMActor->setBckFromIndex(3);
		tears->mMActor->setFrameRate(
		    tears->mTearsParams->mSLHitAnmFrameRate.get()
		        * SMSGetAnmFrameRate(),
		    0);
	}

	if (!tears->checkCurAnmEnd(0))
		return false;

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    0xD5, &tears->mPosition, 0, nullptr);
	if (emitter)
		emitter->setScale(tears->mScaling);

	tears->mRecoverCollision->mColliding  = false;
	tears->mRecoverCollision->mRecovering = true;
	tears->mRecoverCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
	tears->mRecoverCollision->mPosition = tears->mPosition;
	static_cast<TBEelTearsManager*>(tears->mManager)
	    ->splitTears(tears->mPosition);
	SMSGetMSound()->startSoundActor(0x8927, &tears->mPosition, 0, nullptr, 0,
	                                4);
	tears->onLiveFlag(LIVE_FLAG_HIDDEN);
	spine->pushAfterCurrent(&TNerveBEelTearsMarioRecover::theNerve());
	return true;
}

void TOilBall::load(JSUMemoryInputStream& stream)
{
	mHighPoly = false;
	TSpineEnemy::load(stream);
	mInitialPosition = mPosition;
	reset();
}

void TOilBall::reset()
{
	mActorType = 0x08000003;
	TSpineEnemy::reset();
	mSpine->initWith(&TNerveBEelTearsGenerate::theNerve());
	onLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT
	            | LIVE_FLAG_UNK100 | LIVE_FLAG_UNK200 | LIVE_FLAG_UNK400
	            | LIVE_FLAG_UNK8);
	mGroundPlane = gpMap->getIllegalCheckData();
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mPosition = mInitialPosition;
	mSpine->initWith(&TNerveOilBallStay::theNerve());
	mMActor = mMActorKeeper->getMActor("tears.bmd");
	mMActor->setBckFromIndex(2);
	mHighPoly = false;
}

void TOilBall::moveObject()
{
	setHitParams(mTearsParams->mSLTearsAttackRadius.get() * mScaling.x,
	             mTearsParams->mSLTearsAttackHeight.get() * mScaling.x,
	             mTearsParams->mSLTearsDamageRadius.get() * mScaling.x,
	             mTearsParams->mSLTearsDamageHeight.get() * mScaling.x);

	for (int i = 0; i < getColNum(); ++i) {
		THitActor* actor = getCollision(i);
		if (actor->isActorType(0x80000001)) {
			if (mSpine->getCurrentNerve() == &TNerveBEelTearsMoveUp::theNerve()
			    || mSpine->getCurrentNerve()
			           == &TNerveOilBallStay::theNerve()) {
				SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
				mSpine->setNext(&TNerveBEelTearsSplit::theNerve());
			}
		} else {
			JGeometry::TVec3<f32> push = mPosition;
			push.sub(actor->mPosition);
			if (push.isZero())
				push.x += 1.0f;
			MsVECNormalize(&push, &push);
			push.scale(5.0f);
			mLinearVelocity = push;
		}
	}
	TLiveActor::moveObject();
}

void TOilBall::calcRootMatrix() { TSpineEnemy::calcRootMatrix(); }

DEFINE_NERVE(TNerveOilBallStay, TLiveActor)
{
	TOilBall* oilBall = static_cast<TOilBall*>(spine->getBody());
	if (oilBall->mMActor->checkCurBckFromIndex(3)
	    || (oilBall->checkCurAnmEnd(0)
	        && !oilBall->mMActor->checkCurBckFromIndex(1))) {
		oilBall->mMActor = oilBall->mMActorKeeper->getMActor("tears.bmd");
		oilBall->mMActor->setBckFromIndex(1);
	}
	return false;
}

TBossEelSaveParams::TBossEelSaveParams()
    : TParams("/enemy/bosseel.prm")
    , PARAM_INIT(mSLInitTransYOffset, 0.0f)
    , PARAM_INIT(mSLAppearMoveDistY, 6000.0f)
    , PARAM_INIT(mSLBodyScale, 0.75f)
    , PARAM_INIT(mSLViewClipFar, 25000.0f)
    , PARAM_INIT(mSLViewClipRadius, 12000.0f)
    , PARAM_INIT(mSLBodyToHeadDistance, 6000.0f)
    , PARAM_INIT(mSLBodyAttackRadius, 2400.0f)
    , PARAM_INIT(mSLBodyAttackHeight, 6300.0f)
    , PARAM_INIT(mSLBodyDamageRadius, 2200.0f)
    , PARAM_INIT(mSLBodyDamageHeight, 6100.0f)
    , PARAM_INIT(mSLHeadAttackRadius, 3600.0f)
    , PARAM_INIT(mSLHeadAttackHeight, 5600.0f)
    , PARAM_INIT(mSLHeadDamageRadius, 3400.0f)
    , PARAM_INIT(mSLHeadDamageHeight, 5400.0f)
    , PARAM_INIT(mSLToothAttackRadius, 500.0f)
    , PARAM_INIT(mSLToothAttackHeight, 300.0f)
    , PARAM_INIT(mSLToothDamageRadius, 600.0f)
    , PARAM_INIT(mSLToothDamageHeight, 400.0f)
    , PARAM_INIT(mSLSpinAccel, 0.01f)
    , PARAM_INIT(mSLSpinMaxSpeed, 10.0f)
    , PARAM_INIT(mSLToothUpSpeed, 5.0f)
    , PARAM_INIT(mSLToothLiveHeight, 2000.0f)
    , PARAM_INIT(mSLToothMaxHitPoint, 50)
    , PARAM_INIT(mSLGenTearsTime, 100)
    , PARAM_INIT(mSLVortexAttackRadius, 500.0f)
    , PARAM_INIT(mSLVortexAttackHeight, 300.0f)
    , PARAM_INIT(mSLVortexDamageRadius, 600.0f)
    , PARAM_INIT(mSLVortexDamageHeight, 400.0f)
    , PARAM_INIT(mSLVortexLiveTimer, 300)
    , PARAM_INIT(mSLVortexScaleXZ, 1.0f)
    , PARAM_INIT(mSLVortexScaleY, 1.0f)
    , PARAM_INIT(mSLMouthOpenFrame, 500)
    , PARAM_INIT(mSLMouthOpenInterval, 2000)
    , PARAM_INIT(mSLCanEatFrame, 200)
    , PARAM_INIT(mSLBreathInPower, 10.0f)
{
	TParams::load(mPrmPath);
}

void TBossEelManager::loadAfter()
{
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_awa_tooth.jpa", 0xD3);
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_awa_mouth.jpa", 0xD4);
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_eyeblur.jpa", 0x192);
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_spin_smoke.jpa", 0x193);
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_spin_smoke_l.jpa", 0x194);
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_spin_awa.jpa", 0x195);
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_spin_awa_l.jpa", 0x196);
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_awa_body.jpa", 0x197);
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_awa_dead.jpa", 0x198);
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_suikomi.jpa", 0x199);
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_tooth_wash.jpa", 0x19A);
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_tooth_kira.jpa", 0x19B);
	SMS_LoadParticle("/scene/bosseel/jpa/ms_meo_tooth_always.jpa", 0x19C);
}

void TBossEelManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "meoto_model.bmd",
		  J3DMLF_MaterialPEFull | (2 << J3DMLF_TevStageNumShift), 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TBossEelManager::clipEnemies(JDrama::TGraphics* graphics)
{
	clipActorsAux(graphics, mSaveParams.mSLViewClipRadius.get(),
	              mSaveParams.mSLViewClipFar.get());
}

TBossEelTooth::TBossEelTooth(u8 toothType, TBossEel* owner,
                             const char* jointName, SDLModelData* modelData,
                             const char* name)
    : THitActor(name)
    , mSharedParts(nullptr)
    , mOwner(owner)
    , mHitPoints(0)
    , mToothType(toothType)
    , mTrembleRotation(0.0f, 0.0f, 0.0f)
    , mDamageCooldown(0)
    , mCanShedTears(true)
{
	s32 jointIndex
	    = owner->getModel()->getModelData()->getJointName()->getIndex(
	        jointName);
	mSharedParts
	    = new TSharedParts(owner, jointIndex, modelData, 0, "<TSharedParts>");

	MActor* actor = mSharedParts->getMActor();
	actor->setLightType(1);
	actor->setBckFromIndex(22);
	actor->setFrameRate(0.0f, 0);

	J3DModel* model = actor->getModel();
	for (u16 i = 0; i < model->getModelData()->getMaterialNum(); ++i)
		SMS_InitPacket_OneTevKColorAndFog(model, i, GX_KCOLOR0, &mColor);

	mColor.a   = 0xFF;
	mHitPoints = owner->getBossEelParams().mSLToothMaxHitPoint.get();
	initHitActor(0x08000022, 5, 0x81000000,
	             owner->getBossEelParams().mSLToothAttackRadius.get(),
	             owner->getBossEelParams().mSLToothAttackHeight.get(),
	             owner->getBossEelParams().mSLToothDamageRadius.get(),
	             owner->getBossEelParams().mSLToothDamageHeight.get());

	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

// UNUSED: retail mario.MAP size 0x8.
void TBossEelTooth::changeToothAlpha(u8 alpha) { mColor.a = alpha; }

BOOL TBossEelTooth::receiveMessage(THitActor* sender, u32 message)
{
	if (message != HIT_MESSAGE_SPRAYED_BY_WATER)
		return false;

	if (mDamageCooldown != 0 || mOwner->mToothBroken || mHitPoints <= 1)
		return true;

	mDamageCooldown = 2;
	--mHitPoints;
	mSharedParts->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
	mOwner->mToothDamaged = true;
	mColor.a              = static_cast<u8>(
        mHitPoints * 0xFF
        / mOwner->getBossEelParams().mSLToothMaxHitPoint.get());

	if (mToothType == 1 && mHitPoints % 20 == 1)
		mOwner->forceShedTears(mCanShedTears);

	if (mHitPoints != 1)
		return true;

	mColor.a = 0;
	MTXCopy(mSharedParts->getConnectedMtx(), mDetachedMtx);
	if (mToothType == 1) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_UNG_TEATH_COMEOFF, &mPosition,
		                                0, nullptr, 0, 4);
		SMSGetMSound()->startSoundActor(mCanShedTears
		                                    ? MSD_SE_BS_UNG_VOICE_M_CRY
		                                    : MSD_SE_BS_UNG_VOICE_W_CRY,
		                                &mOwner->mPosition, 0, nullptr, 0, 4);
		mOwner->mToothBroken = true;

		JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0xD3, &mPosition, 0, nullptr);
		if (emitter)
			emitter->setScale(mOwner->mScaling);
	} else {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_UNG_TEATH_FLASH, &mPosition,
		                                0, nullptr, 0, 4);
		SMSGetMSound()->startSoundActor(mCanShedTears
		                                    ? MSD_SE_BS_UNG_VOICE_M_JOY
		                                    : MSD_SE_BS_UNG_VOICE_W_JOY,
		                                &mOwner->mPosition, 0, nullptr, 0, 4);
	}
	return true;
}

// UNUSED: retail mario.MAP size 0x4. The retail body is empty.
void TBossEelTooth::updateTremble() { }

void TBossEelTooth::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mOwner->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT) || mHitPoints == 0)
		return;

	MActor* actor = mSharedParts->getMActor();
	if (cue & CUE_MOVE) {
		f32 scale = mOwner->mScaling.x;
		mAttackRadius
		    = mOwner->getBossEelParams().mSLToothAttackRadius.get() * scale;
		mAttackHeight
		    = mOwner->getBossEelParams().mSLToothAttackHeight.get() * scale;
		mDamageRadius
		    = mOwner->getBossEelParams().mSLToothDamageRadius.get() * scale;
		mDamageHeight
		    = mOwner->getBossEelParams().mSLToothDamageHeight.get() * scale;
		calcEntryRadius();

		for (s32 i = 0; i < mColCount; ++i) {
			if (mOwner->mSpine->getCurrentNerve()
			        != &TNerveBossEelEat::theNerve()
			    && mCollisions[i]->isActorType(0x80000001) && mHitPoints > 1) {
				SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
			}
		}

		if (mDamageCooldown > 0) {
			--mDamageCooldown;
		} else {
			if (actor->checkCurBckFromIndex(22)
			    && actor->curAnmEndsNext(0, nullptr))
				actor->setFrameRate(0.0f, 0);

			if (mHitPoints == 1 && mToothType == 1) {
				if (actor->checkCurBckFromIndex(22)) {
					actor->setBckFromIndex(20);
					JGeometry::TVec3<f32> tearsPosition(
					    mDetachedMtx[0][3],
					    mTrembleRotation.y + mDetachedMtx[1][3],
					    mDetachedMtx[2][3]);
					TBEelTears* tears = static_cast<TBEelTears*>(
					    gpConductor->makeOneEnemyAppear(
					        tearsPosition, "めおとウナギ涙マネージャー", 0));
					if (tears) {
						tears->mRecoverCollision->mColliding = false;
						tears->mRecoverCollision->offHitFlag(
						    HIT_FLAG_NO_COLLISION);
						tears->mRecoverCollision->mRecovering = true;
						tears->mRecoverCollision->mPosition = tears->mPosition;
						tears->mSpine->initWith(
						    &TNerveBEelTearsMarioRecover::theNerve());
						tears->onLiveFlag(LIVE_FLAG_HIDDEN);
						tears->mRecoverCollision->mColliding = true;
					}
					actor->setFrameRate(SMSGetAnmFrameRate(), 0);
				}
				if (actor->checkCurBckFromIndex(20)
				    && actor->curAnmEndsNext(0, nullptr))
					actor->setBckFromIndex(21);
			}

			if (mHitPoints == 1 && mToothType == 1) {
				mTrembleRotation.y
				    += mOwner->getBossEelParams().mSLToothUpSpeed.get();
				if (mTrembleRotation.y
				        > mOwner->getBossEelParams().mSLToothLiveHeight.get()
				    || mPosition.y > gpMarioPos->y + 2000.0f) {
					mHitPoints = 0;
					onHitFlag(HIT_FLAG_NO_COLLISION);
				}
			}
		}
	}

	if (cue & CUE_CALC_ANIM) {
		Mtx toothMtx;
		if (mHitPoints > 1 || mToothType == 0 || mToothType == 2)
			MTXCopy(mSharedParts->getConnectedMtx(), toothMtx);
		else
			MTXCopy(mDetachedMtx, toothMtx);

		if (mHitPoints > 1) {
			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToPosPtr(0x19C, &mPosition,
			                                                  1, this);
			if (emitter)
				emitter->setScale(mOwner->mScaling);
		}
		if (actor->checkCurBckFromIndex(22)
		    && actor->getFrameCtrl(0)->getRate() > 0.0f) {
			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToPosPtr(0x19A, &mPosition,
			                                                  1, this);
			if (emitter)
				emitter->setScale(mOwner->mScaling);
		}
		if ((mToothType == 0 || mToothType == 2) && mHitPoints == 1) {
			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToPosPtr(0x19B, &mPosition,
			                                                  1, this);
			if (emitter)
				emitter->setScale(mOwner->mScaling);
		}

		Mtx transform;
		MTXIdentity(transform);
		transform[2][3] = mTrembleRotation.x;
		MTXConcat(toothMtx, transform, toothMtx);
		MsMtxSetRotRPH(transform, mTrembleRotation.z, mTrembleRotation.z, 0.0f);
		MTXConcat(toothMtx, transform, toothMtx);
		toothMtx[1][3] += mTrembleRotation.y;
		mPosition.set(toothMtx[0][3], toothMtx[1][3], toothMtx[2][3]);
		MTXCopy(toothMtx, actor->getModel()->getBaseTRMtx());
	}

	THitActor::perform(cue, graphics);
	actor->perform(cue, graphics);
}
