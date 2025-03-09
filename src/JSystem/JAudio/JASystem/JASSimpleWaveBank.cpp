#include <JSystem/JAudio/JASystem/JASSimpleWaveBank.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <types.h>

namespace JASystem {

TSimpleWaveBank::TSimpleWaveBank()
    : unk8(0)
    , unkC(0)
    , unk10(0)
    , unk40(0)
{
}

TSimpleWaveBank::~TSimpleWaveBank()
{
	delete[] unk8;
	delete[] unk10;
}

void TSimpleWaveBank::setWaveTableSize(u32 size)
{
	delete[] unk8;
	unk8 = new (TWaveBank::getCurrentHeap(), 0) TWaveHandle[size];
	unkC = size;
}

TSimpleWaveBank::TWaveHandle* TSimpleWaveBank::getWaveHandle(u32 i) const
{
	if (i >= unkC)
		return nullptr;
	return &unk8[i];
}

void TSimpleWaveBank::setWaveInfo(u32 i, const TWaveInfo& info)
{
	unk8[i].mWaveInfo       = info;
	unk8[i].mWaveInfo.unk24 = &unk40;
	unk8[i].mHeap           = &unk14;
}

void TSimpleWaveBank::setWaveArcFileName(const char* name)
{
	delete[] unk10;
	u32 len = strlen(name);
	unk10   = new (TWaveBank::getCurrentHeap(), 0) char[len + 1];
	strcpy(unk10, name);
}

} // namespace JASystem
