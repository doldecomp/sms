#include <JSystem/JAudio/JASystem/JASSeqParser.hpp>
#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASPlayer_impl.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <dolphin/os.h>

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

int TSeqParser::cmdOpenTrack(TTrack* track, u32* args)
{
	u32 param1 = args[0];
	u32 param2 = args[1];
	u8 b1      = param1 & 0xF;
	u8 b2      = (param1 >> 6) & 3;
	if (param1 & 0x20)
		b2 = 4;

	TTrack* child = track->openTrack(b1);
	child->startTrack(track, b1, b2, param2);
	return 0;
}

int TSeqParser::cmdOpenTrackBros(TTrack* track, u32* args)
{
	TTrack* parent = track->unk2C0;
	if (!parent)
		return 0;

	u32 param1 = args[0];
	u32 param2 = args[1];
	u8 b1      = param1 & 0xF;
	u8 b2      = (param1 >> 6) & 3;
	if (param1 & 0x20)
		b2 = 4;

	TTrack* bros = parent->openTrack(b1);
	bros->startTrack(parent, b1, b2, param2);
	return 0;
}

int TSeqParser::cmdCall(TTrack* track, u32* args)
{
	u8 flag = track->mSeqCtrl.readByte();

	u32 offset;
	if (flag & 0x80) {
		offset = track->readRegDirect(track->mSeqCtrl.readByte());

		if (flag & 0x40) {
			u32 offs;
			if (flag & 0x20)
				offs = track->readRegDirect(track->mSeqCtrl.readByte());
			else
				offs = track->mSeqCtrl.read24();

			offset = track->mSeqCtrl.get24(offs + offset * 3);
		}
	} else {
		offset = track->mSeqCtrl.read24();
	}

	if (conditionCheck(track, flag))
		track->mSeqCtrl.call(offset);

	return 0;
}

int TSeqParser::cmdRet(TTrack* track, u32* args)
{
	if (conditionCheck(track, args[0])) {
		bool tmp;
		if (track->mSeqCtrl.mLoopIndex == 0)
			tmp = false;
		else
			tmp = track->mSeqCtrl.ret();

		if (!tmp)
			return 3;
	}
	return 0;
}

int TSeqParser::cmdJmp(TTrack* track, u32* args)
{
	void* trackptr = 0;
	u32 offset;
	u8 flag = track->mSeqCtrl.readByte();

	if (flag & 0x80) {
		u8 cData = track->mSeqCtrl.readByte();

		if (flag & 0x40) {
			u32 otherOffset = track->readRegDirect(cData);
			u32 offs;
			if (flag & 0x20) {
				cData = track->mSeqCtrl.readByte();
				offs  = track->readRegDirect(cData);
			} else {
				offs = track->mSeqCtrl.read24();
			}
			offset = track->mSeqCtrl.get24(offs + otherOffset * 3);
		} else {
			if (cData >= 0x28 && 0x2b >= cData)
				trackptr = (void*)track->readReg32(cData);
			else
				offset = track->readReg32(cData);
		}
	} else {
		offset = track->mSeqCtrl.read24();
	}

	if (conditionCheck(track, flag)) {
		if (!trackptr)
			track->mSeqCtrl.jump(offset);
		else
			track->mSeqCtrl.init(trackptr, 0);
	}
	return 0;
}

int TSeqParser::cmdLoopS(TTrack* track, u32* args)
{
	u32 timer = args[0];
	track->mSeqCtrl.loopS(timer);
	return 0;
}

int TSeqParser::cmdLoopE(TTrack* track, u32* args)
{
	if (!track->mSeqCtrl.loopE())
		return 0;
	else
		return 0;
}

int TSeqParser::cmdReadPort(TTrack* track, u32* args)
{
	track->writeRegDirect(args[1], track->mTrackPort.readImport(args[0]));
	return 0;
}

int TSeqParser::cmdWritePort(TTrack* track, u32* args)
{
	track->mTrackPort.writeExport(args[0], args[1]);
	return 0;
}

