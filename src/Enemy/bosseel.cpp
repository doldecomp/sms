
#include <Enemy/BossEel.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <Map/Map.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/SharedParts.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/Application.hpp>
#include <System/Particles.hpp>
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

void TBEelTears::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("tears.bmd", 0);
	mMActorKeeper->createMActor("tears_waterhit.bmd", 0);
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

const char** TBEelTears::getBasNameTable() const
{
	static const char* bossEelTears_bastable[]
	    = { nullptr, nullptr, nullptr, nullptr };
	return bossEelTears_bastable;
}
