#include "dolphin/types.h"
#include <types.h>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/RumbleType.hpp>
#include <System/Application.hpp>
#include <Player/MarioAccess.hpp>

RumbleMgr* SMSRumbleMgr;

u32 RumbleMgr::mMotorCountLimit  = 900;
u16 RumbleMgr::mMotorTimerPeriod = 80;
f32 RumbleMgr::mPowerThreshold   = 0.5f;

RumbleChannelMgr::RumbleChannelMgr()
{
	mElapsedTime         = 0.0f;
	mCurrentIntensity    = 0.0f;
	mChannelID           = -1;
	mLoopCount           = 0;
	mExternalDampenPtr   = 0;
	mPositionalSourcePtr = 0;
	rumbleData           = 0;
}

f32 RumbleChannelMgr::update()
{
	f32 dampenFactor;
	f32* externalDampenSource = this->mExternalDampenPtr;
	f32 temp_f31              = 1.0f / SMSGetVSyncTimesPerSec();
	if (externalDampenSource != NULL) {
		f32 temp_f1;
		temp_f1      = *externalDampenSource;
		dampenFactor = temp_f1;
		if (temp_f1 < 0.0f) {
			dampenFactor = 0.0f;
		} else if (dampenFactor > 1.0f) {
			dampenFactor = 1.0f;
		}
	} else {
		if (this->mPositionalSourcePtr != NULL) {
			f32 temp_f2;
			JGeometry::TVec3<f32> temp;
			temp.set(SMS_GetMarioPos());
			temp -= *this->mPositionalSourcePtr;
			// temp = SMS_GetMarioPos() - *this->mPositionalSourcePtr;
			f32 distance = temp.length(); // why are you getting inlined :(
			// dampenFactor = clamp(distance, 0, 1);
			dampenFactor = distance;
			if (distance < 0.0f) {
				dampenFactor = 0.0f;
			} else if (dampenFactor > 1.0f) {
				dampenFactor = 1.0f;
			}
		} else {
			dampenFactor = 1.0f;
		}
	}

	this->mCurrentIntensity = 0.0;
	if (!this->mLoopCount) {
		this->mElapsedTime         = 0.0;
		this->mCurrentIntensity    = 0.0;
		this->mChannelID           = -1;
		this->mLoopCount           = 0;
		this->mExternalDampenPtr   = 0;
		this->mPositionalSourcePtr = 0;
		this->rumbleData           = 0;
	}

	this->mCurrentIntensity *= dampenFactor;
	return this->mCurrentIntensity;
}

void RumbleControllerMgr::reset()
{
	currentPower = 0.0f;

	// @FIXME: registers arent being assigned correctly during the unroll
	for (u32 i = 0; i < MAX_RUMBLE_CHANNELS; ++i) {
		RumbleChannelMgr* channel     = &channels[i];
		channel->mElapsedTime         = 0.0f;
		channel->mCurrentIntensity    = 0.0f;
		channel->mChannelID           = -1;
		channel->mLoopCount           = 0;
		channel->mExternalDampenPtr   = 0;
		channel->mPositionalSourcePtr = 0;
		channel->rumbleData           = 0;
	}

	unkC      = 0;
	motorTime = 0;
	unk12     = false;
}

#pragma dont_inline on
void RumbleControllerMgr::start(int channelId, int loopCount,
                                float* externalDampenPtr)
{
	for (s32 i = 0; i < MAX_RUMBLE_CHANNELS; ++i) {
		RumbleChannelMgr* channel = &this->channels[i];

		if (channel->rumbleData == NULL) {
			channel->mElapsedTime       = 0.0f;
			channel->rumbleData         = &channelDataTbl[channelId];
			channel->mChannelID         = channelId;
			channel->mLoopCount         = loopCount;
			channel->mExternalDampenPtr = externalDampenPtr;
			return; // Found a slot and started the rumble, so we exit
		}
	}
}

