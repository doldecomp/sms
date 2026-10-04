#include <JSystem/JAudio/JASystem/JASBasicBank.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>

namespace JASystem {

TBasicBank::TBasicBank()
    : mInstCount(0)
    , mInsts(nullptr)
{
}

TBasicBank::~TBasicBank() { delete[] mInsts; }

void TBasicBank::setInstCount(u32 count)
{
	delete[] mInsts;
	mInsts = new (TBank::getCurrentHeap(), 0) TInst*[count];
	Calc::bzero(mInsts, count * sizeof(TInst*));
	mInstCount = count;
}

void TBasicBank::setInst(int index, TInst* inst) { mInsts[index] = inst; }

TInst* TBasicBank::getInst(int index) const
{
	if (index >= mInstCount)
		return nullptr;
	return mInsts[index];
}

} // namespace JASystem
