#ifndef JAIENTRYSEQUENCE_HPP
#define JAIENTRYSEQUENCE_HPP

#include <JSystem/JAudio/JAInterface/JAIEntry.hpp>

struct JAIActor;

class JAISeqEntry : public JAIEntry {
public:
	void storeBuffer(JAISound** sound, JAIActor* actor, u32 param1, u32 param2,
	                 u8 param3, void* data);
};

#endif // JAIENTRYSEQUENCE_HPP
