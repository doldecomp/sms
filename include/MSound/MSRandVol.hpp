#ifndef MSRANDVOL_HPP
#define MSRANDVOL_HPP

#include <dolphin/types.h>
#include <JSystem/JSupport/JSUList.hpp>

namespace MSoundSESystem {

class MSRandVol {
public:
	MSRandVol(u32);

	virtual u32 getRandVol(u32);

	static void construct(u32);
	u32 getRandomVolume(u32, u32);
	static JSUList<MSRandVol> smList;
	static float getRandomVolumeNormal(u32);

public:
	/* 0x4 */ JSULink<MSRandVol> unk4;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ f32 unk48;
};

} // namespace MSoundSESystem
#endif // MSRANDVOL_HPP
