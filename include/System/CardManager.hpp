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
};

#endif
