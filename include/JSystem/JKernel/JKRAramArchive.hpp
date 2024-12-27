#ifndef JKR_ARAM_ARCHIVE_HPP
#define JKR_ARAM_ARCHIVE_HPP

#include <Jsystem/JKernel/JKRArchive.hpp>

class JKRAramArchive : public JKRArchive {
public:
	JKRAramArchive();
	JKRAramArchive(s32, EMountDirection);

	virtual ~JKRAramArchive();                        // _08
	virtual void* fetchResource(SDIFileEntry*, u32*); // _40
	virtual void* fetchResource(void*, u32, SDIFileEntry*, u32*,
	                            JKRExpandSwitch expandSwitch); // _44

	bool open(s32);
	u32 getAramAddress_Entry(SDIFileEntry* fileEntry);
	u32 getAramAddress(u32, const char* file);
	static u32 fetchResource_subroutine(u32, u32, u8*, u32, int);
	static u32 fetchResource_subroutine(u32, u32, JKRHeap*, int, u8**);

	void fixedInit(s32 entryNum, EMountDirection direction);
	bool mountFixed(s32 entryNum, EMountDirection direction);
	bool mountFixed(const char* path, EMountDirection direction);
	void unmountFixed();

	// _00     = VTBL
	// _00-_60 = JKRArchive
	JKRAramBlock* mBlock; // _60
	JKRFile* mDvdFile;    // _64
};

#endif
