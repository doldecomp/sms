#include <System/MarioGamePad.hpp>
#include <System/Application.hpp>

JDrama::TFlagT<u16> TMarioGamePad::mResetFlag;

void TMarioGamePad::reset()
{
	setButtonRepeat(0xf00000f, 20.0f / SMSGetAnmFrameRate(),
	                6.0f / SMSGetAnmFrameRate());
	mNeutralKeyTimer = 0;
	mDisabledFrames  = 0;
}

void TMarioGamePad::updateMeaning()
{
	if (mDisabledFrames > 0)
		mDisabledFrames -= 1;

	u32 prevMeaning = resetMeaning();

	updateMeaning(START, MEANING_START, prevMeaning);

	if (checkFlag(PAD_FLAG_GUIDE_INPUT)) {
		updateMeaning(A, MEANING_MENU_A, prevMeaning);
		updateMeaning(B, MEANING_MENU_B, prevMeaning);
		mCompSPos[4 * 2]     = mMainStick.mPosX;
		mCompSPos[4 * 2 + 1] = mMainStick.mPosY;
		goto finalize;
	}

	if (checkFlag(PAD_FLAG_MENU_INPUT)) {
		// Some kind of 2d menu navigation, i think maybe for debug menu
		if (mButton.mRepeat & (MAINSTICK_UP | DPAD_UP))
			mMeaning |= MEANING_MENU_UP;
		if (mButton.mRepeat & (MAINSTICK_DOWN | DPAD_DOWN))
			mMeaning |= MEANING_MENU_DOWN;
		if (mButton.mRepeat & (MAINSTICK_LEFT | DPAD_LEFT))
			mMeaning |= MEANING_MENU_LEFT;
		if (mButton.mRepeat & (MAINSTICK_RIGHT | DPAD_RIGHT))
			mMeaning |= MEANING_MENU_RIGHT;

		updateMeaning(A, MEANING_MENU_A, prevMeaning);
		updateMeaning(B, MEANING_MENU_B, prevMeaning);
		goto finalize;
	}

	if (!checkFlag(PAD_FLAG_GAME_INPUT))
		goto finalize;

	if (checkFlag(PAD_FLAG_TALK_SELECT)) {
		// Menu navigation
		if (mButton.mRepeat & (MAINSTICK_UP | DPAD_UP))
			mMeaning |= MEANING_SELECT_UP;
		if (mButton.mRepeat & (MAINSTICK_DOWN | DPAD_DOWN))
			mMeaning |= MEANING_SELECT_DOWN;

		updateMeaning(A, MEANING_SELECT_A, prevMeaning);
		updateMeaning(B, MEANING_SELECT_B, prevMeaning);
		mCompSPos[1 * 2]     = (f32)(mButton.mAnalogL);
		mCompSPos[1 * 2 + 1] = (f32)(mButton.mAnalogR);
		updateMeaning(R, MEANING_R, prevMeaning);
		updateMeaning(Z, MEANING_Z, prevMeaning);
		updateMeaning(L, MEANING_L, prevMeaning);
		goto finalize;
	}

	if (checkFlag(PAD_FLAG_NO_INPUT))
		goto finalize;

	if (mDisabledFrames <= 0) {
		bool isLButtonCameraSpecifyMode = gpCamera->isLButtonCamera();
		if (isLButtonCameraSpecifyMode) {
			// This is for when in y camera
			mCompSPos[1 * 2]     = (f32)mButton.mAnalogL;
			mCompSPos[1 * 2 + 1] = (f32)mButton.mAnalogR;

			updateMeaning(R, MEANING_R, prevMeaning);
			updateMeaning(Z, MEANING_Z, prevMeaning);
			updateMeaning(L, MEANING_L, prevMeaning);
			mCompSPos[2 * 2]     = mMainStick.mPosX;
			mCompSPos[2 * 2 + 1] = mMainStick.mPosY;
			updateMeaning(A, MEANING_CAM_AB, prevMeaning);
			updateMeaning(B, MEANING_CAM_AB, prevMeaning);
			updateMeaning(Y, MEANING_Y, prevMeaning);

			if (((_DE & 0x1) != 0)
			    || (((_DC & 1) != 0
			         && ((prevMeaning & MEANING_UNK200) != 0)))) {
				mMeaning |= MEANING_UNK200;
			}

			updateMeaning(X, MEANING_X, prevMeaning);
		} else {
			considerMarioStick(&mCompSPos[0 * 2]);

			mCompSPos[1 * 2]     = (f32)mButton.mAnalogL;
			mCompSPos[1 * 2 + 1] = (f32)mButton.mAnalogR;
			updateMeaning(A, MEANING_A, prevMeaning);

			if (checkFlag(PAD_FLAG_TALK_NPC))
				updateMeaning(B, MEANING_TALK_B, prevMeaning);
			else if (!checkFlag(PAD_FLAG_NO_B))
				updateMeaning(B, MEANING_B, prevMeaning);

			updateMeaning(R, MEANING_R, prevMeaning);
			updateMeaning(Z, MEANING_Z, prevMeaning);
			updateMeaning(L, MEANING_L, prevMeaning);

			mCompSPos[3 * 2]     = mSubStick.mPosX;
			mCompSPos[3 * 2 + 1] = mSubStick.mPosY;

			updateMeaning(Y, MEANING_Y, prevMeaning);
			updateMeaning(L, MEANING_CAM_L, prevMeaning);

			if (((_DE & 0x1) != 0)
			    || (((_DC & 1) != 0
			         && ((prevMeaning & MEANING_UNK200) != 0)))) {
				mMeaning |= MEANING_UNK200;
			}

			updateMeaning(X, MEANING_X, prevMeaning);
		}
	}

finalize:
	mEnabledFrameMeaning  = mMeaning & ~prevMeaning;
	mDisabledFrameMeaning = prevMeaning & ~mMeaning;
}

void TMarioGamePad::considerMarioStick(f32* stick)
{
	f32 stickScaling = 1.0f;
	bool isScaled    = false;

	if (0 < mNeutralKeyTimer)
		mNeutralKeyTimer -= 1;

	if (0 < mNeutralKeyTimer) {
		s16 frame = 0x3d - mNeutralKeyTimer;
		isScaled  = true;
		stickScaling
		    = frame <= 0x28 ? 0.0f : CLBCalcRatio<s16>(0x28, 0x3c, frame);
	}

	if (isScaled) {
		stick[0] = stickScaling * mMainStick.mPosX;
		stick[1] = stickScaling * mMainStick.mPosY;
	} else {
		stick[0] = mMainStick.mPosX;
		stick[1] = mMainStick.mPosY;
	}
}

void TMarioGamePad::onNeutralMarioKey() { mNeutralKeyTimer = 0x3c; }

void TMarioGamePad::read()
{
	JUTGamePad::read();

	s32 resetPort = 0;
	char trash[4];
	if (checkReset(&resetPort))
		handleReset(resetPort);
}

void TMarioGamePad::rumble(TType type, u32 length) { }

void TMarioGamePad::keepRumble(TType type) { }
