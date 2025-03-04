#include <JSystem/JAudio/JASystem/JASHeapCtrl.hpp>

namespace JASystem {

namespace Kernel {

	u32 global_id = 0;

	void aramFinish(u32) { }

	void dmaAramToAram(u32, u32, u32) { }

	void dmaDramToDram(u32, u32, u32) { }

	// THeap implementation
	THeap::THeap() { }

	void THeap::init() { }

	void* THeap::alloc(THeap*, u32) { return 0; }

	void* THeap::allocHeapCheck(THeap*, u32) { return 0; }

	void THeap::free() { }

	void* THeap::selfAlloc(THeap*, u32, u32) { return 0; }

	void THeap::initMotherHeap(u32, u32, u8) { }

	void* THeap::getUnlockHeap() { return 0; }

	void THeap::checkAlloc() { }

	void THeap::selfInit(u32, u32, u32) { }

	void THeap::setGroupHeap(THeap*) { }

	void THeap::cutDown() { }

	void THeap::garbageCollectionSt() { }

	void THeap::checkFreeHeapTotal() { }

	void THeap::checkFreeHeapLinear() { }

	void THeap::show(u32) { }

	void THeap::moveChildren(s32) { }

	// TSolidHeap implementation
	TSolidHeap::TSolidHeap() { }

	TSolidHeap::TSolidHeap(u8*, s32) { }

	void TSolidHeap::init(u8*, s32) { }

	void* TSolidHeap::alloc(s32) { return 0; }

	s32 TSolidHeap::getRemain() { return 0; }

	void TSolidHeap::freeLast() { }

	void TSolidHeap::freeAll() { }

} // namespace Kernel

} // namespace JASystem
