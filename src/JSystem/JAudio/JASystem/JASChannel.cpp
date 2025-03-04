#include <JSystem/JAudio/JASystem/JASChannel.hpp>

namespace JASystem {

namespace Driver {
	// TODO: This is definitely the wrong data type
	u8 calc_sw_table[81]
	    = { 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 2, 0, 1, 0,
		    0, 2, 1, 0, 2, 2, 1, 0, 0, 1, 0, 1, 1, 0, 2, 1, 1, 0, 1, 1, 1,
		    1, 1, 2, 1, 2, 0, 1, 2, 1, 1, 2, 2, 1, 0, 0, 2, 0, 1, 2, 0, 2,
		    2, 1, 0, 2, 1, 1, 2, 1, 2, 2, 2, 0, 2, 2, 1, 2, 2, 2 };
} // namespace Driver

void TChannel::init() { }

void TChannel::setOscillator(u32 index, TOscillator* oscillator) { }

void TChannel::setOscInit(u32 index, const TOscillator::Osc_* osc) { }

void TChannel::forceStopOsc(u32 index) { }

void TChannel::releaseOsc(u32 index) { }

void TChannel::directReleaseOsc(u32 index, u16 release) { }

void TChannel::bankOscToOfs(u32 index) { }

void TChannel::effectOsc(u32 index, f32 effect) { }

u32 TChannel::getOscState(u32 index) const { return 0; }

bool TChannel::isOsc(u32 index) { return false; }

void TChannel::copyOsc(u32 index, TOscillator::Osc_* dest) { }

void TChannel::overwriteOsc(u32 index, TOscillator::Osc_* src) { }

void TChannel::overwriteOscMultiple(TOscillator::Osc_* osc1,
                                    TOscillator::Osc_* osc2,
                                    TOscillator::Osc_* osc3,
                                    TOscillator::Osc_* osc4)
{
}

void TChannel::initOscForOscSound() { }

void TChannel::setKeySweepTarget(u8 key, u32 target) { }

void TChannel::setPauseFlag(u8 flag) { }

void TChannel::setPauseFlagReq(u8 flag) { }

void TChannel::setPanPower(f32 fl, f32 fr, f32 rl, f32 rr) { }

void TChannel::setPanParam(const f32* pan, const f32* dolby, const f32* fx) { }

void TChannel::setFxParam(const f32* pan, const f32* dolby, const f32* fx) { }

void TChannel::setDolbyParam(const f32* pan, const f32* dolby, const f32* fx) {
}

bool TChannel::checkLogicalChannel() { return false; }

void TChannel::resetInitialVolume() { }

void TChannel::play(u32 param) { }

void TChannel::stop(u16 release) { }

void TChannel::updateJcToDSP() { }

void TChannel::forceStopLogicalChannel() { }

void TChannel::stopLogicalChannel() { }

void TChannel::playLogicalChannel() { }

void TChannel::updateEffectorParam() { }

namespace Driver {
	void calcEffect(const PanMatrix_* matrix1, const PanMatrix_* matrix2,
	                u8 param)
	{
	}

	void calcPan(const PanMatrix_* matrix1, const PanMatrix_* matrix2, u8 param)
	{
	}

	void __UpdateJcToDSP(TChannel* channel) { }

	void __UpdateJcToDSPInit(TChannel* channel) { }

	void extraUpdate(TChannel* channel, u32 param) { }

	void updatecallLogicalChannel(TChannel* channel, u32 param) { }

	void killBrokenLogicalChannels(TDSPChannel* dspChannel) { }

	void updateAutoMixer(TChannel* channel, f32 fl, f32 fr, f32 rl, f32 rr) { }

	void updateMixer(TChannel* channel, f32 fl, f32 fr, f32 rl, f32 rr) { }

	void updatecallDSPChannel(TDSPChannel* dspChannel, u32 param) { }
} // namespace Driver

} // namespace JASystem
