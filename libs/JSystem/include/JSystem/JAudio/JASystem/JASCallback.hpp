#ifndef JASCALLBACK_HPP
#define JASCALLBACK_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Kernel {

	void setNumOfCallbacks(u32 num);
	void resetCallback();
	int checkCallback(s32 (*callback)(void*), void* data);
	void rejectCallback(s32 (*callback)(void*), void* data);

	int registerSubframeCallback(s32 (*callback)(void*), void* data);
	void subframeCallback();

	int registerAiCallback(s32 (*callback)(void*), void* data);
	void aiCallback();

} // namespace Kernel

} // namespace JASystem

#endif // JASCALLBACK_HPP
