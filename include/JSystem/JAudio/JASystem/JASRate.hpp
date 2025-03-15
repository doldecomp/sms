#ifndef JASRATE_HPP
#define JASRATE_HPP

#include <dolphin/types.h>

namespace JASystem {
namespace Kernel {
	extern f32 gDacRate;
	extern u32 gOutputRate;
	extern u32 gSubFrames;
	extern u32 gDacSize;
	extern u32 gFrameSamples;
	extern u32 gAiSetting;

	inline f32 getDacRate() { return gDacRate; }
	inline u32 getSubFrames() { return gSubFrames; }
	inline u32 getFrameSamples() { return gFrameSamples; }
	inline u32 getDacSize() { return gDacSize; }
	inline int getOutputRate() { return gOutputRate; }
	inline int getAiSetting() { return gAiSetting; }
} // namespace Kernel
} // namespace JASystem

#endif // JASRATE_HPP
