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

	list->push_back(camera1, CUE_SET_PROJECTION);
	list->push_back(setViewMtx, CUE_CALC_VIEW);
	list->push_back("DrawBuf Sky Opa", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("DrawBuf Sky Xlu", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("空グループ", CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back("DrawBuf MapOpa", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("DrawBuf MapXlu", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back(mapGroup, CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back(mirrorDisplayModelControl, CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back("DrawBuf Map 半透明優先 (opa)",
	                CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("DrawBuf Map 半透明優先 (xlu)",
	                CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("マップ", CUE_SEMITRANSPARENT_PRIO_1 | CUE_ENTRY);
	list->push_back("DrawBuf Map 半透明優先2 (opa)",
	                CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("DrawBuf Map 半透明優先2 (xlu)",
	                CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("マップ", CUE_SEMITRANSPARENT_PRIO_2 | CUE_ENTRY);
	list->push_back("DrawBuf Graffito", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("落書きグループ", CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back("ライトマネージャー", CUE_SET_DRAW_BUFFER);
	list->push_back("DrawBuf ChrOpa", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("DrawBuf ChrXlu", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("マネージャーグループ", CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back("コンダクター", CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back(
	    new JDrama::TViewport(SMSGetRederRect_Game(), "vp WParticle 2"),
	    CUE_DRAW);
	list->push_back(camera1, CUE_SET_PROJECTION);
	list->push_back("水マネージャ", CUE_CALC_VIEW);
	list->push_back("水飛沫マネージャ", CUE_CALC_VIEW);
	list->push_back("クエッションマネージャ", CUE_CALC_VIEW);

	JDrama::TViewObj* indirectSheen
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("インダイレクトシーン");
	if (indirectSheen) {
		list->push_back("DrawBuf Indirect",
		                CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
		list->push_back(indirectSheen,
		                CUE_UNK40000000 | CUE_ENTRY | CUE_CALC_VIEW);
	}
	list->push_back(camera1, CUE_SET_PROJECTION);
	list->push_back(setViewMtx, CUE_CALC_VIEW);
	list->push_back("プレーヤーグループ", CUE_UNK10000000);
	list->push_back("プレーヤーグループ", CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back("プレーヤーグループ", CUE_UNK8000000);
}
