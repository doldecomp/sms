#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASPlayer_impl.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASBankMgr.hpp>
#include <JSystem/JAudio/JASystem/JASRate.hpp>
#include <JSystem/JAudio/JASystem/JASCallback.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <JSystem/JAudio/JASystem/JASChGlobal.hpp>
#include <JSystem/JAudio/JASystem/JASSeqParser.hpp>
#include <JSystem/JMath.hpp>
#include <dolphin/os.h>

namespace JASystem {

namespace Player {
	static JMath::TRandom_enough_ oRandom(0);
}

TSeqParser TTrack::sParser;

u8 TTrack::sUpdateSyncMode                 = 0;
u16 (*TTrack::sCallBackFunc)(TTrack*, u16) = nullptr;

u8 TTrack::sOscTable[5] = { 1, 2, 8, 4, 16 };

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

	for (int i = 0; i < 2; ++i)
		unk3A0[i] = 0xf;

	mChannelUpdater.init();

	Calc::bzero(&mTimedParam, sizeof(mTimedParam));

	for (int i = 0; i < 2; ++i)
		unk33C[i].setOsc(&unk30C[i]);
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

	u32 index = param_1;
	if (mNoteMgr.getChannel(index))
		noteOff(param_1, 0);

	TTrack* r24      = unk2C0;
	TChannelMgr* r30 = &mChannelUpdater;
	TTrack* r3       = r24;
	while (!r30->unk0 || !r30->unk8) {
		if (r3 == nullptr) {
			r30 = &mChannelUpdater;
			break;
		}
		r30 = &r24->mChannelUpdater;
		r3  = r3->unk2C0;
	}

	if (unk3BC == 4) {
		if (r24 == nullptr)
			return -1;

		if (r30 != &r24->mChannelUpdater) {
			TChannel* chan = r30->getListHead(0);
			if (chan) {
				--r30->unk0;
				mChannelUpdater.addListHead(chan, 0);
				++r24->mChannelUpdater.unk0;
				chan->unk4 = &r24->mChannelUpdater;
			}
			r30 = &r24->mChannelUpdater;
		}
	} else {
		if (r30 != &mChannelUpdater) {
			TChannel* chan = r30->getListHead(0);
			if (chan) {
				--r30->unk0;
				mChannelUpdater.addListHead(chan, 0);
				++mChannelUpdater.unk0;
				chan->unk4 = &mChannelUpdater;
			}
			r30 = &mChannelUpdater;
		}
	}

	u32 reg     = readRegDirect(6);
	u32 physNum = BankMgr::getPhysicalNumber((reg >> 8) & 0xFF);

	TChannel* chan
	    = BankMgr::noteOn(r30, (u8)physNum, (u8)reg, param_2, param_3, param_4);

	if (!chan)
		return -1;

	mNoteMgr.unk0[index]  = chan;
	mNoteMgr.unk20[index] = chan->unkC6;

	// TODO: some tricky inlines happening here
	chan->setPanPower(mRegisterParam.mPanPower[0], mRegisterParam.mPanPower[1],
	                  mRegisterParam.mPanPower[2], mRegisterParam.mPanPower[3]);

	for (u8 i = 0; i < 2; ++i) {
		u32 someThing = unk3A0[i];
		if (someThing != 0xF && someThing != 0xE) {
			if (someThing >= 8) {
				someThing -= 8;
				if (chan->isOsc(someThing))
					chan->copyOsc(someThing, &unk30C[i]);
			} else if (someThing >= 4) {
				someThing -= 4;
				s16* v = unk30C[i].unkC;
				if (chan->isOsc(someThing)) {
					chan->copyOsc(someThing, &unk30C[i]);
					unk30C[i].unkC = v;
				}
			}
			chan->overwriteOsc(someThing, &unk30C[i]);
		}
	}

	if (sUpdateSyncMode == 0)
		updateTrack(0xB);

	chan->resetInitialVolume();

	return 0;
}

