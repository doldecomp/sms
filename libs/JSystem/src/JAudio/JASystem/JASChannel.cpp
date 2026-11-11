#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <JSystem/JAudio/JASystem/JASDSPChannel.hpp>
#include <JSystem/JAudio/JASystem/JASChAllocQueue.hpp>
#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>
#include <JSystem/JAudio/JASystem/JASChGlobal.hpp>
#include <JSystem/JAudio/JASystem/JASDriverIF.hpp>
#include <JSystem/JAudio/JASystem/JASDriverTables.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <dolphin/types.h>

namespace JASystem {

namespace Driver {

	enum CalcSource {
		CALC_Sound   = 0,
		CALC_Effect  = 1,
		CALC_Channel = 2,
	};

	enum CalcStyle {
		CALC_NONE   = 0, // don't add that component
		CALC_ADD    = 1, // simply add that component
		CALC_WEIGHT = 2, // add component weighted by power
	};

	// clang-format off
	static u8 calc_sw_table[27][3] = {
		// sound, effect, channel
		{ CALC_NONE, CALC_NONE, CALC_NONE },       // 0, null
		{ CALC_NONE, CALC_NONE, CALC_ADD },        // 1, add only channel
		{ CALC_NONE, CALC_NONE, CALC_ADD },        // 2, add only channel
		{ CALC_NONE, CALC_ADD, CALC_NONE },        // 3, add only effect
		{ CALC_NONE, CALC_ADD, CALC_ADD },         // 4, add effect and channel
		{ CALC_NONE, CALC_ADD, CALC_WEIGHT },      // 5, add effect, weight channel
		{ CALC_NONE, CALC_ADD, CALC_NONE },        // 6, add only effect
		{ CALC_NONE, CALC_WEIGHT, CALC_ADD },      // 7, weight effect, add channel
		{ CALC_NONE, CALC_WEIGHT, CALC_WEIGHT },   // 8, weight effect and channel
		{ CALC_ADD, CALC_NONE, CALC_NONE },        // 9, add only sound
		{ CALC_ADD, CALC_NONE, CALC_ADD },         // 10, add sound and channel
		{ CALC_ADD, CALC_NONE, CALC_WEIGHT },      // 11, add sound, weight channel
		{ CALC_ADD, CALC_ADD, CALC_NONE },         // 12, add sound and effect
		{ CALC_ADD, CALC_ADD, CALC_ADD },          // 13, add all
		{ CALC_ADD, CALC_ADD, CALC_WEIGHT },       // 14, add sound and effect, weight channel
		{ CALC_ADD, CALC_WEIGHT, CALC_NONE },      // 15, add sound, weight effect
		{ CALC_ADD, CALC_WEIGHT, CALC_ADD },       // 16, add sound and channel, weight effect
		{ CALC_ADD, CALC_WEIGHT, CALC_WEIGHT },    // 17, add sound, weight effect and channel
		{ CALC_ADD, CALC_NONE, CALC_NONE },        // 18, add only sound
		{ CALC_WEIGHT, CALC_NONE, CALC_ADD },      // 19, weight sound, add channel
		{ CALC_WEIGHT, CALC_NONE, CALC_WEIGHT },   // 20, weight sound and channel
		{ CALC_WEIGHT, CALC_ADD, CALC_NONE },      // 21, weight sound, add effect
		{ CALC_WEIGHT, CALC_ADD, CALC_ADD },       // 22, weight sound, add effect and channel
		{ CALC_WEIGHT, CALC_ADD, CALC_WEIGHT },    // 23, weight sound and channel, weight effect
		{ CALC_WEIGHT, CALC_WEIGHT, CALC_NONE },   // 24, weight sound and effect
		{ CALC_WEIGHT, CALC_WEIGHT, CALC_ADD },    // 25, weight sound and effect, add channel
		{ CALC_WEIGHT, CALC_WEIGHT, CALC_WEIGHT }, // 26, weight all
	};
	// clang-format on

