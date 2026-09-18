#include <MoveBG/MapObjAirport.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Map/PollutionManager.hpp>
#include <Enemy/GateKeeper.hpp>
#include <Camera/CameraShake.hpp>
#include <System/MarDirector.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TAirportPool::appear() { }

TAirportPool::TAirportPool(const char* name)
    : TPool(name)
{
}

void TAirportSwitch::breaking()
{
	if (animIsFinished())
		unk148->appear();
}

void TAirportSwitch::loadAfter()
{
	TMapObjGeneral::loadAfter();
	unk148 = JDrama::TNameRefGen::search<TAirportPool>("AirportPool");
}

TAirportSwitch::TAirportSwitch(const char* name)
    : TMapObjGeneral(name)
    , unk148(nullptr)
{
}

void TAirportEventSink::finishControl()
{
	gpPollution->getCounterLayer().pushJointObjStampTask(
	    0, 0, getPollutionObj(mRaisingBuildingIdx));
	gpItemManager->makeShineAppearWithDemoOffset(
	    "シャイン（落書き用）", "落書きシャインカメラ", 0.0f, 0.0f, 0.0f);
	gpPollution->offLayer(0);

	TMapEventSinkInPollutionReset::finishControl();
}

bool TAirportEventSink::control()
{
	if (unk4C == unk40 - 300)
		gpPollution->offLayer(0);

	if (unk4C > unk48)
		gpCameraShake->keepShake(CAM_SHAKE_MODE_BUILDING_APPEAR, 0.1);

	return TMapEventSinkInPollutionReset::control();
}

// Exact. The frame is the sum of four inline levels, three of them local (the
// unnamed `JDrama::TFlagT<u16>(0)` temporary every other fireStartDemoCamera
// caller passes, `getGateKeeper()` in the guard, and `SMSGetPollutionLayer`'s
// `SMSGetPollution()` step) and the fourth the pollution-layer lookup itself:
// this site and TMapEventSirenaSink::watch reach the layer two levels below
// `getLayer(i)`, through `SMSGetPollution()->getLayers()[i]`. Substituting
// `getLayer(0)` here loses 8 bytes of expansion temporaries, and pushing the
// extra level down into `TJointModelManager::getJointModel` instead reaches
// every other caller and breaks the source-linked Map/MapEvent.
bool TAirportEventSink::watch()
{
	if (!mIsBuildingRecovered[0]
	    && getGateKeeper()->checkLiveFlag(LIVE_FLAG_DEAD)) {
		mRaisingBuildingIdx = 0;
		SMSGetMarDirector()->fireStartDemoCamera(
		    "空港坂上げカメラ", &unk6C->mPosition, -1, 0.0f, true, nullptr, 0,
		    nullptr, JDrama::TFlagT<u16>(0));
		SMSGetPollutionLayer(0)->startDecay();
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CLEAR_SIGN_BIG, 0, nullptr,
		                                 0);
		return true;
	}

	return false;
}

void TAirportEventSink::loadAfter()
{
	TMapEventSinkInPollutionReset::loadAfter();

	unk6C = JDrama::TNameRefGen::search<TGateKeeperBase>(
	    "ゲートキーパー（ビアンコ）");
	unk40 = 720;
	unk44 = 480;
	unk48 = 60;
	unk38 = 200.0f;
}

TAirportEventSink::TAirportEventSink(const char* name)
    : TMapEventSinkInPollutionReset(name)
    , unk64(0)
    , unk68(360)
{
}
