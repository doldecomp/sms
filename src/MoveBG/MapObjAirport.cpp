#include <MoveBG/MapObjAirport.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Map/PollutionManager.hpp>
#include <Camera/CameraShake.hpp>
#include <System/MarDirector.hpp>
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
	    0, 0, getPollutionObj(unk28));
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
		gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 0.1);

	return TMapEventSinkInPollutionReset::control();
}

// TODO: this is all fake, need to analyze a bunch of similar functions together
// and figure out the real inlines
inline TMarDirector* getMarDirector() { return gpMarDirector; }
inline TPollutionManager* getPollution() { return gpPollution; }

bool TAirportEventSink::watch()
{
	if (!unk54[0] && unk6C->checkLiveFlag(0x1)) {
		unk28                  = 0;
		TMarDirector* director = getMarDirector();

		director->fireStartDemoCamera("空港坂上げカメラ", &unk6C->mPosition, -1,
		                              0.0f, true, nullptr, 0, nullptr,
		                              JDrama::TFlagT<u16>(0));
		getPollution()->getLayer(0)->onUnk32(0x2);
		return true;
	}

	return false;
}

void TAirportEventSink::loadAfter()
{
	TMapEventSinkInPollutionReset::loadAfter();

	unk6C
	    = JDrama::TNameRefGen::search<TLiveActor>("ゲートキーパー（ビアンコ）");
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
