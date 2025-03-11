#include <JSystem/JAudio/JASystem/JASCallback.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os.h>
#include <types.h>

namespace JASystem {

namespace Kernel {
	// fabricated
	struct CallListEntry {
		/* 0x00 */ s32 (*callback)(void*);
		/* 0x04 */ void* userData;
		/* 0x08 */ u32 cbType;
	};

	static u32 maxCallbacksUser    = 0x10;
	static CallListEntry* callList = nullptr;
	static bool callbackInit       = 0;

	void setNumOfCallbacks(u32 num) { }

	void resetCallback()
	{
		if (callbackInit != true) {
			callList = new (JASDram, 0) CallListEntry[maxCallbacksUser];

			for (int i = 0; i < maxCallbacksUser; i++)
				callList[i].callback = nullptr;

			callbackInit = true;
		}
	}

	int checkCallback(s32 (*callback)(void*), void* data)
	{
		if (callbackInit == false)
			return -1;

		for (int i = 0; i < maxCallbacksUser; i++)
			if (callList[i].callback == callback
			    && callList[i].userData == data)
				return i;

		return -1;
	}

	void rejectCallback(s32 (*callback)(void*), void* data) { }

	int registerSubframeCallback(s32 (*callback)(void*), void* data)
	{
		if (callbackInit == false)
			return -1;

		int idx = registerAiCallback(callback, data);
		if (idx == -1)
			return -1;

		callList[idx].cbType = 1;
		return idx;
	}

	int registerAiCallback(s32 (*callback)(void*), void* data)
	{
		if (callbackInit == false)
			return -1;

		int idx;
		for (idx = 0; idx < maxCallbacksUser; idx++)
			if (callList[idx].callback == NULL)
				break;

		if (idx == maxCallbacksUser)
			return -1;

		if (checkCallback(callback, data) != -1)
			return -1;

		callList[idx].callback = callback;
		callList[idx].userData = data;
		callList[idx].cbType   = 0;
		return idx;
	}

	void aiCallback()
	{
		if (!callbackInit)
			return;

		for (int i = 0; i < maxCallbacksUser; i++) {
			if (callList[i].callback && callList[i].cbType == 0) {
				int result = callList[i].callback(callList[i].userData);
				if (result == -1)
					callList[i].callback = nullptr;
			}
		}
	}

	void subframeCallback()
	{
		if (!callbackInit)
			return;

		for (int i = 0; i < maxCallbacksUser; i++) {
			if (callList[i].callback && callList[i].cbType == 1) {
				int result = callList[i].callback(callList[i].userData);
				if (result == -1)
					callList[i].callback = nullptr;
			}
		}
	}

} // namespace Kernel

} // namespace JASystem
