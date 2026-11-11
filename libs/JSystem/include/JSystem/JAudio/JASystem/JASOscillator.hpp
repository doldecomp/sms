#ifndef JASOSCILLATOR_HPP
#define JASOSCILLATOR_HPP

#include <dolphin/types.h>

namespace JASystem {

/**
 * @brief Generic modulation source used by the audio engine. Supports a wide
 * range of data-driven envelope curves, including periodic and infinitely
 * sustainable. Release must be requested explicitly.
 */
class TOscillator {
public:
	/// Serialized data that customizes this oscillator
	struct Osc_ {
		/* 0x00 */ u8 mTarget;
		/* 0x04 */ f32 mRate;
		/* 0x08 */ s16* mAdsTable;
		/* 0x0C */ s16* mRelTable;
		/* 0x10 */ f32 mDepth;
		/* 0x14 */ f32 mBias;
	};

	TOscillator() { init(); }
	~TOscillator() { }

	void initStart();
	void init();

	/// Advances the oscillator one step and returns it's new value
	f32 getOffset();

	// random wrapper for no reason (from TWW)
	f32 bankOscToOfs() { return getOffset(); }

	/// Gets the current value of the oscillator without advancing it
	f32 getOffsetNoCount();

	/// Requests a release
	bool release();

	void releaseDirect(u16 param_0) { mDirectRelease = param_0; }

	bool forceStop();

	u8 getEffectorKind() { return mOsc->mTarget; }
	u8 getOscMode() const { return mState; }
	void setOsc(const Osc_* osc) { mOsc = osc; }
	const Osc_* getOsc() const { return mOsc; }
	BOOL isOsc() { return mOsc != NULL ? TRUE : FALSE; }

	enum State {
		STATE_STOPPED        = 0,
		STATE_READY          = 1,
		STATE_RUNNING        = 2,
		STATE_HOLD           = 3,
		STATE_RELEASE        = 4,
		STATE_FORCE_STOP     = 5,
		STATE_DIRECT_RELEASE = 6,
	};

	enum CurveType {
		CURVE_LINEAR      = 0,
		CURVE_SQUARE      = 1,
		CURVE_SQRT        = 2,
		CURVE_SAMPLE_CELL = 3,
		// Sentinel values in the data, not actual curves.
		CURVE_LOOP = 13,
		CURVE_HOLD = 14,
		CURVE_HALT = 15,
	};

private:
	f32 calc(s16*);

	/* 0x0 */ const Osc_* mOsc;
	/* 0x04 */ u8 mState;
	/* 0x05 */ u8 mCurveType;
	/* 0x06 */ u16 mTableIndex;
	/* 0x08 */ f32 mTimeRemaining;
	/* 0x0C */ f32 mCurrentValue;
	/* 0x10 */ f32 mTargetValue;
	/* 0x14 */ f32 mInterpolationScale;
	/* 0x18 */ u16 mDirectRelease;
	/* 0x1C */ f32 mSegmentDuration;
};

namespace Driver {
	extern s16 FORCE_STOP_TABLE[];
} // namespace Driver

} // namespace JASystem

#endif // JASOSCILLATOR_HPP
