#ifndef MAP_MAP_XLU_HPP
#define MAP_MAP_XLU_HPP

#include <dolphin/types.h>

class JSUMemoryInputStream;

class TMapXlu {
public:
	TMapXlu();
	void init(JSUMemoryInputStream&);
	bool changeXluJoint(int);
	void changeNormalJoint();

	struct Entry {
		/* 0x0 */ s32 unk0;
		/* 0x4 */ u32* unk4;
		/* 0x8 */ u32* unk8;
	};

public:
	/* 0x0 */ s32 unk0;
	/* 0x4 */ Entry* unk4;
};

#endif
