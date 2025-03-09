#include <JSystem/JAudio/JASystem/JASChAllocQueue.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASDSPChannel.hpp>
#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

namespace JASystem {

namespace Driver {

	namespace DSPQueue {
		static u32 waitMax      = 0x20;
		static TChannel** waitp = 0;
		static u32* waittime    = 0;
		static u32 cur_waits    = 0;
		static u32 cur_top      = 0;
		static u32 cur_tail     = 0;
	} // namespace DSPQueue

	void DSPQueue::init(u32 maxWait)
	{
		waitMax  = maxWait;
		waitp    = new (JASDram, 0) TChannel*[waitMax];
		waittime = new (JASDram, 0) u32[waitMax];
		Calc::bzero(waitp, waitMax * sizeof(TChannel*));
		Calc::bzero(waittime, waitMax * sizeof(u32));
	}

	BOOL DSPQueue::enQueue(JASystem::TChannel* channel)
	{
		if (cur_waits == waitMax)
			return false;

		if (!TDSPChannel::breakLower(channel->unkC0))
			return false;

		waittime[cur_tail] = 0;
		waitp[cur_tail]    = channel;
		++cur_tail;
		++cur_waits;
		if (cur_tail == waitMax)
			cur_tail = 0;

		return true;
	}

	void DSPQueue::deQueue(u8 channelIndex)
	{
		while (cur_waits) {
			TChannel* chan;
			chan = waitp[cur_top];
			if (chan) {
				TDSPChannel* dspChan = TDSPChannel::alloc(0, (u32)chan);
				if (!dspChan)
					return;
				chan->unk20 = dspChan;
				chan->playLogicalChannel();
				if (chan->unk4->cutList(chan) != -1) {
					chan->unk4->addListTail(chan, 1);
				}

				++cur_top;
				if (cur_top == waitMax)
					cur_top = 0;
				--cur_waits;

				if (channelIndex == 1)
					break;
			} else {
				++cur_top;
				if (cur_top == waitMax)
					cur_top = 0;
				--cur_waits;
			}
		}
	}

	bool DSPQueue::deleteQueue(JASystem::TChannel* channel)
	{
		for (int i = 0; i < cur_waits; ++i) {
			if (waitp[(cur_top + i) % waitMax] == channel) {
				waitp[(cur_top + i) % waitMax] = nullptr;
				return true;
			}
		}
		return false;
	}

	void DSPQueue::checkQueue()
	{
		deQueue(0);
		for (int i = 0; i < cur_waits; ++i) {
			TChannel* chan = waitp[(cur_top + i) % waitMax];
			if (chan != nullptr) {
				++waittime[(cur_top + i) % waitMax];
				if (chan->unk30 > 0)
					--chan->unk30;
				if (chan->unk30 == 0) {
					chan->unk28(chan, 6);
					waitp[(cur_top + i) % waitMax] = nullptr;
				}
			}
		}
	}

} // namespace Driver

} // namespace JASystem
