#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <JSystem/JAudio/JASystem/JASDSPChannel.hpp>
#include <JSystem/JAudio/JASystem/JASChAllocQueue.hpp>
#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>
#include <JSystem/JAudio/JASystem/JASChGlobal.hpp>
#include <JSystem/JAudio/JASystem/JASDriverIF.hpp>
#include <JSystem/JAudio/JASystem/JASDriverTables.hpp>
#include <types.h>

namespace JASystem {

namespace Driver {
	// TODO: This is definitely the wrong data type
	u8 calc_sw_table[81]
	    = { 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 2, 0, 1, 0,
		    0, 2, 1, 0, 2, 2, 1, 0, 0, 1, 0, 1, 1, 0, 2, 1, 1, 0, 1, 1, 1,
		    1, 1, 2, 1, 2, 0, 1, 2, 1, 1, 2, 2, 1, 0, 0, 2, 0, 1, 2, 0, 2,
		    2, 1, 0, 2, 1, 1, 2, 1, 2, 2, 2, 0, 2, 2, 1, 2, 2, 2 };

	static void calcEffect(const PanMatrix_* matrix1, const PanMatrix_* matrix2,
	                       u8 param)
	{
	}

	static void calcPan(const PanMatrix_* matrix1, const PanMatrix_* matrix2,
	                    u8 param)
	{
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

		if (channel->unkA8[0] == 0xffff) {
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

	static void extraUpdate(TChannel* channel, u32 param) { }

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

	static void killBrokenLogicalChannels(TDSPChannel* dspChannel) { }

	static void updateAutoMixer(TChannel* channel, f32 fl, f32 fr, f32 rl,
	                            f32 rr)
	{
	}

	static void updateMixer(TChannel* channel, f32 fl, f32 fr, f32 rl, f32 rr)
	{
	}

	int updatecallDSPChannel(TDSPChannel* dspChannel, u32 param) { }
} // namespace Driver

void TChannel::init() { }

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
		unk6C = effect;
		break;
	case 3:
		unk78 = effect;
		break;
	case 4:
		unk84 = effect;
		break;
	}
}

u32 TChannel::getOscState(u32 index) const { return unk38[index]->mState; }

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

	f32 val = Driver::C5BASE_PITCHTABLE[thing] * unk48;
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

	unk5C = fl / px;
	unk60 = fr / px;
	unk64 = rl / px;
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

bool TChannel::play(u32 param)
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
		s.asS16 = unkA8[i];
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

void TChannel::updateEffectorParam() { }

} // namespace JASystem
