#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <JSystem/JAudio/JASystem/JASDSPChannel.hpp>
#include <JSystem/JAudio/JASystem/JASChAllocQueue.hpp>
#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>
#include <JSystem/JAudio/JASystem/JASChGlobal.hpp>
#include <JSystem/JAudio/JASystem/JASDriverIF.hpp>
#include <JSystem/JAudio/JASystem/JASDriverTables.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <types.h>

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
		DSPInterface::DSPBuffer* buf = channel->unk20->unkC;
		if (channel->unkD0) {
			for (u8 i = 0; i < 6; ++i)
				buf->setMixerVolumeOnly(i, channel->unkB4[i]);
			buf->setPitch(channel->unk98);
			buf->setPauseFlag(channel->unk2);
		} else {
			for (u8 i = 0; i < 6; ++i)
				buf->setMixerVolume(i, channel->unkB4[i],
				                    channel->unk4->unk5A[i]);

			buf->setPitch(channel->unk98);

			if (channel->unk4->unk61 & 0x20)
				buf->setIIRFilterParam(channel->unk4->unk3C);
			if (channel->unk4->unk61 & 0x1)
				buf->setFIR8FilterParam(channel->unk4->unk2C);

			buf->setFilterMode(channel->unk4->unk61);
			buf->setDistFilter(channel->unk4->unk4C);
			buf->setPauseFlag(channel->unk2);
		}
	}

	static void __UpdateJcToDSPInit(TChannel* channel)
	{
		DSPInterface::DSPBuffer* buf = channel->unk20->unkC;

		if (channel->unkA8[0].mWhole == 0xffff) {
			buf->initAutoMixer();
		} else {
			buf->setMixerInitDelayMax(channel->unk4->unk60);
			for (u8 i = 0; i < 6; ++i)
				buf->setMixerInitVolume(i, channel->unkB4[i],
				                        channel->unk4->unk5A[i]);
		}

		buf->setPitch(channel->unk98);
		if (channel->unk4->unk61 & 0x20)
			buf->setIIRFilterParam(channel->unk4->unk3C);
		if (channel->unk4->unk61 & 0x1)
			buf->setFIR8FilterParam(channel->unk4->unk2C);

		buf->setFilterMode(channel->unk4->unk61);
		buf->setPauseFlag(channel->unk2);
	}

	static u32 extraUpdate(TChannel* channel, u32 param)
	{
		if (channel->unk9A != 0) {
			f32 f31           = channel->unk94 - channel->unk50;
			u8 updateInterval = Driver::getUpdateInterval();
			if (channel->unk9A <= updateInterval) {
				channel->unk9A = 1;
			}
			f31 /= channel->unk9A;
			channel->unk50 += f31 * updateInterval;
			if ((channel->unk9A - updateInterval) <= 0) {
				channel->unk9A = 0;
			} else {
				channel->unk9A -= updateInterval;
			}
			if (channel->unk9A == 0) {
				channel->unk2C = nullptr;
			}
			return 1;
		} else {
			return 0;
		}
	}

	static BOOL updatecallLogicalChannel(TChannel* channel, u32 param)
	{
		TChannelMgr* mgr = channel->unk4;

		if (param == 0) {
			for (u32 i = 0; i < 4; ++i)
				channel->releaseOsc(i);

			if (channel->unk20)
				channel->unk20->unk3 = channel->unkC0 >> 8;

			if (mgr->cutList(channel) != -1) {
				mgr->addListTail(channel, 2);
			}
			channel->unk30 = -1;
			return FALSE;
		} else if (param == 1 || param == 2 || param == 6) {
			if (mgr->unk4 != 0) {
				if (mgr->cutList(channel) != -1) {
					--mgr->unk4;
					if (u32 thing = channel->unkC8) {
						channel->unkC8 = 0;
						mgr->checkLimitStop(channel, thing);
					}
					ChGlobal::release(channel);
				}
			} else {
				if (mgr->cutList(channel) != -1) {
					if (u32 thing = channel->unkC8) {
						channel->unkC8 = 0;
						mgr->checkLimitStop(channel, thing);
					}
					mgr->addListHead(channel, 0);
				}
			}

			if (param != 6)
				channel->stopLogicalChannel();
			else
				DSPQueue::deleteQueue(channel);

			channel->unk1  = 0xFF;
			channel->unk30 = 0xFFFFFFFF;
			channel->unk28 = 0;
		}
		return FALSE;
	}

	static void killBrokenLogicalChannels(TDSPChannel* dspChannel)
	{
		for (u32 i = 0; i < 256; i++) {
			TChannel* channel = ChGlobal::getChannelHandle(i);
			if (channel == nullptr)
				continue;
			if (channel->unk20 != dspChannel)
				continue;

			if (channel->unk4 == nullptr)
				continue;

			TChannelMgr* mgr = channel->unk4;

			channel->stopLogicalChannel();

			if (mgr->cutList(channel) != -1)
				mgr->addListHead(channel, 0);
		}
	}

	static void updateAutoMixer(TChannel* channel, f32 volume, f32 pan,
	                            f32 fxmix, f32 dolby)
	{
		channel->unk20->unkC->setAutoMixer(volume * 32767.5f, pan * 127.5f,
		                                   dolby * 127.5f, fxmix * 127.5f,
		                                   channel->unkA8[1].mWhole);
	}

	static void updateMixer(TChannel* channel, f32 volume, f32 pan, f32 fxmix,
	                        f32 dolby)
	{
		for (u32 i = 0; i < 6; i++) {
			f32 vol = volume;

			TChannel::MixConfig config = channel->unkA8[i];
			if (config.mParts.u == 0) {
				channel->unkB4[i] = 0;
			} else {
				f32 scale;

				if (config.mParts.l0 != 0) {
					switch (config.mParts.l0) {
					case 1:
						scale = pan;
						break;
					case 2:
						scale = fxmix;
						break;
					case 3:
						scale = dolby;
						break;
					case 5:
						scale = 1.0f - pan;
						break;
					case 6:
						scale = 1.0f - fxmix;
						break;
					case 7:
						scale = 1.0f - dolby;
						break;
					}

					vol *= Calc::sinfT(scale);
				}

				if (config.mParts.l1 != 0) {
					switch (config.mParts.l1) {
					case 1:
						scale = pan;
						break;
					case 2:
						scale = fxmix;
						break;
					case 3:
						scale = dolby;
						break;
					case 5:
						scale = 1.0f - pan;
						break;
					case 6:
						scale = 1.0f - fxmix;
						break;
					case 7:
						scale = 1.0f - dolby;
						break;
					}

					switch (config.mParts.l1) {
					case 3:
					case 7:
						vol *= Calc::sinfDolby2(scale);
						break;
					default:
						vol *= Calc::sinfT(scale);
						break;
					}
				}

				if (vol <= 0.0f)
					vol = 0.0f;
				else if (vol >= 1.0f)
					vol = 1.0f;
				channel->unkB4[i] = vol * Driver::getMixerLevel();
			}
		}
	}

	int updatecallDSPChannel(TDSPChannel* dspChannel, u32 param)
	{
		TChannel* channel = dspChannel->getLogicalChannel();
		TChannelMgr* mgr  = channel->unk4;

		u32 i;
		u32 r28 = 0;

		if (channel == nullptr) {
			dspChannel->unk10 = nullptr;
			dspChannel->unk3  = 0;
			killBrokenLogicalChannels(dspChannel);
			return 0;
		}

		if (channel->unk20 != dspChannel) {
			if (channel->unk20 != nullptr
			    && channel == channel->unk20->getLogicalChannel()) {
				killBrokenLogicalChannels(dspChannel);
			} else {
				channel->stopLogicalChannel();
				if (mgr->cutList(channel) != -1) {
					mgr->addListHead(channel, 0);
				}
			}
			dspChannel->forceDelete();
			return 0;
		} else {
			if (param == 2) {
				if (channel->unk28 != nullptr) {
					channel->unk28(channel, 1);
				} else {
					channel->stopLogicalChannel();
					if (mgr->cutList(channel) != -1) {
						mgr->addListHead(channel, 0);
					}
				}
				return 0;
			}

			if (channel->unk10 != nullptr && channel->unk10->unk24[0] == 0) {
				channel->unk20->forceStop();
				return -1;
			}

			if (param == 4) {
				u8 priority = channel->getLifeTimePriority();
				if (channel->unk20 != nullptr) {
					if (priority < channel->unk20->unk3) {
						channel->unk20->unk3 = priority;
					}
				}
				return 0;
			}

			if (param == 3) {
				channel->forceStopOsc(0);
				if (mgr->cutList(channel) == -1) {
					return 1;
				}
				mgr->addListHead(channel, 3);
				param = 0;
			}

			if (param == 0) {
				channel->unk8C         = 1.0f;
				channel->unk90         = 1.0f;
				channel->unk68.mEffect = 0.5f;
				channel->unk74.mEffect = 0.0f;
				channel->unk80.mEffect = 0.0f;

				for (i = 0; i < 4; i++) {
					if (!channel->isOsc(i))
						continue;
					channel->effectOsc(i, channel->bankOscToOfs(i));
					if (i == 0 && channel->getOscState(i) == 0) {
						if (channel->unk28 == nullptr) {
							if (!channel->stopLogicalChannel()) {
								dspChannel->stop();
							}
							if (mgr->cutList(channel) != -1) {
								mgr->addListHead(channel, 0);
							}
							return 0;
						}
						channel->unk28(channel, 2);
						return 0;
					}
					r28++;
				}

				if (r28 != 0) {
					channel->updateEffectorParam();
					channel->unk3 = 1;
				}

				if (channel->unk2C != nullptr) {
					if (channel->unk2C(channel, 0) == 1) {
						channel->unk3++;
					}
				}

				u8 updateInterval = Driver::getUpdateInterval();
				if (channel->unk28 == nullptr) {
					return updateInterval;
				}
				if (channel->unk34 > 0) {
					if (channel->unk34 > updateInterval) {
						channel->unk34 -= updateInterval;
					} else {
						channel->unk34 = 0;
					}
				}
			}

			if (channel->unk34 == 0) {
				channel->unk28(channel, 0);
				channel->unk34 = channel->unk30;
			}
			if (channel->unk3 != 0) {
				channel->updateJcToDSP();
				channel->unk3 = 0;
			}

			return Driver::getUpdateInterval();
		}
		dspChannel->forceDelete();
	}
} // namespace Driver

