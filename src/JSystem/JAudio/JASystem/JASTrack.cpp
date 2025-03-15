#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASPlayer_impl.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASBankMgr.hpp>
#include <JSystem/JAudio/JASystem/JASRate.hpp>
#include <JSystem/JAudio/JASystem/JASCallback.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <JSystem/JAudio/JASystem/JASChGlobal.hpp>

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
		unk2C4[i] = nullptr;

	for (int i = 0; i < 3; ++i)
		unk3C8[i] = unk3C5[i] = 0;

	for (int i = 0; i < 12; ++i)
		unk37C[i] = Player::sAdsTable[i];
	for (int i = 0; i < 6; ++i)
		unk394[i] = Player::sRelTable[i];

	unk3A0[0] = 0xf;
	unk3A0[1] = 0xf;

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

#pragma dont_inline on
void TTrack::noteOff(u8 note, u16 release) { }
#pragma dont_inline off

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

#pragma dont_inline on
void TTrack::updateTrackAll() { }
#pragma dont_inline off

#pragma dont_inline on
void TTrack::updateTrack(u32 param) { }
#pragma dont_inline off

void TTrack::updateTempo()
{
	if (unk2C0 == 0) {
		unk3B0 = unk3BA;
		unk3B0 *= unk3B8;
		unk3B0 /= Kernel::getDacRate();
		unk3B0 *= 80.0f;
		unk3B0 /= 60.0f;
		if (mOuterParam->checkOuterSwitch(0x40))
			unk3B0 *= mOuterParam->unk18;
	} else {
		unk3B0 = unk2C0->unk3B0;
		unk3BA = unk2C0->unk3BA;
	}

	for (u32 i = 0; i < 16; ++i)
		if (unk2C4[i] && unk2C4[i]->unk3C4)
			unk2C4[i]->updateTempo();
}

void TTrack::updateSeq(u32 param_1, bool param_2)
{
	u32 uVar3 = param_1 | unk3B4;
	if (mOuterParam) {
		uVar3 |= mOuterParam->getOuterUpdate();
		mOuterParam->setOuterUpdate(0);
	}

	unk3B4 = 0;
	if (uVar3 != 0) {
		updateTrack(uVar3);
	}

	for (int i = 0; i < 16; ++i) {
		TTrack* track = unk2C4[i];
		if (track && track->unk3C4) {
			if (param_2)
				track->updateSeq(uVar3, param_2);
			else
				track->unk3B4 |= uVar3;
		}
	}
}

void TTrack::incSelfOsc()
{
	for (int i = 0; i < 2; ++i)
		if (unk3A0[i] == 0xE)
			unk33C[i].getOffset();

	for (int i = 0; i < 16; ++i)
		if (unk2C4[i])
			unk2C4[i]->incSelfOsc();
}

void TTrack::mainProc() { }

int TTrack::seqTimeToDspTime(s32 param_1, u8 param_2)
{
	f32 res = param_1;
	res *= param_2;
	res /= 100.0f;
	if (unk3CB) {
		res /= unk3B0;
	} else {
		res = (res * 120.0f) / unk3BA;
		if (Kernel::getOutputRate() == 0)
			res = (res * Kernel::getSubFrames()) / 10.0f;
	}
	return res;
}

void TTrack::writeTimeParam(u8 param) { }

void TTrack::writeRegParam(u8 param) { }

void TTrack::setSeqData(u8* data, s32 size, Player::SEQ_PLAYMODE mode) { }

bool TTrack::startSeq()
{
	switch (unk3C4) {
	case 0:
		return false;
	case 1:
		return false;
	case 3:
		return false;
	case 2:
		unk3C4 = 1;
		break;
	}
	Kernel::registerSubframeCallback(&TTrack::rootCallback, this);
	return true;
}

void TTrack::stopSeqMain() { }

bool TTrack::stopSeq()
{
	switch (unk3C4) {
	case 0:
		break;

	case 2:
		unk3C4 = 0;
		if (unk3BE == 1)
			TrackMgr::backTrack(this);
		TrackMgr::deAllocRoot(this);
		break;

	default:
		unk3C4 = 3;
		break;
	}
	return true;
}

void TTrack::allNoteOff() { }

bool TTrack::closeTrack()
{
	if (!unk3C4)
		return false;

	if (!unk2C0)
		for (u8 i = 0; i < 8; ++i)
			noteOff(i, 10);
	else
		for (u8 i = 0; i < 8; ++i)
			noteOff(i, 0);

	mNoteMgr.init();
	unk3C4 = 0;

	if (unk3BE == 1)
		TrackMgr::backTrack(this);

	for (u8 i = 0; i < 16; ++i)
		if (unk2C4[i]) {
			unk2C4[i]->closeTrack();
			unk2C4[i] = nullptr;
		}

	unk3C2 = 0;

	if (unk2C0)
		unk2C0->mChannelUpdater.receiveAllChannels(&mChannelUpdater);
	else
		ChGlobal::releaseAll(&mChannelUpdater);

	if (unk3CC) {
		TrackMgr::unRegistTrack(this);
		unk3CC = nullptr;
	}

	return false;
}

