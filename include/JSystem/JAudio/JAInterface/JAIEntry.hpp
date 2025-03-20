#ifndef JAIENTRY_HPP
#define JAIENTRY_HPP

#include <types.h>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>

class JAIActor;
class JAIBasic;

class JAIEntry {
public:
	BOOL checkSoundHandle(JAISound** sound, u32 param, void* data);
	void initSoundParameter(JAISound* sound1, JAISound** sound2,
	                        JAIActor* actor, u32 param1, u32 param2, u8 param3,
	                        void* data);
	BOOL checkSoundHandle(JAISound** sound, JAISound* soundParam);
	void checkAllSoundHandle(JAISound** sound);

	/* 0x0 */ JAIBasic* unk0;
};

#endif // JAIENTRY_HPP
