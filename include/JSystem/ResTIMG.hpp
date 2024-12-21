#ifndef RESTIMG_HPP
#define RESTIMG_HPP

#include <dolphin/types.h>

struct ResTIMG {
	char unk0[0xc];
	u32 mPaletteOffset;
	char unk10[0xc];
	u32 mImageDataOffset;
};

#endif
