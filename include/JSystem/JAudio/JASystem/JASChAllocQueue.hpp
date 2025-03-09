#ifndef JASCHALLOCQUEUE_HPP
#define JASCHALLOCQUEUE_HPP

#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <types.h>

namespace JASystem {

namespace Driver {

	namespace DSPQueue {
		void init(u32 maxWait);
		BOOL enQueue(JASystem::TChannel* channel);
		void deQueue(u8 channelIndex);
		bool deleteQueue(JASystem::TChannel* channel);
		void checkQueue();
	}; // namespace DSPQueue

} // namespace Driver

} // namespace JASystem

#endif // JASCHALLOCQUEUE_HPP
