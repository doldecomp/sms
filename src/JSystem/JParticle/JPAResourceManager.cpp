#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JParticle/JPAEmitterLoader.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>

JPATextureResource::JPATextureResource(u32 count, JKRHeap* heap)
{
	unk24 = 0;
	unk28 = count;
	unk2C = new (heap, 0) JPATexture*[unk28];
	unk0.initialize(heap);
}

int JPATextureResource::registration(const u8* data, JKRHeap* heap)
{
	const char* incomingName = (const char*)(data + 0xC);
	for (int i = 0; i < unk24; ++i) {
		const char* name = (const char*)((u8*)unk2C[i]->mRawData + 0xC);
		if (strcmp(name, incomingName) == 0)
			return i;
	}

	unk2C[unk24] = new (heap, 0) JPATexture(data, heap);
	return unk24++;
}

JPAEmitterResource::JPAEmitterResource(u32 count, JKRHeap* heap)
{
	unk0 = 0;
	unk4 = count;
	unk8 = new (heap, 0) JPAEmitterData*[unk4];
}

int JPAEmitterResource::registration(JPAEmitterData* param_1, u16 param_2)
{
	if (unk0 < unk4) {
		param_1->setUserIndex(param_2);
		unk8[unk0] = param_1;
	}
	return unk0++;
}

JPAEmitterData* JPAEmitterResource::getByUserIndex(u16 idx)
{
	for (int i = 0; i < unk0; ++i) {
		if (unk8[i]->getUserIndex() == idx)
			return unk8[i];
	}
	return nullptr;
}

JPAResourceManager::JPAResourceManager(u32 param_1, u32 param_2,
                                       JKRHeap* param_3)
{
	unk0 = param_3 ? param_3 : JKRHeap::getCurrentHeap();

	unk4 = new (unk0, 0) JPAEmitterResource(param_1, unk0);
	unk8 = new (unk0, 0) JPATextureResource(param_2, unk0);
}

int JPAResourceManager::load(const char* param_1, u16 param_2)
{
	return load(JKRFileLoader::getGlbResource(param_1), param_2);
}

int JPAResourceManager::load(const void* param_1, u16 param_2)
{
	JPAEmitterData* data
	    = JPAEmitterLoaderDataBase::load((const u8*)param_1, unk0, unk8);
	return getEmitterResource()->registration(data, param_2);
}
