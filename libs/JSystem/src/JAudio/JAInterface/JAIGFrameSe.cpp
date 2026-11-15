#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <math.h>

void JAIBasic::checkNextFrameSe()
{
	JAISound dummySound;

	struct Candidate {
		/* 0x0 */ u8 state;
		/* 0x4 */ u32 score;
		/* 0x8 */ JAISound* sound;
	};
	Candidate candidates[16];

	s16 priority;
	u8 cameraBegin;
	u8 cameraEnd;
	JAISound* selectedSound;
	u8 existingSlotIndex;
	int category;
	u8 candidateIndex;
	u8 slotIndex;
	u8 canAssign;
	JAISound* sound;
	u8 slotCount;
	u8 cameraIndex;
	JAISound::FabricatedPositionInfo* posInfo;
	u8 candidateCount;

	f32 maxDistanceSq
	    = JAIGlobalParameter::distanceMax * JAIGlobalParameter::distanceMax;
	f32 priorityScale = JAIGlobalParameter::distanceMax / 1000.0f;
	if (priorityScale == 0.0f)
		priorityScale = 1.0f;

	// All categories are processed independently here.
	for (category = 0; category < JAIGlobalParameter::getParamSeCategoryMax();
	     ++category) {
		// Re-init the candidate list for this category.
		for (slotIndex = 0;
		     slotIndex
		     < mData->mSeCategoryInfoTable[mSoundScene][category].mMaxPlaying;
		     ++slotIndex) {
			candidates[slotIndex].score = 0x7fffffff;
			candidates[slotIndex].sound = nullptr;
			candidates[slotIndex].state = 0xff;
		}

		candidateCount = 0;

		// Iterate over all sound effects in this category & build up the array
		// of candidates to be playing this frame
		sound = mData->mSeRegist[category].mUsedHead;
		while (sound) {
			if (sound->mState == SOUNDSTATE_Stored
			    && (sound->mSoundID & JAISoundID_SeModeMask)) {
				sound->decWait();
			} else if (!(sound->mSoundID & JAISoundID_SeModeMask)
			           && sound->mState == SOUNDSTATE_Stopping) {
				dummySound.mNextSound = sound->mNextSound;
				releaseSeRegist(sound);
				sound = &dummySound;
			}

			if (sound->mWaitTimer == 0) {
				dummySound.mNextSound = sound->mNextSound;
				releaseSeRegist(sound);
				sound = &dummySound;
			} else if (sound->mState != SOUNDSTATE_Inactive) {
				f32 minDistanceSq = 2147483647.0f;
				if (sound->mCameraIdx == 4) {
					cameraBegin = 0;
					cameraEnd   = JAIGlobalParameter::audioCameraMax;
				} else {
					cameraEnd   = sound->mCameraIdx + 1;
					cameraBegin = sound->mCameraIdx;
				}

				// Calculate sound priorities based on distances to audio
				// cameras and inherent priorities of the sounds.
				for (cameraIndex = cameraBegin; cameraIndex < cameraEnd;
				     ++cameraIndex) {
					posInfo = &sound->unk1C[cameraIndex];

					posInfo->mPrevCamSpacePos = posInfo->mCamSpacePos;
					if (sound->getTrans() == nullptr) {
						posInfo->mCamSpacePos = JAIConst::dummyZeroVec;
					} else {
						MTXMultVec(mAudioCameras[cameraIndex].nViewMtx,
						           (Vec*)sound->getTrans(),
						           &posInfo->mCamSpacePos);
					}

					// Squared for now but will get rooted later
					posInfo->mCameraDistance
					    = posInfo->mCamSpacePos.x * posInfo->mCamSpacePos.x
					      + posInfo->mCamSpacePos.y * posInfo->mCamSpacePos.y
					      + posInfo->mCamSpacePos.z * posInfo->mCamSpacePos.z;
					priority = sound->getInfoPriority();
					if (sound->mAdjustPrio) {
						priority += sound->mAdjustPrio;
						if (priority < 0)
							priority = 0;
						else if (priority > 0xff)
							priority = 0xff;
					}

					sound->mPriority
					    = (u32)((0xff - priority) * (0xff - priority) * 0x1690
					            / priorityScale)
					      + (u32)(posInfo->mCameraDistance / priorityScale);
					if (posInfo->mCamSpacePos.z > 0.0f)
						sound->mPriority += (u32)(posInfo->mCamSpacePos.z * 6.0f
						                          / priorityScale);

					if (cameraIndex == 0
					    || posInfo->mCameraDistance < minDistanceSq)
						minDistanceSq = posInfo->mCameraDistance;
				}

				if (sound->mCameraIdx == 4)
					sound->mPriority /= JAIGlobalParameter::audioCameraMax;

				f32 distanceLimitSq;
				if (sound->getSwBit() & JAISeSwBit_DistanceLimit)
					distanceLimitSq = maxDistanceSq;
				else
					distanceLimitSq = 1e+10f;

				if (minDistanceSq > distanceLimitSq) {
					if (!(sound->mSoundID & JAISoundID_SeModeMask)) {
						if (sound->mState != SOUNDSTATE_Stored) {
							JAISystemInterface::writePortApp(
							    mSeSequence->getSeqParameter()->mSeqHandle,
							    (sound->mTrack >> 4) + 0x20000000
							        + ((sound->mTrack & 0xf) << 4),
							    0);
							mSeSequence->setTrackInterruptSwitch(sound->mTrack,
							                                     1);
						}
						sound->mState = SOUNDSTATE_Stored;
					} else {
						dummySound.mNextSound = sound->mNextSound;
						stopSoundHandle(sound, 0);
						sound = &dummySound;
					}
				} else {
					slotCount = mData
					                ->mSeCategoryInfoTable
					                    [mSoundScene]
					                    [(u8)sound->getSeCategoryNumber()]
					                .mMaxPlaying;
					for (slotIndex = 0; slotIndex < slotCount; ++slotIndex) {
						if (sound->mPriority < candidates[slotIndex].score
						    || (sound->mPriority == candidates[slotIndex].score
						        && candidates[slotIndex].state
						               >= sound->mState)) {
							// We take this candidate -- insertion-sort it into
							// the candidates array
							if (candidateCount < slotCount)
								++candidateCount;
							for (candidateIndex = slotCount - 1;
							     candidateIndex > slotIndex; --candidateIndex) {
								candidates[candidateIndex].score
								    = candidates[candidateIndex - 1].score;
								candidates[candidateIndex].sound
								    = candidates[candidateIndex - 1].sound;
								candidates[slotIndex].state
								    = candidates[candidateIndex - 1].state;
							}
							candidates[slotIndex].score = sound->mPriority;
							candidates[slotIndex].sound = sound;
							candidates[slotIndex].state = sound->mState;

							slotIndex = slotCount;
						}
					}
				}
			}

			if (sound != nullptr)
				sound = sound->mNextSound;
		}

		for (candidateIndex = 0; candidateIndex < candidateCount;
		     ++candidateIndex) {
			selectedSound = candidates[candidateIndex].sound;
			if (selectedSound->mState == SOUNDSTATE_Stored)
				selectedSound->mState = SOUNDSTATE_Prepared;
			else if (selectedSound->mState == SOUNDSTATE_Playing)
				selectedSound->mState = SOUNDSTATE_Started;
		}

		// Update playing SE slots with our chosen candidates
		slotCount
		    = mData->mSeCategoryInfoTable[mSoundScene][category].mMaxPlaying;
		for (slotIndex = 0; slotIndex < slotCount; ++slotIndex) {
			selectedSound = mData->mSeTrack[category][slotIndex].mSound;
			canAssign     = 0;
			if (selectedSound == nullptr) {
				canAssign = 1;
			} else if (selectedSound->mState == SOUNDSTATE_Playing) {
				if (selectedSound->mSoundID & JAISoundID_SeModeMask) {
					releaseSeRegist(selectedSound);
				} else {
					selectedSound->mState     = SOUNDSTATE_Stored;
					selectedSound->mWaitTimer = 0;
				}
				canAssign = 1;
			} else if (selectedSound->mState == SOUNDSTATE_Inactive) {
				mData->mSeTrack[category][slotIndex].mSound = nullptr;
				canAssign                                   = 1;
			} else {
				for (candidateIndex = 0; candidateIndex < slotCount;
				     ++candidateIndex) {
					if (mData->mSeTrack[category][slotIndex].mSound
					    == candidates[candidateIndex].sound) {
						candidates[candidateIndex].sound = nullptr;
						candidateIndex                   = slotCount;
					}
				}
			}

			if (canAssign == 1) {
				for (candidateIndex = 0; candidateIndex < slotCount;
				     ++candidateIndex) {
					selectedSound = candidates[candidateIndex].sound;
					if (selectedSound != nullptr
					    && selectedSound->mState != SOUNDSTATE_Started) {
						for (existingSlotIndex = 0;
						     existingSlotIndex < slotCount;
						     ++existingSlotIndex) {
							if (mData->mSeTrack[category][existingSlotIndex]
							        .mSound
							    && selectedSound
							           == mData
							                  ->mSeTrack[category]
							                            [existingSlotIndex]
							                  .mSound) {
								canAssign         = 0;
								existingSlotIndex = slotCount;
							}
						}

						if (canAssign == 1) {
							mData->mSeTrack[category][slotIndex].mSound
							    = selectedSound;
							candidates[candidateIndex].sound = nullptr;
							candidateIndex                   = slotCount + 1;
						}
					}
				}

				if (candidateIndex == slotCount)
					mData->mSeTrack[category][slotIndex].mSound = nullptr;
			}
		}
	}
}

