#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIInter.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JASystem/JASHeapCtrl.hpp>
#include <JSystem/JAudio/JASystem/JASCallback.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JASystem/JASDSPChannel.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <dolphin/dvd.h>
#include <macros.h>

namespace JAInter {
namespace StreamLib {

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

	JASystem::Kernel::TSolidHeap streamHeap;

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

	static u32 adpcm_remain    = 0;
	static u32 adpcm_loadpoint = 0;
	static u32 loadsize        = 0;
	static s16* adpcm_buffer   = nullptr;
	static s16*** loop_buffer  = nullptr;
	static void** store_buffer = nullptr;
	static JASystem::TDSPChannel* assign_ch[2];
	static u32 playside            = 0;
	static u32 playback_samples    = 0;
	static u32 loadup_samples      = 0;
	static u32 adpcmbuf_state      = 0;
	static u32 movieframe          = 0;
	static bool stopflag           = false;
	static bool stopflag2          = false;
	static u8 playflag             = false;
	static u8 playflag2            = 0;
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
	static u32 loop_start_flag     = false;
	static u32 outpause            = 0;
	static u32 playmode            = 0;
	static u32 shift_sample        = 0;
	static u32 extra_sample        = 0;
	static BOOL DvdLoadFlag        = false;
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

void JAIBasic::checkPlayingStream()
{
	JAIStreamUpdateParameter* sud = unk0->unk184;
	JAISound* sound               = sud->unk14;

	if (sound == nullptr)
		return;

	u32& r29 = sud->unk10;

	if (sound->unk1 >= 4) {
		sound->getStreamParameter(); // huh?
		if (JAInter::StreamLib::getPlayingFlag() == 2) {
			sound->unk1 = 0;
			if (sound->getStreamParameter()->unk3D4 != nullptr)
				sound->getStreamParameter()->unk3D4->unk14 = nullptr;

			releaseStreamParameterPointer(sound->getStreamParameter());
			sound->clearMainSoundPPointer();
			releaseControllerHandle(&unk0->unk21C, sound);
			return;
		}

		if (sound->unk2 != 0)
			sound->unk2--;

		if (r29 & 2) {
			sound->setStreamInterVolume(6, 0.0f, sound->unk10);
			sound->unk1 = 5;
			r29 ^= 2;
		}

		if (sound->unk1 == 5
		    && (sound->getStreamInterVolume(6) == 0.0f || sound->unk10 == 0)
		    && sound->unk2 == 0) {
			JAInter::StreamLib::stop();
			sound->unk1 = 0;
			if (sound->getStreamParameter()->unk3D4 != nullptr)
				sound->getStreamParameter()->unk3D4->unk14 = nullptr;

			releaseStreamParameterPointer(sound->getStreamParameter());
			sound->clearMainSoundPPointer();
			releaseControllerHandle(&unk0->unk21C, sound);
		}
	}

	if (sound->unk1 < 3)
		return;

	JAIStreamParameter* streamParam = sound->getStreamParameter();

	if (r29 & 0x40000) {
		f32 vol = 1.0f;
		for (u8 j = 0; j < 13; ++j) {
			JAIMoveParaSet* mps = &streamParam->unk14[j];
			if (streamParam->unk8 & (1 << j))
				if (!unk0->moveParameter(mps))
					streamParam->unk8 ^= 1 << j;

			vol *= mps->unk4;
		}
		if (unk0->unk184->unk4 != vol) {
			JAInter::StreamLib::setVolume(vol);
			unk0->unk184->unk4 = vol;
		}
		if (streamParam->unk8 == 0)
			r29 ^= 0x40000;
	}

	if (r29 & 0x100000) {
		f32 pitch = 1.0f;
		for (u8 j = 0; j < 13; ++j) {
			JAIMoveParaSet* mps = &streamParam->unk154[j];
			if (streamParam->unkC & (1 << j))
				if (!unk0->moveParameter(mps))
					streamParam->unkC ^= 1 << j;

			pitch *= mps->unk4;
		}
		if (unk0->unk184->unk8 != pitch) {
			JAInter::StreamLib::setPitch(pitch);
			unk0->unk184->unk8 = pitch;
		}
		if (streamParam->unkC == 0)
			r29 ^= 0x100000;
	}

	if (r29 & 0x80000) {
		f32 pan = 0.0f;
		for (u8 j = 0; j < 13; ++j) {
			JAIMoveParaSet* mps = &streamParam->unk294[j];
			if (streamParam->unk10 & (1 << j))
				if (!unk0->moveParameter(mps))
					streamParam->unk10 ^= 1 << j;

			pan += mps->unk4 - 0.5f;
		}
		pan += 0.5f;
		if (pan > 1.0f)
			pan = 1.0f;
		else if (pan < 0.0f)
			pan = 0.0f;

		if (unk0->unk184->unkC != pan) {
			JAInter::StreamLib::setPan(pan);
			unk0->unk184->unkC = pan;
		}
		if (streamParam->unk10 == 0)
			r29 ^= 0x80000;
	}

	sound->unk14++;
}

namespace JAInter {
namespace StreamLib {

