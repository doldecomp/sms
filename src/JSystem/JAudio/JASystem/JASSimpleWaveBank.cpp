#include <JSystem/JAudio/JASystem/JASSimpleWaveBank.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <types.h>

namespace JASystem {

TSimpleWaveBank::TSimpleWaveBank()
    : mWaveTable(0)
    , mWaveTableSize(0)
    , mWaveArcFileName(0)
    , mLoadFlag(0)
{
}

TSimpleWaveBank::~TSimpleWaveBank()
{
	delete[] mWaveTable;
	delete[] mWaveArcFileName;
}

void TSimpleWaveBank::setWaveTableSize(u32 size)
{
	delete[] mWaveTable;
	mWaveTable     = new (TWaveBank::getCurrentHeap(), 0) TWaveHandle[size];
	mWaveTableSize = size;
}

TSimpleWaveBank::TWaveHandle* TSimpleWaveBank::getWaveHandle(u32 i) const
{
	if (i >= mWaveTableSize)
		return nullptr;
	return &mWaveTable[i];
}

void TSimpleWaveBank::setWaveInfo(u32 i, const TWaveInfo& info)
{
	mWaveTable[i].mWaveInfo              = info;
	mWaveTable[i].mWaveInfo.mLoadFlagPtr = &mLoadFlag;
	mWaveTable[i].mHeap                  = &mHeap;
}

void TSimpleWaveBank::setWaveArcFileName(const char* name)
{
	delete[] mWaveArcFileName;
	u32 len          = strlen(name);
	mWaveArcFileName = new (TWaveBank::getCurrentHeap(), 0) char[len + 1];
	strcpy(mWaveArcFileName, name);
}

} // namespace JASystem