void JAIBasic::sendPlayingSeCommand()
{
	u16 port2Status;
	u16 port0Status;
	u8 slotIndex;
	JAISound* sound;
	u8 trackNo = 0;

	for (u8 category = 0;
	     category < JAIGlobalParameter::getParamSeCategoryMax(); ++category) {
		for (slotIndex = 0;
		     slotIndex < mData->mSeCategoryInfoTable[mSoundScene][(u8)category]
		                     .mMaxPlaying;
		     ++trackNo, ++slotIndex) {
			sound = mData->mSeTrack[category][slotIndex].mSound;
			if (sound == nullptr)
				continue;

			// Essentially, update JAS backend state of sound effects which were
			// selected as playing for this frame.

			sound->incPlayGameFrameCounter();

			u32 trackRoute
			    = ((trackNo >> 4) & 0xF) + 0x20000000 + ((trackNo & 0xF) << 4);

			u32 seqHandle = mSeSequence->getSeqParameter()->mSeqHandle;

			JAISystemInterface::readPortApp(seqHandle, trackRoute + 0x20000,
			                                &port2Status);
			JAISystemInterface::readPortApp(seqHandle, trackRoute,
			                                &port0Status);

			JAISound::FabricatedPositionInfo* positions = sound->unk1C;
			for (u8 i = 0; i < JAIGlobalParameter::audioCameraMax; ++i) {
				f32* distance = &positions[i].mCameraDistance;
				*distance     = std::sqrtf(*distance);
			}

			u8 soundState = sound->mState;
			if (soundState == SOUNDSTATE_Prepared) {
				u32 soundFlags = sound->getSwBit();
				sound->mTrack  = trackNo;
				if (soundFlags & JAISeSwBit_SeqMute)
					setSeqMuteFromSeStart(sound);

				if (soundFlags & JAISeSwBit_RandomPitchWidthMask) {
					s32 randomValue
					    = (s32)(255.0f * JAIConst::random.get_ufloat_1());
					switch (soundFlags & JAISeSwBit_RandomPitchWidthMask) {
					case JAISeSwBit_RandomPitchWidthSmall:
						sound->setRandom(randomValue & 0xF);
						break;
					case JAISeSwBit_RandomPitchWidthMiddle:
						sound->setRandom(randomValue & 0x1F);
						break;
					case JAISeSwBit_RandomPitchWidthLarge:
						sound->setRandom(randomValue & 0x3F);
						break;
					default:
						sound->setRandom(0);
						break;
					}
				}

				u16* pendingPorts = &sound->getSeParameter()->mPortUpdate;
				for (u8 portNo = 0; *pendingPorts != 0; ++portNo) {
					u32 portBit = 1 << portNo;
					if (*pendingPorts & portBit) {
						mSeSequence->setTrackPortData(
						    sound->mTrack, portNo,
						    sound->getSeParameter()->mPortData[portNo]);
						*pendingPorts ^= portBit;
					}
				}

				sound->setSeDistanceParameters();
				setSeExtParameter(sound);

				if (sound->mFadeCounter > 1) {
					sound->setSeInterVolume(6, 0.0f, 0, 0);
					sound->setSeInterVolume(6, 127.0f, sound->mFadeCounter, 0);
					sound->mFadeCounter = 0;
				}

				sendSeAllParameter(sound);

				u16 soundIndex = sound->mSoundID & JAISoundID_IndexMask;
				if (sound->checkSwBit(JAISeSwBit_GroundVariant)) {
					u32 groundNumber = sound->mActorGroundNumber;
					soundIndex += getMapInfoGround(groundNumber);
				}

				u16 distanceWait;
				if (JAIGlobalParameter::audioCameraMax == 1
				    && sound->checkSwBit(JAISeSwBit_DistanceWait)) {
					if (sound->unk1C[0].mCameraDistance
					    < JAIGlobalParameter::distanceMax) {
						distanceWait = JAIGlobalParameter::seDistanceWaitMax
						               * (u32)sound->unk1C[0].mCameraDistance
						               / (u32)JAIGlobalParameter::distanceMax;
					} else {
						distanceWait = JAIGlobalParameter::seDistanceWaitMax;
					}
				} else {
					distanceWait = 0;
				}

				JAISystemInterface::writePortApp(
				    seqHandle, trackRoute + 0x30000, distanceWait);
				JAISystemInterface::writePortApp(
				    seqHandle, trackRoute + 0x60000,
				    getMapInfoFxline(sound->mActorGroundNumber));
				JAISystemInterface::writePortApp(
				    seqHandle, trackRoute + 0x40000, soundIndex);
				JAISystemInterface::writePortApp(seqHandle, trackRoute, 1);

				if (sound->mSoundID & JAISoundID_SeModeMask)
					sound->mState = SOUNDSTATE_Playing;
				else
					sound->mState = SOUNDSTATE_Stopping;
			} else if (port2Status == 0 && port0Status != 1) {
				releaseSeRegist(sound);
			} else if (sound->mFadeCounter != 0) {
				if (sound->getSeParameter()->mVolume[6].mCurrentValue != 0.0f) {
					sound->setSeDistanceParameters();
					sendSeAllParameter(sound);
					if (sound->mSoundID & JAISoundID_SeModeMask)
						sound->mState = SOUNDSTATE_Playing;
					else
						sound->mState = SOUNDSTATE_Stopping;
				} else {
					releaseSeRegist(sound);
				}
			} else if (soundState == SOUNDSTATE_Started) {
				sound->setSeDistanceParameters();
				sendSeAllParameter(sound);
				if (sound->mSoundID & JAISoundID_SeModeMask)
					sound->mState = SOUNDSTATE_Playing;
				else
					sound->mState = SOUNDSTATE_Stopping;
			}
		}
	}
}

