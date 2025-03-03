#ifndef JASAICTRL_HPP
#define JASAICTRL_HPP

#include <types.h>

namespace JASystem {
namespace Kernel {

	// AI Control functions
	void registerDacCallback(void (*callback)(s16*, s32));
	s16* mixMonoTrack(s16* buffer, s32 sampleCount, s16* (*mixCallback)(s32));
	s16* mixMonoTrackWide(s16* buffer, s32 sampleCount,
	                      s16* (*mixCallback)(s32));
	s16* mixExtraTrack(s16* buffer, s32 sampleCount, s16* (*mixCallback)(s32));
	s16* mixInterleaveTrack(s16* buffer, s32 sampleCount,
	                        s16* (*mixCallback)(s32));
	s16* (*getMixCallback(u8* mode))(s32);
	void registerMixCallback(s16* (*callback)(s32), u8 mode);
	u32 getCurrentVCounter();

	// AI Control variables
	extern void (*dacCallbackFunc)(s16*, s32);
	extern s16* (*extMixCallback)(s32);
	extern u8 extMixMode;
	extern u8 useRspMadep;
	extern u8 lastRspMadep;
	extern u8 vframeWorkRunning;
	extern u32 JASVframeCounter;
	extern u32 JASUniversalDacCounter;

	// System functions
	void init();
	void initSystem();
	void vframeWork();
	void updateDac();

} // namespace Kernel
} // namespace JASystem

#endif // JASAICTRL_HPP