int TSeqParser::cmdCheckPortImport(TTrack* track, u32* args)
{
	track->writeRegDirect(3, track->mTrackPort.checkImport(args[0]));
	return 0;
}

int TSeqParser::cmdCheckPortExport(TTrack* track, u32* args)
{
	track->writeRegDirect(3, track->mTrackPort.checkExport(args[0]));
	return 0;
}

int TSeqParser::cmdWait(TTrack* track, u32* args)
{
	s32 param1 = args[0];
	track->mSeqCtrl.wait(param1);
	return param1 != 0 ? 1 : 0;
}

int TSeqParser::cmdConnectName(TTrack* track, u32* args)
{
	track->unk3A8 = args[0] << 16 | args[1];
	return 0;
}

int TSeqParser::cmdParentWritePort(TTrack* track, u32* args)
{
	track->unk2C0->writePortAppDirect(args[0] & 0xf, args[1]);
	return 0;
}

int TSeqParser::cmdChildWritePort(TTrack* track, u32* args)
{
	track->unk2C4[args[0] >> 4]->writePortAppDirect(args[0] & 0xf, args[1]);
	return 0;
}

int TSeqParser::cmdSetLastNote(TTrack* track, u32* args)
{
	u32 key = args[0];
	key += track->unk3C0;
	track->mNoteMgr.setLastNote(key);
	return 0;
}

int TSeqParser::cmdTimeRelate(TTrack* track, u32* args)
{
	track->unk3CB = args[0] ? 1 : 0;
	return 0;
}

int TSeqParser::cmdSimpleOsc(TTrack* track, u32* args)
{
	track->oscSetupSimple(args[0]);
	return 0;
}

int TSeqParser::cmdSimpleEnv(TTrack* track, u32* args)
{
	track->oscSetupSimpleEnv(args[0], args[1]);
	return 0;
}

int TSeqParser::cmdSimpleADSR(TTrack* track, u32* args)
{
	s16 realArgs[5];
	for (u8 i = 0; i < 5; ++i)
		realArgs[i] = args[i];

	track->unk30C[0]      = Player::sAdsrDef;
	track->unk30C[0].unk8 = track->unk37C;
	track->unk30C[0].unkC = track->unk394;

	track->unk37C[1] = realArgs[0];
	track->unk37C[4] = realArgs[1];
	track->unk37C[7] = realArgs[2];
	track->unk37C[8] = realArgs[3];
	track->unk394[1] = realArgs[4];
	return 0;
}

int TSeqParser::cmdTranspose(TTrack* track, u32* args)
{
	track->unk3BF = args[0];
	track->unk3C0 = track->unk3BF;
	if (track->unk2C0)
		track->unk3C0 += track->unk2C0->unk3BF;
	return 0;
}

int TSeqParser::cmdCloseTrack(TTrack* track, u32* args)
{
	u8 i = args[0];

	TTrack* child = track->unk2C4[i];
	if (!child)
		return 0;

	child->closeTrack();
	track->unk2C4[i] = nullptr;

	return 0;
}

int TSeqParser::cmdOutSwitch(TTrack* track, u32* args)
{
	TTrack::TOuterParam* outer = track->mOuterParam;
	if (outer) {
		outer->setOuterSwitch(args[0]);
		outer->setOuterUpdate(0xffff);
	}
	return 0;
}

int TSeqParser::cmdUpdateSync(TTrack* track, u32* args)
{
	track->updateTrack(args[0]);
	return 0;
}

int TSeqParser::cmdBusConnect(TTrack* track, u32* args)
{
	u32 i = args[0];
	if (i < 6)
		track->mChannelUpdater.unk4E[i] = args[1];
	return 0;
}

int TSeqParser::cmdPauseStatus(TTrack* track, u32* args)
{
	track->unk3C1 = args[0];
	return 0;
}

int TSeqParser::cmdVolumeMode(TTrack* track, u32* args)
{
	track->unk3C3 = args[0];
	return 0;
}

int TSeqParser::cmdSetInterrupt(TTrack* track, u32* args)
{
	track->mIntrMgr.setIntr(args[0], track->mSeqCtrl.getBase() + args[1]);
	return 0;
}

