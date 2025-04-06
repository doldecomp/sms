#ifndef SYSTEM_GAME_SEQUENCE_HPP
#define SYSTEM_GAME_SEQUENCE_HPP

#include <JSystem/JDrama/JDRFlag.hpp>
#include <dolphin/types.h>

class TGameSequence {
public:
	void set(u8, u8, JDrama::TFlagT<u16>);

public:
	u8 _000;
	u8 _001;
	u16 _002;
};

#endif
