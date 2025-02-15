#include "types.h"
#include <JSystem/JUtility/JUTGamePad.hpp>
#include <math.h>



static u32 channel_mask[4]
    = { PAD_CHAN0_BIT, PAD_CHAN1_BIT, PAD_CHAN2_BIT, PAD_CHAN3_BIT };

JSUList<JUTGamePad> JUTGamePad::mPadList(false);

bool JUTGamePad::mListInitialized;

u8 JUTGamePad::mPadAssign[4];

JUTGamePad::JUTGamePad(EPadPort port)
    : mRumble(this)
    , mLink(this)
{
	mPortNum = port;
	mPadAssign[port]++;

	initList();
	mPadList.append(&mLink);
	update();
	mPadRecord = 0;
	mPadReplay = 0;
}

JUTGamePad::~JUTGamePad()
{
	if (mPortNum != EPortInvalid) {
		mPadAssign[mPortNum]--;
		mPortNum = EPortInvalid;
	}

	mPadList.remove(&mLink);
}

void JUTGamePad::initList()
{
	if (!mListInitialized) {
		mPadList.initiate();
		mListInitialized = true;
	}
}

u32 JUTGamePad::mSuppressPadReset;

s32 JUTGamePad::sAnalogMode;

BOOL JUTGamePad::init()
{
	PADSetSpec(PAD_SPEC_5);
	setAnalogMode(3);
	return PADInit();
}

PADStatus JUTGamePad::mPadStatus[4];

JUTGamePad::CButton JUTGamePad::mPadButton[4];

JUTGamePad::CStick JUTGamePad::mPadMStick[4];

JUTGamePad::CStick JUTGamePad::mPadSStick[4];

JUTGamePad::EStickMode JUTGamePad::mStickMode = EStickMode1;

u32 JUTGamePad::read()
{
	PADRead(mPadStatus);
	PADClamp(mPadStatus);
	u32 reset_mask = 0;
	for (int i = 0; i < 4; i++) {
		u32 bittest = PAD_CHAN0_BIT >> i;

		if (mPadStatus[i].err == 0) {
			const PADStatus* status = &mPadStatus[i];
			u32 stick_status;
			stick_status = mPadMStick[i].update(status->stickX, status->stickY,
			                                    mStickMode, EMainStick)
			               << 0x18;
			stick_status
			    |= (mPadSStick[i].update(status->substickX, status->substickY,
			                             mStickMode, ESubStick)
			        << 0x10);

			mPadButton[i].update(status, stick_status);
		} else if (mPadStatus[i].err == -1) {
			mPadMStick[i].update(0, 0, mStickMode, EMainStick);
			mPadSStick[i].update(0, 0, mStickMode, ESubStick);
			mPadButton[i].update(nullptr, 0);

			if (!(mSuppressPadReset & bittest)) {
				reset_mask |= bittest;
			}
		} else {
			mPadButton[i].mTrigger = 0;
			mPadButton[i].mRelease = 0;
			mPadButton[i].mRepeat  = 0;
		}
	}

	for (JSUListIterator<JUTGamePad> pad(mPadList.getFirst());
	     pad != mPadList.getEnd(); pad++) {
		if (pad->getPadReplay() != nullptr) {
			u8* data = pad->getPadReplay()->read();
			if (!data) {
				pad->mMainStick.update(0, 0, mStickMode, EMainStick);
				pad->mSubStick.update(0, 0, mStickMode, ESubStick);
				pad->mButton.update(nullptr, 0);
			} else {
				PADStatus status;

				// NOTE: probably some kind of an insane memzero inline?
				char trash[0x4];
				{
					u8* b = (u8*)&status;
					for (int i = 0; i < 12; ++i)
						*b++ = 0;
				}

				pad->mPadReplay->streamDataToPadStatus(&status, data);

				u32 stick_status;
				stick_status
				    = pad->mMainStick.update(status.stickX, status.stickY,
				                             mStickMode, EMainStick)
				      << 0x18;
				stick_status
				    |= pad->mSubStick.update(status.substickX, status.substickY,
				                             mStickMode, ESubStick)
				       << 0x10;

				pad->mButton.update(&status, stick_status);
			}
		} else {
			if (pad->mPortNum == EPortInvalid) {
				pad->assign();
			}
			pad->update();
		}

		if (pad->getPadRecord()) {
			int port = pad->mPortNum;
			if (port != EPortInvalid && mPadStatus[port].err == 0) {
				pad->getPadRecord()->write(&mPadStatus[port]);
			}
		}
	}

	if (reset_mask != 0) {
		PADReset(reset_mask);
	}

	checkResetSwitch();
}

