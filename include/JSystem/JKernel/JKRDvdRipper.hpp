#ifndef JKRDVDRIPPER_H
#define JKRDVDRIPPER_H

#include <JSystem/JKernel/JKREnum.hpp>
#include <JSystem/JSupport/JSUList.hpp>

struct SYaz0Header {
	u32 signature;
	u32 length;
};

class JKRDMCommand {
	JKRDMCommand();
	~JKRDMCommand();
};

class JKRHeap;
class JKRDvdFile;
class JKRDvdRipper {
public:
	static JSUList<JKRDMCommand> sDvdAsyncList;
	static u32 szpBufferSize;
	static bool errorRetry;

	enum EAllocDirection {
		DEFAULT_EALLOC_DIRECTION = 0,
		ALLOC_DIRECTION_FORWARD  = 1,
		ALLOC_DIRECTION_BACKWARD = 2,
	};

	static void setSzpBufferSize(u32 size) { szpBufferSize = size; }

	static void* loadToMainRAM(char const*, u8*, JKRExpandSwitch, u32, JKRHeap*,
	                           EAllocDirection, u32, int*);
	static void* loadToMainRAM(s32, u8*, JKRExpandSwitch, u32, JKRHeap*,
	                           EAllocDirection, u32, int*);
	static void* loadToMainRAM(JKRDvdFile*, u8*, JKRExpandSwitch, u32, JKRHeap*,
	                           EAllocDirection, u32, int*);

	static bool isErrorRetry(void) { return errorRetry; }
	inline static u32 getSzpBufferSize() { return szpBufferSize; }
};

// void JKRDecompressFromDVD(JKRDvdFile*, void*, u32, u32, u32, u32, u32*);

inline void* JKRDvdToMainRam(s32 entryNum, u8* dst,
                             JKRExpandSwitch expandSwitch, u32 dstLength,
                             JKRHeap* heap,
                             JKRDvdRipper::EAllocDirection allocDirection,
                             u32 offset, int* returnSize)
{
	return JKRDvdRipper::loadToMainRAM(entryNum, dst, expandSwitch, dstLength,
	                                   heap, allocDirection, offset,
	                                   returnSize);
}

inline void* JKRDvdToMainRam(const char* name, u8* dst,
                             JKRExpandSwitch expandSwitch, u32 dstLength,
                             JKRHeap* heap,
                             JKRDvdRipper::EAllocDirection allocDirection,
                             u32 offset, int* returnSize)
{
	return JKRDvdRipper::loadToMainRAM(name, dst, expandSwitch, dstLength, heap,
	                                   allocDirection, offset, returnSize);
}

#endif /* JKRDVDRIPPER_H */
