#ifndef JAIANIMATION_HPP
#define JAIANIMATION_HPP

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>

class JAIActor;

struct JAIAnimeFrameSoundData { };

class JAIAnimeSound {
public:
	JAIAnimeSound();
	virtual ~JAIAnimeSound();

	void setAnimSound(JAIBasic* basic, f32 param1, f32 param2, u8 param3);
	void setAnimSoundVec(JAIBasic* basic, Vec* pos, f32 param1, f32 param2,
	                     u32 param3, u8 param4);
	void setAnimSoundActor(JAIBasic* basic, JAIActor* actor, f32 param1,
	                       f32 param2, u8 param3);
	void playActorAnimSound(JAIBasic* basic, JAIActor* actor, f32 param,
	                        u8 flag);
	void startAnimSound(void* data, u32 id, JAISound** sound, JAIActor* actor,
	                    u8 flag);
	void setSpeedModifySound(JAISound* sound, JAIAnimeFrameSoundData* data,
	                         f32 speed);
	void initActorAnimSound(void* data, u32 param, f32 value);
	void initActorAnimSound(void* data, JAIActor actor, u32 param, f32 value);
	void stop();
};

#endif // JAIANIMATION_HPP
