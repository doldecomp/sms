#ifndef JASBASICBANK_HPP
#define JASBASICBANK_HPP

#include <JSystem/JAudio/JASystem/JASBank.hpp>

namespace JASystem {

class TInst;

class TBasicBank : public TBank {
public:
	TBasicBank();
	virtual ~TBasicBank();

	virtual int getType() const;
	virtual TInst* getInst(int) const;

	void setInstCount(unsigned long);
	void setInst(int, TInst*);

private:
	unsigned long mInstCount;
	TInst** mInsts;
};

} // namespace JASystem

#endif // JASBASICBANK_HPP
