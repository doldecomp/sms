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
		/* 0x10 */ short unk10[8];
	};

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

		struct Channel {
			/* 0x0 */ u16 id;
			/* 0x4 */ u16 targetVolume;
			/* 0x8 */ u16 currentVolume;
			/* 0xC */ u16 unkC;
		};

		// TODO: verify whether this struct is this & label:
		// https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/HW/DSPHLE/UCodes/Zelda.cpp#L683
		// As far as I understand, a bunch of games have the same DSP microcode
		// as Zelda (including sms), and that DSP microcode reads this struct
		// directly on the DSP side, so it's layout is the same in all such
		// games, presumably.
	public:
		/* 0x00 */ u16 unk0;
		/* 0x02 */ u16 unk2;
		/* 0x04 */ u16 unk4;
		/* 0x06 */ u8 unk6[0x8 - 0x6];
		/* 0x08 */ u16 unk8;
		/* 0x0A */ u8 unkA[0xc - 0xa];
		/* 0x0C */ u16 unkC;
		/* 0x0E */ u16 unkE;
		/* 0x10 */ Channel unk10[6];
		/* 0x18 */ u8 unk18[0x50 - 0x40];
		/* 0x50 */ u16 unk50;
		/* 0x52 */ u16 unk52;
		/* 0x54 */ u16 unk54;
		/* 0x56 */ u16 unk56;
		/* 0x58 */ u16 unk58;
		/* 0x5A */ u8 unk5A[0x60 - 0x5a];
		/* 0x60 */ short unk60;
		/* 0x62 */ u8 unk62[0x64 - 0x62];
		/* 0x64 */ short unk64;
		/* 0x66 */ short unk66;
		/* 0x68 */ int unk68;
		/* 0x6C */ u32 unk6C;
		/* 0x70 */ u8 unk70[0x74 - 0x70];
		/* 0x74 */ u32 unk74;
		/* 0x78 */ short unk78[4];
		/* 0x80 */ u16 unk80[20];
		/* 0xA8 */ short unkA8[4];
		/* 0xB8 */ u16 unkB0[16];
		/* 0xD0 */ u8 unkD0[0x100 - 0xd0];
		/* 0x100 */ u16 unk100;
		/* 0x102 */ u16 unk102;
		/* 0x104 */ short unk104;
		/* 0x106 */ short unk106;
		/* 0x108 */ short unk108;
		/* 0x10A */ u16 unk10A;
		/* 0x10C */ u32 unk10C;
		/* 0x110 */ u32 unk110;
		/* 0x114 */ u32 unk114;
		/* 0x118 */ u32 unk118;
		/* 0x11C */ int unk11C;
		/* 0x120 */ short unk120[8];
		/* 0x130 */ u8 unk130[0x148 - 0x130];
		/* 0x148 */ short unk148[4];
		/* 0x150 */ short unk150;
		/* 0x152 */ u8 unk152[0x180 - 0x152];
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
} // namespace DSPInterface

} // namespace JASystem

#endif // JASDSPINTERFACE_HPP
