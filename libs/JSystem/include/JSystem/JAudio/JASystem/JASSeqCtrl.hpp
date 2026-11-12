#ifndef JASSEQCTRL_HPP
#define JASSEQCTRL_HPP

#include <dolphin/types.h>

namespace JASystem {
class TSeqCtrl {
public:
	void init(void* data, u32 seq_offset);
	bool loopE();
	bool waitCountDown();
	bool callIntr(void* handler);
	bool retIntr();
	u16 read16();
	u16 get16(u32 offset) const;
	u32 get24(u32 offset) const;
	u32 get32(u32 offset) const;
	u32 read24();
	u32 read32();

	u8 getByte(u32 offset) const { return mRawFilePtr[offset]; }
	u8 readByte() { return *mCurrentFilePtr++; }

	u8* getBase() { return mRawFilePtr; }
	u8* getAddr(u32 offset) { return mRawFilePtr + offset; }

	void call(u32 offset)
	{
		mStackPositions[mStackDepth++] = mCurrentFilePtr;
		mCurrentFilePtr                = mRawFilePtr + offset;
	}
	bool ret()
	{
		mCurrentFilePtr = mStackPositions[--mStackDepth];
		return true;
	}
	void jump(u32 offset) { mCurrentFilePtr = mRawFilePtr + offset; }
	void loopS(u32 timer)
	{
		mStackPositions[mStackDepth] = mCurrentFilePtr;
		mLoopTimers[mStackDepth++]   = timer;
	}

	void wait(s32 timer) { mWaitTimer = timer; }
	s32 getWait() const { return mWaitTimer; }

	u16 getLoopCount() const
	{
		if (mStackDepth == 0)
			return 0;
		return mLoopTimers[mStackDepth - 1];
	}

	bool isIntr() const { return mInterruptReturnPtr != 0 ? true : false; }
	void clrIntr() { mInterruptReturnPtr = 0; }

public:
	/* 0x00 */ u8* mRawFilePtr;
	/* 0x04 */ u8* mCurrentFilePtr;
	/* 0x08 */ int mWaitTimer;
	/* 0x0C */ u32 mStackDepth;
	/* 0x10 */ u8* mStackPositions[8];
	/* 0x30 */ u16 mLoopTimers[8];
	/* 0x40 */ int mSavedWaitTimer;
	/* 0x44 */ u8* mInterruptReturnPtr;
};
} // namespace JASystem

#endif // JASSEQCTRL_HPP
