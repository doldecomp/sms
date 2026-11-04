#include <Map/MapEventSirena.hpp>
#include <Map/PollutionManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CameraMapTool.hpp>
#include <System/MarDirector.hpp>
#include <System/FlagManager.hpp>
#include <System/Particles.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

// rogue includes
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Map/MapCollisionEntry.hpp>

bool TMapEventSirenaSink::watch()
{
	if (mRiseRequested) {
		gpPollution->getLayer(0)->startDecay();
		mRaisingBuildingIdx = 0;
		SMSGetMarDirector()->fireStartDemoCamera(
		    "ホテル上げカメラ", &mShineSpawnPos, -1, 0.0f, true, nullptr, 0,
		    nullptr, JDrama::TFlagT<u16>(0));
		gpItemManager->makeShineAppearWithDemo(
		    "シャイン（ホテル上げ用）", "ホテル上げシャインカメラ",
		    mShineSpawnPos.x, mShineSpawnPos.y, mShineSpawnPos.z);
		TFlagManager::getInstance()->setBool(true, MSF_HOTEL_RISING);
		SMS_MarioWarpRequest(mMarioWarpPosition, mMarioWarpAngle);
		gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_HOTEL_A,
		                             &gpMapObjManager->getUnk44(), 0, nullptr);
		gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_HOTEL_B,
		                             &gpMapObjManager->getUnk44(), 2, nullptr);

		return true;
	}

	return false;
}

void TMapEventSirenaSink::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	mDemoDuration = static_cast<TCameraMapTool*>(
	                    JDrama::TNameRefGen::search("ホテル上げカメラ"))
	                    ->getDemoLengthFrames();
	mRiseStartDelay    = 240;
	mRiseEndDelay      = 240;
	mSinkDepthOverride = 3500.0f;
	mShineSpawnPos.x   = 0.0f;
	mShineSpawnPos.y   = 3300.0f;
	mShineSpawnPos.z   = -2570.0f;
}

void TMapEventSirenaSink::load(JSUMemoryInputStream& stream)
{
	TMapEventSink::load(stream);
	stream.readString();
	stream >> mMarioWarpPosition.x >> mMarioWarpPosition.y
	    >> mMarioWarpPosition.z;
	f32 unused;
	stream >> unused;
	stream >> mMarioWarpAngle;
	SMS_LoadParticle("/scene/map/map/ms_objup_hotel_a.jpa",
	                 MAP_MAP_MS_OBJUP_HOTEL_A);
	SMS_LoadParticle("/scene/map/map/ms_objup_hotel_b.jpa",
	                 MAP_MAP_MS_OBJUP_HOTEL_B);
}

TMapEventSirenaSink::TMapEventSirenaSink(const char* name)
    : TMapEventSink(name)
{
	mRiseRequested  = false;
	mMarioWarpAngle = 0.0f;
	mShineSpawnPos.zero();
	mMarioWarpPosition.zero();
}
