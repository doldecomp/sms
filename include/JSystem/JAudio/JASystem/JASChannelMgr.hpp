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
	void receiveAllChannels(TChannelMgr*);
	bool checkLimitStart(u32);
	bool checkLimitStop(TChannel*, u32);

public:
	u32 unk0;
	u32 unk4;
	char unk8[0x24];
	s16 unk2C[8];
	s16 unk3C[4];
	char unk44[0x8];
	s16 unk4C;
	char unk4E[2];
	char unk50[0xA];
	u8 unk5A[6];
	u8 unk60;
	u8 unk61;
};

namespace Driver {
	extern u8 polys_table[];
} // namespace Driver

} // namespace JASystem

#endif // JASCHANNELMGR_HPP
