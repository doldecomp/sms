#ifndef JASCHANNEL_HPP
#define JASCHANNEL_HPP

#include <JSystem/JAudio/JASystem/JASOscillator.hpp>
#include <stdint.h>

namespace JASystem {

enum MixParameter {
	MIX_PAN   = 0,
	MIX_FXMIX = 1,
	MIX_DOLBY = 2,
	MIX_COUNT,
};

class TDSPChannel;

namespace Driver {
	struct PanMatrix_ {
		/* 0x0 */ f32 mInstrument;
		/* 0x4 */ f32 mModulation;
		/* 0x8 */ f32 mChannelManager;
	};

	struct Wave_ {
		/* 0x0 */ u8 unk0;
		/* 0x1 */ u8 mFormat;
		/* 0x2 */ u8 mKey;
		/* 0x4 */ f32 mSampleRate;
		/* 0x8 */ s32 mOffset;
		/* 0xC */ s32 mDataSize;
		/* 0x10 */ s32 mLoop;
		/* 0x14 */ s32 mLoopStart;
		/* 0x18 */ s32 mLoopEnd;
		/* 0x1C */ s32 mSampleCount;
		/* 0x20 */ s16 mLoopHistory1;
		/* 0x22 */ s16 mLoopHistory2;
		/* 0x24 */ u32* mLoadFlagPtr;
		/* 0x28 */ s32 unk28;
	};
} // namespace Driver

class TChannelMgr;

/**
 * @brief A single logical channel used by the audio engine. Playing it produces
 * a single note, be it a synthesized not or a wave sample, with particular
 * mixing & filtering setup.
 *
 * @details A logical channel plays it's note either from a wave sample, or
 * synthesizes it in the DSP based on a waveform config. The mixing parameters
 * of a channel are:
 * - Volume
 * - Pitch (essentially sample playback speed)
 * - Fxmix (essentially reverb)
 * - Pan (left-right position of the sound)
 * - Dolby (front-back position of the sound)
 *
 * Each channel also contains 4 oscillators, which can modulate any of it's mix
 * parameters with an almost arbitrary envelope. Channels are also grouped via
 * JASystem::TChannelMgr and share mixing and filtering parameters within the
 * group.
 *
 * Updates for a playing note are pumped backwards from DSP syncs to physical
 * DSP channels and then back to logical channels, applying the necessary
 * modulation from oscillators to be included in the next audio frame.
 *
 * Logical channels compete for physical DSP channels, the former being
 * unlimited, the later there being only 64 of. The scheduling mechanism is
 * spread out over JASystem::TChannel, JASystem::TDSPChannel and
 * JASystem::Driver::DSPQueue, and has priorities, stealing, and other fancy
 * mechanisms.
 *
 * @warning The shipped scheduling implementation seems to contain a bug, see
 * the killBrokenLogicalChannels workaround.
 */
class TChannel {
public:
	enum CalcType {
		CALC_IGNORE_ALL       = 0,
		CALC_MANAGER_ONLY     = 1,
		CALC_SUM_ALL          = 13,
		CALC_WEIGHTED_SUM_ALL = 26,
	};

	TChannel()
	    : mManager(nullptr)
	    , mOwningList(nullptr)
	    , mDspChannel(nullptr)
	    , mNext(nullptr)
	{
		for (int i = 0; i < 4; ++i)
			mOscillators[i] = nullptr;
	}
	~TChannel() { }

	/// Sets the data backing an oscillator
	void setOscInit(u32 index, const TOscillator::Osc_* osc);

	/// Makes the release of an oscillator direct: release_config specifies a
	/// custom time and curve for the release
	void directReleaseOsc(u32 index, u16 release_config);

	/// True when oscillator \p index has backing data
	BOOL isOsc(u32 index);

	/// Copies backing data of one of the oscillators to \p dest
	void copyOsc(u32 index, TOscillator::Osc_* dest);

	/// Replaces oscillator \p index with a new oscillator \p src,
	/// re-initializing it to off state and applying it's first value
	/// immediately.
	void overwriteOsc(u32 index, TOscillator::Osc_* src);

	void setKeySweepTarget(u8 key, u32 target);

	/// True pauses the channel from playing but does not evict it from the DSP
	void setPauseFlag(u8 flag);

	/// Sets the power of various pan sources, normalizing the sum to unity
	void setPanPower(f32 instrument, f32 modulation, f32 manager, f32);

	/// Updates the DSP parameters of this channel and sends them to it
	bool resetInitialVolume();

	/// Starts playing the logical channel by attaching it to a DSP channel
	BOOL play(u32 gate_time);

