#include <Player/MarioRecord.hpp>

template <typename T> bool TRecordValueManager<T>::get(T* outValue)
{
	*outValue = *mCurrentValuePtr;
	mElapsedFrames++;

	if (mElapsedFrames >= *mCurrentDurationPtr) {
		mCurrentDurationPtr++;
		mCurrentValuePtr++;
		mElapsedFrames = 0;
		return true;
	}
	return false;
}

template <typename T> void TRecordValueManager<T>::reset()
{
	mCurrentDurationPtr = mFirstDurationPtr;
	mCurrentValuePtr    = mFirstValuePtr;
	mElapsedFrames      = 0;
	mUnk14              = 0;
}

void TMarioInputReplay::init(u8* iData)
{
	mUnk0        = 0;
	mCanPlay     = 0;
	mPrevBtnMask = 0;
	mReplayPos   = 0;

	s32 offset = 0x10;

	mReplayLength = *reinterpret_cast<s32*>(iData + offset);

	offset += 4;
	mMag.mFirstDurationPtr = (u32*)(iData + *(s32*)(iData + offset));
	offset += 4;
	mMag.mFirstValuePtr = (f32*)(iData + *(s32*)(iData + offset));
	mMag.reset();

	offset += 4;
	mYaw.mFirstDurationPtr = (u32*)(iData + *(s32*)(iData + offset));
	offset += 4;
	mYaw.mFirstValuePtr = (s16*)(iData + *(s32*)(iData + offset));
	mYaw.reset();

	offset += 4;
	mBtnMask.mFirstDurationPtr = (u32*)(iData + *(s32*)(iData + offset));
	offset += 4;
	mBtnMask.mFirstValuePtr = (u16*)(iData + *(s32*)(iData + offset));
	mBtnMask.reset();

	offset += 4;
	mUnk64.mFirstDurationPtr = (u32*)(iData + *(s32*)(iData + offset));
	offset += 4;
	mUnk64.mFirstValuePtr = (u8*)(iData + *(s32*)(iData + offset));
	mUnk64.reset();

	offset += 4;
	mUnk80.mFirstDurationPtr = (u32*)(iData + *(s32*)(iData + offset));
	offset += 4;
	mUnk80.mFirstValuePtr = (u8*)(iData + *(s32*)(iData + offset));
	mUnk80.reset();
}

bool TMarioInputReplay::play(f32* outIntendedMag, s16* outIntendedYaw,
                             u32* outBtnMask, u32* outJustPressedBtnMask,
                             u8* outParam5, u8* outParam6)
{
	if (mCanPlay == 1) {
		if (mReplayPos < mReplayLength) {
			mMag.get(outIntendedMag);
			mYaw.get(outIntendedYaw);

			u16 btnMask;
			mBtnMask.get(&btnMask);

			u16 justPressedBtns = btnMask & (btnMask ^ mPrevBtnMask);
			mPrevBtnMask        = btnMask;

			*outBtnMask = (*outBtnMask & 0xFFFF0000) | btnMask;
			*outJustPressedBtnMask
			    = justPressedBtns | (*outJustPressedBtnMask & 0xFFFF0000);

			mUnk64.get(outParam5);
			mUnk80.get(outParam6);

			mReplayPos++;
			return true;
		}
		mCanPlay = 0;
	}
	return false;
}

void TMarioInputReplay::reset()
{
	mCanPlay     = 0;
	mPrevBtnMask = 0;
	mReplayPos   = 0;

	mMag.reset();
	mYaw.reset();
	mBtnMask.reset();
	mUnk64.reset();
	mUnk80.reset();
}
