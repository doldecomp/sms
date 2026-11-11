#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <JSystem/JAudio/JASystem/JASPlayer_impl.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASBankMgr.hpp>
#include <JSystem/JAudio/JASystem/JASRate.hpp>
#include <JSystem/JAudio/JASystem/JASCallback.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <JSystem/JAudio/JASystem/JASChGlobal.hpp>
#include <JSystem/JAudio/JASystem/JASSeqParser.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JSupport.hpp>
#include <dolphin/os.h>
#include <macros.h>

namespace JASystem {

namespace Player {
	static JMath::TRandom_enough_ oRandom(0);
}

TSeqParser TTrack::sParser;

u8 TTrack::sUpdateSyncMode                 = 0;
u16 (*TTrack::sCallBackFunc)(TTrack*, u16) = nullptr;

// Indexed by TOscillator::Osc_::mTarget (0 volume, 1 pitch, 2 pan,
// 3 fxmix, 4 dolby).
u8 TTrack::sOscTable[5]
    = { TTrack::UPDATE_Volume, TTrack::UPDATE_Pitch, TTrack::UPDATE_Pan,
	    TTrack::UPDATE_Fxmix, TTrack::UPDATE_Dolby };

TTrack::TTrack()
    : mParent(0)
    , mOuterParam(0)
    , mTrackRoute(0)
    , mConnectName(0)
{
	mTickCounter        = 0.0f;
	mTickRate           = 0.0f;
	mPendingUpdateFlags = 0;
	mTempo              = 0x78;
	mTimeBase           = 0x78;

	mTrackMode           = TRACK_MODE_INHERIT;
	mOverrideParentTempo = false;

	mInnerMemory    = 0;
	mTranspose      = 0;
	mTransposeTotal = 0;
	mPauseStatus    = 0;
	mMute           = 0;
	mVolumeMode     = 0;
	mSeqState       = 0;

	mTimeRelate = 0;
	mConnected  = 0;
	mPause      = 0;

	for (int i = 0; i < 16; ++i)
		mChildren[i] = nullptr;

	for (int i = 0; i < 3; ++i)
		mPanSwitchParent[i] = mPanSwitchExt[i] = 0;

	for (int i = 0; i < 12; ++i)
		mAdsTable[i] = Player::sAdsTable[i];
	for (int i = 0; i < 6; ++i)
		mRelTable[i] = Player::sRelTable[i];

	for (int i = 0; i < 2; ++i)
		mOscMode[i] = 0xf;

	mChannelUpdater.init();

	Calc::bzero(&mTimedParam, sizeof(mTimedParam));

	for (int i = 0; i < 2; ++i)
		mOscillators[i].setOsc(&mOscData[i]);
}

void TTrack::setInterrupt(u16 interrupt) { mIntrMgr.request(interrupt); }

bool TTrack::tryInterrupt()
{
	if (mSeqCtrl.isIntr())
		return false;

	void* intr = mIntrMgr.checkIntr();
	if (intr == nullptr)
		return false;

	return mSeqCtrl.callIntr(intr);
}

void TTrack::setBankNumber(u8 bank) { }

bool TTrack::assignExtBuffer(TOuterParam* buffer)
{
	if (buffer == nullptr)
		return false;
	mOuterParam = buffer;
	return true;
}

void TTrack::setInnerMemory(u8 param) { mInnerMemory = param; }

void TTrack::setPanSwitchExt(u8 param1, u8 param2)
{
	mPanSwitchExt[param2] = param1;
}

void TTrack::setPanSwitchParent(u8 param1, u8 param2)
{
	mPanSwitchParent[param2] = param1;
}

u8 TTrack::getBank() const { return mRegisterParam.getBankNumber(); }

u8 TTrack::getProgram() const { return mRegisterParam.getProgramNumber(); }

f32 TTrack::getVolume() const
{
	return mTimedParam.mInnerParam.mVolume.mCurrentValue;
}

f32 TTrack::getPitch() const
{
	return mTimedParam.mInnerParam.mPitch.mCurrentValue;
}

f32 TTrack::getPan() const
{
	return mTimedParam.mInnerParam.mPan.mCurrentValue;
}

f32 TTrack::getFxmix() const
{
	return mTimedParam.mInnerParam.mFxmix.mCurrentValue;
}

f32 TTrack::getDolby() const
{
	return mTimedParam.mInnerParam.mDolby.mCurrentValue;
}

u8 TTrack::getNoteStatus(u8 voice_slot) { return 0; }

void TTrack::releaseChannelAll()
{
	if (mParent)
		mParent->mChannelUpdater.receiveAllChannels(&mChannelUpdater);
	else
		ChGlobal::releaseAll(&mChannelUpdater);
}

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

int TTrack::noteOn(u8 voice_slot, s32 key, s32 velocity, s32 gate_time)
{
	if (mMute && (mPauseStatus & 0x40))
		return -1;

	u32 voiceSlot = voice_slot;
	if (mNoteMgr.getChannel(voiceSlot))
		noteOff(voice_slot, 0);

	TTrack* parent       = mParent;
	TChannelMgr* chanMgr = &mChannelUpdater;

	TTrack* it = parent;
	while (!chanMgr->mManagedChannels || !chanMgr->mFreeChannels) {
		if (it == nullptr) {
			chanMgr = &mChannelUpdater;
			break;
		}
		// BUG: should be it->mChannelUpdater?
		chanMgr = &parent->mChannelUpdater;
		it      = it->getParent();
	}

	u32 reg;

	TChannel* borrowed;
	if (mTrackMode == TRACK_MODE_PARENT_CHANNELS) {
		if (parent == nullptr)
			return -1;

		if (chanMgr != &parent->mChannelUpdater) {
			borrowed = chanMgr->getListHead(TChannelMgr::CHAN_LIST_FREE);
			if (borrowed) {
				--chanMgr->mManagedChannels;
				// BUG: should be parent->mChannelUpdater.addListHead
				mChannelUpdater.addListHead(borrowed,
				                            TChannelMgr::CHAN_LIST_FREE);
				++parent->mChannelUpdater.mManagedChannels;
				borrowed->mManager = &parent->mChannelUpdater;
			}
			chanMgr = &parent->mChannelUpdater;
		}
	} else {
		if (chanMgr != &mChannelUpdater) {
			borrowed = chanMgr->getListHead(TChannelMgr::CHAN_LIST_FREE);
			if (borrowed) {
				--chanMgr->mManagedChannels;
				mChannelUpdater.addListHead(borrowed,
				                            TChannelMgr::CHAN_LIST_FREE);
				++mChannelUpdater.mManagedChannels;
				borrowed->mManager = &mChannelUpdater;
			}
			chanMgr = &mChannelUpdater;
		}
	}

	reg       = readRegDirect(6);
	u8 bankNo = BankMgr::getPhysicalNumber(reg >> 8);
	u8 progNo = reg & 0xff;
	TChannel* chan
	    = BankMgr::noteOn(chanMgr, bankNo, progNo, key, velocity, gate_time);

	if (!chan)
		return -1;

	mNoteMgr.setChannel(voiceSlot, chan);

	chan->setPanPower(
	    mRegisterParam.getPanPowerBank(), mRegisterParam.getPanPowerExt(),
	    mRegisterParam.getPanPowerOsc(), mRegisterParam.getPanPowerParent());

	for (u8 i = 0; i < 2; ++i) {
		u32 oscMode = mOscMode[i];
		if (oscMode != 0xF && oscMode != 0xE) {
			if (oscMode >= 8) {
				oscMode -= 8;
				if (chan->isOsc(oscMode))
					chan->copyOsc(oscMode, &mOscData[i]);
			} else if (oscMode >= 4) {
				oscMode -= 4;
				s16* v = mOscData[i].mRelTable;
				if (chan->isOsc(oscMode)) {
					chan->copyOsc(oscMode, &mOscData[i]);
					mOscData[i].mRelTable = v;
				}
			}
			chan->overwriteOsc(oscMode, &mOscData[i]);
		}
	}

	if (sUpdateSyncMode == 0)
		updateTrack(UPDATE_Volume | UPDATE_Pitch | UPDATE_Pan);

	chan->resetInitialVolume();

	return 0;
}

bool TTrack::noteOff(u8 voice_slot, u16 release_time)
{
	TChannel* chan = mNoteMgr.getChannel(voice_slot);
	if (!chan)
		return false;

	if (release_time == 0)
		chan->stop(0);
	else
		chan->stop(release_time);

	mNoteMgr.releaseChannel(voice_slot);
	return true;
}

int TTrack::gateOn(u8 voice_slot, s32 key, s32 velocity, s32 gate_time)
{
	TChannel* channel = mNoteMgr.getChannel(voice_slot);
	if (!channel)
		return -1;

	BankMgr::gateOn(channel, key, velocity, gate_time);
	return 0;
}

bool TTrack::checkNoteStop(s32 voice_slot)
{
	TChannel* chan = mNoteMgr.getChannel(voice_slot);
	if (chan == nullptr)
		return true;

	if (chan->mKey == 0xff)
		return true;

	return false;
}

void TTrack::oscSetupFull(u8 param_1, u32 param_2, u32 param_3)
{
	u32 var1  = (param_1 & 0x10) >> 4;
	int var2  = param_1 & 0x0f;
	bool var3 = param_1 & 0x80 ? true : false;
	bool var4 = param_1 & 0x40 ? true : false;
	bool var5 = param_1 & 0x20 ? true : false;

	if (var3) {
		mOscData[var1]         = Player::sEnvelopeDef;
		mOscData[var1].mTarget = var2;
		switch (var2) {
		case EFFECT_PITCH:
			mOscData[var1].mBias = 1.0f;
			break;
		}
	}

	if (var4) {
		if (param_2 == 0)
			mOscData[var1].mAdsTable = nullptr;

		mOscData[var1].mAdsTable = (s16*)(mSeqCtrl.getBase() + param_2);
	}

	if (!var5)
		return;

	if (param_3 == 0)
		mOscData[var1].mRelTable = Player::sRelTable;

	mOscData[var1].mRelTable = (s16*)(mSeqCtrl.getBase() + param_3);
}

void TTrack::oscSetupSimpleEnv(u8 param_1, u32 param_2)
{
	switch (param_1) {
	case 0:
		mOscData[0]           = Player::sEnvelopeDef;
		mOscData[0].mAdsTable = (s16*)(mSeqCtrl.getBase() + param_2);
		break;
	case 1:
		mOscData[0].mRelTable = (s16*)(mSeqCtrl.getBase() + param_2);
		break;
	}
}

void TTrack::oscUpdateParam(u8 param_id, f32 value)
{
	switch (param_id) {
	case 6:
		mOscData[0].mDepth = value;
		break;
	case 7:
		mOscData[0].mRate = value;
		break;
	case 8:
		mOscData[0].mBias = value;
		break;
	case 9:
		mOscData[1].mDepth = value;
		break;
	case 10:
		mOscData[1].mRate = value;
		break;
	case 11:
		mOscData[1].mBias = value;
		break;
	}
}

void TTrack::oscSetupSimple(u8 param_1)
{
	switch (param_1) {
	case 0:
		mOscData[1] = Player::sVibratoDef;
		break;
	case 1:
		mOscData[0] = Player::sTremoroDef;
		break;
	case 2:
		mOscData[1] = Player::sTremoroDef;
		break;
	}
}

void TTrack::updateTrackAll()
{
	f32 fVar6 = mRegisterParam.getPanPowerParent() / 32767.0f;
	f32 curVolume;
	f32 curPitch;
	f32 curFxmix;
	f32 curDolby;
	f32 curPan;

	if (mTrackMode == TRACK_MODE_PARENT_CHANNELS)
		return;

	mChannelUpdater.mPriorityConfig = mRegisterParam.getPriority() | 0x10000;
	mChannelUpdater.mPriorityTime   = 0;

	u8 cVar8  = 0;
	f32 thing = mTimedParam.mInnerParam.unk110.mCurrentValue * 128.0f;
	s8 local_4c;
	OSf32tos8(&thing, &local_4c);
	if (local_4c < 0) {
		cVar8    = -local_4c;
		local_4c = 0;
	}

	mChannelUpdater.mMixerDelayMax        = 0x10;
	mChannelUpdater.mMixerDelaySamples[0] = cVar8;
	mChannelUpdater.mMixerDelaySamples[1] = local_4c;

	curVolume = mTimedParam.mInnerParam.mVolume.mCurrentValue;
	if (mVolumeMode == 0)
		curVolume = curVolume * curVolume;

	if (mMute != 0)
		curVolume = 0.0f;

	curPitch = Player::pitchToCent(mTimedParam.mInnerParam.mPitch.mCurrentValue,
	                               mRegisterParam.getBendSense());

	curPan   = mTimedParam.mInnerParam.mPan.mCurrentValue;
	curFxmix = mTimedParam.mInnerParam.mFxmix.mCurrentValue;
	curDolby = mTimedParam.mInnerParam.mDolby.mCurrentValue;

	if (mOuterParam) {
		if (mOuterParam->checkOuterSwitch(UPDATE_Volume))
			curVolume *= mOuterParam->getVolume();

		if (mOuterParam->checkOuterSwitch(UPDATE_Pitch))
			curPitch *= mOuterParam->getPitch();

		if (mOuterParam->checkOuterSwitch(UPDATE_Fxmix))
			curFxmix = panCalc(curFxmix, mOuterParam->getFxVol(), fVar6,
			                   mPanSwitchExt[1]);

		if (mOuterParam->checkOuterSwitch(UPDATE_Dolby))
			curDolby = panCalc(curDolby, mOuterParam->getDolby(), fVar6,
			                   mPanSwitchExt[2]);

		if (mOuterParam->checkOuterSwitch(UPDATE_Pan))
			curPan = panCalc(curPan, mOuterParam->getPan(), fVar6,
			                 mPanSwitchParent[0]);
	}

	if (!mParent || (mTrackMode & TRACK_MODE_INDEPENDENT_PARAMS)) {
		mChannelUpdater.mVolume = curVolume;
		mChannelUpdater.mPitch  = curPitch;
		mChannelUpdater.mPan    = curPan;
		mChannelUpdater.mFxmix  = curFxmix;
		mChannelUpdater.mDolby  = curDolby;
	} else {
		f32 fVar6               = mRegisterParam.getPanPowerTrack() / 32767.0f;
		mChannelUpdater.mVolume = mParent->mChannelUpdater.mVolume * curVolume;
		mChannelUpdater.mPitch  = mParent->mChannelUpdater.mPitch * curPitch;
		mChannelUpdater.mPan    = panCalc(curPan, mParent->mChannelUpdater.mPan,
		                                  fVar6, mPanSwitchParent[0]);
		mChannelUpdater.mFxmix
		    = panCalc(curFxmix, mParent->mChannelUpdater.mFxmix, fVar6,
		              mPanSwitchParent[1]);
		mChannelUpdater.mDolby
		    = panCalc(curDolby, mParent->mChannelUpdater.mDolby, fVar6,
		              mPanSwitchParent[2]);

		if (mOuterParam && mOuterParam->checkOuterSwitch(UPDATE_FirFilter)) {
			for (u8 i = 0; i < 8; ++i)
				mChannelUpdater.mFirCoeffs[i] = mOuterParam->getIntFirFilter(i);
			mChannelUpdater.mFilterMode = 8;
		}

		for (u8 i = 0; i < 4; ++i) {
			mChannelUpdater.mIirCoeffs[i]
			    = mTimedParam.mInnerParam.mIIRs[i].mCurrentValue * 32767.0f;
		}

		mChannelUpdater.mFilterMode
		    |= DSPInterface::DSPBuffer::FILTER_IIR_ENABLED;
		mChannelUpdater.mDistFilter
		    = mTimedParam.mInnerParam.unk50.mCurrentValue * 32767.0f;
	}
}

void TTrack::updateTrack(u32 update_flags)
{
	f32 curVolume;
	f32 curPitch;
	f32 curPan;
	f32 curFxmix;
	f32 curDolby;
	f32 fVar3 = mRegisterParam.getPanPowerParent() / 32767.0f;

	if (mTrackMode == TRACK_MODE_PARENT_CHANNELS)
		return;

	if (update_flags & UPDATE_Unk17) {
		u8 cVar8  = 0;
		f32 thing = mTimedParam.mInnerParam.unk110.mCurrentValue * 128.0f;
		s8 local_4c;
		OSf32tos8(&thing, &local_4c);
		if (local_4c < 0) {
			cVar8    = -local_4c;
			local_4c = 0;
		}

		mChannelUpdater.mMixerDelaySamples[0] = cVar8;
		mChannelUpdater.mMixerDelaySamples[1] = local_4c;
	}

	if ((update_flags & UPDATE_Tempo) && !mParent)
		updateTempo();

	if (update_flags & UPDATE_Volume) {
		curVolume = mTimedParam.mInnerParam.mVolume.mCurrentValue;
		if (!mVolumeMode)
			curVolume *= curVolume;

		if (mMute)
			curVolume = 0.0f;

		if (mOuterParam && mOuterParam->checkOuterSwitch(UPDATE_Volume))
			curVolume *= mOuterParam->getVolume();

		if (mPause && (mPauseStatus & 1))
			curVolume *= mTimedParam.mInnerParam.unk100.mCurrentValue;
	}

	if (update_flags & UPDATE_Pitch) {
		curPitch
		    = Player::pitchToCent(mTimedParam.mInnerParam.mPitch.mCurrentValue,
		                          mRegisterParam.getBendSense());

		if (mOuterParam && mOuterParam->checkOuterSwitch(UPDATE_Pitch))
			curPitch *= mOuterParam->getPitch();
	}

	if (update_flags & UPDATE_Pan) {
		curPan = mTimedParam.mInnerParam.mPan.mCurrentValue;

		if (mOuterParam && mOuterParam->checkOuterSwitch(UPDATE_Pan))
			curPan = panCalc(curPan, mOuterParam->getPan(), fVar3,
			                 mPanSwitchExt[0]);
	}

	if (update_flags & UPDATE_Fxmix) {
		curFxmix = mTimedParam.mInnerParam.mFxmix.mCurrentValue;

		if (mOuterParam && mOuterParam->checkOuterSwitch(UPDATE_Fxmix))
			curFxmix = panCalc(curFxmix, mOuterParam->getFxVol(), fVar3,
			                   mPanSwitchExt[1]);
	}

	if (update_flags & UPDATE_Dolby) {
		curDolby = mTimedParam.mInnerParam.mDolby.mCurrentValue;

		if (mOuterParam && mOuterParam->checkOuterSwitch(UPDATE_Dolby))
			curDolby = panCalc(curDolby, mOuterParam->getDolby(), fVar3,
			                   mPanSwitchExt[2]);
	}

	if (update_flags & UPDATE_IIRs) {
		for (u8 i = 0; i < 4; ++i)
			mChannelUpdater.mIirCoeffs[i]
			    = mTimedParam.mInnerParam.mIIRs[i].mCurrentValue * 32767.0f;

		mChannelUpdater.mFilterMode
		    |= DSPInterface::DSPBuffer::FILTER_IIR_ENABLED;
	}

	if (mOuterParam && (update_flags & UPDATE_FirFilter)
	    && mOuterParam->checkOuterSwitch(UPDATE_FirFilter)) {
		for (u8 i = 0; i < 8; ++i)
			mChannelUpdater.mFirCoeffs[i] = mOuterParam->getIntFirFilter(i);

		mChannelUpdater.mFilterMode
		    = (mChannelUpdater.mFilterMode
		       & DSPInterface::DSPBuffer::FILTER_IIR_ENABLED)
		      + 8;
	}

	if (update_flags & UPDATE_Unk5) {
		mChannelUpdater.mDistFilter
		    = mTimedParam.mInnerParam.unk50.mCurrentValue * 32767.0f;
	}

	for (int i = 0; i < 2; ++i) {
		if (mOscMode[i] == 0xE) {
			f32 off = mOscillators[i].getOffsetNoCount();
			switch (mOscillators[i].getEffectorKind()) {
			case EFFECT_PITCH:
				curPitch *= off;
				break;
			case EFFECT_VOLUME:
				curVolume *= off;
				break;
			case EFFECT_PAN:
				curPan *= off;
				break;
			case EFFECT_FXMIX:
				curFxmix *= off;
				break;
			case EFFECT_DOLBY:
				curDolby *= off;
				break;
			}
		}
	}

	if (!mParent || (mTrackMode & TRACK_MODE_INDEPENDENT_PARAMS)) {
		if (update_flags & UPDATE_Volume)
			mChannelUpdater.mVolume = curVolume;
		if (update_flags & UPDATE_Pitch)
			mChannelUpdater.mPitch = curPitch;
		if (update_flags & UPDATE_Pan)
			mChannelUpdater.mPan = curPan;
		if (update_flags & UPDATE_Fxmix)
			mChannelUpdater.mFxmix = curFxmix;
		if (update_flags & UPDATE_Dolby)
			mChannelUpdater.mDolby = curDolby;
	} else {
		fVar3 = mRegisterParam.getPanPowerTrack() / 32767.0f;
		if (update_flags & UPDATE_Volume)
			mChannelUpdater.mVolume
			    = mParent->mChannelUpdater.mVolume * curVolume;
		if (update_flags & UPDATE_Pitch)
			mChannelUpdater.mPitch = mParent->mChannelUpdater.mPitch * curPitch;
		if (update_flags & UPDATE_Pan)
			mChannelUpdater.mPan
			    = panCalc(curPan, mParent->mChannelUpdater.mPan, fVar3,
			              mPanSwitchParent[0]);
		if (update_flags & UPDATE_Fxmix)
			mChannelUpdater.mFxmix
			    = panCalc(curFxmix, mParent->mChannelUpdater.mFxmix, fVar3,
			              mPanSwitchParent[1]);
		if (update_flags & UPDATE_Dolby)
			mChannelUpdater.mDolby
			    = panCalc(curDolby, mParent->mChannelUpdater.mDolby, fVar3,
			              mPanSwitchParent[2]);
	}
}

void TTrack::updateTempo()
{
	if (mParent == 0) {
		mTickRate = mTimeBase;
		mTickRate *= mTempo;
		mTickRate /= Kernel::getDacRate();
		mTickRate *= 80.0f;
		mTickRate /= 60.0f;
		if (mOuterParam->checkOuterSwitch(UPDATE_Tempo))
			mTickRate *= mOuterParam->getTempo();
	} else {
		mTickRate = mParent->mTickRate;
		mTimeBase = mParent->mTimeBase;
	}

	for (u32 i = 0; i < 16; ++i)
		if (mChildren[i] && mChildren[i]->mSeqState)
			mChildren[i]->updateTempo();
}

void TTrack::updateSeq(u32 update_flags, bool recurse_now)
{
	u32 totalUpdateFlags = update_flags | mPendingUpdateFlags;
	if (mOuterParam) {
		totalUpdateFlags |= mOuterParam->getOuterUpdate();
		mOuterParam->setOuterUpdate(0);
	}

	mPendingUpdateFlags = 0;
	if (totalUpdateFlags != 0)
		updateTrack(totalUpdateFlags);

	for (int i = 0; i < 16; ++i) {
		TTrack* child = mChildren[i];
		if (child && child->mSeqState) {
			if (recurse_now)
				child->updateSeq(totalUpdateFlags, recurse_now);
			else
				child->mPendingUpdateFlags |= totalUpdateFlags;
		}
	}
}

void TTrack::incSelfOsc()
{
	for (int i = 0; i < 2; ++i)
		if (mOscMode[i] == 0xE)
			mOscillators[i].getOffset();

	for (int i = 0; i < 16; ++i)
		if (mChildren[i])
			mChildren[i]->incSelfOsc();
}

s8 TTrack::mainProc()
{
	if (mParent && mOverrideParentTempo == true) {
		f32 tempo = mTempo;
		tempo /= mParent->mTempo;
		if (tempo > 1.0f)
			tempo = 1.0f;
		mTickCounter += tempo;
		if (mTickCounter < 1.0f)
			return 0;
		mTickCounter -= 1.0f;
	}

	if (mParent && mChannelUpdater.mManagedChannels != 0) {
		TChannel* head
		    = mChannelUpdater.getListHead(TChannelMgr::CHAN_LIST_FREE);
		if (head) {
			mParent->mChannelUpdater.addListHead(head,
			                                     TChannelMgr::CHAN_LIST_FREE);
			head->mManager = &mParent->mChannelUpdater;
			--mChannelUpdater.mManagedChannels;
			++mParent->mChannelUpdater.mManagedChannels;
		}
	}

	mTransposeTotal = mTranspose;
	if (mParent)
		mTransposeTotal += mParent->mTransposeTotal;
	mIntrMgr.request(7);
	mIntrMgr.timerProcess();

	s32 retcode;
	do {
		tryInterrupt();

		if (mPause != 0 && (mPauseStatus & 2))
			goto bail; // TODO: is this goto real? looks quite real to me

		if (mSeqCtrl.getWait() == -1) {
			if (!checkNoteStop(0))
				break;
			mSeqCtrl.wait(0);
		}

		if (mSeqCtrl.getWait() > 0) {
			if (!mSeqCtrl.waitCountDown())
				break;
			mNoteMgr.endProcess();
		}

		retcode = sParser.mainProc(this, &mSeqCtrl);

		if (retcode == -1)
			return -1;
	} while (retcode == -2);

	u32 r31 = 0;
	for (int i = 0; i != TIMED_Count; ++i) {
		MoveParam_* param = &mTimedParam.mMoveParams[i];
		if (param->mMoveTime > 0.0f) {
			param->mCurrentValue += param->mMoveAmount;
			param->mMoveTime -= 1.0f;
			if (i <= TIMED_Unk5 || i >= TIMED_Osc1_Bias) {
				r31 |= 1 << i;
			} else {
				f32 cur = param->mCurrentValue;
				switch ((u8)i) {
				case TIMED_Osc0_Depth:
					mOscData[0].mDepth = cur;
					break;
				case TIMED_Osc0_Rate:
					mOscData[0].mRate = cur;
					break;
				case TIMED_Osc0_Bias:
					mOscData[0].mBias = cur;
					break;
				case TIMED_Osc1_Depth:
					mOscData[1].mDepth = cur;
					break;
				case TIMED_Osc1_Rate:
					mOscData[1].mRate = cur;
					break;
				case TIMED_Osc1_Bias:
					mOscData[1].mBias = cur;
					break;
				}
			}
		}
	}

	for (int i = 0; i < 2; ++i)
		if (mOscMode[i] == 0xE)
			r31 |= sOscTable[mOscData[i].mTarget];

bail:
	mPendingUpdateFlags |= r31;

	updateSeq(0, false);

	for (int i = 0; i < 16; ++i) {
		TTrack* child = mChildren[i];
		if (child && child->mSeqState) {
			if (child->mainProc() == -1) {
				child->closeTrack();
				mChildren[i] = nullptr;
			}
		}
	}

	return 0;
}

int TTrack::seqTimeToDspTime(s32 param_1, u8 param_2)
{
	f32 res = param_1;
	res *= param_2;
	res /= 100.0f;
	if (mTimeRelate) {
		res /= mTickRate;
	} else {
		res = (res * 120.0f) / mTimeBase;
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

void TTrack::writeRegParam(u8 param)
{

	u32 bVar9 = param & 0xC;
	u32 bVar8 = param & 0x3;

	u32 r26;

	if ((param & 0xF) == 0xB) {
		bVar9 = 0;
		bVar8 = 0xB;
	}

	if ((param & 0xF) == 0xA) {
		bVar8 = 10;
		param = mSeqCtrl.readByte();
		bVar9 = param & 0xC;
		r26   = ((param >> 4) & 0xF) + 4;
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

	s16 r24;

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

	s16 uVar5 = readRegDirect(bVar1);

	switch (bVar8) {
	case 0x0:
		break;
	case 0x1:
		if (bVar9 == 4)
			r24 = Player::extend8to16(r24);
		r24 = uVar5 + r24;
		break;
	case 0x2: {
		u32 product = uVar5;
		product *= r24;
		writeRegDirect(4, product >> 0x10);
		writeRegDirect(5, JSULoHalf(product));
		return;
	}

	case 0x3:
		mRegisterParam.setFlag(uVar5 - r24);
		return;
	case 0xB:
		r24 = uVar5 - r24;
		break;
	case 0x10:
		if (bVar9 == 4)
			r24 = Player::extend8to16(r24);
		if (r24 < 0)
			r24 = (u16)uVar5 >> -r24;
		else
			r24 = (u16)uVar5 << r24;
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
		r24 = r25 % (u16)r24;
		break;
	case 0xA:
		r25 = loadTbl(r25, r24, r26);
		r24 = (u16)r25;
		break;
	}

	s16 uVar6;
	switch (bVar1) {
	case 0:
	case 1:
	case 2:
		r24   = (u8)r24;
		uVar6 = Player::extend8to16(r24);
		break;

	case 0x21:
		r24   = (u8)r24 | (mRegisterParam.getBankNumber() << 8);
		bVar1 = 6;
		break;

	case 0x20:
		r24   = (r24 << 8) | (u8)mRegisterParam.getProgramNumber();
		bVar1 = 6;
		break;

	case 0x2E:
		r24   = (mRegisterParam.getPriority() & 0xff00) | JSULoByte(r24);
		bVar1 = 0xd;
		break;

	case 0x2F:
		r24   = (r24 << 8) | JSULoByte(mRegisterParam.getPriority());
		bVar1 = 0xd;
		break;

	case 0x22:
		writeRegDirect(0, r24 >> 8);
		r24   = r24 & 0xff;
		uVar6 = r24;
		bVar1 = 1;
		break;

	case 0x28:
	case 0x29:
	case 0x2A:
	case 0x2B:
		mRegisterParam.setAddress(bVar1 - 0x28, r25);
		return;

	default:
		uVar6 = r24;
		break;
	}

	mRegisterParam.mRegisters[bVar1] = r24;
	mRegisterParam.setFlag(uVar6);

	if (bVar1 == 6) {
		for (int i = 0; i < 2; i++) {
			if (mOscMode[i] != 0xE)
				mOscMode[i] = 0xF;
		}
	}

	if (bVar1 == 7)
		mPendingUpdateFlags |= UPDATE_Pitch;

	if (bVar1 == 0xD) {
		mChannelUpdater.mPriorityConfig
		    = mRegisterParam.getPriority() | 0x10000;
		mChannelUpdater.mPriorityTime = 0;
	}
}

int TTrack::setSeqData(u8* data, s32 size, Player::SEQ_PLAYMODE mode)
{
	int result = TrackMgr::allocNewRoot(this);
	if (result == -1)
		return -1;

	mTrackRoute = result;
	mTrackMode  = TRACK_MODE_INDEPENDENT;
	initTrack(data, 0, nullptr);
	mChannelUpdater.initAllocChannel(0);
	mTickCounter = 0.0f;
	mTickRate    = 1.0f;
	updateTrackAll();
	mSeqState = 2;

	return result;
}

bool TTrack::startSeq()
{
	switch (mSeqState) {
	case 0:
		return false;
	case 1:
		return false;
	case 3:
		return false;
	case 2:
		mSeqState = 1;
		break;
	}
	Kernel::registerSubframeCallback(&TTrack::rootCallback, this);
	return true;
}

void TTrack::stopSeqMain()
{
	updateSeq(0, true);
	closeTrack();
	TrackMgr::deAllocRoot(this);
}

bool TTrack::stopSeq()
{
	switch (mSeqState) {
	case 0:
		break;

	case 2:
		mSeqState = 0;
		if (mInnerMemory == 1)
			TrackMgr::backTrack(this);
		TrackMgr::deAllocRoot(this);
		break;

	default:
		mSeqState = 3;
		break;
	}
	return true;
}

void TTrack::allNoteOff()
{
	if (!mParent)
		for (u8 i = 0; i < 8; ++i)
			noteOff(i, 10);
	else
		for (u8 i = 0; i < 8; ++i)
			noteOff(i, 0);

	mNoteMgr.init();
}

bool TTrack::closeTrack()
{
	if (!mSeqState)
		return false;

	allNoteOff();
	mSeqState = 0;

	if (mInnerMemory == 1)
		TrackMgr::backTrack(this);

	for (u8 i = 0; i < 16; ++i)
		if (mChildren[i]) {
			mChildren[i]->closeTrack();
			mChildren[i] = nullptr;
		}

	mMute = 0;

	releaseChannelAll();

	if (mConnected) {
		TrackMgr::unRegistTrack(this);
		mConnected = 0;
	}

	return false;
}

void TTrack::muteTrack(u8 flag)
{
	mMute = flag;
	mPendingUpdateFlags |= UPDATE_Volume;
	if (mMute && (mPauseStatus & 0x20)) {
		for (u8 i = 0; i < 8; ++i) {
			TChannel* chan = mNoteMgr.getChannel(i);
			if (chan) {
				chan->stop(10);
				mNoteMgr.releaseChannel(i);
			}
		}
	}

	for (int i = 0; i < 16; ++i) {
		TTrack* track = mChildren[i];
		if (track)
			track->muteTrack(flag);
	}
}

void TTrack::muteChildTracks(u16 flags) { }

void TTrack::initTrack(void* data, u32 size, TTrack* parent)
{
	mSeqCtrl.init(data, size);
	if (parent == nullptr) {
		mTempo       = 0x78;
		mTimeBase    = 0x30;
		mTimeRelate  = 1;
		mPause       = 0;
		mPauseStatus = 10;
		mVolumeMode  = 0;
		mMute        = 0;
	} else {
		mTempo               = parent->mTempo;
		mOverrideParentTempo = false;
		mTickRate            = parent->mTickRate;
		mTimeBase            = parent->mTimeBase;
		mTimeRelate          = parent->mTimeRelate;
		mPause               = parent->mPause;
		mPauseStatus         = parent->mPauseStatus;
		mVolumeMode          = parent->mVolumeMode;
		mMute                = parent->mMute;
	}

	mNoteMgr.init();
	mSeqState = 1;
	mParent   = parent;
	mIntrMgr.init();

	initTimed();

	if ((mTrackMode & TRACK_MODE_CLEAR_REGISTERS) || !mParent) {
		mRegisterParam.init();
		for (u8 i = 0; i < 3; ++i) {
			mPanSwitchExt[i]              = 0;
			mPanSwitchParent[i]           = 0;
			mChannelUpdater.mCalcTypes[i] = TChannel::CALC_WEIGHTED_SUM_ALL;
		}
	} else {
		mRegisterParam.inherit(mParent->mRegisterParam);
		for (u8 i = 0; i < 3; ++i) {
			mPanSwitchExt[i]    = mParent->mPanSwitchExt[i];
			mPanSwitchParent[i] = mParent->mPanSwitchParent[i];
			mChannelUpdater.mCalcTypes[i]
			    = mParent->mChannelUpdater.mCalcTypes[i];
		}
	}

	for (u8 i = 0; i < 16; ++i)
		mChildren[i] = nullptr;

	mOscData[0] = Player::sEnvelopeDef;
	mOscMode[0] = 0xf;
	mOscData[1] = Player::sVibratoDef;
	mOscillators[1].initStart();
	mOscMode[1] = 0xe;

	mTranspose      = 0;
	mTransposeTotal = 0;
	mTrackPort.init();
	mConnected = 0;
}

int TTrack::startTrack(TTrack* parent, u8 child_index, u8 track_mode,
                       u32 seq_offset)
{
	mTrackRoute = (((parent->mTrackRoute << 4) | child_index) & 0xfffffff)
	              | ((parent->mTrackRoute & 0xf0000000) + 0x10000000);

	mConnectName = 0;
	mTrackMode   = track_mode;
	initTrack(parent->mSeqCtrl.getBase(), seq_offset, parent);
	if (mOuterParam)
		mOuterParam->initExtBuffer();
	mChannelUpdater.initAllocChannel(0);
	updateTrackAll();
	return 0;
}

TTrack* TTrack::openTrack(u8 param)
{
	TTrack* oldTrack = mChildren[param];
	if (oldTrack)
		oldTrack->closeTrack();

	TTrack* newTrack = TrackMgr::getNewTrack();
	if (!newTrack)
		return nullptr;

	mChildren[param] = newTrack;
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
	case 6: {
		u32 offset = param_2 + 2 * param_2;
		res        = mSeqCtrl.get24(param_1 + offset);
		break;
	}
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
		result = mRegisterParam.getAddress(reg - 0x28);
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
		return mTrackPort.get((u8)(reg - 0x40));
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
			TTrack* child = mChildren[i];
			result <<= 1;
			if (child && child->mSeqState)
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
			else if (chan->mKey == 0xff)
				bit = 1;
			else
				bit = 0;
			result |= bit;
		}
		break;

	case 48:
		result = mSeqCtrl.getLoopCount();
		break;

	default:
		result = mRegisterParam.mRegisters[reg];
		break;
	}

	return result;
}

void TTrack::writeRegDirect(u8 reg, u16 value)
{
	u16 r4;

	switch (reg) {
	case 0:
	case 1:
	case 2:
		value = value & 0xff;
		r4    = Player::extend8to16(value);
		break;

	case 32:
	case 33:
		return;

	case 34: {
		writeRegDirect(0, value >> 8);

		reg   = 1;
		r4    = value;
		value = value & 0xff;
		break;
	}

	default:
		r4 = value;
		break;
	}

	mRegisterParam.mRegisters[reg] = value;
	mRegisterParam.setFlag(r4);
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
	*value = mTrackPort.readExport(port);
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
		found = found->getChild(route & 0xF);

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
	mPause = 1;
	if (mPauseStatus & 0x1)
		mPendingUpdateFlags |= UPDATE_Volume;

	if (mPauseStatus & 0x4) {
		for (u8 i = 0; i < 8; ++i) {
			TChannel* chan = mNoteMgr.getChannel(i);
			if (chan) {
				chan->stop(10);
				mNoteMgr.releaseChannel(i);
			}
		}
	}

	if (mPauseStatus & 8) {
		for (u8 i = 0; i < 8; ++i) {
			TChannel* chan = mNoteMgr.getChannel(i);
			if (chan)
				chan->setPauseFlag(1);
		}
	}

	mIntrMgr.request(0);

	if (flag == 1) {
		for (u32 i = 0; i < 16; ++i) {
			if (mChildren[i] && mChildren[i]->mSeqState)
				mChildren[i]->pauseTrack(1);
		}
	}
}

void TTrack::unPauseTrack(u8 flag)
{
	mPause = 0;
	mPendingUpdateFlags |= UPDATE_Volume;

	for (u8 i = 0; i < 8; ++i) {
		TChannel* chan = mNoteMgr.getChannel(i);
		if (chan)
			chan->setPauseFlag(0);
	}

	mIntrMgr.request(1);

	if (flag == 1) {
		for (u8 i = 0; i < 16; ++i)
			if (mChildren[i] && mChildren[i]->mSeqState)
				mChildren[i]->unPauseTrack(1);
	}
}

void TTrack::pauseTrackAll() { pauseTrack(1); }

void TTrack::unPauseTrackAll() { unPauseTrack(1); }

void TTrack::reset()
{
	mSeqState = 0;
	mMute     = 0;
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
	if (self && self->mSeqState != 0) {
		if (self->mSeqState == 3) {
			self->stopSeqMain();
			return -1;
		} else {
			self->mTickCounter += self->mTickRate;

			if (self->mTickCounter < 1.0f) {
				self->updateSeq(0, true);
			} else {
				while (self->mTickCounter >= 1.0f) {
					self->mTickCounter -= 1.0f;
					if ((int)self->mainProc() != -1)
						continue;

					self->stopSeqMain();
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

void TTrack::updateSyncSw(u8 param) { sUpdateSyncMode = param; }

bool TTrack::registerTrackCallback(u16 (*callback)(TTrack*, u16))
{
	sCallBackFunc = callback;
	return true;
}

} // namespace JASystem
