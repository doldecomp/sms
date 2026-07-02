#ifndef MAP_MAP_XLU_HPP
#define MAP_MAP_XLU_HPP

#include <dolphin/types.h>

class JSUMemoryInputStream;

class TMapXlu {
public:
	TMapXlu();

	void init(JSUMemoryInputStream&);
	bool changeXluJoint(int prio);
	void changeNormalJoint();

private:
	struct TXluPrioGroup {
		/* 0x0 */ s32 mObjectNum;
		/* 0x4 */ u32* mChildIdx;
		/* 0x8 */ u32* mGrandchildIdx;
	};

	/* 0x0 */ s32 mPrioGroupNum;
	/* 0x4 */ TXluPrioGroup* mPrioGroups;
};

#endif
