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

void TMapCollisionManager::init(const char* file, u16 param_2, const char* path)
{
	char fullPath[256];
	char buffer[256];
	const char* folder;

	if (mFolder == nullptr)
		mFolder = path;

	createCollision(file, col_type(param_2));

	if (mFolder) {
		if (mFolder[0] != '/') {
			snprintf(buffer, 256, "/%s", mFolder);
			folder = buffer;
		} else {
			folder = mFolder;
		}
	} else {
		folder = "";
	}

	if (file[0] != '/')
		sprintf(fullPath, "%s/%s", folder, file);
	else
		sprintf(fullPath, "%s%s", folder, file);

	mEntries[mEntryNum]->init(fullPath, col_other(param_2) | 2, mOwnerActor);

	if (mEntryNum == 0) {
		mActiveEntry = mEntries[mEntryNum];
	} else {
		mActiveEntry = nullptr;
	}

	if (col_type(param_2) == 0) {
		mActiveEntry = mEntries[mEntryNum];
	}

	mEntryNum++;
}

TMapCollisionManager::TMapCollisionManager(u16 param_1, const char* param_2,
                                           const TLiveActor* param_3)
{
	mEntries     = new TMapCollisionBase*[param_1];
	mMaxEntries  = param_1;
	mEntryNum    = 0;
	mActiveEntry = nullptr;
	mFolder      = param_2;
	mOwnerActor  = param_3;
	unk14        = 0;
}
