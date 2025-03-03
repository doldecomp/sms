#ifndef JASTRACKPORT_HPP
#define JASTRACKPORT_HPP

#include "types.h"

namespace JASystem {

class TTrackPort {
public:
	static void init();
	static void readWriteCheck(unsigned long, unsigned char);
};

} // namespace JASystem

#endif // JASTRACKPORT_HPP
