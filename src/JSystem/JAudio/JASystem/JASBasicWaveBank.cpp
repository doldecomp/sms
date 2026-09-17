#include <JSystem/JAudio/JASystem/JASBasicWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>

namespace JASystem {

TBasicWaveBank::TBasicWaveBank()
    : mWaveTable(nullptr)
    , mWaveTableSize(0)
    , mWaveGroups(nullptr)
    , mWaveGroupCount(0)
{
}

TBasicWaveBank::~TBasicWaveBank()
{
	delete[] mWaveTable;
	delete[] mWaveGroups;
}

TBasicWaveBank::TWaveGroup* TBasicWaveBank::getWaveGroup(int index)
{
	if (index >= mWaveGroupCount)
		return nullptr;
	return &mWaveGroups[index];
}

void TBasicWaveBank::setGroupCount(u32 count)
{
	delete[] mWaveGroups;
	mWaveGroupCount = count;
	mWaveGroups     = new (TWaveBank::getCurrentHeap(), 0) TWaveGroup[count];
}

void TBasicWaveBank::setWaveTableSize(u32 size)
{
	delete[] mWaveTable;
	mWaveTable = new (TWaveBank::getCurrentHeap(), 0) TWaveInfo*[size];
	Calc::bzero(mWaveTable, size * sizeof(TWaveInfo*));
	mWaveTableSize = size;
}

void TBasicWaveBank::incWaveTable(const TWaveGroup* group)
{
	TWaveInfo* waveInfo;

	for (int i = 0; i < group->mWaveCount; i++) {
		waveInfo         = &group->mWaves[i];
		TWaveInfo** slot = &mWaveTable[waveInfo->mWaveID];
		waveInfo->mNext  = nullptr;
		waveInfo->mPrev  = *slot;
		if (*slot)
			(*slot)->mNext = waveInfo;

		*slot = waveInfo;
	}
}

void TBasicWaveBank::decWaveTable(const TWaveGroup* group)
{
	for (int i = 0; i < group->mWaveCount; i++) {
		TWaveInfo* wave     = &group->mWaves[i];
		TWaveInfo** headPtr = &mWaveTable[wave->mWaveID];

		for (TWaveInfo* it = *headPtr; it; it = it->mPrev) {
			if (it != wave)
				continue;

			if (it->mNext == nullptr)
				*headPtr = it->mPrev;
			else
				it->mNext->mPrev = it->mPrev;

			if (it->mPrev != nullptr)
				it->mPrev->mNext = it->mNext;

			break;
		}
	}
}

TBasicWaveBank::TWaveHandle* TBasicWaveBank::getWaveHandle(u32 id) const
{
	if (id >= mWaveTableSize)
		return nullptr;

	TWaveInfo* info = mWaveTable[id];
	if (info)
		return &info->mWaveHandle;

	return nullptr;
}

TBasicWaveBank::TWaveGroup::TWaveGroup()
    : mLoadFlag(0)
    , mWaves(0)
    , mWaveCount(0)
    , mWaveArcFileName(0)
{
}

TBasicWaveBank::TWaveGroup::~TWaveGroup()
{
	delete[] mWaves;
	delete[] mWaveArcFileName;
}

void TBasicWaveBank::TWaveGroup::setWaveArcFileName(const char* name)
{
	delete[] mWaveArcFileName;
	u32 len          = strlen(name);
	mWaveArcFileName = new (TWaveBank::getCurrentHeap(), 0) char[len + 1];
	strcpy(mWaveArcFileName, name);
}

void TBasicWaveBank::TWaveGroup::setWaveCount(u32 count)
{
	delete[] mWaves;
	mWaveCount = count;
	mWaves     = new (TWaveBank::getCurrentHeap(), 0) TWaveInfo[count];
	for (int i = 0; i < count; ++i) {
		mWaves[i].mWaveHandle.mHeap                  = &mHeap;
		mWaves[i].mWaveHandle.mWaveInfo.mLoadFlagPtr = &mLoadFlag;
	}
}

void TBasicWaveBank::TWaveGroup::setWaveInfo(int index, u32 id,
                                             const JASystem::TWaveInfo& info)
{
	mWaves[index].mWaveID                            = id;
	mWaves[index].mWaveHandle.mWaveInfo              = info;
	mWaves[index].mWaveHandle.mWaveInfo.mLoadFlagPtr = &mLoadFlag;
}

} // namespace JASystem