bool TTrack::noteOff(u8 note, u16 release)
{
	TChannel* chan = mNoteMgr.getChannel(note);
	if (!chan)
		return false;

	if (release == 0)
		chan->stop(0);
	else
		chan->stop(release);

	mNoteMgr.releaseChannel(note);
	return true;
}

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

void TTrack::updateTrackAll()
{
	f32 fVar6 = mRegisterParam.mPanPower[3] / 32767.0f;
	f32 curVolume;
	f32 curPitch;
	f32 curFxmix;
	f32 curDolby;
	f32 curPan;

	if (unk3BC == 4)
		return;

	mChannelUpdater.unk68 = mRegisterParam.unk1A | 0x10000;
	mChannelUpdater.unk6C = 0;

	u8 cVar8  = 0;
	f32 thing = mTimedParam.mInnerParam.unk110.mCurrentValue * 128.0f;
	s8 local_4c;
	OSf32tos8(&thing, &local_4c);
	if (local_4c < 0) {
		cVar8    = -local_4c;
		local_4c = 0;
	}

	mChannelUpdater.unk60    = 0x10;
	mChannelUpdater.unk5A[0] = cVar8;
	mChannelUpdater.unk5A[1] = local_4c;

	curVolume = mTimedParam.mInnerParam.mVolume.mCurrentValue;
	if (unk3C3 == 0)
		curVolume = curVolume * curVolume;

	if (unk3C2 != 0)
		curVolume = 0.0f;

	curPitch = Player::pitchToCent(mTimedParam.mInnerParam.mPitch.mCurrentValue,
	                               mRegisterParam.unkE);

	curPan   = mTimedParam.mInnerParam.mPan.mCurrentValue;
	curFxmix = mTimedParam.mInnerParam.mFxmix.mCurrentValue;
	curDolby = mTimedParam.mInnerParam.mDolby.mCurrentValue;

	if (mOuterParam) {
		if (mOuterParam->checkOuterSwitch(1))
			curVolume *= mOuterParam->unk4;

		if (mOuterParam->checkOuterSwitch(2))
			curPitch *= mOuterParam->unk8;

		if (mOuterParam->checkOuterSwitch(4))
			curFxmix = panCalc(curFxmix, mOuterParam->unkC, fVar6, unk3C5[1]);

		if (mOuterParam->checkOuterSwitch(0x10))
			curDolby = panCalc(curDolby, mOuterParam->unk10, fVar6, unk3C5[2]);

		if (mOuterParam->checkOuterSwitch(8))
			curPan = panCalc(curPan, mOuterParam->unk14, fVar6, unk3C8[0]);
	}

	if (!unk2C0 || (unk3BC & 1)) {
		mChannelUpdater.unk18 = curVolume;
		mChannelUpdater.unk1C = curPitch;
		mChannelUpdater.unk20 = curPan;
		mChannelUpdater.unk24 = curFxmix;
		mChannelUpdater.unk28 = curDolby;
	} else {
		f32 fVar6             = mRegisterParam.mPanPower[4] / 32767.0f;
		mChannelUpdater.unk18 = unk2C0->mChannelUpdater.unk18 * curVolume;
		mChannelUpdater.unk1C = unk2C0->mChannelUpdater.unk1C * curPitch;
		mChannelUpdater.unk20
		    = panCalc(curPan, unk2C0->mChannelUpdater.unk20, fVar6, unk3C8[0]);
		mChannelUpdater.unk24 = panCalc(curFxmix, unk2C0->mChannelUpdater.unk24,
		                                fVar6, unk3C8[1]);
		mChannelUpdater.unk28 = panCalc(curDolby, unk2C0->mChannelUpdater.unk28,
		                                fVar6, unk3C8[2]);

		if (mOuterParam && mOuterParam->checkOuterSwitch(0x80)) {
			for (u8 i = 0; i < 8; ++i)
				mChannelUpdater.unk2C[i] = mOuterParam->getIntFirFilter(i);
			mChannelUpdater.unk61 = 8;
		}

		for (u8 i = 0; i < 4; ++i) {
			mChannelUpdater.unk3C[i]
			    = mTimedParam.mInnerParam.mIIRs[i].mCurrentValue * 32767.0f;
		}

		mChannelUpdater.unk61 |= 0x20;
		mChannelUpdater.unk4C
		    = mTimedParam.mInnerParam.unk50.mCurrentValue * 32767.0f;
	}
}

