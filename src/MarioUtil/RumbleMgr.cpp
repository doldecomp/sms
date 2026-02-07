#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <System/Application.hpp>
#include <Player/MarioAccess.hpp>

int RumbleMgr::mMotorCountLimit  = 900;
u16 RumbleMgr::mMotorTimerPeriod = 80;
f32 RumbleMgr::mPowerThreshold   = 0.5f;

RumbleMgr* SMSRumbleMgr = nullptr;

// Size needed: 0x10, current: 0x10
void RumbleBase::init()
{
	mPower       = 0.0f;
	mAccumulator = 0.0f;
}

// Size needed: 0x28, current: 0x28
void RumbleBase::stop() { PADControlMotor(mChan, PAD_MOTOR_STOP_HARD); }

// Size needed: 0xAC, current: 0xA0
void RumbleBase::update(f32 power, bool stopHard)
{
	// This function is the likely cause for stack mismatch in
	// RumbleMgr::update() as it's the only one whose size mismatches.
	mPower = power;
	mAccumulator += mPower;

	if (mPower <= 0.0f) {
		PADControlMotor(mChan, PAD_MOTOR_STOP_HARD);
	} else if (mAccumulator < 1.0f) {
		if (stopHard) {
			PADControlMotor(mChan, PAD_MOTOR_STOP_HARD);
		} else {
			PADControlMotor(mChan, PAD_MOTOR_STOP);
		}
	} else {
		// @fake - this doesn't fix our function size but it does match stack in
		// RumbleMgr::update()
		// f32 accumulator = mAccumulator;
		// mAccumulator    = accumulator - 1.0f;
		mAccumulator -= 1.0f;
		PADControlMotor(mChan, PAD_MOTOR_RUMBLE);
	}
}

// Size needed: 0x10, current: 0x10
void RumbleChannelDataMgr::init() { mChannelDataTbl = channelDataTbl; }

// Size needed: 0x14, current: 0x14
RumbleChannelDataTbl* RumbleChannelDataMgr::getChannelDataTbl(int index)
{
	// This feels wrong, but I only see direct loads from channelDataTbl.
	// Also, it only matches size when doing this.
	return &channelDataTbl[index];
}

RumbleChannelMgr::RumbleChannelMgr()
{
	mElapsedTime    = 0.0f;
	mPower          = 0.0f;
	mChannelDataIdx = -1;
	mRepeatCount    = 0;
	mMultiplierF    = nullptr;
	mWorldPos       = nullptr;
	mChannelDataTbl = nullptr;
}

// Size needed: 0x30, current: 0x2C
void RumbleChannelMgr::init(RumbleChannelDataMgr* dataMgr)
{
	mElapsedTime    = 0.0f;
	mPower          = 0.0f;
	mChannelDataIdx = -1;
	mRepeatCount    = 0;
	mMultiplierF    = nullptr;
	mWorldPos       = nullptr;
	mChannelDataTbl = nullptr;
	mChannelDataMgr = dataMgr;
}

// Size needed: 0x2C, current: 0x2C
void RumbleChannelMgr::reset()
{
	mElapsedTime    = 0.0f;
	mPower          = 0.0f;
	mChannelDataIdx = -1;
	mRepeatCount    = 0;
	mMultiplierF    = nullptr;
	mWorldPos       = nullptr;
	mChannelDataTbl = nullptr;
}

// Size needed: 0x0C, current: 0x0C
void RumbleChannelMgr::repeat() { mElapsedTime = 0.0f; }

// Size needed: 0x2C, current: 0x2C
void RumbleChannelMgr::start(int channelDataIdx, int repeatCount,
                             f32* multiplierF)
{
	mElapsedTime    = 0.0f;
	mChannelDataTbl = mChannelDataMgr->getChannelDataTbl(channelDataIdx);
	mChannelDataIdx = channelDataIdx;
	mRepeatCount    = repeatCount;
	mMultiplierF    = multiplierF;
}

