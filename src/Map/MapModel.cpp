#include <Map/MapModel.hpp>

TMapModelManager::TMapModelManager(const char* name)
    : TJointModelManager(name)
{
}

void TMapModelManager::init() { }

TMapModel::TMapModel() { }

void TMapModel::initJointModel(TJointModelManager*, const char*, MActorAnmData*)
{
}

void TMapModel::initUnderpass() { }

void TMapModel::perform(unsigned long, JDrama::TGraphics*) { }

TMapModelManager::~TMapModelManager() { }

void TMapModelManager::newJointModel(int) const { }
