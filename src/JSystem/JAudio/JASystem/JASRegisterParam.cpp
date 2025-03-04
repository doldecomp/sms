#include "JSystem/JAudio/JASystem/JASRegisterParam.hpp"

namespace JASystem {

TRegisterParam::TRegisterParam() { }

void TRegisterParam::init() { }

void TRegisterParam::inherit(const TRegisterParam& other) { }

void TRegisterParam::setBankNumber(u8 bankNum) { }

u8 TRegisterParam::getBankNumber() const { return 0; }

u8 TRegisterParam::getProgramNumber() const { return 0; }

} // namespace JASystem
