#include <JSystem/JAudio/JASystem/JASChGlobal.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASChAllocQueue.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <types.h>

namespace JASystem {

namespace ChGlobal {
	static TChannelMgr* GLOBAL_CHANNEL = 0;
	static TChannel* CHANNEL           = 0;
	static TOscillator* OSCILLATOR     = 0;
} // namespace ChGlobal

u32 ChGlobal::getGlobalHandle() { return 0; }

TChannel* ChGlobal::getChannelHandle(u32 index)
{
	if (index >= 256)
		return nullptr;
	return &CHANNEL[index];
}

void ChGlobal::init()
{
	char trash[0x10]; // TODO: someone figure this out

	TChannelMgr* mgr = new (JASDram, 32) TChannelMgr;
	GLOBAL_CHANNEL   = mgr;
	mgr->init();

	CHANNEL    = new (JASDram, 32) TChannel[256];
	OSCILLATOR = new (JASDram, 32) TOscillator[1024];

	for (int i = 0; i < 256; ++i) {
		for (u32 j = 0; j < 4; ++j)
			CHANNEL[i].setOscillator(j, &OSCILLATOR[i * 4 + j]);

		CHANNEL[i].init();
		mgr->addListHead(&CHANNEL[i], 0);
		CHANNEL[i].unk4 = mgr;
	}
	mgr->unk0 = 256;
}

int ChGlobal::alloc(TChannelMgr* mgr, u32 param)
{
	int i;
	for (i = 0; i < param; i++) {
		TChannel* channel = GLOBAL_CHANNEL->getListHead(0);
		if (!channel) {
			break;
		}
		mgr->addListHead(channel, 0);
		channel->unk4 = mgr;
		channel->init();
	}
	mgr->unk0 += i;
	GLOBAL_CHANNEL->unk0 -= i;
	return i;
}

int ChGlobal::release(TChannel* channel)
{
	GLOBAL_CHANNEL->addListHead(channel, 0);
	channel->unk4->unk0--;
	GLOBAL_CHANNEL->unk0++;
	channel->unk4 = GLOBAL_CHANNEL;
	return 0;
}

int ChGlobal::releaseAll(TChannelMgr* mgr)
{
	TChannel* channel;
	while (true) {
		channel = mgr->getListHead(0);
		if (!channel)
			break;

		GLOBAL_CHANNEL->addListHead(channel, 0);
		channel->unk4 = GLOBAL_CHANNEL;
	};
	while (true) {
		channel = mgr->getListHead(1);
		if (!channel)
			break;

		GLOBAL_CHANNEL->addListHead(channel, 1);
		channel->unk4  = GLOBAL_CHANNEL;
		channel->unkD0 = 1;
	}
	while (true) {
		channel = mgr->getListHead(2);
		if (!channel)
			break;

		GLOBAL_CHANNEL->addListHead(channel, 2);
		channel->unk4  = GLOBAL_CHANNEL;
		channel->unkD0 = 1;
	}
	while (true) {
		channel = mgr->getListHead(3);
		if (!channel)
			break;

		if (Driver::DSPQueue::deleteQueue(channel))
			GLOBAL_CHANNEL->addListHead(channel, 0);
		else
			GLOBAL_CHANNEL->addListHead(channel, 3);

		channel->unk4 = GLOBAL_CHANNEL;
	}
	GLOBAL_CHANNEL->unk0 += mgr->unk0;
	mgr->unk0 = 0;
	return 0;
}

} // namespace JASystem
