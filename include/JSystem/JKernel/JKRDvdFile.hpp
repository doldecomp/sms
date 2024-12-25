#ifndef JKR_DVD_FILE_H
#define JKR_DVD_FILE_H

#include <JSystem/JKernel/JKRFile.hpp>
#include <dolphin/os.h>
#include <dolphin/dvd.h>

class JKRAramBlock;
class JSUFileInputStream;
class JKRDvdFile;

struct JKRDvdFileInfo : DVDFileInfo {
	JKRDvdFile* mFile;
};

class JKRDvdFile : public JKRFile {
public:
	JKRDvdFile();
	JKRDvdFile(s32 entrynum);

	virtual ~JKRDvdFile();

	void initiate();

	virtual bool open(const char* filename);
	virtual bool close();
	virtual int readData(void* data, s32 length, s32 offset);
	virtual int writeData(const void* data, s32 length, s32 offset);
	virtual s32 getFileSize() const { return mDvdFileInfo.length; }
	virtual bool open(s32 entrynum);

	s32 sync();
	static void doneProcess(s32 result, DVDFileInfo* info);

	static JSUList<JKRDvdFile> sDvdList;

private:
	OSMutex mDvdMutex;
	OSMutex mAramMutex;
	JKRAramBlock* mAramBlock;
	OSThread* mAramThread;
	JSUFileInputStream* mInputStream;
	u32 _58;
	JKRDvdFileInfo mDvdFileInfo;
	OSMessageQueue mAramMessageQueue;
	OSMessage mAramMessage;
	OSMessageQueue mDvdMessageQueue;
	OSMessage mDvdMessage;
	JSULink<JKRDvdFile> mLink;
	OSThread* mDvdThread;
};

#endif
