#include <JSystem/JParticle/JPADataBlock.hpp>
#include <dolphin/os.h>
#include <macros.h>

JPADataBlock::JPADataBlock(const u8* param_1, JKRHeap* param_2)
{
	u32 size = *(u32*)(param_1 + 4);
	unk4     = JKRHeap::alloc(size, 0x20, param_2);
	memcpy(unk4, param_1, size);
	DCStoreRange(unk4, ALIGN_NEXT(size, 0x20));
}