void JAIBasic::setSeqMuteFromSeStart(JAISound* se_sound)
{
	for (u32 i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound* sequence = mData->mSeqTrackInfo[i].mSound;
		if (i != mSeSequence->mTrack && sequence
		    && !(sequence->getSwBit() & JAISeqSwBit_NoSeqMute)) {
			sequence->setSeqInterVolume(
			    9, JAIGlobalParameter::seqMuteVolumeSePlay / 127.0f,
			    JAIGlobalParameter::seqMuteMoveSpeedSePlay);
			mSeqMuteSeTrackMask |= 1 << se_sound->mTrack;
		}
	}
}

void JAIBasic::clearSeqMuteFromSeStop(JAISound* se_sound)
{
	if (mSeqMuteSeTrackMask == 0
	    || !(se_sound->getSwBit() & JAISeSwBit_SeqMute))
		return;

	for (u32 i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound* sequence = mData->mSeqTrackInfo[i].mSound;
		if (i != mSeSequence->mTrack && sequence
		    && !(sequence->getSwBit() & JAISeqSwBit_NoSeqMute)) {
			mSeqMuteSeTrackMask &= (1 << se_sound->mTrack) ^ 0xffffffff;
			if (mSeqMuteSeTrackMask == 0) {
				sequence->setSeqInterVolume(
				    9, 1.0f, JAIGlobalParameter::seqMuteMoveSpeedSePlay);
			}
		}
	}
}

