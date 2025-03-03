#ifndef JAIENTRY_HPP
#define JAIENTRY_HPP

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIActor.hpp>

class JAIEntry {
public:
	static void checkSoundHandle(JAISound** sound, unsigned long param,
	                             void* data);
	static void initSoundParameter(JAISound* sound1, JAISound** sound2,
	                               JAIActor* actor, unsigned long param1,
	                               unsigned long param2, unsigned char param3,
	                               void* data);
	static void checkSoundHandle(JAISound** sound, JAISound* soundParam);
	static void checkAllSoundHandle(JAISound** sound);
};

#endif // JAIENTRY_HPP
