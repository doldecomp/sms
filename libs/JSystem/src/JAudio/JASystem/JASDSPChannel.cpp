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

TDSPChannel* TDSPChannel::DSPCH = nullptr;
u32 TDSPChannel::smnUse         = 0;
u32 TDSPChannel::smnFree        = DSPInterface::DSP_VOICE_COUNT;
static f32 DSP_LIMIT_RATIO      = 1.1f;

void TDSPChannel::init(u8 number)
{
	mNumber       = number;
	mStatus       = STATUS_FREE;
	mSign         = 0;
	mCBInterval   = 0;
	mCallback     = nullptr;
	mPriority     = 0;
	mPriorityTime = 0;
	mDSPHandle    = DSPInterface::getDSPHandle(number);
}

BOOL TDSPChannel::allocate(uintptr_t sign)
{
	if (!isFree())
		return false;

	mStatus   = STATUS_ALLOCATED;
	mSign     = sign;
	mPriority = 1;
	mDSPHandle->allocInit();
	return true;
}

void TDSPChannel::free()
{
	mStatus   = STATUS_FREE;
	mPriority = 0;
	mCallback = nullptr;
	mSign     = 0;
}

bool TDSPChannel::forceStop()
{
	if (mStatus == STATUS_STOPPING)
		return false;

	if (mStatus == STATUS_FREE)
		return false;

	if (mDSPHandle->enabled == 0)
		return false;

	smnUse--;
	mDSPHandle->endRequested = 1;
	mDSPHandle->flushChannel();
	mStatus = STATUS_STOPPING;
	return true;
}

