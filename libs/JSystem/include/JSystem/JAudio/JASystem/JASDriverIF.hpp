#ifndef JASDRIVERIF_HPP
#define JASDRIVERIF_HPP

#include <dolphin/types.h>

enum JASOutputRate {
	JASOUTRATE_0, // 0
};

enum JASMixMode {
	MixMode_Mono       = 0,
	MixMode_Mono_Wide  = 1,
	MixMode_Extra      = 2,
	MixMode_InterLeave = 3,
};

enum JASOutputMode {
	JASOUTPUT_Mono     = 0,
	JASOUTPUT_Stereo   = 1,
	JASOUTPUT_Surround = 2,
};

namespace JASystem {

namespace Driver {
	void init();

	void setMixerLevel(f32 channel_level, f32 dsp_level);
	u16 getMixerLevel();

	void setOutputMode(u32 mode);
	u32 getOutputMode();

	void setUpdateInterval(u8 interval);
	u8 getUpdateInterval();

	void setDSPQueueNum(u32 num);

	inline f32 Clamp01(f32 value)
	{
		if (value <= 0.0f)
			return 0.0f;
		if (value >= 1.0f)
			return 1.0f;
		return value;
	}

} // namespace Driver

} // namespace JASystem

#endif // JASDRIVERIF_HPP
