#include <JSystem/JKernel/JKRDvdAramRipper.hpp>
#include <JSystem/JKernel/JKRAram.hpp>
#include <JSystem/JKernel/JKRAramPiece.hpp>
#include <JSystem/JKernel/JKRAramStream.hpp>
#include <JSystem/JKernel/JKRDecomp.hpp>
#include <JSystem/JKernel/JKRDvdFile.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JSupport/JSUFileInputStream.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <string.h>
#include <macros.h>

static int JKRDecompressFromDVDToAram(JKRDvdFile*, u32, u32, u32, u32, u32);
static int decompSZS_subroutine(u8*, u32);
static u8* firstSrcData();
static u8* nextSrcData(u8*);
static u32 dmaBufferFlush(u32);

JKRAramBlock* JKRDvdAramRipper::loadToAram(s32 entryNumber, u32 address,
                                           JKRExpandSwitch expandSwitch,
                                           u32 param_3, u32 param_4)
{
	JKRDvdFile dvdFile;
	if (!dvdFile.open(entryNumber)) {
		return nullptr;
	} else {
		return loadToAram(&dvdFile, address, expandSwitch, param_3, param_4);
	}
}

JKRAramBlock* JKRDvdAramRipper::loadToAram(JKRDvdFile* dvdFile, u32 address,
                                           JKRExpandSwitch expandSwitch,
                                           u32 param_3, u32 param_4)
{
	JKRADCommand* command = loadToAram_Async(dvdFile, address, expandSwitch,
	                                         nullptr, param_3, param_4);
	syncAram(command, 0);

	if (address) {
		delete command;
		return (JKRAramBlock*)-1;
	}

	JKRAramBlock* result = command->mBlock;
	delete command;
	return result;
}

bool JKRDvdAramRipper::errorRetry = true;

JKRADCommand* JKRDvdAramRipper::loadToAram_Async(JKRDvdFile* dvdFile,
                                                 u32 address,
                                                 JKRExpandSwitch expandSwitch,
                                                 void (*callback)(u32),
                                                 u32 param_4, u32 param_5)
{
	JKRADCommand* command  = new (JKRGetSystemHeap(), -4) JKRADCommand();
	command->mDvdFile      = dvdFile;
	command->mAddress      = address;
	command->mBlock        = nullptr;
	command->mExpandSwitch = expandSwitch;
	command->mCallback     = callback;
	command->mOffset       = param_4;
	command->field_0x18    = param_5;

	if (!callCommand_Async(command)) {
		delete command;
		return nullptr;
	}

	return command;
}

JSUList<JKRADCommand> JKRDvdAramRipper::sDvdAramAsyncList;

