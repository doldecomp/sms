#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>
#include <JSystem/JAudio/JASystem/JASVload.hpp>
#include <math.h>

void JAIBasic::stopSeq(JAISound* param_1)
{
	if (param_1->getSwBit() & 1) {
		for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
			JAISound* sound = unk0->mSeqTrackInfo[i].mSound;
			if (param_1 != sound && sound) {
				if (sound->mState >= SOUNDSTATE_Started
				    && !(sound->getSwBit() & 2)) {
					sound->setSeqInterVolume(10, 1.0f, 10);
					JASystem::TrackMgr::handleToSeq(
					    sound->getSeqParameter()->mSeqHandle)
					    ->unPauseTrackAll();
				}
			}
		}
	}

	param_1->setMainSoundPPointer(nullptr);
	param_1->getSeqParameter()->mUpdateData = nullptr;
	if (param_1->getStatus() >= SOUNDSTATE_Started) {
		unk0->releaseAutoHeapPointer(
		    param_1->getSeqParameter()->mAutoHeapPosition);
	}
	param_1->mState = SOUNDSTATE_Inactive;
	releaseSeqParameterPointer(param_1->getSeqParameter());
	releaseControllerHandle(&unk0->mSeqControlBuffer, param_1);
	unk0->mSeqTrackInfo[param_1->mTrack].mSound = nullptr;
}

void JAIBasic::checkEntriedSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISeqUpdateData* sud = &unk0->mSeqTrackInfo[i];
		JAISoundHandle& sound = unk0->mSeqTrackInfo[i].mSound;
		u32& r27              = unk0->mSeqTrackInfo[i].unk8;

		if (!sound)
			continue;

		if (!(r27 & 1))
			continue;

		if (sud->unk3 != 0)
			return;

		u32 size = JASystem::Vload::checkSize(
		    mSeqArchiveHandle + (sound->mSoundID & JAISoundID_IndexMask));

		u8 pos;
		u8* ptr = (u8*)unk0->checkOnMemory(
		    sound->mSoundID & JAISoundID_IndexMask, &pos);

		if (ptr == nullptr) {
			if (sound->checkSwBit(0x10)) {
				ptr = unk0->getFreeStayHeapPointer(
				    size, sound->mSoundID & JAISoundID_IndexMask);
				pos                                         = 0xFF;
				sound->getSeqParameter()->mAutoHeapPosition = 0xFF;
				if (ptr == nullptr) {
					sound->checkSwBit(0x20);
				}
			}

			if (ptr == nullptr) {
				if (sound->checkSwBit(0x20) || !sound->checkSwBit(0x10)) {
					pos = unk0->checkUsefulAutoHeapPosition();
					if (pos >= JAIGlobalParameter::autoHeapMax) {
						for (int ii = 0; ii < JAIGlobalParameter::autoHeapMax;
						     ++ii) {
							if (unk0->mAutoHeap[ii].mUseOrder == -1)
								continue;

							u32 j;
							for (j = 0; j < JAIGlobalParameter::seqPlayTrackMax;
							     ++j) {
								if (unk0->mSeqTrackInfo[j].mSound
								    && unk0->mAutoHeap[ii].mSeqNumber
								           == (u8)unk0->mSeqTrackInfo[j]
								                  .mSound->mSoundID) {
									j = JAIGlobalParameter::seqPlayTrackMax;
								}
							}
							if (j == JAIGlobalParameter::seqPlayTrackMax) {
								unk0->releaseAutoHeapPointer(ii);
								pos = ii;
							}
						}
						if (pos >= JAIGlobalParameter::autoHeapMax) {
							sound->stop(0);
							return;
						}
					} else if (size >= JAIGlobalParameter::autoHeapRoomSize) {
						sound->stop(0);
						return;
					}

					sound->getSeqParameter()->mAutoHeapPosition = pos;
					ptr = (u8*)unk0->getFreeAutoHeapPointer(
					    sound->getSeqParameter()->mAutoHeapPosition,
					    sound->mSoundID & JAISoundID_IndexMask);
				}
			}

			if (!sound->checkSwBit(0x40)) {
				sound->mState = SOUNDSTATE_Stored;

				u32 swBit8 = sound->mSoundID;
				// Keep the signed, two-step packing for MWCC register
				// allocation.
				s32 param = pos << 8;
				param = (i | ((swBit8 & JAISoundID_IndexMask) << 16)) | param;

				unk0->setAutoHeapLoadedFlag(pos, 1);
				JASystem::Vload::loadFileAsync(
				    mSeqArchiveHandle + (swBit8 & JAISoundID_IndexMask), ptr, 0,
				    size, checkDvdLoadArc, param);
				sud->unk3 = 1;
			} else {
				JASystem::Vload::loadFile(
				    mSeqArchiveHandle
				        + (sound->mSoundID & JAISoundID_IndexMask),
				    ptr, 0, size);
				sound->mState = SOUNDSTATE_Prepared;
			}
		} else {
			if (ptr == (u8*)0xFFFFFFFF)
				return;

			JAISound* snd = sound;
			if (pos != 0xFF) {
				unk0->getFreeAutoHeapPointer(pos, snd->mSoundID
				                                      & JAISoundID_IndexMask);
			}

			sound->getSeqParameter()->mAutoHeapPosition = pos;

			sound->mState = SOUNDSTATE_Prepared;
		}

		if (ptr != nullptr) {
			sud->mSeqData = ptr;
			r27 ^= 1;
		} else {
			stopSeq(sound);
		}
	}
}

