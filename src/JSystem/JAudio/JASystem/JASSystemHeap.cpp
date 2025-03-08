#include "JSystem/JAudio/JASystem/JASSystemHeap.hpp"

JKRHeap* JASDram;

namespace JASystem {
namespace Kernel {

	u32 audioDramSize        = 0x0c8000;
	u32 audioAramSize        = 0x400000;
	u32 audioAramTop         = 0;
	void* audioAramHeap      = 0;
	u32 CARD_SECURITY_BUFFER = 0;
	u32 JASAramDmaBufferTop  = 0;

	void sysDramSetup(JKRSolidHeap* heap) { }

	void sysAramSetup(u32 size) { }

	void* allocFromSysDram(u32 size) { return 0; }

	void* allocFromSysAramFull(u32* size) { return 0; }

	u32 getSysDramRemain() { return 0; }

	void setSysDramSize(u32 size) { }

	u32 getSysDramSize() { return 0; }

	void* getSysDramTop() { return 0; }

	void setSysAramSize(u32 size) { }

	void* allocFromSysAram(u32 size) { return 0; }

	void freeToSysAramLast() { }

	u32 getSysAramRemain() { return 0; }

	u32 getSysAramSize() { return 0; }

	void* getSysAramTop() { return 0; }

} // namespace Kernel
} // namespace JASystem
