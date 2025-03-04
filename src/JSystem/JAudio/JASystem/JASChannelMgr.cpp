#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>

namespace JASystem {

void TChannelMgr::init() { }

void TChannelMgr::stopAll() { }

void TChannelMgr::stopAllRelease() { }

void TChannelMgr::checkGlobalRelease() { }

TChannel* TChannelMgr::getLogicalChannel(u32 param) { return 0; }

void TChannelMgr::initAllocChannel(u32 param) { }

TChannel* TChannelMgr::getListHead(u32 param) { return 0; }

u32 TChannelMgr::countList(u32 param) { return 0; }

void TChannelMgr::cutList(TChannel* channel) { }

void TChannelMgr::addListHead(TChannel* channel, u32 param) { }

void TChannelMgr::addListTail(TChannel* channel, u32 param) { }

void TChannelMgr::receiveAllChannels(TChannelMgr* mgr) { }

bool TChannelMgr::checkLimitStart(u32 param) { return false; }

bool TChannelMgr::checkLimitStop(TChannel* channel, u32 param) { return false; }

namespace Driver {
	u8 polys_table[16] = { 0x0, 0x1, 0x2, 0x3,  0x4,  0x5,  0x6,  0x8,
		                   0xa, 0xc, 0xe, 0x10, 0x14, 0x18, 0x1c, 0x20 };
} // namespace Driver

} // namespace JASystem
