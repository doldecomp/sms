#ifndef ENEMY_TYPICAL_ENEMY_HPP
#define ENEMY_TYPICAL_ENEMY_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TLiveActor;

class TTypicalEnemy : public TSpineEnemy {
public:
	TTypicalEnemy(const char* name);
	void init(TLiveManager*);
};

class TTypicalParams : public TSpineEnemyParams {
public:
	TTypicalParams(const char* path);

	/* 0xA8 */ TParamRT<f32> mSLMoveSpeed;
};

class TTypicalManager : public TEnemyManager {
public:
	TTypicalManager(const char*);
	void load(JSUMemoryInputStream&);
	TTypicalEnemy* createEnemyInstance();
};

DECLARE_NERVE(TNerveTypicalGraphWander, TLiveActor);

#endif
