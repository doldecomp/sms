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

// TODO: instruction-exact, frame-only residue. Retail's frame is 0x40 with the
// TFlagT temporary at 0x3c; the named `flags` local below gives 0x40 with the
// temporary at 0x34, and the unnamed `JDrama::TFlagT<u16>(0)` form every other
// caller uses gives 0x38/0x30. Either way the low (inline-expansion) region is
// 12 bytes short, and named locals are allocated *above* the temporary (an
// uninitialised TVec3 anywhere in the body only grows the frame), so the 12
// bytes are three missing accessor expansions, not a recovered local.
// Measured temp-slot ladder for this function (target 0x3c):
//   `gpPollution->getLayer(0)->startDecay()` is worth 24 bytes of temporaries
//   (raw `mJointModels[0]` + raw `mFlags |=` is 4, adding `startDecay()` +4,
//   adding `getJointModel()` +12, adding `getLayer()` +4); `SMSGetMSound()`
//   over `gpMSound` +4; spelling the MSound wrapper out as
//   `if (gateCheck()) MSoundSE::startSoundSystemSE()` -4; an extra same-class
//   accessor level inside `getJointModel` +4; `SMSGetMarDirector()` over
//   `gpMarDirector`, a `setRaisingBuildingIdx()` setter and an indexed
//   `isBuildingRecovered(0)` accessor are all worth 0. A `getGateKeeper()`
//   accessor on this class is +4 and saturates.
//   `&unk6C->getPosition()` is +8 but CSEs the two `unk6C` loads retail keeps.
// TMapEventSirenaSink::watch is 12 short in the same region, and
// TMapEventSinkInPollution::watch / TMapEventSinkBianco::watch are 24 short,
// so the missing levels are most likely in the shared pollution accessors.
// Confirmed on this function and Sirena's: removing one statement at a time
// leaves the whole residue in `gpPollution->getLayer(0)->startDecay()`, and one
// extra 0-param accessor level inside `TJointModelManager::getJointModel`
// (`getJointModels()`, a shared header, so not applied) recovers 8 of the 12
// bytes here and 4 of Sirena's. Two such levels regress both.
bool TAirportEventSink::watch()
{
	if (!mIsBuildingRecovered[0] && unk6C->checkLiveFlag(LIVE_FLAG_DEAD)) {
		mRaisingBuildingIdx = 0;
		JDrama::TFlagT<u16> flags(0);
		SMSGetMarDirector()->fireStartDemoCamera(
		    "空港坂上げカメラ", &unk6C->mPosition, -1, 0.0f, true, nullptr, 0,
		    nullptr, flags);
		gpPollution->getLayer(0)->startDecay();
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
