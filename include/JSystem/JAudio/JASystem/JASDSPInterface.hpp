#ifndef JASDSPINTERFACE_HPP
#define JASDSPINTERFACE_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Driver {
	struct Wave_;
}

namespace DSPInterface {
	struct FxlineConfig_;

	class DSPBuffer {
	public:
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
	};

	class FXBuffer {
	public:
		void setFXLine(s16*, FxlineConfig_*);
		void changeFXLineParam(u8, u32);
	};

	extern u32 SEND_TABLE;
	extern u32 JAS_DSP_PREFIX;
	extern u32 CH_BUF;
	extern u32 FX_BUF;

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
} // namespace DSPInterface

} // namespace JASystem

#endif // JASDSPINTERFACE_HPP
