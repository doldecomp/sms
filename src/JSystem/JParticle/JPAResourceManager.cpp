#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

JPATextureResource::JPATextureResource(u32 count, JKRHeap* heap)
{
	unk24 = 0;
	unk28 = count;
	unk2C = new (heap, 0) void*[unk28];
	unk0.initialize(heap);
}

void JPATextureResource::registration(const u8*, JKRHeap*) { }

JPAEmitterResource::JPAEmitterResource(u32 count, JKRHeap* heap)
{
	unk0 = 0;
	unk4 = count;
	unk8 = new (heap, 0) void*[unk4];
}

void JPAEmitterResource::registration(JPAEmitterData*, u16) { }

void JPAEmitterResource::getByUserIndex(u16) { }

JPAResourceManager::JPAResourceManager(u32 param_1, u32 param_2,
                                       JKRHeap* param_3)
{
	unk0 = param_3 ? param_3 : JKRHeap::sCurrentHeap;

	unk4 = new (unk0, 0) JPAEmitterResource(param_1, unk0);
	unk8 = new (unk0, 0) JPATextureResource(param_2, unk0);
}

void JPAResourceManager::load(const char*, u16) { }

void JPAResourceManager::load(const void*, u16) { }
