#include <JSystem/JAudio/JASystem/JASDrumSet.hpp>

namespace JASystem {

TDrumSet::TPerc::TPerc() { }

TDrumSet::TPerc::~TPerc() { }

void TDrumSet::TPerc::setRelease(u32) { }

void TDrumSet::TPerc::setEffectCount(u32) { }

void TDrumSet::TPerc::setEffect(int, TInstEffect*) { }

void TDrumSet::TPerc::setVeloRegionCount(u32) { }

TInstVeloRegion* TDrumSet::TPerc::getVeloRegion(int) { return 0; }

const TInstVeloRegion* TDrumSet::TPerc::getVeloRegion(int) const { return 0; }

TInstEffect* TDrumSet::TPerc::getEffect(int) { return 0; }

TDrumSet::TDrumSet() { }

TDrumSet::~TDrumSet() { }

int TDrumSet::getType() const { return 0; }

bool TDrumSet::getParam(int, int, TInstParam*) const { return false; }

TDrumSet::TPerc* TDrumSet::getPerc(int) { return 0; }

const TDrumSet::TPerc* TDrumSet::getPerc(int) const { return 0; }

} // namespace JASystem
