
#include <Enemy/BossEel.hpp>
#include <Enemy/Conductor.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraShake.hpp>
#include <GC2D/GCConsole2.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <Strategic/SharedParts.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/Application.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

f32 TBossEel::mOpenRollSpeed    = 0.3f;
bool TBossEel::mUseObjCollision = true;
f32 TBossEel::mForcePow         = 10.0f;
bool TBossEel::mUseMapCollision;
bool TBossEel::mToothDamageAnm;
f32 TBossEel::mTestAngY;

static const char* bosseel_bastable[] = {
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/bosseel/bas/meoto_in_loop.bas",
	nullptr,
	"/scene/bosseel/bas/meoto_mogu.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/bosseel/bas/meoto_out_loop.bas",
	"/scene/bosseel/bas/meoto_paku.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

static const char* bossEelTears_bastable[]
    = { nullptr, nullptr, nullptr, nullptr };

static f32 testHeight;

TBEelTearsDrop::TBEelTearsDrop(TBEelTears* owner, int jointIndex,
                               SDLModelData* modelData, const char* name)
    : THitActor(name)
    , mSharedParts(nullptr)
    , mOwner(owner)
{
	mSharedParts
	    = new TSharedParts(mOwner, jointIndex, modelData, 0, "<TSharedParts>");
	initHitActor(0x2000002C, 3, ACTOR_TYPE_PLAYER,
	             mOwner->mTearsParams->mSLTearsDropAttackRadius.get(),
	             mOwner->mTearsParams->mSLTearsDropAttackHeight.get(),
	             mOwner->mTearsParams->mSLTearsDropDamageRadius.get(),
	             mOwner->mTearsParams->mSLTearsDropDamageHeight.get());
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
	mActive = false;

	TScreenTexture* screenTexture
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
	const ResTIMG* textureInfo = screenTexture->getTexture()->getTexInfo();
	new J3DSkinDeform;
	MActor* actor = mSharedParts->getMActor();
	SMS_ChangeTextureAll(actor->getModel()->getModelData(), "M_dummy",
	                     *textureInfo);
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
		Mtx transform;
		// TODO: inline?
		MtxPtr ptr = transform;
		MsMtxSetXYZRPH(ptr, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
		               mRotation.y, mRotation.z);
		mSharedParts->getMActor()->getModel()->setBaseTRMtx(ptr);
		f32 scale = mOwner->mTearsParams->mSLTearsDropScaleLow.get();
		mScaling.set(scale, scale, scale);
		mSharedParts->getMActor()->getModel()->setBaseScale(mScaling);
	}
	mSharedParts->getMActor()->perform(cue, graphics);
}

void TBEelTearsDrop::generate(JGeometry::TVec3<f32>& position) { }

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
    , mBodyScaleRange(0.0f, 1.0f)
    , mTearsDropScaleRange(0.0f, 1.0f)
{
	TParams::load(mPrmPath);
	mBodyScaleRange.mMin      = mSLBodyScaleLow.get();
	mBodyScaleRange.mMax      = mSLBodyScaleHigh.get();
	mTearsDropScaleRange.mMin = mSLTearsDropScaleLow.get();
	mTearsDropScaleRange.mMax = mSLTearsDropScaleHigh.get();
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
	static TModelDataLoadEntry entry[] = {
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

	void* resource = JKRGetResource("/scene/bossEelTears/tears_drop.bmd");
	SDLModelData* modelData
	    = new SDLModelData(J3DModelLoaderDataBase::load(resource, 0x11240000));
	TBEelTears* owner = static_cast<TBEelTears*>(getObj(0));
	for (int i = 0; i < 30; ++i)
		mTearsDrops[i] = new TBEelTearsDrop(owner, 0, modelData, "涙粒");
}

void TBEelTearsManager::splitTears(JGeometry::TVec3<f32>& position)
{
	int tearsLeft = getSaveParam()->mSLTearsSplitNum.get();
	position.y += 600.0f;
	TMsRange<f32> positionRange(-250.0f, 250.0f);

	for (int i = 0; i < 30; ++i) {
		TBEelTearsDrop** dropPtr = &mTearsDrops[i];
		if ((*dropPtr)->mActive)
			continue;

		JGeometry::TVec3<f32> dropPosition = position;
		dropPosition.x += positionRange.rand();
		dropPosition.y += positionRange.rand();
		dropPosition.z += positionRange.rand();

		--tearsLeft;
		TBEelTearsDrop* drop = *dropPtr;
		drop->offHitFlag(HIT_FLAG_NO_COLLISION);
		drop->mActive = true;
		TMsRange<f32> riseRange(4.0f, 6.0f);
		drop->mRiseSpeed = riseRange.rand();
		drop->mPosition  = dropPosition;

		TMsRange<f32> unused(1.0f, 1.5f);
		unused.rand();

		f32 scale = drop->mOwner->mTearsParams->mTearsDropScaleRange.rand();
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
    , mSpawnMtx(nullptr)
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

	J3DSkinDeform* deform = new J3DSkinDeform;
	MActor* tearsActor    = mMActorKeeper->getMActor("tears.bmd");
	tearsActor->getModel()->setSkinDeform(deform, J3D_DEFORM_ATTACH_FLAG_UNK_1);
	tearsActor->resetDL();
	SMS_ChangeTextureAll(tearsActor->getModel()->getModelData(), "M_dummy",
	                     screenTexInfo);
	tearsActor->setLightType(3);

	MActor* waterHitActor = mMActorKeeper->getMActor("tears_waterhit.bmd");
	SMS_ChangeTextureAll(waterHitActor->getModel()->getModelData(), "M_dummy",
	                     screenTexInfo);
	waterHitActor->setLightType(3);

	onLiveFlag(LIVE_FLAG_DEAD);
	mBodyScale = mTearsParams->mBodyScaleRange.rand();

	TIdxGroupObj* enemyGroup
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	mRecoverCollision = new TBossEelTearsRecoverCollision(
	    mMActor->getModel()->getAnmMtx(0), "回復コリジョン");
	mRecoverCollision->initCollision();

	enemyGroup->getChildren().push_back(mRecoverCollision);
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
	TBEelTearsSaveLoadParams* params = mTearsParams;
	f32 liveHeight                   = params->mSLTearsLiveHeight.get();
	mVelocity.x *= 0.9f;
	mVelocity.z *= 0.9f;
	mPosition.x += mVelocity.x;
	mPosition.z += mVelocity.z;

	if (!mRecoverCollision->mColliding && mSpawnMtx != nullptr
	    && (mPosition.y > gpMarioPos->y + 3000.0f
	        || mPosition.y > mSpawnMtx[1][3] + liveHeight)) {
		kill();
		return;
	}

	f32 scale        = mScaling.x;
	s32 attackRadius = params->mSLTearsAttackRadius.get();
	s32 attackHeight = params->mSLTearsAttackHeight.get();
	s32 damageRadius = params->mSLTearsDamageRadius.get();
	s32 damageHeight = params->mSLTearsDamageHeight.get();
	setHitParams(attackRadius * scale, attackHeight * scale,
	             damageRadius * scale, damageHeight * scale);

	for (int i = 0; i < getColNum(); ++i) {
		THitActor* actor = getCollision(i);
		if (actor->isActorType(0x80000001)) {
			if (mSpine->getCurrentNerve()
			    == &TNerveBEelTearsMoveUp::theNerve()) {
				SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
				mSpine->pushNerve(&TNerveBEelTearsSplit::theNerve());
			}
		} else {
			JGeometry::TVec3<f32> velocity(0.0f, 0.0f, 0.0f);
			JGeometry::TVec3<f32> push(mPosition.x - actor->mPosition.x,
			                           mPosition.y - actor->mPosition.y,
			                           mPosition.z - actor->mPosition.z);
			if (push.x == 0.0f && push.y == 0.0f && push.z == 0.0f)
				push.x += 1.0f;
			MsVECNormalize(&push, &push);
			push *= 5.0f;
			velocity += push;
			mLinearVelocity = velocity;
		}
	}

	TLiveActor::moveObject();
}

void TBEelTears::calcRootMatrix()
{
	if (mSpawnMtx != nullptr) {
		if (mPosition.y < mSpawnMtx[1][3])
			mPosition.y = mSpawnMtx[1][3];
		TSpineEnemy::calcRootMatrix();

		if (mSpine->getCurrentNerve() == &TNerveBEelTearsGenerate::theNerve()) {
			TPosition3f transform(mPosition.x, mPosition.y, mPosition.z);
			transform.ref(0, 3)
			    = transform.at(0, 3)
			      + (mSpawnMtx[0][3] - transform.at(0, 3)) * 0.1f;
			transform.ref(2, 3)
			    = transform.at(2, 3)
			      + (mSpawnMtx[2][3] - transform.at(2, 3)) * 0.1f;

			mScaling.setAll(mTearsParams->mSLBodyScaleLow.get());
			mMActor->getModel()->setBaseScale(mScaling);
			mMActor->getModel()->setBaseTRMtx(transform);
		}
	} else {
		TSpineEnemy::calcRootMatrix();
	}
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
	}

	if (cue & CUE_CALC_ANIM)
		mMActor->frameUpdate();

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
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		mStateTimer = 60;
		if (mSpine->getCurrentNerve() == &TNerveBEelTearsMoveUp::theNerve()
		    || mSpine->getCurrentNerve() == &TNerveOilBallStay::theNerve())
			mSpine->pushNerve(&TNerveBEelTearsWaterHit::theNerve());

		if (mSpine->getCurrentNerve() == &TNerveBEelTearsWaterHit::theNerve()) {
			MActor* actor = mMActor;
			actor->setFrameRate(SMSGetAnmFrameRate(), 0);
		}
		return true;
	}
	return false;
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
	offLiveFlag(LIVE_FLAG_DEAD);
	offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
	offLiveFlag(LIVE_FLAG_UNK200);
	offLiveFlag(LIVE_FLAG_HIDDEN);
	mGroundPlane = gpMap->getIllegalCheckData();
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBEelTears::setBubble()
{
	mRecoverCollision->mColliding = false;
	mRecoverCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
	mRecoverCollision->mRecovering = true;
	mRecoverCollision->mPosition   = mPosition;
	mRecoverCollision->mPosition   = mPosition;
	mSpine->initWith(&TNerveBEelTearsMarioRecover::theNerve());
	onLiveFlag(LIVE_FLAG_HIDDEN);
	mRecoverCollision->mColliding = true;
}

void TBEelTears::setRecoverTears() { }

void TBEelTears::deadEffect()
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    BOSSEELTEARS_MS_MEO_TEAR_BOMB, &mPosition, 0, nullptr);
	if (emitter)
		emitter->setGlobalScale(mScaling);

	mRecoverCollision->mColliding = false;
	mRecoverCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
	mRecoverCollision->mRecovering = true;
	mRecoverCollision->mPosition   = mPosition;
	mRecoverCollision->mPosition   = mPosition;
	static_cast<TBEelTearsManager*>(mManager)->splitTears(mPosition);
	SMSGetMSound()->startSoundActor(MSD_SE_BS_UNG_TEAR_CLASH, &mPosition, 0,
	                                nullptr, 0, 4);
	onLiveFlag(LIVE_FLAG_HIDDEN);
}

