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
	/* 0x4 */ JSULink<MSRandVol> mLink;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ f32 mAmplitude;
	/* 0x1C */ f32 mPSlopes[4];
	/* 0x2C */ f32 mCSlopes[4];
	/* 0x3C */ f32 mAmplitudes[4];
};

} // namespace MSoundSESystem
#endif // MSRANDVOL_HPP
