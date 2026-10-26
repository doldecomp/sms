#ifndef MARIO_UTIL_RUMBLE_MGR_HPP
#define MARIO_UTIL_RUMBLE_MGR_HPP

#include <dolphin/mtx.h>
#include <dolphin/pad.h>
#include <dolphin/types.h>

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
	void update(f32 power, bool stopHard);

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
	RumbleChannelDataTbl* getChannelDataTbl(int index);

	RumbleChannelDataTbl* mChannelDataTbl;
};

class RumbleChannelMgr {
public:
	RumbleChannelMgr();
	void init(RumbleChannelDataMgr* dataMgr);
	void reset();
	void repeat();
	void start(int channelDataIdx, int repeatCount, f32* multiplierF);
	void start(int channelDataIdx, int repeatCount, Vec* worldPos);
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
	void start(int channelDataIdx, int repeatCount, f32* multiplierF);
	void start(int channelDataIdx, int repeatCount, Vec* worldPos);
	void stop();
	void stop(int channelDataIdx);
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
	RumbleMgr(bool controller1, bool controller2, bool controller3,
	          bool controller4);
	void init();
	void reset();
	void start(int channelDataIdx, f32* multiplierF);
	void start(int channelDataIdx, Vec* worldPos);
	void start(int channelDataIdx, MtxPtr mtx);
	void start(int channelDataIdx, int repeatCount, f32* multiplierF);
	void start(int channelDataIdx, int repeatCount, Vec* worldPos);
	void start(int channelDataIdx, int repeatCount, MtxPtr mtx);
	void start(int controllerIdx, int channelDataIdx, int repeatCount,
	           f32* multiplierF);
	void start(int controllerIdx, int channelDataIdx, int repeatCount,
	           Vec* worldPos);
	void start(int controllerIdx, int channelDataIdx, int repeatCount,
	           MtxPtr mtx);
	void stop();
	void stop(int channelDataIdx);
	void stop(int controllerIdx, int channelDataIdx);
	void update();
	void setActive(bool active);
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