void TChannel::init()
{
	unk28 = nullptr;
	unk2C = nullptr;
	unk30 = 0;
	unk34 = 0;
	unk10 = nullptr;
	unkC  = 0;
	unk14 = 0;
	unk18 = 0;
	unk1C = 0;
	unkD0 = 0;
	if (!unk4) {
		unkA8[0].mWhole = 0x150;
		unkA8[1].mWhole = 0x210;
		unkA8[2].mWhole = 0x352;
		unkA8[3].mWhole = 0x412;
		unkA8[4].mWhole = 0;
		unkA8[5].mWhole = 0;
		unkC0           = 0x10101;
		unkC4           = 600;
		unk58[0]        = CALC_WeightAll;      // Pan
		unk58[1]        = CALC_AddChannelOnly; // FxMix
		unk58[2]        = CALC_AddChannelOnly;
	} else {
		for (int i = 0; i < 6; i++)
			unkA8[i].mWhole = unk4->unk4E[i];

		unkC0 = unk4->unk68;
		unkC4 = unk4->unk6C;
		for (int i = 0; i < 3; i++)
			unk58[i] = unk4->unk62[i];
	}
	for (u32 i = 0; i < 4; i++) {
		unk38[i]->setOsc(nullptr);
		unk38[i]->init();
	}
	unk2 = 0;
	unkC6++;
	if ((s32)unkC6 == 0)
		unkC6 = 1;
}