	static f32 calcEffect(const PanMatrix_* params, const PanMatrix_* power,
	                      u8 calcType)
	{
		f32 value           = 0.0f;
		const u8* calcTypes = Driver::calc_sw_table[calcType];
		switch (calcTypes[CALC_Sound]) {
		case CALC_NONE:
			break;
		case CALC_ADD:
			value += params->mSound;
			break;
		case CALC_WEIGHT:
			value += params->mSound * power->mSound;
			break;
		}

		switch (calcTypes[CALC_Effect]) {
		case CALC_NONE:
			break;
		case CALC_ADD:
			value += params->mEffect;
			break;
		case CALC_WEIGHT:
			value += params->mEffect * power->mEffect;
			break;
		}

		switch (calcTypes[CALC_Channel]) {
		case CALC_NONE:
			break;
		case CALC_ADD:
			value += params->mChannel;
			break;
		case CALC_WEIGHT:
			value += params->mChannel * power->mChannel;
			break;
		}

		return value;
	}

	static f32 calcPan(const PanMatrix_* params, const PanMatrix_* power,
	                   u8 calcType)
	{
		f32 value           = 0.0f;
		const u8* calcTypes = Driver::calc_sw_table[calcType];
		switch (calcTypes[CALC_Sound]) {
		case CALC_NONE:
			break;
		case CALC_ADD:
			value += (params->mSound - 0.5f);
			break;
		case CALC_WEIGHT:
			value += (params->mSound - 0.5f) * power->mSound;
			break;
		}

		switch (calcTypes[CALC_Effect]) {
		case CALC_NONE:
			break;
		case CALC_ADD:
			value += (params->mEffect - 0.5f);
			break;
		case CALC_WEIGHT:
			value += (params->mEffect - 0.5f) * power->mEffect;
			break;
		}

		switch (calcTypes[CALC_Channel]) {
		case CALC_NONE:
			break;
		case CALC_ADD:
			value += (params->mChannel - 0.5f);
			break;
		case CALC_WEIGHT:
			value += (params->mChannel - 0.5f) * power->mChannel;
			break;
		}

		value += 0.5f;
		return value;
	}

	static void __UpdateJcToDSP(TChannel* channel)
	{
		DSPInterface::DSPBuffer* buf = channel->mDspChannel->mDSPHandle;
		if (channel->mPreserveDspConfig) {
			for (u8 i = 0; i < 6; ++i)
				buf->setMixerVolumeOnly(i, channel->mMixerVolumes[i]);
			buf->setPitch(channel->mDspPitch);
			buf->setPauseFlag(channel->mPauseFlag);
		} else {
			for (u8 i = 0; i < 6; ++i)
				buf->setMixerVolume(i, channel->mMixerVolumes[i],
				                    channel->mManager->mMixerDelaySamples[i]);

			buf->setPitch(channel->mDspPitch);

			if (channel->mManager->mFilterMode
			    & DSPInterface::DSPBuffer::FILTER_IIR_ENABLED)
				buf->setIIRFilterParam(channel->mManager->mIirCoeffs);
			if (channel->mManager->mFilterMode
			    & DSPInterface::DSPBuffer::FILTER_FIR_TAP_COUNT_MASK)
				buf->setFIR8FilterParam(channel->mManager->mFirCoeffs);

			buf->setFilterMode(channel->mManager->mFilterMode);
			buf->setDistFilter(channel->mManager->mDistFilter);
			buf->setPauseFlag(channel->mPauseFlag);
		}
	}

	static void __UpdateJcToDSPInit(TChannel* channel)
	{
		DSPInterface::DSPBuffer* buf = channel->mDspChannel->mDSPHandle;

		if (channel->isDolbyMode()) {
			channel->mDspChannel->mDSPHandle->initAutoMixer();
		} else {
			buf->setMixerInitDelayMax(channel->mManager->mMixerDelayMax);
			for (u8 i = 0; i < 6; ++i)
				buf->setMixerInitVolume(
				    i, channel->mMixerVolumes[i],
				    channel->mManager->mMixerDelaySamples[i]);
		}

		buf->setPitch(channel->mDspPitch);
		if (channel->mManager->mFilterMode
		    & DSPInterface::DSPBuffer::FILTER_IIR_ENABLED)
			buf->setIIRFilterParam(channel->mManager->mIirCoeffs);
		if (channel->mManager->mFilterMode
		    & DSPInterface::DSPBuffer::FILTER_FIR_TAP_COUNT_MASK)
			buf->setFIR8FilterParam(channel->mManager->mFirCoeffs);

		buf->setFilterMode(channel->mManager->mFilterMode);
		buf->setPauseFlag(channel->mPauseFlag);
	}

