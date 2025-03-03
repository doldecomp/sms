#ifndef JAIENTRYSEQUENCE_HPP
#define JAIENTRYSEQUENCE_HPP

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIActor.hpp>

class JAISeqEntry {
public:
	static void storeBuffer(JAISound** sound, JAIActor* actor,
	                        unsigned long param1, unsigned long param2,
	                        unsigned char param3, void* data);
};

#endif // JAIENTRYSEQUENCE_HPP
