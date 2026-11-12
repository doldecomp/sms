#ifndef JASTRACKINTERRUPT_HPP
#define JASTRACKINTERRUPT_HPP

#include <dolphin/types.h>

namespace JASystem {

class TIntrMgr {
public:
	enum InterruptType {
		INTR_PAUSE       = 0,
		INTR_RESUME      = 1,
		INTR_UNKNOWN_2   = 2,
		INTR_PORT0_WRITE = 3,
		INTR_PORT1_WRITE = 4,
		INTR_APPLICATION = 5,
		INTR_TIMER       = 6,
		INTR_TICK        = 7,
	};

	void init();
	void request(u32 interrupt);
	void setIntr(u32 interrupt, void* handler);
	void resetInter(u32 interrupt);
	void* checkIntr();
	void timerProcess();

	void disable() { mEnabled = 0; }
	void enable() { mEnabled = 1; }

	void setTimer(u32 count, u32 period_ticks)
	{
		mTimerFiringsRemaining = count;
		mTimerTicksRemaining   = period_ticks;
		mTimerPeriodTicks      = period_ticks;
	}

public:
	/* 0x00 */ u8 mEnabled;
	/* 0x01 */ u8 mPendingMask;
	/* 0x02 */ u8 mEnabledMask;
	/* 0x03 */ u8 mTimerFiringsRemaining;
	/* 0x04 */ u32 mTimerTicksRemaining;
	/* 0x08 */ u32 mTimerPeriodTicks;
	/* 0x0C */ void* mHandlers[8];
};

} // namespace JASystem

#endif // JASTRACKINTERRUPT_HPP
