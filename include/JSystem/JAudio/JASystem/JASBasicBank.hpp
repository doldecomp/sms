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
	virtual int getType() const;

	void setInstCount(u32);
	void setInst(int, TInst*);

private:
	u32 mInstCount;
	TInst** mInsts;
};

} // namespace JASystem

#endif // JASBASICBANK_HPP
