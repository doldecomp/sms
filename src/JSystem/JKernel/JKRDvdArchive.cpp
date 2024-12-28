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
	if (!open(entryNum))
		return;

	mVolumeType = 'RARC';
	mVolumeName = mStrTable + mDirectories->mOffset;
	getVolumeList().prepend(&mFileLoaderLink);
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
			mArcInfoBlock = nullptr;
		}

		// if (mExpandedSize) {
		// 	JKRFree(mExpandedSize);
		// 	mExpandedSize = nullptr;
		// }

		if (mDvdFile) {
			delete mDvdFile;
		}

		getVolumeList().remove(&mFileLoaderLink);
		mIsMounted = false;
	}
}

static void dummy()
{
	OSReport(__FILE__);
	OSReport("isMounted()");
	OSReport("Halt");
	OSReport("mMountCount == 1");
}

bool JKRDvdArchive::open(s32 entryNum)
{
	int alignment;
	u8 useCompression;
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
	// mExpandedSize = nullptr;

	useCompression = 0;
	fileEntry      = mFileEntries;
	for (u32 i = 0; i < mArcInfoBlock->num_file_entries; fileEntry++, i++) {
		if (FLAG_ON(fileEntry->getAttr(), 1)) {
			useCompression |= fileEntry->getCompressFlag();
		}
	}

	if (useCompression) {
		s32* mExpandedSize = (s32*)JKRAllocFromHeap(
		    mHeap, sizeof(s32) * mArcInfoBlock->num_file_entries,
		    abs(alignment));
		if (!mExpandedSize) {
			// !@bug: mArcInfoBlock is allocated from mHeap but free'd to
			// sSystemHeap. I don't know what will happen if mHeap !=
			// sSystemHeap, but it's still a bug to free to the wrong allocator.
			JKRFreeToSysHeap(mArcInfoBlock);
			mMountMode = UNKNOWN_MOUNT_MODE;
			goto cleanup;
		}

		memset(mExpandedSize, 0, sizeof(s32) * mArcInfoBlock->num_file_entries);
	}

	mDataOffset = arcHeader->header_length + arcHeader->file_data_offset;

cleanup:
	if (arcHeader) {
		JKRFreeToSysHeap(arcHeader);
	}

	if (mMountMode == UNKNOWN_MOUNT_MODE) {
		OSReport(":::Cannot alloc memory [%s][%d]\n", __FILE__, 382);
		if (mDvdFile) {
			delete mDvdFile;
		}
		return false;
	}

	return true;
}

void* JKRDvdArchive::fetchResource(SDIFileEntry* fileEntry, u32* returnSize)
{
	JUT_ASSERT(413, isMounted());
	u32 tempReturnSize;
	if (returnSize == nullptr) {
		returnSize = &tempReturnSize;
	}

	JKRCompression fileCompression
	    = JKRConvertAttrToCompressionType(fileEntry->getAttr());
	if (!fileEntry->mData) {
		u8* resourcePtr;
		u32 resourceSize = fetchResource_subroutine(
		    mEntryNum, this->mDataOffset + fileEntry->mDataOffset,
		    fileEntry->mDataOffset, mHeap, fileCompression, mCompression,
		    &resourcePtr);
		*returnSize = resourceSize;
		if (resourceSize == 0) {
			return nullptr;
		}

		fileEntry->mData = resourcePtr;
		if (fileCompression == JKR_COMPRESSION_YAZ0) {
			// setExpandSize(fileEntry, *returnSize);
		}
	} else {
		if (fileCompression == JKR_COMPRESSION_YAZ0) {
			// u32 resourceSize = getExpandSize(fileEntry);
			// *returnSize      = resourceSize;
		} else {
			*returnSize = fileEntry->mSize;
		}
	}

	return fileEntry->mData;
}