// Size needed: 0x2C, current: 0x2C
void RumbleChannelMgr::start(int channelDataIdx, int repeatCount, Vec* worldPos)
{
	mElapsedTime    = 0.0f;
	mChannelDataTbl = mChannelDataMgr->getChannelDataTbl(channelDataIdx);
	mChannelDataIdx = channelDataIdx;
	mRepeatCount    = repeatCount;
	mWorldPos       = worldPos;
}

f32 RumbleChannelMgr::update()
{
	f32 updateRate = 1.0f / SMSGetVSyncTimesPerSec();
	f32 mult;

	if (mMultiplierF != nullptr) {
		mult = *mMultiplierF;
		if (mult < 0.0f) {
			mult = 0.0f;
		} else if (mult > 1.0f) {
			mult = 1.0f;
		}
	} else if (mWorldPos != nullptr) {
		JGeometry::TVec3<f32>& v
		    = *static_cast<JGeometry::TVec3<f32>*>(mWorldPos);
		f32 dist = SMS_DistanceFromMario(v);
		mult     = -((dist - 300.0f) * (1.0f / 3000.0f) - 1.0f);
		if (mult < 0.0f) {
			mult = 0.0f;
		} else if (mult > 1.0f) {
			mult = 1.0f;
		}
	} else {
		mult = 1.0f;
	}

	mPower = 0.0f;
	if (mRepeatCount == 0) {
		reset();
	}

	if (mChannelDataTbl != nullptr) {
		mElapsedTime += updateRate;

		for (int i = 0; i < *mChannelDataTbl->mPointData - 1; i++) {
			if (mElapsedTime > mChannelDataTbl->mFrameData[i]) {
				f32 delta = mChannelDataTbl->mFrameData[i + 1]
				            - mChannelDataTbl->mFrameData[i];
				f32 frameDelta = mElapsedTime - mChannelDataTbl->mFrameData[i];
				f32 progress;

				if (delta > 0.0f) {
					progress = frameDelta / delta;
				} else {
					progress = 1.0f;
				}

				mPower = (1.0f - progress) * mChannelDataTbl->mPowerData[i]
				         + progress * mChannelDataTbl->mPowerData[i + 1];
			}
		}

		if (mElapsedTime
		    > mChannelDataTbl->mFrameData[*mChannelDataTbl->mPointData - 1]) {
			if (mRepeatCount > 1) {
				repeat();
				mRepeatCount--;
			} else if (mRepeatCount < 0) {
				repeat();
			} else {
				reset();
			}
		}
	}

	mPower *= mult;
	return mPower;
}

// Size needed: 0x58, current: 0x58
RumbleControllerMgr::RumbleControllerMgr()
{
	mPower         = 0.0f;
	mChannelMgrTbl = new RumbleChannelMgr[RUMBLE_CHANNELS_PER_CONTROLLER];
}

// Size needed: 0x1C4, current: 0x19C
void RumbleControllerMgr::init()
{
	mPower = 0.0f;

	for (int i = 0; i < RUMBLE_CHANNELS_PER_CONTROLLER; i++) {
		mChannelMgrTbl[i].init(mChannelDataMgr);
	}
}

void RumbleControllerMgr::reset()
{
	mPower = 0.0f;

	for (int i = 0; i < RUMBLE_CHANNELS_PER_CONTROLLER; i++) {
		mChannelMgrTbl[i].reset();
	}

	mMotorCounter = 0;
	mMotorTimer   = 0;
	unk12         = 0;
}

// Hmm... why does this inline in RumbleMgr::start(int, f32*)?
#pragma dont_inline on
void RumbleControllerMgr::start(int channelDataIdx, int repeatCount,
                                f32* multiplierF)
{
	for (int i = 0; i < RUMBLE_CHANNELS_PER_CONTROLLER; i++) {
		if (mChannelMgrTbl[i].mChannelDataTbl == nullptr) {
			mChannelMgrTbl[i].start(channelDataIdx, repeatCount, multiplierF);
			break;
		}
	}
}
#pragma dont_inline reset