int TSeqParser::cmdDisInterrupt(TTrack* track, u32* args)
{
	track->mIntrMgr.resetInter(args[0]);
	return 0;
}

int TSeqParser::cmdClrI(TTrack* track, u32* args)
{
	track->mIntrMgr.enable();
	track->mSeqCtrl.clrIntr();
	return 0;
}

int TSeqParser::cmdSetI(TTrack* track, u32* args)
{
	track->mIntrMgr.disable();
	return 0;
}

int TSeqParser::cmdRetI(TTrack* track, u32* args)
{
	track->mIntrMgr.enable();
	track->mSeqCtrl.retIntr();
	return 2;
}

int TSeqParser::cmdIntTimer(TTrack* track, u32* args)
{
	track->mIntrMgr.setTimer(args[0], args[1]);
	return 0;
}

int TSeqParser::cmdConnectOpen(TTrack* track, u32* args)
{
	TrackMgr::registTrack(track->unk3A8, track);
	track->unk3CC = 1;
	return 0;
}

int TSeqParser::cmdConnectClose(TTrack* track, u32* args)
{
	if (track->unk3CC) {
		TrackMgr::unRegistTrack(track);
		track->unk3CC = 0;
	}
	return 0;
}

int TSeqParser::cmdSyncCPU(TTrack* track, u32* args)
{
	u16 var1 = 0xffff;
	if (TTrack::sCallBackFunc) {
		var1 = TTrack::sCallBackFunc(track, args[0]);
	}
	track->writeRegDirect(3, var1);
	return 0;
}

int TSeqParser::cmdFlushAll(TTrack* track, u32* args)
{
	track->flushAll();
	return 0;
}

int TSeqParser::cmdFlushRelease(TTrack* track, u32* args)
{
	track->mChannelUpdater.stopAllRelease();
	return 0;
}

int TSeqParser::cmdTimeBase(TTrack* track, u32* args)
{
	track->unk3BA = args[0];
	if (!track->unk2C0)
		track->updateTempo();
	return 0;
}

int TSeqParser::cmdTempo(TTrack* track, u32* args)
{
	track->unk3B8 = args[0];
	if (!track->unk2C0)
		track->updateTempo();
	else
		track->unk3BD = 1;
	return 0;
}

int TSeqParser::cmdFinish(TTrack* track, u32* args)
{
	u32 mask = 0;
	for (u8 i = 0; i < 18; ++i) {
		TTrack::MoveParam_* param = &track->mTimedParam.mMoveParams[i];
		if (param->mMoveTime > 0.0f) {
			param->mCurrentValue += param->mMoveAmount;
			param->mMoveTime -= 1.0f;
			if (i <= 5 || i >= 11)
				mask = mask | 1 << i;
			else
				track->oscUpdateParam(i, param->mCurrentValue);
		}
	}
	track->updateSeq(mask, true);
	return 3;
}

int TSeqParser::cmdNop(TTrack* track, u32* args) { return 0; }

int TSeqParser::cmdPanPowSet(TTrack* track, u32* args)
{
	track->mRegisterParam.setPanPower(0, args[0]);
	track->mRegisterParam.setPanPower(1, args[1]);
	track->mRegisterParam.setPanPower(2, args[2]);
	track->mRegisterParam.setPanPower(3, args[3] * 327.67f);
	track->mRegisterParam.setPanPower(4, args[4] * 327.67f);
	return 0;
}

int TSeqParser::cmdIIRSet(TTrack* track, u32* args)
{
	for (u8 i = 0; i < 4; ++i) {
		s16 target              = args[i];
		TTrack::MoveParam_* iir = &track->mTimedParam.mInnerParam.mIIRs[i];
		iir->mTargetValue       = target / 32768.0f;
		iir->mCurrentValue      = iir->mTargetValue;
		iir->mMoveAmount        = 0.0f;
		iir->mMoveTime          = 1.0f;
	}
	return 0;
}

int TSeqParser::cmdFIRSet(TTrack* track, u32* args)
{
	track->setExtFirFilterD((s16*)(track->mSeqCtrl.mRawFilePtr + args[0]));
	return 0;
}

