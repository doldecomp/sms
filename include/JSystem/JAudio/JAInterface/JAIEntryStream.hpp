#ifndef JAIENTRYSTREAM_HPP
#define JAIENTRYSTREAM_HPP

#include <JSystem/JAudio/JAInterface/JAIEntry.hpp>

class JAIActor;

class JAIStreamEntry : public JAIEntry {
public:
	void storeBuffer(JAISound** sound, JAIActor* actor, u32 param1, u32 param2,
	                 u8 param3, void* data);
};

#endif // JAIENTRYSTREAM_HPP
