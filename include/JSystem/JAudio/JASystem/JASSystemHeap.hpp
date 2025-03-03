#ifndef JASSYSTEMHEAP_HPP
#define JASSYSTEMHEAP_HPP

#include "types.h"

class JKRSolidHeap;

namespace JASystem {
namespace Kernel {

	extern unsigned long audioDramSize;
	extern unsigned long audioAramSize;
	extern unsigned long audioAramTop;
	extern void* audioAramHeap;
	extern unsigned long CARD_SECURITY_BUFFER;
	extern unsigned long JASAramDmaBufferTop;

	void sysDramSetup(JKRSolidHeap* heap);
	void sysAramSetup(unsigned long size);
	void* allocFromSysDram(unsigned long size);
	void* allocFromSysAramFull(unsigned long* size);
	unsigned long getSysDramRemain();
	void setSysDramSize(unsigned long size);
	unsigned long getSysDramSize();
	void* getSysDramTop();
	void setSysAramSize(unsigned long size);
	void* allocFromSysAram(unsigned long size);
	void* allocFromSysAramFull(unsigned long* size);
	void freeToSysAramLast();
	unsigned long getSysAramRemain();
	unsigned long getSysAramSize();
	void* getSysAramTop();

} // namespace Kernel
} // namespace JASystem

#endif // JASSYSTEMHEAP_HPP
