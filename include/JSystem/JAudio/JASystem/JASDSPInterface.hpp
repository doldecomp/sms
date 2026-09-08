#ifndef JASDSPINTERFACE_HPP
#define JASDSPINTERFACE_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Driver {
	struct Wave_;
}

namespace DSPInterface {
	struct FxlineConfig_ {
		/* 0x00 */ u8 unk0;
		/* 0x02 */ u16 unk2;
		/* 0x04 */ s16 unk4;
		/* 0x06 */ u16 unk6;
		/* 0x08 */ s16 unk8;
		/* 0x0C */ int unkC;
		/* 0x10 */ s16 unk10[8];
	};

	struct DSPBuffer {
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
		void setWaveInfo(Driver::Wave_*, u32);
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

		// Names stolen from Dolphin, see
		// https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/HW/DSPHLE/UCodes/Zelda.cpp#L682

		/* 0x000 */ u16 enabled;
		/* 0x002 */ u16 done;
		/* 0x004 */ u16 resamplingRatio;
		/* 0x006 */ u8 unk6[0x8 - 0x6];
		/* 0x008 */ u16 resetVpb;
		/* 0x00A */ u16 endReached;
		/* 0x00C */ u16 useConstantSample;
		/* 0x00E */ u16 samplesToKeepCount;
		/* 0x010 */ Channel mixChannels[6];
		/* 0x040 */ u8 unk40[0x50 - 0x40];
		/* 0x050 */ u16 dolbyVoicePosition;
		/* 0x052 */ u16 dolbyReverbFactor;
		/* 0x054 */ u16 dolbyVolumeCurrent;
		/* 0x056 */ u16 dolbyVolumeTarget;
		/* 0x058 */ u16 useDolbyVolume;
		/* 0x05A */ u8 unk5A[0x60 - 0x5a];
		/* 0x060 */ s16 currentPosFrac;
		/* 0x062 */ u8 unk62[0x64 - 0x62];
		/* 0x064 */ s16 afcRemainingDecodedSamples;
		/* 0x066 */ s16 constantSample;
		/* 0x068 */ int currentPosition;
		/* 0x06C */ u32 samplesBeforeLoop;
		/* 0x070 */ u32 currentAramAddr;
		/* 0x074 */ u32 remainingLength;
		/* 0x078 */ s16 resampleBuffer[4];
		/* 0x080 */ u16 variableFirHistory[20];
		/* 0x0A8 */ s16 biquadHistory[4];
		/* 0x0B0 */ u16 afcRemainingSamples[16];
		/* 0x0D0 */ s16 lowPassHistory[2];
		/* 0x0D4 */ u8 unkD4[0x100 - 0xd4];
		/* 0x100 */ u16 samplesSourceType;
		/* 0x102 */ u16 isLooping;
		/* 0x104 */ s16 loopYN1;
		/* 0x106 */ s16 loopYN2;
		/* 0x108 */ s16 filterMode;
		/* 0x10A */ u16 endRequested;
		/* 0x10C */ u32 unk10C;
		/* 0x110 */ s16* loopAddress;
		/* 0x114 */ u32 loopStartPosition;
		/* 0x118 */ s16* baseAddress;
		/* 0x11C */ int unk11C;
		/* 0x120 */ s16 variableFirCoeffs[20];
		/* 0x148 */ s16 biquadFilterCoeffs[4];
		/* 0x150 */ s16 lowPassCoeff;
		/* 0x152 */ u8 padding[0x180 - 0x152];
	};

	class FXBuffer {
	public:
		BOOL setFXLine(s16*, FxlineConfig_*);
		void changeFXLineParam(u8, u32);

	public:
		/* 0x00 */ u16 unk0;
		/* 0x02 */ u16 unk2;
		/* 0x04 */ s16* unk4;
		/* 0x08 */ u16 unk8;
		/* 0x0A */ s16 unkA;
		/* 0x0C */ u16 unkC;
		/* 0x0E */ s16 unkE;
		/* 0x10 */ s16 unk10[8];
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
	inline void setOscInfo(u8 i, u32 type)
	{
		getDSPHandle(i)->setOscInfo(type);
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
