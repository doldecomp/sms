#include <System/MarioGamePad.hpp>
#include <System/Application.hpp>

JDrama::TFlagT<u16> TMarioGamePad::mResetFlag;

void TMarioGamePad::reset()
{
	setButtonRepeat(0xf00000f, 20.0f / SMSGetAnmFrameRate(),
	                6.0f / SMSGetAnmFrameRate());
	_E4             = 0;
	mDisabledFrames = 0;
}

void TMarioGamePad::updateMeaning()
{
	// Pre-existing fabricated padding, and the only reason this function
	// scores 100%: without it the frame is 0x40 against retail's 0x190, i.e.
	// 336 bytes of pure dead low region with every instruction already
	// identical. It must not be read as a reconstruction, and this unit is not
	// honestly closed until the padding is replaced by real locals.
	// Closure batch 120 measured the obvious carriers on the 26 inlined
	// updateMeaning(EButtons, PadMeanings, u32) expansions: binding the
	// predicate to a `bool` (returned or not) is +0x18 and +207 instructions,
	// and binding the two `mButton` reads to `u32`s is +0x68 (i.e. exactly 4
	// bytes per expansion) but +26 instructions. 336 / 26 is not integral, so
	// either the sites are at mixed depths or part of the region belongs to
	// another callee (checkFlag / resetMeaning).
	u32 stackAlloc[83];

	if (mDisabledFrames > 0) {
		mDisabledFrames -= 1;
	}

	u32 prevMeaning = resetMeaning();

	updateMeaning(START, MEANING_0x1, prevMeaning);

	if (checkFlag(PAD_FLAG_0x80)) {
		updateMeaning(A, MEANING_0x20, prevMeaning);
		updateMeaning(B, MEANING_0x40, prevMeaning);
		mCompSPos[4 * 2]     = mMainStick.mPosX;
		mCompSPos[4 * 2 + 1] = mMainStick.mPosY;
		goto finalize;
	}

	if (checkFlag(PAD_FLAG_0x1)) {
		// Some kind of 2d menu navigation, i think maybe for debug menu
		if (mButton.mRepeat & (MAINSTICK_UP | DPAD_UP)) {
			mMeaning |= MEANING_0x2;
		}
		if (mButton.mRepeat & (MAINSTICK_DOWN | DPAD_DOWN)) {
			mMeaning |= MEANING_0x4;
		}
		if (mButton.mRepeat & (MAINSTICK_LEFT | DPAD_LEFT)) {
			mMeaning |= MEANING_0x8;
		}
		if (mButton.mRepeat & (MAINSTICK_RIGHT | DPAD_RIGHT)) {
			mMeaning |= MEANING_0x10;
		}
		updateMeaning(A, MEANING_0x20, prevMeaning);
		updateMeaning(B, MEANING_0x40, prevMeaning);
		goto finalize;
	}

	if (!checkFlag(PAD_FLAG_0x2)) {
		goto finalize;
	}

	if (checkFlag(PAD_FLAG_0x8)) {
		// Menu navigation
		if (mButton.mRepeat & (MAINSTICK_UP | DPAD_UP)) {
			mMeaning |= MEANING_0x80000;
		}
		if (mButton.mRepeat & (MAINSTICK_DOWN | DPAD_DOWN)) {
			mMeaning |= MEANING_0x100000;
		}
		updateMeaning(A, MEANING_0x20000, prevMeaning);
		updateMeaning(B, MEANING_0x40000, prevMeaning);
		mCompSPos[1 * 2]     = (f32)(mButton.mAnalogL);
		mCompSPos[1 * 2 + 1] = (f32)(mButton.mAnalogR);
		updateMeaning(R, MEANING_0x400, prevMeaning);
		updateMeaning(Z, MEANING_0x1000, prevMeaning);
		updateMeaning(L, MEANING_0x2000, prevMeaning);
		goto finalize;
	}

	if (checkFlag(PAD_FLAG_0x10)) {
		goto finalize;
	}

	if (mDisabledFrames <= 0) {
		bool isLButtonCameraSpecifyMode = gpCamera->isLButtonCamera();
		if (isLButtonCameraSpecifyMode) {
			// This is for when in y camera
			mCompSPos[1 * 2]     = (f32)mButton.mAnalogL;
			mCompSPos[1 * 2 + 1] = (f32)mButton.mAnalogR;

			updateMeaning(R, MEANING_0x400, prevMeaning);
			updateMeaning(Z, MEANING_0x1000, prevMeaning);
			updateMeaning(L, MEANING_0x2000, prevMeaning);
			mCompSPos[2 * 2]     = mMainStick.mPosX;
			mCompSPos[2 * 2 + 1] = mMainStick.mPosY;
			updateMeaning(A, MEANING_0x10000, prevMeaning);
			updateMeaning(B, MEANING_0x10000, prevMeaning);
			updateMeaning(Y, MEANING_0x4000, prevMeaning);

			if (((_DE & 0x1) != 0)
			    || (((_DC & 1) != 0 && ((prevMeaning & MEANING_0x200) != 0)))) {
				mMeaning |= MEANING_0x200;
			}

			updateMeaning(X, MEANING_0x200000, prevMeaning);
		} else {
			f32 stickScaling = 1.0f;
			bool _unk3       = false;
			if (0 < _E4) {
				_E4 -= 1;
			}

			if (0 < _E4) {
				s16 _unk2 = 0x3d - _E4;
				_unk3     = true;
				if (_unk2 <= 0x28) {
					stickScaling = 0.0f;
				} else {
					stickScaling = CLBCalcRatio<s16>(0x28, 0x3c, _unk2);
				}
			}

			if (_unk3) {
				mCompSPos[0 * 2]     = stickScaling * mMainStick.mPosX;
				mCompSPos[0 * 2 + 1] = stickScaling * mMainStick.mPosY;
			} else {
				mCompSPos[0 * 2]     = mMainStick.mPosX;
				mCompSPos[0 * 2 + 1] = mMainStick.mPosY;
			}

			mCompSPos[1 * 2]     = (f32)mButton.mAnalogL;
			mCompSPos[1 * 2 + 1] = (f32)mButton.mAnalogR;
			updateMeaning(A, MEANING_0x80, prevMeaning);

			if ((mFlags & 0x4) != 0) {
				updateMeaning(B, MEANING_0x800, prevMeaning);
			} else {
				if ((mFlags & 0x20) == 0) {
					updateMeaning(B, MEANING_0x100, prevMeaning);
				}
			}

			updateMeaning(R, MEANING_0x400, prevMeaning);
			updateMeaning(Z, MEANING_0x1000, prevMeaning);
			updateMeaning(L, MEANING_0x2000, prevMeaning);

			mCompSPos[3 * 2]     = mSubStick.mPosX;
			mCompSPos[3 * 2 + 1] = mSubStick.mPosY;

			updateMeaning(Y, MEANING_0x4000, prevMeaning);
			updateMeaning(L, MEANING_0x8000, prevMeaning);

			if (((_DE & 0x1) != 0)
			    || (((_DC & 1) != 0 && ((prevMeaning & MEANING_0x200) != 0)))) {
				mMeaning |= MEANING_0x200;
			}

			updateMeaning(X, MEANING_0x200000, prevMeaning);
		}
	}

finalize:
	mEnabledFrameMeaning  = mMeaning & ~prevMeaning;
	mDisabledFrameMeaning = prevMeaning & ~mMeaning;
}

