#ifndef JASTRACK_HPP
#define JASTRACK_HPP

#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>
#include <JSystem/JAudio/JASystem/JASRegisterParam.hpp>
#include <JSystem/JAudio/JASystem/JASSeqCtrl.hpp>
#include <JSystem/JAudio/JASystem/JASTrackInterrupt.hpp>
#include <JSystem/JAudio/JASystem/JASTrackPort.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <dolphin/types.h>

namespace JASystem {

namespace Player {
	enum SEQ_PLAYMODE { SEQ_PLAYMODE_UNK_0 };
} // namespace Player

class TSeqCtrl;
class TSeqParser;

class TTrack {
public:
	class TNoteMgr {
	public:
		enum ConnectFlag {
			CONNECT_NONE      = 0,
			CONNECT_TIE       = 1 << 0,
			CONNECT_KEY_SWEEP = 1 << 1,
		};

		void init();
		void endProcess();
		TChannel* getChannel(int voice_slot);

		void setChannel(int voice_slot, TChannel* channel)
		{
			JUT_ASSERT(voice_slot >= 0);
			JUT_ASSERT(voice_slot < 8);
			mVoices[voice_slot]           = channel;
			mVoiceGenerations[voice_slot] = channel->mGeneration;
		}

		void releaseChannel(int voice_slot) { mVoices[voice_slot] = nullptr; }

		void setLastNote(u8 note) { mLastNote = note; }
		u8 getLastNote() const { return mLastNote; }

		void setConnectCase(u8 val) { mConnectFlags = val; }
		u8 getConnectCase() const { return mConnectFlags; }

		void setBaseTime(u32 time) { mBaseTime = time; }
		void setBeforeTieMode(bool mode) { mBeforeTieMode = mode; }

		bool checkBeforeTieMode() const { return mBeforeTieMode; }

	public:
		/* 0x00 */ TChannel* mVoices[8];
		/* 0x20 */ u16 mVoiceGenerations[8];
		/* 0x30 */ int mBaseTime;
		/* 0x34 */ u8 mConnectFlags;
		/* 0x35 */ u8 mLastNote;
		/* 0x36 */ u8 mBeforeTieMode;
	};

	class TOuterParam {
	public:
		TOuterParam();
		void initExtBuffer();
		void setParam(u8 param, f32 value);
		void onSwitch(u16);
		bool checkOuterSwitch(u16);
		s16 getIntFirFilter(u8);

		f32 getVolume() const { return mVolume; }
		f32 getPitch() const { return mPitch; }
		f32 getFxVol() const { return mFxmix; }
		f32 getDolby() const { return mDolby; }
		f32 getPan() const { return mPan; }
		f32 getTempo() const { return mTempo; }

		u16 getOuterUpdate();
		void setOuterUpdate(u16);
		void setOuterSwitch(u16);
		void setFirFilter(s16*);
		u16 getSwitch();
		void setIntFirFilter(s16, u8);

	public:
		/* 0x00 */ u16 mEnabledParams;
		/* 0x02 */ u16 mDirtyParams;
		/* 0x04 */ f32 mVolume;
		/* 0x08 */ f32 mPitch;
		/* 0x0C */ f32 mFxmix;
		/* 0x10 */ f32 mDolby;
		/* 0x14 */ f32 mPan;
		/* 0x18 */ f32 mTempo;
		/* 0x1C */ s16 mFirFilter[8];
	};

	struct MoveParam_ {
#ifdef __MWERKS__
		MoveParam_()
		    : mCurrentValue(0.0f)
		    , mTargetValue(0.0f)
		    , mTicksRemaining(0.0f)
		    , mStepPerTick(0.0f)
		{
		}
#endif

		/* 0x00 */ f32 mCurrentValue;
		/* 0x04 */ f32 mTargetValue;
		/* 0x08 */ f32 mTicksRemaining;
		/* 0x0C */ f32 mStepPerTick;
	};

