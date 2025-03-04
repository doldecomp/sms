#ifndef JASTRACKINTERRUPT_HPP
#define JASTRACKINTERRUPT_HPP

#include <dolphin/types.h>

namespace JASystem {

class TIntrMgr {
public:
	void init();
	u32 request(u32);
	void setIntr(u32, void*);
	void resetInter(u32);
	void checkIntr();
	void timerProcess();
};

} // namespace JASystem

#endif // JASTRACKINTERRUPT_HPP
