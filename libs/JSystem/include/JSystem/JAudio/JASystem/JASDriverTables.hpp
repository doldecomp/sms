#ifndef JASDRIVERTABLES_HPP
#define JASDRIVERTABLES_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Driver {
	extern f32 C5BASE_PITCHTABLE[];
} // namespace Driver

namespace DSPInterface {
	struct DSPRenderTables {
		s16 resamplingCoefficients[64][4];
		s16 waveforms[4][64];
		s16 sine[128];
	};

	extern s16 DSPADPCM_FILTER[16][2];
	extern DSPRenderTables DSPRES_FILTER;
} // namespace DSPInterface

} // namespace JASystem

#endif // JASDRIVERTABLES_HPP
