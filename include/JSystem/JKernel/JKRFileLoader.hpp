#ifndef JKR_FILELOADER_HPP
#define JKR_FILELOADER_HPP

#include <JSystem/JKernel/JKREnum.hpp>
#include <JSystem/JKernel/JKRDisposer.hpp>
#include <JSystem/JSupport/JSUList.hpp>

class JKRFileFinder;

class JKRFileLoader : public JKRDisposer {
public:
	JKRFileLoader();

	virtual ~JKRFileLoader();
	virtual void unmount();
	static JKRFileLoader* getVolume(const char*);
	virtual bool becomeCurrent(const char*)               = 0;
	virtual void* getResource(const char* path)           = 0;
	virtual void* getResource(u32 type, const char* name) = 0;
	virtual size_t readResource(void* resourceBuffer, u32 bufferSize,
	                            const char* path)
	    = 0;
	virtual size_t readResource(void* resourceBuffer, u32 bufferSize, u32 type,
	                            const char* name)
	    = 0;
	virtual void removeResourceAll()                       = 0;
	virtual bool removeResource(void*)                     = 0;
	virtual bool detachResource(void*)                     = 0;
	virtual long getResSize(const void*) const             = 0;
	virtual u32 countFile(const char*) const               = 0;
	virtual JKRFileFinder* getFirstFile(const char*) const = 0;

	bool isMounted() const { return mIsMounted; }
	u32 getVolumeType() const { return mVolumeType; }

	static void changeDirectory(const char* dir);

	static void* getGlbResource(const char*);
	static void* getGlbResource(const char*, JKRFileLoader* fileLoader);
	static long getResSize(void* resourceBuffer, JKRFileLoader* fileLoader);
	static size_t readGlbResource(void* resourceBuffer, u32 bufferSize,
	                              const char* path,
	                              JKRExpandSwitch expandSwitch);

	static bool removeResource(void* resourceBuffer, JKRFileLoader* fileLoader);
	static bool detachResource(void* resourceBuffer, JKRFileLoader* fileLoader);

	static JKRFileLoader* findVolume(const char**);
	static JKRFileFinder* findFirstFile(const char*);
	static const char* fetchVolumeName(char*, long, const char*);

	static JKRFileLoader* getCurrentVolume() { return sCurrentVolume; }
	static void setCurrentVolume(JKRFileLoader* fileLoader)
	{
		sCurrentVolume = fileLoader;
	}
	static JSUList<JKRFileLoader>& getVolumeList() { return sVolumeList; }

	static JKRFileLoader* sCurrentVolume;
	static JSUList<JKRFileLoader> sVolumeList;

protected:
	/* 0x18 */ JSULink<JKRFileLoader> mFileLoaderLink;
	/* 0x28 */ const char* mVolumeName;
	/* 0x2C */ u32 mVolumeType;
	/* 0x30 */ bool mIsMounted;
	/* 0x34 */ u32 mMountCount;
};

inline bool JKRDetachResource(void* resource, JKRFileLoader* fileLoader)
{
	return JKRFileLoader::detachResource(resource, fileLoader);
}

inline void* JKRGetNameResource(const char* name, JKRFileLoader* loader)
{
	return JKRFileLoader::getGlbResource(name, loader);
}

inline void* JKRGetResource(const char* name)
{
	return JKRFileLoader::getGlbResource(name);
}

#endif