// This one also inlines in RumbleMgr::start(int, Vec*)?
#pragma dont_inline on
void RumbleControllerMgr::start(int channelDataIdx, int repeatCount,
                                Vec* worldPos)
{
	for (int i = 0; i < RUMBLE_CHANNELS_PER_CONTROLLER; i++) {
		if (mChannelMgrTbl[i].mChannelDataTbl == nullptr) {
			mChannelMgrTbl[i].start(channelDataIdx, repeatCount, worldPos);
			break;
		}
	}
}
#pragma dont_inline reset

// Size needed: 0x10C, current: 0x10C
void RumbleControllerMgr::stop()
{
	for (int i = 0; i < RUMBLE_CHANNELS_PER_CONTROLLER; i++) {
		if (mChannelMgrTbl[i].mChannelDataTbl != nullptr) {
			mChannelMgrTbl[i].reset();
		}
	}
}

// This one also inlines in RumbleMgr::stop(int)?
#pragma dont_inline on
void RumbleControllerMgr::stop(int channelDataIdx)
{
	for (int i = 0; i < RUMBLE_CHANNELS_PER_CONTROLLER; i++) {
		if (mChannelMgrTbl[i].mChannelDataTbl != nullptr
		    && mChannelMgrTbl[i].mChannelDataIdx == channelDataIdx) {
			mChannelMgrTbl[i].reset();
		}
	}
}
#pragma dont_inline reset

// Size needed: 0x11C, current: 0x11C
bool RumbleControllerMgr::channelMgrIsAllFree()
{
	bool allFree = true;
	for (int i = 0; i < RUMBLE_CHANNELS_PER_CONTROLLER; i++) {
		if (mChannelMgrTbl[i].mChannelDataTbl != nullptr) {
			allFree = false;
		}
	}
	return allFree;
}

void RumbleControllerMgr::updateMotorCount()
{
	if (unk12) {
		if (mMotorCounter > 0) {
			mMotorCounter--;
		} else {
			unk12       = 0;
			mMotorTimer = 0;
		}
	} else if (mMotorCounter > RumbleMgr::mMotorCountLimit) {
		unk12 = 1;
	} else if (mMotorTimer > 0) {
		mMotorTimer--;
		mMotorCounter++;
	} else {
		mMotorCounter = 0;
	}
}

f32 RumbleControllerMgr::update()
{
	f32 maxPower = 0.0f;
	for (int i = 0; i < RUMBLE_CHANNELS_PER_CONTROLLER; i++) {
		if (mChannelMgrTbl[i].mChannelDataTbl != nullptr) {
			mPower = mChannelMgrTbl[i].update();
			if (mPower > maxPower) {
				maxPower = mPower;
			}
		}
	}

	mPower = maxPower;
	if (mPower > 1.0f) {
		mPower = 1.0f;
	} else if (mPower < 0.0f) {
		mPower = 0.0f;
	}

	if (mPower > RumbleMgr::mPowerThreshold) {
		mMotorTimer = RumbleMgr::mMotorTimerPeriod;
	}

	if (unk12) {
		mPower = 0.0f;
	}

	return mPower;
}

RumbleMgr::RumbleMgr(bool controller1, bool controller2, bool controller3,
                     bool controller4)
{
	bool enable[4];

	mPower = 0.0f;
	mTimer = 0.0f;

	enable[0] = controller1;
	enable[1] = controller2;
	enable[2] = controller3;
	enable[3] = controller4;

	for (int i = 0; i < PAD_MAX_CONTROLLERS; i++) {
		mControllerMgrTbl[i] = nullptr;
		mRumbleBaseTbl[i]    = nullptr;

		if (enable[i]) {
			mControllerMgrTbl[i] = new RumbleControllerMgr();
			mRumbleBaseTbl[i]    = new RumbleBase(i);
		}
	}

	mChannelDataMgr = nullptr;
	mChannelDataMgr = new RumbleChannelDataMgr();
	mActive         = true;
	mPaused         = false;
	mStopHard       = true;
}

