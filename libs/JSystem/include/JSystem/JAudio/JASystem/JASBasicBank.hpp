#ifndef JASBASICBANK_HPP
#define JASBASICBANK_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JASystem/JASBank.hpp>

namespace JASystem {

class TBasicBank : public TBank {
public:
	TBasicBank();

	virtual ~TBasicBank();
	virtual TInst* getInst(int) const;
	virtual int getType() const { return 'BSIC'; }

	void setInstCount(u32);
	void setInst(int, TInst*);

private:
	/* 0x8 */ TInst** mInsts;
	/* 0xC */ u32 mInstCount;
};

} // namespace JASystem

#endif // JASBASICBANK_HPP
