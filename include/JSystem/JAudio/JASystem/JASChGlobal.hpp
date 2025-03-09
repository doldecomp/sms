#ifndef JASCHGLOBAL_HPP
#define JASCHGLOBAL_HPP

#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>
#include <dolphin/types.h>

namespace JASystem {

class ChGlobal {
public:
	void init();
	static void release(TChannel* channel);
	static void releaseAll(TChannelMgr* channelMgr);
	static TChannel* alloc(TChannelMgr* channelMgr, u32 param);
	u32 getChannelHandle(u32 param);
	u32 getGlobalHandle();

	static u32 GLOBAL_CHANNEL;
	static u32 CHANNEL;
	static u32 OSCILLATOR;
};

} // namespace JASystem

#endif // JASCHGLOBAL_HPP
