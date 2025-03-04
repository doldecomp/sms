#include <JSystem/JAudio/JASystem/JASSimpleWaveBank.hpp>
#include <types.h>

namespace JASystem {

TSimpleWaveBank::TWaveHandle::TWaveHandle() { }

TSimpleWaveBank::TWaveHandle::~TWaveHandle() { }

const void* TSimpleWaveBank::TWaveHandle::getWavePtr() const { return 0; }

const TWaveInfo* TSimpleWaveBank::TWaveHandle::getWaveInfo() const
{
	return nullptr;
}

TSimpleWaveBank::TSimpleWaveBank() { }

TSimpleWaveBank::~TSimpleWaveBank() { }

void* TSimpleWaveBank::getHeap() { return 0; }

const char* TSimpleWaveBank::getWaveArcFileName() const { return 0; }

bool* TSimpleWaveBank::getLoadFlagPtr() { return 0; }

int TSimpleWaveBank::getType() const { return 0; }

TSimpleWaveBank::TWaveHandle* TSimpleWaveBank::getWaveHandle(u32) const
{
	return 0;
}

void TSimpleWaveBank::setWaveTableSize(u32) { }

void TSimpleWaveBank::setWaveInfo(u32, const TWaveInfo&) { }

void TSimpleWaveBank::setWaveArcFileName(const char*) { }

} // namespace JASystem
