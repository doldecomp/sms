#include <JSystem/JAudio/JASystem/JASBank.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>

namespace JASystem {

JKRHeap* TBank::sCurrentHeap = nullptr;

JKRHeap* TBank::getCurrentHeap()
{
	if (sCurrentHeap)
		return sCurrentHeap;

	return JASDram;
}

} // namespace JASystem
