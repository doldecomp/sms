#ifndef JASREGISTERPARAM_HPP
#define JASREGISTERPARAM_HPP

#include <dolphin/types.h>
#include <JSystem/JUtility/JUTAssert.hpp>

namespace JASystem {

class TRegisterParam {
public:
	enum PanPowerSource {
		PAN_POWER_BANK   = 0,
		PAN_POWER_OSC    = 1,
		PAN_POWER_TRACK  = 2,
		PAN_POWER_EXT    = 3,
		PAN_POWER_PARENT = 4,
	};

	enum RegisterId {
		REG_U8_0              = 0x00,
		REG_U8_1              = 0x01,
		REG_U8_2              = 0x02,
		REG_FLAG              = 0x03,
		REG_PRODUCT_HI        = 0x04,
		REG_PRODUCT_LO        = 0x05,
		REG_BANK_PROGRAM      = 0x06,
		REG_BEND_SENSE        = 0x07,
		REG_PAN_POWER_BANK    = 0x08,
		REG_PAN_POWER_OSC     = 0x09,
		REG_PAN_POWER_TRACK   = 0x0A,
		REG_PAN_POWER_EXT     = 0x0B,
		REG_PAN_POWER_PARENT  = 0x0C,
		REG_PRIORITY          = 0x0D,
		REG_BANK              = 0x20,
		REG_PROGRAM           = 0x21,
		REG_U8_PAIR           = 0x22,
		REG_PRODUCT           = 0x23,
		REG_ADDRESS0          = 0x28,
		REG_ADDRESS1          = 0x29,
		REG_ADDRESS2          = 0x2A,
		REG_ADDRESS3          = 0x2B,
		REG_ACTIVE_CHILD_MASK = 0x2C,
		REG_NOTE_STOP_MASK    = 0x2D,
		REG_NOTE_ON_PRIORITY  = 0x2E,
		REG_RELEASE_PRIORITY  = 0x2F,
		REG_LOOP_COUNT        = 0x30,
		REG_PORT_BASE         = 0x40,
	};

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
		mRegisters[REG_PAN_POWER_BANK + i] = power;
	}
	u16 getPanPower(int i) const { return mRegisters[REG_PAN_POWER_BANK + i]; }

	u16 getPanPowerBank() const { return mRegisters[REG_PAN_POWER_BANK]; }
	u16 getPanPowerOsc() const { return mRegisters[REG_PAN_POWER_OSC]; }
	u16 getPanPowerTrack() const { return mRegisters[REG_PAN_POWER_TRACK]; }
	u16 getPanPowerExt() const { return mRegisters[REG_PAN_POWER_EXT]; }
	u16 getPanPowerParent() const { return mRegisters[REG_PAN_POWER_PARENT]; }
	u16 getBendSense() const { return mRegisters[REG_BEND_SENSE]; }
	u16 getPriority() const { return mRegisters[REG_PRIORITY]; }

	u16 getFlag() const { return mRegisters[REG_FLAG]; }
	void setFlag(u16 flag) { mRegisters[REG_FLAG] = flag; }

	int getAddress(int i) const
	{
		JUT_ASSERT(i >= 0);
		JUT_ASSERT(i < 4);
		return mAddresses[i];
	}
	void setAddress(int i, u32 value)
	{
		JUT_ASSERT(i >= 0);
		JUT_ASSERT(i < 4);
		mAddresses[i] = value;
	}

public:
	/* 0x00 */ u16 mRegisters[14];
	/* 0x1C */ int unk1C;
	/* 0x20 */ int mAddresses[4];
};

} // namespace JASystem

#endif // JASREGISTERPARAM_HPP