void* JKRDvdArchive::fetchResource(void* buffer, u32 bufferSize,
                                   SDIFileEntry* fileEntry, u32* returnSize)
{
	JUT_ASSERT(489, isMounted());
	u32 expandSize;
	u32 size = fileEntry->mSize;
	JKRCompression fileCompression
	    = JKRConvertAttrToCompressionType(fileEntry->getAttr());

	if (!fileEntry->mData) {
		bufferSize = (s32)ALIGN_PREV(bufferSize, 0x20);
		size       = fetchResource_subroutine(
            mEntryNum, mDataOffset + fileEntry->mDataOffset, fileEntry->mSize,
            (u8*)buffer, bufferSize, fileCompression, mCompression);
	} else {
		if (fileCompression == JKR_COMPRESSION_YAZ0) {
			// expandSize = getExpandSize(fileEntry);
			if (expandSize) {
				size = expandSize;
			}
		}

		if (size > bufferSize) {
			size = bufferSize;
		}

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
			// The dst pointer to JKRDvdToMainRam should be aligned to 32 bytes.
			// This will align arcHeader to 32 bytes on the stack.
			char arcHeaderBuffer[64];
			u8* arcHeader = (u8*)ALIGN_NEXT((u32)arcHeaderBuffer, 0x20);
			JKRDvdToMainRam(entryNum, arcHeader, EXPAND_SWITCH_NONE,
			                sizeof(SArcHeader), nullptr,
			                JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset,
			                nullptr);
			DCInvalidateRange(arcHeader, sizeof(SArcHeader));

			u32 decompressedSize        = JKRDecompExpandSize(arcHeader);
			u32 alignedDecompressedSize = ALIGN_NEXT(decompressedSize, 0x20);
			if (alignedDecompressedSize > alignedDstLength) {
				alignedDecompressedSize = alignedDstLength;
			}

			JKRDvdToMainRam(entryNum, dst, EXPAND_SWITCH_DECOMPRESS,
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
		OSPanic(__FILE__, 610, "Sorry, not prepared for SZP archive.\n");
		return 0;
	}

	default: {
		OSPanic(__FILE__, 616, ":::??? bad sequence\n");
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
		case JKR_COMPRESSION_NONE:
			buffer
			    = (u8*)JKRAllocFromHeap(heap, alignedSize, sizeof(SArcHeader));
			JUT_ASSERT(VERSION_SELECT(638, 631, 631), buffer != nullptr);

			JKRDvdToMainRam(
			    entryNum, buffer, EXPAND_SWITCH_DEFAULT, alignedSize, nullptr,
			    JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset, nullptr);

			*returnResource = buffer;
			return alignedSize;
		case JKR_COMPRESSION_YAY0:
		case JKR_COMPRESSION_YAZ0:
			// The dst pointer to JKRDvdToMainRam should be aligned to 32 bytes.
			// This will align arcHeader to 32 bytes on the stack.
			char arcHeaderBuffer[64];
			u8* arcHeader = (u8*)ALIGN_NEXT((u32)arcHeaderBuffer, 0x20);
			JKRDvdToMainRam(entryNum, arcHeader, EXPAND_SWITCH_NONE,
			                sizeof(SArcHeader), nullptr,
			                JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset,
			                nullptr);
			DCInvalidateRange(arcHeader, sizeof(SArcHeader));

			alignedSize = JKRDecompExpandSize(arcHeader);
			buffer
			    = (u8*)JKRAllocFromHeap(heap, alignedSize, sizeof(SArcHeader));
			JUT_ASSERT(VERSION_SELECT(674, 660, 660), buffer);
			JKRDvdToMainRam(entryNum, buffer, EXPAND_SWITCH_DECOMPRESS,
			                alignedSize, nullptr,
			                JKRDvdRipper::ALLOC_DIRECTION_FORWARD, offset,
			                nullptr);

			*returnResource = buffer;
			return alignedSize;
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
		OSPanic(__FILE__, 697, "Sorry, not prepared for SZP archive.\n");
		return 0;
	}

	default: {
		OSPanic(__FILE__, 702, ":::??? bad sequence\n");
		return 0;
	}
	}
}