	void Play_DirectPCM(JASystem::TDSPChannel* channel, s16* param_2,
	                    u16 param_3, u32 param_4)
	{
		JASystem::DSPInterface::DSPBuffer* pDVar1
		    = JASystem::DSPInterface::getDSPHandle(channel->unk0);
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
		s16* p1;
		s16* p2;
		s16* src;

		u32 lsz     = loadsize;
		u32 samples = loadsize / 4;

		p1  = loop_buffer[0][playside];
		p2  = loop_buffer[1][playside];
		src = adpcm_buffer;
		for (s32 i = 0; i < samples; ++i) {
			*p1 = src[0];
			*p2 = src[1];
			++p1;
			++p2;
			src += 2;
		}

		loadup_samples += lsz / 4;
		DCStoreRange(loop_buffer[0][playside], lsz / 2);

		DCStoreRange(loop_buffer[1][playside], loadsize / 2);
	}

	static void __DecodeADPCM()
	{
		static s16 L1;
		static s16 L2;
		static s16 R1;
		static s16 R2;

		u32 lsz;
		u8 hdr;
		u32 blocks;
		u32 skipBytes = 0;
		s16* leftOut;
		s16* rightOut;
		u8* src;

		if (movieframe == 0 && playside == 0) {
			R2 = 0;
			R1 = 0;
			L2 = 0;
			L1 = 0;
		}

		leftOut  = (s16*)store_buffer[0];
		rightOut = (s16*)store_buffer[1];
		src      = (u8*)adpcm_buffer;

		if (loop_start_flag) {
			skipBytes       = ((header.unk14 >> 4) & 7) * 18;
			loop_start_flag = false;
			loadsize        = 0x1680 - skipBytes;
			src             = (u8*)adpcm_buffer + skipBytes;
		}

		lsz    = loadsize;
		blocks = lsz / 18;
		for (u32 block = 0; block < blocks; ++block) {
			hdr        = *src++;
			s16 cL1    = filter_table[(hdr & 0xF) * 2];
			s16 cL2    = filter_table[(hdr & 0xF) * 2 + 1];
			u32 shiftL = (hdr >> 4) & 0xF;
			for (int i = 0; i < 4; ++i) {
				u8 b1Hi = src[0] >> 4;
				u8 b1Lo = src[0] & 0xF;
				s32 l1;
				s32 l2;

				l2 = (table4[b1Hi] << shiftL) + ((cL1 * L1 + cL2 * L2) >> 11);
				*leftOut++ = l2;
				L2         = l2;
				l1 = (table4[b1Lo] << shiftL) + ((cL1 * L2 + cL2 * L1) >> 11);
				*leftOut++ = l1;
				L1         = l1;

				u8 b2Hi = src[1] >> 4;
				u8 b2Lo = src[1] & 0xf;

				l2 = (table4[b2Hi] << shiftL) + ((cL1 * L1 + cL2 * L2) >> 11);
				*leftOut++ = l2;
				L2         = l2;
				l1 = (table4[b2Lo] << shiftL) + ((cL1 * L2 + cL2 * L1) >> 11);
				*leftOut++ = l1;
				L1         = l1;

				src += 2;
			}

			u8 hdrR    = *src++;
			s16 cR1    = filter_table[(hdrR & 0xF) * 2];
			s16 cR2    = filter_table[(hdrR & 0xF) * 2 + 1];
			u32 shiftR = (hdrR >> 4) & 0xF;
			for (int i = 0; i < 4; ++i) {

				u8 b1Hi = src[0] >> 4;
				u8 b1Lo = src[0] & 0xF;

				s32 l1;
				s32 l2;

				l2 = (table4[b1Hi] << shiftR) + ((cR1 * R2 + cR2 * R1) >> 11);
				*rightOut++ = l2;
				R2          = l2;
				l1 = (table4[b1Lo] << shiftR) + ((cR1 * R2 + cR2 * R1) >> 11);
				*rightOut++ = l1;
				R1          = l1;

				u8 b2Hi = src[1] >> 4;
				u8 b2Lo = src[1] & 0xF;
				l2 = (table4[b2Hi] << shiftR) + ((cR1 * R1 + cR2 * R2) >> 11);
				*rightOut++ = l2;
				R2          = l2;
				l1 = (table4[b2Lo] << shiftR) + ((cR1 * R2 + cR2 * R1) >> 11);
				*rightOut++ = l1;
				R1          = l1;
				src += 2;
			}
		}

		loadup_samples += (((lsz - skipBytes) / 18) & 0x7FFFFFF) << 4;

		u32 pos;
		u32 sampleIdx;
		for (sampleIdx = 0; sampleIdx < (loadsize / 18) * 16; ++sampleIdx) {
			pos = sampleIdx + shift_sample;
			if (pos == 0x1400) {
				DCStoreRange(loop_buffer[0][playside] + shift_sample,
				             (0x1400 - shift_sample) * 2);
				DCStoreRange(loop_buffer[1][playside] + shift_sample,
				             (0x1400 - shift_sample) * 2);
				playside = (playside + 1) % LOOP_BLOCKS;
			}
			if (pos >= 0x1400)
				pos -= 0x1400;

			loop_buffer[0][playside][pos] = ((s16*)store_buffer[0])[sampleIdx];
			loop_buffer[1][playside][pos] = ((s16*)store_buffer[1])[sampleIdx];
		}

		u32 endPos = sampleIdx + shift_sample;
		DCStoreRange(loop_buffer[0][playside], 0x2800);
		DCStoreRange(loop_buffer[1][playside], 0x2800);
		if (endPos == 0x1400) {
			playside = (playside + 1) % LOOP_BLOCKS;
		}
		if (endPos >= 0x1400) {
			shift_sample = endPos - 0x1400;
		} else {
			shift_sample = endPos;
			if (endPos > 0x1400) {
				shift_sample -= 0x1400;
			}
		}
	}

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

