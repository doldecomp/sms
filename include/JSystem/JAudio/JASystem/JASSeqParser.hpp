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
	static void cmdNoteOn(TTrack* track, unsigned char param);
	static void cmdNoteOff(TTrack* track, unsigned char param);
	static void RegCmd_Process(TTrack* track, int param1, int param2);
	static void Cmd_Process(TTrack* track, unsigned char param1,
	                        unsigned short param2);
	static bool conditionCheck(TTrack* track, unsigned char condition);
	static void cmdWait(TTrack* track, unsigned char param);

	// Command functions with unsigned long* parameter
	static void cmdOpenTrack(TTrack* track, unsigned long* param);
	static void cmdOpenTrackBros(TTrack* track, unsigned long* param);
	static void cmdCall(TTrack* track, unsigned long* param);
	static void cmdRet(TTrack* track, unsigned long* param);
	static void cmdJmp(TTrack* track, unsigned long* param);
	static void cmdLoopS(TTrack* track, unsigned long* param);
	static void cmdLoopE(TTrack* track, unsigned long* param);
	static void cmdReadPort(TTrack* track, unsigned long* param);
	static void cmdWritePort(TTrack* track, unsigned long* param);
	static void cmdCheckPortImport(TTrack* track, unsigned long* param);
	static void cmdCheckPortExport(TTrack* track, unsigned long* param);
	static void cmdWait(TTrack* track, unsigned long* param);
	static void cmdConnectName(TTrack* track, unsigned long* param);
	static void cmdParentWritePort(TTrack* track, unsigned long* param);
	static void cmdChildWritePort(TTrack* track, unsigned long* param);
	static void cmdSetLastNote(TTrack* track, unsigned long* param);
	static void cmdTimeRelate(TTrack* track, unsigned long* param);
	static void cmdSimpleOsc(TTrack* track, unsigned long* param);
	static void cmdSimpleEnv(TTrack* track, unsigned long* param);
	static void cmdSimpleADSR(TTrack* track, unsigned long* param);
	static void cmdTranspose(TTrack* track, unsigned long* param);
	static void cmdCloseTrack(TTrack* track, unsigned long* param);
	static void cmdOutSwitch(TTrack* track, unsigned long* param);
	static void cmdUpdateSync(TTrack* track, unsigned long* param);
	static void cmdBusConnect(TTrack* track, unsigned long* param);
	static void cmdPauseStatus(TTrack* track, unsigned long* param);
	static void cmdSetInterrupt(TTrack* track, unsigned long* param);
	static void cmdDisInterrupt(TTrack* track, unsigned long* param);
	static void cmdClrI(TTrack* track, unsigned long* param);
	static void cmdSetI(TTrack* track, unsigned long* param);
	static void cmdRetI(TTrack* track, unsigned long* param);
	static void cmdIntTimer(TTrack* track, unsigned long* param);
	static void cmdConnectOpen(TTrack* track, unsigned long* param);
	static void cmdConnectClose(TTrack* track, unsigned long* param);
	static void cmdSyncCPU(TTrack* track, unsigned long* param);
	static void cmdFlushAll(TTrack* track, unsigned long* param);
	static void cmdFlushRelease(TTrack* track, unsigned long* param);
	static void cmdPanPowSet(TTrack* track, unsigned long* param);
	static void cmdIIRSet(TTrack* track, unsigned long* param);
	static void cmdFIRSet(TTrack* track, unsigned long* param);
	static void cmdEXTSet(TTrack* track, unsigned long* param);
	static void cmdPanSwSet(TTrack* track, unsigned long* param);
	static void cmdOscRoute(TTrack* track, unsigned long* param);
	static void cmdIIRCutOff(TTrack* track, unsigned long* param);
	static void cmdOscFull(TTrack* track, unsigned long* param);
	static void cmdVolumeMode(TTrack* track, unsigned long* param);
	static void cmdCheckWave(TTrack* track, unsigned long* param);
	static void cmdPrintf(TTrack* track, unsigned long* param);
	static void cmdNop(TTrack* track, unsigned long* param);
	static void cmdTempo(TTrack* track, unsigned long* param);
	static void cmdTimeBase(TTrack* track, unsigned long* param);
	static void cmdFinish(TTrack* track, unsigned long* param);

	static Arglist sCmdPList;
};

} // namespace JASystem

#endif // JASSEQPARSER_HPP
