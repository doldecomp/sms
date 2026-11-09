#ifndef JASDSPINTERFACE_HPP
#define JASDSPINTERFACE_HPP

#include <dolphin/types.h>
#include <stdint.h>

namespace JASystem {

namespace Driver {
	struct Wave_;
}

namespace DSPInterface {
	struct FxlineConfig_ {
		/* 0x00 */ u8 mFlags;
		/* 0x02 */ u16 mSend0Bus;
		/* 0x04 */ s16 mSend0Gain;
		/* 0x06 */ u16 mSend1Bus;
		/* 0x08 */ s16 mSend1Gain;
		/* 0x0C */ int mDelayBlocks;
		/* 0x10 */ s16 mFirCoeffs[8];
	};

	/**
	 * @brief A single voice as seen and processed by the DSP microcode.
	 * The struct gets DMA'd to the DSP as-is and some fields are only used on
	 * the DSP side, while some are shared between the CPU and the DSP.
	 */
	struct DSPBuffer {
		enum SampleSourceType {
			SOURCE_SQUARE                  = 0x00,
			SOURCE_SAW                     = 0x01,
			SOURCE_PHASE_MODULATED_SQUARE  = 0x02,
			SOURCE_QUARTER_DUTY_SQUARE     = 0x03,
			SOURCE_PATTERN_1               = 0x04,
			SOURCE_AFC_LQ                  = 0x05,
			SOURCE_CONSTANT                = 0x06,
			SOURCE_PATTERN_0               = 0x07,
			SOURCE_PCM8_ARAM               = 0x08,
			SOURCE_AFC_HQ                  = 0x09,
			SOURCE_PHASE_MODULATED_PATTERN = 0x0A,
			SOURCE_PATTERN_2               = 0x0B,
			SOURCE_PATTERN_3               = 0x0C,
			SOURCE_PCM16_ARAM              = 0x10,
			SOURCE_PCM16_MRAM_DIRECT       = 0x20,
			SOURCE_PCM16_MRAM_RESAMPLED    = 0x21,
		};

		void flushChannel();
		void allocInit();
		void initFilter();
		void setAutoMixer(u16, u8, u8, u8, u8);
		void setMixerVolumeOnly(u8, s16);
		void setPitch(u16);
		void setPauseFlag(u8);
		void setMixerVolume(u8, s16, u8);
		void setIIRFilterParam(s16*);
		void setFIR8FilterParam(s16*);
		void setFilterMode(u16);
		void setDistFilter(s16);
		void setWaveInfo(Driver::Wave_* wave, uintptr_t samples_address);
		void setOscInfo(u32);
		void setBusConnect(u8, u8);
		void initAutoMixer();
		void setMixerInitDelayMax(u8);
		void setMixerInitVolume(u8, s16, u8);
		void playStart();
		void playStop();
		void updateAMVolume(u16);
		void updateAMPan(u8, u8);
		void updateAMFX(u8);
		void setPitchIndirect(f32, f32);
		void cacheChannel();

		bool isFinish() { return done != 0; }
		void replyFinishRequest()
		{
			done    = 0;
			enabled = 0;
		}

		struct Channel {
			/* 0x0 */ u16 id;
			/* 0x2 */ u16 targetVolume;
			/* 0x4 */ u16 currentVolume;
			/* 0x6 */ u16 level;
		};

		/* 0x000 */ u16 enabled;
		/* 0x002 */ u16 done;
		/* 0x004 */ u16 resamplingRatio;
		/* 0x006 */ char pad6[0x8 - 0x6];
		/* 0x008 */ u16 resetVpb;
		/* 0x00A */ u16 endReached;
		/* 0x00C */ u16 useConstantSample;
		/* 0x00E */ u16 samplesToKeepCount;
		/* 0x010 */ Channel mixChannels[6];
		/* 0x040 */ char pad40[0x50 - 0x40];
		/* 0x050 */ u16 dolbyVoicePosition;
		/* 0x052 */ u16 dolbyReverbFactor;
		/* 0x054 */ u16 dolbyVolumeCurrent;
		/* 0x056 */ u16 dolbyVolumeTarget;
		/* 0x058 */ u16 useDolbyVolume;
		/* 0x05A */ u16 useDolbyVolumeCopy;
		/* 0x05C */ char pad5C[0x60 - 0x5C];
		/* 0x060 */ u16 currentPosFrac;
		/* 0x062 */ u16 afcDecodedSampleIndex;
		/* 0x064 */ u16 afcRemainingDecodedSamples;
		/* 0x066 */ s16 constantSample;
		/* 0x068 */ int currentPosition;
		/* 0x06C */ u32 samplesBeforeLoop;
		/* 0x070 */ u32 currentAramAddr;
		/* 0x074 */ u32 remainingLength;
		/* 0x078 */ s16 resampleBuffer[4];
		/* 0x080 */ s16 variableFirHistory[20];
		/* 0x0A8 */ s16 biquadHistory[4];
		/* 0x0B0 */ union {
			/* 0x0B0 */ s16 afcRemainingSamples[16];
			struct {
				/* 0x0B0 */ s16 samples[14];
				/* 0x0CC */ s16 yn2;
				/* 0x0CE */ s16 yn1;
			} afcHistory;
		};
		/* 0x0D0 */ s16 mixerDelayHistory[16];
		/* 0x0F0 */ s16 lowPassHistory[2];
		/* 0x0F4 */ char padF4[0x100 - 0xF4];
		/* 0x100 */ u16 samplesSourceType;
		/* 0x102 */ u16 isLooping;
		/* 0x104 */ s16 loopYN1;
		/* 0x106 */ s16 loopYN2;
		/* 0x108 */ s16 filterMode;
		/* 0x10A */ u16 endRequested;
		/* 0x10C */ u32 updateCount;
		/* 0x110 */ s16* loopAddress;
		/* 0x114 */ u32 endPosition;
		/* 0x118 */ s16* baseAddress;
		/* 0x11C */ int sampleCount;
		/* 0x120 */ s16 variableFirCoeffs[20];
		/* 0x148 */ s16 biquadFilterCoeffs[4];
		/* 0x150 */ s16 lowPassCoeff;
		/* 0x152 */ char pad152[0x180 - 0x152];
	};

