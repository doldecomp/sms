#include <JSystem/JAudio/JASystem/JASDSPChannel.hpp>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASRate.hpp>
#include <JSystem/JAudio/JASystem/JASChAllocQueue.hpp>
#include <JSystem/JAudio/JASystem/JASAudioThread.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/base/PPCArch.h>
#include <dolphin/os.h>

namespace JASystem {

TDSPChannel* TDSPChannel::DSPCH = 0;
u32 TDSPChannel::smnUse         = 0;
u32 TDSPChannel::smnFree        = 0x40;
static f32 DSP_LIMIT_RATIO      = 1.1f;

void TDSPChannel::init(u8 number)
{
	mNumber       = number;
	mStatus       = 1;
	unk8          = 0;
	mCBInterval   = 0;
	mCallback     = nullptr;
	mPriority     = 0;
	mPriorityTime = 0;
	mDSPHandle    = DSPInterface::getDSPHandle(number);
}

BOOL TDSPChannel::allocate(u32 param)
{
	if (!isFree())
		return false;

	mStatus   = 0;
	unk8      = param;
	mPriority = 1;
	mDSPHandle->allocInit();
	return true;
}

void TDSPChannel::free()
{
	mStatus   = 1;
	mPriority = 0;
	mCallback = nullptr;
	unk8      = 0;
}

bool TDSPChannel::forceStop()
{
	if (mStatus == 2)
		return false;

	if (mStatus == 1)
		return false;

	if (mDSPHandle->enabled == 0)
		return false;

	smnUse--;
	mDSPHandle->endRequested = 1;
	mDSPHandle->flushChannel();
	mStatus = 2;
	return true;
}

void TDSPChannel::forceDelete()
{
	unk8      = 0;
	mPriority = 0;
	mCallback = nullptr;
}

void TDSPChannel::play()
{
	mDSPHandle->playStart();
	mDSPHandle->flushChannel();
}

void TDSPChannel::stop()
{
	mDSPHandle->enabled = 0;
	mDSPHandle->flushChannel();
}

void TDSPChannel::pause() { }

void TDSPChannel::restart() { }

TDSPChannel* TDSPChannel::getHandle(u32 handle) { return &DSPCH[handle]; }

void TDSPChannel::initAll()
{
	static int first = 1;
	if (first == 0)
		return;

	DSPCH = new (JASDram, 0x20) TDSPChannel[64];
	for (u8 i = 0; i < 64; i++)
		DSPCH[i].init(i);

	first = 0;
}

TDSPChannel* TDSPChannel::alloc(u32 param1, u32 param2)
{
	if (param1)
		return nullptr;

	u32 i = 0;
	do {
		if (DSPCH[i].isFree() && DSPCH[i].allocate(param2)) {
			smnFree--;
			smnUse++;
			return &DSPCH[i];
		}
		i++;
	} while (i < 64);

	return nullptr;
}

int TDSPChannel::free(TDSPChannel* channel, u32 param)
{
	if (!channel)
		return -1;

	if (!(channel->unk8 == param ? TRUE : FALSE))
		return -2;

	if (channel->mStatus == 0)
		smnUse--;

	if (channel->mStatus != 1)
		smnFree++;

	channel->free();
	return 0;
}

JASystem::TDSPChannel* TDSPChannel::getLower()
{
	u8 r31  = 0xff;
	u8 r30  = 0;
	u32 r29 = 0;
	for (u8 i = 0; i < 64; i++) {
		if (DSPCH[i].mStatus == 2)
			continue;

		if (DSPCH[i].isFree()) {
			r30 = i;
			break;
		}

		if (DSPCH[i].mCallback) {
			TDSPChannel* chan = &DSPCH[i];
			if (chan->mPriority <= r31) {
				if (chan->mPriority != r31 || chan->mDSPHandle->unk10C >= r29) {
					r29 = chan->mDSPHandle->unk10C;
					r30 = i;
					r31 = chan->mPriority;
				}
			}
		}
	}
	return &DSPCH[r30];
}

JASystem::TDSPChannel* TDSPChannel::getLowerActive()
{
	u8 r29  = 0xff;
	u8 r28  = 0;
	u32 r27 = 0;
	for (u8 i = 0; i < 64; i++) {
		if (DSPCH[i].mStatus == 2 || DSPCH[i].mStatus == 1)
			continue;

		TDSPChannel* dspch = &DSPCH[i];
		if (dspch->mPriority <= r29) {
			if (dspch->mPriority != r29 || dspch->mDSPHandle->unk10C >= r27) {
				r27 = dspch->mDSPHandle->unk10C;
				r28 = i;
				r29 = dspch->mPriority;
			}
		}
	}
	return &DSPCH[r28];
}

BOOL TDSPChannel::breakLower(u8 param)
{
	TDSPChannel* dspch = getLower();
	if (dspch->mPriority > param)
		return false;

	if (!dspch->isFree()) {
		if (dspch->mCallback)
			dspch->mCBInterval = dspch->mCallback(dspch, 3);

		dspch->forceStop();
	} else {
		return false;
	}
	return true;
}

BOOL TDSPChannel::breakLowerActive(u8 param)
{
	TDSPChannel* dspch = getLowerActive();
	if (dspch->mPriority > param)
		return false;

	if (!dspch->isFree()) {
		if (dspch->mCallback)
			dspch->mCBInterval = dspch->mCallback(dspch, 3);

		dspch->forceStop();
	} else {
		return false;
	}
	return true;
}

void TDSPChannel::setLimitDSP(f32 limit) { DSP_LIMIT_RATIO = limit; }

static OSTick history[10] = { 1000000, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static OSTick old_time;

f32* TDSPChannel::getHistory() { return (f32*)history; }

void TDSPChannel::updateAll()
{
	DSPInterface::DSPBuffer* dspBuffer;
	{
		OSTick time   = OSGetTick();
		OSTick delta  = time - old_time;
		old_time      = time;
		u32 var2      = 7 - AudioThread::getDSPSyncCount();
		history[var2] = delta;
		if (var2)
			if (f32(history[0]) / delta < DSP_LIMIT_RATIO)
				breakLowerActive(126);
	}

	for (u32 i = 0; i < 64; i++) {
		dspBuffer               = DSPCH[i].mDSPHandle;
		TDSPChannel* dspChannel = &DSPCH[i];
		if (dspChannel->getStatus() == 1)
			continue;

		if (dspBuffer->isFinish()) {
			if (dspChannel->mCallback)
				dspChannel->setCBInterval(dspChannel->mCallback(dspChannel, 2));

			dspBuffer->replyFinishRequest();
			dspBuffer->flushChannel();
		}
		if (dspBuffer->endRequested == 0) {
			dspBuffer->unk10C++;
			if (dspBuffer->unk10C == dspChannel->getPriorityTime()
			    && dspChannel->mCallback)
				dspChannel->mCallback(dspChannel, 4);
		}
		if (dspChannel->mCallback) {
			dspChannel->decCBInterval();

			if (dspChannel->getCBInterval() == 0) {
				dspChannel->setCBInterval(dspChannel->mCallback(dspChannel, 0));
				if (dspChannel->getCBInterval() == 0) {
					dspBuffer->replyFinishRequest();
					Driver::DSPQueue::deQueue(1);
					dspBuffer->flushChannel();
				}
			}
		}
	}
	Driver::DSPQueue::checkQueue();
	PPCSync();
}

u32 TDSPChannel::getNumUse() { return smnUse; }

u32 TDSPChannel::getNumFree() { return smnFree; }

} // namespace JASystem
