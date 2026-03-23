#include <JSystem/JKernel/JKRCompArchive.hpp>
#include <JSystem/JKernel/JKRAram.hpp>
#include <JSystem/JKernel/JKRAramArchive.hpp>
#include <JSystem/JKernel/JKRAramBlock.hpp>
#include <JSystem/JKernel/JKRDecomp.hpp>
#include <JSystem/JKernel/JKRDvdAramRipper.hpp>
#include <JSystem/JKernel/JKRDvdArchive.hpp>
#include <JSystem/JKernel/JKRDvdFile.hpp>
#include <JSystem/JKernel/JKRDvdRipper.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include "JSystem/JKernel/JKRArchive.hpp"
#include "dolphin/os.h"

JKRCompArchive::JKRCompArchive(s32 entryNum,
                               JKRArchive::EMountDirection mountDirection)
    : JKRArchive(entryNum, MOUNT_COMP)
{
	mMountDirection = mountDirection;

	open(entryNum);

	mVolumeType = 'RARC';
	mVolumeName = &mStrTable[mDirectories->mOffset];
	sVolumeList.prepend(&mFileLoaderLink);
	mIsMounted = true;
}

JKRCompArchive::~JKRCompArchive()
{
	if (mArcInfoBlock != nullptr) {
		SDIFileEntry* file = mFileEntries;
		for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
			u32 flags = file->mFlagsAndNameOffset >> 24;
			if (!(flags & 0x10) && file->mData != nullptr) {
				JKRFreeToHeap(mHeap, file->mData);
			}

			file++;
		}

		JKRFreeToHeap(mHeap, mArcInfoBlock);
		mArcInfoBlock = nullptr;
	}

	if (mAramPart != nullptr) {
		delete mAramPart;
	}

	if (mDvdFile != nullptr) {
		delete mDvdFile;
	}

	sVolumeList.remove(&mFileLoaderLink);
	mIsMounted = false;
}

static void dummy()
{
	const int unknownTable1[4]   = { 0, 2, 1, 3 };
	const float unknownTable2[3] = { 1, 1, 1 };
	const float unknownTable3[3] = { 1, 1, 1 };
}

