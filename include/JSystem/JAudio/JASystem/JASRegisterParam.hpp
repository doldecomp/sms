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
	u16 getPanPower(int i) const { return mPanPower[i]; }

	u16 getPanPowerBank() const { return mPanPower[0]; }
	u16 getPanPowerExt() const { return mPanPower[1]; }
	u16 getPanPowerOsc() const { return mPanPower[2]; }
	u16 getPanPowerParent() const { return mPanPower[3]; }
	u16 getPanPowerTrack() const { return mPanPower[4]; }

	u16 getFlag() const { return unk0[3]; }
	void setFlag(u16 flag) { unk0[3] = flag; }

	int getAddress(int i) const { return unk20[i]; }
	void setAddress(int i, u32 value) { unk20[i] = value; }

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
