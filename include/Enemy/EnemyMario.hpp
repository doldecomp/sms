#ifndef ENEMY_ENEMY_MARIO_HPP
#define ENEMY_ENEMY_MARIO_HPP

#include <Player/Mario.hpp>
#include "Strategic/HitActor.hpp"

class TEnemyMario : public TMario {
public:
	// stub
	void initEnemyValues();
	void startMonteReplay(u32);
	void startDisappear(u16);
	void startRunAway();
	void startGateDrawing();
	void hitWater(THitActor*);
	u8 thinkTrample();
	void reachGoal();

	// Frabricated
	bool checkUnk4292() const
	{
		if (unk4292 == 0xb || unk4292 == 0xc || unk4292 == 0x11) {
			return true;
		}
		return false;
	}

public:
	u16 unk4290; // goal flag for
	u16 unk4292; // some kind of state
	char unk4294[10];
	TEMario* mEMario; // 0x42A0
	char unk42A4[12];
	f32 unk42B0; // distance for a collision check
	char unk42B4[94];
};

#endif
