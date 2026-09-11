#ifndef JAIENTRYSE_HPP
#define JAIENTRYSE_HPP

#include <types.h>
#include <JSystem/JAudio/JAInterface/JAIEntry.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>

class JAIActor;

class JAISeEntry : public JAIEntry {
public:
	void storeBuffer(JAISoundHandle* out_handle, JAIActor* actor, u32 sound_id,
	                 u32 fade, u8 param3, void* info);
};

#endif // JAIENTRYSE_HPP