void TTrack::updateTrack(u32 param)
{
	f32 curVolume;
	f32 curPitch;
	f32 curPan;
	f32 curFxmix;
	f32 curDolby;
	f32 fVar3 = mRegisterParam.mPanPower[3] / 32767.0f;

	if (unk3BC == 4)
		return;

	if (param & 0x20000) {
		u8 cVar8  = 0;
		f32 thing = mTimedParam.mInnerParam.unk110.mCurrentValue * 128.0f;
		s8 local_4c;
		OSf32tos8(&thing, &local_4c);
		if (local_4c < 0) {
			cVar8    = -local_4c;
			local_4c = 0;
		}

		mChannelUpdater.unk5A[0] = cVar8;
		mChannelUpdater.unk5A[1] = local_4c;
	}

	if ((param & 0x40) && !unk2C0)
		updateTempo();

	if (param & 1) {
		curVolume = mTimedParam.mInnerParam.mVolume.mCurrentValue;
		if (!unk3C3)
			curVolume *= curVolume;

		if (unk3C2)
			curVolume = 0.0f;

		if (mOuterParam && mOuterParam->checkOuterSwitch(1))
			curVolume *= mOuterParam->unk4;

		if (unk3CD && (unk3C1 & 1))
			curVolume *= mTimedParam.mInnerParam.unk100.mCurrentValue;
	}

	if (param & 2) {
		curPitch = Player::pitchToCent(
		    mTimedParam.mInnerParam.mPitch.mCurrentValue, mRegisterParam.unkE);

		if (mOuterParam && mOuterParam->checkOuterSwitch(2))
			curPitch *= mOuterParam->unk8;
	}

	if (param & 8) {
		curPan = mTimedParam.mInnerParam.mPan.mCurrentValue;

		if (mOuterParam && mOuterParam->checkOuterSwitch(8))
			curPan = panCalc(curPan, mOuterParam->unk14, fVar3, unk3C5[0]);
	}

	if (param & 4) {
		curFxmix = mTimedParam.mInnerParam.mFxmix.mCurrentValue;

		if (mOuterParam && mOuterParam->checkOuterSwitch(4))
			curFxmix = panCalc(curFxmix, mOuterParam->unkC, fVar3, unk3C5[1]);
	}

	if (param & 0x10) {
		curDolby = mTimedParam.mInnerParam.mDolby.mCurrentValue;

		if (mOuterParam && mOuterParam->checkOuterSwitch(0x10))
			curDolby = panCalc(curDolby, mOuterParam->unk10, fVar3, unk3C5[2]);
	}

	if (param & 0xf000) {
		for (u8 i = 0; i < 4; ++i)
			mChannelUpdater.unk3C[i]
			    = mTimedParam.mInnerParam.mIIRs[i].mCurrentValue * 32767.0f;

		mChannelUpdater.unk61 |= 0x20;
	}

	if (mOuterParam && (param & 0x80) && mOuterParam->checkOuterSwitch(0x80)) {
		for (u8 i = 0; i < 8; ++i)
			mChannelUpdater.unk2C[i] = mOuterParam->getIntFirFilter(i);

		mChannelUpdater.unk61 = (mChannelUpdater.unk61 & 0x20) + 8;
	}

	if (param & 0x20) {
		mChannelUpdater.unk4C
		    = mTimedParam.mInnerParam.unk50.mCurrentValue * 32767.0f;
	}

	for (int i = 0; i < 2; ++i) {
		if (unk3A0[i] == 0xE) {
			f32 off = unk33C[i].getOffsetNoCount();
			switch (unk33C[i].getOsc()->unk0) {
			case 1:
				curPitch *= off;
				break;
			case 0:
				curVolume *= off;
				break;
			case 2:
				curPan *= off;
				break;
			case 3:
				curFxmix *= off;
				break;
			case 4:
				curDolby *= off;
				break;
			}
		}
	}

	if (!unk2C0 || (unk3BC & 1)) {
		if (param & 1)
			mChannelUpdater.unk18 = curVolume;
		if (param & 2)
			mChannelUpdater.unk1C = curPitch;
		if (param & 8)
			mChannelUpdater.unk20 = curPan;
		if (param & 4)
			mChannelUpdater.unk24 = curFxmix;
		if (param & 0x10)
			mChannelUpdater.unk28 = curDolby;
	} else {
		fVar3 = mRegisterParam.mPanPower[4] / 32767.0f;
		if (param & 1)
			mChannelUpdater.unk18 = unk2C0->mChannelUpdater.unk18 * curVolume;
		if (param & 2)
			mChannelUpdater.unk1C = unk2C0->mChannelUpdater.unk1C * curPitch;
		if (param & 8)
			mChannelUpdater.unk20 = panCalc(
			    curPan, unk2C0->mChannelUpdater.unk20, fVar3, unk3C8[0]);
		if (param & 4)
			mChannelUpdater.unk24 = panCalc(
			    curFxmix, unk2C0->mChannelUpdater.unk24, fVar3, unk3C8[1]);
		if (param & 0x10)
			mChannelUpdater.unk28 = panCalc(
			    curDolby, unk2C0->mChannelUpdater.unk28, fVar3, unk3C8[2]);
	}
}

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

