#ifndef GC2D_MESSAGE_LOADER_HPP
#define GC2D_MESSAGE_LOADER_HPP

#include <dolphin/types.h>

class TMessageLoader {
public:
	struct EntryInfo {
		/* 0x0 */ u32 unk0;
		/* 0x4 */ s16 unk4;
		/* 0x6 */ s16 unk6;
		/* 0x8 */ char unk8[0x4];
	};

	TMessageLoader();
	TMessageLoader(const char*);

	u32 loadMessageData(const char*);
	void readHeader(u32*, u32*, void*);
	void* parseBlock(u32, u32, void*);
	EntryInfo* getMessageEntry(u32);
	int readInfoBlock(void*);

	u16 getMessageNum() const { return unk0; }
	const u8* getMessageData() const { return (u8*)unk4; }

public:
	/* 0x0 */ u16 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ void* unk4;
	/* 0x8 */ EntryInfo unk8[255];
};

#endif
