#ifndef JAIANIMATION_HPP
#define JAIANIMATION_HPP

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIActor.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>

class Vec;

struct JAIAnimeFrameSoundData {
	// Placeholder structure for animation frame sound data
};

class JAIAnimeSound {
public:
	JAIAnimeSound();
	virtual ~JAIAnimeSound();

	void setAnimSound(JAIBasic* basic, float param1, float param2,
	                  unsigned char param3);
	void setAnimSoundVec(JAIBasic* basic, Vec* pos, float param1, float param2,
	                     unsigned long param3, unsigned char param4);
	void setAnimSoundActor(JAIBasic* basic, JAIActor* actor, float param1,
	                       float param2, unsigned char param3);
	void playActorAnimSound(JAIBasic* basic, JAIActor* actor, float param,
	                        unsigned char flag);
	void startAnimSound(void* data, unsigned long id, JAISound** sound,
	                    JAIActor* actor, unsigned char flag);
	void setSpeedModifySound(JAISound* sound, JAIAnimeFrameSoundData* data,
	                         float speed);
	void initActorAnimSound(void* data, unsigned long param, float value);
	void initActorAnimSound(void* data, JAIActor actor, unsigned long param,
	                        float value);
	void stop();
};

#endif // JAIANIMATION_HPP
