#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIInter.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JASystem/JASHeapCtrl.hpp>
#include <JSystem/JAudio/JASystem/JASCallback.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <macros.h>

namespace JAInter {
namespace StreamLib {

	JASystem::Kernel::TSolidHeap streamHeap;

	// TODO: from TWW, might be wrong
	struct StreamHeader {
		int unk0;
		int unk4;
		u16 unk8;
		u16 unkA;
		u16 unkC;
		u16 unkE;
		u32 unk10;
		u32 unk14;
		int unk18;
		int unk1C;
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
	static s16* adpcm_buffer       = nullptr;
	static s16*** loop_buffer      = nullptr;
	static void** store_buffer     = nullptr;
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

void JAIBasic::checkEntriedStream()
{
	JAISound* it;
	for (it = unk0->unk21C.unk4; it != nullptr; it = it->unk30) {
		bool bVar1 = false;
		if (it->unk1 == 1) {
			if (!unk0->unk184->unk14) {
				JAInter::StreamLib::stop();
				bVar1 = true;
			} else if (unk0->unk184->unk14->unk2 == 0) {
				JAInter::StreamLib::stop();
				bVar1 = true;
			}
			if (bVar1) {
				it->unk1 = 2;

				it->getStreamParameter()->unk3D4 = unk0->unk184;
				unk0->initStreamUpdateParameter();
				unk0->unk184->unk14 = it;
			}
		}
	}
}

void JAIBasic::checkWaitStream()
{
	JAISound* sound = unk0->unk184->unk14;
	if (!sound)
		return;
	if (sound->unk1 != 2)
		return;

	char buffer[64];
	strcpy(buffer, JAIGlobalParameter::streamPath);
	strcat(buffer, unk0->unk1F8[sound->unk8 & 0x3FF].unk10);
	setSeExtParameter(sound);
	sound->unk1 = 3;
	checkPlayingStream();
	JAInter::StreamLib::start(buffer, sound->getStreamParameter()->unk4,
	                          &unk0->unk1F8[sound->unk8 & 0x3FF].unk20);
	JAInter::StreamLib::setPrepareFlag(1);
}

void JAIBasic::checkRequestStream()
{
	JAISound* sound = unk0->unk184->unk14;
	if (!sound)
		return;
	if (sound->unk1 != 3)
		return;
	if (unk0->unk184->unk2 != 0)
		return;
	sound->unk1 = 4;
	if (sound->unk10 > 1) {
		sound->setStreamInterVolume(6, 0.0f, 0);
		sound->setStreamInterVolume(6, 1.0f, sound->unk10);
	}
	JAInter::StreamLib::setPrepareFlag(0);
}

void JAIBasic::checkPlayingStream() { }

namespace JAInter {
namespace StreamLib {

	void Play_DirectPCM(JASystem::TDSPChannel* channel, s16* param_2,
	                    u16 param_3, u32 param_4)
	{
		JASystem::DSPInterface::DSPBuffer* pDVar1;

		pDVar1         = JASystem::DSPInterface::getDSPHandle(channel->unk0);
		pDVar1->unk118 = param_2;
		pDVar1->unk102 = 0;
		pDVar1->unk100 = 0x21;
		pDVar1->unk74  = param_4;
		pDVar1->unk110 = param_2;
		pDVar1->unk114 = param_3 << 0x10;

		JASystem::DSPInterface::getDSPHandle(channel->unk0)
		    ->setMixerInitDelayMax(0);

		for (u8 i = 0; i < 6; ++i) {
			if (i < 2) {
				JASystem::DSPInterface::getDSPHandle(channel->unk0)
				    ->setMixerInitVolume(i, 0x7fff, '\0');
			} else {
				JASystem::DSPInterface::getDSPHandle(channel->unk0)
				    ->setMixerInitVolume(i, 0, '\0');
			}

			JASystem::DSPInterface::getDSPHandle(channel->unk0)
			    ->setBusConnect(i, i + 1);
		}
		JASystem::DSPInterface::getDSPHandle(channel->unk0)->setPitch(0x800);
		JASystem::DSPInterface::getDSPHandle(channel->unk0)->playStart();
		JASystem::DSPInterface::getDSPHandle(channel->unk0)->flushChannel();
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

	void allocBuffer(void* buffer, s32 size)
	{
		if (allocFlag)
			return;

		streamHeap.init((u8*)buffer, size);
		loop_buffer = (s16***)streamHeap.alloc(sizeof(s16**) * 2);
		for (u32 i = 0; i < 2; ++i) {
			loop_buffer[i]
			    = (s16**)streamHeap.alloc(LOOP_BLOCKS * sizeof(s16*));
			for (u32 j = 0; j < LOOP_BLOCKS; ++j) {
				loop_buffer[i][j] = (s16*)streamHeap.alloc(0x2800);
			}
		}
		store_buffer = (void**)streamHeap.alloc(sizeof(void*) * 2);
		for (u32 i = 0; i < 2; ++i) {
			store_buffer[i] = streamHeap.alloc(0x5000);
		}

		adpcm_buffer = (s16*)streamHeap.alloc(0x5000);

		allocFlag = 1;
	}

	void deallocBuffer() { }

	bool checkBufferStatus() { return false; }

	void setBufferMode(bool mode) { }

	u32 getNeedBufferSize()
	{
		u32 size = 0x20;

		for (u32 i = 0; i < 2; ++i) {
			size += ALIGN_PREV(LOOP_BLOCKS, 8) * 4 + 0x20;
			for (u32 j = 0; j < LOOP_BLOCKS; ++j) {
				size += 0x2820;
			}
		}

		return size + 0xF080;
	}

	void sync(s32 param) { }

	void Hvqm_SetAudioDmaBuffers(u32 buffers) { }

	static void __DecodePCM()
	{
		u32 samples = loadsize / 4;
		s16* p1     = loop_buffer[0][playside];
		s16* p2     = loop_buffer[1][playside];
		s16* p3     = adpcm_buffer;
		for (s32 i = 0; i < samples; ++i) {
			*p1 = p3[0];
			*p2 = p3[1];
			++p1;
			++p2;
			p3 += 2;
		}

		loadup_samples += loadsize / 4;
		DCStoreRange(loop_buffer[0][playside], loadsize / 2);
		DCStoreRange(loop_buffer[1][playside], loadsize / 2);
	}

	static void __DecodeADPCM() { }

	static void __Decode() { }

	static void __LoadFin(s32 param, DVDFileInfo* info)
	{
		DvdLoadFlag = 0;
		if (adpcmbuf_state == 3)
			return;
		adpcmbuf_state = 2;
	}

	static void LoadADPCM()
	{
		if (adpcmbuf_state != 0)
			return;

		switch (header.unkA) {
		case 2:
			loadsize = 0x5000;
			break;
		case 4:
			loadsize = 0x1680;
			break;
		}

		extra_sample = 0;

		if (adpcm_remain < loadsize) {
			if ((adpcm_remain & 0x1F) != 0) {
				loadsize     = adpcm_remain + (0x20 - (adpcm_remain & 0x1F));
				extra_sample = loadsize - adpcm_remain;
			} else {
				loadsize = adpcm_remain;
			}
		}

		adpcmbuf_state = 1;
		DVDReadAsyncPrio(&finfo, adpcm_buffer, loadsize, adpcm_loadpoint,
		                 &__LoadFin, 2);
		DvdLoadFlag = 1;
		adpcm_loadpoint += loadsize;
		if (adpcm_remain < loadsize) {
			adpcm_remain = 0;
		} else {
			adpcm_remain = adpcm_remain - loadsize;
		}
	}

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
