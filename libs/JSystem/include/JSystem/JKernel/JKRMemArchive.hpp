#ifndef JKR_MEM_ARCHIVE_HPP
#define JKR_MEM_ARCHIVE_HPP

#include <JSystem/JKernel/JKRArchive.hpp>

class JKRMemArchive : public JKRArchive {
public:
	JKRMemArchive();
	JKRMemArchive(s32, EMountDirection);
	JKRMemArchive(void*, u32, JKRMemBreakFlag);
	JKRMemArchive(const char*, EMountDirection);

	virtual ~JKRMemArchive();
	virtual void removeResourceAll();
	virtual bool removeResource(void*);
	virtual void* fetchResource(SDIFileEntry* entry, u32* outSize);
	virtual void* fetchResource(void* resourceBuffer, u32 bufferSize,
	                            SDIFileEntry* entry, u32* resSize);

	bool open(s32, EMountDirection);
	bool open(void*, u32, JKRMemBreakFlag);
	static u32 fetchResource_subroutine(u8*, u32, u8*, u32, int);

	void fixedInit(s32);
	void mountFixed(s32, EMountDirection);
	void mountFixed(const char*, EMountDirection);
	bool mountFixed(void*, JKRMemBreakFlag);
	void unmountFixed();
	void open(const char*, EMountDirection);

private:
	/* 0x60 */ SArcHeader* mArcHeader;
	/* 0x64 */ u8* mArchiveData;
	/* 0x68 */ bool mIsOpen;
	/* 0x69 */ u8 field_0x6d[3];
};

#endif
