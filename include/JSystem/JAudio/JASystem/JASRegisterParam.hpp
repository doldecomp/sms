#ifndef JASREGISTERPARAM_HPP
#define JASREGISTERPARAM_HPP

#include "types.h"

namespace JASystem {
class TRegisterParam {
public:
	TRegisterParam();

	void init();
	void inherit(const TRegisterParam& other);
	void setBankNumber(unsigned char bankNum);
	unsigned char getBankNumber() const;
	unsigned char getProgramNumber() const;

private:
	// Add appropriate member variables based on the functions
	unsigned char mBankNumber;
	unsigned char mProgramNumber;
};
} // namespace JASystem

#endif // JASREGISTERPARAM_HPP
