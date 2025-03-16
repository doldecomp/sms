#ifndef JASTRACK_HPP
#define JASTRACK_HPP

#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>
#include <JSystem/JAudio/JASystem/JASRegisterParam.hpp>
#include <JSystem/JAudio/JASystem/JASSeqCtrl.hpp>
#include <JSystem/JAudio/JASystem/JASTrackInterrupt.hpp>
#include <JSystem/JAudio/JASystem/JASTrackPort.hpp>
#include <types.h>

namespace JASystem {

namespace Player {
	enum SEQ_PLAYMODE {};
} // namespace Player

class TSeqCtrl;
class TSeqParser;

class TTrack {
public:
	class TNoteMgr {
	public:
		void init();
		void endProcess();
		TChannel* getChannel(int);

		void releaseChannel(int i) { unk0[i] = nullptr; }

		u16 getUnk20(u8 i) { return unk20[i]; }

	public:
		/* 0x00 */ TChannel* unk0[8];
		/* 0x20 */ u16 unk20[8];
		/* 0x30 */ int mBaseTime;
		/* 0x34 */ u8 mConnectCase;
		/* 0x35 */ u8 mLastNote;
		/* 0x36 */ u8 mBeforeTieMode;
	};

	class TOuterParam {
	public:
		TOuterParam();
		void initExtBuffer();
		void setParam(u8, f32);
		void onSwitch(u16);
		bool checkOuterSwitch(u16);
		s16 getIntFirFilter(u8);
		u16 getOuterUpdate();
		void setOuterUpdate(u16);
		void setOuterSwitch(u16);
		void setFirFilter(s16*);
		u16 getSwitch();
		void setIntFirFilter(s16, u8);

	public:
		/* 0x00 */ u16 unk0;
		/* 0x02 */ u16 unk2;
		/* 0x04 */ f32 unk4;
		/* 0x08 */ f32 unk8;
		/* 0x0C */ f32 unkC;
		/* 0x10 */ f32 unk10;
		/* 0x14 */ f32 unk14;
		/* 0x18 */ f32 unk18;
		/* 0x1C */ s16 unk1C[8];
	};

	struct MoveParam_ {
#ifdef __MWERKS__
		MoveParam_()
		    : mCurrentValue(0.0f)
		    , mTargetValue(0.0f)
		    , mMoveTime(0.0f)
		    , mMoveAmount(0.0f)
		{
		}
#endif

		/* 0x00 */ f32 mCurrentValue;
		/* 0x04 */ f32 mTargetValue;
		/* 0x08 */ f32 mMoveTime;
		/* 0x0C */ f32 mMoveAmount;
	};
	enum TimedParamType {
		TIMED_Volume      = 0,
		TIMED_Pitch       = 1,
		TIMED_Fxmix       = 2,
		TIMED_Pan         = 3,
		TIMED_Dolby       = 4,
		TIMED_Unk5        = 5,
		TIMED_Osc0_Width  = 6,
		TIMED_Osc0_Rate   = 7,
		TIMED_Osc0_Vertex = 8,
		TIMED_Osc1_Width  = 9,
		TIMED_Osc1_Rate   = 10,
		TIMED_Osc1_Vertex = 11,
		TIMED_IIR_Unk0    = 12,
		TIMED_IIR_Unk1    = 13,
		TIMED_IIR_Unk2    = 14,
		TIMED_IIR_Unk3    = 15,
		TIMED_Unk16       = 16,
		TIMED_Unk17       = 17,
		TIMED_Count, // 18
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
		/* 0x050 */ MoveParam_ unk50;
		/* 0x060 */ MoveParam_ mOsc0Width;
		/* 0x070 */ MoveParam_ mOsc0Rate;
		/* 0x080 */ MoveParam_ mOsc0Vertex;
		/* 0x090 */ MoveParam_ mOsc1Width;
		/* 0x0A0 */ MoveParam_ mOsc1Rate;
		/* 0x0B0 */ MoveParam_ mOsc1Vertex;
		/* 0x0C0 */ MoveParam_ mIIRs[4];
		/* 0x100 */ MoveParam_ unk100;
		/* 0x110 */ MoveParam_ unk110;
	};

	struct TimedParam_ {
		TimedParam_() { }

		union {
			AInnerParam_ mInnerParam;
			MoveParam_ mMoveParams[TIMED_Count];
		};
	};

	TTrack();
	~TTrack() { }

