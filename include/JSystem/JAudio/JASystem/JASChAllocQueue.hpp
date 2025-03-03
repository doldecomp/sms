#ifndef JASCHALLOCQUEUE_HPP
#define JASCHALLOCQUEUE_HPP

#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <types.h>

namespace JASystem {

namespace Driver {

	class DSPQueue {
	public:
		static void init(u32 maxWait);
		static void enQueue(JASystem::TChannel* channel);
		static void deQueue(u8 channelIndex);
		static void deleteQueue(JASystem::TChannel* channel);
		static void checkQueue();

		static u32 waitMax;
		static u32 waitp;
		static u32 waittime;
		static u32 cur_waits;
		static u32 cur_top;
		static u32 cur_tail;
	};

} // namespace Driver

} // namespace JASystem

#endif // JASCHALLOCQUEUE_HPP
