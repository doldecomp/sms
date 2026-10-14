#ifndef ANIMAL_BIRD_HPP
#define ANIMAL_BIRD_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TAnimalBird : public TSpineEnemy {
public:
	TAnimalBird(const char* name = "(幸せの青い)鳥");

	/* 0x150 */ char unk150[0x184 - 0x150];
};

class TAnimalBirdManager : public TEnemyManager {
public:
	TAnimalBirdManager(const char* name = "(幸せの青い)鳥マネージャー");
};

#endif
