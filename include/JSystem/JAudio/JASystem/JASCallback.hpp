#ifndef JASCALLBACK_HPP
#define JASCALLBACK_HPP

namespace JASystem {

namespace Kernel {

	void registerSubframeCallback(long (*callback)(void*), void* data);
	void callbackInit();
	void registerAiCallback(long (*callback)(void*), void* data);
	extern unsigned long maxCallbacksUser;
	extern void* callList;
	void resetCallback();
	void subframeCallback();
	void aiCallback();
	void setNumOfCallbacks(unsigned long num);
	bool checkCallback(long (*callback)(void*), void* data);
	void rejectCallback(long (*callback)(void*), void* data);

} // namespace Kernel

} // namespace JASystem

#endif // JASCALLBACK_HPP
