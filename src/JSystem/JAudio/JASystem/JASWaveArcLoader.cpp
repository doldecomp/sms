#include <JSystem/JAudio/JASystem/JASWaveArcLoader.hpp>
#include <JSystem/JAudio/JASystem/JASHeapCtrl.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <types.h>
#include <string.h>

namespace JASystem {

namespace WaveArcLoader {
	static char sCurrentDir[64] = "/Banks/";
	static Kernel::THeap sAramHeap;
} // namespace WaveArcLoader

bool WaveArcLoader::init()
{
	u32 local_8;
	void* var1 = Kernel::allocFromSysAramFull(&local_8);
	if (!var1) {
		return false;
	}
	sAramHeap.initMotherHeap((u32)var1, local_8, 0);
	return true;
}

void WaveArcLoader::setCurrentDir(const char* dir)
{
	strcpy(sCurrentDir, dir);
	int len = strlen(sCurrentDir);
	if (sCurrentDir[len - 1] == '/')
		return;

	sCurrentDir[len]     = '/';
	sCurrentDir[len + 1] = 0;
}

const char* WaveArcLoader::getCurrentDir() { return sCurrentDir; }

bool WaveArcLoader::loadWave(TObject* obj)
{
	Kernel::THeap* heap = obj->getHeap();

	if (!heap)
		return false;

	if (heap->getUnk8() != nullptr)
		return false;

	char buffer[128];
	strcpy(buffer, sCurrentDir);
	strcat(buffer, obj->getWaveArcFileName());
	u32 extent = Dvd::checkFileExtend(buffer);
	if (!extent)
		return false;

	void* allocation = heap->alloc(&sAramHeap, extent);
	if (!allocation)
		return false;

	u32* flagPtr = obj->getLoadFlagPtr();
	*flagPtr     = 0;
	s32 res      = Dvd::loadToAramDvdT(0, buffer, heap->getUnk8(), 0, extent,
	                                   flagPtr, nullptr);
	if (res == -1) {
		heap->free();
		return false;
	}

	return true;
}

bool WaveArcLoader::eraseWave(TObject* obj)
{
	Kernel::THeap* heap = obj->getHeap();
	if (!heap)
		return false;

	if (!heap->getUnk8())
		return false;

	u32* flagPtr = obj->getLoadFlagPtr();
	*flagPtr     = 0;
	heap->free();
	return true;
}

Kernel::THeap* WaveArcLoader::getRootHeap() { return &sAramHeap; }

} // namespace JASystem
