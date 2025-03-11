#ifndef JASDSPCHANNEL_HPP
#define JASDSPCHANNEL_HPP

#include <dolphin/types.h>
#include <types.h>

namespace JASystem {

namespace DSPInterface {
	class DSPBuffer;
}

class TChannel;

class TDSPChannel {
private:
public:
	TDSPChannel()
	    : unkC(nullptr)
	    , unk10(nullptr)
	{
	}
	~TDSPChannel() { }

	static void initAll();
	static void updateAll();
	static TDSPChannel* alloc(u32 param1, u32 param2);
	static int free(TDSPChannel* channel, u32 param);
	TDSPChannel* getHandle(u32 handle);
	u32 getNumUse();
	u32 getNumFree();
	void setLimitDSP(f32 limit);
	f32* getHistory();

	void init(u8 param);
	BOOL allocate(u32 param);
	void free();
	void play();
	void stop();
	void pause();
	void restart();
	bool forceStop();
	void forceDelete();
	static JASystem::TDSPChannel* getLower();
	static JASystem::TDSPChannel* getLowerActive();
	static BOOL breakLower(u8 param);
	static BOOL breakLowerActive(u8 param);

	BOOL isUnk1One() const { return unk1 == 1 ? TRUE : FALSE; }

	static TDSPChannel* DSPCH;
	static u32 smnUse;
	static u32 smnFree;

public:
	/* 0x0 */ u8 unk0;
	/* 0x0 */ u8 unk1;
	/* 0x0 */ u8 unk2;
	/* 0x0 */ u8 unk3;
	/* 0x4 */ u16 unk4;
	/* 0x6 */ u16 unk6;
	/* 0x8 */ u32 unk8;
	/* 0xC */ DSPInterface::DSPBuffer* unkC;
	/* 0x10 */ int (*unk10)(TDSPChannel*, u32);
};

} // namespace JASystem

#endif // JASDSPCHANNEL_HPP
