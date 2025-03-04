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
	static void mainProc(TTrack* track, TSeqCtrl* ctrl);
	static void cmdNoteOn(TTrack* track, u8 param);
	static void cmdNoteOff(TTrack* track, u8 param);
	static void RegCmd_Process(TTrack* track, int param1, int param2);
	static void Cmd_Process(TTrack* track, u8 param1, u16 param2);
	static bool conditionCheck(TTrack* track, u8 condition);
	static void cmdWait(TTrack* track, u8 param);

	// Command functions with u32* parameter
	static void cmdOpenTrack(TTrack* track, u32* param);
	static void cmdOpenTrackBros(TTrack* track, u32* param);
	static void cmdCall(TTrack* track, u32* param);
	static void cmdRet(TTrack* track, u32* param);
	static void cmdJmp(TTrack* track, u32* param);
	static void cmdLoopS(TTrack* track, u32* param);
	static void cmdLoopE(TTrack* track, u32* param);
	static void cmdReadPort(TTrack* track, u32* param);
	static void cmdWritePort(TTrack* track, u32* param);
	static void cmdCheckPortImport(TTrack* track, u32* param);
	static void cmdCheckPortExport(TTrack* track, u32* param);
	static void cmdWait(TTrack* track, u32* param);
	static void cmdConnectName(TTrack* track, u32* param);
	static void cmdParentWritePort(TTrack* track, u32* param);
	static void cmdChildWritePort(TTrack* track, u32* param);
	static void cmdSetLastNote(TTrack* track, u32* param);
	static void cmdTimeRelate(TTrack* track, u32* param);
	static void cmdSimpleOsc(TTrack* track, u32* param);
	static void cmdSimpleEnv(TTrack* track, u32* param);
	static void cmdSimpleADSR(TTrack* track, u32* param);
	static void cmdTranspose(TTrack* track, u32* param);
	static void cmdCloseTrack(TTrack* track, u32* param);
	static void cmdOutSwitch(TTrack* track, u32* param);
	static void cmdUpdateSync(TTrack* track, u32* param);
	static void cmdBusConnect(TTrack* track, u32* param);
	static void cmdPauseStatus(TTrack* track, u32* param);
	static void cmdSetInterrupt(TTrack* track, u32* param);
	static void cmdDisInterrupt(TTrack* track, u32* param);
	static void cmdClrI(TTrack* track, u32* param);
	static void cmdSetI(TTrack* track, u32* param);
	static void cmdRetI(TTrack* track, u32* param);
	static void cmdIntTimer(TTrack* track, u32* param);
	static void cmdConnectOpen(TTrack* track, u32* param);
	static void cmdConnectClose(TTrack* track, u32* param);
	static void cmdSyncCPU(TTrack* track, u32* param);
	static void cmdFlushAll(TTrack* track, u32* param);
	static void cmdFlushRelease(TTrack* track, u32* param);
	static void cmdPanPowSet(TTrack* track, u32* param);
	static void cmdIIRSet(TTrack* track, u32* param);
	static void cmdFIRSet(TTrack* track, u32* param);
	static void cmdEXTSet(TTrack* track, u32* param);
	static void cmdPanSwSet(TTrack* track, u32* param);
	static void cmdOscRoute(TTrack* track, u32* param);
	static void cmdIIRCutOff(TTrack* track, u32* param);
	static void cmdOscFull(TTrack* track, u32* param);
	static void cmdVolumeMode(TTrack* track, u32* param);
	static void cmdCheckWave(TTrack* track, u32* param);
	static void cmdPrintf(TTrack* track, u32* param);
	static void cmdNop(TTrack* track, u32* param);
	static void cmdTempo(TTrack* track, u32* param);
	static void cmdTimeBase(TTrack* track, u32* param);
	static void cmdFinish(TTrack* track, u32* param);

	static Arglist sCmdPList;
};

} // namespace JASystem

#endif // JASSEQPARSER_HPP