#pragma dont_inline on
s8 TTrack::mainProc() { }
#pragma dont_inline off

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

void TTrack::writeTimeParam(u8 param)
{

	u8 bVar1 = mSeqCtrl.readByte();

	s16 r29;
	switch (param & 0xC) {
	case 0:
		r29 = readRegDirect(mSeqCtrl.readByte());
		break;
	case 4:
		r29 = mSeqCtrl.readByte();
		break;
	case 8: {
		u16 byte = mSeqCtrl.readByte();
		if (byte & 0x80)
			r29 = byte << 8;
		else
			r29 = byte << 8 | byte << 1;
		break;
	}
	case 0xC:
		r29 = mSeqCtrl.read16();
		break;
	}

	s32 iVar7 = 0;
	switch (param & 3) {
	case 0:
		iVar7 = -1;
		break;
	case 1:
		iVar7 = readRegDirect(mSeqCtrl.readByte());
		break;
	case 2:
		iVar7 = mSeqCtrl.readByte();
		break;
	case 3:
		iVar7 = mSeqCtrl.read16();
		break;
	}

	MoveParam_* moveParam   = &mTimedParam.mMoveParams[bVar1];
	moveParam->mTargetValue = r29 / 32768.0f;
	if (iVar7 <= 0) {
		moveParam->mCurrentValue = moveParam->mTargetValue;
		moveParam->mMoveAmount   = 0.0f;
		moveParam->mMoveTime     = 1.0f;
	} else {
		moveParam->mMoveAmount
		    = (moveParam->mTargetValue - moveParam->mCurrentValue) / iVar7;
		moveParam->mMoveTime = iVar7;
	}
}

