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
	if (mPortNum >= 0) {
		mPadAssign[port]++;
	}

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
	u32 reset_mask = 0;
	u32 bittest;
	for (int i = 0; i < 4; i++) {
		bittest = PAD_CHAN0_BIT >> i;

		if (mPadStatus[i].err == 0) {
			u32 stick_status;
			stick_status = mPadMStick[i].update(mPadStatus[i].stickX,
			                                    mPadStatus[i].stickY,
			                                    mStickMode, EMainStick)
			               << 0x18;
			stick_status |= (mPadSStick[i].update(mPadStatus[i].substickX,
			                                      mPadStatus[i].substickY,
			                                      mStickMode, ESubStick)
			                 << 0x10);

			mPadButton[i].update(&mPadStatus[i], stick_status);
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

	u32 unkR30 = 12;
	for (JSUListIterator<JUTGamePad> pad(mPadList.getFirst());
	     pad != mPadList.getEnd(); pad++) {
		if (pad->getPadReplay() != nullptr) {
			if (!pad->getPadReplay()->isActive()) {
				pad->mMainStick.update(0, 0, mStickMode, EMainStick);
				pad->mSubStick.update(0, 0, mStickMode, ESubStick);
				pad->mButton.update(nullptr, 0);
			} else {
				u32 dataSize = 0;
				u8 data[0x120];
				u8* dataPtr = data;
				if (unkR30 < 8) {
					++dataSize;
					data[0] = 0;
					data[1] = 0;
					data[2] = 0;
					data[3] = 0;
					data[4] = 0;
					data[5] = 0;
					data[6] = 0;
					data[7] = 0;
					dataPtr += 8;
				}

				while (dataSize < 12)
					*dataPtr++ = 0, ++dataSize;

				PADStatus status;

				((JUTGamePadRecord*)pad->mPadRecord)
				    ->streamDataToPadStatus(&status, data);

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

	if (C3ButtonReset::sResetOccurred == 0
	    && (C3ButtonReset::sResetPattern & mButton.mButton)
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
	s32 clamp = stick == EMainStick ? 54 : 42;

	mRawX  = x;
	mRawY  = y;
	mPosX  = (f32)x / (f32)clamp;
	mPosY  = (f32)y / (f32)clamp;
	mValue = std::sqrtf((mPosX * mPosX) + (mPosY * mPosY));

	if (mValue > 1.0f) {
		if (mode == EStickMode1) {
			mPosX /= mValue;
			mPosY /= mValue;
		}
		mValue = 1.0f;
	}

	if (mValue > 0.0f) {
		if (mPosY == 0.0f) {
			if (mPosX > 0.0f) {
				mAngle = 0x4000;
			} else {
				mAngle = -0x4000;
			}
		} else {
			mAngle = (0x8000 / 3.1415926f) * std::atan2f(mPosX, -mPosY);
		}
	}

	u32 button_shift = stick == EMainStick ? 0x18 : 0x10;
	return getButton();
}

u32 JUTGamePad::CStick::getButton()
{
	u32 buttons; // was mPadButton[i].mButton?
	u32 button = buttons & 0xF;

	if (-0.25f < mPosX && mPosX < 0.25f) {
		button = button & ~0x3;
	} else if (mPosX <= -0.5f) {
		button = (button & ~0x2) | 1;
	} else if (mPosX >= 0.5f) {
		button = (button & ~0x1) | 2;
	}

	if (-0.25f < mPosY && mPosY < 0.25f) {
		button = button & ~0xC;
	} else if (mPosY <= -0.5f) {
		button = (button & ~0x8) | 4;
	} else if (mPosY >= 0.5f) {
		button = (button & ~0x4) | 8;
	}

	return button;
}

void JUTGamePad::CRumble::clear()
{
	mFrame      = 0;
	mLength     = 0;
	mPattern    = nullptr;
	mFrameCount = 0;
	unk10       = 0;
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
	if (isEnabledPort(port)) {
		PADControlMotor(port, PAD_MOTOR_RUMBLE);
		mStatus[port] = true;
	}
}

void JUTGamePad::CRumble::stopMotor(int port)
{
	if (isEnabledPort(port)) {
		PADControlMotor(port, PAD_MOTOR_STOP);
		mStatus[port] = false;
	}
}

void JUTGamePad::CRumble::stopMotorHard(int port)
{
	if (isEnabledPort(port)) {
		PADControlMotor(port, PAD_MOTOR_STOP_HARD);
		mStatus[port] = false;
	}
}

static bool getNumBit(u8* pattern, int index)
{
	u8 bit = pattern[index >> 3] & (0x80 >> (index & 7));
	return bit != 0;
}

void JUTGamePad::CRumble::update(s16 port)
{
	if (isEnabledPort(port) == false) {
		mFrame      = 0;
		mLength     = 0;
		mPattern    = nullptr;
		mFrameCount = 0;
		unk10       = 0;
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
		bool enabled = getNumBit(mPattern, mFrame % mFrameCount);
		u8 status    = mStatus[port] != false;

		if (enabled && !status) {
			startMotor(port);
		} else if (!enabled) {
			bool hard_stop = false;
			// if (field_0x10) {
			// 	hard_stop = getNumBit(field_0x10, mFrame % mFrameCount);
			// }

			if (status) {
				stopMotor(port);
			} else if (hard_stop) {
				stopMotorHard(port);
			}
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

void JUTGamePad::CRumble::setEnable(u32 mask)
{
	for (int i = 0; i < 4; i++) {
		if ((mEnable & channel_mask[i]) == 0) {
			if (mStatus[i]) {
				stopMotor(i);
			}

			JUTGamePad* pad = getGamePad(i);
			if (pad != nullptr) {
				pad->mRumble.stopPatternedRumble(pad->mPortNum);
			}
		}
	}

	mEnable
	    = (mask
	       & (PAD_CHAN3_BIT | PAD_CHAN2_BIT | PAD_CHAN1_BIT | PAD_CHAN0_BIT));
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
	mButton.mRepeatStart = 0;
	mButton.mRepeatCount = 0;
	mButton.mRepeatMask  = mask;
	mButton.mRepeatDelay = delay;
	mButton.mRepeatRate  = rate;

	s16 unk10 = mRumble.unk10;
	if (unk10 == -1)
		return;

	mPadButton[unk10].setRepeat(mask, delay, rate);
}

bool JUTGamePad::recalibrate(u32 mask)
{
	for (int i = 0; i < 4; i++) {
		if (mSuppressPadReset & channel_mask[i]) {
			mask &= channel_mask[i] ^ 0xFFFFFFFF;
		}
	}

	return PADRecalibrate(mask);
}

u8* JUTGamePadRecord::read()
{
	u8* uVar5;

	uVar5 = mStream;
	if (uVar5 < mStreamStart)
		return 0;

	if ((uVar5 >= mStreamEnd) || unkC <= (mStream - mStreamStart) / unk18) {
		uVar5 = 0;
	}

	if (uVar5 != 0) {
		u8* uVar1 = mStream + unk18;
		if (mStream + unk18 < mStreamStart) {
			uVar1 = mStreamStart;
		}
		if (uVar1 > mStreamEnd) {
			uVar1 = mStreamEnd;
		}
		mStream = uVar1;
	}

	if ((uVar5 >= mStreamEnd) || unkC <= (mStream - mStreamStart) / unk18) {
		unk14 = 1;
	}

	return uVar5;
}

void JUTGamePadRecord::write(PADStatus* status)
{
	if (mStream < mStreamStart || mStream >= mStreamEnd)
		return;

	padStatusToStreamData(status, mStream);

	u8* uVar1 = mStream + unk18;
	if (uVar1 < mStreamStart)
		uVar1 = mStreamStart;

	if (mStreamEnd > uVar1)
		uVar1 = mStreamEnd;

	mStream = uVar1;

	if (mStream >= mStreamEnd)
		unk14 = 1;

	u32 uVar2 = (mStream - mStreamStart) / unk18;
	if (unkC < uVar2) {
		unkC = uVar2;
	}
}

void JUTGamePadRecord::streamDataToPadStatus(PADStatus* status, u8* data)
{
	if (unk15 & 1) {
		status->button = *(u16*)data;
		data += 2;
	}
	if (unk15 & 2) {
		status->stickX = data[0];
		status->stickY = data[1];
		data += 2;
	}
	if (unk15 & 4) {
		status->substickX = data[0];
		status->substickY = data[1];
		data += 2;
	}
	if (unk15 & 8) {
		status->triggerLeft  = data[0];
		status->triggerRight = data[1];
		data += 2;
	}
	if (unk15 & 0x10) {
		status->analogA = data[0];
		status->analogB = data[1];
	}
}

void JUTGamePadRecord::padStatusToStreamData(PADStatus* status, u8* data)
{
	if (unk15 & 1) {
		*(u16*)data = status->button;
		data += 2;
	}
	if (unk15 & 2) {
		data[0] = status->stickX;
		data[1] = status->stickY;
		data += 2;
	}
	if (unk15 & 4) {
		data[0] = status->substickX;
		data[1] = status->substickY;
		data += 2;
	}
	if (unk15 & 8) {
		data[0] = status->triggerLeft;
		data[1] = status->triggerRight;
		data += 2;
	}
	if (unk15 & 0x10) {
		data[0] = status->analogA;
		data[1] = status->analogB;
	}
}
