#include <JSystem/JAudio/JASystem/JASChAllocQueue.hpp>

namespace JASystem {

namespace Driver {

	u32 DSPQueue::waitMax   = 0x20;
	u32 DSPQueue::waitp     = 0;
	u32 DSPQueue::waittime  = 0;
	u32 DSPQueue::cur_waits = 0;
	u32 DSPQueue::cur_top   = 0;
	u32 DSPQueue::cur_tail  = 0;

	void DSPQueue::init(u32 maxWait) { }

	void DSPQueue::enQueue(JASystem::TChannel* channel) { }

	void DSPQueue::deQueue(u8 channelIndex) { }

	void DSPQueue::deleteQueue(JASystem::TChannel* channel) { }

	void DSPQueue::checkQueue() { }

} // namespace Driver

} // namespace JASystem
