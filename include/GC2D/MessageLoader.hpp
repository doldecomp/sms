#ifndef GC2D_MESSAGE_LOADER_HPP
#define GC2D_MESSAGE_LOADER_HPP

#include <dolphin/types.h>

class TMessageLoader {
public:
	TMessageLoader();
	TMessageLoader(const char*);

	u32 loadMessageData(const char*);
	void readHeader(u32*, u32*, void*);
	void* parseBlock(u32, u32, void*);
	void* getMessageEntry(u32);
	int readInfoBlock(void*);

public:
	/* 0x0 */ u16 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ void* unk4;
	/* 0x8 */ char unk8[255][0xC];
};

#endif
