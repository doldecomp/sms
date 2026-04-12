#ifndef NPC_TRAMPLE_HPP
#define NPC_TRAMPLE_HPP

#include <dolphin/types.h>

class TNpcTrample {
public:
	TNpcTrample()
	    : unk0(0.0f)
	    , unk4(0)
	    , unk6(0)
	{
		msAmpDecrease = 0.0f;
	}

	void startTrample();
	bool updateTrample(f32, f32*);

	static f32 msAmpDecrease;

public:
	/* 0x0 */ f32 unk0;
	/* 0x4 */ s16 unk4;
	/* 0x6 */ s16 unk6;
};

#endif
