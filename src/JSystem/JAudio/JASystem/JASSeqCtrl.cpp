#include "JSystem/JAudio/JASystem/JASSeqCtrl.hpp"

namespace JASystem {

void TSeqCtrl::init(void* data, u32 size) { }

void TSeqCtrl::callIntr(void* intr) { }

void TSeqCtrl::waitCountDown() { }

u16 TSeqCtrl::read16() { return 0; }

u16 TSeqCtrl::get16(u32 offset) const { return 0; }

u32 TSeqCtrl::get24(u32 offset) const { return 0; }

u32 TSeqCtrl::get32(u32 offset) const { return 0; }

u32 TSeqCtrl::read24() { return 0; }

u32 TSeqCtrl::read32() { return 0; }

void TSeqCtrl::loopE() { }

void TSeqCtrl::retIntr() { }

} // namespace JASystem
