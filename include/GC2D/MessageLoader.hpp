#ifndef GC2D_MESSAGE_LOADER_HPP
#define GC2D_MESSAGE_LOADER_HPP

#include <dolphin/types.h>

class TMessageLoader {
public:
	TMessageLoader();
	TMessageLoader(const char*);

	void loadMessageData(const char*);
	void readHeader(u32*, u32*, void*);
	void parseBlock(u32, u32, void*);
	void getMessageEntry(u32);
	void readInfoBlock(void*);
};

#endif