bool JKRCompArchive::open(s32 entryNum)
{
	mArcInfoBlock   = nullptr;
	mArcFileData    = nullptr;
	mAramPart       = nullptr;
	_68             = 0;
	mSizeOfMemPart  = 0;
	mSizeOfAramPart = 0;
	_78             = 0;
	mDirectories    = nullptr;
	mFileEntries    = nullptr;
	mStrTable       = nullptr;

	mDvdFile = new (JKRGetSystemHeap(), 0) JKRDvdFile(entryNum);
	if (mDvdFile == nullptr) {
		mMountMode = 0;
		return 0;
	}

	// NOTE: unconfirmed if this struct is used
	SArcHeader* arcHeader
	    = (SArcHeader*)JKRAllocFromSysHeap(sizeof(SArcHeader), -32);
	if (arcHeader == nullptr) {
		mMountMode = 0;
	} else {
		int alignment;
		JKRDvdToMainRam(entryNum, (u8*)arcHeader, EXPAND_SWITCH_DECOMPRESS, 32,
		                nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0,
		                &mCompression);

		mSizeOfMemPart  = arcHeader->_14;
		mSizeOfAramPart = arcHeader->_18;

		switch (mCompression) {
		case JKR_COMPRESSION_NONE:
		case JKR_COMPRESSION_YAZ0:
			alignment     = mMountDirection == MOUNT_DIRECTION_HEAD ? 32 : -32;
			mArcInfoBlock = (SArcDataInfo*)JKRAllocFromHeap(
			    mHeap, arcHeader->file_data_offset + mSizeOfMemPart, alignment);

			if (mArcInfoBlock == nullptr) {
				mMountMode = 0;
				break;
			}

			JKRDvdToMainRam(
			    entryNum, (u8*)mArcInfoBlock, EXPAND_SWITCH_DECOMPRESS,
			    (u32)arcHeader->file_data_offset + mSizeOfMemPart, nullptr,
			    JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0x20, nullptr);
			mArcFileData = (u8*)mArcInfoBlock + arcHeader->file_data_offset;

			if (mSizeOfAramPart != 0) {
				mAramPart = (JKRAramBlock*)JKRAllocFromAram(mSizeOfAramPart,
				                                            JKRAramHeap::HEAD);
				if (mAramPart == nullptr) {
					mMountMode = 0;
					break;
				}

				JKRDvdToAram(entryNum, mAramPart->getAddress(),
				             EXPAND_SWITCH_DECOMPRESS,
				             arcHeader->header_length
				                 + arcHeader->file_data_offset + mSizeOfMemPart,
				             0);
			}

			mDirectories = (SDIDirEntry*)((u8*)mArcInfoBlock
			                              + mArcInfoBlock->node_offset);
			mFileEntries = (SDIFileEntry*)((u8*)mArcInfoBlock
			                               + mArcInfoBlock->file_entry_offset);
			mStrTable    = (const char*)((u8*)mArcInfoBlock
                                      + mArcInfoBlock->string_table_offset);
			_68 = arcHeader->header_length + arcHeader->file_data_offset;
			break;

		case JKR_COMPRESSION_YAY0:
			u8* buf         = nullptr;
			u8* mem         = nullptr;
			u32 alignedSize = mDvdFile->getFileSize();

			while (true) {
				alignedSize = ALIGN_NEXT(alignedSize, 32);
				alignment = mMountDirection == MOUNT_DIRECTION_HEAD ? 32 : -32;

				buf = (u8*)JKRAllocFromSysHeap(alignedSize, -alignment);

				if (buf == nullptr) {
					mMountMode = 0;
					break;
				}

				JKRDvdToMainRam(entryNum, buf, EXPAND_SWITCH_NONE, alignedSize,
				                nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD,
				                0, nullptr);
				u32 expandSize = JKRDecompExpandSize(buf);
				expandSize     = ALIGN_NEXT(expandSize, 32);
				mem = (u8*)JKRAllocFromHeap(mHeap, expandSize, -alignment);

				if (mem == nullptr) {
					mMountMode = 0;
					break;
				}

				arcHeader = (SArcHeader*)mem;
				JKRDecompress(buf, mem, expandSize, 0);
				JKRFreeToSysHeap(buf);

				mArcInfoBlock = (SArcDataInfo*)JKRAllocFromHeap(
				    mHeap, arcHeader->file_data_offset + mSizeOfMemPart,
				    alignment);

				if (mArcInfoBlock == nullptr) {
					mMountMode = 0;
					break;
				}

				// arcHeader + 1 should lead to 0x20, which is the
				// data after the header
				JKRHeap::copyMemory(mArcInfoBlock, arcHeader + 1,
				                    arcHeader->file_data_offset
				                        + mSizeOfMemPart);
				mArcFileData = (u8*)mArcInfoBlock + arcHeader->file_data_offset;
				if (mSizeOfAramPart == 0)
					break;

				mAramPart = (JKRAramBlock*)JKRAllocFromAram(mSizeOfAramPart,
				                                            JKRAramHeap::HEAD);
				if (mAramPart == nullptr) {
					mMountMode = 0;
					break;
				}

				JKRMainRamToAram(mem + arcHeader->header_length
				                     + arcHeader->file_data_offset
				                     + mSizeOfMemPart,
				                 mAramPart->getAddress(), mSizeOfAramPart,
				                 EXPAND_SWITCH_DEFAULT, 0, nullptr, -1);
				break;
			}
			mDirectories = (SDIDirEntry*)((u8*)mArcInfoBlock
			                              + mArcInfoBlock->node_offset);
			mFileEntries = (SDIFileEntry*)((u8*)mArcInfoBlock
			                               + mArcInfoBlock->file_entry_offset);
			mStrTable    = (char*)((u8*)mArcInfoBlock
                                + mArcInfoBlock->string_table_offset);
			_68 = arcHeader->header_length + arcHeader->file_data_offset;
			break;
		}
	}

	if (arcHeader != nullptr) {
		// BUG: freeing to wrong heap
		JKRFreeToHeap(mHeap, arcHeader);
	}

	if (mMountMode == 0 && mDvdFile != nullptr) {
		delete mDvdFile;
		mDvdFile = nullptr;
	}

	if (mMountMode == 0)
		return false;

	return true;
}

