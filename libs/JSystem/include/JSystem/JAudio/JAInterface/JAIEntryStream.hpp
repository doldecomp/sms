#ifndef JAIENTRYSTREAM_HPP
#define JAIENTRYSTREAM_HPP

#include <JSystem/JAudio/JAInterface/JAIEntry.hpp>

class JAIActor;

class JAIStreamEntry : public JAIEntry {
public:
	void storeBuffer(JAISoundHandle* out_handle, JAIActor* actor, u32 sound_id,
	                 u32 fade, u8 camera_idx, void* info);
};

#endif // JAIENTRYSTREAM_HPP