const char** TBEelTears::getBasNameTable() const
{
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
		SMSGetMSound()->startSoundActor(MSD_SE_BS_UNG_TEAR_TREMBLE,
		                                &tears->mPosition, 0, nullptr, 0, 4);
		tears->mMActor = tears->mMActorKeeper->getMActor("tears_waterhit.bmd");
		tears->mMActor->setBckFromIndex(3);
	}

	--tears->mStateTimer;
	f32 frameRate = tears->mTearsParams->mSLHitAnmFrameRate.get();
	if (tears->mStateTimer < 0) {
		MActor* actor = tears->mMActor;
		actor->setFrameRate(-frameRate * SMSGetAnmFrameRate(), 0);
		if (tears->getCurAnmFrameNo(0) < 1.0f)
			return true;
	} else {
		MActor* actor = tears->mMActor;
		actor->setFrameRate(frameRate * SMSGetAnmFrameRate(), 0);
	}

	if (tears->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveBEelTearsMarioRecover::theNerve());
		tears->deadEffect();
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
		    BOSSEELTEARS_MS_MEO_TEAR_AWAGET, gpMarioPos, 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(tears->mScaling);
		tears->kill();
		return true;
	}

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    BOSSEELTEARS_MS_MEO_TEAR_AWA, &tears->mPosition, 1, tears);
	if (emitter)
		emitter->setGlobalScale(tears->mScaling);

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
		SMSGetMSound()->startSoundActor(MSD_SE_BS_UNG_TEAR_TREMBLE,
		                                &tears->mPosition, 0, nullptr, 0, 4);
		tears->mMActor = tears->mMActorKeeper->getMActor("tears_waterhit.bmd");
		tears->mMActor->setBckFromIndex(3);
		MActor* actor = tears->mMActor;
		f32 frameRate = tears->mTearsParams->mSLHitAnmFrameRate.get();
		actor->setFrameRate(frameRate * SMSGetAnmFrameRate(), 0);
	}

	if (tears->checkCurAnmEnd(0)) {
		tears->deadEffect();
		spine->pushAfterCurrent(&TNerveBEelTearsMarioRecover::theNerve());
		return true;
	}
	return false;
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
	TBEelTears::reset();
	mPosition = mInitialPosition;
	mSpine->initWith(&TNerveOilBallStay::theNerve());
	mMActor = mMActorKeeper->getMActor("tears.bmd");
	mMActor->setBckFromIndex(2);
	mHighPoly = false;
}

void TOilBall::moveObject()
{
	TBEelTearsSaveLoadParams* params = mTearsParams;
	f32 scale                        = mScaling.x;
	s32 attackRadius                 = params->mSLTearsAttackRadius.get();
	s32 attackHeight                 = params->mSLTearsAttackHeight.get();
	s32 damageRadius                 = params->mSLTearsDamageRadius.get();
	s32 damageHeight                 = params->mSLTearsDamageHeight.get();
	setHitParams(attackRadius * scale, attackHeight * scale,
	             damageRadius * scale, damageHeight * scale);

	for (int i = 0; i < getColNum(); ++i) {
		THitActor* actor = getCollision(i);
		if (actor->isActorType(0x80000001)) {
			if (mSpine->getCurrentNerve() == &TNerveBEelTearsMoveUp::theNerve()
			    || mSpine->getCurrentNerve()
			           == &TNerveOilBallStay::theNerve()) {
				SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
				mSpine->pushNerve(&TNerveBEelTearsSplit::theNerve());
			}
		} else {
			JGeometry::TVec3<f32> velocity(0.0f, 0.0f, 0.0f);
			JGeometry::TVec3<f32> push;
			push.sub(mPosition, actor->mPosition);
			if (push.x == 0.0f && push.y == 0.0f && push.z == 0.0f)
				push.x += 1.0f;
			MsVECNormalize(&push, &push);
			push.scale(5.0f);
			velocity.add(push);
			mLinearVelocity = velocity;
		}
	}
	TLiveActor::moveObject();
}

void TOilBall::calcRootMatrix() { TSpineEnemy::calcRootMatrix(); }

