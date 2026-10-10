#ifndef ENEMY_BOSS_TELESA_HPP
#define ENEMY_BOSS_TELESA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

// fabricated: declaration recovered from mario.MAP statics, layout unknown

class TBossTelesa : public TSpineEnemy {
public:
	TBossTelesa(const char*); // declared only: suppresses the implicit default
	                          // constructor
	// static members (map: .sdata)
	static f32 mEnemyGenRate;
	static f32 mItemGenRate;
	static u8 mNormalAlpha;
	static f32 mBaseHoseiPosY;
	static f32 mRouletteUpRate;
	static s32 mTelesaGenerateInterval;
	static f32 mCameraMoveLimit;
	static f32 mCameraMoveSp;

public:
	// TODO: unknown layout, size from getNameRef_BossEnemy's operator new
	/* 0x150 */ u8 unk150[0x23C];
};

// fabricated: declaration only, layout unknown beyond TEnemyManager
class TBossTelesaManager : public TEnemyManager {
public:
	TBossTelesaManager(const char*);
};

// fabricated: declaration only, layout unknown
class TBubbleManager : public TEnemyManager {
public:
	TBubbleManager(const char*);

public:
	// TODO: unknown layout, size from getNameRef_BossEnemy's operator new
	/* 0x54 */ u8 unk54[0xC];
};

#endif
