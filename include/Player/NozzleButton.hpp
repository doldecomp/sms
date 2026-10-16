#ifndef PLAYER_NOZZLEBUTTON_HPP
#define PLAYER_NOZZLEBUTTON_HPP

#include <Player/NozzleBase.hpp>

// TODO: UNUSED nozzle; any additional member layout is unknown.
// Implementation found in demo disc
// Still needs validation, mostly clanker pass
class TNozzleButton : public TNozzleBase {
public:
	TNozzleButton(const char*, const char*, TWaterGun*);
	virtual void init();
	virtual s32 getNozzleKind() const { return 0; }
	virtual void movement(const TMarioControllerWork&);
	virtual void emit(int);
	virtual void animation(int);

	/* 0x384 */ s32 mButtonState;
};

#endif
