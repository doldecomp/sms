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

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static void dummy(Vec* v)
{
	*v = (Vec) { 0.0f, 0.0f, 0.0f };
	*v = (Vec) { 1.0f, 1.0f, 1.0f };
}

// Exact. Three inline levels pay for the frame: the named
// `TFlagManager* flagManager` before `setBool`, and the two that
// `SMSGetPollutionLayer` adds over `getLayer(i)` (see PollutionManager.hpp --
// this site and TAirportEventSink::watch are the only two that read the layer
// through `SMSGetPollution()->getLayers()[i]`). Worth zero here: own-class
// accessors for unk64/unk68, an extra level inside TPollutionLayer::startDecay
// or TPollutionManager::getLayer, and a named layer local. Worse: `&getUnk68()`
// (97.8%) and accessors on the warp arguments (97.0%).
bool TMapEventSirenaSink::watch()
{
	if (unk64) {
		SMSGetPollutionLayer(0)->startDecay();
		mRaisingBuildingIdx = 0;
		SMSGetMarDirector()->fireStartDemoCamera(
		    "ホテル上げカメラ", &unk68, -1, 0.0f, true, nullptr, 0, nullptr,
		    JDrama::TFlagT<u16>(0));
		gpItemManager->makeShineAppearWithDemo("シャイン（ホテル上げ用）",
		                                       "ホテル上げシャインカメラ",
		                                       unk68.x, unk68.y, unk68.z);
		TFlagManager* flagManager = TFlagManager::getInstance();
		flagManager->setBool(true, 0x50008);
		SMS_MarioWarpRequest(unk74, unk80);
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
	unk40 = JDrama::TNameRefGen::search<TCameraMapTool>("ホテル上げカメラ")
	            ->getDemoLengthFrames();
	unk44   = 240;
	unk48   = 240;
	unk38   = 3500.0f;
	unk68.x = 0.0f;
	unk68.y = 3300.0f;
	unk68.z = -2570.0f;
}

void TMapEventSirenaSink::load(JSUMemoryInputStream& stream)
{
	TMapEventSink::load(stream);
	stream.readString();
	stream >> unk74.x >> unk74.y >> unk74.z;
	f32 unused;
	stream >> unused;
	stream >> unk80;
	SMS_LoadParticle("/scene/map/map/ms_objup_hotel_a.jpa",
	                 MAP_MAP_MS_OBJUP_HOTEL_A);
	SMS_LoadParticle("/scene/map/map/ms_objup_hotel_b.jpa",
	                 MAP_MAP_MS_OBJUP_HOTEL_B);
}

TMapEventSirenaSink::TMapEventSirenaSink(const char* name)
    : TMapEventSink(name)
{
	unk64 = false;
	unk80 = 0.0f;
	unk68.zero();
	unk74.zero();
}
