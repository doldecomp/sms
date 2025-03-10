#include <JSystem/JAudio/JASystem/JASOscillator.hpp>
#include <JSystem/JAudio/JASystem/JASDriverIF.hpp>
#include <JSystem/JAudio/JASystem/JASRate.hpp>
#include <math.h>
#include <types.h>

namespace JASystem {

namespace Driver {
	s16 FORCE_STOP_TABLE[6] = { 0, 15, 0, 15, 0, 0 };

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
	mOsc                = nullptr;
	mState              = 1;
	unk5                = 0;
	unk6                = 0;
	mReleaseRate        = 0.0f;
	mPhase              = 0.0f;
	mTargetPhase        = 0.0f;
	mPhaseChangeRate    = 0.0f;
	mDirectRelease      = 0;
	mInitialReleaseRate = 0.0f;
}

void TOscillator::initStart()
{
	mState         = 2;
	mDirectRelease = 0;
	if (mOsc == nullptr || !mOsc->unk8) {
		mPhase = 0.0f;
		return;
	}
	unk6           = 0;
	mReleaseRate   = 0.0f;
	mTargetPhase   = 0.0f;
	mDirectRelease = 0;
	mReleaseRate -= mOsc->unk4;
}

f32 TOscillator::getOffset()
{
	if (mOsc == nullptr) {
		mPhase = 1.0f;
		return 1.0f;
	}

	switch (mState) {
	case 0:
		return 1.0f;
	case 3:
		return mOsc->unk14 + (mPhase * mOsc->unk10);

	case 1:
		mState = 2;
		/* Fallthrough */
	default:
		s16* var_r4;
		if (mState == 4) {
			var_r4 = mOsc->unkC;
		} else if (mState == 5) {
			var_r4 = Driver::FORCE_STOP_TABLE;
		} else {
			var_r4 = mOsc->unk8;
		}

		if (var_r4 == nullptr && mState != 6) {
			mPhase = 1.0f;
			return 1.0f;
		}

		if (mState == 5) {
			mReleaseRate -= 1.0f;
		} else {
			mReleaseRate -= mOsc->unk4;
		}

		return calc(var_r4);
	}
}

f32 TOscillator::getOffsetNoCount()
{
	if (mOsc == nullptr) {
		mPhase = 1.0f;
		return 1.0f;
	}

	switch (mState) {
	case 0:
		return 1.0f;
	case 1:
		mState = 2;
		break;
	}

	return mPhase * mOsc->unk10 + mOsc->unk14;
}

bool TOscillator::forceStop()
{
	if (mState == 5)
		return false;

	unk6         = 0;
	mReleaseRate = 0.0f;
	mTargetPhase = mPhase;
	mState       = 5;
	return true;
}

bool TOscillator::release()
{
	if (mState == 5)
		return false;

	if (mOsc->unk8 != mOsc->unkC) {
		unk6         = 0;
		mReleaseRate = 0.0f;
		mTargetPhase = mPhase;
	}

	if (mOsc->unkC == nullptr && mDirectRelease == 0)
		mDirectRelease = 0x10;

	if (mDirectRelease != 0) {
		mState = 6;
		unk5   = (mDirectRelease >> 14) & 3;

		f32 temp_f31 = mDirectRelease & 0x3FFF;
		temp_f31 *= ((Kernel::getDacRate() / 80.0f) / 600.0f);
		temp_f31 /= Driver::getUpdateInterval();
		mReleaseRate = temp_f31;

		if (mReleaseRate < 1.0f)
			mReleaseRate = 1.0f;

		mInitialReleaseRate = mReleaseRate;
		mTargetPhase        = 0.0f;
		if (unk5 == 0)
			mPhaseChangeRate = (mTargetPhase - mPhase) / mReleaseRate;
		else
			mPhaseChangeRate = mTargetPhase - mPhase;
	} else {
		mState = 4;
	}

	return true;
}

f32 TOscillator::calc(s16* table)
{
	while (mReleaseRate <= 0.0f) {
		int idx = unk6 * 3;
		mPhase  = mTargetPhase;
		if (mState == 6) {
			mState = 0;
			break;
		}

		s16 envMode  = table[idx];
		s16 envTime  = table[idx + 1];
		s16 envValue = table[idx + 2];

		if (envMode == 13) {
			unk6 = envValue;
			continue;
		}

		if (envMode == 15) {
			mState = 0;
			break;
		}

		if (envMode == 14) {
			mState = 3;
			return mOsc->unk14 + mPhase * mOsc->unk10;
		}

		unk5 = envMode;

		if (envTime == 0) {
			mTargetPhase = envValue / 32768.0f;
			unk6 += 1;
			continue;
		}

		mReleaseRate = envTime * ((Kernel::getDacRate() / 80.0f) / 600.0f)
		               / Driver::getUpdateInterval();
		mInitialReleaseRate = mReleaseRate;
		mTargetPhase        = envValue / 32768.0f;

		if (unk5 == 0)
			mPhaseChangeRate = (mTargetPhase - mPhase) / mReleaseRate;
		else
			mPhaseChangeRate = mTargetPhase - mPhase;

		unk6 += 1;
	}

	if (mOsc->unk10 == 0.0f)
		return mOsc->unk14;

	f32 temp_f31 = 0.0f;
	f32 newPhase;
	if (mInitialReleaseRate == 0.0) { // Developer probably forgot the f suffix
		newPhase = mTargetPhase;
		mPhase   = mTargetPhase;
	} else {
		if (unk5 == 0 || (temp_f31 = mPhaseChangeRate) == 0.0f) {
			newPhase = mTargetPhase - (mPhaseChangeRate * mReleaseRate);
			mPhase   = newPhase;
		} else if (unk5 == 3 || unk5 == 1 || unk5 == 2) {
			const f32* table = nullptr;

			switch (unk5) {
			case 3:
				table = Driver::relTableSampleCell;
				break;
			case 1:
				table = Driver::relTableSquare;
				break;
			case 2:
				table = Driver::relTableSqRoot;
				break;
			}

			f32 fIdx;

			if (temp_f31 < 0.0f)
				fIdx = 16.0f * (1.0f - (mReleaseRate / mInitialReleaseRate));
			else
				fIdx = 16.0f * (mReleaseRate / mInitialReleaseRate);

			u32 idx  = fIdx;
			f32 prop = fIdx - (f32)idx;
			if (idx >= 16) {
				idx  = 15;
				prop = 1.0f;
			}

			f32 valAbs = std::fabsf(
			    temp_f31 * (prop * (table[idx + 1] - table[idx]) + table[idx]));

			if (mPhaseChangeRate < 0.0f)
				newPhase = mTargetPhase + valAbs;
			else
				newPhase = mTargetPhase - (mPhaseChangeRate - valAbs);

			mPhase = newPhase;
		} else {
			newPhase = mTargetPhase - (temp_f31 * mReleaseRate);
			mPhase   = newPhase;
		}
	}

	return newPhase * mOsc->unk10 + mOsc->unk14;
}

} // namespace JASystem
