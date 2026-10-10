#ifndef ENEMY_TABE_PUKU_HPP
#define ENEMY_TABE_PUKU_HPP

#include <Enemy/SmallEnemy.hpp>

class TTabePuku : public TSmallEnemy {
public:
	TTabePuku(const char* name = "プクプク(レール巡回)");

	/* 0x194 */ char unk194[0x1F0 - 0x194];
};

class TTabePukuManager : public TSmallEnemyManager {
public:
	TTabePukuManager(const char* name = "プクプク(レール巡回)マネージャー");
};

#endif
