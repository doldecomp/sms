#include <JSystem/JKernel/JKRArchive.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <ctype.h>

u32 JKRArchive::sCurrentDirID;

JKRArchive::JKRArchive()
{
	mIsMounted      = false;
	mMountDirection = MOUNT_DIRECTION_HEAD;
}

JKRArchive::JKRArchive(s32 entryNumber, JKRArchive::EMountMode mountMode)
{
	mIsMounted  = false;
	mMountMode  = mountMode;
	mMountCount = 1;
	_54         = 1;

	mHeap = JKRHeap::findFromRoot(this);
	if (mHeap == nullptr) {
		mHeap = JKRHeap::getCurrentHeap();
	}

	mEntryNum = entryNumber;
	if (getCurrentVolume() == nullptr) {
		setCurrentVolume(this);
		setCurrentDirID(0);
	}
}

JKRArchive::~JKRArchive() { }

bool JKRArchive::isSameName(JKRArchive::CArcName& name, u32 nameOffset,
                            u16 nameHash) const
{
	u16 hash = name.getHash();
	if (hash != nameHash)
		return false;
	return strcmp(mStrTable + nameOffset, name.getString()) == 0;
}

JKRArchive::SDIDirEntry* JKRArchive::findDirectory(const char* name,
                                                   u32 directoryId) const
{
	if (name == nullptr) {
		return mDirectories + directoryId;
	}

	CArcName arcName(&name, '/');
	SDIDirEntry* dirEntry   = mDirectories + directoryId;
	SDIFileEntry* fileEntry = mFileEntries + dirEntry->mFirstIdx;

	for (int i = 0; i < dirEntry->mNum; fileEntry++, i++) {
		if (isSameName(arcName, fileEntry->getNameOffset(), fileEntry->mHash)) {
			if (fileEntry->isDirectory()) {
				return findDirectory(name, fileEntry->mDataOffset);
			}
			break;
		}
	}

	return nullptr;
}

JKRArchive::SDIDirEntry* JKRArchive::findResType(u32 type) const
{
	SDIDirEntry* node = mDirectories;
	u32 count         = 0;
	while (count < mArcInfoBlock->num_nodes) {
		if (node->mType == type) {
			return node;
		}

		node++;
		count++;
	}

	return nullptr;
}

JKRArchive::SDIFileEntry* JKRArchive::findTypeResource(u32 type,
                                                       const char* name) const
{
	if (type) {
		CArcName arcName(name);
		SDIDirEntry* dirEntry = findResType(type);

		if (dirEntry) {
			SDIFileEntry* fileEntry = mFileEntries + dirEntry->mFirstIdx;
			for (int i = 0; i < dirEntry->mNum; fileEntry++, i++) {
				if (isSameName(arcName, fileEntry->getNameOffset(),
				               fileEntry->mHash)) {
					return fileEntry;
				}
			}
		}
	}

	return nullptr;
}

JKRArchive::SDIFileEntry* JKRArchive::findFsResource(const char* name,
                                                     u32 directoryId) const
{
	if (name) {
		CArcName arcName(&name, '/');
		SDIDirEntry* dirEntry   = mDirectories + directoryId;
		SDIFileEntry* fileEntry = mFileEntries + dirEntry->mFirstIdx;

		for (int i = 0; i < dirEntry->mNum; fileEntry++, i++) {
			if (isSameName(arcName, fileEntry->getNameOffset(),
			               fileEntry->mHash)) {
				if (fileEntry->isDirectory()) {
					SDIFileEntry* ntry
					    = findFsResource(name, fileEntry->mDataOffset);
					return ntry;
				}

				if (name == nullptr) {
					return fileEntry;
				}

				return nullptr;
			}
		}
	}

	return nullptr;
}

JKRArchive::SDIFileEntry* JKRArchive::findIdxResource(u32 fileIndex) const
{
	if (fileIndex < mArcInfoBlock->num_file_entries) {
		return mFileEntries + fileIndex;
	}

	return nullptr;
}

JKRArchive::SDIFileEntry* JKRArchive::findNameResource(const char* name) const
{
	SDIFileEntry* fileEntry = mFileEntries;

	CArcName arcName(name);
	for (int i = 0; i < mArcInfoBlock->num_file_entries; fileEntry++, i++) {
		if (isSameName(arcName, fileEntry->getNameOffset(), fileEntry->mHash)) {
			return fileEntry;
		}
	}

	return nullptr;
}

JKRArchive::SDIFileEntry*
JKRArchive::findPtrResource(const void* resource) const
{
	SDIFileEntry* fileEntry = mFileEntries;
	for (int i = 0; i < mArcInfoBlock->num_file_entries; fileEntry++, i++) {
		if (fileEntry->mData == resource) {
			return fileEntry;
		}
	}

	return nullptr;
}

void JKRArchive::CArcName::store(const char* name)
{
	mHash      = 0;
	s32 length = 0;
	while (*name) {
		s32 ch = tolower(*name);
		mHash  = ch + mHash * 3;
		if (length < (s32)ARRAY_COUNT(mString)) {
			mString[length++] = ch;
		}
		name++;
	}

	mLength         = (u16)length;
	mString[length] = 0;
}

const char* JKRArchive::CArcName::store(const char* name, char endChar)
{
	mHash      = 0;
	s32 length = 0;
	while (*name && *name != endChar) {
		s32 lch = tolower((int)*name);
		mHash   = lch + mHash * 3;
		if (length < (s32)ARRAY_COUNT(mString)) {
			mString[length++] = lch;
		}
		name++;
	}

	mLength         = (u16)length;
	mString[length] = 0;

	if (*name == 0)
		return nullptr;
	return name + 1;
}
