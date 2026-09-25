#include <JSystem/JKernel/JKRFileCache.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <JSystem/JKernel/JKRFileFinder.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

JKRFileCache* JKRFileCache::mount(const char* path, JKRHeap* heap,
                                  const char* volume)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

JKRFileCache::JKRFileCache(const char* path, const char* volume)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
}

JKRFileCache::~JKRFileCache() { JUT_ASSERT_F(false, "UNIMPLEMENTED"); }

bool JKRFileCache::becomeCurrent(const char* path)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return false;
}

void* JKRFileCache::getResource(const char* path)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

void* JKRFileCache::getRelResource(const char* path)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

void* JKRFileCache::getResource(u32 type, const char* name)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

size_t JKRFileCache::readResource(void* resourceBuffer, u32 bufferSize,
                                  const char* path)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return 0;
}

size_t JKRFileCache::readRelResource(void* resourceBuffer, u32 bufferSize,
                                     const char* path)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return 0;
}

size_t JKRFileCache::readResource(void* resourceBuffer, u32 bufferSize,
                                  u32 type, const char* name)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return 0;
}

void JKRFileCache::removeResourceAll() { JUT_ASSERT_F(false, "UNIMPLEMENTED"); }

bool JKRFileCache::removeResource(void* resource)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return false;
}

bool JKRFileCache::detachResource(void* resource)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return false;
}

s32 JKRFileCache::getResSize(const void* resource) const
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return -1;
}

u32 JKRFileCache::countFile(const char* path) const
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return 0;
}

JKRFileFinder* JKRFileCache::getFirstFile(const char* path) const
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

// The linker takes the weak JKRFileFinder destructor and vtable from this
// position in this TU, and no function left here emits them, so this reference
// stands in for whatever the original code did.
static void dummy() { reinterpret_cast<JKRFileFinder*>(0)->~JKRFileFinder(); }

JKRFileCache::CCacheBlock*
JKRFileCache::findCacheBlock(const void* resource) const
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

JKRFileCache::CCacheBlock* JKRFileCache::findCacheBlock(u32 fileID) const
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

bool JKRFileCache::findFile(char* path, const char* fileName) const
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return false;
}

char* JKRFileCache::getDvdPathName(const char* path) const
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

void JKRFileCache::convStrLower(char* buffer) const
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
}

JKRFileCache::CCacheBlock::CCacheBlock(u32 fileID, u32 fileSize,
                                       const void* resource)
    : mCacheBlockLink(this)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
}

void* JKRFileCache::getFsResource(const char* path)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

void* JKRFileCache::getNameResource(u32 type, const char* name)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

size_t JKRFileCache::readFsResource(void* resourceBuffer, u32 bufferSize,
                                    const char* path)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return 0;
}

size_t JKRFileCache::readNameResource(void* resourceBuffer, u32 bufferSize,
                                      u32 type, const char* name)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return 0;
}
