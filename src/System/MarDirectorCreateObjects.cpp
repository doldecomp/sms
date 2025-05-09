#include <System/MarDirector.hpp>
#include <System/MarNameRefGen.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

bool TMarDirector::createObjects()
{
	JDrama::TNameRefGen::instance = new TMarNameRefGen;
	gpLightManager = new TLightWithDBSetManager("ライトマネージャー");
	return false;
}
