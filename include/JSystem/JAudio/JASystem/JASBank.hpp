#ifndef JASBANK_HPP
#define JASBANK_HPP

#include <JSystem/JKernel/JKRHeap.hpp>

namespace JASystem {

class TInst;
class TWaveBank;

class TBank {
public:
	TBank()
	    : mWaveBank(nullptr)
	{
	}

	virtual ~TBank() { }
	virtual TInst* getInst(int) const = 0;
	virtual int getType() const       = 0;

	static JKRHeap* getCurrentHeap();

	TWaveBank* getWaveBank() const { return mWaveBank; }
	void assignWaveBank(TWaveBank* bank) { mWaveBank = bank; }

private:
	static JKRHeap* sCurrentHeap;

public:
	/* 0x4 */ TWaveBank* mWaveBank;
};

} // namespace JASystem

#endif // JASBANK_HPP
