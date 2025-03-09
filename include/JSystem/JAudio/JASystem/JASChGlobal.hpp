#ifndef JASCHGLOBAL_HPP
#define JASCHGLOBAL_HPP

#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>
#include <dolphin/types.h>

namespace JASystem {

namespace ChGlobal {

	void init();
	int release(TChannel* channel);
	int releaseAll(TChannelMgr* channelMgr);
	int alloc(TChannelMgr* channelMgr, u32 param);
	TChannel* getChannelHandle(u32 param);
	u32 getGlobalHandle();

} // namespace ChGlobal

} // namespace JASystem

#endif // JASCHGLOBAL_HPP
