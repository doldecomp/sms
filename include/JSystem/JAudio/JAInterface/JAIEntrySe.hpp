#ifndef JAIENTRYSE_HPP
#define JAIENTRYSE_HPP

#include <types.h>
#include <JSystem/JAudio/JAInterface/JAIEntry.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>

class JAIActor;

class JAISeEntry : public JAIEntry {
public:
	void storeBuffer(JAISound** sound, JAIActor* actor, u32 param1, u32 param2,
	                 u8 param3, void* data);
};

#endif // JAIENTRYSE_HPP
