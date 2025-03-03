#ifndef JASDRIVERIF_HPP
#define JASDRIVERIF_HPP

namespace JASystem {

namespace Driver {
	void setUpdateInterval(unsigned char);
	unsigned char getUpdateInterval();
	extern unsigned long JAS_UPDATE_INTERVAL;

	void setOutputMode(unsigned long);
	unsigned long getOutputMode();
	extern unsigned long JAS_SYSTEM_OUTPUT_MODE;

	void setMixerLevel(float, float);
	float getMixerLevel();
	extern float MAX_MIXERLEVEL;

	void init();
	void setDSPQueueNum(unsigned long);
	extern unsigned long DSP_VOICE_WAITS_MAX;
} // namespace Driver

} // namespace JASystem

#endif // JASDRIVERIF_HPP