// TODO: This is pure pain
void TTrack::writeRegParam(u8 param)
{

	u8 bVar9 = param & 0xC;
	u8 bVar8 = param & 0x3;

	u16 r26;

	if ((param & 0xF) == 0xB) {
		bVar9 = 0;
		bVar8 = 0xB;
	}

	if ((param & 0xF) == 0xA) {
		bVar8 = 10;
		param = mSeqCtrl.readByte();
		bVar9 = param & 0xC;
		r26   = (param >> 4) + 4;
	}

	if ((param & 0xF) == 0x9) {
		bVar8 = mSeqCtrl.readByte();
		bVar9 = bVar8 & 0xC;
		bVar8 &= 0xF0;
		if (bVar9 == 8)
			bVar9 = 0x10;
	}

	u8 bVar1 = mSeqCtrl.readByte();
	u32 r25;

	if (bVar8 == 10)
		r25 = readReg32(mSeqCtrl.readByte());

	s32 r24;

	switch (bVar9) {
	case 0:
		r24 = readRegDirect(mSeqCtrl.readByte());
		break;
	case 4:
		r24 = mSeqCtrl.readByte();
		break;
	case 8: {
		u16 byte = mSeqCtrl.readByte();
		if (byte & 0x80)
			r24 = byte << 8;
		else
			r24 = byte << 8 | byte << 1;
		break;
	}
	case 0xC:
		r24 = mSeqCtrl.read16();
		break;
	case 0x10:
		r24 = 0xffff;
		break;
	}

	s32 uVar5 = readRegDirect(bVar1);

	switch (bVar8) {
	case 0x1:
		if (bVar9 == 4)
			r24 = Player::extend8to16(r24);
		r24 = uVar5 + r24;
		break;
	case 0x2:
		writeRegDirect(4, (uVar5 * r24) >> 0x10);
		writeRegDirect(5, uVar5 * r24);
		break;
	case 0x3:
		mRegisterParam.unk0[3] = uVar5 - r24;
		break;
	case 0xA:
		r25 = loadTbl(r25, r24, r26);
		r24 = r25;
		break;
	case 0x10:
		if (bVar9 == 4)
			r24 = Player::extend8to16(r24);
		if (r24 < 0)
			r24 = uVar5 >> -r24;
		else
			r24 = uVar5 << r24;
		break;
	case 0x20:
		if (bVar9 == 4)
			r24 = Player::extend8to16(r24);
		if (r24 < 0)
			r24 = uVar5 >> -r24;
		else
			r24 = uVar5 << r24;
		break;
	case 0x30:
		r24 = uVar5 & r24;
		break;
	case 0x40:
		r24 = uVar5 | r24;
		break;
	case 0x50:
		r24 = uVar5 ^ r24;
		break;
	case 0x60:
		r24 = -uVar5;
		break;
	case 0x90:
		r25 = Player::getRandomS32();
		r24 = r25 - (r25 / r24) * r24;
		break;
	}

	u32 uVar10 = bVar1;
	switch (uVar10) {
	case 0x1F:
		uVar5 = r24;
		if (uVar10 < 3) {
			uVar5 = Player::extend8to16(r24);
			r24 &= 0xff;
		}
		break;
	case 0x21:
		r24    = (mRegisterParam.getBankNumber() & 0xff) << 8 | r24 & 0xff;
		uVar10 = 6;
		break;
	case 0x20:
		r24    = mRegisterParam.getProgramNumber() | (r24 << 8);
		uVar10 = 6;
		break;
	case 0x2E:
		uVar10 = 0xd;
		r24    = mRegisterParam.unk1A & 0xff00 | r24 & 0xff;
		break;
	case 0x27:
		uVar5 = r24;
		if (uVar10 < 0x2C && uVar10 > 0x27)
			mRegisterParam.mPanPower[uVar10 - 0x28] = r25;
		break;
	case 0x22:
		writeRegDirect(0, r24 >> 8);
		r24 &= 0xff;
		uVar10 = 1;
		uVar5  = r24;
		break;
	}

	mRegisterParam.unk0[uVar10] = r24;
	mRegisterParam.unk0[3]      = uVar5;

	if (uVar10 == 6) {
		if (unk3A0[0] != 0xE)
			unk3A0[0] = 0xF;
		if (unk3A0[1] != 0xE)
			unk3A0[1] = 0xF;
	}

	if (uVar10 == 7)
		unk3B4 |= 2;

	if (uVar10 == 0xD) {
		mChannelUpdater.unk68 = mRegisterParam.unk1A | 0x10000;
		mChannelUpdater.unk6C = 0;
	}
}