	/// Requests the channel to stop playing, possibly overriding the release
	/// time with \p release_time
	void stop(u16 release_time);

public:
	// private-ISH stuff that's internal to the quartet of
	// TChannel-TChannelMgr-DSPQueue-DSPChannel.
	void init();
	void setOscillator(u32 index, TOscillator* oscillator);
	bool forceStopOsc(u32 index);
	bool releaseOsc(u32 index);
	f32 bankOscToOfs(u32 index);
	void effectOsc(u32 index, f32 modulation);
	u8 getOscState(u32 index) const;
	void overwriteOscMultiple(TOscillator::Osc_* osc1, TOscillator::Osc_* osc2,
	                          TOscillator::Osc_* osc3, TOscillator::Osc_* osc4);
	void initOscForOscSound();
	void setPauseFlagReq(u8 flag);
	void setPanParam(const f32* pan, const f32* dolby, const f32* fx);
	void setFxParam(const f32* pan, const f32* dolby, const f32* fx);
	void setDolbyParam(const f32* pan, const f32* dolby, const f32* fx);
	BOOL checkLogicalChannel();
	void updateJcToDSP();
	BOOL stopLogicalChannel();
	void updateEffectorParam();
	BOOL forceStopLogicalChannel();
	BOOL playLogicalChannel();

	u8 getNoteOnPriority() const { return mPriorityConfig & 0xff; }
	u8 getReleasePriority() const { return (mPriorityConfig >> 8) & 0xff; }
	u8 getLifeTimePriority() const { return mPriorityConfig >> 0x10; }
	bool isDolbyMode() const { return mMixConfigs[0].mWhole == 0xFFFF; }

	union MixConfig {
		enum GainSource {
			GAIN_NONE      = 0,
			GAIN_PAN       = 1,
			GAIN_FXMIX     = 2,
			GAIN_DOLBY     = 3,
			GAIN_INV_PAN   = 5,
			GAIN_INV_FXMIX = 6,
			GAIN_INV_DOLBY = 7,
		};

		u16 mWhole;
		struct {
			u8 mSendDestination;
			u8 mGainSource0 : 4;
			u8 mGainSource1 : 4;
		} mParts;
	};

	enum EventType {
		EVENT_RELEASE           = 0,
		EVENT_DSP_FINISHED      = 1,
		EVENT_ENVELOPE_FINISHED = 2,
		EVENT_CANCELLED         = 6,
	};

public:
	/* 0x0 */ u8 mVelocity;
	/* 0x1 */ u8 mKey;
	/* 0x2 */ u8 mPauseFlag;
	/* 0x3 */ u8 mDspUpdatePending;
	/* 0x4 */ TChannelMgr* mManager;
	/* 0x8 */ TChannel** mOwningList;
	/* 0xC */ u8 mLogicalChanType;
	/* 0x10 */ Driver::Wave_* mWaveData;
	/* 0x14 */ uintptr_t mSourceData; // wave samples addr or waveform type
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ TDSPChannel* mDspChannel;
	/* 0x24 */ TChannel* mNext;
	/* 0x28 */ BOOL (*mEventCallback)(TChannel*, u32);
	/* 0x2C */ u32 (*mUpdateCallback)(TChannel*, u32);
	/* 0x30 */ s32 mGateTime;
	/* 0x34 */ s32 mGateTimeRemaining;
	/* 0x38 */ TOscillator* mOscillators[4];
	/* 0x48 */ f32 mBasePitch;
	/* 0x4C */ f32 mInstrumentVolume;
	/* 0x50 */ f32 mNotePitch;
	/* 0x54 */ f32 mNoteVolume;
	/* 0x58 */ u8 mCalcTypes[MIX_COUNT];
	/* 0x5C */ Driver::PanMatrix_ mSourceWeights;
	/* 0x68 */ Driver::PanMatrix_ mMixInputs[MIX_COUNT];
	/* 0x8C */ f32 mOscPitch;
	/* 0x90 */ f32 mOscVolume;
	/* 0x94 */ f32 mPitchSweepTarget;
	/* 0x98 */ u16 mDspPitch;
	/* 0x9A */ u16 mPitchSweepRemaining;
	/* 0x9C */ TChannelMgr* mInitialManager;
	/* 0xA0 */ f32 mManagerPitch;
	/* 0xA4 */ f32 mManagerVolume;
	/* 0xA8 */ MixConfig mMixConfigs[6];
	/* 0xB4 */ s16 mMixerVolumes[6];
	/* 0xC0 */ u32 mPriorityConfig;
	/* 0xC4 */ u16 mPriorityTime;
	/* 0xC6 */ u16 mGeneration;
	/* 0xC8 */ u32 mPolyphonyKey;
	/* 0xCC */ char unkCC[0x4];
	/* 0xD0 */ BOOL mPreserveDspConfig;
	/* 0xD4 */ char unkD4[0xC];
};

} // namespace JASystem

#endif // JASCHANNEL_HPP
