#ifndef JASWAVEBANK_HPP
#define JASWAVEBANK_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JASystem/JASHeapCtrl.hpp>

namespace JASystem {

class TWaveInfo;

class TWaveHandle {
public:
	virtual ~TWaveHandle();
	virtual const TWaveInfo* getWaveInfo() const = 0;
	virtual const void* getWavePtr() const       = 0;
};

class TWaveBank {
public:
	virtual ~TWaveBank() { }
	virtual TWaveHandle* getWaveHandle(u32) const = 0;
	virtual int getType() const                   = 0;

	void* getCurrentHeap();

	static void* sCurrentHeap;
};

} // namespace JASystem

#endif // JASWAVEBANK_HPP
