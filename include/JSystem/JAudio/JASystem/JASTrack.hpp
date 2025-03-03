#ifndef JASTRACK_HPP
#define JASTRACK_HPP

#include "types.h"

namespace JASystem {

namespace Player {
	enum SEQ_PLAYMODE;
	extern unsigned long oRandom;
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
	void initTrack(void* data, unsigned long size, TTrack* parent);
	void initTimed();
	void openTrack(unsigned char param);
	void closeTrack();
	void startTrack(TTrack* parent, unsigned char param1, unsigned char param2,
	                unsigned long param3);
	void startSeq();
	void stopSeq();
	void stopSeqMain();
	void allNoteOff();
	void setSeqData(unsigned char* data, long size, Player::SEQ_PLAYMODE mode);
	void updateSeq(unsigned long param, bool flag);
	void updateTrack(unsigned long param);
	void updateTrackAll();
	void updateTempo();
	void mainProc();
	void noteOn(unsigned char note, long velocity, long length, long delay);
	void noteOff(unsigned char note, unsigned short release);
	void gateOn(unsigned char note, long velocity, long length, long delay);
	void checkNoteStop(long param);
	void releaseChannelAll();
	void flushAll();
	void flushRelease();
	void muteTrack(unsigned char flag);
	void muteChildTracks(unsigned short flags);
	void pauseTrack(unsigned char flag);
	void pauseTrackAll();
	void unPauseTrack(unsigned char flag);
	void unPauseTrackAll();
	void setInterrupt(unsigned short interrupt);
	void tryInterrupt();
	void setInnerMemory(unsigned char param);
	void setBankNumber(unsigned char bank);
	void assignExtBuffer(TOuterParam* buffer);
	void setPanSwitchExt(unsigned char param1, unsigned char param2);
	void setPanSwitchParent(unsigned char param1, unsigned char param2);
	void setTrackExtPanPower(float power);
	void setExtFirFilterD(short* filter);
	void routeTrack(unsigned long route);
	void writePortApp(unsigned long port, unsigned short value);
	void readPortApp(unsigned long port, unsigned short* value);
	void writePortAppDirect(unsigned long port, unsigned short value);
	void readPortAppDirect(unsigned long port, unsigned short* value);
	void checkExportApp(unsigned long port);
	void checkImportApp(unsigned long port);
	void loadTbl(unsigned long param1, unsigned long param2,
	             unsigned long param3);
	void readReg32(unsigned char reg);
	void exchangeRegisterValue(unsigned char reg);
	void readRegDirect(unsigned char reg);
	void writeRegDirect(unsigned char reg, unsigned short value);
	void writeTimeParam(unsigned char param);
	void writeRegParam(unsigned char param);
	void seqTimeToDspTime(long time, unsigned char param);
	void panCalc(float param1, float param2, float param3,
	             unsigned char param4);
	void rootCallback(void* param);
	void updateSyncSw(unsigned char param);
	void incSelfOsc();
	void oscSetupSimple(unsigned char param);
	void oscSetupSimpleEnv(unsigned char param, unsigned long env);
	void oscSetupFull(unsigned char param1, unsigned long param2,
	                  unsigned long param3);
	void oscUpdateParam(unsigned char param, float value);

	unsigned char getBank() const;
	unsigned char getProgram() const;
	float getVolume() const;
	float getPitch() const;
	float getPan() const;
	float getFxmix() const;
	float getDolby() const;
	unsigned char getNoteStatus(unsigned char note);

	static void
	registerTrackCallback(unsigned short (*callback)(TTrack*, unsigned short));
	static TSeqParser* sParser;
	static unsigned char sUpdateSyncMode;
	static unsigned short (*sCallBackFunc)(TTrack*, unsigned short);
	static unsigned long sOscTable;
};

} // namespace JASystem

#endif // JASTRACK_HPP
