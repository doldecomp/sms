#include <System/MarDirector.hpp>
#include <System/Application.hpp>
#include <System/MarioGamePad.hpp>
#include <System/DrawSyncManager.hpp>
#include <System/TalkCursor.hpp>
#include <System/MSoundMainSide.hpp>
#include <System/PerformList.hpp>
#include <System/StageEventInfo.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapEventSink.hpp>
#include <Player/Mario.hpp>
#include <Camera/SunMgr.hpp>
#include <GC2D/GCConsole2.hpp>
#include <GC2D/ScrnFader.hpp>
#include <GC2D/PauseMenu2.hpp>
#include <GC2D/Guide.hpp>
#include <GC2D/CardLoad.hpp>
#include <GC2D/Talk2D2.hpp>
#include <GC2D/SunGlass.hpp>
#include <THPPlayer/THPPlayer.h>
#include <MSound/MSound.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjDolpic.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static void dummy(Vec* v)
{
	*v = (Vec) { 0.0f, 0.0f, 0.0f };
	*v = (Vec) { 1.0f, 1.0f, 1.0f };
}

class JPAEmitterManager;

extern JPAEmitterManager* gpEmitterManager4D2;

// TODO: 99.9%, every instruction and register exact. The frame is 0x310
// against retail's 0x410 and the slots now shift uniformly: every named slot
// (graphics at 0x2f4/0x1ec and its ctor's writes) is 0x108 apart and the
// TColor inline temp 0x10c, while the save area and frame are 0x100 apart. So
// the inline-temporary pool is 0x10c = 268 bytes short (ours 0xc..0x1e0 = 468
// bytes, retail 0xc..0x2ec = 736) and the extra 4 bytes mod 8 is why our
// TColor temp leaves a 4-byte hole below `graphics` where retail's is
// contiguous. 268 is not `sizeof(TGraphics)`; it is 256 + 12, so the shape is
// a 256-byte object plus a TVec3-sized one, or a run of smaller temps.
// Since `graphics` is the only function-scope local and retail's sits at the
// top of the pool, the missing bytes belong to inlined callees, not to this
// body: a `volatile char trash[256]` declared last reaches 0x410 but leaves
// every slot 0x100 low, and a dead 256-byte non-trivial local in a TU-static
// inlined callee overshoots to 0x710. Which callee owns them is open; the 15
// `TNameRefGen::search<T>` expansions are the only candidates numerous enough
// to account for 468 bytes in our own build.
//
// The `graphics.unk0 = 0` below is read off the target: retail's second `sth`
// goes to graphics+0x00, not graphics+0xFE (the +0xF4 `stw -1` and +0xFC `sth
// 0` before it are TColor's and TFlagT's default ctors, since TGraphics has
// only an implicit one).
void TMarDirector::setup2()
{
	unkBC = JDrama::TNameRefGen::search<TNameRefAryT<TStageEventInfo> >(
	    "イベントテーブル");
	if (unkBC) {
		u16 eventId = 0;
		for (TStageEventInfo* it = unkBC->begin(); it != unkBC->end(); ++it) {
			TMapObjBase* obj
			    = JDrama::TNameRefGen::search<TMapObjBase>(it->unk14);
			if (obj) {
				obj->mEventId = eventId;
				it->unk28     = obj;
			}
			eventId++;
		}
	}

	JDrama::TNameRefGen::search<TMario>("マリオ")->setGamePad(unk18[0]);

	TMarioGamePad* gamePad = getGamePad();
	JDrama::TNameRefGen::search<CPolarSubCamera>("camera 1")->unk120 = gamePad;

	unk84 = JDrama::TNameRefGen::search<TTalkCursor>("会話カーソル");

	mConsole = JDrama::TNameRefGen::search<TGCConsole2>("GCコンソール");

	mConsole->unkC = CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW;

	unkDC = JDrama::TNameRefGen::search<TShineFader>("シャインフェーダー");

	unkDC->mRate = 120.0f;
	unkDC->setColor(JUtility::TColor(0xD2, 0xD2, 0xD2, 0xFF));

	unkE0 = JDrama::TNameRefGen::search<TSunGlass>("サングラスフェーダ");
	unk78 = JDrama::TNameRefGen::search<TGuide>("ガイド画面");
	unkAC            = JDrama::TNameRefGen::search<TPauseMenu2>("ポーズメニュー");
	unkAC->mGamePad  = unk18[0];
	unkB0            = JDrama::TNameRefGen::search<TTalk2D2>("会話表示");
	unkB0->mGamePad  = unk18[0];
	unk70 = JDrama::TNameRefGen::search<TCardLoad>("データロード");

	unk70->unk38 = unk18[0];
	unk78->mGamePad = unk18[0];

	unk18[0]->mFlags = 0;
	if (mMap == 15) {
		unkAC->unkC = CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW;
		unkB0->unkC = CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW;
		unk18[0]->onFlag(0x20);
	} else {
		unk70->unkC = CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW;
	}

	unk254 = JDrama::TNameRefGen::search<TDemoCannon>("デモ砲台");

	TDrawSyncManager::smInstance->setCallback(1, 0x7D, 0x7D, gpSunMgr);
	TDrawSyncManager::smInstance->setCallback(2, 0x7E, 0x91,
	                                          &gpPollution->getCounterLayer());
	TDrawSyncManager::smInstance->setCallback(3, 0x92, 0xA5,
	                                          &gpPollution->getCounterObj());
	TDrawSyncManager::smInstance->setCallback(4, 0x7C, 0x7C, gpMarioOriginal);

	gpMSound->setCameraInfo(&gpCamera->unk124, gpCamera->unk13C,
	                        gpCamera->unk1EC, 0);

	// TODO: retail evaluates these two raw member reads right to left
	// (`lbz r4, 0x7d` before `lbz r3, 0x7c`), which is the documented
	// argument order; two plain member reads give us left to right instead.
	unk258 = MSStage::init(mMap, unk7D);

	JDrama::TGraphics graphics;
	graphics.unk0 = 0;
	unk40->perform(CUE_ALL, &graphics);
	unk38->perform(CUE_ALL, &graphics);
	GXSetDrawDone();
	GXWaitDrawDone();

	TMapEventSinkInPollution* sinkInPollutionEvent;

	sinkInPollutionEvent
	    = JDrama::TNameRefGen::search<TMapEventSinkInPollution>(
	        "イベント（地形沈む）");

	if (!sinkInPollutionEvent) {
		sinkInPollutionEvent
		    = JDrama::TNameRefGen::search<TMapEventSinkInPollution>(
		        "イベント（地形沈む再汚染）");
		if (!sinkInPollutionEvent) {
			sinkInPollutionEvent
			    = JDrama::TNameRefGen::search<TMapEventSinkInPollution>(
			        "イベント（地形沈むビアンコ）");
		}
	}

	if (sinkInPollutionEvent)
		sinkInPollutionEvent->initBuriedBuilding();
}

