#ifndef JASSYSTEMHEAP_HPP
#define JASSYSTEMHEAP_HPP

#include <dolphin/types.h>

class JKRSolidHeap;
class JKRHeap;

extern JKRHeap* JASDram;

namespace JASystem {
namespace Kernel {

	extern u32 audioDramSize;
	extern u32 audioAramSize;
	extern u32 audioAramTop;
	extern void* audioAramHeap;
	extern u32 CARD_SECURITY_BUFFER;
	extern u32 JASAramDmaBufferTop;

	void sysDramSetup(JKRSolidHeap* heap);
	void sysAramSetup(u32 size);
	void* allocFromSysDram(u32 size);
	void* allocFromSysAramFull(u32* size);
	u32 getSysDramRemain();
	void setSysDramSize(u32 size);
	u32 getSysDramSize();
	void* getSysDramTop();
	void setSysAramSize(u32 size);
	void* allocFromSysAram(u32 size);
	void freeToSysAramLast();
	u32 getSysAramRemain();
	u32 getSysAramSize();
	void* getSysAramTop();

} // namespace Kernel
} // namespace JASystem

#endif // JASSYSTEMHEAP_HPP
