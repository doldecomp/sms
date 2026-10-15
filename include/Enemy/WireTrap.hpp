#ifndef ENEMY_WIRE_TRAP_HPP
#define ENEMY_WIRE_TRAP_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TWireTrap : public TSpineEnemy {
public:
	TWireTrap(const char* name = "電線トラップ");

	/* 0x150 */ char unk150[0x184 - 0x150];
};

class TWireTrapManager : public TEnemyManager {
public:
	TWireTrapManager(const char* name = "電線トラップマネージャー");
};

#endif
