#include <JSystem/JKernel/JKRAram.hpp>
#include <JSystem/JKernel/JKRAramPiece.hpp>
#include <JSystem/JKernel/JKRAramStream.hpp>
#include <JSystem/JKernel/JKRDecomp.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JKernel/JKRDvdRipper.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <dolphin/ar.h>
#include <dolphin/os.h>
#include "string.h"
#include "macros.h"

static int JKRDecompressFromAramToMainRam(u32, void*, u32, u32, u32);
static int decompSZS_subroutine(u8*, u8*);
static u8* firstSrcData();
static u8* nextSrcData(u8*);

JKRAram* JKRAram::sAramObject;

/* 802B42C4-802B4360       .text create__7JKRAramFUlUllll */
JKRAram* JKRAram::create(u32 aram_audio_buffer_size, u32 aram_audio_graph_size,
                         s32 stream_priority, s32 decomp_priority,
                         s32 piece_priority)
{
	if (!sAramObject) {
		sAramObject = new (JKRHeap::getSystemHeap(), 0) JKRAram(
		    aram_audio_buffer_size, aram_audio_graph_size, piece_priority);
	}

	JKRCreateAramStreamManager(stream_priority);
	JKRCreateDecompManager(decomp_priority);
	sAramObject->resume();
	return sAramObject;
}

OSMessage JKRAram::sMessageBuffer[4]  = { nullptr, nullptr, nullptr, nullptr };
OSMessageQueue JKRAram::sMessageQueue = { 0 };

JKRAram::JKRAram(u32 audio_buffer_size, u32 audio_graph_size, s32 priority)
    : JKRThread(0x4000, 0x10, priority)
{
	u32 aramBase = ARInit(mStackArray, ARRAY_COUNT(mStackArray));
	ARQInit();

	u32 aramSize = ARGetSize();

	mAudioMemorySize = audio_buffer_size;
	if (audio_graph_size == 0xFFFFFFFF) {
		mGraphMemorySize = (aramSize - audio_buffer_size) - aramBase;
		mAramMemorySize  = 0;
	} else {
		mGraphMemorySize = audio_graph_size;
		mAramMemorySize
		    = (aramSize - (audio_buffer_size + audio_graph_size)) - aramBase;
	}

	mAudioMemoryPtr = ARAlloc(mAudioMemorySize);
	mGraphMemoryPtr = ARAlloc(mGraphMemorySize);

	if (mAramMemorySize) {
		mAramMemoryPtr = ARAlloc(mAramMemorySize);
	} else {
		mAramMemoryPtr = nullptr;
	}

	mAramHeap = new (JKRHeap::getSystemHeap(), 0)
	    JKRAramHeap(mGraphMemoryPtr, mGraphMemorySize);
}

JKRAram::~JKRAram()
{
	sAramObject = nullptr;
	if (mAramHeap) {
		delete mAramHeap;
	}
}

void* JKRAram::run()
{
	int result;
	JKRAMCommand* command;
	JKRAramPiece::Message* message;
	OSInitMessageQueue(&sMessageQueue, sMessageBuffer, 4);
	do {
		OSReceiveMessage(&sMessageQueue, (OSMessage*)&message,
		                 OS_MESSAGE_BLOCK);
		result  = message->field_0x00;
		command = message->command;
		delete message;

		switch (result) {
		case 1:
			JKRAramPiece::startDMA(command);
			break;
		}
	} while (true);
}

bool JKRAram::checkOkAddress(u8* addr, u32 size, JKRAramBlock* block,
                             u32 param_4)
{
	if (!IS_ALIGNED((u32)addr, 0x20) && !IS_ALIGNED(size, 0x20)) {
		OSPanic(__FILE__, 225, ":::address not 32Byte aligned.");
		return false;
	}

	if (block && !IS_ALIGNED((u32)block->getAddress() + param_4, 0x20)) {
		OSPanic(__FILE__, 234, ":::address not 32Byte aligned.");
		return false;
	}
	return true;
}

void JKRAram::changeGroupIdIfNeed(u8* data, int groupId)
{
	JKRHeap* currentHeap = JKRHeap::getCurrentHeap();
	if (currentHeap->getHeapType() == 'EXPH' && groupId >= 0) {
		JKRExpHeap::CMemBlock* block = JKRExpHeap::CMemBlock::getBlock(data);
		block->newGroupId(groupId);
	}
}

