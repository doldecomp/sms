#ifndef PLAYER_NOZZLETURBO_HPP
#define PLAYER_NOZZLETURBO_HPP

#include <Player/NozzleBase.hpp>

// TODO: UNUSED nozzle; any additional member layout is unknown.
class TNozzleTurbo : public TNozzleBase {
public:
	TNozzleTurbo(const char*, const char*, TWaterGun*);
	virtual s32 getNozzleKind() const { return 0; }
	virtual void movement(const TMarioControllerWork&);
	virtual void animation(int);
};

#endif
