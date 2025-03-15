#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASPlayer_impl.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASBankMgr.hpp>
#include <types.h>

namespace JASystem {

TSeqParser* TTrack::sParser                = nullptr;
u8 TTrack::sUpdateSyncMode                 = 0;
u16 (*TTrack::sCallBackFunc)(TTrack*, u16) = nullptr;
u32 TTrack::sOscTable                      = 0;

TTrack::TTrack()
    : unk2C0(0)
    , mOuterParam(0)
    , unk308(0)
    , unk3A8(0)
{
	unk3AC = 0.0f;
	unk3B0 = 0.0f;
	unk3B4 = 0;
	unk3B8 = 0x78;
	unk3BA = 0x78;

	unk3BC = 0;
	unk3BD = 0;
	unk3BE = 0;
	unk3BF = 0;
	unk3C0 = 0;
	unk3C1 = 0;
	unk3C2 = 0;
	unk3C3 = 0;
	unk3C4 = 0;

	unk3CB = 0;
	unk3CC = 0;
	unk3CD = 0;

	for (int i = 0; i < 16; ++i)
		unk2C4[i] = 0;

	for (int i = 0; i < 3; ++i)
		unk3C8[i] = unk3C5[i] = 0;

	for (int i = 0; i < 12; ++i)
		unk37C[i] = Player::sAdsTable[i];
	for (int i = 0; i < 6; ++i)
		unk394[i] = Player::sRelTable[i];

	unk3A0 = 0xf;
	unk3A4 = 0xf;

	mChannelUpdater.init();

	Calc::bzero(&mTimedParam, sizeof(mTimedParam));

	unk33C[0].setOsc(&unk30C[0]);
	unk33C[1].setOsc(&unk30C[1]);
}

void TTrack::setInterrupt(u16 interrupt) { mIntrMgr.request(interrupt); }

void TTrack::tryInterrupt() { }

void TTrack::setBankNumber(u8 bank) { }

bool TTrack::assignExtBuffer(TOuterParam* buffer)
{
	if (buffer == nullptr)
		return false;
	mOuterParam = buffer;
	return true;
}

void TTrack::setInnerMemory(u8 param) { unk3BE = param; }

void TTrack::setPanSwitchExt(u8 param1, u8 param2) { unk3C5[param2] = param1; }

void TTrack::setPanSwitchParent(u8 param1, u8 param2)
{
	unk3C8[param2] = param1;
}

u8 TTrack::getBank() const { return 0; }

u8 TTrack::getProgram() const { return 0; }

f32 TTrack::getVolume() const { return 0.0f; }

f32 TTrack::getPitch() const { return 0.0f; }

f32 TTrack::getPan() const { return 0.0f; }

f32 TTrack::getFxmix() const { return 0.0f; }

f32 TTrack::getDolby() const { return 0.0f; }

u8 TTrack::getNoteStatus(u8 note) { return 0; }

void TTrack::releaseChannelAll() { }

void TTrack::flushAll()
{
	mChannelUpdater.stopAll();
	mChannelUpdater.stopAllRelease();
}

void TTrack::initTimed()
{
	for (u8 i = 0; i < 18; i++) {
		mTimedParam.mMoveParams[i].mMoveTime     = 0.0f;
		mTimedParam.mMoveParams[i].mCurrentValue = 1.0f;
		mTimedParam.mMoveParams[i].mTargetValue  = 1.0f;
	}
	mTimedParam.mMoveParams[1].mCurrentValue  = 0.0f;
	mTimedParam.mMoveParams[1].mTargetValue   = 0.0f;
	mTimedParam.mMoveParams[3].mCurrentValue  = 0.5f;
	mTimedParam.mMoveParams[3].mTargetValue   = 0.5f;
	mTimedParam.mMoveParams[16].mCurrentValue = 0.5f;
	mTimedParam.mMoveParams[16].mTargetValue  = 0.5f;
	mTimedParam.mMoveParams[17].mCurrentValue = 0.0f;
	mTimedParam.mMoveParams[17].mTargetValue  = 0.0f;
	mTimedParam.mMoveParams[2].mCurrentValue  = 0.0f;
	mTimedParam.mMoveParams[2].mTargetValue   = 0.0f;
	mTimedParam.mMoveParams[4].mCurrentValue  = 0.0f;
	mTimedParam.mMoveParams[4].mTargetValue   = 0.0f;
	for (u8 i = 1; i < 4; i++) {
		mTimedParam.mMoveParams[i + 12].mCurrentValue = 0.0f;
		mTimedParam.mMoveParams[i + 12].mTargetValue  = 0.0f;
	}
	mTimedParam.mMoveParams[5].mCurrentValue = 0.0f;
	mTimedParam.mMoveParams[5].mTargetValue  = 0.0f;
}

int TTrack::noteOn(u8 param_1, s32 param_2, s32 param_3, s32 param_4)
{
	if (unk3C2 && (unk3C1 & 0x40))
		return -1;

	return 0;
}

void TTrack::noteOff(u8 note, u16 release) { }

int TTrack::gateOn(u8 param_1, s32 param_2, s32 param_3, s32 param_4)
{
	TChannel* channel = mNoteMgr.getChannel(param_1);
	if (!channel)
		return -1;

	BankMgr::gateOn(channel, param_2, param_3, param_4);
	return 0;
}

void TTrack::checkNoteStop(s32 param) { }

void TTrack::oscSetupFull(u8 param_1, u32 param_2, u32 param_3)
{
	u32 var1  = (param_1 & 0x10) >> 4;
	int var2  = param_1 & 0x0f;
	bool var3 = (param_1 & 0x80) >> 7 ? true : false;
	bool var4 = param_1 & 0x40 ? true : false;
	bool var5 = param_1 & 0x20 ? true : false;

	if (var3) {
		unk30C[var1]      = Player::sEnvelopeDef;
		unk30C[var1].unk0 = var2;
		if (var2 == 1)
			unk30C[var1].unk14 = 1.0f;
	}

	if (var4) {
		if (param_2 == 0)
			unk30C[var1].unk8 = nullptr;

		unk30C[var1].unk8 = (s16*)(mSeqCtrl.mRawFilePtr + param_2);
	}

	if (!var5)
		return;

	if (param_3 == 0)
		unk30C[var1].unkC = Player::sRelTable;

	unk30C[var1].unkC = (s16*)(mSeqCtrl.mRawFilePtr + param_2);
}

void TTrack::oscSetupSimpleEnv(u8 param_1, u32 param_2)
{
	switch (param_1) {
	case 0:
		unk30C[0]      = Player::sEnvelopeDef;
		unk30C[0].unk8 = (s16*)(mSeqCtrl.mRawFilePtr + param_2);
		break;
	case 1:
		unk30C[0].unkC = (s16*)(mSeqCtrl.mRawFilePtr + param_2);
		break;
	}
}

void TTrack::oscUpdateParam(u8 param_1, f32 param_2)
{
	switch (param_1) {
	case 6:
		unk30C[0].unk10 = param_2;
		break;
	case 7:
		unk30C[0].unk4 = param_2;
		break;
	case 8:
		unk30C[0].unk14 = param_2;
		break;
	case 9:
		unk30C[1].unk10 = param_2;
		break;
	case 10:
		unk30C[1].unk4 = param_2;
		break;
	case 11:
		unk30C[1].unk14 = param_2;
		break;
	}
}

void TTrack::oscSetupSimple(u8 param_1)
{
	switch (param_1) {
	case 0:
		unk30C[1] = Player::sVibratoDef;
		break;
	case 1:
		unk30C[0] = Player::sTremoroDef;
		break;
	case 2:
		unk30C[1] = Player::sTremoroDef;
		break;
	}
}

void TTrack::updateTrackAll() { }

void TTrack::updateTrack(u32 param) { }

void TTrack::updateTempo() { }

void TTrack::updateSeq(u32 param, bool flag) { }

void TTrack::incSelfOsc() { }

void TTrack::mainProc() { }

void TTrack::seqTimeToDspTime(s32 time, u8 param) { }

void TTrack::writeTimeParam(u8 param) { }

void TTrack::writeRegParam(u8 param) { }

void TTrack::setSeqData(u8* data, s32 size, Player::SEQ_PLAYMODE mode) { }

void TTrack::startSeq() { }

void TTrack::stopSeqMain() { }

void TTrack::stopSeq() { }

void TTrack::allNoteOff() { }

void TTrack::closeTrack() { }

void TTrack::muteTrack(u8 flag) { }

void TTrack::muteChildTracks(u16 flags) { }

void TTrack::initTrack(void* data, u32 size, TTrack* parent) { }

void TTrack::startTrack(TTrack* parent, u8 param1, u8 param2, u32 param3) { }

void TTrack::openTrack(u8 param) { }

int TTrack::loadTbl(u32 param_1, u32 param_2, u32 param_3)
{
	u32 res;
	switch (param_3) {
	case 4:
		res = mSeqCtrl.getByte(param_1 + param_2);
		break;
	case 5:
		res = mSeqCtrl.get16(param_1 + 2 * param_2);
		break;
	case 6:
		res = mSeqCtrl.get24(param_1 + 3 * param_2);
		break;
	case 7:
		res = mSeqCtrl.get32(param_1 + 4 * param_2);
		break;
	case 8:
		res = mSeqCtrl.get32(param_1 + param_2);
		break;
	}
	return res;
}

void TTrack::readReg32(u8 reg) { }

void TTrack::exchangeRegisterValue(u8 reg) { }

void TTrack::readRegDirect(u8 reg) { }

void TTrack::writeRegDirect(u8 reg, u16 value) { }

void TTrack::setTrackExtPanPower(f32 power) { }

void TTrack::writePortAppDirect(u32 port, u16 value) { }

void TTrack::readPortAppDirect(u32 port, u16* value) { }

void TTrack::setExtFirFilterD(s16* filter) { }

void TTrack::routeTrack(u32 route) { }

void TTrack::writePortApp(u32 port, u16 value) { }

void TTrack::readPortApp(u32 port, u16* value) { }

void TTrack::checkExportApp(u32 port) { }

void TTrack::checkImportApp(u32 port) { }

void TTrack::pauseTrack(u8 flag) { }

void TTrack::unPauseTrack(u8 flag) { }

void TTrack::pauseTrackAll() { }

void TTrack::unPauseTrackAll() { }

void TTrack::reset() { }

void TTrack::panCalc(f32 param1, f32 param2, f32 param3, u8 param4) { }

void TTrack::rootCallback(void* param) { }

void TTrack::updateSyncSw(u8 param) { }

void TTrack::registerTrackCallback(u16 (*callback)(TTrack*, u16)) { }

} // namespace JASystem