	u8 getPlayingFlag() { return playflag2; }

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

	void __start()
	{
		startInitFlag = 0;
		playmode      = Mode;

		if (playflag != 0) {
			assign_ch[0]->forceStop();
			assign_ch[1]->forceStop();
			playflag++;
		}

		DVDOpen(Filename, &finfo);

		if (Head == nullptr) {
			DVDReadPrio(&finfo, adpcm_buffer, 0x20, 0, 2);
		} else {
			for (int i = 0; i < 2; ++i) {
				for (int j = 0; j < 16; ++j) {
					((u8*)adpcm_buffer)[i * 16 + j] = ((u8*)Head)[i * 16 + j];
				}
			}
		}

		adpcm_loadpoint = 0x20;
		header          = *(StreamHeader*)adpcm_buffer;

		adpcm_remain     = header.unk0;
		playback_samples = header.unk4;
		if (playmode != 0)
			header.unk10 = 0;

		stopflag        = false;
		stopflag2       = false;
		playflag2       = 0;
		prepareflag     = 0;
		outvolume       = 1.0f;
		outpitch        = 1.0f;
		outpan          = 0.5f;
		loadup_samples  = 0;
		movieframe      = 0;
		loop_start_flag = false;
		adpcmbuf_state  = 0;
		playside        = 0;
		shift_sample    = 0;
		LOOP_SAMPLESIZE = LOOP_BLOCKS * 0x1400;

		JASystem::Dvd::pauseDvdT();
		LoadADPCM();

		for (u32 i = 0; i < ARRAY_COUNT(assign_ch); ++i) {
			if (assign_ch[i] != nullptr && assign_ch[i]->unk8 != 0)
				JASystem::TDSPChannel::free(assign_ch[i], (u32)&assign_ch[i]);

			assign_ch[i] = nullptr;
		}
		dspch_deallockflag = true;
	}

	// fabricated
	void hackyHackMcHackson(u32 param_1)
	{
		// NOTE: probably was used for debug or something? idk
		static u32 before = 0;
		before            = param_1;
	}

