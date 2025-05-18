#include <Map/MapWireManager.hpp>

void TMapWireActor::checkTakingActor() { }

void TMapWireActor::getPosInWire() const { }

void TMapWireActor::getTipPoints(JGeometry::TVec3<float>*,
                                 JGeometry::TVec3<float>*) const
{
}

u32 TMapWireActor::receiveMessage(THitActor*, unsigned long) { }

void TMapWireActor::init(TMapWireActorManager*) { }

TMapWireActor::TMapWireActor(const char* name)
    : TTakeActor(name)
{
}

static void initDraw() { }

void TMapWireActorManager::doActorToWire() { }

void TMapWireActorManager::doWireToActor() { }

TMapWireActorManager::TMapWireActorManager(TTakeActor*) { }

void TMapWireManager::getWireNo(const JGeometry::TVec3<float>&) const { }

void TMapWireManager::getPointPosInNthWire(int, const JGeometry::TVec3<float>&,
                                           JGeometry::TVec3<float>*) const
{
}

void TMapWireManager::getPointPosInWire(const JGeometry::TVec3<float>&,
                                        JGeometry::TVec3<float>*) const
{
}

void TMapWireManager::perform(unsigned long, JDrama::TGraphics*) { }

void TMapWireManager::entry(TTakeActor*) { }

void TMapWireManager::loadAfter() { }

void TMapWireManager::load(JSUMemoryInputStream&) { }

TMapWireManager::TMapWireManager(const char*) { }
