#include "dolphin/types.h"
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/RumbleType.hpp>
#include <Player/MarioAccess.hpp>
#include <System/Application.hpp>
#include <types.h>

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
	f32 targetFrameTime       = 1.0f / SMSGetVSyncTimesPerSec();
	if (externalDampenSource != NULL) {
		dampenFactor = *mExternalDampenPtr;
		if (dampenFactor >= 0.0f) {
			if (dampenFactor > 1.0f)
				dampenFactor = 1.0f;
		} else {
			dampenFactor = 0.0f;
		}
	} else {
		if (this->mPositionalSourcePtr != NULL) {
			f32 distance
			    = (SMS_GetMarioPos() - *this->mPositionalSourcePtr).length();
			const f32 kScaleFactor = 1.0f / 3000.0f;
			distance     = -((kScaleFactor * (distance - 300.0f)) - 1.0f);
			dampenFactor = distance;
			if (distance >= 0.0f) {
				if (distance > 1.0f)
					dampenFactor = 1.0f;
			} else {
				dampenFactor = 0.0f;
			}
		} else {
			dampenFactor = 1.0f;
		}
	}

	this->mCurrentIntensity = 0.0f;
	if (!this->mLoopCount) {
		this->mElapsedTime         = 0.0f;
		this->mCurrentIntensity    = 0.0f;
		this->mChannelID           = -1;
		this->mLoopCount           = 0;
		this->mExternalDampenPtr   = 0;
		this->mPositionalSourcePtr = 0;
		this->rumbleData           = 0;
	}

	if (this->rumbleData) {
		int totalRumblePoints   = 0;
		int rumblePointIterator = 0;
		int keyframeIndex       = 0;

		this->mElapsedTime = this->mElapsedTime + targetFrameTime;

		while (true) {
			totalRumblePoints = *this->rumbleData->point;
			if (rumblePointIterator >= totalRumblePoints - 1)
				break;
			float* currentFrame = this->rumbleData->frame;

			if (this->mElapsedTime > this->rumbleData->frame[keyframeIndex]) {
				float lerpFactor = 1.0f;
				if ((this->rumbleData->frame[keyframeIndex + 1]
				     - this->rumbleData->frame[keyframeIndex])
				    <= 0.0)
					lerpFactor = 1.0f;
				else
					lerpFactor = ((this->mElapsedTime
					               - this->rumbleData->frame[keyframeIndex])
					              / (this->rumbleData->frame[keyframeIndex + 1]
					                 - this->rumbleData->frame[keyframeIndex]));
				this->mCurrentIntensity
				    = ((1.0f - lerpFactor) * rumbleData->power[keyframeIndex])
				      + (lerpFactor * rumbleData->power[keyframeIndex + 1]);
			}

			++rumblePointIterator;
			++keyframeIndex;
			break;
		}

		if (this->mElapsedTime > rumbleData->frame[totalRumblePoints - 1]) {
			if (this->mLoopCount <= 1) {
				this->mElapsedTime = 0.0f;
				if (mLoopCount >= 0) {
					this->mCurrentIntensity    = 0.0f;
					this->mChannelID           = -1;
					this->mLoopCount           = 0;
					this->mExternalDampenPtr   = 0;
					this->mPositionalSourcePtr = 0;
					this->rumbleData           = 0;
				}
			} else {
				this->mElapsedTime = 0.0f;
				--this->mLoopCount;
			}
		}
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
#define CLAMP(val, min, max)                                                   \
	((val) > (max) ? (max) : (val) < (min) ? (min) : (val))

inline f32 clamp(f32 val, f32 min, f32 max) {
    float value = val;
    if (val > max)
        return max;
    return value;
    if (val < min)
        value = min;
    return value;
}

f32 RumbleControllerMgr::update()
{
	float maxCurrentPower = 0.0f;

	for (int channelIdx = 0; channelIdx < MAX_RUMBLE_CHANNELS; ++channelIdx) {
		RumbleChannelMgr* currChannelManager = &this->channels[channelIdx];

		if (currChannelManager->rumbleData) {
			this->currentPower = currChannelManager->update();

			if (this->currentPower > maxCurrentPower) {
				maxCurrentPower = this->currentPower;
			}
		}
	}

	// this->currentPower = CLAMP(maxCurrentPower, 0.0f, 1.0f);
	this->currentPower = clamp(maxCurrentPower, 0.0f, 1.0f);

	if (this->currentPower > RumbleMgr::mPowerThreshold)
		this->motorTime = RumbleMgr::mMotorTimerPeriod;
	if (this->unk12)
		this->currentPower = 0.0f;
	return this->currentPower;
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

	const RumbleChannelData** pOutput = (const RumbleChannelData**)new int;
	if (pOutput != nullptr) {
		*pOutput = 0;
	}

	this->m_rumbleOutput  = pOutput;
	this->m_isInitialized = TRUE;
	this->m_flags         = 1;
}

void RumbleMgr::init()
{
	this->m_masterIntensityL = 0.0f;
	this->m_masterIntensityR = 0.0f;

	for (int i = 0; i < 4; ++i) {
		RumbleControllerMgr* pControllerMgr = this->m_controllerManagers[i];

		if (pControllerMgr != nullptr) {
			pControllerMgr->currentPower = 0.0f;
			for (int ch = 0; ch < MAX_RUMBLE_CHANNELS; ++ch) {
				void* unk20               = pControllerMgr->unk8;
				RumbleChannelMgr& channel = pControllerMgr->channels[ch];

				channel.mElapsedTime         = 0.0f;
				channel.mCurrentIntensity    = 0.0f;
				channel.mChannelID           = -1;
				channel.mLoopCount           = 0;
				channel.mExternalDampenPtr   = nullptr;
				channel.mPositionalSourcePtr = nullptr;
				channel.rumbleData           = nullptr;
				channel.unk20                = unk20;
			}

			RumbleControllerState* pState = this->m_controllerStates[i];
			pState->m_currentIntensityL   = 0.0f;
			pState->m_currentIntensityR   = 0.0f;
		}
	}

	*this->m_rumbleOutput = channelDataTbl;
	this->m_isInitialized = true;

	this->m_flags = true;
	this->unkA    = false;
}

void RumbleMgr::reset()
{
	this->m_masterIntensityL = 0.0f;
	this->m_masterIntensityR = 0.0f;

	for (int i = 0; i < 4; ++i) {
		RumbleControllerMgr* pMgr = this->m_controllerManagers[i];

		if (pMgr != nullptr) {
			pMgr->currentPower = 0.0f;
			for (int ch = 0; ch < MAX_RUMBLE_CHANNELS; ++ch) {
				RumbleChannelMgr& channel = pMgr->channels[ch];

				channel.mElapsedTime         = 0.0f;
				channel.mCurrentIntensity    = 0.0f;
				channel.mChannelID           = -1;
				channel.mLoopCount           = 0;
				channel.mExternalDampenPtr   = nullptr;
				channel.mPositionalSourcePtr = nullptr;
				channel.rumbleData           = nullptr;
			}

			pMgr->unkC      = 0;
			pMgr->motorTime = 0;
			pMgr->unk12     = false;

			RumbleControllerState* pState = this->m_controllerStates[i];
			PADControlMotor(pState->m_controllerIndex, PAD_MOTOR_STOP_HARD);

			pState->m_currentIntensityL = 0.0f;
			pState->m_currentIntensityR = 0.0f;
		}
	}

	this->m_flags = false;
}

void RumbleMgr::start(int channelId, float* externalDampenPtr)
{
	int index = 0;

	if (!this->m_flags && this->m_isInitialized) {
		if (this->m_controllerManagers[index]) {
			this->m_controllerManagers[index]->start(channelId, 1, externalDampenPtr);
		}
	}
}

void RumbleMgr::start(int channelId, Vec* positionalSourcePtr)
{
	int index = 0;

	if (!this->m_flags && this->m_isInitialized) {
		if (this->m_controllerManagers[index]) {
			this->m_controllerManagers[index]->start(channelId, 1, positionalSourcePtr);
		}
	}
}

void RumbleMgr::start(int channelId, int loopCount, f32* externalDampenPtr)
{
	int index = 0;

	if (!this->m_flags && this->m_isInitialized) {
		if (this->m_controllerManagers[index]) {
			this->m_controllerManagers[index]->start(channelId, loopCount, externalDampenPtr);
		}
	}
}

void RumbleMgr::start(int channelId, int loopCount, Vec* positionalSourcePtr)
{
	int index = 0;

	if (!this->m_flags && this->m_isInitialized) {
		if (this->m_controllerManagers[index]) {
			this->m_controllerManagers[index]->start(channelId, loopCount, positionalSourcePtr);
		}
	}
}

void RumbleMgr::stop()
{
	for (int controllerPort = 0; controllerPort < 4; controllerPort++) {
		RumbleControllerMgr* controllerMgr
		    = this->m_controllerManagers[controllerPort];

		if (controllerMgr != nullptr) {
			for (int rumbleChannel = 0; rumbleChannel < MAX_RUMBLE_CHANNELS;
			     rumbleChannel++) {
				RumbleChannelMgr& channel
				    = controllerMgr->channels[rumbleChannel];
					
				if (channel.rumbleData != nullptr) {
					channel.mElapsedTime         = 0.0f;
					channel.mCurrentIntensity    = 0.0f;
					channel.mChannelID           = -1;
					channel.mLoopCount           = 0;
					channel.mExternalDampenPtr   = nullptr;
					channel.mPositionalSourcePtr = nullptr;
					channel.rumbleData           = nullptr;
				}
			}
		}
	}
}

void RumbleMgr::stop(int channelId)
{
	int index = 0;

	if (!this->m_flags && this->m_isInitialized) {
		if (this->m_controllerManagers[index]) {
			this->m_controllerManagers[index]->stop(channelId);
		}
	}
}

void RumbleMgr::update()
{
	float delta = 1.0f / SMSGetVSyncTimesPerSec();

	if (!this->m_flags && this->m_isInitialized) {
		for (int i = 0; i < 4; ++i) {
			RumbleControllerMgr* controller = this->m_controllerManagers[i];

			if (controller) {
				bool isSilent = true;
				for (int ch = 0; ch < MAX_RUMBLE_CHANNELS; ch++) {
					RumbleChannelMgr& channel = controller->channels[ch];
					if (channel.rumbleData != nullptr
					    || channel.mElapsedTime != 0.0f) {
						isSilent = false;
						break;
					}
				}

				RumbleControllerState* state = this->m_controllerStates[i];

				if (isSilent) {
					PADControlMotor(state->m_controllerIndex,
					                PAD_MOTOR_STOP_HARD);
					state->m_currentIntensityL = 0.0f;
					state->m_currentIntensityR = 0.0f;
				} else {
					this->m_masterIntensityL = controller->update();

					if (this->m_masterIntensityR > 0.0f) {
						this->m_masterIntensityL
						    = this->m_masterIntensityL
						      * ((1.0f - this->m_masterIntensityL)
						             * (this->m_masterIntensityR / 0.5f)
						         + 1.0f);
					}

					if (this->m_masterIntensityL <= 1.0f) {
						if (this->m_masterIntensityL < 0.0f) {
							this->m_masterIntensityL = 0.0f;
						}
					} else {
						this->m_masterIntensityL = 1.0f;
					}

					state->m_currentIntensityL = this->m_masterIntensityL;
					state->m_currentIntensityR += state->m_currentIntensityL;

					if (state->m_currentIntensityL > 0.0f) {
						if (state->m_currentIntensityR >= 1.0f) {
							state->m_currentIntensityR -= 1.0f;
							PADControlMotor(state->m_controllerIndex,
							                PAD_MOTOR_RUMBLE);
						} else {
							if (this->unkA)
								PADControlMotor(state->m_controllerIndex,
								                PAD_MOTOR_STOP_HARD);
							else
								PADControlMotor(state->m_controllerIndex,
								                PAD_MOTOR_STOP);
						}
					} else {
						if (this->unkA)
							PADControlMotor(state->m_controllerIndex,
							                PAD_MOTOR_STOP_HARD);
						else
							PADControlMotor(state->m_controllerIndex,
							                PAD_MOTOR_STOP);
					}
				}
				controller->updateMotorCount();
			}
		}

		if (this->m_masterIntensityR > 0.0f) {
			this->m_masterIntensityR = this->m_masterIntensityR - delta;
		}
	}
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
