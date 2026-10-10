#ifndef ENEMY_FRUITS_BOAT_HPP
#define ENEMY_FRUITS_BOAT_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TFruitsBoat : public TSpineEnemy {
public:
	TFruitsBoat(const char* name = "フルーツ運搬船");

	/* 0x150 */ char unk150[0x178 - 0x150];
};

class TFruitsBoatManager : public TEnemyManager {
public:
	TFruitsBoatManager(int, const char* name = "フルーツ運搬船マネージャ");

	/* 0x54 */ int unk54;
};

#endif
