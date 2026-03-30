#ifndef MSRANDVOL_HPP
#define MSRANDVOL_HPP

#include <dolphin/types.h>
#include <JSystem/JSupport/JSUList.hpp>

namespace MSoundSESystem {

class MSRandVol {
public:
	MSRandVol(u32);

	virtual f32 getRandVol(u32);

	static void construct(u32);
	u32 getRandomVolume(u32, u32);
	static JSUList<MSRandVol> smList;
	static f32 getRandomVolumeNormal(u32 param_1)
	{
		JSULink<MSRandVol>* link = smList.getFirst();
		if (!link)
			return 1.0f;

		MSRandVol* ptr = link->getObject();
		return ptr->getRandVol(param_1);
	}

public:
	/* 0x4 */ JSULink<MSRandVol> unk4;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C[4];
	/* 0x2C */ f32 unk2C[4];
	/* 0x3C */ f32 unk3C[4];
};

} // namespace MSoundSESystem
#endif // MSRANDVOL_HPP