void RumbleMgr::init()
{
	mPower = 0.0f;
	mTimer = 0.0f;

	for (int i = 0; i < PAD_MAX_CONTROLLERS; i++) {
		if (mControllerMgrTbl[i] != nullptr) {
			mControllerMgrTbl[i]->init();
			mRumbleBaseTbl[i]->init();
		}
	}

	mChannelDataMgr->init();
	mActive   = true;
	mPaused   = false;
	mStopHard = true;
}

void RumbleMgr::reset()
{
	mPower = 0.0f;
	mTimer = 0.0f;

	for (int i = 0; i < PAD_MAX_CONTROLLERS; i++) {
		if (mControllerMgrTbl[i] != nullptr) {
			mControllerMgrTbl[i]->reset();
			mRumbleBaseTbl[i]->stop();
			mRumbleBaseTbl[i]->init();
		}
	}

	mPaused = 0;
}

void RumbleMgr::start(int channelDataIdx, f32* multiplierF)
{
	int controllerIdx = 0;

	if (!mPaused && mActive && mControllerMgrTbl[controllerIdx] != nullptr) {
		mControllerMgrTbl[controllerIdx]->start(channelDataIdx, 1, multiplierF);
	}
}

void RumbleMgr::start(int channelDataIdx, Vec* worldPos)
{
	int controllerIdx = 0;

	if (!mPaused && mActive && mControllerMgrTbl[controllerIdx] != nullptr) {
		mControllerMgrTbl[controllerIdx]->start(channelDataIdx, 1, worldPos);
	}
}

// Size needed: 0x74, current: 0x74
void RumbleMgr::start(int channelDataIdx, MtxPtr mtx)
{
	int controllerIdx = 0;

	if (!mPaused && mActive && mControllerMgrTbl[controllerIdx] != nullptr) {
		Vec worldPos;
		worldPos.x = mtx[3][0];
		worldPos.y = mtx[3][1];
		worldPos.z = mtx[3][2];

		mControllerMgrTbl[controllerIdx]->start(channelDataIdx, 1, &worldPos);
	}
}

void RumbleMgr::start(int channelDataIdx, int repeatCount, f32* multiplierF)
{
	int controllerIdx = 0;

	if (!mPaused && mActive && mControllerMgrTbl[controllerIdx] != nullptr) {
		mControllerMgrTbl[controllerIdx]->start(channelDataIdx, repeatCount,
		                                        multiplierF);
	}
}

void RumbleMgr::start(int channelDataIdx, int repeatCount, Vec* worldPos)
{
	int controllerIdx = 0;

	if (!mPaused && mActive && mControllerMgrTbl[controllerIdx] != nullptr) {
		mControllerMgrTbl[controllerIdx]->start(channelDataIdx, repeatCount,
		                                        worldPos);
	}
}

// Size needed: 0x70, current: 0x70
void RumbleMgr::start(int channelDataIdx, int repeatCount, MtxPtr mtx)
{
	int controllerIdx = 0;

	if (!mPaused && mActive && mControllerMgrTbl[controllerIdx] != nullptr) {
		Vec worldPos;
		worldPos.x = mtx[3][0];
		worldPos.y = mtx[3][1];
		worldPos.z = mtx[3][2];

		mControllerMgrTbl[controllerIdx]->start(channelDataIdx, repeatCount,
		                                        &worldPos);
	}
}

// Size needed: 0x94, current: 0x58
void RumbleMgr::start(int controllerIdx, int channelDataIdx, int repeatCount,
                      f32* multiplierF)
{
	if (!mPaused && mActive && mControllerMgrTbl[controllerIdx] != nullptr) {
		mControllerMgrTbl[controllerIdx]->start(channelDataIdx, repeatCount,
		                                        multiplierF);
	}
}