int TSeqParser::cmdEXTSet(TTrack* track, u32* args)
{
	u32 offset                      = args[0];
	u8* outerParamAddr              = track->mSeqCtrl.mRawFilePtr + offset;
	TTrack::TOuterParam* outerParam = (TTrack::TOuterParam*)outerParamAddr;
	outerParam->initExtBuffer();
	track->assignExtBuffer(outerParam);
	return 0;
}

int TSeqParser::cmdPanSwSet(TTrack* track, u32* args)
{
	u8 calcTypes[]       = { 0, 0, 0, 1, 1, 2, 2 };
	u8 parentCalcTypes[] = { 0, 1, 2, 0, 2, 0, 2 };

	for (u8 i = 0; i < 3; i++) {
		track->unk3C5[i]                = calcTypes[args[i] >> 5];
		track->unk3C8[i]                = parentCalcTypes[args[i] >> 5];
		track->mChannelUpdater.unk62[i] = args[i] & 0x1F;
		track->unk3B4 |= 8;
	}

	return 0;
}

int TSeqParser::cmdOscRoute(TTrack* track, u32* args)
{
	s32 arg = args[0];
	u32 i   = (arg >> 4) & 0xF;
	s32 v   = arg & 0xF;

	track->unk3A0[i] = v;
	if (v == 0xE)
		track->unk33C[i].initStart();
	return 0;
}

int TSeqParser::cmdIIRCutOff(TTrack* track, u32* args)
{
	u8 iirTableIdx = args[0];
	for (u8 i = 0; i < 4; i++) {
		s16* table  = JASystem::Player::CUTOFF_TO_IIR_TABLE[iirTableIdx];
		u8 iirIndex = TTrack::TIMED_IIR_Unk0 + i;
		TTrack::MoveParam_* iir = &track->mTimedParam.mMoveParams[iirIndex];
		iir->mTargetValue       = table[i] / (32768.0f - 1.0f);
		iir->mCurrentValue      = iir->mTargetValue;
		iir->mMoveAmount        = 0.0f;
		iir->mMoveTime          = 1.0f;
	}
	return 0;
}

int TSeqParser::cmdOscFull(TTrack* track, u32* args)
{
	track->oscSetupFull(args[0], args[1], args[2]);
	return 0;
}

int TSeqParser::cmdCheckWave(TTrack* track, u32* args)
{
	track->writeRegDirect(3, 0);
	return 0;
}

int TSeqParser::cmdPrintf(TTrack* track, u32* args)
{
	char buffer[128];
	u8 byteArray[4];
	int registers[4];
	u32 count = 0;

	u32 i;
	for (i = 0; i < 128; ++i) {
		buffer[i] = track->mSeqCtrl.readByte();
		if (!buffer[i])
			break;

		if (buffer[i] == '\\') {
			buffer[i] = track->mSeqCtrl.readByte();
			if (!buffer[i])
				break;

			switch (buffer[i]) {
			case 'n':
				buffer[i] = '\r';
				continue;
			default:
				continue;
			}
		}

		if (buffer[i] != '%')
			continue;

		++i;
		buffer[i] = track->mSeqCtrl.readByte();
		if (!buffer[i])
			break;

		switch (buffer[i]) {
		case 'd':
			byteArray[count] = 0;
			break;
		case 'x':
			byteArray[count] = 1;
			break;
		case 's':
			byteArray[count] = 2;
			break;
		case 'r':
			byteArray[count] = 3;
			buffer[i]        = 'd';
			break;
		case 'R':
			byteArray[count] = 4;
			buffer[i]        = 'x';
			break;
		case 't':
			byteArray[count] = 5;
			buffer[i]        = 'x';
			break;
		}
		++count;
	}

	for (i = 0; i < count; ++i) {
		registers[i] = track->mSeqCtrl.readByte();
		if (byteArray[i] == 2)
			registers[i] = (int)&track->mSeqCtrl.mRawFilePtr[registers[i]];
		else if (byteArray[i] == 5)
			registers[i] = track->unk308;
		else if (byteArray[i] >= 3)
			registers[i] = track->exchangeRegisterValue(registers[i]);
	}

	// Thrown out in release build
	// OSReport(buf, registers[0], registers[1], registers[2], registers[3]);

	return 0;
}