void JAIBasic::checkPlayingSeqTrack(u32 trackID)
{
	JAISeqUpdateData* sud = &unk0->mSeqTrackInfo[trackID];
	JAISoundHandle& sound = sud->mSound;
	if (sound->getSeqParameter()->mPauseMode == 2)
		return;

	u32& r30 = sud->unk8;

	JAISeqParameter* seqParam = sound->getSeqParameter();
	u32* portFlags            = sud->mTrackUpdate;

	for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax + 1; ++j)
		portFlags[j] = 0;

	if (r30 & 2) {
		JAISound* snd = sound;
		if (snd->mFadeCounter == 0 || snd->mState < SOUNDSTATE_Playing) {
			if (snd->mState >= SOUNDSTATE_Started) {
				JAISystemInterface::stopSeq(seqParam->mSeqHandle);
			}
			sound->clearMainSoundPPointer();
			stopSeq(sound);
			r30 = 0;
			return;
		} else {
			snd->setSeqInterVolume(6, 0.0f, snd->mFadeCounter);
			sound->mState = SOUNDSTATE_Stopping;
			r30 ^= 2;
		}
	}

	if (sound != nullptr && sound->mActor != 0) {
		u32 s, e;
		u32 i;
		if (sound->mCameraIdx == 4) {
			s = 0;
			e = JAIGlobalParameter::audioCameraMax;
		} else {
			s = sound->mCameraIdx;
			e = sound->mCameraIdx + 1;
		}

		for (i = s; i < e; ++i) {
			JAISound::FabricatedPositionInfo* pi = &sound->unk1C[i];

			pi->mPrevCamSpacePos = pi->mCamSpacePos;
			MTXMultVec(mAudioCameras[i].nViewMtx, (Vec*)sound->mActorTrans,
			           &pi->mCamSpacePos);

			pi->unk18 = std::sqrtf(pi->mCamSpacePos.x * pi->mCamSpacePos.x
			                       + pi->mCamSpacePos.y * pi->mCamSpacePos.y
			                       + pi->mCamSpacePos.z * pi->mCamSpacePos.z);

			sound->setSeqInterVolume(
			    4,
			    (f32)(u8)(127.0f
			              * sound->setDistanceVolumeCommon(
			                  JAIGlobalParameter::distanceMax, 0)),
			    JAIGlobalParameter::distanceParameterMoveTime);

			sound->setSeqInterPan(
			    4, (f32)(u8)sound->setDistancePanCommon(),
			    JAIGlobalParameter::distanceParameterMoveTime);

			sound->setSeqInterPitch(4, sound->setPositionDopplarCommon(0x100),
			                        JAIGlobalParameter::dopplarMoveTime);
		}
	}

	if (sound != nullptr)
		sound->incPlayGameFrameCounter();

	if (r30 == 0)
		return;

	u8 envMax = JAIGlobalParameter::seqPlayTrackMax + 12;

	if (r30 & 0x40000) {
		f32 vol = 1.0f;
		for (u8 j = 0; j < envMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->mVolume[j];
			if (seqParam->mVolumeUpdate & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->mVolumeUpdate ^= 1 << j;
				}
			}
			vol *= mps->mCurrentValue;
		}
		if (sud->mSeqVolume != vol) {
			sud->mSeqVolume = vol;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->mSeqTrackInfo[trackID], JAIGlobalParameter::seqTrackMax,
			    2, vol);
			portFlags[JAIGlobalParameter::seqTrackMax] |= 1;
		}
		if (seqParam->mVolumeUpdate == 0) {
			r30 ^= 0x40000;
		}
	}

	if (r30 & 0x80000) {
		f32 pan = 0.0f;
		for (u8 j = 0; j < envMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->mPan[j];
			if (seqParam->mPanUpdate & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->mPanUpdate ^= 1 << j;
				}
			}
			pan += mps->mCurrentValue - 0.5f;
		}
		pan += 0.5f;
		if (pan > 1.0f)
			pan = 1.0f;
		else if (pan < 0.0f)
			pan = 0.0f;
		if (sud->mSeqPan != pan) {
			sud->mSeqPan = pan;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->mSeqTrackInfo[trackID], JAIGlobalParameter::seqTrackMax,
			    4, pan);
			portFlags[JAIGlobalParameter::seqTrackMax] |= 4;
		}
		if (seqParam->mPanUpdate == 0) {
			r30 ^= 0x80000;
		}
	}

	if (r30 & 0x100000) {
		f32 pitch = 1.0f;
		for (u8 j = 0; j < envMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->mPitch[j];
			if (seqParam->mPitchUpdate & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->mPitchUpdate ^= 1 << j;
				}
			}
			pitch *= mps->mCurrentValue;
		}
		if (sud->mSeqPitch != pitch) {
			sud->mSeqPitch = pitch;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->mSeqTrackInfo[trackID], JAIGlobalParameter::seqTrackMax,
			    3, pitch);
			portFlags[JAIGlobalParameter::seqTrackMax] |= 2;
		}
		if (seqParam->mPitchUpdate == 0) {
			r30 ^= 0x100000;
		}
	}

	if (r30 & 0x200000) {
		f32 fxmix = 0.0f;
		for (u8 j = 0; j < envMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->mFxmix[j];
			if (seqParam->mFxmixUpdate & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->mFxmixUpdate ^= 1 << j;
				}
				fxmix += mps->mCurrentValue;
			}
		}
		if (fxmix > 1.0f)
			fxmix = 1.0f;
		if (sud->mSeqFxmix != fxmix) {
			sud->mSeqFxmix = fxmix;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->mSeqTrackInfo[trackID], JAIGlobalParameter::seqTrackMax,
			    5, fxmix);
			portFlags[JAIGlobalParameter::seqTrackMax] |= 8;
		}
		if (seqParam->mFxmixUpdate == 0) {
			r30 ^= 0x200000;
		}
	}

	if (r30 & 0x400000) {
		f32 dolby = 1.0f;
		for (u8 j = 0; j < envMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->mDolby[j];
			if (seqParam->mDolbyUpdate & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->mDolbyUpdate ^= 1 << j;
				}
				dolby *= mps->mCurrentValue;
			}
		}
		if (sud->mSeqDolby != dolby) {
			sud->mSeqDolby = dolby;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->mSeqTrackInfo[trackID], JAIGlobalParameter::seqTrackMax,
			    6, dolby);
			portFlags[JAIGlobalParameter::seqTrackMax] |= 0x10;
		}
		if (seqParam->mDolbyUpdate == 0) {
			r30 ^= 0x400000;
		}
	}

	if (r30 & 4) {
		if (!unk0->moveParameter(&seqParam->mTempo)) {
			r30 ^= 4;
		}
		if (sud->mSeqTempo != seqParam->mTempo.mCurrentValue) {
			sud->mSeqTempo = seqParam->mTempo.mCurrentValue;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->mSeqTrackInfo[trackID], JAIGlobalParameter::seqTrackMax,
			    9, seqParam->mTempo.mCurrentValue);
			portFlags[JAIGlobalParameter::seqTrackMax] |= 0x80;
		}
	}

	if (r30 & 0x10) {
		for (u8 j = 0; j < 16; ++j) {
			if (seqParam->mPortUpdate & (1 << j))
				if (!unk0->moveParameter(&seqParam->mPortData[j]))
					seqParam->mPortUpdate ^= (1 << j);

			u16 readVal;
			JAISystemInterface::readPortApp(seqParam->mSeqHandle, j << 16,
			                                &readVal);
			if (readVal != (u16)seqParam->mPortData[j].mCurrentValue)
				JAISystemInterface::writePortApp(
				    seqParam->mSeqHandle, j << 16,
				    seqParam->mPortData[j].mCurrentValue);
		}
		if (seqParam->mPortUpdate == 0)
			r30 ^= 0x10;
	}

	if (r30 & 0x40) {
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->mTrackVolume[j];
			if (seqParam->mTrackVolumeUpdate & (1 << j)) {
				if (!unk0->moveParameter(mps))
					seqParam->mTrackVolumeUpdate ^= 1 << j;

				if (mps->mCurrentValue != sud->mTrackVolume[j]) {
					sud->mTrackVolume[j] = mps->mCurrentValue;
					portFlags[j] |= 1;
					JAISystemInterface::setSeqPortargsF32(
					    &unk0->mSeqTrackInfo[trackID], j, 2,
					    mps->mCurrentValue);
				}
			}
		}
		if (seqParam->mTrackVolumeUpdate == 0)
			r30 ^= 0x40;
	}

	if (r30 & 0x20) {
		r30 ^= 0x20;
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			if (seqParam->mMuteBits[j].mEnable == 1
			    && seqParam->mMuteBits[j].mCurrent
			           != seqParam->mMuteBits[j].mTarget) {
				JASystem::TTrack* tt
				    = JAISystemInterface::trackToSeqp(sound, j);
				if (tt != nullptr)
					tt->muteTrack(seqParam->mMuteBits[j].mTarget);
				seqParam->mMuteBits[j].mCurrent
				    = seqParam->mMuteBits[j].mTarget;
			}
		}
	}

	if (r30 & 0x80) {
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->mTrackPan[j];
			if (seqParam->mTrackPanUpdate & (1 << j)) {
				if (!unk0->moveParameter(mps))
					seqParam->mTrackPanUpdate ^= 1 << j;

				if (mps->mCurrentValue != sud->mTrackPan[j]) {
					sud->mTrackPan[j] = mps->mCurrentValue;
					portFlags[j] |= 4;
					JAISystemInterface::setSeqPortargsF32(
					    &unk0->mSeqTrackInfo[trackID], j, 4,
					    seqParam->mTrackPan[j].mCurrentValue);
				}
			}
		}
		if (seqParam->mTrackPanUpdate == 0)
			r30 ^= 0x80;
	}

	if (r30 & 0x200) {
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->mTrackPitch[j];
			if (seqParam->mTrackPitchUpdate & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->mTrackPitchUpdate ^= 1 << j;
				}
				if (mps->mCurrentValue != sud->mTrackPitch[j]) {
					sud->mTrackPitch[j] = mps->mCurrentValue;
					portFlags[j] |= 2;
					JAISystemInterface::setSeqPortargsF32(
					    &unk0->mSeqTrackInfo[trackID], j, 3,
					    seqParam->mTrackPitch[j].mCurrentValue);
				}
			}
		}
		if (seqParam->mTrackPitchUpdate == 0)
			r30 ^= 0x200;
	}

	if (r30 & 0x800) {
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->mTrackFxmix[j];
			if (seqParam->mTrackFxmixUpdate & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->mTrackFxmixUpdate ^= 1 << j;
				}
				if (mps->mCurrentValue != sud->mTrackFxmix[j]) {
					sud->mTrackFxmix[j] = mps->mCurrentValue;
					portFlags[j] |= 8;
					JAISystemInterface::setSeqPortargsF32(
					    &unk0->mSeqTrackInfo[trackID], j, 5,
					    seqParam->mTrackFxmix[j].mCurrentValue);
				}
			}
		}
		if (seqParam->mTrackFxmixUpdate == 0)
			r30 ^= 0x800;
	}

	if (r30 & 0x100) {
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->mTrackDolby[j];
			if (seqParam->mTrackDolbyUpdate & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->mTrackDolbyUpdate ^= 1 << j;
				}
				if (mps->mCurrentValue != sud->mTrackDolby[j]) {
					sud->mTrackDolby[j] = mps->mCurrentValue;
					portFlags[j] |= 0x10;
					JAISystemInterface::setSeqPortargsF32(
					    &unk0->mSeqTrackInfo[trackID], j, 6,
					    seqParam->mTrackDolby[j].mCurrentValue);
				}
			}
		}
		if (seqParam->mTrackDolbyUpdate == 0)
			r30 ^= 0x100;
	}

	if (r30 & 0x800000) {
		r30 ^= 0x800000;
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			u8* sw = &seqParam->mTrackInterruptSwitch[j];
			if (*sw == 1) {
				portFlags[j] |= 0x40;
				JAISystemInterface::setSeqPortargsU32(
				    &unk0->mSeqTrackInfo[trackID], j, 8, 1);
				*sw = 0;
			}
		}
	}

	if (r30 & 0x1000) {
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			u16* ports = seqParam->mTrackPortData[j];
			if (seqParam->mTrackPortUpdate & (1 << j)) {
				seqParam->mTrackPortUpdate ^= 1 << j;
				for (u8 k = 0; k < 16; ++k) {
					if (seqParam->mTrackPortDataUpdate[j] & (1 << k)) {
						JAISystemInterface::writePortApp(
						    seqParam->mSeqHandle,
						    sound->getTrackPortRoute(j, k), ports[k]);
						seqParam->mTrackPortDataUpdate[j] ^= 1 << k;
					}
				}
			}
		}
		if (seqParam->mTrackPortUpdate == 0)
			r30 ^= 0x1000;
	}
}

