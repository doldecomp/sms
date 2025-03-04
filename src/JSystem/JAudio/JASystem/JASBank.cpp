#include <JSystem/JAudio/JASystem/JASBank.hpp>

namespace JASystem {

JKRHeap* TBank::sCurrentHeap = nullptr;

JKRHeap* TBank::getCurrentHeap() { return sCurrentHeap; }

} // namespace JASystem
