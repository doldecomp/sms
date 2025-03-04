#ifndef JASSEQPARSER_HPP
#define JASSEQPARSER_HPP

#include "types.h"

namespace JASystem {

class TTrack;
class TSeqCtrl;

struct Arglist {
	// Placeholder for Arglist structure
};

class TSeqParser {
public:
	void mainProc(TTrack* track, TSeqCtrl* ctrl);
	void cmdNoteOn(TTrack* track, u8 param);
	void cmdNoteOff(TTrack* track, u8 param);
	void RegCmd_Process(TTrack* track, int param1, int param2);
	void Cmd_Process(TTrack* track, u8 param1, u16 param2);
	bool conditionCheck(TTrack* track, u8 condition);
	void cmdWait(TTrack* track, u8 param);
	void cmdOpenTrack(TTrack* track, u32* param);
	void cmdOpenTrackBros(TTrack* track, u32* param);
	void cmdCall(TTrack* track, u32* param);
	void cmdRet(TTrack* track, u32* param);
	void cmdJmp(TTrack* track, u32* param);
	void cmdLoopS(TTrack* track, u32* param);
	void cmdLoopE(TTrack* track, u32* param);
	void cmdReadPort(TTrack* track, u32* param);
	void cmdWritePort(TTrack* track, u32* param);
	void cmdCheckPortImport(TTrack* track, u32* param);
	void cmdCheckPortExport(TTrack* track, u32* param);
	void cmdWait(TTrack* track, u32* param);
	void cmdConnectName(TTrack* track, u32* param);
	void cmdParentWritePort(TTrack* track, u32* param);
	void cmdChildWritePort(TTrack* track, u32* param);
	void cmdSetLastNote(TTrack* track, u32* param);
	void cmdTimeRelate(TTrack* track, u32* param);
	void cmdSimpleOsc(TTrack* track, u32* param);
	void cmdSimpleEnv(TTrack* track, u32* param);
	void cmdSimpleADSR(TTrack* track, u32* param);
	void cmdTranspose(TTrack* track, u32* param);
	void cmdCloseTrack(TTrack* track, u32* param);
	void cmdOutSwitch(TTrack* track, u32* param);
	void cmdUpdateSync(TTrack* track, u32* param);
	void cmdBusConnect(TTrack* track, u32* param);
	void cmdPauseStatus(TTrack* track, u32* param);
	void cmdSetInterrupt(TTrack* track, u32* param);
	void cmdDisInterrupt(TTrack* track, u32* param);
	void cmdClrI(TTrack* track, u32* param);
	void cmdSetI(TTrack* track, u32* param);
	void cmdRetI(TTrack* track, u32* param);
	void cmdIntTimer(TTrack* track, u32* param);
	void cmdConnectOpen(TTrack* track, u32* param);
	void cmdConnectClose(TTrack* track, u32* param);
	void cmdSyncCPU(TTrack* track, u32* param);
	void cmdFlushAll(TTrack* track, u32* param);
	void cmdFlushRelease(TTrack* track, u32* param);
	void cmdPanPowSet(TTrack* track, u32* param);
	void cmdIIRSet(TTrack* track, u32* param);
	void cmdFIRSet(TTrack* track, u32* param);
	void cmdEXTSet(TTrack* track, u32* param);
	void cmdPanSwSet(TTrack* track, u32* param);
	void cmdOscRoute(TTrack* track, u32* param);
	void cmdIIRCutOff(TTrack* track, u32* param);
	void cmdOscFull(TTrack* track, u32* param);
	void cmdVolumeMode(TTrack* track, u32* param);
	void cmdCheckWave(TTrack* track, u32* param);
	void cmdPrintf(TTrack* track, u32* param);
	void cmdNop(TTrack* track, u32* param);
	void cmdTempo(TTrack* track, u32* param);
	void cmdTimeBase(TTrack* track, u32* param);
	void cmdFinish(TTrack* track, u32* param);

	static Arglist sCmdPList;
};

} // namespace JASystem

#endif // JASSEQPARSER_HPP
