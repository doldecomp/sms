#ifndef NOZZLETRIGGER_HPP
#define NOZZLETRIGGER_HPP

#include <Player/NozzleBase.hpp>

class TNozzleTrigger : public TNozzleBase {
public:
	// Add constructor
	TNozzleTrigger(const char* name, const char* prm, TWaterGun* fludd)
	    : TNozzleBase(name, prm, fludd)
	{
		unk38C     = 0xffffffff;
		unk384     = false;
		unk385     = INACTIVE;
		mAnimState = 0;
		unk386     = 0;
		unk388     = 0.0f;
	}

	virtual void init();
	virtual s32 getNozzleKind() const { return 1; };
	virtual void movement(const TMarioControllerWork&);
	virtual void emit(int);
	virtual void animation(int);

	enum SPRAYSTATE { INACTIVE, ACTIVE, DEAD };

	bool unk384; // 0x0384 - Do rumble on charge
	s8 unk385;   // 0x0385 - Spray state
	s16 unk386;  // 0x0386 - Spray quarterframes left
	f32 unk388;  // 0x0388 - trigger on fill
	u32 unk38C;  // 0x038C - SoundID to play on charge
};

#endif
