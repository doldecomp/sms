#ifndef JASSEQPARSER_HPP
#define JASSEQPARSER_HPP

#include <dolphin/types.h>

namespace JASystem {

class TTrack;
class TSeqCtrl;

class TSeqParser {
public:
	enum Opcode {
		OP_NOTE_ON           = 0x00,
		OP_WAIT8             = 0x80,
		OP_NOTE_OFF          = 0x80,
		OP_WAIT16            = 0x88,
		OP_TIME_PARAM        = 0x90,
		OP_REG_PARAM         = 0xA0,
		OP_REG_CMD           = 0xB0,
		OP_COMMAND_BASE      = 0xC0,
		OP_OPEN_TRACK        = 0xC1,
		OP_OPEN_TRACK_BROS   = 0xC2,
		OP_CALL              = 0xC4,
		OP_RET               = 0xC6,
		OP_JMP               = 0xC8,
		OP_LOOP_S            = 0xC9,
		OP_LOOP_E            = 0xCA,
		OP_READ_PORT         = 0xCB,
		OP_WRITE_PORT        = 0xCC,
		OP_CHECK_PORT_IMPORT = 0xCD,
		OP_CHECK_PORT_EXPORT = 0xCE,
		OP_WAIT_REG          = 0xCF,
		OP_CONNECT_NAME      = 0xD0,
		OP_PARENT_WRITE_PORT = 0xD1,
		OP_CHILD_WRITE_PORT  = 0xD2,
		OP_SET_LAST_NOTE     = 0xD4,
		OP_TIME_RELATE       = 0xD5,
		OP_SIMPLE_OSC        = 0xD6,
		OP_SIMPLE_ENV        = 0xD7,
		OP_SIMPLE_ADSR       = 0xD8,
		OP_TRANSPOSE         = 0xD9,
		OP_CLOSE_TRACK       = 0xDA,
		OP_OUT_SWITCH        = 0xDB,
		OP_UPDATE_SYNC       = 0xDC,
		OP_BUS_CONNECT       = 0xDD,
		OP_PAUSE_STATUS      = 0xDE,
		OP_SET_INTERRUPT     = 0xDF,
		OP_DIS_INTERRUPT     = 0xE0,
		OP_CLR_I             = 0xE1,
		OP_SET_I             = 0xE2,
		OP_RET_I             = 0xE3,
		OP_INT_TIMER         = 0xE4,
		OP_CONNECT_OPEN      = 0xE5,
		OP_CONNECT_CLOSE     = 0xE6,
		OP_SYNC_CPU          = 0xE7,
		OP_FLUSH_ALL         = 0xE8,
		OP_FLUSH_RELEASE     = 0xE9,
		OP_WAIT24            = 0xEA,
		OP_PAN_POW_SET       = 0xEB,
		OP_IIR_SET           = 0xEC,
		OP_FIR_SET           = 0xED,
		OP_EXT_SET           = 0xEE,
		OP_PAN_SW_SET        = 0xEF,
		OP_OSC_ROUTE         = 0xF0,
		OP_IIR_CUT_OFF       = 0xF1,
		OP_OSC_FULL          = 0xF2,
		OP_VOLUME_MODE       = 0xF3,
		OP_NOTE_OFF_REG      = 0xF9,
		OP_CHECK_WAVE        = 0xFA,
		OP_PRINTF            = 0xFB,
		OP_NOP               = 0xFC,
		OP_TEMPO             = 0xFD,
		OP_TIME_BASE         = 0xFE,
		OP_FINISH            = 0xFF,
	};

	enum OpcodeField {
		OPCODE_COMMAND_BIT = 0x80,
		OPCODE_FAMILY_MASK = 0xF0,
		OPCODE_PARAM_MASK  = 0x0F,
	};

	enum NoteOffFlag {
		NOTE_OFF_VOICE_MASK       = 0x07,
		NOTE_OFF_RELEASE_TIME     = 0x08,
		NOTE_OFF_REG_RELEASE_TIME = 0x80,
	};

	enum RegCmdFlag {
		REG_CMD_OPCODE_REGISTER          = 0x08,
		REG_CMD_ARG_COUNT_MINUS_ONE_MASK = 0x07,
		REG_CMD_REGISTER_ARG             = 0x80,
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

	enum CommandArgType {
		ARG_U8        = 0,
		ARG_U16       = 1,
		ARG_U24       = 2,
		ARG_REGISTER  = 3,
		ARG_TYPE_MASK = 3,
	};

	int mainProc(TTrack* track, TSeqCtrl* ctrl);
	int RegCmd_Process(TTrack* track, int opcode_register,
	                   int arg_count_minus_one);
	int Cmd_Process(TTrack* track, u8 opcode, u16 register_arg_types);
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
