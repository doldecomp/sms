#ifndef JKR_DVD_ARCHIVE_HPP
#define JKR_DVD_ARCHIVE_HPP

#include <JSystem/JKernel/JKRArchive.hpp>

struct JKRDvdArchive : public JKRArchive {
	JKRDvdArchive();
	JKRDvdArchive(s32, JKRArchive::EMountDirection);

	virtual ~JKRDvdArchive();                                       // _00
	virtual void* fetchResource(SDIFileEntry* entry, u32* outSize); // _38
	virtual void* fetchResource(void* resourceBuffer, u32 bufferSize,
	                            SDIFileEntry* entry, u32* resSize,
	                            JKRExpandSwitch expandSwitch); // _3C

	bool open(s32);
	static u32 fetchResource_subroutine(s32, u32, u32, u8*, u32, int, int);
	static u32 fetchResource_subroutine(s32, u32, u32, JKRHeap*, int, int,
	                                    u8**);

	// _00     = VTBL
	// _00-_5C = JKRArchive
	int _60;           // _60
	JKRFile* mDvdFile; // _64
};

#endif
