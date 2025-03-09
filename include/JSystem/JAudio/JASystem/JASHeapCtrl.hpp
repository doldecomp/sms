#ifndef JASHEAPCTRL_HPP
#define JASHEAPCTRL_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Kernel {
	class THeap {
	public:
		THeap();
		~THeap() { }

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

		void* getUnk8() const { return unk8; }

	public:
		/* 0x0 */ u8 unk0;
		/* 0x1 */ u8 unk1;
		/* 0x2 */ u16 unk2;
		/* 0x4 */ u32 unk4;
		/* 0x8 */ void* unk8;
		/* 0xC */ u32 unkC;
		/* 0x10 */ u32 unk10;
		/* 0x14 */ u32 unk14;
		/* 0x18 */ u32 unk18;
		/* 0x1C */ u32 unk1C;
		/* 0x20 */ u32 unk20;
		/* 0x24 */ u32 unk24;
		/* 0x28 */ u32 unk28;
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
