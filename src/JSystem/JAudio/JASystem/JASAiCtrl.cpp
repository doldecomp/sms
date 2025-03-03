#include <JSystem/JAudio/JASystem/JASAiCtrl.hpp>

namespace JASystem {
namespace Kernel {

	// AI Control variables
	void (*dacCallbackFunc)(s16*, s32) = nullptr;
	s16* (*extMixCallback)(s32)        = nullptr;
	u8 extMixMode                      = 0;
	u8 useRspMadep                     = 0;
	u8 lastRspMadep                    = 0;
	u8 vframeWorkRunning               = 0;
	u32 JASVframeCounter               = 0;
	u32 JASUniversalDacCounter         = 0;

	// AI Control functions
	void registerDacCallback(void (*callback)(s16*, s32)) { }

	s16* mixMonoTrack(s16* buffer, s32 sampleCount, s16* (*mixCallback)(s32))
	{
		return nullptr;
	}

	s16* mixMonoTrackWide(s16* buffer, s32 sampleCount,
	                      s16* (*mixCallback)(s32))
	{
		return nullptr;
	}

	s16* mixExtraTrack(s16* buffer, s32 sampleCount, s16* (*mixCallback)(s32))
	{
		return nullptr;
	}

	s16* mixInterleaveTrack(s16* buffer, s32 sampleCount,
	                        s16* (*mixCallback)(s32))
	{
		return nullptr;
	}

	s16* (*getMixCallback(u8* mode))(s32) { return nullptr; }

	void registerMixCallback(s16* (*callback)(s32), u8 mode)
	{
		extMixCallback = callback;
		extMixMode     = mode;
	}

	u32 getCurrentVCounter() { return 0; }

	// System functions
	void init() { }

	void initSystem() { }

	void vframeWork() { }

	void updateDac() { }

} // namespace Kernel
} // namespace JASystem