// TODO: 99.9%, zero instruction differences: frame 0x20 against retail's 0x38,
// 24 bytes of dead low region. Ruled out (all inert): splitting the five
// `(JKRMemArchive*)getVolume(...)` casts into two named locals each, folding
// the five unmount blocks into one inlined `unmountFixedVolume(const char*)`
// helper, SMSGetMSound() over gpMSound, getGamePad() over unk18[0], and a
// TDrawSyncManager::getInstance() level over the four smInstance reads.
TMarDirector::~TMarDirector()
{
	gpMSound->exitStage();
	if (gpApplication.mCurrArea.unk0 == 15) {
		if (JKRMemArchive* arch
		    = (JKRMemArchive*)JKRFileLoader::getVolume("option"))
			arch->unmountFixed();
	}

	if (JKRMemArchive* arch
	    = (JKRMemArchive*)JKRFileLoader::getVolume("game_6"))
		arch->unmountFixed();

	if (JKRMemArchive* arch = (JKRMemArchive*)JKRFileLoader::getVolume("guide"))
		arch->unmountFixed();

	if (JKRMemArchive* arch = (JKRMemArchive*)JKRFileLoader::getVolume("yoshi"))
		arch->unmountFixed();

	if (JKRMemArchive* arch = (JKRMemArchive*)JKRFileLoader::getVolume("scene"))
		arch->unmountFixed();

	unk18[0]->offFlag(0x20);
	if (mMap == 1 || (mMap == 0 && unk7D == 0)) {
		THPPlayerStop();
		THPPlayerClose();
		THPPlayerQuit();
	}

	TDrawSyncManager::smInstance->setCallback(1, 0, 0, nullptr);
	TDrawSyncManager::smInstance->setCallback(2, 0, 0, nullptr);
	TDrawSyncManager::smInstance->setCallback(3, 0, 0, nullptr);
	TDrawSyncManager::smInstance->setCallback(4, 0, 0, nullptr);
	gpEmitterManager4D2           = nullptr;
	JDrama::TNameRefGen::instance = nullptr;
	gpMarDirector                 = nullptr;
}
