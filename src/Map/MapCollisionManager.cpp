#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <stdio.h>

TMapCollisionManager::TMapCollisionManager(u16 param_1, const char* param_2,
                                           const TLiveActor* param_3)
{
	mEntries    = new TMapCollisionBase*[param_1];
	mMaxEntries = param_1;
	mEntryNum   = 0;
	unk8        = nullptr;
	mFolder     = param_2;
	unk10       = param_3;
	unk14       = 0;
}

// fabricated
inline u8 col_type(u16 param_1) { return param_1 & 3; }
inline u16 col_other(u16 param_1) { return param_1 & 0xFFFC; }

void TMapCollisionManager::init(const char* param_1, u16 param_2,
                                const char* param_3)
{
	char fullPath[256];
	char buffer[256];
	const char* folder;

	if (mFolder == nullptr)
		mFolder = param_3;

	createCollision(param_1, col_type(param_2));

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

	if (param_1[0] != '/')
		sprintf(fullPath, "%s/%s", folder, param_1);
	else
		sprintf(fullPath, "%s%s", folder, param_1);

	mEntries[mEntryNum]->init(fullPath, col_other(param_2) | 2, unk10);

	if (mEntryNum == 0) {
		unk8 = mEntries[mEntryNum];
	} else {
		unk8 = nullptr;
	}

	if (col_type(param_2) == 0) {
		unk8 = mEntries[mEntryNum];
	}

	mEntryNum++;
}

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

void TMapCollisionManager::getFileName(const char*, char*) { }

void TMapCollisionManager::changeCollision(u32 i)
{
	if (i < mEntryNum) {
		if (unk8 != mEntries[i]) {
			if (unk8 != nullptr)
				unk8->remove();
			unk8 = mEntries[i];
		}
	}
}
