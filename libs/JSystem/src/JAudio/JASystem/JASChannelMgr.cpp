#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <JSystem/JAudio/JASystem/JASChGlobal.hpp>
#include <JSystem/JAudio/JASystem/JASDSPChannel.hpp>
#include <JSystem/JAudio/JASystem/JASChAllocQueue.hpp>
#include <dolphin/types.h>

namespace JASystem {

void TChannelMgr::init()
{
	mFreeChannels    = nullptr;
	mPlayingChannels = nullptr;
	mReleaseChannels = nullptr;
	mPendingChannels = nullptr;

	mGlobalChannelBorrows = 0;
	mManagedChannels      = 0;
	mMaintainPoolSize     = TRUE;

	mVolume = 1.0f;
	mPitch  = 1.0f;
	mPan    = 0.5f;
	mFxmix  = 0.0f;
	mDolby  = 0.0f;

	for (int i = 0; i < 8; ++i)
		mFirCoeffs[i] = 0;

	mFirCoeffs[0] = 0x7FFF;
	mDistFilter   = 0;

	for (int i = 0; i < 4; ++i)
		mIirCoeffs[i] = 0;

	for (int i = 0; i < 6; ++i)
		mMixerDelaySamples[i] = 0;

	mMixerDelayMax = 0;
	mIirCoeffs[0]  = 0x7FFF;
	mFilterMode    = 0;

	mMixConfig[0] = (DSPInterface::DSPBuffer::SEND_FRONT_LEFT << 8)
	                | (TChannel::MixConfig::GAIN_INV_PAN << 4)
	                | TChannel::MixConfig::GAIN_NONE;
	mMixConfig[1] = (DSPInterface::DSPBuffer::SEND_FRONT_RIGHT << 8)
	                | (TChannel::MixConfig::GAIN_PAN << 4)
	                | TChannel::MixConfig::GAIN_NONE;
	mMixConfig[2] = (DSPInterface::DSPBuffer::SEND_EFFECTS_0 << 8)
	                | (TChannel::MixConfig::GAIN_INV_PAN << 4)
	                | TChannel::MixConfig::GAIN_FXMIX;
	mMixConfig[3] = (DSPInterface::DSPBuffer::SEND_EFFECTS_1 << 8)
	                | (TChannel::MixConfig::GAIN_PAN << 4)
	                | TChannel::MixConfig::GAIN_FXMIX;
	mMixConfig[4] = DSPInterface::DSPBuffer::SEND_NONE;
	mMixConfig[5] = DSPInterface::DSPBuffer::SEND_NONE;

	mPriorityConfig = 0x20103;
	mPriorityTime   = 600;

	mCalcTypes[0] = 0xD;
	mCalcTypes[1] = 0xD;
	mCalcTypes[2] = 0xD;
}

void TChannelMgr::stopAll()
{
	TChannel* channel = mPlayingChannels;
	while (channel) {
		TChannel* r31 = channel->mNext;
		channel->stop(0);
		channel = r31;
	}
}

void TChannelMgr::stopAllRelease()
{
	for (TChannel* channel = mReleaseChannels; channel;
	     channel           = channel->mNext)
        for (u32 i = 0; i < 4; ++i)
            channel->forceStopOsc(i);
}

BOOL TChannelMgr::checkGlobalRelease() { return mManagedChannels != 0; }

void TChannelMgr::addListHead(TChannel* channel, u32 list_type)
{
	TChannel** list;
	switch (list_type) {
	case CHAN_LIST_FREE:
		list = &mFreeChannels;
		break;
	case CHAN_LIST_PLAYING:
		list = &mPlayingChannels;
		break;
	case CHAN_LIST_RELEASE:
		list = &mReleaseChannels;
		break;
	case CHAN_LIST_PENDING:
		list = &mPendingChannels;
		break;
	default:
		list = nullptr;
		break;
	}
	TChannel* prevHead   = *list;
	channel->mOwningList = list;
	*list                = channel;
	channel->mNext       = prevHead;
}

void TChannelMgr::addListTail(TChannel* channel, u32 list_type)
{
	TChannel** list;
	switch (list_type) {
	case CHAN_LIST_FREE:
		list = &mFreeChannels;
		break;
	case CHAN_LIST_PLAYING:
		list = &mPlayingChannels;
		break;
	case CHAN_LIST_RELEASE:
		list = &mReleaseChannels;
		break;
	case CHAN_LIST_PENDING:
		list = &mPendingChannels;
		break;
	default:
		list = nullptr;
		break;
	}

	TChannel* head       = *list;
	channel->mOwningList = list;

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

TChannel* TChannelMgr::getListHead(u32 list_type)
{
	TChannel** list;
	switch (list_type) {
	case CHAN_LIST_FREE:
		list = &mFreeChannels;
		break;
	case CHAN_LIST_PLAYING:
		list = &mPlayingChannels;
		break;
	case CHAN_LIST_RELEASE:
		list = &mReleaseChannels;
		break;
	case CHAN_LIST_PENDING:
		list = &mPendingChannels;
		break;
	default:
		list = nullptr;
		break;
	}

	TChannel* head = *list;
	if (!head)
		return nullptr;

	*list             = head->mNext;
	head->mOwningList = nullptr;
	return head;
}

u32 TChannelMgr::countList(u32) const { return 0; }

int TChannelMgr::cutList(TChannel* channel)
{
	int i = 0;

	TChannel* head = *channel->mOwningList;

	if (!head)
		return -1;

	if (head == channel) {
		*channel->mOwningList = channel->mNext;
		channel->mOwningList  = nullptr;
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

	head->mNext          = channel->mNext;
	channel->mOwningList = nullptr;
	return i;
}

void TChannelMgr::initAllocChannel(u32 capacity)
{
	if (checkGlobalRelease() != FALSE)
		ChGlobal::releaseAll(this);

	init();

	ChGlobal::alloc(this, capacity);
	mMaintainPoolSize = capacity == 0 ? FALSE : TRUE;
}

TChannel* TChannelMgr::getLogicalChannel(u32 polyphony_key)
{
	BOOL bVar1 = false;
	if (!checkLimitStart(polyphony_key)) {
		if ((polyphony_key >> 0x18) & 0x10)
			return nullptr;
		bVar1 = true;
	}

	TChannel* result = getListHead(CHAN_LIST_FREE);
	if (result == nullptr) {
		if (!ChGlobal::alloc(this, 1))
			return nullptr;

		++mGlobalChannelBorrows;
		result = getListHead(CHAN_LIST_FREE);

		if (mMaintainPoolSize == TRUE) {
			TChannel* chan = getListHead(CHAN_LIST_RELEASE);
			if (chan == nullptr)
				chan = getListHead(CHAN_LIST_PLAYING);
			if (chan != nullptr) {
				chan->forceStopOsc(0);
				addListHead(chan, CHAN_LIST_PENDING);
				TDSPChannel::forceStop(chan->mDspChannel);
			}
		}
	}

	result->init();
	if (bVar1) {
		result->mPauseFlag        = 1;
		result->mDspUpdatePending = 1;
	}

	result->mPolyphonyKey = polyphony_key;
	result->unk18         = 0;
	result->setPanPower(1.0f, 1.0f, 1.0f, 1.0f);
	return result;
}

int TChannelMgr::receiveAllChannels(TChannelMgr* other)
{
	TChannel* stolen;
	for (;;) {
		stolen = other->getListHead(CHAN_LIST_FREE);
		if (!stolen)
			break;
		addListHead(stolen, CHAN_LIST_FREE);
		stolen->mManager = this;
	}
	for (;;) {
		stolen = other->getListHead(CHAN_LIST_PLAYING);
		if (!stolen)
			break;
		addListHead(stolen, CHAN_LIST_PLAYING);
		stolen->mManager           = this;
		stolen->mPreserveDspConfig = TRUE;
	}
	for (;;) {
		stolen = other->getListHead(CHAN_LIST_RELEASE);
		if (!stolen)
			break;
		addListHead(stolen, CHAN_LIST_RELEASE);
		stolen->mManager           = this;
		stolen->mPreserveDspConfig = TRUE;
	}
	for (;;) {
		stolen = other->getListHead(CHAN_LIST_PENDING);
		if (!stolen)
			break;
		if (Driver::DSPQueue::deleteQueue(stolen))
			addListHead(stolen, CHAN_LIST_FREE);
		else
			addListHead(stolen, CHAN_LIST_PENDING);
		stolen->mManager = this;
	}
	mManagedChannels += other->mManagedChannels;
	other->mManagedChannels = 0;
	mGlobalChannelBorrows += other->mGlobalChannelBorrows;
	other->mGlobalChannelBorrows = 0;

	return 0;
}

namespace Driver {
	const u8 polys_table[16] = { 0x0, 0x1, 0x2, 0x3,  0x4,  0x5,  0x6,  0x8,
		                         0xa, 0xc, 0xe, 0x10, 0x14, 0x18, 0x1c, 0x20 };
} // namespace Driver

// BUG: oooops, all infinite loops!
BOOL TChannelMgr::checkLimitStart(u32 polyphony_key)
{
	u32 thing = Driver::polys_table[(polyphony_key >> 0x18) & 0xF];
	if (!thing)
		return true;

	int i = 0;

	TChannel* head = mReleaseChannels;
	if (((polyphony_key >> 0x18) & 0x20)) {
		for (;;) {
			if (!head)
				break;

			if (head->mPolyphonyKey == polyphony_key && head->mPauseFlag == 0)
				++i;
		}

		head = mPlayingChannels;
		for (;;) {
			if (!head)
				break;

			if (head->mPolyphonyKey == polyphony_key && head->mPauseFlag == 0)
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

			if (head->mPolyphonyKey == polyphony_key && head->mPauseFlag == 0) {
				if (found == nullptr)
					found = head;
				if (i == thing) {
					found->forceStopLogicalChannel();
					return 1;
				}
				++i;
			}
		}

		head = mPlayingChannels;
		for (;;) {
			if (!head)
				break;

			if (head->mPolyphonyKey == polyphony_key && head->mPauseFlag == 0) {
				if (found == nullptr)
					found = head;
				if (i == thing) {
					if ((polyphony_key >> 0x18) & 0x10)
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

void TChannelMgr::checkLimitStop(TChannel* channel, u32 polyphony_key)
{
	TChannel* chan = channel->mManager->mPlayingChannels;
	u32 thing      = Driver::polys_table[(polyphony_key >> 0x18) & 0xF];

	if (polyphony_key == 0)
		return;
	if (thing == 0)
		return;

	if ((polyphony_key >> 0x18) & 0x20) {
		for (;;) {
			if (!chan)
				break;
			if (chan->mPolyphonyKey == polyphony_key && chan->mPauseFlag == 1) {
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
			if (chan->mPolyphonyKey == polyphony_key && chan->mPauseFlag == 1)
				found = chan;
			chan = chan->mNext;
		}

		if (found)
			found->setPauseFlagReq(0);
	}
}

} // namespace JASystem
