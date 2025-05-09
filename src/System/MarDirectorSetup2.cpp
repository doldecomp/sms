#include <System/MarDirector.hpp>
#include <System/Application.hpp>
#include <System/MarioGamePad.hpp>
#include <System/DrawSyncManager.hpp>
#include <System/TalkCursor.hpp>
#include <System/MSoundMainSide.hpp>
#include <System/PerformList.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapEventSink.hpp>
#include <Player/MarioMain.hpp>
#include <Camera/SunMgr.hpp>
#include <GC2D/GCConsole2.hpp>
#include <GC2D/ScrnFader.hpp>
#include <THPPlayer/THPPlayer.h>
#include <MSound/MSound.hpp>
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

TMarDirector::~TMarDirector()
{
	gpMSound->exitStage();
	if (gpApplication.currArea._000 == 15) {
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

	(*unk18)->offFlag(0x20);
	if (map == 1 || map == 0 || _07D == 0) {
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

void TMarDirector::setup2()
{
	unkBC = JDrama::TNameRefGen::search<JDrama::TNameRef>("イベントテーブル");
	// TODO: some code related to unkBC, but what is it...
	JDrama::TNameRefGen::search<TMario>("マリオ")->setGamePad(*unk18);
	JDrama::TNameRefGen::search<CPolarSubCamera>("camera 1")->unk120 = *unk18;
	unk84   = JDrama::TNameRefGen::search<TTalkCursor>("会話カーソル");
	console = JDrama::TNameRefGen::search<TGCConsole2>("GCコンソール");
	console->unkC = 0xB;
	unkDC = JDrama::TNameRefGen::search<TSMSFader>("シャインフェーダー");
	unkDC->unk14 = 120.0f;
	unkDC->setColor(JUtility::TColor(0xD2, 0xD2, 0xD2, 0xFF));
	unkE0 = JDrama::TNameRefGen::search<TSMSFader>("サングラスフェーダ");
	unk78 = JDrama::TNameRefGen::search<JDrama::TNameRef>("ガイド画面");
	unkAC = JDrama::TNameRefGen::search<JDrama::TViewObj>("ポーズメニュー");
	unkB0 = JDrama::TNameRefGen::search<JDrama::TViewObj>("会話表示");
	unk70 = JDrama::TNameRefGen::search<JDrama::TViewObj>("データロード");

	// TODO:
	// unk70->unk38 = *unk18;
	// unk78->unkC0 = *unk18;

	(*unk18)->mFlags = 0;
	if (map == 15) {
		unkAC->unkC = 0xB;
		unkB0->unkC = 0xB;
		(*unk18)->onFlag(0x20);
	} else {
		unk70->unkC = 0xB;
	}

	unk254 = JDrama::TNameRefGen::search<JDrama::TNameRef>("デモ砲台");

	TDrawSyncManager::smInstance->setCallback(1, 0x7D, 0x7D, gpSunMgr);
	TDrawSyncManager::smInstance->setCallback(2, 0x7E, 0x91,
	                                          &gpPollution->getCounterLayer());
	TDrawSyncManager::smInstance->setCallback(3, 0x92, 0xA5,
	                                          &gpPollution->getCounterObj());
	TDrawSyncManager::smInstance->setCallback(4, 0x7C, 0x7C, gpMarioOriginal);

	gpMSound->setCameraInfo(&gpCamera->unk124, gpCamera->unk13C,
	                        gpCamera->unk1EC, 0);

	unk258 = MSStage::init(map, _07D);

	JDrama::TGraphics graphics;
	unk40->perform(0xffffffff, &graphics);
	unk38->perform(0xffffffff, &graphics);
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
