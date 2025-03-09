#ifndef JASCHANNEL_HPP
#define JASCHANNEL_HPP

#include <JSystem/JAudio/JASystem/JASOscillator.hpp>

namespace JASystem {

class TDSPChannel;

namespace Driver {
	struct PanMatrix_;
	struct Wave_;
} // namespace Driver

class TChannelMgr;

class TChannel {
public:
	void init();
	void setOscillator(u32 index, TOscillator* oscillator);
	void setOscInit(u32 index, const TOscillator::Osc_* osc);
	void forceStopOsc(u32 index);
	void releaseOsc(u32 index);
	void directReleaseOsc(u32 index, u16 release);
	void bankOscToOfs(u32 index);
	void effectOsc(u32 index, f32 effect);
	u32 getOscState(u32 index) const;
	bool isOsc(u32 index);
	void copyOsc(u32 index, TOscillator::Osc_* dest);
	void overwriteOsc(u32 index, TOscillator::Osc_* src);
	void overwriteOscMultiple(TOscillator::Osc_* osc1, TOscillator::Osc_* osc2,
	                          TOscillator::Osc_* osc3, TOscillator::Osc_* osc4);
	void initOscForOscSound();
	void setKeySweepTarget(u8 key, u32 target);
	void setPauseFlag(u8 flag);
	void setPauseFlagReq(u8 flag);
	void setPanPower(f32 fl, f32 fr, f32 rl, f32 rr);
	void setPanParam(const f32* pan, const f32* dolby, const f32* fx);
	void setFxParam(const f32* pan, const f32* dolby, const f32* fx);
	void setDolbyParam(const f32* pan, const f32* dolby, const f32* fx);
	bool checkLogicalChannel();
	void resetInitialVolume();
	void play(u32 param);
	void stop(u16 release);
	void updateJcToDSP();
	void forceStopLogicalChannel();
	void stopLogicalChannel();
	void playLogicalChannel();
	void updateEffectorParam();

public:
	/* 0x0 */ char unk0[0x4];
	/* 0x4 */ TChannelMgr* unk4;
	/* 0x0 */ char unk8[0x18];
	/* 0x20 */ TDSPChannel* unk20;
	/* 0x24 */ char unk24[0x4];
	/* 0x28 */ void (*unk28)(TChannel*, u32);
	/* 0x2C */ char unk2C[0x4];
	/* 0x30 */ s32 unk30;
	/* 0x34 */ char unk34[0x8C];
	/* 0xC0 */ u32 unkC0;
};

namespace Driver {
	void calcEffect(const PanMatrix_* matrix1, const PanMatrix_* matrix2,
	                u8 param);
	void calcPan(const PanMatrix_* matrix1, const PanMatrix_* matrix2,
	             u8 param);
	void __UpdateJcToDSP(TChannel* channel);
	void __UpdateJcToDSPInit(TChannel* channel);
	void extraUpdate(TChannel* channel, u32 param);
	void updatecallLogicalChannel(TChannel* channel, u32 param);
	void killBrokenLogicalChannels(TDSPChannel* dspChannel);
	void updateAutoMixer(TChannel* channel, f32 fl, f32 fr, f32 rl, f32 rr);
	void updateMixer(TChannel* channel, f32 fl, f32 fr, f32 rl, f32 rr);
	void updatecallDSPChannel(TDSPChannel* dspChannel, u32 param);

	extern u8 calc_sw_table[];
} // namespace Driver

} // namespace JASystem

#endif // JASCHANNEL_HPP
