#ifndef JASAICTRL_HPP
#define JASAICTRL_HPP

#include <types.h>

namespace JASystem {
namespace Kernel {

	typedef s16* (*MixCallback)(s32);

	void init();
	void initSystem();
	void vframeWork();
	void updateDac();
	void registerDacCallback(void (*callback)(s16*, s32));

	MixCallback getMixCallback(s32);
	void registerMixCallback(MixCallback callback, u8 mode);
	u32 getCurrentVCounter();

	extern u32 JASUniversalDacCounter;

} // namespace Kernel
} // namespace JASystem

#endif // JASAICTRL_HPP
