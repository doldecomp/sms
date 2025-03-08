#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>

namespace JASystem {

JKRHeap* TWaveBank::sCurrentHeap = 0;

JKRHeap* TWaveBank::getCurrentHeap()
{
	if (sCurrentHeap)
		return sCurrentHeap;
	return JASDram;
}

} // namespace JASystem