void JAIBasic::checkPlayingSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISoundHandle& sound = unk0->mSeqTrackInfo[i].mSound;
		if (sound && sound->mState >= SOUNDSTATE_Playing) {
			checkPlayingSeqTrack(i);
			for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax + 1; ++j) {
				if (unk0->mSeqTrackInfo[i].mTrackUpdate[j] != 0) {
					JAISystemInterface::setSeqPortargsU32(
					    &unk0->mSeqTrackInfo[i], j, 1,
					    unk0->mSeqTrackInfo[i].mTrackUpdate[j]);

					unk0->mSeqTrackInfo[i].unk4C[j].unk2C.mHead = nullptr;
					unk0->mSeqTrackInfo[i].unk4C[j].unk2C.addPortCmdOnce();
				}
			}
		}
	}
}

void JAIBasic::checkStoppedSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISoundHandle& sound = unk0->mSeqTrackInfo[i].mSound;

		if (!sound)
			continue;

		if (sound->mState == SOUNDSTATE_Playing
		    || sound->mState == SOUNDSTATE_Stopping) {
			u8 flag = JAISystemInterface::checkSeqActiveFlag(
			    sound->getSeqParameter()->mSeqHandle);
			if (flag == 0) {
				sound->clearMainSoundPPointer();
				stopSeq(sound);
				unk0->mSeqTrackInfo[i].unk8 = 0;
			}
		}
	}
}