void JAIBasic::checkSeMovePara()
{
	if (!mSeSequence || mSeSequence->getSeqParameter()->mPauseMode == 2)
		return;

	for (u8 i = 0; i < JAIGlobalParameter::getParamSeCategoryMax(); ++i) {
		for (JAISound* sound = mData->mSeRegist[i].mUsedHead; sound != nullptr;
		     sound           = sound->mNextSound) {
			mData->setSeMovePara(sound->getSeParameter()->mVolume);
			mData->setSeMovePara(sound->getSeParameter()->mPan);
			mData->setSeMovePara(sound->getSeParameter()->mFxmix);
			mData->setSeMovePara(sound->getSeParameter()->mFir);
			mData->setSeMovePara(sound->getSeParameter()->mDolby);
			mData->setSeMovePara(sound->getSeParameter()->mPitch);
		}
	}
}

void JAIBasic::sendSeAllParameter(JAISound* sound)
{
	JAIData::FabricatedSeTrackParameter* trackParams
	    = &mData->unk0[sound->mTrack];
	JAISeParameter* seParams = sound->getSeParameter();

	// Volume
	f32 volume;
	if (seParams->mVolume[7].mCurrentValue == -1.0f) {
		if (seParams->mVolumePointer != nullptr)
			seParams->mVolume[0].mCurrentValue = *seParams->mVolumePointer;

		volume = 1.0f;
		for (int paramIndex = 0; paramIndex < 7; ++paramIndex)
			volume *= seParams->mVolume[paramIndex].mCurrentValue;
	} else {
		volume = seParams->mVolume[7].mCurrentValue;
	}
	volume *= mSeCategoryVolume[(u8)sound->getSeCategoryNumber()];
	if (trackParams->mVolume != volume) {
		trackParams->mVolume = volume;
		if (sound->mState != SOUNDSTATE_Prepared) {
			mData->mSeqTrackInfo[mSeSequence->mTrack]
			    .mTrackPendingFlags[sound->mTrack]
			    |= JASystem::Kernel::TPortArgs::UPDATE_VOLUME;
			JAISystemInterface::setSeqPortargsF32(
			    &mData->mSeqTrackInfo[mSeSequence->mTrack], sound->mTrack, 2,
			    volume);
		}
	}

	// Pan
	f32 pan;
	if (seParams->mPan[7].mCurrentValue == -1.0f) {
		if (seParams->mPanPointer != nullptr) {
			seParams->mPan[0].mCurrentValue = *seParams->mPanPointer;
		}
		pan = 0.0f;
		for (int paramIndex = 0; paramIndex < 7; ++paramIndex)
			if (seParams->mPan[paramIndex].mCurrentValue != 0.5f)
				pan += seParams->mPan[paramIndex].mCurrentValue - 0.5f;
		pan += 0.5f;
		if (pan < 0.0f)
			pan = 0.0f;
		else if (pan > 1.0f)
			pan = 1.0f;
	} else {
		pan = seParams->mPan[7].mCurrentValue;
	}
	if (trackParams->mPan != pan) {
		trackParams->mPan = pan;
		if (sound->mState != SOUNDSTATE_Prepared) {
			mData->mSeqTrackInfo[mSeSequence->mTrack]
			    .mTrackPendingFlags[sound->mTrack]
			    |= JASystem::Kernel::TPortArgs::UPDATE_PAN;
			JAISystemInterface::setSeqPortargsF32(
			    &mData->mSeqTrackInfo[mSeSequence->mTrack], sound->mTrack, 4,
			    pan);
		}
	}

	// Pitch
	f32 pitch;
	if (seParams->mPitch[7].mCurrentValue == -1.0f) {
		if (seParams->mPitchPointer != nullptr) {
			seParams->mPitch[0].mCurrentValue = *seParams->mPitchPointer;
		}
		pitch = 1.0f;
		for (int paramIndex = 0; paramIndex < 7; ++paramIndex)
			pitch *= seParams->mPitch[paramIndex].mCurrentValue;
	} else {
		pitch = seParams->mPitch[7].mCurrentValue;
	}
	if (trackParams->mPitch != pitch) {
		trackParams->mPitch = pitch;
		if (sound->mState != SOUNDSTATE_Prepared) {
			mData->mSeqTrackInfo[mSeSequence->mTrack]
			    .mTrackPendingFlags[sound->mTrack]
			    |= JASystem::Kernel::TPortArgs::UPDATE_PITCH;
			JAISystemInterface::setSeqPortargsF32(
			    &mData->mSeqTrackInfo[mSeSequence->mTrack], sound->mTrack, 3,
			    pitch);
		}
	}

	// FxMix
	f32 fxmix;
	if (seParams->mFxmix[7].mCurrentValue == -1.0f) {
		if (seParams->mFxmixPointer != nullptr) {
			seParams->mFxmix[0].mCurrentValue = *seParams->mFxmixPointer;
		}
		fxmix = 0.0f;
		for (int paramIndex = 0; paramIndex < 7; ++paramIndex)
			fxmix += seParams->mFxmix[paramIndex].mCurrentValue;
	} else {
		fxmix = seParams->mFxmix[7].mCurrentValue;
	}
	if (trackParams->mFxmix != fxmix) {
		trackParams->mFxmix = fxmix;
		if (sound->mState != SOUNDSTATE_Prepared) {
			mData->mSeqTrackInfo[mSeSequence->mTrack]
			    .mTrackPendingFlags[sound->mTrack]
			    |= JASystem::Kernel::TPortArgs::UPDATE_FXMIX;
			JAISystemInterface::setSeqPortargsF32(
			    &mData->mSeqTrackInfo[mSeSequence->mTrack], sound->mTrack, 5,
			    fxmix);
		}
	}

	// Dolby
	f32 dolby;
	if (seParams->mDolby[7].mCurrentValue == -1.0f) {
		if (seParams->mDolbyPointer != nullptr) {
			seParams->mDolby[0].mCurrentValue = *seParams->mDolbyPointer;
		}
		f32 dolbyCenter = JAIGlobalParameter::seDolbyCenterValue / 127.0f;
		dolby           = 0.0f;
		for (int paramIndex = 0; paramIndex < 7; ++paramIndex)
			dolby += seParams->mDolby[paramIndex].mCurrentValue - dolbyCenter;
		dolby += dolbyCenter;
		if (dolby < 0.0f)
			dolby = 0.0f;
		else if (dolby > 1.0f)
			dolby = 1.0f;
	} else {
		dolby = seParams->mDolby[7].mCurrentValue;
	}
	if (trackParams->mDolby != dolby) {
		trackParams->mDolby = dolby;
		if (sound->mState != SOUNDSTATE_Prepared) {
			mData->mSeqTrackInfo[mSeSequence->mTrack]
			    .mTrackPendingFlags[sound->mTrack]
			    |= JASystem::Kernel::TPortArgs::UPDATE_DOLBY;
			JAISystemInterface::setSeqPortargsF32(
			    &mData->mSeqTrackInfo[mSeSequence->mTrack], sound->mTrack, 6,
			    dolby);
		}
	}

	// Final block: U32 param
	if (mData->mSeqTrackInfo[mSeSequence->mTrack]
	        .mTrackPendingFlags[sound->mTrack]
	    != 0) {
		JAISystemInterface::setSeqPortargsU32(
		    &mData->mSeqTrackInfo[mSeSequence->mTrack], sound->mTrack, 1,
		    mData->mSeqTrackInfo[mSeSequence->mTrack]
		        .mTrackPendingFlags[sound->mTrack]);
		mData->mSeqTrackInfo[mSeSequence->mTrack]
		    .mTrackCommands[sound->mTrack]
		    .mCmd.mQueue
		    = nullptr;
		mData->mSeqTrackInfo[mSeSequence->mTrack]
		    .mTrackCommands[sound->mTrack]
		    .mCmd.addPortCmdOnce();
	}
}