	void reset();
	void initTrack(void* data, u32 size, TTrack* parent);
	void initTimed();
	TTrack* openTrack(u8 param);
	bool closeTrack();
	int startTrack(TTrack* parent, u8 param1, u8 param2, u32 param3);
	bool startSeq();
	bool stopSeq();
	void stopSeqMain();
	void allNoteOff();
	int setSeqData(u8* data, s32 size, Player::SEQ_PLAYMODE mode);
	void updateSeq(u32 param, bool flag);
	void updateTrack(u32 param);
	void updateTrackAll();
	void updateTempo();
	s8 mainProc();
	int noteOn(u8 note, s32 velocity, s32 length, s32 delay);
	bool noteOff(u8 note, u16 release);
	int gateOn(u8 note, s32 velocity, s32 length, s32 delay);
	void checkNoteStop(s32 param);
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
	void tryInterrupt();
	void setInnerMemory(u8 param);
	void setBankNumber(u8 bank);
	bool assignExtBuffer(TOuterParam* buffer);
	void setPanSwitchExt(u8 param1, u8 param2);
	void setPanSwitchParent(u8 param1, u8 param2);
	void setTrackExtPanPower(f32 power);
	void setExtFirFilterD(s16* filter);
	TTrack* routeTrack(u32 route);
	bool writePortApp(u32 port, u16 value);
	bool readPortApp(u32 port, u16* value);
	bool writePortAppDirect(u32 port, u16 value);
	bool readPortAppDirect(u32 port, u16* value);
	void checkExportApp(u32 port);
	void checkImportApp(u32 port);
	int loadTbl(u32 param1, u32 param2, u32 param3);
	u32 readReg32(u8 reg);
	u32 exchangeRegisterValue(u8 reg);
	u16 readRegDirect(u8 reg);
	void writeRegDirect(u8 reg, u16 value);
	void writeTimeParam(u8 param);
	void writeRegParam(u8 param);
	int seqTimeToDspTime(s32 time, u8 param);
	f32 panCalc(f32 param1, f32 param2, f32 param3, u8 param4);
	static s32 rootCallback(void* param);
	void updateSyncSw(u8 param);
	void incSelfOsc();
	void oscSetupSimple(u8 param);
	void oscSetupSimpleEnv(u8 param, u32 env);
	void oscSetupFull(u8 param1, u32 param2, u32 param3);
	void oscUpdateParam(u8 param, f32 value);

	u8 getBank() const;
	u8 getProgram() const;
	f32 getVolume() const;
	f32 getPitch() const;
	f32 getPan() const;
	f32 getFxmix() const;
	f32 getDolby() const;
	u8 getNoteStatus(u8 note);

	static bool registerTrackCallback(u16 (*callback)(TTrack*, u16));
	static TSeqParser sParser;
	static u8 sUpdateSyncMode;
	static u16 (*sCallBackFunc)(TTrack*, u16);
	static u8 sOscTable[];

public:
	/* 0x0 */ TSeqCtrl mSeqCtrl;
	/* 0x48 */ TTrackPort mTrackPort;
	/* 0x88 */ TIntrMgr mIntrMgr;
	/* 0xB4 */ TNoteMgr mNoteMgr;
	/* 0xEC */ TChannelMgr mChannelUpdater;
	/* 0x160 */ TimedParam_ mTimedParam;
	/* 0x280 */ TRegisterParam mRegisterParam;
	/* 0x2B0 */ char unk2B0[0x2C0 - 0x2B0];
	/* 0x2C0 */ TTrack* unk2C0;
	/* 0x2C4 */ TTrack* unk2C4[16];
	/* 0x304 */ TOuterParam* mOuterParam;
	/* 0x308 */ u32 unk308;
	/* 0x30C */ TOscillator::Osc_ unk30C[2];
	/* 0x33C */ TOscillator unk33C[2];
	/* 0x37C */ s16 unk37C[12];
	/* 0x394 */ s16 unk394[6];
	/* 0x3A0 */ s32 unk3A0[2];
	/* 0x3A8 */ u32 unk3A8;
	/* 0x3AC */ f32 unk3AC;
	/* 0x3B0 */ f32 unk3B0;
	/* 0x3B4 */ u32 unk3B4;
	/* 0x3B8 */ u16 unk3B8;
	/* 0x3BA */ u16 unk3BA;
	/* 0x3BC */ u8 unk3BC;
	/* 0x3BD */ u8 unk3BD;
	/* 0x3BE */ u8 unk3BE;
	/* 0x3BF */ u8 unk3BF;
	/* 0x3C0 */ u8 unk3C0;
	/* 0x3C1 */ u8 unk3C1;
	/* 0x3C2 */ u8 unk3C2;
	/* 0x3C3 */ u8 unk3C3;

	/* 0x3C4 */ u8 unk3C4;
	/* 0x3C5 */ u8 unk3C5[3];
	/* 0x3C8 */ u8 unk3C8[3];

	/* 0x3CB */ u8 unk3CB;
	/* 0x3CC */ u8 unk3CC;
	/* 0x3CD */ u8 unk3CD;
};

} // namespace JASystem

#endif // JASTRACK_HPP
