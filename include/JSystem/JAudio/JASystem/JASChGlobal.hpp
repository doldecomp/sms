#ifndef JASCHGLOBAL_HPP
#define JASCHGLOBAL_HPP

#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>

namespace JASystem {

class ChGlobal {
public:
	static void init();
	static void release(TChannel* channel);
	static void releaseAll(TChannelMgr* channelMgr);
	static TChannel* alloc(TChannelMgr* channelMgr, unsigned long param);
	static unsigned long getChannelHandle(unsigned long param);
	static unsigned long getGlobalHandle();

	static unsigned long GLOBAL_CHANNEL;
	static unsigned long CHANNEL;
	static unsigned long OSCILLATOR;
};

} // namespace JASystem

#endif // JASCHGLOBAL_HPP
