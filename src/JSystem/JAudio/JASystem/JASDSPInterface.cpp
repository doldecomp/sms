#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASDriverTables.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/dspproc.h>
#include <dolphin/os.h>

u64 DSP_CreateMap()
{
	u64 var1 = 0;
	for (u8 i = 0; i < 64; i++) {
		var1 <<= 1;

		JASystem::DSPInterface::DSPBuffer* buffer
		    = JASystem::DSPInterface::getDSPHandle(i);
		if (buffer->enabled) {
			var1 |= 1;
		}
	}
	return var1;
}

namespace JASystem {

namespace DSPInterface {

	static u16 SEND_TABLE[] = {
		0x0D00, 0x0D60, 0x0DC0, 0x0E20, 0x0E80, 0x0EE0,
		0x0CA0, 0x0F40, 0x0FA0, 0x0B00, 0x09A0, 0x0000,
	};

	static DSPBuffer* CH_BUF = 0;
	static FXBuffer* FX_BUF  = 0;

	u16 JAS_DSP_PREFIX = 0xF355;

	DSPBuffer* getDSPHandle(u8 i) { return &CH_BUF[i]; }
	// "Nc" is the uncached view of the same buffer: the DSP writes these
	// structures behind the CPU's cache, so a reader that must not see a stale
	// line goes through the 0xC0000000 mirror. One instruction over the cached
	// accessor is what both map sizes ask for (0x18 here, 0x14 for the FX one).
	DSPBuffer* getDSPHandleNc(u8 i)
	{
		return (DSPBuffer*)OSCachedToUncached(&CH_BUF[i]);
	}

	void setFilterTable(s16* dst, s16* src, u32 size)
	{
		for (u32 i = 0; i < size; i++)
			*dst++ = *src++;
	}
	void flushBuffer()
	{
		DCFlushRange(CH_BUF, sizeof(DSPBuffer) * 64);
		DCFlushRange(FX_BUF, sizeof(FXBuffer) * 4);
	}
	void flushChannelAll() { DCFlushRange(CH_BUF, sizeof(DSPBuffer) * 64); }
	void cacheChannelAll() { }
	void invalChannelAll()
	{
		DCInvalidateRange(CH_BUF, sizeof(DSPBuffer) * 64);
	}
	void clearBuffer()
	{
		for (u32 i = 0; i < 64; i++)
			Calc::bzero(CH_BUF + i, sizeof(DSPBuffer));

		for (u32 i = 0; i < 4; i++)
			Calc::bzero(FX_BUF + i, sizeof(FXBuffer));
	}
	void setupBuffer()
	{
		DsetupTable(64, (u32)CH_BUF, (u32)&DSPRES_FILTER, (u32)&DSPADPCM_FILTER,
		            (u32)FX_BUF);
	}
	void initBuffer()
	{
		CH_BUF = new (JASDram, 0x20) DSPBuffer[64];
		FX_BUF = new (JASDram, 0x20) FXBuffer[4];

		for (u8 i = 0; i < 4; i++)
			FX_BUF[i].setFXLine(nullptr, nullptr);

		clearBuffer();
		setupBuffer();
		flushBuffer();
	}

	FXBuffer* getFXHandle(u8 i) { return &FX_BUF[i]; }
	FXBuffer* getFXHandleNc(u8 i)
	{
		return (FXBuffer*)OSCachedToUncached(&FX_BUF[i]);
	}

	BOOL FXBuffer::setFXLine(s16* buffer, FxlineConfig_* config)
	{
		BOOL enable = OSDisableInterrupts();
		unk0        = 0;

		if (config) {
			unkA = config->unk4;
			unk8 = SEND_TABLE[config->unk2];
			unkE = config->unk8;
			unkC = SEND_TABLE[config->unk6];
			unk2 = config->unkC;
			setFilterTable(unk10, config->unk10, 8);
		}

		if (buffer && config) {
			u32 bufsize = config->unkC * 0xA0;
			unk4        = buffer;
			Calc::bzero(buffer, bufsize);
			DCFlushRange(buffer, bufsize);
		} else if (!config || buffer) {
			unk4 = buffer;
		}

		if (unk4)
			unk0 = config->unk0;
		else
			unk0 = 0;

		DCFlushRange(this, sizeof(*this));
		OSRestoreInterrupts(enable);
		return true;
	}
	void FXBuffer::changeFXLineParam(u8, u32) { }

