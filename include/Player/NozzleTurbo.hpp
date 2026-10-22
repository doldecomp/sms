#ifndef PLAYER_NOZZLETURBO_HPP
#define PLAYER_NOZZLETURBO_HPP

#include <Player/NozzleBase.hpp>

// TODO: UNUSED nozzle; any additional member layout is unknown.
// Fabricated inheritance, assumed from size by clanker and may be wrong!
class TNozzleTurbo : public TNozzleDeform {
public:
	TNozzleTurbo(const char*, const char*, TWaterGun*);
	virtual s32 getNozzleKind() const { return 4; }
	// Implementation found in demo disc
	virtual void movement(const TMarioControllerWork&);
	// Implementation found in demo disc
	virtual void animation(int);

	/* 0x714 */ f32 unk714;
};

#endif
