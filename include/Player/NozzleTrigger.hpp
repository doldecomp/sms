#ifndef NOZZLETRIGGER_HPP
#define NOZZLETRIGGER_HPP

#include <Player/NozzleBase.hpp>
#include <Player/MarioMain.hpp>

class TNozzleTrigger : public TNozzleBase {
public:
	TNozzleTrigger(const char* name, const char* prm, TWaterGun* fludd)
	    : TNozzleBase(name, prm, fludd)
	{
		unk38C = 0xffffffff;
		unk384 = false;
		unk385 = INACTIVE;
		unk36C = 0;
		unk386 = 0;
		unk388 = 0.0f;
	}

	virtual void init();
	virtual s32 getNozzleKind() const { return 1; };
	virtual void movement(const TMarioControllerWork&);
	virtual void emit(int);
	virtual void animation(int);

	// Inactive = not holding R, Active = charging R, Dead = R Waiting to be
	// depressed
	enum SPRAYSTATE { INACTIVE, ACTIVE, DEAD };

	/* 0x384 */ bool unk384; // mRumbleOnCharge
	/* 0x385 */ s8 unk385;   // mSprayState, Current spray state
	/* 0x386 */ s16 unk386;  // Quarter frames left of spray (i think)
	/* 0x388 */ f32 unk388;  // mTriggerFill - How far the trigger has gotten
	/* 0x38C */ u32 unk38C;  // mSoundID - The sound to play when triggering
};

#endif
