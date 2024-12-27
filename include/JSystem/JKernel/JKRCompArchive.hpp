#ifndef JKR_COMP_ARCHIVE_HPP
#define JKR_COMP_ARCHIVE_HPP

#include <JSystem/JKernel/JKRArchive.hpp>

class JKRCompArchive : public JKRArchive {
public:
	JKRCompArchive(s32, EMountDirection);

	virtual ~JKRCompArchive();                                      // _08
	virtual void removeResourceAll();                               // _24
	virtual bool removeResource(void*);                             // _28
	virtual void* fetchResource(SDIFileEntry* entry, u32* outSize); // _40
	virtual void* fetchResource(void* resourceBuffer, u32 bufferSize,
	                            SDIFileEntry* entry, u32* resSize,
	                            JKRExpandSwitch expandSwitch); // _44

	bool open(s32);

	// Unused/inlined:
	void fixedInit(s32);
	void mountFixed(s32);
	void mountFixed(const char*);
	void unmountFixed();

	// _00     = VTBL
	// _00-_5C = JKRArchive
	u32 _60;                 // _60
	JKRAramBlock* mAramPart; // _64
	u32 _68;                 // _68
	JKRFile* mDvdFile;       // _6C
	u32 mSizeOfMemPart;      // _70
	u32 mSizeOfAramPart;     // _74
	u32 _78;                 // _78
};

#endif