DEFINE_NERVE(TNerveOilBallStay, TLiveActor)
{
	TOilBall* oilBall = static_cast<TOilBall*>(spine->getBody());
	if (oilBall->getMActor()->checkCurBckFromIndex(3)
	    || (oilBall->checkCurAnmEnd(0)
	        && !oilBall->getMActor()->checkCurBckFromIndex(1))) {
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
    , unk78(0.0f, 0.0f, 0.0f)
    , mDamageCooldown(0)
    , mCanShedTears(true)
{
	s32 jointIndex = mOwner->getMActor()
	                     ->getModel()
	                     ->getModelData()
	                     ->getJointName()
	                     ->getIndex(jointName);
	mSharedParts
	    = new TSharedParts(mOwner, jointIndex, modelData, 0, "<TSharedParts>");
	MActor* actor = mSharedParts->getMActor();

	mSharedParts->getMActor()->setLightType(1);
	mSharedParts->getMActor()->setBckFromIndex(22);
	mSharedParts->getMActor()->setFrameRate(0.0f, 0);

	for (u16 i = 0; i < actor->getModel()->getModelData()->getMaterialNum();
	     ++i)
		SMS_InitPacket_OneTevKColorAndFog(actor->getModel(), i, GX_KCOLOR0,
		                                  &mColor);

	mColor.a   = 0xFF;
	mHitPoints = mOwner->getBossEelParams().mSLToothMaxHitPoint.get();
	initHitActor(0x08000022, 5, 0x81000000,
	             mOwner->getBossEelParams().mSLToothAttackRadius.get(),
	             mOwner->getBossEelParams().mSLToothAttackHeight.get(),
	             mOwner->getBossEelParams().mSLToothDamageRadius.get(),
	             mOwner->getBossEelParams().mSLToothDamageHeight.get());

	JDrama::TNameRefGen::search<TIdxGroupObj>("オブジェクトグループ")
	    ->getChildren()
	    .push_back(this);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBossEelTooth::changeToothAlpha(u8 alpha) { mColor.a = alpha; }

BOOL TBossEelTooth::receiveMessage(THitActor* sender, u32 message)
{
	BOOL result = false;
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (mDamageCooldown == 0 && !mOwner->mToothBroken && mHitPoints > 1) {
			mDamageCooldown = 2;
			--mHitPoints;
			MActor* actor = mSharedParts->getMActor();
			actor->setFrameRate(SMSGetAnmFrameRate(), 0);
			mOwner->mToothDamaged = true;
			mColor.a              = static_cast<u8>(
                static_cast<f32>(mHitPoints * 0xFF)
                / static_cast<f32>(
                    mOwner->getBossEelParams().mSLToothMaxHitPoint.get()));

			if (mToothType == 1 && mHitPoints % 20 == 1)
				mOwner->forceShedTears(mCanShedTears);

			if (mHitPoints == 1) {
				mColor.a = 0;
				MTXCopy(mSharedParts->getConnectedMtx(), mDetachedMtx);
				if (mToothType == 1) {
					SMSGetMSound()->startSoundActor(MSD_SE_BS_UNG_TEATH_COMEOFF,
					                                &mPosition, 0, nullptr, 0,
					                                4);
					if (mCanShedTears)
						SMSGetMSound()->startSoundActor(
						    MSD_SE_BS_UNG_VOICE_M_CRY, &mOwner->mPosition, 0,
						    nullptr, 0, 4);
					else
						SMSGetMSound()->startSoundActor(
						    MSD_SE_BS_UNG_VOICE_W_CRY, &mOwner->mPosition, 0,
						    nullptr, 0, 4);
					mOwner->mToothBroken = true;

					JPABaseEmitter* emitter = gpMarioParticleManager->emit(
					    BOSSEEL_JPA_MS_MEO_AWA_TOOTH, &mPosition, 0, nullptr);
					if (emitter)
						emitter->setGlobalScale(mOwner->mScaling);
				} else {
					SMSGetMSound()->startSoundActor(MSD_SE_BS_UNG_TEATH_FLASH,
					                                &mPosition, 0, nullptr, 0,
					                                4);
					if (mCanShedTears)
						SMSGetMSound()->startSoundActor(
						    MSD_SE_BS_UNG_VOICE_M_JOY, &mOwner->mPosition, 0,
						    nullptr, 0, 4);
					else
						SMSGetMSound()->startSoundActor(
						    MSD_SE_BS_UNG_VOICE_W_JOY, &mOwner->mPosition, 0,
						    nullptr, 0, 4);
				}
			}
		}
		result = true;
	}
	return result;
}

void TBossEelTooth::updateTremble() { }

void TBossEelTooth::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mOwner->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT) || mHitPoints == 0)
		return;

	if (cue & CUE_MOVE) {
		TBossEelSaveParams* params = mOwner->mSaveParams;
		f32 scale                  = mOwner->mScaling.x;
		f32 attackRadius           = params->mSLToothAttackRadius.get();
		f32 attackHeight           = params->mSLToothAttackHeight.get();
		f32 damageHeight           = params->mSLToothDamageHeight.get();
		f32 damageRadius           = params->mSLToothDamageRadius.get();
		attackRadius *= scale;
		attackHeight *= scale;
		damageRadius *= scale;
		damageHeight *= scale;
		setHitParams(attackRadius, attackHeight, damageRadius, damageHeight);

		for (s32 i = 0; i < mColCount; ++i) {
			THitActor* collision = mCollisions[i];
			if (mOwner->isValidToothDamage()
			    && collision->isActorType(0x80000001) && mHitPoints > 1) {
				SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
			}
		}

		if (mDamageCooldown > 0) {
			--mDamageCooldown;
		} else {
			if (mSharedParts->getMActor()->checkCurBckFromIndex(22)
			    && mSharedParts->getMActor()->curAnmEndsNext(0, nullptr))
				mSharedParts->getMActor()->setFrameRate(0.0f, 0);

			if (mHitPoints == 1) {
				if (mToothType == 1) {
					if (mSharedParts->getMActor()->checkCurBckFromIndex(22)) {
						mSharedParts->getMActor()->setBckFromIndex(20);
						JGeometry::TVec3<f32> tearsPosition(
						    mDetachedMtx[0][3], unk78.y + mDetachedMtx[1][3],
						    mDetachedMtx[2][3]);
						mOwner->generateBubble(tearsPosition);
						mSharedParts->getMActor()->setFrameRate(
						    SMSGetAnmFrameRate(), 0);
					}
					if (mSharedParts->getMActor()->checkCurBckFromIndex(20)
					    && mSharedParts->getMActor()->curAnmEndsNext(0,
					                                                 nullptr))
						mSharedParts->getMActor()->setBckFromIndex(21);
				}

				f32 speed = mOwner->getBossEelParams().mSLToothUpSpeed.get();
				if (mToothType == 1) {
					unk78.y += speed;
					if (unk78.y > mOwner->getBossEelParams()
					                  .mSLToothLiveHeight.get()
					    || mPosition.y > gpMarioPos->y + 2000.0f) {
						mHitPoints = 0;
						onHitFlag(HIT_FLAG_NO_COLLISION);
					}
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
			    = gpMarioParticleManager->emitAndBindToPosPtr(
			        BOSSEEL_JPA_MS_MEO_TOOTH_ALWAYS, &mPosition, 1, this);
			if (emitter)
				emitter->setGlobalScale(mOwner->mScaling);
		}
		if (mSharedParts->getMActor()->checkCurBckFromIndex(22)
		    && mSharedParts->getMActor()->getFrameCtrl(0)->getRate() > 0.0f) {
			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToPosPtr(
			        BOSSEEL_JPA_MS_MEO_TOOTH_WASH, &mPosition, 1, this);
			if (emitter)
				emitter->setGlobalScale(mOwner->mScaling);
		}
		if ((mToothType == 0 || mToothType == 2) && mHitPoints == 1) {
			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToPosPtr(
			        BOSSEEL_JPA_MS_MEO_TOOTH_KIRA, &mPosition, 1, this);
			if (emitter)
				emitter->setGlobalScale(mOwner->mScaling);
		}

		TPosition3f transform(0, 0, unk78.x);
		MTXConcat(toothMtx, transform, toothMtx);
		MsMtxSetRotRPH(transform, unk78.z, unk78.z, 0.0f);
		MTXConcat(toothMtx, transform, toothMtx);
		toothMtx[1][3] += unk78.y;
		mPosition.x = toothMtx[0][3];
		mPosition.y = toothMtx[1][3];
		mPosition.z = toothMtx[2][3];
		mSharedParts->getMActor()->getModel()->setBaseTRMtx(toothMtx);
	}

	THitActor::perform(cue, graphics);
	mSharedParts->getMActor()->perform(cue, graphics);
}

TBossEelVortex::TBossEelVortex(TBossEel* owner, const char* name)
    : THitActor(name)
    , mOwner(owner)
    , mInactive(true)
{
	initHitActor(0x8000003, 3, ACTOR_TYPE_PLAYER,
	             mOwner->getBossEelParams().mSLVortexAttackRadius.get(),
	             mOwner->getBossEelParams().mSLVortexAttackHeight.get(),
	             mOwner->getBossEelParams().mSLVortexDamageRadius.get(),
	             mOwner->getBossEelParams().mSLVortexDamageHeight.get());
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBossEelVortex::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mInactive)
		return;

	if (cue & CUE_MOVE) {
		{
			TBossEel* owner            = mOwner;
			TBossEelSaveParams* params = owner->mSaveParams;
			f32 scale                  = owner->mScaling.x;
			f32 attackRadius           = params->mSLVortexAttackRadius.get();
			f32 attackHeight           = params->mSLVortexAttackHeight.get();
			f32 damageHeight           = params->mSLVortexDamageHeight.get();
			f32 damageRadius           = params->mSLVortexDamageRadius.get();
			mAttackRadius              = attackRadius * scale;
			mAttackHeight              = attackHeight * scale;
			mDamageRadius              = damageRadius * scale;
			mDamageHeight              = damageHeight * scale;
		}
		calcEntryRadius();

		++mTimer;
		if (mTimer > 30) {
			if (mOwner->mMActor->checkCurBckFromIndex(14)
			    || mOwner->mMActor->checkCurBckFromIndex(17)) {
				for (s32 i = 0; i < mColCount; ++i) {
					if (!mCollisions[i]->isActorType(0x80000001))
						continue;

					JGeometry::TVec3<f32> marioTarget;
					marioTarget.set(mOwner->mPosition.x - gpMarioPos->x,
					                mOwner->mPosition.y - gpMarioPos->y,
					                mOwner->mPosition.z - gpMarioPos->z);
					MsVECNormalize(&marioTarget, &marioTarget);

					f32 power = mOwner->mSaveParams->mSLBreathInPower.get();
					f32 wave  = fabsf(JMASSin(
                        static_cast<s16>(182.04445f * (0.9f * mTimer))));
					if (wave > 1.0f)
						wave = 1.0f;
					else if (wave < 0.1f)
						wave = 0.1f;

					power *= wave;
					SMSRumbleMgr->start(8, &mPosition);
					if (mOwner->mMActor->checkCurBckFromIndex(17))
						power *= 0.5f;
					marioTarget.scale(power);
					marioTarget.add(*gpMarioPos);
					SMS_MarioMoveRequest(marioTarget);
				}
			} else {
				onHitFlag(HIT_FLAG_NO_COLLISION);
			}
		}
	}

	if (cue & CUE_CALC_ANIM) {
		mPosition.x = mOwner->mMActor->getModel()->getAnmMtx(
		    mOwner->mMapCollisionJointIndices[2])[0][3];
		mPosition.y = mOwner->mMActor->getModel()->getAnmMtx(
		                  mOwner->mMapCollisionJointIndices[2])[1][3]
		              + 1000.0f;
		mPosition.z = mOwner->mMActor->getModel()->getAnmMtx(
		    mOwner->mMapCollisionJointIndices[2])[2][3];
	}
	THitActor::perform(cue, graphics);
}

void TBossEelVortex::reset()
{
	offHitFlag(HIT_FLAG_NO_COLLISION);
	onHitFlag(ACTOR_TYPE_PLAYER);
	mTimer = 0;
}

TBossEelEye::TBossEelEye(const TLiveActor* owner, int jointIndex,
                         SDLModelData* modelData, u32 modelFlags,
                         const char* name)
    : TSharedParts(owner, jointIndex, modelData, modelFlags, name)
    , mBlendModel(nullptr)
    , mCopyConnectedMtx(-1)
    , mBlinkTimer(0)
    , mBlinkDelay(0)
    , mBlurTimer(0)
    , mBlurDuration(50)
{
	mBlendModel = new SDLModel(modelData, modelFlags, 1);
	mBlendModel->getModelData()->getMaterialName()->getIndex("_mat7");
	getMActor()->initNormalMotionBlend();
	mPreviousBckIndex = getMActor()->getCurAnmIdx(0);
	mAnimationMode    = 0;
	mBlendRatio       = 1.0f;
	getMActor()->setBckOldMotionBlendAnmPtr(getMActor()->getBckAnm());
	getMActor()->setBckFromIndex(0);
	getMActor()->setMotionBlendRatioForBck(mBlendRatio);
}

void TBossEelEye::perform(u32 cue, JDrama::TGraphics* graphics)
{
	const TBossEel* owner = getOwner();
	if (owner->mLiveFlag
	    & (LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT))
		return;

	if (cue & CUE_CALC_ANIM) {
		mBlurPosition.set(getConnectedMtx()[0][3], getConnectedMtx()[1][3],
		                  getConnectedMtx()[2][3]);
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    BOSSEEL_JPA_MS_MEO_EYEBLUR, &mBlurPosition, 1, this);
		if (emitter)
			emitter->setGlobalScale(owner->mScaling);

		Mtx eyeMtx;
		MTXCopy(getConnectedMtx(), eyeMtx);
		getMActor()->getModel()->setBaseTRMtx(eyeMtx);
		if (mCopyConnectedMtx == 0)
			MTXCopy(eyeMtx, mBlendMtx);

		mBlendRatio
		    = JGeometry::TUtil<f32>::clamp(mBlendRatio - 0.01f, 0.0f, 1.0f);
		getMActor()->setMotionBlendRatioForBck(mBlendRatio);
		if (mAnimationMode == 1 && getMActor()->curAnmEndsNext(0, nullptr)) {
			++mAnimationLoopCount;
			if (mAnimationLoopCount > 3) {
				mPreviousBckIndex = getMActor()->getCurAnmIdx(0);
				mAnimationMode    = 0;
				mBlendRatio       = 1.0f;
				getMActor()->setBckOldMotionBlendAnmPtr(
				    getMActor()->getBckAnm());
				getMActor()->setBckFromIndex(0);
				getMActor()->setMotionBlendRatioForBck(mBlendRatio);

				TBossEelEye* paired = mPairedEye;
				paired->mPreviousBckIndex
				    = paired->getMActor()->getCurAnmIdx(0);
				paired->mAnimationMode = 0;
				paired->mBlendRatio    = 1.0f;
				paired->getMActor()->setBckOldMotionBlendAnmPtr(
				    paired->getMActor()->getBckAnm());
				paired->getMActor()->setBckFromIndex(0);
				paired->getMActor()->setMotionBlendRatioForBck(
				    paired->mBlendRatio);
			}
		}
	}
	getMActor()->perform(cue, graphics);
	mCopyConnectedMtx = -1;
}

