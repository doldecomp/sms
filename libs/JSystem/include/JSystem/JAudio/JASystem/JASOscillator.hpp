#ifndef JASOSCILLATOR_HPP
#define JASOSCILLATOR_HPP

#include <dolphin/types.h>

namespace JASystem {

class TOscillator {
public:
	TOscillator() { init(); }
	~TOscillator() { }

	struct Osc_ {
		/* 0x00 */ u8 mTarget;
		/* 0x04 */ f32 mRate;
		/* 0x08 */ s16* mAdsTable;
		/* 0x0C */ s16* mRelTable;
		/* 0x10 */ f32 mWidth;
		/* 0x14 */ f32 mVertex;
	};

	bool release();
	void initStart();
	void init();
	f32 getOffsetNoCount();
	bool forceStop();
	f32 getOffset();
	f32 calc(s16*);

	const Osc_* getOsc() const { return mOsc; }
	u8 getTarget() const { return mOsc->mTarget; }
	void setOsc(const Osc_* osc) { mOsc = osc; }
	BOOL isOsc() { return mOsc != NULL ? TRUE : FALSE; }
	void releaseDirect(u16 param_0) { mDirectRelease = param_0; }

public:
	/* 0x0 */ const Osc_* mOsc;
	/* 0x04 */ u8 mState;
	/* 0x05 */ u8 unk5;
	/* 0x06 */ u16 unk6;
	/* 0x08 */ f32 mReleaseRate;
	/* 0x0C */ f32 mPhase;
	/* 0x10 */ f32 mTargetPhase;
	/* 0x14 */ f32 mPhaseChangeRate;
	/* 0x18 */ u16 mDirectRelease;
	/* 0x1C */ f32 mInitialReleaseRate;
};

namespace Driver {
	extern s16 FORCE_STOP_TABLE[];
} // namespace Driver

} // namespace JASystem

#endif // JASOSCILLATOR_HPP
