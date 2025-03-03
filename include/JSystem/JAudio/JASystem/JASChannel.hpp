#ifndef JASCHANNEL_HPP
#define JASCHANNEL_HPP

namespace JASystem {

class TOscillator;
class TDSPChannel;

namespace Driver {
	struct PanMatrix_;
}

class TChannel {
public:
	void init();
	void setOscillator(unsigned long, TOscillator*);
	void setOscInit(unsigned long, const TOscillator::Osc_*);
	void forceStopOsc(unsigned long);
	void releaseOsc(unsigned long);
	void directReleaseOsc(unsigned long, unsigned short);
	void bankOscToOfs(unsigned long);
	void effectOsc(unsigned long, float);
	unsigned long getOscState(unsigned long) const;
	bool isOsc(unsigned long);
	void copyOsc(unsigned long, TOscillator::Osc_*);
	void overwriteOsc(unsigned long, TOscillator::Osc_*);
	void overwriteOscMultiple(TOscillator::Osc_*, TOscillator::Osc_*,
	                          TOscillator::Osc_*, TOscillator::Osc_*);
	void initOscForOscSound();
	void setKeySweepTarget(unsigned char, unsigned long);
	void setPauseFlag(unsigned char);
	void setPauseFlagReq(unsigned char);
	void setPanPower(float, float, float, float);
	void setPanParam(const float*, const float*, const float*);
	void setFxParam(const float*, const float*, const float*);
	void setDolbyParam(const float*, const float*, const float*);
	bool checkLogicalChannel();
	void resetInitialVolume();
	void play(unsigned long);
	void stop(unsigned short);
	void updateJcToDSP();
	void forceStopLogicalChannel();
	void stopLogicalChannel();
	void playLogicalChannel();
	void updateEffectorParam();
};

namespace Driver {
	void calcEffect(const PanMatrix_*, const PanMatrix_*, unsigned char);
	void calcPan(const PanMatrix_*, const PanMatrix_*, unsigned char);
	void __UpdateJcToDSP(TChannel*);
	void __UpdateJcToDSPInit(TChannel*);
	void extraUpdate(TChannel*, unsigned long);
	void updatecallLogicalChannel(TChannel*, unsigned long);
	void killBrokenLogicalChannels(TDSPChannel*);
	void updateAutoMixer(TChannel*, float, float, float, float);
	void updateMixer(TChannel*, float, float, float, float);
	void updatecallDSPChannel(TDSPChannel*, unsigned long);

	extern unsigned long OSC_REL;
	extern unsigned long OSC_ENV;
	extern unsigned long calc_sw_table;
} // namespace Driver

} // namespace JASystem

#endif // JASCHANNEL_HPP
