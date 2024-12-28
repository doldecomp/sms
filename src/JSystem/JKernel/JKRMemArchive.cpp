#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JKernel/JKRDecomp.hpp>
#include <JSystem/JKernel/JKRDvdRipper.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <string.h>

JKRMemArchive::JKRMemArchive() { }

JKRMemArchive::JKRMemArchive(s32 entryNum,
                             JKRArchive::EMountDirection mountDirection)
    : JKRArchive(entryNum, MOUNT_MEM)
{
	mMountDirection = mountDirection;
	open(entryNum, mMountDirection);

	mVolumeType = 'RARC';
	mVolumeName = mStrTable + mDirectories->mOffset;

	JSULink<JKRFileLoader>* fileLoaderLinkPtr = &mFileLoaderLink;
	getVolumeList().prepend(fileLoaderLinkPtr);
	mIsMounted = true;
}

JKRMemArchive::JKRMemArchive(void* buffer, u32 bufferSize,
                             JKRMemBreakFlag memBreakFlag)
    : JKRArchive((s32)buffer, MOUNT_MEM) // WTF?
{
	open(buffer, bufferSize, memBreakFlag);

	mVolumeType = 'RARC';
	mVolumeName = mStrTable + mDirectories->mOffset;

	JSULink<JKRFileLoader>* fileLoaderLinkPtr = &mFileLoaderLink;
	getVolumeList().prepend(fileLoaderLinkPtr);

	mIsMounted = true;
}

JKRMemArchive::~JKRMemArchive()
{
	if (mIsMounted == true) {
		if (mIsOpen) {
			if (mArcHeader)
				JKRFreeToHeap(mHeap, mArcHeader);
		}

		getVolumeList().remove(&mFileLoaderLink);
		mIsMounted = false;
	}
}

void JKRMemArchive::fixedInit(s32 param_1)
{
	mIsMounted  = false;
	mMountMode  = 1;
	mMountCount = 1;
	_54         = 2;
	mHeap       = JKRHeap::sCurrentHeap;
	mEntryNum   = param_1;
	if (sCurrentVolume)
		return;
	sCurrentVolume = this;
	setCurrentDirID(0);
}

bool JKRMemArchive::mountFixed(void* param_1, JKRMemBreakFlag param_2)
{
	if (check_mount_already((s32)param_1)) {
		return false;
	}
	fixedInit((s32)param_1);
	if (!open(param_1, 0xffff, param_2)) {
		return false;
	}
	mVolumeType = 'RARC';
	mVolumeName = mStrTable + mDirectories->mOffset;
	sVolumeList.prepend(&mFileLoaderLink);
	mIsOpen    = param_2 == 1 ? true : false; // ewwwww
	mIsMounted = true;
	return true;
}

void JKRMemArchive::unmountFixed()
{
	JUT_ASSERT(337, isMounted());
	JUT_ASSERT(340, mMountCount == 1);
	if (sCurrentVolume == this) {
		sCurrentVolume = nullptr;
	}
	if (mIsOpen && mArcHeader) {
		JKRHeap::free(mArcHeader, mHeap);
	}
	sVolumeList.remove(&mFileLoaderLink);
	mIsMounted = false;
}

bool JKRMemArchive::open(s32 entryNum,
                         JKRArchive::EMountDirection mountDirection)
{
	mArcHeader      = nullptr;
	mArcInfoBlock   = nullptr;
	mArchiveData    = nullptr;
	mDirectories    = nullptr;
	mFileEntries    = nullptr;
	mStrTable       = nullptr;
	mIsOpen         = false;
	mMountDirection = mountDirection;

	if (mMountDirection == JKRArchive::MOUNT_DIRECTION_HEAD) {
		u32 loadedSize;
		mArcHeader = (SArcHeader*)JKRDvdRipper::loadToMainRAM(
		    entryNum, nullptr, EXPAND_SWITCH_DECOMPRESS, 0, mHeap,
		    JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, (int*)&mCompression);
	} else {
		u32 loadedSize;
		mArcHeader = (SArcHeader*)JKRDvdRipper::loadToMainRAM(
		    entryNum, nullptr, EXPAND_SWITCH_DECOMPRESS, 0, mHeap,
		    JKRDvdRipper::ALLOC_DIRECTION_BACKWARD, 0, (int*)&mCompression);
	}

	if (!mArcHeader) {
		mMountMode = UNKNOWN_MOUNT_MODE;
	} else {
		JUT_ASSERT(418, mArcHeader->signature == 'RARC');
		mArcInfoBlock
		    = (SArcDataInfo*)((u8*)mArcHeader + mArcHeader->header_length);
		mDirectories = (SDIDirEntry*)((u8*)&mArcInfoBlock->num_nodes
		                              + mArcInfoBlock->node_offset);
		mFileEntries = (SDIFileEntry*)((u8*)&mArcInfoBlock->num_nodes
		                               + mArcInfoBlock->file_entry_offset);
		mStrTable    = (char*)((u8*)&mArcInfoBlock->num_nodes
                            + mArcInfoBlock->string_table_offset);

		mArchiveData = (u8*)((u32)mArcHeader + mArcHeader->header_length
		                     + mArcHeader->file_data_offset);
		mIsOpen      = true;
	}

	return (mMountMode == UNKNOWN_MOUNT_MODE) ? false : true;
}