void* JKRCompArchive::fetchResource(SDIFileEntry* fileEntry, u32* pSize)
{
	JUT_ASSERT(597, isMounted());

	if (fileEntry->mData == nullptr) {
		u32 flag = fileEntry->mFlagsAndNameOffset >> 24;
		if (flag & 0x10) {
			fileEntry->mData = mArcFileData + fileEntry->mDataOffset;
			if (pSize)
				*pSize = fileEntry->mSize;
		} else if (flag & 0x20) {
			u8* data;
			u32 size = JKRAramArchive::fetchResource_subroutine(
			    fileEntry->mDataOffset + mAramPart->getAddress()
			        - mSizeOfMemPart,
			    fileEntry->mSize, mHeap,
			    JKRConvertAttrToCompressionType((u8)flag), &data);
			if (pSize)
				*pSize = size;
			fileEntry->mData = data;
		} else if (flag & 0x40) {
			u8* data;
			u32 size = JKRDvdArchive::fetchResource_subroutine(
			    mEntryNum, _68 + fileEntry->mDataOffset, fileEntry->mSize,
			    mHeap, JKRConvertAttrToCompressionType((u8)flag), mCompression,
			    &data);
			if (pSize)
				*pSize = size;
			fileEntry->mData = data;
		}
	} else {
		if (pSize != nullptr) {
			*pSize = fileEntry->mSize;
		}
	}
	return fileEntry->mData;
}

void* JKRCompArchive::fetchResource(void* data, u32 compressedSize,
                                    SDIFileEntry* fileEntry, u32* pSize)
{
	u32 size        = 0;
	u32 alignedSize = ALIGN_NEXT(fileEntry->mSize, 32);

	if (alignedSize == 0)
		OSPanic(__FILE__, 0x287, ":::bad resource size. size = 0\n");

	if (fileEntry->mData != nullptr) {
		if (alignedSize > ALIGN_PREV(compressedSize, 0x20))
			alignedSize = ALIGN_PREV(compressedSize, 0x20);

		JKRHeap::copyMemory(data, fileEntry->mData, alignedSize);

		size = alignedSize;
	} else {
		u32 flags       = fileEntry->mFlagsAndNameOffset >> 24;
		int compression = JKRConvertAttrToCompressionType((u8)flags);
		if (flags & 0x10) {
			if (alignedSize > ALIGN_PREV(compressedSize, 0x20))
				alignedSize = ALIGN_PREV(compressedSize, 0x20);

			if (!(flags & 0x4)) {
				JKRHeap::copyMemory(data, mArcFileData + fileEntry->mDataOffset,
				                    alignedSize);
			} else {
				u32 decompressedSize = JKRDecompExpandSize(
				    mArcFileData + fileEntry->mDataOffset);

				decompressedSize = decompressedSize > compressedSize
				                       ? compressedSize
				                       : decompressedSize;
				JKRDecompress(mArcFileData + fileEntry->mDataOffset, (u8*)data,
				              decompressedSize, 0);
			}

			size = JKRMemArchive::fetchResource_subroutine(
			    mArcFileData + fileEntry->mDataOffset, alignedSize, (u8*)data,
			    compressedSize, compression);
		} else if (flags & 0x20) {
			size = JKRAramArchive::fetchResource_subroutine(
			    fileEntry->mDataOffset + mAramPart->getAddress()
			        - mSizeOfMemPart,
			    alignedSize, (u8*)data, compressedSize, compression);
		} else if (flags & 0x40) {
			size = JKRDvdArchive::fetchResource_subroutine(
			    mEntryNum, _68 + fileEntry->mDataOffset, alignedSize, (u8*)data,
			    compressedSize, compression, mCompression);
		} else {
			OSPanic(__FILE__, 727, ":::CompArchive: bad mode.");
		}
	}

	if (pSize != nullptr)
		*pSize = size;

	return data;
}

void JKRCompArchive::removeResourceAll()
{
	if (mArcInfoBlock != nullptr && mMountMode != MOUNT_MEM) {
		SDIFileEntry* fileEntry = mFileEntries;
		for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
			int flags = fileEntry->mFlagsAndNameOffset >> 24;
			if (fileEntry->mData != nullptr) {
				if (!(flags & 0x10)) {
					JKRFreeToHeap(mHeap, fileEntry->mData);
				}

				fileEntry->mData = nullptr;
			}
		}
	}
}

bool JKRCompArchive::removeResource(void* resource)
{
	SDIFileEntry* fileEntry = findPtrResource(resource);
	if (!fileEntry)
		return false;

	int flags = fileEntry->mFlagsAndNameOffset >> 24;
	if (!(flags & 0x10)) {
		JKRFreeToHeap(mHeap, resource);
	}

	fileEntry->mData = nullptr;
	return true;
}