void TBossEelEye::setBckAnm(int index)
{
	mPreviousBckIndex = getMActor()->getCurAnmIdx(0);
	mAnimationMode    = 1;
	mBlendRatio       = 1.0f;
	getMActor()->setBckOldMotionBlendAnmPtr(getMActor()->getBckAnm());
	getMActor()->setBckFromIndex(index);
	getMActor()->setMotionBlendRatioForBck(mBlendRatio);
}

TBossEelHeartCoin::TBossEelHeartCoin(const TLiveActor* owner, int jointIndex,
                                     SDLModelData* modelData, u32 modelFlags,
                                     const char* name)
    : TSharedParts(owner, jointIndex, modelData, modelFlags, name)
    , mActive(false)
    , mOwner(nullptr)
{
	mOwner = owner;
	for (s32 i = 0; i < 20; ++i) {
		mCoins[i] = gpItemManager->newAndRegisterCoinReal();
		mCoins[i]->kill();
	}
}

void TBossEelHeartCoin::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!mActive)
		return;

	u32 calcAnim = cue & CUE_CALC_ANIM;
	if (calcAnim) {
		s32 jointIndex = mOwner->mMActor->getModel()
		                     ->getModelData()
		                     ->getJointName()
		                     ->getIndex("ha7");
		MtxPtr jointMtx = mOwner->mMActor->getModel()->getAnmMtx(jointIndex);
		if (mOwner->mMActor->checkCurBckFromIndex(3)
		    && mOwner->mMActor->getFrameCtrl(0)->getFrame() < 700.0f) {
			mPosition.set(jointMtx[0][3], jointMtx[1][3], jointMtx[2][3]);
		}
		mPosition.y = jointMtx[1][3];

		TPosition3f heartMtx(mPosition.x, mPosition.y, mPosition.z);
		getMActor()->getModel()->setBaseTRMtx(heartMtx);
	}
	getMActor()->perform(cue, graphics);

	if (calcAnim) {
		for (s32 i = 0; i < 20; ++i) {
			MtxPtr coinMtx = getMActor()->getModel()->getAnmMtx(i + 2);
			mCoins[i]->mPosition.set(coinMtx[0][3], coinMtx[1][3],
			                         coinMtx[2][3]);
		}
	}
}

#pragma dont_inline on
void TBossEelHeartCoin::generate(JGeometry::TVec3<f32>& position)
{
	mPosition.set(position.x, position.y, position.z);
	mActive = true;
	for (s32 i = 0; i < 20; ++i) {
		mCoins[i]->appear();
		mCoins[i]->onMapObjFlag(0x10000000);
		MtxPtr coinMtx = getMActor()->getModel()->getAnmMtx(0);
		mCoins[i]->mPosition.set(coinMtx[0][3], coinMtx[1][3], coinMtx[2][3]);
	}
}
#pragma dont_inline off

TBossEel::TBossEel(const char* name)
    : TSpineEnemy(name)
    , mHeadCollision(nullptr)
    , mMouthCubeManager(nullptr)
    , mBodyCollision(nullptr)
    , mBckBlendRatio(1.0f)
    , mTearEyeIndex(0)
    , mTearCycleTimer(0)
    , mSpinAngle(500.0f)
    , mTearEyeToggle(true)
    , mMouthOpenAmount(2350.0f)
    , mMouthOpenSpeed(0.75f)
    , mSaveParams(nullptr)
    , mInDemo(false)
    , mAppearOffset(0.0f)
    , mToothDamaged(false)
    , mToothBroken(false)
    , mIsBiting(false)
    , mBattleTimer(0)
    , mBarrierCollision(nullptr)
    , mAwaCollision(nullptr)
    , mHeartCoin(nullptr)
    , mMoguCameraActive(false)
    , mCollisionEnabled(true)
{
	mSpinTimer = new JGeometry::TVec2<s32>(0, 1);
}

void TBossEel::generateBubble(JGeometry::TVec3<f32>& position)
{
	TBEelTears* tears
	    = static_cast<TBEelTears*>(gpConductor->makeOneEnemyAppear(
	        position, "めおとウナギ涙マネージャー", 0));
	if (tears)
		tears->setBubble();
}

TBossEelCollision::TBossEelCollision(MtxPtr collisionMtx, const char* name)
    : THitActor(name)
    , mCollisionMtx(collisionMtx)
    , mBaseAttackRadius(0.0f)
    , mBaseAttackHeight(0.0f)
    , mBaseDamageRadius(0.0f)
    , mBaseDamageHeight(0.0f)
    , mOwner(nullptr)
{
}

void TBossEelCollision::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		calcEntryRadius();
		for (s32 i = 0; i < mColCount; ++i) {
			if (mCollisions[i]->isActorType(0x80000001))
				behaveToMario();
		}
	}
	if (cue & CUE_CALC_ANIM) {
		mPosition.x = mCollisionMtx[0][3];
		mPosition.y = mCollisionMtx[1][3];
		mPosition.z = mCollisionMtx[2][3];
	}
	THitActor::perform(cue, graphics);
}

void TBossEelCollision::initCollision()
{
	initHitActor(0x08000023, 5, ACTOR_TYPE_PLAYER, mBaseAttackRadius,
	             mBaseAttackHeight, mBaseDamageRadius, mBaseDamageHeight);
}

void TBossEelCollision::behaveToMario()
{
	JGeometry::TVec3<f32> marioTarget(0.0f, TBossEel::mForcePow, 0.0f);
	marioTarget += SMS_GetMarioPos();
	SMS_MarioMoveRequest(marioTarget);

	if (mOwner && mOwner->canEatMario())
		mOwner->forceEat();
}

void TBossEelBodyCollision::initCollision()
{
	mBaseAttackRadius = 2000.0f;
	mBaseDamageRadius = 2000.0f;
	mBaseAttackHeight = 5800.0f;
	mBaseDamageHeight = 5000.0f;
	TBossEelCollision::initCollision();
}

void TBossEelAwaCollision::initCollision()
{
	mBaseDamageRadius = 2000.0f;
	mBaseAttackRadius = 2000.0f;
	mBaseDamageHeight = 2000.0f;
	mBaseAttackHeight = 2000.0f;
	initHitActor(0x08000003, 2, ACTOR_TYPE_PLAYER, mBaseAttackRadius,
	             mBaseAttackHeight, mBaseDamageRadius, mBaseDamageHeight);
}

void TBossEelAwaCollision::behaveToMario()
{
	JGeometry::TVec3<f32> marioTarget(0.0f, 10.0f, 0.0f);
	marioTarget.y  = 15.0f;
	*gpMarioSpeedY = 0.0f;
	marioTarget.add(*gpMarioPos);
	SMS_MarioMoveRequest(marioTarget);
}

void TBossEelAwaCollision::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		calcEntryRadius();
		if (gpMarioPos->y < mPosition.y + 500.0f)
			offHitFlag(HIT_FLAG_NO_COLLISION);
		if (gpMarioPos->y > mPosition.y + mAttackHeight)
			onHitFlag(HIT_FLAG_NO_COLLISION);

		for (s32 i = 0; i < mColCount; ++i) {
			if (mCollisions[i]->isActorType(0x80000001))
				behaveToMario();
		}
	}
	if (cue & CUE_CALC_ANIM) {
		mPosition.x = mCollisionMtx[0][3];
		mPosition.y = mCollisionMtx[1][3] + 11000.0f;
		mPosition.z = mCollisionMtx[2][3];
	}
	THitActor::perform(cue, graphics);
}

void TBossEelBarrierCollision::initCollision()
{
	mBaseDamageRadius = 0.0f;
	mBaseAttackRadius = 0.0f;
	mBaseDamageHeight = 0.0f;
	mBaseAttackHeight = 0.0f;
	TBossEelCollision::initCollision();
}

void TBossEelBarrierCollision::behaveToMario()
{
	JGeometry::TVec3<f32> marioTarget(0.0f, TBossEel::mForcePow, 0.0f);
	marioTarget.add(*gpMarioPos);
	SMS_MarioMoveRequest(marioTarget);
}

void TBossEelTearsRecoverCollision::initCollision()
{
	mBaseDamageRadius = 400.0f;
	mBaseAttackRadius = 400.0f;
	mBaseDamageHeight = 400.0f;
	mBaseAttackHeight = 400.0f;
	initHitActor(0x2000002C, 3, ACTOR_TYPE_PLAYER, mBaseAttackRadius,
	             mBaseAttackHeight, mBaseDamageRadius, mBaseDamageHeight);
}

void TBossEelTearsRecoverCollision::behaveToMario()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
	mRecovering = false;
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBossEelTearsRecoverCollision::perform(u32 cue,
                                            JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		calcEntryRadius();
		for (s32 i = 0; i < mColCount; ++i) {
			if (mCollisions[i]->isActorType(0x80000001))
				behaveToMario();
		}
	}
	THitActor::perform(cue, graphics);
}

