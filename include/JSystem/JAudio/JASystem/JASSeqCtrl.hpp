#ifndef JASSEQCTRL_HPP
#define JASSEQCTRL_HPP

#include <dolphin/types.h>

namespace JASystem {
class TSeqCtrl {
public:
	void init(void* data, u32 size);
	bool loopE();
	bool waitCountDown();
	bool callIntr(void* intr);
	bool retIntr();
	u16 read16();
	u16 get16(u32 offset) const;
	u32 get24(u32 offset) const;
	u32 get32(u32 offset) const;
	u32 read24();
	u32 read32();

	// Inlines taken from TWW, probably fabricated

	u8 getByte(u32 offset) const { return mRawFilePtr[offset]; }
	u8 readByte() { return *mCurrentFilePtr++; }

	u8* getBase() { return mRawFilePtr; }

	void call(u32 offset)
	{
		mLoopStartPositions[mLoopIndex++] = mCurrentFilePtr;
		mCurrentFilePtr                   = mRawFilePtr + offset;
	}
	bool ret()
	{
		mCurrentFilePtr = mLoopStartPositions[--mLoopIndex];
		return true;
	}
	void jump(u32 offset) { mCurrentFilePtr = mRawFilePtr + offset; }
	void loopS(u32 timer)
	{
		mLoopStartPositions[mLoopIndex] = mCurrentFilePtr;
		mLoopTimers[mLoopIndex++]       = timer;
	}

	void wait(s32 timer) { mWaitTimer = timer; }

	void clrIntr() { mPreviousFilePtr = 0; }

public:
	/* 0x00 */ u8* mRawFilePtr;
	/* 0x04 */ u8* mCurrentFilePtr;
	/* 0x08 */ int mWaitTimer;
	/* 0x0C */ u32 mLoopIndex;
	/* 0x10 */ u8* mLoopStartPositions[8];
	/* 0x30 */ u16 mLoopTimers[8];
	/* 0x40 */ int field_0x40;
	/* 0x44 */ u8* mPreviousFilePtr;
};
} // namespace JASystem

#endif // JASSEQCTRL_HPP