void RumbleControllerMgr::start(int channelId, int loopCount,
                                Vec* positionalSourcePtr)
{
	for (s32 i = 0; i < MAX_RUMBLE_CHANNELS; ++i) {
		RumbleChannelMgr* channel = &this->channels[i];

		if (channel->rumbleData == NULL) {
			channel->mElapsedTime         = 0.0f;
			channel->rumbleData           = &channelDataTbl[channelId];
			channel->mChannelID           = channelId;
			channel->mLoopCount           = loopCount;
			channel->mPositionalSourcePtr = positionalSourcePtr;
			return; // Found a slot and started the rumble, so we exit
		}
	}
}

void RumbleControllerMgr::stop(int channelId)
{
	for (u32 i = 0; i < MAX_RUMBLE_CHANNELS; ++i) {
		RumbleChannelMgr* channel = &channels[i];
		if (channel->rumbleData && channelId == channel->mChannelID) {
			channel->mElapsedTime         = 0.0f;
			channel->mCurrentIntensity    = 0.0f;
			channel->mChannelID           = -1;
			channel->mLoopCount           = 0;
			channel->mExternalDampenPtr   = 0;
			channel->mPositionalSourcePtr = 0;
			channel->rumbleData           = 0;
		}
	}
}
#pragma dont_inline off

void RumbleControllerMgr::updateMotorCount()
{
	u16 temp_r4_2;
	u32 temp_r4;

	if ((s8)unk12 != 0) {
		temp_r4 = unkC;
		if (temp_r4 != 0U) {
			unkC = temp_r4 - 1;
			return;
		}
		unk12     = 0;
		motorTime = 0;
		return;
	}
	if (unkC > RumbleMgr::mMotorCountLimit) {
		unk12 = 1;
		return;
	}
	temp_r4_2 = motorTime;
	if (temp_r4_2 != 0) {
		motorTime = temp_r4_2 - 1;
		unkC += 1;
		return;
	}
	unkC = 0U;
}

f32 RumbleControllerMgr::update()
{
	RumbleChannelMgr* temp_r3;
	f32 temp_f0;
	f32 temp_f1;
	f32 var_f31;
	s32 var_r30;
	s32 var_r31;

	var_r31 = 0;
	var_r30 = 0;
	var_f31 = 0.0f;
	do {
		temp_r3 = this->channels + var_r31;
		if (temp_r3->rumbleData != 0) {
			this->currentPower = temp_r3->update();
			temp_f0            = this->currentPower;
			if (temp_f0 > var_f31) {
				var_f31 = temp_f0;
			}
		}
		var_r30 += 1;
		var_r31 += 0x20;
	} while (var_r30 < 0x20);

	currentPower = var_f31;
	temp_f1      = currentPower;
	if (temp_f1 > 1.0f) {
		currentPower = 1.0f;
	} else if (temp_f1 < 0.0f) {
		currentPower = 0.0f;
	}

	if (currentPower > RumbleMgr::mPowerThreshold) {
		motorTime = RumbleMgr::mPowerThreshold;
	}
	if (unk12 != false) {
		currentPower = 0.0f;
	}
	return currentPower;
}

RumbleMgr::RumbleMgr(bool bController1Avail, bool bController2Avail,
                     bool bController3Avail, bool bController4Avail)
{
	bool aControllers[4];
	this->m_masterIntensityL = 0.0f;
	this->m_masterIntensityR = 0.0f;
	aControllers[0]          = bController1Avail;
	aControllers[1]          = bController2Avail;
	aControllers[2]          = bController3Avail;
	aControllers[3]          = bController4Avail;

	for (int i = 0; i < 4; ++i) {
		if (aControllers[i]) {
			RumbleControllerMgr* pMgr = new RumbleControllerMgr();
			if (pMgr) {
				pMgr->currentPower = 0.0f;
				pMgr->channels     = new RumbleChannelMgr[MAX_RUMBLE_CHANNELS];
			}
			this->m_controllerManagers[i] = pMgr;

			RumbleControllerState* pState = new RumbleControllerState();
			if (pState) {
				pState->m_currentIntensityL = 0.0f;
				pState->m_currentIntensityR = 0.0f;
				pState->m_controllerIndex   = i;
			}
			this->m_controllerStates[i] = pState;
		}
	}

	int* pOutput = new int;
	if (pOutput)
		*pOutput = 0;

	this->m_rumbleOutput  = (const int***)pOutput;
	this->m_isInitialized = TRUE;
	this->m_flags         = 1;
}

