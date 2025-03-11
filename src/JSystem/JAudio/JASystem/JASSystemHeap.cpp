#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASHeapCtrl.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JKernel/JKRStdHeap.hpp>
#include <dolphin/ar.h>

JKRHeap* JASDram;

namespace JASystem {
namespace Kernel {

	static u32 audioDramSize = 0x0c8000;

	void sysDramSetup(JKRSolidHeap* heap)
	{
		if (heap) {
			JASDram = heap;
			return;
		}
		JASDram = JKRCreateSolidHeap(
		    audioDramSize,
		    JKRStdHeap::create(audioDramSize + 0x100, nullptr, false), false);
	}

	void* allocFromSysDram(u32 size)
	{
		void* ptr;
		BOOL enable = OSDisableInterrupts();
		ptr         = new (JASDram, 0x20) u8[size];
		OSRestoreInterrupts(enable);
		return ptr;
	}

	u32 getSysDramRemain() { return 0; }

	void setSysDramSize(u32 size) { }

	u32 getSysDramSize() { return 0; }

	void* getSysDramTop() { return 0; }

	static Kernel::TSolidHeap audioAramHeap;

	static u32 audioAramSize = 0x400000;
	static u32 audioAramTop;
	static u32 CARD_SECURITY_BUFFER;

	u32 JASAramDmaBufferTop;

	void sysAramSetup(u32 size)
	{
		if (!size) {
			size = audioAramSize;
		}
		audioAramTop         = ARGetBaseAddress();
		CARD_SECURITY_BUFFER = 0x40;
		JASAramDmaBufferTop  = audioAramTop;
		audioAramHeap.init((u8*)audioAramTop + 0xC000,
		                   size - (audioAramTop + 0xC000));
	}

	void setSysAramSize(u32 size) { }

	void* allocFromSysAram(u32 size) { return 0; }

	void* allocFromSysAramFull(u32* size)
	{
		u32 remain = audioAramHeap.getRemain();
		void* buf  = audioAramHeap.alloc(remain - 0x20);
		*size      = remain - 0x20;
		return buf;
	}

	void freeToSysAramLast() { }

	u32 getSysAramRemain() { return 0; }

	u32 getSysAramSize() { return 0; }

	void* getSysAramTop() { return 0; }

} // namespace Kernel
} // namespace JASystem