	static u32 extraUpdate(TChannel* channel, u32 param)
	{
		if (channel->mPitchSweepRemaining != 0) {
			f32 f31 = channel->mPitchSweepTarget - channel->mNotePitch;
			u8 updateInterval = Driver::getUpdateInterval();
			if (channel->mPitchSweepRemaining <= updateInterval) {
				channel->mPitchSweepRemaining = 1;
			}
			f31 /= channel->mPitchSweepRemaining;
			channel->mNotePitch += f31 * updateInterval;
			if ((channel->mPitchSweepRemaining - updateInterval) <= 0) {
				channel->mPitchSweepRemaining = 0;
			} else {
				channel->mPitchSweepRemaining -= updateInterval;
			}
			if (channel->mPitchSweepRemaining == 0) {
				channel->mUpdateCallback = nullptr;
			}
			return TRUE;
		} else {
			return FALSE;
		}
	}

	static BOOL updatecallLogicalChannel(TChannel* channel, u32 event)
	{
		TChannelMgr* mgr = channel->mManager;

		if (event == TChannel::EVENT_RELEASE) {
			for (u32 i = 0; i < 4; ++i)
				channel->releaseOsc(i);

			if (channel->mDspChannel)
				channel->mDspChannel->mPriority = channel->getReleasePriority();

			if (mgr->cutList(channel) != -1) {
				mgr->addListTail(channel, TChannelMgr::CHAN_LIST_RELEASE);
			}
			channel->mGateTime = -1;
			return FALSE;
		} else if (event == TChannel::EVENT_DSP_FINISHED
		           || event == TChannel::EVENT_ENVELOPE_FINISHED
		           || event == TChannel::EVENT_CANCELLED) {
			if (mgr->mGlobalChannelBorrows != 0) {
				if (mgr->cutList(channel) != -1) {
					--mgr->mGlobalChannelBorrows;
					if (channel->mPolyphonyKey) {
						u32 polyphonyKey       = channel->mPolyphonyKey;
						channel->mPolyphonyKey = 0;
						mgr->checkLimitStop(channel, polyphonyKey);
					}
					ChGlobal::release(channel);
				}
			} else {
				if (mgr->cutList(channel) != -1) {
					if (channel->mPolyphonyKey) {
						u32 polyphonyKey       = channel->mPolyphonyKey;
						channel->mPolyphonyKey = 0;
						mgr->checkLimitStop(channel, polyphonyKey);
					}
					mgr->addListHead(channel, TChannelMgr::CHAN_LIST_FREE);
				}
			}

			if (event != TChannel::EVENT_CANCELLED)
				channel->stopLogicalChannel();
			else
				DSPQueue::deleteQueue(channel);

			channel->mKey           = 0xFF;
			channel->mGateTime      = -1;
			channel->mEventCallback = nullptr;
		}
		return FALSE;
	}

	static void killBrokenLogicalChannels(TDSPChannel* dspChannel)
	{
		for (u32 i = 0; i < 256; i++) {
			TChannel* channel = ChGlobal::getChannelHandle(i);
			if (channel == nullptr)
				continue;
			if (channel->mDspChannel != dspChannel)
				continue;

			if (channel->mManager == nullptr)
				continue;

			TChannelMgr* mgr = channel->mManager;

			channel->stopLogicalChannel();

			if (mgr->cutList(channel) != -1)
				mgr->addListHead(channel, TChannelMgr::CHAN_LIST_FREE);
		}
	}

	static void updateAutoMixer(TChannel* channel, f32 volume, f32 pan,
	                            f32 fxmix, f32 dolby)
	{
		channel->mDspChannel->mDSPHandle->setAutoMixer(
		    volume * 32767.5f, pan * 127.5f, dolby * 127.5f, fxmix * 127.5f,
		    channel->mMixConfigs[1].mWhole);
	}

