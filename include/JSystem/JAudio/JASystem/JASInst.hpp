#ifndef JASINST_HPP
#define JASINST_HPP

#include <JSystem/JAudio/JASystem/JASOscillator.hpp>

namespace JASystem {

struct TVeloRegion {
	/* 0x00 */ s32 unk0;
	/* 0x04 */ s32 unk4;
	/* 0x08 */ f32 unk8;
	/* 0x0C */ f32 unkC;
};

struct TInstParam {
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
