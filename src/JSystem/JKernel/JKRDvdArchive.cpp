#include <JSystem/JKernel/JKRDvdArchive.hpp>
#include <JSystem/JKernel/JKRDecomp.hpp>
#include <JSystem/JKernel/JKRDvdFile.hpp>
#include <JSystem/JKernel/JKRDvdRipper.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <stdlib.h>
#include <string.h>
#include <dolphin/os.h>

JKRDvdArchive::JKRDvdArchive(s32 entryNum,
                             JKRArchive::EMountDirection mountDirection)
    : JKRArchive(entryNum, MOUNT_DVD)
{
	mMountDirection = mountDirection;
	open(entryNum);

	mVolumeType = 'RARC';
	mVolumeName = mStrTable + mDirectories->mOffset;

	JSULink<JKRFileLoader>* fileLoaderLinkPtr = &mFileLoaderLink;
	getVolumeList().prepend(fileLoaderLinkPtr);
	mIsMounted = true;
}

JKRDvdArchive::~JKRDvdArchive()
{
	if (mIsMounted == true) {
		if (mArcInfoBlock) {
			SDIFileEntry* fileEntry = mFileEntries;
			int i                   = 0;
			for (; i < mArcInfoBlock->num_file_entries; fileEntry++, i++) {
				if (fileEntry->mData) {
					JKRFreeToHeap(mHeap, fileEntry->mData);
				}
			}

			JKRFreeToHeap(mHeap, mArcInfoBlock);
		}

		if (mDvdFile) {
			delete mDvdFile;
		}

		getVolumeList().remove(&mFileLoaderLink);
		mIsMounted = false;
	}
}

bool JKRDvdArchive::open(s32 entryNum)
{
	int alignment;
	SDIFileEntry* fileEntry;

	mArcInfoBlock = nullptr;
	mDataOffset   = nullptr;
	mDirectories  = nullptr;
	mFileEntries  = nullptr;
	mStrTable     = nullptr;

	mDvdFile = new (JKRGetSystemHeap(), 0) JKRDvdFile(entryNum);
	if (!mDvdFile) {
		mMountMode = UNKNOWN_MOUNT_MODE;
		return false;
	}

	SArcHeader* arcHeader
	    = (SArcHeader*)JKRAllocFromSysHeap(sizeof(SArcHeader), 0x20);
	if (!arcHeader) {
		mMountMode = UNKNOWN_MOUNT_MODE;
		goto cleanup;
	}

	JKRDvdToMainRam(entryNum, (u8*)arcHeader, EXPAND_SWITCH_DECOMPRESS,
	                sizeof(SArcHeader), nullptr,
	                JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, &mCompression);

	alignment = mMountDirection == MOUNT_DIRECTION_HEAD ? 0x20 : -0x20;

	mArcInfoBlock = (SArcDataInfo*)JKRAllocFromHeap(
	    mHeap, arcHeader->file_data_offset, alignment);
	if (!mArcInfoBlock) {
		mMountMode = UNKNOWN_MOUNT_MODE;
		goto cleanup;
	}

	JKRDvdToMainRam(entryNum, (u8*)mArcInfoBlock, EXPAND_SWITCH_DECOMPRESS,
	                arcHeader->file_data_offset, nullptr,
	                JKRDvdRipper::ALLOC_DIRECTION_FORWARD, sizeof(SArcHeader),
	                nullptr);

	mDirectories = (SDIDirEntry*)((int)&mArcInfoBlock->num_nodes
	                              + mArcInfoBlock->node_offset);
	mFileEntries = (SDIFileEntry*)((int)&mArcInfoBlock->num_nodes
	                               + mArcInfoBlock->file_entry_offset);
	mStrTable    = (char*)((int)&mArcInfoBlock->num_nodes
                        + mArcInfoBlock->string_table_offset);

	mDataOffset = arcHeader->header_length + arcHeader->file_data_offset;

cleanup:
	if (arcHeader) {
		JKRFreeToSysHeap(arcHeader);
	}

	if (mMountMode == UNKNOWN_MOUNT_MODE && mDvdFile) {
		delete mDvdFile;
	}

	if (mMountMode == UNKNOWN_MOUNT_MODE)
		return false;

	return true;
}

void* JKRDvdArchive::fetchResource(SDIFileEntry* fileEntry, u32* returnSize)
{
	if (!fileEntry->mData) {
		u8* resourcePtr;
		u32 resourceSize = fetchResource_subroutine(
		    mEntryNum, mDataOffset + fileEntry->mDataOffset, fileEntry->mSize,
		    mHeap,
		    JKRArchive::convertAttrToCompressionType(fileEntry->getAttr()),
		    mCompression, &resourcePtr);
		if (returnSize)
			*returnSize = resourceSize;

		fileEntry->mData = resourcePtr;
	} else if (returnSize) {
		*returnSize = fileEntry->mSize;
	}

	return fileEntry->mData;
}

void* JKRDvdArchive::fetchResource(void* buffer, u32 bufferSize,
                                   SDIFileEntry* fileEntry, u32* returnSize)
{
	bufferSize = (s32)ALIGN_PREV(bufferSize, 0x20);
	u32 size   = ALIGN_NEXT(fileEntry->mSize, 0x20);

	if (size > bufferSize)
		size = bufferSize;

	if (!fileEntry->mData) {
		JKRCompression fileCompression
		    = JKRArchive::convertAttrToCompressionType(fileEntry->getAttr());
		size = fetchResource_subroutine(
		    mEntryNum, mDataOffset + fileEntry->mDataOffset, fileEntry->mSize,
		    (u8*)buffer, bufferSize, fileCompression, mCompression);
	} else {
		JKRHeap::copyMemory(buffer, fileEntry->mData, size);
	}

	if (returnSize) {
		*returnSize = size;
	}
	return buffer;
}

