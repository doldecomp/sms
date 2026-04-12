#ifndef NPC_THROW_HPP
#define NPC_THROW_HPP

#include <dolphin/types.h>

class THitActor;

class TNpcThrow {
public:
	TNpcThrow(f32 param_1, f32 param_2)
	    : unk0(param_1)
	    , unk4(param_2)
	{
	}

	void throwMario(THitActor*);

public:
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
};

#endif
