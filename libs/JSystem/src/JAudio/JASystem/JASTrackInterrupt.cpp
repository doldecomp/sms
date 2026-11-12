#include <JSystem/JAudio/JASystem/JASTrackInterrupt.hpp>
#include <dolphin/types.h>

namespace JASystem {

void TIntrMgr::init()
{
	mEnabled               = 1;
	mPendingMask           = 0;
	mEnabledMask           = 0;
	mTimerFiringsRemaining = 0;
	mTimerTicksRemaining   = 0;
	mTimerPeriodTicks      = 0;
	for (int i = 0; i < 8; i++)
		mHandlers[i] = nullptr;
}

void TIntrMgr::request(u32 interrupt)
{
	if ((mEnabledMask & 1 << interrupt) == 0)
		return;

	mPendingMask |= 1 << interrupt;
}

void TIntrMgr::setIntr(u32 interrupt, void* handler)
{
	mEnabledMask |= 1 << interrupt;
	mHandlers[interrupt] = handler;
}

void TIntrMgr::resetInter(u32 interrupt) { mEnabledMask &= ~(1 << interrupt); }

void* TIntrMgr::checkIntr()
{
	if (mEnabled == 0)
		return nullptr;

	u32 r4 = mEnabledMask & mPendingMask;
	for (u32 i = 0; r4; i++) {
		if (r4 & 1) {
			mPendingMask &= ~(1 << i);
			return mHandlers[i];
		}
		r4 >>= 1;
	}
	return nullptr;
}

void TIntrMgr::timerProcess()
{
	if (mTimerTicksRemaining == 0)
		return;

	mTimerTicksRemaining--;
	if (mTimerTicksRemaining != 0)
		return;

	request(INTR_TIMER);
	if (mTimerFiringsRemaining) {
		mTimerFiringsRemaining--;
		if (mTimerFiringsRemaining)
			mTimerTicksRemaining = mTimerPeriodTicks;
	} else {
		mTimerTicksRemaining = mTimerPeriodTicks;
	}
}

} // namespace JASystem
