#ifndef JUT_DATA_HEADER_HPP
#define JUT_DATA_HEADER_HPP

// Note: The name of this header is fake.

#include <dolphin/types.h>

struct JUTDataBlockHeader {
	/* 0x0 */ u32 mType;
	/* 0x4 */ u32 mSize;
};

struct JUTDataFileHeader { // actual struct name unknown
	/* 0x00 */ u32 mMagic;
	/* 0x04 */ u32 mType;
	/* 0x08 */ u32 mFileSize;
	/* 0x0C */ u32 mBlockNum;
	/* 0x10 */ u8 _10[0x1C - 0x10];
	/* 0x1C */ u32 mSeAnmOffset; // Only exists for some BCKs
	/* 0x20 */ JUTDataBlockHeader mFirstBlock;
};

#endif
