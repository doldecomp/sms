#include <JSystem/JAudio/JAInterface/JAIAnimation.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>

JAIAnimeSound::JAIAnimeSound()
{
	unk60 = 0;
	unk64 = 0;
	unk68 = 0;
	unk6C = 0;
	for (u8 i = 0; i < 8; ++i) {
		mSlots[i].mSoundHandle = nullptr;
		mSlots[i].mIsPlaying   = false;
	}

	for (u8 i = 0; i < 2; ++i)
		unk70[i] = nullptr;

	mDataCounter = 0;
	mLoopCount   = 0;
}

void JAIAnimeSound::initActorAnimSound(void*, JAIActor, u32, f32) { }

void JAIAnimeSound::initActorAnimSound(void* interface, u32 param, f32 frame)
{
	u32 counter = 0;
	mData       = (JAIAnimeSoundData*)interface;
	if (mData != nullptr) {
		mDataCounter    = 0;
		mDataCounterInc = param;
		for (; counter < mData->mCount; counter++) {
			if (mData->mEntries[counter].mStartFrame >= frame)
				break;
		}

		if (param == 1) {
			mDataCounter      = 0;
			mDataCounterLimit = counter;
			mCurrentTime      = 0.0f;
			mLoopCount        = 0;
		} else {
			mDataCounter      = counter;
			mDataCounterLimit = 0;
			mCurrentTime      = 0.0f;
			mLoopCount        = -1;
		}
	}

	for (u8 i = 0; i < 8; ++i) {
		JAISoundHandle sound       = mSlots[i].mSoundHandle;
		JAIAnimeFrameSoundData* sd = mSlots[i].mData;
		if (sound != nullptr && (sd->unk10 & 4)) {
			sound->stop(1);
			mSlots[i].mIsPlaying = 0;
		}
		if (sound == nullptr) {
			mSlots[i].mIsPlaying = 0;
		} else if (!(sound->getID() & 0xC00)) {
			mSlots[i].mIsPlaying = 0;
		}
	}
}

void JAIAnimeSound::setAnimSound(JAIBasic*, f32, f32, u8) { }

void JAIAnimeSound::setAnimSoundVec(JAIBasic* interface, Vec* position,
                                    f32 frame, f32 speed, u32 ground_no,
                                    u8 param_6)
{
	JAIActor actor(position, position, position, ground_no);
	setAnimSoundActor(interface, &actor, frame, speed, param_6);
}

void JAIAnimeSound::setAnimSoundActor(JAIBasic* interface, JAIActor* actor,
                                      f32 frame, f32 speed, u8 param3)
{
	JAIAnimeSoundData* d = mData;
	if (d == nullptr)
		return;

	u16 count = d->mCount;

	if (mDataCounterInc == 1) {
		if (mCurrentTime > frame) {
			while (mDataCounter < count
			       && d->mEntries[mDataCounter].mStartFrame
			              <= mCurrentTime + speed) {
				playActorAnimSound(interface, actor, speed, param3);
			}
			mDataCounter = mDataCounterLimit;
			mCurrentTime = frame;
			if (mLoopCount < 0x100)
				mLoopCount++;
		}

		for (u8 i = 0; i < 8; ++i) {
			JAIAnimeFrameSoundData* sd;
			JAISoundHandle& handle = mSlots[i].mSoundHandle;
			if (mSlots[i].mIsPlaying != 0) {
				u32 id = (sd = mSlots[i].mData)->mSoundID;
				if (!(id & 0xC00) && (speed != 0.0f || !(sd->unk10 & 0x20))) {
					f32 start = sd->mStartFrame;
					f32 end   = sd->mEndFrame;
					if (start == end
					    || (start < end && end > frame && start <= frame)
					    || (start > end && (end > frame || start < frame))) {
						startAnimSound(interface, id, &handle, actor, param3);
					} else {
						mSlots[i].mIsPlaying = 0;
					}
				}
				if (handle != nullptr) {
					setSpeedModifySound(handle, sd, speed);
					if ((sd->unk10 & 0x10) && sd->mEndFrame <= frame)
						handle->stop(1);
				}
			} else {
				mSlots[i].mIsPlaying = 0;
			}
		}

		while (mDataCounter < count
		       && d->mEntries[mDataCounter].mStartFrame <= frame) {
			playActorAnimSound(interface, actor, speed, param3);
		}
	} else {
		if (mCurrentTime < frame) {
			while (mDataCounter < count && mDataCounter >= 0
			       && d->mEntries[mDataCounter].mStartFrame
			              >= mCurrentTime - speed) {
				playActorAnimSound(interface, actor, speed, param3);
			}
			mDataCounter = count - 1;
			mCurrentTime = frame;
			if (mLoopCount == -1 || mLoopCount < 0x100) {
				mLoopCount++;
			}
		}

		for (u8 i = 0; i < 8; ++i) {
			JAIAnimeFrameSoundData* sd;
			JAISoundHandle& handle = mSlots[i].mSoundHandle;
			if (mSlots[i].mIsPlaying != 0) {
				u32 id = (sd = mSlots[i].mData)->mSoundID;
				if (!(id & 0xC00) && (speed != 0.0f || !(sd->unk10 & 0x20))) {
					f32 t1 = sd->mStartFrame;
					f32 t2 = sd->mEndFrame;
					if (t1 == t2 || (t1 > t2 && t2 < frame && t1 > frame)
					    || (t1 < t2 && (t2 < frame || t1 > frame))) {
						startAnimSound(interface, id, &handle, actor, param3);
					} else {
						mSlots[i].mIsPlaying = 0;
					}
				}
				if (handle != nullptr) {
					setSpeedModifySound(handle, sd, speed);
					if ((sd->unk10 & 0x10) && sd->mEndFrame >= frame)
						handle->stop(1);
				}
			} else {
				mSlots[i].mIsPlaying = 0;
			}
		}

		while (mDataCounter < count && mDataCounter >= 0
		       && d->mEntries[mDataCounter].mStartFrame >= frame) {
			playActorAnimSound(interface, actor, speed, param3);
		}
	}

	mCurrentTime = frame;
}

