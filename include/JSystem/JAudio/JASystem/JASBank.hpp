#ifndef JASBANK_HPP
#define JASBANK_HPP

#include <JSystem/JKernel/JKRHeap.hpp>

namespace JASystem {

class TBank {
public:
	virtual ~TBank();
	static JKRHeap* getCurrentHeap();

private:
	static JKRHeap* sCurrentHeap;
};

} // namespace JASystem

#endif // JASBANK_HPP