void TBossEel::init(TLiveManager* manager)
{
	mManager = manager;
	manager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(manager);
	mMActorKeeper->createMActorFromAllBmd(0);
	mMActor     = mMActorKeeper->getMActor(0);
	mSaveParams = &static_cast<TBossEelManager*>(manager)->mSaveParams;
	mInitialPosition.set(mPosition);
	onLiveFlag(LIVE_FLAG_UNK8 | LIVE_FLAG_UNK10);
	mGroundHeight = gpMap->checkGround(
	    mPosition.x, mPosition.y + getHeadHeight(), mPosition.z, &mGroundPlane);
	mMActor->initNormalMotionBlend();
	mSpine->initWith(&TNerveBossEelWaitAppear::theNerve());

	initHitActor(0x08000003, 1, ACTOR_TYPE_PLAYER,
	             mSaveParams->mSLBodyAttackRadius.get(),
	             mSaveParams->mSLBodyAttackHeight.get(),
	             mSaveParams->mSLBodyDamageRadius.get(),
	             mSaveParams->mSLBodyDamageHeight.get());
	offHitFlag(HIT_FLAG_NO_COLLISION);
	J3DModel* model = mMActor->getModel();
	if (!model->getSkinDeform())
		model->setSkinDeform(new J3DSkinDeform, J3D_DEFORM_ATTACH_FLAG_UNK_1);
	mMActor->resetDL();

	mHeadCollision = new THitActor("めおとウナギの頭部");
	mHeadCollision->initHitActor(0x08000003, 2, ACTOR_TYPE_PLAYER,
	                             mSaveParams->mSLHeadAttackRadius.get(),
	                             mSaveParams->mSLHeadAttackHeight.get(),
	                             mSaveParams->mSLHeadDamageRadius.get(),
	                             mSaveParams->mSLHeadDamageHeight.get());
	TIdxGroupObj* enemyGroup
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	enemyGroup->getChildren().push_back(mHeadCollision);
	mHeadCollision->offHitFlag(HIT_FLAG_NO_COLLISION);

	mBodyCollision = new TBossEelBodyCollision(
	    mMActor->getModel()->getBaseTRMtx(), "体コリジョン");
	mBodyCollision->initCollision();
	mBodyCollision->mOwner = this;
	enemyGroup->getChildren().push_back(mBodyCollision);
	mBodyCollision->offHitFlag(HIT_FLAG_NO_COLLISION);

	mBarrierCollision = new TBossEelBarrierCollision(
	    mMActor->getModel()->getAnmMtx(7), "障害コリジョン");
	mBarrierCollision->initCollision();
	enemyGroup->getChildren().push_back(mBarrierCollision);
	mBarrierCollision->offHitFlag(HIT_FLAG_NO_COLLISION);

	JUTNameTab* jntNames = mMActor->getModel()->getModelData()->getJointName();

	{
		static const char* sEyePartsJointTable[]
		    = { "eye1", "eye2", "eye3", "eye4" };

		void* eyeResource = JKRGetResource("/scene/bosseel/eye.bmd");
		SDLModelData* eyeModelData
		    = new SDLModelData(J3DModelLoaderDataBase::load(
		        eyeResource, J3DMLF_MaterialPEFull | J3DMLF_MaterialTexGenFull
		                         | (2 << J3DMLF_TevStageNumShift)));
		for (s32 i = 0; i < 4; ++i) {
			s32 jointIndex = jntNames->getIndex(sEyePartsJointTable[i]);
			mEyes[i]       = new TBossEelEye(this, jointIndex, eyeModelData, 3,
			                                 "めおとウナギ目");
		}
		mEyes[0]->mPairedEye = mEyes[1];
		mEyes[1]->mPairedEye = mEyes[0];
		mEyes[2]->mPairedEye = mEyes[3];
		mEyes[3]->mPairedEye = mEyes[2];
		mEyes[2]->getMActor()->getFrameCtrl(0)->setFrame(100.0f);
		mEyes[3]->getMActor()->getFrameCtrl(0)->setFrame(100.0f);
	}

	{
		SDLModelData* toothModelData[3];

		void* resource1   = JKRGetResource("/scene/bosseel/tooth.bmd");
		toothModelData[0] = new SDLModelData(J3DModelLoaderDataBase::load(
		    resource1,
		    J3DMLF_MaterialPEFull | (16 << J3DMLF_TevStageNumShift)));
		void* resource2   = JKRGetResource("/scene/bosseel/bad_tooth.bmd");
		toothModelData[0] = new SDLModelData(J3DModelLoaderDataBase::load(
		    resource2,
		    J3DMLF_MaterialPEFull | (16 << J3DMLF_TevStageNumShift)));
		void* resource3   = JKRGetResource("/scene/bosseel/gold_tooth.bmd");
		toothModelData[0] = new SDLModelData(J3DModelLoaderDataBase::load(
		    resource3,
		    J3DMLF_MaterialPEFull | (16 << J3DMLF_TevStageNumShift)));

		static const char* sToothPartsJointTable[]
		    = { "ha1", "ha2", "ha3", "ha4", "ha5", "ha6", "ha7", "ha8" };
		for (s32 i = 0; i < 8; ++i) {
			u8 toothType = 0;
			if (i == 1 || i == 4 || i == 7)
				toothType = 1;
			if (i == 6)
				toothType = 2;
			mTeeth[i] = new TBossEelTooth(
			    toothType, this, sToothPartsJointTable[i],
			    toothModelData[toothType], "めおとウナギの歯");
			mTeeth[i]->mCanShedTears = false;
			if (i == 0 || i == 1 || i == 2 || i == 7)
				mTeeth[i]->mCanShedTears = true;
		}
	}

	{
		void* heartResource
		    = JKRGetResource("/scene/bosseel/meoto_heartcoin.bmd");
		SDLModelData* heartModelData
		    = new SDLModelData(J3DModelLoaderDataBase::load(
		        heartResource, J3DMLF_MaterialPEFull | J3DMLF_MaterialTexGenFull
		                           | (2 << J3DMLF_TevStageNumShift)));
		mHeartCoin = new TBossEelHeartCoin(this, 0, heartModelData, 3,
		                                   "めおとウナギハートコイン");
	}

	{
		static const char* sCollisionJointTable[]
		    = { "headcol1", "headcol2", "sebone5", "sebone5" };
		static const char* sCollisionFileTable[]
		    = { "/bosseel/meoto_head", "/bosseel/meoto_head2",
			    "/bosseel/meoto_camera", "/bosseel/meoto_out_loop" };

		for (s32 i = 0; i < 4; ++i) {
			mMapCollisionJointIndices[i]
			    = jntNames->getIndex(sCollisionJointTable[i]);
			mMapCollisions[i] = new TMapCollisionMove;
			mMapCollisions[i]->init(sCollisionFileTable[i], 2, this);
			mMapCollisions[i]->moveTrans(mPosition);
		}
	}

	mAwaCollision = new TBossEelAwaCollision(
	    mMActor->getModel()->getAnmMtx(mMapCollisionJointIndices[2]),
	    "泡コリジョン");
	mAwaCollision->initCollision();
	enemyGroup->getChildren().push_back(mAwaCollision);
	mAwaCollision->onHitFlag(HIT_FLAG_NO_COLLISION);

	mVortex           = new TBossEelVortex(this, "めおとウナギ渦");
	mMouthCubeManager = new TCubeManagerBase("コリジョンキューブ", 2);
	TCubeGeneralInfo* mouthCube
	    = *mMouthCubeManager->unk14->getChildren().begin();
	mouthCube->unkC.set(mPosition.x, mPosition.y + 9600.0f, mPosition.z);
	mouthCube->unk24.set(7000.0f, 10000.0f, 7000.0f);

	if (mInstanceIndex == 0) {
		for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); ++i)
			; // assert?
	}

	initAnmSound();
	mMActor->getModel()->calc();
}

MtxPtr TBossEel::getTakingMtx() { return mMActor->getModel()->getAnmMtx(7); }

void TBossEel::calcAndSetCollisionCubeBite_()
{
	TCubeGeneralInfo* mouthCube
	    = *mMouthCubeManager->unk14->getChildren().begin();
	mouthCube->unk18.set(mRotation);
	mouthCube->unkC.set(mPosition.x, mPosition.y + 1900.0f, mPosition.z);
	mouthCube->unk24.set(1100.0f, 1000.0f, 1100.0f);
	mouthCube->unkC.set(mPosition.x, mPosition.y + 9600.0f * mScaling.y,
	                    mPosition.z);
	mouthCube->unk24.set(7000.0f * mScaling.x, 10000.0f * mScaling.y,
	                     7000.0f * mScaling.z);
}

void TBossEel::updateTearsCnt()
{
	static const s32 eyeTable[] = { 0, 2, 1, 3 };

	++mTearCycleTimer;
	s32 interval = mSaveParams->mSLGenTearsTime.get();
	f32 height   = fabsf(mPosition.y - gpMarioPos->y);
	if (height > 30000.0f)
		interval *= 4;
	else if (height > 15000.0f)
		interval *= 3;
	else if (height > 6000.0f)
		interval *= 2;

	if (mTearCycleTimer == interval - 100) {
		s32 eyeIndex = eyeTable[mTearEyeIndex];
		mEyes[eyeIndex]->setBckAnm(1);
		mEyes[eyeIndex]->mPairedEye->setBckAnm(1);
	}

	if (mTearCycleTimer > interval) {
		mTearCycleTimer = 0;
		s32 eyeIndex    = eyeTable[mTearEyeIndex];
		MtxPtr spawnMtx = mEyes[eyeIndex]->getConnectedMtx();
		mEyes[eyeIndex]->setBckAnm(1);
		mEyes[eyeIndex]->mAnimationLoopCount = 0;
		shedTears(spawnMtx);
		mTearEyeIndex += 1;
		if (mTearEyeIndex >= 4)
			mTearEyeIndex = 0;
	}
}

void TBossEel::forceEat()
{
	if (mSpine->getCurrentNerve() != &TNerveBossEelEat::theNerve())
		mSpine->pushNerve(&TNerveBossEelEat::theNerve());
}

bool TBossEel::canEatMario()
{
	if (mForceEat)
		return true;

	MtxPtr mouthMtx
	    = getMActor()->getModel()->getAnmMtx(mMapCollisionJointIndices[0]);
	JGeometry::TVec3<f32> distance = *gpMarioPos;
	distance.x -= mouthMtx[0][3];
	distance.y -= mouthMtx[1][3];
	distance.z -= mouthMtx[2][3];
	if (MsVECMag2(&distance) < mMouthOpenAmount * mMouthOpenSpeed)
		return true;
	return false;
}

BOOL TBossEel::isEyeBlurOn() { }

