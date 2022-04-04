#ifndef NPCBASE_HPP
#define NPCBASE_HPP

#include "dolphin.h"

class TBaseNPC {
	void setKinoActionFlag_();
	void setMareActionFlag_();
	void setMonteActionFlag_();

	char pad_000[0x16C];
	s32 _16C;
	u32 mActionFlag;
};

#endif
