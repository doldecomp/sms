#ifndef JKR_ARAM_ARCHIVE_HPP
#define JKR_ARAM_ARCHIVE_HPP

#include <JSystem/JKernel/JKRArchive.hpp>

class JKRAramArchive : public JKRArchive {
public:
	JKRAramArchive();
	JKRAramArchive(s32, EMountDirection);

	virtual ~JKRAramArchive();
	virtual void* fetchResource(SDIFileEntry*, u32*);
	virtual void* fetchResource(void*, u32, SDIFileEntry*, u32*);

	bool open(s32, EMountDirection);
	u32 getAramAddress_Entry(SDIFileEntry* fileEntry);
	u32 getAramAddress(const char* file);
	u32 getAramAddress(u32, const char* file);
	u32 getAramAddress(u16 idx);
	u32 getIdxAramAddress(u32 idx);
	static u32 fetchResource_subroutine(u32, u32, u8*, u32, int);
	static u32 fetchResource_subroutine(u32, u32, JKRHeap*, int, u8**);

	void fixedInit(s32 entryNum);
	bool mountFixed(s32 entryNum);
	bool mountFixed(const char* path);
	void unmountFixed();

private:
	/* 0x60 */ JKRAramBlock* mBlock;
	/* 0x64 */ JKRFile* mDvdFile;
};

#endif
