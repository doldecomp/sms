#include <JSystem/JStage/JSGSystem.hpp>

JStage::TSystem::~TSystem() { }

s32 JStage::TSystem::JSGFGetType() const { return 1; }

void* JStage::TSystem::JSGFindObject(const char*, JStage::TEObject) const
{
	return nullptr;
}

bool JStage::TSystem::JSGCreateObject(const char*, JStage::TEObject, u32)
{
	return false;
}

void JStage::TSystem::JSGDestroyObject(JStage::TObject*) { }

bool JStage::TSystem::JSGGetSystemData(u32) { return false; }

void JStage::TSystem::JSGSetSystemData(u32, u32) { }