u32 JKRDvdArchive::fetchResource_subroutine(s32 entryNum, u32 offset, u32 size,
                                            u8* dst, u32 dstLength,
                                            int fileCompression,
                                            int archiveCompression)
{
	u32 alignedSize      = ALIGN_NEXT(size, 0x20);
	u32 alignedDstLength = ALIGN_PREV(dstLength, 0x20);

	switch (archiveCompression) {
	case JKR_COMPRESSION_NONE: {
		switch (fileCompression) {
		case JKR_COMPRESSION_NONE:
			if (alignedSize > alignedDstLength) {
				alignedSize = alignedDstLength;
			}

			JKRDvdToMainRam(entryNum, dst, EXPAND_SWITCH_DEFAULT, alignedSize,
			                nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD,
			                offset, nullptr);
			return alignedSize;
		case JKR_COMPRESSION_YAY0:
		case JKR_COMPRESSION_YAZ0:
			u8* arcHeader = (u8*)JKRAllocFromSysHeap(sizeof(SArcHeader), 0x20);
			JKRDvdToMainRam(entryNum, arcHeader, EXPAND_SWITCH_NONE,
			                sizeof(SArcHeader), nullptr,
			                JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset,
			                nullptr);
			u32 decompressedSize = JKRDecompExpandSize(arcHeader);
			JKRFreeToSysHeap(arcHeader);
			u32 alignedDecompressedSize = ALIGN_NEXT(decompressedSize, 0x20);
			if (alignedDecompressedSize > alignedDstLength) {
				alignedDecompressedSize = alignedDstLength;
			}

			JKRDvdToMainRam(entryNum, dst, EXPAND_SWITCH_DEFAULT,
			                alignedDecompressedSize, nullptr,
			                JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset,
			                nullptr);
			return decompressedSize;
		}
	}

	case JKR_COMPRESSION_YAZ0: {
		if (size > alignedDstLength) {
			size = alignedDstLength;
		}

		JKRDvdToMainRam(entryNum, dst, EXPAND_SWITCH_DECOMPRESS, size, nullptr,
		                JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset, nullptr);
		return size;
	}

	case JKR_COMPRESSION_YAY0: {
		OSPanic(__FILE__, 533, "Sorry, not prepared for SZP archive.\n");
		return 0;
	}

	default: {
		OSPanic(__FILE__, 542, ":::??? bad sequence\n");
		return 0;
	}
	}
}

u32 JKRDvdArchive::fetchResource_subroutine(s32 entryNum, u32 offset, u32 size,
                                            JKRHeap* heap, int fileCompression,
                                            int archiveCompression,
                                            u8** returnResource)
{
	u32 alignedSize = ALIGN_NEXT(size, 0x20);
	u8* buffer;

	switch (archiveCompression) {
	case JKR_COMPRESSION_NONE: {
		switch (fileCompression) {
		case JKR_COMPRESSION_NONE: {
			buffer
			    = (u8*)JKRAllocFromHeap(heap, alignedSize, sizeof(SArcHeader));
			JUT_ASSERT(VERSION_SELECT(638, 631, 631), buffer != nullptr);

			JKRDvdToMainRam(
			    entryNum, buffer, EXPAND_SWITCH_DEFAULT, alignedSize, nullptr,
			    JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset, nullptr);

			*returnResource = buffer;
			return alignedSize;
		}
		case JKR_COMPRESSION_YAY0:
		case JKR_COMPRESSION_YAZ0: {
			u8* arcHeader
			    = (u8*)JKRAllocFromHeap(heap, sizeof(SArcHeader), 0x20);
			JKRDvdToMainRam(entryNum, arcHeader, EXPAND_SWITCH_NONE,
			                sizeof(SArcHeader), nullptr,
			                JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset,
			                nullptr);
			u32 decompressedSize = JKRDecompExpandSize(arcHeader);
			JKRFreeToHeap(heap, arcHeader);

			buffer = (u8*)JKRAllocFromHeap(heap, decompressedSize,
			                               sizeof(SArcHeader));

			JKRDvdToMainRam(entryNum, buffer, EXPAND_SWITCH_DECOMPRESS,
			                decompressedSize, nullptr,
			                JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset,
			                nullptr);

			*returnResource = buffer;
			return decompressedSize;
		}
		}
	}

	case JKR_COMPRESSION_YAZ0: {
		buffer = (u8*)JKRAllocFromHeap(heap, alignedSize, sizeof(SArcHeader));
		JUT_ASSERT(VERSION_SELECT(686, 672, 672), buffer);

		JKRDvdToMainRam(entryNum, buffer, EXPAND_SWITCH_DECOMPRESS, size,
		                nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset,
		                nullptr);

		*returnResource = buffer;
		return alignedSize;
	}

	case JKR_COMPRESSION_YAY0: {
		OSPanic(__FILE__, 608, "Sorry, not prepared for SZP archive.\n");
		return 0;
	}

	default: {
		OSPanic(__FILE__, 613, ":::??? bad sequence\n");
		return 0;
	}
	}
}
