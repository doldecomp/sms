#include <Enemy/SmallEnemy.hpp>

class TPakkunManager : public TSmallEnemyManager {
public:
	static float mRootExplosionScaleRate;
	static float mTestFlyAngX;
	static float mIgnoreHitWaterY;
};

float TPakkunManager::mRootExplosionScaleRate = 2.0f;
float TPakkunManager::mTestFlyAngX             = 30.0f;
float TPakkunManager::mIgnoreHitWaterY         = 50.0f;