void JUTGamePad::assign()
{
	for (int i = 0; i < 4; i++) {
		if (mPadStatus[i].err == 0 && mPadAssign[i] == 0) {
			mPortNum      = i;
			mPadAssign[i] = 1;
			mPadButton[i].setRepeat(mButton.mRepeatMask, mButton.mRepeatDelay,
			                        mButton.mRepeatRate);
			mRumble.clear(this);
			break;
		}
	}
}

bool JUTGamePad::CRumble::mStatus[4];

u32 JUTGamePad::CRumble::mEnable;

callbackFn JUTGamePad::C3ButtonReset::sCallback;

void* JUTGamePad::C3ButtonReset::sCallbackArg;

OSTime JUTGamePad::C3ButtonReset::sThreshold
    = (OSTime)(OS_TIMER_CLOCK / 60) * 30;

bool JUTGamePad::C3ButtonReset::sResetSwitchPushing;

bool JUTGamePad::C3ButtonReset::sResetOccurred;

s32 JUTGamePad::C3ButtonReset::sResetOccurredPort;

void JUTGamePad::checkResetCallback(OSTime holdTime)
{
	if (holdTime >= JUTGamePad::C3ButtonReset::sThreshold) {
		JUTGamePad::C3ButtonReset::sResetOccurred     = true;
		JUTGamePad::C3ButtonReset::sResetOccurredPort = mPortNum;

		if (JUTGamePad::C3ButtonReset::sCallback != nullptr) {
			JUTGamePad::C3ButtonReset::sCallback(
			    mPortNum, JUTGamePad::C3ButtonReset::sCallbackArg);
		}
	}
}

u32 JUTGamePad::C3ButtonReset::sResetPattern
    = PAD_BUTTON_START | PAD_BUTTON_X | PAD_BUTTON_B;

void JUTGamePad::update()
{
	if (mPortNum == EPortInvalid)
		return;

	mButton      = mPadButton[mPortNum];
	mMainStick   = mPadMStick[mPortNum];
	mSubStick    = mPadSStick[mPortNum];
	mErrorStatus = mPadStatus[mPortNum].err;

	if (C3ButtonReset::sResetOccurred == 0) {
		if ((C3ButtonReset::sResetPattern & mButton.mButton)
		    == C3ButtonReset::sResetPattern) {
			if (mButtonReset.mReset == true) {
				OSTime hold_time = OSGetTime() - mResetHoldStartTime;
				checkResetCallback(hold_time);
			} else {
				mButtonReset.mReset = true;
				mResetHoldStartTime = OSGetTime();
			}
		} else {
			mButtonReset.mReset = false;
		}
	}

	mRumble.update(mPortNum);
}

void JUTGamePad::checkResetSwitch()
{
	if (!JUTGamePad::C3ButtonReset::sResetOccurred) {
		if (OSGetResetSwitchState()) {
			C3ButtonReset::sResetSwitchPushing = true;
		} else {
			if (C3ButtonReset::sResetSwitchPushing == true) {
				C3ButtonReset::sResetOccurred     = true;
				C3ButtonReset::sResetOccurredPort = EPortInvalid;

				if (C3ButtonReset::sCallback != nullptr) {
					C3ButtonReset::sCallback(EPortInvalid,
					                         C3ButtonReset::sCallbackArg);
				}
			}
			C3ButtonReset::sResetSwitchPushing = false;
		}
	}
}

void JUTGamePad::clearForReset()
{
	CRumble::setEnable(0);
	recalibrate(PAD_CHAN3_BIT | PAD_CHAN2_BIT | PAD_CHAN1_BIT | PAD_CHAN0_BIT);
}

void JUTGamePad::CButton::clear()
{
	mButton      = 0;
	mTrigger     = 0;
	mRelease     = 0;
	mRepeat      = 0;
	mAnalogA     = 0;
	mAnalogB     = 0;
	mAnalogL     = 0;
	mAnalogR     = 0;
	mRepeatCount = 0;
	mRepeatStart = 0;
	mRepeatMask  = 0;
	mRepeatDelay = 0;
	mRepeatRate  = 0;
}

