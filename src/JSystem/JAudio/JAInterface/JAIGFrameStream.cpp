#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIInter.hpp>

void JAIBasic::checkEntriedStream() { }

void JAIBasic::checkWaitStream() { }

void JAIBasic::checkRequestStream() { }

void JAIBasic::checkPlayingStream() { }

namespace JAInter {
namespace StreamLib {

	void* Play_DirectPCM(JASystem::TDSPChannel* channel, s16* buffer, u16 param,
	                     u32 size)
	{
		return nullptr;
	}

	void* Get_DirectPCM_LoopRemain(JASystem::DSPInterface::DSPBuffer* buffer)
	{
		return nullptr;
	}

	void* Get_DirectPCM_Counter(JASystem::DSPInterface::DSPBuffer* buffer)
	{
		return nullptr;
	}

	void* Get_DirectPCM_Remain(JASystem::DSPInterface::DSPBuffer* buffer)
	{
		return nullptr;
	}

	void init(bool mode) { }

	void allocBuffer(void* buffer, s32 size) { }

	void deallocBuffer() { }

	bool checkBufferStatus() { return false; }

	void setBufferMode(bool mode) { }

	u32 getNeedBufferSize() { return 0; }

	void sync(s32 param) { }

	void Hvqm_SetAudioDmaBuffers(u32 buffers) { }

	void __DecodePCM() { }

	void __DecodeADPCM() { }

	void __Decode() { }

	void __LoadFin(s32 param, DVDFileInfo* info) { }

	void LoadADPCM() { }

	void setVolume(f32 volume) { }

	void setPitch(f32 pitch) { }

	void setPan(f32 pan) { }

	void stop() { }

	void setPauseFlag(u8 flag) { }

	void clearPauseFlag(u8 flag) { }

	void setPrepareFlag(u8 flag) { prepareflag = flag; }

	void getPrepareFlag() { }

	void setOutputMode(u32 mode) { outputmode = mode; }

	bool getPlayingFlag() { return playflag2; }

	void setDecodedBufferBlocks(u32 blocks) { }

	void LoopInit() { }

	void start(char* filename, u32 mode, void* param) { }

	void __start() { }

	void callBack(void* param) { }

	bool stopflag           = false;
	bool stopflag2          = false;
	u32 outputmode          = 0;
	u8 outpause             = 0;
	bool playflag2          = false;
	f32 stackvolume         = 0.0f;
	bool outflag_volume     = false;
	f32 stackpitch          = 0.0f;
	bool outflag_pitch      = false;
	f32 stackpan            = 0.0f;
	bool outflag_pan        = false;
	u8 prepareflag          = 0;
	bool startInitFlag      = false;
	char* Filename          = nullptr;
	DVDFileInfo* finfo      = nullptr;
	void* header            = nullptr;
	bool DvdLoadFlag        = false;
	bool playflag           = false;
	u32 Mode                = 0;
	u32 playmode            = 0;
	void* assign_ch         = nullptr;
	void* Head              = nullptr;
	void* adpcm_buffer      = nullptr;
	u32 adpcm_loadpoint     = 0;
	u32 adpcm_remain        = 0;
	u32 playback_samples    = 0;
	u32 LOOP_BLOCKS         = 0;
	f32 outvolume           = 0.0f;
	f32 outpitch            = 0.0f;
	f32 outpan              = 0.0f;
	u32 loadup_samples      = 0;
	u32 movieframe          = 0;
	bool loop_start_flag    = false;
	u32 adpcmbuf_state      = 0;
	u32 playside            = 0;
	u32 shift_sample        = 0;
	u32 LOOP_SAMPLESIZE     = 0;
	u32 loadsize            = 0;
	u32 extra_sample        = 0;
	bool dspch_deallockflag = false;
	void* loop_buffer       = nullptr;
	void* filter_table      = nullptr;
	void* table4            = nullptr;
	void* store_buffer      = nullptr;
	bool bufferMode         = false;
	bool allocFlag          = false;
	void* streamHeap        = nullptr;

} // namespace StreamLib
} // namespace JAInter
