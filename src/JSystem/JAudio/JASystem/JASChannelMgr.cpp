#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>
#include <JSystem/JAudio/JASystem/JASChGlobal.hpp>
#include <JSystem/JAudio/JASystem/JASDSPChannel.hpp>
#include <JSystem/JAudio/JASystem/JASChAllocQueue.hpp>
#include <types.h>

namespace JASystem {

void TChannelMgr::init()
{
	unk8  = 0;
	unkC  = 0;
	unk10 = 0;
	unk14 = 0;

	unk4             = 0;
	mManagedChannels = 0;
	unk70            = TRUE;

	mVolume = 1.0f;
	mPitch  = 1.0f;
	mPan    = 0.5f;
	mFxmix  = 0.0f;
	mDolby  = 0.0f;

	for (int i = 0; i < 8; ++i)
		unk2C[i] = 0;

	unk2C[0] = 0x7FFF;
	unk4C    = 0;

	for (int i = 0; i < 4; ++i)
		unk3C[i] = 0;

	for (int i = 0; i < 6; ++i)
		unk5A[i] = 0;

	unk60    = 0;
	unk3C[0] = 0x7FFF;
	unk61    = 0;

	unk4E[0] = 0x150;
	unk4E[1] = 0x210;
	unk4E[2] = 0x352;
	unk4E[3] = 0x412;
	unk4E[4] = 0;
	unk4E[5] = 0;

	unk68 = 0x20103;
	unk6C = 600;

	unk62[0] = 0xD;
	unk62[1] = 0xD;
	unk62[2] = 0xD;
}

void TChannelMgr::stopAll()
{
	TChannel* channel = unkC;
	while (channel) {
		TChannel* r31 = channel->mNext;
		channel->stop(0);
		channel = r31;
	}
}

void TChannelMgr::stopAllRelease()
{
	for (TChannel* channel = unk10; channel; channel = channel->mNext)
		for (u32 i = 0; i < 4; ++i)
			channel->forceStopOsc(i);
}

BOOL TChannelMgr::checkGlobalRelease() { return mManagedChannels != 0; }

void TChannelMgr::addListHead(TChannel* channel, u32 param)
{
	TChannel** list;
	switch (param) {
	case 0:
		list = &unk8;
		break;
	case 1:
		list = &unkC;
		break;
	case 2:
		list = &unk10;
		break;
	case 3:
		list = &unk14;
		break;
	default:
		list = nullptr;
		break;
	}
	TChannel* prevHead = *list;
	channel->unk8      = list;
	*list              = channel;
	channel->mNext     = prevHead;
}

void TChannelMgr::addListTail(TChannel* channel, u32 param)
{
	TChannel** list;
	switch (param) {
	case 0:
		list = &unk8;
		break;
	case 1:
		list = &unkC;
		break;
	case 2:
		list = &unk10;
		break;
	case 3:
		list = &unk14;
		break;
	default:
		list = nullptr;
		break;
	}

	TChannel* head = *list;
	channel->unk8  = list;

	if (!head) {
		*list          = channel;
		channel->mNext = nullptr;
		return;
	}

	while (true) {
		if (!head->mNext) {
			head->mNext    = channel;
			channel->mNext = nullptr;
			break;
		}
		head = head->mNext;
	}
}

TChannel* TChannelMgr::getListHead(u32 param)
{
	TChannel** list;
	switch (param) {
	case 0:
		list = &unk8;
		break;
	case 1:
		list = &unkC;
		break;
	case 2:
		list = &unk10;
		break;
	case 3:
		list = &unk14;
		break;
	default:
		list = nullptr;
		break;
	}

	TChannel* head = *list;
	if (!head)
		return nullptr;

	*list      = head->mNext;
	head->unk8 = nullptr;
	return head;
}

u32 TChannelMgr::countList(u32 param) { return 0; }

int TChannelMgr::cutList(TChannel* channel)
{
	int i = 0;

	TChannel* head = *channel->unk8;

	if (!head)
		return -1;

	if (head == channel) {
		*channel->unk8 = channel->mNext;
		channel->unk8  = nullptr;
		return 0;
	}

	for (;;) {
		++i;
		if (head == nullptr)
			return -1;
		if (head->mNext == channel)
			break;
		head = head->mNext;
	}

	head->mNext   = channel->mNext;
	channel->unk8 = nullptr;
	return i;
}

void TChannelMgr::initAllocChannel(u32 param)
{
	if (checkGlobalRelease() != FALSE)
		ChGlobal::releaseAll(this);

	init();

	ChGlobal::alloc(this, param);
	unk70 = !param ? FALSE : TRUE;
}

TChannel* TChannelMgr::getLogicalChannel(u32 param)
{
	BOOL bVar1 = false;
	if (!checkLimitStart(param)) {
		if ((param >> 0x18) & 0x10)
			return nullptr;
		bVar1 = true;
	}

	TChannel* head = getListHead(0);
	if (head == nullptr) {
		if (!ChGlobal::alloc(this, 1))
			return nullptr;

		++unk4;
		head = getListHead(0);
		if (unk70 == TRUE) {
			TChannel* chan = getListHead(2);
			if (chan == nullptr)
				chan = getListHead(1);
			if (chan != nullptr) {
				chan->forceStopOsc(0);
				addListHead(chan, 3);
				if (chan->unk20 != nullptr)
					chan->unk20->forceStop();
			}
		}
	}

	head->init();
	if (bVar1) {
		head->unk2 = 1;
		head->unk3 = 1;
	}

	head->unkC8 = param;
	head->unk18 = 0;
	head->setPanPower(1.0f, 1.0f, 1.0f, 1.0f);
	return head;
}

int TChannelMgr::receiveAllChannels(TChannelMgr* other)
{
	TChannel* stolen;
	for (;;) {
		stolen = other->getListHead(0);
		if (!stolen)
			break;
		addListHead(stolen, 0);
		stolen->unk4 = this;
	}
	for (;;) {
		stolen = other->getListHead(1);
		if (!stolen)
			break;
		addListHead(stolen, 1);
		stolen->unk4  = this;
		stolen->unkD0 = 1;
	}
	for (;;) {
		stolen = other->getListHead(2);
		if (!stolen)
			break;
		addListHead(stolen, 2);
		stolen->unk4  = this;
		stolen->unkD0 = 1;
	}
	for (;;) {
		stolen = other->getListHead(3);
		if (!stolen)
			break;
		if (Driver::DSPQueue::deleteQueue(stolen))
			addListHead(stolen, 0);
		else
			addListHead(stolen, 3);
		stolen->unk4 = this;
	}
	mManagedChannels += other->mManagedChannels;
	other->mManagedChannels = 0;
	unk4 += other->unk4;
	other->unk4 = 0;

	return 0;
}

namespace Driver {
	const u8 polys_table[16] = { 0x0, 0x1, 0x2, 0x3,  0x4,  0x5,  0x6,  0x8,
		                         0xa, 0xc, 0xe, 0x10, 0x14, 0x18, 0x1c, 0x20 };
} // namespace Driver

// BUG: oooops, all infinite loops!
BOOL TChannelMgr::checkLimitStart(u32 param)
{
	u32 thing = Driver::polys_table[(param >> 0x18) & 0xF];
	if (!thing)
		return true;

	int i = 0;

	TChannel* head = unk10;
	if (((param >> 0x18) & 0x20)) {
		for (;;) {
			if (!head)
				break;

			if (head->unkC8 == param && head->unk2 == 0)
				++i;
		}

		head = unkC;
		for (;;) {
			if (!head)
				break;

			if (head->unkC8 == param && head->unk2 == 0)
				++i;
		}

		if (i == thing)
			return false;

		if (i > thing)
			return false;

		return true;
	} else {
		TChannel* found = nullptr;
		for (;;) {
			if (!head)
				break;

			if (head->unkC8 == param && head->unk2 == 0) {
				if (found == nullptr)
					found = head;
				if (i == thing) {
					found->forceStopLogicalChannel();
					return 1;
				}
				++i;
			}
		}

		head = unkC;
		for (;;) {
			if (!head)
				break;

			if (head->unkC8 == param && head->unk2 == 0) {
				if (found == nullptr)
					found = head;
				if (i == thing) {
					if ((param >> 0x18) & 0x10)
						found->forceStopLogicalChannel();
					else
						head->setPauseFlagReq(1);

					return 1;
				}
				++i;
			}
		}
		return true;
	}
}

void TChannelMgr::checkLimitStop(TChannel* channel, u32 param)
{
	TChannel* chan = channel->unk4->unkC;
	u32 thing      = Driver::polys_table[(param >> 0x18) & 0xF];

	if (param == 0)
		return;
	if (thing == 0)
		return;

	if ((param >> 0x18) & 0x20) {
		for (;;) {
			if (!chan)
				break;
			if (chan->unkC8 == param && chan->unk2 == 1) {
				chan->setPauseFlagReq(0);
				return;
			}
			chan = chan->mNext;
		}
	} else {
		TChannel* found = nullptr;
		for (;;) {
			if (!chan)
				break;
			if (chan->unkC8 == param && chan->unk2 == 1)
				found = chan;
			chan = chan->mNext;
		}

		if (found)
			found->setPauseFlagReq(0);
	}
}

} // namespace JASystem