void JUTGamePad::CButton::update(const PADStatus* padStatus, u32 stickStatus)
{
	u32 buttons;
	if (padStatus != nullptr) {
		buttons = padStatus->button;
	} else {
		buttons = 0;
	}

	buttons = stickStatus | buttons;
	mRepeat = 0;

	if (mRepeatDelay != 0 && mRepeatMask != 0) {
		u32 repeatButton = buttons & mRepeatMask;
		mRepeat          = 0;

		if (repeatButton == 0) {
			mRepeatStart = 0;
			mRepeatCount = 0;
		} else if (mRepeatStart == repeatButton) {
			mRepeatCount++;

			if (mRepeatCount == mRepeatDelay
			    || (mRepeatCount > mRepeatDelay
			        && (mRepeatCount - mRepeatDelay) % mRepeatRate == 0)) {
				mRepeat = repeatButton;
			}
		} else {
			mRepeat      = repeatButton & (mRepeatStart ^ 0xFFFFFFFF);
			mRepeatStart = repeatButton;
			mRepeatCount = 0;
		}
	}

	mTrigger = buttons & (buttons ^ mButton);
	mRelease = mButton & (buttons ^ mButton);
	mButton  = buttons;
	mRepeat |= (mRepeatMask ^ 0xFFFFFFFF) & mTrigger;

	if (padStatus != nullptr) {
		mAnalogA = padStatus->analogA;
		mAnalogB = padStatus->analogB;
		mAnalogL = padStatus->triggerLeft;
		mAnalogR = padStatus->triggerRight;
	} else {
		mAnalogA = 0;
		mAnalogB = 0;
		mAnalogL = 0;
		mAnalogR = 0;
	}

	mAnalogLf = (s32)mAnalogL / 150.0f;
	mAnalogRf = (s32)mAnalogR / 150.0f;
}

void JUTGamePad::CStick::clear()
{
	mPosX  = 0.0f;
	mPosY  = 0.0f;
	mValue = 0.0f;
	mAngle = 0;
}

u32 JUTGamePad::CStick::update(s8 x, s8 y, JUTGamePad::EStickMode mode,
                               JUTGamePad::EWhichStick stick)
{
	char trash[0x4];

	s32 clamp = stick == EMainStick ? 54 : 42;

	mPosX  = (f32)x / clamp;
	mPosY  = (f32)y / clamp;
	mValue = std::sqrtf((mPosX * mPosX) + (mPosY * mPosY));

	if (mValue > 1.0f) {
		if (mode == EStickMode1) {
			mPosX /= mValue;
			mPosY /= mValue;
		}
		mValue = 1.0f;
	}

	if (mValue > 0.0f)
		mAngle = 32768.0f * std::atan2f(mPosX, -mPosY) / 3.1415926;

	return getButton();
}

u32 JUTGamePad::CStick::getButton()
{
	u32 button = 0;

	if (-0.25f < mPosX && mPosX < 0.25f) {
		button = button & ~0x3;
	} else if (mPosX <= -0.5f) {
		button = button | 1;
	} else if (mPosX >= 0.5f) {
		button = button | 2;
	}

	if (-0.25f < mPosY && mPosY < 0.25f) {
		button = button & ~0xC;
	} else if (mPosY <= -0.5f) {
		button = button | 4;
	} else if (mPosY >= 0.5f) {
		button = button | 8;
	}

	return button;
}

void JUTGamePad::CRumble::clear()
{
	mFrame      = 0;
	mLength     = 0;
	mPattern    = nullptr;
	mFrameCount = 0;
	mEnable = (PAD_CHAN3_BIT | PAD_CHAN2_BIT | PAD_CHAN1_BIT | PAD_CHAN0_BIT);
}

void JUTGamePad::CRumble::clear(JUTGamePad* pad)
{
	if (pad->getPortNum() >= 0 && pad->getPortNum() < 4) {
		mStatus[pad->getPortNum()] = false;
		pad->stopMotorHard();
	}
	clear();
}

void JUTGamePad::CRumble::startMotor(int port)
{
	if (isEnabled(channel_mask[port])) {
		PADControlMotor(port, PAD_MOTOR_RUMBLE);
		mStatus[port] = true;
	}
}

void JUTGamePad::CRumble::stopMotor(int port)
{
	if (isEnabled(channel_mask[port])) {
		PADControlMotor(port, PAD_MOTOR_STOP);
		mStatus[port] = false;
	}
}

void JUTGamePad::CRumble::stopMotorHard(int port)
{
	if (isEnabled(channel_mask[port])) {
		PADControlMotor(port, PAD_MOTOR_STOP_HARD);
		mStatus[port] = false;
	}
}

static u8 getNumBit(u8* pattern, u32 index)
{
	return pattern[index >> 3] & (0x80 >> (index & 7));
}

void JUTGamePad::CRumble::update(s16 port)
{
	if (!isEnabled(channel_mask[port])) {
		mFrame      = 0;
		mLength     = 0;
		mPattern    = nullptr;
		mFrameCount = 0;
	}

	if (mLength == 0) {
		return;
	}

	if (mFrame >= mLength) {
		stopMotorHard(port);
		mLength = 0;
	} else if (mFrameCount == 0) {
		if (mStatus[port] == 0) {
			startMotor(port);
		}
		return;
	} else {
		u8 enabled = getNumBit(mPattern, mFrame % mFrameCount);

		if (enabled && !mStatus[port]) {
			startMotor(port);
		} else if (!enabled && mStatus[port]) {
			stopMotorHard(port);
		}
	}

	mFrame++;
}