void JAIBasic::checkStartedSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISoundHandle& sound = unk0->mSeqTrackInfo[i].mSound;

		if (!sound)
			continue;

		if (sound->mState == SOUNDSTATE_Started) {
			u8 flag = JAISystemInterface::checkSeqActiveFlag(
			    sound->getSeqParameter()->mSeqHandle);
			if (flag != 0) {
				sound->mState = SOUNDSTATE_Playing;
				JAISystemInterface::trackInit(&unk0->mSeqTrackInfo[i]);
			}
		}
	}
}

void JAIBasic::checkFadeoutSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISoundHandle& sound = unk0->mSeqTrackInfo[i].mSound;

		if (!sound)
			continue;

		if (sound->mState == SOUNDSTATE_Stopping
		    && sound->getSeqInterVolume(6) == 0.0f) {
			JAISystemInterface::stopSeq(sound->getSeqParameter()->mSeqHandle);
			sound->clearMainSoundPPointer();
			stopSeq(sound);
			unk0->mSeqTrackInfo[i].unk8 = 0;
		}
	}
}

void JAIBasic::checkReadSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISeqUpdateData* sud = &unk0->mSeqTrackInfo[i];
		JAISoundHandle& sound = sud->mSound;
		if (!sound)
			continue;
		if (sound->mState != SOUNDSTATE_Prepared)
			continue;
		if (sound->getSeqParameter()->mWaitSceneSet != 0xffffffff)
			continue;
		if (sound->getSeqParameter()->mUpdateData->unk2 != 0)
			continue;

		u32 lVar2 = JASystem::Vload::checkSize(
		    mSeqArchiveHandle + (sound->mSoundID & JAISoundID_IndexMask));
		int uVar3 = JAISystemInterface::setSeqData(
		    nullptr, sud->mSeqData, lVar2,
		    JASystem::Player::SEQ_PLAYMODE_UNK_0);

		sound->getSeqParameter()->mSeqHandle = uVar3;
		sound->getSeqParameter();
		if (sound->getSeqParameter()->mSeqHandle != -1) {
			unk0->initSeqTrackInfoParameter(sound->mTrack);
			sound->mState = SOUNDSTATE_Started;
			if (sound->mFadeCounter > 1) {
				sound->setSeqInterVolume(6, 0.0f, 0);
				sound->setSeqInterVolume(6, 1.0f, sound->mFadeCounter);
			}
			if (sud->unk0 != 0) {
				sound->setPauseMode(sud->unk0, sud->unk1);
				sud->mSeqVolume = 1.1f;
			}
			setSeExtParameter(sound);
			checkPlayingSeqTrack(i);
			if (sound != nullptr) {
				JAISystemInterface::rootInit(sud);
				JAISystemInterface::startSeq(
				    sound->getSeqParameter()->mSeqHandle);
			}
		} else {
			JAISound* snd = sound;
			stopSeq(snd);
		}
	}
}

