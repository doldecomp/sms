#ifndef ENEMY_TIN_KOOPA_HPP
#define ENEMY_TIN_KOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

// fabricated: declaration only, layout unknown
class TTinKoopa : public TSpineEnemy {
public:
	TTinKoopa(const char*);

public:
	// TODO: unknown layout, size from getNameRef_BossEnemy's operator new
	/* 0x150 */ u8 unk150[0xAC];
};

// fabricated: declaration only, layout unknown beyond TEnemyManager
class TTinKoopaManager : public TEnemyManager {
public:
	TTinKoopaManager(const char*);
};

#endif