JKRAramBlock* JKRAram::mainRamToAram(u8* buf, u32 bufSize, u32 alignedSize,
                                     JKRExpandSwitch expandSwitch, u32 fileSize,
                                     JKRHeap* heap, int id)
{
	JKRAramBlock* block = nullptr;
	checkOkAddress(buf, bufSize, nullptr, 0);
	if (expandSwitch == EXPAND_SWITCH_DECOMPRESS) {
		expandSwitch = (JKRCheckCompressed(buf) != JKR_COMPRESSION_NONE)
		                   ? EXPAND_SWITCH_DECOMPRESS
		                   : EXPAND_SWITCH_DEFAULT;
	}
	if (expandSwitch == EXPAND_SWITCH_DECOMPRESS) {
		u32 expandSize = JKRCheckCompressed(buf) != JKR_COMPRESSION_NONE
		                     ? JKRDecompExpandSize(buf)
		                     : 0;
		if (fileSize == 0 || fileSize > expandSize) {
			fileSize = expandSize;
		}
		if (bufSize == 0) {
			block = JKRAllocFromAram(fileSize, JKRAramHeap::HEAD);
			if (block == nullptr)
				return nullptr;

			block->newGroupID(decideAramGroupId(id));
			bufSize = block->getAddress();
		}
		if (alignedSize == 0 || alignedSize > expandSize)
			alignedSize = expandSize;

		if (fileSize > alignedSize)
			fileSize = alignedSize;

		void* allocatedMem = JKRAllocFromHeap(heap, fileSize, -32);
		if (allocatedMem == nullptr) {
			if (block != nullptr) {
				JKRFreeToAram(block);
			}
			block = nullptr;
		} else {
			JKRDecompress(buf, (u8*)allocatedMem, fileSize, 0);
			JKRAramPcs(0, (u32)allocatedMem, bufSize, alignedSize, block);
			JKRFreeToHeap(heap, allocatedMem);
			block = block == nullptr ? (JKRAramBlock*)-1 : block;
		}
	} else {
		if (bufSize == 0) {

			JKRAramBlock* allocatedBlock = (JKRAramBlock*)JKRAllocFromAram(
			    alignedSize, JKRAramHeap::HEAD);
			block = allocatedBlock;
			block->newGroupID(decideAramGroupId(id));
			if (block == nullptr)
				return nullptr;

			bufSize = allocatedBlock->getAddress();
		}

		JKRAramPcs(0, (u32)buf, bufSize, alignedSize, block);
		block = block == nullptr ? (JKRAramBlock*)-1 : block;
	}
	return block;
}

JKRAramBlock* JKRAram::mainRamToAram(u8* buf, JKRAramBlock* block,
                                     u32 alignedSize,
                                     JKRExpandSwitch expandSwitch, u32 fileSize,
                                     JKRHeap* heap, int id)
{
	checkOkAddress(buf, 0, block, 0);
	if (!block) {
		return mainRamToAram(buf, u32(0), alignedSize, expandSwitch, fileSize,
		                     heap, id);
	}
	u32 blockSize = block->mSize;
	if (expandSwitch == 1) {
		fileSize = fileSize >= blockSize ? blockSize : fileSize;
	}
	return mainRamToAram(buf, block->mAddress,
	                     alignedSize > blockSize ? blockSize : alignedSize,
	                     expandSwitch, fileSize, heap, id);
}

