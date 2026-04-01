#ifndef MSMODBGM_HPP
#define MSMODBGM_HPP

#include <dolphin/types.h>

class JAISound;

class MSModBgm {
public:
	JAISound* modBgm(u8, u8);
	void changeTempo(u8, u8);
	void loop();

public:
	/* 0x0 */ bool unk0;
	/* 0x4 */ u32 unk4;
};

class MSBgmXFade {
public:
	MSBgmXFade()
	    : unk0(0.0f)
	{
	}

	static f32 scTiming[];
	static f32 scExp[];

	void xFadeBgm(f32);
	void xFadeBgmForce(f32);
	u8 getTimingForce(f32);
	u8 getTiming(f32, u32*);

public:
	/* 0x0 */ f32 unk0;
};

#endif // MSMODBGM_HPP
