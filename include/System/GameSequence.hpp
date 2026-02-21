#ifndef SYSTEM_GAME_SEQUENCE_HPP
#define SYSTEM_GAME_SEQUENCE_HPP

#include <JSystem/JDrama/JDRFlag.hpp>
#include <dolphin/types.h>

class TGameSequence {
public:
	TGameSequence() { set(0, 0, 0); }

	TGameSequence& operator=(const TGameSequence& other)
	{
		set(other.unk0, other.unk1, other.unk2);
		return *this;
	}

	void set(u8 param_1, u8 param_2, JDrama::TFlagT<u16> param_3)
	{
		unk0 = param_1;
		unk1 = param_2;
		unk2 = param_3;
	}

	// fabricated
	u8 getStage() const { return unk0; }
	u8 getScenario() const { return unk1; }

public:
	/* 0x0 */ u8 unk0;
	/* 0x1 */ u8 unk1;
	/* 0x2 */ JDrama::TFlagT<u16> unk2;
};

#endif
