#ifndef JAIENTRY_HPP
#define JAIENTRY_HPP

#include <types.h>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>

class JAIActor;

class JAIEntry {
public:
	void checkSoundHandle(JAISound** sound, u32 param, void* data);
	void initSoundParameter(JAISound* sound1, JAISound** sound2,
	                        JAIActor* actor, u32 param1, u32 param2, u8 param3,
	                        void* data);
	void checkSoundHandle(JAISound** sound, JAISound* soundParam);
	void checkAllSoundHandle(JAISound** sound);
};

#endif // JAIENTRY_HPP