	void DSPBuffer::allocInit()
	{
		useConstantSample = 0;
		done              = 0;
		endRequested      = 0;
		enabled           = 0;
		useDolbyVolume    = 0;
		initFilter();
		flushChannel();
	}
	void DSPBuffer::playStart()
	{
		unk10C          = 0;
		currentPosition = 0;
		currentPosFrac  = 0;
		resetVpb        = 1;
		constantSample  = 0;

		s32 i;
		for (i = 0; i < 4; ++i) {
			resampleBuffer[i] = 0;
			biquadHistory[i]  = 0;
		}

		for (i = 0; i < 20; ++i)
			variableFirHistory[i] = 0;

		enabled = 1;
	}
	void DSPBuffer::playStop() { enabled = 0; }
	void DSPBuffer::setWaveInfo(Driver::Wave_* param_1, u32 param_2)
	{
		static u8 COMP_BLOCKSAMPLES[8] = {
			0x10, 0x10, 0x01, 0x01, 0x01, 0x10, 0x10, 0x01,
		};
		static u8 COMP_BLOCKBYTES[8] = {
			0x09, 0x05, 0x08, 0x10, 0x01, 0x01, 0x01, 0x01,
		};

		baseAddress                = (s16*)param_2;
		afcRemainingDecodedSamples = COMP_BLOCKSAMPLES[param_1->unk1];
		samplesSourceType          = COMP_BLOCKBYTES[param_1->unk1];

		if (samplesSourceType < 4)
			return;

		unk11C    = param_1->unk1C;
		isLooping = param_1->unk10;

		if (isLooping != 0) {
			loopAddress       = (s16*)param_1->unk14;
			loopStartPosition = param_1->unk18;
			loopYN1           = param_1->unk20;
			loopYN2           = param_1->unk22;
		} else {
			loopStartPosition = unk11C;
		}

		for (s32 i = 0; i < 16; i++)
			afcRemainingSamples[i] = 0;
	}
	void DSPBuffer::setOscInfo(u32 param_1)
	{
		baseAddress                = 0;
		afcRemainingDecodedSamples = 16;
		samplesSourceType          = param_1;
	}
	void DSPBuffer::initAutoMixer()
	{
		if (useDolbyVolume) {
			dolbyVolumeCurrent = dolbyVolumeTarget;
			return;
		}
		dolbyVolumeCurrent = 0;
		useDolbyVolume     = 1;
	}
	void DSPBuffer::setAutoMixer(u16 param_1, u8 param_2, u8 param_3,
	                             u8 param_4, u8 param_5)
	{
		dolbyVoicePosition = param_2 << 8 | param_3;
		dolbyReverbFactor  = param_4 << 8;
		dolbyVolumeTarget  = param_1;
		useDolbyVolume     = 1;
	}
	void DSPBuffer::updateAMVolume(u16 param_1) { dolbyVolumeTarget = param_1; }
	void DSPBuffer::updateAMPan(u8 param_1, u8 param_2)
	{
		dolbyVoicePosition = param_1 << 8 | param_2;
	}
	void DSPBuffer::updateAMFX(u8 param_1) { dolbyReverbFactor = param_1 << 8; }
	void DSPBuffer::setPitch(u16 param_1)
	{
		if (param_1 >= 0x7fff)
			param_1 = 0x7fff;

		resamplingRatio = param_1;
	}
	void DSPBuffer::setPitchIndirect(f32 param_1, f32 param_2) { }

	void DSPBuffer::setMixerInitDelayMax(u8 param_1)
	{
		samplesToKeepCount = param_1;
	}
	void DSPBuffer::setMixerInitVolume(u8 param_1, s16 param_2, u8 param_3)
	{
		Channel& chan      = mixChannels[param_1];
		chan.currentVolume = param_2;
		chan.targetVolume  = param_2;
		chan.level         = param_3 << 8 | param_3;
	}
	void DSPBuffer::setMixerVolume(u8 param_1, s16 param_2, u8 param_3)
	{
		if (endRequested)
			return;

		Channel& chan     = mixChannels[param_1];
		chan.targetVolume = param_2;
		chan.level        = param_3 << 8 | chan.level & 0xff;
	}
	void DSPBuffer::setMixerVolumeOnly(u8 param_1, s16 param_2)
	{
		if (endRequested)
			return;

		mixChannels[param_1].targetVolume = param_2;
	}
	void DSPBuffer::setPauseFlag(u8 flag) { useConstantSample = flag; }
	void DSPBuffer::flushChannel() { DCFlushRangeNoSync(this, sizeof(*this)); }
	void DSPBuffer::cacheChannel() { }

	void DSPBuffer::setIIRFilterParam(s16* param)
	{
		setFilterTable(biquadFilterCoeffs, param, 4);
	}
	void DSPBuffer::setFIR8FilterParam(s16* param)
	{
		setFilterTable(variableFirCoeffs, param, 8);
	}

	void DSPBuffer::setFilterMode(u16 param)
	{
		u8 r30 = param & 0x20;
		u8 r31 = param & 0x1f;
		if (r30) {
			if (r31 > 0x14) {
				r31 = 0x14;
			}
		} else {
			if (r31 > 0x18) {
				r31 = 0x18;
			}
		}
		filterMode = r30 + r31;
	}

	void DSPBuffer::initFilter()
	{
		for (int i = 0; i < 8; ++i)
			variableFirCoeffs[i] = 0;

		variableFirCoeffs[0] = 0x7FFF;

		for (int i = 0; i < 4; ++i)
			biquadFilterCoeffs[i] = 0;

		biquadFilterCoeffs[0] = 0x7FFF;

		lowPassCoeff = 0;
	}

	void DSPBuffer::setDistFilter(s16 value) { lowPassCoeff = value; }

	void DSPBuffer::setBusConnect(u8 param_1, u8 param_2)
	{
		static u16 connect_table[12] = {
			0x0000, 0x0D00, 0x0D60, 0x0DC0, 0x0E20, 0x0E80,
			0x0EE0, 0x0CA0, 0x0F40, 0x0FA0, 0x0B00, 0x09A0,
		};
		Channel& chan = mixChannels[param_1];
		chan.id       = connect_table[param_2];
	}

} // namespace DSPInterface

} // namespace JASystem
