#ifndef JASHEAPCTRL_HPP
#define JASHEAPCTRL_HPP

#include <stdint.h>
#include <dolphin/types.h>

namespace JASystem {

namespace Kernel {
	class THeap {
	public:
		THeap();
		~THeap() { }

		void init();
		void* alloc(THeap* parent, u32 size);
		BOOL allocHeapCheck(THeap* parent, u32 size);
		BOOL free();
		BOOL selfAlloc(THeap* parent, u32 size, uintptr_t address);
		void initMotherHeap(uintptr_t address, u32 size, u8 param_3);
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

		void* getBase() const { return mBase; }
		bool isAllocated() const { return mBase; }

	public:
		/* 0x0 */ u8 unk0;
		/* 0x1 */ u8 unk1;
		/* 0x2 */ u16 mChildCount;
		/* 0x4 */ u32 mId;
		/* 0x8 */ u8* mBase;
		/* 0xC */ u32 mEndOffset;
		/* 0x10 */ u32 mSize;
		/* 0x14 */ THeap* mFirstChild;
		/* 0x18 */ THeap* mParent;
		/* 0x1C */ THeap* mNextSibling;
		/* 0x20 */ THeap* mGroupOwner;
		/* 0x24 */ THeap* mFirstGroupMember;
		/* 0x28 */ THeap* mNextGroupMember;
	};

	class TSolidHeap {
	public:
		TSolidHeap();
		TSolidHeap(u8* mem_start, s32 mem_size);
		~TSolidHeap() { }

		void init(u8* mem_start, s32 mem_size);
		void* alloc(s32 size);
		u32 getRemain();
		void freeLast();
		void freeAll();

	public:
		/* 0x00 */ u8* mStart;
		/* 0x04 */ u8* mCurrent;
		/* 0x08 */ int mSize;
		/* 0x0C */ int mAllocCount;
		/* 0x10 */ u8* mLastAlloc;
	};

	extern u32 global_id;

	void aramFinish(u32);
	void dmaAramToAram(u32, u32, u32);
	void dmaDramToDram(u32, u32, u32);
} // namespace Kernel

} // namespace JASystem

#endif // JASHEAPCTRL_HPP