bool JKRMemArchive::open(void* buffer, u32 bufferSize, JKRMemBreakFlag flag)
{
	mArcHeader = (SArcHeader*)buffer;
	JUT_ASSERT(471, mArcHeader->signature == 'RARC');
	mArcInfoBlock
	    = (SArcDataInfo*)((u8*)mArcHeader + mArcHeader->header_length);
	mDirectories = (SDIDirEntry*)((u8*)&mArcInfoBlock->num_nodes
	                              + mArcInfoBlock->node_offset);
	mFileEntries = (SDIFileEntry*)((u8*)&mArcInfoBlock->num_nodes
	                               + mArcInfoBlock->file_entry_offset);
	mStrTable    = (char*)((u8*)&mArcInfoBlock->num_nodes
                        + mArcInfoBlock->string_table_offset);
	mArchiveData = (u8*)(((u32)mArcHeader + mArcHeader->header_length)
	                     + mArcHeader->file_data_offset);
	mIsOpen      = (flag == MBF_1) ? true : false; // mIsOpen might be u8
	mHeap        = JKRHeap::findFromRoot(buffer);
	mCompression = JKR_COMPRESSION_NONE;
	return true;
}

void* JKRMemArchive::fetchResource(SDIFileEntry* fileEntry, u32* resourceSize)
{
	JUT_ASSERT(535, isMounted());
	if (!fileEntry->mData) {
		fileEntry->mData = mArchiveData + fileEntry->mDataOffset;
	}

	if (resourceSize) {
		*resourceSize = fileEntry->mSize;
	}

	return fileEntry->mData;
}

void* JKRMemArchive::fetchResource(void* buffer, u32 bufferSize,
                                   SDIFileEntry* fileEntry, u32* resourceSize)
{
	JUT_ASSERT(575, isMounted());
	bufferSize    = ALIGN_PREV(bufferSize, 0x20);
	u32 srcLength = ALIGN_NEXT(fileEntry->mSize, 0x20);
	if (srcLength > bufferSize) {
		srcLength = bufferSize;
	}

	if (fileEntry->mData != nullptr) {
		JKRHeap::copyMemory(buffer, fileEntry->mData, srcLength);
	} else {
		JKRCompression compression
		    = JKRArchive::convertAttrToCompressionType(fileEntry->getAttr());
		srcLength = fetchResource_subroutine(
		    (u8*)mArchiveData + fileEntry->mDataOffset, srcLength, (u8*)buffer,
		    bufferSize, compression);
	}

	if (resourceSize) {
		*resourceSize = srcLength;
	}

	return buffer;
}

void JKRMemArchive::removeResourceAll()
{
	JUT_ASSERT(622, isMounted());

	if (mArcInfoBlock == nullptr)
		return;
	if (mMountMode == MOUNT_MEM)
		return;

	// !@bug: looping over file entries without incrementing the fileEntry
	// pointer. Thus, only the first fileEntry will clear/remove the resource
	// data.
	SDIFileEntry* fileEntry = mFileEntries;
	for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
		if (fileEntry->mData) {
			fileEntry->mData = nullptr;
		}
	}
}

bool JKRMemArchive::removeResource(void* resource)
{
	JUT_ASSERT(653, isMounted());

	SDIFileEntry* fileEntry = findPtrResource(resource);
	if (!fileEntry)
		return false;

	fileEntry->mData = nullptr;
	return true;
}

u32 JKRMemArchive::fetchResource_subroutine(u8* src, u32 srcLength, u8* dst,
                                            u32 dstLength, int compression)
{
	dstLength = ALIGN_PREV(dstLength, 0x20);
	srcLength = ALIGN_NEXT(srcLength, 0x20);

	switch (compression) {
	case JKR_COMPRESSION_NONE: {
		if (srcLength > dstLength) {
			srcLength = dstLength;
		}

		JKRHeap::copyMemory(dst, src, srcLength);
		return srcLength;
	}
	case JKR_COMPRESSION_YAY0:
	case JKR_COMPRESSION_YAZ0: {
		u32 expandedSize = JKRDecompExpandSize(src);
		srcLength        = expandedSize;
		if (expandedSize > dstLength) {
			srcLength = dstLength;
		}

		JKRDecompress(src, dst, srcLength, 0);
		return srcLength;
	}
	default:
		OSPanic(__FILE__, 720, ":::??? bad sequence\n");
		break;
	}

	return 0;
}

// u32 JKRMemArchive::getExpandedResSize(const void* resource) const {
//     SDIFileEntry* fileEntry = findPtrResource(resource);
//     if (fileEntry == nullptr)
//         return -1;

//     if (fileEntry->isCompressed() == false) {
//         return getResSize(resource);
//     } else {
//         return JKRDecompExpandSize((u8*)resource);
//     }
// }