JUTGamePad* JUTGamePad::getGamePad(int port)
{
	JSUListIterator<JUTGamePad> pad(mPadList.getFirst());

	for (; pad != mPadList.getEnd(); pad++) {
		if (port == pad->mPortNum) {
			return pad.getObject();
		}
	}

	return nullptr;
}

void JUTGamePad::CRumble::stopPatternedRumble(s16 port)
{
	mLength = 0;
	stopMotorHard(port);
}

void JUTGamePad::CRumble::setEnable(u32 mask)
{
	mask &= (PAD_CHAN3_BIT | PAD_CHAN2_BIT | PAD_CHAN1_BIT | PAD_CHAN0_BIT);

	for (int i = 0; i < 4; ++i) {
		if (mEnable & channel_mask[i])
			continue;

		if (mStatus[i])
			stopMotor(i);

		if (JUTGamePad* pad = getGamePad(i))
			pad->mRumble.stopPatternedRumble(pad->mPortNum);
	}

	mEnable = mask;
}

void JUTGamePad::CButton::setRepeat(u32 mask, u32 delay, u32 rate)
{
	mRepeatStart = 0;
	mRepeatCount = 0;
	mRepeatMask  = mask;
	mRepeatDelay = delay;
	mRepeatRate  = rate;
}

void JUTGamePad::setButtonRepeat(u32 mask, u32 delay, u32 rate)
{
	mButton.setRepeat(mask, delay, rate);

	if (mPortNum == -1)
		return;

	mPadButton[mPortNum].setRepeat(mask, delay, rate);
}

bool JUTGamePad::recalibrate(u32 mask)
{
	const u32 const_channel_mask[4]
	    = { PAD_CHAN0_BIT, PAD_CHAN1_BIT, PAD_CHAN2_BIT, PAD_CHAN3_BIT };

	if (mSuppressPadReset & const_channel_mask[0]) {
		mask &= const_channel_mask[0] ^ 0xFFFFFFFF;
	}
	if (mSuppressPadReset & const_channel_mask[1]) {
		mask &= const_channel_mask[1] ^ 0xFFFFFFFF;
	}
	if (mSuppressPadReset & const_channel_mask[2]) {
		mask &= const_channel_mask[2] ^ 0xFFFFFFFF;
	}
	if (mSuppressPadReset & const_channel_mask[3]) {
		mask &= const_channel_mask[3] ^ 0xFFFFFFFF;
	}

	return PADRecalibrate(mask);
}

u8* JUTGamePadRecord::read()
{
	u8* currPtr = mStream;
	if (currPtr < mStreamStart)
		return 0;

	if ((currPtr >= mStreamEnd)
	    || mRecordedStates <= (mStream - mStreamStart) / mStep)
		currPtr = 0;

	if (currPtr != 0) {
		step();
	}

	if ((currPtr >= mStreamEnd)
	    || mRecordedStates <= (mStream - mStreamStart) / mStep)
		unk14 = 1;

	return currPtr;
}

void JUTGamePadRecord::write(PADStatus* status)
{
	char trash[0x8];

	if (mStream < mStreamStart || mStream >= mStreamEnd)
		return;

	padStatusToStreamData(status, mStream);

	step();

	if (mStream >= mStreamEnd)
		unk14 = 1;

	if (mRecordedStates < (mStream - mStreamStart) / mStep) {
		mRecordedStates = (mStream - mStreamStart) / mStep;
	}
}

void JUTGamePadRecord::streamDataToPadStatus(PADStatus* status, u8* data)
{
	if (mStateFlags & 1) {
		status->button = *(u16*)data;
		data += 2;
	}
	if (mStateFlags & 2) {
		status->stickX = *data++;
		status->stickY = *data++;
	}
	if (mStateFlags & 4) {
		status->substickX = *data++;
		status->substickY = *data++;
	}
	if (mStateFlags & 8) {
		status->triggerLeft  = *data++;
		status->triggerRight = *data++;
	}
	if (mStateFlags & 0x10) {
		status->analogA = *data++;
		status->analogB = *data++;
	}
}

void JUTGamePadRecord::padStatusToStreamData(PADStatus* status, u8* data)
{
	if (mStateFlags & 1) {
		*(u16*)data = status->button;
		data += 2;
	}
	if (mStateFlags & 2) {
		data[0] = status->stickX;
		data[1] = status->stickY;
		data += 2;
	}
	if (mStateFlags & 4) {
		data[0] = status->substickX;
		data[1] = status->substickY;
		data += 2;
	}
	if (mStateFlags & 8) {
		data[0] = status->triggerLeft;
		data[1] = status->triggerRight;
		data += 2;
	}
	if (mStateFlags & 0x10) {
		data[0] = status->analogA;
		data[1] = status->analogB;
	}
}
