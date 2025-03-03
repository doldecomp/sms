#ifndef JASDSPINTERFACE_HPP
#define JASDSPINTERFACE_HPP

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
		void setAutoMixer(unsigned short, unsigned char, unsigned char,
		                  unsigned char, unsigned char);
		void setMixerVolumeOnly(unsigned char, short);
		void setPitch(unsigned short);
		void setPauseFlag(unsigned char);
		void setMixerVolume(unsigned char, short, unsigned char);
		void setIIRFilterParam(short*);
		void setFIR8FilterParam(short*);
		void setFilterMode(unsigned short);
		void setDistFilter(short);
		void setWaveInfo(Driver::Wave_*, unsigned long);
		void setOscInfo(unsigned long);
		void setBusConnect(unsigned char, unsigned char);
		void initAutoMixer();
		void setMixerInitDelayMax(unsigned char);
		void setMixerInitVolume(unsigned char, short, unsigned char);
		void playStart();
		void playStop();
		void updateAMVolume(unsigned short);
		void updateAMPan(unsigned char, unsigned char);
		void updateAMFX(unsigned char);
		void setPitchIndirect(float, float);
		void cacheChannel();
	};

	class FXBuffer {
	public:
		void setFXLine(short*, FxlineConfig_*);
		void changeFXLineParam(unsigned char, unsigned long);
	};

	extern unsigned long SEND_TABLE;
	extern unsigned long JAS_DSP_PREFIX;
	extern unsigned long CH_BUF;
	extern unsigned long FX_BUF;

	DSPBuffer* getDSPHandle(unsigned char);
	DSPBuffer* getDSPHandleNc(unsigned char);
	FXBuffer* getFXHandle(unsigned char);
	FXBuffer* getFXHandleNc(unsigned char);
	void setFilterTable(short*, short*, unsigned long);
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