u8* JKRAram::aramToMainRam(u32 address, u8* buf, u32 p3,
                           JKRExpandSwitch expandSwitch, u32 p5, JKRHeap* heap,
                           int id, u32* pSize)
{
	JKRCompression compression = JKR_COMPRESSION_NONE;
	if (pSize != nullptr)
		*pSize = 0;

	checkOkAddress(buf, address, nullptr, 0);

	u32 expandSize;
	if (expandSwitch == EXPAND_SWITCH_DECOMPRESS) {
		u8 buffer[64];
		u8* bufPtr = (u8*)ALIGN_NEXT((u32)buffer, 32);
		JKRAramPcs(1, address, (u32)bufPtr, sizeof(buffer) / 2,
		           nullptr); // probably change sizeof(buffer) / 2 to 32
		compression = JKRCheckCompressed(bufPtr);
		expandSize  = JKRDecompExpandSize(bufPtr);
	}

	if (compression == JKR_COMPRESSION_YAZ0) { // SZS
		if (p5 != 0 && p5 < expandSize)
			expandSize = p5;

		if (buf == nullptr)
			buf = (u8*)JKRAllocFromHeap(heap, expandSize, 32);

		if (buf == nullptr) {
			return nullptr;
		} else {
			changeGroupIdIfNeed(buf, id);
			JKRDecompressFromAramToMainRam(address, buf, p3, expandSize, 0);
			DCStoreRange(buf, expandSize);
			if (pSize != nullptr) {
				*pSize = expandSize;
			}
			return buf;
		}
	} else if (compression == JKR_COMPRESSION_YAY0) { // SZP
		u8* szpSpace = (u8*)JKRAllocFromHeap(heap, p3, -32);
		if (szpSpace == nullptr) {
			return nullptr;
		} else {
			JKRAramPcs(1, address, (u32)szpSpace, p3, nullptr);
			if (p5 != 0 && p5 < expandSize)
				expandSize = p5;

			if (buf == nullptr)
				buf = (u8*)JKRAllocFromHeap(heap, expandSize, 32);

			if (buf == nullptr) {
				JKRFree(szpSpace);
				return nullptr;
			} else {
				changeGroupIdIfNeed(buf, id);
				JKRDecompress(szpSpace, buf, expandSize, 0);
				JKRFreeToHeap(heap, szpSpace);
				if (pSize != nullptr) {
					*pSize = expandSize;
				}
				return buf;
			}
		}
	} else { // Not compressed or ASR
		if (buf == nullptr)
			buf = (u8*)JKRAllocFromHeap(heap, p3, 32);

		if (buf == nullptr) {
			return nullptr;
		} else {
			changeGroupIdIfNeed(buf, id);
			JKRAramPcs(1, address, (u32)buf, p3, nullptr);
			if (pSize != nullptr) {
				*pSize = p3;
			}
			return buf;
		}
	}
}

u8* JKRAram::aramToMainRam(JKRAramBlock* block, u8* buf, u32 p3, u32 p4,
                           JKRExpandSwitch expandSwitch, u32 p6, JKRHeap* heap,
                           int id, u32* pSize)
{
	if (pSize) {
		*pSize = 0;
	}
	checkOkAddress(buf, 0, block, p4);
	if (!block) {
		OSPanic(__FILE__, 673, ":::Bad Aram Block specified.\n");
	}
	if (p4 >= block->mSize) {
		return nullptr;
	}
	p3 = p3 == 0 ? block->mSize : p3;
	if (p4 + p3 > block->mSize) {
		p3 = block->mSize - p4;
	}
	return aramToMainRam(p4 + block->mAddress, buf, p3, expandSwitch, p6, heap,
	                     id, pSize);
}

static void dummy()
{
	OSReport(
	    "---------------- BAD SYNC. you'd set callback, but now call sync.\n");
}
JSUList<JKRAMCommand> JKRAram::sAramCommandList;
u32 JKRAram::szpBufferSize = 0x00000400;
static u8* szpBuf;
static u8* szpEnd;
static u8* refBuf;
static u8* refEnd;
static u8* refCurrent;
static u32 srcOffset;
static u32 transLeft;
static u8* srcLimit;
static u32 srcAddress;
static u32 fileOffset;
static u32 readCount;
static u32 maxDest;

static int JKRDecompressFromAramToMainRam(u32 src, void* dst, u32 srcLength,
                                          u32 dstLength, u32 offset)
{
	// TODO: what is 0x400?
	szpBuf = (u8*)JKRAllocFromSysHeap(0x400, 32);
	JUT_ASSERT(VERSION_SELECT(1091, 1077, 1077), szpBuf != nullptr);

	szpEnd = szpBuf + 0x400;
	if (offset != 0) {
		refBuf = (u8*)JKRAllocFromSysHeap(0x1120, 0);
		JUT_ASSERT(VERSION_SELECT(1100, 1086, 1086), refBuf != nullptr);
		refEnd     = refBuf + 0x1120;
		refCurrent = refBuf;
	} else {
		refBuf = nullptr;
	}
	srcAddress = src;
	srcOffset  = 0;
	transLeft  = (srcLength != 0) ? srcLength : -1;
	fileOffset = offset;
	readCount  = 0;
	maxDest    = dstLength;

	u8* data             = firstSrcData();
	u32 decompressedSize = ((u32*)data)[1];
	decompSZS_subroutine(data, (u8*)dst);
	JKRFree(szpBuf);
	if (refBuf) {
		JKRFree(refBuf);
	}

	return 0;
}

