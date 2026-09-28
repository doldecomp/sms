#ifndef JAIENTRY_HPP
#define JAIENTRY_HPP

#include <JSystem/JAudio/JAInterface/JAISound.hpp>

class JAIActor;
class JAIBasic;
class JAISound;

class JAIEntry {
public:
	u32 checkSoundHandle(JAISoundHandle* handle, u32 sound_id, void* info);
	void initSoundParameter(JAISound* sound, JAISoundHandle* out_handle,
	                        JAIActor* actor, u32 param_4, u32 fade, u8 param_6,
	                        void* info);
	BOOL checkSoundHandle(JAISoundHandle*, JAISound*);
	void checkAllSoundHandle(JAISoundHandle*);

	/* 0x0 */ JAIBasic* unk0;
};

#endif // JAIENTRY_HPP
