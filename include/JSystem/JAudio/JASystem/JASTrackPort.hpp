#ifndef JASTRACKPORT_HPP
#define JASTRACKPORT_HPP

#include <dolphin/types.h>

namespace JASystem {

class TTrackPort {
public:
	void init();
	void readWriteCheck(u32, u8);
};

} // namespace JASystem

#endif // JASTRACKPORT_HPP
