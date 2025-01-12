#include <JSystem/JUtility/JUTDirectFile.hpp>
#include <dolphin/os.h>
#include <dolphin/dvd.h>
#include <types.h>
#include <macros.h>

int JUTDirectFile::fetch32byte()
{
	mToRead = mLength - ALIGN_PREV(mPos, DVD_MIN_TRANSFER_SIZE);

	if (mToRead > JUTDF_BUFSIZE) {
		mToRead = JUTDF_BUFSIZE;
	}
	int interrupts = OSEnableInterrupts();
	int readRes    = DVDReadAsyncPrio(
        &mFileInfo, mSectorStart, ALIGN_NEXT(mToRead, DVD_MIN_TRANSFER_SIZE),
        ALIGN_PREV(mPos, DVD_MIN_TRANSFER_SIZE), nullptr, 2);
	OSRestoreInterrupts(interrupts);
	if (!readRes) {
		return -1;
	} else {
		interrupts = OSEnableInterrupts();
		while (DVDGetCommandBlockStatus(&mFileInfo.cb)) {
			;
		}
		OSRestoreInterrupts(interrupts);
		return mToRead;
	}
}

JUTDirectFile::JUTDirectFile()
{
	mLength      = 0;
	mPos         = 0;
	mToRead      = 0;
	mSectorStart = (u8*)ALIGN_NEXT((u32)mBuffer, DVD_MIN_TRANSFER_SIZE);
	mIsOpen      = false;
}

JUTDirectFile::~JUTDirectFile() { mIsOpen = false; }

bool JUTDirectFile::fopen(char* filename)
{
	if (!filename) {
		return false;
	}

	int interrupts = OSEnableInterrupts();
	int dvdRes     = DVDOpen(const_cast<char*>(filename), &mFileInfo);
	OSRestoreInterrupts(interrupts);

	if (!dvdRes) {
		mIsOpen = false;
		return false;
	}

	int interrupts2 = OSEnableInterrupts();
	mLength         = mFileInfo.length;
	OSRestoreInterrupts(interrupts2);

	mPos    = 0;
	mIsOpen = true;
	return true;
}

void JUTDirectFile::fclose()
{
	if (mIsOpen) {
		int interrupts = OSEnableInterrupts();
		DVDClose(&mFileInfo);
		OSRestoreInterrupts(interrupts);
		mIsOpen = false;
	}
}

int JUTDirectFile::fgets(void* buf, int len)
{
	if (!mIsOpen) {
		return -1;
	}

	if (len == 0) {
		return 0;
	}

	if (len == 1) {
		return 1;
	}

	if (!buf) {
		return -1;
	}

	if (mPos >= mLength) {
		return -1;
	}

	u8* byteBuf   = (u8*)buf;
	int readCount = 0;
	int readMax;

	while (readMax = len - 1, mPos < mLength) {
		if (mToRead == 0 && fetch32byte() < 0) {
			return -1;
		}

		u32 currPos   = mPos & (JUTDF_BUFSIZE - 1);
		u32 chunkSize = (mToRead - currPos);
		if (readCount + chunkSize > readMax) {
			chunkSize = len - readCount - 1;
		}

		BOOL isAtEnd = FALSE;
		for (int i = 0; i < chunkSize; i++) {
			u8 byte    = mSectorStart[currPos++];
			*byteBuf++ = byte;

			if (byte == '\n') {
				isAtEnd   = TRUE;
				chunkSize = i + 1;
				break;
			}
		}

		if (currPos >= JUTDF_BUFSIZE) {
			mToRead = 0;
		}

		if (isAtEnd == TRUE) {
			*byteBuf = 0;
			mPos += chunkSize;
			readCount += chunkSize;
			break;
		}

		mPos += chunkSize;
		readCount += chunkSize;

		if (readCount >= readMax) {
			*byteBuf = 0;
			break;
		}
	}

	if (mPos >= mLength) {
		*byteBuf = 0;
	}

	return readCount;
}
