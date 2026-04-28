#ifndef MARIORECORD_HPP
#define MARIORECORD_HPP

#include <dolphin/types.h>

template <typename T> class TRecordValueManager {
public:
	void reset();
	bool get(T* outValue);

public:
	u32* mFirstDurationPtr;   // 0x0
	T* mFirstValuePtr;        // 0x4
	u32* mCurrentDurationPtr; // 0x8
	T* mCurrentValuePtr;      // 0xC
	s32 mElapsedFrames;       // 0x10
	s32 mUnk14;               // 0x14
	s32 mUnk18;               // 0x18
};

class TMarioInputReplay {
public:
	void init(u8* iData);
	bool play(f32* outIntendedMag, s16* outIntendedYaw, u32* outPressedBtns,
	          u32* outJustPressedBtns, u8*, u8*);
	void reset();

private:
	s16 mUnk0;                         // 0x0
	u16 mCanPlay;                      // 0x2
	u16 mPrevBtnMask;                  // 0x4
	s16 mUnk6;                         // 0x6
	u32 mReplayPos;                    // 0x8
	u32 mReplayLength;                 // 0xC
	TRecordValueManager<f32> mMag;     // 0x10
	TRecordValueManager<s16> mYaw;     // 0x2C
	TRecordValueManager<u16> mBtnMask; // 0x48
	TRecordValueManager<u8> mUnk64;    // 0x64
	TRecordValueManager<u8> mUnk80;    // 0x80
};

#endif // MARIORECORD_HPP
