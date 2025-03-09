#include <JSystem/JAudio/JASystem/JASDSPChannel.hpp>

namespace JASystem {

// Static member initialization
TDSPChannel* TDSPChannel::DSPCH  = 0;
u32 TDSPChannel::smnUse          = 0;
u32 TDSPChannel::smnFree         = 0;
f32 TDSPChannel::DSP_LIMIT_RATIO = 0.0f;
f32* TDSPChannel::history        = 0;

TDSPChannel::TDSPChannel() { }

TDSPChannel::~TDSPChannel() { }

void TDSPChannel::initAll() { }

void TDSPChannel::updateAll() { }

TDSPChannel* TDSPChannel::alloc(u32 param1, u32 param2) { return 0; }

void TDSPChannel::free(TDSPChannel* channel, u32 param) { }

TDSPChannel* TDSPChannel::getHandle(u32 handle) { return 0; }

u32 TDSPChannel::getNumUse() { return 0; }

u32 TDSPChannel::getNumFree() { return 0; }

void TDSPChannel::setLimitDSP(f32 limit) { }

f32* TDSPChannel::getHistory() { return 0; }

void TDSPChannel::init(u8 param) { }

void TDSPChannel::allocate(u32 param) { }

void TDSPChannel::free() { }

void TDSPChannel::play() { }

void TDSPChannel::stop() { }

void TDSPChannel::pause() { }

void TDSPChannel::restart() { }

void TDSPChannel::forceStop() { }

void TDSPChannel::forceDelete() { }

u8 TDSPChannel::getLower() { return 0; }

u8 TDSPChannel::getLowerActive() { return 0; }

int TDSPChannel::breakLower(u8 param) { }

void TDSPChannel::breakLowerActive(u8 param) { }

} // namespace JASystem
