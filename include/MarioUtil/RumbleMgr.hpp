#ifndef MARIO_UTIL_RUMBLE_MGR_HPP
#define MARIO_UTIL_RUMBLE_MGR_HPP

#include <dolphin/mtx.h>

class RumbleBase {
public:
	void init();
	void stop();
	void update(float, bool);
};

class RumbleChannelDataMgr {
public:
	void init();
	void getChannelDataTbl(int);
};

class RumbleChannelMgr {
public:
	RumbleChannelMgr();
	void init(RumbleChannelDataMgr*);
	void reset();
	void repeat();
	void start(int, int, float*);
	void start(int, int, Vec*);
	void update();
};

class RumbleControllerMgr {
public:
	RumbleControllerMgr();
	void init();
	void reset();
	void start(int, int, float*);
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
	void start(int, float*);
	void start(int, Vec*);
	void start(int, float (*)[4]);
	void start(int, int, float*);
	void start(int, int, Vec*);
	void start(int, int, float (*)[4]);
	void start(int, int, int, float*);
	void start(int, int, int, Vec*);
	void start(int, int, int, float (*)[4]);
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
