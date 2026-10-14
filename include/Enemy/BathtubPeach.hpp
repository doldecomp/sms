#ifndef ENEMY_BATHTUB_PEACH_HPP
#define ENEMY_BATHTUB_PEACH_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TBathtubPeach : public TSpineEnemy {
public:
	TBathtubPeach(const char*);

public:
	/* 0x150 */ u8 unk150[0x24];
};

class TBathtubPeachManager : public TEnemyManager {
public:
	TBathtubPeachManager(const char*);
};

#endif
