#include <JSystem/JAudio/JASystem/JASBasicWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>

namespace JASystem {

TBasicWaveBank::TBasicWaveBank()
    : unk4(nullptr)
    , unk8(0)
    , unkC(nullptr)
    , unk10(0)
{
}

TBasicWaveBank::~TBasicWaveBank()
{
	delete[] unk4;
	delete[] unkC;
}

TBasicWaveBank::TWaveGroup* TBasicWaveBank::getWaveGroup(int index)
{
	if (index >= unk10)
		return nullptr;
	return &unkC[index];
}

void TBasicWaveBank::setGroupCount(u32 count)
{
	delete[] unkC;
	unk10 = count;
	unkC  = new (TWaveBank::getCurrentHeap(), 0) TWaveGroup[count];
}

void TBasicWaveBank::setWaveTableSize(u32 size)
{
	delete[] unk4;
	unk4 = new (TWaveBank::getCurrentHeap(), 0) TWaveInfo*[size];
	Calc::bzero(unk4, size * sizeof(TWaveInfo*));
	unk8 = size;
}

void TBasicWaveBank::incWaveTable(const TWaveGroup* group)
{
	for (int i = 0; i < group->unk38; i++) {
		u32 waveID          = group->unk34[i].unk34;
		TWaveInfo** table   = unk4;
		TWaveInfo* waveInfo = &group->unk34[i];
		waveInfo->mNext     = nullptr;
		waveInfo->mPrev     = table[waveID];
		if (table[waveID])
			table[waveID]->mNext = waveInfo;

		table[waveID] = waveInfo;
	}
}

void TBasicWaveBank::decWaveTable(const TWaveGroup* group)
{
	for (int i = 0; i < group->unk38; i++) {
		TWaveInfo* needle    = &group->unk34[i];
		TWaveInfo** haystack = &unk4[needle->unk34];

		for (TWaveInfo* ptr = *haystack; ptr; ptr = ptr->mPrev) {
			if (ptr != needle)
				continue;

			if (ptr->mNext == nullptr)
				*haystack = ptr->mPrev;
			else
				ptr->mNext->mPrev = ptr->mPrev;

			if (ptr->mPrev != nullptr)
				ptr->mPrev->mNext = ptr->mNext;

			break;
		}
	}
}

TBasicWaveBank::TWaveHandle* TBasicWaveBank::getWaveHandle(u32 id) const
{
	if (id >= unk8)
		return nullptr;

	if (unk4[id])
		return &unk4[id]->mWaveHandle;

	return nullptr;
}

TBasicWaveBank::TWaveGroup::TWaveGroup()
    : unk30(0)
    , unk34(0)
    , unk38(0)
    , unk3C(0)
{
}

TBasicWaveBank::TWaveGroup::~TWaveGroup()
{
	delete[] unk34;
	delete[] unk3C;
}

void TBasicWaveBank::TWaveGroup::setWaveArcFileName(const char* name)
{
	delete[] unk3C;
	u32 len = strlen(name);
	unk3C   = new (TWaveBank::getCurrentHeap(), 0) char[len + 1];
	strcpy(unk3C, name);
}

void TBasicWaveBank::TWaveGroup::setWaveCount(u32 count)
{
	delete[] unk34;
	unk38 = count;
	unk34 = new (TWaveBank::getCurrentHeap(), 0) TWaveInfo[count];
	for (int i = 0; i < count; ++i) {
		unk34[i].mWaveHandle.unk30      = &unk4;
		unk34[i].mWaveHandle.unk4.unk24 = &unk30;
	}
}

void TBasicWaveBank::TWaveGroup::setWaveInfo(int index, u32 id,
                                             const JASystem::TWaveInfo& info)
{
	unk34[index].unk34                  = id;
	unk34[index].mWaveHandle.unk4       = info;
	unk34[index].mWaveHandle.unk4.unk24 = &unk30;
}

} // namespace JASystem