void TTrack::muteTrack(u8 flag)
{
	unk3C2 = flag;
	unk3B4 |= 1;
	if (unk3C2 && (unk3C1 & 0x20)) {
		for (u8 i = 0; i < 8; ++i) {
			TChannel* chan = mNoteMgr.getChannel(i);
			if (chan) {
				chan->stop(10);
				mNoteMgr.releaseChannel(i);
			}
		}
	}

	for (int i = 0; i < 16; ++i) {
		TTrack* track = unk2C4[i];
		if (track)
			track->muteTrack(flag);
	}
}

void TTrack::muteChildTracks(u16 flags) { }

void TTrack::initTrack(void* data, u32 size, TTrack* parent)
{
	mSeqCtrl.init(data, size);
	if (parent == nullptr) {
		unk3B8 = 0x78;
		unk3BA = 0x30;
		unk3CB = 1;
		unk3CD = 0;
		unk3C1 = 10;
		unk3C3 = 0;
		unk3C2 = 0;
	} else {
		unk3B8 = parent->unk3B8;
		unk3BD = 0;
		unk3B0 = parent->unk3B0;
		unk3BA = parent->unk3BA;
		unk3CB = parent->unk3CB;
		unk3CD = parent->unk3CD;
		unk3C1 = parent->unk3C1;
		unk3C3 = parent->unk3C3;
		unk3C2 = parent->unk3C2;
	}

	mNoteMgr.init();
	unk3C4 = 1;
	unk2C0 = parent;
	mIntrMgr.init();

	initTimed();

	if ((unk3BC & 2) || !unk2C0) {
		mRegisterParam.init();
		for (int i = 0; i < 3; ++i) {
			unk3C5[i]                = 0;
			unk3C8[i]                = 0;
			mChannelUpdater.unk62[i] = 0x1A;
		}
	} else {
		mRegisterParam.inherit(unk2C0->mRegisterParam);
		for (int i = 0; i < 3; ++i) {
			unk3C5[i]                = unk2C0->unk3C5[i];
			unk3C8[i]                = unk2C0->unk3C8[i];
			mChannelUpdater.unk62[i] = unk2C0->mChannelUpdater.unk62[i];
		}
	}

	for (u8 i = 0; i < 16; ++i)
		unk2C4[i] = nullptr;

	unk30C[0] = Player::sEnvelopeDef;
	unk3A0[0] = 0xf;
	unk30C[1] = Player::sVibratoDef;
	unk33C[1].initStart();
	unk3A0[1] = 0xe;

	unk3BF = 0;
	unk3C0 = 0;
	mTrackPort.init();
	unk3CC = 0;
}

int TTrack::startTrack(TTrack* parent, u8 param2, u8 param3, u32 param4)
{
	unk308 = (((parent->unk308 << 4) | param2) & 0xfffffff)
	         | ((parent->unk308 & 0xf0000000) + 0x10000000);

	unk3A8 = 0;
	unk3BC = param3;
	initTrack(parent->mSeqCtrl.mRawFilePtr, param4, parent);
	if (mOuterParam)
		mOuterParam->initExtBuffer();
	mChannelUpdater.initAllocChannel(0);
	updateTrackAll();
	return 0;
}

TTrack* TTrack::openTrack(u8 param)
{
	TTrack* oldTrack = unk2C4[param];
	if (oldTrack)
		oldTrack->closeTrack();

	TTrack* newTrack = TrackMgr::getNewTrack();
	if (!newTrack)
		return nullptr;

	unk2C4[param] = newTrack;
	return newTrack;
}

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

u32 TTrack::readReg32(u8 reg)
{
	u32 result;
	switch (reg) {
	case 0x28:
	case 0x29:
	case 0x2A:
	case 0x2B:
		result = mRegisterParam.unk20[reg - 0x28];
		break;

	case 0x23:
		result = readRegDirect(4);
		result <<= 0x10;
		result |= readRegDirect(5);
		break;

	default:
		result = readRegDirect(reg);
		break;
	}
	return result;
}

u32 TTrack::exchangeRegisterValue(u8 reg)
{
	if (reg < 0x40)
		return readReg32(reg);
	else
		return mNoteMgr.getUnk20(reg - 0x40);
}

u16 TTrack::readRegDirect(u8 reg) { }

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

s32 TTrack::rootCallback(void* param) { }

void TTrack::updateSyncSw(u8 param) { }

void TTrack::registerTrackCallback(u16 (*callback)(TTrack*, u16)) { }

} // namespace JASystem
