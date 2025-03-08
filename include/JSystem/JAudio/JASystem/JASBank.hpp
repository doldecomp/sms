#ifndef JASBANK_HPP
#define JASBANK_HPP

#include <JSystem/JKernel/JKRHeap.hpp>

namespace JASystem {

class TInst;
class TWaveBank;

class TBank {
public:
	TBank()
	    : unk4(nullptr)
	{
	}

	virtual ~TBank() { }
	virtual TInst* getInst(int) const = 0;
	virtual int getType() const       = 0;

	static JKRHeap* getCurrentHeap();

private:
	static JKRHeap* sCurrentHeap;

public:
	/* 0x4 */ TWaveBank* unk4;
};

} // namespace JASystem

#endif // JASBANK_HPP
