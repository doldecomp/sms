#ifndef MARIO_UTIL_RUMBLE_MGR_HPP
#define MARIO_UTIL_RUMBLE_MGR_HPP

#include <dolphin/mtx.h>
#include <dolphin/pad.h>
#include <JSystem/JGeometry.hpp>

#define MAX_RUMBLE_ID (25)
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
	void init(RumbleChannelDataMgr*);
	void reset();
	void repeat();
	void start(int, int, f32*);
	void start(int, int, Vec*);
	void update();
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
	void channelMgrIsAllFree();
	void updateMotorCount();
	void update();
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
};

#endif
