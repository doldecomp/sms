#ifndef JASINST_HPP
#define JASINST_HPP

#include <JSystem/JAudio/JASystem/JASOscillator.hpp>
#include <types.h>

namespace JASystem {

struct TVeloRegion {
	/* 0x00 */ s32 unk0;
	/* 0x04 */ s32 unk4;
	/* 0x08 */ f32 unk8;
	/* 0x0C */ f32 unkC;
};

struct TInstParam {
	TInstParam()
	{
		unk0      = 0;
		unk4      = 0;
		mOscData  = nullptr;
		mOscCount = 0;
		unk10     = 1.0f;
		unk14     = 1.0f;
		unk18     = 1.0f;
		unk1C     = 1.0f;
		unk20     = 0.5f;
		unk24     = 0.0f;
		unk28     = 0.0f;
		unk2C     = 0.5f;
		unk30     = 0.0f;
		unk34     = 0.0f;
		unk38     = 0;
		unk3A     = 0;
		unk3C     = 0;
		unk40     = 0;
	}

	/* 0x00 */ u8 unk0;
	/* 0x04 */ int unk4;
	/* 0x08 */ TOscillator::Osc_** mOscData;
	/* 0x0C */ u32 mOscCount;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ u8 unk38;
	/* 0x3A */ u16 unk3A;
	/* 0x3C */ int unk3C;
	/* 0x40 */ int unk40;
};

class TInst {
public:
	virtual ~TInst() { }

	virtual bool getParam(int, int, TInstParam*) const = 0;
	virtual int getType() const                        = 0;
};

} // namespace JASystem

#endif
