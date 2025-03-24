#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIInter.hpp>
#include <JSystem/JAudio/JASystem/JASHeapCtrl.hpp>
#include <JSystem/JAudio/JASystem/JASCallback.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>

namespace JAInter {
namespace StreamLib {

	JASystem::Kernel::TSolidHeap streamHeap;

	// TODO: from TWW, might be wrong
	struct StreamHeader {
		int field_0x0;
		int field_0x4;
		u16 field_0x8;
		u16 field_0xa;
		u16 field_0xc;
		u16 field_0xe;
		u32 field_0x10;
		u32 field_0x14;
		int field_0x18;
		int field_0x1c;
	};

	static DVDFileInfo finfo;
	static StreamHeader header;
	static char Filename[100];

	static s16 filter_table[32] = {
		0x0000, 0x0000, 0x0800, 0x0000, 0x0000, 0x0800, 0x0400, 0x0400,
		0x1000, -0x800, 0x0E00, -0x600, 0x0C00, -0x400, 0x1200, -0xA00,
		0x1068, -0x8C8, 0x12C0, -0x8FC, 0x1400, -0xC00, 0x0800, -0x800,
		0x0400, -0x400, -0x400, 0x0400, -0x400, 0x0000, -0x800, 0x0000,
	};

	s16 table4[] = {
		0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
		-0x008, -0x007, -0x006, -0x005, -0x004, -0x003, -0x002, -0x001,
	};

	static u32 adpcm_remain        = 0;
	static u32 adpcm_loadpoint     = 0;
	static u32 loadsize            = 0;
	static void* adpcm_buffer      = nullptr;
	static void* loop_buffer       = nullptr;
	static void* store_buffer      = nullptr;
	static void* assign_ch         = nullptr;
	static u32 playside            = 0;
	static u32 playback_samples    = 0;
	static u32 loadup_samples      = 0;
	static u32 adpcmbuf_state      = 0;
	static u32 movieframe          = 0;
	static bool stopflag           = false;
	static bool stopflag2          = false;
	static bool playflag           = false;
	static bool playflag2          = false;
	static u8 prepareflag          = 0;
	static bool dspch_deallockflag = false;
	static f32 outvolume           = 0.0f;
	static f32 outpitch            = 0.0f;
	static f32 outpan              = 0.0f;
	static f32 stackvolume         = 0.0f;
	static f32 stackpitch          = 0.0f;
	static f32 stackpan            = 0.0f;
	static bool outflag_volume     = false;
	static bool outflag_pan        = false;
	static bool outflag_pitch      = false;
	static bool loop_start_flag    = false;
	static u32 outpause            = 0;
	static u32 playmode            = 0;
	static u32 shift_sample        = 0;
	static u32 extra_sample        = 0;
	static u32 DvdLoadFlag         = false;
	static u32 startInitFlag       = false;
	static u32 Mode                = 0;
	static void* Head              = nullptr;

	bool bufferMode = false;
	bool allocFlag  = false;

	static u32 LOOP_BLOCKS     = 0xC;
	static u32 LOOP_SAMPLESIZE = 0xf000;
	static u32 outputmode      = 1;

} // namespace StreamLib
} // namespace JAInter

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

	void init(bool mode)
	{
		bufferMode = mode;
		if (!mode) {
			u32 sz = getNeedBufferSize();
			allocBuffer(JASDram->alloc(sz, 0), sz);
		}
	}

	void allocBuffer(void* buffer, s32 size) { }

	void deallocBuffer() { }

	bool checkBufferStatus() { return false; }

	void setBufferMode(bool mode) { }

	u32 getNeedBufferSize() { return 0; }

	void sync(s32 param) { }

	void Hvqm_SetAudioDmaBuffers(u32 buffers) { }

	static void __DecodePCM() { }

	static void __DecodeADPCM() { }

	static void __Decode() { }

	static void __LoadFin(s32 param, DVDFileInfo* info)
	{
		DvdLoadFlag = 0;
		if (adpcmbuf_state == 3)
			return;
		adpcmbuf_state = 2;
	}

	static void LoadADPCM() { }

	void setVolume(f32 volume)
	{
		stackvolume    = volume;
		outflag_volume = true;
	}

	void setPitch(f32 pitch)
	{
		stackpitch    = pitch;
		outflag_pitch = true;
	}

	void setPan(f32 pan)
	{
		stackpan    = pan;
		outflag_pan = true;
	}

	void stop()
	{
		stopflag  = true;
		stopflag2 = true;
	}

	void setPauseFlag(u8 flag) { outpause |= flag; }

	void clearPauseFlag(u8 flag) { outpause &= (flag ^ 0xff); }

	void setPrepareFlag(u8 flag) { prepareflag = flag; }

	void getPrepareFlag() { }

	void setOutputMode(u32 mode) { outputmode = mode; }

	bool getPlayingFlag() { return playflag2; }

	void setDecodedBufferBlocks(u32 blocks) { }

	void LoopInit() { }

	static s32 callBack(void* param);

	void start(char* filename, u32 mode, void* param)
	{
		if (startInitFlag == 0) {
			strcpy(Filename, filename);
			Mode = mode;
			Head = param;
			++startInitFlag;
			JASystem::Kernel::registerSubframeCallback(&callBack, nullptr);
		}
	}

	void __start() { }

	s32 callBack(void* param) { }

} // namespace StreamLib
} // namespace JAInter
