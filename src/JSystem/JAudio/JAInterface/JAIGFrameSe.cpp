#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <math.h>

void JAIBasic::checkNextFrameSe()
{
	JAISound sound;

	struct Candidate {
		/* 0x0 */ u8 state;
		/* 0x4 */ u32 score;
		/* 0x8 */ JAISound* sound;
	};
	Candidate candidates[16];

	s16 prio;
	u8 camStart;
	u8 camEnd;
	JAISound* snd;
	u8 l;
	int i;
	u8 k;
	u8 j;
	u8 bVar7;
	u8 bVar18;
	JAISound::FabricatedPositionInfo* pi;

	f32 fVar6
	    = JAIGlobalParameter::distanceMax * JAIGlobalParameter::distanceMax;
	f32 fVar1 = JAIGlobalParameter::distanceMax / 1000.0f;
	if (fVar1 == 0.0f)
		fVar1 = 1.0f;

	for (i = 0; i < JAIGlobalParameter::getParamSeCategoryMax(); ++i) {
		for (j = 0; j < unk0->mCategoryInfoTable[mSoundScene][i].mMaxPlaying;
		     ++j) {
			candidates[j].score = 0x7fffffff;
			candidates[j].sound = nullptr;
			candidates[j].state = 0xff;
		}

		u8 bVar19 = 0;

		JAISound* it = unk0->mSeRegist[i].mUsedHead;
		while (it) {
			if (it->mState == SOUNDSTATE_Stored && (it->mSoundID & 0xC00)) {
				it->decWait();
			} else if (!(it->mSoundID & 0xC00)
			           && it->mState == SOUNDSTATE_Stopping) {
				sound.mNextSound = it->mNextSound;
				releaseSeRegist(it);
				it = &sound;
			}

			if (it->getWait() == 0) {
				sound.mNextSound = it->mNextSound;
				releaseSeRegist(it);
				it = &sound;
			} else if (it->mState != SOUNDSTATE_Inactive) {
				f32 fVar2 = 2147483647.0f;
				if (it->unk4 == 4) {
					camStart = 0;
					camEnd   = JAIGlobalParameter::audioCameraMax;
				} else {
					camEnd   = it->unk4 + 1;
					camStart = it->unk4;
				}

				for (u8 cam = camStart; cam < camEnd; ++cam) {
					pi = &it->unk1C[cam];

					pi->unkC = pi->unk0;
					if (it->mActorTrans == nullptr) {
						pi->unk0 = JAIConst::dummyZeroVec;
					} else {
						MTXMultVec(mAudioCameras[cam].unk8,
						           (Vec*)it->mActorTrans, &pi->unk0);
					}

					pi->unk18 = pi->unk0.x * pi->unk0.x
					            + pi->unk0.y * pi->unk0.y
					            + pi->unk0.z * pi->unk0.z;
					prio = it->getInfoPriority();
					if (it->getAdjustPriority()) {
						prio += it->getAdjustPriority();
						if (prio < 0)
							prio = 0;
						else if (prio > 0xff)
							prio = 0xff;
					}

					it->unkC
					    = (u32)((0xff - prio) * (0xff - prio) * 0x1690 / fVar1)
					      + (u32)(pi->unk18 / fVar1);
					if (pi->unk0.z > 0.0f)
						it->unkC += (u32)(pi->unk0.z * 6.0f / fVar1);

					if (cam == 0 || pi->unk18 < fVar2)
						fVar2 = pi->unk18;
				}

				if (it->unk4 == 4)
					it->unkC /= JAIGlobalParameter::audioCameraMax;

				f32 fVar3;
				if (it->getSwBit() & 0x20)
					fVar3 = fVar6;
				else
					fVar3 = 1e+10f;

				if (fVar2 > fVar3) {
					if (!(it->mSoundID & 0xC00)) {
						if (it->mState != SOUNDSTATE_Stored) {
							JAISystemInterface::writePortApp(
							    mSeSequence->getSeqParameter()->mSeqHandle,
							    (it->mTrack >> 4) + 0x20000000
							        + ((it->mTrack & 0xf) << 4),
							    0);
							mSeSequence->setTrackInterruptSwitch(it->mTrack, 1);
						}
						it->mState = SOUNDSTATE_Stored;
					} else {
						sound.mNextSound = it->mNextSound;
						stopSoundHandle(it, 0);
						it = &sound;
					}
				} else {
					bVar18 = unk0->mCategoryInfoTable
					             [mSoundScene][(u8)it->getSeCategoryNumber()]
					                 .mMaxPlaying;
					for (j = 0; j < bVar18; ++j) {
						if (it->unkC < candidates[j].score
						    || (it->unkC == candidates[j].score
						        && candidates[j].state >= it->mState)) {
							if (bVar19 < bVar18)
								++bVar19;
							for (k = bVar18 - 1; k > j; --k) {
								candidates[k].score = candidates[k - 1].score;
								candidates[k].sound = candidates[k - 1].sound;
								candidates[j].state = candidates[k - 1].state;
							}
							candidates[j].score = it->unkC;
							candidates[j].sound = it;
							candidates[j].state = it->mState;

							j = bVar18;
						}
					}
				}
			}
			if (it != nullptr)
				it = it->mNextSound;
		}

		for (k = 0; k < bVar19; ++k) {
			snd = candidates[k].sound;
			if (snd->mState == SOUNDSTATE_Stored) {
				snd->mState = SOUNDSTATE_Prepared;
			} else if (snd->mState == SOUNDSTATE_Playing) {
				snd->mState = SOUNDSTATE_Started;
			}
		}

		bVar19 = unk0->mCategoryInfoTable[mSoundScene][i].mMaxPlaying;
		for (j = 0; j < bVar19; ++j) {
			snd   = unk0->mSeTrack[i][j].mSound;
			bVar7 = 0;
			if (snd == nullptr) {
				bVar7 = 1;
			} else if (snd->mState == SOUNDSTATE_Playing) {
				if (snd->mSoundID & 0xC00) {
					releaseSeRegist(snd);
				} else {
					snd->mState     = SOUNDSTATE_Stored;
					snd->mWaitTimer = 0;
				}
				bVar7 = 1;
			} else if (snd->mState == SOUNDSTATE_Inactive) {
				unk0->mSeTrack[i][j].mSound = nullptr;
				bVar7                       = 1;
			} else {
				for (k = 0; k < bVar19; ++k) {
					if (unk0->mSeTrack[i][j].mSound == candidates[k].sound) {
						candidates[k].sound = nullptr;
						k                   = bVar19;
					}
				}
			}

			if (bVar7 == 1) {
				for (k = 0; k < bVar19; ++k) {
					snd = candidates[k].sound;
					if (snd != nullptr && snd->mState != SOUNDSTATE_Started) {
						for (l = 0; l < bVar19; ++l) {
							if (unk0->mSeTrack[i][l].mSound
							    && snd == unk0->mSeTrack[i][l].mSound) {
								bVar7 = 0;
								l     = bVar19;
							}
						}

						if (bVar7 == 1) {
							unk0->mSeTrack[i][j].mSound = snd;
							candidates[k].sound         = nullptr;
							k                           = bVar19 + 1;
						}
					}
				}
				if (k == bVar19) {
					unk0->mSeTrack[i][j].mSound = nullptr;
				}
			}
		}
	}
}

