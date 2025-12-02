#ifndef NOZZLETRIGGER_HPP
#define NOZZLETRIGGER_HPP

#include <Player/NozzleBase.hpp>

class TNozzleTrigger : public TNozzleBase {
public:
	// Add constructor
	TNozzleTrigger(const char* name, const char* prm, TWaterGun* fludd)
	    : TNozzleBase(name, prm, fludd)
	{
		mSoundID                = 0xffffffff;
		mRumbleOnCharge         = false;
		mSprayState             = INACTIVE;
		mAnimState              = 0;
		mSprayQuarterFramesLeft = 0;
		mTriggerFill            = 0.0f;
	}

	virtual void init();
	virtual s32 getNozzleKind() const { return 1; };
	virtual void movement(const void*);
	virtual void emit(int);
	virtual void animation(int);

	enum SPRAYSTATE { INACTIVE, ACTIVE, DEAD };

	bool mRumbleOnCharge;        // 0x0384
	s8 mSprayState;              // 0x0385
	s16 mSprayQuarterFramesLeft; // 0x0386
	f32 mTriggerFill;            // 0x0388
	u32 mSoundID;                // 0x038C
};

#endif
