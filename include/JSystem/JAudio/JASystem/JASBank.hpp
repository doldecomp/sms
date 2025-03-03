#ifndef JASBANK_HPP
#define JASBANK_HPP

#include <JSystem/JKernel/JKRHeap.hpp>

namespace JASystem {

class TBank {
public:
	static JKRHeap* getCurrentHeap();

private:
	static JKRHeap* sCurrentHeap;
};

} // namespace JASystem

#endif // JASBANK_HPP
