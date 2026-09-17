#ifndef GC2D_MESSAGE_LOADER_HPP
#define GC2D_MESSAGE_LOADER_HPP

#include <dolphin/types.h>

/**
 * @brief One entry of a JMessage (.bmg) INF1 block.
 *
 * @details TMessageLoader hands these out; only the byte offset of the
 * message text inside the DAT1 block and the voice-clip index are read.
 * The name is not a guess: TTalk2D2::setupTextBox and setupBoardTextBox
 * mangle as ...FPCvP12JMSMesgEntry.
 *
 * TODO: this is JSystem's own type and belongs beside the rest of JMessage
 * rather than in a GC2D header; there is no evidence yet for the file it
 * lived in.
 */
class JMSMesgEntry {
public:
	/* 0x0 */ u32 mTextOffset;
	/* 0x4 */ s16 unk4;
	/* 0x6 */ s16 unk6;
	/* 0x8 */ u8 mVoiceIndex;
	/* 0x9 */ u8 unk9[3];
};

class TMessageLoader {
public:
	TMessageLoader();
	TMessageLoader(const char*);

	u32 loadMessageData(const char*);
	void readHeader(u32*, u32*, void*);
	void* parseBlock(u32, u32, void*);
	JMSMesgEntry* getMessageEntry(u32);
	int readInfoBlock(void*);

	u16 getMessageNum() const { return unk0; }
	const u8* getMessageData() const { return (u8*)unk4; }

public:
	/* 0x0 */ u16 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ void* unk4;
	/* 0x8 */ JMSMesgEntry unk8[255];
};

#endif
