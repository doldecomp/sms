#ifndef MOVE_BG_MAP_OBJ_LIB_WAVE_HPP
#define MOVE_BG_MAP_OBJ_LIB_WAVE_HPP

#include <dolphin/types.h>

class TMapObjLibWave {
public:
	TMapObjLibWave(f32, f32, f32, f32);
	void movement();
	f32 getCurrentHeight(f32, f32) const;
	void push();

	static f32 mWaveSpeed;

public:
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
};

#endif
