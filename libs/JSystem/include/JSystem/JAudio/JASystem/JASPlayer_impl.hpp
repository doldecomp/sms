#ifndef JASPLAYER_IMPL_HPP
#define JASPLAYER_IMPL_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JASystem/JASOscillator.hpp>

namespace JASystem {

namespace Player {

	extern TOscillator::Osc_ sEnvelopeDef;
	extern s16 sRelTable[];
	extern TOscillator::Osc_ sVibratoDef;
	extern s16 sVibTable[];
	extern TOscillator::Osc_ sTremoroDef;
	extern s16 sTreTable[];
	extern TOscillator::Osc_ sAdsrDef;
	extern s16 sAdsTable[];

	extern s16 CUTOFF_TO_IIR_TABLE[][4];

	f32 pitchToCent(f32, f32);
	u16 extend8to16(u8);
	int getRandomS32();
} // namespace Player

} // namespace JASystem

#endif // JASPLAYER_IMPL_HPP