void TBossEel::shedTears(MtxPtr spawnMtx)
{
	JGeometry::TVec3<f32> position(spawnMtx[0][3], spawnMtx[1][3],
	                               spawnMtx[2][3]);
	TBEelTears* tears
	    = static_cast<TBEelTears*>(gpConductor->makeOneEnemyAppear(
	        position, "めおとウナギ涙マネージャー", 0));
	if (!tears)
		return;

	tears->mSpawnMtx = spawnMtx;
	tears->reset();

	JGeometry::TVec3<f32> direction(0.0f, 0.0f, 100.0f);
	TMsRange<f32> angleRange(0.0f, 360.0f);
	Mtx rotation;
	MsMtxSetRotRPH(rotation, 0.0f, angleRange.rand(), 0.0f);
	MTXMultVec(rotation, &direction, &direction);
	tears->mPosition.x += direction.x;
	tears->mPosition.z += direction.z;
	MsVECNormalize(&direction, &direction);
	direction.x *= 10.0f;
	direction.z *= 10.0f;
	tears->mVelocity = direction;
}

#pragma dont_inline on
void TBossEel::forceShedTears(bool rearEyes)
{
	mTearEyeToggle = !mTearEyeToggle;
	s32 eyeIndex;
	if (!rearEyes) {
		eyeIndex = 0;
		if (mTearEyeToggle)
			eyeIndex = 1;
	} else {
		eyeIndex = 2;
		if (mTearEyeToggle)
			eyeIndex = 3;
	}

	MtxPtr spawnMtx = mEyes[eyeIndex]->getConnectedMtx();
	mEyes[eyeIndex]->setBckAnm(1);
	mEyes[eyeIndex]->mAnimationLoopCount = 0;
	shedTears(spawnMtx);
}
#pragma dont_inline off

void TBossEel::generateVortex()
{
	mVortex->reset();
	MtxPtr vortexMtx
	    = getMActor()->getModel()->getAnmMtx(mMapCollisionJointIndices[2]);
	mVortex->mPosition.set(vortexMtx[0][3], vortexMtx[1][3], vortexMtx[2][3]);
	mVortex->mInactive = false;
	mVortex->mScaling.set(mSaveParams->mSLVortexScaleXZ.get(),
	                      mSaveParams->mSLVortexScaleY.get(),
	                      mSaveParams->mSLVortexScaleXZ.get());
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBossEel::invalidateAllCollision()
{
	mBodyCollision->onHitFlag(HIT_FLAG_NO_COLLISION);
	mBarrierCollision->onHitFlag(HIT_FLAG_NO_COLLISION);
	mVortex->onHitFlag(HIT_FLAG_NO_COLLISION);
	mHeadCollision->onHitFlag(HIT_FLAG_NO_COLLISION);
	mAwaCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBossEel::collideToMario()
{
	JGeometry::TVec3<f32> marioTarget = *gpMarioPos;
	JGeometry::TVec3<f32> correction(0.0f, 0.0f, 0.0f);

	for (s32 i = 0; i < 2; ++i) {
		MtxPtr collisionMtx
		    = mMActor->getModel()->getAnmMtx(mMapCollisionJointIndices[i]);
		JGeometry::TVec3<f32> center(collisionMtx[0][3], collisionMtx[1][3],
		                             collisionMtx[2][3]);
		JGeometry::TVec3<f32> axis;
		axis.x = collisionMtx[0][1];
		axis.y = collisionMtx[1][1];
		axis.z = collisionMtx[2][1];
		if (i == 0)
			axis.negate();
		axis.normalize();

		JGeometry::TVec3<f32> distance = marioTarget;
		distance.sub(center);
		f32 length = distance.length();
		if (length == 0.0f || length >= mMouthOpenAmount)
			continue;

		f32 penetration = mMouthOpenAmount - length;

		f32 axialDistance = distance.dot(axis);
		if (axialDistance >= 0.0f)
			continue;

		JGeometry::TVec3<f32> push;
		if (-axialDistance < penetration) {
			push = axis;
			push.scale(-axialDistance);
		} else {
			push = distance;
			push.normalize();
			push.scale(penetration);
		}
		correction.add(push);
	}

	marioTarget.add(correction);
	SMS_MarioMoveRequest(marioTarget);
}

void TBossEel::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if ((cue & CUE_CALC_ANIM)
	    && mSpine->getCurrentNerve() == &TNerveBossEelDie::theNerve()) {
		mDeathEffectPosition.set(0.0f, -14000.0f, 0.0f);
		gpMarioParticleManager->emit(BOSSEEL_JPA_MS_MEO_AWA_DEAD,
		                             &mDeathEffectPosition, 1, this);
	}

	if (mMActor->checkCurBckFromIndex(4))
		mAwaCollision->perform(cue, graphics);

	if (checkLiveFlag(LIVE_FLAG_UNK200))
		return;

	if (cue & CUE_MOVE) {
		mScaling.x = mScaling.y = mScaling.z = mSaveParams->mSLBodyScale.get();
		mPosition.y
		    = mAppearOffset
		      + (mInitialPosition.y + mSaveParams->mSLInitTransYOffset.get());

		TCubeGeneralInfo* mouthCube
		    = *mMouthCubeManager->unk14->getChildren().begin();
		mouthCube->unkC.set(mPosition.x, mPosition.y + 9600.0f * mScaling.y,
		                    mPosition.z);
		mouthCube->unk24.set(7000.0f * mScaling.x, 10000.0f * mScaling.y,
		                     7000.0f * mScaling.z);
		moveObject();

		mMActor->calcAnm();
		calcRootMatrix();
		collideToMario();

		TPosition3f collisionMtx;
		collisionMtx.set(
		    mMActor->getModel()->getAnmMtx(mMapCollisionJointIndices[0]));
		mMapCollisions[2]->moveMtx(collisionMtx);

		if (mUseMapCollision) {
			for (s32 i = 0; i < 2; ++i) {
				TPosition3f collisionMtx;
				collisionMtx.set(mMActor->getModel()->getAnmMtx(
				    mMapCollisionJointIndices[i]));
				mMapCollisions[i]->moveMtx(collisionMtx);
			}
		}

		if (mUseObjCollision
		    && mSpine->getCurrentNerve() != &TNerveBossEelDie::theNerve())
			mBodyCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
		else
			mBodyCollision->onHitFlag(HIT_FLAG_NO_COLLISION);

		mHeadCollision->mPosition = mPosition;
		f32 bodyToHeadDistance    = mSaveParams->mSLBodyToHeadDistance.get();
		bodyToHeadDistance *= mScaling.y;
		mHeadCollision->mPosition.y += bodyToHeadDistance;

		mHeadCollision->setHitParams(
		    mSaveParams->mSLHeadAttackRadius.get() * mScaling.x,
		    mSaveParams->mSLHeadAttackHeight.get() * mScaling.x,
		    mSaveParams->mSLHeadDamageRadius.get() * mScaling.x,
		    mSaveParams->mSLHeadDamageHeight.get() * mScaling.x);

		setHitParams(mSaveParams->mSLBodyAttackRadius.get() * mScaling.x,
		             mSaveParams->mSLBodyAttackHeight.get() * mScaling.x,
		             mSaveParams->mSLBodyDamageRadius.get() * mScaling.x,
		             mSaveParams->mSLBodyDamageHeight.get() * mScaling.x);

		calcAndSetCollisionCubeBite_();

		if (mHitPoints != 0) {
			mForceEat = false;
			for (s32 i = 0; i < mHeadCollision->getColNum(); ++i) {
				if (mHeadCollision->getCollision(i)->isActorType(0x80000001))
					mForceEat = true;
			}

			if (mSpine->getCurrentNerve() != &TNerveBossEelFirstSpin::theNerve()
			    && mSpine->getCurrentNerve()
			           != &TNerveBossEelSecondSpin::theNerve()
			    && mSpine->getCurrentNerve()
			           != &TNerveBossEelMouthOpenWait::theNerve()
			    && (mMActor->checkCurBckFromIndex(10)
			        || mMActor->checkCurBckFromIndex(16)
			        || mMActor->checkCurBckFromIndex(18)
			        || mMActor->checkCurBckFromIndex(19)))
				updateTearsCnt();

			mBckBlendRatio = MsClamp(mBckBlendRatio - 0.01f, 0.0f, 1.0f);
			mMActor->setMotionBlendRatioForBck(mBckBlendRatio);
		}

		deadCheck();
	}

	if (cue & CUE_CALC_ANIM) {
		offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		updateAnmSound();

		if (mSpine->getCurrentNerve() == &TNerveBossEelSecondSpin::theNerve()
		    || mSpine->getCurrentNerve()
		           == &TNerveBossEelFirstSpin::theNerve()) {
			MtxPtr breathMtx = mMActor->getModel()->getAnmMtx(5);
			mBreathParticlePosition.set(breathMtx[0][3], breathMtx[1][3],
			                            breathMtx[2][3]);

			JPABaseEmitter* emitter;
			if (checkLiveFlag(LIVE_FLAG_UNK10000))
				emitter = gpMarioParticleManager->emit(
				    BOSSEEL_JPA_MS_MEO_SPIN_SMOKE, &mBreathParticlePosition, 1,
				    this);
			else
				emitter = gpMarioParticleManager->emit(
				    BOSSEEL_JPA_MS_MEO_SPIN_SMOKE_L, &mBreathParticlePosition,
				    1, this);
			if (emitter)
				emitter->setGlobalScale(mScaling);

			if (checkLiveFlag(LIVE_FLAG_UNK10000))
				emitter = gpMarioParticleManager->emit(
				    BOSSEEL_JPA_MS_MEO_SPIN_AWA, &mBreathParticlePosition, 1,
				    this);
			else
				emitter = gpMarioParticleManager->emit(
				    BOSSEEL_JPA_MS_MEO_SPIN_AWA_L, &mBreathParticlePosition, 1,
				    this);
			if (emitter)
				emitter->setGlobalScale(mScaling);
		}

		if (mMActor->checkCurBckFromIndex(15)) {
			J3DFrameCtrl* frameCtrl = mMActor->getFrameCtrl(0);
			MtxPtr breathMtx        = mMActor->getModel()->getAnmMtx(5);
			mBreathParticlePosition.set(breathMtx[0][3], breathMtx[1][3],
			                            breathMtx[2][3]);
			if (frameCtrl->checkPass(102.0f)) {
				JPABaseEmitter* emitter
				    = gpMarioParticleManager->emitAndBindToPosPtr(
				        BOSSEEL_JPA_MS_MEO_AWA_MOUTH, &mBreathParticlePosition,
				        0, nullptr);
				if (emitter)
					emitter->setGlobalScale(mScaling);
			}
			if (frameCtrl->getFrame() < 40.0f) {
				JPABaseEmitter* emitter = gpMarioParticleManager->emit(
				    BOSSEEL_JPA_MS_MEO_AWA_BODY, &mBreathParticlePosition, 1,
				    this);
				if (emitter)
					emitter->setGlobalScale(mScaling);
			}
		}

		if (mMActor->checkCurBckFromIndex(14)) {
			J3DFrameCtrl* frameCtrl = mMActor->getFrameCtrl(0);
			MtxPtr breathMtx        = mMActor->getModel()->getAnmMtx(5);
			mBreathParticlePosition.set(breathMtx[0][3], breathMtx[1][3],
			                            breathMtx[2][3]);
			SMSGetMSound()->startSoundActor(MSD_SE_BS_UNG_VACUUM, &mPosition, 0,
			                                nullptr, 0, 4);
			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToPosPtr(
			        BOSSEEL_JPA_MS_MEO_SUIKOMI, &mBreathParticlePosition, 1,
			        this);
			if (emitter) {
				JGeometry::TVec3<f32> scale(3.0f, 3.0f, 3.0f);
				emitter->setGlobalScale(scale);
			}
		}
	}

	if (cue & CUE_CALC_VIEW)
		mMActor->viewCalc();
	if (cue & CUE_ENTRY)
		mMActor->entry();

	for (s32 i = 0; i < 4; ++i)
		mEyes[i]->testPerform(cue, graphics);
	for (s32 i = 0; i < 8; ++i)
		mTeeth[i]->testPerform(cue, graphics);

	mHeartCoin->perform(cue, graphics);
	mVortex->perform(cue, graphics);
	mBodyCollision->perform(cue, graphics);
	mBarrierCollision->perform(cue, graphics);
}

void TBossEel::setBckAnm(int index)
{
	mPreviousBckIndex = getMActor()->getCurAnmIdx(0);
	mCurrentBckIndex  = index;
	mBckBlendRatio    = 1.0f;
	getMActor()->setBckOldMotionBlendAnmPtr(getMActor()->getBckAnm());
	getMActor()->setBckFromIndex(index);
	getMActor()->setMotionBlendRatioForBck(mBckBlendRatio);
	getMActor()->getFrameCtrl(0)->setRate(0.25f * SMSGetAnmFrameRate());
	const char** basTable = getBasNameTable();
	const char* basName;
	if (basTable == nullptr)
		basName = nullptr;
	else
		basName = basTable[index];
	setAnmSound(basName);
}

void TBossEel::deadCheck()
{
	for (s32 i = 0; i < 8; ++i)
		if (mTeeth[i]->mHitPoints > 1)
			return;

	if (mSpine->getCurrentNerve() != &TNerveBossEelDie::theNerve()) {
		mSpine->setNext(&TNerveBossEelDie::theNerve());
		mHeadCollision->onHitFlag(HIT_FLAG_NO_COLLISION);
	}
}

BOOL TBossEel::isValidToothDamage()
{
	if (mSpine->getCurrentNerve() == &TNerveBossEelEat::theNerve())
		return false;

	return true;
}

// TODO: size is wrong but it feels very right
void TBossEel::quickBack()
{
	s32 i;
	TBossEelTooth** teeth = mTeeth;
	for (i = 0; i < 8; ++i, ++teeth) {
		TBossEelTooth* tooth = *teeth;

		if (tooth && tooth->mHitPoints > 1) {
			tooth->mHitPoints
			    = tooth->mOwner->mSaveParams->mSLToothMaxHitPoint.get();
			tooth->mColor.a = 0xFF;
		}
	}
}

bool TBossEel::isInBossEelMoguDemo()
{
	if (mMActor->checkCurBckFromIndex(12) || mMActor->checkCurBckFromIndex(3))
		return true;
	return false;
}

static s32 hoseiDiveCameraCallback(u32 actorAddress, u32 state)
{
	if (state == 1) {
		const TLiveActor* actor
		    = reinterpret_cast<const TLiveActor*>(actorAddress);
		JGeometry::TVec3<f32> position = actor->mPosition;
		position.y += 12300.0f;
		gpCamera->warpPosAndAt(position, *gpMarioPos);
	}
	return 0;
}

void TBossEel::startMoguCamera()
{
	if (!mMoguCameraActive) {
		gpMarDirector->getConsole()->startAppearBalloon(0xE0015, true);
		SMSGetMarDirector()->fireStartDemoCamera(
		    "meoto_mogu_camera", &mPosition, -1, 0.0f, false,
		    &hoseiDiveCameraCallback, reinterpret_cast<u32>(this), nullptr,
		    JDrama::TFlagT<u16>(0));
		mMoguCameraActive = true;
	}
}

const char** TBossEel::getBasNameTable() const { return bosseel_bastable; }

DEFINE_NERVE(TNerveBossEelWaitAppear, TLiveActor)
{
	TBossEel* eel = static_cast<TBossEel*>(spine->getBody());
	if (spine->getTime() == 0)
		eel->setBckAnm(10);

	if (spine->getTime() == 2500)
		gpMarDirector->getConsole()->startAppearBalloon(0xE0012, true);

	JGeometry::TVec3<f32> marioPosition = *gpMarioPos;
	marioPosition.y += 75.0f;
	if (eel->mMouthCubeManager->isInCube(marioPosition, (s32)0)) {
		spine->pushAfterCurrent(&TNerveBossEelFirstSpin::theNerve());
		return true;
	}
	return false;
}

void ExecSpinNerve_Sub(TBossEel* eel)
{
	f32 spinSpeed = eel->mTurnSpeed;
	CLBChaseGeneralConstantSpecifySpeed(&spinSpeed,
	                                    eel->mSaveParams->mSLSpinMaxSpeed.get(),
	                                    eel->mSaveParams->mSLSpinAccel.get());
	eel->mTurnSpeed = spinSpeed;
	gpCameraShake->keepShake(static_cast<EnumCamShakeMode>(0x18), 1.0f);

	if (eel->checkLiveFlag(TBossEel::LIVE_FLAG_UNK10000)) {
		eel->mRotation.y -= spinSpeed;
		if (eel->mRotation.y <= 0.0f)
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_BS_UNG_ROLL, &eel->mPosition, nullptr, spinSpeed, 0, 0,
			    nullptr, 0, 4);
	} else {
		eel->mRotation.y += spinSpeed;
		if (eel->mRotation.y >= 360.0f)
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_BS_UNG_ROLL, &eel->mPosition, nullptr, spinSpeed, 0, 0,
			    nullptr, 0, 4);
	}
	eel->mRotation.y = MsWrap(eel->mRotation.y, 0.0f, 360.0f);
}

