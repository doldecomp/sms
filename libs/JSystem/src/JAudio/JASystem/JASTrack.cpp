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

enum OscSetupFlag {
	OSC_SETUP_TARGET_MASK     = 0x0F,
	OSC_SETUP_OSCILLATOR_MASK = 0x10,
	OSC_SETUP_RELEASE_TABLE   = 0x20,
	OSC_SETUP_ATTACK_TABLE    = 0x40,
	OSC_SETUP_RESET           = 0x80,
};

enum OscEnvTable {
	OSC_ENV_ATTACK  = 0,
	OSC_ENV_RELEASE = 1,
};

enum OscPreset {
	OSC_PRESET_VIBRATO1 = 0,
	OSC_PRESET_TREMOLO0 = 1,
	OSC_PRESET_TREMOLO1 = 2,
};

enum RegisterValueSource {
	VALUE_NEGATIVE_ONE = 0x10,
};

enum TableLoadMode {
	TABLE_LOAD_U8              = 4,
	TABLE_LOAD_U16             = 5,
	TABLE_LOAD_U24             = 6,
	TABLE_LOAD_U32             = 7,
	TABLE_LOAD_U32_BYTE_OFFSET = 8,
};

namespace Player {
	static JMath::TRandom_enough_ oRandom(0);
}

TSeqParser TTrack::sParser;

u8 TTrack::sUpdateSyncMode                 = 0;
u16 (*TTrack::sCallBackFunc)(TTrack*, u16) = nullptr;

// Indexed by TOscillator::Osc_::mTarget (0 volume, 1 pitch, 2 pan,
// 3 fxmix, 4 dolby).
u8 TTrack::sOscTable[5] = {
	TTrack::UPDATE_VOLUME, TTrack::UPDATE_PITCH, TTrack::UPDATE_PAN,
	TTrack::UPDATE_FXMIX,  TTrack::UPDATE_DOLBY,
};