int TTrack::setSeqData(u8* data, s32 size, Player::SEQ_PLAYMODE mode)
{
	int result = TrackMgr::allocNewRoot(this);
	if (result == -1)
		return -1;

	unk308 = result;
	unk3BC = 3;
	initTrack(data, 0, nullptr);
	mChannelUpdater.initAllocChannel(0);
	unk3AC = 0.0f;
	unk3B0 = 1.0f;
	updateTrackAll();
	unk3C4 = 2;

	return result;
}

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

void TTrack::allNoteOff()
{
	if (!unk2C0)
		for (u8 i = 0; i < 8; ++i)
			noteOff(i, 10);
	else
		for (u8 i = 0; i < 8; ++i)
			noteOff(i, 0);
}

bool TTrack::closeTrack()
{
	if (!unk3C4)
		return false;

	allNoteOff();

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

u16 TTrack::readRegDirect(u8 reg)
{
	u16 result;
	int i;

	switch (reg) {
	case 32:
		result = mRegisterParam.getBankNumber();
		break;

	case 33:
		result = mRegisterParam.getProgramNumber();
		break;

	case 34:
		result = readRegDirect(0);
		result <<= 8;
		result |= readRegDirect(1);
		break;

	case 44:
		result = 0;
		for (i = 15; i >= 0; --i) {
			TTrack* child = unk2C4[i];
			result <<= 1;
			if (child && child->unk3C4)
				result |= 1;
		}
		break;

	case 45:
		result = 0;
		for (i = 7; i >= 0; --i) {
			result <<= 1;
			TChannel* chan = mNoteMgr.getChannel(i);
			u8 bit;
			if (!chan)
				bit = 1;
			else if (chan->unk1 == 0xff)
				bit = 1;
			else
				bit = 0;
			result |= bit;
		}
		break;

	case 48:
		result = mSeqCtrl.mLoopIndex == 0
		             ? (u16)0
		             : mSeqCtrl.mLoopTimers[mSeqCtrl.mLoopIndex - 1];
		break;

	default:
		result = mRegisterParam.unk0[reg];
		break;
	}

	return result;
}

void TTrack::writeRegDirect(u8 reg, u16 value)
{
	u16 top;
	u16 uVar1;
	u8 r30 = reg;
	u16 r4;

	switch (reg) {
	case 0:
	case 1:
	case 2:
		value &= 0xff;
		r4 = Player::extend8to16(value & 0xff);
		break;

	case 32:
	case 33:
		return;

	case 34: {
		top                    = value >> 8;
		uVar1                  = Player::extend8to16(top);
		mRegisterParam.unk0[0] = top;
		mRegisterParam.unk0[3] = uVar1;

		r4    = value;
		value = value & 0xff;
		r30   = 1;
		break;
	}

	default:
		r4 = value;
		break;
	}

	mRegisterParam.unk0[r30] = value;
	mRegisterParam.unk0[3]   = r4;
}

void TTrack::setTrackExtPanPower(f32 power) { }

bool TTrack::writePortAppDirect(u32 port, u16 value)
{
	mTrackPort.mImportFlag[port] = 1;
	mTrackPort.mValue[port]      = value;
	if (port <= 1)
		mIntrMgr.request(port == 0 ? 3 : 4);
	return true;
}

bool TTrack::readPortAppDirect(u32 port, u16* value)
{
	mTrackPort.mExportFlag[port] = 0;
	*value                       = mTrackPort.mValue[port];
	return true;
}

void TTrack::setExtFirFilterD(s16* filter)
{
	if (mOuterParam)
		mOuterParam->setFirFilter(filter);
}

TTrack* TTrack::routeTrack(u32 route)
{
	TTrack* found  = this;
	u32 iterations = route >> 28;
	for (int i = 0; i < iterations; ++i, route >>= 4) {
		found = found->unk2C4[route & 0xF];

		if (found)
			continue;

		found = nullptr;
		break;
	}
	return found;
}

bool TTrack::writePortApp(u32 port, u16 value)
{
	TTrack* found = routeTrack(port);

	if (!found)
		return false;

	u32 realPort = (port >> 16) & 0xFF;

	found->mTrackPort.mImportFlag[realPort] = 1;
	found->mTrackPort.mValue[realPort]      = value;

	if (realPort <= 1)
		found->mIntrMgr.request(realPort == 0 ? 3 : 4);
	return true;
}

bool TTrack::readPortApp(u32 port, u16* value)
{
	TTrack* found = routeTrack(port);

	if (!found)
		return false;

	u32 realPort = (port >> 16) & 0xFF;

	found->mTrackPort.mExportFlag[realPort] = 0;
	*value = found->mTrackPort.mValue[realPort];

	return true;
}

void TTrack::checkExportApp(u32 port) { }

void TTrack::checkImportApp(u32 port) { }

void TTrack::pauseTrack(u8 flag)
{
	unk3CD = 1;
	if (unk3C1 & 0x1)
		unk3B4 |= 1;

	if (unk3C1 & 0x4) {
		for (u8 i = 0; i < 8; ++i) {
			TChannel* chan = mNoteMgr.getChannel(i);
			if (chan) {
				chan->stop(10);
				mNoteMgr.releaseChannel(i);
			}
		}
	}

	if (unk3C1 & 8) {
		for (u8 i = 0; i < 8; ++i) {
			TChannel* chan = mNoteMgr.getChannel(i);
			if (chan)
				chan->setPauseFlag(1);
		}
	}

	mIntrMgr.request(0);

	if (flag == 1) {
		for (u32 i = 0; i < 16; ++i) {
			if (unk2C4[i] && unk2C4[i]->unk3C4)
				unk2C4[i]->pauseTrack(1);
		}
	}
}

void TTrack::unPauseTrack(u8 flag)
{
	unk3CD = 0;
	unk3B4 |= 1;

	for (u8 i = 0; i < 8; ++i) {
		TChannel* chan = mNoteMgr.getChannel(i);
		if (chan)
			chan->setPauseFlag(0);
	}

	mIntrMgr.request(1);

	if (flag == 1) {
		for (u8 i = 0; i < 16; ++i)
			if (unk2C4[i] && unk2C4[i]->unk3C4)
				unk2C4[i]->unPauseTrack(1);
	}
}

void TTrack::pauseTrackAll() { pauseTrack(1); }

void TTrack::unPauseTrackAll() { unPauseTrack(1); }

void TTrack::reset()
{
	unk3C4 = 0;
	unk3C2 = 0;
}

f32 TTrack::panCalc(f32 param1, f32 param2, f32 param3, u8 param4)
{
	f32 result;
	switch (param4) {
	case 0:
		return param1;
	case 1:
		return param2;
	case 2:
		result = param1 * (1.0f - param3) + param2 * param3;
	}
	return result;
}

s32 TTrack::rootCallback(void* param)
{
	TTrack* self = static_cast<TTrack*>(param);
	if (self && self->unk3C4 != 0) {
		if (self->unk3C4 == 3) {
			self->updateSeq(0, true);
			self->closeTrack();
			TrackMgr::deAllocRoot(self);
			return -1;
		} else {
			self->unk3AC += self->unk3B0;

			// TODO: this if is completely wrong, control flow is crazy here,
			// probably an inline?
			if (self->unk3AC > 1.0f) {
				self->updateSeq(0, true);
				while (self->unk3AC >= 1.0f) {
					self->unk3AC -= 1.0f;
					if ((int)self->mainProc() != -1)
						continue;

					self->updateSeq(0, true);
					self->closeTrack();
					TrackMgr::deAllocRoot(self);
					return -1;
				}
			}

			self->incSelfOsc();
		}
	} else {
		return -1;
	}

	return 0;
}

void TTrack::updateSyncSw(u8 param) { }

bool TTrack::registerTrackCallback(u16 (*callback)(TTrack*, u16))
{
	sCallBackFunc = callback;
	return true;
}

} // namespace JASystem
