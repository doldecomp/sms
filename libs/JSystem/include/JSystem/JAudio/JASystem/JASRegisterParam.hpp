#ifndef JASREGISTERPARAM_HPP
#define JASREGISTERPARAM_HPP

#include <dolphin/types.h>
#include <JSystem/JUtility/JUTAssert.hpp>

namespace JASystem {

class TRegisterParam {
public:
	TRegisterParam();

	void init();
	void inherit(const TRegisterParam& other);
	void setBankNumber(u8 bankNum);
	u8 getBankNumber() const;
	u8 getProgramNumber() const;

	void setPanPower(int i, u16 power)
	{
		JUT_ASSERT(i >= 0);
		JUT_ASSERT(i < 5);
		mRegisters[8 + i] = power;
	}
	u16 getPanPower(int i) const { return mRegisters[8 + i]; }

	u16 getPanPowerBank() const { return mRegisters[8]; }
	u16 getPanPowerExt() const { return mRegisters[9]; }
	u16 getPanPowerOsc() const { return mRegisters[10]; }
	u16 getPanPowerParent() const { return mRegisters[11]; }
	u16 getPanPowerTrack() const { return mRegisters[12]; }
	u16 getBendSense() const { return mRegisters[7]; }
	u16 getPriority() const { return mRegisters[13]; }

	u16 getFlag() const { return mRegisters[3]; }
	void setFlag(u16 flag) { mRegisters[3] = flag; }

	int getAddress(int i) const
	{
		JUT_ASSERT(i >= 0);
		JUT_ASSERT(i < 4);
		return unk20[i];
	}
	void setAddress(int i, u32 value)
	{
		JUT_ASSERT(i >= 0);
		JUT_ASSERT(i < 4);
		unk20[i] = value;
	}

public:
	/* 0x00 */ u16 mRegisters[14];
	/* 0x1C */ int unk1C;
	/* 0x20 */ int unk20[4];
};

} // namespace JASystem

#endif // JASREGISTERPARAM_HPP
