#ifndef JASSEQPARSER_HPP
#define JASSEQPARSER_HPP

#include <dolphin/types.h>

namespace JASystem {

class TTrack;
class TSeqCtrl;

class TSeqParser {
public:
	enum OpcodeField {
		OPCODE_COMMAND_BIT = 0x80,
		OPCODE_FAMILY_MASK = 0xF0,
		OPCODE_PARAM_MASK  = 0x0F,
	};

	enum ParamValueEncoding {
		VALUE_REGISTER   = 0x00,
		VALUE_U8         = 0x04,
		VALUE_S16_FROM_8 = 0x08,
		VALUE_S16        = 0x0C,
		VALUE_MASK       = 0x0C,
	};

	enum TimeParamDuration {
		DURATION_IMMEDIATE = 0,
		DURATION_REGISTER  = 1,
		DURATION_U8        = 2,
		DURATION_U16       = 3,
		DURATION_MASK      = 3,
	};

	enum RegParamOperation {
		REG_SET              = 0x00,
		REG_ADD              = 0x01,
		REG_MULTIPLY         = 0x02,
		REG_COMPARE          = 0x03,
		REG_OPERATION_MASK   = 0x03,
		REG_EXTENDED         = 0x09,
		REG_LOAD_TABLE       = 0x0A,
		REG_SUBTRACT         = 0x0B,
		REG_SHIFT_LOGICAL    = 0x10,
		REG_SHIFT_ARITHMETIC = 0x20,
		REG_AND              = 0x30,
		REG_OR               = 0x40,
		REG_XOR              = 0x50,
		REG_NEGATE           = 0x60,
		REG_RANDOM           = 0x90,
	};

	enum ParseResult {
		PARSE_RECHECK = -2,
		PARSE_FINISH  = -1,
		PARSE_WAIT    = 0,
	};

	int mainProc(TTrack* track, TSeqCtrl* ctrl);

private:
	int RegCmd_Process(TTrack* track, int opcode_register,
	                   int arg_count_minus_one);
	int Cmd_Process(TTrack* track, u8 opcode, u16 register_arg_types);
	bool conditionCheck(TTrack* track, u8 condition);

	int cmdNoteOn(TTrack* track, u8 opcode);
	int cmdNoteOff(TTrack* track, u8 opcode);
	int cmdWait(TTrack* track, u8 opcode);
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