	enum TimedParamType {
		TIMED_VOLUME              = 0,
		TIMED_PITCH               = 1,
		TIMED_FXMIX               = 2,
		TIMED_PAN                 = 3,
		TIMED_DOLBY               = 4,
		TIMED_DIST_FILTER         = 5,
		TIMED_OSC0_DEPTH          = 6,
		TIMED_OSC0_RATE           = 7,
		TIMED_OSC0_BIAS           = 8,
		TIMED_OSC1_DEPTH          = 9,
		TIMED_OSC1_RATE           = 10,
		TIMED_OSC1_BIAS           = 11,
		TIMED_IIR0                = 12,
		TIMED_IIR1                = 13,
		TIMED_IIR2                = 14,
		TIMED_IIR3                = 15,
		TIMED_PAUSE_VOLUME        = 16,
		TIMED_MIXER_DELAY_BALANCE = 17,
		TIMED_COUNT, // 18
	};

	enum ProcessResult {
		TRACK_ACTIVE   = 0,
		TRACK_FINISHED = -1,
	};

	enum OscRouteMode {
		OSC_ROUTE_CHANNEL           = 0,
		OSC_ROUTE_COPY_KEEP_RELEASE = 4,
		OSC_ROUTE_COPY              = 8,
		OSC_ROUTE_TRACK             = 0xE,
		OSC_ROUTE_DISABLED          = 0xF,
	};

	enum VolumeMode {
		VOLUME_SQUARED = 0,
		VOLUME_LINEAR  = 1,
	};

	enum SeqState {
		SEQ_INACTIVE     = 0,
		SEQ_PLAYING      = 1,
		SEQ_READY        = 2,
		SEQ_STOP_PENDING = 3,
	};

	enum TrackMode {
		TRACK_MODE_INHERIT            = 0,
		TRACK_MODE_INDEPENDENT_PARAMS = 1 << 0,
		TRACK_MODE_INIT_REGISTERS     = 1 << 1,
		TRACK_MODE_INDEPENDENT
		= TRACK_MODE_INDEPENDENT_PARAMS | TRACK_MODE_INIT_REGISTERS,
		TRACK_MODE_PARENT_CHANNELS = 4,
	};

	enum MixMode {
		MIX_MODE_LOCAL  = 0,
		MIX_MODE_SOURCE = 1,
		MIX_MODE_BLEND  = 2,
	};

	enum PauseMuteFlag {
		PAUSE_MUTE_NONE    = 0,
		PAUSE_SCALE_VOLUME = 1 << 0,
		PAUSE_SEQUENCE     = 1 << 1,
		PAUSE_STOP_NOTES   = 1 << 2,
		PAUSE_CHANNELS     = 1 << 3,
		PAUSE_SKIP_NOTE_ON = 1 << 4,
		MUTE_STOP_NOTES    = 1 << 5,
		MUTE_SKIP_NOTE_ON  = 1 << 6,
	};

	enum ParamUpdateFlag {
		UPDATE_VOLUME      = 1 << TIMED_VOLUME,      // 0x00001
		UPDATE_PITCH       = 1 << TIMED_PITCH,       // 0x00002
		UPDATE_FXMIX       = 1 << TIMED_FXMIX,       // 0x00004
		UPDATE_PAN         = 1 << TIMED_PAN,         // 0x00008
		UPDATE_DOLBY       = 1 << TIMED_DOLBY,       // 0x00010
		UPDATE_DIST_FILTER = 1 << TIMED_DIST_FILTER, // 0x00020
		UPDATE_TEMPO       = 0x00040,
		UPDATE_FIR_FILTER  = 0x00080,
		UPDATE_IIR0        = 1 << TIMED_IIR0, // 0x01000
		UPDATE_IIR1        = 1 << TIMED_IIR1, // 0x02000
		UPDATE_IIR2        = 1 << TIMED_IIR2, // 0x04000
		UPDATE_IIR3        = 1 << TIMED_IIR3, // 0x08000
		UPDATE_IIRS = UPDATE_IIR0 | UPDATE_IIR1 | UPDATE_IIR2 | UPDATE_IIR3,
		UPDATE_PAUSE_VOLUME        = 1 << TIMED_PAUSE_VOLUME,        // 0x10000
		UPDATE_MIXER_DELAY_BALANCE = 1 << TIMED_MIXER_DELAY_BALANCE, // 0x20000
	};
	struct AInnerParam_ {
#ifdef __MWERKS__
		AInnerParam_() { }
#endif

