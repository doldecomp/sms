#ifndef MSRANDVOL_HPP
#define MSRANDVOL_HPP

#include <dolphin/types.h>
#include <JSystem/JSupport/JSUList.hpp>

namespace MSoundSESystem {

class MSRandVol {
public:
	MSRandVol(u32);
	void construct(u32);
	u32 getRandVol(u32);
	u32 getRandomVolume(u32, u32);
	static JSUList<MSRandVol> smList;
	static float getRandomVolumeNormal(u32);
};

} // namespace MSoundSESystem
#endif // MSRANDVOL_HPP