// UNUSED, map size 0x90; inlined away or dead. Its one `f32*` argument and
// its position between updateMeaning and onNeutralMarioKey are all the
// evidence there is.
// TODO: incorrect size (0x4 vs 0x90).
void TMarioGamePad::considerMarioStick(f32* stick) { }

void TMarioGamePad::onNeutralMarioKey() { _E4 = 0x3c; }

void TMarioGamePad::read()
{
	JUTGamePad::read();

	// Dead 4-byte local. Retail puts `resetPort` at 0xc(r1) and leaves the
	// word at 0x8 reserved below it, which only a local declared *after* it
	// can do (the named block fills bottom-up in reverse declaration order);
	// without it `resetPort` lands at 0x8 and every other instruction is
	// identical. It is never written, so the original name is unrecoverable.
	s32 resetPort = 0;
	s32 unusedPort;

	if (checkReset(&resetPort)) {
		handleReset(resetPort);
	}
}

// UNUSED, map size 0x38. The rumble API TMarioGamePad exposed; no caller
// survives in the image.
// TODO: incorrect size (0x4 vs 0x38).
void TMarioGamePad::rumble(TType type, u32 param_2) { }

// UNUSED, map size 0x48.
// TODO: incorrect size (0x4 vs 0x48).
void TMarioGamePad::keepRumble(TType type) { }
