#ifndef JASTRACK_HPP
#define JASTRACK_HPP

#include "types.h"

namespace JASystem {

namespace Player {
	enum SEQ_PLAYMODE {};
	extern u32 oRandom;
} // namespace Player

class TSeqCtrl;
class TSeqParser;

class TTrack {
public:
	struct TOuterParam;
	struct AInnerParam_;
	struct MoveParam_;

	TTrack();
	~TTrack();

	void reset();
	void initTrack(void* data, u32 size, TTrack* parent);
	void initTimed();
	void openTrack(u8 param);
	void closeTrack();
	void startTrack(TTrack* parent, u8 param1, u8 param2, u32 param3);
	void startSeq();
	void stopSeq();
	void stopSeqMain();
	void allNoteOff();
	void setSeqData(u8* data, s32 size, Player::SEQ_PLAYMODE mode);
	void updateSeq(u32 param, bool flag);
	void updateTrack(u32 param);
	void updateTrackAll();
	void updateTempo();
	void mainProc();
	void noteOn(u8 note, s32 velocity, s32 length, s32 delay);
	void noteOff(u8 note, u16 release);
	void gateOn(u8 note, s32 velocity, s32 length, s32 delay);
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
	void assignExtBuffer(TOuterParam* buffer);
	void setPanSwitchExt(u8 param1, u8 param2);
	void setPanSwitchParent(u8 param1, u8 param2);
	void setTrackExtPanPower(f32 power);
	void setExtFirFilterD(s16* filter);
	void routeTrack(u32 route);
	void writePortApp(u32 port, u16 value);
	void readPortApp(u32 port, u16* value);
	void writePortAppDirect(u32 port, u16 value);
	void readPortAppDirect(u32 port, u16* value);
	void checkExportApp(u32 port);
	void checkImportApp(u32 port);
	void loadTbl(u32 param1, u32 param2, u32 param3);
	void readReg32(u8 reg);
	void exchangeRegisterValue(u8 reg);
	void readRegDirect(u8 reg);
	void writeRegDirect(u8 reg, u16 value);
	void writeTimeParam(u8 param);
	void writeRegParam(u8 param);
	void seqTimeToDspTime(s32 time, u8 param);
	void panCalc(f32 param1, f32 param2, f32 param3, u8 param4);
	void rootCallback(void* param);
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

	static void registerTrackCallback(u16 (*callback)(TTrack*, u16));
	static TSeqParser* sParser;
	static u8 sUpdateSyncMode;
	static u16 (*sCallBackFunc)(TTrack*, u16);
	static u32 sOscTable;
};

} // namespace JASystem

#endif // JASTRACK_HPP
