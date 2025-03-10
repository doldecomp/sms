#include <JSystem/JAudio/JASystem/JASDriverIF.hpp>
#include <JSystem/JAudio/JASystem/JASDSPBuf.hpp>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <JSystem/JAudio/JASystem/JASDSPChannel.hpp>
#include <JSystem/JAudio/JASystem/JASChGlobal.hpp>
#include <JSystem/JAudio/JASystem/JASChAllocQueue.hpp>
#include <JSystem/dspproc.h>

namespace JASystem {

namespace Driver {
	static u16 MAX_MIXERLEVEL         = 0x2ee0;
	static u32 JAS_SYSTEM_OUTPUT_MODE = 1;
	static u8 JAS_UPDATE_INTERVAL     = 1;
	static u32 DSP_VOICE_WAITS_MAX    = 0x20;

	void init()
	{
		DSPBuf::process(DSPBuf::DSPBUF_EVENTS_UNK0);
		DSPInterface::initBuffer();
		TDSPChannel::initAll();
		ChGlobal::init();
		Driver::DSPQueue::init(Driver::DSP_VOICE_WAITS_MAX);
	}

	void setMixerLevel(f32 channel_level, f32 dsp_level)
	{
		MAX_MIXERLEVEL = channel_level * 16384.0f;
		DsetMixerLevel(dsp_level);
	}

	u16 getMixerLevel() { return MAX_MIXERLEVEL; }

	void setOutputMode(u32 mode) { JAS_SYSTEM_OUTPUT_MODE = mode; }

	u32 getOutputMode() { return JAS_SYSTEM_OUTPUT_MODE; }

	void setUpdateInterval(u8 interval) { JAS_UPDATE_INTERVAL = interval; }

	u8 getUpdateInterval() { return JAS_UPDATE_INTERVAL; }

	void setDSPQueueNum(u32 num) { DSP_VOICE_WAITS_MAX = num; }
} // namespace Driver

} // namespace JASystem
