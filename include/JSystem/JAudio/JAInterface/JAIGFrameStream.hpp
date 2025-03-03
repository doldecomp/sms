#ifndef JAIINTERFACE_JAIGFRAMESTREAM_HPP
#define JAIINTERFACE_JAIGFRAMESTREAM_HPP

#include <dolphin/dvd.h>
#include <JSystem/JAudio/JASystem/JASKernel.h>

namespace JAInter {
namespace StreamLib {
	// Functions
	void stop();
	void setOutputMode(unsigned long);
	void setPauseFlag(unsigned char);
	void clearPauseFlag(unsigned char);
	void setVolume(float);
	void setPitch(float);
	void setPan(float);
	void setPrepareFlag(unsigned char);
	unsigned char getPrepareFlag();
	void start(char*, unsigned long, void*);
	void callBack(void*);
	void __start();
	void LoadADPCM();
	void __LoadFin(long, DVDFileInfo*);
	void __DecodePCM();
	void __DecodeADPCM();
	void Play_DirectPCM(JASystem::TDSPChannel*, short*, unsigned short,
	                    unsigned long);
	void init(bool);
	long getNeedBufferSize();
	void allocBuffer(void*, long);
	void deallocBuffer();
	bool checkBufferStatus();
	void setBufferMode(bool);
	void sync(long);
	void Hvqm_SetAudioDmaBuffers(unsigned long);
	void __Decode();
	bool getPlayingFlag();
	void setDecodedBufferBlocks(unsigned long);
	void LoopInit();
	unsigned long Get_DirectPCM_LoopRemain(JASystem::DSPInterface::DSPBuffer*);
	unsigned long Get_DirectPCM_Counter(JASystem::DSPInterface::DSPBuffer*);
	unsigned long Get_DirectPCM_Remain(JASystem::DSPInterface::DSPBuffer*);

	// Variables
	extern unsigned char stopflag;
	extern unsigned char stopflag2;
	extern unsigned long outputmode;
	extern unsigned char outpause;
	extern unsigned char playflag2;
	extern float stackvolume;
	extern unsigned char outflag_volume;
	extern float stackpitch;
	extern unsigned char outflag_pitch;
	extern float stackpan;
	extern unsigned char outflag_pan;
	extern unsigned char prepareflag;
	extern unsigned char startInitFlag;
	extern char* Filename;
	extern DVDFileInfo* finfo;
	extern void* header;
	extern unsigned char DvdLoadFlag;
	extern unsigned char playflag;
	extern unsigned long Mode;
	extern unsigned long playmode;
	extern JASystem::TDSPChannel* assign_ch;
	extern unsigned long Head;
	extern void* adpcm_buffer;
	extern unsigned long adpcm_loadpoint;
	extern unsigned long adpcm_remain;
	extern unsigned long playback_samples;
	extern unsigned long LOOP_BLOCKS;
	extern float outvolume;
	extern float outpitch;
	extern float outpan;
	extern unsigned long loadup_samples;
	extern unsigned long movieframe;
	extern unsigned char loop_start_flag;
	extern unsigned char adpcmbuf_state;
	extern unsigned char playside;
	extern unsigned long shift_sample;
	extern unsigned long LOOP_SAMPLESIZE;
	extern unsigned long loadsize;
	extern unsigned long extra_sample;
	extern unsigned char dspch_deallockflag;
	extern void* loop_buffer;
	extern short* filter_table;
	extern short* table4;
	extern void* store_buffer;
	extern bool bufferMode;
	extern bool allocFlag;
	extern JASystem::Kernel::TSolidHeap* streamHeap;
} // namespace StreamLib
} // namespace JAInter

#endif // JAIINTERFACE_JAIGFRAMESTREAM_HPP
