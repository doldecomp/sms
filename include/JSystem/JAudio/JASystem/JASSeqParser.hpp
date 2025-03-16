#ifndef JASSEQPARSER_HPP
#define JASSEQPARSER_HPP

#include "types.h"

namespace JASystem {

class TTrack;
class TSeqCtrl;

class TSeqParser {
public:
	int mainProc(TTrack* track, TSeqCtrl* ctrl);
	int RegCmd_Process(TTrack* track, int param1, int param2);
	int Cmd_Process(TTrack* track, u8 param1, u16 param2);
	bool conditionCheck(TTrack* track, u8 condition);

	int cmdNoteOn(TTrack* track, u8 param);
	int cmdNoteOff(TTrack* track, u8 param);
	int cmdWait(TTrack* track, u8 param);
	int cmdOpenTrack(TTrack* track, u32* param);
	int cmdOpenTrackBros(TTrack* track, u32* param);
	int cmdCall(TTrack* track, u32* param);
	int cmdRet(TTrack* track, u32* param);
	int cmdJmp(TTrack* track, u32* param);
	int cmdLoopS(TTrack* track, u32* param);
	int cmdLoopE(TTrack* track, u32* param);
	int cmdReadPort(TTrack* track, u32* param);
	int cmdWritePort(TTrack* track, u32* param);
	int cmdCheckPortImport(TTrack* track, u32* param);
	int cmdCheckPortExport(TTrack* track, u32* param);
	int cmdWait(TTrack* track, u32* param);
	int cmdConnectName(TTrack* track, u32* param);
	int cmdParentWritePort(TTrack* track, u32* param);
	int cmdChildWritePort(TTrack* track, u32* param);
	int cmdSetLastNote(TTrack* track, u32* param);
	int cmdTimeRelate(TTrack* track, u32* param);
	int cmdSimpleOsc(TTrack* track, u32* param);
	int cmdSimpleEnv(TTrack* track, u32* param);
	int cmdSimpleADSR(TTrack* track, u32* param);
	int cmdTranspose(TTrack* track, u32* param);
	int cmdCloseTrack(TTrack* track, u32* param);
	int cmdOutSwitch(TTrack* track, u32* param);
	int cmdUpdateSync(TTrack* track, u32* param);
	int cmdBusConnect(TTrack* track, u32* param);
	int cmdPauseStatus(TTrack* track, u32* param);
	int cmdSetInterrupt(TTrack* track, u32* param);
	int cmdDisInterrupt(TTrack* track, u32* param);
	int cmdClrI(TTrack* track, u32* param);
	int cmdSetI(TTrack* track, u32* param);
	int cmdRetI(TTrack* track, u32* param);
	int cmdIntTimer(TTrack* track, u32* param);
	int cmdConnectOpen(TTrack* track, u32* param);
	int cmdConnectClose(TTrack* track, u32* param);
	int cmdSyncCPU(TTrack* track, u32* param);
	int cmdFlushAll(TTrack* track, u32* param);
	int cmdFlushRelease(TTrack* track, u32* param);
	int cmdPanPowSet(TTrack* track, u32* param);
	int cmdIIRSet(TTrack* track, u32* param);
	int cmdFIRSet(TTrack* track, u32* param);
	int cmdEXTSet(TTrack* track, u32* param);
	int cmdPanSwSet(TTrack* track, u32* param);
	int cmdOscRoute(TTrack* track, u32* param);
	int cmdIIRCutOff(TTrack* track, u32* param);
	int cmdOscFull(TTrack* track, u32* param);
	int cmdVolumeMode(TTrack* track, u32* param);
	int cmdCheckWave(TTrack* track, u32* param);
	int cmdPrintf(TTrack* track, u32* param);
	int cmdNop(TTrack* track, u32* param);
	int cmdTempo(TTrack* track, u32* param);
	int cmdTimeBase(TTrack* track, u32* param);
	int cmdFinish(TTrack* track, u32* param);

	typedef int (TSeqParser::*CmdFunc)(JASystem::TTrack*, u32*);

	static CmdFunc sCmdPList[];
};

} // namespace JASystem

#endif // JASSEQPARSER_HPP
