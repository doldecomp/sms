#ifndef MARIO_UTIL_RUMBLE_MGR_HPP
#define MARIO_UTIL_RUMBLE_MGR_HPP

#include <dolphin/mtx.h>
#include <dolphin/pad.h>
#include <JSystem/JGeometry.hpp>

#define MAX_RUMBLE_ID       (25)
#define MAX_RUMBLE_CHANNELS (32)

struct RumbleChannelData {
	int* point;
	float* frame;
	float* power;
};

extern const RumbleChannelData channelDataTbl[];
extern int channelNum;

// RumbleMgr

class RumbleChannelMgr {
public:
	RumbleChannelMgr();
	f32 update();

public:
	/* 0x00 */ f32 mElapsedTime;                    /* inferred */
	/* 0x04 */ f32 mCurrentIntensity;               /* inferred */
	/* 0x08 */ s32 mChannelID;                      /* inferred */
	/* 0x0C */ s32 mLoopCount;                      /* inferred */
	/* 0x10 */ f32* mExternalDampenPtr;             /* inferred */
	/* 0x14 */ Vec* mPositionalSourcePtr;           /* inferred */
	/* 0x18 */ const RumbleChannelData* rumbleData; /* inferred */
	/* 0x1C */ void* unk20;                         /* inferred */
};

class RumbleControllerMgr {
public:
	void reset();
	void start(int channelId, int loopCount, float* externalDampenPtr);
	void start(int channelId, int loopCount, Vec* positionalSourcePtr);
	void stop(int channelId);
	void channelMgrIsAllFree();
	void updateMotorCount();
	f32 update();

public:
	f32 currentPower;           // 0x00
	RumbleChannelMgr* channels; // 0x04
	RumbleChannelMgr* unk8;     // 0x08
	u32 unkC;                   // 0x0C
	u16 motorTime;              // 0x10
	bool unk12;                 // 0x12
	u8 padding_13;              // 0x13
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

public:
	struct RumbleControllerState {
		f32 m_currentIntensityL; // 0x00
		f32 m_currentIntensityR; // 0x04
		s32 m_controllerIndex;   // 0x08
	};

	f32 m_masterIntensityL;                       // 0x00
	f32 m_masterIntensityR;                       // 0x04
	bool m_isInitialized;                         // 0x08
	bool m_flags;                                 // 0x09
	bool unkA;                                    // 0x0A
	RumbleControllerState* m_controllerStates[4]; // 0x0C
	RumbleControllerMgr* m_controllerManagers[4]; // 0x1C
	const int*** m_rumbleOutput;                  // 0x2C

public:
	static u32 mMotorCountLimit;
	static u16 mMotorTimerPeriod;
	static f32 mPowerThreshold;
};

#endif
