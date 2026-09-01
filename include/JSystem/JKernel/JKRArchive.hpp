#ifndef JKR_ARCHIVE_HPP
#define JKR_ARCHIVE_HPP

#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <macros.h>

// NOTE: Vtable offsets are off

#define JKRARCHIVE_ATTR_COMPRESSION 0x04
#define JKRARCHIVE_ATTR_YAY0        0x80

inline u32 read_big_endian_u32(void* ptr)
{
	u8* uptr = (u8*)ptr;
	return ((u32)uptr[0] << 0x18) | ((u32)uptr[1] << 0x10) | ((u32)uptr[2] << 8)
	       | (u32)uptr[3];
}

class JKRAramBlock;
class JKRFile;

class JKRArchive : public JKRFileLoader {
public:
	enum EMountMode {
		UNKNOWN_MOUNT_MODE = 0,
		MOUNT_MEM          = 1,
		MOUNT_ARAM         = 2,
		MOUNT_DVD          = 3,
		MOUNT_COMP         = 4,
	};

	enum EMountDirection {
		UNKNOWN_MOUNT_DIRECTION = 0,
		MOUNT_DIRECTION_HEAD    = 1,
		MOUNT_DIRECTION_TAIL    = 2,
	};

	class CArcName {
	public:
		CArcName();
		CArcName(const char* data) { store(data); }
		CArcName(const char** p1, char p2) { p1[0] = store(p1[0], p2); }

		void store(const char*);
		const char* store(const char*, char);

		const char* getString() const { return mString; }
		u16 getHash() const { return mHash; }

	private:
		/* 0x0 */ u16 mHash;
		/* 0x2 */ u16 mLength;
		/* 0x4 */ char mString[257];
	};

	struct SDIFileEntry {
		/* 0x00 */ u16 mFileID;
		/* 0x02 */ u16 mHash;
		/* 0x04 */ u32 mFlagsAndNameOffset;
		/* 0x08 */ u32 mDataOffset;
		/* 0x0C */ u32 mSize;
		/* 0x10 */ void* mData;
	};

	struct SDirEntry {
		/* 0x0 */ u8 mFlags;
		/* 0x1 */ u8 _01;
		/* 0x2 */ u16 mID;
		/* 0x4 */ const char* mName;
	};

	struct SDIDirEntry {
		/* 0x0 */ u32 mType;
		/* 0x4 */ u32 mOffset;
		/* 0x8 */ u16 _08;
		/* 0xA */ u16 mNum;
		/* 0xC */ u32 mFirstIdx;
	};

	// NB: Fabricated name
	struct SArcDataInfo {
		/* 0x00 */ u32 num_nodes;
		/* 0x04 */ u32 node_offset;
		/* 0x08 */ u32 num_file_entries;
		/* 0x0C */ u32 file_entry_offset;
		/* 0x10 */ u32 string_table_length;
		/* 0x14 */ u32 string_table_offset;
		/* 0x18 */ u16 nextFreeFileID;
		/* 0x1A */ bool isSyncIDs;
		/* 0x1B */ u8 _1B[5];
	};

	// NB: Fabricated name - need to check size
	struct SArcHeader {
		/* 0x00 */ u32 signature;
		/* 0x04 */ u32 file_length;
		/* 0x08 */ u32 header_length;
		/* 0x0C */ u32 file_data_offset;
		/* 0x10 */ u32 file_data_length;
		/* 0x14 */ u32 _14;
		/* 0x18 */ u32 _18;
		/* 0x1C */ u32 _1C;
	};

	JKRArchive();
	JKRArchive(s32, EMountMode);

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
	virtual void* fetchResource(SDIFileEntry* entry, u32* outSize) = 0;
	virtual void* fetchResource(void* resourceBuffer, u32 bufferSize,
	                            SDIFileEntry* entry, u32* resSize)
	    = 0;

	// NOTE: has to be defined not first to get the vtable
	// to emit to the correct TU
	virtual ~JKRArchive();

	SDIDirEntry* findDirectory(const char*, u32) const;
	SDIFileEntry* findFsResource(const char*, u32) const;
	SDIFileEntry* findIdResource(u16) const;
	SDIFileEntry* findIdxResource(u32) const;
	SDIFileEntry* findNameResource(const char*) const;
	SDIFileEntry* findPtrResource(const void*) const;
	SDIFileEntry* findTypeResource(u32, const char*) const;
	bool isSameName(CArcName&, u32, u16) const;

	bool getDirEntry(SDirEntry*, u32) const;
	void* getIdxResource(u32 index);

	static JKRArchive* mount(char const*, EMountMode, JKRHeap*,
	                         EMountDirection);
	static JKRArchive* mount(void*, JKRHeap*, EMountDirection);
	static void* getGlbResource(u32 type, const char* name,
	                            JKRArchive* archive);

	JKRArchive(const char* p1, EMountMode mountMode);
	static JKRArchive* check_mount_already(s32);
	static JKRArchive* check_mount_already(s32, JKRHeap*);
	SDIDirEntry* findResType(u32) const;
	SDIFileEntry* findTypeResource(u32, u32) const;

	static JKRCompression convertAttrToCompressionType(u8 attr)
	{
		if (!(attr & JKRARCHIVE_ATTR_COMPRESSION))
			return JKR_COMPRESSION_NONE;
		else if (attr & JKRARCHIVE_ATTR_YAY0)
			return JKR_COMPRESSION_YAZ0;
		else
			return JKR_COMPRESSION_YAY0;
	}

	u32 getMountMode() const { return mMountMode; }
	u32 countFile() const { return mArcInfoBlock->num_file_entries; }
	int countDirectory() const { return mArcInfoBlock->num_nodes; }
	static u32 getCurrentDirID() { return sCurrentDirID; }
	static void setCurrentDirID(u32 dirID) { sCurrentDirID = dirID; }

	static u32 sCurrentDirID;

protected:
	/* 0x38 */ JKRHeap* mHeap;
	/* 0x3C */ u8 mMountMode;
	/* 0x40 */ s32 mEntryNum;
	/* 0x44 */ SArcDataInfo* mArcInfoBlock;
	/* 0x48 */ SDIDirEntry* mDirectories;
	/* 0x4C */ SDIFileEntry* mFileEntries;
	/* 0x50 */ const char* mStrTable;
	/* 0x54 */ int _54;
	/* 0x58 */ JKRCompression mCompression;
	/* 0x5C */ EMountDirection mMountDirection;
};

enum JKRMemBreakFlag { MBF_0 = 0, MBF_1 = 1 };

inline JKRCompression JKRConvertAttrToCompressionType(u8 attr)
{
	return JKRArchive::convertAttrToCompressionType(attr);
}

inline JKRArchive* JKRMountArchive(const char* path,
                                   JKRArchive::EMountMode mountMode,
                                   JKRHeap* heap,
                                   JKRArchive::EMountDirection mountDirection)
{
	return JKRArchive::mount(path, mountMode, heap, mountDirection);
}

inline JKRArchive* JKRMountArchive(void* inBuf, JKRHeap* heap,
                                   JKRArchive::EMountDirection mountDirection)
{
	return JKRArchive::mount(inBuf, heap, mountDirection);
}

#endif
