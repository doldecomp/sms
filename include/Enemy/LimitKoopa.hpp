#ifndef ENEMY_LIMIT_KOOPA_HPP
#define ENEMY_LIMIT_KOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TLimitKoopa : public TSpineEnemy {
public:
	TLimitKoopa(const char*);

public:
	/* 0x150 */ u8 unk150[0x78];
};

class TLimitKoopaManager : public TEnemyManager {
public:
	TLimitKoopaManager(const char*);
};

#endif
