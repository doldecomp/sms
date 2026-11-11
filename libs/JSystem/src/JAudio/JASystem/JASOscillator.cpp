#include <JSystem/JAudio/JASystem/JASOscillator.hpp>
#include <JSystem/JAudio/JASystem/JASDriverIF.hpp>
#include <JSystem/JAudio/JASystem/JASRate.hpp>
#include <math.h>
#include <dolphin/types.h>

namespace JASystem {

namespace Driver {
	s16 FORCE_STOP_TABLE[6] = {
		TOscillator::CURVE_LINEAR, 15, 0, TOscillator::CURVE_HALT, 0, 0,
	};

	static const f32 relTableSampleCell[17] = {
		1.0f,        0.970489f,   0.781274f,   0.54628098f, 0.39979199f,
		0.28931499f, 0.21210399f, 0.15747599f, 0.112613f,   0.081789598f,
		0.0579852f,  0.0436415f,  0.0308237f,  0.0237129f,  0.0152593f,
		0.00915555f, 0.0f,
	};
	static const f32 relTableSqRoot[17] = {
		1.0f,      0.878906f,  0.765625f, 0.660156f,   0.5625f,   0.472656f,
		0.390625f, 0.316406f,  0.25f,     0.191406f,   0.140625f, 0.097656198f,
		0.0625f,   0.0351562f, 0.015625f, 0.00390625f, 0.0f,
	};
	static const f32 relTableSquare[17] = {
		1.0f,        0.96824598f, 0.935414f, 0.90138799f, 0.86602497f,
		0.82915598f, 0.790569f,   0.75f,     0.707107f,   0.66143799f,
		0.61237198f, 0.559017f,   0.5f,      0.43301299f, 0.353553f,
		0.25f,       0.0f,
	};
} // namespace Driver

void TOscillator::init()
{
	mOsc                 = nullptr;
	mState               = STATE_READY;
	mCurveType           = CURVE_LINEAR;
	mTableIndex          = 0;
	mTimeRemaining       = 0.0f;
	mCurrentValue        = 0.0f;
	mTargetValue         = 0.0f;
	mInterpolationScale  = 0.0f;
	mDirectReleaseConfig = 0;
	mSegmentDuration     = 0.0f;
}

void TOscillator::initStart()
{
	mState               = STATE_RUNNING;
	mDirectReleaseConfig = 0;
	if (mOsc == nullptr || !mOsc->mAdsTable) {
		mCurrentValue = 0.0f;
		return;
	}
	mTableIndex          = 0;
	mTimeRemaining       = 0.0f;
	mTargetValue         = 0.0f;
	mDirectReleaseConfig = 0;
	mTimeRemaining -= mOsc->mRate;
}

f32 TOscillator::getOffset()
{
	if (mOsc == nullptr) {
		mCurrentValue = 1.0f;
		return 1.0f;
	}

	switch (mState) {
	case STATE_STOPPED:
		return 1.0f;
	case STATE_HOLD:
		return mOsc->mBias + (mCurrentValue * mOsc->mDepth);

	case STATE_READY:
		mState = STATE_RUNNING;
		/* Fallthrough */
	default:
		s16* table;
		if (mState == STATE_RELEASE) {
			table = mOsc->mRelTable;
		} else if (mState == STATE_FORCE_STOP) {
			table = Driver::FORCE_STOP_TABLE;
		} else {
			table = mOsc->mAdsTable;
		}

		if (table == nullptr && mState != STATE_DIRECT_RELEASE) {
			mCurrentValue = 1.0f;
			return 1.0f;
		}

		if (mState == STATE_FORCE_STOP) {
			mTimeRemaining -= 1.0f;
		} else {
			mTimeRemaining -= mOsc->mRate;
		}

		return calc(table);
	}
}

f32 TOscillator::getOffsetNoCount()
{
	if (mOsc == nullptr) {
		mCurrentValue = 1.0f;
		return 1.0f;
	}

	switch (mState) {
	case STATE_STOPPED:
		return 1.0f;
	case STATE_READY:
		mState = STATE_RUNNING;
		break;
	}

	return mCurrentValue * mOsc->mDepth + mOsc->mBias;
}

bool TOscillator::forceStop()
{
	if (mState == STATE_FORCE_STOP)
		return false;

	mTableIndex    = 0;
	mTimeRemaining = 0.0f;
	mTargetValue   = mCurrentValue;
	mState         = STATE_FORCE_STOP;
	return true;
}

bool TOscillator::release()
{
	if (mState == STATE_FORCE_STOP)
		return false;

	if (mOsc->mAdsTable != mOsc->mRelTable) {
		mTableIndex    = 0;
		mTimeRemaining = 0.0f;
		mTargetValue   = mCurrentValue;
	}

	if (mOsc->mRelTable == nullptr && mDirectReleaseConfig == 0)
		mDirectReleaseConfig = 16;

	if (mDirectReleaseConfig != 0) {
		mState     = STATE_DIRECT_RELEASE;
		mCurveType = (mDirectReleaseConfig >> 14) & 3;

		f32 releaseTime = mDirectReleaseConfig & 0x3FFF;
		releaseTime *= ((Kernel::getDacRate() / 80.0f) / 600.0f);
		releaseTime /= Driver::getUpdateInterval();
		mTimeRemaining = releaseTime;

		if (mTimeRemaining < 1.0f)
			mTimeRemaining = 1.0f;

		mSegmentDuration = mTimeRemaining;
		mTargetValue     = 0.0f;
		if (mCurveType == CURVE_LINEAR)
			mInterpolationScale
			    = (mTargetValue - mCurrentValue) / mTimeRemaining;
		else
			mInterpolationScale = mTargetValue - mCurrentValue;
	} else {
		mState = STATE_RELEASE;
	}

	return true;
}

f32 TOscillator::calc(s16* table)
{
	while (mTimeRemaining <= 0.0f) {
		int idx       = mTableIndex * 3;
		mCurrentValue = mTargetValue;
		if (mState == STATE_DIRECT_RELEASE) {
			mState = STATE_STOPPED;
			break;
		}

		s16 type  = table[idx];
		s16 time  = table[idx + 1];
		s16 value = table[idx + 2];

		if (type == CURVE_LOOP) {
			mTableIndex = value;
			continue;
		}

		if (type == CURVE_HALT) {
			mState = STATE_STOPPED;
			break;
		}

		if (type == CURVE_HOLD) {
			mState = STATE_HOLD;
			return mOsc->mBias + mCurrentValue * mOsc->mDepth;
		}

		mCurveType = type;

		if (time == 0) {
			mTargetValue = value / 32768.0f;
			mTableIndex += 1;
			continue;
		}

		mTimeRemaining = time * ((Kernel::getDacRate() / 80.0f) / 600.0f)
		                 / Driver::getUpdateInterval();
		mSegmentDuration = mTimeRemaining;
		mTargetValue     = value / 32768.0f;

		if (mCurveType == CURVE_LINEAR)
			mInterpolationScale
			    = (mTargetValue - mCurrentValue) / mTimeRemaining;
		else
			mInterpolationScale = mTargetValue - mCurrentValue;

		mTableIndex += 1;
	}

	if (mOsc->mDepth == 0.0f)
		return mOsc->mBias;

	f32 newValue;
	if (mSegmentDuration == 0.0) { // Developer probably forgot the f suffix
		newValue      = mTargetValue;
		mCurrentValue = mTargetValue;
	} else {
		if (mCurveType == CURVE_LINEAR || mInterpolationScale == 0.0f) {
			newValue = mTargetValue - (mInterpolationScale * mTimeRemaining);
			mCurrentValue = newValue;
		} else if (mCurveType == CURVE_SAMPLE_CELL || mCurveType == CURVE_SQUARE
		           || mCurveType == CURVE_SQRT) {
			const f32* table = nullptr;

			switch (mCurveType) {
			case CURVE_SAMPLE_CELL:
				table = Driver::relTableSampleCell;
				break;
			case CURVE_SQUARE:
				table = Driver::relTableSquare;
				break;
			case CURVE_SQRT:
				table = Driver::relTableSqRoot;
				break;
			}

			f32 fIdx;

			if (mInterpolationScale < 0.0f)
				fIdx = 16.0f * (1.0f - (mTimeRemaining / mSegmentDuration));
			else
				fIdx = 16.0f * (mTimeRemaining / mSegmentDuration);

			u32 idx  = fIdx;
			f32 prop = fIdx - (f32)idx;
			if (idx >= 16) {
				idx  = 15;
				prop = 1.0f;
			}

			f32 valAbs = std::fabsf(
			    mInterpolationScale
			    * (prop * (table[idx + 1] - table[idx]) + table[idx]));

			if (mInterpolationScale < 0.0f)
				newValue = mTargetValue + valAbs;
			else
				newValue = mTargetValue - (mInterpolationScale - valAbs);

			mCurrentValue = newValue;
		} else {
			newValue = mTargetValue - (mInterpolationScale * mTimeRemaining);
			mCurrentValue = newValue;
		}
	}

	return newValue * mOsc->mDepth + mOsc->mBias;
}

} // namespace JASystem
