#include <JSystem/JAudio/JASystem/JASBasicWaveBank.hpp>

namespace JASystem {

TWaveHandle::~TWaveHandle() { }

TBasicWaveBank::TBasicWaveBank() { }

TBasicWaveBank::~TBasicWaveBank() { }

int TBasicWaveBank::getType() const { return 0; }

TBasicWaveBank::TWaveHandle* TBasicWaveBank::getWaveHandle(u32 id) const
{
	return nullptr;
}

void TBasicWaveBank::setGroupCount(u32 count) { }

void TBasicWaveBank::setWaveTableSize(u32 size) { }

TBasicWaveBank::TWaveInfo::TWaveInfo() { }

TBasicWaveBank::TWaveInfo::~TWaveInfo() { }

TBasicWaveBank::TWaveGroup::TWaveGroup() { }

TBasicWaveBank::TWaveGroup::~TWaveGroup() { }

void TBasicWaveBank::TWaveGroup::setWaveCount(u32 count) { }

void TBasicWaveBank::TWaveGroup::setWaveInfo(int index, u32 id,
                                             const JASystem::TWaveInfo& info)
{
}

void TBasicWaveBank::TWaveGroup::setWaveArcFileName(const char* name) { }

const char* TBasicWaveBank::TWaveGroup::getWaveArcFileName() const
{
	return nullptr;
}

JKRHeap* TBasicWaveBank::TWaveGroup::getHeap() { return nullptr; }

bool* TBasicWaveBank::TWaveGroup::getLoadFlagPtr() { return nullptr; }

TBasicWaveBank::TWaveGroup* TBasicWaveBank::getWaveGroup(int index)
{
	return nullptr;
}

void TBasicWaveBank::incWaveTable(const TWaveGroup* group) { }

void TBasicWaveBank::decWaveTable(const TWaveGroup* group) { }

TBasicWaveBank::TWaveHandle::~TWaveHandle() { }

const void* TBasicWaveBank::TWaveHandle::getWavePtr() const { return nullptr; }

const TWaveInfo* TBasicWaveBank::TWaveHandle::getWaveInfo() const
{
	return nullptr;
}

} // namespace JASystem
