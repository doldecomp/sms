#ifndef JASWAVEBANK_HPP
#define JASWAVEBANK_HPP

#include "types.h"

namespace JASystem {

class TWaveBank {
public:
	static void* getCurrentHeap();

	static void* sCurrentHeap;
};

} // namespace JASystem

#endif // JASWAVEBANK_HPP