	s32 callBack(void* param)
	{
		bool decoded = false;

		if (startInitFlag != 0) {
			if (DvdLoadFlag != 0)
				return 0;

			__start();
		}

		if (startInitFlag == 0) {
			if (outflag_volume) {
				outflag_volume = false;
				outvolume      = stackvolume;
			}
			if (outflag_pitch) {
				outflag_pitch = false;
				outpitch      = stackpitch;
			}
			if (outflag_pan) {
				outflag_pan = false;
				outpan      = stackpan;
			}
		}

		if (assign_ch[0] == nullptr) {
			assign_ch[0] = JASystem::TDSPChannel::alloc(0, (u32)&assign_ch[0]);
			assign_ch[1] = JASystem::TDSPChannel::alloc(0, (u32)&assign_ch[1]);
			if (assign_ch[0] != nullptr && assign_ch[1] != nullptr) {
				assign_ch[0]->unk3 = 0x7F;
				assign_ch[1]->unk3 = 0x7F;
			}
		}

		if (assign_ch[0] == nullptr || assign_ch[1] == nullptr) {
			hackyHackMcHackson(-1);
			playflag  = 0;
			playflag2 = 2;
			JASystem::Dvd::unpauseDvdT();
			return -1;
		}

		static s32 oldstat = 0;

		s32 stat = DVDGetDriveStatus();
		switch (stat) {
		case 5:
			JASystem::DSPInterface::getDSPHandle(assign_ch[0]->unk0)
			    ->setPauseFlag(1);
			JASystem::DSPInterface::getDSPHandle(assign_ch[1]->unk0)
			    ->setPauseFlag(1);
			outpause = 1;
			JASystem::DSPInterface::getDSPHandle(assign_ch[0]->unk0)
			    ->flushChannel();
			JASystem::DSPInterface::getDSPHandle(assign_ch[1]->unk0)
			    ->flushChannel();
			break;
		case 0:
			if (oldstat != DVDGetDriveStatus()) {
				JASystem::DSPInterface::getDSPHandle(assign_ch[0]->unk0)
				    ->setPauseFlag(0);
				JASystem::DSPInterface::getDSPHandle(assign_ch[1]->unk0)
				    ->setPauseFlag(0);
				JASystem::DSPInterface::getDSPHandle(assign_ch[0]->unk0)
				    ->flushChannel();
				JASystem::DSPInterface::getDSPHandle(assign_ch[1]->unk0)
				    ->flushChannel();
				outpause = 0;
			}
			break;
		}

		oldstat = (u32)DVDGetDriveStatus();

		if (outpause != 0)
			return 0;

		BOOL needDecode = false;
		if (movieframe != 0) {
			JASystem::DSPInterface::DSPBuffer* buf
			    = JASystem::DSPInterface::getDSPHandle(assign_ch[0]->unk0);
			if (buf->unk2 != 0) {
				if (adpcmbuf_state != 2) {
					JASystem::TDSPChannel::free(assign_ch[0],
					                            (u32)&assign_ch[0]);
					JASystem::TDSPChannel::free(assign_ch[1],
					                            (u32)&assign_ch[1]);
					hackyHackMcHackson(-1);
					playflag  = 0;
					playflag2 = 2;
					JASystem::Dvd::unpauseDvdT();
					return -1;
				}
				return 0;
			}

			hackyHackMcHackson(((playback_samples - buf->unk74) * header.unkE)
			                   / header.unk8);
			movieframe++;
			u32 cur = (LOOP_SAMPLESIZE - (buf->unk6C >> 16)) / 0x1400;
			static u32 old_dspside = 0;
			if (old_dspside != cur)
				old_dspside = cur;

			if (cur != (playside + 1) % LOOP_BLOCKS)
				needDecode = true;
			else
				needDecode = false;
		}

		if ((needDecode == true || movieframe == 0)
		    && (adpcmbuf_state == 2 || adpcmbuf_state == 4)) {
			if (adpcmbuf_state == 2) {
				switch (header.unkA) {
				case 3:
					break;
				case 2:
					__DecodePCM();
					break;
				case 4:
					__DecodeADPCM();
					break;
				}
				adpcmbuf_state = 0;
			}
			if (movieframe == 0) {
				if (playside == LOOP_BLOCKS - 2) {
					if (prepareflag != 0) {
						prepareflag    = 2;
						adpcmbuf_state = 4;
						return 0;
					}
					movieframe++;
					prepareflag = 2;
					playflag2   = 1;
					for (u32 i = 0; i < 2; ++i) {
						JASystem::DSPInterface::DSPBuffer* buf2
						    = JASystem::DSPInterface::getDSPHandle(
						        assign_ch[i]->unk0);
						u16 pitch = (header.unk8 << 12) / 32000;
						Play_DirectPCM(assign_ch[i], loop_buffer[i][2],
						               (u16)LOOP_SAMPLESIZE, playback_samples);
						s16 v0, v1;
						if (outputmode == 1) {
							v0 = 0x7FFF;
							v1 = 0;
						} else {
							v0 = 0x5A7E;
							v1 = 0x5A7E;
						}
						JASystem::DSPInterface::getDSPHandle(assign_ch[i]->unk0)
						    ->setMixerVolume(i, v0, 0);
						JASystem::DSPInterface::getDSPHandle(assign_ch[i]->unk0)
						    ->setMixerVolume(1 - i, v1, 0);
						JASystem::DSPInterface::getDSPHandle(assign_ch[i]->unk0)
						    ->setPitch(pitch);

						decoded = true;

						if (header.unk10 != 0)
							buf2->unk74 = -1;

						JASystem::DSPInterface::getDSPHandle(assign_ch[i]->unk0)
						    ->flushChannel();
					}
					if (adpcmbuf_state != 3)
						adpcmbuf_state = 0;
				}
			}
		}

		if (stopflag && stopflag2) {
			stopflag2 = false;
			assign_ch[0]->forceStop();
			assign_ch[1]->forceStop();
			adpcmbuf_state = 3;
		}

		if (!decoded) {
			f32 fL = 1.0f;
			f32 fR = 1.0f;
			u16 base;
			if (outputmode == 1) {
				if (outpan < 0.5f)
					fR = 1.4142f * JASystem::Calc::sinfT(outpan);
				else
					fL = 1.4142f * JASystem::Calc::sinfT(1.0f - outpan);

				base = 0x7FFF;

				JASystem::DSPInterface::getDSPHandle(assign_ch[0]->unk0)
				    ->setMixerVolume(1, 0, 0);

				JASystem::DSPInterface::getDSPHandle(assign_ch[1]->unk0)
				    ->setMixerVolume(0, 0, 0);
			} else {
				base   = 0x5A7E;
				s16 v1 = (s16)(23166.0f * outvolume);
				JASystem::DSPInterface::getDSPHandle(assign_ch[0]->unk0)
				    ->setMixerVolume(1, v1, 0);

				s16 v2 = (s16)(23166.0f * outvolume);
				JASystem::DSPInterface::getDSPHandle(assign_ch[1]->unk0)
				    ->setMixerVolume(0, v2, 0);
			}

			s16 vL = outvolume * (f32)base * fL;
			JASystem::DSPInterface::getDSPHandle(assign_ch[0]->unk0)
			    ->setMixerVolume(0, vL, 0);

			s16 vR = outvolume * (f32)base * fR;
			JASystem::DSPInterface::getDSPHandle(assign_ch[1]->unk0)
			    ->setMixerVolume(1, vR, 0);

			u16 pitch1 = outpitch * (f32)((header.unk8 << 12) / 32000);
			JASystem::DSPInterface::getDSPHandle(assign_ch[0]->unk0)
			    ->setPitch(pitch1);

			u16 pitch2 = outpitch * (f32)((header.unk8 << 12) / 32000);
			JASystem::DSPInterface::getDSPHandle(assign_ch[1]->unk0)
			    ->setPitch(pitch2);

			JASystem::DSPInterface::getDSPHandle(assign_ch[0]->unk0)
			    ->flushChannel();

			JASystem::DSPInterface::getDSPHandle(assign_ch[1]->unk0)
			    ->flushChannel();
		}

		if (adpcmbuf_state == 0) {
			if (adpcm_remain == 0) {
				if (header.unk10 != 0) {
					loop_start_flag = true;
					adpcm_loadpoint
					    = ((header.unk14 - (header.unk14 & 0x7F)) >> 4) * 0x12
					      + 0x20;
					adpcm_remain = header.unk0 - (adpcm_loadpoint - 0x20);
				} else {
					adpcmbuf_state = 3;
				}
			} else if (stopflag) {
				adpcmbuf_state = 3;
			} else {
				LoadADPCM();
			}
		}

		return 0;
	}

} // namespace StreamLib
} // namespace JAInter