void JAIAnimeSound::playActorAnimSound(JAIBasic* interface, JAIActor* actor,
                                       f32 param, u8 flag)
{
	// TODO: debug assert or something? This is crazy...
	(0 <= mDataCounter) && (mDataCounter <= mData->mCount) ? (void)0 : (void)0;

	JAIAnimeFrameSoundData* curData = &mData->mEntries[mDataCounter];

	u8 i;
	for (i = 0; i < 8;) {
		if (mSlots[i].mIsPlaying == 0)
			break;
		if (curData->mSoundID != mSlots[i].mData->mSoundID) {
			++i;
			continue;
		}
		if (!(curData->mSoundID & 0xC00)) {
			mDataCounter += mDataCounterInc;
			return;
		}
		break;
	}

	if (i != 8) {
		if (!(curData->unk10 & 8) || mLoopCount == curData->unk16) {
			if ((mDataCounterInc == 1 && !(curData->unk10 & 2))
			    || (mDataCounterInc == -1 && !(curData->unk10 & 1))) {
				Slot* slot             = &mSlots[i];
				JAISoundHandle& handle = slot->mSoundHandle;
				startAnimSound(interface, curData->mSoundID, &handle, actor,
				               flag);
				if (handle != nullptr) {
					slot->mData      = curData;
					slot->mIsPlaying = 1;

					handle->setVolumeU7(curData->mVolume, 0, 5);
					f32 pitch = curData->mPitchScale * (param - 1.0f) / 32.0f
					            + curData->mPitch;
					handle->setPitch(pitch, 0, 5);
					handle->setPan((f32)curData->mPan / 127.0f, 0, 5);
				}
			}
		}
	}

	mDataCounter += mDataCounterInc;
}

void JAIAnimeSound::startAnimSound(void* interface, u32 id,
                                   JAISoundHandle* out_handle, JAIActor* actor,
                                   u8 param_5)
{
	((JAIBasic*)interface)->startSoundActor(id, out_handle, actor, 0, param_5);
}

void JAIAnimeSound::setSpeedModifySound(JAISound* param_1,
                                        JAIAnimeFrameSoundData* param_2,
                                        f32 speed)
{
	f32 pitch = param_2->mPitch;
	if (param_2->mPitchScale != 0)
		pitch += param_2->mPitchScale * (speed - 1.0f) / 32.0f;
	param_1->setPitch(pitch, 0, 5);

	s16 volume = param_2->mVolume;
	if (param_2->mPitchScale != 0) {
		volume += (s16)((f32)param_2->mVolumeScale * 2.0f * (speed - 1.0f));
		if (volume > 0x7F)
			volume = 0x7F;
		else if (volume < 0)
			volume = 0;
	}
	param_1->setVolumeU7(volume, 0, 5);
}

void JAIAnimeSound::stop()
{
	for (u8 i = 0; i < 8; i++)
		if (mSlots[i].mSoundHandle)
			mSlots[i].mSoundHandle->stop(0);
}