void TChannel::setOscillator(u32 index, TOscillator* oscillator)
{
	unk38[index] = oscillator;
}

void TChannel::setOscInit(u32 index, const TOscillator::Osc_* osc)
{
	unk38[index]->setOsc(osc);
	unk38[index]->initStart();
}

bool TChannel::forceStopOsc(u32 index)
{
	return unk38[index]->isOsc() ? unk38[index]->forceStop() : false;
}

bool TChannel::releaseOsc(u32 index)
{
	return unk38[index]->isOsc() ? unk38[index]->release() : false;
}

void TChannel::directReleaseOsc(u32 index, u16 release)
{
	unk38[index]->releaseDirect(release);
}

f32 TChannel::bankOscToOfs(u32 index)
{
	return unk38[index]->isOsc() ? unk38[index]->getOffset() : 1.0f;
}

void TChannel::effectOsc(u32 index, f32 effect)
{
	switch (unk38[index]->getOsc()->unk0) {
	case 1:
		unk8C *= effect;
		break;
	case 0:
		unk90 *= effect;
		break;
	case 2:
		unk68.mEffect = effect;
		break;
	case 3:
		unk74.mEffect = effect;
		break;
	case 4:
		unk80.mEffect = effect;
		break;
	}
}

u8 TChannel::getOscState(u32 index) const { return unk38[index]->mState; }

