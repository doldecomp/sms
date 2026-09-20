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

// Two-local binder over search2: 0x408 alone / 0x410 with Setup2GamePad.
// TColor then sits 4 low of retail (pool ordering). Root/instance forks that
// closed preEntry's same residue are inert here (every site already shares
// one binder). One-local forms drop to 0x3d0.
template <class T> static inline T* Setup2Search(const char* name)
{
	JDrama::TNameRef* ref = JDrama::TNameRefGen::search2(name);
	T* obj                = (T*)ref;
	return obj;
}

static inline TMarioGamePad* Setup2GamePad(TMarDirector* dir)
{
	TMarioGamePad* pad = dir->getGamePad();
	return pad;
}

// TODO: 100% fuzzy, frame exact at 0x410. Every slot matches except the
// setColor TColor temporary: retail constructs at 0x2ec / copies at 0x2f0
// (contiguous with graphics at 0x2f4); ours is 4 low (0x2e8 / 0x2ec) with a
// dead word in the hole. Same residue class as preEntry's closed 4-byte
// ordering gap, but the instance/root forks that fixed preEntry are inert
// here (every search site already shares one two-local binder). Rejected:
// named fadeColor (lands in the named block at 0x3ec), u32 TColor ctor,
// setColor-before-mRate, a shine-fade wrapper, search<T> at the shine site,
// one-local binders (frame 0x3d0). Setup2Search + Setup2GamePad +
// getCurrentMap/Stage are load-bearing for the exact frame and arg order.
//
// The `graphics.unk0 = 0` below is read off the target: retail's second `sth`
// goes to graphics+0x00, not graphics+0xFE (the +0xF4 `stw -1` and +0xFC `sth
// 0` before it are TColor's and TFlagT's default ctors, since TGraphics has
// only an implicit one).
void TMarDirector::setup2()
{
	unkBC = Setup2Search<TNameRefAryT<TStageEventInfo> >("イベントテーブル");
	if (unkBC) {
		u16 eventId = 0;
		for (TStageEventInfo* it = unkBC->begin(); it != unkBC->end(); ++it) {
			TMapObjBase* obj = Setup2Search<TMapObjBase>(it->unk14);
			if (obj) {
				obj->mEventId = eventId;
				it->unk28     = obj;
			}
			eventId++;
		}
	}

	Setup2Search<TMario>("マリオ")->setGamePad(unk18[0]);

	Setup2Search<CPolarSubCamera>("camera 1")->unk120 = Setup2GamePad(this);

	unk84 = Setup2Search<TTalkCursor>("会話カーソル");

	mConsole = Setup2Search<TGCConsole2>("GCコンソール");

	mConsole->unkC = CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW;

	unkDC = Setup2Search<TShineFader>("シャインフェーダー");

	unkDC->mRate = 120.0f;
	unkDC->setColor(JUtility::TColor(0xD2, 0xD2, 0xD2, 0xFF));

	unkE0 = Setup2Search<TSunGlass>("サングラスフェーダ");
	unk78 = Setup2Search<TGuide>("ガイド画面");
	unkAC            = Setup2Search<TPauseMenu2>("ポーズメニュー");
	unkAC->mGamePad  = unk18[0];
	unkB0            = Setup2Search<TTalk2D2>("会話表示");
	unkB0->mGamePad  = unk18[0];
	unk70 = Setup2Search<TCardLoad>("データロード");

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

	unk254 = Setup2Search<TDemoCannon>("デモ砲台");

	TDrawSyncManager::smInstance->setCallback(1, 0x7D, 0x7D, gpSunMgr);
	TDrawSyncManager::smInstance->setCallback(2, 0x7E, 0x91,
	                                          &gpPollution->getCounterLayer());
	TDrawSyncManager::smInstance->setCallback(3, 0x92, 0xA5,
	                                          &gpPollution->getCounterObj());
	TDrawSyncManager::smInstance->setCallback(4, 0x7C, 0x7C, gpMarioOriginal);

	gpMSound->setCameraInfo(&gpCamera->unk124, gpCamera->unk13C,
	                        gpCamera->unk1EC, 0);

	unk258 = MSStage::init(getCurrentMap(), getCurrentStage());

	JDrama::TGraphics graphics;
	graphics.unk0 = 0;
	unk40->perform(CUE_ALL, &graphics);
	unk38->perform(CUE_ALL, &graphics);
	GXSetDrawDone();
	GXWaitDrawDone();

	TMapEventSinkInPollution* sinkInPollutionEvent
	    = Setup2Search<TMapEventSinkInPollution>("イベント（地形沈む）");

	if (!sinkInPollutionEvent) {
		sinkInPollutionEvent = Setup2Search<TMapEventSinkInPollution>(
		    "イベント（地形沈む再汚染）");
		if (!sinkInPollutionEvent) {
			sinkInPollutionEvent = Setup2Search<TMapEventSinkInPollution>(
			    "イベント（地形沈むビアンコ）");
		}
	}

	if (sinkInPollutionEvent)
		sinkInPollutionEvent->initBuriedBuilding();
}

// Three of five getVolume sites through a pointer-returning binder (+8 each)
// lands the 0x38 frame; yoshi/scene stay as raw casts. Five binder sites
// overshoot to 0x48; zero stay at 0x20.
static inline JKRMemArchive* Setup2GetMemArchive(const char* name)
{
	JKRMemArchive* arch = (JKRMemArchive*)JKRFileLoader::getVolume(name);
	return arch;
}

TMarDirector::~TMarDirector()
{
	gpMSound->exitStage();
	if (gpApplication.mCurrArea.unk0 == 15) {
		if (JKRMemArchive* arch = Setup2GetMemArchive("option"))
			arch->unmountFixed();
	}

	if (JKRMemArchive* arch = Setup2GetMemArchive("game_6"))
		arch->unmountFixed();

	if (JKRMemArchive* arch = Setup2GetMemArchive("guide"))
		arch->unmountFixed();

	if (JKRMemArchive* arch
	    = (JKRMemArchive*)JKRFileLoader::getVolume("yoshi"))
		arch->unmountFixed();

	if (JKRMemArchive* arch
	    = (JKRMemArchive*)JKRFileLoader::getVolume("scene"))
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
