#include <JSystem/JAudio/JASystem/JASBasicBank.hpp>

namespace JASystem {

TBasicBank::TBasicBank()
    : mInstCount(0)
    , mInsts(nullptr)
{
}
TBank::~TBank() { }
TBasicBank::~TBasicBank() { }

int TBasicBank::getType() const { return 0; }

TInst* TBasicBank::getInst(int index) const { return nullptr; }

void TBasicBank::setInstCount(u32 count) { mInstCount = count; }

void TBasicBank::setInst(int index, TInst* inst) { }

} // namespace JASystem
