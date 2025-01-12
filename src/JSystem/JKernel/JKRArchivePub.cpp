#include <JSystem/JKernel/JKRArchive.hpp>
#include <JSystem/JKernel/JKRAramArchive.hpp>
#include <JSystem/JKernel/JKRCompArchive.hpp>
#include <JSystem/JKernel/JKRDvdArchive.hpp>
#include <JSystem/JKernel/JKRFileFinder.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <dolphin/dvd.h>

JKRArchive* JKRArchive::check_mount_already(s32 entryNum)
{
	for (JSUListIterator<JKRFileLoader> iterator = getVolumeList().getFirst();
	     iterator != getVolumeList().getEnd(); ++iterator) {
		if (iterator->getVolumeType() == 'RARC') {
			JKRArchive* archive = (JKRArchive*)iterator.getObject();
			if (archive->mEntryNum == entryNum) {
				archive->mMountCount++;
				return archive;
			}
		}
	}

	return nullptr;
}

JKRArchive* JKRArchive::mount(const char* path,
                              JKRArchive::EMountMode mountMode, JKRHeap* heap,
                              JKRArchive::EMountDirection mountDirection)
{
	s32 entryNum = DVDConvertPathToEntrynum((char*)path);
	if (entryNum < 0)
		return nullptr;

	if (JKRArchive* archive = check_mount_already(entryNum)) {
		return archive;
	}

	int alignment = mountDirection == JKRArchive::MOUNT_DIRECTION_HEAD ? 4 : -4;

	JKRArchive* archive;
	switch (mountMode) {
	case JKRArchive::MOUNT_MEM:
		if (entryNum == -1)
			archive = nullptr;
		else
			archive
			    = new (heap, alignment) JKRMemArchive(entryNum, mountDirection);
		break;
	case JKRArchive::MOUNT_ARAM:
		archive
		    = new (heap, alignment) JKRAramArchive(entryNum, mountDirection);
		break;
	case JKRArchive::MOUNT_DVD:
		archive = new (heap, alignment) JKRDvdArchive(entryNum, mountDirection);
		break;
	case JKRArchive::MOUNT_COMP:
		archive
		    = new (heap, alignment) JKRCompArchive(entryNum, mountDirection);
		break;
	}

	if (archive && archive->getMountMode() == JKRArchive::UNKNOWN_MOUNT_MODE) {
		delete archive;
		archive = nullptr;
	}

	return archive;
}

bool JKRArchive::becomeCurrent(const char* path)
{
	SDIDirEntry* dirEntry;
	if (*path == '/') {
		path++;

		if (*path == '\0')
			path = nullptr;
		dirEntry = findDirectory(path, 0);
	} else {
		dirEntry = findDirectory(path, getCurrentDirID());
	}

	bool found = dirEntry != nullptr;
	if (found) {
		sCurrentVolume = this;
		sCurrentDirID  = dirEntry - mDirectories;
	}

	return found;
}

bool JKRArchive::getDirEntry(SDirEntry* dirEntry, u32 index) const
{
	SDIFileEntry* fileEntry = findIdxResource(index);
	if (!fileEntry) {
		return false;
	}

	dirEntry->mFlags = fileEntry->mFlag >> 24;
	dirEntry->mID    = fileEntry->mFileID;
	dirEntry->mName  = mStrTable + fileEntry->getNameOffset();
	return true;
}

void* JKRArchive::getGlbResource(u32 param_0, const char* path,
                                 JKRArchive* archive)
{
	void* resource = nullptr;

	if (archive) {
		resource = archive->getResource(param_0, path);
	} else {
		JSUList<JKRFileLoader>& volumeList = getVolumeList();
		for (JSUListIterator<JKRFileLoader> iterator = volumeList.getFirst();
		     iterator != volumeList.getEnd(); ++iterator) {
			if (iterator->getVolumeType() == 'RARC') {
				resource = iterator->getResource(param_0, path);
				if (resource)
					break;
			}
		}
	}

	return resource;
}

