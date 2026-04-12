#ifndef NPCBALLOON_HPP
#define NPCBALLOON_HPP

#include <dolphin/types.h>

class TNpcBalloon {
public:
	TNpcBalloon()
	    : unk0(0)
	    , mBalloonAppearTimer(-1)
	{
	}

	bool updateBalloon();
	void setNextMessage(u32, s32);

public:
	/* 0x0 */ int unk0;
	/* 0x4 */ int mBalloonAppearTimer;
};

#endif
