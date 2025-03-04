#ifndef JAIINTER_HPP
#define JAIINTER_HPP

#include <types.h>
#include <dolphin/dvd.h>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <JSystem/JAudio/JASystem/JASDSPChannel.hpp>

namespace JAInter {
namespace StreamLib {
	void stop();
	extern bool stopflag;
	extern bool stopflag2;
	void setOutputMode(u32 mode);
	extern u32 outputmode;
	void setPauseFlag(u8 flag);
	extern u8 outpause;
	void clearPauseFlag(u8 flag);
	bool getPlayingFlag();
	extern bool playflag2;
	void setVolume(f32 volume);
	extern f32 stackvolume;
	extern bool outflag_volume;
	void setPitch(f32 pitch);
	extern f32 stackpitch;
	extern bool outflag_pitch;
	void setPan(f32 pan);
	extern f32 stackpan;
	extern bool outflag_pan;
	void setPrepareFlag(u8 flag);
	extern u8 prepareflag;
	void start(char* filename, u32 mode, void* param);
	extern bool startInitFlag;
	extern char* Filename;
	void callBack(void* param);
	extern DVDFileInfo* finfo;
	extern void* header;
	extern bool DvdLoadFlag;
	void __start();
	extern bool playflag;
	extern u32 Mode;
	extern u32 playmode;
	extern void* assign_ch;
	extern void* Head;
	extern void* adpcm_buffer;
	extern u32 adpcm_loadpoint;
	extern u32 adpcm_remain;
	extern u32 playback_samples;
	extern u32 LOOP_BLOCKS;
	extern f32 outvolume;
	extern f32 outpitch;
	extern f32 outpan;
	extern u32 loadup_samples;
	extern u32 movieframe;
	extern bool loop_start_flag;
	extern u32 adpcmbuf_state;
	extern u32 playside;
	extern u32 shift_sample;
	extern u32 LOOP_SAMPLESIZE;
	void LoadADPCM();
	extern u32 loadsize;
	extern u32 extra_sample;
	void __LoadFin(s32 param, DVDFileInfo* info);
	extern bool dspch_deallockflag;
	void __DecodePCM();
	extern void* loop_buffer;
	void __DecodeADPCM();
	extern void* filter_table;
	extern void* table4;
	extern void* store_buffer;
	void* Play_DirectPCM(JASystem::TDSPChannel* channel, s16* buffer, u16 param,
	                     u32 size);
	void init(bool mode);
	extern bool bufferMode;
	u32 getNeedBufferSize();
	void allocBuffer(void* buffer, s32 size);
	extern bool allocFlag;
	extern void* streamHeap;
	void deallocBuffer();
	bool checkBufferStatus();
	void setBufferMode(bool mode);
	void sync(s32 param);
	void Hvqm_SetAudioDmaBuffers(u32 buffers);
	void __Decode();
	void getPrepareFlag();
	void setDecodedBufferBlocks(u32 blocks);
	void LoopInit();
	void* Get_DirectPCM_LoopRemain(JASystem::DSPInterface::DSPBuffer* buffer);
	void* Get_DirectPCM_Counter(JASystem::DSPInterface::DSPBuffer* buffer);
	void* Get_DirectPCM_Remain(JASystem::DSPInterface::DSPBuffer* buffer);
} // namespace StreamLib
} // namespace JAInter

#endif // JAIINTER_HPP
