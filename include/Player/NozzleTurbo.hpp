#ifndef PLAYER_NOZZLETURBO_HPP
#define PLAYER_NOZZLETURBO_HPP

#include <Player/NozzleBase.hpp>

// TODO: UNUSED nozzle; any additional member layout is unknown.
// Implementation found in demo disc
// Still needs validation, mostly clanker pass
class TNozzleTurbo : public TNozzleDeform {
public:
	TNozzleTurbo(const char*, const char*, TWaterGun*);
	virtual s32 getNozzleKind() const { return 4; }
	virtual void movement(const TMarioControllerWork&);
	virtual void animation(int);

	/* 0x714 */ f32 unk714;
};

#endif
