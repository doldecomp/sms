#include <JSystem/JKernel/JKRAramArchive.hpp>
#include <JSystem/JKernel/JKRAram.hpp>
#include <JSystem/JKernel/JKRAramBlock.hpp>
#include <JSystem/JKernel/JKRAramHeap.hpp>
#include <JSystem/JKernel/JKRDecomp.hpp>
#include <JSystem/JKernel/JKRDvdFile.hpp>
#include <JSystem/JKernel/JKRDvdAramRipper.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include "string.h"
#include "dolphin/os.h"

JKRAramArchive::JKRAramArchive(s32 entryNumber,
                               JKRArchive::EMountDirection mountDirection)
    : JKRArchive(entryNumber, MOUNT_ARAM)
{
	mMountDirection = mountDirection;

	if (!this->open(entryNumber)) {
		return;
	}

	mVolumeType = 'RARC';
	mVolumeName = mStrTable + mDirectories->mOffset;
	JKRFileLoader::sVolumeList.prepend(&mFileLoaderLink);
	mIsMounted = true;
}

JKRAramArchive::~JKRAramArchive()
{
	if (mIsMounted == true) {
		if (mArcInfoBlock != nullptr) {
			SDIFileEntry* entry = mFileEntries;
			for (int i = 0; i < mArcInfoBlock->num_file_entries; entry++, i++) {
				if (entry->mData != nullptr) {
					JKRFreeToHeap(mHeap, entry->mData);
				}
			}

			JKRFreeToHeap(mHeap, mArcInfoBlock);
			mArcInfoBlock = nullptr;
		}

		if (mDvdFile != nullptr) {
			delete mDvdFile;
		}

		if (mBlock != nullptr) {
			delete mBlock;
		}

		JKRFileLoader::sVolumeList.remove(&mFileLoaderLink);
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

bool JKRAramArchive::open(s32 entryNum)
{
	mArcInfoBlock = nullptr;
	mDirectories  = nullptr;
	mFileEntries  = nullptr;
	mStrTable     = nullptr;
	mBlock        = nullptr;

	mDvdFile = new (JKRGetSystemHeap(),
	                mMountDirection == MOUNT_DIRECTION_HEAD ? 4 : -4)
	    JKRDvdFile(entryNum);
	if (mDvdFile == nullptr) {
		mMountMode = 0;
		return 0;
	}

	// NOTE: a different struct is used here for sure, unfortunately i can't get
	// any hits on this address, so gonna leave it like this for now
	SArcHeader* mem = (SArcHeader*)JKRAllocFromSysHeap(32, -32);
	if (mem == nullptr) {
		mMountMode = 0;
	} else {
		JKRDvdToMainRam(entryNum, (u8*)mem, EXPAND_SWITCH_DECOMPRESS, 32,
		                nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0,
		                &mCompression);
		int alignment   = mMountDirection == MOUNT_DIRECTION_HEAD ? 32 : -32;
		u32 alignedSize = ALIGN_NEXT(mem->file_data_offset, 32);
		mArcInfoBlock
		    = (SArcDataInfo*)JKRAllocFromHeap(mHeap, alignedSize, alignment);
		if (mArcInfoBlock == nullptr) {
			mMountMode = 0;
		} else {
			JKRDvdToMainRam(entryNum, (u8*)mArcInfoBlock,
			                EXPAND_SWITCH_DECOMPRESS, alignedSize, nullptr,
			                JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 32, nullptr);

			mDirectories = (SDIDirEntry*)((u8*)mArcInfoBlock
			                              + mArcInfoBlock->node_offset);
			mFileEntries = (SDIFileEntry*)((u8*)mArcInfoBlock
			                               + mArcInfoBlock->file_entry_offset);
			mStrTable    = (char*)((u8*)mArcInfoBlock
                                + mArcInfoBlock->string_table_offset);

			u8 compressedFiles
			    = 0; // maybe a check for if the last file is compressed?

			SDIFileEntry* fileEntry = mFileEntries;
			for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
				u8 flag = fileEntry->mFlag >> 24;
				if ((flag & 1)) {
					compressedFiles |= (flag & JKRARCHIVE_ATTR_COMPRESSION);
				}
				fileEntry++;
			}

			u32 aramSize = ALIGN_NEXT(mem->file_data_length, 32);
			mBlock       = (JKRAramBlock*)JKRAllocFromAram(
                aramSize, mMountDirection == MOUNT_DIRECTION_HEAD
			                        ? JKRAramHeap::HEAD
			                        : JKRAramHeap::TAIL);
			if (mBlock == nullptr) {
				if (mArcInfoBlock) {
					JKRFree(mArcInfoBlock);
				}

				mMountMode = 0;
			} else {
				JKRDvdToAram(entryNum, mBlock->getAddress(),
				             EXPAND_SWITCH_DECOMPRESS,
				             mem->header_length + mem->file_data_offset, 0);
			}
		}
	}
cleanup:
	if (mem != nullptr) {
		JKRFreeToSysHeap(mem);
	}
	if (mMountMode == 0) {
		OSReport(":::[%s: %d] Cannot alloc memory\n", __FILE__, 407);
		if (mDvdFile != nullptr) {
			delete mDvdFile;
		}
		return false;
	}
	return true;
}

void* JKRAramArchive::fetchResource(SDIFileEntry* pEntry, u32* pOutSize)
{
	JUT_ASSERT(VERSION_SELECT(434, 427, 427), isMounted());
	u32 outSize;
	u8* outBuf;
	if (pOutSize == nullptr) {
		pOutSize = &outSize;
	}

	JKRCompression compression
	    = JKRConvertAttrToCompressionType(pEntry->getFlags());
	if (pEntry->mData == nullptr) {
		u32 size = JKRAramArchive::fetchResource_subroutine(
		    pEntry->mDataOffset + mBlock->getAddress(), pEntry->mSize, mHeap,
		    compression, &outBuf);

		*pOutSize = size;
		if (size == nullptr) {
			return nullptr;
		}

		pEntry->mData = outBuf;
	} else {
		*pOutSize = pEntry->mSize;
	}

	return pEntry->mData;
}

void* JKRAramArchive::fetchResource(void* buffer, u32 bufferSize,
                                    SDIFileEntry* pEntry, u32* resourceSize)
{
	JUT_ASSERT(VERSION_SELECT(514, 500, 500), isMounted());
	u32 size = pEntry->mSize;
	if (size > bufferSize) {
		size = bufferSize;
	}
	JKRCompression compression
	    = JKRConvertAttrToCompressionType(pEntry->getFlags());
	if (pEntry->mData == nullptr) {
		bufferSize = (s32)ALIGN_PREV(bufferSize, 0x20);
		size       = JKRAramArchive::fetchResource_subroutine(
            pEntry->mDataOffset + mBlock->getAddress(), size, (u8*)buffer,
            bufferSize, compression);
	} else {
		if (size > bufferSize) {
			size = bufferSize;
		}
		JKRHeap::copyMemory(buffer, pEntry->mData, size);
	}
	if (resourceSize != nullptr) {
		*resourceSize = size;
	}
	return buffer;
}

u32 JKRAramArchive::fetchResource_subroutine(u32 srcAram, u32 srcLength,
                                             u8* dst, u32 dstLength,
                                             int compression)
{
	JUT_ASSERT(VERSION_SELECT(627, 613, 613), (srcAram & 0x1f) == 0);
	u32 outLen;
	u32 srcSize = ALIGN_NEXT(srcLength, 0x20);
	u32 dstSize = ALIGN_PREV(dstLength, 0x20);

	switch (compression) {
	case JKR_COMPRESSION_NONE:
		if (srcSize > dstSize) {
			srcSize = dstSize;
		}
		JKRAramToMainRam(srcAram, dst, srcSize, EXPAND_SWITCH_DEFAULT, dstSize,
		                 nullptr, -1, &outLen);
		return outLen;
	case JKR_COMPRESSION_YAY0:
	case JKR_COMPRESSION_YAZ0:
		JKRAramToMainRam(srcAram, dst, srcSize, EXPAND_SWITCH_DECOMPRESS,
		                 dstSize, nullptr, -1, &outLen);
		return outLen;
	default:
		OSPanic(__FILE__, 654, ":::??? bad sequence\n");
		return 0;
	}
}

u32 JKRAramArchive::fetchResource_subroutine(u32 entryNum, u32 length,
                                             JKRHeap* pHeap, int compression,
                                             u8** out)
{
	u32 alignedLen = ALIGN_NEXT(length, 0x20);

	u8* buffer;
	switch (compression) {
	case JKR_COMPRESSION_NONE: {
		buffer = (u8*)(JKRAllocFromHeap(pHeap, alignedLen, 0x20));
		JUT_ASSERT(VERSION_SELECT(676, 662, 662), buffer != nullptr);
		JKRAramToMainRam(entryNum, buffer, alignedLen, EXPAND_SWITCH_DEFAULT,
		                 alignedLen, nullptr, -1, nullptr);
		*out = buffer;
		return length;
	}
	case JKR_COMPRESSION_YAY0:
	case JKR_COMPRESSION_YAZ0: {
		u8 headerBuf[0x40];
		u8* alignHeader
		    = (u8*)ALIGN_NEXT((s32)&headerBuf[0], sizeof(SArcHeader));
		JKRAramToMainRam(entryNum, alignHeader, sizeof(SArcHeader),
		                 EXPAND_SWITCH_DEFAULT, 0, nullptr, -1, nullptr);
		u32 decompressedLen
		    = ALIGN_NEXT(JKRDecompExpandSize(alignHeader), sizeof(SArcHeader));
		buffer = (u8*)(JKRAllocFromHeap(pHeap, decompressedLen,
		                                sizeof(SArcHeader)));
		JUT_ASSERT(VERSION_SELECT(709, 688, 688), buffer);
		u32 readLen;
		JKRAramToMainRam(entryNum, buffer, alignedLen, EXPAND_SWITCH_DECOMPRESS,
		                 decompressedLen, pHeap, -1, &readLen);
		*out = buffer;
		return readLen;
	}
	default:
		OSPanic(__FILE__, 719, ":::??? bad sequence\n");
		return 0;
	}
}
