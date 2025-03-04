#include <JSystem/JAudio/JASystem/JASBasicInst.hpp>

namespace JASystem {

TInst::~TInst() { }

TBasicInst::TBasicInst() { }

TBasicInst::~TBasicInst() { }

int TBasicInst::getType() const { return 0; }

int TBasicInst::getKeymapIndex(int index) const { return 0; }

bool TBasicInst::getParam(int param1, int param2, TInstParam* param) const
{
	return false;
}

void TBasicInst::setOscCount(u32 count) { }

void TBasicInst::setOsc(int index, TOscillator::Osc_* osc) { }

TOscillator::Osc_* TBasicInst::getOsc(int index) { return 0; }

void TBasicInst::setEffectCount(u32 count) { }

void TBasicInst::setEffect(int index, TInstEffect* effect) { }

TInstEffect* TBasicInst::getEffect(int index) { return 0; }

void TBasicInst::setKeyRegionCount(u32 count) { }

int TBasicInst::searchKeymap(int key) const { return 0; }

TBasicInst::TKeymap* TBasicInst::getKeyRegion(int index) { return 0; }

const TBasicInst::TKeymap* TBasicInst::getKeyRegion(int index) const
{
	return 0;
}

TBasicInst::TKeymap::TKeymap() { }

TBasicInst::TKeymap::~TKeymap() { }

void TBasicInst::TKeymap::setVeloRegionCount(u32 count) { }

void* TBasicInst::TKeymap::getVeloRegion(int index) { return 0; }

const void* TBasicInst::TKeymap::getVeloRegion(int index) const { return 0; }

} // namespace JASystem
