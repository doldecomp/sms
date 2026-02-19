#ifndef GC2D_CARD_MANAGER_HPP
#define GC2D_CARD_MANAGER_HPP

#include <dolphin/card.h>

class JSUMemoryInputStream;
class JSUMemoryOutputStream;
struct HeaderData;
struct TCardSector;

struct TCardBookmarkInfo {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u64 unk8;
	/* 0x10 */ u64 unk10;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u16 unk1C;
	/* 0x1E */ u16 unk1E;
};

/**
 * @brief Async wrapper around the CARD SDK API.
 * @details Essentially, an SPSC two-way channel between the main thread and a
 * worker thread that performs CARD operations. It also performs basic error
 * checking in case of data loss.
 */
class TCardManager {
public:
	struct TCriteria {
		TCriteria() { mState = STATE_UNREAD; }

		enum TEBlockStat {
			STATE_UNREAD       = 0,
			STATE_EMPTY        = 1,
			STATE_CHECKSUM_BAD = 2,
			STATE_VALID        = 3,
		};

		void set(TEBlockStat, u32, const void*);
		void setEmpty();

		// fabricated
		TEBlockStat getState() const { return mState; }
		u32 getWriteCount() const { return mWriteCount; }
		void* getPreviewBytes() { return mPreviewBytes; }
		static TEBlockStat getStateFor(bool match)
		{
			return match ? STATE_VALID : STATE_CHECKSUM_BAD;
		}

		/* 0x0 */ TEBlockStat mState;
		/* 0x4 */ u32 mWriteCount;
		/* 0x8 */ u8 mPreviewBytes[0x20]; // first 0x20 bytes of the sector
	};

	static s32 decideUseSector(TCriteria*);
	static s32 getLoadIndex(TCriteria*);
	static s32 getWriteCount(TCriteria*);
	static void copyTo(TCriteria*, TCardBookmarkInfo*);

	TCardManager(void*, void*, s32, s32, void*, u32);
	~TCardManager();

	u32 probe();
	s32 unmount();
	void format();
	void createFile();
	void getBookmarkInfos(TCardBookmarkInfo*);

	void readBlock(u32);
	void writeBlock(u32);

	void getReadStream(JSUMemoryInputStream*);
	void getWriteStream(JSUMemoryOutputStream*);

	void readOptionBlock();
	void writeOptionBlock();

	void getOptionReadStream(JSUMemoryInputStream*);
	void getOptionWriteStream(JSUMemoryOutputStream*);

	s32 getLastStatus();

	s32 cmdLoop();

private:
	void issue(s32);
	s32 probe_();
	s32 mount_(bool);
	s32 unmount_();
	s32 format_();
	s32 createFile_();
	s32 filledInitData_(CARDFileInfo*);
	s32 setCardStat_(CARDFileInfo*);
	void buildHeader_(HeaderData*);
	s32 open_(CARDFileInfo*);
	s32 getBookmarkInfos_();
	s32 readBlock_(u32);
	s32 readOptionBlock_();
	s32 writeBlock_(u32);
	s32 writeOptionBlock_();
	s32 writeCardSector_(CARDFileInfo*, s32, TCardSector*, TCriteria*);

public:
	enum {
		CMD_NONE          = 0,
		CMD_FORMAT        = 1,
		CMD_CREATE        = 2,
		CMD_BOOKMARK_SCAN = 3,
		CMD_READ_BLOCK    = 5,
		CMD_WRITE_BLOCK   = 6,
		CMD_READ_OPTION   = 7,
		CMD_WRITE_OPTION  = 8,
		CMD_THREAD_EXIT   = 9,
	};

	/* 0x0 */ s32 mChannel;
	/* 0x4 */ const void* mIcons;
	/* 0x8 */ const void* mBanner;
	/* 0xC */ TCriteria mSectorCriteria[7];
	/* 0x124 */ bool mNeedsInit;
	/* 0x125 */ bool mIsMounted;
	/* 0x126 */ bool mFsCheckedOk;
	/* 0x128 */ u32 mLastStatus; // guarded by mMutex
	/* 0x12C */ void* mCardMountWorkArea;
	/* 0x130 */ void* mSector;
	/* 0x134 */ char unk134[0x4];
	/* 0x138 */ OSThread mWorkerThread;
	/* 0x448 */ s32 mPendingCommand; // guarded by mMutex
	/* 0x44C */ OSMutex mMutex;
	/* 0x464 */ OSCond mCommandIsPending;
	/* 0x46C */ TCardBookmarkInfo* mGetBookmarkInfosArg; // guarded by mMutex
	/* 0x470 */ char unk470[0x4];
	/* 0x474 */ u32 mReadWriteBlockArg; // guarded by mMutex
};

struct HeaderData {
	/* 0x04 */ char mTitle[0x20];
	/* 0x24 */ char mComment[0x20];
	/* 0x40 */ char mBanner[0xE00];
	/* 0xE40 */ char mIcons[0xA00];
};

struct TCardSector {
	void clearData();
	void setCheckSum(u32);
	s32 read(CARDFileInfo*, s32, TCardManager::TCriteria*);

	// fabricated
	HeaderData* getHeader() { return &mHeader; }
	void* getData() { return &mHeader; }
	size_t getDataSize() const
	{
		return sizeof(mHeader) + sizeof(mOptionBlock);
	}
	s32 getWriteCount() const { return mWriteCount; }

public:
	/* 0x0 */ s32 mWriteCount;
	/* 0x4 */ HeaderData mHeader;
	/* 0x1844 */ char mOptionBlock[0x7B8];
	/* 0x1FFC */ s32 mCheckSum;
};

#endif
