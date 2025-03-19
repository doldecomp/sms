#ifndef JASCHANNEL_HPP
#define JASCHANNEL_HPP

#include <JSystem/JAudio/JASystem/JASOscillator.hpp>

namespace JASystem {

class TDSPChannel;

namespace Driver {
	struct PanMatrix_ {
		/* 0x0 */ f32 mSound;
		/* 0x4 */ f32 mEffect;
		/* 0x8 */ f32 mChannel;
	};

	struct Wave_ {
		/* 0x0 */ u8 unk0;
		/* 0x1 */ u8 unk1;
		/* 0x2 */ u8 unk2;
		/* 0x4 */ char unk4[0xC];
		/* 0x10 */ int unk10;
		/* 0x14 */ int unk14;
		/* 0x18 */ int unk18;
		/* 0x1C */ int unk1C;
		/* 0x20 */ short unk20;
		/* 0x22 */ short unk22;
		/* 0x24 */ u32* unk24;
	};
} // namespace Driver

class TChannelMgr;

class TChannel {
public:
	enum CalcType {
		// (TWW) NB: haven't seen others used, add from calc_sw_table in
		// JASChannel.cpp if needed
		CALC_None           = 0,
		CALC_AddChannelOnly = 1,
		CALC_AddAll         = 13,
		CALC_WeightAll      = 26,
	};

	TChannel()
	    : unk4(0)
	    , unk8(0)
	    , unk20(0)
	    , mNext(0)
	{
		for (int i = 0; i < 4; ++i)
			unk38[i] = 0;
	}
	~TChannel() { }

	void init();
	void setOscillator(u32 index, TOscillator* oscillator);
	void setOscInit(u32 index, const TOscillator::Osc_* osc);
	bool forceStopOsc(u32 index);
	bool releaseOsc(u32 index);
	void directReleaseOsc(u32 index, u16 release);
	f32 bankOscToOfs(u32 index);
	void effectOsc(u32 index, f32 effect);
	u8 getOscState(u32 index) const;
	BOOL isOsc(u32 index);
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
	BOOL checkLogicalChannel();
	bool resetInitialVolume();
	BOOL play(u32 param);
	void stop(u16 release);
	void updateJcToDSP();
	BOOL forceStopLogicalChannel();
	BOOL stopLogicalChannel();
	BOOL playLogicalChannel();
	void updateEffectorParam();

	u8 getLifeTimePriority() const { return unkC0 >> 0x10; }

	union MixConfig {
		u16 mWhole;
		struct {
			u8 u;
			u8 l0 : 4;
			u8 l1 : 4;
		} mParts;
	};

public:
	/* 0x0 */ u8 unk0;
	/* 0x1 */ u8 unk1;
	/* 0x2 */ u8 unk2;
	/* 0x3 */ u8 unk3;
	/* 0x4 */ TChannelMgr* unk4;
	/* 0x8 */ TChannel** unk8;
	/* 0xC */ u8 unkC;
	/* 0x10 */ Driver::Wave_* unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ TDSPChannel* unk20;
	/* 0x24 */ TChannel* mNext;
	/* 0x28 */ BOOL (*unk28)(TChannel*, u32);
	/* 0x2C */ u32 (*unk2C)(TChannel*, u32);
	/* 0x30 */ s32 unk30;
	/* 0x34 */ s32 unk34;
	/* 0x38 */ TOscillator* unk38[4];
	/* 0x48 */ f32 unk48;
	/* 0x4C */ f32 unk4C;
	/* 0x50 */ f32 unk50;
	/* 0x54 */ f32 unk54;
	/* 0x58 */ u8 unk58[3];
	/* 0x5C */ Driver::PanMatrix_ unk5C;
	/* 0x68 */ Driver::PanMatrix_ unk68;
	/* 0x74 */ Driver::PanMatrix_ unk74;
	/* 0x80 */ Driver::PanMatrix_ unk80;
	/* 0x8C */ f32 unk8C;
	/* 0x90 */ f32 unk90;
	/* 0x94 */ f32 unk94;
	/* 0x98 */ u16 unk98;
	/* 0x9A */ u16 unk9A;
	/* 0x9C */ TChannelMgr* unk9C;
	/* 0xA0 */ f32 unkA0;
	/* 0xA0 */ f32 unkA4;
	/* 0xA8 */ MixConfig unkA8[6];
	/* 0xB4 */ s16 unkB4[6];
	/* 0xC0 */ u32 unkC0;
	/* 0xC4 */ u16 unkC4;
	/* 0xC6 */ u16 unkC6;
	/* 0xC8 */ u32 unkC8;
	/* 0xCC */ char unkCC[0x4];
	/* 0xD0 */ s32 unkD0;
	/* 0xD4 */ char unkD4[0xC];
};

} // namespace JASystem

#endif // JASCHANNEL_HPP
