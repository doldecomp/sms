#include <Map/JointModelManager.hpp>

void TJointModelManager::newJointModel(int) const { }

TJointModelManager::TJointModelManager(const char* name)
    : JDrama::TViewObj(name)
{
}

void TJointModelManager::initJointModel(const char*, const char**) { }

void TJointModelManager::perform(unsigned long, JDrama::TGraphics*) { }
