#include <Enemy/DemoBossHanachan.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/Map.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/BaseParam.hpp>
#include <System/ParamInst.hpp>
#include <System/Params.hpp>

TDemoBossHanachanSaveParams::TDemoBossHanachanSaveParams(const char* param_1)
    : TParams(param_1)
    , PARAM_INIT(mSLViewClipFar, 25000.0f)
    , PARAM_INIT(mSLViewClipRadius, 3500.0f)
{
	load(mPrmPath);
}

void TDemoBossHanachanManager::clipEnemies(JDrama::TGraphics* param_1)
{
	clipActorsAux(param_1, mSaveParams->mSLViewClipFar.get(),
	              mSaveParams->mSLViewClipRadius.get());
}

void TDemoBossHanachan::initBase(TLiveManager* param_1, u32 param_2)
{
	mManager = param_1;
	param_1->manageActor(this);

	mMActorKeeper = new TMActorKeeper(param_1);
	mMActor       = mMActorKeeper->createMActorFromNthData(0, param_2);

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

BOOL TDemoBossHanachan::receiveMessage(THitActor* param_1, u32 param_2)
{
	return 0;
}
