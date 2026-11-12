#include <JSystem/JAudio/JASystem/JASRegisterParam.hpp>

namespace JASystem {

TRegisterParam::TRegisterParam()
{
	mRegisters[REG_U8_0]             = 0;
	mRegisters[REG_U8_1]             = 0;
	mRegisters[REG_U8_2]             = 0;
	mRegisters[REG_FLAG]             = 0;
	mRegisters[REG_PRODUCT_HI]       = 0;
	mRegisters[REG_PRODUCT_LO]       = 0;
	mRegisters[REG_BANK_PROGRAM]     = 0;
	mRegisters[REG_BEND_SENSE]       = 0;
	mRegisters[REG_PRIORITY]         = 0;
	mRegisters[REG_PAN_POWER_BANK]   = 0;
	mRegisters[REG_PAN_POWER_OSC]    = 0;
	mRegisters[REG_PAN_POWER_TRACK]  = 0;
	mRegisters[REG_PAN_POWER_EXT]    = 0;
	mRegisters[REG_PAN_POWER_PARENT] = 0;

	for (int i = 0; i < 4; ++i)
		unk20[i] = 0;
}

void TRegisterParam::init()
{
	mRegisters[REG_U8_0]             = 0;
	mRegisters[REG_U8_1]             = 0;
	mRegisters[REG_U8_2]             = 0;
	mRegisters[REG_FLAG]             = 0;
	mRegisters[REG_PRODUCT_HI]       = 0;
	mRegisters[REG_PRODUCT_LO]       = 0;
	mRegisters[REG_BANK_PROGRAM]     = 0xf0;
	mRegisters[REG_BEND_SENSE]       = 0x0c;
	mRegisters[REG_PRIORITY]         = 0x40;
	mRegisters[REG_PAN_POWER_BANK]   = 0;
	mRegisters[REG_PAN_POWER_OSC]    = 1;
	mRegisters[REG_PAN_POWER_TRACK]  = 1;
	mRegisters[REG_PAN_POWER_EXT]    = 0x7fff;
	mRegisters[REG_PAN_POWER_PARENT] = 0x4000;

	for (int i = 0; i < 4; ++i)
		unk20[i] = 0;
}

void TRegisterParam::inherit(const TRegisterParam& other)
{
	mRegisters[REG_U8_0]         = 0;
	mRegisters[REG_U8_1]         = 0;
	mRegisters[REG_U8_2]         = 0;
	mRegisters[REG_FLAG]         = 0;
	mRegisters[REG_PRODUCT_HI]   = 0;
	mRegisters[REG_PRODUCT_LO]   = 0;
	mRegisters[REG_BANK_PROGRAM] = other.mRegisters[REG_BANK_PROGRAM];
	mRegisters[REG_BEND_SENSE]   = other.mRegisters[REG_BEND_SENSE];
	mRegisters[REG_PRIORITY]     = other.mRegisters[REG_PRIORITY];

	for (int i = 0; i < 5; ++i)
		mRegisters[REG_PAN_POWER_BANK + i]
		    = other.mRegisters[REG_PAN_POWER_BANK + i];

	for (int i = 0; i < 4; ++i)
		unk20[i] = 0;
}

void TRegisterParam::setBankNumber(u8 bankNum) { }

u8 TRegisterParam::getBankNumber() const
{
	return mRegisters[REG_BANK_PROGRAM] >> 8;
}

u8 TRegisterParam::getProgramNumber() const
{
	return mRegisters[REG_BANK_PROGRAM];
}

} // namespace JASystem
