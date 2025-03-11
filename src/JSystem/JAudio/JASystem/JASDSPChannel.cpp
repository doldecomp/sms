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

void TDSPChannel::init(u8 param)
{
	unk0  = param;
	unk1  = 1;
	unk8  = 0;
	unk6  = 0;
	unk10 = nullptr;
	unk3  = 0;
	unk4  = 0;
	unkC  = DSPInterface::getDSPHandle(param);
}

BOOL TDSPChannel::allocate(u32 param)
{
	if (!isUnk1One())
		return false;

	unk1 = 0;
	unk8 = param;
	unk3 = 1;
	unkC->allocInit();
	return true;
}

void TDSPChannel::free()
{
	unk1  = 1;
	unk3  = 0;
	unk10 = nullptr;
	unk8  = 0;
}

bool TDSPChannel::forceStop()
{
	if (unk1 == 2)
		return false;

	if (unk1 == 1)
		return false;

	if (unkC->unk0 == 0)
		return false;

	smnUse--;
	unkC->unk10A = 1;
	unkC->flushChannel();
	unk1 = 2;
	return true;
}

void TDSPChannel::forceDelete()
{
	unk8  = 0;
	unk3  = 0;
	unk10 = nullptr;
}

void TDSPChannel::play()
{
	unkC->playStart();
	unkC->flushChannel();
}

void TDSPChannel::stop()
{
	unkC->unk0 = 0;
	unkC->flushChannel();
}

void TDSPChannel::pause() { }

void TDSPChannel::restart() { }

TDSPChannel* TDSPChannel::getHandle(u32 handle) { return 0; }

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
		if (DSPCH[i].isUnk1One() && DSPCH[i].allocate(param2)) {
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

	if (channel->unk1 == 0)
		smnUse--;

	if (channel->unk1 != 1)
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
		TDSPChannel* dspch = &DSPCH[i];
		if (dspch->unk1 == 2)
			continue;

		if (dspch->unk1 == 1 ? TRUE : FALSE) {
			r30 = i;
			break;
		}

		if (dspch->unk10) {
			if (dspch->unk3 <= r31) {
				if (dspch->unk3 != r31 || dspch->unkC->unk10C >= r29) {
					r29 = dspch->unkC->unk10C;
					r30 = i;
					r31 = dspch->unk3;
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
		TDSPChannel* dspch = &DSPCH[i];
		if (dspch->unk1 == 2 || dspch->unk1 == 1)
			continue;

		if (dspch->unk3 <= r29) {
			if (dspch->unk3 != r29 || dspch->unkC->unk10C >= r27) {
				r27 = dspch->unkC->unk10C;
				r28 = i;
				r29 = dspch->unk3;
			}
		}
	}
	return &DSPCH[r28];
}

BOOL TDSPChannel::breakLower(u8 param)
{
	TDSPChannel* dspch = getLower();
	if (dspch->unk3 > param)
		return false;

	if (!(dspch->unk1 == 1 ? TRUE : FALSE)) {
		if (dspch->unk10)
			dspch->unk6 = dspch->unk10(dspch, 3);

		dspch->forceStop();
	} else {
		return false;
	}
	return true;
}

BOOL TDSPChannel::breakLowerActive(u8 param)
{
	TDSPChannel* dspch = getLowerActive();
	if (dspch->unk3 > param)
		return false;

	if (!dspch->isUnk1One()) {
		if (dspch->unk10)
			dspch->unk6 = dspch->unk10(dspch, 3);

		dspch->forceStop();
	} else {
		return false;
	}
	return true;
}

void TDSPChannel::setLimitDSP(f32 limit) { }

static OSTick history[10] = { 1000000, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static OSTick old_time;

f32* TDSPChannel::getHistory() { return 0; }

void TDSPChannel::updateAll()
{
	DSPInterface::DSPBuffer* dspBuffer;
	{
		OSTick time = OSGetTick();
		u32 var2;
		OSTick var3   = time - old_time;
		old_time      = time;
		var2          = 7 - AudioThread::getDSPSyncCount();
		history[var2] = var3;
		if (var2)
			if (f32(history[0]) / var3 < DSP_LIMIT_RATIO)
				breakLowerActive(126);
	}

	for (u32 i = 0; i < 64; i++) {
		dspBuffer               = DSPCH[i].unkC;
		TDSPChannel* dspChannel = &DSPCH[i];
		if (dspChannel->unk1 == 1)
			continue;

		if (dspBuffer->unk2 != 0) {
			if (dspChannel->unk10)
				dspChannel->unk6 = dspChannel->unk10(dspChannel, 2);

			dspBuffer->unk2 = 0;
			dspBuffer->unk0 = 0;
			dspBuffer->flushChannel();
		}
		if (dspBuffer->unk10A == 0) {
			dspBuffer->unk10C++;
			if (dspBuffer->unk10C == dspChannel->unk4 && dspChannel->unk10)
				dspChannel->unk10(dspChannel, 4);
		}
		if (dspChannel->unk10) {
			dspChannel->unk6--;

			if (dspChannel->unk6 == 0) {
				dspChannel->unk6 = dspChannel->unk10(dspChannel, 0);
				if (dspChannel->unk6 == 0) {
					dspBuffer->unk2 = 0;
					dspBuffer->unk0 = 0;
					Driver::DSPQueue::deQueue(1);
					dspBuffer->flushChannel();
				}
			}
		}
	}
	Driver::DSPQueue::checkQueue();
	PPCSync();
}

u32 TDSPChannel::getNumUse() { return 0; }

u32 TDSPChannel::getNumFree() { return 0; }

} // namespace JASystem