int TSeqParser::Cmd_Process(TTrack* track, u8 param_2, u16 param_3)
{
	JASystem::Arg_s sp08 = Arglist[param_2 - 0xC0];
	u16 r29              = sp08.m02;
	r29 |= param_3;
	u32 sp18[8];
	for (int i = 0; i < sp08.m00; i++) {
		u32 r3 = 0;
		switch (r29 & 0x3) {
		case 0:
			r3 = track->mSeqCtrl.readByte();
			break;
		case 1:
			r3 = track->mSeqCtrl.read16();
			break;
		case 2:
			r3 = track->mSeqCtrl.read24();
			break;
		case 3:
			r3 = track->exchangeRegisterValue(track->mSeqCtrl.readByte());
			break;
		}
		sp18[i] = r3;
		r29 >>= 2;
	}

	JASystem::TSeqParser::CmdFunc cmdFunc = sCmdPList[param_2 - 0xC0];
	return cmdFunc == nullptr ? 0 : (this->*cmdFunc)(track, sp18);
}

int TSeqParser::RegCmd_Process(TTrack* track, int param_2, int param_3)
{
	u8 r5 = track->mSeqCtrl.readByte();
	if (param_2 != 0) {
		r5 = track->exchangeRegisterValue(r5);
	}
	u16 r6 = 0;
	if (param_2 == 0 || param_3 != 0) {
		u8 r3  = track->mSeqCtrl.readByte();
		u16 r4 = 3;
		for (int i = 0; i < (u32)param_3 + 1; i++) {
			if (r3 & 0x80) {
				r6 |= r4;
			}
			r3 <<= 1;
			r4 <<= 2;
		}
	}
	return Cmd_Process(track, r5, r6);
}

int TSeqParser::cmdWait(TTrack* track, u8 flag)
{
	int end = flag == 0x80 ? 1 : 2;
	int val = 0;
	for (int i = 0; i < end; i++) {
		val <<= 8;
		val |= track->mSeqCtrl.readByte();
	}
	track->mSeqCtrl.mWaitTimer = val;
	if (!val) {
		return 0;
	} else {
		return 1;
	}
}

int TSeqParser::cmdNoteOff(TTrack* track, u8 flag)
{
	if (flag == 0xF9) {
		u32 r31   = track->mSeqCtrl.readByte();
		u8 rdata2 = track->exchangeRegisterValue(r31 & 0x7);

		if (rdata2 > 7 || rdata2 == 0) {
			if (r31 & 0x80)
				++track->mSeqCtrl.mCurrentFilePtr;

			return 0;
		}

		flag = rdata2 + 0x80;
		if (r31 & 0x80)
			flag |= 0x08;
	}

	u8 note = flag & 0xF;

	s32 release = 0;
	if (flag & 0x8) {
		note -= 0x8;
		release = track->mSeqCtrl.readByte();
		if (release > 100)
			release = (release - 98) * 20;
	}
	track->noteOff(note, release);
	return 0;
}