BOOL TChannel::isOsc(u32 index) { return unk38[index]->isOsc(); }

void TChannel::copyOsc(u32 index, TOscillator::Osc_* dest)
{
	if (isOsc(index)) {
		*dest = *unk38[index]->getOsc();
	}
}

void TChannel::overwriteOsc(u32 index, TOscillator::Osc_* src)
{
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
	s32 thing;

	if (unkC == 2 || unk10 == 0)
		thing = key;
	else
		thing = key + 0x3C - unk10->unk2;

	if (thing < 0)
		thing = 0;
	else if (thing > 0x7F)
		thing = 0x7F;

	f32 val = Driver::C5BASE_PITCHTABLE[thing];
	val *= unk48;
	if (target == 0) {
		unk50 = val;
		unk2C = 0;
		return;
	}

	unk94 = val;
	unk9A = target;
	unk2C = &Driver::extraUpdate;
}

void TChannel::setPauseFlag(u8 flag) { unk2 = flag; }

void TChannel::setPauseFlagReq(u8 flag)
{
	unk2 = flag;
	unk3 = 1;
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
	if (!unk10 && unkC == 0)
		return false;

	return true;
}

bool TChannel::resetInitialVolume()
{
	if (!unk20)
		return false;

	if (!checkLogicalChannel())
		return false;

	updateEffectorParam();
	Driver::__UpdateJcToDSPInit(this);
	unk20->unkC->flushChannel();

	return true;
}

BOOL TChannel::play(u32 param)
{
	if (param == 0) {
		param = -1;
	}
	unk30 = param;
	unk34 = unk30;
	unk28 = &Driver::updatecallLogicalChannel;
	unk20 = TDSPChannel::alloc(0, (u32)this);

	if (unk20 == nullptr) {
		if (checkLogicalChannel() == TRUE) {
			if (Driver::DSPQueue::enQueue(this) != TRUE) {
				unk4->addListTail(this, 0);
				return false;
			} else {
				unk4->addListTail(this, 3);
				return true;
			}
		}
	}

	if (playLogicalChannel() == FALSE) {
		TDSPChannel::free(unk20, (u32)this);
		unk20 = nullptr;
		unk4->addListTail(this, 0);
		return false;
	} else {
		unk4->addListTail(this, 1);
		return true;
	}
}

void TChannel::stop(u16 release)
{
	if (unk20 == 0)
		Driver::updatecallLogicalChannel(this, 6);
	else if (release == 0)
		Driver::updatecallLogicalChannel(this, 0);
	else {
		unk38[0]->releaseDirect(release);
		Driver::updatecallLogicalChannel(this, 0);
	}
}

void TChannel::updateJcToDSP()
{
	Driver::__UpdateJcToDSP(this);
	unk20->unkC->flushChannel();
}

