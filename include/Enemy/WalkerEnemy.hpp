#ifndef ENEMY_WALKER_ENEMY_HPP
#define ENEMY_WALKER_ENEMY_HPP

#include <Enemy/SmallEnemy.hpp>
#include <stdlib.h>

// fabricated
class FakeRandInterval {
public:
	FakeRandInterval()
	    : mMin(0.0f)
	    , mMax(1.0f)
	{
	}

	FakeRandInterval(f32 min, f32 max)
	    : mMin(min)
	    , mMax(max)
	{
	}

	f32 get() const
	{
		f32 dist = mMax - mMin;
		f32 r    = rand() * (1.f / (RAND_MAX + 1)) * dist;
		return r + mMin;
	}

	f32 max() const { return mMax; }

private:
	f32 mMin;
	f32 mMax;
};

class TWalkerEnemyParams : public TSmallEnemyParams {
public:
	TWalkerEnemyParams(const char*);

	/* 0x2D4 */ TParamRT<f32> mSLZigzagCycle;
	/* 0x2E8 */ TParamRT<f32> mSLZigzagAngle;
	/* 0x2FC */ TParamRT<f32> mSLMarchSpeedLow;
	/* 0x310 */ TParamRT<f32> mSLMarchSpeedHigh;

	// TODO: random interval class?
	/* 0x324 */ FakeRandInterval unk324;
};

class TWalkerEnemy : public TSmallEnemy {
public:
	TWalkerEnemy(const char* name);

	virtual void init(TLiveManager*);
	virtual void moveObject();
	virtual void reset();
	virtual void attackToMario();
	virtual void initAttacker(THitActor*);
	virtual void behaveToFindMario();
	virtual void walkBehavior(int, float);
	virtual bool isResignationAttack();
	virtual void initialGraphNode();

	bool isReachedToGoalXZ();

	// fabricated
	TWalkerEnemyParams* getSaveParam() const
	{
		return (TWalkerEnemyParams*)TSpineEnemy::getSaveParam();
	}
};

class TLiveActor;

DECLARE_NERVE(TNerveWalkerGenerate, TLiveActor);
DECLARE_NERVE(TNerveWalkerGraphWander, TLiveActor);
DECLARE_NERVE(TNerveWalkerAttack, TLiveActor);
DECLARE_NERVE(TNerveWalkerPostAttack, TLiveActor);
DECLARE_NERVE(TNerveWalkerEscape, TLiveActor);
DECLARE_NERVE(TNerveWalkerTraceMario, TLiveActor);

#endif