	static void updateMixer(TChannel* channel, f32 volume, f32 pan, f32 fxmix,
	                        f32 dolby)
	{
		f32 vol;
		f32 invPan   = 1.0f - pan;
		f32 invFxmix = 1.0f - fxmix;
		f32 invDolby = 1.0f - dolby;

		for (u32 i = 0; i < 6; i++) {
			vol = volume;

			TChannel::MixConfig config = channel->mMixConfigs[i];
			if (config.mParts.mSendDestination
			    == DSPInterface::DSPBuffer::SEND_NONE) {
				channel->mMixerVolumes[i] = 0;
			} else {
				f32 modulation;

				if (config.mParts.mGainSource0
				    != TChannel::MixConfig::GAIN_NONE) {
					switch (config.mParts.mGainSource0) {
					case TChannel::MixConfig::GAIN_PAN:
						modulation = pan;
						break;
					case TChannel::MixConfig::GAIN_FXMIX:
						modulation = fxmix;
						break;
					case TChannel::MixConfig::GAIN_DOLBY:
						modulation = dolby;
						break;
					case TChannel::MixConfig::GAIN_INV_PAN:
						modulation = invPan;
						break;
					case TChannel::MixConfig::GAIN_INV_FXMIX:
						modulation = invFxmix;
						break;
					case TChannel::MixConfig::GAIN_INV_DOLBY:
						modulation = invDolby;
						break;
					}

					vol *= Calc::sinfT(modulation);
				}

				if (config.mParts.mGainSource1
				    != TChannel::MixConfig::GAIN_NONE) {
					switch (config.mParts.mGainSource1) {
					case TChannel::MixConfig::GAIN_PAN:
						modulation = pan;
						break;
					case TChannel::MixConfig::GAIN_FXMIX:
						modulation = fxmix;
						break;
					case TChannel::MixConfig::GAIN_DOLBY:
						modulation = dolby;
						break;
					case TChannel::MixConfig::GAIN_INV_PAN:
						modulation = invPan;
						break;
					case TChannel::MixConfig::GAIN_INV_FXMIX:
						modulation = invFxmix;
						break;
					case TChannel::MixConfig::GAIN_INV_DOLBY:
						modulation = invDolby;
						break;
					}

					switch (config.mParts.mGainSource1) {
					case TChannel::MixConfig::GAIN_DOLBY:
					case TChannel::MixConfig::GAIN_INV_DOLBY:
						vol *= Calc::sinfDolby2(modulation);
						break;
					default:
						vol *= Calc::sinfT(modulation);
						break;
					}
				}

				if (vol <= 0.0f)
					vol = 0.0f;
				else if (vol >= 1.0f)
					vol = 1.0f;
				channel->mMixerVolumes[i] = vol * Driver::getMixerLevel();
			}
		}
	}

