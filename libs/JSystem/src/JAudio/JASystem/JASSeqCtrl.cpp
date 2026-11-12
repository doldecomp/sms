#include <JSystem/JAudio/JASystem/JASSeqCtrl.hpp>
#include <dolphin/types.h>

namespace JASystem {

void TSeqCtrl::init(void* data, u32 seq_offset)
{
	mRawFilePtr     = (u8*)data;
	mCurrentFilePtr = mRawFilePtr + seq_offset;
	mWaitTimer      = 0;
	mStackDepth     = 0;
	for (int i = 0; i < 8; i++) {
		mStackPositions[i] = nullptr;
		mLoopTimers[i]     = 0;
	}
	mSavedWaitTimer     = 0;
	mInterruptReturnPtr = nullptr;
}

bool TSeqCtrl::loopE()
{
	u32 var1 = mStackDepth;
	if (var1 == 0)
		return false;

	u16 var3 = mLoopTimers[var1 - 1];
	if (var3)
		var3--;

	if (var3 == 0) {
		mStackDepth--;
		return true;
	}

	mLoopTimers[var1 - 1] = var3;
	mCurrentFilePtr       = mStackPositions[mStackDepth - 1];
	return true;
}

bool TSeqCtrl::waitCountDown()
{
	if (mWaitTimer > 0) {
		mWaitTimer--;
		if (mWaitTimer)
			return false;
	}
	return true;
}

bool TSeqCtrl::callIntr(void* handler)
{
	if (mInterruptReturnPtr)
		return false;

	mInterruptReturnPtr = mCurrentFilePtr;
	mCurrentFilePtr     = (u8*)handler;
	mSavedWaitTimer     = mWaitTimer;
	mWaitTimer          = 0;
	return true;
}

bool TSeqCtrl::retIntr()
{
	if (!mInterruptReturnPtr)
		return false;

	mWaitTimer          = mSavedWaitTimer;
	mCurrentFilePtr     = mInterruptReturnPtr;
	mInterruptReturnPtr = nullptr;
	return true;
}

u16 TSeqCtrl::get16(u32 offset) const
{
	u16 result = 0;
	result |= getByte(offset++);
	result <<= 8;
	result |= getByte(offset);
	return result;
}

u32 TSeqCtrl::get24(u32 offset) const
{
	u32 result = 0;
	result |= getByte(offset++);
	result <<= 8;
	result |= getByte(offset++);
	result <<= 8;
	result |= getByte(offset);
	return result;
}

u32 TSeqCtrl::get32(u32 offset) const
{
	u32 result = 0;
	result |= getByte(offset++);
	result <<= 8;
	result |= getByte(offset++);
	result <<= 8;
	result |= getByte(offset++);
	result <<= 8;
	result |= getByte(offset);
	return result;
}

u16 TSeqCtrl::read16()
{
	u16 result = 0;
	result |= readByte();
	result <<= 8;
	result |= readByte();
	return result;
}

u32 TSeqCtrl::read24()
{
	u32 result = 0;
	result |= readByte();
	result <<= 8;
	result |= readByte();
	result <<= 8;
	result |= readByte();
	return result;
}

u32 TSeqCtrl::read32()
{
	u32 result = 0;
	result |= readByte();
	result <<= 8;
	result |= readByte();
	result <<= 8;
	result |= readByte();
	result <<= 8;
	result |= readByte();
	return result;
}

} // namespace JASystem
