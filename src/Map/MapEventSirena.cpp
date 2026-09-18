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

// TODO: instruction-exact; the TFlagT temporary passed to fireStartDemoCamera
// sits at 0x38 in a 0x48 frame where the ROM has 0x44 in a 0x50 frame, i.e. the
// low (inline-expansion) region is 12 bytes short. Removing one statement at a
// time shows the whole residue belongs to `gpPollution->getLayer(0)->
// startDecay()`: it is worth 24 bytes of temporaries for us (dropping it moves
// the slot to 0x24) and every other statement here contributes none, so the
// ROM's copy of that chain must be 12 bytes deeper. Measured levers, none of
// them committable: a TU-local `SMSGetPollution()` wrapper over `gpPollution`
// is +4 (and saturates - two or three levels still give +4); one extra 0-param
// accessor level inside `TJointModelManager::getJointModel` (shared header) is
// another +4, and the two together give the right 0x50 frame with the slot at
// 0x40, four bytes short. Worth zero: own-class accessors for unk64/unk68
// (unlike MapObjAirport, where `getGateKeeper()` in the guard is +4), an extra
// level inside `TPollutionLayer::startDecay` or `TPollutionManager::getLayer`,
// and a named layer local. Worse: `&getUnk68()` (97.8%), accessors on the warp
// arguments (97.0%), a TU-local wrapper for the whole `getLayer(i)` chain
// (84.6%) and two or more extra levels inside `getJointModel` (84.6%).
// TAirportEventSink::watch is 12 short in the same chain and
// TMapEventSinkInPollution::watch / TMapEventSinkBianco::watch 24.
// The exact recipe for this function is known: a TU-local `static inline
// TPollutionManager* SMSGetPollution()` over `gpPollution` in the decay
// statement (+4), a named `TFlagManager* flagManager = TFlagManager::
// getInstance();` before `setBool` (+4), and one extra 0-param accessor level
// under `TJointModelManager::getJointModel` (`return getJointModels()[i];`) for
// the last +4. All three together make it byte-exact. The third is a shared
// header and is not committable: globally it takes the source-linked
// `mario/Map/MapEvent` from 100% to 95.9% code and costs `Map/MapEventSink`
// and `Map/PollutionManager`, so it would break the DOL. The open question is
// what adds that one level on the `getLayer(i)` path alone.
bool TMapEventSirenaSink::watch()
{
	if (unk64) {
		gpPollution->getLayer(0)->startDecay();
		mRaisingBuildingIdx = 0;
		SMSGetMarDirector()->fireStartDemoCamera(
		    "ホテル上げカメラ", &unk68, -1, 0.0f, true, nullptr, 0, nullptr,
		    JDrama::TFlagT<u16>(0));
		gpItemManager->makeShineAppearWithDemo("シャイン（ホテル上げ用）",
		                                       "ホテル上げシャインカメラ",
		                                       unk68.x, unk68.y, unk68.z);
		TFlagManager::getInstance()->setBool(true, 0x50008);
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
