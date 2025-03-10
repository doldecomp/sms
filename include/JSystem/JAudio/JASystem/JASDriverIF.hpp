#ifndef JASDRIVERIF_HPP
#define JASDRIVERIF_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Driver {
	void init();

	void setMixerLevel(f32, f32);
	u16 getMixerLevel();

	void setOutputMode(u32);
	u32 getOutputMode();

	void setUpdateInterval(u8);
	u8 getUpdateInterval();

	void setDSPQueueNum(u32);
} // namespace Driver

} // namespace JASystem

#endif // JASDRIVERIF_HPP
