#include <JSystem/JAudio/JASystem/JASSeqParser.hpp>
#include <JSystem/JAudio/JASystem/JASTrack.hpp>

namespace JASystem {

TSeqParser::CmdFunc TSeqParser::sCmdPList[] = {
	nullptr,
	&TSeqParser::cmdOpenTrack,
	&TSeqParser::cmdOpenTrackBros,
	nullptr,
	&TSeqParser::cmdCall,
	nullptr,
	&TSeqParser::cmdRet,
	nullptr,
	&TSeqParser::cmdJmp,
	&TSeqParser::cmdLoopS,
	&TSeqParser::cmdLoopE,
	&TSeqParser::cmdReadPort,
	&TSeqParser::cmdWritePort,
	&TSeqParser::cmdCheckPortImport,
	&TSeqParser::cmdCheckPortExport,
	&TSeqParser::cmdWait,
	nullptr,
	&TSeqParser::cmdConnectName,
	&TSeqParser::cmdParentWritePort,
	&TSeqParser::cmdChildWritePort,
	nullptr,
	&TSeqParser::cmdSetLastNote,
	&TSeqParser::cmdTimeRelate,
	&TSeqParser::cmdSimpleOsc,
	&TSeqParser::cmdSimpleEnv,
	&TSeqParser::cmdSimpleADSR,
	&TSeqParser::cmdTranspose,
	&TSeqParser::cmdCloseTrack,
	&TSeqParser::cmdOutSwitch,
	&TSeqParser::cmdUpdateSync,
	&TSeqParser::cmdBusConnect,
	&TSeqParser::cmdPauseStatus,
	&TSeqParser::cmdSetInterrupt,
	&TSeqParser::cmdDisInterrupt,
	&TSeqParser::cmdClrI,
	&TSeqParser::cmdSetI,
	&TSeqParser::cmdRetI,
	&TSeqParser::cmdIntTimer,
	nullptr,
	&TSeqParser::cmdConnectOpen,
	&TSeqParser::cmdConnectClose,
	nullptr,
	&TSeqParser::cmdSyncCPU,
	&TSeqParser::cmdFlushAll,
	&TSeqParser::cmdFlushRelease,
	&TSeqParser::cmdWait,
	&TSeqParser::cmdPanPowSet,
	&TSeqParser::cmdIIRSet,
	&TSeqParser::cmdFIRSet,
	&TSeqParser::cmdEXTSet,
	&TSeqParser::cmdPanSwSet,
	&TSeqParser::cmdOscRoute,
	&TSeqParser::cmdIIRCutOff,
	&TSeqParser::cmdOscFull,
	&TSeqParser::cmdVolumeMode,
	nullptr,
	nullptr,
	nullptr,
	&TSeqParser::cmdCheckWave,
	&TSeqParser::cmdPrintf,
	&TSeqParser::cmdNop,
	&TSeqParser::cmdTempo,
	&TSeqParser::cmdTimeBase,
	&TSeqParser::cmdFinish,
};

// Fabricated
struct Arg_s {
	u16 m00;
	u16 m02;
};

static const Arg_s Arglist[0x40] = {
	{ 0x0000, 0x0000 }, { 0x0002, 0x0008 }, { 0x0002, 0x0008 },
	{ 0x0001, 0x0002 }, { 0x0000, 0x0000 }, { 0x0000, 0x0000 },
	{ 0x0001, 0x0000 }, { 0x0001, 0x0002 }, { 0x0000, 0x0000 },
	{ 0x0001, 0x0001 }, { 0x0000, 0x0000 }, { 0x0002, 0x0000 },
	{ 0x0002, 0x000C }, { 0x0001, 0x0000 }, { 0x0001, 0x0000 },
	{ 0x0001, 0x0003 }, { 0x0002, 0x0005 }, { 0x0002, 0x000C },
	{ 0x0002, 0x000C }, { 0x0000, 0x0000 }, { 0x0001, 0x0000 },
	{ 0x0001, 0x0000 }, { 0x0001, 0x0000 }, { 0x0002, 0x0008 },
	{ 0x0005, 0x0155 }, { 0x0001, 0x0000 }, { 0x0001, 0x0000 },
	{ 0x0001, 0x0000 }, { 0x0001, 0x0001 }, { 0x0002, 0x0004 },
	{ 0x0001, 0x0000 }, { 0x0002, 0x0008 }, { 0x0001, 0x0000 },
	{ 0x0000, 0x0000 }, { 0x0000, 0x0000 }, { 0x0000, 0x0000 },
	{ 0x0002, 0x0004 }, { 0x0000, 0x0000 }, { 0x0000, 0x0000 },
	{ 0x0001, 0x0001 }, { 0x0000, 0x0000 }, { 0x0000, 0x0000 },
	{ 0x0001, 0x0002 }, { 0x0005, 0x0000 }, { 0x0004, 0x0055 },
	{ 0x0001, 0x0002 }, { 0x0001, 0x0002 }, { 0x0003, 0x0000 },
	{ 0x0001, 0x0000 }, { 0x0001, 0x0000 }, { 0x0003, 0x0028 },
	{ 0x0001, 0x0000 }, { 0x0000, 0x0000 }, { 0x0000, 0x0000 },
	{ 0x0000, 0x0000 }, { 0x0000, 0x0000 }, { 0x0000, 0x0000 },
	{ 0x0000, 0x0000 }, { 0x0001, 0x0001 }, { 0x0000, 0x0000 },
	{ 0x0000, 0x0000 }, { 0x0001, 0x0001 }, { 0x0001, 0x0001 },
	{ 0x0000, 0x0000 },
};

int TSeqParser::cmdOpenTrack(TTrack* track, u32* args) { }

int TSeqParser::cmdOpenTrackBros(TTrack* track, u32* args) { }

int TSeqParser::cmdCall(TTrack* track, u32* args) { }

int TSeqParser::cmdRet(TTrack* track, u32* args) { }

int TSeqParser::cmdJmp(TTrack* track, u32* args) { }

int TSeqParser::cmdLoopS(TTrack* track, u32* args) { }

int TSeqParser::cmdLoopE(TTrack* track, u32* args) { }

int TSeqParser::cmdReadPort(TTrack* track, u32* args) { }

int TSeqParser::cmdWritePort(TTrack* track, u32* args) { }

int TSeqParser::cmdCheckPortImport(TTrack* track, u32* args) { }

int TSeqParser::cmdCheckPortExport(TTrack* track, u32* args) { }

int TSeqParser::cmdWait(TTrack* track, u32* args) { }

int TSeqParser::cmdConnectName(TTrack* track, u32* args) { }

int TSeqParser::cmdParentWritePort(TTrack* track, u32* args) { }

int TSeqParser::cmdChildWritePort(TTrack* track, u32* args) { }

int TSeqParser::cmdSetLastNote(TTrack* track, u32* args) { }

int TSeqParser::cmdTimeRelate(TTrack* track, u32* args) { }

int TSeqParser::cmdSimpleOsc(TTrack* track, u32* args) { }

int TSeqParser::cmdSimpleEnv(TTrack* track, u32* args) { }

int TSeqParser::cmdSimpleADSR(TTrack* track, u32* args) { }

int TSeqParser::cmdTranspose(TTrack* track, u32* args) { }

int TSeqParser::cmdCloseTrack(TTrack* track, u32* args) { }

int TSeqParser::cmdOutSwitch(TTrack* track, u32* args) { }

int TSeqParser::cmdUpdateSync(TTrack* track, u32* args) { }

int TSeqParser::cmdBusConnect(TTrack* track, u32* args) { }

int TSeqParser::cmdPauseStatus(TTrack* track, u32* args) { }

int TSeqParser::cmdVolumeMode(TTrack* track, u32* args) { }

int TSeqParser::cmdSetInterrupt(TTrack* track, u32* args) { }

int TSeqParser::cmdDisInterrupt(TTrack* track, u32* args) { }

int TSeqParser::cmdClrI(TTrack* track, u32* args) { }

int TSeqParser::cmdSetI(TTrack* track, u32* args) { }

int TSeqParser::cmdRetI(TTrack* track, u32* args) { }

int TSeqParser::cmdIntTimer(TTrack* track, u32* args) { }

int TSeqParser::cmdConnectOpen(TTrack* track, u32* args) { }

int TSeqParser::cmdConnectClose(TTrack* track, u32* args) { }

int TSeqParser::cmdSyncCPU(TTrack* track, u32* args) { }

int TSeqParser::cmdFlushAll(TTrack* track, u32* args) { }

int TSeqParser::cmdFlushRelease(TTrack* track, u32* args) { }

int TSeqParser::cmdTimeBase(TTrack* track, u32* args) { }

int TSeqParser::cmdTempo(TTrack* track, u32* args) { }

int TSeqParser::cmdFinish(TTrack* track, u32* args) { }

int TSeqParser::cmdNop(TTrack* track, u32* args) { }

int TSeqParser::cmdPanPowSet(TTrack* track, u32* args) { }

int TSeqParser::cmdIIRSet(TTrack* track, u32* args) { }

int TSeqParser::cmdFIRSet(TTrack* track, u32* args) { }

int TSeqParser::cmdEXTSet(TTrack* track, u32* args) { }

int TSeqParser::cmdPanSwSet(TTrack* track, u32* args) { }

int TSeqParser::cmdOscRoute(TTrack* track, u32* args) { }

int TSeqParser::cmdIIRCutOff(TTrack* track, u32* args) { }

int TSeqParser::cmdOscFull(TTrack* track, u32* args) { }

int TSeqParser::cmdCheckWave(TTrack* track, u32* args) { }

int TSeqParser::cmdPrintf(TTrack* track, u32* args) { }

void TSeqParser::Cmd_Process(TTrack* track, u8 param1, u16 param2) { }

void TSeqParser::RegCmd_Process(TTrack* track, int param1, int param2) { }

int TSeqParser::cmdWait(TTrack* track, u8 args) { }

int TSeqParser::cmdNoteOff(TTrack* track, u8 args) { }

int TSeqParser::cmdNoteOn(TTrack* track, u8 args) { }

bool TSeqParser::conditionCheck(TTrack* track, u8 condition) { return false; }

void TSeqParser::mainProc(TTrack* track, TSeqCtrl* ctrl) { }

} // namespace JASystem
