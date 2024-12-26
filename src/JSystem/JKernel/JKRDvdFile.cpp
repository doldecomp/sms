#include <JSystem/JKernel/JKRDvdFile.hpp>
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <macros.h>

JSUList<JKRDvdFile> JKRDvdFile::sDvdList;

JKRDvdFile::JKRDvdFile()
    : mLink(this)
{
	initiate();
}

JKRDvdFile::JKRDvdFile(s32 entrynum)
    : mLink(this)
{
	initiate();
	mFileOpen = open(entrynum);

	if (isAvailable()) {
		return;
	}
}

JKRDvdFile::~JKRDvdFile() { close(); }

void JKRDvdFile::initiate()
{
	/* Reference to self. Used to retrieve reference in the DVDReadAsync
	 * DVDCallback func. */
	mDvdFileInfo.mFile = this;
	OSInitMutex(&mDvdMutex);
	OSInitMutex(&mAramMutex);
	OSInitMessageQueue(&mDvdMessageQueue, &mDvdMessage, 1);
	OSInitMessageQueue(&mAramMessageQueue, &mAramMessage, 1);
	mDvdThread  = nullptr;
	mAramThread = nullptr;
	_58         = 0;
}

bool JKRDvdFile::open(const char* filename)
{
	if (mFileOpen == false) {
		mFileOpen = DVDOpen((char*)filename, &mDvdFileInfo);
		if (mFileOpen) {
			sDvdList.append(&mLink);
			DVDGetCommandBlockStatus(&mDvdFileInfo.cb);
		}
	}

	return mFileOpen;
}

bool JKRDvdFile::open(s32 entrynum)
{
	if (mFileOpen == false) {
		mFileOpen = DVDFastOpen(entrynum, &mDvdFileInfo);
		if (mFileOpen) {
			sDvdList.append(&mLink);
			DVDGetCommandBlockStatus(&mDvdFileInfo.cb);
		}
	}

	return mFileOpen;
}

bool JKRDvdFile::close()
{
	if (mFileOpen) {
		if (DVDClose(&mDvdFileInfo)) {
			mFileOpen = false;
			return sDvdList.remove(&mLink);
		} else {
			OSErrorLine(
			    212, "cannot close DVD file\n"); /* JKRDvdFile.cpp line 212 */
		}
	}
}

int JKRDvdFile::readData(void* data, s32 length, s32 ofs)
{
	length = ALIGN_NEXT(length, 32);

	OSLockMutex(&mDvdMutex);
	int ret;

	if (mDvdThread != nullptr) {
		OSUnlockMutex(&mDvdMutex);
		return -1;
	} else {
		mDvdThread = OSGetCurrentThread();
		ret        = -1;
		if (DVDReadAsync(&mDvdFileInfo, data, length, ofs,
		                 JKRDvdFile::doneProcess)) {
			ret = sync();
		}

		mDvdThread = nullptr;
		OSUnlockMutex(&mDvdMutex);
	}

	return ret;
}

int JKRDvdFile::writeData(const void*, s32, s32) { return -1; }

s32 JKRDvdFile::sync()
{
	OSMessage m;

	OSLockMutex(&mDvdMutex);
	OSReceiveMessage(&mDvdMessageQueue, &m, OS_MESSAGE_BLOCK);
	mDvdThread = nullptr;
	OSUnlockMutex(&mDvdMutex);
	return (s32)m;
}

void JKRDvdFile::doneProcess(s32 result, DVDFileInfo* info)
{
	OSSendMessage(&static_cast<JKRDvdFileInfo*>(info)->mFile->mDvdMessageQueue,
	              (OSMessage)result, OS_MESSAGE_NOBLOCK);
}
