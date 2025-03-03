#ifndef JAIENTRYSE_HPP
#define JAIENTRYSE_HPP

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIActor.hpp>

class JAISeEntry {
public:
	static void storeBuffer(JAISound** sound, JAIActor* actor,
	                        unsigned long param1, unsigned long param2,
	                        unsigned char param3, void* data);
};

#endif // JAIENTRYSE_HPP
