#ifndef JASDSPCHANNEL_HPP
#define JASDSPCHANNEL_HPP

#include <dolphin/types.h>
#include <dolphin/os/OSTime.h>
#include <stdint.h>

namespace JASystem {

namespace DSPInterface {
	struct DSPBuffer;
}

class TChannel;

/**
 * @brief Represents a single physical channel/voice of the DSP.
 * Only 64 exist. Essentially, a wrapper around DSPInterface::DSPBuffer.
 */
class TDSPChannel {
private:
public:
	enum Status {
		STATUS_ALLOCATED = 0,
		STATUS_FREE      = 1,
		STATUS_STOPPING  = 2,
	};

	enum EventType {
		EVENT_UPDATE           = 0,
		EVENT_FINISHED         = 2,
		EVENT_FORCE_STOP       = 3,
		EVENT_PRIORITY_EXPIRED = 4,
	};

	TDSPChannel()
	    : mDSPHandle(nullptr)
	    , mCallback(nullptr)
	{
	}
	~TDSPChannel() { }

	void init(u8 number);
	BOOL allocate(uintptr_t sign);
	void free();
	void play();
	void stop();
	void pause();
	void restart();
	bool forceStop();
	void forceDelete();
	static u32 getNumUse();
	static u32 getNumFree();
	static void setLimitDSP(f32 limit);
	static OSTick* getHistory();

	BOOL isFree() const { return mStatus == STATUS_FREE ? TRUE : FALSE; }
	BOOL checkSign(uintptr_t sign) { return mSign == sign ? TRUE : FALSE; }

	u8 getNumber() { return mNumber; }
	u8 getStatus() { return mStatus; }

	u8 getPriority() { return mPriority; }
	void setPriority(u8 priority) { mPriority = priority; }

	u16 getPriorityTime() { return mPriorityTime; }
	void setPriorityTime(u16 time) { mPriorityTime = time; }

	u16 getCBInterval() { return mCBInterval; }
	void setCBInterval(u16 interval) { mCBInterval = interval; }
	void decCBInterval() { mCBInterval--; }

	TChannel* getLogicalChannel()
	{
		if (mCallback != nullptr) {
			return (TChannel*)mSign; // (TWW) ?? is this userdata?
		} else {
			return nullptr;
		}
	}

	void onUpdate(u32 event) { mCBInterval = mCallback(this, event); }

	static void initAll();
	static void updateAll();
	static TDSPChannel* alloc(u32 param1, uintptr_t sign);
	static int free(TDSPChannel* channel, uintptr_t sign);
	static TDSPChannel* getHandle(u32 handle);
	static JASystem::TDSPChannel* getLower();
	static JASystem::TDSPChannel* getLowerActive();
	static BOOL breakLower(u8 priority);
	static BOOL breakLowerActive(u8 priority);
	static void forceStop(TDSPChannel* channel)
	{
		if (channel != nullptr)
			channel->forceStop();
	}

	static TDSPChannel* DSPCH;
	static u32 smnUse;
	static u32 smnFree;

public:
	/* 0x0 */ u8 mNumber;
	/* 0x0 */ u8 mStatus;
	/* 0x0 */ u8 unk2;
	/* 0x0 */ u8 mPriority;
	/* 0x4 */ u16 mPriorityTime;
	/* 0x6 */ u16 mCBInterval;
	/* 0x8 */ uintptr_t mSign;
	/* 0xC */ DSPInterface::DSPBuffer* mDSPHandle;
	/* 0x10 */ int (*mCallback)(TDSPChannel*, u32);
};

} // namespace JASystem

#endif // JASDSPCHANNEL_HPP