TTrack::TTrack()
    : mParent(0)
    , mOuterParam(0)
    , mTrackRoute(0)
    , mRegistrationId(0)
{
	mTickAccumulator    = 0.0f;
	mTicksPerSubframe   = 0.0f;
	mPendingUpdateFlags = 0;
	mTempo              = 0x78;
	mTimeBase           = 0x78;

	mTrackMode           = TRACK_MODE_INHERIT;
	mOverrideParentTempo = false;

	mFromTrackPool      = 0;
	mTranspose          = 0;
	mEffectiveTranspose = 0;
	mPauseMuteFlags     = PAUSE_MUTE_NONE;
	mMute               = 0;
	mVolumeMode         = VOLUME_SQUARED;
	mSeqState           = SEQ_INACTIVE;

	mTimeRelate = 0;
	mRegistered = 0;
	mPaused     = 0;

	for (int i = 0; i < 16; ++i)
		mChildren[i] = nullptr;

	for (int i = MIX_PAN; i < MIX_COUNT; ++i)
		mParentMixModes[i] = mExternalMixModes[i] = MIX_MODE_LOCAL;

	for (int i = 0; i < 12; ++i)
		mAdsTable[i] = Player::sAdsTable[i];
	for (int i = 0; i < 6; ++i)
		mRelTable[i] = Player::sRelTable[i];

	for (int i = 0; i < 2; ++i)
		mOscMode[i] = OSC_ROUTE_DISABLED;

	mChannelMgr.init();

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

void TTrack::setInnerMemory(u8 param) { mFromTrackPool = param; }

void TTrack::setPanSwitchExt(u8 mix_mode, u8 mix_param)
{
	mExternalMixModes[mix_param] = mix_mode;
}

void TTrack::setPanSwitchParent(u8 mix_mode, u8 mix_param)
{
	mParentMixModes[mix_param] = mix_mode;
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
		mParent->mChannelMgr.receiveAllChannels(&mChannelMgr);
	else
		ChGlobal::releaseAll(&mChannelMgr);
}

void TTrack::flushAll()
{
	mChannelMgr.stopAll();
	mChannelMgr.stopAllRelease();
}

void TTrack::initTimed()
{
	for (u8 i = 0; i < TIMED_COUNT; i++) {
		mTimedParam.mMoveParams[i].mTicksRemaining = 0.0f;
		mTimedParam.mMoveParams[i].mCurrentValue   = 1.0f;
		mTimedParam.mMoveParams[i].mTargetValue    = 1.0f;
	}
	mTimedParam.mMoveParams[TIMED_PITCH].mCurrentValue               = 0.0f;
	mTimedParam.mMoveParams[TIMED_PITCH].mTargetValue                = 0.0f;
	mTimedParam.mMoveParams[TIMED_PAN].mCurrentValue                 = 0.5f;
	mTimedParam.mMoveParams[TIMED_PAN].mTargetValue                  = 0.5f;
	mTimedParam.mMoveParams[TIMED_PAUSE_VOLUME].mCurrentValue        = 0.5f;
	mTimedParam.mMoveParams[TIMED_PAUSE_VOLUME].mTargetValue         = 0.5f;
	mTimedParam.mMoveParams[TIMED_MIXER_DELAY_BALANCE].mCurrentValue = 0.0f;
	mTimedParam.mMoveParams[TIMED_MIXER_DELAY_BALANCE].mTargetValue  = 0.0f;
	mTimedParam.mMoveParams[TIMED_FXMIX].mCurrentValue               = 0.0f;
	mTimedParam.mMoveParams[TIMED_FXMIX].mTargetValue                = 0.0f;
	mTimedParam.mMoveParams[TIMED_DOLBY].mCurrentValue               = 0.0f;
	mTimedParam.mMoveParams[TIMED_DOLBY].mTargetValue                = 0.0f;
	for (u8 i = 1; i < 4; i++) {
		mTimedParam.mMoveParams[i + TIMED_IIR0].mCurrentValue = 0.0f;
		mTimedParam.mMoveParams[i + TIMED_IIR0].mTargetValue  = 0.0f;
	}
	mTimedParam.mMoveParams[TIMED_DIST_FILTER].mCurrentValue = 0.0f;
	mTimedParam.mMoveParams[TIMED_DIST_FILTER].mTargetValue  = 0.0f;
}

int TTrack::noteOn(u8 voice_slot, s32 key, s32 velocity, s32 gate_time)
{
	if (mMute && (mPauseMuteFlags & MUTE_SKIP_NOTE_ON))
		return -1;

	u32 voiceSlot = voice_slot;
	if (mNoteMgr.getChannel(voiceSlot))
		noteOff(voice_slot, 0);

	TTrack* parent       = mParent;
	TChannelMgr* chanMgr = &mChannelMgr;

	TTrack* it = parent;
	while (!chanMgr->mManagedChannels || !chanMgr->mFreeChannels) {
		if (it == nullptr) {
			chanMgr = &mChannelMgr;
			break;
		}
		// BUG: should be it->mChannelMgr?
		chanMgr = &parent->mChannelMgr;
		it      = it->getParent();
	}

	u32 reg;

	TChannel* borrowed;
	if (mTrackMode == TRACK_MODE_PARENT_CHANNELS) {
		if (parent == nullptr)
			return -1;

		if (chanMgr != &parent->mChannelMgr) {
			borrowed = chanMgr->getListHead(TChannelMgr::CHAN_LIST_FREE);
			if (borrowed) {
				--chanMgr->mManagedChannels;
				// BUG: should be parent->mChannelMgr.addListHead
				mChannelMgr.addListHead(borrowed, TChannelMgr::CHAN_LIST_FREE);
				++parent->mChannelMgr.mManagedChannels;
				borrowed->mManager = &parent->mChannelMgr;
			}
			chanMgr = &parent->mChannelMgr;
		}
	} else {
		if (chanMgr != &mChannelMgr) {
			borrowed = chanMgr->getListHead(TChannelMgr::CHAN_LIST_FREE);
			if (borrowed) {
				--chanMgr->mManagedChannels;
				mChannelMgr.addListHead(borrowed, TChannelMgr::CHAN_LIST_FREE);
				++mChannelMgr.mManagedChannels;
				borrowed->mManager = &mChannelMgr;
			}
			chanMgr = &mChannelMgr;
		}
	}

	reg       = readRegDirect(TRegisterParam::REG_BANK_PROGRAM);
	u8 bankNo = BankMgr::getPhysicalNumber(reg >> 8);
	u8 progNo = reg & 0xff;
	TChannel* chan
	    = BankMgr::noteOn(chanMgr, bankNo, progNo, key, velocity, gate_time);

	if (!chan)
		return -1;

	mNoteMgr.setChannel(voiceSlot, chan);

	chan->setPanPower(
	    mRegisterParam.getPanPowerBank(), mRegisterParam.getPanPowerOsc(),
	    mRegisterParam.getPanPowerTrack(), mRegisterParam.getPanPowerExt());

	for (u8 i = 0; i < 2; ++i) {
		u32 oscMode = mOscMode[i];
		if (oscMode != OSC_ROUTE_DISABLED && oscMode != OSC_ROUTE_TRACK) {
			if (oscMode >= OSC_ROUTE_COPY) {
				oscMode -= OSC_ROUTE_COPY;
				if (chan->isOsc(oscMode))
					chan->copyOsc(oscMode, &mOscData[i]);
			} else if (oscMode >= OSC_ROUTE_COPY_KEEP_RELEASE) {
				oscMode -= OSC_ROUTE_COPY_KEEP_RELEASE;
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
		updateTrack(UPDATE_VOLUME | UPDATE_PITCH | UPDATE_PAN);

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

void TTrack::oscSetupFull(u8 flags, u32 attack_table_offset,
                          u32 release_table_offset)
{
	u32 oscillatorIndex  = (flags & OSC_SETUP_OSCILLATOR_MASK) >> 4;
	int effectTarget     = flags & OSC_SETUP_TARGET_MASK;
	bool resetDefinition = flags & OSC_SETUP_RESET ? true : false;
	bool setAttackTable  = flags & OSC_SETUP_ATTACK_TABLE ? true : false;
	bool setReleaseTable = flags & OSC_SETUP_RELEASE_TABLE ? true : false;

	if (resetDefinition) {
		mOscData[oscillatorIndex]         = Player::sEnvelopeDef;
		mOscData[oscillatorIndex].mTarget = effectTarget;
		switch (effectTarget) {
		case EFFECT_PITCH:
			mOscData[oscillatorIndex].mBias = 1.0f;
			break;
		}
	}

	if (setAttackTable) {
		if (attack_table_offset == 0)
			mOscData[oscillatorIndex].mAdsTable = nullptr;

		mOscData[oscillatorIndex].mAdsTable
		    = (s16*)(mSeqCtrl.getBase() + attack_table_offset);
	}

	if (!setReleaseTable)
		return;

	if (release_table_offset == 0)
		mOscData[oscillatorIndex].mRelTable = Player::sRelTable;

	mOscData[oscillatorIndex].mRelTable
	    = (s16*)(mSeqCtrl.getBase() + release_table_offset);
}

void TTrack::oscSetupSimpleEnv(u8 table, u32 table_offset)
{
	switch (table) {
	case OSC_ENV_ATTACK:
		mOscData[0]           = Player::sEnvelopeDef;
		mOscData[0].mAdsTable = (s16*)(mSeqCtrl.getBase() + table_offset);
		break;
	case OSC_ENV_RELEASE:
		mOscData[0].mRelTable = (s16*)(mSeqCtrl.getBase() + table_offset);
		break;
	}
}

void TTrack::oscUpdateParam(u8 param_id, f32 value)
{
	switch (param_id) {
	case TIMED_OSC0_DEPTH:
		mOscData[0].mDepth = value;
		break;
	case TIMED_OSC0_RATE:
		mOscData[0].mRate = value;
		break;
	case TIMED_OSC0_BIAS:
		mOscData[0].mBias = value;
		break;
	case TIMED_OSC1_DEPTH:
		mOscData[1].mDepth = value;
		break;
	case TIMED_OSC1_RATE:
		mOscData[1].mRate = value;
		break;
	case TIMED_OSC1_BIAS:
		mOscData[1].mBias = value;
		break;
	}
}

void TTrack::oscSetupSimple(u8 preset)
{
	switch (preset) {
	case OSC_PRESET_VIBRATO1:
		mOscData[1] = Player::sVibratoDef;
		break;
	case OSC_PRESET_TREMOLO0:
		mOscData[0] = Player::sTremoroDef;
		break;
	case OSC_PRESET_TREMOLO1:
		mOscData[1] = Player::sTremoroDef;
		break;
	}
}

void TTrack::updateTrackAll()
{
	f32 fVar6 = mRegisterParam.getPanPowerExt() / 32767.0f;
	f32 curVolume;
	f32 curPitch;
	f32 curFxmix;
	f32 curDolby;
	f32 curPan;

	if (mTrackMode == TRACK_MODE_PARENT_CHANNELS)
		return;

	mChannelMgr.mPriorityConfig = mRegisterParam.getPriority() | 0x10000;
	mChannelMgr.mPriorityTime   = 0;

	u8 cVar8 = 0;
	f32 thing
	    = mTimedParam.mInnerParam.mMixerDelayBalance.mCurrentValue * 128.0f;
	s8 local_4c;
	OSf32tos8(&thing, &local_4c);
	if (local_4c < 0) {
		cVar8    = -local_4c;
		local_4c = 0;
	}

	mChannelMgr.mMixerDelayMax        = 0x10;
	mChannelMgr.mMixerDelaySamples[0] = cVar8;
	mChannelMgr.mMixerDelaySamples[1] = local_4c;

	curVolume = mTimedParam.mInnerParam.mVolume.mCurrentValue;
	if (mVolumeMode == VOLUME_SQUARED)
		curVolume = curVolume * curVolume;

	if (mMute != 0)
		curVolume = 0.0f;

	curPitch = Player::pitchToCent(mTimedParam.mInnerParam.mPitch.mCurrentValue,
	                               mRegisterParam.getBendSense());

	curPan   = mTimedParam.mInnerParam.mPan.mCurrentValue;
	curFxmix = mTimedParam.mInnerParam.mFxmix.mCurrentValue;
	curDolby = mTimedParam.mInnerParam.mDolby.mCurrentValue;

	if (mOuterParam) {
		if (mOuterParam->checkOuterSwitch(UPDATE_VOLUME))
			curVolume *= mOuterParam->getVolume();

		if (mOuterParam->checkOuterSwitch(UPDATE_PITCH))
			curPitch *= mOuterParam->getPitch();

		if (mOuterParam->checkOuterSwitch(UPDATE_FXMIX))
			curFxmix = panCalc(curFxmix, mOuterParam->getFxVol(), fVar6,
			                   mExternalMixModes[MIX_FXMIX]);

		if (mOuterParam->checkOuterSwitch(UPDATE_DOLBY))
			curDolby = panCalc(curDolby, mOuterParam->getDolby(), fVar6,
			                   mExternalMixModes[MIX_DOLBY]);

		if (mOuterParam->checkOuterSwitch(UPDATE_PAN))
			curPan = panCalc(curPan, mOuterParam->getPan(), fVar6,
			                 mParentMixModes[MIX_PAN]);
	}

	if (!mParent || (mTrackMode & TRACK_MODE_INDEPENDENT_PARAMS)) {
		mChannelMgr.mVolume = curVolume;
		mChannelMgr.mPitch  = curPitch;
		mChannelMgr.mPan    = curPan;
		mChannelMgr.mFxmix  = curFxmix;
		mChannelMgr.mDolby  = curDolby;
	} else {
		f32 fVar6           = mRegisterParam.getPanPowerParent() / 32767.0f;
		mChannelMgr.mVolume = mParent->mChannelMgr.mVolume * curVolume;
		mChannelMgr.mPitch  = mParent->mChannelMgr.mPitch * curPitch;
		mChannelMgr.mPan    = panCalc(curPan, mParent->mChannelMgr.mPan, fVar6,
		                              mParentMixModes[MIX_PAN]);
		mChannelMgr.mFxmix  = panCalc(curFxmix, mParent->mChannelMgr.mFxmix,
		                              fVar6, mParentMixModes[MIX_FXMIX]);
		mChannelMgr.mDolby  = panCalc(curDolby, mParent->mChannelMgr.mDolby,
		                              fVar6, mParentMixModes[MIX_DOLBY]);

		if (mOuterParam && mOuterParam->checkOuterSwitch(UPDATE_FIR_FILTER)) {
			for (u8 i = 0; i < 8; ++i)
				mChannelMgr.mFirCoeffs[i] = mOuterParam->getIntFirFilter(i);
			mChannelMgr.mFilterMode = 8;
		}

		for (u8 i = 0; i < 4; ++i) {
			mChannelMgr.mIirCoeffs[i]
			    = mTimedParam.mInnerParam.mIIRs[i].mCurrentValue * 32767.0f;
		}

		mChannelMgr.mFilterMode |= DSPInterface::DSPBuffer::FILTER_IIR_ENABLED;
		mChannelMgr.mDistFilter
		    = mTimedParam.mInnerParam.mDistFilter.mCurrentValue * 32767.0f;
	}
}

void TTrack::updateTrack(u32 update_flags)
{
	f32 curVolume;
	f32 curPitch;
	f32 curPan;
	f32 curFxmix;
	f32 curDolby;
	f32 fVar3 = mRegisterParam.getPanPowerExt() / 32767.0f;

	if (mTrackMode == TRACK_MODE_PARENT_CHANNELS)
		return;

	if (update_flags & UPDATE_MIXER_DELAY_BALANCE) {
		u8 cVar8 = 0;
		f32 thing
		    = mTimedParam.mInnerParam.mMixerDelayBalance.mCurrentValue * 128.0f;
		s8 local_4c;
		OSf32tos8(&thing, &local_4c);
		if (local_4c < 0) {
			cVar8    = -local_4c;
			local_4c = 0;
		}

		mChannelMgr.mMixerDelaySamples[0] = cVar8;
		mChannelMgr.mMixerDelaySamples[1] = local_4c;
	}

	if ((update_flags & UPDATE_TEMPO) && !mParent)
		updateTempo();

	if (update_flags & UPDATE_VOLUME) {
		curVolume = mTimedParam.mInnerParam.mVolume.mCurrentValue;
		if (mVolumeMode == VOLUME_SQUARED)
			curVolume *= curVolume;

		if (mMute)
			curVolume = 0.0f;

		if (mOuterParam && mOuterParam->checkOuterSwitch(UPDATE_VOLUME))
			curVolume *= mOuterParam->getVolume();

		if (mPaused && (mPauseMuteFlags & PAUSE_SCALE_VOLUME))
			curVolume *= mTimedParam.mInnerParam.mPauseVolume.mCurrentValue;
	}

	if (update_flags & UPDATE_PITCH) {
		curPitch
		    = Player::pitchToCent(mTimedParam.mInnerParam.mPitch.mCurrentValue,
		                          mRegisterParam.getBendSense());

		if (mOuterParam && mOuterParam->checkOuterSwitch(UPDATE_PITCH))
			curPitch *= mOuterParam->getPitch();
	}

	if (update_flags & UPDATE_PAN) {
		curPan = mTimedParam.mInnerParam.mPan.mCurrentValue;

		if (mOuterParam && mOuterParam->checkOuterSwitch(UPDATE_PAN))
			curPan = panCalc(curPan, mOuterParam->getPan(), fVar3,
			                 mExternalMixModes[MIX_PAN]);
	}

	if (update_flags & UPDATE_FXMIX) {
		curFxmix = mTimedParam.mInnerParam.mFxmix.mCurrentValue;

		if (mOuterParam && mOuterParam->checkOuterSwitch(UPDATE_FXMIX))
			curFxmix = panCalc(curFxmix, mOuterParam->getFxVol(), fVar3,
			                   mExternalMixModes[MIX_FXMIX]);
	}

	if (update_flags & UPDATE_DOLBY) {
		curDolby = mTimedParam.mInnerParam.mDolby.mCurrentValue;

		if (mOuterParam && mOuterParam->checkOuterSwitch(UPDATE_DOLBY))
			curDolby = panCalc(curDolby, mOuterParam->getDolby(), fVar3,
			                   mExternalMixModes[MIX_DOLBY]);
	}

	if (update_flags & UPDATE_IIRS) {
		for (u8 i = 0; i < 4; ++i)
			mChannelMgr.mIirCoeffs[i]
			    = mTimedParam.mInnerParam.mIIRs[i].mCurrentValue * 32767.0f;

		mChannelMgr.mFilterMode |= DSPInterface::DSPBuffer::FILTER_IIR_ENABLED;
	}

	if (mOuterParam && (update_flags & UPDATE_FIR_FILTER)
	    && mOuterParam->checkOuterSwitch(UPDATE_FIR_FILTER)) {
		for (u8 i = 0; i < 8; ++i)
			mChannelMgr.mFirCoeffs[i] = mOuterParam->getIntFirFilter(i);

		mChannelMgr.mFilterMode
		    = (mChannelMgr.mFilterMode
		       & DSPInterface::DSPBuffer::FILTER_IIR_ENABLED)
		      + 8;
	}

	if (update_flags & UPDATE_DIST_FILTER) {
		mChannelMgr.mDistFilter
		    = mTimedParam.mInnerParam.mDistFilter.mCurrentValue * 32767.0f;
	}

	for (int i = 0; i < 2; ++i) {
		if (mOscMode[i] == OSC_ROUTE_TRACK) {
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
		if (update_flags & UPDATE_VOLUME)
			mChannelMgr.mVolume = curVolume;
		if (update_flags & UPDATE_PITCH)
			mChannelMgr.mPitch = curPitch;
		if (update_flags & UPDATE_PAN)
			mChannelMgr.mPan = curPan;
		if (update_flags & UPDATE_FXMIX)
			mChannelMgr.mFxmix = curFxmix;
		if (update_flags & UPDATE_DOLBY)
			mChannelMgr.mDolby = curDolby;
	} else {
		fVar3 = mRegisterParam.getPanPowerParent() / 32767.0f;
		if (update_flags & UPDATE_VOLUME)
			mChannelMgr.mVolume = mParent->mChannelMgr.mVolume * curVolume;
		if (update_flags & UPDATE_PITCH)
			mChannelMgr.mPitch = mParent->mChannelMgr.mPitch * curPitch;
		if (update_flags & UPDATE_PAN)
			mChannelMgr.mPan = panCalc(curPan, mParent->mChannelMgr.mPan, fVar3,
			                           mParentMixModes[MIX_PAN]);
		if (update_flags & UPDATE_FXMIX)
			mChannelMgr.mFxmix = panCalc(curFxmix, mParent->mChannelMgr.mFxmix,
			                             fVar3, mParentMixModes[MIX_FXMIX]);
		if (update_flags & UPDATE_DOLBY)
			mChannelMgr.mDolby = panCalc(curDolby, mParent->mChannelMgr.mDolby,
			                             fVar3, mParentMixModes[MIX_DOLBY]);
	}
}

void TTrack::updateTempo()
{
	if (mParent == 0) {
		mTicksPerSubframe = mTimeBase;
		mTicksPerSubframe *= mTempo;
		mTicksPerSubframe /= Kernel::getDacRate();
		mTicksPerSubframe *= 80.0f;
		mTicksPerSubframe /= 60.0f;
		if (mOuterParam->checkOuterSwitch(UPDATE_TEMPO))
			mTicksPerSubframe *= mOuterParam->getTempo();
	} else {
		mTicksPerSubframe = mParent->mTicksPerSubframe;
		mTimeBase         = mParent->mTimeBase;
	}

	for (u32 i = 0; i < 16; ++i)
		if (mChildren[i] && mChildren[i]->mSeqState != SEQ_INACTIVE)
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
		if (child && child->mSeqState != SEQ_INACTIVE) {
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
		if (mOscMode[i] == OSC_ROUTE_TRACK)
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
		mTickAccumulator += tempo;
		if (mTickAccumulator < 1.0f)
			return TRACK_ACTIVE;
		mTickAccumulator -= 1.0f;
	}

	if (mParent && mChannelMgr.mManagedChannels != 0) {
		TChannel* head = mChannelMgr.getListHead(TChannelMgr::CHAN_LIST_FREE);
		if (head) {
			mParent->mChannelMgr.addListHead(head, TChannelMgr::CHAN_LIST_FREE);
			head->mManager = &mParent->mChannelMgr;
			--mChannelMgr.mManagedChannels;
			++mParent->mChannelMgr.mManagedChannels;
		}
	}

	mEffectiveTranspose = mTranspose;
	if (mParent)
		mEffectiveTranspose += mParent->mEffectiveTranspose;
	mIntrMgr.request(TIntrMgr::INTR_TICK);
	mIntrMgr.timerProcess();

	u32 updateFlags;
	s32 retcode;
	do {
		tryInterrupt();

		if (mPaused != 0 && (mPauseMuteFlags & PAUSE_SEQUENCE))
			goto finishTick;

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

		if (retcode == TSeqParser::PARSE_FINISH)
			return TRACK_FINISHED;
	} while (retcode == TSeqParser::PARSE_RECHECK);

	updateFlags = 0;
	for (int i = 0; i != TIMED_COUNT; ++i) {
		MoveParam_* param = &mTimedParam.mMoveParams[i];
		if (param->mTicksRemaining > 0.0f) {
			param->mCurrentValue += param->mStepPerTick;
			param->mTicksRemaining -= 1.0f;
			if (i <= TIMED_DIST_FILTER || i >= TIMED_OSC1_BIAS) {
				updateFlags |= 1 << i;
			} else {
				updateOscParam(i, param->mCurrentValue);
			}
		}
	}

	for (int i = 0; i < 2; ++i)
		if (mOscMode[i] == OSC_ROUTE_TRACK)
			updateFlags |= sOscTable[mOscData[i].mTarget];

finishTick:
	// BUG: updateFlags might be uninitialized!
	mPendingUpdateFlags |= updateFlags;

	updateSeq(0, false);

	for (int i = 0; i < 16; ++i) {
		TTrack* child = mChildren[i];
		if (child && child->mSeqState != SEQ_INACTIVE) {
			if (child->mainProc() == TRACK_FINISHED) {
				child->closeTrack();
				mChildren[i] = nullptr;
			}
		}
	}

	return TRACK_ACTIVE;
}

int TTrack::seqTimeToDspTime(s32 time, u8 gate_percent)
{
	f32 res = time;
	res *= gate_percent;
	res /= 100.0f;
	if (mTimeRelate) {
		res /= mTicksPerSubframe;
	} else {
		res = (res * 120.0f) / mTimeBase;
		if (Kernel::getOutputRate() == 0)
			res = (res * Kernel::getSubFrames()) / 10.0f;
	}
	return res;
}

void TTrack::writeTimeParam(u8 opcode_params)
{
	u8 paramIdx = mSeqCtrl.readByte();

	s16 value;
	switch (opcode_params & TSeqParser::VALUE_MASK) {
	case TSeqParser::VALUE_REGISTER:
		value = readRegDirect(mSeqCtrl.readByte());
		break;
	case TSeqParser::VALUE_U8:
		value = mSeqCtrl.readByte();
		break;
	case TSeqParser::VALUE_S16_FROM_8: {
		u16 byte = mSeqCtrl.readByte();
		if (byte & 0x80)
			value = byte << 8;
		else
			value = byte << 8 | byte << 1;
		break;
	}
	case TSeqParser::VALUE_S16:
		value = mSeqCtrl.read16();
		break;
	}

	s32 duration = 0;
	switch (opcode_params & TSeqParser::DURATION_MASK) {
	case TSeqParser::DURATION_IMMEDIATE:
		duration = -1;
		break;
	case TSeqParser::DURATION_REGISTER:
		duration = readRegDirect(mSeqCtrl.readByte());
		break;
	case TSeqParser::DURATION_U8:
		duration = mSeqCtrl.readByte();
		break;
	case TSeqParser::DURATION_U16:
		duration = mSeqCtrl.read16();
		break;
	}

	MoveParam_* moveParam   = &mTimedParam.mMoveParams[paramIdx];
	moveParam->mTargetValue = value / 32768.0f;
	if (duration <= 0) {
		moveParam->mCurrentValue   = moveParam->mTargetValue;
		moveParam->mStepPerTick    = 0.0f;
		moveParam->mTicksRemaining = 1.0f;
	} else {
		moveParam->mStepPerTick
		    = (moveParam->mTargetValue - moveParam->mCurrentValue) / duration;
		moveParam->mTicksRemaining = duration;
	}
}

void TTrack::writeRegParam(u8 opcode_params)
{
	u32 valueSource = opcode_params & TSeqParser::VALUE_MASK;
	u32 regAction   = opcode_params & TSeqParser::REG_OPERATION_MASK;

	u32 tableLoadMode;

	if ((opcode_params & TSeqParser::OPCODE_PARAM_MASK)
	    == TSeqParser::REG_SUBTRACT) {
		valueSource = TSeqParser::VALUE_REGISTER;
		regAction   = TSeqParser::REG_SUBTRACT;
	}

	if ((opcode_params & TSeqParser::OPCODE_PARAM_MASK)
	    == TSeqParser::REG_LOAD_TABLE) {
		regAction     = TSeqParser::REG_LOAD_TABLE;
		opcode_params = mSeqCtrl.readByte();
		valueSource   = opcode_params & TSeqParser::VALUE_MASK;
		tableLoadMode = ((opcode_params >> 4) & 0xF) + TABLE_LOAD_U8;
	}

	if ((opcode_params & TSeqParser::OPCODE_PARAM_MASK)
	    == TSeqParser::REG_EXTENDED) {
		regAction   = mSeqCtrl.readByte();
		valueSource = regAction & TSeqParser::VALUE_MASK;
		regAction &= TSeqParser::OPCODE_FAMILY_MASK;
		if (valueSource == TSeqParser::VALUE_S16_FROM_8)
			valueSource = VALUE_NEGATIVE_ONE;
	}

	u8 regNo = mSeqCtrl.readByte();
	u32 r25;

	if (regAction == TSeqParser::REG_LOAD_TABLE)
		r25 = readReg32(mSeqCtrl.readByte());

	s16 value;

	switch (valueSource) {
	case TSeqParser::VALUE_REGISTER:
		value = readRegDirect(mSeqCtrl.readByte());
		break;
	case TSeqParser::VALUE_U8:
		value = mSeqCtrl.readByte();
		break;
	case TSeqParser::VALUE_S16_FROM_8: {
		u16 byte = mSeqCtrl.readByte();
		if (byte & 0x80)
			value = byte << 8;
		else
			value = byte << 8 | byte << 1;
		break;
	}
	case TSeqParser::VALUE_S16:
		value = mSeqCtrl.read16();
		break;
	case VALUE_NEGATIVE_ONE:
		value = 0xffff;
		break;
	}

	s16 oldValue = readRegDirect(regNo);

	switch (regAction) {
	case TSeqParser::REG_SET:
		break;

	case TSeqParser::REG_ADD:
		if (valueSource == TSeqParser::VALUE_U8)
			value = Player::extend8to16(value);
		value = oldValue + value;
		break;

	case TSeqParser::REG_MULTIPLY: {
		u32 product = oldValue;
		product *= value;
		writeRegDirect(TRegisterParam::REG_PRODUCT_HI, product >> 0x10);
		writeRegDirect(TRegisterParam::REG_PRODUCT_LO, JSULoHalf(product));
		return;
	}

	case TSeqParser::REG_COMPARE:
		mRegisterParam.setFlag(oldValue - value);
		return;

	case TSeqParser::REG_SUBTRACT:
		value = oldValue - value;
		break;

	case TSeqParser::REG_SHIFT_LOGICAL:
		if (valueSource == TSeqParser::VALUE_U8)
			value = Player::extend8to16(value);
		if (value < 0)
			value = (u16)oldValue >> -value;
		else
			value = (u16)oldValue << value;
		break;

	case TSeqParser::REG_SHIFT_ARITHMETIC:
		if (valueSource == TSeqParser::VALUE_U8)
			value = Player::extend8to16(value);
		if (value < 0)
			value = oldValue >> -value;
		else
			value = oldValue << value;
		break;

	case TSeqParser::REG_AND:
		value = oldValue & value;
		break;

	case TSeqParser::REG_OR:
		value = oldValue | value;
		break;

	case TSeqParser::REG_XOR:
		value = oldValue ^ value;
		break;

	case TSeqParser::REG_NEGATE:
		value = -oldValue;
		break;

	case TSeqParser::REG_RANDOM:
		r25   = Player::getRandomS32();
		value = r25 % (u16)value;
		break;

	case TSeqParser::REG_LOAD_TABLE:
		r25   = loadTbl(r25, value, tableLoadMode);
		value = (u16)r25;
		break;
	}

	s16 flagRegisterValue;
	switch (regNo) {
	case TRegisterParam::REG_U8_0:
	case TRegisterParam::REG_U8_1:
	case TRegisterParam::REG_U8_2:
		value             = (u8)value;
		flagRegisterValue = Player::extend8to16(value);
		break;

	case TRegisterParam::REG_PROGRAM:
		value = (u8)value | (mRegisterParam.getBankNumber() << 8);
		regNo = TRegisterParam::REG_BANK_PROGRAM;
		break;

	case TRegisterParam::REG_BANK:
		value = (value << 8) | (u8)mRegisterParam.getProgramNumber();
		regNo = TRegisterParam::REG_BANK_PROGRAM;
		break;

	case TRegisterParam::REG_NOTE_ON_PRIORITY:
		value = (mRegisterParam.getPriority() & 0xff00) | JSULoByte(value);
		regNo = TRegisterParam::REG_PRIORITY;
		break;

	case TRegisterParam::REG_RELEASE_PRIORITY:
		value = (value << 8) | JSULoByte(mRegisterParam.getPriority());
		regNo = TRegisterParam::REG_PRIORITY;
		break;

	case TRegisterParam::REG_U8_PAIR:
		writeRegDirect(TRegisterParam::REG_U8_0, value >> 8);
		value             = value & 0xff;
		flagRegisterValue = value;
		regNo             = TRegisterParam::REG_U8_1;
		break;

	case TRegisterParam::REG_ADDRESS0:
	case TRegisterParam::REG_ADDRESS1:
	case TRegisterParam::REG_ADDRESS2:
	case TRegisterParam::REG_ADDRESS3:
		mRegisterParam.setAddress(regNo - TRegisterParam::REG_ADDRESS0, r25);
		return;

	default:
		flagRegisterValue = value;
		break;
	}

	mRegisterParam.mRegisters[regNo] = value;
	mRegisterParam.setFlag(flagRegisterValue);

	if (regNo == TRegisterParam::REG_BANK_PROGRAM) {
		for (int i = 0; i < 2; i++)
			if (mOscMode[i] != OSC_ROUTE_TRACK)
				mOscMode[i] = OSC_ROUTE_DISABLED;
	}

	if (regNo == TRegisterParam::REG_BEND_SENSE)
		mPendingUpdateFlags |= UPDATE_PITCH;

	if (regNo == TRegisterParam::REG_PRIORITY) {
		mChannelMgr.mPriorityConfig = mRegisterParam.getPriority() | 0x10000;
		mChannelMgr.mPriorityTime   = 0;
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
	mChannelMgr.initAllocChannel(0);
	mTickAccumulator  = 0.0f;
	mTicksPerSubframe = 1.0f;
	updateTrackAll();
	mSeqState = SEQ_READY;

	return result;
}

bool TTrack::startSeq()
{
	switch (mSeqState) {
	case SEQ_INACTIVE:
		return false;
	case SEQ_PLAYING:
		return false;
	case SEQ_STOP_PENDING:
		return false;
	case SEQ_READY:
		mSeqState = SEQ_PLAYING;
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
	case SEQ_INACTIVE:
		break;

	case SEQ_READY:
		mSeqState = SEQ_INACTIVE;
		if (mFromTrackPool == 1)
			TrackMgr::backTrack(this);
		TrackMgr::deAllocRoot(this);
		break;

	default:
		mSeqState = SEQ_STOP_PENDING;
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
	if (mSeqState == SEQ_INACTIVE)
		return false;

	allNoteOff();
	mSeqState = SEQ_INACTIVE;

	if (mFromTrackPool == 1)
		TrackMgr::backTrack(this);

	for (u8 i = 0; i < 16; ++i)
		if (mChildren[i]) {
			mChildren[i]->closeTrack();
			mChildren[i] = nullptr;
		}

	mMute = 0;

	releaseChannelAll();

	if (mRegistered) {
		TrackMgr::unRegistTrack(this);
		mRegistered = 0;
	}

	return false;
}

void TTrack::muteTrack(u8 flag)
{
	mMute = flag;
	mPendingUpdateFlags |= UPDATE_VOLUME;
	if (mMute && (mPauseMuteFlags & MUTE_STOP_NOTES)) {
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

void TTrack::initTrack(void* data, u32 seq_offset, TTrack* parent)
{
	mSeqCtrl.init(data, seq_offset);
	if (parent == nullptr) {
		mTempo          = 0x78;
		mTimeBase       = 0x30;
		mTimeRelate     = 1;
		mPaused         = 0;
		mPauseMuteFlags = PAUSE_SEQUENCE | PAUSE_CHANNELS;
		mVolumeMode     = VOLUME_SQUARED;
		mMute           = 0;
	} else {
		mTempo               = parent->mTempo;
		mOverrideParentTempo = false;
		mTicksPerSubframe    = parent->mTicksPerSubframe;
		mTimeBase            = parent->mTimeBase;
		mTimeRelate          = parent->mTimeRelate;
		mPaused              = parent->mPaused;
		mPauseMuteFlags      = parent->mPauseMuteFlags;
		mVolumeMode          = parent->mVolumeMode;
		mMute                = parent->mMute;
	}

	mNoteMgr.init();
	mSeqState = SEQ_PLAYING;
	mParent   = parent;
	mIntrMgr.init();

	initTimed();

	if ((mTrackMode & TRACK_MODE_INIT_REGISTERS) || !mParent) {
		mRegisterParam.init();
		for (u8 i = MIX_PAN; i < MIX_COUNT; ++i) {
			mExternalMixModes[i]      = MIX_MODE_LOCAL;
			mParentMixModes[i]        = MIX_MODE_LOCAL;
			mChannelMgr.mCalcTypes[i] = TChannel::CALC_WEIGHTED_SUM_ALL;
		}
	} else {
		mRegisterParam.inherit(mParent->mRegisterParam);
		for (u8 i = MIX_PAN; i < MIX_COUNT; ++i) {
			mExternalMixModes[i]      = mParent->mExternalMixModes[i];
			mParentMixModes[i]        = mParent->mParentMixModes[i];
			mChannelMgr.mCalcTypes[i] = mParent->mChannelMgr.mCalcTypes[i];
		}
	}

	for (u8 i = 0; i < 16; ++i)
		mChildren[i] = nullptr;

	mOscData[0] = Player::sEnvelopeDef;
	mOscMode[0] = OSC_ROUTE_DISABLED;
	mOscData[1] = Player::sVibratoDef;
	mOscillators[1].initStart();
	mOscMode[1] = OSC_ROUTE_TRACK;

	mTranspose          = 0;
	mEffectiveTranspose = 0;
	mTrackPort.init();
	mRegistered = 0;
}

int TTrack::startTrack(TTrack* parent, u8 child_index, u8 track_mode,
                       u32 seq_offset)
{
	mTrackRoute = (((parent->mTrackRoute << 4) | child_index) & 0xfffffff)
	              | ((parent->mTrackRoute & 0xf0000000) + 0x10000000);

	mRegistrationId = 0;
	mTrackMode      = track_mode;
	initTrack(parent->mSeqCtrl.getBase(), seq_offset, parent);
	if (mOuterParam)
		mOuterParam->initExtBuffer();
	mChannelMgr.initAllocChannel(0);
	updateTrackAll();
	return 0;
}

TTrack* TTrack::openTrack(u8 index)
{
	TTrack* oldTrack = mChildren[index];
	if (oldTrack)
		oldTrack->closeTrack();

	TTrack* newTrack = TrackMgr::getNewTrack();
	if (!newTrack)
		return nullptr;

	mChildren[index] = newTrack;
	return newTrack;
}

int TTrack::loadTbl(u32 table_offset, u32 index_or_offset, u32 mode)
{
	u32 res;
	switch (mode) {
	case TABLE_LOAD_U8:
		res = mSeqCtrl.getByte(table_offset + index_or_offset);
		break;
	case TABLE_LOAD_U16:
		res = mSeqCtrl.get16(table_offset + 2 * index_or_offset);
		break;
	case TABLE_LOAD_U24: {
		u32 offset = index_or_offset + 2 * index_or_offset;
		res        = mSeqCtrl.get24(table_offset + offset);
		break;
	}
	case TABLE_LOAD_U32:
		res = mSeqCtrl.get32(table_offset + 4 * index_or_offset);
		break;
	case TABLE_LOAD_U32_BYTE_OFFSET:
		res = mSeqCtrl.get32(table_offset + index_or_offset);
		break;
	}
	return res;
}

u32 TTrack::readReg32(u8 reg)
{
	u32 result;
	switch (reg) {
	case TRegisterParam::REG_ADDRESS0:
	case TRegisterParam::REG_ADDRESS1:
	case TRegisterParam::REG_ADDRESS2:
	case TRegisterParam::REG_ADDRESS3:
		result = mRegisterParam.getAddress(reg - TRegisterParam::REG_ADDRESS0);
		break;

	case TRegisterParam::REG_PRODUCT:
		result = readRegDirect(TRegisterParam::REG_PRODUCT_HI);
		result <<= 0x10;
		result |= readRegDirect(TRegisterParam::REG_PRODUCT_LO);
		break;

	default:
		result = readRegDirect(reg);
		break;
	}
	return result;
}

u32 TTrack::exchangeRegisterValue(u8 reg)
{
	if (reg < TRegisterParam::REG_PORT_BASE)
		return readReg32(reg);
	else
		return mTrackPort.get((u8)(reg - TRegisterParam::REG_PORT_BASE));
}

u16 TTrack::readRegDirect(u8 reg)
{
	u16 result;
	int i;

	switch (reg) {
	case TRegisterParam::REG_BANK:
		result = mRegisterParam.getBankNumber();
		break;

	case TRegisterParam::REG_PROGRAM:
		result = mRegisterParam.getProgramNumber();
		break;

	case TRegisterParam::REG_U8_PAIR:
		result = readRegDirect(TRegisterParam::REG_U8_0);
		result <<= 8;
		result |= readRegDirect(TRegisterParam::REG_U8_1);
		break;

	case TRegisterParam::REG_ACTIVE_CHILD_MASK:
		result = 0;
		for (i = 15; i >= 0; --i) {
			TTrack* child = mChildren[i];
			result <<= 1;
			if (child && child->mSeqState != SEQ_INACTIVE)
				result |= 1;
		}
		break;

	case TRegisterParam::REG_NOTE_STOP_MASK:
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

	case TRegisterParam::REG_LOOP_COUNT:
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
	u16 flags;

	switch (reg) {
	case TRegisterParam::REG_U8_0:
	case TRegisterParam::REG_U8_1:
	case TRegisterParam::REG_U8_2:
		value = value & 0xff;
		flags = Player::extend8to16(value);
		break;

	case TRegisterParam::REG_BANK:
	case TRegisterParam::REG_PROGRAM:
		return;

	case TRegisterParam::REG_U8_PAIR: {
		writeRegDirect(TRegisterParam::REG_U8_0, value >> 8);

		reg   = TRegisterParam::REG_U8_1;
		flags = value;
		value = value & 0xff;
		break;
	}

	default:
		flags = value;
		break;
	}

	mRegisterParam.mRegisters[reg] = value;
	mRegisterParam.setFlag(flags);
}

void TTrack::setTrackExtPanPower(f32 power) { }

bool TTrack::writePortAppDirect(u32 port, u16 value)
{
	mTrackPort.mImportFlag[port] = 1;
	mTrackPort.mValue[port]      = value;
	if (port <= 1)
		mIntrMgr.request(port == 0 ? TIntrMgr::INTR_PORT0_WRITE
		                           : TIntrMgr::INTR_PORT1_WRITE);
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
		found->mIntrMgr.request(realPort == 0 ? TIntrMgr::INTR_PORT0_WRITE
		                                      : TIntrMgr::INTR_PORT1_WRITE);
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
	mPaused = 1;
	if (mPauseMuteFlags & PAUSE_SCALE_VOLUME)
		mPendingUpdateFlags |= UPDATE_VOLUME;

	if (mPauseMuteFlags & PAUSE_STOP_NOTES) {
		for (u8 i = 0; i < 8; ++i) {
			TChannel* chan = mNoteMgr.getChannel(i);
			if (chan) {
				chan->stop(10);
				mNoteMgr.releaseChannel(i);
			}
		}
	}

	if (mPauseMuteFlags & PAUSE_CHANNELS) {
		for (u8 i = 0; i < 8; ++i) {
			TChannel* chan = mNoteMgr.getChannel(i);
			if (chan)
				chan->setPauseFlag(1);
		}
	}

	mIntrMgr.request(TIntrMgr::INTR_PAUSE);

	if (flag == 1) {
		for (u32 i = 0; i < 16; ++i) {
			if (mChildren[i] && mChildren[i]->mSeqState != SEQ_INACTIVE)
				mChildren[i]->pauseTrack(1);
		}
	}
}

void TTrack::unPauseTrack(u8 flag)
{
	mPaused = 0;
	mPendingUpdateFlags |= UPDATE_VOLUME;

	for (u8 i = 0; i < 8; ++i) {
		TChannel* chan = mNoteMgr.getChannel(i);
		if (chan)
			chan->setPauseFlag(0);
	}

	mIntrMgr.request(TIntrMgr::INTR_RESUME);

	if (flag == 1) {
		for (u8 i = 0; i < 16; ++i)
			if (mChildren[i] && mChildren[i]->mSeqState != SEQ_INACTIVE)
				mChildren[i]->unPauseTrack(1);
	}
}

void TTrack::pauseTrackAll() { pauseTrack(1); }

void TTrack::unPauseTrackAll() { unPauseTrack(1); }

void TTrack::reset()
{
	mSeqState = SEQ_INACTIVE;
	mMute     = 0;
}

f32 TTrack::panCalc(f32 local_value, f32 source_value, f32 source_weight,
                    u8 mix_mode)
{
	f32 result;
	switch (mix_mode) {
	case MIX_MODE_LOCAL:
		return local_value;
	case MIX_MODE_SOURCE:
		return source_value;
	case MIX_MODE_BLEND:
		result = local_value * (1.0f - source_weight)
		         + source_value * source_weight;
	}
	return result;
}

s32 TTrack::rootCallback(void* param)
{
	TTrack* self = static_cast<TTrack*>(param);
	if (self && self->mSeqState != SEQ_INACTIVE) {
		if (self->mSeqState == SEQ_STOP_PENDING) {
			self->stopSeqMain();
			return -1;
		} else {
			self->mTickAccumulator += self->mTicksPerSubframe;

			if (self->mTickAccumulator < 1.0f) {
				self->updateSeq(0, true);
			} else {
				while (self->mTickAccumulator >= 1.0f) {
					self->mTickAccumulator -= 1.0f;
					if ((int)self->mainProc() != TRACK_FINISHED)
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