		/* 0x000 */ MoveParam_ mVolume;
		/* 0x010 */ MoveParam_ mPitch;
		/* 0x020 */ MoveParam_ mFxmix;
		/* 0x030 */ MoveParam_ mPan;
		/* 0x040 */ MoveParam_ mDolby;
		/* 0x050 */ MoveParam_ mDistFilter;
		/* 0x060 */ MoveParam_ mOsc0Depth;
		/* 0x070 */ MoveParam_ mOsc0Rate;
		/* 0x080 */ MoveParam_ mOsc0Bias;
		/* 0x090 */ MoveParam_ mOsc1Depth;
		/* 0x0A0 */ MoveParam_ mOsc1Rate;
		/* 0x0B0 */ MoveParam_ mOsc1Bias;
		/* 0x0C0 */ MoveParam_ mIIRs[4];
		/* 0x100 */ MoveParam_ mPauseVolume;
		/* 0x110 */ MoveParam_ mMixerDelayBalance;
	};

	struct TimedParam_ {
		TimedParam_() { }

		union {
			AInnerParam_ mInnerParam;
			MoveParam_ mMoveParams[TIMED_COUNT];
		};
	};

	TTrack();
	~TTrack() { }

	void reset();
	void initTrack(void* data, u32 seq_offset, TTrack* parent);
	void initTimed();
	TTrack* openTrack(u8 index);
	bool closeTrack();
	int startTrack(TTrack* parent, u8 child_index, u8 track_mode,
	               u32 seq_offset);
	bool startSeq();
	bool stopSeq();
	void stopSeqMain();
	void allNoteOff();
	int setSeqData(u8* data, s32 size, Player::SEQ_PLAYMODE mode);
	void updateSeq(u32 update_flags, bool recurse_now);
	void updateTrack(u32 update_flags);
	void updateTrackAll();
	void updateTempo();
	s8 mainProc();
	int noteOn(u8 voice_slot, s32 key, s32 velocity, s32 gate_time);
	bool noteOff(u8 voice_slot, u16 release_time);
	int gateOn(u8 voice_slot, s32 key, s32 velocity, s32 gate_time);
	bool checkNoteStop(s32 voice_slot);
	void releaseChannelAll();
	void flushAll();
	void flushRelease();
	void muteTrack(u8 flag);
	void muteChildTracks(u16 flags);
	void pauseTrack(u8 flag);
	void pauseTrackAll();
	void unPauseTrack(u8 flag);
	void unPauseTrackAll();
	void setInterrupt(u16 interrupt);
	bool tryInterrupt();
	void setInnerMemory(u8 param);
	void setBankNumber(u8 bank);
	bool assignExtBuffer(TOuterParam* buffer);
	void setPanSwitchExt(u8 mix_mode, u8 mix_param);
	void setPanSwitchParent(u8 mix_mode, u8 mix_param);
	void setTrackExtPanPower(f32 power);
	void setExtFirFilterD(s16* filter);
	TTrack* routeTrack(u32 route);
	bool writePortApp(u32 port, u16 value);
	bool readPortApp(u32 port, u16* value);
	bool writePortAppDirect(u32 port, u16 value);
	bool readPortAppDirect(u32 port, u16* value);
	void checkExportApp(u32 port);
	void checkImportApp(u32 port);
	int loadTbl(u32 table_offset, u32 index_or_offset, u32 mode);
	u32 readReg32(u8 reg);
	u32 exchangeRegisterValue(u8 reg);
	u16 readRegDirect(u8 reg);
	void writeRegDirect(u8 reg, u16 value);
	void writeTimeParam(u8 opcode_params);
	void writeRegParam(u8 opcode_params);
	int seqTimeToDspTime(s32 time, u8 gate_percent);
	f32 panCalc(f32 local_value, f32 source_value, f32 source_weight,
	            u8 mix_mode);
	static s32 rootCallback(void* param);
	void updateSyncSw(u8 param);
	void incSelfOsc();
	void oscSetupSimple(u8 preset);
	void oscSetupSimpleEnv(u8 table, u32 table_offset);
	void oscSetupFull(u8 flags, u32 attack_table_offset,
	                  u32 release_table_offset);
	void oscUpdateParam(u8 param_id, f32 value);

