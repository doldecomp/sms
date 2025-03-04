#ifndef JASHEAPCTRL_HPP
#define JASHEAPCTRL_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Kernel {
	class THeap {
	public:
		THeap();
		virtual ~THeap() { }

		void init();
		void* alloc(THeap*, u32);
		void* allocHeapCheck(THeap*, u32);
		void free();
		void* selfAlloc(THeap*, u32, u32);
		void initMotherHeap(u32, u32, u8);
		void* getUnlockHeap();
		void checkAlloc();
		void selfInit(u32, u32, u32);
		void setGroupHeap(THeap*);
		void cutDown();
		void garbageCollectionSt();
		void checkFreeHeapTotal();
		void checkFreeHeapLinear();
		void show(u32);
		void moveChildren(s32);
	};

	class TSolidHeap : public THeap {
	public:
		TSolidHeap();
		TSolidHeap(u8*, s32);

		void init(u8*, s32);
		void* alloc(s32);
		s32 getRemain();
		void freeLast();
		void freeAll();
	};

	extern u32 global_id;

	void aramFinish(u32);
	void dmaAramToAram(u32, u32, u32);
	void dmaDramToDram(u32, u32, u32);
} // namespace Kernel

} // namespace JASystem

#endif // JASHEAPCTRL_HPP
