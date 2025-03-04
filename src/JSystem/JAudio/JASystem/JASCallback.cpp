#include <JSystem/JAudio/JASystem/JASCallback.hpp>

namespace JASystem {

namespace Kernel {

	u32 maxCallbacksUser   = 0x10;
	void* callList         = 0;
	bool (*callbackInit)() = 0;

	void registerSubframeCallback(s32 (*callback)(void*), void* data) { }

	void registerAiCallback(s32 (*callback)(void*), void* data) { }

	void resetCallback() { }

	void subframeCallback() { }

	void aiCallback() { }

	void setNumOfCallbacks(u32 num) { }

	bool checkCallback(s32 (*callback)(void*), void* data) { return false; }

	void rejectCallback(s32 (*callback)(void*), void* data) { }

} // namespace Kernel

} // namespace JASystem
