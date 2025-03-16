#ifndef JASTRACKINTERRUPT_HPP
#define JASTRACKINTERRUPT_HPP

#include <dolphin/types.h>

namespace JASystem {

class TIntrMgr {
public:
	void init();
	void request(u32);
	void setIntr(u32, void*);
	void resetInter(u32);
	void* checkIntr();
	void timerProcess();

	void disable() { unk0 = 0; }
	void enable() { unk0 = 1; }

	void setTimer(u32 param_1, u32 param_2)
	{
		unk3 = param_1;
		unk4 = param_2;
		unk8 = param_2;
	}

public:
	/* 0x00 */ u8 unk0;
	/* 0x01 */ u8 unk1;
	/* 0x02 */ u8 unk2;
	/* 0x03 */ u8 unk3;
	/* 0x04 */ u32 unk4;
	/* 0x08 */ u32 unk8;
	/* 0x0C */ void* unkC[8];
};

} // namespace JASystem

#endif // JASTRACKINTERRUPT_HPP
