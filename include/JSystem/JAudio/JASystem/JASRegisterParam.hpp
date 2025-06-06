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

	void setPanPower(int i, u16 power) { mPanPower[i] = power; }

public:
	/* 0x00 */ u16 unk0[6];
	/* 0x0C */ u16 unkC;
	/* 0x0E */ u16 unkE;
	/* 0x10 */ u16 mPanPower[5];
	/* 0x1A */ u16 unk1A;
	/* 0x1C */ int unk1C;
	/* 0x20 */ int unk20[4];
};

} // namespace JASystem

#endif // JASREGISTERPARAM_HPP