void JAIBasic::sendPlayingSeCommand()
{
	u16 readStatus0;
	u16 readStatus1;
	u8 j;
	JAISound* sound;
	u8 trackId = 0;

	for (u8 cat = 0; cat < JAIGlobalParameter::getParamSeCategoryMax(); ++cat) {
		for (j = 0;
		     j < unk0->mCategoryInfoTable[mSoundScene][(u8)cat].mMaxPlaying;
		     ++trackId, ++j) {
			sound = unk0->mSeTrack[cat][j].mSound;
			if (sound == nullptr)
				continue;

			sound->incPlayGameFrameCounter();

			u32 portAddr
			    = ((trackId >> 4) & 0xF) + 0x20000000 + ((trackId & 0xF) << 4);

			u32 seqPort = mSeSequence->getSeqParameter()->mSeqHandle;

			JAISystemInterface::readPortApp(seqPort, portAddr + 0x20000,
			                                &readStatus0);
			JAISystemInterface::readPortApp(seqPort, portAddr, &readStatus1);

			JAISound::FabricatedPositionInfo* infos = sound->unk1C;
			for (u8 k = 0; k < JAIGlobalParameter::audioCameraMax; ++k) {
				f32* dPtr = &infos[k].unk18;
				*dPtr     = std::sqrtf(*dPtr);
			}

			u8 state = sound->mState;
			if (state == SOUNDSTATE_Prepared) {
				u32 swBit     = sound->getSwBit();
				sound->mTrack = trackId;
				if (swBit & 8) {
					setSeqMuteFromSeStart(sound);
				}
				if (swBit & 0xC0) {
					s32 rnd = (s32)(255.0f * JAIConst::random.get_ufloat_1());
					switch (swBit & 0xC0) {
					case 0x40:
						sound->setRandom(rnd & 0xF);
						break;
					case 0x80:
						sound->setRandom(rnd & 0x1F);
						break;
					case 0xC0:
						sound->setRandom(rnd & 0x3F);
						break;
					default:
						sound->setRandom(0);
						break;
					}
				}

				u16* portMask = &sound->getSeParameter()->mPortUpdate;
				for (u8 i = 0; *portMask != 0; ++i) {
					u32 bit = 1 << i;
					if (*portMask & bit) {
						mSeSequence->setTrackPortData(
						    sound->mTrack, i,
						    sound->getSeParameter()->mPortData[i]);
						*portMask ^= bit;
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

				u16 portValue = sound->mSoundID & 0x3FF;
				if (sound->checkSwBit(0x800)) {
					u32 tmp = sound->mActorGroundNumber;
					portValue += getMapInfoGround(tmp);
				}

				u16 distArg;
				if (JAIGlobalParameter::audioCameraMax == 1
				    && sound->checkSwBit(0x1000)) {
					if (sound->unk1C[0].unk18
					    < JAIGlobalParameter::distanceMax) {
						distArg = JAIGlobalParameter::seDistanceWaitMax
						          * (u32)sound->unk1C[0].unk18
						          / (u32)JAIGlobalParameter::distanceMax;
					} else {
						distArg = JAIGlobalParameter::seDistanceWaitMax;
					}
				} else {
					distArg = 0;
				}

				JAISystemInterface::writePortApp(seqPort, portAddr + 0x30000,
				                                 distArg);
				JAISystemInterface::writePortApp(
				    seqPort, portAddr + 0x60000,
				    getMapInfoFxline(sound->mActorGroundNumber));
				JAISystemInterface::writePortApp(seqPort, portAddr + 0x40000,
				                                 portValue);
				JAISystemInterface::writePortApp(seqPort, portAddr, 1);

				if (sound->mSoundID & 0xC00) {
					sound->mState = SOUNDSTATE_Playing;
				} else {
					sound->mState = SOUNDSTATE_Stopping;
				}
			} else if (readStatus0 == 0 && readStatus1 != 1) {
				releaseSeRegist(sound);
			} else if (sound->mFadeCounter != 0) {
				if (sound->getSeParameter()->mVolume[6].mCurrentValue != 0.0f) {
					sound->setSeDistanceParameters();
					sendSeAllParameter(sound);
					if (sound->mSoundID & 0xC00) {
						sound->mState = SOUNDSTATE_Playing;
					} else {
						sound->mState = SOUNDSTATE_Stopping;
					}
				} else {
					releaseSeRegist(sound);
				}
			} else if (state == SOUNDSTATE_Started) {
				sound->setSeDistanceParameters();
				sendSeAllParameter(sound);
				if (sound->mSoundID & 0xC00) {
					sound->mState = SOUNDSTATE_Playing;
				} else {
					sound->mState = SOUNDSTATE_Stopping;
				}
			}
		}
	}
}

void JAIBasic::setSeqMuteFromSeStart(JAISound* param_1)
{
	for (u32 i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound* sound = unk0->mSeqTrackInfo[i].mSound;
		if (i != mSeSequence->mTrack && sound && !(sound->getSwBit() & 8)) {
			sound->setSeqInterVolume(
			    9, JAIGlobalParameter::seqMuteVolumeSePlay / 127.0f,
			    JAIGlobalParameter::seqMuteMoveSpeedSePlay);
			unk30 |= 1 << param_1->mTrack;
		}
	}
}

void JAIBasic::clearSeqMuteFromSeStop(JAISound* sound)
{
	if (unk30 == 0 || !(sound->getSwBit() & 8))
		return;

	for (u32 i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound* seq = unk0->mSeqTrackInfo[i].mSound;
		if (i != mSeSequence->mTrack && seq && !(seq->getSwBit() & 8)) {
			unk30 &= (1 << sound->mTrack) ^ 0xffffffff;
			if (unk30 == 0) {
				seq->setSeqInterVolume(
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
		for (JAISound* it = unk0->mSeRegist[i].mUsedHead; it != nullptr;
		     it           = it->mNextSound) {
			unk0->setSeMovePara(it->getSeParameter()->mVolume);
			unk0->setSeMovePara(it->getSeParameter()->mPan);
			unk0->setSeMovePara(it->getSeParameter()->mFxmix);
			unk0->setSeMovePara(it->getSeParameter()->mFir);
			unk0->setSeMovePara(it->getSeParameter()->mDolby);
			unk0->setSeMovePara(it->getSeParameter()->mPitch);
		}
	}
}

void JAIBasic::sendSeAllParameter(JAISound* sound)
{
	JAIData::FabricatedSeTrackParameter* slot = &unk0->unk0[sound->mTrack];
	JAISeParameter* seParam                   = sound->getSeParameter();

	// Volume
	f32 vol;
	if (seParam->mVolume[7].mCurrentValue == -1.0f) {
		if (seParam->mVolumePointer != nullptr) {
			seParam->mVolume[0].mCurrentValue = *seParam->mVolumePointer;
		}
		vol = 1.0f;
		for (int i = 0; i < 7; ++i)
			vol *= seParam->mVolume[i].mCurrentValue;
	} else {
		vol = seParam->mVolume[7].mCurrentValue;
	}
	vol *= mSeCategoryVolume[(u8)sound->getSeCategoryNumber()];
	if (slot->mVolume != vol) {
		slot->mVolume = vol;
		if (sound->mState != SOUNDSTATE_Prepared) {
			unk0->mSeqTrackInfo[mSeSequence->mTrack].mTrackUpdate[sound->mTrack]
			    |= 0x1;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->mSeqTrackInfo[mSeSequence->mTrack], sound->mTrack, 2,
			    vol);
		}
	}

	// Pan
	f32 pan;
	if (seParam->mPan[7].mCurrentValue == -1.0f) {
		if (seParam->mPanPointer != nullptr) {
			seParam->mPan[0].mCurrentValue = *seParam->mPanPointer;
		}
		pan = 0.0f;
		for (int i = 0; i < 7; ++i)
			if (seParam->mPan[i].mCurrentValue != 0.5f)
				pan += seParam->mPan[i].mCurrentValue - 0.5f;
		pan += 0.5f;
		if (pan < 0.0f)
			pan = 0.0f;
		else if (pan > 1.0f)
			pan = 1.0f;
	} else {
		pan = seParam->mPan[7].mCurrentValue;
	}
	if (slot->mPan != pan) {
		slot->mPan = pan;
		if (sound->mState != SOUNDSTATE_Prepared) {
			unk0->mSeqTrackInfo[mSeSequence->mTrack].mTrackUpdate[sound->mTrack]
			    |= 0x4;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->mSeqTrackInfo[mSeSequence->mTrack], sound->mTrack, 4,
			    pan);
		}
	}

	// Pitch
	f32 pitch;
	if (seParam->mPitch[7].mCurrentValue == -1.0f) {
		if (seParam->mPitchPointer != nullptr) {
			seParam->mPitch[0].mCurrentValue = *seParam->mPitchPointer;
		}
		pitch = 1.0f;
		for (int i = 0; i < 7; ++i)
			pitch *= seParam->mPitch[i].mCurrentValue;
	} else {
		pitch = seParam->mPitch[7].mCurrentValue;
	}
	if (slot->mPitch != pitch) {
		slot->mPitch = pitch;
		if (sound->mState != SOUNDSTATE_Prepared) {
			unk0->mSeqTrackInfo[mSeSequence->mTrack].mTrackUpdate[sound->mTrack]
			    |= 0x2;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->mSeqTrackInfo[mSeSequence->mTrack], sound->mTrack, 3,
			    pitch);
		}
	}

	// FxMix
	f32 fxmix;
	if (seParam->mFxmix[7].mCurrentValue == -1.0f) {
		if (seParam->mFxmixPointer != nullptr) {
			seParam->mFxmix[0].mCurrentValue = *seParam->mFxmixPointer;
		}
		fxmix = 0.0f;
		for (int i = 0; i < 7; ++i)
			fxmix += seParam->mFxmix[i].mCurrentValue;
	} else {
		fxmix = seParam->mFxmix[7].mCurrentValue;
	}
	if (slot->mFxmix != fxmix) {
		slot->mFxmix = fxmix;
		if (sound->mState != SOUNDSTATE_Prepared) {
			unk0->mSeqTrackInfo[mSeSequence->mTrack].mTrackUpdate[sound->mTrack]
			    |= 0x8;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->mSeqTrackInfo[mSeSequence->mTrack], sound->mTrack, 5,
			    fxmix);
		}
	}

	// Dolby
	f32 dolby;
	if (seParam->mDolby[7].mCurrentValue == -1.0f) {
		if (seParam->mDolbyPointer != nullptr) {
			seParam->mDolby[0].mCurrentValue = *seParam->mDolbyPointer;
		}
		f32 center = JAIGlobalParameter::seDolbyCenterValue / 127.0f;
		dolby      = 0.0f;
		for (int i = 0; i < 7; ++i)
			dolby += seParam->mDolby[i].mCurrentValue - center;
		dolby += center;
		if (dolby < 0.0f)
			dolby = 0.0f;
		else if (dolby > 1.0f)
			dolby = 1.0f;
	} else {
		dolby = seParam->mDolby[7].mCurrentValue;
	}
	if (slot->mDolby != dolby) {
		slot->mDolby = dolby;
		if (sound->mState != SOUNDSTATE_Prepared) {
			unk0->mSeqTrackInfo[mSeSequence->mTrack].mTrackUpdate[sound->mTrack]
			    |= 0x10;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->mSeqTrackInfo[mSeSequence->mTrack], sound->mTrack, 6,
			    dolby);
		}
	}

	// Final block: U32 param
	if (unk0->mSeqTrackInfo[mSeSequence->mTrack].mTrackUpdate[sound->mTrack]
	    != 0) {
		JAISystemInterface::setSeqPortargsU32(
		    &unk0->mSeqTrackInfo[mSeSequence->mTrack], sound->mTrack, 1,
		    unk0->mSeqTrackInfo[mSeSequence->mTrack]
		        .mTrackUpdate[sound->mTrack]);
		unk0->mSeqTrackInfo[mSeSequence->mTrack]
		    .unk4C[sound->mTrack]
		    .unk2C.mHead
		    = 0;
		unk0->mSeqTrackInfo[mSeSequence->mTrack]
		    .unk4C[sound->mTrack]
		    .unk2C.addPortCmdOnce();
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

	u8 cat;
	u8 maxCount = unk0->mCategoryInfoTable[mSoundScene]
	                                      [(u8)sound->getSeCategoryNumber()]
	                                          .mMaxPlaying;
	cat = sound->getSeCategoryNumber();
	for (u8 j = 0; j < maxCount; ++j) {
		JAISound** slot = &unk0->mSeTrack[cat][j].mSound;
		if (*slot == sound) {
			*slot = nullptr;
			j     = maxCount;
		}
	}

	sound->clearMainSoundPPointer();
	sound->mState = SOUNDSTATE_Inactive;
	releaseSeParameterPointer(sound->getSeParameter());
	releaseControllerHandle(&unk0->mSeRegist[cat], sound);
}
