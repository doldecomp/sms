#include <System/MarDirector.hpp>
#include <System/PerformList.hpp>
#include <System/RenderModeObj.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewport.hpp>

// TODO: which headers are these infections strings from?
// See InfectiousStrings.hpp also
static const char* dummy                 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE = "メモリが足りません\n";
static const char cDirtyFileName[]       = "/scene/map/pollution/H_ma_rak.bti";
static const char cDirtyTexName[]        = "H_ma_rak_dummy";

void TMarDirector::preEntry(TPerformList* list)
{
	JDrama::TViewObj* setViewMtx
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>(
	        "J3D System Set View Mtx");
	JDrama::TViewObj* mirrorDisplayModelControl
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("鏡表示モデル管理");
	JDrama::TViewObj* mapGroup
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("マップグループ");
	JDrama::TViewObj* camera1
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("camera 1");

	list->push_back(camera1, 0x10);
	list->push_back(setViewMtx, 0x4);
	list->push_back("DrawBuf Sky Opa", 0x480);
	list->push_back("DrawBuf Sky Xlu", 0x480);
	list->push_back("空グループ", 0x204);
	list->push_back("DrawBuf MapOpa", 0x480);
	list->push_back("DrawBuf MapXlu", 0x480);
	list->push_back(mapGroup, 0x204);
	list->push_back(mirrorDisplayModelControl, 0x204);
	list->push_back("DrawBuf Map 半透明優先 (opa)", 0x480);
	list->push_back("DrawBuf Map 半透明優先 (xlu)", 0x480);
	list->push_back("マップ", 0x4000200);
	list->push_back("DrawBuf Map 半透明優先2 (opa)", 0x480);
	list->push_back("DrawBuf Map 半透明優先2 (xlu)", 0x480);
	list->push_back("マップ", 0x2000200);
	list->push_back("DrawBuf Graffito", 0x480);
	list->push_back("落書きグループ", 0x204);
	list->push_back("ライトマネージャー", 0x400);
	list->push_back("DrawBuf ChrOpa", 0x480);
	list->push_back("DrawBuf ChrXlu", 0x480);
	list->push_back("マネージャーグループ", 0x204);
	list->push_back("コンダクター", 0x204);
	list->push_back(
	    new JDrama::TViewport(SMSGetRederRect_Game(), "vp WParticle 2"), 0x8);
	list->push_back(camera1, 0x10);
	list->push_back("水マネージャ", 0x4);
	list->push_back("水飛沫マネージャ", 0x4);
	list->push_back("クエッションマネージャ", 0x4);

	JDrama::TViewObj* indirectSheen
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("インダイレクトシーン");
	if (indirectSheen) {
		list->push_back("DrawBuf Indirect", 0x480);
		list->push_back(indirectSheen, 0x40000204);
	}
	list->push_back(camera1, 0x10);
	list->push_back(setViewMtx, 0x4);
	list->push_back("プレーヤーグループ", 0x10000000);
	list->push_back("プレーヤーグループ", 0x204);
	list->push_back("プレーヤーグループ", 0x8000000);
}
