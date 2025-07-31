#ifndef ENEMY_WALKER_ENEMY_HPP
#define ENEMY_WALKER_ENEMY_HPP

#include <Enemy/SmallEnemy.hpp>

class TWalkerEnemyParams {
public:
	TWalkerEnemyParams(const char*);
};

class TWalkerEnemy : public TSmallEnemy {
public:
	TWalkerEnemy(const char*);

	virtual void init(TLiveManager*);
	virtual void moveObject();
	virtual void reset();
	virtual void attackToMario();
	virtual void initAttacker(THitActor*);
	virtual void behaveToFindMario();
	virtual void walkBehavior(int, float);
	virtual void isResignationAttack();
	virtual void initialGraphNode();

	void isReachedToGoalXZ();
};

class TLiveActor;

DECLARE_NERVE(TNerveWalkerGenerate, TLiveActor);
DECLARE_NERVE(TNerveWalkerGraphWander, TLiveActor);
DECLARE_NERVE(TNerveWalkerAttack, TLiveActor);
DECLARE_NERVE(TNerveWalkerPostAttack, TLiveActor);
DECLARE_NERVE(TNerveWalkerEscape, TLiveActor);
DECLARE_NERVE(TNerveWalkerTraceMario, TLiveActor);

#endif
