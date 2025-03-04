#ifndef JASBANK_HPP
#define JASBANK_HPP

#include <JSystem/JKernel/JKRHeap.hpp>

namespace JASystem {

class TInst;

class TBank {
public:
	virtual ~TBank() { }
	virtual TInst* getInst(int) const = 0;
	virtual int getType() const       = 0;

	static JKRHeap* getCurrentHeap();

private:
	static JKRHeap* sCurrentHeap;
};

} // namespace JASystem

#endif // JASBANK_HPP
