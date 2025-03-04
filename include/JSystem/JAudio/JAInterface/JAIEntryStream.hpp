#ifndef JAIENTRYSTREAM_HPP
#define JAIENTRYSTREAM_HPP

#include <types.h>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>

class JAIActor;

class JAIStreamEntry {
public:
	static void storeBuffer(JAISound** sound, JAIActor* actor, u32 param1,
	                        u32 param2, u8 param3, void* data);
};

#endif // JAIENTRYSTREAM_HPP
