#ifndef ENEMY_LAUNCHER_HPP
#define ENEMY_LAUNCHER_HPP

#include <Enemy/Enemy.hpp>

class TLauncher : public TSpineEnemy {
public:
	TLauncher(const char*);

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void control();
	virtual void bind();
	virtual void changeState(int);
	virtual void stateInitial();
	virtual void stateHitByWater();
	virtual void stateNormal();
	virtual void stateLaunch();
	virtual void stateDie();

	void resetLaunchTimer();
	void getProperEnemy(const char*);
};

#endif
