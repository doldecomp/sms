#include <JSystem/JAudio/JASystem/JASHeapCtrl.hpp>
#include <macros.h>
#include <types.h>

namespace JASystem {

namespace Kernel {

	u32 global_id = 0;

	THeap::THeap()
	    : unk8(0)
	    , unkC(0)
	    , unk10(0)
	{
		unk4  = global_id++;
		unk0  = 0;
		unk1  = 1;
		unk2  = 0;
		unk14 = 0;
		unk18 = 0;
		unk1C = 0;
		unk20 = 0;
		unk24 = 0;
		unk28 = 0;
	}

	void* THeap::getUnlockHeap() { return 0; }

	void THeap::checkAlloc() { }

	void THeap::init() { }

	void THeap::selfInit(u32, u32, u32) { }

	void* THeap::selfAlloc(THeap*, u32, u32) { return 0; }

	void THeap::setGroupHeap(THeap*) { }

	void THeap::cutDown() { }

	void THeap::initMotherHeap(u32 param_1, u32 param_2, u8 param_3)
	{
		unk8  = (void*)ALIGN_NEXT(param_1, 0x20);
		unkC  = 0;
		unk10 = param_2 - (param_1 & 0x1F);
		unk4  = global_id++;
		unk0  = 1;
		unk1  = param_3;
		unk2  = 0;
		unk14 = 0;
		unk18 = 0;
		unk1C = 0;
		unk20 = 0;
		unk24 = 0;
		unk28 = 0;
	}

	void* THeap::allocHeapCheck(THeap* param_1, u32 param_2) { return 0; }

	void* THeap::alloc(THeap* param_1, u32 param_2)
	{
		void* res = allocHeapCheck(param_1, param_2);
		if (res == nullptr)
			return nullptr;

		return unk8;
	}

	void THeap::free() { }

	void THeap::garbageCollectionSt() { }

	void THeap::checkFreeHeapTotal() { }

	void THeap::checkFreeHeapLinear() { }

	void THeap::show(u32) { }

	void THeap::moveChildren(s32) { }

	void aramFinish(u32) { }

	void dmaAramToAram(u32, u32, u32) { }

	void dmaDramToDram(u32, u32, u32) { }

	TSolidHeap::TSolidHeap()
	    : mStart(0)
	    , mEnd(0)
	    , mSize(0)
	    , unkC(0)
	    , unk10(0)
	{
	}

	TSolidHeap::TSolidHeap(u8* param_1, s32 param_2) { init(param_1, param_2); }

	void* TSolidHeap::alloc(s32 size)
	{
		u32 alignedSize = ALIGN_NEXT(size, 0x20);
		if (mStart == 0)
			return 0;

		u8* result = mEnd;
		if (mEnd + alignedSize <= mStart + mSize)
			mEnd += alignedSize;
		else
			return 0;

		unkC++;
		unk10 = result;
		return result;
	}

	void TSolidHeap::freeLast() { }

	void TSolidHeap::freeAll() { }

	void TSolidHeap::init(u8* mem_start, s32 mem_size)
	{
		unkC = 0;
		if (!mem_start) {
			mSize = 0;
			mEnd  = 0;
			unk10 = 0;
		} else {
			mStart = (u8*)ALIGN_NEXT((u32)mem_start, 0x20);
			mEnd   = mStart;
			mSize  = mem_size - ((u32)mem_start & 0x1F);
			unk10  = 0;
		}
	}

	u32 TSolidHeap::getRemain() { return mSize - (mEnd - mStart); }

} // namespace Kernel

} // namespace JASystem
