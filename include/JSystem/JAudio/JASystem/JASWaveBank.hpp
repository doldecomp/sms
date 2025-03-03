#ifndef JASWAVEBANK_HPP
#define JASWAVEBANK_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JASystem/JASHeapCtrl.hpp>

namespace JASystem {

class TWaveBank {
public:
	virtual ~TWaveBank();

	static void* getCurrentHeap();

	static void* sCurrentHeap;
};

} // namespace JASystem

#endif // JASWAVEBANK_HPP
