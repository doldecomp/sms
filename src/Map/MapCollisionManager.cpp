#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <stdio.h>

void TMapCollisionManager::changeCollision(u32 i)
{
	if (i < mEntryNum) {
		if (mActiveEntry != mEntries[i]) {
			if (mActiveEntry != nullptr)
				mActiveEntry->remove();
			mActiveEntry = mEntries[i];
		}
	}
}

void TMapCollisionManager::getFileName(const char*, char*) { }

#pragma dont_inline on
void TMapCollisionManager::createCollision(const char* param_1, u8 param_2)
{
	switch (param_2) {
	case 0:
		unk14 |= 0x8000;
		mEntries[mEntryNum] = new TMapCollisionStatic;
		break;
	case 1:
		mEntries[mEntryNum] = new TMapCollisionMove;
		break;
	case 2:
		mEntries[mEntryNum] = new TMapCollisionWarp;
		break;
	}
}
#pragma dont_inline off

// fabricated
inline u8 col_type(u16 param_1) { return param_1 & 3; }
inline u16 col_other(u16 param_1) { return param_1 & 0xFFFC; }

void TMapCollisionManager::init(const char* col_file, u16 flags,
                                const char* folder)
{
	char fullPath[256];
	char buffer[256];
	const char* actualFolder;

	if (mFolder == nullptr)
		mFolder = folder;

	createCollision(col_file, col_type(flags));

	if (mFolder) {
		if (mFolder[0] != '/') {
			snprintf(buffer, 256, "/%s", mFolder);
			actualFolder = buffer;
		} else {
			actualFolder = mFolder;
		}
	} else {
		actualFolder = "";
	}

	if (col_file[0] != '/')
		sprintf(fullPath, "%s/%s", actualFolder, col_file);
	else
		sprintf(fullPath, "%s%s", actualFolder, col_file);

	mEntries[mEntryNum]->init(fullPath, col_other(flags) | 2, mOwnerActor);

	if (mEntryNum == 0) {
		mActiveEntry = mEntries[mEntryNum];
	} else {
		mActiveEntry = nullptr;
	}

	if (col_type(flags) == 0) {
		mActiveEntry = mEntries[mEntryNum];
	}

	mEntryNum++;
}

TMapCollisionManager::TMapCollisionManager(u16 max_entries, const char* folder,
                                           const TLiveActor* owner)
{
	mEntries     = new TMapCollisionBase*[max_entries];
	mMaxEntries  = max_entries;
	mEntryNum    = 0;
	mActiveEntry = nullptr;
	mFolder      = folder;
	mOwnerActor  = owner;
	unk14        = 0;
}
