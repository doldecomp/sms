#ifndef MSMODBGM_HPP
#define MSMODBGM_HPP

#include <dolphin/types.h>

class MSModBgm {
public:
	void modBgm(u8, u8);
	void changeTempo(u8, u8);
	void loop();
};

class MSBgmXFade {
public:
	static f32 scTiming[];
	static f32 scExp[];

	void xFadeBgm(f32);
	void xFadeBgmForce(f32);
	f32 getTimingForce(f32);
	void getTiming(f32, u32*);
};

#endif // MSMODBGM_HPP
