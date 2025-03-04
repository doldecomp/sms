#ifndef JASBASICBANK_HPP
#define JASBASICBANK_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JASystem/JASBank.hpp>

namespace JASystem {

class TInst;

class TBasicBank : public TBank {
public:
	TBasicBank();
	virtual ~TBasicBank();

	virtual int getType() const;
	virtual TInst* getInst(int) const;

	void setInstCount(u32);
	void setInst(int, TInst*);

private:
	u32 mInstCount;
	TInst** mInsts;
};

} // namespace JASystem

#endif // JASBASICBANK_HPP
