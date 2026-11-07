#include <JSystem/JAudio/JASystem/JASChGlobal.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASChAllocQueue.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/types.h>

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
		mgr->addListHead(&CHANNEL[i], TChannelMgr::CHAN_LIST_FREE);
		CHANNEL[i].mManager = mgr;
	}
	mgr->mManagedChannels = 256;
}

u32 ChGlobal::alloc(TChannelMgr* mgr, u32 count)
{
	int i;
	for (i = 0; i < count; i++) {
		TChannel* channel
		    = GLOBAL_CHANNEL->getListHead(TChannelMgr::CHAN_LIST_FREE);
		if (!channel)
			break;

		mgr->addListHead(channel, TChannelMgr::CHAN_LIST_FREE);
		channel->mManager = mgr;
		channel->init();
	}
	mgr->mManagedChannels += i;
	GLOBAL_CHANNEL->mManagedChannels -= i;
	return i;
}

int ChGlobal::release(TChannel* channel)
{
	GLOBAL_CHANNEL->addListHead(channel, TChannelMgr::CHAN_LIST_FREE);
	channel->mManager->mManagedChannels--;
	GLOBAL_CHANNEL->mManagedChannels++;
	channel->mManager = GLOBAL_CHANNEL;
	return 0;
}

int ChGlobal::releaseAll(TChannelMgr* mgr)
{
	TChannel* channel;
	while (true) {
		channel = mgr->getListHead(TChannelMgr::CHAN_LIST_FREE);
		if (!channel)
			break;

		GLOBAL_CHANNEL->addListHead(channel, TChannelMgr::CHAN_LIST_FREE);
		channel->mManager = GLOBAL_CHANNEL;
	};
	while (true) {
		channel = mgr->getListHead(TChannelMgr::CHAN_LIST_PLAYING);
		if (!channel)
			break;

		GLOBAL_CHANNEL->addListHead(channel, TChannelMgr::CHAN_LIST_PLAYING);
		channel->mManager           = GLOBAL_CHANNEL;
		channel->mPreserveDspConfig = TRUE;
	}
	while (true) {
		channel = mgr->getListHead(TChannelMgr::CHAN_LIST_RELEASE);
		if (!channel)
			break;

		GLOBAL_CHANNEL->addListHead(channel, TChannelMgr::CHAN_LIST_RELEASE);
		channel->mManager           = GLOBAL_CHANNEL;
		channel->mPreserveDspConfig = TRUE;
	}
	while (true) {
		channel = mgr->getListHead(TChannelMgr::CHAN_LIST_PENDING);
		if (!channel)
			break;

		if (Driver::DSPQueue::deleteQueue(channel))
			GLOBAL_CHANNEL->addListHead(channel, TChannelMgr::CHAN_LIST_FREE);
		else
			GLOBAL_CHANNEL->addListHead(channel,
			                            TChannelMgr::CHAN_LIST_PENDING);

		channel->mManager = GLOBAL_CHANNEL;
	}
	GLOBAL_CHANNEL->mManagedChannels += mgr->mManagedChannels;
	mgr->mManagedChannels = 0;
	return 0;
}

} // namespace JASystem
