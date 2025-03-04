#include "JSystem/JAudio/JASystem/JASSeqParser.hpp"

namespace JASystem {

void TSeqParser::mainProc(TTrack* track, TSeqCtrl* ctrl) { }

void TSeqParser::cmdNoteOn(TTrack* track, u8 param) { }

void TSeqParser::cmdNoteOff(TTrack* track, u8 param) { }

void TSeqParser::RegCmd_Process(TTrack* track, int param1, int param2) { }

void TSeqParser::Cmd_Process(TTrack* track, u8 param1, u16 param2) { }

bool TSeqParser::conditionCheck(TTrack* track, u8 condition) { return false; }

void TSeqParser::cmdWait(TTrack* track, u8 param) { }

void TSeqParser::cmdOpenTrack(TTrack* track, u32* param) { }

void TSeqParser::cmdOpenTrackBros(TTrack* track, u32* param) { }

void TSeqParser::cmdCall(TTrack* track, u32* param) { }

void TSeqParser::cmdRet(TTrack* track, u32* param) { }

void TSeqParser::cmdJmp(TTrack* track, u32* param) { }

void TSeqParser::cmdLoopS(TTrack* track, u32* param) { }

void TSeqParser::cmdLoopE(TTrack* track, u32* param) { }

void TSeqParser::cmdReadPort(TTrack* track, u32* param) { }

void TSeqParser::cmdWritePort(TTrack* track, u32* param) { }

void TSeqParser::cmdCheckPortImport(TTrack* track, u32* param) { }

void TSeqParser::cmdCheckPortExport(TTrack* track, u32* param) { }

void TSeqParser::cmdWait(TTrack* track, u32* param) { }

void TSeqParser::cmdConnectName(TTrack* track, u32* param) { }

void TSeqParser::cmdParentWritePort(TTrack* track, u32* param) { }

void TSeqParser::cmdChildWritePort(TTrack* track, u32* param) { }

void TSeqParser::cmdSetLastNote(TTrack* track, u32* param) { }

void TSeqParser::cmdTimeRelate(TTrack* track, u32* param) { }

void TSeqParser::cmdSimpleOsc(TTrack* track, u32* param) { }

void TSeqParser::cmdSimpleEnv(TTrack* track, u32* param) { }

void TSeqParser::cmdSimpleADSR(TTrack* track, u32* param) { }

void TSeqParser::cmdTranspose(TTrack* track, u32* param) { }

void TSeqParser::cmdCloseTrack(TTrack* track, u32* param) { }

void TSeqParser::cmdOutSwitch(TTrack* track, u32* param) { }

void TSeqParser::cmdUpdateSync(TTrack* track, u32* param) { }

void TSeqParser::cmdBusConnect(TTrack* track, u32* param) { }

void TSeqParser::cmdPauseStatus(TTrack* track, u32* param) { }

void TSeqParser::cmdSetInterrupt(TTrack* track, u32* param) { }

void TSeqParser::cmdDisInterrupt(TTrack* track, u32* param) { }

void TSeqParser::cmdClrI(TTrack* track, u32* param) { }

void TSeqParser::cmdSetI(TTrack* track, u32* param) { }

void TSeqParser::cmdRetI(TTrack* track, u32* param) { }

void TSeqParser::cmdIntTimer(TTrack* track, u32* param) { }

void TSeqParser::cmdConnectOpen(TTrack* track, u32* param) { }

void TSeqParser::cmdConnectClose(TTrack* track, u32* param) { }

void TSeqParser::cmdSyncCPU(TTrack* track, u32* param) { }

void TSeqParser::cmdFlushAll(TTrack* track, u32* param) { }

void TSeqParser::cmdFlushRelease(TTrack* track, u32* param) { }

void TSeqParser::cmdPanPowSet(TTrack* track, u32* param) { }

void TSeqParser::cmdIIRSet(TTrack* track, u32* param) { }

void TSeqParser::cmdFIRSet(TTrack* track, u32* param) { }

void TSeqParser::cmdEXTSet(TTrack* track, u32* param) { }

void TSeqParser::cmdPanSwSet(TTrack* track, u32* param) { }

void TSeqParser::cmdOscRoute(TTrack* track, u32* param) { }

void TSeqParser::cmdIIRCutOff(TTrack* track, u32* param) { }

void TSeqParser::cmdOscFull(TTrack* track, u32* param) { }

void TSeqParser::cmdVolumeMode(TTrack* track, u32* param) { }

void TSeqParser::cmdCheckWave(TTrack* track, u32* param) { }

void TSeqParser::cmdPrintf(TTrack* track, u32* param) { }

void TSeqParser::cmdNop(TTrack* track, u32* param) { }

void TSeqParser::cmdTempo(TTrack* track, u32* param) { }

void TSeqParser::cmdTimeBase(TTrack* track, u32* param) { }

void TSeqParser::cmdFinish(TTrack* track, u32* param) { }

Arglist TSeqParser::sCmdPList;

} // namespace JASystem
