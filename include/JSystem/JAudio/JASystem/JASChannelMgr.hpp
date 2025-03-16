#ifndef JASCHANNELMGR_HPP
#define JASCHANNELMGR_HPP

#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <dolphin/types.h>

namespace JASystem {

class TChannelMgr {
public:
	void init();
	void stopAll();
	void stopAllRelease();
	void checkGlobalRelease();
	TChannel* getLogicalChannel(u32);
	void initAllocChannel(u32);
	TChannel* getListHead(u32);
	u32 countList(u32);
	int cutList(TChannel*);
	void addListHead(TChannel*, u32);
	void addListTail(TChannel*, u32);
	int receiveAllChannels(TChannelMgr*);
	BOOL checkLimitStart(u32);
	void checkLimitStop(TChannel*, u32);

public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ TChannel* unk8;
	/* 0xC */ TChannel* unkC;
	/* 0x10 */ TChannel* unk10;
	/* 0x14 */ TChannel* unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ s16 unk2C[8];
	/* 0x3C */ s16 unk3C[4];
	char unk44[0x8];
	/* 0x4C */ s16 unk4C;
	/* 0x4E */ u16 unk4E[6];
	/* 0x5A */ u8 unk5A[6];
	/* 0x60 */ u8 unk60;
	/* 0x61 */ u8 unk61;
	/* 0x62 */ u8 unk62[3];
	/* 0x68 */ u32 unk68;
	/* 0x6C */ u16 unk6C;
	/* 0x70 */ s32 unk70;
};

} // namespace JASystem

#endif // JASCHANNELMGR_HPP