BOOL TChannel::forceStopLogicalChannel()
{
	if (!unk20)
		return FALSE;

	unk20->forceStop();
	return TRUE;
}

BOOL TChannel::stopLogicalChannel()
{
	if (!unk20)
		return FALSE;

	unk20->unk10 = 0;
	unk20->unk6  = 0;
	unk20->stop();
	TDSPChannel::free(unk20, (u32)this);
	unk20 = nullptr;

	return TRUE;
}

BOOL TChannel::playLogicalChannel()
{
	if (!unk20)
		return FALSE;

	if (!checkLogicalChannel())
		return FALSE;

	unk20->unk10 = &Driver::updatecallDSPChannel;
	unk20->unk6  = 1;

	DSPInterface::DSPBuffer* buf = unk20->unkC;

	switch (unkC) {
	case 0:
		buf->setWaveInfo(unk10, unk14);
		break;
	case 2:
		buf->setOscInfo(unk14);
		break;
	}

	for (u8 i = 0; i < 6; ++i) {
		// Something ungodly is occurring here.

		union {
			u16 asS16;
			struct {
				u8 hi;
				u8 lo;
			} asP;
		} s;
		s.asS16 = unkA8[i].mWhole;
		u32 om  = Driver::getOutputMode();

		if (om == 0) {
			switch (s.asP.hi) {
			case 8:
				s.asP.hi = 0xB;
				break;
			case 9:
				s.asP.hi = 0x2;
				break;
			}
		} else if (om == 1) {
			if (s.asP.hi == 8)
				s.asP.hi = 0xB;
		}

		buf->setBusConnect(i, s.asP.hi);
	}

	unk9C = unk4;

	for (u32 i = 0; i < 4; ++i)
		if (unk38[i]->isOsc())
			effectOsc(i, unk38[i]->isOsc() ? unk38[i]->getOffset() : 1.0f);

	updateEffectorParam();
	Driver::__UpdateJcToDSPInit(this);
	unk20->unk3 = unkC0;
	unk20->unk4 = unkC4;
	unk20->play();

	return TRUE;
}

// fabricated
static inline f32 clamp01(f32 value)
{
	if (value <= 0.0f)
		return 0.0f;
	else if (value >= 1.0f)
		return 1.0f;
	else
		return value;
}

void TChannel::updateEffectorParam()
{
	f32 pan;
	f32 fxmix;
	f32 dolby = 0.0f;

	if (unk9C == unk4) {
		unkA0          = unk4->mPitch;
		unkA4          = unk4->mVolume;
		unk68.mChannel = unk4->mPan;
		unk74.mChannel = unk4->mFxmix;
		unk80.mChannel = unk4->mDolby;
		for (int i = 0; i < 3; i++)
			unk58[i] = unk4->unk62[i];
	}

	switch (Driver::getOutputMode()) {
	case 0:
		pan   = 0.5f;
		dolby = 0.0f;
		fxmix = calcEffect(&unk74, &unk5C, unk58[1]);
		break;
	case 1:
		if (unk58[0] == CALC_None)
			pan = 0.5f;
		else
			pan = calcPan(&unk68, &unk5C, unk58[0]);
		fxmix = calcEffect(&unk74, &unk5C, unk58[1]);
		dolby = 0.0f;
		break;
	case 2:
		if (unk58[0] == CALC_None)
			pan = 0.5f;
		else
			pan = calcPan(&unk68, &unk5C, unk58[0]);
		fxmix = calcEffect(&unk74, &unk5C, unk58[1]);
		dolby = calcEffect(&unk80, &unk5C, unk58[2]);
		break;
	}

	f32 volume = unkA4 * (unk50 * unk90);

	pan   = clamp01(pan);
	fxmix = clamp01(fxmix);
	dolby = clamp01(dolby);

	unk98 = 4096.0f * (unkA0 * (unk50 * unk8C));

	if (unkA8[0].mWhole != 0xFFFF) {
		Driver::updateMixer(this, volume, pan, fxmix, dolby);
	} else {
		Driver::updateAutoMixer(this, volume, pan, fxmix, dolby);
	}
}

} // namespace JASystem
