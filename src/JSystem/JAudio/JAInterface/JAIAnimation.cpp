#include <JSystem/JAudio/JAInterface/JAIAnimation.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>

JAIAnimeSound::JAIAnimeSound()
{
	unk60 = 0;
	unk64 = 0;
	unk68 = 0;
	unk6C = 0;
	for (u8 i = 0; i < 8; ++i) {
		mSlots[i].mSound     = nullptr;
		mSlots[i].mIsPlaying = false;
	}

	for (u8 i = 0; i < 2; ++i)
		unk70[i] = nullptr;

	mDataCounter = 0;
	mLoopCount   = 0;
}

void JAIAnimeSound::initActorAnimSound(void* data, JAIActor actor, u32 param,
                                       f32 value)
{
}

void JAIAnimeSound::initActorAnimSound(void* data, u32 param, f32 value)
{
	mData       = (JAIAnimeSoundData*)data;
	int counter = 0;
	if (mData != nullptr) {
		mDataCounter    = counter;
		mDataCounterInc = param;
		u16 count       = mData->mCount;
		for (u32 i = 0; i < count; ++i) {
			if (mData->mEntries[i].unk4 >= value)
				break;
			counter++;
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
		JAISound* snd              = mSlots[i].mSound;
		JAIAnimeFrameSoundData* sd = mSlots[i].mData;
		if (snd != nullptr && (sd->unk10 & 4)) {
			snd->stop(1);
			mSlots[i].mIsPlaying = 0;
		}
		if (snd == nullptr) {
			mSlots[i].mIsPlaying = 0;
		} else if (!(snd->unk8 & 0xC00)) {
			mSlots[i].mIsPlaying = 0;
		}
	}
}

void JAIAnimeSound::setAnimSound(JAIBasic* basic, f32 param1, f32 param2,
                                 u8 param3)
{
}

void JAIAnimeSound::setAnimSoundVec(JAIBasic* param_1, Vec* param_2,
                                    f32 param_3, f32 param_4, u32 param_5,
                                    u8 param_6)
{
	JAIActor actor(param_2, param_2, param_2, param_5);
	setAnimSoundActor(param_1, &actor, param_3, param_4, param_6);
}

void JAIAnimeSound::setAnimSoundActor(JAIBasic* basic, JAIActor* actor,
                                      f32 param1, f32 param2, u8 param3)
{
	JAIAnimeSoundData* d = mData;
	if (d == nullptr)
		return;

	u16 count = d->mCount;

	if (mDataCounterInc == 1) {
		if (mCurrentTime > param1) {
			while (mDataCounter < count
			       && d->mEntries[mDataCounter].unk4 <= mCurrentTime + param2) {
				playActorAnimSound(basic, actor, param2, param3);
			}
			mDataCounter = mDataCounterLimit;
			mCurrentTime = param1;
			if (mLoopCount < 0x100)
				mLoopCount++;
		}

		for (u8 i = 0; i < 8; ++i) {
			Slot* slot         = &mSlots[i];
			JAISound** sndAddr = &slot->mSound;
			if (slot->mIsPlaying != 0) {
				JAIAnimeFrameSoundData* sd = slot->mData;
				if (!(sd->unk0 & 0xC00)
				    && (param2 != 0.0f || !(sd->unk10 & 0x20))) {
					f32 t1 = sd->unk4;
					f32 t2 = sd->unk8;
					if (t1 == t2 || (t1 < t2 && t2 > param1 && t1 <= param1)
					    || (t1 > t2 && (t2 > param1 || t1 < param1))) {
						startAnimSound(basic, sd->unk0, sndAddr, actor, param3);
					} else {
						slot->mIsPlaying = 0;
					}
				}
				if (*sndAddr != nullptr) {
					setSpeedModifySound(*sndAddr, sd, param2);
					if ((sd->unk10 & 0x10) && sd->unk8 <= param1) {
						(*sndAddr)->stop(1);
					}
				}
			} else {
				slot->mIsPlaying = 0;
			}
		}

		while (mDataCounter < count
		       && d->mEntries[mDataCounter].unk4 <= param1) {
			playActorAnimSound(basic, actor, param2, param3);
		}
	} else {
		if (mCurrentTime < param1) {
			while (mDataCounter < count
			       && d->mEntries[mDataCounter].unk4 >= mCurrentTime - param2) {
				playActorAnimSound(basic, actor, param2, param3);
			}
			mDataCounter = count - 1;
			mCurrentTime = param1;
			if (mLoopCount == -1 || mLoopCount < 0x100) {
				mLoopCount++;
			}
		}

		for (u8 i = 0; i < 8; ++i) {
			Slot* slot         = &mSlots[i];
			JAISound** sndAddr = &slot->mSound;
			if (slot->mIsPlaying != 0) {
				JAIAnimeFrameSoundData* sd = slot->mData;
				if (!(sd->unk0 & 0xC00)
				    && (param2 != 0.0f || !(sd->unk10 & 0x20))) {
					f32 t1 = sd->unk4;
					f32 t2 = sd->unk8;
					if (t1 == t2 || (t1 > t2 && t2 < param1 && t1 > param1)
					    || (t1 < t2 && (t2 < param1 || t1 > param1))) {
						startAnimSound(basic, sd->unk0, sndAddr, actor, param3);
					} else {
						slot->mIsPlaying = 0;
					}
				}
				if (*sndAddr != nullptr) {
					setSpeedModifySound(*sndAddr, sd, param2);
					if ((sd->unk10 & 0x10) && sd->unk8 >= param1) {
						(*sndAddr)->stop(1);
					}
				}
			} else {
				slot->mIsPlaying = 0;
			}
		}

		while (mDataCounter < count
		       && d->mEntries[mDataCounter].unk4 >= param1) {
			playActorAnimSound(basic, actor, param2, param3);
		}
	}

	mCurrentTime = param1;
}

void JAIAnimeSound::playActorAnimSound(JAIBasic* basic, JAIActor* actor,
                                       f32 param, u8 flag)
{
	JAIAnimeFrameSoundData* curData = &mData->mEntries[mDataCounter];

	u8 i;
	for (i = 0; i < 8;) {
		if (mSlots[i].mIsPlaying == 0)
			break;
		if (curData->unk0 != mSlots[i].mData->unk0) {
			++i;
			continue;
		}
		if (!(curData->unk0 & 0xC00)) {
			mDataCounter += mDataCounterInc;
			return;
		}
		break;
	}

	if (i != 8) {
		if (!(curData->unk10 & 8) || mLoopCount == curData->unk16) {
			if ((mDataCounterInc == 1 && !(curData->unk10 & 2))
			    || (mDataCounterInc + 0x10000 == 0xFFFF
			        && !(curData->unk10 & 1))) {
				Slot* slot        = &mSlots[i];
				JAISound** sndPtr = &slot->mSound;
				startAnimSound(basic, curData->unk0, sndPtr, actor, flag);
				if (*sndPtr != nullptr) {
					slot->mData      = curData;
					slot->mIsPlaying = 1;

					(*sndPtr)->setVolume((f32)curData->unk14 / 127.0f, 0, 5);
					(*sndPtr)->setPitch(curData->unkC
					                        + (f32)curData->unk15
					                              * (param - 1.0f) / 32.0f,
					                    0, 5);
					(*sndPtr)->setPan((f32)curData->unk17 / 127.0f, 0, 5);
				}
			}
		}
	}

	mDataCounter += mDataCounterInc;
}

void JAIAnimeSound::startAnimSound(void* param_1, u32 param_2,
                                   JAISound** param_3, JAIActor* param_4,
                                   u8 param_5)
{
	JAIBasic* basic = (JAIBasic*)param_1;
	basic->startSoundActor(param_2, param_3, param_4, 0, param_5);
}

void JAIAnimeSound::setSpeedModifySound(JAISound* param_1,
                                        JAIAnimeFrameSoundData* param_2,
                                        f32 param_3)
{
	f32 fVar1 = param_2->unkC;
	if (param_2->unk15 != 0)
		fVar1 += param_2->unk15 * (param_3 - 1.0f) / 32.0f;
	param_1->setPitch(fVar1, 0, 5);

	s16 uVar2 = param_2->unk14;
	if (param_2->unk15 != 0) {
		uVar2 += (s16)((f32)param_2->unk18 * 2.0f * (param_3 - 1.0f));
		if (uVar2 > 0x7F)
			uVar2 = 0x7F;
		else if (uVar2 < 0)
			uVar2 = 0;
	}
	param_1->setVolume((u8)uVar2 / 127.0f, 0, 5);
}

void JAIAnimeSound::stop()
{
	for (u8 i = 0; i < 8; i++)
		if (mSlots[i].mSound)
			mSlots[i].mSound->stop(0);
}
