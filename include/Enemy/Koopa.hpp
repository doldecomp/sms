#ifndef ENEMY_KOOPA_HPP
#define ENEMY_KOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TKoopa : public TSpineEnemy {
public:
	TKoopa(const char*);

	void fall();
	void stagger(bool);
	void getDown();
	bool allowsLaunch() const;

public:
	// TODO: unknown layout, size from getNameRef_BossEnemy's operator new
	/* 0x150 */ u8 unk150[0x6C];
};

// fabricated: declaration only, layout unknown beyond TEnemyManager
class TKoopaManager : public TEnemyManager {
public:
	TKoopaManager(const char*);
};

#endif