DEFINE_NERVE(TNerveBossEelFirstSpin, TLiveActor)
{
	TBossEel* eel = static_cast<TBossEel*>(spine->getBody());
	if (spine->getTime() == 0) {
		eel->mTurnSpeed = 0.0f;
		SMSGetMSound()->startSoundActorWithInfo(MSD_SE_BS_UNG_ROLL,
		                                        &eel->mPosition, nullptr, 2.0f,
		                                        0, 0, nullptr, 0, 4);
		eel->setBckAnm(10);
		if (MsRandF() < 0.5f)
			eel->onLiveFlag(TBossEel::LIVE_FLAG_UNK10000);
		else
			eel->offLiveFlag(TBossEel::LIVE_FLAG_UNK10000);
	}

	ExecSpinNerve_Sub(eel);

	if (spine->getTime() >= 360) {
		spine->pushAfterCurrent(&TNerveBossEelAppear::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBossEelSecondSpin, TLiveActor)
{
	TBossEel* eel = static_cast<TBossEel*>(spine->getBody());
	if (spine->getTime() == 0) {
		eel->setBckAnm(10);
		JGeometry::TVec2<s32>* spinTimer = eel->mSpinTimer;
		spinTimer->x                     = 0;
		spinTimer->y    = static_cast<s32>(MsRandF() * 960.0f) + 241;
		eel->mTurnSpeed = 0.0f;
		SMSGetMSound()->startSoundActorWithInfo(MSD_SE_BS_UNG_ROLL,
		                                        &eel->mPosition, nullptr, 2.0f,
		                                        0, 0, nullptr, 0, 4);
		if (MsRandF() < 0.5f)
			eel->onLiveFlag(TBossEel::LIVE_FLAG_UNK10000);
		else
			eel->offLiveFlag(TBossEel::LIVE_FLAG_UNK10000);
	}

	ExecSpinNerve_Sub(eel);

	bool atLimit                     = false;
	JGeometry::TVec2<s32>* spinTimer = eel->mSpinTimer;
	++spinTimer->x;
	if (spinTimer->x >= spinTimer->y) {
		spinTimer->x = spinTimer->y;
		atLimit      = true;
	}
	if (atLimit) {
		spine->pushAfterCurrent(&TNerveBossEelAppear::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBossEelAppear, TLiveActor)
{
	TBossEel* eel = static_cast<TBossEel*>(spine->getBody());
	if (spine->getTime() == 0) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_UNG_UP, &eel->mPosition, 0,
		                                nullptr, 0, 4);
		eel->mBarrierCollision->offHitFlag(HIT_FLAG_NO_COLLISION);
		eel->setBckAnm(15);
		eel->mInDemo = true;
		gpCameraShake->startShake(static_cast<EnumCamShakeMode>(0x19), 1.0f);
		f32 duration = eel->mMActor->getFrameCtrl(0)->getEnd() * 2;
		eel->mAppearAcceleration
		    = (eel->mSaveParams->mSLAppearMoveDistY.get() / duration)
		      / duration;
		testHeight = 0.0f;
	}

	s32 duration = eel->mMActor->getFrameCtrl(0)->getEnd() * 2;
	if (spine->getTime() < duration) {
		eel->mAppearOffset
		    += eel->mSaveParams->mSLAppearMoveDistY.get() / duration;

		f32 sp = eel->mTurnSpeed *= 0.98f;

		if (eel->checkLiveFlag(TBossEel::LIVE_FLAG_UNK10000))
			eel->mRotation.y -= sp;
		else
			eel->mRotation.y += sp;
		eel->mRotation.y = MsWrap(eel->mRotation.y, 0.0f, 360.0f);
	} else if (eel->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveBossEelOutWait::theNerve());
		eel->mBarrierCollision->onHitFlag(HIT_FLAG_NO_COLLISION);
		if (eel->mCollisionEnabled) {
			eel->mCollisionEnabled = false;
			gpMarDirector->getConsole()->startAppearBalloon(0xE0013, true);
		}
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBossEelOutWait, TLiveActor)
{
	TBossEel* eel = static_cast<TBossEel*>(spine->getBody());
	++eel->mBattleTimer;
	if (spine->getTime() == 0) {
		eel->mToothDamaged = false;
		eel->setBckAnm(16);
	}

	if (eel->checkCurAnmEnd(0)) {
		s32 r28 = eel->mSaveParams->mSLMouthOpenInterval.get();

		if (eel->getMActor()->checkCurBckFromIndex(19)) {
			spine->setNext(&TNerveBossEelQuickBack::theNerve());
		} else {
			if (eel->mBattleTimer >= 3600) {
				spine->pushAfterCurrent(&TNerveBossEelSlowBack::theNerve());
				eel->mBattleTimer = 0;
				return true;
			}

			if (spine->getTime() > r28) {
				spine->pushAfterCurrent(&TNerveBossEelOutWait::theNerve());
				spine->pushAfterCurrent(
				    &TNerveBossEelMouthOpenWait::theNerve());
				return true;
			}

			if (eel->mToothBroken) {
				eel->setBckAnm(19);
			} else if (eel->mToothDamaged) {
				eel->mToothDamaged = false;
				eel->setBckAnm(18);
			} else {
				eel->setBckAnm(16);
			}
		}
	}
	return false;
}

static BOOL ExecBackNerve_Sub(TSpineBase<TLiveActor>* spine, f32 speed)
{
	TBossEel* eel = static_cast<TBossEel*>(spine->getBody());
	if (spine->getTime() == 1) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_UNG_DOWN, &eel->mPosition, 0,
		                                nullptr, 0, 4);
		if (eel->mToothBroken) {
			eel->mToothBroken = false;
			eel->setBckAnm(6);
			gpCameraShake->startShake(static_cast<EnumCamShakeMode>(0x1A),
			                          1.0f);
		} else {
			eel->setBckAnm(9);
		}
	}

	eel->mAppearOffset -= eel->mSaveParams->mSLAppearMoveDistY.get()
	                      / (eel->mMActor->getFrameCtrl(0)->getEnd() * 2);
	if (eel->mAppearOffset < 0.0f) {
		eel->mAppearOffset = 0.0f;
		if (eel->checkCurAnmEnd(0)) {
			eel->mInDemo = false;
			spine->pushAfterCurrent(&TNerveBossEelSecondSpin::theNerve());
			if (eel->mMActor->checkCurBckFromIndex(6))
				spine->pushAfterCurrent(
				    &TNerveBossEelSleepOnBottom::theNerve());
			return true;
		}
	}
	return false;
}

