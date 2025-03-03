#ifndef JASDRIVERIF_HPP
#define JASDRIVERIF_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Driver {
	void setUpdateInterval(u8);
	u8 getUpdateInterval();
	extern s32 JAS_UPDATE_INTERVAL;

	void setOutputMode(u32);
	u32 getOutputMode();
	extern u32 JAS_SYSTEM_OUTPUT_MODE;

	void setMixerLevel(f32, f32);
	f32 getMixerLevel();
	extern f32 MAX_MIXERLEVEL;

	void init();
	void setDSPQueueNum(u32);
	extern u32 DSP_VOICE_WAITS_MAX;
} // namespace Driver

} // namespace JASystem

#endif // JASDRIVERIF_HPP
