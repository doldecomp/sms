#ifndef JKR_COMP_ARCHIVE_HPP
#define JKR_COMP_ARCHIVE_HPP

#include <JSystem/JKernel/JKRArchive.hpp>

class JKRCompArchive : public JKRArchive {
public:
	JKRCompArchive(s32, EMountDirection);

	virtual ~JKRCompArchive();
	virtual void removeResourceAll();
	virtual bool removeResource(void*);
	virtual void* fetchResource(SDIFileEntry* entry, u32* outSize);
	virtual void* fetchResource(void* resourceBuffer, u32 bufferSize,
	                            SDIFileEntry* entry, u32* resSize);

	bool open(s32);

	void fixedInit(s32);
	void mountFixed(s32);
	void mountFixed(const char*);
	void unmountFixed();

private:
	/* 0x60 */ u8* mArcFileData;
	/* 0x64 */ JKRAramBlock* mAramPart;
	/* 0x68 */ u32 _68;
	/* 0x6C */ JKRFile* mDvdFile;
	/* 0x70 */ u32 mSizeOfMemPart;
	/* 0x74 */ u32 mSizeOfAramPart;
	/* 0x78 */ u32 _78;
};

#endif