void JAIBasic::checkSeqWave()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISeqUpdateData* sud = &unk0->mSeqTrackInfo[i];
		JAISoundHandle& sound = sud->mSound;
		if (!sound)
			continue;
		if (!sound->getSeqParameter())
			continue;
		if (sound->getSeqParameter()->mWaitSceneSet == 0xffffffff)
			continue;
		if (mFinishedSceneSet == 0xffffffff)
			continue;

		u32 id = sound->getSeqParameter()->mWaitSceneSet;

		JAISound* snd = sound;
		if (mFinishedSceneSet == id || id == 0xff00ff00
		    || (((mFinishedSceneSet & 0xffff0000) == (id & 0xffff0000)
		         && id == 0xffff))
		    || (((mFinishedSceneSet & 0xffff) == (id & 0xffff)
		         && id == 0xffff0000))) {
			snd->getSeqParameter()->mWaitSceneSet = 0xffffffff;
		}
	}
}

void JAIBasic::checkDvdLoadArc(u32 param_1)
{
	u8 hi   = param_1 >> 8;
	u8 lo   = param_1 & 0xff;
	u16 hi2 = ((param_1 >> 16) & JAISoundID_IndexMask);
	if (hi != 0xff)
		basic->unk0->setAutoHeapLoadedFlag(hi, 0);

	if (lo < 0xFE) {
		JAISound* sound = basic->unk0->mSeqTrackInfo[lo].mSound;
		basic->unk0->mSeqTrackInfo[lo].unk3 = 0;
		if (sound && sound->mState == SOUNDSTATE_Stored
		    && hi2 == (sound->mSoundID & JAISoundID_IndexMask))
			sound->mState = SOUNDSTATE_Prepared;
		else
			basic->unk0->releaseAutoHeapPointer(hi);
	} else if (lo == 0xFE) {
		basic->unk0->releaseAutoHeapPointer(hi);
	}
}