int TSeqParser::cmdNoteOn(TTrack* track, u8 note)
{
	u8 r31 = note + track->unk3C0;

	// TODO: very fake, but IDK how to make mwcc push it off
	// to the stack =/
	volatile u8 r25_or_0x1C = track->mSeqCtrl.readByte();
	if (r25_or_0x1C & 0x80) {
		r31 = track->exchangeRegisterValue(note);
		r31 += track->unk3C0;
	}

	u8 r30;
	if ((r25_or_0x1C >> 5) & 0x2) {
		r30 = r31;
		r31 = track->mNoteMgr.getLastNote();
	}

	u8 r29 = track->mSeqCtrl.readByte();
	if (r29 >= 0x80)
		r29 = track->exchangeRegisterValue(r29 - 0x80);

	u32 r28;
	u8 r26;
	u8 r27;

	if (!(r25_or_0x1C & 0x7)) {
		r27 = 0;
		r26 = track->mSeqCtrl.readByte();
		if (r26 >= 0x80)
			r26 = track->exchangeRegisterValue(r26 - 0x80);

		r28 = 0;
		for (u8 i = 0; i < ((r25_or_0x1C >> 3) & 0x3); ++i) {
			r28 <<= 8;
			r28 |= track->mSeqCtrl.readByte();
		}

		if ((u32)((r25_or_0x1C >> 3) & 0x3) == 1)
			if (r28 >= 0x80)
				r28 = track->exchangeRegisterValue(r28 - 0x80);

	} else {
		r27 = r25_or_0x1C & 0x7;

		if ((r25_or_0x1C >> 3) & 0x3)
			r27 = track->exchangeRegisterValue(r27 - 1);

		r28 = -1;
		r26 = 100;
	}

	track->mNoteMgr.setConnectCase((r25_or_0x1C >> 5) & 0x3);

	s32 r25 = r28;
	if (track->mNoteMgr.checkBeforeTieMode()) {
		if (track->mNoteMgr.getConnectCase() & 1)
			r25 = -1;

		if (r25 != -1)
			r25 = track->seqTimeToDspTime(r25, r26);

		if (!track->unk3CD || !(track->unk3C1 & 0x10))
			track->gateOn(r27, r31, r29, r25);
	} else {
		if ((s32)r25 != -1)
			r25 = track->seqTimeToDspTime(r25, r26);

		if (track->mNoteMgr.getConnectCase() & 1)
			r25 = -1;

		if (!track->unk3CD || !(track->unk3C1 & 0x10))
			track->noteOn(r27, r31, r29, r25);
	}

	track->mNoteMgr.setBaseTime(r28);
	track->mNoteMgr.setBeforeTieMode(
	    track->mNoteMgr.getConnectCase() & 1 ? true : false);

	if (track->mNoteMgr.getConnectCase() & 0x2) {
		if (r25 == -1)
			r25 = track->seqTimeToDspTime(r28, r26);

		JASystem::TChannel* channel = track->mNoteMgr.getChannel(0);
		if (channel)
			channel->setKeySweepTarget(r30 + track->unk3C0, r25);

		r31 = r30;
	}

	track->mNoteMgr.setLastNote(r31);
	if (r28 == 0xFFFFFFFF)
		return 0;

	track->mSeqCtrl.wait(r28 ? r28 : -1);

	return 1;
}

bool TSeqParser::conditionCheck(TTrack* track, u8 condition)
{
	u16 val = track->readRegDirect(3);

	switch (condition & 0xF) {
	case 0:
		return true;
	case 1:
		return val == 0;
	case 2:
		return val != 0;
	case 3:
		return val == 1;
	case 4:
		return val >= 0x8000;
	case 5:
		return val < 0x8000;
	}

	return false;
}

int TSeqParser::mainProc(TTrack* track, TSeqCtrl* ctrl)
{
	while (true) {
		u8 flag     = track->mSeqCtrl.readByte();
		u32 retCode = 0;
		if (!(flag & 0x80)) {
			retCode = cmdNoteOn(track, flag);
		} else if (((flag & 0xF0) == 128) && !(flag & 0x07)) {
			retCode = cmdWait(track, flag);
		} else if (((flag & 0xF0) == 128) || flag == (u32)0xF9) {
			retCode = cmdNoteOff(track, flag);
		} else {
			switch ((s32)flag & 0xF0) {
			case 0x90:
				track->writeTimeParam(flag & 0xF);
				break;
			case 0xA0:
				track->writeRegParam(flag & 0xF);
				break;
			case 0xB0:
				retCode
				    = RegCmd_Process(track, (flag & 8) ? 1 : 0, flag & 0x07);
				break;
			default:
				retCode = Cmd_Process(track, flag, 0);
				break;
			}
		}

		if (retCode == 0)
			continue;

		if (retCode == 1)
			break;

		if (retCode == 2)
			return -2;

		if (retCode == 3)
			return -1;
	}

	return 0;
}

} // namespace JASystem
