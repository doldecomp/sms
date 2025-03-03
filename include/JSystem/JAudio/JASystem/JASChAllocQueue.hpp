#ifndef JASCHALLOCQUEUE_HPP
#define JASCHALLOCQUEUE_HPP

#include <JSystem/JAudio/JASystem/JASChannel.hpp>

namespace JASystem {

namespace Driver {

	class DSPQueue {
	public:
		static void init(unsigned long);
		static void enQueue(JASystem::TChannel*);
		static void deQueue(unsigned char);
		static void deleteQueue(JASystem::TChannel*);
		static void checkQueue();

		static unsigned long waitMax;
		static unsigned long waitp;
		static unsigned long waittime;
		static unsigned long cur_waits;
		static unsigned long cur_top;
		static unsigned long cur_tail;
	};

} // namespace Driver

} // namespace JASystem

#endif // JASCHALLOCQUEUE_HPP
