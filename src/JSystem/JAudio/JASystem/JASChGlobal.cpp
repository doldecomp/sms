#include <JSystem/JAudio/JASystem/JASChGlobal.hpp>

namespace JASystem {

void ChGlobal::init() { }

void ChGlobal::release(TChannel* channel) { }

void ChGlobal::releaseAll(TChannelMgr* channelMgr) { }

TChannel* ChGlobal::alloc(TChannelMgr* channelMgr, u32 param) { return 0; }

u32 ChGlobal::getChannelHandle(u32 param) { return 0; }

u32 ChGlobal::getGlobalHandle() { return 0; }

u32 ChGlobal::GLOBAL_CHANNEL = 0;
u32 ChGlobal::CHANNEL        = 0;
u32 ChGlobal::OSCILLATOR     = 0;

} // namespace JASystem
