#include <JSystem/JAudio/JASystem/JASDriverIF.hpp>

namespace JASystem {

namespace Driver {
	f32 MAX_MIXERLEVEL         = 1.0186341E-10;
	u32 JAS_SYSTEM_OUTPUT_MODE = 0x00000001;
	s32 JAS_UPDATE_INTERVAL    = 0x01000000;
	u32 DSP_VOICE_WAITS_MAX    = 0x00000020;

	void setUpdateInterval(u8 interval) { }

	u8 getUpdateInterval() { return 0; }

	void setOutputMode(u32 mode) { }

	u32 getOutputMode() { return 0; }

	void setMixerLevel(f32 level, f32 param) { }

	f32 getMixerLevel() { return 0.0f; }

	void init() { }

	void setDSPQueueNum(u32 num) { }
} // namespace Driver

} // namespace JASystem
