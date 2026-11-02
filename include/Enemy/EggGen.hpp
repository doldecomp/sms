#ifndef ENEMY_EGGGEN_HPP
#define ENEMY_EGGGEN_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TEggGenerator : public TSpineEnemy {
public:
	TEggGenerator(const char* name = "タマゴジェネレータ");

	virtual void init(TLiveManager* manager);
	virtual void control();
};

class TEggGenManager : public TEnemyManager {
public:
	TEggGenManager(const char* name = "タマゴジェネレータマネージャ");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void createModelData();
};

#endif
