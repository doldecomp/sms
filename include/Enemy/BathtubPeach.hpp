#ifndef ENEMY_BATHTUB_PEACH_HPP
#define ENEMY_BATHTUB_PEACH_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

// fabricated: declaration only, layout unknown
class TBathtubPeach : public TSpineEnemy {
public:
	TBathtubPeach(const char*);

public:
	// TODO: unknown layout, size from getNameRef_BossEnemy's operator new
	/* 0x150 */ u8 unk150[0x24];
};

// fabricated: declaration only, layout unknown beyond TEnemyManager
class TBathtubPeachManager : public TEnemyManager {
public:
	TBathtubPeachManager(const char*);
};

#endif
