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

	TSolidHeap::TSolidHeap() { }

	TSolidHeap::TSolidHeap(u8*, s32) { }

	void* TSolidHeap::alloc(s32) { return 0; }

	void TSolidHeap::freeLast() { }

	void TSolidHeap::freeAll() { }

	void TSolidHeap::init(u8*, s32) { }

	s32 TSolidHeap::getRemain() { return 0; }

} // namespace Kernel

} // namespace JASystem
