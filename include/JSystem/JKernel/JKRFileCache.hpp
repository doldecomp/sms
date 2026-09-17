#ifndef JKR_FILE_CACHE_HPP
#define JKR_FILE_CACHE_HPP

#include <JSystem/JKernel/JKRFileLoader.hpp>

class JKRHeap;

class JKRFileCache : public JKRFileLoader {
public:
	class CCacheBlock {
	public:
		CCacheBlock(u32, u32, const void*);
		~CCacheBlock() { }

		/* 0x00 */ JSULink<CCacheBlock> mCacheBlockLink;
		/* 0x10 */ u32 mReferenceCount;
		/* 0x14 */ u32 mFileID;
		/* 0x18 */ u32 mFileSize;
		/* 0x1C */ void* mMemory;
	};

	JKRFileCache(const char*, const char*);

	virtual ~JKRFileCache();
	virtual bool becomeCurrent(const char*);
	virtual void* getResource(const char* path);
	virtual void* getResource(u32 type, const char* name);
	virtual size_t readResource(void* resourceBuffer, u32 bufferSize,
	                            const char* path);
	virtual size_t readResource(void* resourceBuffer, u32 bufferSize, u32 type,
	                            const char* name);
	virtual void removeResourceAll();
	virtual bool removeResource(void*);
	virtual bool detachResource(void*);
	virtual s32 getResSize(const void*) const;
	virtual u32 countFile(const char*) const;
	virtual JKRFileFinder* getFirstFile(const char*) const;
	virtual void* getFsResource(const char* path);
	virtual void* getNameResource(u32 type, const char* name);
	virtual size_t readFsResource(void* resourceBuffer, u32 bufferSize,
	                              const char* path);
	virtual size_t readNameResource(void* resourceBuffer, u32 bufferSize,
	                                u32 type, const char* name);

	void* getRelResource(const char* path);
	size_t readRelResource(void* resourceBuffer, u32 bufferSize,
	                       const char* path);

	CCacheBlock* findCacheBlock(const void*) const;
	CCacheBlock* findCacheBlock(u32) const;
	bool findFile(char*, const char*) const;
	char* getDvdPathName(const char*) const;
	void convStrLower(char*) const;

	static JKRFileCache* mount(const char*, JKRHeap*, const char*);

private:
	/* 0x38 */ JKRHeap* mParentHeap;
	/* 0x3C */ JSUList<CCacheBlock> mCacheBlockList;
	/* 0x48 */ char* mRootPath;
	/* 0x4C */ char* mCurrentPath;
	/* 0x50 */ char* mVolumePath;
};

inline JKRFileCache* JKRMountDvdDrive(const char* path, JKRHeap* heap,
                                      const char* volume)
{
	return JKRFileCache::mount(path, heap, volume);
}

inline void JKRUnmountDvdDrive(JKRFileCache* cache) { cache->unmount(); }

#endif
