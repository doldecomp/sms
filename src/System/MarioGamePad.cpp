#include <System/MarioGamePad.hpp>
#include <System/Application.hpp>

u16 TMarioGamePad::mResetFlag = 0;

u32 TMarioGamePad::read()
{
	JUTGamePad::read();

	// TODO: I could not make the register check work properly here.
	s32 resetPort = 0;
	if (checkReset(&resetPort)) {
		handleReset(resetPort);
	}
}

void TMarioGamePad::onNeutralMarioKey() { _E4 = 0x3c; }

void TMarioGamePad::reset()
{
	setButtonRepeat(0xf00000f, 6.0f / SMSGetAnmFrameRate(),
	                20.0f / SMSGetAnmFrameRate());
	_E4             = 0;
	mDisabledFrames = 0;
}

void TMarioGamePad::updateMeaning()
{
	u32 stackAlloc[83]; // A lot of stack space still missing

	if (mDisabledFrames > 0) {
		mDisabledFrames -= 1;
	}

	u32 prevMeaning = resetMeaning();

	updateMeaning(START, MEANING_0x1, prevMeaning);

	if (checkFlag(PAD_FLAG_0x80)) {
		updateMeaning(A, MEANING_0x20, prevMeaning);
		updateMeaning(B, MEANING_0x40, prevMeaning);
		mCompSPos[4].x = mMainStick.mPosX;
		mCompSPos[4].y = mMainStick.mPosY;
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
		mCompSPos[1].x = (f32)(mButton.mAnalogL);
		mCompSPos[1].y = (f32)(mButton.mAnalogR);
		updateMeaning(R, MEANING_0x400, prevMeaning);
		updateMeaning(Z, MEANING_0x1000, prevMeaning);
		updateMeaning(L, MEANING_0x2000, prevMeaning);
		goto finalize;
	}

	if (checkFlag(PAD_FLAG_0x10)) {
		goto finalize;
	}

	if (mDisabledFrames <= 0) {
		bool isLButtonCameraSpecifyMode
		    = gpCamera->isLButtonCameraSpecifyMode(gpCamera->mMode);
		if (isLButtonCameraSpecifyMode) {
			// This is for when in y camera
			mCompSPos[1].x = (f32)mButton.mAnalogL;
			mCompSPos[1].y = (f32)mButton.mAnalogR;

			updateMeaning(R, MEANING_0x400, prevMeaning);
			updateMeaning(Z, MEANING_0x1000, prevMeaning);
			updateMeaning(L, MEANING_0x2000, prevMeaning);
			mCompSPos[2].x = mMainStick.mPosX;
			mCompSPos[2].y = mMainStick.mPosY;
			updateMeaning(A, MEANING_0x10000, prevMeaning);
			updateMeaning(B, MEANING_0x10000, prevMeaning);
			updateMeaning(Y, MEANING_0x4000, prevMeaning);

			if (((_DE & 0x1) != 0)
			    || (((_DC & 1) != 0 && ((prevMeaning & MEANING_0x200) != 0)))) {
				mMeaning |= MEANING_0x200;
			}

			updateMeaning(X, MEANING_0x200000, prevMeaning);
		} else {
			f32 stickScaling = 176.0f;
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
					// TODO: CLBCalcRatio
					// stickScaling =
					stickScaling = CLBCalcRatio<s16>(0x28, 0x3c, _unk2);
				}
			}

			if (_unk3) {
				mCompSPos[0].x = stickScaling * mMainStick.mPosX;
				mCompSPos[0].y = stickScaling * mMainStick.mPosY;
			} else {
				mCompSPos[0].x = mMainStick.mPosX;
				mCompSPos[0].y = mMainStick.mPosY;
			}

			mCompSPos[1].x = (f32)mButton.mAnalogL;
			mCompSPos[1].y = (f32)mButton.mAnalogR;

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

			mCompSPos[3].x = mSubStick.mPosX;
			mCompSPos[3].y = mSubStick.mPosY;

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
