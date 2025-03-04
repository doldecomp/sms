#ifndef JASCHALLOCQUEUE_HPP
#define JASCHALLOCQUEUE_HPP

#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <types.h>

namespace JASystem {

namespace Driver {

	class DSPQueue {
	public:
		void init(u32 maxWait);
		void enQueue(JASystem::TChannel* channel);
		void deQueue(u8 channelIndex);
		void deleteQueue(JASystem::TChannel* channel);
		void checkQueue();

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
