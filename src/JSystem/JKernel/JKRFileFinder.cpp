#include <JSystem/JKernel/JKRFileFinder.hpp>
#include <JSystem/JKernel/JKRArchive.hpp>
#include <dolphin/dvd.h>

JKRArcFinder::JKRArcFinder(JKRArchive* archive, long startindex, long entries)
    : JKRFileFinder()
{
	mArchive = archive;

	mIsAvailable = entries > 0;
	mStartIndex  = startindex;
	mEndIndex    = startindex + entries - 1;
	mNextIndex   = mStartIndex;

	findNextFile();
}

bool JKRArcFinder::findNextFile()
{
	if (mIsAvailable) {
		mIsAvailable = (mNextIndex <= mEndIndex);
		if (mIsAvailable) {
			JKRArchive::SDirEntry dirEntry;
			mIsAvailable         = mArchive->getDirEntry(&dirEntry, mNextIndex);
			mBase.mFileName      = dirEntry.mName;
			mBase.mFileIndex     = mNextIndex;
			mBase.mFileID        = dirEntry.mID;
			mBase.mFileTypeFlags = dirEntry.mFlags;
			mIsDir               = FLAG_OFF(mBase.mFileTypeFlags, 2);
			mNextIndex++;
		}
	}
	return mIsAvailable;
}
