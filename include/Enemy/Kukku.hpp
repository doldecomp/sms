#ifndef ENEMY_KUKKU_HPP
#define ENEMY_KUKKU_HPP

#include <Enemy/SmallEnemy.hpp>

class TKukku : public TSmallEnemy {
public:
	TKukku(const char* name = "クック");

	/* 0x194 */ char unk194[0x1B4 - 0x194];
};

class TKukkuManager : public TSmallEnemyManager {
public:
	TKukkuManager(const char* name = "クックマネージャー");

	/* 0x60 */ u32 unk60;
};

#endif
