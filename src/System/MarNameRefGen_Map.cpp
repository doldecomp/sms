
#include "Camera/SunMgr.hpp"
#include "Map/BathWaterManager.hpp"
#include "Map/Map.hpp"
#include "Map/MapDraw.hpp"
#include "Map/MarineSnow.hpp"
#include "Map/PollutionManager.hpp"
#include "Map/Shimmer.hpp"
#include "Map/Sky.hpp"
#include "Map/StickyStainManager.hpp"
#include <System/MarNameRefGen.hpp>

JDrama::TNameRef* TMarNameRefGen::getNameRef_Map(const char* name) const
{
	if (strcmp(name, "Map") == 0)
		return new TMap;

	if (strcmp(name, "MapDrawWall") == 0)
		return new TMapDrawWall;

	if (strcmp(name, "Sky") == 0)
		return new TSky;

	if (strcmp(name, "Shimmer") == 0)
		return new TShimmer;

	if (strcmp(name, "Pollution") == 0)
		return new TPollutionManager;

	// TODO:
	// if ( strcmp(name, "PollutionTest" ) == 0 )
	//     return new TPollutionTest("落書きテスト");

	if (strcmp(name, "SunMgr") == 0)
		return new TSunMgr;

	// TODO:
	// if ( strcmp(name, "SunModel" ) == 0 )
	//     return new TSunModel(false, "<TSunModel>");

	// TODO:
	// if ( strcmp(name, "SunsetModel" ) == 0 )
	//     return new TSunModel(true, "<TSunModel>");

	if (strcmp(name, "MarineSnow") == 0)
		return new TMarineSnow;

	if (strcmp(name, "StickyStain") == 0)
		return new TStickyStainManager;

	if (strcmp(name, "BathWater") == 0)
		return new TBathWaterManager;

	return nullptr;
}
