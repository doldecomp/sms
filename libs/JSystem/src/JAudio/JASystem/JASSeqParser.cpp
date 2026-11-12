#include <JSystem/JAudio/JASystem/JASSeqParser.hpp>
#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASPlayer_impl.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <dolphin/os.h>
#include <stdint.h>

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
	&TSeqParser::cmdConnectOpen,
	&TSeqParser::cmdConnectClose,
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

enum Condition {
	COND_ALWAYS      = 0,
	COND_ZERO        = 1,
	COND_NONZERO     = 2,
	COND_ONE         = 3,
	COND_NEGATIVE    = 4,
	COND_NONNEGATIVE = 5,
	COND_MASK        = 0xF,
};

enum CommandResult {
	RC_CONTINUE         = 0,
	RC_WAIT             = 1,
	RC_RETURN_INTERRUPT = 2,
	RC_FINISH           = 3,
};

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

enum BranchFlag {
	BRANCH_REGISTER_TABLE_BASE = 0x20,
	BRANCH_TABLE_LOOKUP        = 0x40,
	BRANCH_REGISTER_TARGET     = 0x80,
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

enum CommandArgType {
	ARG_U8        = 0,
	ARG_U16       = 1,
	ARG_U24       = 2,
	ARG_REGISTER  = 3,
	ARG_TYPE_MASK = 3,
};

// Fabricated
struct Arg_s {
	u16 mArgCount;
	u16 mArgTypes;
};

static const Arg_s Arglist[0x40] = {
	{ 0, 0 },
	{ 2, ARG_U8 | (ARG_U24 << 2) },
	{ 2, ARG_U8 | (ARG_U24 << 2) },
	{ 1, ARG_U24 },
	{ 0, 0 },
	{ 0, 0 },
	{ 1, ARG_U8 },
	{ 1, ARG_U24 },
	{ 0, 0 },
	{ 1, ARG_U16 },
	{ 0, 0 },
	{ 2, ARG_U8 | (ARG_U8 << 2) },
	{ 2, ARG_U8 | (ARG_REGISTER << 2) },
	{ 1, ARG_U8 },
	{ 1, ARG_U8 },
	{ 1, ARG_REGISTER },
	{ 2, ARG_U16 | (ARG_U16 << 2) },
	{ 2, ARG_U8 | (ARG_REGISTER << 2) },
	{ 2, ARG_U8 | (ARG_REGISTER << 2) },
	{ 0, 0 },
	{ 1, ARG_U8 },
	{ 1, ARG_U8 },
	{ 1, ARG_U8 },
	{ 2, ARG_U8 | (ARG_U24 << 2) },
	{ 5, ARG_U16 | (ARG_U16 << 2) | (ARG_U16 << 4) | (ARG_U16 << 6)
	         | (ARG_U16 << 8) },
	{ 1, ARG_U8 },
	{ 1, ARG_U8 },
	{ 1, ARG_U8 },
	{ 1, ARG_U16 },
	{ 2, ARG_U8 | (ARG_U16 << 2) },
	{ 1, ARG_U8 },
	{ 2, ARG_U8 | (ARG_U24 << 2) },
	{ 1, ARG_U8 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 2, ARG_U8 | (ARG_U16 << 2) },
	{ 0, 0 },
	{ 0, 0 },
	{ 1, ARG_U16 },
	{ 0, 0 },
	{ 0, 0 },
	{ 1, ARG_U24 },
	{ 5,
	  ARG_U8 | (ARG_U8 << 2) | (ARG_U8 << 4) | (ARG_U8 << 6) | (ARG_U8 << 8) },
	{ 4, ARG_U16 | (ARG_U16 << 2) | (ARG_U16 << 4) | (ARG_U16 << 6) },
	{ 1, ARG_U24 },
	{ 1, ARG_U24 },
	{ 3, ARG_U8 | (ARG_U8 << 2) | (ARG_U8 << 4) },
	{ 1, ARG_U8 },
	{ 1, ARG_U8 },
	{ 3, ARG_U8 | (ARG_U24 << 2) | (ARG_U24 << 4) },
	{ 1, ARG_U8 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 1, ARG_U16 },
	{ 0, 0 },
	{ 0, 0 },
	{ 1, ARG_U16 },
	{ 1, ARG_U16 },
	{ 0, 0 },
};

int TSeqParser::cmdOpenTrack(TTrack* track, u32* args)
{
	u32 flags            = args[0];
	u32 entryPointOffset = args[1];
	u8 childIndex        = flags & 0xF;
	u8 trackMode         = (flags >> 6)
	               & (TTrack::TRACK_MODE_INDEPENDENT_PARAMS
	                  | TTrack::TRACK_MODE_INIT_REGISTERS);
	if (flags & 0x20)
		trackMode = TTrack::TRACK_MODE_PARENT_CHANNELS;

	TTrack* child = track->openTrack(childIndex);
	child->startTrack(track, childIndex, trackMode, entryPointOffset);
	return RC_CONTINUE;
}

int TSeqParser::cmdOpenTrackBros(TTrack* track, u32* args)
{
	TTrack* parent = track->getParent();
	if (!parent)
		return RC_CONTINUE;

	u32 flags            = args[0];
	u32 entryPointOffset = args[1];
	u8 childIndex        = flags & 0xF;
	u8 trackMode         = (flags >> 6)
	               & (TTrack::TRACK_MODE_INDEPENDENT_PARAMS
	                  | TTrack::TRACK_MODE_INIT_REGISTERS);
	if (flags & 0x20)
		trackMode = TTrack::TRACK_MODE_PARENT_CHANNELS;

	TTrack* sibling = parent->openTrack(childIndex);
	sibling->startTrack(parent, childIndex, trackMode, entryPointOffset);
	return RC_CONTINUE;
}

int TSeqParser::cmdCall(TTrack* track, u32* args)
{
	u8 flags = track->mSeqCtrl.readByte();

	u32 offset;
	if (flags & BRANCH_REGISTER_TARGET) {
		offset = track->readRegDirect(track->mSeqCtrl.readByte());

		if (flags & BRANCH_TABLE_LOOKUP) {
			u32 offs;
			if (flags & BRANCH_REGISTER_TABLE_BASE)
				offs = track->readRegDirect(track->mSeqCtrl.readByte());
			else
				offs = track->mSeqCtrl.read24();

			offset = track->mSeqCtrl.get24(offs + offset * 3);
		}
	} else {
		offset = track->mSeqCtrl.read24();
	}

	if (conditionCheck(track, flags))
		track->mSeqCtrl.call(offset);

	return RC_CONTINUE;
}

int TSeqParser::cmdRet(TTrack* track, u32* args)
{
	if (conditionCheck(track, args[0])) {
		bool doFinish;
		if (track->mSeqCtrl.mStackDepth == 0)
			doFinish = false;
		else
			doFinish = track->mSeqCtrl.ret();

		if (!doFinish)
			return RC_FINISH;
	}

	return RC_CONTINUE;
}

int TSeqParser::cmdJmp(TTrack* track, u32* args)
{
	void* trackptr = nullptr;
	u32 offset;
	u8 flags = track->mSeqCtrl.readByte();

	if (flags & BRANCH_REGISTER_TARGET) {
		u8 regId = track->mSeqCtrl.readByte();

		if (flags & BRANCH_TABLE_LOOKUP) {
			u32 otherOffset = track->readRegDirect(regId);
			u32 offs;
			if (flags & BRANCH_REGISTER_TABLE_BASE) {
				offs = track->readRegDirect(track->mSeqCtrl.readByte());
			} else {
				offs = track->mSeqCtrl.read24();
			}
			offset = track->mSeqCtrl.get24(offs + otherOffset * 3);
		} else {
			if (regId >= TRegisterParam::REG_ADDRESS0
			    && TRegisterParam::REG_ADDRESS3 >= regId)
				trackptr = (void*)track->readReg32(regId);
			else
				offset = track->readReg32(regId);
		}
	} else {
		offset = track->mSeqCtrl.read24();
	}

	if (conditionCheck(track, flags)) {
		if (!trackptr)
			track->mSeqCtrl.jump(offset);
		else
			track->mSeqCtrl.init(trackptr, 0);
	}
	return RC_CONTINUE;
}

int TSeqParser::cmdLoopS(TTrack* track, u32* args)
{
	u32 timer = args[0];
	track->mSeqCtrl.loopS(timer);
	return RC_CONTINUE;
}

int TSeqParser::cmdLoopE(TTrack* track, u32* args)
{
	if (!track->mSeqCtrl.loopE())
		return RC_CONTINUE;
	else
		return RC_CONTINUE;
}

int TSeqParser::cmdReadPort(TTrack* track, u32* args)
{
	track->writeRegDirect(args[1], track->mTrackPort.readImport(args[0]));
	return RC_CONTINUE;
}

int TSeqParser::cmdWritePort(TTrack* track, u32* args)
{
	track->mTrackPort.writeExport(args[0], args[1]);
	return RC_CONTINUE;
}

int TSeqParser::cmdCheckPortImport(TTrack* track, u32* args)
{
	track->writeRegDirect(TRegisterParam::REG_FLAG,
	                      track->checkImport(args[0]));
	return RC_CONTINUE;
}

int TSeqParser::cmdCheckPortExport(TTrack* track, u32* args)
{
	track->writeRegDirect(TRegisterParam::REG_FLAG,
	                      track->checkExport(args[0]));
	return RC_CONTINUE;
}

int TSeqParser::cmdWait(TTrack* track, u32* args)
{
	s32 time = args[0];
	track->mSeqCtrl.wait(time);
	return time != 0 ? RC_WAIT : RC_CONTINUE;
}

int TSeqParser::cmdConnectName(TTrack* track, u32* args)
{
	track->mRegistrationId = args[0] << 16 | args[1];
	return RC_CONTINUE;
}

int TSeqParser::cmdParentWritePort(TTrack* track, u32* args)
{
	track->mParent->writePortAppDirect(args[0] & 0xf, args[1]);
	return RC_CONTINUE;
}

int TSeqParser::cmdChildWritePort(TTrack* track, u32* args)
{
	track->getChild(args[0] >> 4)->writePortAppDirect(args[0] & 0xf, args[1]);
	return RC_CONTINUE;
}

int TSeqParser::cmdSetLastNote(TTrack* track, u32* args)
{
	u32 key = args[0];
	key += track->mEffectiveTranspose;
	track->mNoteMgr.setLastNote(key);
	return RC_CONTINUE;
}

int TSeqParser::cmdTimeRelate(TTrack* track, u32* args)
{
	track->mTimeRelate = args[0] ? 1 : 0;
	return RC_CONTINUE;
}

int TSeqParser::cmdSimpleOsc(TTrack* track, u32* args)
{
	track->oscSetupSimple(args[0]);
	return RC_CONTINUE;
}

int TSeqParser::cmdSimpleEnv(TTrack* track, u32* args)
{
	track->oscSetupSimpleEnv(args[0], args[1]);
	return RC_CONTINUE;
}

int TSeqParser::cmdSimpleADSR(TTrack* track, u32* args)
{
	s16 realArgs[5];
	for (u8 i = 0; i < 5; ++i)
		realArgs[i] = args[i];

	track->mOscData[0]           = Player::sAdsrDef;
	track->mOscData[0].mAdsTable = track->mAdsTable;
	track->mOscData[0].mRelTable = track->mRelTable;

	track->mAdsTable[1] = realArgs[0];
	track->mAdsTable[4] = realArgs[1];
	track->mAdsTable[7] = realArgs[2];
	track->mAdsTable[8] = realArgs[3];
	track->mRelTable[1] = realArgs[4];
	return RC_CONTINUE;
}

int TSeqParser::cmdTranspose(TTrack* track, u32* args)
{
	track->setTranspose(args[0]);
	track->mEffectiveTranspose = track->mTranspose;
	if (track->getParent())
		track->mEffectiveTranspose += track->getParent()->mTranspose;
	return RC_CONTINUE;
}

int TSeqParser::cmdCloseTrack(TTrack* track, u32* args)
{
	u8 i = args[0];

	TTrack* child = track->getChild(i);
	if (!child)
		return RC_CONTINUE;

	child->closeTrack();
	track->mChildren[i] = nullptr;

	return RC_CONTINUE;
}

int TSeqParser::cmdOutSwitch(TTrack* track, u32* args)
{
	TTrack::TOuterParam* outer = track->getOuterParam();
	if (outer) {
		outer->setOuterSwitch(args[0]);
		outer->setOuterUpdate(0xffff);
	}
	return RC_CONTINUE;
}

int TSeqParser::cmdUpdateSync(TTrack* track, u32* args)
{
	track->updateTrack(args[0]);
	return RC_CONTINUE;
}

int TSeqParser::cmdBusConnect(TTrack* track, u32* args)
{
	u32 configIdx = args[0];
	if (configIdx < 6)
		track->mChannelMgr.mMixConfig[configIdx] = args[1];
	return RC_CONTINUE;
}

int TSeqParser::cmdPauseStatus(TTrack* track, u32* args)
{
	track->setPauseStatus(args[0]);
	return RC_CONTINUE;
}

int TSeqParser::cmdVolumeMode(TTrack* track, u32* args)
{
	track->setVolumeMode(args[0]);
	return RC_CONTINUE;
}

int TSeqParser::cmdSetInterrupt(TTrack* track, u32* args)
{
	track->mIntrMgr.setIntr(args[0], track->mSeqCtrl.getBase() + args[1]);
	return RC_CONTINUE;
}

int TSeqParser::cmdDisInterrupt(TTrack* track, u32* args)
{
	track->mIntrMgr.resetInter(args[0]);
	return RC_CONTINUE;
}

int TSeqParser::cmdClrI(TTrack* track, u32* args)
{
	track->mIntrMgr.enable();
	track->mSeqCtrl.clrIntr();
	return RC_CONTINUE;
}

int TSeqParser::cmdSetI(TTrack* track, u32* args)
{
	track->mIntrMgr.disable();
	return RC_CONTINUE;
}

int TSeqParser::cmdRetI(TTrack* track, u32* args)
{
	track->mIntrMgr.enable();
	track->mSeqCtrl.retIntr();
	return RC_RETURN_INTERRUPT;
}

int TSeqParser::cmdIntTimer(TTrack* track, u32* args)
{
	track->mIntrMgr.setTimer(args[0], args[1]);
	return RC_CONTINUE;
}

int TSeqParser::cmdConnectOpen(TTrack* track, u32* args)
{
	TrackMgr::registTrack(track->mRegistrationId, track);
	track->mRegistered = 1;
	return RC_CONTINUE;
}

int TSeqParser::cmdConnectClose(TTrack* track, u32* args)
{
	if (track->mRegistered) {
		TrackMgr::unRegistTrack(track);
		track->mRegistered = 0;
	}
	return RC_CONTINUE;
}

int TSeqParser::cmdSyncCPU(TTrack* track, u32* args)
{
	u16 retval = 0xffff;
	if (TTrack::sCallBackFunc)
		retval = TTrack::sCallBackFunc(track, args[0]);
	track->writeRegDirect(TRegisterParam::REG_FLAG, retval);
	return RC_CONTINUE;
}

int TSeqParser::cmdFlushAll(TTrack* track, u32* args)
{
	track->flushAll();
	return RC_CONTINUE;
}

int TSeqParser::cmdFlushRelease(TTrack* track, u32* args)
{
	track->mChannelMgr.stopAllRelease();
	return RC_CONTINUE;
}

int TSeqParser::cmdTimeBase(TTrack* track, u32* args)
{
	track->mTimeBase = args[0];
	if (!track->getParent())
		track->updateTempo();
	return RC_CONTINUE;
}

int TSeqParser::cmdTempo(TTrack* track, u32* args)
{
	track->mTempo = args[0];
	if (!track->getParent())
		track->updateTempo();
	else
		track->mOverrideParentTempo = true;
	return RC_CONTINUE;
}

int TSeqParser::cmdFinish(TTrack* track, u32* args)
{
	u32 mask = 0;
	for (u8 i = 0; i < TTrack::TIMED_COUNT; ++i) {
		TTrack::MoveParam_* param = &track->mTimedParam.mMoveParams[i];
		if (param->mTicksRemaining > 0.0f) {
			param->mCurrentValue += param->mStepPerTick;
			param->mTicksRemaining -= 1.0f;
			if (i <= TTrack::TIMED_DIST_FILTER || i >= TTrack::TIMED_OSC1_BIAS)
				mask = mask | 1 << i;
			else
				track->oscUpdateParam(i, param->mCurrentValue);
		}
	}
	track->updateSeq(mask, true);
	return RC_FINISH;
}

int TSeqParser::cmdNop(TTrack* track, u32* args) { return RC_CONTINUE; }

int TSeqParser::cmdPanPowSet(TTrack* track, u32* args)
{
	track->setPanPower(TRegisterParam::PAN_POWER_BANK, args[0]);
	track->setPanPower(TRegisterParam::PAN_POWER_OSC, args[1]);
	track->setPanPower(TRegisterParam::PAN_POWER_TRACK, args[2]);
	track->setPanPower(TRegisterParam::PAN_POWER_EXT, args[3] * 327.67f);
	track->setPanPower(TRegisterParam::PAN_POWER_PARENT, args[4] * 327.67f);
	return RC_CONTINUE;
}

int TSeqParser::cmdIIRSet(TTrack* track, u32* args)
{
	for (u8 i = 0; i < 4; ++i) {
		s16 target              = args[i];
		TTrack::MoveParam_* iir = &track->mTimedParam.mInnerParam.mIIRs[i];
		iir->mTargetValue       = target / 32768.0f;
		iir->mCurrentValue      = iir->mTargetValue;
		iir->mStepPerTick       = 0.0f;
		iir->mTicksRemaining    = 1.0f;
	}
	return RC_CONTINUE;
}

int TSeqParser::cmdFIRSet(TTrack* track, u32* args)
{
	track->setExtFirFilterD((s16*)track->mSeqCtrl.getAddr(args[0]));
	return RC_CONTINUE;
}

int TSeqParser::cmdEXTSet(TTrack* track, u32* args)
{
	u32 offset                      = args[0];
	u8* outerParamAddr              = track->mSeqCtrl.getAddr(offset);
	TTrack::TOuterParam* outerParam = (TTrack::TOuterParam*)outerParamAddr;
	outerParam->initExtBuffer();
	track->assignExtBuffer(outerParam);
	return RC_CONTINUE;
}

int TSeqParser::cmdPanSwSet(TTrack* track, u32* args)
{
	u8 calcTypes[] = {
		TTrack::MIX_MODE_LOCAL,  TTrack::MIX_MODE_LOCAL,
		TTrack::MIX_MODE_LOCAL,  TTrack::MIX_MODE_SOURCE,
		TTrack::MIX_MODE_SOURCE, TTrack::MIX_MODE_BLEND,
		TTrack::MIX_MODE_BLEND,
	};
	u8 parentCalcTypes[] = {
		TTrack::MIX_MODE_LOCAL, TTrack::MIX_MODE_SOURCE, TTrack::MIX_MODE_BLEND,
		TTrack::MIX_MODE_LOCAL, TTrack::MIX_MODE_BLEND,  TTrack::MIX_MODE_LOCAL,
		TTrack::MIX_MODE_BLEND,
	};

	for (u8 i = MIX_PAN; i < MIX_COUNT; i++) {
		track->mExternalMixModes[i]      = calcTypes[args[i] >> 5];
		track->mParentMixModes[i]        = parentCalcTypes[args[i] >> 5];
		track->mChannelMgr.mCalcTypes[i] = args[i] & 0x1F;
		track->mPendingUpdateFlags |= TTrack::UPDATE_PAN;
	}

	return RC_CONTINUE;
}

int TSeqParser::cmdOscRoute(TTrack* track, u32* args)
{
	s32 arg = args[0];
	u32 i   = (arg >> 4) & 0xF;
	s32 v   = arg & 0xF;

	track->mOscMode[i] = v;
	if (v == TTrack::OSC_ROUTE_TRACK)
		track->mOscillators[i].initStart();
	return RC_CONTINUE;
}

int TSeqParser::cmdIIRCutOff(TTrack* track, u32* args)
{
	u8 iirTableIdx = args[0];
	for (u8 i = 0; i < 4; i++) {
		s16* table  = JASystem::Player::CUTOFF_TO_IIR_TABLE[iirTableIdx];
		u8 iirIndex = TTrack::TIMED_IIR0 + i;
		TTrack::MoveParam_* iir = &track->mTimedParam.mMoveParams[iirIndex];
		iir->mTargetValue       = table[i] / (32768.0f - 1.0f);
		iir->mCurrentValue      = iir->mTargetValue;
		iir->mStepPerTick       = 0.0f;
		iir->mTicksRemaining    = 1.0f;
	}
	return RC_CONTINUE;
}

int TSeqParser::cmdOscFull(TTrack* track, u32* args)
{
	track->oscSetupFull(args[0], args[1], args[2]);
	return RC_CONTINUE;
}

int TSeqParser::cmdCheckWave(TTrack* track, u32* args)
{
	track->writeRegDirect(TRegisterParam::REG_FLAG, 0);
	return RC_CONTINUE;
}

int TSeqParser::cmdPrintf(TTrack* track, u32* args)
{
	char formatString[128];
	u8 argSources[4];
	uintptr_t printArgs[4];
	u32 count = 0;

	u32 i;
	for (i = 0; i < 128; ++i) {
		formatString[i] = track->mSeqCtrl.readByte();
		if (!formatString[i])
			break;

		if (formatString[i] == '\\') {
			formatString[i] = track->mSeqCtrl.readByte();
			if (!formatString[i])
				break;

			switch (formatString[i]) {
			case 'n':
				formatString[i] = '\r';
				continue;
			default:
				continue;
			}
		}

		if (formatString[i] == '%') {
			++i;
			formatString[i] = track->mSeqCtrl.readByte();
			if (!formatString[i])
				break;

			switch (formatString[i]) {
			case 'd':
				argSources[count] = 0;
				break;
			case 'x':
				argSources[count] = 1;
				break;
			case 's':
				argSources[count] = 2;
				break;
			case 'r':
				argSources[count] = 3;
				formatString[i]   = 'd';
				break;
			case 'R':
				argSources[count] = 4;
				formatString[i]   = 'x';
				break;
			case 't':
				argSources[count] = 5;
				formatString[i]   = 'x';
				break;
			}
			++count;
		}
	}

	for (i = 0; i < count; ++i) {
		printArgs[i] = track->mSeqCtrl.readByte();
		if (argSources[i] == 2)
			printArgs[i] = (uintptr_t)track->mSeqCtrl.getAddr(printArgs[i]);
		else if (argSources[i] == 5)
			printArgs[i] = track->mTrackRoute;
		else if (argSources[i] >= 3)
			printArgs[i] = track->exchangeRegisterValue(printArgs[i]);
	}

#ifndef NDEBUG
	OSReport(formatString, printArgs[0], printArgs[1], printArgs[2],
	         printArgs[3]);
#endif

	return RC_CONTINUE;
}

int TSeqParser::Cmd_Process(TTrack* track, u8 opcode, u16 indirect_arg_override)
{
	Arg_s sp08       = Arglist[opcode - OP_COMMAND_BASE];
	u16 argTypesMask = sp08.mArgTypes;
	argTypesMask |= indirect_arg_override;
	u32 args[8];
	for (int i = 0; i < sp08.mArgCount; i++) {
		u32 arg = 0;
		switch (argTypesMask & ARG_TYPE_MASK) {
		case ARG_U8:
			arg = track->mSeqCtrl.readByte();
			break;
		case ARG_U16:
			arg = track->mSeqCtrl.read16();
			break;
		case ARG_U24:
			arg = track->mSeqCtrl.read24();
			break;
		case ARG_REGISTER:
			arg = track->exchangeRegisterValue(track->mSeqCtrl.readByte());
			break;
		}
		args[i] = arg;
		argTypesMask >>= 2;
	}

	JASystem::TSeqParser::CmdFunc cmdFunc = sCmdPList[opcode - OP_COMMAND_BASE];
	return cmdFunc == nullptr ? RC_CONTINUE : (this->*cmdFunc)(track, args);
}

int TSeqParser::RegCmd_Process(TTrack* track, int opcode_register,
                               int arg_count_minus_one)
{
	u8 opcode = track->mSeqCtrl.readByte();
	if (opcode_register != 0)
		opcode = track->exchangeRegisterValue(opcode);

	u16 indirectArgsOverride = 0;
	if (opcode_register == 0 || arg_count_minus_one != 0) {
		u8 indirectArgsMask = track->mSeqCtrl.readByte();
		u16 currMask        = ARG_REGISTER;
		for (int i = 0; i < (u32)arg_count_minus_one + 1; i++) {
			if (indirectArgsMask & REG_CMD_REGISTER_ARG)
				indirectArgsOverride |= currMask;

			indirectArgsMask <<= 1;
			currMask <<= 2;
		}
	}
	return Cmd_Process(track, opcode, indirectArgsOverride);
}

int TSeqParser::cmdWait(TTrack* track, u8 opcode)
{
	int bytes = opcode == OP_WAIT8 ? 1 : 2;
	int time  = 0;
	for (int i = 0; i < bytes; i++) {
		time <<= 8;
		time |= track->mSeqCtrl.readByte();
	}
	track->mSeqCtrl.wait(time);
	if (!time)
		return RC_CONTINUE;
	else
		return RC_WAIT;
}

int TSeqParser::cmdNoteOff(TTrack* track, u8 opcode)
{
	if (opcode == OP_NOTE_OFF_REG) {
		u32 reg  = track->mSeqCtrl.readByte();
		u8 value = track->exchangeRegisterValue(reg & NOTE_OFF_VOICE_MASK);

		if (value > NOTE_OFF_VOICE_MASK || value == 0) {
			if (reg & NOTE_OFF_REG_RELEASE_TIME)
				track->mSeqCtrl.readByte();

			return RC_CONTINUE;
		}

		opcode = value + OP_NOTE_OFF;
		if (reg & NOTE_OFF_REG_RELEASE_TIME)
			opcode |= NOTE_OFF_RELEASE_TIME;
	}

	u8 voiceSlot = opcode & OPCODE_PARAM_MASK;

	s32 releaseTime = 0;
	if (voiceSlot & NOTE_OFF_RELEASE_TIME) {
		voiceSlot -= NOTE_OFF_RELEASE_TIME;
		releaseTime = track->mSeqCtrl.readByte();
		if (releaseTime > 100)
			releaseTime = (releaseTime - 98) * 20;
	}
	track->noteOff(voiceSlot, releaseTime);
	return RC_CONTINUE;
}

int TSeqParser::cmdNoteOn(TTrack* track, u8 opcode)
{
	u8 key = opcode + track->mEffectiveTranspose;

	union {
		u8 b;
		struct {
			u8 regKey : 1;
			u8 connect : 2;
			u8 length : 2;
			u8 voice : 3;
		} bits;
	} options;

	u8 optionsByte = track->mSeqCtrl.readByte();
	if (optionsByte & 0x80) {
		key = track->exchangeRegisterValue(key);
		key += track->mEffectiveTranspose;
	}
	options.b = optionsByte;

	u8 r30;
	if (options.bits.connect & TTrack::TNoteMgr::CONNECT_KEY_SWEEP) {
		r30 = key;
		key = track->mNoteMgr.getLastNote();
	}

	u8 velocity = track->mSeqCtrl.readByte();
	if (velocity >= 0x80)
		velocity = track->exchangeRegisterValue(velocity - 0x80);

	u32 time;
	u8 voiceSlot;
	u8 gatePercent;

	if (options.bits.voice == 0) {
		voiceSlot   = 0;
		gatePercent = track->mSeqCtrl.readByte();
		if (gatePercent >= 0x80)
			gatePercent = track->exchangeRegisterValue(gatePercent - 128);

		time = 0;
		for (int i = 0; i < options.bits.length; ++i) {
			time <<= 8;
			time |= track->mSeqCtrl.readByte();
		}

		if (options.bits.length == 1)
			if (time >= 128)
				time = track->exchangeRegisterValue(time - 128);
	} else {
		voiceSlot = options.bits.voice;

		if (options.bits.length)
			voiceSlot = track->exchangeRegisterValue(voiceSlot - 1);

		time        = -1;
		gatePercent = 100;
	}

	track->mNoteMgr.setConnectCase(options.bits.connect);

	s32 dspTime = time;
	if (track->mNoteMgr.checkBeforeTieMode()) {
		if (track->mNoteMgr.getConnectCase() & TTrack::TNoteMgr::CONNECT_TIE)
			dspTime = -1;

		if (dspTime != -1)
			dspTime = track->seqTimeToDspTime(dspTime, gatePercent);

		if (!track->mPaused
		    || !(track->mPauseMuteFlags & TTrack::PAUSE_SKIP_NOTE_ON))
			track->gateOn(voiceSlot, key, velocity, dspTime);
	} else {
		if (dspTime != -1)
			dspTime = track->seqTimeToDspTime(dspTime, gatePercent);

		if (track->mNoteMgr.getConnectCase() & TTrack::TNoteMgr::CONNECT_TIE)
			dspTime = -1;

		if (!track->mPaused
		    || !(track->mPauseMuteFlags & TTrack::PAUSE_SKIP_NOTE_ON))
			track->noteOn(voiceSlot, key, velocity, dspTime);
	}

	track->mNoteMgr.setBaseTime(time);
	track->mNoteMgr.setBeforeTieMode(track->mNoteMgr.getConnectCase()
	                                         & TTrack::TNoteMgr::CONNECT_TIE
	                                     ? true
	                                     : false);

	if (track->mNoteMgr.getConnectCase()
	    & TTrack::TNoteMgr::CONNECT_KEY_SWEEP) {
		if (dspTime == -1)
			dspTime = track->seqTimeToDspTime(time, gatePercent);

		JASystem::TChannel* channel = track->mNoteMgr.getChannel(0);
		if (channel)
			channel->setKeySweepTarget(r30 + track->mEffectiveTranspose,
			                           dspTime);

		key = r30;
	}

	track->mNoteMgr.setLastNote(key);
	if (time == 0xFFFFFFFF)
		return RC_CONTINUE;

	track->mSeqCtrl.wait(time ? time : -1);

	return RC_WAIT;
}

bool TSeqParser::conditionCheck(TTrack* track, u8 condition)
{
	u16 val = track->readRegDirect(TRegisterParam::REG_FLAG);

	switch (condition & COND_MASK) {
	case COND_ALWAYS:
		return true;
	case COND_ZERO:
		return val == 0;
	case COND_NONZERO:
		return val != 0;
	case COND_ONE:
		return val == 1;
	case COND_NEGATIVE:
		return val >= 0x8000;
	case COND_NONNEGATIVE:
		return val < 0x8000;
	}

	return false;
}

int TSeqParser::mainProc(TTrack* track, TSeqCtrl* ctrl)
{
	while (true) {
		u8 opcode   = ctrl->readByte();
		u32 retCode = RC_CONTINUE;
		if (!(opcode & OPCODE_COMMAND_BIT)) {
			retCode = cmdNoteOn(track, opcode);
		} else if (((opcode & OPCODE_FAMILY_MASK) == OP_NOTE_OFF)
		           && !(opcode & NOTE_OFF_VOICE_MASK)) {
			retCode = cmdWait(track, opcode);
		} else if ((opcode & OPCODE_FAMILY_MASK) == OP_NOTE_OFF
		           || opcode == (u32)OP_NOTE_OFF_REG) {
			retCode = cmdNoteOff(track, opcode);
		} else {
			switch ((s32)opcode & OPCODE_FAMILY_MASK) {
			case OP_TIME_PARAM:
				track->writeTimeParam(opcode & OPCODE_PARAM_MASK);
				break;
			case OP_REG_PARAM:
				track->writeRegParam(opcode & OPCODE_PARAM_MASK);
				break;
			case OP_REG_CMD:
				retCode = RegCmd_Process(
				    track, opcode & REG_CMD_OPCODE_REGISTER ? 1 : 0,
				    opcode & REG_CMD_ARG_COUNT_MINUS_ONE_MASK);
				break;
			default:
				retCode = Cmd_Process(track, opcode, 0);
				break;
			}
		}

		if (retCode == RC_CONTINUE)
			continue;

		if (retCode == RC_WAIT)
			break;

		if (retCode == RC_RETURN_INTERRUPT)
			return PARSE_RECHECK;

		if (retCode == RC_FINISH)
			return PARSE_FINISH;
	}

	return PARSE_WAIT;
}

} // namespace JASystem
