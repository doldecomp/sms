#include "JSystem/JAudio/JASystem/JASRegisterParam.hpp"

namespace JASystem {

TRegisterParam::TRegisterParam()
{
	unk0[0] = 0;
	unk0[1] = 0;
	unk0[2] = 0;
	unk0[3] = 0;
	unk0[4] = 0;
	unk0[5] = 0;
	unkC    = 0;
	unkE    = 0;
	unk1A   = 0;
	mPanPower[0] = 0;
	mPanPower[1] = 0;
	mPanPower[2] = 0;
	mPanPower[3] = 0;
	mPanPower[4] = 0;
	unk20   = 0;
	unk24   = 0;
	unk28   = 0;
	unk2C   = 0;
}

void TRegisterParam::init()
{
	unk0[0] = 0;
	unk0[1] = 0;
	unk0[2] = 0;
	unk0[3] = 0;
	unk0[4] = 0;
	unk0[5] = 0;
	unkC    = 0xf0;
	unkE    = 0x0c;
	unk1A   = 0x40;
	mPanPower[0] = 0;
	mPanPower[1] = 1;
	mPanPower[2] = 1;
	mPanPower[3] = 0x7fff;
	mPanPower[4] = 0x4000;
	unk20   = 0;
	unk24   = 0;
	unk28   = 0;
	unk2C   = 0;
}

void TRegisterParam::inherit(const TRegisterParam& other)
{
	unk0[0] = 0;
	unk0[1] = 0;
	unk0[2] = 0;
	unk0[3] = 0;
	unk0[4] = 0;
	unk0[5] = 0;
	unkC    = other.unkC;
	unkE    = other.unkE;
	unk1A   = other.unk1A;

	for (int i = 0; i < 5; i++)
		mPanPower[i] = other.mPanPower[i];

	unk20 = 0;
	unk24 = 0;
	unk28 = 0;
	unk2C = 0;
}

void TRegisterParam::setBankNumber(u8 bankNum) { }

u8 TRegisterParam::getBankNumber() const { return unkC >> 8; }

u8 TRegisterParam::getProgramNumber() const { return unkC; }

} // namespace JASystem
