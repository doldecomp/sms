#include <JSystem/JAudio/JAInterface/JAIAnimation.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>

JAIAnimeSound::JAIAnimeSound() { }

// Unused / inline
void JAIAnimeSound::setAnimSound(JAIBasic* basic, f32 param1, f32 param2,
                                 u8 param3)
{
}

void JAIAnimeSound::setAnimSoundVec(JAIBasic* basic, Vec* pos, f32 param1,
                                    f32 param2, u32 param3, u8 param4)
{
}

void JAIAnimeSound::setAnimSoundActor(JAIBasic* basic, JAIActor* actor,
                                      f32 param1, f32 param2, u8 param3)
{
}

void JAIAnimeSound::playActorAnimSound(JAIBasic* basic, JAIActor* actor,
                                       f32 param, u8 flag)
{
}

void JAIAnimeSound::startAnimSound(void* data, u32 id, JAISound** sound,
                                   JAIActor* actor, u8 flag)
{
}

void JAIAnimeSound::setSpeedModifySound(JAISound* sound,
                                        JAIAnimeFrameSoundData* data, f32 speed)
{
}

void JAIAnimeSound::initActorAnimSound(void* data, u32 param, f32 value) { }

// void JAIAnimeSound::initActorAnimSound(void* data, JAIActor actor, u32 param,
//                                        f32 value)
// {
// }

void JAIAnimeSound::stop() { }
