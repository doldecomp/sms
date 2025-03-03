#ifndef JASCHANNELMGR_HPP
#define JASCHANNELMGR_HPP

#include <JSystem/JAudio/JASystem/JASChannel.hpp>

namespace JASystem {

class TChannelMgr {
public:
	static void init();
	static void stopAll();
	static void stopAllRelease();
	static void checkGlobalRelease();
	static TChannel* getLogicalChannel(unsigned long);
	static void initAllocChannel(unsigned long);
	static TChannel* getListHead(unsigned long);
	static unsigned long countList(unsigned long) const;
	static void cutList(TChannel*);
	static void addListHead(TChannel*, unsigned long);
	static void addListTail(TChannel*, unsigned long);
	static void receiveAllChannels(TChannelMgr*);
	static bool checkLimitStart(unsigned long);
	static bool checkLimitStop(TChannel*, unsigned long);
};

namespace Driver {
	extern unsigned long polys_table;
} // namespace Driver

} // namespace JASystem

#endif // JASCHANNELMGR_HPP
