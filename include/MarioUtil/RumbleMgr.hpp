#ifndef MARIO_UTIL_RUMBLE_MGR_HPP
#define MARIO_UTIL_RUMBLE_MGR_HPP

#include <types.h>
#include <dolphin/mtx.h>
#include <dolphin/pad.h>

#define RUMBLE_CHANNELS_PER_CONTROLLER 32

struct RumbleChannelDataTbl {
	int* mPointData;
	float* mFrameData;
	float* mPowerData;
};

extern RumbleChannelDataTbl channelDataTbl[];

class RumbleBase {
public:
	RumbleBase(int chan)
	    : mPower(0.0f)
	    , mAccumulator(0.0f)
	    , mChan(chan)
	{
	}
	void init();
	void stop();
	void update(f32, bool);

	f32 mPower;
	f32 mAccumulator;
	int mChan;
};

class RumbleChannelDataMgr {
public:
	RumbleChannelDataMgr()
	    : mChannelDataTbl(nullptr)
	{
	}
	void init();
	RumbleChannelDataTbl* getChannelDataTbl(int);

	RumbleChannelDataTbl* mChannelDataTbl;
};

class RumbleChannelMgr {
public:
	RumbleChannelMgr();
	void init(RumbleChannelDataMgr*);
	void reset();
	void repeat();
	void start(int, int, f32*);
	void start(int, int, Vec*);
	f32 update();

	f32 mElapsedTime;
	f32 mPower;
	int mChannelDataIdx;
	int mRepeatCount;
	f32* mMultiplierF;
	Vec* mWorldPos;
	RumbleChannelDataTbl* mChannelDataTbl;
	RumbleChannelDataMgr* mChannelDataMgr;
};

class RumbleControllerMgr {
public:
	RumbleControllerMgr();
	void init();
	void reset();
	void start(int, int, f32*);
	void start(int, int, Vec*);
	void stop();
	void stop(int);
	bool channelMgrIsAllFree();
	void updateMotorCount();
	f32 update();

	f32 mPower;
	RumbleChannelMgr* mChannelMgrTbl;
	RumbleChannelDataMgr* mChannelDataMgr;
	u32 mMotorCounter;
	u16 mMotorTimer;
	s8 unk12;
};

class RumbleMgr;

extern RumbleMgr* SMSRumbleMgr;

class RumbleMgr {
public:
	RumbleMgr(bool, bool, bool, bool);
	void init();
	void reset();
	void start(int, f32*);
	void start(int, Vec*);
	void start(int, MtxPtr);
	void start(int, int, f32*);
	void start(int, int, Vec*);
	void start(int, int, MtxPtr);
	void start(int, int, int, f32*);
	void start(int, int, int, Vec*);
	void start(int, int, int, MtxPtr);
	void stop();
	void stop(int);
	void stop(int, int);
	void update();
	void setActive(bool);
	void startPause();
	void finishPause();
	void changePause();
	void changeMode();

	f32 mPower;
	f32 mTimer;
	bool mActive;
	bool mPaused;
	bool mStopHard;
	RumbleBase* mRumbleBaseTbl[4];
	RumbleControllerMgr* mControllerMgrTbl[4];
	RumbleChannelDataMgr* mChannelDataMgr;

	static int mMotorCountLimit;
	static u16 mMotorTimerPeriod;
	static f32 mPowerThreshold;
};

#endif
