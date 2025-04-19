#include <MoveBG/MapObjManager.hpp>

TMapObjBaseManager::TMapObjBaseManager(const char* name)
    : TLiveManager(name)
{
}

void TMapObjBaseManager::load(JSUMemoryInputStream&) { }

void TMapObjBaseManager::getObjNumWithActorType(unsigned long) const { }

void TMapObjBaseManager::createModelData() { }

void TMapObjBaseManager::clipActors(JDrama::TGraphics*) { }

void TMapObjBaseManager::getActorTypeByEventID(unsigned long) { }

void TMapObjBaseManager::newAndRegisterObjByEventID(unsigned long, const char*)
{
}

void TMapObjBaseManager::newAndRegisterObj(const char*,
                                           const JGeometry::TVec3<float>&,
                                           const JGeometry::TVec3<float>&,
                                           const JGeometry::TVec3<float>&)
{
}

TMapObjBase* newUniqueObjByName(const char*) { }

void newItemByName(const char*) { }

void TMapObjBaseManager::makeObjAppeared(unsigned long) { }

void TMapObjBaseManager::makeObjAppear(unsigned long) { }

void TMapObjBaseManager::makeObjAppear(float, float, float, unsigned long, bool)
{
}

void TMapObjBaseManager::canAppear(const TMapObjBase*, unsigned long) const { }

TMapObjManager::TMapObjManager(const char* name)
    : TMapObjBaseManager(name)
{
}

void TMapObjManager::load(JSUMemoryInputStream&) { }

void TMapObjManager::loadMatTable(const char*) { }

void TMapObjManager::initDrawBuffer() { }

void TMapObjManager::loadAfter() { }

void TMapObjManager::entryStaticDrawBufferSun(J3DModel*) { }

void TMapObjManager::entryStaticDrawBufferShadow(J3DModel*) { }
