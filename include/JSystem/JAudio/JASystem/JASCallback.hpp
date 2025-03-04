#ifndef JASCALLBACK_HPP
#define JASCALLBACK_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Kernel {

	void registerSubframeCallback(s32 (*callback)(void*), void* data);
	void registerAiCallback(s32 (*callback)(void*), void* data);
	extern u32 maxCallbacksUser;
	extern bool (*callbackInit)();
	extern void* callList;

	void resetCallback();
	void subframeCallback();
	void aiCallback();
	void setNumOfCallbacks(u32 num);
	bool checkCallback(s32 (*callback)(void*), void* data);
	void rejectCallback(s32 (*callback)(void*), void* data);

} // namespace Kernel

} // namespace JASystem

#endif // JASCALLBACK_HPP
