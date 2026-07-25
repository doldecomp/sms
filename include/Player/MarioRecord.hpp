#ifndef MARIORECORD_HPP
#define MARIORECORD_HPP

#include <dolphin/types.h>

template <typename T> class TRecordValueManager {
public:
	void reset();
	bool get(T* outValue);

public:
	/* 0x0 */ u32* mFirstDurationPtr;
	/* 0x4 */ T* mFirstValuePtr;
	/* 0x8 */ u32* mCurrentDurationPtr;
	/* 0xC */ T* mCurrentValuePtr;
	/* 0x10 */ s32 mElapsedFrames;
	/* 0x14 */ s32 mUnk14;
	/* 0x18 */ s32 mUnk18;
};

class TMarioInputReplay {
public:
	void init(u8* iData);
	bool play(f32* outIntendedMag, s16* outIntendedYaw, u32* outPressedBtns,
	          u32* outJustPressedBtns, u8*, u8*);
	void reset();
	void start() { mCanPlay = 1; }
	bool canPlay() const
	{
		if (mCanPlay == 1) {
			return true;
		}
		return false;
	}

	// fabricated
	void start() { mCanPlay = 1; }
	bool canPlay() const
	{
		if (mCanPlay == 1)
			return true;
		return false;
	}

private:
	/* 0x0 */ s16 mUnk0;
	/* 0x2 */ u16 mCanPlay;
	/* 0x4 */ u16 mPrevBtnMask;
	/* 0x6 */ s16 mUnk6;
	/* 0x8 */ u32 mReplayPos;
	/* 0xC */ u32 mReplayLength;
	/* 0x10 */ TRecordValueManager<f32> mMag;
	/* 0x2C */ TRecordValueManager<s16> mYaw;
	/* 0x48 */ TRecordValueManager<u16> mBtnMask;
	/* 0x64 */ TRecordValueManager<u8> mUnk64;
	/* 0x80 */ TRecordValueManager<u8> mUnk80;
};

#endif // MARIORECORD_HPP
