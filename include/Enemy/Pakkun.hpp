#ifndef ENEMY_PAKKUN_HPP
#define ENEMY_PAKKUN_HPP

#include <Enemy/SmallEnemy.hpp>

// fabricated: declaration recovered from mario.MAP statics, layout unknown

class TPakkunManager : public TSmallEnemyManager {
public:
	// static members (map: .sdata)
	static f32 mRootExplosionScaleRate;
	static f32 mTestFlyAngX;
	static f32 mIgnoreHitWaterY;
};

#endif
