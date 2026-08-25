#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JParticle/JPAEmitterLoader.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>

JPATextureResource::JPATextureResource(u32 num, JKRHeap* heap)
{
	registNum    = 0;
	maxNum       = num;
	pTexResArray = new (heap, 0) JPATexture*[maxNum];
	JUT_ASSERT(24, pTexResArray);
	defaultTex.initialize(heap);
}

int JPATextureResource::registration(const u8* data, JKRHeap* heap)
{
	const char* incomingName = (const char*)(data + 0xC);
	for (int i = 0; i < registNum; ++i) {
		const char* name = pTexResArray[i]->getName();
		if (strcmp(name, incomingName) == 0)
			return i;
	}

	JUT_ASSERT(76, registNum < maxNum);
	pTexResArray[registNum] = new (heap, 0) JPATexture(data, heap);
	return registNum++;
}

JPAEmitterResource::JPAEmitterResource(u32 num, JKRHeap* heap)
{
	registNum     = 0;
	maxNum        = num;
	pEmtrResArray = new (heap, 0) JPAEmitterData*[maxNum];
	JUT_ASSERT(93, pEmtrResArray);
}

int JPAEmitterResource::registration(JPAEmitterData* res, u16 userIndex)
{
	JUT_ASSERT(107, registNum < maxNum);
	if (registNum < maxNum) {
		res->setUserIndex(userIndex);
		pEmtrResArray[registNum] = res;
	}
	return registNum++;
}

JPAEmitterData* JPAEmitterResource::getByUserIndex(u16 userIndex)
{
	for (int i = 0; i < registNum; ++i) {
		if (pEmtrResArray[i]->getUserIndex() == userIndex)
			return pEmtrResArray[i];
	}
	return nullptr;
}

JPAResourceManager::JPAResourceManager(u32 eArraySize, u32 tArraySize,
                                       JKRHeap* heap)
{
	JUT_ASSERT(60, (eArraySize != 0) && (tArraySize != 0));
	pHeap = heap ? heap : JKRGetCurrentHeap();

	pEmtrResMgr = new (pHeap, 0) JPAEmitterResource(eArraySize, pHeap);
	pTexResMgr  = new (pHeap, 0) JPATextureResource(tArraySize, pHeap);
	JUT_ASSERT(65, pEmtrResMgr && pTexResMgr);
}

int JPAResourceManager::load(const char* name, u16 userIndex)
{
	void* binData = JKRGetResource(name);
	JUT_WARNING_F(binData, "jpa file %s was not found\n", name);
	return load(binData, userIndex);
}

int JPAResourceManager::load(const void* binData, u16 userIndex)
{
	JUT_ASSERT(77, binData);
	JPAEmitterData* emtrData
	    = JPAEmitterLoaderDataBase::load((const u8*)binData, pHeap, pTexResMgr);
	int ret = getEmitterResource()->registration(emtrData, userIndex);
	return ret;
}