DEFINE_NERVE(TNerveBossEelSlowBack, TLiveActor)
{
	return ExecBackNerve_Sub(spine, 10.0f);
}

DEFINE_NERVE(TNerveBossEelQuickBack, TLiveActor)
{
	if (ExecBackNerve_Sub(spine, 40.0f)) {
		TBossEel* eel = static_cast<TBossEel*>(spine->getBody());
		eel->quickBack();
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBossEelEat, TLiveActor)
{
	TBossEel* eel = static_cast<TBossEel*>(spine->getBody());
	if (spine->getTime() == 0)
		eel->setBckAnm(17);

	if (eel->checkCurAnmEnd(0)) {
		if (eel->mMActor->checkCurBckFromIndex(17)) {
			if (eel->canEatMario()) {
				eel->setBckAnm(12);
				if (SMS_SendMessageToMario(eel, 4)) {
					eel->mHeldObject
					    = reinterpret_cast<TTakeActor*>(SMS_GetMarioHitActor());
					eel->startMoguCamera();
				}
			} else {
				eel->setBckAnm(5);
				MtxPtr breathMtx = eel->getMActor()->getModel()->getAnmMtx(5);
				eel->mBreathParticlePosition.set(
				    breathMtx[0][3], breathMtx[1][3], breathMtx[2][3]);
				JPABaseEmitter* emitter
				    = gpMarioParticleManager->emitAndBindToPosPtr(
				        BOSSEEL_JPA_MS_MEO_AWA_MOUTH,
				        &eel->mBreathParticlePosition, 0, nullptr);
				if (emitter)
					emitter->setGlobalScale(eel->mScaling);
			}
		} else if (eel->mMActor->checkCurBckFromIndex(12)) {
			if (SMS_SendMessageToMario(eel, 8)) {
				eel->mHeldObject = nullptr;
				SMS_SendMessageToMario(eel, 14);
				gpMarDirector->fireEndDemoCamera();
				eel->mMoguCameraActive = false;
			}
			spine->reset();
			spine->setDefaultNext();
			spine->pushAfterCurrent(&TNerveBossEelQuickBack::theNerve());
			return true;
		}
	}

	if (eel->mMActor->checkCurBckFromIndex(12)) {
		if (eel->mMActor->getFrameCtrl(0)->getFrame() < 250.0f)
			SMSRumbleMgr->start(8, &eel->mPosition);
		else
			SMSRumbleMgr->start(20, 10, static_cast<f32*>(nullptr));
		gpMarioOriginal->mGamePad->onNeutralMarioKey();
		if (SMS_SendMessageToMario(eel, HIT_MESSAGE_TAKE)) {
			eel->mHeldObject
			    = reinterpret_cast<TTakeActor*>(SMS_GetMarioHitActor());
			eel->startMoguCamera();
		}
	}
	return false;
}

DEFINE_NERVE(TNerveBossEelDie, TLiveActor)
{
	TBossEel* eel = static_cast<TBossEel*>(spine->getBody());
	if (spine->getTime() == 0) {
		SMSGetMSound()->startSoundActor(MSD_SE_BS_UNG_VOICE_LAST,
		                                &eel->mPosition, 0, nullptr, 0, 4);
		gpMarDirector->getConsole()->startAppearBalloon(0xE0014, true);
		MSBgm::stopTrackBGMs(7, 10);
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK1E, 1.0f);
		eel->setBckAnm(3);
		eel->invalidateAllCollision();
		eel->mHeartCoin->generate(eel->mPosition);
		if (SMS_SendMessageToMario(eel, 4)) {
			eel->mHeldObject
			    = reinterpret_cast<TTakeActor*>(SMS_GetMarioHitActor());
			SMSGetMarDirector()->fireStartDemoCamera(
			    "meoto_end_camera", &eel->mPosition, -1, 0.0f, false, nullptr,
			    0, nullptr, JDrama::TFlagT<u16>(0));
		}
	}

	if (eel->mMActor->checkCurBckFromIndex(3)) {
		if (eel->mMActor->getFrameCtrl(0)->checkPass(650.0f))
			eel->mHeartCoin->getMActor()->setBckFromIndex(8);

		if (eel->checkCurAnmEnd(0)) {
			if (SMS_SendMessageToMario(eel, 8)) {
				eel->mHeldObject = nullptr;
				SMS_SendMessageToMario(eel, 14);
				gpMarDirector->fireEndDemoCamera();
			}

			JGeometry::TVec3<f32> marioPosition = *gpMarioPos;
			eel->generateBubble(marioPosition);

			s32 jointIndex = eel->mMActor->getModel()
			                     ->getModelData()
			                     ->getJointName()
			                     ->getIndex("ha7");
			MtxPtr shineMtx = eel->mMActor->getModel()->getAnmMtx(jointIndex);
			gpItemManager->makeShineAppearWithDemo(
			    "シャイン（ボス用）", "めおとウナギシャインカメラ",
			    shineMtx[0][3], shineMtx[1][3], shineMtx[2][3]);

			eel->mTeeth[6]->mHitPoints = 0;
			eel->mTeeth[6]->onHitFlag(HIT_FLAG_NO_COLLISION);
			eel->setBckAnm(4);
		}
	}
	return false;
}

DEFINE_NERVE(TNerveBossEelMouthOpenWait, TLiveActor)
{
	TBossEel* eel = static_cast<TBossEel*>(spine->getBody());
	if (spine->getTime() == 0) {
		if (eel->mToothBroken)
			eel->mToothBroken = false;
		eel->setBckAnm(13);
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK1B, 1.0f);
	} else if (eel->checkCurAnmEnd(0)) {
		if (eel->mMActor->checkCurBckFromIndex(13)) {
			eel->setBckAnm(14);
			gpCameraShake->startShake(CAM_SHAKE_MODE_UNK1C, 1.0f);
			eel->generateVortex();
		} else {
			s32 openFrames = eel->mSaveParams->mSLMouthOpenFrame.get();
			if (spine->getTime()
			    > openFrames - eel->mSaveParams->mSLCanEatFrame.get()) {
				if (eel->canEatMario()) {
					spine->pushAfterCurrent(&TNerveBossEelEat::theNerve());
					return true;
				}
			}

			if (spine->getTime() > openFrames) {
				if (eel->mMActor->checkCurBckFromIndex(2))
					return true;
				gpCameraShake->startShake(CAM_SHAKE_MODE_UNK1D, 1.0f);
				eel->setBckAnm(2);
			}
		}
	}

	if (eel->mMActor->checkCurBckFromIndex(14))
		eel->mRotation.y += TBossEel::mOpenRollSpeed;
	return false;
}

DEFINE_NERVE(TNerveBossEelSleepOnBottom, TLiveActor)
{
	TBossEel* eel = static_cast<TBossEel*>(spine->getBody());

	if (spine->getTime() == 0) {
		eel->mBattleTimer = 0;
		eel->setBckAnm(10);
	}

	if (spine->getTime() % 100 == 1)
		eel->forceShedTears(spine->getTime() % 2);

	if (spine->getTime() > 1000 && eel->checkCurAnmEnd(0))
		return true;

	return false;
}
