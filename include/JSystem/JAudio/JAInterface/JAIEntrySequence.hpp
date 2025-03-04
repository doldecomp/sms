#ifndef JAIENTRYSEQUENCE_HPP
#define JAIENTRYSEQUENCE_HPP

#include <types.h>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>

struct JAIActor;

class JAISeqEntry {
public:
	static void storeBuffer(JAISound** sound, JAIActor* actor, u32 param1,
	                        u32 param2, u8 param3, void* data);
};

#endif // JAIENTRYSEQUENCE_HPP
