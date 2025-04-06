#ifndef SYSTEM_MARIO_GAME_PAD_HPP
#define SYSTEM_MARIO_GAME_PAD_HPP

#include <dolphin/types.h>

class TMarioGamePad {
public:
	struct TType { };

	~TMarioGamePad();
	void keepRumble(TMarioGamePad::TType);
	void rumble(TMarioGamePad::TType, u32);
	void read();
	void onNeutralMarioKey();
	void considerMarioStick(f32*);
	void updateMeaning();
	void reset();
};

#endif
