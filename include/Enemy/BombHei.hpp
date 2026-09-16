#ifndef ENEMY_BOMB_HEI_HPP
#define ENEMY_BOMB_HEI_HPP

#include <Enemy/WalkerEnemy.hpp>
#include <Enemy/BombheiNerve.hpp>

// TODO: fabricated stub; only what cannon.cpp touches is declared here.
class TBombHei : public TWalkerEnemy {
public:
	TBombHei(const char*);

	bool isDamageToCannon();
};

#endif