// Size needed: 0x94, current: 0x58
void RumbleMgr::start(int controllerIdx, int channelDataIdx, int repeatCount,
                      Vec* worldPos)
{
	if (!mPaused && mActive && mControllerMgrTbl[controllerIdx] != nullptr) {
		mControllerMgrTbl[controllerIdx]->start(channelDataIdx, repeatCount,
		                                        worldPos);
	}
}

// Size needed: 0x7C, current: 0x74
void RumbleMgr::start(int controllerIdx, int channelDataIdx, int repeatCount,
                      MtxPtr mtx)
{
	if (!mPaused && mActive && mControllerMgrTbl[controllerIdx] != nullptr) {
		Vec worldPos;
		worldPos.x = mtx[3][0];
		worldPos.y = mtx[3][1];
		worldPos.z = mtx[3][2];

		mControllerMgrTbl[controllerIdx]->start(channelDataIdx, repeatCount,
		                                        &worldPos);
	}
}

// @stack
void RumbleMgr::stop()
{
	// @fake - this fixes our stack mismatch, perhaps a leftover debug print?
	int stack[2];
	(void)stack;

	for (int i = 0; i < PAD_MAX_CONTROLLERS; i++) {
		if (mControllerMgrTbl[i] != nullptr) {
			mControllerMgrTbl[i]->stop();
		}
	}
}

void RumbleMgr::stop(int channelDataIdx)
{
	int controllerIdx = 0;

	if (!mPaused && mActive && mControllerMgrTbl[controllerIdx] != nullptr) {
		mControllerMgrTbl[controllerIdx]->stop(channelDataIdx);
	}
}

// Size needed: 0x178, current: 0x50
void RumbleMgr::stop(int controllerIdx, int channelDataIdx)
{
	if (!mPaused && mActive && mControllerMgrTbl[controllerIdx] != nullptr) {
		mControllerMgrTbl[controllerIdx]->stop(channelDataIdx);
	}
}

// @stack
void RumbleMgr::update()
{
	// @fake - this fixes our stack mismatch, perhaps a leftover debug print?
	// NOTE: same hack fixes RumbleMgr::stop()
	int stack[2];
	(void)stack;

	f32 updateRate = 1.0f / SMSGetVSyncTimesPerSec();

	if (mPaused || !mActive) {
		return;
	}

	for (int i = 0; i < PAD_MAX_CONTROLLERS; i++) {
		if (mControllerMgrTbl[i] != nullptr) {
			if (!mControllerMgrTbl[i]->channelMgrIsAllFree()) {
				mPower = mControllerMgrTbl[i]->update();

				if (mTimer > 0.0f) {
					mPower *= ((1.0f - mPower) * (mTimer / 0.5f) + 1.0f);
				}

				if (mPower > 1.0f) {
					mPower = 1.0f;
				} else if (mPower < 0.0f) {
					mPower = 0.0f;
				}

				mRumbleBaseTbl[i]->update(mPower, mStopHard);
			} else {
				mRumbleBaseTbl[i]->stop();
				mRumbleBaseTbl[i]->init();
			}

			mControllerMgrTbl[i]->updateMotorCount();
		}
	}

	if (mTimer > 0.0f) {
		mTimer -= updateRate;
	}
}

void RumbleMgr::setActive(bool active)
{
	mActive = active;
	if (!mActive) {
		reset();
	}
}

void RumbleMgr::startPause()
{
	for (int i = 0; i < PAD_MAX_CONTROLLERS; i++) {
		if (mRumbleBaseTbl[i] != nullptr) {
			mRumbleBaseTbl[i]->stop();
		}
	}

	mPaused = true;
}

void RumbleMgr::finishPause()
{
	mTimer  = 0.5f;
	mPaused = false;
}

// Size needed: 0x94, current: 0x94
void RumbleMgr::changePause()
{
	if (mPaused) {
		finishPause();
	} else {
		startPause();
	}
}

// Size needed: 0x24, current: 0x24
void RumbleMgr::changeMode()
{
	if (mStopHard) {
		mStopHard = false;
	} else {
		mStopHard = true;
	}
}
