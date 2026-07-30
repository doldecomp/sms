#ifndef ENEMY_WALKER_ENEMY_HPP
#define ENEMY_WALKER_ENEMY_HPP

#include <MarioUtil/RandomUtil.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <stdlib.h>

class TWalker;

class TWalkerEnemyParams : public TSmallEnemyParams {
public:
	TWalkerEnemyParams(const char*);

	f32 getSLZigzagCycle() const { return mSLZigzagCycle.get(); }
	f32 getSLZigzagAngle() const { return mSLZigzagAngle.get(); }

	/* 0x2D4 */ TParamRT<f32> mSLZigzagCycle;
	/* 0x2E8 */ TParamRT<f32> mSLZigzagAngle;
	/* 0x2FC */ TParamRT<f32> mSLMarchSpeedLow;
	/* 0x310 */ TParamRT<f32> mSLMarchSpeedHigh;

	/* 0x324 */ TMsRange<f32> unk324;
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
	TWalkerEnemyParams* getSaveParam2() const
	{
		return (TWalkerEnemyParams*)getSaveParam();
	}

	// fabricated
	TWalker* getWalker() const { return (TWalker*)mBinder; }
};

class TLiveActor;

DECLARE_NERVE(TNerveWalkerGenerate, TLiveActor);
DECLARE_NERVE(TNerveWalkerGraphWander, TLiveActor);
DECLARE_NERVE(TNerveWalkerAttack, TLiveActor);
DECLARE_NERVE(TNerveWalkerPostAttack, TLiveActor);
DECLARE_NERVE(TNerveWalkerEscape, TLiveActor);
DECLARE_NERVE(TNerveWalkerTraceMario, TLiveActor);

#endif
