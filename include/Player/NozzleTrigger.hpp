#ifndef NOZZLETRIGGER_HPP
#define NOZZLETRIGGER_HPP

#include <Dolphin/types.h>
#include <Player/NozzleBase.hpp>

class TNozzleTrigger : public TNozzleBase {
public:
	// Add constructor
	TNozzleTrigger(const char* name, const char* prm, TWaterGun* fludd)
	    : TNozzleBase(name, prm, fludd)
	{
		mRunbleOnCharge         = false;
		mSprayState             = INACTIVE;
		mSprayQuarterFramesLeft = 0;
		mTriggerFill            = 0.0f;
		mSoundID                = 0;
	}

	enum SPRAYSTATE { INACTIVE, ACTIVE, DEAD };

	bool mRunbleOnCharge;        // 0x0384
	u8 mSprayState;              // 0x0385
	s16 mSprayQuarterFramesLeft; // 0x0386
	f32 mTriggerFill;            // 0x0388
	u32 mSoundID;                // 0x038C
};

#endif
