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

void JAIAnimeSound::initActorAnimSound(void* data, u32 param, f32 value) { }

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
}

void JAIAnimeSound::playActorAnimSound(JAIBasic* basic, JAIActor* actor,
                                       f32 param, u8 flag)
{
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
