#ifndef ENEMY_LIMIT_KOOPA_HPP
#define ENEMY_LIMIT_KOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

// fabricated: declaration only, layout unknown
class TLimitKoopa : public TSpineEnemy {
public:
	TLimitKoopa(const char*);

public:
	// TODO: unknown layout, size from getNameRef_BossEnemy's operator new
	/* 0x150 */ u8 unk150[0x78];
};

// fabricated: declaration only, layout unknown beyond TEnemyManager
class TLimitKoopaManager : public TEnemyManager {
public:
	TLimitKoopaManager(const char*);
};

#endif
