#include <Enemy/DemoBossHanachanBase.hpp>
#include <Map/Map.hpp>
#include <Strategic/ObjModel.hpp>
#include <M3DUtil/MActor.hpp>

#include <M3DUtil/InfectiousStrings.hpp>

TDemoBossHanachanSaveParams::TDemoBossHanachanSaveParams(const char* prm)
    : TParams(prm)
    , PARAM_INIT(mSLViewClipFar, 25000.0f)
    , PARAM_INIT(mSLViewClipRadius, 3500.0f)
{
	TParams::load(mPrmPath);
}

void TDemoBossHanachanManager::clipEnemies(JDrama::TGraphics* graphics)
{
	clipActorsAux(graphics, mSaveParams->mSLViewClipFar.get(),
	              mSaveParams->mSLViewClipRadius.get());
}

void TDemoBossHanachan::initBase(TLiveManager* manager, u32 model_flags)
{
	mManager = manager;
	manager->manageActor(this);

	mMActorKeeper = new TMActorKeeper(manager);
	mMActor       = mMActorKeeper->createMActorFromNthData(0, model_flags);

	mBodyScale        = 1.0f;
	mBodyRadius       = 280.0f;
	mWallRadius       = mBodyRadius;
	mHeadHeight       = 200.0f;
	mMarchSpeed       = 0.0f;
	mScaledBodyRadius = 1000.0f;
	onLiveFlag(LIVE_FLAG_UNK8 | LIVE_FLAG_UNK10);

	mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
	    mPosition.x, mPosition.y + getHeadHeight(), mPosition.z, &mGroundPlane);
	mMActor->setLightType(1);
}

BOOL TDemoBossHanachan::receiveMessage(THitActor* sender, u32 message)
{
	return FALSE;
}