	u8 getBank() const;
	u8 getProgram() const;
	f32 getVolume() const;
	f32 getPitch() const;
	f32 getPan() const;
	f32 getFxmix() const;
	f32 getDolby() const;
	u8 getNoteStatus(u8 voice_slot);

	static bool registerTrackCallback(u16 (*callback)(TTrack*, u16));
	static TSeqParser sParser;
	static u8 sUpdateSyncMode;
	static u16 (*sCallBackFunc)(TTrack*, u16);
	static u8 sOscTable[];

	// From tww
	TTrack* getParent() { return mParent; }
	TTrack* getChild(int index) { return mChildren[index]; }
	TOuterParam* getOuterParam() { return mOuterParam; }
	TSeqCtrl& getSeq() { return mSeqCtrl; }

	u8 checkImport(int i) const { return mTrackPort.checkImport(i); }
	u8 checkExport(int i) const { return mTrackPort.checkExport(i); }

	void setPanPower(int i, u16 power) { mRegisterParam.setPanPower(i, power); }
	void setPauseStatus(u8 status) { mPauseMuteFlags = status; }
	void setTranspose(s32 transpose) { mTranspose = transpose; }
	void setVolumeMode(u8 mode) { mVolumeMode = mode; }

	void updateOscParam(u8 param, f32 value)
	{
		switch (param) {
		case TIMED_OSC0_DEPTH:
			mOscData[0].mDepth = value;
			break;
		case TIMED_OSC0_RATE:
			mOscData[0].mRate = value;
			break;
		case TIMED_OSC0_BIAS:
			mOscData[0].mBias = value;
			break;
		case TIMED_OSC1_DEPTH:
			mOscData[1].mDepth = value;
			break;
		case TIMED_OSC1_RATE:
			mOscData[1].mRate = value;
			break;
		case TIMED_OSC1_BIAS:
			mOscData[1].mBias = value;
			break;
		}
	}

	// fabricated
	u32 getTrackRoute() const { return mTrackRoute; }

public:
	/* 0x0 */ TSeqCtrl mSeqCtrl;
	/* 0x48 */ TTrackPort mTrackPort;
	/* 0x88 */ TIntrMgr mIntrMgr;
	/* 0xB4 */ TNoteMgr mNoteMgr;
	/* 0xEC */ TChannelMgr mChannelMgr;
	/* 0x160 */ TimedParam_ mTimedParam;
	/* 0x280 */ TRegisterParam mRegisterParam;
	/* 0x2B0 */ char unk2B0[0x2C0 - 0x2B0];
	/* 0x2C0 */ TTrack* mParent;
	/* 0x2C4 */ TTrack* mChildren[16];
	/* 0x304 */ TOuterParam* mOuterParam;
	/* 0x308 */ u32 mTrackRoute;
	/* 0x30C */ TOscillator::Osc_ mOscData[2];
	/* 0x33C */ TOscillator mOscillators[2];
	/* 0x37C */ s16 mAdsTable[12];
	/* 0x394 */ s16 mRelTable[6];
	/* 0x3A0 */ s32 mOscMode[2];
	/* 0x3A8 */ u32 mRegistrationId;
	/* 0x3AC */ f32 mTickAccumulator;
	/* 0x3B0 */ f32 mTicksPerSubframe;
	/* 0x3B4 */ u32 mPendingUpdateFlags;
	/* 0x3B8 */ u16 mTempo;
	/* 0x3BA */ u16 mTimeBase;
	/* 0x3BC */ u8 mTrackMode;
	/* 0x3BD */ bool mOverrideParentTempo;
	/* 0x3BE */ u8 mFromTrackPool;
	/* 0x3BF */ s8 mTranspose;
	/* 0x3C0 */ s8 mEffectiveTranspose;
	/* 0x3C1 */ u8 mPauseMuteFlags;
	/* 0x3C2 */ u8 mMute;
	/* 0x3C3 */ u8 mVolumeMode;

	/* 0x3C4 */ u8 mSeqState;
	/* 0x3C5 */ u8 mExternalMixModes[MIX_COUNT];
	/* 0x3C8 */ u8 mParentMixModes[MIX_COUNT];

	/* 0x3CB */ u8 mTimeRelate;
	/* 0x3CC */ u8 mRegistered;
	/* 0x3CD */ u8 mPaused;
};

} // namespace JASystem

#endif // JASTRACK_HPP