JKRADCommand* JKRDvdAramRipper::callCommand_Async(JKRADCommand* command)
{
	s32 compression;
	u32 uncompressedSize;
	bool bVar1          = true;
	JKRDvdFile* dvdFile = command->mDvdFile;
	compression         = 0;
	OSLockMutex(&dvdFile->mAramMutex);

	if (dvdFile->mAramThread) {
		bVar1 = false;
	} else {
		dvdFile->mAramThread = OSGetCurrentThread();
		JSUFileInputStream* stream
		    = new (JKRGetSystemHeap(), -4) JSUFileInputStream(dvdFile);
		dvdFile->mInputStream = stream;
		u32 fileSize          = dvdFile->getFileSize();
		if (command->field_0x18 && fileSize > command->field_0x18) {
			fileSize = command->field_0x18;
		}
		fileSize = ALIGN_NEXT(fileSize, 0x20);

		if (command->mExpandSwitch == 1) {
			u8 buffer[0x40];
			u8* bufPtr = (u8*)ALIGN_NEXT((u32)&buffer, 0x20);
			while (true) {
				if (DVDReadPrio(dvdFile->getFileInfo(), bufPtr, 0x20, 0, 2)
				    >= 0) {
					break;
				}

				if (errorRetry == 0) {
					delete stream;
					return nullptr;
				}

				VIWaitForRetrace();
			}

			compression      = JKRCheckCompressed(bufPtr);
			uncompressedSize = JKRDecompExpandSize(bufPtr);
			if (command->field_0x18 && uncompressedSize > command->field_0x18) {
				uncompressedSize = command->field_0x18;
			}
		}

		if (compression == 0) {
			command->mExpandSwitch = EXPAND_SWITCH_DEFAULT;
		}

		if (command->mExpandSwitch == EXPAND_SWITCH_DECOMPRESS) {
			if (command->mAddress == 0 && command->mBlock == nullptr) {
				command->mBlock
				    = JKRAllocFromAram(uncompressedSize, JKRAramHeap::HEAD);
				if (command->mBlock) {
					command->mAddress = command->mBlock->getAddress();
				}
				dvdFile->mAramBlock = command->mBlock;
			}

			if (command->mBlock) {
				command->mAddress = command->mBlock->getAddress();
			}

			if (command->mAddress == 0) {
				dvdFile->mAramThread = nullptr;
				return nullptr;
			}
		} else {
			if (command->mAddress == 0 && !command->mBlock) {
				command->mBlock = JKRAram::getAramHeap()->alloc(
				    fileSize, JKRAramHeap::HEAD);
			}

			if (command->mBlock) {
				command->mAddress = command->mBlock->getAddress();
			}

			if (command->mAddress == 0) {
				dvdFile->mAramThread = nullptr;
				return nullptr;
			}
		}

		if (compression == 0) {
			command->mStreamCommand = JKRStreamToAram_Async(
			    stream, command->mAddress, fileSize - command->mOffset,
			    command->mOffset, nullptr);
		} else if (compression == 1) {
			command->mStreamCommand = JKRStreamToAram_Async(
			    stream, command->mAddress, fileSize - command->mOffset,
			    command->mOffset, nullptr);
		} else if (compression == 2) {
			command->mStreamCommand = nullptr;
			JKRDecompressFromDVDToAram(command->mDvdFile, command->mAddress,
			                           fileSize, uncompressedSize,
			                           command->mOffset, 0);
		}

		if (!command->mCallback) {
			sDvdAramAsyncList.append(&command->mLink);
		} else {
			command->mCallback((u32)command);
		}
	}

	OSUnlockMutex(&dvdFile->mAramMutex);
	return bVar1 == true ? command : nullptr;
}

bool JKRDvdAramRipper::syncAram(JKRADCommand* command, int param_1)
{
	JKRDvdFile* dvdFile = command->mDvdFile;
	OSLockMutex(&dvdFile->mAramMutex);

	if (command->mStreamCommand) {
		JKRAramStreamCommand* var1
		    = JKRAramStream::sync(command->mStreamCommand, param_1);

		if (param_1 != 0 && var1 == nullptr) {
			OSUnlockMutex(&dvdFile->mAramMutex);
			return false;
		}
	}

	sDvdAramAsyncList.remove(&command->mLink);
	if (command->mStreamCommand) {
		delete command->mStreamCommand;
	}

	delete dvdFile->mInputStream;
	dvdFile->mAramThread = nullptr;
	OSUnlockMutex(&dvdFile->mAramMutex);
	return true;
}

JKRADCommand::JKRADCommand()
    : mLink(this)
{
	field_0x48 = false;
}

JKRADCommand::~JKRADCommand()
{
	if (field_0x48 == true) {
		delete mDvdFile;
	}
}

u32 JKRDvdAramRipper::szpBufferSize = 0x00000400;
static u8* szpBuf;
static u8* szpEnd;
static u8* refBuf;
static u8* refEnd;
static u8* refCurrent;
u8* dmaBuf;
u8* dmaEnd;
u8* dmaCurrent;
static u32 srcOffset;
static u32 transLeft;
static u8* srcLimit;
static JKRDvdFile* srcFile;
static u32 fileOffset;
static int readCount;
static u32 maxDest;
static bool isInitMutex;

static int JKRDecompressFromDVDToAram(JKRDvdFile* dvdFile, u32 param_1,
                                      u32 fileSize, u32 uncompressedSize,
                                      u32 param_4, u32 param_5)
{
	int result = 0;
	szpBuf
	    = (u8*)JKRAllocFromSysHeap(JKRDvdAramRipper::getSzpBufferSize(), 0x20);
	szpEnd     = szpBuf + JKRDvdAramRipper::getSzpBufferSize();
	refBuf     = (u8*)JKRAllocFromSysHeap(0x1120, 0);
	refEnd     = refBuf + 0x1120;
	refCurrent = refBuf;
	dmaBuf     = (u8*)JKRAllocFromSysHeap(0x100, 0x20);
	dmaEnd     = dmaBuf + 0x100;
	dmaCurrent = dmaBuf;
	srcFile    = dvdFile;
	srcOffset  = param_5;
	transLeft  = fileSize - param_5;
	fileOffset = param_4;
	readCount  = 0;
	maxDest    = uncompressedSize;
	u8* first  = firstSrcData();
	if (first)
		result = decompSZS_subroutine(first, param_1);
	JKRHeap::free(szpBuf, 0);
	JKRHeap::free(refBuf, 0);
	JKRHeap::free(dmaBuf, 0);
	return result;
}

