#ifndef ENEMY_BOSS_TELESA_HPP
#define ENEMY_BOSS_TELESA_HPP

#include <Enemy/Enemy.hpp>

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
};

#endif
