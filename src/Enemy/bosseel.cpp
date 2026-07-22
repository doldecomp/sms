
#include <Enemy/BossEel.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/SharedParts.hpp>
#include <Strategic/Strategy.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
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

TBEelTears::TBEelTears(const char* name)
    : TSpineEnemy(name)
    , mTearsParams(nullptr)
    , mHighPoly(true)
    , mStateTimer(0)
    , mDrop(nullptr)
    , mTearsState(0)
{
}
