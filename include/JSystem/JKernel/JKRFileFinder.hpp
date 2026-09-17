#ifndef JKR_FILE_FINDER_H
#define JKR_FILE_FINDER_H

#include <dolphin/dvd.h>
#include <dolphin/os.h>

class JKRArchive;

struct JKRFileFinderBase {
	/* 0x0 */ const char* mFileName;
	/* 0x4 */ int mFileIndex;
	/* 0x8 */ u16 mFileID;
	/* 0xA */ u16 mFileTypeFlags;
};

class JKRFileFinder {
public:
	JKRFileFinder()
	    : mIsAvailable(false)
	    , mIsDir(false)
	{
	}

	JKRFileFinderBase mBase;

	virtual ~JKRFileFinder() { }
	virtual bool findNextFile() = 0;

	bool isAvailable() const { return mIsAvailable; }

	/* 0x10 */ bool mIsAvailable;
	/* 0x11 */ bool mIsDir;
};

class JKRArcFinder : public JKRFileFinder {
public:
	JKRArcFinder(JKRArchive*, s32, s32);

	virtual ~JKRArcFinder() { }
	virtual bool findNextFile();

	/* 0x14 */ JKRArchive* mArchive;
	/* 0x18 */ s32 mStartIndex;
	/* 0x1C */ s32 mEndIndex;
	/* 0x20 */ s32 mNextIndex;
};

class JKRDvdFinder : public JKRFileFinder {
public:
	JKRDvdFinder(const char*);

	virtual ~JKRDvdFinder();
	virtual bool findNextFile();

	/* 0x14 */ DVDDir mDir;
	/* 0x20 */ bool mIsDvdOpen;
};

#endif
