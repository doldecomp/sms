class TBossTelesa {
public:
	static float mEnemyGenRate;
	static float mItemGenRate;
	static unsigned char mNormalAlpha;
	static float mBaseHoseiPosY;
	static float mRouletteUpRate;
	static int mTelesaGenerateInterval;
	static float mCameraMoveLimit;
	static float mCameraMoveSp;
};

float TBossTelesa::mEnemyGenRate = 0.5f;
float TBossTelesa::mItemGenRate = 0.1f;
unsigned char TBossTelesa::mNormalAlpha = 150;
float TBossTelesa::mBaseHoseiPosY = -300.0f;
float TBossTelesa::mRouletteUpRate = 0.03f;
int TBossTelesa::mTelesaGenerateInterval = 400;
float TBossTelesa::mCameraMoveLimit = 1000.0f;
float TBossTelesa::mCameraMoveSp = 0.02f;
