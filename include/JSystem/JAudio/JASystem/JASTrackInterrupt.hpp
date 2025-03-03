#ifndef JASTRACKINTERRUPT_HPP
#define JASTRACKINTERRUPT_HPP

#include "types.h"

namespace JASystem {

class TIntrMgr {
public:
	static void init();
	static unsigned long request(unsigned long);
	static void setIntr(unsigned long, void*);
	static void resetInter(unsigned long);
	static void checkIntr();
	static void timerProcess();
};

} // namespace JASystem

#endif // JASTRACKINTERRUPT_HPP
