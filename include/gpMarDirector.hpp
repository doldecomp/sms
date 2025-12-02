#ifndef GPMARDIRECTOR_H
#define GPMARDIRECTOR_H

#include "GC2D/GCConsole2.hpp"

extern class TMarDirector {
public:
	char pad_000[0x58];
	s32 frameIndex;
	char pad_001[0x14];
	int _070;
	TGCConsole2* console;
	int _078;
	u8 map;
	u8 _07D;
	u8 _07E;
	u8 _07F;
	u32 _080[0x29];
	u8 mGameState;

}* gpMarDirector;

#endif
