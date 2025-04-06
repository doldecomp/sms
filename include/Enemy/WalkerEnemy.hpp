#ifndef ENEMY_WALKER_ENEMY_HPP
#define ENEMY_WALKER_ENEMY_HPP

#include <Enemy/SmallEnemy.hpp>

class TWalkerEnemyParams {
public:
	TWalkerEnemyParams(const char*);
};

class TWalkerEnemy : public TSmallEnemy {
	TWalkerEnemy(const char*);
	void init(TLiveManager*);
	void moveObject();
	void attackToMario();
	void reset();
	void walkBehavior(int, float);
	void behaveToFindMario();
	void initAttacker(THitActor*);
	void isResignationAttack();
	void isReachedToGoalXZ();
	void initialGraphNode();
};

class TLiveActor;

DECLARE_NERVE(TNerveWalkerGenerate, TLiveActor);
DECLARE_NERVE(TNerveWalkerGraphWander, TLiveActor);
DECLARE_NERVE(TNerveWalkerAttack, TLiveActor);
DECLARE_NERVE(TNerveWalkerPostAttack, TLiveActor);
DECLARE_NERVE(TNerveWalkerEscape, TLiveActor);
DECLARE_NERVE(TNerveWalkerTraceMario, TLiveActor);

#endif
