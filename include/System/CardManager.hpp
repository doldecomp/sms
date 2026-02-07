#ifndef GC2D_CARD_MANAGER_HPP
#define GC2D_CARD_MANAGER_HPP

#include <dolphin/card.h>

class JSUMemoryInputStream;
class JSUMemoryOutputStream;
struct HeaderData;

struct TCardBookmarkInfo { };
struct TCardSector { };

class TCardManager {
public:
	struct TCriteria { };

	void decideUseSector(TCriteria*);
	void getLoadIndex(TCriteria*);
	void getWriteCount(TCriteria*);
	void copyTo(TCriteria*, TCardBookmarkInfo*);
	TCardManager(void*, void*, s32, s32, void*, u32);
	~TCardManager();
	void issue(s32);
	void probe();
	void unmount();
	void format();
	void createFile();
	void getBookmarkInfos(TCardBookmarkInfo*);
	void readBlock(u32);
	void getReadStream(JSUMemoryInputStream*);
	void getWriteStream(JSUMemoryOutputStream*);
	void writeBlock(u32);
	void readOptionBlock();
	void getOptionReadStream(JSUMemoryInputStream*);
	void getOptionWriteStream(JSUMemoryOutputStream*);
	void writeOptionBlock();
	int getLastStatus();

	void probe_();
	void mount_(bool);
	void unmount_();
	void format_();
	void createFile_();
	void filledInitData_(CARDFileInfo*);
	void setCardStat_(CARDFileInfo*);
	void buildHeader_(HeaderData*);
	void open_(CARDFileInfo*);
	void getBookmarkInfos_();
	void readBlock_(u32);
	void readOptionBlock_();
	void writeBlock_(u32);
	void writeOptionBlock_();
	void writeCardSector_(CARDFileInfo*, s32, TCardSector*, TCriteria*);
	void cmdLoop();

public:
	/* 0x0 */ s32 unk0;
	/* 0x4 */ void* unk4;
	/* 0x8 */ void* unk8;
	/* 0xC */ TCriteria unkC[7];
	/* 0x124 */ u8 unk124;
	/* 0x125 */ u8 unk125;
	/* 0x126 */ u8 unk126;
	/* 0x128 */ u32 unk128;
	/* 0x12C */ void* unk12C;
	/* 0x130 */ void* unk130;
	/* 0x134 */ char unk134[0x4];
	/* 0x138 */ OSThread unk138;
	/* 0x448 */ u32 unk448;
	/* 0x44C */ OSMutex unk44C;
	/* 0x464 */ OSCond unk464;
	/* 0x46C */ TCardBookmarkInfo* unk46C;
	/* 0x470 */ char unk470[0x4];
	/* 0x474 */ u32 unk474;
};

#endif
