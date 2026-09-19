
#include "Camera/SunMgr.hpp"
#include "Camera/SunModel.hpp"
#include "Map/BathWaterManager.hpp"
#include "Map/Map.hpp"
#include "Map/MapDraw.hpp"
#include "Map/MarineSnow.hpp"
#include "Map/PollutionEvent.hpp"
#include "Map/PollutionManager.hpp"
#include "Map/Shimmer.hpp"
#include "Map/Sky.hpp"
#include "Map/StickyStainManager.hpp"
#include <System/MarNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>

JDrama::TNameRef* TMarNameRefGen::getNameRef_Map(const char* name) const
{
	if (strcmp(name, "Map") == 0)
		return new TMap("マップ");

	if (strcmp(name, "MapDrawWall") == 0)
		return new TMapDrawWall("カメラ食い込み壁");

	if (strcmp(name, "Sky") == 0)
		return new TSky("空");

	if (strcmp(name, "Shimmer") == 0)
		return new TShimmer("<Shimmer>");

	if (strcmp(name, "Pollution") == 0)
		return new TPollutionManager("落書き管理");

	if (strcmp(name, "PollutionTest") == 0)
		return new TPollutionTest("落書きテスト");

	if (strcmp(name, "SunMgr") == 0)
		return new TSunMgr("<TSunMgr>");

	if (strcmp(name, "SunModel") == 0)
		return new TSunModel(false, "<TSunModel>");

	if (strcmp(name, "SunsetModel") == 0)
		return new TSunModel(true, "<TSunModel>");

	if (strcmp(name, "MarineSnow") == 0)
		return new TMarineSnow("MarineSnow");

	if (strcmp(name, "StickyStain") == 0)
		return new TStickyStainManager("<TViewObj>");

	if (strcmp(name, "BathWater") == 0)
		return new TBathWaterManager;

	return nullptr;
}
