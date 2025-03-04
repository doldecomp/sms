#ifndef JASTRACKINTERRUPT_HPP
#define JASTRACKINTERRUPT_HPP

#include <dolphin/types.h>

namespace JASystem {

class TIntrMgr {
public:
	static void init();
	static u32 request(u32);
	static void setIntr(u32, void*);
	static void resetInter(u32);
	static void checkIntr();
	static void timerProcess();
};

} // namespace JASystem

#endif // JASTRACKINTERRUPT_HPP
