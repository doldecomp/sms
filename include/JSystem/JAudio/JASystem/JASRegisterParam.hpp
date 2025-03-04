#ifndef JASREGISTERPARAM_HPP
#define JASREGISTERPARAM_HPP

#include <dolphin/types.h>

namespace JASystem {
class TRegisterParam {
public:
	TRegisterParam();

	void init();
	void inherit(const TRegisterParam& other);
	void setBankNumber(u8 bankNum);
	u8 getBankNumber() const;
	u8 getProgramNumber() const;

private:
	// Add appropriate member variables based on the functions
	u8 mBankNumber;
	u8 mProgramNumber;
};
} // namespace JASystem

#endif // JASREGISTERPARAM_HPP
