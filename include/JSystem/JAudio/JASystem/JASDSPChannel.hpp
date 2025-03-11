#ifndef JASDSPCHANNEL_HPP
#define JASDSPCHANNEL_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace DSPInterface {
	class DSPBuffer;
}

class TChannel;

class TDSPChannel {
private:
public:
	TDSPChannel();
	~TDSPChannel();

	static void initAll();
	static void updateAll();
	static TDSPChannel* alloc(u32 param1, u32 param2);
	static void free(TDSPChannel* channel, u32 param);
	TDSPChannel* getHandle(u32 handle);
	u32 getNumUse();
	u32 getNumFree();
	void setLimitDSP(f32 limit);
	f32* getHistory();

	void init(u8 param);
	void allocate(u32 param);
	void free();
	void play();
	void stop();
	void pause();
	void restart();
	void forceStop();
	void forceDelete();
	u8 getLower();
	u8 getLowerActive();
	static int breakLower(u8 param);
	void breakLowerActive(u8 param);

	static TDSPChannel* DSPCH;
	static u32 smnUse;
	static u32 smnFree;
	static f32 DSP_LIMIT_RATIO;
	static f32* history;

public:
	/* 0x0 */ u8 unk0;
	/* 0x0 */ u8 unk1;
	/* 0x0 */ u8 unk2;
	/* 0x0 */ u8 unk3;
	/* 0x4 */ u16 unk4;
	/* 0x6 */ u16 unk6;
	/* 0x8 */ char unk8[0x4];
	/* 0xC */ DSPInterface::DSPBuffer* unkC;
	/* 0x10 */ void (*unk10)(TDSPChannel*, u32);
};

} // namespace JASystem

#endif // JASDSPCHANNEL_HPP
