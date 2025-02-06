#ifndef RESTIMG_HPP
#define RESTIMG_HPP

#include <types.h>

struct ResTIMG {
	/* 0x0 */ u8 format;
	/* 0x1 */ char unk1[0x1];
	/* 0x2 */ u16 width;
	/* 0x4 */ u16 height;
	/* 0x6 */ u8 wrapS;
	/* 0x7 */ u8 wrapT;
	/* 0x8 */ u8 isIndexTexture;
	/* 0x9 */ u8 colorFormat;
	/* 0xA */ u16 numColors;
	/* 0xC */ u32 paletteOffset;
	/* 0x10 */ char unk10[0x1];
	/* 0x11 */ u8 doEdgeLod;
	/* 0x12 */ u8 biasClamp;
	/* 0x13 */ u8 maxAnisotropy;
	/* 0x14 */ u8 minFilter;
	/* 0x15 */ u8 magFilter;
	/* 0x16 */ s8 minLod;
	/* 0x17 */ s8 maxLod;
	/* 0x18 */ char unk18[0x2];
	/* 0x1A */ s16 lodBias;
	/* 0x1C */ u32 imageDataOffset;
};

#endif
