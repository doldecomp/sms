#ifndef ENEMY_LAUNCHER_HPP
#define ENEMY_LAUNCHER_HPP

#include <Enemy/EnemyManager.hpp>
#include <Enemy/Enemy.hpp>

class TLauncherParams : public TSpineEnemyParams {
public:
	TLauncherParams(const char* path);

	s32 getLaunchPeriod() const { return mSLLaunchPeriod.get(); }

	/* 0xA8 */ TParamRT<s32> mSLLaunchPeriod;
};

class TLauncher : public TSpineEnemy {
public:
	enum {
		STATE_INITIAL    = 0,
		STATE_HITBYWATER = 1,
		STATE_NORMAL     = 2,
		STATE_LAUNCH     = 3,
		STATE_DIE        = 4,
	};

	TLauncher(const char* name);

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
	TSpineEnemy* getProperEnemy(const char*);

	// fabricated
	TLauncherParams* getSaveParam2() const
	{
		return (TLauncherParams*)TSpineEnemy::getSaveParam();
	}

public:
	/* 0x150 */ int mState;
	/* 0x154 */ int mNextState;
	/* 0x158 */ int mTicksSpentInCurState;
	/* 0x15C */ int mLaunchCooldown;
	/* 0x160 */ int mRegenTimer;
};

class TLauncherManager : public TEnemyManager {
public:
	TLauncherManager(const char* name);
	virtual void load(JSUMemoryInputStream&);
};

class TCommonLauncher : public TLauncher {
public:
	TCommonLauncher(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveManager*);
	virtual const char** getBasNameTable() const;
	virtual void stateInitial();
	virtual void stateHitByWater();
	virtual void stateNormal();
	virtual void stateLaunch();
	virtual void stateDie();

	void changeBck(int);

public:
	/* 0x164 */ const char* unk164;
	/* 0x168 */ int mLaunchPeriod;
};

class TCommonLauncherManager : public TLauncherManager {
public:
	TCommonLauncherManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();

	void initJParticle();
};

template <class T> class TNerveWaitForever : public TNerveBase<T> {
public:
	virtual BOOL execute(TSpineBase<T>*) const { return false; }

	static const TNerveWaitForever& theNerve()
	{
		static TNerveWaitForever instance;
		return instance;
	}
};

#endif
