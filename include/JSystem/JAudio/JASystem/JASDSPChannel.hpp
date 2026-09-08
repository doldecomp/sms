#ifndef JASDSPCHANNEL_HPP
#define JASDSPCHANNEL_HPP

#include <dolphin/types.h>
#include <types.h>

namespace JASystem {

namespace DSPInterface {
	struct DSPBuffer;
}

class TChannel;

class TDSPChannel {
private:
public:
	TDSPChannel()
	    : mDSPHandle(nullptr)
	    , mCallback(nullptr)
	{
	}
	~TDSPChannel() { }

	static void initAll();
	static void updateAll();
	static TDSPChannel* alloc(u32 param1, u32 param2);
	static int free(TDSPChannel* channel, u32 param);
	static TDSPChannel* getHandle(u32 handle);
	u32 getNumUse();
	u32 getNumFree();
	void setLimitDSP(f32 limit);
	f32* getHistory();

	void init(u8 param);
	BOOL allocate(u32 param);
	void free();
	void play();
	void stop();
	void pause();
	void restart();
	bool forceStop();
	void forceDelete();
	static JASystem::TDSPChannel* getLower();
	static JASystem::TDSPChannel* getLowerActive();
	static BOOL breakLower(u8 param);
	static BOOL breakLowerActive(u8 param);

	BOOL isFree() const { return mStatus == 1 ? TRUE : FALSE; }

	u8 getNumber() { return mNumber; }
	u8 getStatus() { return mStatus; }

	u8 getPriority() { return mPriority; }
	void setPriority(u8 priority) { mPriority = priority; }

	u16 getPriorityTime() { return mPriorityTime; }
	void setPriorityTime(u16 time) { mPriorityTime = time; }

	u16 getCBInterval() { return mCBInterval; }
	void setCBInterval(u16 interval) { mCBInterval = interval; }
	void decCBInterval() { mCBInterval--; }

	static TDSPChannel* DSPCH;
	static u32 smnUse;
	static u32 smnFree;

	// fake, stolen from tww
	TChannel* getLogicalChannel()
	{
		if (mCallback != nullptr) {
			return (TChannel*)unk8; // (TWW) ?? is this userdata?
		} else {
			return nullptr;
		}
	}

public:
	/* 0x0 */ u8 mNumber;
	/* 0x0 */ u8 mStatus;
	/* 0x0 */ u8 unk2;
	/* 0x0 */ u8 mPriority;
	/* 0x4 */ u16 mPriorityTime;
	/* 0x6 */ u16 mCBInterval;
	/* 0x8 */ u32 unk8;
	/* 0xC */ DSPInterface::DSPBuffer* mDSPHandle;
	/* 0x10 */ int (*mCallback)(TDSPChannel*, u32);
};

} // namespace JASystem

#endif // JASDSPCHANNEL_HPP
