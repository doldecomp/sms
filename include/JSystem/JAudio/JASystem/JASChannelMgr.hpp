#ifndef JASCHANNELMGR_HPP
#define JASCHANNELMGR_HPP

#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <dolphin/types.h>

namespace JASystem {

class TChannelMgr {
public:
	static void init();
	static void stopAll();
	static void stopAllRelease();
	static void checkGlobalRelease();
	static TChannel* getLogicalChannel(u32);
	static void initAllocChannel(u32);
	static TChannel* getListHead(u32);
	static u32 countList(u32);
	static void cutList(TChannel*);
	static void addListHead(TChannel*, u32);
	static void addListTail(TChannel*, u32);
	static void receiveAllChannels(TChannelMgr*);
	static bool checkLimitStart(u32);
	static bool checkLimitStop(TChannel*, u32);
};

namespace Driver {
	extern u8 polys_table[];
} // namespace Driver

} // namespace JASystem

#endif // JASCHANNELMGR_HPP
