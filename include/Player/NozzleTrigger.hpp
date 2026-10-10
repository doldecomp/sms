#ifndef NOZZLETRIGGER_HPP
#define NOZZLETRIGGER_HPP

#include <Player/NozzleBase.hpp>

class TNozzleTrigger : public TNozzleBase {
public:
	TNozzleTrigger(const char* name, const char* prm, TWaterGun* fludd);
	virtual void init();
	virtual s32 getNozzleKind() const { return 1; };
	virtual void movement(const TMarioControllerWork&);
	virtual void emit(int);
	virtual void animation(int);

	// Fabricated
	int getSprayState() const { return mSprayState; }

	// Inactive = not holding R, Active = charging R, Dead = R Waiting to be
	// depressed
	enum SprayState {
		SPRAY_STATE_INACTIVE = 0,
		SPRAY_STATE_ACTIVE   = 1,
		SPRAY_STATE_DEAD     = 2
	};

	/* 0x384 */ bool mRumbleOnCharge;
	/* 0x385 */ u8 mSprayState;
	/* 0x386 */ s16 mSprayTimer;
	/* 0x388 */ f32 mInsidePressure;
	/* 0x38C */ u32 mSoundId;
};

#endif
