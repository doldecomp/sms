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

void TMarDirector::setup2()
{
	unkBC = static_cast<TNameRefAryT<TStageEventInfo>*>(
	    JDrama::TNameRefGen::search("イベントテーブル"));
	if (unkBC) {
		for (TStageEventInfo* it = unkBC->begin(); it != unkBC->end(); ++it) {
			JDrama::TNameRef* ref = JDrama::TNameRefGen::search(it->unk14);
			if (ref) {
				// TODO: what is ref?
				it->unk28 = ref;
			}
		}
	}

	static_cast<TMario*>(JDrama::TNameRefGen::search("マリオ"))
	    ->setGamePad(unk18[0]);
	static_cast<CPolarSubCamera*>(JDrama::TNameRefGen::search("camera 1"))
	    ->unk120
	    = unk18[0];

	unk84 = static_cast<TTalkCursor*>(
	    JDrama::TNameRefGen::search("会話カーソル"));

	mConsole = static_cast<TGCConsole2*>(
	    JDrama::TNameRefGen::search("GCコンソール"));

	mConsole->unkC = CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW;

	unkDC = static_cast<TShineFader*>(
	    JDrama::TNameRefGen::search("シャインフェーダー"));

	unkDC->mRate = 120.0f;
	unkDC->setColor(JUtility::TColor(0xD2, 0xD2, 0xD2, 0xFF));

	unkE0 = static_cast<TSunGlass*>(
	    JDrama::TNameRefGen::search("サングラスフェーダ"));
	unk78 = static_cast<TGuide*>(JDrama::TNameRefGen::search("ガイド画面"));
	unkAC = static_cast<TPauseMenu2*>(
	    JDrama::TNameRefGen::search("ポーズメニュー"));
	unkB0 = static_cast<TTalk2D2*>(JDrama::TNameRefGen::search("会話表示"));
	unk70
	    = static_cast<TCardLoad*>(JDrama::TNameRefGen::search("データロード"));

	unk70->unk38 = unk18[0];
	unk78->unkC0 = unk18[0];

	unk18[0]->mFlags = 0;
	if (mMap == 15) {
		unkAC->unkC = CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW;
		unkB0->unkC = CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW;
		unk18[0]->onFlag(TMarioGamePad::PAD_FLAG_NO_B);
	} else {
		unk70->unkC = CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW;
	}

	unk254 = static_cast<TDemoCannon*>(JDrama::TNameRefGen::search("デモ砲台"));

	TDrawSyncManager::smInstance->setCallback(1, 0x7D, 0x7D, gpSunMgr);
	TDrawSyncManager::smInstance->setCallback(2, 0x7E, 0x91,
	                                          &gpPollution->getCounterLayer());
	TDrawSyncManager::smInstance->setCallback(3, 0x92, 0xA5,
	                                          &gpPollution->getCounterObj());
	TDrawSyncManager::smInstance->setCallback(4, 0x7C, 0x7C, gpMarioOriginal);

	gpMSound->setCameraInfo(&gpCamera->unk124, gpCamera->unk13C,
	                        gpCamera->unk1EC, 0);

	unk258 = MSStage::init(mMap, unk7D);

	JDrama::TGraphics graphics;
	graphics.unkFE = 0;
	unk40->perform(CUE_ALL, &graphics);
	unk38->perform(CUE_ALL, &graphics);
	GXSetDrawDone();
	GXWaitDrawDone();

	TMapEventSinkInPollution* sinkInPollutionEvent;

	sinkInPollutionEvent = static_cast<TMapEventSinkInPollution*>(
	    JDrama::TNameRefGen::search("イベント（地形沈む）"));

	if (!sinkInPollutionEvent) {
		sinkInPollutionEvent = static_cast<TMapEventSinkInPollution*>(
		    JDrama::TNameRefGen::search("イベント（地形沈む再汚染）"));
		if (!sinkInPollutionEvent) {
			sinkInPollutionEvent = static_cast<TMapEventSinkInPollution*>(
			    JDrama::TNameRefGen::search("イベント（地形沈むビアンコ）"));
		}
	}

	if (sinkInPollutionEvent)
		sinkInPollutionEvent->initBuriedBuilding();
}

TMarDirector::~TMarDirector()
{
	char trash[0x18];
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

	unk18[0]->offFlag(TMarioGamePad::PAD_FLAG_NO_B);
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