void TDSPChannel::forceDelete()
{
	mSign     = 0;
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

void TDSPChannel::pause()
{
	mDSPHandle->useConstantSample = 1;
	mDSPHandle->flushChannel();
}

void TDSPChannel::restart()
{
	mDSPHandle->useConstantSample = 0;
	mDSPHandle->flushChannel();
}

TDSPChannel* TDSPChannel::getHandle(u32 handle) { return &DSPCH[handle]; }

void TDSPChannel::initAll()
{
	static int first = 1;
	if (first == 0)
		return;

	DSPCH = new (JASDram, 0x20) TDSPChannel[DSPInterface::DSP_VOICE_COUNT];
	for (u8 i = 0; i < DSPInterface::DSP_VOICE_COUNT; i++)
		DSPCH[i].init(i);

	first = 0;
}

TDSPChannel* TDSPChannel::alloc(u32 param1, uintptr_t sign)
{
	if (param1)
		return nullptr;

	for (u32 i = 0; i < DSPInterface::DSP_VOICE_COUNT; ++i) {
		if (DSPCH[i].isFree() && DSPCH[i].allocate(sign)) {
			smnFree--;
			smnUse++;
			return &DSPCH[i];
		}
	}

	return nullptr;
}

int TDSPChannel::free(TDSPChannel* channel, uintptr_t sign)
{
	if (!channel)
		return -1;

	if (!channel->checkSign(sign))
		return -2;

	if (channel->mStatus == STATUS_ALLOCATED)
		smnUse--;

	if (channel->mStatus != STATUS_FREE)
		smnFree++;

	channel->free();
	return 0;
}

JASystem::TDSPChannel* TDSPChannel::getLower()
{
	u8 lowestFoundPrio          = 0xff;
	u8 bestFoundIndex           = 0;
	u32 highestFoundUpdateCount = 0;
	for (u8 i = 0; i < DSPInterface::DSP_VOICE_COUNT; i++) {
		if (DSPCH[i].mStatus == STATUS_STOPPING)
			continue;

		if (DSPCH[i].isFree()) {
			bestFoundIndex = i;
			break;
		}

		if (DSPCH[i].mCallback) {
			TDSPChannel* chan = &DSPCH[i];
			if (chan->mPriority <= lowestFoundPrio) {
				if (chan->mPriority != lowestFoundPrio
				    || chan->mDSPHandle->updateCount
				           >= highestFoundUpdateCount) {
					highestFoundUpdateCount = chan->mDSPHandle->updateCount;
					bestFoundIndex          = i;
					lowestFoundPrio         = chan->mPriority;
				}
			}
		}
	}
	return &DSPCH[bestFoundIndex];
}

JASystem::TDSPChannel* TDSPChannel::getLowerActive()
{
	u8 lowestFoundPrio          = 0xff;
	u8 bestFoundIndex           = 0;
	u32 highestFoundUpdateCount = 0;
	for (u8 i = 0; i < DSPInterface::DSP_VOICE_COUNT; i++) {
		if (DSPCH[i].mStatus == STATUS_STOPPING
		    || DSPCH[i].mStatus == STATUS_FREE)
			continue;

		TDSPChannel* dspch = &DSPCH[i];
		if (dspch->mPriority <= lowestFoundPrio) {
			if (dspch->mPriority != lowestFoundPrio
			    || dspch->mDSPHandle->updateCount >= highestFoundUpdateCount) {
				highestFoundUpdateCount = dspch->mDSPHandle->updateCount;
				bestFoundIndex          = i;
				lowestFoundPrio         = dspch->mPriority;
			}
		}
	}
	return &DSPCH[bestFoundIndex];
}

BOOL TDSPChannel::breakLower(u8 priority)
{
	TDSPChannel* dspch = getLower();
	if (dspch->mPriority > priority)
		return false;

	if (!dspch->isFree()) {
		if (dspch->mCallback)
			dspch->onUpdate(EVENT_FORCE_STOP);

		dspch->forceStop();
	} else {
		return false;
	}
	return true;
}

BOOL TDSPChannel::breakLowerActive(u8 priority)
{
	TDSPChannel* dspch = getLowerActive();
	if (dspch->mPriority > priority)
		return false;

	if (!dspch->isFree()) {
		if (dspch->mCallback)
			dspch->onUpdate(EVENT_FORCE_STOP);

		dspch->forceStop();
	} else {
		return false;
	}
	return true;
}

void TDSPChannel::setLimitDSP(f32 limit) { DSP_LIMIT_RATIO = limit; }

static OSTick history[10] = { 1000000, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static OSTick old_time;

OSTick* TDSPChannel::getHistory() { return history; }

void TDSPChannel::updateAll()
{
	{
		OSTick time = OSGetTick();
		OSTick dt   = time - old_time;
		old_time    = time;

		u32 subframesDone      = 7 - AudioThread::getDSPSyncCount();
		history[subframesDone] = dt;
		// history[0] is the dt since last batch of subframes finished.
		// If that leeway becomes too small compared to however long subframes
		// take right now (times 1.1), we sacrifice a low-prio voice.
		if (subframesDone != 0)
			if (f32(history[0]) / dt < DSP_LIMIT_RATIO)
				breakLowerActive(126);
	}

	TDSPChannel* dspChannel;
	DSPInterface::DSPBuffer* dspBuffer;

	for (u32 i = 0; i < DSPInterface::DSP_VOICE_COUNT; i++) {
		dspChannel = &DSPCH[i];
		u8 status  = dspChannel->getStatus();
		dspBuffer  = dspChannel->mDSPHandle;
		if (status == STATUS_FREE)
			continue;

		if (dspBuffer->isFinish()) {
			if (dspChannel->mCallback)
				dspChannel->onUpdate(EVENT_FINISHED);

			dspBuffer->replyFinishRequest();
			dspBuffer->flushChannel();
		}
		if (dspBuffer->endRequested == 0) {
			dspBuffer->updateCount++;
			if (dspBuffer->updateCount == dspChannel->getPriorityTime()
			    && dspChannel->mCallback)
				dspChannel->mCallback(dspChannel, EVENT_PRIORITY_EXPIRED);
		}
		if (dspChannel->mCallback) {
			dspChannel->decCBInterval();

			if (dspChannel->getCBInterval() == 0) {
				dspChannel->onUpdate(EVENT_UPDATE);
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