	int updatecallDSPChannel(TDSPChannel* dspChannel, u32 event)
	{
		TChannel* channel = dspChannel->getLogicalChannel();
		TChannelMgr* mgr  = channel->mManager;

		u32 i;
		u32 r28 = 0;

		// Ultra mega hack #1
		if (channel == nullptr) {
			dspChannel->mCallback = nullptr;
			dspChannel->setPriority(0);
			killBrokenLogicalChannels(dspChannel);
			return 0;
		}

		// Ultra mega hack #2
		if (channel->mDspChannel != dspChannel) {
			if (channel->mDspChannel != nullptr
			    && channel == channel->mDspChannel->getLogicalChannel()) {
				killBrokenLogicalChannels(dspChannel);
			} else {
				channel->stopLogicalChannel();
				if (mgr->cutList(channel) != -1)
					mgr->addListHead(channel, TChannelMgr::CHAN_LIST_FREE);
			}
			dspChannel->forceDelete();
			return 0;
		}

		if (event == TDSPChannel::EVENT_FINISHED) {
			if (channel->mEventCallback != nullptr) {
				channel->mEventCallback(channel, TChannel::EVENT_DSP_FINISHED);
			} else {
				channel->stopLogicalChannel();
				if (mgr->cutList(channel) != -1)
					mgr->addListHead(channel, TChannelMgr::CHAN_LIST_FREE);
			}
			return 0;
		}

		if (channel->mWaveData != nullptr
		    && channel->mWaveData->mLoadFlagPtr[0] == 0) {
			channel->mDspChannel->forceStop();
			return -1;
		}

		if (event == TDSPChannel::EVENT_PRIORITY_EXPIRED) {
			u8 priority = channel->getLifeTimePriority();
			if (channel->mDspChannel != nullptr)
				if (priority < channel->mDspChannel->mPriority)
					channel->mDspChannel->mPriority = priority;
			return 0;
		}

		if (event == TDSPChannel::EVENT_FORCE_STOP) {
			channel->forceStopOsc(0);
			if (mgr->cutList(channel) == -1)
				return 1;

			mgr->addListHead(channel, TChannelMgr::CHAN_LIST_PENDING);
			event = TDSPChannel::EVENT_UPDATE;
		}

		if (event == TDSPChannel::EVENT_UPDATE) {
			channel->mOscPitch        = 1.0f;
			channel->mOscVolume       = 1.0f;
			channel->unk68[0].mEffect = 0.5f;
			channel->unk68[1].mEffect = 0.0f;
			channel->unk68[2].mEffect = 0.0f;

			for (i = 0; i < 4; i++) {
				if (!channel->isOsc(i))
					continue;
				channel->effectOsc(i, channel->bankOscToOfs(i));
				if (i == 0
				    && channel->getOscState(i) == TOscillator::STATE_STOPPED) {
					if (channel->mEventCallback == nullptr) {
						if (!channel->stopLogicalChannel()) {
							dspChannel->stop();
						}
						if (mgr->cutList(channel) != -1) {
							mgr->addListHead(channel,
							                 TChannelMgr::CHAN_LIST_FREE);
						}
						return 0;
					}
					channel->mEventCallback(channel,
					                        TChannel::EVENT_ENVELOPE_FINISHED);
					return 0;
				}
				r28++;
			}

			if (r28 != 0) {
				channel->updateEffectorParam();
				channel->mDspUpdatePending = 1;
			}

			if (channel->mUpdateCallback != nullptr)
				if (channel->mUpdateCallback(channel, 0) == TRUE)
					channel->mDspUpdatePending++;

			u8 updateInterval = Driver::getUpdateInterval();
			if (channel->mEventCallback == nullptr)
				return updateInterval;

			if (channel->mGateTimeRemaining > 0) {
				if (channel->mGateTimeRemaining > updateInterval)
					channel->mGateTimeRemaining -= updateInterval;
				else
					channel->mGateTimeRemaining = 0;
			}
		}

		if (channel->mGateTimeRemaining == 0) {
			channel->mEventCallback(channel, TChannel::EVENT_RELEASE);
			channel->mGateTimeRemaining = channel->mGateTime;
		}

		if (channel->mDspUpdatePending != 0) {
			channel->updateJcToDSP();
			channel->mDspUpdatePending = 0;
		}

		return Driver::getUpdateInterval();
	}
} // namespace Driver

void TChannel::init()
{
	mEventCallback     = nullptr;
	mUpdateCallback    = nullptr;
	mGateTime          = 0;
	mGateTimeRemaining = 0;
	mWaveData          = nullptr;
	mLogicalChanType   = 0;
	mSourceData        = 0;
	unk18              = 0;
	unk1C              = 0;
	mPreserveDspConfig = FALSE;
	if (!mManager) {
		mMixConfigs[0].mWhole = (DSPInterface::DSPBuffer::SEND_FRONT_LEFT << 8)
		                        | (MixConfig::GAIN_INV_PAN << 4)
		                        | MixConfig::GAIN_NONE;
		mMixConfigs[1].mWhole = (DSPInterface::DSPBuffer::SEND_FRONT_RIGHT << 8)
		                        | (MixConfig::GAIN_PAN << 4)
		                        | MixConfig::GAIN_NONE;
		mMixConfigs[2].mWhole = (DSPInterface::DSPBuffer::SEND_EFFECTS_0 << 8)
		                        | (MixConfig::GAIN_INV_PAN << 4)
		                        | MixConfig::GAIN_FXMIX;
		mMixConfigs[3].mWhole = (DSPInterface::DSPBuffer::SEND_EFFECTS_1 << 8)
		                        | (MixConfig::GAIN_PAN << 4)
		                        | MixConfig::GAIN_FXMIX;
		mMixConfigs[4].mWhole = DSPInterface::DSPBuffer::SEND_NONE;
		mMixConfigs[5].mWhole = DSPInterface::DSPBuffer::SEND_NONE;
		mPriorityConfig       = 0x10101;
		mPriorityTime         = 600;
		mCalcTypes[0]         = CALC_WeightAll;      // Pan
		mCalcTypes[1]         = CALC_AddChannelOnly; // FxMix
		mCalcTypes[2]         = CALC_AddChannelOnly;
	} else {
		for (int i = 0; i < 6; i++)
			mMixConfigs[i].mWhole = mManager->mMixConfig[i];

		mPriorityConfig = mManager->mPriorityConfig;
		mPriorityTime   = mManager->mPriorityTime;
		mCalcTypes[0]   = mManager->mCalcTypes[0];
		mCalcTypes[1]   = mManager->mCalcTypes[1];
		mCalcTypes[2]   = mManager->mCalcTypes[2];
	}
	for (u32 i = 0; i < 4; i++) {
		JUT_ASSERT(mOscillators[i]);
		mOscillators[i]->setOsc(nullptr);
		mOscillators[i]->init();
	}
	mPauseFlag = 0;
	mGeneration++;
	if ((s32)mGeneration == 0)
		mGeneration = 1;
}

void TChannel::setOscillator(u32 index, TOscillator* oscillator)
{
	JUT_ASSERT(index < 4);
	mOscillators[index] = oscillator;
}

void TChannel::setOscInit(u32 index, const TOscillator::Osc_* osc)
{
	JUT_ASSERT(index < 4);
	mOscillators[index]->setOsc(osc);
	mOscillators[index]->initStart();
}

bool TChannel::forceStopOsc(u32 index)
{
	JUT_ASSERT(index < 4);
	return mOscillators[index]->isOsc() ? mOscillators[index]->forceStop()
	                                    : false;
}

bool TChannel::releaseOsc(u32 index)
{
	JUT_ASSERT(index < 4);
	return mOscillators[index]->isOsc() ? mOscillators[index]->release()
	                                    : false;
}

void TChannel::directReleaseOsc(u32 index, u16 release)
{
	JUT_ASSERT(index < 4);
	mOscillators[index]->releaseDirect(release);
}

f32 TChannel::bankOscToOfs(u32 index)
{
	JUT_ASSERT(index < 4);
	return mOscillators[index]->isOsc() ? mOscillators[index]->bankOscToOfs()
	                                    : 1.0f;
}

void TChannel::effectOsc(u32 index, f32 effect)
{
	JUT_ASSERT(index < 4);
	switch (mOscillators[index]->getEffectorKind()) {
	case 1:
		mOscPitch *= effect;
		break;
	case 0:
		mOscVolume *= effect;
		break;
	case 2:
		unk68[0].mEffect = effect;
		break;
	case 3:
		unk68[1].mEffect = effect;
		break;
	case 4:
		unk68[2].mEffect = effect;
		break;
	}
}

u8 TChannel::getOscState(u32 index) const
{
	JUT_ASSERT(index < 4);
	return mOscillators[index]->getOscMode();
}

BOOL TChannel::isOsc(u32 index)
{
	JUT_ASSERT(index < 4);
	return mOscillators[index]->isOsc();
}

void TChannel::copyOsc(u32 index, TOscillator::Osc_* dest)
{
	JUT_ASSERT(index < 4);
	if (isOsc(index))
		*dest = *mOscillators[index]->getOsc();
}

void TChannel::overwriteOsc(u32 index, TOscillator::Osc_* src)
{
	JUT_ASSERT(index < 4);
	setOscInit(index, src);
	effectOsc(index, bankOscToOfs(index));
}

void TChannel::overwriteOscMultiple(TOscillator::Osc_* osc1,
                                    TOscillator::Osc_* osc2,
                                    TOscillator::Osc_* osc3,
                                    TOscillator::Osc_* osc4)
{
}

void TChannel::initOscForOscSound() { }

void TChannel::setKeySweepTarget(u8 key, u32 target)
{
	s32 pitchIdx;

	if (mLogicalChanType == 2 || mWaveData == 0)
		pitchIdx = key;
	else
		pitchIdx = key + 0x3C - mWaveData->mKey;

	if (pitchIdx < 0)
		pitchIdx = 0;
	else if (pitchIdx > 0x7F)
		pitchIdx = 0x7F;

	f32 pitch = Driver::C5BASE_PITCHTABLE[pitchIdx];
	pitch *= mBasePitch;
	if (target == 0) {
		mNotePitch      = pitch;
		mUpdateCallback = 0;
		return;
	}

	mPitchSweepTarget    = pitch;
	mPitchSweepRemaining = target;
	mUpdateCallback      = &Driver::extraUpdate;
}

void TChannel::setPauseFlag(u8 flag) { mPauseFlag = flag; }

void TChannel::setPauseFlagReq(u8 flag)
{
	mPauseFlag        = flag;
	mDspUpdatePending = 1;
}

void TChannel::setPanPower(f32 fl, f32 fr, f32 rl, f32 rr)
{
	f32 px = fl + fr + rl;
	if (px == 0.0f)
		px = 1.0f;

	unk5C.mSound   = fl / px;
	unk5C.mEffect  = fr / px;
	unk5C.mChannel = rl / px;
}

void TChannel::setPanParam(const f32* pan, const f32* dolby, const f32* fx) { }

void TChannel::setFxParam(const f32* pan, const f32* dolby, const f32* fx) { }

void TChannel::setDolbyParam(const f32* pan, const f32* dolby, const f32* fx) {
}

BOOL TChannel::checkLogicalChannel()
{
	if (!mWaveData && mLogicalChanType == 0)
		return false;

	return true;
}

bool TChannel::resetInitialVolume()
{
	if (!mDspChannel)
		return false;

	if (!checkLogicalChannel())
		return false;

	updateEffectorParam();
	Driver::__UpdateJcToDSPInit(this);
	mDspChannel->mDSPHandle->flushChannel();

	return true;
}

BOOL TChannel::play(u32 gate_time)
{
	if (gate_time == 0)
		gate_time = -1;

	mGateTime          = gate_time;
	mGateTimeRemaining = mGateTime;
	mEventCallback     = &Driver::updatecallLogicalChannel;
	mDspChannel        = TDSPChannel::alloc(0, (uintptr_t)this);

	if (mDspChannel == nullptr) {
		if (checkLogicalChannel() == TRUE) {
			if (Driver::DSPQueue::enQueue(this) != TRUE) {
				mManager->addListTail(this, TChannelMgr::CHAN_LIST_FREE);
				return false;
			} else {
				mManager->addListTail(this, TChannelMgr::CHAN_LIST_PENDING);
				return true;
			}
		}
	}

	if (playLogicalChannel() == FALSE) {
		TDSPChannel::free(mDspChannel, (uintptr_t)this);
		mDspChannel = nullptr;
		mManager->addListTail(this, TChannelMgr::CHAN_LIST_FREE);
		return false;
	} else {
		mManager->addListTail(this, TChannelMgr::CHAN_LIST_PLAYING);
		return true;
	}
}

void TChannel::stop(u16 release)
{
	if (mDspChannel == 0)
		Driver::updatecallLogicalChannel(this, EVENT_CANCELLED);
	else if (release == 0)
		Driver::updatecallLogicalChannel(this, EVENT_RELEASE);
	else {
		mOscillators[0]->releaseDirect(release);
		Driver::updatecallLogicalChannel(this, EVENT_RELEASE);
	}
}

void TChannel::updateJcToDSP()
{
	Driver::__UpdateJcToDSP(this);
	mDspChannel->mDSPHandle->flushChannel();
}

BOOL TChannel::forceStopLogicalChannel()
{
	if (!mDspChannel)
		return FALSE;

	mDspChannel->forceStop();
	return TRUE;
}

BOOL TChannel::stopLogicalChannel()
{
	if (!mDspChannel)
		return FALSE;

	mDspChannel->mCallback   = nullptr;
	mDspChannel->mCBInterval = 0;
	mDspChannel->stop();
	TDSPChannel::free(mDspChannel, (uintptr_t)this);
	mDspChannel = nullptr;

	return TRUE;
}

BOOL TChannel::playLogicalChannel()
{
	if (!mDspChannel)
		return FALSE;

	if (!checkLogicalChannel())
		return FALSE;

	mDspChannel->mCallback   = &Driver::updatecallDSPChannel;
	mDspChannel->mCBInterval = 1;

	DSPInterface::DSPBuffer* buf = mDspChannel->mDSPHandle;

	switch (mLogicalChanType) {
	case 0:
		buf->setWaveInfo(mWaveData, mSourceData);
		break;
	case 2:
		buf->setOscInfo(mSourceData);
		break;
	}

	for (u8 i = 0; i < 6; ++i) {
		MixConfig config = mMixConfigs[i];
		u32 om           = Driver::getOutputMode();

		if (om == JASOUTPUT_Mono) {
			switch (config.mParts.mSendDestination) {
			case DSPInterface::DSPBuffer::SEND_REAR_LEFT:
				config.mParts.mSendDestination
				    = DSPInterface::DSPBuffer::SEND_AUX_CENTER;
				break;
			case DSPInterface::DSPBuffer::SEND_AUX_RIGHT_SUBTRACT:
				config.mParts.mSendDestination
				    = DSPInterface::DSPBuffer::SEND_FRONT_RIGHT;
				break;
			}
		} else if (om == JASOUTPUT_Stereo) {
			if (config.mParts.mSendDestination
			    == DSPInterface::DSPBuffer::SEND_REAR_LEFT)
				config.mParts.mSendDestination
				    = DSPInterface::DSPBuffer::SEND_AUX_CENTER;
		}

		buf->setBusConnect(i, config.mParts.mSendDestination);
	}

	mInitialManager = mManager;

	for (u32 i = 0; i < 4; ++i)
		if (mOscillators[i]->isOsc())
			effectOsc(i, bankOscToOfs(i));

	updateEffectorParam();
	Driver::__UpdateJcToDSPInit(this);
	mDspChannel->setPriority(getNoteOnPriority());
	mDspChannel->setPriorityTime(mPriorityTime);
	mDspChannel->play();

	return TRUE;
}

void TChannel::updateEffectorParam()
{
	f32 pan;
	f32 fxmix;
	f32 dolby = 0.0f;

	if (mInitialManager == mManager) {
		mManagerPitch     = mManager->mPitch;
		mManagerVolume    = mManager->mVolume;
		unk68[0].mChannel = mManager->mPan;
		unk68[1].mChannel = mManager->mFxmix;
		unk68[2].mChannel = mManager->mDolby;
		for (int i = 0; i < 3; i++)
			mCalcTypes[i] = mManager->mCalcTypes[i];
	}

	switch (Driver::getOutputMode()) {
	case JASOUTPUT_Mono:
		pan   = 0.5f;
		dolby = 0.0f;
		fxmix = calcEffect(&unk68[1], &unk5C, mCalcTypes[1]);
		break;
	case JASOUTPUT_Stereo:
		if (mCalcTypes[0] == CALC_None)
			pan = 0.5f;
		else
			pan = calcPan(&unk68[0], &unk5C, mCalcTypes[0]);
		fxmix = calcEffect(&unk68[1], &unk5C, mCalcTypes[1]);
		dolby = 0.0f;
		break;
	case JASOUTPUT_Surround:
		if (mCalcTypes[0] == CALC_None)
			pan = 0.5f;
		else
			pan = calcPan(&unk68[0], &unk5C, mCalcTypes[0]);
		fxmix = calcEffect(&unk68[1], &unk5C, mCalcTypes[1]);
		dolby = calcEffect(&unk68[2], &unk5C, mCalcTypes[2]);
		break;
	}

	f32 volume = mManagerVolume * (mNoteVolume * mOscVolume);

	pan   = Driver::Clamp01(pan);
	fxmix = Driver::Clamp01(fxmix);
	dolby = Driver::Clamp01(dolby);

	mDspPitch = 4096.0f * (mManagerPitch * (mNotePitch * mOscPitch));

	if (mMixConfigs[0].mWhole != 0xFFFF) {
		Driver::updateMixer(this, volume, pan, fxmix, dolby);
	} else {
		Driver::updateAutoMixer(this, volume, pan, fxmix, dolby);
	}
}

} // namespace JASystem