void* JKRArchive::getResource(const char* path)
{

	SDIFileEntry* fileEntry;
	if (*path == '/') {
		fileEntry = findFsResource(path + 1, 0);
	} else {
		fileEntry = findFsResource(path, getCurrentDirID());
	}

	if (fileEntry) {
		return fetchResource(fileEntry, nullptr);
	}

	return nullptr;
}

void* JKRArchive::getResource(u32 type, const char* path)
{

	SDIFileEntry* fileEntry;
	if (type == 0 || type == '\x3f\x3f\x3f\x3f') { // '????'
		fileEntry = findNameResource(path);
	} else {
		fileEntry = findTypeResource(type, path);
	}

	if (fileEntry) {
		return fetchResource(fileEntry, nullptr);
	}

	return nullptr;
}

size_t JKRArchive::readResource(void* buffer, u32 bufferSize, u32 type,
                                const char* path)
{

	SDIFileEntry* fileEntry;
	if (type == 0 || type == '\x3f\x3f\x3f\x3f') { // '????'
		fileEntry = findNameResource(path);
	} else {
		fileEntry = findTypeResource(type, path);
	}

	if (fileEntry) {
		u32 resourceSize;
		fetchResource(buffer, bufferSize, fileEntry, &resourceSize);
		return resourceSize;
	}

	return 0;
}

size_t JKRArchive::readResource(void* buffer, u32 bufferSize, const char* path)
{

	SDIFileEntry* fileEntry;
	if (*path == '/') {
		fileEntry = findFsResource(path + 1, 0);
	} else {
		fileEntry = findFsResource(path, getCurrentDirID());
	}

	if (fileEntry) {
		u32 resourceSize;
		fetchResource(buffer, bufferSize, fileEntry, &resourceSize);
		return resourceSize;
	}

	return 0;
}

void JKRArchive::removeResourceAll()
{
	if (mArcInfoBlock && mMountMode != MOUNT_MEM) {
		SDIFileEntry* fileEntry = mFileEntries;
		// BUT: fileEntry is not incremented
		for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
			if (fileEntry->mData) {
				// BUG: freeing nullptr, mem leak
				fileEntry->mData = nullptr;
				JKRFreeToHeap(mHeap, fileEntry->mData);
			}
		}
	}
}

bool JKRArchive::removeResource(void* resource)
{

	SDIFileEntry* fileEntry = findPtrResource(resource);
	if (fileEntry == nullptr)
		return false;

	fileEntry->mData = nullptr;
	JKRFreeToHeap(mHeap, resource);
	return true;
}

bool JKRArchive::detachResource(void* resource)
{

	SDIFileEntry* fileEntry = findPtrResource(resource);
	if (fileEntry == nullptr)
		return false;

	fileEntry->mData = nullptr;
	return true;
}

s32 JKRArchive::getResSize(const void* resource) const
{

	SDIFileEntry* fileEntry = findPtrResource(resource);
	if (fileEntry == nullptr)
		return -1;

	return fileEntry->mSize;
}

u32 JKRArchive::countFile(const char* path) const
{
	SDIDirEntry* dirEntry;
	if (*path == '/') {
		path++;

		if (*path == '\0')
			path = nullptr;
		dirEntry = findDirectory(path, 0);
	} else {
		dirEntry = findDirectory(path, sCurrentDirID);
	}

	if (dirEntry) {
		return dirEntry->mNum;
	}

	return 0;
}

JKRFileFinder* JKRArchive::getFirstFile(const char* path) const
{
	SDIDirEntry* dirEntry;
	if (*path == '/') {
		path++;

		if (*path == '\0')
			path = nullptr;
		dirEntry = findDirectory(path, 0);
	} else {
		dirEntry = findDirectory(path, sCurrentDirID);
	}

	if (dirEntry) {
		return new (JKRHeap::sSystemHeap, 0) JKRArcFinder(
		    (JKRArchive*)this, dirEntry->mFirstIdx, dirEntry->mNum);
	}

	return nullptr;
}
