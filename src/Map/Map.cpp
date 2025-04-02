#include <Map/Map.hpp>

TMap::~TMap() { }

TMap::TMap(const char* name)
    : JDrama::TViewObj(name)
{
}

void TMap::load(JSUMemoryInputStream&) { }

void TMap::loadAfter() { }

void TMap::perform(unsigned long, JDrama::TGraphics*) { }

void TMap::changeModel(short) const { }

f32 TMap::checkGround(float, float, float, const TBGCheckData**) const { }

void TMap::checkGround(const JGeometry::TVec3<float>&,
                       const TBGCheckData**) const
{
}

void TMap::checkGroundExactY(const JGeometry::TVec3<float>&,
                             const TBGCheckData**) const
{
}

void TMap::checkGroundExactY(float, float, float, const TBGCheckData**) const {
}

void TMap::checkGroundIgnoreWaterSurface(const JGeometry::TVec3<float>&,
                                         const TBGCheckData**) const
{
}

void TMap::checkGroundIgnoreWaterSurface(float, float, float,
                                         const TBGCheckData**) const
{
}

void TMap::checkGroundIgnoreWaterThrough(float, float, float,
                                         const TBGCheckData**) const
{
}

void TMap::checkRoof(const JGeometry::TVec3<float>&, const TBGCheckData**) const
{
}

void TMap::checkRoof(float, float, float, const TBGCheckData**) const { }

void TMap::checkRoofIgnoreWaterThrough(float, float, float,
                                       const TBGCheckData**) const
{
}

void TMap::isTouchedWallsAndMoveXZ(TBGWallCheckRecord*) const { }

void TMap::isTouchedOneWallAndMoveXZ(float*, float, float*, float) const { }

void TMap::isTouchedOneWall(float, float, float, float) const { }

void TMap::isTouchedOneWall(const JGeometry::TVec3<float>&, float) const { }

void TMap::intersectLine(const JGeometry::TVec3<float>&,
                         const JGeometry::TVec3<float>&, bool,
                         JGeometry::TVec3<float>*) const
{
}

void TMap::isInArea(float, float) const { }

TBGCheckData* TMap::getIllegalCheckData() { }

void TMap::update() { }

void updateRicco() { }

void TMap::updateMonte() { }

void TMap::updateDelfino() { }

static void initStage() { }

static void initStageCommon() { }

static void initDolpic() { }

static void initBianco() { }

static void initPinnaBeach() { }

static void initPinnaParco() { }

static void initMare() { }

static void initMonte() { }

static void initSirena() { }

static void initOption() { }