void RumbleMgr::init() { channelDataTbl; }

void RumbleMgr::reset() { PADControlMotor(0, 0); }

void RumbleMgr::start(int a2, float* a3)
{
	int index = 0;

	if (!this->m_flags && this->m_isInitialized) {
		if (this->m_controllerManagers[index]) {
			this->m_controllerManagers[index]->start(a2, 1, a3);
		}
	}
}

void RumbleMgr::start(int a2, Vec* a3)
{
	int index = 0;

	if (!this->m_flags && this->m_isInitialized) {
		if (this->m_controllerManagers[index]) {
			this->m_controllerManagers[index]->start(a2, 1, a3);
		}
	}
}

void RumbleMgr::start(int arg0, int arg1, f32* arg2)
{
	int index = 0;

	if (!this->m_flags && this->m_isInitialized) {
		if (this->m_controllerManagers[index]) {
			this->m_controllerManagers[index]->start(arg0, arg1, arg2);
		}
	}
}

void RumbleMgr::start(int a2, int a3, Vec* a4)
{
	int index = 0;

	if (!this->m_flags && this->m_isInitialized) {
		if (this->m_controllerManagers[index]) {
			this->m_controllerManagers[index]->start(a2, a3, a4);
		}
	}
}

void RumbleMgr::stop() { }

void RumbleMgr::stop(int arg0)
{
	int index = 0;

	if (!this->m_flags && this->m_isInitialized) {
		if (this->m_controllerManagers[index]) {
			this->m_controllerManagers[index]->stop(arg0);
		}
	}
}

void RumbleMgr::update()
{
	f32 fuck = SMSGetVSyncTimesPerSec();
	s32 a    = fuck * mPowerThreshold;
	s32 b    = mMotorTimerPeriod;
	s32 c    = a * b;
	// unk0  = c;
}

void RumbleMgr::setActive(bool activeState)
{
	int v3;                    // r30
	int v4;                    // r31
	RumbleControllerMgr* v5;   // r3
	RumbleControllerState* v6; // r29
	float* v7;                 // r3

	this->m_isInitialized = activeState;
	if (!this->m_isInitialized) {
		v3                       = 0;
		v4                       = 0;
		this->m_masterIntensityL = 0.0;
		this->m_masterIntensityR = 0.0;
		do {
			v5 = this->m_controllerManagers[v4];
			if (v5) {
				v5->reset();
				v6 = m_controllerStates[v4];
				PADControlMotor(v6->m_controllerIndex, PAD_MOTOR_STOP_HARD);
				v6->m_currentIntensityL = 0.0f;
				v6->m_currentIntensityR = 0.0f;
			}
			++v3;
			++v4;
		} while (v3 < 4);
		this->m_flags = 0;
	}
}

void RumbleMgr::startPause()
{
	int padIter;                               // r31
	int i;                                     // r30
	RumbleControllerState* theControllerState; // r3

	padIter = 0;
	for (i = 0; i < 4; ++i) {
		theControllerState = this->m_controllerStates[padIter];
		if (theControllerState)
			PADControlMotor(theControllerState->m_controllerIndex,
			                PAD_MOTOR_STOP_HARD);
		++padIter;
	}
	this->m_flags = 1;
}

void RumbleMgr::finishPause()
{
	this->m_masterIntensityR = 0.5;
	this->m_flags            = 0;
}