	class FXBuffer {
	public:
		BOOL setFXLine(s16*, FxlineConfig_*);
		void changeFXLineParam(u8, u32);

	public:
		/* 0x00 */ u16 mFlags;
		/* 0x02 */ u16 mDelayBlocks;
		/* 0x04 */ s16* mDelayBuffer;
		/* 0x08 */ u16 mSend0Address;
		/* 0x0A */ s16 mSend0Gain;
		/* 0x0C */ u16 mSend1Address;
		/* 0x0E */ s16 mSend1Gain;
		/* 0x10 */ s16 mFirCoeffs[8];
	};

	extern u16 JAS_DSP_PREFIX;

	DSPBuffer* getDSPHandle(u8);
	DSPBuffer* getDSPHandleNc(u8);
	FXBuffer* getFXHandle(u8);
	FXBuffer* getFXHandleNc(u8);
	void setFilterTable(s16*, s16*, u32);
	void flushBuffer();
	void flushChannelAll();
	void cacheChannelAll();
	void invalChannelAll();
	void clearBuffer();
	void setupBuffer();
	void initBuffer();

	// TODO: TWW also lists boot(void (*)(void*)), finishWork(u16), restart()
	// and sync(u32, u32, u32). No body is known for those yet.
	inline void allocInit(u8 i) { getDSPHandle(i)->allocInit(); }
	inline void playStart(u8 i) { getDSPHandle(i)->playStart(); }
	inline void playStop(u8 i) { getDSPHandle(i)->playStop(); }
	inline void flushChannel(u8 i) { getDSPHandle(i)->flushChannel(); }
	inline void cacheChannel(u8 i) { getDSPHandle(i)->cacheChannel(); }
	inline void initFilter(u8 i) { getDSPHandle(i)->initFilter(); }
	inline void initAutoMixer(u8 i) { getDSPHandle(i)->initAutoMixer(); }

	inline void setPitch(u8 i, u16 pitch) { getDSPHandle(i)->setPitch(pitch); }
	inline void setPitchIndirect(u8 i, f32 pitch, f32 rate)
	{
		getDSPHandle(i)->setPitchIndirect(pitch, rate);
	}
	inline void setPauseFlag(u8 i, u8 flag)
	{
		getDSPHandle(i)->setPauseFlag(flag);
	}
	inline void setOscInfo(u8 i, u32 waveform_type)
	{
		getDSPHandle(i)->setOscInfo(waveform_type);
	}
	inline void setWaveInfo(u8 i, Driver::Wave_* wave, u32 base)
	{
		getDSPHandle(i)->setWaveInfo(wave, base);
	}
	inline void setBusConnect(u8 i, u8 mixer, u8 bus)
	{
		getDSPHandle(i)->setBusConnect(mixer, bus);
	}

	inline void setMixerInitDelayMax(u8 i, u8 delay)
	{
		getDSPHandle(i)->setMixerInitDelayMax(delay);
	}
	inline void setMixerInitVolume(u8 i, u8 mixer, s16 volume, u8 level)
	{
		getDSPHandle(i)->setMixerInitVolume(mixer, volume, level);
	}
	inline void setMixerVolume(u8 i, u8 mixer, s16 volume, u8 level)
	{
		getDSPHandle(i)->setMixerVolume(mixer, volume, level);
	}
	inline void setMixerVolumeOnly(u8 i, u8 mixer, s16 volume)
	{
		getDSPHandle(i)->setMixerVolumeOnly(mixer, volume);
	}
	inline void setAutoMixer(u8 i, u16 volume, u8 pan, u8 fxmix, u8 dolby,
	                         u8 delay)
	{
		getDSPHandle(i)->setAutoMixer(volume, pan, fxmix, dolby, delay);
	}
	inline void updateAMVolume(u8 i, u16 volume)
	{
		getDSPHandle(i)->updateAMVolume(volume);
	}
	inline void updateAMPan(u8 i, u8 pan, u8 dolby)
	{
		getDSPHandle(i)->updateAMPan(pan, dolby);
	}
	inline void updateAMFX(u8 i, u8 fxmix)
	{
		getDSPHandle(i)->updateAMFX(fxmix);
	}

	inline void setFilterMode(u8 i, u16 mode)
	{
		getDSPHandle(i)->setFilterMode(mode);
	}
	inline void setDistFilter(u8 i, s16 dist)
	{
		getDSPHandle(i)->setDistFilter(dist);
	}
	inline void setIIRFilterParam(u8 i, s16* param)
	{
		getDSPHandle(i)->setIIRFilterParam(param);
	}
	inline void setFIR8FilterParam(u8 i, s16* param)
	{
		getDSPHandle(i)->setFIR8FilterParam(param);
	}

	inline bool setFXLine(u8 i, s16* buffer, FxlineConfig_* config)
	{
		return getFXHandle(i)->setFXLine(buffer, config);
	}
} // namespace DSPInterface

} // namespace JASystem

#endif // JASDSPINTERFACE_HPP
