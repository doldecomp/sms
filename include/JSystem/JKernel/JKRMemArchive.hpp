#ifndef JKR_MEM_ARCHIVE_HPP
#define JKR_MEM_ARCHIVE_HPP

#include <Jsystem/JKernel/JKRArchive.hpp>

class JKRMemArchive : public JKRArchive {
public:
	JKRMemArchive(); // unused/inlined
	JKRMemArchive(s32, EMountDirection);
	JKRMemArchive(void*, u32, JKRMemBreakFlag);
	JKRMemArchive(const char*, EMountDirection); // unused/inlined

	virtual ~JKRMemArchive();                                       // _08
	virtual void removeResourceAll();                               // _24
	virtual bool removeResource(void*);                             // _28
	virtual void* fetchResource(SDIFileEntry* entry, u32* outSize); // _40
	virtual void* fetchResource(void* resourceBuffer, u32 bufferSize,
	                            SDIFileEntry* entry, u32* resSize); // _44

	bool open(s32, EMountDirection);
	bool open(void*, u32, JKRMemBreakFlag);
	static u32 fetchResource_subroutine(u8*, u32, u8*, u32, int);

	void fixedInit(s32);
	void mountFixed(s32, EMountDirection);
	void mountFixed(const char*, EMountDirection);
	bool mountFixed(void*, JKRMemBreakFlag);
	void unmountFixed();
	void open(const char*, EMountDirection);

	// _00     = VTBL
	// _00-_60 = JKRArchive
	SArcHeader* mArcHeader; // _60
	u8* mArchiveData;       // _64
	bool mIsOpen;           // _68
	u8 field_0x6d[3];       // _69
};

#endif
