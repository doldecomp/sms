#ifndef JASPLAYER_IMPL_HPP
#define JASPLAYER_IMPL_HPP

#include <dolphin/types.h>

namespace JASystem {

struct IIRCoefficients {
	s16 mCoeff1;
	s16 mCoeff2;
	s16 mCoeff3;
	s16 mCoeff4;
};

namespace Player {
	// These are most likely all wrong to some degree.
	extern f32 sEnvelopeDef[];
	extern u16 sRelTable[];
	extern u32 sVibratoDef[];
	extern u32 sVibTable[];
	extern f32 s_key_table[];
	extern u32 sTremoroDef[];
	extern u32 sTreTable[];
	extern f32 sAdsrDef[];
	extern u16 sAdsTable[];
	extern IIRCoefficients CUTOFF_TO_IIR_TABLE[];
	extern u32 sV0;
	extern u32 sV1;

	f32 pitchToCent(f32, f32);
	u16 extend8to16(u8);
	int getRandomS32();
} // namespace Player

} // namespace JASystem

#endif // JASPLAYER_IMPL_HPP
