#ifndef JAIENTRYSEQUENCE_HPP
#define JAIENTRYSEQUENCE_HPP

#include <JSystem/JAudio/JAInterface/JAIEntry.hpp>

struct JAIActor;

class JAISeqEntry : public JAIEntry {
public:
	void storeBuffer(JAISoundHandle* out_handle, JAIActor* actor, u32 sound_idx,
	                 u32 fade, u8 camera_idx, void* data);
};

#endif // JAIENTRYSEQUENCE_HPP
