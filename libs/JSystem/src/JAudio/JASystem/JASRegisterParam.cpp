#include <JSystem/JAudio/JASystem/JASRegisterParam.hpp>

namespace JASystem {

TRegisterParam::TRegisterParam()
{
	mRegisters[0]  = 0;
	mRegisters[1]  = 0;
	mRegisters[2]  = 0;
	mRegisters[3]  = 0;
	mRegisters[4]  = 0;
	mRegisters[5]  = 0;
	mRegisters[6]  = 0;
	mRegisters[7]  = 0;
	mRegisters[13] = 0;
	mRegisters[8]  = 0;
	mRegisters[9]  = 0;
	mRegisters[10] = 0;
	mRegisters[11] = 0;
	mRegisters[12] = 0;

	for (int i = 0; i < 4; ++i)
		unk20[i] = 0;
}

void TRegisterParam::init()
{
	mRegisters[0]  = 0;
	mRegisters[1]  = 0;
	mRegisters[2]  = 0;
	mRegisters[3]  = 0;
	mRegisters[4]  = 0;
	mRegisters[5]  = 0;
	mRegisters[6]  = 0xf0;
	mRegisters[7]  = 0x0c;
	mRegisters[13] = 0x40;
	mRegisters[8]  = 0;
	mRegisters[9]  = 1;
	mRegisters[10] = 1;
	mRegisters[11] = 0x7fff;
	mRegisters[12] = 0x4000;

	for (int i = 0; i < 4; ++i)
		unk20[i] = 0;
}

void TRegisterParam::inherit(const TRegisterParam& other)
{
	mRegisters[0]  = 0;
	mRegisters[1]  = 0;
	mRegisters[2]  = 0;
	mRegisters[3]  = 0;
	mRegisters[4]  = 0;
	mRegisters[5]  = 0;
	mRegisters[6]  = other.mRegisters[6];
	mRegisters[7]  = other.mRegisters[7];
	mRegisters[13] = other.mRegisters[13];

	for (int i = 0; i < 5; ++i)
		mRegisters[8 + i] = other.mRegisters[8 + i];

	for (int i = 0; i < 4; ++i)
		unk20[i] = 0;
}

void TRegisterParam::setBankNumber(u8 bankNum) { }

u8 TRegisterParam::getBankNumber() const { return mRegisters[6] >> 8; }

u8 TRegisterParam::getProgramNumber() const { return mRegisters[6]; }

} // namespace JASystem
