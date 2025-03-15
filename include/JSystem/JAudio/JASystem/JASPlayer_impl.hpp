#ifndef JASPLAYER_IMPL_HPP
#define JASPLAYER_IMPL_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JASystem/JASOscillator.hpp>

namespace JASystem {

struct IIRCoefficients {
	s16 mCoeff1;
	s16 mCoeff2;
	s16 mCoeff3;
	s16 mCoeff4;
};

namespace Player {

	extern TOscillator::Osc_ sEnvelopeDef;
	extern s16 sRelTable[];
	extern TOscillator::Osc_ sVibratoDef;
	extern s16 sVibTable[];
	extern TOscillator::Osc_ sTremoroDef;
	extern s16 sTreTable[];
	extern TOscillator::Osc_ sAdsrDef;
	extern s16 sAdsTable[];

	extern IIRCoefficients CUTOFF_TO_IIR_TABLE[];

	f32 pitchToCent(f32, f32);
	u16 extend8to16(u8);
	int getRandomS32();
} // namespace Player

} // namespace JASystem

#endif // JASPLAYER_IMPL_HPP