void JAIBasic::releaseSeRegist(JAISound* sound)
{
	if (sound->mState != SOUNDSTATE_Stored) {
		JAISystemInterface::writePortApp(
		    mSeSequence->getSeqParameter()->mSeqHandle,
		    (sound->mTrack >> 4) + 0x20000000 + ((sound->mTrack & 0xf) << 4),
		    0);
		mSeSequence->setTrackInterruptSwitch(sound->mTrack, 1);
	}

	clearSeqMuteFromSeStop(sound);

	u8 category;
	u8 slotCount = mData
	                   ->mSeCategoryInfoTable[mSoundScene]
	                                         [(u8)sound->getSeCategoryNumber()]
	                   .mMaxPlaying;
	category = sound->getSeCategoryNumber();
	for (u8 slotIndex = 0; slotIndex < slotCount; ++slotIndex) {
		JAISound** soundHandle = &mData->mSeTrack[category][slotIndex].mSound;
		if (*soundHandle == sound) {
			*soundHandle = nullptr;
			slotIndex    = slotCount;
		}
	}

	sound->clearMainSoundPPointer();
	sound->mState = SOUNDSTATE_Inactive;
	releaseSeParameterPointer(sound->getSeParameter());
	releaseControllerHandle(&mData->mSeRegist[category], sound);
}
