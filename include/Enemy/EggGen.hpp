#ifndef ENEMY_EGGGEN_HPP
#define ENEMY_EGGGEN_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TEggGenerator : public TSpineEnemy {
public:
	TEggGenerator(const char*);

	virtual void init(TLiveManager*);
	virtual void control();
};

class TEggGenManager : public TEnemyManager {
public:
	TEggGenManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

#endif