static int decompSZS_subroutine(u8* src, u8* dest)
{
	s32 validBitCount = 0;
	s32 currCodeByte  = 0;

	if (src[0] != 'Y' || src[1] != 'a' || src[2] != 'z' || src[3] != '0') {
		return -1;
	}

	u8* endPtr = dest + (((SYaz0Header*)src)->length - fileOffset);
	if (endPtr > dest + maxDest) {
		endPtr = dest + maxDest;
	}

	src += 0x10;
	do {
		if (validBitCount == 0) {
			if ((src > srcLimit) && transLeft) {
				src = nextSrcData(src);
			}
			currCodeByte  = *src;
			validBitCount = 8;
			src++;
		}
		if (currCodeByte & 0x80) {
			if (fileOffset != 0) {
				if (readCount >= fileOffset) {
					*dest = *src;
					dest++;
					if (dest == endPtr) {
						break;
					}
				}
				*(refCurrent++) = *src;
				if (refCurrent == refEnd) {
					refCurrent = refBuf;
				}
				src++;
			} else {
				*dest = *src;
				dest++;
				src++;
				if (dest == endPtr) {
					break;
				}
			}
			readCount++;
		} else {
			u32 dist     = ((src[0] & 0x0f) << 8) | src[1];
			s32 numBytes = src[0] >> 4;
			src += 2;
			u8* copySource;
			if (fileOffset != 0) {
				copySource = refCurrent - dist - 1;
				if (copySource < refBuf) {
					copySource += refEnd - refBuf;
				}
			} else {
				copySource = dest - dist - 1;
			}
			if (numBytes == 0) {
				numBytes = *src + 0x12;
				src += 1;
			} else {
				numBytes += 2;
			}
			if (fileOffset != 0) {
				do {
					if (readCount >= fileOffset) {
						*dest = *copySource;
						dest++;
						if (dest == endPtr) {
							break;
						}
					}
					*(refCurrent++) = *copySource;
					if (refCurrent == refEnd) {
						refCurrent = refBuf;
					}
					copySource++;
					if (copySource == refEnd) {
						copySource = refBuf;
					}
					readCount++;
					numBytes--;
				} while (numBytes != 0);
			} else {
				do {
					*dest = *copySource;
					dest++;
					if (dest == endPtr) {
						break;
					}
					readCount++;
					numBytes--;
					copySource++;
				} while (numBytes != 0);
			}
		}
		currCodeByte <<= 1;
		validBitCount--;
	} while (dest < endPtr);
	return 0;
}

static u8* firstSrcData()
{
	srcLimit   = szpEnd - 0x19;
	u8* buffer = szpBuf;

	u32 size   = szpEnd - buffer;
	u32 length = transLeft < size ? transLeft : size;

	JKRAramPcs(1, (u32)(srcAddress + srcOffset), (u32)buffer,
	           ALIGN_NEXT(length, 0x20), nullptr);

	srcOffset += length;
	transLeft -= length;

	return buffer;
}

static u8* nextSrcData(u8* current)
{
	u8* dest;
	u32 left = (u32)(szpEnd - current);
	if (IS_NOT_ALIGNED(left, 0x20)) {
		dest = szpBuf + 0x20 - (left & (0x20 - 1));
	} else {
		dest = szpBuf;
	}

	memcpy(dest, current, left);
	u32 transSize = (u32)(szpEnd - (dest + left));
	if (transSize > transLeft) {
		transSize = transLeft;
	}
	JUT_ASSERT(VERSION_SELECT(1376, 1361, 1361), transSize > 0);

	JKRAramPcs(1, (u32)(srcAddress + srcOffset), ((u32)dest + left),
	           ALIGN_NEXT(transSize, 0x20), nullptr);
	srcOffset += transSize;
	transLeft -= transSize;

	if (transLeft == 0)
		srcLimit = (dest + left) + transSize;

	return dest;
}
