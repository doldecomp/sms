#ifndef UNSORTED_TYPES_H
#define UNSORTED_TYPES_H

#include <types.h>

struct UnkStruct3B0378 {
	unsigned int unk0;
	char* str;
};

struct gpMarDirectorStruct {
	char filler0[0x7C];
	u8 unk7C;
};

struct gpCameraStruct {
	char filler0[0x50];
	s32 unk50;
	char filler1[0x2C8 - 0x54];
	s16 unk2C8;
};

struct gpCubeFastCStruct {
	char filler0[0x1C];
	u32 unk1C;
};

struct UnkStructUnk58 {
	char filler0[0x14];
	float unk14;
};

struct UnkStruct44 {
	char filler0[0x4C];
	int unk4C;
};

#endif // UNSORTED_TYPES_H