static int decompSZS_subroutine(u8* src, u32 dest)
{
	u32 endAddr;
	u8* copySource;
	s32 validBitCount = 0;
	u32 currCodeByte  = 0;
	s32 numBytes;
	u32 startDest = dest;

	if (src[0] != 'Y' || src[1] != 'a' || src[2] != 'z' || src[3] != '0') {
		return -1;
	}

	endAddr = dest + (((SYaz0Header*)src)->length - fileOffset);
	if (endAddr > dest + maxDest) {
		endAddr = dest + maxDest;
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
			if (readCount >= fileOffset) {
				*(dmaCurrent++) = *src;
				dest++;
				if (dmaCurrent == dmaEnd) {
					startDest += dmaBufferFlush(startDest);
				}
				if (dest == endAddr) {
					break;
				}
			}
			*(refCurrent++) = *src;
			if (refCurrent == refEnd) {
				refCurrent = refBuf;
			}
			src++;
			readCount++;
		} else {
			u32 dist = ((src[0] & 0x0F) << 8) | src[1];
			numBytes = src[0] >> 4;
			src += 2;
			copySource = refCurrent - dist - 1;
			if (copySource < refBuf) {
				copySource += refEnd - refBuf;
			}
			if (numBytes == 0) {
				numBytes = *(src++);
				numBytes += 0x12;
			} else {
				numBytes += 2;
			}
			do {
				if (readCount >= fileOffset) {
					*(dmaCurrent++) = *copySource;
					dest++;
					if (dmaCurrent == dmaEnd) {
						startDest += dmaBufferFlush(startDest);
					}
					if (dest == endAddr) {
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
		}
		currCodeByte <<= 1;
		validBitCount--;
	} while (dest < endAddr);
	dmaBufferFlush(startDest);
	return 0;
}

static u8* firstSrcData()
{
	srcLimit    = szpEnd - 0x19;
	u8* buffer  = szpBuf;
	u32 bufSize = szpEnd - buffer;
	u32 length  = transLeft < bufSize ? transLeft : bufSize;
	while (true) {
		int result = DVDReadPrio(srcFile->getFileInfo(), buffer, length, 0, 2);
		if (result >= 0) {
			break;
		}
		if (JKRDvdAramRipper::errorRetry == 0) {
			return nullptr;
		}
		VIWaitForRetrace();
	}
	srcOffset += length;
	transLeft -= length;
	return buffer;
}

static u8* nextSrcData(u8* src)
{
	u32 size = szpEnd - src;
	u8* dest;
	if (IS_NOT_ALIGNED(size, 0x20))
		dest = szpBuf + 0x20 - (size & (0x20 - 1));
	else
		dest = szpBuf;
	memcpy(dest, src, size);
	dest += size;
	u32 transSize = szpEnd - dest;
	if (transSize > transLeft) {
		transSize = transLeft;
	}
	while (true) {
		s32 result = DVDReadPrio(srcFile->getFileInfo(), dest, transSize,
		                         srcOffset, 2);
		if (result >= 0) {
			break;
		}
		if (JKRDvdAramRipper::errorRetry == 0) {
			return nullptr;
		}
		VIWaitForRetrace();
	}
	srcOffset += transSize;
	transLeft -= transSize;
	if (transLeft == 0) {
		srcLimit = dest + transSize;
	}
	return dest;
}

static u32 dmaBufferFlush(u32 param_1)
{
	if (dmaCurrent == dmaBuf) {
		return 0;
	}
	u32 size = ALIGN_NEXT(dmaCurrent - dmaBuf, 0x20);
	JKRAramPiece::orderSync(0, (u32)dmaBuf, param_1, size, nullptr);
	dmaCurrent = dmaBuf;
	return size;
}
